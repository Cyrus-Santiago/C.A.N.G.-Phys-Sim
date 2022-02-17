/* This class file was made with the help of https://learnopengl.com */
#include "../include/shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// this method is used to change the current shader being used
Shader &Shader::Use() {
    glUseProgram(this->ID);
    return *this;
}

// this method is used to compile our shaders
void Shader::Compile(const char * vShaderCode,
                     const char * fShaderCode) {    
    unsigned int vertex, fragment;

    // compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	checkCompileErrors(vertex, "VERTEX");

    // compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
    checkCompileErrors(fragment, "FRAGMENT");

    // link shaders
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    // print linking errors if any
    checkCompileErrors(this->ID, "PROGRAM");

    // free the memory!
    glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int object, std::string type) {
    int success;
    char infoLog[512];
    if (type == "PROGRAM") {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 512, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n" << std::endl;
        }
    } else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n" << std::endl;
        }
    }
}