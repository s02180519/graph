#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexture;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = VertexTexture;
    gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
}