/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#include "../include/glad.h"
#include <iostream>
#include <sstream>
#include <fstream>

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
    /* Loads all files needed in game file */
    static void initializeResources();
private:
    ResourceManager() { }
    // load and gen shader from a file
    static Shader loadShaderFromFile(const char * vShaderFile, const char * fShaderFile);
    // load and gen texture from file
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif
