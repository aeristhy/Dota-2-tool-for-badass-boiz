#pragma once

#include "CEntityInstance.h"
#include "C_DOTAPlayer.h"
#include "PatternFinder.h"
#include "insreader.h"
#include "GlobalVars.h"
#define EntityHandle unsigned int

void RefreshGameSystem();

class PlayerPoolManipulator
{
public:
	__inline int GetPlayerHighestIndex()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		if (!this)
			return 0;

		for (int i = 2; i < 100; i++)
		{
			auto player = *(C_DOTAPlayer**)((char*)this + (0x78 * i));
			if (!player)
				return i -2;
		}
	}

	__inline C_DOTAPlayer* GetPlayerByPlayerIndex(int i)
	{
		if (!this)
		{
#ifdef CrashCatcher
				_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
				return 0;
		}
		return *(C_DOTAPlayer**)((char*)this + (0x78 * (i + 2)));
	}
};

class CGameEntitySystem {
public:
	__inline PlayerPoolManipulator* GetPlayerPool()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		if (this)
			return *(PlayerPoolManipulator**)((char*)this + 0x10);
		else
		{
			RefreshGameSystem();
			return 0;
		}
	}
};

CGameEntitySystem* GameEntitySystem = 0;

void RefreshGameSystem()
{
	long long SomeKindOfPool_offset = (long long)PatternFinder::PatternScan("client.dll", "48 8B ? ? ? ? ? 8B 0D ? ? ? ? BE");
	if (SomeKindOfPool_offset)
	{
		auto SomeKindOfPool_ptr = GetAddressFromInstruction(SomeKindOfPool_offset, 3, 7);
		if (SomeKindOfPool_ptr)
			GameEntitySystem = *(CGameEntitySystem**)SomeKindOfPool_ptr;
	}
}