#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.hpp"

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float fov;
    bool grounded;

    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
public:
    static const glm::vec3 worldUp;
    enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

    Camera(glm::vec3 position, float yaw, float pitch, bool grounded = true) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(2.5f), mouseSensitivity(0.01f), fov(45.0f) {
        this->position = position;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }
    glm::mat4 getViewMatrixMan() {
        glm::mat4 basisChange = glm::mat4(1.0f), trans = glm::mat4(1.0f);
        glm::vec3 tright = glm::normalize(glm::cross(-front, worldUp));
        glm::vec3 tup = glm::normalize(glm::cross(right, -front));
        setMatrixRow(basisChange, tright, 0);
        setMatrixRow(basisChange, tup, 1);
        setMatrixRow(basisChange, -front, 2);
        setMatrixCol(trans, -position, 3);
        return basisChange * trans;
    }
    float getFov() { return fov; }

    void processKeyboard(Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD) {
            position += (grounded ? glm::normalize(glm::vec3(front.x, 0, front.z)) : front) * velocity;
        } else if (direction == BACKWARD) {
            position -= (grounded ? glm::normalize(glm::vec3(front.x, 0, front.z)) : front) * velocity;
        } else if (direction == LEFT) {
            position -= right * velocity;
        } else position += right * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        updateCameraVectors();
    }

    float processMouseScroll(float yoffset) {
        fov -= yoffset;
        if (fov < 1.0f) fov = 1.0f;
        if (fov > 45.0f) fov = 45.0f;
        return fov;
    }
};

const glm::vec3 Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

#endif