#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal; // Normal input

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 normalMatrix; // Corrects normal transformations

void main() {
    texCoord = aTexCoord;
    FragPos = vec3(transform * vec4(aPos, 1.0)); 
    Normal = mat3(normalMatrix) * aNormal; // Transform normals correctly
    gl_Position = transform * vec4(aPos, 1.0);
}
