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
// �������ᱨ�� LINK 2019 ���޷������ⲿ����
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"// ������Ҫ��C/C++�ĳ����еĸ��Ӱ���Ŀ¼�����.h���ڵ�·��

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

#pragma region ȫ�ֶ���

// �����ڹ�ϣ���жϰ����Ƿ���
bool keys[1024];
// ��ǰ֡����һ֡��ʱ���
GLfloat deltaTime = 0.0f;
// ��һ֡��ʱ��
GLfloat lastFrame = 0.0f;
// ��������ͷ��
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// ����Ĭ��λ�ã�������¼����ƶ���ֵ
GLfloat lastX = 400, lastY = 300;
// ��갴�µı�־λ������Ϊfalse�����ɿ�����true
bool firstMouse = true;

// ͶӰ��ɫ���Ĵ�С
const unsigned int WIDTH = 800, HEIGHT = 600;

// �����������λ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

#pragma endregion

#pragma region �ص�����

// ���ڴ�С�Զ���������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// �˳����ں���
void processInput(GLFWwindow* window);
// ���̻ص�����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// ����ͷ����
void do_movement();
// ����ƶ��ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// �����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#pragma endregion

int main(void)
{
    #pragma region ��ά������ʼ��

    GLFWwindow* window;
    // glfw��ʼ��
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // ���л�ȡ�����ڵ�������
    
    // ��������
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // ����GL������
    glfwMakeContextCurrent(window);

    // glad/glew��ʼ������һ���ֲ��ܷ���������ǰ�棬���߲���ȥ
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    #pragma endregion

    #pragma region ע��ص�����

    // ע����̻ص�����
    glfwSetKeyCallback(window, key_callback);
    // ע������ƶ��ص�����
    glfwSetCursorPosCallback(window, mouse_callback);
    // ע�������ֻص�����
    glfwSetScrollCallback(window, scroll_callback);
    // ���ع�꣬����׽��
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ע��������ڴ�С�Ļص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ִ��GL����Ȳ��ԣ���ֹ�������渲��
    glEnable(GL_DEPTH_TEST);

    #pragma endregion    
    
    {
        #pragma region ����VBO/VAO/����/��ɫ��

        // ���������{���������ֹ������ǰ�ͷţ���Ҳ���Ǻܶ���,���ǿ��Դ����������������
        
        // ���㻺��VBO
        float positions[] = {
            // ----- λ�� -----  -- �������� --
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

        // ������������VAO�������ܶ��㻺��VBO�����ݸ�ʽ
        VertexArray basicVAO;
        // ��VBO���úø�ʽ��4*7�����С���˴�����Ҳ����
        VertexBuffer vb(positions, 36 * 3 * sizeof(float));
        // ����VAO���ж�ÿһ�����㻺��ķ����ڵ㣨���磬232����2��λ�ã�3����ɫ��2������
        VertexBufferLayout layout;
        // ģ��ȫƫ����vector
        layout.Push<float>(3); 
        // VAO��VBO�������Զ������va.bind()��vb.bind()
        basicVAO.AddBuffer(vb, layout);

        // �����ƹ���¶��㻺������
        VertexArray lightVAO;
        lightVAO.AddBuffer(vb, layout);
     
        // ������ɫ�������ȡ��Ⱦ�ļ�
        Shader shaderBasic("Basic.shader"); 
        Shader shaderLight("Lightwww.shader");
        
        //���VAO��VBO��TU����ɫ�������󴴽�ʱ�Ĺ��캯�����Ѿ��󶨹��ˣ�
        basicVAO.Unbind();
        lightVAO.Unbind();
        vb.UnBind();
        shaderBasic.Unbind();
        shaderLight.Unbind();

        #pragma endregion

        while (!glfwWindowShouldClose(window))
        {
            #pragma region ��Ⱦ������ʼ��

            // �洢��һ֡��Ⱦ���¼���������Ϊ���ó����ڲ�ͬ�������ܵ��ٶȶ���һ��
            GLfloat currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // �����¼�
            glfwPollEvents(); 
            do_movement();

            // ÿ���ظ���Ⱦ֮ǰ�����Ȼ�����
            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            // ��ʼ�󶨸����������        
            basicVAO.Bind();
            lightVAO.Bind();
            vb.Bind();

            #pragma endregion

            #pragma region ��Ⱦ���������

            // ע�⣡����������ȴ���mat4��Ȼ������һ���ٸ�ֵ���������Ⱦʧ�ܣ���֪��Ϊʲô 
            // ����ģ�;���
            glm::mat4 model;
            model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            // ��ȡ�۲����
            glm::mat4 view;
            view = camera.GetViewMatrix();
            // ����ͶӰ����
            glm::mat4 projection;
            projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

            // ������ɫ����������ʵ��UseProgram()
            shaderBasic.Bind();
            //// ͨ��Uniform�����޸Ĺ���
            shaderBasic.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
            shaderBasic.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
            //// ͨ��Uniform�����޸�ģ��
            shaderBasic.SetUniformMat4f("model", model);
            shaderBasic.SetUniformMat4f("view", view);
            shaderBasic.SetUniformMat4f("projection", projection);
            //// ��ͼ
            glDrawArrays(GL_TRIANGLES, 0, 36);


            shaderLight.Bind();
            model = glm::mat4();
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));

            shaderLight.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
            shaderLight.SetUniformMat4f("model", model);
            shaderLight.SetUniformMat4f("view", view);
            shaderLight.SetUniformMat4f("projection", projection);
            // ��ͼ 
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glfwSwapBuffers(window);

            #pragma endregion
        }
    }

    glfwTerminate();
    return 0;
}

#pragma region �ص�����

// ���ڴ�С�Զ���������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// �˳����ں���
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)   //�������ESC��
        glfwSetWindowShouldClose(window, true);   //��WindowShouldClose���Ը�Ϊtrue�رմ���
}

// ���̻ص�����
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

// ����ͷ����
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

// ����ƶ��ص�����
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

// �����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

#pragma endregion