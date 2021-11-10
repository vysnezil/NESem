#pragma once
#include "Mapper.h"

class Mapper000 : public Mapper
{
public:
	Mapper000(uint8_t prgBanks, uint8_t chrBanks);

public:
	uint32_t cpuMapRead(uint16_t addr) override;
	uint32_t cpuMapWrite(uint16_t addr) override;
	uint32_t ppuMapRead(uint16_t addr) override;
	uint32_t ppuMapWrite(uint16_t addr) override;
};