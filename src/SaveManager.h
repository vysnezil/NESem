#pragma once
#include "Save.h"

#include <string>
#include <sstream>

extern "C" {
#include "md5.h"
}

class SaveManager
{
public:
	void save(Save save) {

	}

	Save* getSavesByRom(char* hash) {

	}

	const char* getHash(const char* path);

	static SaveManager& getInstance() {
		static SaveManager instance;
		return instance;
	}

private:
	SaveManager() {};
};