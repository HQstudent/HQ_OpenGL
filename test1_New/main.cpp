#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <unordered_map>

#include <algorithm>
#include <time.h>
#include <functional>
#include <numeric>
#include <thread>
#include <future>
#include <mutex>  
#include <atomic>
#include <assert.h>
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// 不加这句会报错 LINK 2019 即无法解析外部符号
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"// 这里需要在C/C++的常规中的附加包含目录中添加.h所在的路径

#include "resource.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "vertexbufferlayout.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

using namespace std;

#pragma region 全局定义

// 类似于哈希表，判断按键是否按下
bool keys[1024];
// 当前帧遇上一帧的时间差
GLfloat deltaTime = 0.0f;
// 上一帧的时间
GLfloat lastFrame = 0.0f;
// 创建摄像头类
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// 鼠标的默认位置，用来记录鼠标移动差值
GLfloat lastX = 400, lastY = 300;
// 鼠标按下的标志位，按下为false，再松开会置true
bool firstMouse = true;

// 投影着色器的大小
const unsigned int WIDTH = 800, HEIGHT = 600;

// 光照摄像机的位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

#pragma endregion

#pragma region 回调函数

// 窗口大小自动调整函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 退出窗口函数
void processInput(GLFWwindow* window);
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// 摄像头控制
void do_movement();
// 鼠标移动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#pragma endregion

