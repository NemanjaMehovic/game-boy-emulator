#include "cartridge.h"

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

static const std::unordered_map<uint8, std::string> OLD_LIC_CODE = {
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

Cartridge::Cartridge(std::string location)
  : cartridge_location(location)
{
  log_info("Loading %s cartridge", cartridge_location.c_str());
  // Open cartridge
  std::ifstream ifs(cartridge_location, std::ifstream::binary);
  std::filebuf* pbuf = ifs.rdbuf();
  // get cartridge size
  std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
  pbuf->pubseekpos(0, ifs.in);
  data = std::make_unique<uint8[]>(size);
  // read data
  pbuf->sgetn(reinterpret_cast<char*>(data.get()), size);
  ifs.close();
  // set values
  cartridge_header = reinterpret_cast<header*>(data.get() + 0x100);
  // last byte also used to indicate if it supports gmb colour
  cartridge_header->title[15] = 0;
  cartridge_size = size;

  uint8_t checksum = 0;
  for (uint16_t address = 0x0134; address <= 0x014C; address++) {
    checksum = checksum - data[address] - 1;
  }

  if (cartridge_header->checksum != checksum) {
    log_error(
      "Checksums do not match %d is in the header and %d was calculated",
      cartridge_header->checksum,
      checksum);
  }

  std::string str = "Finished loading the cartridge:\n" + getDebugMsg();
  log_info("%s", str.c_str());
}

std::string
Cartridge::getTitle()
{
  std::string str;
  str = cartridge_header->title;
  return str;
}

std::string
Cartridge::getTypeName()
{
  std::string str = "Unknown";
  auto itr = ROM_TYPES.find(cartridge_header->type);
  if (itr != ROM_TYPES.cend())
    str = itr->second;
  return str;
}

std::string
Cartridge::getLicName()
{
  std::string str = "Unknown";
  if (cartridge_header->lic_code == 0x33) {
    auto itr = LIC_CODE.find(cartridge_header->lic_code_new);
    str = itr != LIC_CODE.cend() ? itr->second : "Unknown";
  } else {
    auto itr = OLD_LIC_CODE.find(cartridge_header->lic_code);
    str = itr != OLD_LIC_CODE.cend() ? itr->second : "Unknown";
  }
  return str;
}

std::string
Cartridge::getDebugMsg()
{
  std::string str = "  Game title: " + getTitle() + "\n";
  str += "  Type: " + getTypeName() + "\n";
  str += "  LIC Code: " + getLicName() + "\n";
  str +=
    "  ROM Size: " + std::to_string(32 << cartridge_header->rom_size) + " KB\n";
  str += "  RAM type: " + std::to_string(cartridge_header->ram_size) + "\n";
  str += "  ROM Version: " + std::to_string(cartridge_header->version);
  return str;
}
