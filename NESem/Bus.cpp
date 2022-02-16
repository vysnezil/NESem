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

void Bus::setInput(uint8_t* controller)
{
	this->controller[0] = controller;
}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0x1FFF) ram[addr & 0x07FF] = data;
	else if (addr >= 0x2000 && addr <= 0x3FFF) ppu.cpuWrite(addr & 0x0007, data);
	else if (addr == 0x4014)
	{
		DMA_page = data;
		DMA_addr = 0x00;
		DMA = true;
	}
	else if (addr == 0x4016 || addr == 0x4017)
	{
		//only one controller connected
		controller_state[0] = **controller;
		//controller_state[addr & 0x0001];
	}
	else cart->cpuWrite(addr, data);
}

uint8_t Bus::read(uint16_t addr)
{
	uint8_t data = 0;
	if (addr >= 0x0000 && addr <= 0x1FFF) return ram[addr & 0x07FF];
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		return ppu.cpuRead(addr & 0x0007);
	}
	else if (addr == 0x4016 || addr == 0x4017)
	{
		data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
	}
	else {
		uint8_t temp = 0;
		cart->cpuRead(addr, temp);
		return temp;
	}
	
	//Logger::logHex(addr);
	//Logger::logHex(temp);
	return data;

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
		if (DMA)
		{
			if (DMA_sync_flag) {
				if (system_clocks % 2 == 1)
					DMA_sync_flag = false;
				}
			else
			{
				if (system_clocks % 2 == 0) DMA_data = read(DMA_page << 8 | DMA_addr);
				else
				{
					((uint8_t*)ppu.OAM)[DMA_addr++] = DMA_data;
					if (DMA_addr == 0x00)
					{
						DMA = false;
						DMA_sync_flag = true;
					}
				}
			}
		}
		else cpu.clock();
	}
	system_clocks++;
}