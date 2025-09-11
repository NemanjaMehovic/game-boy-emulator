#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

// Wram
constexpr uint16 WramStart = 0xC000;
constexpr uint16 WramEnd = 0xDFFF;
constexpr uint16 WramSize = WramEnd - WramStart + 1;
// Vram
constexpr uint16 VramStart = 0x8000;
constexpr uint16 VramEnd = 0x9FFF;
constexpr uint16 VramSize = VramEnd - VramStart + 1;
// Hram
constexpr uint16 HramStart = 0xFF80;
constexpr uint16 HramEnd = 0xFFFE;
constexpr uint16 HramSize = HramEnd - HramStart + 1;
// Oam
constexpr uint16 OamStart = 0xFE00;
constexpr uint16 OamEnd = 0xFE9F;
constexpr uint16 OamSize = OamEnd - OamStart + 1;
// Echo Ram
constexpr uint16 EchoRamStart = 0xE000;
constexpr uint16 EchoRamEnd = 0xFDFF;
// Unusable Memory Area
constexpr uint16 UnusableStart = 0xFEA0;
constexpr uint16 UnusableEnd = 0xFEFF;
// Rom and External Ram
constexpr uint16 RomStart = 0x0000;
constexpr uint16 RomEnd = 0x7FFF;
constexpr uint16 ExternalRamStart = 0xA000;
constexpr uint16 ExternalRamEnd = 0xBFFF;
// IO Registers
constexpr uint16 IoRegistersStart = 0xFF00;
constexpr uint16 IoRegistersEnd = 0xFF7F;
// Timer Registers
constexpr uint16 TimerStart = 0xFF04;
constexpr uint16 TimerEnd = 0xFF07;
// PPU Registers
constexpr uint16 PpuStart = 0xFF40;
constexpr uint16 PpuEnd = 0xFF4B;
// APU Registers
constexpr uint16 ApuStart = 0xFF10;
constexpr uint16 ApuEnd = 0xFF3F;
// Serial Registers
constexpr uint16 SerialStart = 0xFF01;
constexpr uint16 SerialEnd = 0xFF02;
// Interrupt Registers
constexpr uint16 IFRAddr = 0xFF0F;
constexpr uint16 IEAddr = 0xFFFF;
// BootRom Register
constexpr uint16 BootRomAddr = 0xFF50;

// Gameboy screen resolution
constexpr int GB_WIDTH = 160;
constexpr int GB_HEIGHT = 144;
const uint32 GB_COLORS[4] = { 0xFFFFFFFF, 0xFF3b3b3b, 0xFF222222, 0xFF000000 };

enum class Interrupt
{
  VBlank = 0x01,
  LCDStat = 0x02,
  Timer = 0x04,
  Serial = 0x08,
  Joypad = 0x10
};

constexpr uint8 NoMBC = 0x00;

constexpr uint8 MBC1 = 0x01;
constexpr uint8 MBC1Ram = 0x02;
constexpr uint8 MBC1RamB = 0x03;

constexpr uint8 MBC2 = 0x05;
constexpr uint8 MBC2B = 0x06;

constexpr uint8 MBC3BT = 0x0F;
constexpr uint8 MBC3RamBT = 0x10;
constexpr uint8 MBC3 = 0x11;
constexpr uint8 MBC3Ram = 0x12;
constexpr uint8 MBC3RamB = 0x13;

constexpr uint8 MBC5 = 0x19;
constexpr uint8 MBC5Ram = 0x1A;
constexpr uint8 MBC5RamB = 0x1B;
constexpr uint8 MBC5R = 0x1C;
constexpr uint8 MBC5RamR = 0x1D;
constexpr uint8 MBC5RamBR = 0x1E;

