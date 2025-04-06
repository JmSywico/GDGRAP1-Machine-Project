#include "Camera.h"


PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, float fov, float nearP, float farP)
    : position(pos), fov(fov), nearPlane(nearP), farPlane(farP), yaw(-90.0f), pitch(0.0f), radius(10.0f) {}

void PerspectiveCamera::UpdateCameraPosition(const glm::vec3& carPosition, float carYaw) {
    float offsetDistance = 10.0f; 
    glm::vec3 cameraOffset = glm::vec3(0.0f, 2.0f, offsetDistance);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(carYaw), glm::vec3(0.0f, 1.0f, 0.0f));

    position = carPosition + glm::vec3(rotationMatrix * glm::vec4(cameraOffset, 1.0f));

    yaw = carYaw;

}

glm::mat4 PerspectiveCamera::GetViewMatrix() {
    glm::vec3 target = position - glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)); 
    return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 pos, float fov, float nearP, float farP)
    : position(pos), fov(fov), nearPlane(nearP), farPlane(farP), yaw(-90.0f), pitch(0.0f), radius(10.0f) {}

void ThirdPersonCamera::UpdateCameraPosition(const glm::vec3& carPosition, float carYaw) {
    this->carPosition = carPosition; // Store car position
    float offsetDistance = -10.0f;
    glm::vec3 cameraOffset = glm::vec3(0.0f, 3.0f, offsetDistance);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    position = carPosition + glm::vec3(rotationMatrix * glm::vec4(cameraOffset, 1.0f));
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() {
    glm::vec3 target = carPosition; // Always look at the car
    return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 ThirdPersonCamera::GetProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}