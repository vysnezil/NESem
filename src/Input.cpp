#include "Input.h"
#include "SaveManager.h"

uint8_t Input::controller = 0;
bool Input::singleStep = false;
bool Input::step = false;
bool Input::stepFrame = false;
bool Input::saveFlag = false;
bool Input::menuFlag = false;
bool Input::loadFlag = false;

int Input::recordKey = 0;

int Input::keys[8] = { GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_A, GLFW_KEY_S };

void Input::loadKeys() {
    json j;
    std::string path = "saves/config.json";
    if (std::filesystem::exists(path)) {
        std::ifstream i;
        i.open("saves/config.json");
        std::stringstream ss;
        ss << i.rdbuf();
        j = json::parse(ss.str());

        int y = j["down"];
        keys[1] = j["up"];
        keys[2] = j["left"];
        keys[3] = j["right"];
        keys[4] = j["a"];
        keys[5] = j["b"];
        keys[6] = j["select"];
        keys[7] = j["start"];
        Logger::getInstance().log("loaded keys");
    }
}

void Input::saveKeys() {
    json j = {
        {"down", keys[0]},
        {"up", keys[1]},
        {"left", keys[2]},
        {"right", keys[3]},
        {"a", keys[4]},
        {"b", keys[5]},
        {"select", keys[6]},
        {"start", keys[7]}
    };
    auto savesDir = std::filesystem::create_directory("saves");
    std::ofstream outfile("saves/config.json");
    outfile << j;
    outfile.close();
}

char* Input::getArrow(int code)
{
    switch (code)
    {
    case GLFW_KEY_DOWN: return "DOWN";
    case GLFW_KEY_UP: return "UP";
    case GLFW_KEY_LEFT: return "LEFT";
    case GLFW_KEY_RIGHT: return "RIGHT";
    case GLFW_KEY_SPACE: return "SPACE";
    }
    return "?";
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        bool contains = false;
        for (int i = 0; i < 8; i++) if (keys[i] == key) contains = true;
        if (recordKey) {
            if (key != GLFW_KEY_ESCAPE && !contains) {
                keys[recordKey - 1] = key;
                saveKeys();
            }
            recordKey = 0;
        }
        switch (key)
        {
        case GLFW_KEY_F8:
            saveFlag = true;
            break;
        case GLFW_KEY_F5:
            loadFlag = true;
            break;
        case GLFW_KEY_ESCAPE:
            menuFlag = true;
            break;
        case GLFW_KEY_SPACE:
            singleStep = false;
            Logger::getInstance().log("Single Stepping: ", singleStep ? "true" : "false");
            break;
        default:
            break;
        }



        if (key == keys[0]) controller |= 0x04;
        else if (key == keys[1]) controller |= 0x08;
        else if (key == keys[2]) controller |= 0x02;
        else if (key == keys[3]) controller |= 0x01;
        else if (key == keys[4]) controller |= 0x80;
        else if (key == keys[5]) controller |= 0x40;
        else if (key == keys[6]) controller |= 0x20;
        else if (key == keys[7]) controller |= 0x10;
        
    }
    else if (action == GLFW_RELEASE) {
        if (key == keys[0]) controller &= ~0x04;
        else if (key == keys[1]) controller &= ~0x08;
        else if (key == keys[2]) controller &= ~0x02;
        else if (key == keys[3]) controller &= ~0x01;
        else if (key == keys[4]) controller &= ~0x80;
        else if (key == keys[5]) controller &= ~0x40;
        else if (key == keys[6]) controller &= ~0x20;
        else if (key == keys[7]) controller &= ~0x10;
    }
}

Input::Input(GLFWwindow* window)
{
    glfwSetKeyCallback(window, this->key_callback);
}



