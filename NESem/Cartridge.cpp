#include "Cartridge.h"
#include <fstream>

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
			file.read((char*)PRGMemory, sizeof(PRGMemory));

			CHRBankCount = header.chr_chunks;
			CHRMemory = new uint8_t[8192 * CHRBankCount];
			file.read((char*)CHRMemory, sizeof(CHRMemory));
		}

		switch (mapperID)
		{
		case 0:
			//create mapper 0
			break;
		}

		file.close();
	}
	//else handle arror
}
