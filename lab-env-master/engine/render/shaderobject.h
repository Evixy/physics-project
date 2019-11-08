#pragma once
#include <GL/glew.h>

#include <memory>
#include <math/matrix44.h>
#include <math/matrix33.h>

using namespace Math;

namespace Render
{
class ALIGN16 ShaderObject
{
public:

	ShaderObject();
	ShaderObject(const char* vs, const char* fs);
	~ShaderObject();

	void* operator new(size_t i);
	void operator delete(void* p);

	void Destructor();

	GLuint CreateShader(const std::string& text, GLenum shaderType);
	GLuint getIdByName(const char* name);

	std::string LoadShaderFromFile(const std::string& shaderFileName);

	void setUniformMatrix4fv(const char* name, matrix44& matrix);
	void setUniformMatrix3fv(const char* name, matrix33& matrix);
	void setUniformVector4fv(const char* name, vector4& vector);
	void setUniformVector3fv(const char* name, vector4& vector);
	void setUniform1f(const char* name, GLfloat& floatVal);

	void LoadShader(const std::string& vertexShader, const std::string& fragmentShader);
	void Bind();
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	const char* model_in = "model_in";
	const char* view_in = "view_in";
	const char* proj_in = "proj_in";
	const char* color_in = "color_in";

private:

	GLuint program;
	GLuint vertexShader, fragmentShader;

};
}