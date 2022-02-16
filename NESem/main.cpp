#include "Display.h"
#include "Cartridge.h"
#include "Bus.h"
#include "PPU.h"
#include <chrono>
#include <thread>
#include "Input.h"

int main(int argc, char** argv) {
	Display display;
	Input input(display.window);
	const char* path = argv[1] ? argv[1] : "..\\roms\\nestest.nes";
	Logger::log("Loaded rom: ");
	Logger::log(path);
	Cartridge card(path);
	Bus* bus = new Bus();
	bus->setInput(&input.controller);
	bus->loadCartridge(&card);
	bus->reset();
	//bus->cpu.jump(0xc000);

	//uint8_t* dat = new uint8_t[256 * 240 * 3];
	while (!display.shouldClose()) {
		do { bus->clock(); } while (!bus->ppu.finished);
		bus->ppu.finished = false;
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		display.update(bus->ppu.dat);
		display.updateEvents();
	}
	//Logger::file.close();
}