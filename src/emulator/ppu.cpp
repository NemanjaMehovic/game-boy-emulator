#include "ppu.h"
#include "mmu.h"

PPU::PPU() {
    initialize();
}

void PPU::initialize() {
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
    mode = PpuMode::VBlank;
    dma_state = DMAState::Inactive;
    dma_transferes = 0;
}

void PPU::tick_dma() {
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

void PPU::tick() {
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
}

void PPU::OamSearch() {
    // OAM Search logic
}

void PPU::PixelTransfer() {
    // Pixel Transfer logic
}

void PPU::HBlank() {
    // HBlank logic
}

void PPU::VBlank() {
    // VBlank logic
}

uint8 PPU::read(uint16 addr) const {
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

void PPU::write(uint16 addr, uint8 val) {
    switch (addr) {
        case 0xFF40:
            LCDC = val;
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