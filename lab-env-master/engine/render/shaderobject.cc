#include "config.h"
#include "shaderobject.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <math/matrix33.h>



using namespace Math;
namespace Render
{
//------------------------------------------------------------------------------
/**
*/
ShaderObject::ShaderObject()
{

}

//------------------------------------------------------------------------------
/**
*/
ShaderObject::ShaderObject(const char* vs, const char* fs)
{
	this->LoadShader(vs, fs);
}

//------------------------------------------------------------------------------
/**
*/
ShaderObject::~ShaderObject()
{
	this->Destructor();
}

//------------------------------------------------------------------------------
/**
*/
void* ShaderObject::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::operator delete(void* p)
{
	_mm_free(p);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::Destructor()
{
	glDetachShader(this->program, this->vertexShader);
	glDetachShader(this->program, this->fragmentShader);

	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);

	glDeleteProgram(this->program);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::LoadShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	this->program = glCreateProgram();
	this->vertexShader = CreateShader(LoadShaderFromFile(vertexShader), GL_VERTEX_SHADER);
	this->fragmentShader = CreateShader(LoadShaderFromFile(fragmentShader), GL_FRAGMENT_SHADER);

	glAttachShader(this->program, this->vertexShader);
	glAttachShader(this->program, this->fragmentShader);

	glLinkProgram(this->program);
	CheckShaderError(this->program, GL_LINK_STATUS, true, "Failed to link program");

	glValidateProgram(this->program);
	CheckShaderError(this->program, GL_VALIDATE_STATUS, true, "Program is invalid");

}

//------------------------------------------------------------------------------
/**
*/
std::string ShaderObject::LoadShaderFromFile(const std::string& shaderFileName)
{

	std::ifstream file;
	file.open((shaderFileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cout << "Unable to load shader: " << shaderFileName << std::endl;
	}

	return output;
}

//------------------------------------------------------------------------------
/**
*/
GLuint ShaderObject::CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cout << "Shader creation failed" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringsLength[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringsLength[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringsLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Shader compilation failed");

	return shader;
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		std::cout << errorMessage << ": '" << error << "'" << std::endl;
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::Bind()
{
	glUseProgram(this->program);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setUniformVector4fv(const char* name, Math::vector4& vector)
{
	glUniform4fv(this->getIdByName(name), 1, static_cast<GLfloat*>(vector.vec.f));
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setUniformVector3fv(const char* name, vector4& vector)
{
	glUniform3fv(this->getIdByName(name), 1, static_cast<GLfloat*>(vector.vec.f));
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setUniformMatrix4fv(const char* name, matrix44& matrix)
{
	glUniformMatrix4fv(this->getIdByName(name), 1, GL_FALSE, matrix.get());

}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setUniformMatrix3fv(const char* name, matrix33& matrix)
{
	glUniformMatrix4fv(this->getIdByName(name), 1, GL_FALSE, matrix.get());

}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setUniform1f(const char* name, GLfloat& floatVal)
{
	glUniform1f(this->getIdByName(name), floatVal);
}

//------------------------------------------------------------------------------
/**
*/
GLuint ShaderObject::getIdByName(const char* name)
{
	return glGetUniformLocation(this->program, name);
}

}