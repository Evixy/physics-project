#include "config.h"
#include "meshresource.h"
#include <cstring>
#include <fstream>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <atomic>
#include <system/objectloader.h>
#include "vertex.h"

using namespace std;

namespace Render
{
//------------------------------------------------------------------------------
/**
*/
MeshResource::MeshResource() :
	vertexArraySize(0),
	facesArraySize(0)
{	

}

//------------------------------------------------------------------------------
/**
*/
MeshResource::MeshResource(const char* meshFile)
{
	this->CreateMeshFromFile(meshFile);
}

//------------------------------------------------------------------------------
/**
*/
MeshResource::~MeshResource()
{
	this->Destructor();
}

//------------------------------------------------------------------------------
/**
*/
void MeshResource::Destructor()
{
	glDeleteBuffers(1, &this->ibo);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->tbo);
	glDeleteBuffers(1, &this->nbo);
	glDeleteVertexArrays(1, &this->vao);
	this->CleanUp();
}

//------------------------------------------------------------------------------
/**
*/
void MeshResource::CreateMeshFromFile(const char* filename)
{
	assert(System::ObjectLoader::LoadObjectFile(filename, this->vb));
	this->InitializeMesh();
}

//------------------------------------------------------------------------------
/**
*/
void* MeshResource::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

//------------------------------------------------------------------------------
/**
*/
void MeshResource::operator delete(void* p)
{
	_mm_free(p);
}

//------------------------------------------------------------------------------
/**
*/
void MeshResource::DrawObject() const
{
	glBindVertexArray(this->vao);	
	glDrawArrays(GL_TRIANGLES, 0, this->vb.size());	
	glBindVertexArray(0);
}

//------------------------------------------------------------------------------
/**
*/
void MeshResource::CleanUp()
{
	this->vb.clear();
	this->ib.clear();
}

//------------------------------------------------------------------------------
/**
*/
void MeshResource::InitializeMesh()
{
	this->numVertices = this->vb.size();
	this->ibSize = this->ib.size();
	//Vertex array object
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	//Vertex buffer object
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	//Normals buffer object
	glGenBuffers(1, &this->nbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->nbo);

	//Texture buffer object
	glGenBuffers(1, &this->tbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->tbo);
	
	//Bufferdata
	glBufferData(GL_ARRAY_BUFFER, this->numVertices * sizeof(Vertex), &this->vb[0], GL_STATIC_DRAW);

	//Enable vertex attribute array
	glEnableVertexAttribArray(0);	//Vertex positions
	glEnableVertexAttribArray(1);	//Normals coordinates
	glEnableVertexAttribArray(2);	//Texture coordinates

	//Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);

	//Normals coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (4 * sizeof(float)));

	//Texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (8 * sizeof(float)));


	//Index buffer object
	//glGenBuffers(1, &this->ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->ibSize * sizeof(unsigned int), &indexBuffer[0], GL_STATIC_DRAW);

	//Draw only lines
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
}