int main(void)
{
    #pragma region 三维环境初始化

    GLFWwindow* window;
    // glfw初始化
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 本行会取消窗口调整功能
    
    // 创建窗口
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // 创建GL上下文
    glfwMakeContextCurrent(window);

    // glad/glew初始化，这一部分不能放在上下文前面，会走不下去
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    #pragma endregion

    #pragma region 注册回调函数

    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    // 注册鼠标移动回调函数
    glfwSetCursorPosCallback(window, mouse_callback);
    // 注册鼠标滚轮回调函数
    glfwSetScrollCallback(window, scroll_callback);
    // 隐藏光标，并捕捉它
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 注册调整窗口大小的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 执行GL的深度测试，防止六边形面覆盖
    glEnable(GL_DEPTH_TEST);

    #pragma endregion    
    
    {
        #pragma region 设置VBO/VAO/纹理/着色器

        // 上面这个“{”作用域防止变量提前释放（我也不是很懂）,但是可以触发各类的析构函数
        
        // 顶点缓冲VBO
        float positions[] = {
            // ----- 位置 -----  -- 纹理坐标 --
             -0.5f, -0.5f, -0.5f,  
              0.5f, -0.5f, -0.5f,  
              0.5f,  0.5f, -0.5f,  
              0.5f,  0.5f, -0.5f,  
             -0.5f,  0.5f, -0.5f,  
             -0.5f, -0.5f, -0.5f,  
            
             -0.5f, -0.5f,  0.5f,  
              0.5f, -0.5f,  0.5f,  
              0.5f,  0.5f,  0.5f,  
              0.5f,  0.5f,  0.5f,  
             -0.5f,  0.5f,  0.5f,  
             -0.5f, -0.5f,  0.5f,  
            
             -0.5f,  0.5f,  0.5f,  
             -0.5f,  0.5f, -0.5f,  
             -0.5f, -0.5f, -0.5f,  
             -0.5f, -0.5f, -0.5f,  
             -0.5f, -0.5f,  0.5f,  
             -0.5f,  0.5f,  0.5f,  
            
              0.5f,  0.5f,  0.5f,  
              0.5f,  0.5f, -0.5f,  
              0.5f, -0.5f, -0.5f,  
              0.5f, -0.5f, -0.5f,  
              0.5f, -0.5f,  0.5f,  
              0.5f,  0.5f,  0.5f,  
            
             -0.5f, -0.5f, -0.5f,  
              0.5f, -0.5f, -0.5f,  
              0.5f, -0.5f,  0.5f,  
              0.5f, -0.5f,  0.5f,  
             -0.5f, -0.5f,  0.5f,  
             -0.5f, -0.5f, -0.5f,  
            
             -0.5f,  0.5f, -0.5f,  
              0.5f,  0.5f, -0.5f,  
              0.5f,  0.5f,  0.5f,  
              0.5f,  0.5f,  0.5f,  
             -0.5f,  0.5f,  0.5f,  
             -0.5f,  0.5f, -0.5f  
        };

        // 创建顶点数组VAO用来保管顶点缓冲VBO的数据格式
        VertexArray basicVAO;
        // 将VBO设置好格式，4*7代表大小，此处调大也可以
        VertexBuffer vb(positions, 36 * 3 * sizeof(float));
        // 辅助VAO以判断每一个顶点缓冲的分区节点（比如，232代表2个位置，3个颜色，2个纹理）
        VertexBufferLayout layout;
        // 模板全偏推入vector
        layout.Push<float>(3); 
        // VAO绑定VBO，这里自动会调用va.bind()和vb.bind()
        basicVAO.AddBuffer(vb, layout);

        // 创建灯光的新顶点缓冲数组
        VertexArray lightVAO;
        lightVAO.AddBuffer(vb, layout);
     
        // 创建着色器对象读取渲染文件
        Shader shaderBasic("Basic.shader"); 
        Shader shaderLight("Lightwww.shader");
        
        //解绑VAO、VBO、TU和着色器（对象创建时的构造函数就已经绑定过了）
        basicVAO.Unbind();
        lightVAO.Unbind();
        vb.UnBind();
        shaderBasic.Unbind();
        shaderLight.Unbind();

        #pragma endregion

        while (!glfwWindowShouldClose(window))
        {
            #pragma region 渲染环境初始化

            // 存储上一帧渲染的事件，该行是为了让程序在不同电脑上跑的速度都很一致
            GLfloat currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // 监听事件
            glfwPollEvents(); 
            do_movement();

            // 每次重复渲染之前清除深度缓冲区
            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            // 开始绑定各缓冲和数组        
            basicVAO.Bind();
            lightVAO.Bind();
            vb.Bind();

            #pragma endregion

            #pragma region 渲染摄像机矩阵

            // 注意！！这里必须先创建mat4，然后另起一行再赋值，否则会渲染失败！不知道为什么 
            // 创建模型矩阵
            glm::mat4 model;
            model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            // 获取观察矩阵
            glm::mat4 view;
            view = camera.GetViewMatrix();
            // 定义投影矩阵
            glm::mat4 projection;
            projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

            // 开启着色器，该行其实是UseProgram()
            shaderBasic.Bind();
            //// 通过Uniform变量修改光照
            shaderBasic.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
            shaderBasic.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
            //// 通过Uniform变量修改模型
            shaderBasic.SetUniformMat4f("model", model);
            shaderBasic.SetUniformMat4f("view", view);
            shaderBasic.SetUniformMat4f("projection", projection);
            //// 绘图
            glDrawArrays(GL_TRIANGLES, 0, 36);


            shaderLight.Bind();
            model = glm::mat4();
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));

            shaderLight.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
            shaderLight.SetUniformMat4f("model", model);
            shaderLight.SetUniformMat4f("view", view);
            shaderLight.SetUniformMat4f("projection", projection);
            // 绘图 
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glfwSwapBuffers(window);

            #pragma endregion
        }
    }

    glfwTerminate();
    return 0;
}

#pragma region 回调函数

// 窗口大小自动调整函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 退出窗口函数
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)   //如果按下ESC键
        glfwSetWindowShouldClose(window, true);   //将WindowShouldClose属性改为true关闭窗口
}

// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

// 摄像头控制
void do_movement()
{
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// 鼠标移动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    GLfloat xoffset = (float)(xpos - lastX);
    GLfloat yoffset = (float)(lastY - ypos);  // Reversed since y-coordinates go from bottom to left

    lastX = (float)xpos;
    lastY = (float)ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// 鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

#pragma endregion