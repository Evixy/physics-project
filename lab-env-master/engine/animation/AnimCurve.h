#pragma once
#include "defines.h"
#include "Math/vector4.h"

class AnimCurve
{
public:
	AnimCurve();
	~AnimCurve();

	Math::vector4 staticKey;
	IndexT firstKeyIndex;
	CurveType::Code curveType;
	bool isActive;
	bool isStatic;

private:

};

