#include "camera.h"

// ����ʸ���Ĺ��캯��
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

// ���б���ֵ�Ĺ��캯��
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

// ����ʹ��ŷ���Ǻ�ע�Ӿ���������ͼ����
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// ������κ����Ƽ��̵�����ϵͳ���յ����룬�����������ENUM��ʽ������������Ӵ���ϵͳ����ȡ��
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

// ������������ϵͳ���յ����룬��Ҫx��y�����ϵ�ƫ��ֵ��
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // ȷ��Ͷ�����ʱ����Ļ���ᷭת
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // ʹ�ø��µ�ŷ���Ǹ���ǰʸ������ʸ������ʸ��
    this->updateCameraVectors();
}

// ������������¼����յ����룬ֻ��Ҫ�ڴ�ֱ������������
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

// ��������ģ����µģ�ŷ���Ǽ���ǰʸ��
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // ͬʱ���¼������Һ�����ʸ��
    // �����������Ϊ���ϻ����¿���Խ�࣬�����ĳ��Ⱦ�Խ�ӽ�0����ᵼ���ƶ��ٶȱ�����
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}