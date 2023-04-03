#pragma once
#include <string>
#include <glm/glm.hpp>
#include <stdint.h>

class Shader {
public:
    Shader(const char* vertFilename, const char* fragFilename);
    ~Shader();

    void use();

    uint32_t getUniform(const char* name);

    uint32_t getID();

    void loadVector(uint32_t location, glm::vec3 vec);
    void loadMatrix4(uint32_t location, glm::mat4 mat);
    void loadFloat(uint32_t location, float value);
private:
    void checkCompileErrors(unsigned int shader, std::string type);
private:
    uint32_t programID;
};
