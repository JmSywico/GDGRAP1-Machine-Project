#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D normalMap;
uniform sampler2D tex0;
uniform vec3 lightColor = vec3(3.0, 3.0, 3.0);
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform struct DirectionalLight { //Added Directional Light Struct
    vec3 color;
    float intensity;
} dirLight;

uniform struct PointLight { // Added Point Light struct
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
} pointLight;


void main() {
    // Sample the normal map
    vec3 normal = texture(normalMap, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 lightDir = normalize(lightPos - FragPos);

    // Diffuse Light
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * dirLight.color * dirLight.intensity; //using dirLight

    // Specular Light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
    vec3 specular = spec * dirLight.color * dirLight.intensity; //using dirLight

    // Attenuation Adjustment
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.02 * distance + 0.002 * (distance * distance));

    // Texture Mapping
    vec3 textureColor = texture(tex0, texCoord).rgb;
    vec3 ambient = vec3(0.5) * textureColor;

    // Final Color Composition
    vec3 finalColor = (ambient + attenuation * (diffuse + specular)) * textureColor;
    FragColor = vec4(finalColor, 1.0);
}