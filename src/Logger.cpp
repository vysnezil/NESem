#include "Logger.h"
#include <fstream>

void Logger::logHex(uint16_t num)
{
    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << +num << std::endl;
}

void Logger::logInstruction(uint8_t op, const char* name, uint16_t address, uint8_t x, uint8_t y, uint8_t a, uint8_t sp, uint8_t status, uint16_t pc)
{
    std::ostringstream oss;
    oss << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << +pc << " " << name << " (0x" << std::setfill('0') << std::setw(2) << +op << ") ";
    oss << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << address << " ";
    oss << "x: " << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << +x << " ";
    oss << "y: " << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << +y << " ";
    oss << "a: " << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << +a << " ";
    oss << "sp: " << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << +sp << " ";
    oss << "status: " << "0b" << std::setfill('0') << std::setw(2) << std::right << std::bitset<8>(+status) << std::endl;
    std::cout << oss.str();
  
    //file << std::setfill('0') << std::setw(2) << std::right << std::hex << +pc << std::endl;
}


