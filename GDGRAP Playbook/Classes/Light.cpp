#include "Light.h"

Light::Light(const glm::vec3& color, float intensity) : color(color), intensity(intensity) {}

//Point Light 
PointLight::PointLight(const glm::vec3& pos, const glm::vec3& color, float intensity)
    : Light(color, intensity), position(pos) {} 

glm::vec3 PointLight::GetPosition() const {
    return position;
}

//Directional Light 
DirectionalLight::DirectionalLight(const glm::vec3& dir, const glm::vec3& color, float intensity)
    : Light(color, intensity), direction(glm::normalize(dir)) {}

glm::vec3 DirectionalLight::GetPosition() const {
    return -direction;
}
