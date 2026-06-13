#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

static std::string loadShader(const char* path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "FAILED TO OPEN SHADER: "
                  << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

shaderProgram::shaderProgram(const char* vertexPath, const char* fragmentPath)
{
    std::string vertFile=loadShader(vertexPath); // Vertex shader text
    std::string fragFile=loadShader(fragmentPath); // Fragment shader text

    const char* vSource=vertFile.c_str(); // Vertex shader source
    const char* fSource=fragFile.c_str(); // Fragment shader source

    //Vertex Shader
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infolog[1024];
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 1024, NULL, infolog);
        std::cerr<<"Vertex Shader Compilation Error!: "<<infolog<<std::endl;
    }

    //Fragment Shader
    unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fSource, NULL);
    glCompileShader(fragmentShader);    

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infolog);
        std::cerr<<"Fragment Shader Compilation Error:- "<<infolog<<std::endl;
    }

    // Shader Program: link the compiled vertex and fragment shaders into one program.
    // The program is what we activate when drawing.
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Linking errors usually mean mismatched inputs/outputs between shaders.
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 1024, NULL, infolog);
        std::cerr<<"Shader Program Linking Error:- "<<infolog<<std::endl;
    }

    // Shaders are linked into the program now, so the individual shader objects can be deleted.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void shaderProgram::use()
{
    glUseProgram(program);
}

void shaderProgram::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void shaderProgram::setBool(const std::string &name, bool value) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), (float)value);
}

void shaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), (float)value);
}

/*From shader.vert
#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 2) in vec2 texCoord;

out vec2 texCoords;
out vec2 position;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(Pos, 1.0);

    position = Pos.xy;
    texCoords = texCoord;
}
    
From shader.frag
#version 330 core

in vec2 texCoords;
in vec2 position;

out vec4 screenColor;

uniform float time; 
uniform vec2 resolution;
uniform sampler2D discoBall;


void main(){
    vec4 texColor = texture(discoBall, texCoords);
    if(texColor.a < 0.1) 
        discard;

    vec2 pos = position;

    float aspect = resolution.x/resolution.y;
    pos.x /= aspect;

    
    float radius = 0.5;
    float dist = length(pos);
    if(dist > radius) 
        discard;


    float phase = 2.094; 
    float base = time + atan(pos.y, pos.x);

    float red = (sin(base)+ 1.0)/2.0;
    float green = (sin(base + phase)+ 1.0)/2.0;
    float blue = (sin(base + (2.0 * phase)) + 1.0)/2.0;

    vec3 gradient = vec3(red, green, blue);
    vec3 finalColor = texColor.rgb * gradient;
    screenColor=vec4(finalColor, texColor.a);
}*/