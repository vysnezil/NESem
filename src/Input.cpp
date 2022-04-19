#include "Input.h"

uint8_t Input::controller = 0;
bool Input::singleStep = false;
bool Input::step = false;
bool Input::stepFrame = false;
bool Input::saveFlag = false;
bool Input::menuFlag = false;

int Input::recordKey = 0;

int Input::keys[8] = { GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_A, GLFW_KEY_S };

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
            if (key != GLFW_KEY_ESCAPE && !contains) keys[recordKey-1] = key;
            recordKey = 0;
        }
        switch (key)
        {
        case GLFW_KEY_F8:
            saveFlag = true;
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



