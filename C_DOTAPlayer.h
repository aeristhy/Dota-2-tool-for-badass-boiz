#pragma once
#include "offsets.h"
#define val32 *(__int32*)((char*)this

class C_DOTAPlayer
{
public:
	__inline __int32 GetHeroEntityIndex()
	{
		return val32+ C_DOTAPlayer_HeroIndexOffset);
	}
};