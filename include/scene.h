#pragma once
#include <vector>
#include "sceneobject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addSceneObject(SceneObject* obj);

    std::vector<SceneObject*>* getSceneObjects();
private:
    std::vector<SceneObject*> sceneObjects;
};
