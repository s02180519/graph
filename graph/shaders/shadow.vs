#version 330 core
layout (location = 0) in vec3 VertexPosition;

uniform mat4 lightPV;
uniform mat4 model;

void main()
{
    gl_Position = lightPV * model * vec4(VertexPosition, 1.0);
}