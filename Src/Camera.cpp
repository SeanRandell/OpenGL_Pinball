#include "Camera.h"

Camera::Camera(
    float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, int screenWidth, int screenHeight)
{
    position = glm::vec3(posX, posY, posZ);
    worldUp = glm::vec3(upX, upY, upZ);
    this->front = DEFAULT_FRONT;
    this->yaw = yaw;
    this->pitch = pitch;

    moveLeft = false;
    moveRight = false;
    moveFoward = false;
    moveBackward = false;
    tiltUp = false;
    tiltDown = false;
    turnLeft = false;
    turnRight = false;
    constrainPitch = true;
    firstMouse = true;

    MouseSensitivity = SENSITIVITY;
    movementSpeed = DEFAULT_MOVE_SPEED;
    strafeSpeed= DEFAULT_STRAFE_SPEED;
    turnSpeed = DEFAULT_TURN_SPEED;
    tiltSpeed = DEFAULT_TILT_SPEED;

    UpdateCameraVectors();
}

Camera::~Camera()
{

}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    // also re-calculate the Right and Up vector

    // normalize the vectors, because their length gets closer 
    // to 0 the more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(this->front, worldUp));

    up = glm::normalize(glm::cross(right, this->front));
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity * deltaTime;
    yoffset *= MouseSensitivity * deltaTime;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessCameraMoving(float deltaTime)
{
    //float velocity = MovementSpeed * deltaTime;
    if (moveFoward) {
        position += front * movementSpeed * deltaTime;

        fprintf(stderr, "position: x = %.2f, y = %.2f, z = %.2f, deltaTime = %f\n", position.x, position.y, position.z, deltaTime);
    }
    if (moveBackward) {
        position -= front * movementSpeed * deltaTime;
        //fprintf(stderr, "left triggered\n");
    }
    if (moveLeft) {
        position -= right * strafeSpeed * deltaTime;

    }
    if (moveRight) {
        position += right * strafeSpeed * deltaTime;
        //fprintf(stderr, "left triggered\n");
    }
}

void Camera::ProcessCameraTurning(float deltaTime) {
    if (turnLeft) {
        yaw -= turnSpeed * deltaTime;
    }
    if (turnRight) {
        yaw += turnSpeed * deltaTime;
    }
    if (tiltUp) {
        pitch += tiltSpeed * deltaTime;
    }
    if (tiltDown) {
        pitch -= tiltSpeed * deltaTime;
    }

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    if (yaw >= 360) {
        yaw = 0;
    }
    if (yaw <= -360) {
        yaw = 0;
    }

    UpdateCameraVectors();
}