#pragma once

#include <math/vector4.h>
#include <system/entity.h>

namespace Physics
{
struct Collision
{
	System::Entity* entity;
	Math::vector4 hitPoint;
};
}
