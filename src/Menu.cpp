#include "Menu.h"
#include "SaveManager.h"

Menu::Menu()
{
    glHelper::getInstance().glInit();
}

void Menu::update()
{
    ImGui::ShowDemoWindow();

    glClearColor(.5, .5, .5, 1);
    ImGui::SetNextWindowPos(ImVec2(0, 200));
    ImGui::SetNextWindowSize(ImVec2(280, 295));
    ImGui::Begin("saves", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Saves");
    
    if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 12 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int n = 0; n < saves->size(); n++)
        {
            const bool is_selected = (selected == n);
            if (ImGui::Selectable(saves->at(n).c_str(), is_selected, 0, ImVec2(235, 50))) {
                selected = n;
                selectedSave = saves->at(n).c_str();
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetNextWindowSize(ImVec2(256, 240));
                ImGui::BeginTooltip();
                ImGui::Text("preview");
                ImGui::EndTooltip();
            }
        }
        if (saves->size() == 0) ImGui::Text("There are no saves yet");
        ImGui::EndListBox();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::Dummy(ImVec2(0.0f, 3.0f));
    ImGui::BeginDisabled(saves->size() == 0);
    if (ImGui::Button("Load save", ImVec2(128, 45))) {
        SaveManager::getInstance().loadSave(selectedSave);
        show = false;
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Delete save", ImVec2(128, 20)));
    ImGui::PopStyleColor(3);
    if (ImGui::Button("Rename save", ImVec2(128, 20)));
    ImGui::EndDisabled();
    ImGui::EndGroup();
    ImGui::PopStyleVar();


    Logger::getInstance().updatePopup();
    ImGui::End();
}
