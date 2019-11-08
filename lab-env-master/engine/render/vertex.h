#pragma once
#include "math/vector4.h"
#include "math/vector2.h"

namespace Render
{
struct ALIGN16 Vertex
{
	Vertex()
	{
		this->pos = Math::vector4(0.0f, 0.0f, 0.0f);
		this->norm = Math::vector4(0.0f, 0.0f, 0.0f);
		this->tex = Math::vector2(0.0f, 0.0f);
	}
	Math::vector4 pos;
	Math::vector4 norm;
	Math::vector2 tex;
};
}
