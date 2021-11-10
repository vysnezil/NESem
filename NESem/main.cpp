#include "Cartridge.h"
#include "Bus.h"
#include <chrono>
#include <thread>

int main() {
	Logger logger;

	Cartridge card(".\\roms\\nestest.nes");
	Bus bus;
	bus.loadCartridge(&card);
	bus.reset();
	bus.cpu.jump(0xc000);

	/*for (size_t i = 0x1FFF; i < 32768; i++)
	{
		logger.logHex(bus.read(i));
	}*/
	

	while (1) {
		bus.clock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}