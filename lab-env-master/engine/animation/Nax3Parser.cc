#include "config.h"
#include "Nax3Parser.h"
#include <cstdio>
#include <fstream>
#include "NaxStructs.h"
#include "AnimResource.h"

Nax3Parser::Nax3Parser()
{

}

Nax3Parser::~Nax3Parser()
{

}

Nax3Parser* Nax3Parser::Instance()
{
	static Nax3Parser inst;

	return &inst;
}

bool Nax3Parser::SetupFromNax3(const char* filename)
{
	//AnimResource* anim = new AnimResource();
	std::ifstream ifstream;
	ifstream.open(filename);
	if (!ifstream.is_open())
	{
		printf("Cannot load nax3 file \n");
		return false;
	}

	std::filebuf* pbuf = ifstream.rdbuf();
	std::size_t s = size_t(pbuf->pubseekoff(0, ifstream.end, ifstream.in));
	pbuf->pubseekpos(0, ifstream.in);
		
	char* ptr = new char[s];
	pbuf->sgetn(ptr, s);

	ifstream.close();
	
	Nax3Header* naxHeader = (Nax3Header*) ptr;

	ptr += sizeof(Nax3Header);

	if(naxHeader->numClips > 0)
	{
		//anim->BeginSetupClips(naxHeader->numClips);
		IndexT clipIndex;
		SizeT numClips = SizeT(naxHeader->numClips);
		Nax3Clip** clips = new Nax3Clip*[numClips];

		for(clipIndex = 0; clipIndex < numClips; clipIndex++)
		{
			Nax3Clip* naxClip = (Nax3Clip*)ptr;
			clips[clipIndex] = naxClip;
			ptr += sizeof(Nax3Clip);

			AnimClip clip;			
			clip.startKeyIndex = naxClip->startKeyIndex;
			clip.numKeys = naxClip->numKeys;
			clip.keyStride = naxClip->keyStride;
			clip.keyDuration = naxClip->keyDuration;
			clip.preInfinityType = InfinityType::Code(naxClip->preInfinityType);
			clip.postInfinityType = InfinityType::Code(naxClip->postInfinityType);
			clip.name = naxClip->name;
			IndexT eventIndex;
			for (eventIndex = 0; eventIndex < naxClip->numEvents; eventIndex++)
			{
				Nax3AnimEvent* naxEvent = (Nax3AnimEvent*)ptr;
				ptr += sizeof(Nax3AnimEvent);
				AnimEvent animEvent;
				animEvent.name = naxEvent->name;
				animEvent.category = naxEvent->category;
				animEvent.time = naxEvent->keyIndex * clip.keyDuration;
				clip.events.push_back(animEvent);
			}

			IndexT curveIndex;
			for(curveIndex = 0; curveIndex < naxClip->numCurves; curveIndex++)
			{
				Nax3Curve* naxCurve = (Nax3Curve*)ptr;
				ptr += sizeof(Nax3Curve);

				AnimCurve animCurve;
				animCurve.firstKeyIndex = naxCurve->firstKeyIndex;
				animCurve.isActive = naxCurve->isActive != 0;
				animCurve.isStatic = naxCurve->isStatic != 0;
				animCurve.curveType = CurveType::Code(naxCurve->curveType);
				animCurve.staticKey = Math::vector4(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyZ);

			}
		}
	}
	printf("Nax3 file loaded successfully \n");
	return true;
}
