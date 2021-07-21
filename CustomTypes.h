#pragma once
#include "Include/d3d9.h"
#include "Include/d3dx9math.h"
#include "GlobalVars.h"

typedef HRESULT	(APIENTRY* t0)	(LPDIRECT3DDEVICE9 pDevice);

typedef void	(__thiscall* t1)(CGameEntitySystem* ecx, CBaseEntity* ptr, __int32 index);	//OnAddEntity
typedef void	(__thiscall* t2)(CGameEntitySystem* ecx, CBaseEntity* ptr, __int32 index);	//OnRemoveEntity
typedef __int32	(__thiscall* t3)(__int64*		ecx);								//CastRange, mother of god...........
typedef bool	(__thiscall* t4)(CBaseEntity*	ecx, DOTATeam_t team);				//IsVisibleByTeam
typedef void	(__fastcall* t5)(ParticleMgr* ecx, const char* particleName, CBaseEntity* entity,int radius);
typedef __int64*(__fastcall* t6)(__int64* CParticleSystemMgrPtr, Nekto2C8 var,const char* particleName, char a4);

t0		oEndScene			= 0;

t1		OnAddEntityRet		= 0;
t2		OnRemoveEntityRet	= 0;
t3		CalculateCastRange	= 0;
t4		IsVisibleByTeam		= 0;
t5		DrawParticleOnEntity= 0;
t6		FindOrCreateParticleOrSomething = 0;