const std::unordered_map<uint8, std::string> ROM_TYPES = {
  { NoMBC, "ROM ONLY" },
  { MBC1, "MBC1" },
  { MBC1Ram, "MBC1+RAM" },
  { MBC1RamB, "MBC1+RAM+BATTERY" },
  { MBC2, "MBC2" },
  { MBC2B, "MBC2+BATTERY" },
  { 0x08, "ROM+RAM" },           // never used
  { 0x09, "ROM+RAM+BATTERY" },   // never used
  { 0x0B, "MMM01" },             // not supported
  { 0x0C, "MMM01+RAM" },         // not supported
  { 0x0D, "MMM01+RAM+BATTERY" }, // not supported
  { MBC3BT, "MBC3+TIMER+BATTERY" },
  { MBC3RamBT, "MBC3+TIMER+RAM+BATTERY" },
  { MBC3, "MBC3" },
  { MBC3Ram, "MBC3+RAM" },
  { MBC3RamB, "MBC3+RAM+BATTERY" },
  { MBC5, "MBC5" },
  { MBC5Ram, "MBC5+RAM" },
  { MBC5RamB, "MBC5+RAM+BATTERY" },
  { MBC5R, "MBC5+RUMBLE" },
  { MBC5RamR, "MBC5+RUMBLE+RAM" },
  { MBC5RamBR, "MBC5+RUMBLE+RAM+BATTERY" },
  { 0x20, "MBC6" },                           // not supported
  { 0x22, "MBC7+SENSOR+RUMBLE+RAM+BATTERY" }, // not supported
  { 0xFC, "POCKET CAMERA" },                  // not supported
  { 0xFD, "BANDAI TAMA5" },                   // not supported
  { 0xFE, "HuC3" },                           // not supported
  { 0xFF, "HuC1+RAM+BATTERY" }                // not supported
};

const std::unordered_map<uint8, uint32> RAM_SIZES = {
  { 0x02, 0x2000 },
  { 0x03, 0x8000 },
  { 0x04, 0x20000 },
  { 0x05, 0x10000 },
};

const std::unordered_set<uint8> HAS_BATTERY = { MBC1RamB, MBC2B,    0x09,
                                                0x0D,     MBC3BT,   MBC3RamBT,
                                                MBC3RamB, MBC5RamB, MBC5RamBR,
                                                0x22 };

