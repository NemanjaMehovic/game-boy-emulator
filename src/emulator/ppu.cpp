#include "ppu.h"
#include "mmu.h"

PPU::PPU()
{
  initialize();
}

void
PPU::initialize()
{
  LCDC = 0x91;
  STAT = 0x85;
  SCY = 0;
  SCX = 0;
  LY = 0;
  LYC = 0;
  DMA = 0xFF;
  BGP = 0xFC;
  OBP0 = 0xFF;
  OBP1 = 0xFF;
  WY = 0;
  WX = 0;
  scanline_ticks = 395;
  internal_enable_lyc_eq_ly_irq = true;
  use_turn_on_oam_scan = false;
  last_stat_irq = false;
  last_vblank_line = true;
  mode = PpuMode::VBlank;
  dma_state = DMAState::Inactive;
  dma_transferes = 0;
  num_of_oam_entries = 0;
  PixelTransferReset();
  wy_internal = 0xFF;
  wy_active = false;
  for (int i = 0; i < 256; i++) {
    oam_buffer[i].clear();
  }
}

void
PPU::tick_dma(uint64 Tcycle)
{
  // DMA transfer happens 1 every M-cycle
  if (Tcycle % 4 != 0) {
    return;
  }
  if (dma_state == DMAState::Request) {
    dma_state = DMAState::Active;
    dma_transferes = 0;
    // Not really active yet, will be active next tick
    return;
  }
  if (dma_state == DMAState::Active) {
    mmu->setDmaActive(true);
    uint16 source_addr = (DMA << 8) + dma_transferes;
    uint16 dest_addr = OamStart + dma_transferes;
    // read and write should technically be done on separate ticks
    // but it doesn't really matter for our purposes since nothing else
    // should access memory during a DMA transfer
    uint8 data = mmu->read(source_addr, Component::DMA);
    mmu->write(dest_addr, data, Component::DMA);
    dma_transferes++;
    if (dma_transferes >= 0xA0) {
      dma_state = DMAState::Inactive;
      dma_transferes = 0;
      // We're technically still active for this tick
      // but it doesn't really matter
      mmu->setDmaActive(false);
    }
  }
}

void
PPU::tick(uint64 Tcycle)
{
  if ((LCDC & 0x80) == 0) {
    // LCD is off
    return;
  }

  switch (mode) {
    case PpuMode::OamSearch:
      OamSearch();
      break;
    case PpuMode::PixelTransfer:
      PixelTransfer();
      break;
    case PpuMode::HBlank:
      HBlank();
      break;
    case PpuMode::VBlank:
      VBlank();
      break;
  }

  bool lyc_eq_ly = (LYC == LY) && internal_enable_lyc_eq_ly_irq;
  bool lyc_eq_ly_irq = (STAT & 0x40) && lyc_eq_ly;

  bool hblank_irq =
    (STAT & 0x08) && ((STAT & 0x03) == static_cast<int>(PpuMode::HBlank));
  bool vblank_irq = ((STAT & 0x10) || (STAT & 0x20)) &&
                    ((STAT & 0x03) == static_cast<int>(PpuMode::VBlank));

  bool oam_irq =
    (STAT & 0x20) && ((STAT & 0x03) == static_cast<int>(PpuMode::OamSearch));

  if (lyc_eq_ly) {
    STAT |= 0x04;
  } else {
    STAT &= ~0x04;
  }

  bool request_irq = lyc_eq_ly_irq || hblank_irq || vblank_irq || oam_irq;
  if (request_irq && !last_stat_irq) {
    mmu->requestInterrupt(Interrupt::LCDStat);
  }
  last_stat_irq = request_irq;

  if (LY == WY && (LCDC & 0x20) != 0) {
    // wy is checked every tick not just mode 3
    // and once on it stays on for the rest of the frame
    wy_active = true;
  }
}

