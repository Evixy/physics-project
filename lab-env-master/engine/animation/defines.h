#pragma once

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef int IndexT;
typedef int SizeT;
typedef int Tick;

class InfinityType
{
public:

	enum Code
	{
		Constant,
		Cycle,
	};
};


class CurveType
{
public:

	enum Code
	{
		Translation,
		Scale,
		Rotation,
		Color,
		Velocity,
		Float4
	};
};