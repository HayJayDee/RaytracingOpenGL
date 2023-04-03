#include "inputhandler.h"

InputHandler::InputHandler(GLFWwindow* window) :window(window) {

}

void InputHandler::update() {

}

bool InputHandler::getKey(int key) {
    return glfwGetKey(window, key);
}