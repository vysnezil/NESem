#include "CPU.h"
#include "Bus.h"

void CPU::setBus(Bus* bus) {
	this->bus = bus;
}

void CPU::write(uint16_t address, uint8_t data)
{
	bus->write(address, data);
}

uint8_t CPU::read(uint16_t address) {
	return bus->read(address);
}

void CPU::reset() {
	x = y = a = sp = status = 0;
	setFlag(U, 1);
	sp = 0x00FF;
	pc = read(0xFFFC) | (read(0xFFFD) << 8);
}

void CPU::clock() {
	if (!cycles) {
        setFlag(U, 1);
		uint8_t op = read(pc++);
		cycles = instructionTable[op].cycles;
		address = (this->*instructionTable[op].mode)();
		(this->*instructionTable[op].function)();
	}
	cycles--;
}

void CPU::setFlag(flag flag, bool value) {
	if (value) status |= flag;
	else status &= ~flag;
}

void CPU::updateFlags(uint8_t val) {
	setFlag(Z, !(val & 0x00FF));
	setFlag(N, val & 0x80);
}

bool CPU::getFlag(flag flag) {
	return status & flag;
}

uint16_t CPU::IMP()
{
    return a;
}

uint16_t CPU::IMM()
{
    return pc++;
}

uint16_t CPU::ZP0()
{
    return 0x00FF & read(pc++);
}

uint16_t CPU::ZPX()
{
    return (read(pc++) + x) & 0x00FF;
}

uint16_t CPU::ZPY()
{
    return (read(pc++) + y) & 0x00FF;
}

uint16_t CPU::ABS()
{
    return (read(pc++) << 8) | read(pc++);
}

uint16_t CPU::ABX()
{
    uint8_t lo = read(pc++);
    uint8_t hi = read(pc++);
    uint16_t address = ((hi << 8) | lo) + x;
    if ((address & 0xFF00) == (hi << 8)) cycles++;
    return address;
}

uint16_t CPU::ABY()
{
    uint8_t lo = read(pc++);
    uint8_t hi = read(pc++);
    uint16_t address = ((hi << 8) | lo) + y;
    if ((address & 0xFF00) == (hi << 8)) cycles++;
    return address;
}

uint16_t CPU::IND()
{
    uint16_t ptr = (read(pc++) << 8) | read(pc++);
    //page boundary hardware bug
    if ((ptr & 0x00FF) == 0x00FF) return (read(ptr & 0xFF00) << 8) | read(ptr);
    else return (read(ptr + 1) << 8) | read(ptr);
}

uint16_t CPU::IZX()
{
    uint8_t ptr = read(pc++) + x;
    return read(ptr) & 0x00FF + (read(ptr + 1) << 8) & 0x00FF;
}

uint16_t CPU::IZY()
{
    uint8_t tmp = read(pc++);
    uint8_t lo = read(tmp & 0x00FF);
    uint8_t hi = read((tmp + 1) & 0x00FF);
    return (lo | (hi << 8)) +y;
}

uint16_t CPU::REL()
{
    return pc + (int8_t)read(pc++);
}
