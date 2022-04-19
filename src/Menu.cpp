#include "Menu.h"
#include "SaveManager.h"
#include "Dialog.h"

Menu::Menu()
{
    glHelper::getInstance().glInit();
}

void Menu::update()
{
    glClearColor(.5, .5, .5, 1);
    ImGui::SetNextWindowPos(ImVec2(280, 0));
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
            if (is_selected) ImGui::SetItemDefaultFocus();
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
        SaveManager::getInstance().bus->loadCartridge(card);
        SaveManager::getInstance().loadSave(selectedSave);
        show = false;
        glHelper::getInstance().resizeWindow(false);
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Delete save", ImVec2(128, 20))) {
        saves->erase(saves->begin() + selected);
        Logger::getInstance().log("saves/" + (std::string)card->hash + "/" + selectedSave);
        if (saves->size() > 0) {
            selected = selected > 0 ? selected - 1 : 0;
            selectedSave = saves->at(selected).c_str();
        }
    };
    ImGui::PopStyleColor(3);
    if (ImGui::Button("Rename save", ImVec2(128, 20)));
    ImGui::EndDisabled();
    ImGui::EndGroup();
    ImGui::PopStyleVar();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(280, 295));
    ImGui::Begin("startW", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    if (ImGui::Button("select rom", ImVec2(128, 45))) {
        char* file = Dialog::getFile();
        if ((file != NULL) && (file[0] != '\0')) {
            card = new Cartridge(file);
            saves = SaveManager::getInstance().getSavesByRom((char*)card->hash);
        }
    };
    ImGui::BeginDisabled(card == nullptr);
    if (ImGui::Button("Start", ImVec2(128, 45))) {
        SaveManager::getInstance().bus->loadCartridge(card);
        SaveManager::getInstance().bus->reset();
        show = false;
        glHelper::getInstance().resizeWindow(false);
    }
    ImGui::EndDisabled();
    ImGui::PopStyleVar();

    if (card != nullptr) {
        ImGui::Text(card->name.c_str());
    }
    ImGui::End();


    Logger::getInstance().updatePopup();
    ImGui::End();
}
