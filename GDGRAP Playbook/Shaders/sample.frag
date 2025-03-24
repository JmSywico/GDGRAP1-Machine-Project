#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 FragNormal;
in vec3 FragPos; // World-space position

uniform sampler2D tex0;
uniform vec3 lightColor;
uniform vec3 lightPos; // Position of the point light
uniform vec3 viewPos;  // Camera position for specular lighting

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform DirectionalLight dirLight;

void main()
{
    vec3 norm = normalize(FragNormal);
    
    // **Directional Light Calculation**
    vec3 lightDir = normalize(-dirLight.direction);
    float diffDir = max(dot(norm, lightDir), 0.0);
    vec3 diffuseDir = diffDir * dirLight.color * (dirLight.intensity * 2.0); // Increase multiplier

    // **Point Light Calculation (Existing)**
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance)); 
    vec3 textureColor = texture(tex0, texCoord).rgb;
    vec3 ambient = vec3(0.2) * textureColor;

    // **Final Color (Directional + Point Light)**
    vec3 finalColor = ambient + diffuseDir * textureColor; // Add directional light
    FragColor = vec4(finalColor, 1.0);
}
