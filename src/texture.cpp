/* Code on this page was written with the help https://learnopengl.com */

#include <iostream>

#include "../include/texture.hpp"

Texture2D::Texture2D() : width(0), height(0), internalFormat(GL_RGB), wrapS(GL_REPEAT),
    filterMin(GL_LINEAR), filterMax(GL_LINEAR) {
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char * data) {
    this->width = width;
    this->height = height;
    // create the texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0,
        this->imageFormat, GL_UNSIGNED_BYTE, data);
    // set configuration settings for loaded textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}