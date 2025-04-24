#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <memory>

#include "common.h"

typedef struct _header
{
  uint8 entry[4];
  uint8 logo[0x30];
  char title[16];
  uint16 lic_code_new;
  uint8 sgb_flag;
  uint8 type;
  uint8 rom_size;
  uint8 ram_size;
  uint8 dest_code;
  uint8 lic_code;
  uint8 version;
  uint8 checksum;
  uint16 global_checksum;
} header;

class MBC_Handler;

class Cartridge
{
  friend class MBC_Handler;

public:
  Cartridge(std::string location);
  ~Cartridge();

  std::string getTitle();
  std::string getTypeName();
  std::string getLicName();

  void write(uint16 address, uint8 val);
  uint8 read(uint16 address);

  bool isValidCartridge();

private:
  std::string getDebugMsg();
  bool checkData();

  std::string m_cartridge_location;
  bool m_valid = false;
  // cartridge data
  std::unique_ptr<uint8[]> m_data;
  header* m_cartridge_header;
  uint32 m_cartridge_size;

  std::unique_ptr<MBC_Handler> m_mbc_handler;
};

#endif // CARTRIDGE_H
