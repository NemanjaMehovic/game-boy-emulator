#ifndef PPU_H
#define PPU_H

#include "common.h"

enum class PpuMode {
  HBlank = 0,
  VBlank = 1,
  OamSearch = 2,
  PixelTransfer = 3
};

class PPU {
public:
  PPU();
  void tick(uint64 Tcycle);
  PpuMode getMode() const { return mode; }

private:
    PpuMode mode = PpuMode::OamSearch;
};

#endif // PPU_H