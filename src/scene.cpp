#include "scene.h"

Scene::Scene() {

}
Scene::~Scene() {

}

void Scene::addSceneObject(SceneObject* obj) {
    sceneObjects.push_back(obj);
}

std::vector<SceneObject*>* Scene::getSceneObjects() {
    return &sceneObjects;
}