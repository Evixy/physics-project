#pragma once

#include "tinyxml2.h"
#include "Math/vector4.h"
#include <vector>

class Skeleton;
using namespace tinyxml2;

class SkeletonParser
{
public:
	SkeletonParser();
	~SkeletonParser();

	Skeleton * LoadSkeletonFromFile(const char* filename);
	
private:
	void SplitString(const char* string, char delim, std::vector<float>& elements);
	Math::vector4 StringToVec4(const char* str);
	Math::vector4 ElementsToVec4(std::vector<float>& elements);
};

