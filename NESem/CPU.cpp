#include "CPU.h"
#include "Bus.h"
void CPU::reset() {
	x = y = a = sp = status = 0;
	setFlag(U, 1);
	sp = 0x00FF;
	pc = read(0xFFFC) | (read(0xFFFD) << 8);
}

void CPU::write(uint16_t address, uint8_t data)
{
	bus->write(address, data);
}

uint8_t CPU::read(uint16_t address) {
	return bus->read(address);
}

void CPU::clock() {
	if (cycles == 0) {
		uint8_t op = read(pc++);
		cycles = instructionTable[op].cycles;
		(this->*instructionTable[op].mode)();
		(this->*instructionTable[op].function)();
	}
	cycles--;
}

void CPU::branch(CPU::flag flag, bool status) {

}

void CPU::setFlag(flag flag, bool value) {
	if (value) status |= flag;
	else status &= ~flag;
}

void CPU::updateFlags(uint8_t val) {

}

bool CPU::getFlag(flag flag) {
	return status & flag;
}

void CPU::setBus(Bus* bus) {
	this->bus = bus;
}
