#include "Player.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

// Player constructor initializes the player's model and starting position
Player::Player(Model model, glm::vec3 initialPosition)
    : model(model), position(initialPosition), velocity(0.0f), rotationY(0.0f),
    acceleration(0.1f), deceleration(1.0f) {}

// Player destructor
Player::~Player() {}

// Updates the player's position and velocity based on the elapsed time (Basic Car Physics)
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

// Draws the player's model to the screen
void Player::Draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view) {
    glm::mat4 carTransform = glm::mat4(1.0f);
    carTransform = glm::translate(carTransform, position);
    carTransform = glm::rotate(carTransform, glm::radians(rotationY), glm::vec3(0, 1, 0));
    glm::mat4 carMVP = projection * view * carTransform;
    model.Draw(shaderProgram, carMVP);
}

// Returns the current position of the player
glm::vec3 Player::GetPosition() const {
    return position;
}

// Returns the current rotation of the player around the Y-axis
float Player::GetRotationY() const {
    return rotationY;
}

// Sets the rotation of the player around the Y-axis
void Player::SetRotationY(float newRotationY) {
    rotationY = newRotationY;
}

// Returns the current velocity of the player
float Player::GetVelocity() const {
    return velocity;
}

// Sets the current velocity of the player
void Player::SetVelocity(float newVelocity) {
    velocity = newVelocity;
   
}
