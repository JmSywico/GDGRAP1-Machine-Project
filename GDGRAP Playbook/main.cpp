#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include "Classes/Model.h"
#include "Classes/Camera.h"
#include "Classes/Light.h"
#include "Classes/Skybox.h"
#include "Classes/Player.h"

GLuint shaderProgram;
GLuint lightShaderProgram;
Skybox* skybox;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include "Classes/Model.h"
#include "Classes/Camera.h"
#include "Classes/Light.h"
#include "Classes/Skybox.h"
#include "Classes/Player.h"

GLuint shaderProgram;
GLuint lightShaderProgram;
Skybox* skybox;

// Function to compile the vertex and fragment shaders
void CompileShaders() {
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* vert = vertS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vert, nullptr);
    glCompileShader(vertexShader);

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* frag = fragS.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag, nullptr);
    glCompileShader(fragmentShader);

    std::fstream lightFragSrc("Shaders/light.frag");
    std::stringstream lightFragBuff;
    lightFragBuff << lightFragSrc.rdbuf();
    std::string lightFragS = lightFragBuff.str();
    const char* lightFrag = lightFragS.c_str();

    GLuint lightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(lightFragmentShader, 1, &lightFrag, nullptr);
    glCompileShader(lightFragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    lightShaderProgram = glCreateProgram();
    glAttachShader(lightShaderProgram, vertexShader);
    glAttachShader(lightShaderProgram, lightFragmentShader);
    glLinkProgram(lightShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightFragmentShader);
}

// Initialize perspective camera and third person camera
PerspectiveCamera perspectiveCamera(glm::vec3(0.0f, 0.0f, 10.0f), 45.0f, 0.1f, 100.0f);
ThirdPersonCamera thirdPersonCamera(glm::vec3(0.0f, 0.0f, 20.0f), 5.0f, 0.1f, 200.0f);

// Sets the initial active camera which is our perspective camera
Camera* activeCamera = &perspectiveCamera;

// Directional light properties
glm::vec3 directionalLightDir = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 dirLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
float dirLightIntensity = 1.0f;

// Variables for object rotations
float rotationX = 0.0f;
float rotationY = 0.2f;
float rotationZ = 0.0f;

// Variables to control car movement
bool isCarsMoving = false;
glm::vec3 carPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float carVelocity = 0.0f;
float carRotationY = 0.0f;
float acceleration = 1.0f;
float deceleration = 1.0f;
float car2Velocity = 40.0f; 
float car3Velocity = 20.0f; 

// Flags and variables for game state tracking
bool printTimeOnce = false;
bool isCar1Finished = false;
bool isCar2Finished = false;
bool isCar3Finished = false;

//Mouse input handling
bool firstMouse = true;
float lastX = 400, lastY = 300;

//Key input functions
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //Camera perspective switching
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        if (activeCamera == &perspectiveCamera) {
            activeCamera = &thirdPersonCamera;
        }
        else {
            activeCamera = &perspectiveCamera;
        }
    }
    //Ghost car key
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        isCarsMoving = !isCarsMoving;
    }
    //Player movement
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        float rotationSpeed = 2.0f;
        float turnBoost = 1.5f;

        if (key == GLFW_KEY_A) {
            carRotationY += rotationSpeed;
        }
        if (key == GLFW_KEY_D) {
            carRotationY -= rotationSpeed;
        }

        if (key == GLFW_KEY_W) {
            if (key == GLFW_KEY_A || key == GLFW_KEY_D) {
                carVelocity += acceleration * turnBoost;
            }
            else {
                carVelocity += acceleration;
            }
            if (carVelocity > 30.0f) carVelocity = 30.0f;
        }
        if (key == GLFW_KEY_S) {
            if (key == GLFW_KEY_A || key == GLFW_KEY_D) {
                carVelocity -= acceleration * turnBoost;
            }
            else {
                carVelocity -= acceleration;
            }
            if (carVelocity < -10.0f) carVelocity = -10.0f;
        }
    }
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
            if (carVelocity > 0.0f) {
                carVelocity -= deceleration;
                if (carVelocity < 0.0f) carVelocity = 0.0f;
            }
            else if (carVelocity < 0.0f) {
                carVelocity += deceleration;
                if (carVelocity > 0.0f) carVelocity = 0.0f;
            }
        }
    }
    //Toggling between night and day
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        dirLightColor = glm::vec3(1.0f, 0.95f, 0.8f); 
        dirLightIntensity = 1.0f; 
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        dirLightColor = glm::vec3(0.1f, 0.15f, 0.3f);
        dirLightIntensity = 0.3f;
    }
}

