#include "rom.h"

#include <fstream>
#include <unordered_map>

static const std::unordered_map<uint8, std::string> ROM_TYPES = {
  { 0x00, "ROM ONLY" },
  { 0x01, "MBC1" },
  { 0x02, "MBC1+RAM" },
  { 0x03, "MBC1+RAM+BATTERY" },
  { 0x05, "MBC2" },
  { 0x06, "MBC2+BATTERY" },
  { 0x08, "ROM+RAM" },
  { 0x09, "ROM+RAM+BATTERY" },
  { 0x0B, "MMM01" },
  { 0x0C, "MMM01+RAM" },
  { 0x0D, "MMM01+RAM+BATTERY" },
  { 0x0F, "MBC3+TIMER+BATTERY" },
  { 0x10, "MBC3+TIMER+RAM+BATTERY" },
  { 0x11, "MBC3" },
  { 0x12, "MBC3+RAM" },
  { 0x13, "MBC3+RAM+BATTERY" },
  { 0x19, "MBC5" },
  { 0x1A, "MBC5+RAM" },
  { 0x1B, "MBC5+RAM+BATTERY" },
  { 0x1C, "MBC5+RUMBLE" },
  { 0x1D, "MBC5+RUMBLE+RAM" },
  { 0x1E, "MBC5+RUMBLE+RAM+BATTERY" },
  { 0x20, "MBC6" },
  { 0x22, "MBC7+SENSOR+RUMBLE+RAM+BATTERY" },
  { 0xFC, "POCKET CAMERA" },
  { 0xFD, "BANDAI TAMA5" },
  { 0xFE, "HuC3" },
  { 0xFF, "HuC1+RAM+BATTERY" }
};

static const std::unordered_map<uint8, std::string> LIC_CODE = {
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

static_assert(sizeof(uint8) == sizeof(char));

Rom::Rom(std::string location)
  : rom_location(location)
{
  log_info("Loading %s rom", rom_location.c_str());
  // Open rom
  std::ifstream ifs(rom_location, std::ifstream::binary);
  std::filebuf* pbuf = ifs.rdbuf();
  // get rom size
  std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
  pbuf->pubseekpos(0, ifs.in);
  data = std::make_unique<uint8[]>(size);
  // read data
  pbuf->sgetn(reinterpret_cast<char*>(data.get()), size);
  ifs.close();
  // set values
  rom_header = reinterpret_cast<header*>(data.get() + 0x100);
  // last byte also used to indicate if it supports gmb colour
  rom_header->title[15] = 0;
  rom_size = size;

  uint8_t checksum = 0;
  for (uint16_t address = 0x0134; address <= 0x014C; address++) {
    checksum = checksum - data[address] - 1;
  }

  if (rom_header->checksum != checksum) {
    log_error(
      "Checksums do not match %d is in the header and %d was calculated",
      rom_header->checksum,
      checksum);
  }

  std::string str = "Finished loading the rom:\n" + getDebugMsg();
  log_info("%s", str.c_str());
}

std::string
Rom::getTitle()
{
  std::string str;
  str = rom_header->title;
  return str;
}

std::string
Rom::getTypeName()
{
  std::string str = "Unknown";
  auto itr = ROM_TYPES.find(rom_header->type);
  if (itr != ROM_TYPES.cend())
    str = itr->second;
  return str;
}

std::string
Rom::getLicName()
{
  std::string str = "Unknown";
  auto itr = LIC_CODE.find(rom_header->lic_code);
  if (itr != LIC_CODE.cend())
    str = itr->second;
  return str;
}

std::string
Rom::getDebugMsg()
{
  std::string str = "  Game title: " + getTitle() + "\n";
  str += "  Type: " + getTypeName() + "\n";
  str += "  LIC Code: " + getLicName() + "\n";
  str += "  ROM Size: " + std::to_string(32 << rom_header->rom_size) + " KB\n";
  str += "  RAM type: " + std::to_string(rom_header->ram_size) + "\n";
  str += "  ROM Version: " + std::to_string(rom_header->version);
  return str;
}
