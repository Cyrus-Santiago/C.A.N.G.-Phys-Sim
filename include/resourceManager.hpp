/* Code on this page was written with the help https://learnopengl.com */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#include "../include/glad.h"

#include "shader.hpp"
#include "texture.hpp"

// singleton class that handles loading and storage of shaders
class ResourceManager {
public:
    // for storing shaders
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    // loads shader source code to create a shader program
    static Shader LoadShader(const char * vShaderFile,
                             const char * fShaderFile,
                             std::string name);
    // gets shader from storage
    static Shader GetShader(std::string name);
    // load and gen texture
    static Texture2D LoadTexture(const char *file, bool alpha, std::string name);
    // retrieves texture form storage
    static Texture2D GetTexture(std::string name);
    // free the memory!
    static void Clear();
private:
    // load and gen shader from a file
    static Shader loadShaderFromFile(const char * vShaderFile, const char * fShaderFile);
    // load and gen texture from file
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif