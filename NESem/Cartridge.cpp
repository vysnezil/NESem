#include "Cartridge.h"
#include <fstream>
#include "Mapper000.h"

Cartridge::Cartridge(const char* path)
{
	struct iNES_Header
	{
		char name[4];
		uint8_t prg_chunks;
		uint8_t chr_chunks;
		uint8_t flags1;
		uint8_t flags2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];
	} header;

	std::ifstream file;
	file.open(path, std::ifstream::binary);
	file.seekg(0, std::ios::beg);
	if (file.is_open())
	{
		file.read((char*)&header, sizeof(header));

		//for now, lets assume that this file is valid nes format
		//if (header.name != "NES") std::cerr << "\nInvalid NES file!";

		if (header.flags1 & 1 << 2) file.seekg(512, std::ios_base::cur);
		uint8_t mapperID = (header.flags2 | (header.flags1 >> 4));
		mirror = (header.flags1 & 0x01) ? VERTICAL : HORIZONTAL;

		if (header.flags1 & 0x08) {
			//handle NES 2.0 format
		}

		else {
			//iNES format
			PRGBankCount = header.prg_chunks;
			PRGMemory = new uint8_t[16384 * PRGBankCount];
			file.read((char*)PRGMemory, (16384 * PRGBankCount));

			CHRBankCount = header.chr_chunks;
			CHRMemory = new uint8_t[8192 * CHRBankCount];
			file.read((char*)CHRMemory, (8192 * CHRBankCount));

		}

		switch (mapperID)
		{
		case 0:
			mapper = new Mapper000(PRGBankCount, CHRBankCount);
			break;
		}

		file.close();
	}
	else Logger::log("CARTRIDGE: file opening error");
}

uint8_t Cartridge::cpuRead(uint16_t addr)
{
	return PRGMemory[mapper->cpuMapRead(addr)];
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	PRGMemory[mapper->cpuMapWrite(addr)] = data;
	if (mapper->cpuMapWrite(addr))
	{
		PRGMemory[mapper->cpuMapWrite(addr)] = data;
		return true;
	}
	else
		return false;
}

uint8_t Cartridge::ppuRead(uint16_t addr)
{
	return CHRMemory[mapper->ppuMapRead(addr)];
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	CHRMemory[mapper->ppuMapWrite(addr)] = data;
	if (mapper->ppuMapWrite(addr))
	{
		CHRMemory[mapper->ppuMapWrite(addr)] = data;
		return true;
	}
	else
		return false;
}