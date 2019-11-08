#pragma once
#include <string>
#include "defines.h"

class AnimEvent
{
public:
	AnimEvent();
	~AnimEvent();

	std::string name;
	std::string category;
	Tick time;
};

