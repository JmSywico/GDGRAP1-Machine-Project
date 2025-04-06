#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    virtual glm::mat4 GetViewMatrix() = 0;
    virtual glm::mat4 GetProjectionMatrix(float aspectRatio) = 0;
    virtual ~Camera() {}
};

class PerspectiveCamera : public Camera {
public:
    glm::vec3 position;
    float yaw, pitch;
    float fov;
    float nearPlane, farPlane;
    float radius;

    PerspectiveCamera(glm::vec3 pos, float fov, float nearP, float farP);

    void UpdateCameraPosition(const glm::vec3& carPosition, float carYaw);

    glm::mat4 GetViewMatrix() override;

    glm::mat4 GetProjectionMatrix(float aspectRatio) override;
};

class ThirdPersonCamera : public Camera {
public:
    glm::vec3 position;
    float yaw, pitch;
    float fov;
    float nearPlane, farPlane;
    float radius;
    glm::vec3 carPosition;

    ThirdPersonCamera(glm::vec3 pos, float fov, float nearP, float farP);

    void UpdateCameraPosition(const glm::vec3& carPosition, float carYaw);

    glm::mat4 GetViewMatrix() override;

    glm::mat4 GetProjectionMatrix(float aspectRatio) override;
};

#endif