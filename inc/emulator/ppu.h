#ifndef PPU_H
#define PPU_H

#include "common.h"
#include <deque>
#include <vector>

enum class PpuMode
{
  HBlank = 0,
  VBlank = 1,
  OamSearch = 2,
  PixelTransfer = 3
};

enum class DMAState
{
  Inactive,
  Request,
  Active
};

class MMU;

struct oam_entry
{
  uint8 oam_number;
  uint8 y;
  uint8 x;
};

struct bg_win_pixel_data
{
  uint8 palette_id;
};

struct oam_pixel_data
{
  uint8 palette_id;
  uint8 x;
  uint8 entry_number;
  bool bg_has_priority;
  bool palette_2;
};

class PPU
{
  friend class MMU;

public:
  PPU();
  void tick(uint64 Tcycle);
  void tick_dma(uint64 Tcycle);
  PpuMode getMode() const { return mode; }
  uint8 read(uint16 addr) const;
  void write(uint16 addr, uint8 val);
  void setMMU(MMU* mmu) { this->mmu = mmu; }
  uint32 LCD_PIXELS[GB_HEIGHT * GB_WIDTH];

private:
  enum class FetcherState
  {
    Delay,
    GetTile,
    GetData0,
    GetData1,
    Push
  };

  MMU* mmu = nullptr;

  PpuMode mode = PpuMode::VBlank;
  DMAState dma_state = DMAState::Inactive;

  uint8 LCDC = 0x91;
  uint8 STAT = 0x85;
  uint8 SCY = 0;
  uint8 SCX = 0;
  uint8 LY = 0;
  uint8 LYC = 0;
  uint8 DMA = 0xFF;
  uint8 BGP = 0xFC;
  uint8 OBP0 = 0xFF;
  uint8 OBP1 = 0xFF;
  uint8 WY = 0;
  uint8 WX = 0;
  uint16 scanline_ticks = 0;

  bool turned_on_again = false;

  bool use_turn_on_oam_scan = false;
  bool internal_enable_lyc_eq_ly_irq = true;
  bool last_stat_irq = false;
  bool last_vblank_line = true;
  bool fetching_sprite = false;
  bool wx_active = false;
  bool wy_active = false;
  bool window_initial_activation = false;
  bool skip_initial_delay = false;

  std::deque<oam_entry> oam_buffer[256];
  uint8 num_of_oam_entries = 0;
  uint8 lx = 0;
  uint8 bgx = 0;
  uint8 window_tile = 0;
  uint8 wy_internal = 0xFF;

  std::deque<bg_win_pixel_data> bg_win_fifo;
  std::deque<oam_pixel_data> object_fifo;

  FetcherState fstate = FetcherState::Delay;
  FetcherState object_fstate = FetcherState::Delay;
  FetcherState next_fstate = FetcherState::Delay;
  FetcherState next_object_fstate = FetcherState::Delay;

  uint8 tile_number;
  uint8 tile_data0;
  uint8 tile_data1;

  uint8 oam_tile_number;
  uint8 oam_attributes;
  uint8 oam_tile_data0;
  uint8 oam_tile_data1;

  uint8 dma_transferes = 0;
  void PixelFetcher();
  void SpriteFetcher();
  bool CanPushPixelsToScreen();
  bool CanFetchObject();
  void PixelTransferReset();
  void CheckWindow();
  void initialize();

  void OamSearch();
  void PixelTransfer();
  void HBlank();
  void VBlank();
  void handleLCDCChanges(uint8 old_val);
  void setMode(PpuMode new_mode, bool update_only_stat = false);
};

#endif // PPU_H
