#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 color;
    float intensity;

    Light(const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f);

    virtual ~Light() {}

    virtual glm::vec3 GetPosition() const = 0;
};

class PointLight : public Light {
public:
    glm::vec3 position;

    PointLight(const glm::vec3& pos, const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f);

    glm::vec3 GetPosition() const override;
};

class DirectionalLight : public Light {
public:
    glm::vec3 direction;

    DirectionalLight(const glm::vec3& dir, const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f);

    glm::vec3 GetPosition() const override;
};

#endif