void
PPU::OamSearch()
{
  // OAM search gets one oam entry per 2 ticks
  if (num_of_oam_entries < 10 && (scanline_ticks % 2) == 1) {
    // TODO handle oam bug
    uint8 y_byte =
      mmu->read(OamStart + 4 * (int)(scanline_ticks / 2), Component::PPU);
    uint8 x_byte =
      mmu->read(OamStart + 4 * (int)(scanline_ticks / 2) + 1, Component::PPU);

    uint8 obj_size = 8 << ((LCDC >> 2) & 1);
    const int32 obj_y = y_byte - 16;
    if (obj_y <= LY && LY < (obj_y + obj_size)) {
      oam_entry entry;
      entry.oam_number = scanline_ticks / 2;
      entry.y = y_byte;
      entry.x = x_byte;
      oam_buffer[x_byte].push_back(entry);
      num_of_oam_entries++;
    }
  }
  scanline_ticks++;
  if (scanline_ticks >= 80) {
    num_of_oam_entries = 0;
    bg_win_fifo.clear();
    object_fifo.clear();
    setMode(PpuMode::PixelTransfer);
  }
}

void
PPU::PixelTransferReset()
{
  bg_win_fifo.clear();
  object_fifo.clear();
  lx = 0;
  bgx = 0;
  window_tile = 0;
  fetching_sprite = false;
  wx_active = false;
  window_initial_activation = false;
  fstate = FetcherState::GetTile;
  object_fstate = FetcherState::Delay;
  next_fstate = FetcherState::GetTile;
  next_object_fstate = FetcherState::Delay;
}

bool
PPU::CanFetchObject()
{
  return (oam_buffer[lx].size() != 0) && ((LCDC & 0x02) != 0);
}

void
PPU::SpriteFetcher()
{
  switch (object_fstate) {
    case FetcherState::Delay:
      object_fstate = next_object_fstate;
      break;
    case FetcherState::GetTile: {
      oam_entry entry = oam_buffer[lx].front();
      oam_tile_number =
        mmu->read(OamStart + 4 * entry.oam_number + 2, Component::PPU);
      oam_attributes =
        mmu->read(OamStart + 4 * entry.oam_number + 3, Component::PPU);
      if (skip_initial_delay) {
        skip_initial_delay = false;
        object_fstate = FetcherState::GetData0;
      } else {
        object_fstate = FetcherState::Delay;
        next_object_fstate = FetcherState::GetData0;
      }
    } break;
    case FetcherState::GetData0:
    case FetcherState::GetData1: {
      oam_entry entry = oam_buffer[lx].front();
      bool hight16 = (LCDC & 0x04) != 0;
      uint8 height_mask = hight16 ? 0xF : 0x7;
      uint8 tile_y = (LY - entry.y - 16) & height_mask;
      if ((oam_attributes & 0x40) != 0) {
        tile_y ^= height_mask;
      }
      uint8 tmp_tile_number =
        hight16 ? (oam_tile_number & 0xFE) : oam_tile_number;
      uint16 tile_data_addr = 0x8000 + 16 * tmp_tile_number + 2 * tile_y;
      if (object_fstate == FetcherState::GetData0) {
        oam_tile_data0 = mmu->read(tile_data_addr, Component::PPU);
        object_fstate = FetcherState::Delay;
        next_object_fstate = FetcherState::GetData1;
      } else {
        oam_tile_data1 = mmu->read(tile_data_addr + 1, Component::PPU);
        object_fstate = FetcherState::Push;
      }
    } break;
    case FetcherState::Push: {
      oam_pixel_data object_pixels[8];
      for (int i = 0; i < 8; i++) {
        object_pixels[i].bg_has_priority = (oam_attributes & 0x80) != 0;
        object_pixels[i].palette_2 = (oam_attributes & 0x10) != 0;
        object_pixels[i].x = lx;
        object_pixels[i].entry_number = oam_buffer[lx].front().oam_number;
        uint8 high_bit;
        uint8 low_bit;
        if ((oam_attributes & 0x20) != 0) {
          high_bit = ((oam_tile_data1 >> i) & 1) << 1;
          low_bit = (oam_tile_data0 >> i) & 1;
        } else {
          high_bit = ((oam_tile_data1 >> (7 - i)) & 1) << 1;
          low_bit = (oam_tile_data0 >> (7 - i)) & 1;
        }
        object_pixels[i].palette_id = high_bit | low_bit;
      }

      int fifo_index = 0;
      while (fifo_index < object_fifo.size()) {
        oam_pixel_data fifo_pixel_data = object_fifo[fifo_index];
        bool transparency = fifo_pixel_data.palette_id == 0 &&
                            object_pixels[fifo_index].palette_id != 0;
        bool entry_number = (fifo_pixel_data.palette_id != 0 &&
                             object_pixels[fifo_index].palette_id != 0) &&
                            (fifo_pixel_data.x == object_pixels[fifo_index].x &&
                             object_pixels[fifo_index].entry_number <
                               fifo_pixel_data.entry_number);

        if (transparency || entry_number) {
          object_fifo[fifo_index] = object_pixels[fifo_index];
        }
        fifo_index++;
      }

      while (fifo_index < 8) {
        object_fifo.push_back(object_pixels[fifo_index]);
        fifo_index++;
      }

      oam_buffer[lx].pop_front();
      if (oam_buffer[lx].size() == 0) {
        fetching_sprite = false;
      } else {
        object_fstate = FetcherState::GetTile;
      }
    } break;
  }
}

