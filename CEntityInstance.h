#pragma once
#include "vector.h"
#include "Schema.h"
#include "Color.h"

struct SchemaClassBinding {
	SchemaClassBinding* parent; // I THINK
	const char* bindingName; // ex: C_World
	const char* dllName; // ex: libclient.so
	const char* libName; // ex: client
};

enum OverlayFlags_t : unsigned long long
{
	BBOX = (1 << 2), //4
	ABSBOX = (1 << 5),//32
	RBOX = (1 << 6), //64
	ENTITYBOUNDS = (1 << 7), //128 - Looks like it depends on a byte being set in the class.
	ENTITYATTACHMENTS = (3 << 8), //BYTE1(flags) & 3
	ENTITYJOINTINFO = 0x40000,
	ENTITYSKELETON = (24 << 8), //BYTE1(flags) & 24 // kinda slow.
	ENTITYHITBOXES = (192 << 8), //BYTE1(flags) & 192
	BUDDHA = 0x40000000,
	ENTITYVIEWOFFSET = 0x800000000
};

enum DOTATeam_t : char {
	DOTA_TEAM_INVALID = -1,
	//DOTA_TEAM_FIRST = 2,
	DOTA_TEAM_RADIANT = 2, //Radiant team.
	DOTA_TEAM_DIRE = 3, 	//Dire team.
	DOTA_TEAM_NEUTRALS = 4, //Neutral.
	DOTA_TEAM_NOTEAM = 5,
	DOTA_TEAM_CUSTOM_1 = 6,
	DOTA_TEAM_CUSTOM_MIN = 6,
	DOTA_TEAM_CUSTOM_2 = 7,
	DOTA_TEAM_CUSTOM_3 = 8,
	DOTA_TEAM_CUSTOM_COUNT = 8,
	DOTA_TEAM_CUSTOM_4 = 9,
	DOTA_TEAM_CUSTOM_5 = 10,
	DOTA_TEAM_CUSTOM_6 = 11,
	DOTA_TEAM_CUSTOM_7 = 12,
	DOTA_TEAM_CUSTOM_8 = 13,
	DOTA_TEAM_CUSTOM_MAX = 13,
	DOTA_TEAM_COUNT = 14
};

const char* TeamName(DOTATeam_t teamID)
{
	switch (teamID)
	{
	case DOTA_TEAM_INVALID:
		return "DOTA_TEAM_INVALID";
	case DOTA_TEAM_RADIANT:
		return "RADIANT";
	case DOTA_TEAM_DIRE:
		return "DIRE";
	case DOTA_TEAM_NEUTRALS:
		return "NEUTRALS";
	case DOTA_TEAM_NOTEAM:
		return "NOTEAM";
	case DOTA_TEAM_CUSTOM_2:
		return "DOTA_TEAM_CUSTOM_2";
	case DOTA_TEAM_CUSTOM_3:
		return "DOTA_TEAM_CUSTOM_3";
	case DOTA_TEAM_CUSTOM_4:
		return "DOTA_TEAM_CUSTOM_4";
	case DOTA_TEAM_CUSTOM_5:
		return "DOTA_TEAM_CUSTOM_5";
	case DOTA_TEAM_CUSTOM_6:
		return "DOTA_TEAM_CUSTOM_6";
	case DOTA_TEAM_CUSTOM_7:
		return "DOTA_TEAM_CUSTOM_7";
	default: return "N/a team";
	}
}


struct RenderableInstance_t {
    uint8_t m_nAlpha;
};


/*

Address of signature = client.dll + 0x00B809E0
"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x49\x8B\x00\x48\x8B\x00\xFF\x90", "xx???xx???xxxx?xx?xx?xx?xx"
"48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B ? 49 8B ? 48 8B ? FF 90"
записывает в RAX сперва базоввый ренж каста абилки, а затем добавленный, и суммиру€, возвращает
*/

class Modifier {
public:
	__inline char* Name()
	{
		__int64* qq = (__int64*)(((char*)this) + 0x10);
		char* str = *(char**)qq;
		return str;
	}
	__inline DOTATeam_t GetTeamOwner()
	{
		return *(DOTATeam_t*)(((char*)this) + 0x80);
	}
#ifdef _DEBUG
	__inline DOTATeam_t SetTeamOwner(DOTATeam_t val)
	{
		*(DOTATeam_t*)(((char*)this) + 0x80) = val;
	}
#endif

};

class ModifierPool {
public:
	__inline Modifier* GetModifier(int index)
	{
		__int64 qq = (__int64)((*(__int64*)this) + index*8);
		return *(Modifier**)qq;
	}
	Modifier* operator[](int value)
	{
		return ((Modifier*)this + value);
	}
};

class Skeleton
{
public:
	__inline float* GetPos()
	{
		return (float*)((char*)this + 0x10);
	}
};

class ParticleMgr
{
public:
	__inline __int64* GetParticleHost() //CParticlesLibHost_Client
	{
		return (__int64*)((char*)this + 0x8);
	}
};

//исследовать оффсел 0x498

/*

Address of signature = client.dll + 0x019C327E
"\x89\x6F\x00\x44\x3B", "xx?xx"
"89 6F ? 44 3B"

rdi == CDOTA_Hud_AbilityPanel (hud with buttons and numbers)
A lot of useful info

*/

class CBaseAblity
{
public:
	/*__inline __int32 GetCastRange()
	{
		return CalculateCastRange((__int64*)this);
	}*/
	__inline char* GetName()
	{
		auto temp = *(char**)((char*)this + 0x10);
		return *((char**)((char*)temp + 0x18));
		//*(float*)((char*)this + 0x5A8);
	}
	__inline char GetLevel()
	{
		return *((char*)this + 0x5A0);
	}