//Mouse input for camera controls
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (activeCamera != &thirdPersonCamera) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    thirdPersonCamera.yaw += xoffset;
    thirdPersonCamera.pitch += yoffset;

    thirdPersonCamera.pitch = glm::clamp(thirdPersonCamera.pitch, -89.0f, 89.0f);

    thirdPersonCamera.UpdateCameraPosition(carPosition, thirdPersonCamera.yaw);
}

// Function to check for collision between a car and a collider
bool CheckCollision(glm::vec3 carPosition, glm::vec3 colliderPosition, glm::vec3 colliderSize) {
    return (carPosition.x >= colliderPosition.x - colliderSize.x / 2 &&
        carPosition.x <= colliderPosition.x + colliderSize.x / 2 &&
        carPosition.z >= colliderPosition.z - colliderSize.z / 2 &&
        carPosition.z <= colliderPosition.z + colliderSize.z / 2);
}

// Main function
int main(void) {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(1920, 1080, "Machine Project", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    CompileShaders();

    // Load the player car model
    Model model("3D/Car2.obj", "3D/gtr.png", "3D/steel.png");
    Player player1(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float lastFrame = 0.0f;

    // Load models for other ghost cars, road, tires, and flag
    Model modelCar2("3D/Car2.obj", "3D/gtr.png", "3D/steel.png");
    Model modelCar3("3D/Car2.obj", "3D/gtr.png","3D/steel.png");
    Model roadModel("3D/plane.obj", "3D/asphalt.png");
    Model tireModel("3D/tires.obj", "3D/carbon.png", "3D/brickwall_normal.jpg");
    Model flagModel("3D/Flag.obj", "3D/tuxedosam.png", "3D/brickwall_normal.jpg");

    //Collider properties for the finish line (tires) and its position
    glm::vec3 tireColliderSize = glm::vec3(45.0f, 1.0f, 10.0f);
    glm::vec3 tirePosition = glm::vec3(10.0f, 0.0f, 290.0f);
    glm::vec3 flagPosition = glm::vec3(20.0f, 0.0f, 500.0f);

    //Initial positions for the other cars
    glm::vec3 car2Position = glm::vec3(4.5f, 0.0f, -0.3f);
    glm::vec3 car3Position = glm::vec3(-2.5f, 0.0f, -0.3f);

    //Skybox textures loading
    std::vector<std::string> skyboxFaces = {
        "Skybox/sunset_rt.png",
        "Skybox/sunset_lf.png",
        "Skybox/sunset_up.png",
        "Skybox/sunset_dn.png",
        "Skybox/sunset_ft.png",
        "Skybox/sunset_bk.png"
    };

    skybox = new Skybox(skyboxFaces);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = activeCamera->GetViewMatrix();

        if (activeCamera == &perspectiveCamera) {
            dynamic_cast<PerspectiveCamera*>(activeCamera)->UpdateCameraPosition(carPosition, carRotationY);
        }
        else {
            dynamic_cast<ThirdPersonCamera*>(activeCamera)->UpdateCameraPosition(carPosition, carRotationY);
        }

        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        skybox->Draw(skyboxView, projection);

        glUseProgram(shaderProgram);

        glm::vec3 lightOffset(0.0f, 5.0f, 0.0f);
        glm::vec3 lightDirection = carPosition + lightOffset;
        directionalLightDir = glm::normalize(carPosition - lightDirection);

        GLuint dirLightDirLoc = glGetUniformLocation(shaderProgram, "directionalLightDir");
        glUniform3fv(dirLightDirLoc, 1, glm::value_ptr(directionalLightDir));

        GLint dirLightColorLoc = glGetUniformLocation(shaderProgram, "dirLight.color");
        glUniform3fv(dirLightColorLoc, 1, glm::value_ptr(dirLightColor)); 

        GLint dirLightIntensityLoc = glGetUniformLocation(shaderProgram, "dirLight.intensity");
        glUniform1f(dirLightIntensityLoc, dirLightIntensity);

        float radians = glm::radians(carRotationY);
        glm::vec3 direction(sin(radians), 0.0f, cos(radians));
        carPosition += direction * carVelocity * deltaTime;

        player1.Update(deltaTime);
        player1.SetVelocity(carVelocity);
        player1.SetRotationY(carRotationY);

        if (isCarsMoving) {
            car2Position.z += car2Velocity * deltaTime; 
            car3Position.z += car3Velocity * deltaTime; 
        }

        if (!isCar1Finished && CheckCollision(carPosition, tirePosition, tireColliderSize)) {
            isCar1Finished = true;
            std::cout << "Car 1 Finished!" << std::endl;
        }
        if (!isCar2Finished && CheckCollision(car2Position, tirePosition, tireColliderSize)) {
            isCar2Finished = true;
            std::cout << "Car 2 Finished!" << std::endl;
        }
        if (!isCar3Finished && CheckCollision(car3Position, tirePosition, tireColliderSize)) {
            isCar3Finished = true;
            std::cout << "Car 3 Finished!" << std::endl;
        }

        if (isCar1Finished && isCar2Finished && isCar3Finished && !printTimeOnce) {
            auto endTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = endTime - startTime;
            std::cout << "Game Over! All karts finished in: " << elapsed.count() << " seconds" << std::endl;
            printTimeOnce = true;
        }
        player1.Draw(shaderProgram, projection, view);

        glm::mat4 tireTransform = glm::mat4(1.0f);
        tireTransform = glm::scale(tireTransform, glm::vec3(1.0f, 1.0f, 1.0f));
        tireTransform = glm::translate(tireTransform, tirePosition);
        glm::mat4 tireMVP = projection * view * tireTransform;

        
        glUseProgram(shaderProgram); 

   
        glm::vec3 lightPos = glm::vec3(1, 5, 1);
        GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");


        if (activeCamera == &perspectiveCamera) {
            glUniform3fv(viewPosLoc, 1, glm::value_ptr(dynamic_cast<PerspectiveCamera*>(activeCamera)->position));
        }
        else {
            glUniform3fv(viewPosLoc, 1, glm::value_ptr(dynamic_cast<ThirdPersonCamera*>(activeCamera)->position));
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tireModel.GetTextureID());
        glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tireModel.GetNormalMapTextureID());
        glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 1);

        tireModel.Draw(shaderProgram, tireMVP);

        glm::mat4 flagTransform = glm::mat4(1.0f);
        flagTransform = glm::scale(flagTransform, glm::vec3(0.5f, 0.5f, 0.5f));
        flagTransform = glm::translate(flagTransform, flagPosition);
        glm::mat4 flagMVP = projection * view * flagTransform;
        flagModel.Draw(shaderProgram, flagMVP);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);

        glm::mat4 car2Transform = glm::mat4(1.0f);
        car2Transform = glm::scale(car2Transform, glm::vec3(0.8f, 0.8f, 0.8f));
        car2Transform = glm::translate(car2Transform, car2Position);
        glm::mat4 car2MVP = projection * view * car2Transform;
        modelCar2.Draw(shaderProgram, car2MVP);

        glm::mat4 car3Transform = glm::mat4(1.0f);
        car3Transform = glm::scale(car3Transform, glm::vec3(1.7f, 1.7f, 1.7f));
        car3Transform = glm::translate(car3Transform, car3Position);
        glm::mat4 car3MVP = projection * view * car3Transform;
        modelCar3.Draw(shaderProgram, car3MVP);

        glm::mat4 roadTransform = glm::mat4(1.0f);

        glDisable(GL_BLEND);

        roadTransform = glm::rotate(roadTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        roadTransform = glm::translate(roadTransform, glm::vec3(0.0f, 90.0f, -0.0000001f));
        roadTransform = glm::scale(roadTransform, glm::vec3(10.5f, 205.0f, 10.0f));
        glm::mat4 roadMVP = projection * view * roadTransform;
        roadModel.Draw(shaderProgram, roadMVP);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
