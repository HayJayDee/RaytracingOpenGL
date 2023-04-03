#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"
#include "inputhandler.h"

static float aspect;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    aspect = (float)width/(float)height;
}

void checkCompileErrors(unsigned int shader, std::string type)
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

int main(int argc, char** argv){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Raytracing", nullptr, nullptr);
    if(window == nullptr) {
        std::cerr << "Could not create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    aspect = 800.0f/600.0f;
    //glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Could not load GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

	float quadVertices[] = 
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f
	};

    // Load Global Quad
    uint32_t vao;
    uint32_t vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    std::string vertexSource = readFile("../shaders/shader.vert");
    std::string fragmentSource = readFile("../shaders/shader.frag");

    const char* vs = vertexSource.c_str();
    const char* fs = fragmentSource.c_str();

    // Create Shaders
    uint32_t program = glCreateProgram();
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &vs, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    glShaderSource(fragment, 1, &fs, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    InputHandler handler(window);

    Camera cam(&handler, glm::vec3(0.0,0.0,0.0));

    uint32_t locationAspect = glGetUniformLocation(program, "aspectRatio");
    uint32_t locationCamPos = glGetUniformLocation(program, "camPos");
    
    glClearColor(1.0, 0.0, 0.0, 1.0);
    double currTime = glfwGetTime();
    double delta = 0;
    double elapsed = 0;
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        cam.update(delta);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        glUniform1f(locationAspect, aspect);
        glUniform3fv(locationCamPos, 1, &cam.getPos()[0]);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUseProgram(0);
        glBindVertexArray(0);    
        glfwSwapBuffers(window);

        delta = glfwGetTime() - currTime;
        currTime = glfwGetTime();
        elapsed += delta;
        if(elapsed >= 1){
            //std::cout << "FPS: " << (uint32_t)(1000/delta)<< std::endl;
            elapsed = 0;
        }
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}
