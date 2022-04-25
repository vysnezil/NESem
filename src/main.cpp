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
	

	//const char* path = argv[1] ? argv[1] : Dialog::getFile();

	Logger logger = Logger::getInstance();

	//const char* path = "..\\roms\\smb.nes";
	
	
	/*
	if (!card.success) {
		std::string unused;
		logger.log("Press ENTER to exit");
		std::getline(std::cin, unused);
		exit(1);
	}*/
	
	

	Menu menu;
	glHelper gl = glHelper::getInstance();
	
	Bus* bus = new Bus();
	Input input(glHelper::getInstance().window);

	if (argv[1]){
		menu.card = std::make_shared<Cartridge>(argv[1]);
		menu.show = false;
		bus->loadCartridge(menu.card.get());
		bus->setInput(&input.controller);
		bus->reset();
	}

	Display display;
	
	bus->setInput(&input.controller);

	//menu.saves = SaveManager::getInstance().getSavesByRom((char*)menu.card->hash);
	if (menu.saves->size() > 0) menu.selectedSave = menu.saves->at(0);
	SaveManager::getInstance().bus = bus;

	while (!gl.shouldClose()) {
		gl.setupRender();
		if (menu.show) {
			menu.update();
		}
		else {
			if (input.menuFlag) {
				menu.saves = SaveManager::getInstance().getSavesByRom((char*)menu.card->hash);
				menu.show = true;
				glHelper::getInstance().resizeWindow(true);
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
			menu.selectedSave = save->name;
			input.saveFlag = false;
		}
		if (input.loadFlag) {
			SaveManager::getInstance().loadSave(menu.selectedSave);
			input.loadFlag = false;
		}
	}
}