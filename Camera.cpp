#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVTY),
    Zoom(ZOOM)
{
    this->Position = position; //Vi tri
    this->WorldUp = up; //Huong len tren
    this->Yaw = yaw;    //Quay quanh truc y
    this->Pitch = pitch;    //Quay quanh truc x
    this->updateCameraVectors(); //Cap nhat lai goc nhin
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX,
    GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), //Huong phia truoc cua camera
    MovementSpeed(SPEED), //Toc do cua camera
    MouseSensitivity(SENSITIVTY),   //Do nhay cua chuot
    Zoom(ZOOM) //Thu phong
{
    this->Position = glm::vec3(posX, posY, posZ); 
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

//Ham nay tra ve goc nhin cua camera (mot ma tran)
glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::setPosition(glm::vec3 position)
{
    Position = position;
}
//Khi an cac phim ASDW, tin hieu se truyen vo ham nay de dieu khien camera di chuyen
void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
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

//Camera di chuyen theo con tro chuot
void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }
    this->updateCameraVectors();
}

//Lay tin hieu cua con lan de phong to
void Camera::processMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

//Cap nhat lai huong moi cua camera
void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);

    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

