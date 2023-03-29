#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vertexarray.h"
#include "indexbuffer.h"

#define ASSERT(x) if(!(x))__debugbreak()
// 下面几行的\加空格立刻报错，会认为不是转义符
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);