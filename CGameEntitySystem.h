#pragma once

#include "CEntityInstance.h"
#include "C_DOTAPlayer.h"
#define EntityHandle unsigned int

class PlayerPoolManipulator
{
public:
	__inline int GetPlayerHighestIndex()
	{
		for (int i = 2; i < 100; i++)
		{
			auto player = *(C_DOTAPlayer**)((char*)this + (0x78 * i));
			if (!player)
				return i -2;
		}
	}

	__inline C_DOTAPlayer* GetPlayerByPlayerIndex(int i)
	{
		return *(C_DOTAPlayer**)((char*)this + (0x78 * (i + 2)));
	}
};

class CGameEntitySystem {
public:
	__inline PlayerPoolManipulator* GetPlayerPool()
	{
		return *(PlayerPoolManipulator**)((char*)this + 0x10);
	}
};