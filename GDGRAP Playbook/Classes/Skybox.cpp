#include "Skybox.h"
#include "../stb_image.h" // You need to include this header for loading images
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION

// Constructor
Skybox::Skybox(const std::vector<std::string>& faces)
{
    // Load shaders (vertex and fragment)
    std::fstream sky_vertSrc("Shaders/skybox.vert");
    std::stringstream sky_vertBuff;
    sky_vertBuff << sky_vertSrc.rdbuf();

    std::string sky_vertS = sky_vertBuff.str();
    const char* sky_v = sky_vertS.c_str();

    std::fstream sky_fragSrc("Shaders/skybox.frag");
    std::stringstream sky_fragBuff;
    sky_fragBuff << sky_fragSrc.rdbuf();
    std::string sky_fragS = sky_fragBuff.str();
    const char* sky_f = sky_fragS.c_str();

    GLuint sky_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sky_vertexShader, 1, &sky_v, NULL);
    glCompileShader(sky_vertexShader);

    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
    glCompileShader(sky_fragShader);

    skyboxShader = glCreateProgram();
    glAttachShader(skyboxShader, sky_vertexShader);
    glAttachShader(skyboxShader, sky_fragShader);
    glLinkProgram(skyboxShader);

    // Load cubemap texture
    cubemapTexture = LoadCubemap(faces);

    // Setup skybox VAO and VBO
    float skyboxVertices[] = {
        -1.f, -1.f, 1.f,  // 0
        1.f, -1.f, 1.f,   // 1
        1.f, -1.f, -1.f,  // 2
        -1.f, -1.f, -1.f, // 3
        -1.f, 1.f, 1.f,   // 4
        1.f, 1.f, 1.f,    // 5
        1.f, 1.f, -1.f,   // 6
        -1.f, 1.f, -1.f   // 7
    };

    unsigned int skyboxIndices[] = {
        1, 2, 6, 6, 5, 1,
        0, 4, 7, 7, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 3, 2, 2, 1, 0,
        0, 1, 5, 5, 4, 0,
        3, 7, 6, 6, 2, 3
    };

    // Create VAO, VBO, and EBO for the skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
}

// Destructor
Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);
    glDeleteTextures(1, &cubemapTexture);
}

// Draw function
void Skybox::Draw(const glm::mat4& view, const glm::mat4& projection)
{
    glDepthFunc(GL_LEQUAL); // Set depth function for skybox
    glUseProgram(skyboxShader);

    // Remove translation from the view matrix
    glm::mat4 viewMat = glm::mat4(glm::mat3(view));  // Only take the rotation part of the view matrix
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, &projection[0][0]);

    // Scale the skybox to be very large, but do not translate it!
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));  // Make skybox large
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "model"), 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Draw the skybox as a cube
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);  // Reset depth function for regular objects
}

// Load Cubemap texture
GLuint Skybox::LoadCubemap(const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Set the flip setting here before loading cubemap textures
    stbi_set_flip_vertically_on_load(false);

    for (GLuint i = 0; i < faces.size(); i++) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
