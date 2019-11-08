#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>

namespace Render
{
class TextureResource
{
public:


	//void* operator new(size_t i);
	//void operator delete(void* p);

	TextureResource();
	TextureResource(const char* texFile);
	~TextureResource();

	void LoadTexture(const std::string& filename);
	void Destructor();
	void Bind();

private:

	GLuint texture;

};
}