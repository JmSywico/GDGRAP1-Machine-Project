#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Skybox {
public:
    // Constructor and Destructor
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    // Method to render the skybox
    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint skyboxVAO, skyboxVBO, cubemapTexture, skyboxEBO;  // Added skyboxEBO declaration
    GLuint skyboxShader;

    // Method to load cubemap textures
    GLuint LoadCubemap(const std::vector<std::string>& faces);

    // Optional: Method to load shaders (if you need to implement it separately)
    GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
};

#endif
