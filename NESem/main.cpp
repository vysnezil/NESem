#include "Display.h"
#include "Cartridge.h"
#include "Bus.h"
#include "PPU.h"
#include <chrono>
#include <thread>

int main() {
	Display display;
	Cartridge card("..\\roms\\nestest.nes");
	Bus* bus = new Bus();
	bus->loadCartridge(&card);
	bus->reset();
	//bus->cpu.jump(0xc000);

	//uint8_t* dat = new uint8_t[256 * 240 * 3];
	while (!display.shouldClose()) {
		bus->clock();
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
		if (bus->ppu.finished) display.update(bus->ppu.dat);
		display.updateEvents();
	}
}