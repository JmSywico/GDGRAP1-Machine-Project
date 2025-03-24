#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D tex0;
uniform vec3 lightColor = vec3(3.0, 3.0, 3.0);
uniform vec3 lightPos;
uniform vec3 viewPos;  

void main()
{
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // **Diffuse Light**
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 3.0;

    // **Specular Light**
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 128.0);
    vec3 specular = spec * lightColor * 4; // Adjust intensity

    // **Attenuation Adjustment**
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.02 * distance + 0.002 * (distance * distance)); 

    // **Texture Mapping**
    vec3 textureColor = texture(tex0, texCoord).rgb;
    vec3 ambient = vec3(0.5) * textureColor;

    // **Final Color Composition**
    vec3 finalColor = (ambient + attenuation * (diffuse + specular)) * textureColor;
    FragColor = vec4(finalColor, 1.0);
}
