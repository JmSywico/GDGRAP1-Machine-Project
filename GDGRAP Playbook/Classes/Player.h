// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"

class Player {
public:
    Player(Model model, glm::vec3 initialPosition);
    ~Player();

    void Update(float deltaTime);
    void Draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view);

    glm::vec3 GetPosition() const;
    float GetRotationY() const;
    void SetRotationY(float rotationY);
    float GetVelocity() const;
    void SetVelocity(float velocity);

private:
    Model model;
    glm::vec3 position;
    float velocity;
    float rotationY;
    float acceleration;
    float deceleration;
};

#endif