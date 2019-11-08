#pragma once
#include "AnimClip.h"
#include <map>
#include "NaxStructs.h"
#include "AnimKeyBuffer.h"

class AnimResource
{
public:
	AnimResource();
	~AnimResource();

	/// get number of animation clips
	SizeT GetNumClips() const;
	/// get animation clip at index
	const AnimClip& GetClipByIndex(IndexT clipIndex) const;
	/// return true if clip exists by name
	bool HasClip(const char* clipName) const;
	/// get clip index by name, returns invalid index if not found
	IndexT GetClipIndexByName(const char* clipName) const;
	/// get clip by name
	const AnimClip& GetClipByName(const char* clipName) const;
	/// get pointer to AnimKeyBuffer
	const AnimKeyBuffer* GetKeyBuffer() const;
	
private:
	friend class Nax3Parser;

	/// setup the object, called by the resource loader
	const AnimKeyBuffer* SetupKeyBuffer(SizeT numKeysInBuffer);
	/// begin setting up clips, called by the resource loader
	void BeginSetupClips(SizeT numClips);
	/// access to anim clip for setup
	AnimClip& Clip(IndexT clipIndex);
	/// finish setting up clips, called by the resource loader
	void EndSetupClips();
	/// (re-)build the clip-index-map (maps names to clip indices
	void BuildClipIndexMap();

	AnimClip* animClips;
	std::map<const char*, int> clipIndexMap;
	AnimKeyBuffer* animKeyBuffer;
private:

};
