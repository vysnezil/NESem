#pragma once
#include <cstdint>
class Bus
{
public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

	void clock();
	void resetSystem();
};

