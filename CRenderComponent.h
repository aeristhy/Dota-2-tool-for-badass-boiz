#pragma once
#include "C_DOTAPlayer.h"

class CRenderComponent
{
public:
	__inline C_DOTAPlayer* GetPlayer()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return (C_DOTAPlayer*)*(__int64*)((char*)this + 0xFC8);
	}
};