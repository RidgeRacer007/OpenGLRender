#version 330 core
out vec4 screenColor;

in vec3 uvColor;
in vec2 texCoords;

uniform sampler2D container;
uniform sampler2D IGT;

void main(){
    screenColor = mix (texture(container, texCoords), texture(IGT, texCoords), 0.39);
}