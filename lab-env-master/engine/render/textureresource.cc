#include "config.h"
#include "textureresource.h"
#include "stb_image.h"


namespace Render
{

//------------------------------------------------------------------------------
/**
*/
TextureResource::TextureResource()
{

}

TextureResource::TextureResource(const char* texFile)
{
	this->LoadTexture(texFile);
}

//------------------------------------------------------------------------------
/**
*/
TextureResource::~TextureResource()
{
	Destructor();
}

//------------------------------------------------------------------------------
/**
*/
void TextureResource::LoadTexture(const std::string& filename)
{
	int height, width, comp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* iData = stbi_load(filename.c_str(), &width, &height, &comp, 3);
	if (iData == NULL)
		return;

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);		/* causes GL_INVALID_ENUM error */


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, iData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(iData);
}

//------------------------------------------------------------------------------
/**
*/
void TextureResource::Destructor()
{
	glDeleteTextures(1, &this->texture);
}

//------------------------------------------------------------------------------
/**
*/
void TextureResource::Bind()
{
	glBindTexture(GL_TEXTURE_2D, this->texture);
}
}