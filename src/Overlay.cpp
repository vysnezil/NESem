#include "Overlay.h"

void Overlay::init()
{
    AddDefaultFont(13);
    font = AddDefaultFont(64);
}

void Overlay::update()
{
	if (frames) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("overlay", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
        ImGui::PushFont(font);
        ImGui::Text(text);
        ImGui::PopFont();
        ImGui::End();

        frames--;
	}
}

void Overlay::begin(char* text, int seconds)
{
	this->text = text;
	this->frames = seconds * 60;
}

ImFont* Overlay::AddDefaultFont(float pixel_size)
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = pixel_size;
    config.OversampleH = config.OversampleV = 1;
    config.PixelSnapH = true;
    ImFont* font = io.Fonts->AddFontDefault(&config);
    return font;
}