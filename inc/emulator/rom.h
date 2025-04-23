#ifndef ROM_H
#define ROM_H

#include "common.h"
#include <memory>

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

class Rom
{
public:
  Rom(std::string location);
  std::string getTitle();
  std::string getTypeName();
  std::string getLicName();

private:
  std::string getDebugMsg();

  std::string rom_location;
  // rom data
  std::unique_ptr<uint8[]> data;
  header* rom_header;
  uint32 rom_size;
};

#endif // ROM_H
