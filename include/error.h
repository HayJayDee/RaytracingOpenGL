#pragma once

#include <glad/glad.h>

#define GL_ERROR(stmt) stmt; {int err; while((err = glGetError()) != GL_NO_ERROR) {std::cout << err << std::endl;}}