#pragma once
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "Shader.h"

class ShaderProgram
{
	GLuint shaderProgramID;
	bool isLinked = false;

public:

	void createProgram();
	bool addShaderToProgram(const Shader& shader);
	bool linkProgram();
	void useProgram() const;
	void deleteProgram();
	GLuint getShaderProgramID() const;
};

#endif // !SHADER_PROGRAMM_H
