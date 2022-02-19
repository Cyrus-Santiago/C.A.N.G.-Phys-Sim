/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/resourceManager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "../include/stb_image.h"

// arrays for storing shaders and textures (so we don't have to repeatedly load them)
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

// we provide the file path and a name, and the resource manager stores it as text
Shader ResourceManager::LoadShader(const char *vShaderFile,
                                   const char *fShaderFile,
                                   std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

// retrieve a shader we loaded earlier
Shader ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

// we provide the file path and a name, and the resource manager stores it as text
Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

// retrieve a texture we loaded earlier
Texture2D ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

// FREE THE MEMORY!
void ResourceManager::Clear() {
    // go through and delete all shaders
    for (auto iter : Shaders) {
        glDeleteProgram(iter.second.ID);
    }
    // go through and delete all textures
    for (auto iter : Textures) {
        glDeleteProgram(iter.second.ID);
    }
}

Shader ResourceManager::loadShaderFromFile(const char * vShaderFile, const char * fShaderFile) {
    // retrieve fragment and vertex shade source code from file path
    std::string vertexCode;
    std::string fragmentCode;

    try {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::STREAM::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha) {
    // create texture
    Texture2D texture;
    if (alpha) {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}