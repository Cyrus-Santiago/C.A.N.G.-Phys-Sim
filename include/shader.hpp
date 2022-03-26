/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */
#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader {
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader() {}

	// method that says "use this shader!"
	Shader &Use();

	// for compiling shaders into binary for the GPU
	void Compile(const char * vShaderCode,
				 const char * fShaderCode);
	
	// utility functions that manipulate input data for shaders

	// sets a single integer value for a shader input
	void SetInt(const char * name, int value, bool useShader = false);

	// sets a vector's values for a shader input
    void SetVector3f (const char *name, const glm::vec3 &value,
					  bool useShader = false);

	void SetVector2f(const char *name, const glm::vec2 &value,
							 bool useShader = false);

	// sets a matrice's values for a shader input
    void SetMatrix4 (const char *name, const glm::mat4 &matrix,
				 	 bool useShader = false);

	void SetVector4f(const char *name, const glm::vec4 &value,
	 						 bool useShader = false);
private:
    // checks for errors in compilation and prints them
    void checkCompileErrors(unsigned int object, std::string type); 
};
#endif
