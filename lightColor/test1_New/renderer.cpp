#include "renderer.h"
#include <iostream>

using namespace std;

void GLClearError()
{
    /* 循环获取错误(即清除) */
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << endl;
        return false;
    }
    return true;
}