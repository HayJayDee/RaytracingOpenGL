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
#include <glm/gtc/type_ptr.hpp>
#include "scene.h"
#include "sceneobject.h"
#include "objectrenderer.h"
#include "error.h"

static float aspect;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    aspect = (float)width/(float)height;
}

void errorCallback(int code, const char* msg) {
    std::cout << msg << std::endl;
}

void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    std::cout << message << std::endl;
}

int main(int argc, char** argv){
    glfwInit();
    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Raytracing", nullptr, nullptr);
    if(window == nullptr) {
        std::cerr << "Could not create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    aspect = 800.0f/600.0f;
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Could not load GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    InputHandler handler(window);

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
    GL_ERROR(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    glBindVertexArray(0);
    
    SphereObject sphere0({0.0f,0.0f,-2.0f}, {1.0f,0.0f,0.0f}, 0.5f);
    SphereObject sphere1({0.0f,1.0f,-1.0f}, {0.0f,1.0f,0.0f}, 0.25f);
    SphereObject sphere2({1.0f,1.0f,0.0f}, {0.0f,0.0f,1.0f}, 1.0f);

    Scene scene;
    scene.addSceneObject(&sphere0);
    scene.addSceneObject(&sphere1);
    scene.addSceneObject(&sphere2);

    Shader shader("../shaders/shader.vert", "../shaders/shader.frag");

    ObjectRenderer renderer(&scene, &shader);

    Camera cam(&handler, glm::vec3(0.0,0.0,0.0));

    uint32_t locationAspect = shader.getUniform("aspectRatio");
    uint32_t locationCamPos = shader.getUniform("camPos");
    uint32_t locationView = shader.getUniform("viewMat");
    
    glClearColor(1.0, 0.0, 0.0, 1.0);
    double currTime = glfwGetTime();
    double delta = 0;
    double elapsed = 0;
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        cam.update(delta);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        shader.loadFloat(locationAspect, aspect);
        shader.loadVector(locationCamPos, cam.getPos());
        shader.loadMatrix4(locationView, cam.getViewMat());
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

    glfwTerminate();
    return 0;
}
