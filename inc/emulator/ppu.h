#ifndef PPU_H
#define PPU_H

#include "common.h"

enum class PpuMode {
  HBlank = 0,
  VBlank = 1,
  OamSearch = 2,
  PixelTransfer = 3
};

enum class DMAState {
  Inactive,
  Request,
  Active
};

class MMU;

class PPU {
public:
  PPU();
  void tick();
  void tick_dma();
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

    uint8 dma_transferes = 0;
    void initialize();
    void OamSearch();
    void PixelTransfer();
    void HBlank();
    void VBlank();
};

#endif // PPU_H