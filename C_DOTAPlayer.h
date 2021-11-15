#pragma once
#include "offsets.h"
#define val32 *(__int32*)((char*)this

class C_DOTAPlayer
{
public:
	__inline __int32 GetHeroEntityIndex()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return val32+ C_DOTAPlayer_HeroIndexOffset);
	}
};