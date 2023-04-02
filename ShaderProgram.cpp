#include "GL/glew.h"
#include "ShaderProgram.h"
#include <iostream>

//Sao khi doc file ben Shader.cpp xong thi qua day tao chuong trinh de chay code trong may cai file

//Ham nay tao chuong trinh de chay
void ShaderProgram::createProgram()
{
	shaderProgramID = glCreateProgram();
}

//Gan cac thong so vo chuong trinh
bool ShaderProgram::addShaderToProgram(const Shader& shader)
{
	if (!shader.isShaderLoaded())
		return false;

	glAttachShader(shaderProgramID, shader.getShaderID());
	return true;
}

//Lien ket chuong trinh voi may cai o ben SolarSystem.cpp
bool ShaderProgram::linkProgram()
{
	glLinkProgram(shaderProgramID);
	int linkStatus;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkStatus);
	isLinked = linkStatus == GL_TRUE;
	GLint success;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);

		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	return isLinked;
}

void ShaderProgram::deleteProgram()
{
	if (!isLinked) {
		return;
	}

	glDeleteProgram(shaderProgramID);
	isLinked = false;
}

void ShaderProgram::useProgram() const
{
	if (isLinked) {
		glUseProgram(shaderProgramID);
	}
}

GLuint ShaderProgram::getShaderProgramID() const
{
	return shaderProgramID;
}
