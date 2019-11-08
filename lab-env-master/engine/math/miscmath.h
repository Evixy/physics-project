#pragma once

#include "vector4.h"

namespace Math
{
bool SameSide(vector4& p1, vector4& p2, vector4& a, vector4& b);
bool PointInTriangle(vector4& p, vector4& A, vector4& B, vector4& C);

///NOTE: EXTREMELY important that each point is inserted in order, e.g. either clockwise or counter clockwise
bool PointInQuad(vector4& p, vector4& A, vector4& B, vector4& C, vector4& D);

inline bool SameSide(vector4& p1, vector4& p2, vector4& a, vector4& b)
{
	vector4 _1 = b - a;
	vector4 _2 = p1 - a;
	vector4 _3 = p2 - a;
	vector4 cp1 = vector4::cross3(_1, _2);
	vector4 cp2 = vector4::cross3(_1, _3);

	if(vector4::dot3(cp1, cp2) >= 0)
	{
		return true;
	}
	return false;
}

inline bool PointInQuad(vector4& p, vector4& A, vector4& B, vector4& C, vector4& D)
{
	return PointInTriangle(p, A, B, C) || PointInTriangle(p, A, C, D);
}

inline bool PointInTriangle(vector4& p, vector4& A, vector4& B, vector4& C)
{
	if(SameSide(p, A, B, C) && SameSide(p, B, A, C) && SameSide(p, C, A, B))
	{
		return true;
	}
	return false;	
}

}
