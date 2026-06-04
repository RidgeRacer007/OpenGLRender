#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class textureProgram
{
public:
    unsigned int textureID;

    textureProgram(const char* path);
    void bind(GLenum textureUnit);

};
#endif