#include "Cartridge.h"
#include "Mapper/Mapper000.h"

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

		if (header.name[0]  != 'N' && header.name[1] != 'E' && header.name[2] != 'S' && header.name[3] != 0x1A) {
			Logger::log("CARTRIDGE: Invalid nes file!");
			Logger::log("path: ", path);
			file.close();
			return;
		}

		if (header.flags1 & 0x04) file.seekg(512, std::ios_base::cur);
		uint8_t mapperID = (header.flags2 | (header.flags1 >> 4));
		mirror = (header.flags1 & 0x01) ? VERTICAL : HORIZONTAL;

		if (header.flags1 & 0x08) {
			Logger::log("CARTRIDGE: NES2.0 format not supported yet!");
		}

		else {
			//iNES format
			PRGBankCount = header.prg_chunks;
			PRGMemory = new uint8_t[16384 * PRGBankCount];
			file.read((char*)PRGMemory, (static_cast<std::streamsize>(16384) * PRGBankCount));

			CHRBankCount = header.chr_chunks;
			CHRMemory = new uint8_t[8192 * CHRBankCount];
			if (CHRBankCount == 0)
			{
				CHRMemory = new uint8_t[8192 * CHRBankCount];
			}
			file.read((char*)CHRMemory, (static_cast<std::streamsize>(8192) * CHRBankCount));

		}
		file.close();

		switch (mapperID)
		{
		case 0:
			mapper = new Mapper000(PRGBankCount, CHRBankCount);
			break;
		default:
			Logger::log("CARTRIDGE: Mapper ", std::to_string(mapperID), " not supported!");
			return;
		}
		Logger::log("Loaded rom: ", path);
		success = true;
	}
	else {
		Logger::log("CARTRIDGE: file opening error");
		Logger::log("path: ", path);
	}
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (mapper->cpuMapRead(addr, mapped_addr))
	{
		data = PRGMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (mapper->cpuMapWrite(addr, mapped_addr))
	{
		PRGMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;

	if (mapper->ppuMapRead(addr, mapped_addr))
	{
		data = CHRMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (mapper->ppuMapWrite(addr, mapped_addr))
	{
		CHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}