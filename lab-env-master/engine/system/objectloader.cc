#include "config.h"
#include "objectloader.h"
#include <vector>
#include <GL/glew.h>
#include <cstring>
#include <math/vector4.h>

using namespace std;

namespace System
{
bool ObjectLoader::LoadObjectFile(const char* filename, std::vector<Render::Vertex>& vertices)
{
	vector <GLuint> vertexIndices, uvIndices, normalIndices;
	vector <Math::vector4> temp_vertices;
	vector <Math::vector4> temp_normals;
	vector <Math::vector2> temp_uvs;

	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Cannot load file \n");
		return false;
	}

	bool gotNormals = false;
	bool gotTextures = false;
	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			Math::vector4 vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			gotTextures = true;
			Math::vector2 uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			//uv[1] = -uv[1];					
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			gotNormals = true;
			Math::vector4 normal;
			fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0)
		{
			//Textures AND Normals
			if (gotTextures && gotNormals)
			{
				GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, 
					" %d/%d/%d %d/%d/%d %d/%d/%d \n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				assert(matches == 9);

				for (int i = 0; i < 3; i++)	// loading triangles
				{
					vertexIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);
					normalIndices.push_back(normalIndex[i]);
				}
			}
			
			//Normals but NO Textures
			else if(!gotTextures && gotNormals)
			{
				GLuint vertexIndex[3], normalIndex[3];
				int matches = fscanf(file, 
					" %d/%d %d/%d %d/%d \n",
					&vertexIndex[0], &normalIndex[0],
					&vertexIndex[1], &normalIndex[1],
					&vertexIndex[2], &normalIndex[2]);
				
				assert(matches == 6);

				for (int i = 0; i < 3; i++)	// loading triangles
				{
					vertexIndices.push_back(vertexIndex[i]);					
					normalIndices.push_back(normalIndex[i]);
				}
			}
			
			//Textures but NO Normals
			else if(gotTextures && !gotNormals)
			{
				GLuint vertexIndex[3], uvIndex[3];
				int matches = fscanf(file, 
					" %d/%d %d/%d %d/%d \n",
					&vertexIndex[0], &uvIndex[0],
					&vertexIndex[1], &uvIndex[1],
					&vertexIndex[2], &uvIndex[2]);
				
				assert(matches == 6);

				for (int i = 0; i < 3; i++)	// loading triangles
				{
					vertexIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);					
				}
			}
			
			//NO Textures and NO Normals
			else if(!gotTextures && !gotNormals)
			{
				GLuint vertexIndex[3];
				int matches = fscanf(file,
					" %d %d %d \n",
					&vertexIndex[0], 
					&vertexIndex[1], 
					&vertexIndex[2]);
				assert(matches == 3);

				for (int i = 0; i < 3; i++)	// loading triangles
				{
					vertexIndices.push_back(vertexIndex[i]);					
				}
			}
						
			
		}
	} //end while true

	for (GLuint i = 0; i < vertexIndices.size(); i++)
	{
		Render::Vertex v;

		Math::vector4 vertex = temp_vertices[vertexIndices[i] - 1];
		v.pos = vertex;		

		if(gotTextures)
		{
			Math::vector2 uvs;
			uvs = temp_uvs[uvIndices[i] - 1];
			v.tex = uvs;
		}
		
		if(gotNormals)
		{
			Math::vector4 normals;
			normals = temp_normals[normalIndices[i] - 1];
			v.norm = normals;
		}
		
		vertices.push_back(v);
	}

	printf("File %s successfully loaded! \n", filename);
	return true;
}
}