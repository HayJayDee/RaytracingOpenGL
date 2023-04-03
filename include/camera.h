#pragma once
#include <glm/glm.hpp>
#include "inputhandler.h"

class Camera {
public:
    Camera(InputHandler* handler, glm::vec3 pos);
    ~Camera();

    void update(float dt);

    glm::vec3 getPos();
private:
    glm::vec3 pos;
    InputHandler* handler;
};