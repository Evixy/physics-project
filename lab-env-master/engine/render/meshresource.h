#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>

#include <math/vector4.h>
#include <math/vector2.h>
#include "vertex.h"
#include <core/defines.h>

struct Edge;
struct Face;

//#define DEBUGGING
typedef unsigned int uint;


namespace Render
{
class ALIGN16 MeshResource
{
public:

	MeshResource();
	MeshResource(const char* meshFile);
	~MeshResource();

	void* operator new(size_t i);
	void operator delete(void* p);
	void DrawObject() const;
	

	void InitializeMesh();
	void CleanUp();
	GLint getIbSize();
	
	void Destructor();

	void CreateMeshFromFile(const char* filename);
	//Vertex buffer
	std::vector<Vertex> vb;
	//Index buffer 
	std::vector<int> ib;

private:

	//Vertex Buffer Object
	//Vertex coordinates
	GLuint vbo;
	//Texture coordinates
	GLuint tbo;
	//Normal coordinates
	GLuint nbo;
	//Vertex Array Object
	GLuint vao;
	//Index Buffer Object
	GLuint ibo;

	GLint ibSize;
	GLint numVertices;
	uint vertexArraySize;
	uint facesArraySize;
};

//------------------------------------------------------------------------------
/**
*/
inline GLint MeshResource::getIbSize()
{
	return this->ibSize;
}

//------------------------------------------------------------------------------
/**
*/
inline bool operator== (Vertex& v1, Vertex& v2)
{
	return (v1.pos[0] == v2.pos[0] && v1.pos[1] == v2.pos[1] && v1.pos[2] == v2.pos[2]);
}

//------------------------------------------------------------------------------
/**
*/
inline bool operator!=(Vertex& v1, Vertex& v2)
{
	return (v1.pos[0] != v2.pos[0] || v1.pos[1] != v2.pos[1] || v1.pos[2] != v2.pos[2]);
}
}