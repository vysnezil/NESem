#include "SaveManager.h"

extern "C" unsigned char* md5(unsigned char*, md5_size, unsigned char*);

const char* SaveManager::getHash(char* cart, size_t size)
{
	unsigned char digest[16];
	md5((unsigned char*)cart, size, digest);
	std::ostringstream oss;
	oss << std::hex;
	for (int c : digest) oss << c;
	return strdup(oss.str().c_str());
}

Save* SaveManager::save(Bus* bus)
{
	Save* save = new Save{
		bus->cpu.getState(),
		bus->ppu.getState(),
		bus->system_clocks,
		"New Save",
		bus->ppu.dat,
		(char *)bus->cart->hash
	};
	std::string name = save->name;

	int index = 2;
	while (true)
	{
		if (std::filesystem::exists("saves/" + (std::string)hash + "/" + name + ".json")) {
			name = save->name + "-" + std::to_string(index);
		}
		else break;
		index++;
	}
	save->name = name;
	
	writeToFile(save);
	return save;
}

void SaveManager::writeToFile(Save* save) {
	auto savesDir = std::filesystem::create_directory("saves");
	auto romDir = std::filesystem::create_directory("saves/" + (std::string)save->romHash);
	std::ofstream outfile("saves/" + (std::string)save->romHash + "/" + save->name + ".json");
	outfile << saveToJson(save);
	outfile.close();
}

std::vector<std::string>* SaveManager::getSavesByRom(char* hash) {
	this->hash = hash;
	std::vector<std::string>* saves = new std::vector<std::string>();
	std::string path = "saves/" + (std::string)hash;
	if (std::filesystem::exists(path)) {
		for (const auto& entry : std::filesystem::directory_iterator(path))
			saves->push_back(entry.path().filename().string());
	}
	return saves;
}

json SaveManager::saveToJson(Save* save) {
	return {
		{"name", save->name},
		{"cpuState", {
			{"x", save->cpu_state.x},
			{"y", save->cpu_state.y},
			{"a", save->cpu_state.a},
			{"sp", save->cpu_state.sp},
			{"pc", save->cpu_state.pc},
			{"status", save->cpu_state.status},
			{"address", save->cpu_state.address},
			{"cycles", save->cpu_state.cycles},
		}},
		{"ppuState", {
			{"status", save->ppu_state.status},
			{"mask", save->ppu_state.mask},
			{"control", save->ppu_state.control},
			{"vram", save->ppu_state.vram_addr},
			{"tram", save->ppu_state.tram_addr},
			{"fine_x", save->ppu_state.fine_x},
		}},
		{"clocks", save->system_clocks},
		{"hash", save->romHash}
	};
}

Save* SaveManager::getFromJson(json json) {
	Save* save = new Save();
	save->name = json["name"];
	save->cpu_state.x = json["cpuState"]["x"];
	save->cpu_state.y = json["cpuState"]["y"];
	save->cpu_state.a = json["cpuState"]["a"];
	save->cpu_state.sp = json["cpuState"]["sp"];
	save->cpu_state.status = json["cpuState"]["status"];
	save->cpu_state.pc = json["cpuState"]["pc"];
	save->cpu_state.address = json["cpuState"]["address"];
	save->cpu_state.cycles = json["cpuState"]["cycles"];
	save->ppu_state.status = json["ppuState"]["status"];
	save->ppu_state.mask = json["ppuState"]["mask"];
	save->ppu_state.control = json["ppuState"]["control"];
	save->ppu_state.vram_addr = json["ppuState"]["vram"];
	save->ppu_state.tram_addr = json["ppuState"]["tram"];
	save->ppu_state.fine_x = json["ppuState"]["fine_x"];
	save->system_clocks = json["clocks"];
	save->romHash = json["hash"];
	return save;
}

json SaveManager::readFile(std::string hash, std::string name) {
	json j;
	std::string path = "saves/" + (std::string)hash;
	if (std::filesystem::exists(path)) {
		Logger::getInstance().log(path + "/" + name);
		std::ifstream i;
		i.open(path + "/" + name);
		std::stringstream ss;
		ss << i.rdbuf();
		Logger::getInstance().log(ss.str());
		j = json::parse(ss.str());
	}
	else Logger::getInstance().log("no such rom exists");
	return j;
}

void SaveManager::loadSave(std::string name)
{
	json js = readFile(hash ,name);
	Save* save = getFromJson(js);

}
