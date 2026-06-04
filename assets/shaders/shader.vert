#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 textureCoord;

out vec3 uvColor;
out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);
    uvColor = Color;
    texCoords = textureCoord;
}