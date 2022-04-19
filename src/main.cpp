#include "Display.h"
#include "Cartridge.h"
#include "Bus.h"
#include "PPU.h"
#include <chrono>
#include <thread>
#include "Input.h"
#include "Dialog.h"
#include "Menu.h"
#include "Logger.h"
#include "SaveManager.h"

int main(int argc, char** argv) {
	

	const char* path = argv[1] ? argv[1] : Dialog::getFile();

	Logger logger = Logger::getInstance();

	//const char* path = "..\\roms\\smb.nes";
	Cartridge card(path);

	if (!card.success) {
		std::string unused;
		logger.log("Press ENTER to exit");
		std::getline(std::cin, unused);
		exit(1);
	}
	
	Display display;
	Input input(glHelper::getInstance().window);

	Bus* bus = new Bus();
	bus->setInput(&input.controller);
	bus->loadCartridge(&card);
	bus->reset();

	Menu menu;
	glHelper gl = glHelper::getInstance();
	
	
	menu.saves = SaveManager::getInstance().getSavesByRom((char*)card.hash);
	if (menu.saves->size() > 0) menu.selectedSave = menu.saves->at(0);

	while (!gl.shouldClose()) {
		gl.setupRender();
		if (menu.show) {
			menu.update();
		}
		else {
			if (input.menuFlag) {
				menu.show = true;
				input.menuFlag = false;
			}
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
			display.update(bus->ppu.dat);
		}
		gl.render();
		if (input.saveFlag) {
			Save* save = SaveManager::getInstance().save(bus);
			input.saveFlag = false;
		}
	}
}