/* Code on this page was written with the help https://learnopengl.com */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../include/glad.h"

class Texture2D {
public:
    // used to reference a specific texture
    unsigned int ID;
    // dimension of image in pixels
    unsigned int width, height;
    // texture format details
    unsigned int internalFormat;
    unsigned int imageFormat;
    // configuration options for texture
    unsigned int wrapS;
    unsigned int wrapT;
    unsigned int filterMin;
    unsigned int filterMax;
    // constructor
    Texture2D();
    // generate texture form image
    void Generate(unsigned int width, unsigned int height, unsigned char * data);
    // sets current texture
    void Bind() const;
};

#endif