#include "Menu.h"
#include "SaveManager.h"
#include "Dialog.h"
#include "Input.h"

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
        }
        if (saves->size() == 0) ImGui::Text("There are no saves yet");
        ImGui::EndListBox();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::Dummy(ImVec2(0.0f, 3.0f));
    ImGui::BeginDisabled(saves->size() == 0);
    if (ImGui::Button("Load save", ImVec2(128, 45))) {
        SaveManager::getInstance().bus->loadCartridge(card.get());
        SaveManager::getInstance().loadSave(selectedSave);
        show = false;
        glHelper::getInstance().resizeWindow(false);
    }
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::TextWrapped("Do you really want to delete selected save?");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            selectedSave = "";
            saves->erase(saves->begin() + selected);
            Logger::getInstance().log("saves/" + (std::string)card->hash + "/" + selectedSave);
            std::filesystem::remove("saves/" + (std::string)card->hash + "/" + selectedSave + ".json");
            if (saves->size() > 0) {
                selected = selected > 0 ? selected - 1 : 0;
                selectedSave = saves->at(selected).c_str();
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Delete save", ImVec2(128, 20))) {
        ImGui::OpenPopup("Delete");
    };
    ImGui::PopStyleColor(3);

    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(265, 85));
    if (ImGui::BeginPopupModal("Rename", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        static char newName[128] = "";
        ImGui::PushItemWidth(-1);
        ImGui::InputText("newname", newName, IM_ARRAYSIZE(newName));
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(1, 2));
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            if ((newName != NULL) && (newName[0] != '\0')) {
                SaveManager::getInstance().renameSave(selectedSave, newName);
                saves = SaveManager::getInstance().getSavesByRom((char*)card->hash);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
    

    if (ImGui::Button("Rename save", ImVec2(128, 20))) {
        ImGui::OpenPopup("Rename"); 
    };
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
            this->card = std::make_shared<Cartridge>(file);
            saves = SaveManager::getInstance().getSavesByRom((char*)card->hash);
            selected = 0;
            if (saves->size() > 0) selectedSave = saves->at(0);
        }
    };
    if (card == nullptr) ImGui::BeginDisabled(true);
    else ImGui::BeginDisabled(!card->success);
    ImGui::SameLine();
    if (ImGui::Button("Start", ImVec2(128, 45))) {
        SaveManager::getInstance().bus->loadCartridge(card.get());
        SaveManager::getInstance().bus->reset();
        show = false;
        glHelper::getInstance().resizeWindow(false);
    }
    ImGui::EndDisabled();
    ImGui::PopStyleVar();

    if (card != nullptr) {
        if (card->success) {
            std::filesystem::path p = card->name.c_str();
            ImGui::AlignTextToFramePadding();
            ImGui::TextWrapped(((std::string)"selected: " + p.filename().string()).c_str());
            ImGui::Dummy(ImVec2(1, 87));
        }
        else {
            ImGui::AlignTextToFramePadding();
            ImGui::TextWrapped("Selected NES file is not valid!");
            ImGui::Dummy(ImVec2(1, 87));
        }
    }
    else ImGui::Dummy(ImVec2(1, 110));
    ImGui::Text("Controls: ");
    ImGui::Dummy(ImVec2(45, 45));
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 2 ? "<?>" : glfwGetKeyName(Input::keys[1], 0) != NULL ? glfwGetKeyName(Input::keys[1], 0) : Input::getArrow(Input::keys[1]), ImVec2(45, 45))) Input::recordKey = 2;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("up");
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(45, 45));
    ImGui::SameLine();
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 7 ? "<?>" : glfwGetKeyName(Input::keys[6], 0) != NULL ? glfwGetKeyName(Input::keys[6], 0) : Input::getArrow(Input::keys[6]), ImVec2(45, 45))) Input::recordKey = 7;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("select");
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 8 ? "<?>" : glfwGetKeyName(Input::keys[7], 0) != NULL ? glfwGetKeyName(Input::keys[7], 0) : Input::getArrow(Input::keys[7]), ImVec2(45, 45))) Input::recordKey = 8;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("start");
    ImGui::Dummy(ImVec2(1, 1));
    if (ImGui::Button(Input::recordKey == 3 ? "<?>" : glfwGetKeyName(Input::keys[2], 0) != NULL ? glfwGetKeyName(Input::keys[2], 0) : Input::getArrow(Input::keys[2]), ImVec2(45, 45))) Input::recordKey = 3;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("left");
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 1 ? "<?>" : glfwGetKeyName(Input::keys[0], 0) != NULL ? glfwGetKeyName(Input::keys[0], 0) : Input::getArrow(Input::keys[0]), ImVec2(45, 45))) Input::recordKey = 1;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("down");
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 4 ? "<?>" : glfwGetKeyName(Input::keys[3], 0) != NULL ? glfwGetKeyName(Input::keys[3], 0) : Input::getArrow(Input::keys[3]), ImVec2(45, 45))) Input::recordKey = 4;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("right");
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 6 ? "<?>" : glfwGetKeyName(Input::keys[5], 0) != NULL ? glfwGetKeyName(Input::keys[5], 0) : Input::getArrow(Input::keys[5]), ImVec2(45, 45))) Input::recordKey = 6;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("B");
    ImGui::SameLine();
    if (ImGui::Button(Input::recordKey == 5? "<?>" : glfwGetKeyName(Input::keys[4], 0) != NULL ? glfwGetKeyName(Input::keys[4], 0) : Input::getArrow(Input::keys[4]), ImVec2(45, 45))) Input::recordKey = 5;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("A");

    ImGui::End();

    

    Logger::getInstance().updatePopup();
    ImGui::End();
}
