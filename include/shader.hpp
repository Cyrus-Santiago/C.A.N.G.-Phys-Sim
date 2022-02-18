/* This class file was made with the help of https://learnopengl.com */

#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader() {}

	Shader &Use();

	void Compile(const char * vShaderCode,
				 const char * fShaderCode);
	void setFloat(const char * name, float value, bool useShader = false);
	void setInt(const char * name, int value, bool useShader = false);
	void SetVector2f (const char *name, float x, float y, bool useShader = false);
    void SetVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
    void SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
    void SetVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
    void SetVector4f (const char *name, float x, float y, float z, float w,
					  bool useShader = false);
    void SetVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
    void SetMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);
private:
    // checks for errors in compilation and prints them
    void checkCompileErrors(unsigned int object, std::string type); 
};
#endif
