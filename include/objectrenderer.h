#pragma once
#include <glad/glad.h>
#include "scene.h"
#include "shader.h"

class ObjectRenderer {
public:
    ObjectRenderer(Scene* scene, Shader* shader);
    ~ObjectRenderer();

    void render();
private:
    Scene* scene;
    Shader* shader;
    uint32_t sceneUbo;
};
