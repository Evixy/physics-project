#pragma once
#include "NaxStructs.h"
#include <vector>

class Nax3Parser
{
public:
	Nax3Parser();
	~Nax3Parser();
	static Nax3Parser* Instance();
	bool SetupFromNax3(const char* filename);
private:
	std::vector<Nax3Clip*> clips;
	std::vector<Nax3Curve*> curves;
	std::vector<Nax3AnimEvent*> animEvents;
};

