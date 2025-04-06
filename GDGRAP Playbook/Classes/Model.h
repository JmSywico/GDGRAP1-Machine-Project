// Model.h

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Model {
public:
    Model(const std::string& filename, const std::string& textureFilename, const std::string& normalMapFilename = "");
    ~Model();

    void Draw(GLuint shaderProgram, glm::mat4 modelMatrix);

    GLuint GetTextureID() const { return textureID; }
    GLuint GetNormalMapTextureID() const { return normalMapTextureID; }

private:
    std::vector<GLfloat> vertices;
    GLuint VAO, VBO, EBO;
    GLuint textureID, normalMapTextureID;

    void LoadModel(const std::string& filename);
    GLuint LoadTexture(const std::string& filename);
};

#endif