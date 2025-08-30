#include "mmu.h"
#include "common.h"

MMU::MMU(CPU* cpu, Cartridge* cartridge, PPU* ppu) : cpu(cpu), cartridge(cartridge), ppu(ppu) {

}

uint8 MMU::read_rom(uint16 addr, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to read from ROM during DMA transfer");
            return 0xFF;
        }
    }
    return cartridge->read(addr);
}

void MMU::write_rom(uint16 addr, uint8 val, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to write to ROM during DMA transfer");
            return;
        }
    }
    cartridge->write(addr, val);
}

uint8 MMU::read_external_ram(uint16 addr, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to read from External RAM during DMA transfer");
            return 0xFF;
        }
    }
    return cartridge->read(addr);
}

void MMU::write_external_ram(uint16 addr, uint8 val, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to write to External RAM during DMA transfer");
            return;
        }
    }
    cartridge->write(addr, val);
}

uint8 MMU::read_wram(uint16 addr, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to read from WRAM during DMA transfer");
            return 0xFF;
        }
    }
    return wram[addr - WramStart];
}

void MMU::write_wram(uint16 addr, uint8 val, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to write to WRAM during DMA transfer");
            return;
        }
    }
    wram[addr - WramStart] = val;
}

uint8 MMU::read_vram(uint16 addr, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to read from VRAM during DMA transfer");
            return 0xFF;
        }
        if (ppu->getMode() == PpuMode::PixelTransfer) {
            log_error("Attempted to read from VRAM during PPU Mode 3");
            return 0xFF;
        }
    }
    return vram[addr - VramStart];
}

void MMU::write_vram(uint16 addr, uint8 val, Component component) {
    if (component == Component::CPU) {
        if (dma_active) {
            log_error("Attempted to write to VRAM during DMA transfer");
            return;
        }
        if (ppu->getMode() == PpuMode::PixelTransfer) {
            log_error("Attempted to write to VRAM during PPU Mode 3");
            return;
        }
    }
    vram[addr - VramStart] = val;
}

uint8 MMU::read_oam(uint16 addr, Component component) {
    if (dma_active) {
        if (component == Component::CPU) {
            log_error("Attempted to read from OAM during DMA transfer");
            return 0xFF;
        }
    }
    return oam[addr - OamStart];
}

void MMU::write_oam(uint16 addr, uint8 val, Component component) {
    if (dma_active) {
        if (component == Component::CPU) {
            log_error("Attempted to write to OAM during DMA transfer");
            return;
        }
    }
    oam[addr - OamStart] = val;
}

uint8 MMU::read_hram(uint16 addr) {
    return hram[addr - HramStart];
}

void MMU::write_hram(uint16 addr, uint8 val) {
    hram[addr - HramStart] = val;
}

uint8 MMU::read(uint16 addr, Component component) {
    if (addr <= RomEnd) {
        return read_rom(addr, component);
    } else if (addr >= VramStart && addr <= VramEnd) {
        return read_vram(addr, component);
    } else if (addr >= ExternalRamStart && addr <= ExternalRamEnd) {
        return read_external_ram(addr, component);
    } else if (addr >= WramStart && addr <= WramEnd) {
        return read_wram(addr, component);
    } else if (addr >= EchoRamStart && addr <= EchoRamEnd) {
        return read_wram(mask_n_bits(13, addr), component);
    } else if (addr >= OamStart && addr <= OamEnd) {
        return read_oam(addr, component);
    } else if (addr >= UnusableStart && addr <= UnusableEnd) {
        log_error("Attempted to read from unusable memory area at address 0x%X", addr);
        return 0xFF;
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        // TODO
        return 0; 
    } else if (addr >= HramStart && addr <= HramEnd) {
        return read_hram(addr);
    } else if (addr == 0xFFFF) {
        return cpu->IER;
    } 
    log_error("Attempted to read from invalid memory address 0x%X", addr);
    return 0xFF;
}

void MMU::write(uint16 addr, uint8 val, Component component) {
    if (addr <= RomEnd) {
        write_rom(addr, val, component);
    } else if (addr >= VramStart && addr <= VramEnd) {
        write_vram(addr, val, component);
    } else if (addr >= ExternalRamStart && addr <= ExternalRamEnd) {
        write_external_ram(addr, val, component);
    } else if (addr >= WramStart && addr <= WramEnd) {
        write_wram(addr, val, component);
    } else if (addr >= EchoRamStart && addr <= EchoRamEnd) {
        write_wram(mask_n_bits(13, addr), val, component);
    } else if (addr >= OamStart && addr <= OamEnd) {
        write_oam(addr, val, component);
    } else if (addr >= UnusableStart && addr <= UnusableEnd) {
        log_error("Attempted to write to unusable memory area at address 0x%X", addr);
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        // TODO
    } else if (addr >= HramStart && addr <= HramEnd) {
        write_hram(addr, val);
    } else if (addr == 0xFFFF) {
        cpu->IER = val;
    } else {
        log_error("Attempted to write to invalid memory address 0x%X", addr);
    }
}