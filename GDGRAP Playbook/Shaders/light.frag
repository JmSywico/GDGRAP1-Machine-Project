#version 330 core
out vec4 FragColor;
uniform vec3 lightColor;
void main() {
    FragColor = vec4(lightColor * 0.8 + vec3(1), 1.0); // Add some bias
}