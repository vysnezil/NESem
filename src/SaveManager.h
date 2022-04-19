#pragma once
#include "Save.h"

#include <string>
#include <sstream>

#include "BUS.h"
#include <filesystem>
#include <iostream>
#include <fstream>

#include "json.hpp"

extern "C" {
	#include "md5.h"
}

class SaveManager
{
public:
	Save* save(Bus* bus);

	std::string hash;

	void writeToFile(Save* save);

	std::vector<std::string>* getSavesByRom(char* hash);

	json saveToJson(Save* save);

	Save* getFromJson(json json);

	json readFile(std::string hash, std::string name);

	void loadSave(std::string name);

	const char* getHash(char* cart, size_t size);

	static SaveManager& getInstance() {
		static SaveManager instance;
		return instance;
	}

private:
	SaveManager() {};
};