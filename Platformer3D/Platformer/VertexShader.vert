#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 colorPosition_modelspace;
layout(location = 2) in vec2 uvPosition_modelspace;
layout(location = 3) in vec3 normalPosition_modelspace;

uniform mat4 MVP;

out vec3 fragColor;
out vec2 fragUv;
out vec3 fragNormal;
out vec3 fragPos;

void main() 
{
    // Position du sommet dans l'espace de découpe (MVP * position):
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    
    // Couleur de chaque sommet sera interpolé pour produire la couleur de chaque fragment:
    fragColor = colorPosition_modelspace;
    fragUv = uvPosition_modelspace;
    fragPos= vertexPosition_modelspace;
    fragNormal = normalPosition_modelspace;
}