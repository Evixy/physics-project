#pragma once
#include "math/vector4.h"
#include "math/vector2.h"
#include <vector>
#include <render/vertex.h>


namespace System
{

class ObjectLoader
{
public:

	static bool LoadObjectFile(const char* filename, std::vector<Render::Vertex>& vertices);

};
}
