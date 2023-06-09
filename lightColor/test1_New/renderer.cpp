#include "renderer.h"
#include <iostream>

using namespace std;

void GLClearError()
{
    /* ѭ����ȡ����(�����) */
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