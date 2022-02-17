/* Code on this page was written with the help https://learnopengl.com */

#include "../include/resourceManager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "../include/stb_image.h"

std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char *vShaderFile,
                                   const char *fShaderFile,
                                   std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

void ResourceManager::Clear() {
    // go through and delete all shaders
    for (auto iter : Shaders) {
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