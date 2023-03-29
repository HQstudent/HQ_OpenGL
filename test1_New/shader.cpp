#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.h"
#include "renderer.h"

Shader::Shader(const std::string& filepath)
    :m_FilePath(filepath), m_RendererID(0)
{
    /* 从文件中解析着色器源码 */
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    std::cout << filepath <<":" << m_RendererID << std::endl;
    Bind();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, int count, int* value)
{
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3)); /* 设置对应的统一变量 */
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line, '\r')) {
        if (line.find("#shader") != std::string::npos) { /* 找到#shader标记 */
            if (line.find("vertex") != std::string::npos) { /* 顶点着色器标记 */
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) { /* 片段着色器标记 */
                type = ShaderType::FRAGMENT;
            }
        }
        // 这里需要加一个条件，否则编译器会认为type=-1时，ss[-1]无法推入数据，warning C6385
        else if (type != ShaderType::NONE) {
            ss[(int)type] << line << '\n';          
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id;
    /* 提升作用域 */
    GLCall(id = glCreateShader(type)); /* 创建对应类型的着色器 */
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr)); /* 设置着色器源码 */
    GLCall(glCompileShader(id)); /* 编译着色器 */

    /* 编译错误处理 */
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // 获取当前着色器编译状态
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // 获取日志长度
        char* msg = (char*)_malloca(length * sizeof(char)); /* Cherno这里采用的alloca, 根据IDE提示, 我这里改成了_malloca函数 */
        GLCall(glGetShaderInfoLog(id, length, &length, msg)); // 获取日志信息
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;
        GLCall(glDeleteShader(id)); // 删除着色器
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program;
    GLCall(program = glCreateProgram()); /* 创建程序 */
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* 将着色器附加到程序上 */
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program)); /* 链接程序 */
    GLCall(glValidateProgram(program)); /* 验证 */

    /* 删除着色器 */
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end()) {
        return m_UniformlocationCache[name];
    }
    int location;
    GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    }

    m_UniformlocationCache[name] = location;
    return location;
}