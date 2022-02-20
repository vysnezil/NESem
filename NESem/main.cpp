#include "Display.h"
#include "Cartridge.h"
#include "Bus.h"
#include "PPU.h"
#include <chrono>
#include <thread>
#include "Input.h"
#include "Dialog.h"

int main(int argc, char** argv) {
	

	//const char* path = argv[1] ? argv[1] : Dialog::getFile();
	
	const char* path = "..\\roms\\smb.nes";
	Cartridge card(path);

	if (!card.success) {
		std::string unused;
		Logger::log("Press ENTER to exit");
		std::getline(std::cin, unused);
		exit(1);
	}

	Display display;
	Input input(display.window);

	Bus* bus = new Bus();
	bus->setInput(&input.controller);
	bus->loadCartridge(&card);
	bus->reset();
	//bus->cpu.jump(0xc000);

	//uint8_t* dat = new uint8_t[256 * 240 * 3];
	while (!display.shouldClose()) {
		if (!Input::singleStep) {
			do { bus->clock(); } while (!bus->ppu.finished);
			bus->ppu.finished = false;
		}
		else {
			if (Input::step) {
				bus->clock();
				Input::step = false;
				if (bus->ppu.finished) bus->ppu.finished = false;
			}
			if (Input::stepFrame) {
				do { bus->clock(); } while (!bus->ppu.finished);
				Input::stepFrame = false;
				if (bus->ppu.finished) bus->ppu.finished = false;
			}
		}
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		display.update(bus->ppu.dat);
		display.updateEvents();
	}
	//Logger::file.close();
}