const std::unordered_map<uint8, std::string> OLD_LIC_CODE = {
  { 0x00, "None" },
  { 0x01, "Nintendo" },
  { 0x08, "Capcom" },
  { 0x09, "HOT-B" },
  { 0x0A, "Jaleco" },
  { 0x0B, "Coconuts Japan" },
  { 0x0C, "Elite Systems" },
  { 0x13, "EA (Electronic Arts)" },
  { 0x18, "Hudson Soft" },
  { 0x19, "ITC Entertainment" },
  { 0x1A, "Yanoman" },
  { 0x1D, "Japan Clary" },
  { 0x1F, "Virgin Games Ltd." },
  { 0x24, "PCM Complete" },
  { 0x25, "San-X" },
  { 0x28, "Kemco" },
  { 0x29, "SETA Corporation" },
  { 0x30, "Infogrames" },
  { 0x31, "Nintendo" },
  { 0x32, "Bandai" },
  { 0x34, "Konami" },
  { 0x35, "HectorSoft" },
  { 0x38, "Capcom" },
  { 0x39, "Banpresto" },
  { 0x3C, "Entertainment Interactive (stub)" },
  { 0x3E, "Gremlin" },
  { 0x41, "Ubi Soft" },
  { 0x42, "Atlus" },
  { 0x44, "Malibu Interactive" },
  { 0x46, "Angel" },
  { 0x47, "Spectrum HoloByte" },
  { 0x49, "Irem" },
  { 0x4A, "Virgin Games Ltd." },
  { 0x4D, "Malibu Interactive" },
  { 0x4F, "U.S. Gold" },
  { 0x50, "Absolute" },
  { 0x51, "Acclaim Entertainment" },
  { 0x52, "Activision" },
  { 0x53, "Sammy USA Corporation" },
  { 0x54, "GameTek" },
  { 0x55, "Park Place" },
  { 0x56, "LJN" },
  { 0x57, "Matchbox" },
  { 0x58, "Milton Bradley Company" },
  { 0x5A, "Mindscape" },
  { 0x5B, "Romstar" },
  { 0x5C, "Naxat Soft" },
  { 0x5D, "Tradewest" },
  { 0x60, "Titus Interactive" },
  { 0x61, "Virgin Games Ltd." },
  { 0x67, "Ocean Software" },
  { 0x69, "EA (Electronic Arts)" },
  { 0x6E, "Elite Systems" },
  { 0x6F, "Electro Brain" },
  { 0x70, "Infogrames" },
  { 0x71, "Interplay Entertainment" },
  { 0x72, "Broderbund" },
  { 0x73, "Sculptured Software" },
  { 0x75, "The Sales Curve Limited" },
  { 0x78, "THQ" },
  { 0x79, "Accolade" },
  { 0x7A, "Triffix Entertainment" },
  { 0x7C, "MicroProse" },
  { 0x7F, "Kemco" },
  { 0x80, "Misawa Entertainment" },
  { 0x83, "LOZC G." },
  { 0x86, "Tokuma Shoten" },
  { 0x8B, "Bullet-Proof Software" },
  { 0x8C, "Vic Tokai Corp" },
  { 0x8E, "Ape Inc" },
  { 0x8F, "I’Max" },
  { 0x91, "Chunsoft Co." },
  { 0x92, "Video System" },
  { 0x93, "Tsubaraya Productions" },
  { 0x95, "Varie" },
  { 0x96, "Yonezawa" },
  { 0x97, "Kemco" },
  { 0x99, "Arc" },
  { 0x9A, "Nihon Bussan" },
  { 0x9B, "Tecmo" },
  { 0x9C, "Imagineer" },
  { 0x9D, "Banpresto" },
  { 0x9F, "Nova" },
  { 0xA1, "Hori Electric" },
  { 0xA2, "Bandai" },
  { 0xA4, "Konami" },
  { 0xA6, "Kawada" },
  { 0xA7, "Takara" },
  { 0xA9, "Technos Japan" },
  { 0xAA, "Broderbund" },
  { 0xAC, "Toei Animation" },
  { 0xAD, "Toho" },
  { 0xAF, "Namco" },
  { 0xB0, "Acclaim Entertainment" },
  { 0xB1, "ASCII Corporation" },
  { 0xB2, "Bandai" },
  { 0xB4, "Square Enix" },
  { 0xB6, "HAL Laboratory" },
  { 0xB7, "SNK" },
  { 0xB9, "Pony Canyon" },
  { 0xBA, "Culture Brain" },
  { 0xBB, "Sunsoft" },
  { 0xBD, "Sony Imagesoft" },
  { 0xBF, "Sammy Corporation" },
  { 0xC0, "Taito" },
  { 0xC2, "Kemco" },
  { 0xC3, "Square" },
  { 0xC4, "Tokuma Shoten" },
  { 0xC5, "Data East" },
  { 0xC6, "Tonkin House" },
  { 0xC8, "Koei" },
  { 0xC9, "UFL" },
  { 0xCA, "Ultra Games" },
  { 0xCB, "VAP, Inc." },
  { 0xCC, "Use Corporation" },
  { 0xCD, "Meldac" },
  { 0xCE, "Pony Canyon" },
  { 0xCF, "Angel" },
  { 0xD0, "Taito" },
  { 0xD1, "SOFEL (Software Engineering Lab)" },
  { 0xD2, "Quest" },
  { 0xD3, "Sigma Enterprises" },
  { 0xD4, "ASK Kodansha Co." },
  { 0xD6, "Naxat Soft" },
  { 0xD7, "Copya System" },
  { 0xD9, "Banpresto" },
  { 0xDA, "Tomy" },
  { 0xDB, "LJN" },
  { 0xDD, "Nippon Computer Systems" },
  { 0xDE, "Human Ent." },
  { 0xDF, "Altron" },
  { 0xE0, "Jaleco" },
  { 0xE1, "Towa Chiki" },
  { 0xE2, "Yutaka" },
  { 0xE3, "Varie" },
  { 0xE5, "Epoch" },
  { 0xE7, "Athena" },
  { 0xE8, "Asmik Ace Entertainment" },
  { 0xE9, "Natsume" },
  { 0xEA, "King Records" },
  { 0xEB, "Atlus" },
  { 0xEC, "Epic/Sony Records" },
  { 0xEE, "IGS" },
  { 0xF0, "A Wave" },
  { 0xF3, "Extreme Entertainment" },
  { 0xFF, "LJN" }
};

