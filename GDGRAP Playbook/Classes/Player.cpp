// Player.cpp
#include "Player.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Player::Player(Model model, glm::vec3 initialPosition)
    : model(model), position(initialPosition), velocity(0.0f), rotationY(0.0f),
    acceleration(0.1f), deceleration(1.0f) {}

Player::~Player() {}

void Player::Update(float deltaTime) {
    float radians = glm::radians(rotationY);
    glm::vec3 direction(sin(radians), 0.0f, cos(radians));
    position += direction * velocity * deltaTime;

    if (velocity > 0.0f) {
        velocity -= deceleration * deltaTime;
        if (velocity < 0.0f) velocity = 0.0f;
    }
    else if (velocity < 0.0f) {
        velocity += deceleration * deltaTime;
        if (velocity > 0.0f) velocity = 0.0f;
    }
}

void Player::Draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view) {
    glm::mat4 carTransform = glm::mat4(1.0f);
    carTransform = glm::translate(carTransform, position);
    carTransform = glm::rotate(carTransform, glm::radians(rotationY), glm::vec3(0, 1, 0));
    glm::mat4 carMVP = projection * view * carTransform;
    model.Draw(shaderProgram, carMVP);
}

glm::vec3 Player::GetPosition() const {
    return position;
}

float Player::GetRotationY() const {
    return rotationY;
}

void Player::SetRotationY(float newRotationY) {
    rotationY = newRotationY;
}

float Player::GetVelocity() const {
    return velocity;
}

void Player::SetVelocity(float newVelocity) {
    velocity = newVelocity;
   
}