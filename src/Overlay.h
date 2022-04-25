#include <imgui.h>

class Overlay
{
public:
	void init();
	void update();
	void begin(char* text, int seconds);

	ImFont* AddDefaultFont(float pixel_size);

	static Overlay& getInstance() {
		static Overlay instance;
		return instance;
	}

private:
	Overlay() {};
	int frames;
	char* text;
	ImFont* font;
};