const std::unordered_map<uint8, std::string> LIC_CODE = {
  { 0x00, "None" },
  { 0x01, "Nintendo R&D1" },
  { 0x08, "Capcom" },
  { 0x13, "Electronic Arts" },
  { 0x18, "Hudson Soft" },
  { 0x19, "B-AI" },
  { 0x20, "KSS" },
  { 0x22, "Planning Office WADA" },
  { 0x24, "PCM Complete" },
  { 0x25, "SAN-X" },
  { 0x28, "Kemco" },
  { 0x29, "SETA Corporation" },
  { 0x30, "Viacom" },
  { 0x31, "Nintendo" },
  { 0x32, "Bandai" },
  { 0x33, "Ocean Software/Acclaim Entertainment" },
  { 0x34, "Konami" },
  { 0x35, "HectorSoft" },
  { 0x37, "Taito" },
  { 0x38, "HudsonSoft" },
  { 0x39, "Banpresto" },
  { 0x41, "Ubi Soft" },
  { 0x42, "Atlus" },
  { 0x44, "Malibu Interactive" },
  { 0x46, "Angel" },
  { 0x47, "Bullet-Proof Software" },
  { 0x49, "Irem" },
  { 0x50, "Absolute" },
  { 0x51, "Acclaim Entertainment" },
  { 0x52, "Activision" },
  { 0x53, "Sammy USA Corporation" },
  { 0x54, "Konami" },
  { 0x55, "Hi Tech Expressions" },
  { 0x56, "LJN" },
  { 0x57, "Matchbox" },
  { 0x58, "Mattel" },
  { 0x59, "Milton Bradley Company" },
  { 0x60, "Titus Interactive" },
  { 0x61, "Virgin Games Ltd." },
  { 0x64, "Lucasfilm Games" },
  { 0x67, "Ocean Software" },
  { 0x69, "EA (Electronic Arts)" },
  { 0x70, "Infogrames" },
  { 0x71, "Interplay Entertainment" },
  { 0x72, "Broderbund" },
  { 0x73, "Sculptured Software" },
  { 0x75, "The Sales Curve Limited" },
  { 0x78, "THQ" },
  { 0x79, "Accolade" },
  { 0x80, "Misawa Entertainment" },
  { 0x83, "lozc" },
  { 0x86, "Tokuma Shoten" },
  { 0x87, "Tsukuda Original" },
  { 0x91, "Chunsoft Co." },
  { 0x92, "Video system" },
  { 0x93, "Ocean Software/Acclaim Entertainment" },
  { 0x95, "Varie" },
  { 0x96, "Yonezawa/s’pal" },
  { 0x97, "Kaneko" },
  { 0x99, "Pack-In-Video" },
  { 0xA4, "Konami (Yu-Gi-Oh!)" }
};

constexpr uint8 NINTENDO_LOGO[] = {
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
  0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
  0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

enum class Component
{
  CPU,
  PPU,
  DMA
};

template<typename T>
T
mask_n_bits(uint8 n, T value)
{
  T mask = (1 << n) - 1;
  return value & mask;
}

#define DEBUG_ENABLED false
class Logger
{
public:
  static Logger& getInstance()
  {
    static Logger logger;
    return logger;
  }
  void log(std::string msg);

private:
  Logger()
  {
    if (DEBUG_ENABLED) {
      myfile.open("/home/nemanja/Desktop/debug_output.log", std::ios_base::app);
    }
  }
  ~Logger()
  {
    if (DEBUG_ENABLED) {
      myfile.flush();
      myfile.close();
    }
  }
  std::mutex logger_lock;
  std::ofstream myfile;
};

char*
getTimeString();

#define internal_log(message, args...)                                         \
  {                                                                            \
    int size_s = std::snprintf(nullptr, 0, message, args) + 1;                 \
    if (size_s <= 0) {                                                         \
      Logger::getInstance().log("Error during log formatting.");               \
    } else {                                                                   \
      std::unique_ptr<char[]> buf(new char[size_s]);                           \
      std::snprintf(buf.get(), size_s, message, args);                         \
      Logger::getInstance().log(                                               \
        std::string(buf.get(), buf.get() + size_s - 1));                       \
    }                                                                          \
  }

#define log_error(message, args...)                                            \
  internal_log("%s | %-5s | %s:%d | " message,                                 \
               getTimeString(),                                                \
               "ERROR",                                                        \
               __PRETTY_FUNCTION__,                                            \
               __LINE__,                                                       \
               ##args)

#define log_info(message, args...)                                             \
  internal_log("%s | %-5s | %s:%d | " message,                                 \
               getTimeString(),                                                \
               "INFO",                                                         \
               __PRETTY_FUNCTION__,                                            \
               __LINE__,                                                       \
               ##args)

#define log_debug(message, args...)                                            \
  if (DEBUG_ENABLED) {                                                         \
    internal_log("%-5s | " message, "DEBUG", ##args)                           \
  }

#endif // COMMON_H
