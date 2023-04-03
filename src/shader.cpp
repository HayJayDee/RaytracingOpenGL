#include "shader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>


std::string readFile(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);

    if(!file.is_open()){
        std::cerr << "Could not find file " << fileName << std::endl;
        return "";
    }
    
    std::stringstream ss;
    ss << file.rdbuf();

    file.close();
    return ss.str();
}

Shader::Shader(const char* vertFilename, const char* fragFilename) {
    std::string vertSource = readFile(vertFilename);
    std::string fragSource = readFile(fragFilename);

    const char* vertSourceStr = vertSource.c_str();
    const char* fragSourceStr = fragSource.c_str();

    programID = glCreateProgram();
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &vertSourceStr, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    glShaderSource(fragment, 1, &fragSourceStr, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

uint32_t Shader::getID() {
    return programID;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			exit(1);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			exit(1);
		}
	}
}

void Shader::use() {
    glUseProgram(programID);
}

uint32_t Shader::getUniform(const char* name) {
    uint32_t location = glGetUniformLocation(programID, name);
    return location;
}

void Shader::loadVector(uint32_t location, glm::vec3 vec) {
    glUniform3fv(location, 1, &vec[0]);
}

void Shader::loadMatrix4(uint32_t location, glm::mat4 mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::loadFloat(uint32_t location, float value) {
    glUniform1f(location, value);
}
