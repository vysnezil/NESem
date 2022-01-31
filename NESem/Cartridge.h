#pragma once
#include <../../Mapper.h>
#include <cstdint>
#include "Logger.h"

class Cartridge
{
	uint8_t PRGBankCount = 0;
	uint8_t CHRBankCount = 0;

	uint8_t* PRGMemory = nullptr;
	uint8_t* CHRMemory = nullptr;

	Mapper* mapper = nullptr;

public:
	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

	Cartridge(const char* path);
	uint8_t cpuRead(uint16_t addr);
	bool cpuWrite(uint16_t addr, uint8_t data);

	uint8_t ppuRead(uint16_t addr);
	bool ppuWrite(uint16_t addr, uint8_t data);
};

