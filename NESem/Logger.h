#pragma once
#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <sstream>

class Logger
{
public:
	template<typename T> static void log(T t) {
		std::cout << t << std::endl;
	}

	void logHex(uint16_t num);
	void logInstruction(uint8_t op, const char* name, uint16_t address, uint8_t x, uint8_t y, uint8_t a, uint8_t sp, uint8_t status);
	uint16_t pc;
};