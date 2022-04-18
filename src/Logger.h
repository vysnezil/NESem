#pragma once
#include <iostream>
#include <iomanip>

#include <bitset>
#include <string>
#include <sstream>
#include <fstream>

#include "imgui.h"

class Logger {
public:
	//static std::ofstream file = std::ofstream("logs/latest.txt");
	char* popupMessage;

	inline void log() {
		std::cout << std::endl;
	}

	template<typename T, typename... Args> void log(T t, Args... args) {
		std::cout << t;
		log(args...);
	}


	void updatePopup() {
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(popupMessage);
			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}
	};

	void popupErr(char* err) {
		Logger::log(err);
		popupMessage = err;
		ImGui::OpenPopup("Error");
	}

	void logHex(uint16_t num);
	void logInstruction(uint8_t op, const char* name, uint16_t address, uint8_t x, uint8_t y, uint8_t a, uint8_t sp, uint8_t status, uint16_t pc);

	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

private:
	Logger() {};
};
