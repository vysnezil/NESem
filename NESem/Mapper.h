#pragma once
#include <cstdint>

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);

public:
	virtual uint32_t cpuMapRead(uint16_t addr) = 0;
	virtual uint32_t cpuMapWrite(uint16_t addr) = 0;

	virtual uint32_t ppuMapRead(uint16_t addr) = 0;
	virtual uint32_t ppuMapWrite(uint16_t addr) = 0;

protected:
	uint8_t PRGBankCount = 0;
	uint8_t CHRBankCount = 0;
};
