#version 330 core

in vec3 fragColor;
in vec2 fragUv;
in vec3 fragNormal;
in vec3 fragPos;

out vec3 color;

uniform sampler2D TextureSample;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() 
{   
    // Lumière ambiante:
    float ambiantFactor = 0.2f;
    vec3 ambiant = lightColor * ambiantFactor;
    vec3 texel = texture( TextureSample, fragUv ).rgb;
   
   // Lumière diffuse:
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPos - fragPos);
    float NDotL = max(dot(fragNormal, lightDirection), 0.0f);
    vec3 diffuse = lightColor * NDotL; 

    // Lumière specular:
    float specularFactor = 1.0f;
    float shininess = 256.0f;
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    float NDotH = max(dot(normal, halfDirection), 0.0f);
    vec3 specular = lightColor * specularFactor * pow(NDotH, shininess);

    // Rendu final:
    vec3 result = ambiant + diffuse + specular;
    color = result * texel;
}