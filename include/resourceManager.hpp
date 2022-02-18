/* Code on this page was written with the help https://learnopengl.com */

#include <map>
#include <string>

#include "../include/glad.h"

#include "shader.hpp"

// singleton class that handles loading and storage of shaders
class ResourceManager {
public:
    // for storing shaders
    static std::map<std::string, Shader> Shaders;
    // loads shader source code to create a shader program
    static Shader LoadShader(const char * vShaderFile,
                             const char * fShaderFile,
                             std::string name);
    // gets shader from storage
    static Shader GetShader(std::string name);
    // free the memory!
    static void Clear();
private:
    // load and gen shader from a file
    static Shader loadShaderFromFile(const char * vShaderFile, const char * fShaderFile);
};