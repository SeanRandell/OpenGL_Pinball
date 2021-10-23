#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

// Default camera values
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float DEFAULT_MOVE_SPEED = 20.0f;          // units per second
const float DEFAULT_STRAFE_SPEED = 20.0f;          // units per second
const float DEFAULT_TURN_SPEED = 60.0f;         // degrees per second
const float DEFAULT_TILT_SPEED = 60.0f;         // degrees per second
const float ZOOM = 45.0f;
const float SENSITIVITY = 10.0f;

class Camera
{
public:
    // camera Attributes

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // euler Angles
    float yaw;
    float pitch;

    // camera options
    float movementSpeed;
    float strafeSpeed;
    float turnSpeed;
    float tiltSpeed;

    bool constrainPitch;
    bool mouseControls;

    // movement booleans
    bool moveLeft;
    bool moveRight;
    bool moveFoward;
    bool moveBackward;
    bool tiltUp;
    bool tiltDown;
    bool turnLeft;
    bool turnRight;
    bool firstMouse;

    //mouse bariables
    float MouseSensitivity;
    float lastX;
    float lastY;

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, int screenWidth, int screenHeight);
    ~Camera();

    glm::mat4 GetViewMatrix();
    void ProcessCameraMoving(float deltaTime);
    void ProcessCameraTurning(float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch = true);
private:
    void UpdateCameraVectors();
};