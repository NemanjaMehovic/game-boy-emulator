#ifndef MMU_H
#define MMU_H

#include "common.h"
#include "cpu.h"
#include "cartridge.h"
#include "ppu.h"
#include "timer.h"

class MMU {
public:
    MMU(CPU* cpu, Cartridge* cartridge, PPU* ppu, Timer* timer);
    uint8 read(uint16 addr, Component component);
    void write(uint16 addr, uint8 val, Component component);
    void setDmaActive(bool active) { dma_active = active; }
    void requestInterrupt(Interrupt interrupt);
private:
    uint8 read_rom(uint16 addr, Component component);
    void write_rom(uint16 addr, uint8 val, Component component);
    uint8 read_external_ram(uint16 addr, Component component);
    void write_external_ram(uint16 addr, uint8 val, Component component);
    uint8 read_wram(uint16 addr, Component component);
    void write_wram(uint16 addr, uint8 val, Component component);
    uint8 read_vram(uint16 addr, Component component);
    void write_vram(uint16 addr, uint8 val, Component component);
    uint8 read_oam(uint16 addr, Component component);
    void write_oam(uint16 addr, uint8 val, Component component);
    uint8 read_hram(uint16 addr);
    void write_hram(uint16 addr, uint8 val);
    uint8 read_io(uint16 addr, Component component);
    void write_io(uint16 addr, uint8 val, Component component);

    CPU* cpu;
    Cartridge* cartridge;
    PPU* ppu;
    Timer* timer;
    uint8 wram[WramSize] = {0};
    uint8 vram[VramSize] = {0};
    uint8 oam[OamSize] = {0};
    uint8 hram[HramSize] = {0};

    bool dma_active = false;
};


#endif //MMU_H