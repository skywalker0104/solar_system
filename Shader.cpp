#include "Shader.h"

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Ham nay nhan vao cac file planet.frag, planet.vert, sun.frag, sun.vert
//Code trong cac file do se duoc ham nay chay

void Shader::loadShaderFromFile(const GLchar* shaderPath, GLenum shaderType)
{
	std::string Code;
	std::ifstream shaderFile;
	//Khuc nay doc file
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		Code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	//Khuc nay doc code trong file
	const GLchar* shaderCode = Code.c_str();
	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::" << shaderType
			<< "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderType = shaderType;
	isLoaded = true;
}

void Shader::deleteShader()
{
	if (!isLoaded)
		return;

	isLoaded = false;
	glDeleteShader(shaderID);
}
