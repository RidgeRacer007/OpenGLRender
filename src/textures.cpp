#include "texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


textureProgram::textureProgram(const char* path)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //fitering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load image
    int width, height, nrChannels;
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char*  data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if(nrChannels == 4) //with the alpha channel.
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } 
        else if (nrChannels == 3)  //w/o the alpha channel.
        {
            {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        }
        else
        {
            std::cout << "Unsupported texture img. format. Please choose the right image format!\n";
            stbi_image_free(data);
            return;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Upload texture to GPU
        glTexImage2D(
            GL_TEXTURE_2D, 0, internalFormat,
            width, height, 0, dataFormat,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load the textures! :(" << path << "\n";
    }
    
    stbi_image_free(data);
}

void textureProgram::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

