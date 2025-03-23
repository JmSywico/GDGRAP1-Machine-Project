#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 FragNormal; // Corrected normal
in vec4 FragPos;    // Object-space position

uniform sampler2D tex0;
uniform vec3 lightColor;
uniform vec3 lightDir; // Light direction in object space

void main()
{
    vec3 norm = normalize(FragNormal); // Ensure normal is normalized
    vec3 normLightDir = normalize(lightDir); // Normalize light direction
    
    float diff = max(dot(norm, normLightDir), 0.0); // Lambertian diffuse lighting
    vec3 diffuse = lightColor * diff; // Apply diffuse lighting
    
    vec3 textureColor = texture(tex0, texCoord).rgb;
    vec3 finalColor = textureColor * diffuse; // Multiply with texture color

    FragColor = texture(tex0, texCoord);
}
