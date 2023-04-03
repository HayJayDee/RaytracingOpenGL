#include "camera.h"

Camera::Camera(InputHandler* handler, glm::vec3 pos) : handler(handler), pos(pos) {

}

Camera::~Camera() {

}

void Camera::update(float dt) {
    if(handler->getKey(GLFW_KEY_W)){
        pos.z -= 1.0f * dt;
    }
    if(handler->getKey(GLFW_KEY_A)){
        pos.x -= 1.0f * dt;
    }

    if(handler->getKey(GLFW_KEY_S)){
        pos.z += 1.0f * dt;
    }
    if(handler->getKey(GLFW_KEY_D)){
        pos.x += 1.0f * dt;
    }
    if(handler->getKey(GLFW_KEY_SPACE)){
        pos.y -= 1.0f * dt;
    }
    if(handler->getKey(GLFW_KEY_LEFT_SHIFT)){
        pos.y += 1.0f * dt;
    }
}

glm::vec3 Camera::getPos() {
    return pos;
}
