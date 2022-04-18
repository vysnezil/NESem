#include "SaveManager.h"

extern "C" unsigned char* md5(unsigned char*, md5_size, unsigned char*);

const char* SaveManager::getHash(const char* path)
{
	unsigned char digest[16];
	md5((unsigned char*)path, strlen(path), digest);
	std::ostringstream oss;
	oss << std::hex;
	for (int c : digest) oss << c;
	return strdup(oss.str().c_str());
}