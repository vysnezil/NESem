#include "Bus.h"

Bus::Bus()
{
	cpu.setBus(this);
}

void Bus::loadCartridge(Cartridge* cart)
{
	this->cart = cart;
	ppu.loadCartridge(cart);
}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) ram[addr & 0x07FF] = data;
	else if (addr >= 0x2000 && addr <= 0x3FFF) ppu.cpuWrite(addr & 0x0007, data);
	else cart->cpuWrite(addr, data);
}

uint8_t Bus::read(uint16_t addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) return ram[addr & 0x07FF];
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		return ppu.cpuRead(addr & 0x0007);
	}
	uint8_t temp = cart->cpuRead(addr);
	//Logger::logHex(addr);
	//Logger::logHex(temp);
	return temp;

}

void Bus::reset()
{
	cpu.reset();
	ppu.reset();
	system_clocks = 0;
}

void Bus::clock()
{
	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.NMI();
	}
	ppu.clock();
	if (system_clocks % 3 == 0) {
		cpu.clock();
	}
	system_clocks++;
}