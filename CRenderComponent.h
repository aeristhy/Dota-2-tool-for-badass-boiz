#pragma once
#include "C_DOTAPlayer.h"

class CRenderComponent
{
public:
	__inline C_DOTAPlayer* GetPlayer()
	{
		return (C_DOTAPlayer*)*(__int64*)((char*)this + 0xFC8);
	}
};