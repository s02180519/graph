#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexture;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 lightFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightPV;

void main()
{
    FragPos = vec3(model * vec4(VertexPosition, 1.0));
    Normal = inverse(mat3(model)) * VertexNormal;
    TexCoords = VertexTexture;
    lightFragPos = lightPV * model * vec4(VertexPosition, 1.0);

    gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
}