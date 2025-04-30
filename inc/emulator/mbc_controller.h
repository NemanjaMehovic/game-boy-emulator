#ifndef MBC_CONTROLLER_H
#define MBC_CONTROLLER_H

#include "cartridge.h"
#include "common.h"

class MBC_Handler
{
public:
  MBC_Handler(uint8* data, header* header);
  virtual ~MBC_Handler();
  void write(uint16 address, uint8 val);
  uint8 read(uint16 address);

  static std::unique_ptr<MBC_Handler> CreateHandler(Cartridge* cartridge);

protected:
  uint8* m_data = nullptr;
  std::unique_ptr<uint8[]> m_ram = nullptr;
  header* m_header = nullptr;
  bool m_has_battery = false;
  bool m_enabled_ram = false;

  virtual void write_rom(uint16 address, uint8 val) = 0;
  virtual void write_ram(uint16 address, uint8 val) = 0;
  virtual uint8 read_rom(uint16 address) = 0;
  virtual uint8 read_ram(uint16 address) = 0;
};

class NoMBC_Handler : public MBC_Handler
{
public:
  NoMBC_Handler(uint8* data, header* header)
    : MBC_Handler(data, header)
  {
  }

protected:
  virtual void write_rom(uint16 address, uint8 val) override;
  virtual void write_ram(uint16 address, uint8 val) override;
  virtual uint8 read_rom(uint16 address) override;
  virtual uint8 read_ram(uint16 address) override;
};

class MBC1_Handler : public MBC_Handler
{
public:
  MBC1_Handler(uint8* data, header* header);

protected:
  virtual void write_rom(uint16 address, uint8 val) override;
  virtual void write_ram(uint16 address, uint8 val) override;
  virtual uint8 read_rom(uint16 address) override;
  virtual uint8 read_ram(uint16 address) override;

private:
  bool checkIsMBC1M();

  uint8 m_mode = 0;
  uint8 m_low_banking_bits = 0;
  uint8 m_high_banking_bits = 0;
  bool m_is_mbc1m = false;
};

class MBC2_Handler : public MBC_Handler
{
public:
  MBC2_Handler(uint8* data, header* header);

protected:
  virtual void write_rom(uint16 address, uint8 val) override;
  virtual void write_ram(uint16 address, uint8 val) override;
  virtual uint8 read_rom(uint16 address) override;
  virtual uint8 read_ram(uint16 address) override;

private:
  uint8 m_banking_bits = 0;
};

#endif // MBC_CONTROLLER_H
