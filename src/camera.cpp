#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera(InputHandler* handler, glm::vec3 pos) : handler(handler), pos(pos) {

}

Camera::~Camera() {

}

void Camera::update(float dt) {

    // TODO: Add "forward" and "right" vector
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
    if(handler->getKey(GLFW_KEY_Q)){
        rotation.y += 90.0f * dt;
    }
    if(handler->getKey(GLFW_KEY_E)){
        rotation.y -= 90.0f * dt;
    }
}

glm::vec3 Camera::getPos() {
    return pos;
}

glm::mat4 Camera::getViewMat() {
    glm::mat4 view(1.0f);
    view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    return view;
}