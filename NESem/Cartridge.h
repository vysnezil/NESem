#pragma once
#include <../../Mapper.h>
#include <cstdint>
#include "Logger.h"

class Cartridge
{
	Logger logger;

	uint8_t PRGBankCount = 0;
	uint8_t CHRBankCount = 0;

	uint8_t* PRGMemory = nullptr;
	uint8_t* CHRMemory = nullptr;

	Mapper* mapper = nullptr;

	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

public:
	Cartridge(const char* path);
	uint8_t cpuRead(uint16_t addr);
	void cpuWrite(uint16_t addr, uint8_t data);

	uint8_t ppuRead(uint16_t addr);
	void ppuWrite(uint16_t addr, uint8_t data);
};

