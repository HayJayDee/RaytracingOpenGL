#pragma once
#include <GLFW/glfw3.h>

class InputHandler {
public:
    InputHandler(GLFWwindow* window);

    void update();

    bool getKey(int key);
private:
    GLFWwindow* window;
};