void
PPU::PixelFetcher()
{
  if (fetching_sprite) {
    SpriteFetcher();
    return;
  }
  switch (fstate) {
    case FetcherState::Delay:
      if (next_fstate == FetcherState::Push) {
        bgx += 8;
        if (window_initial_activation) {
          window_initial_activation = false;
          for (int i = 0; i < 8; i++) {
            bg_win_pixel_data tmp;
            uint8 high_bit = ((tile_data1 >> (7 - i)) & 1) << 1;
            uint8 low_bit = (tile_data0 >> (7 - i)) & 1;
            tmp.palette_id = high_bit | low_bit;
            bg_win_fifo.push_back(tmp);
          }
          fstate = FetcherState::GetTile;
          return;
        }
        if (CanFetchObject() && bg_win_fifo.size() > 0) {
          fetching_sprite = true;
          skip_initial_delay = true;
          object_fstate = FetcherState::GetTile;
        }
      }
      fstate = next_fstate;
      break;
    case FetcherState::GetTile: {
      if (wx_active && (LCDC & 0x20) == 0) {
        wx_active = false;
      }
      uint8 tilemap_x;
      uint8 tilemap_y;
      uint16 tilemap_vram_addr;
      uint16 tile_vram_addr;
      if (wx_active) {
        tilemap_x = window_tile;
        window_tile++;
        tilemap_y = wy_internal / 8;
        tilemap_vram_addr = (LCDC & 0x40) != 0 ? 0x9C00 : 0x9800;
        if (window_initial_activation) {
          bgx -= 8;
        }
      } else {
        tilemap_x = ((bgx + SCX) / 8) % 32;
        tilemap_y = ((LY + SCY) / 8) % 32;
        tilemap_vram_addr = (LCDC & 0x08) != 0 ? 0x9C00 : 0x9800;
      }
      tile_vram_addr = tilemap_vram_addr + (32 * tilemap_y) + tilemap_x;
      tile_number = mmu->read(tile_vram_addr, Component::PPU);
      fstate = FetcherState::Delay;
      next_fstate = FetcherState::GetData0;
    } break;
    case FetcherState::GetData0:
    case FetcherState::GetData1: {
      uint16 tile_data_addr = BootRomAddr; // default value for error finding
      uint8_t tile_y;
      if ((LCDC & 0x10) != 0) {
        tile_data_addr = 0x8000 + 16 * tile_number;
      } else {
        int tmp = 16 * (int8)(tile_number);
        tile_data_addr = 0x9000 + tmp;
      }
      if (wx_active) {
        tile_y = wy_internal % 8;
      } else {
        tile_y = (LY + SCY) % 8;
      }
      tile_data_addr = tile_data_addr + 2 * tile_y;
      if (fstate == FetcherState::GetData0) {
        tile_data0 = mmu->read(tile_data_addr, Component::PPU);
        next_fstate = FetcherState::GetData1;
      } else {
        tile_data1 = mmu->read(tile_data_addr + 1, Component::PPU);
        next_fstate = FetcherState::Push;
      }
      fstate = FetcherState::Delay;
    } break;
    case FetcherState::Push: {
      if (bg_win_fifo.size() == 0) {
        for (int i = 0; i < 8; i++) {
          bg_win_pixel_data tmp;
          uint8 high_bit = ((tile_data1 >> (7 - i)) & 1) << 1;
          uint8 low_bit = (tile_data0 >> (7 - i)) & 1;
          tmp.palette_id = high_bit | low_bit;
          bg_win_fifo.push_back(tmp);
        }
        fstate = FetcherState::GetTile;
        if (wx_active) {
          return;
        }
      }
      if (CanFetchObject()) {
        fetching_sprite = true;
        skip_initial_delay = true;
        object_fstate = FetcherState::GetTile;
      }
    } break;
  }
}

