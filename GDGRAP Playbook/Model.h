#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class Model {
public:
    Model(const std::string& path, const std::string& texturePath = "");
    ~Model();
    void Draw(GLuint shaderProgram, const glm::mat4& transformation_matrix);

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> texCoords;  // Store texture coordinates

    GLuint VAO, VBO, EBO, textureID;

    void LoadModel(const std::string& path);
    void LoadTexture(const std::string& texturePath);
};
