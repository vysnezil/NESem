#pragma once
#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>

namespace Logger {
	//static std::ofstream file = std::ofstream("logs/latest.txt");

	inline void log() {
		std::cout << std::endl;
	}

	template<typename T, typename... Args> void log(T t, Args... args) {
		std::cout << t;
		log(args...);
	}

	void logHex(uint16_t num);
	void logInstruction(uint8_t op, const char* name, uint16_t address, uint8_t x, uint8_t y, uint8_t a, uint8_t sp, uint8_t status, uint16_t pc);
}