	__inline float GetMaxCooldown()
	{
		return *(float*)((char*)this + 0x5AC);
	}
	__inline float GetLastCooldown()
	{
		return *(float*)((char*)this + 0x5A8);
	}

	__inline float GetManacost()
	{
		return *(float*)((char*)this + 0x5B0);
	}

	__inline char GetCharges()
	{
		return *((char*)this + 0x5CC);
	}

	__inline float GetLastChargeCooldown()
	{
		return *(float*)((char*)this + 0x5D0);
	}
};

class NPCInfo
{
public:
	char* GetNpcName()
	{
		return *(char**)((char*)this + 0x18) +14;
	}
	CBaseAblity* GetAbility(int i)
	{
		i+=1;
		return *(CBaseAblity**)((char*)this + i* 0x78);
	}
	char GetAbilityCount()
	{
		char count = 0;
		for (int i = 0; i < 16; i++)
		{		
			auto ab = this->GetAbility(i);
			auto nm = ab->GetName();
			
			if (!nm)//|| !*nm
				continue;
			else
			{
				auto npm = this->GetNpcName();
				//if (!memcmp(nm, npm, strlen(npm)))
					count += 1;
			}
		}
		return count;
	}
};

class Nekto2C8
{
public:

};

class NektoA40
{
public:
	__inline Nekto2C8 Get2C8()
	{
		return *(Nekto2C8*)((char*)this + 0x2C8);
	}
};

class CBaseEntity
{
public:
    virtual CSchemaClassBinding* Schema_DynamicBinding(void);
	__inline NPCInfo* GetNpcInfo()
	{
		return *(NPCInfo**)((char*)this + 0x10);
	}
	/*
00007FF9EA17D61F 48 8B 85 E0 00 00 00 mov         rax,qword ptr [this]  
00007FF9EA17D626 48 8B 40 10          mov         rax,qword ptr [rax+10h]  
	}
00007FF9EA17D62A 48 8D A5 C8 00 00 00 lea         rsp,[rbp+0C8h]  
00007FF9EA17D631 5F                   pop         rdi  
00007FF9EA17D632 5D                   pop         rbp  
00007FF9EA17D633 C3                   ret  

00007FF9EA17D61F 48 8B 85 E0 00 00 00 mov         rax,qword ptr [this]
00007FF9EA17D626 48 83 C0 10          add         rax,10h
	}
00007FF9EA17D62A 48 8D A5 C8 00 00 00 lea         rsp,[rbp+0C8h]
00007FF9EA17D631 5F                   pop         rdi
00007FF9EA17D632 5D                   pop         rbp
00007FF9EA17D633 C3                   ret
	*/



	__inline Skeleton* GetSleleton()
	{
		return (Skeleton*)*(__int64*)((char*)this + 0x310);
	}
	__inline __int32 GetMaxHealth()
	{
		return *(__int32*)((char*)this + 0x328);
	}
	__inline __int32 GetCurrentHealth()
	{
		return *(__int32*)((char*)this + 0x32C);
	}
	__inline DOTATeam_t GetTeam()
	{
		return *((DOTATeam_t*)this + 0x3AF);
	}
	__inline ParticleMgr* GetParticleMgr()
	{
		return ((ParticleMgr*)((char*)this + 0x490));
	}
	__inline NektoA40* GetA40()
	{
		return ((NektoA40*)((char*)this + 0xA40));
	}
	/*__inline bool IsLocalPlayer()
	{
		if (!*(__int32*)((char*)this + 0x0b48))
			return 0;
		return 1;
	}*/
	__inline char GetModifiersCount()
	{
		return *((char*)this + 0xE40);
		//Calve moved it 0xE20->0xE28 in summer 2021 (july) 
		//Calve moved it 0xE28->0xE40 in autumn 2021 (september)
	}

	__inline ModifierPool* GetModifiersPool()
	{
		return (ModifierPool*)((char*)this + 0xE48);
		//Calve moved it 0xE28->0xE30 in summer 2021 (july)
		//Calve moved it 0xE30->0xE48 in autumn 2021 (september) //compendium update?
	}
};


class Hud {
public:
	float* GetLastPingCoord()
	{
		return (float*)((char*)this + 0x17C);
	}
};

/*

Address of signature = client.dll + 0x0194FDD5
"\xE8\x00\x00\x00\x00\x48\x8B\x00\x00\x00\x48\x8B\x00\x00\x00\x48\x8B\x00\x00\x00\x48\x8B\x00\x00\x00\x48\x83\xC4\x00\x41\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x0F\xB7", "x????xx???xx???xx???xx???xxx?x?xxxxxxxxxxxxxx"
"E8 ? ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 83 C4 ? 41 ? C3 CC CC CC CC CC CC CC CC CC CC CC 0F B7"

*/


/*

Address of signature = particles.dll + 0x0001E650
"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x8B\x81\x00\x00\x00\x00\x49\x8B", "xx???xx???xx???xxxx?xx????xx"
"48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 8B 81 ? ? ? ? 49 8B"
particle render. 
rbx = 0 (possible to be zero?)
rbp = класс того вокруг чего рисуем
rsi = тоже самое, что и rbp?

Func(0,кака€-то неиспользуема€ хуйн€?,rbp,rsi)


^ код ниже вызывает хуйню выше
__int64 __fastcall sub_7FF8F14AE730(__int64 a1, unsigned int a2, __int64 a3)
{
  return sub_7FF8F14AE650(a1, -1i64, a2, a3);
}

client.dll+0x194FC57
*/

