#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 texCoord;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 MVP;

void main() {
    texCoord = aTexCoord;
    FragPos = vec3(MVP * vec4(aPos, 1.0));

    vec3 T = normalize(mat3(MVP) * aTangent);
    vec3 N = normalize(mat3(MVP) * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);

    gl_Position = MVP * vec4(aPos, 1.0);
}
