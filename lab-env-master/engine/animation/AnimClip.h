#pragma once
#include "defines.h"
#include <map>
#include <vector>
#include "AnimEvent.h"
#include "AnimCurve.h"

class AnimClip
{
public:
	AnimClip();
	~AnimClip();

	const char* name;
	IndexT startKeyIndex;
	SizeT numKeys;
	SizeT keyStride;
	Tick keyDuration;
	InfinityType::Code preInfinityType;
	InfinityType::Code postInfinityType;
	std::vector<AnimCurve> curves;
	std::vector<AnimEvent> events;
	std::map<const char*, IndexT> eventIndexMap;
	IndexT keySliceFirstKeyIndex;      
	SizeT keySliceByteSize;            
	bool keySliceValuesValid;
	bool inBeginEvents;

private:

};