bool
PPU::CanPushPixelsToScreen()
{
  return (bg_win_fifo.size()) > 0 && !fetching_sprite &&
         ((oam_buffer[lx].size() == 0) || ((LCDC & 0x02) == 0));
}

void
PPU::CheckWindow()
{
  if (wy_active && !wx_active && lx == WX && (LCDC & 0x20) != 0) {
    wx_active = true;
    window_initial_activation = true;
    wy_internal++;
    window_tile = 0;
    bg_win_fifo.clear();
    fstate = FetcherState::GetTile;
    next_fstate = FetcherState::Delay;
  }
}

void
PPU::PixelTransfer()
{
  bool pushed_pixel = false;
  // 3 ticks are used to align fifo with scx
  if (scanline_ticks == 80) {
    PixelTransferReset();
    // garbage that is used for aligning with scx will be popped out
    // anyways since pixels from 0-8 are discarded
    for (int i = 0; i < 8; i++) {
      bg_win_pixel_data tmp;
      bg_win_fifo.push_back(tmp);
    }
    // push out scx % 8 pixels to align fifo with scx
    for (int i = 0; i < (SCX % 8); i++) {
      bg_win_fifo.pop_front();
    }
    scanline_ticks++;
    return;
  } else if (scanline_ticks < 83) {
    scanline_ticks++;
    return;
  }
  // Pixel Transfer logic
  if (CanPushPixelsToScreen()) {
    bg_win_pixel_data bg_pixel = bg_win_fifo.front();
    bg_win_fifo.pop_front();

    uint8* palette = &BGP;
    uint8 palette_id = bg_pixel.palette_id;

    if (object_fifo.size() > 0) {
      oam_pixel_data sprite_pixel = object_fifo.front();
      object_fifo.pop_front();

      bool objects_still_active = (LCDC & 0x02) != 0;
      bool isnt_transparent = sprite_pixel.palette_id != 0;
      bool sprite_has_priority =
        !sprite_pixel.bg_has_priority ||
        (sprite_pixel.bg_has_priority && bg_pixel.palette_id == 0);
      if (objects_still_active && isnt_transparent && sprite_has_priority) {
        palette = sprite_pixel.palette_2 ? &OBP1 : &OBP0;
        palette_id = sprite_pixel.palette_id;
      }
    }
    uint8 color_id = ((*palette) >> (palette_id * 2)) & 0x03;
    if (lx >= 8) {
      if (palette == &BGP && (LCDC & 0x01) == 0) {
        color_id = 0;
      }
      LCD_PIXELS[LY * GB_WIDTH + (lx - 8)] = GB_COLORS[color_id];
    }

    pushed_pixel = true;

    CheckWindow();
  }

  PixelFetcher();

  scanline_ticks++;
  if (pushed_pixel) {
    lx++;
    if (lx >= 168) {
      setMode(PpuMode::HBlank);
    }
  }
}

void
PPU::HBlank()
{
  if (use_turn_on_oam_scan) {
    // really strange situation that happens when turning on the LCD
    // we're in hbalnk mode and doing oam scan without actually scanning
    // anything after 80 ticks we go to pixel transfer mode as normal
    scanline_ticks++;
    if (scanline_ticks >= 80) {
      use_turn_on_oam_scan = false;
      setMode(PpuMode::PixelTransfer);
    }
    return;
  }
  // HBlank logic
  scanline_ticks++;
  internal_enable_lyc_eq_ly_irq = !(scanline_ticks == 454);
  if (scanline_ticks == 454) {
    LY++;
  }
  if (scanline_ticks >= 456) {
    if (LY >= 144) {
      setMode(PpuMode::VBlank);
      mmu->requestInterrupt(Interrupt::VBlank);
    } else {
      setMode(PpuMode::OamSearch);
    }
    scanline_ticks = 0;
    for (int i = 0; i < 256; i++) {
      oam_buffer[i].clear();
    }
  }
}

