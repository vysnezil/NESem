#pragma once
#include <cstdint>
#include "CPU.h"
#include "Cartridge.h"

class Bus
{
public:
	CPU cpu;
	Cartridge* cart = nullptr;
	uint8_t ram[2048];

public:
	Bus();
	void loadCartridge(Cartridge* cart);
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

private:
	uint64_t system_clocks = 0;

public:
	void reset();
	void clock();
};
