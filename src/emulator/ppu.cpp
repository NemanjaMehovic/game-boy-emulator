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
  mode = PpuMode::VBlank;
  dma_state = DMAState::Inactive;
  dma_transferes = 0;
  num_of_oam_entries = 0;
  for (int i = 0; i < 256; i++) {
    oam_buffer[i].clear();
  }
}

void
PPU::tick_dma(uint64 Tcycle)
{
  // DMA transfer happens 1 every M-cycle
  if (Tcycle % 4 != 3) {
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
PPU::tick()
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
  bool vblank_irq =
    (STAT & 0x10) && ((STAT & 0x03) == static_cast<int>(PpuMode::VBlank));
  bool oam_irq =
    (STAT & 0x20) && ((STAT & 0x03) == static_cast<int>(PpuMode::OamSearch));

  STAT = (STAT & 0xFE) | (lyc_eq_ly ? 1 : 0);

  bool request_irq = lyc_eq_ly_irq || hblank_irq || vblank_irq || oam_irq;
  if (request_irq && !last_stat_irq) {
    mmu->requestInterrupt(Interrupt::LCDStat);
  }
  last_stat_irq = request_irq;
}

void
PPU::OamSearch()
{
  // OAM search gets one oam entry per 2 ticks
  if (num_of_oam_entries < 10 && (scanline_ticks % 2) == 1) {
    // TODO handle oam bug
    uint8 y_byte = mmu->read(OamStart + 4 * scanline_ticks / 2, Component::PPU);
    uint8 x_byte =
      mmu->read(OamStart + 4 * scanline_ticks / 2 + 1, Component::PPU);

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
    setMode(PpuMode::PixelTransfer);
  }
}

void
PPU::PixelTransfer()
{
  // Pixel Transfer logic
  scanline_ticks++;
  if (scanline_ticks == (80 + 200)) {
    setMode(PpuMode::HBlank);
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
  scanline_ticks++;
  internal_enable_lyc_eq_ly_irq = !(scanline_ticks == 454);
  if (scanline_ticks == 454) {
    LY++;
    if (LY >= 154) {
      LY = 0;
    }
  }
  if (scanline_ticks >= 456) {
    if (LY == 0) {
      setMode(PpuMode::OamSearch);
      for (int i = 0; i < 256; i++) {
        oam_buffer[i].clear();
      }
    }
    scanline_ticks = 0;
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
    scanline_ticks = 0;
    LY = 0;
    setMode(PpuMode::HBlank);
    for (int i = 0; i < 256; i++) {
      oam_buffer[i].clear();
    }
    // TODO clear oam buffer and fifo if needed
  } else {
    use_turn_on_oam_scan = true;
    bool lyc_eq_ly = (LYC == LY) && internal_enable_lyc_eq_ly_irq;
    STAT = (STAT & 0xFE) | (lyc_eq_ly ? 1 : 0);
  }
}
