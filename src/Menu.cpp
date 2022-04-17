#include "Menu.h"

Menu::Menu()
{
    glHelper::getInstance().glInit();
}


void Menu::update()
{
    glClearColor(.5, .5, .5, 1);
    ImGui::SetNextWindowPos(ImVec2(0, 200));
    ImGui::SetNextWindowSize(ImVec2(280, 295));
    ImGui::Begin("saves", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Saves");
    ImGui::BeginChild("list", ImVec2(264, 200), true);
    for (int n = 0; n < 50; n++) {
        if (ImGui::Button("Select", ImVec2(250, 50)));
        if (ImGui::IsItemHovered())
        {
            ImGui::SetNextWindowSize(ImVec2(256, 240));
            ImGui::BeginTooltip();
            ImGui::Text("preview");
            ImGui::EndTooltip();
        }
    }
    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0.0f, 3.0f));
    if (ImGui::Button("Load save", ImVec2(128, 45)));
    ImGui::SameLine();
    ImGui::BeginGroup();
    if (ImGui::Button("Delete save", ImVec2(128, 20)));
    if (ImGui::Button("Rename save", ImVec2(128, 20)));
    ImGui::EndGroup();

    ImGui::End();
}
