#include "glHelper.h"
#include <string>
#include <vector>

class Menu
{
public:
	Menu();
    void update();
	int selected = 0;
	std::vector<std::string>* saves = new std::vector<std::string>();
	std::string selectedSave;
	bool show = true;
};
