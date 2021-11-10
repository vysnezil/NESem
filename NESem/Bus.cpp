#include "Bus.h"

Bus::Bus()
{
	cpu.setBus(this);
}

void Bus::loadCartridge(Cartridge* cart)
{
	this->cart = cart;
}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) ram[addr & 0x07FF] = data;
	else cart->cpuWrite(addr, data);
}

uint8_t Bus::read(uint16_t addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) return ram[addr & 0x07FF];
	return cart->cpuRead(addr);
}

void Bus::reset()
{
	cpu.reset();
	system_clocks = 0;
}

void Bus::clock()
{
	cpu.clock();
	system_clocks++;
}