#include "sceneobject.h"
#include <iostream>

SceneObject::SceneObject(glm::vec3 pos, glm::vec3 color):pos(pos),color(color) {

}

SceneObject::~SceneObject() {
}

glm::vec3 SceneObject::getPosition() {
    return pos;
}

glm::vec3 SceneObject::getColor() {
    return color;
}

SphereObject::SphereObject(glm::vec3 pos, glm::vec3 color, float radius) : SceneObject(pos, color),radius(radius) {
}

SphereObject::~SphereObject() {
}

void SphereObject::update(float dt) {

}

void* SphereObject::makeLayout(size_t* size) {
    struct SphereLayout{
        glm::vec4 layout_pos;
        glm::vec3 layout_color;
        float layout_radius;
    }__attribute((packed)) layout = {
        .layout_pos = glm::vec4(pos, 0),
        .layout_color = color,
        .layout_radius = radius,
    };
    *size = sizeof(SphereLayout);
    SphereLayout* ptr = new SphereLayout;
    *ptr = layout;
    return (void*)ptr;
}

float SphereObject::getRadius() {
    return radius;
}
