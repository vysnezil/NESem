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

	if (argv[1]) {
		menu.card = std::make_shared<Cartridge>(argv[1]);
		menu.show = false;
		glHelper::getInstance().resizeWindow(false);
		bus->loadCartridge(menu.card.get());
		bus->setInput(&input.controller);
		bus->reset();
	}
	else (glHelper::getInstance().resizeWindow(true));

	Display display;
	Overlay::getInstance().init();
	
	bus->setInput(&input.controller);
	input.loadKeys();

	//menu.saves = SaveManager::getInstance().getSavesByRom((char*)menu.card->hash);
	if (menu.saves->size() > 0) menu.selectedSave = menu.saves->at(0);
	SaveManager::getInstance().bus = bus;

	while (!gl.shouldClose()) {
		gl.setupRender();
		if (menu.show) {
			menu.update();
			if (input.saveFlag) input.saveFlag = false;
			if (input.loadFlag) input.loadFlag = false;
			if (input.menuFlag) input.menuFlag = false;
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

			if (input.saveFlag) {
				Save* save = SaveManager::getInstance().save(bus);
				menu.selectedSave = save->name;
				Overlay::getInstance().begin("Save created", 2);
				input.saveFlag = false;
			}
			if (input.loadFlag) {
				if (menu.selectedSave != "") {
					SaveManager::getInstance().loadSave(menu.selectedSave);
					Overlay::getInstance().begin("Loaded last save", 2);
				}
				input.loadFlag = false;
			}
		}
		gl.render();
	}
}