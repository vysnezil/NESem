#include "Cartridge.h"
#include "Bus.h"
#include <chrono>
#include <thread>

int main() {
	Logger logger;

	Cartridge card("..\\roms\\nestest.nes");
	Bus bus;
	bus.loadCartridge(&card);
	bus.reset();
	bus.cpu.jump(0xc000);

	while (1) {
		bus.clock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}