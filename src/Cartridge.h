#pragma once
#include "Mapper/Mapper.h"
#include <cstdint>
#include "Logger.h"
#include <memory>

class Cartridge
{
	uint8_t PRGBankCount = 0;
	uint8_t CHRBankCount = 0;

	uint8_t* PRGMemory = nullptr;
	uint8_t* CHRMemory = nullptr;

	std::shared_ptr<Mapper> mapper = nullptr;

public:
	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

	Cartridge(const char* path);
	bool cpuRead(uint16_t addr, uint8_t& data);
	bool cpuWrite(uint16_t addr, uint8_t data);

	bool ppuRead(uint16_t addr, uint8_t & data);
	bool ppuWrite(uint16_t addr, uint8_t data);

	bool success = 0;
	const char* hash;
	std::string name;
};

