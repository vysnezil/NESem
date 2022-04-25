#pragma once
#include "Save.h"

#include <string>
#include <sstream>

#include "BUS.h"
#include <filesystem>
#include <iostream>
#include <fstream>

#include <algorithm>
#include <cctype>

#include "json.hpp"

extern "C" {
	#include "md5.h"
}

class SaveManager
{
public:
	Save* save(Bus* bus);

	std::string hash;
	Bus* bus;
	void writeToFile(Save* save);

	static bool compareNat(const std::string& a, const std::string& b);

	static std::string toUpper(std::string s);

	std::vector<std::string>* getSavesByRom(char* hash);

	std::string toString(uint8_t* val, int size);

	json saveToJson(Save* save);

	json getSpriteList(Save::Sprite* value);

	uint8_t* getArray(std::string value, int size);

	Save* getFromJson(json json);

	json readFile(std::string hash, std::string name);

	void loadSave(std::string name);

	void renameSave(std::string name, std::string newName);

	const char* getHash(char* cart, size_t size);

	static SaveManager& getInstance() {
		static SaveManager instance;
		return instance;
	}

private:
	SaveManager() {};
};