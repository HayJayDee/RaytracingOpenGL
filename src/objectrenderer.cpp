#include "objectrenderer.h"
#include <iostream>
#include "error.h"

#define SCENE_INDEX 0

ObjectRenderer::ObjectRenderer(Scene* scene, Shader* shader) :scene(scene),shader(shader) {

    uint32_t sceneIndex = glGetUniformBlockIndex(shader->getID(), "Scene");
    glUniformBlockBinding(shader->getID(), sceneIndex, SCENE_INDEX);

    std::vector<std::pair<void*, size_t>> layouts;

    uint32_t sceneSize = 0;
    for(auto& obj : *scene->getSceneObjects()){
        size_t size;
        void* layout = obj->makeLayout(&size);
        layouts.push_back({layout, size});
        sceneSize += size;
    }

    glGenBuffers(1, &sceneUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, sceneUbo);
    glBufferData(GL_UNIFORM_BUFFER, sceneSize, NULL, GL_STATIC_DRAW);
    GL_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, SCENE_INDEX, sceneUbo));

    size_t offset = 0;
    for(auto& layout : layouts){
        std::cout << "Buffer SubData at offset " << offset << " size: " << layout.second << " and datapointer " << layout.first << std::endl;
        glBufferSubData(GL_UNIFORM_BUFFER, offset, layout.second, layout.first);
        offset += layout.second;
        free(layout.first);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ObjectRenderer::~ObjectRenderer() {

}

void ObjectRenderer::render() {
    shader->use();
}
