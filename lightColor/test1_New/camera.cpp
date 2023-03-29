#include "camera.h"

// 带有矢量的构造函数
Camera::Camera(glm::vec3 position, 
    glm::vec3 up,
    GLfloat yaw, GLfloat pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    MovementSpeed(SPEED), 
    MouseSensitivity(SENSITIVTY), 
    Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

// 带有标量值的构造函数
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, 
    GLfloat upX, GLfloat upY, GLfloat upZ, 
    GLfloat yaw, GLfloat pitch) 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED), 
    MouseSensitivity(SENSITIVTY), 
    Zoom(ZOOM)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

// 返回使用欧拉角和注视矩阵计算的视图矩阵
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// 处理从任何类似键盘的输入系统接收的输入，接受相机定义ENUM形式的输入参数（从窗口系统中提取）
void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
}

// 处理从鼠标输入系统接收的输入，需要x和y方向上的偏移值。
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // 确保投球出界时，屏幕不会翻转
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // 使用更新的欧拉角更新前矢量、右矢量和上矢量
    this->updateCameraVectors();
}

// 处理从鼠标滚轮事件接收的输入，只需要在垂直车轮轴上输入
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

// 根据相机的（更新的）欧拉角计算前矢量
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // 同时重新计算向右和向上矢量
    // 规格化向量，因为向上或向下看得越多，向量的长度就越接近0，这会导致移动速度变慢。
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}