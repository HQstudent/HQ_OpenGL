#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 默认摄像头参数
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
    // 摄影机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // 欧拉角
    GLfloat Yaw;
    GLfloat Pitch;
    // 摄像机设置
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // 带有矢量的构造函数，这里不能赋默认初值会报错：重定义
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat yaw = YAW, GLfloat pitch = PITCH);

    // 带有标量值的构造函数
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, 
        GLfloat yaw, GLfloat pitch);
  
    // 返回使用欧拉角和注视矩阵计算的视图矩阵
    glm::mat4 GetViewMatrix();

    // 处理从任何类似键盘的输入系统接收的输入，接受相机定义ENUM形式的输入参数（从窗口系统中提取）
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    // 处理从鼠标输入系统接收的输入，需要x和y方向上的偏移值。
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // 处理从鼠标滚轮事件接收的输入，只需要在垂直车轮轴上输入
    void ProcessMouseScroll(GLfloat yoffset);

private:
    // 根据相机的（更新的）欧拉角计算前矢量
    void updateCameraVectors();
};