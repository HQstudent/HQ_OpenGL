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

// Ĭ������ͷ����
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
    // ��Ӱ������
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // ŷ����
    GLfloat Yaw;
    GLfloat Pitch;
    // ���������
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // ����ʸ���Ĺ��캯�������ﲻ�ܸ�Ĭ�ϳ�ֵ�ᱨ���ض���
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat yaw = YAW, GLfloat pitch = PITCH);

    // ���б���ֵ�Ĺ��캯��
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, 
        GLfloat yaw, GLfloat pitch);
  
    // ����ʹ��ŷ���Ǻ�ע�Ӿ���������ͼ����
    glm::mat4 GetViewMatrix();

    // ������κ����Ƽ��̵�����ϵͳ���յ����룬�����������ENUM��ʽ������������Ӵ���ϵͳ����ȡ��
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    // ������������ϵͳ���յ����룬��Ҫx��y�����ϵ�ƫ��ֵ��
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    // ������������¼����յ����룬ֻ��Ҫ�ڴ�ֱ������������
    void ProcessMouseScroll(GLfloat yoffset);

private:
    // ��������ģ����µģ�ŷ���Ǽ���ǰʸ��
    void updateCameraVectors();
};