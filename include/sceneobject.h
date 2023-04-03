#pragma once
#include <glm/glm.hpp>

class SceneObject {
public:
    SceneObject(glm::vec3 pos, glm::vec3 color);
    virtual ~SceneObject();

    virtual void update(float dt)=0;

    glm::vec3 getPosition();
    glm::vec3 getColor();
    virtual void* makeLayout(size_t* size) = 0;
protected:
    glm::vec3 pos;
    glm::vec3 color;
};

class SphereObject : public SceneObject {
public:
    SphereObject(glm::vec3 pos, glm::vec3 color, float radius);
    ~SphereObject();

    void update(float dt) override;
    void* makeLayout(size_t* size) override;

    float getRadius();
private:
    float radius;
};