void
PPU::VBlank()
{
  if (last_vblank_line) {
    scanline_ticks++;
    internal_enable_lyc_eq_ly_irq = !(scanline_ticks == 2);
    if (scanline_ticks == 2) {
      LY = 0;
    }
    if (scanline_ticks >= 456) {
      if (LY == 0) {
        wy_internal = 0xFF;
        wy_active = false;
        setMode(PpuMode::OamSearch);
        for (int i = 0; i < 256; i++) {
          oam_buffer[i].clear();
        }
        last_vblank_line = false;
      }
      scanline_ticks = 0;
    }
  } else {
    scanline_ticks++;
    internal_enable_lyc_eq_ly_irq = !(scanline_ticks == 454);
    if (scanline_ticks == 454) {
      LY++;
    }
    if (scanline_ticks >= 456) {
      if (LY == 153) {
        last_vblank_line = true;
      }
      scanline_ticks = 0;
    }
  }
}

uint8
PPU::read(uint16 addr) const
{
  switch (addr) {
    case 0xFF40:
      return LCDC;
    case 0xFF41:
      return STAT;
    case 0xFF42:
      return SCY;
    case 0xFF43:
      return SCX;
    case 0xFF44:
      return LY;
    case 0xFF45:
      return LYC;
    case 0xFF46:
      return DMA;
    case 0xFF47:
      return BGP;
    case 0xFF48:
      return OBP0;
    case 0xFF49:
      return OBP1;
    case 0xFF4A:
      return WY;
    case 0xFF4B:
      return WX;
    default:
      log_error("Attempted to read from invalid ppu address: 0x%04x", addr);
      return 0xFF;
  }
}

void
PPU::write(uint16 addr, uint8 val)
{
  uint8 tmp = 0;
  switch (addr) {
    case 0xFF40:
      tmp = LCDC;
      LCDC = val;
      handleLCDCChanges(tmp);
      break;
    case 0xFF41:
      // Only bits [3-6] are writable
      STAT = (STAT & 0x87) | (val & 0x78);
      break;
    case 0xFF42:
      SCY = val;
      break;
    case 0xFF43:
      SCX = val;
      break;
    case 0xFF44:
      // LY is read-only
      log_error("Attempted to write to read-only register LY");
      break;
    case 0xFF45:
      LYC = val;
      break;
    case 0xFF46:
      DMA = val;
      dma_state = DMAState::Request;
      break;
    case 0xFF47:
      BGP = val;
      break;
    case 0xFF48:
      OBP0 = val;
      break;
    case 0xFF49:
      OBP1 = val;
      break;
    case 0xFF4A:
      WY = val;
      break;
    case 0xFF4B:
      WX = val;
      break;
    default:
      log_error("Attempted to write to invalid ppu address: 0x%04x", addr);
      break;
  }
}

void
PPU::setMode(PpuMode new_mode, bool update_only_stat)
{
  if (!update_only_stat) {
    mode = new_mode;
  }
  // Update STAT register mode bits
  STAT = (STAT & 0xFC) | static_cast<uint8>(mode);
}

void
PPU::handleLCDCChanges(uint8 old_val)
{
  bool old_lcd_enabled = old_val & 0x80;
  bool new_lcd_enabled = LCDC & 0x80;
  if (old_lcd_enabled == new_lcd_enabled) {
    // No change in LCD enabled state
    return;
  }
  if (!new_lcd_enabled) {
    log_info("LCDC turned off");
    scanline_ticks = 0;
    LY = 0;
    PixelTransferReset();
    wy_internal = 0xFF;
    wy_active = false;
    last_stat_irq = false;
    last_vblank_line = false;
    num_of_oam_entries = 0;
    setMode(PpuMode::HBlank);
    for (int i = 0; i < 256; i++) {
      oam_buffer[i].clear();
    }
    for (int i = 0; i < (GB_HEIGHT * GB_WIDTH); i++) {
      LCD_PIXELS[i] = GB_COLORS[0];
    }
  } else {
    log_info("LCDC turned on");
    use_turn_on_oam_scan = true;
    bool lyc_eq_ly = (LYC == LY) && internal_enable_lyc_eq_ly_irq;
    turned_on_again = true;
    if (lyc_eq_ly) {
      STAT |= 0x04;
    } else {
      STAT &= ~0x04;
    }
  }
}
