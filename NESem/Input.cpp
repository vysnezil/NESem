#include "Input.h"

uint8_t Input::controller = 0;

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key)
        {
        case GLFW_KEY_DOWN:
            controller |= 0x04;
            break;
        case GLFW_KEY_UP:
            controller |= 0x08;
            break;
        case GLFW_KEY_LEFT:
            controller |= 0x02;
            break;
        case GLFW_KEY_RIGHT:
            controller |= 0x01;
            break;
        case GLFW_KEY_X:
            controller |= 0x80;
            break;
        case GLFW_KEY_Z:
            controller |= 0x40;
            break;
        case GLFW_KEY_A:
            controller |= 0x20;
            break;
        case GLFW_KEY_S:
            controller |= 0x10;
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (key)
        {
        case GLFW_KEY_DOWN:
            controller &= ~0x04;
            break;
        case GLFW_KEY_UP:
            controller &= ~0x08;
            break;
        case GLFW_KEY_LEFT:
            controller &= ~0x02;
            break;
        case GLFW_KEY_RIGHT:
            controller &= ~0x01;
            break;
        case GLFW_KEY_X:
            controller &= ~0x80;
            break;
        case GLFW_KEY_Z:
            controller &= ~0x40;
            break;
        case GLFW_KEY_A:
            controller &= ~0x20;
            break;
        case GLFW_KEY_S:
            controller &= ~0x10;
            break;
        default:
            break;
        }
    }
}

Input::Input(GLFWwindow* window)
{
    glfwSetKeyCallback(window, this->key_callback);
}
