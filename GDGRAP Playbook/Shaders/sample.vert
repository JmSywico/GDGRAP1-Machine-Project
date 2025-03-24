#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 FragNormal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat3 normalMatrix; 

void main() {
    texCoord = aTexCoord;
    FragPos = vec3(transform * vec4(aPos, 1.0)); 
    FragNormal = normalize(normalMatrix * aNormal);
    gl_Position = transform * vec4(aPos, 1.0);
}
