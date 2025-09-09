#ifndef PPU_H
#define PPU_H

#include "common.h"
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

class PPU
{
public:
  PPU();
  void tick();
  void tick_dma(uint64 Tcycle);
  PpuMode getMode() const { return mode; }
  uint8 read(uint16 addr) const;
  void write(uint16 addr, uint8 val);
  void setMMU(MMU* mmu) { this->mmu = mmu; }

private:
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

  bool use_turn_on_oam_scan = false;
  bool internal_enable_lyc_eq_ly_irq = true;
  bool last_stat_irq = false;

  std::vector<oam_entry> oam_buffer[256];
  uint8 num_of_oam_entries = 0;

  uint8 dma_transferes = 0;
  void initialize();
  void OamSearch();
  void PixelTransfer();
  void HBlank();
  void VBlank();
  void handleLCDCChanges(uint8 old_val);
  void setMode(PpuMode new_mode, bool update_only_stat = false);
};

#endif // PPU_H
