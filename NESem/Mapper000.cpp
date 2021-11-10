#include "Mapper000.h"

Mapper000::Mapper000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) { }

uint32_t Mapper000::cpuMapRead(uint16_t addr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		return addr & (PRGBankCount > 1 ? 0x7FFF : 0x3FFF);
	}
}

uint32_t Mapper000::cpuMapWrite(uint16_t addr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		return addr & (PRGBankCount > 1 ? 0x7FFF : 0x3FFF);
	}
}

uint32_t Mapper000::ppuMapRead(uint16_t addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		return addr;
	}
}

uint32_t Mapper000::ppuMapWrite(uint16_t addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		return addr;
	}
}
