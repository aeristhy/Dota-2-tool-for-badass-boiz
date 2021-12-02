#pragma once
//#include "Include/d3d9.h"
#include "Include/d3dx9math.h"
#include "GlobalVars.h"
#include "CConVar.h"
#define cint64 const __int64
#define cint   const int

//typedef HRESULT	(APIENTRY* t0)	(LPDIRECT3DDEVICE9 pDevice); rip (╥﹏╥)
typedef HRESULT(STDMETHODCALLTYPE* t0)(IDXGISwapChain*, unsigned int, unsigned int);

typedef void	(__thiscall* t1)(CGameEntitySystem* ecx, CBaseEntity* ptr, __int32 index);	//OnAddEntity
typedef void	(__thiscall* t2)(CGameEntitySystem* ecx, CBaseEntity* ptr, __int32 index);	//OnRemoveEntity
typedef __int32	(__thiscall* t3)(__int64*		ecx);								//CastRange, mother of god...........
typedef bool	(__thiscall* t4)(CBaseEntity*	ecx, DOTATeam_t team);				//IsVisibleByTeam
typedef void	(__fastcall* t5)(ParticleMgr* ecx, const char* particleName, cint _1, cint64 _0, cint64 _0_2,float* zero, cint64 _0_3);
typedef void   (__thiscall* t11)(__int64*, const char*, int, const char*, bool, int);
typedef __int64*(__fastcall* t6)(__int64* CParticleSystemMgrPtr, Nekto2C8 var,const char* particleName, char a4);
typedef __int64*(__fastcall* t7)(char* str0, char* str1);//test required. Snezh, i'm serious. Test it in nanoseconds. Dont forget, suka
typedef char	(__fastcall* t8)(NetworkVar_m_skeletonInstance* HeroSkeleton, char* reserved, char* Coordinate);
typedef void	(__fastcall* t9)(__int64 CInputService, int a2, int a3, int a4, unsigned int a5, CConVar* ConVar, __int64* SomethingICantExplain);
typedef void   (__fastcall* t10)(byte* color, const char* format, ...);







//t0		oEndScene			= 0; rip (╥﹏╥)
t0	    render_orig			= 0;
t1		OnAddEntityRet		= 0;
t2		OnRemoveEntityRet	= 0;
t3		CalculateCastRange	= 0;
t4		IsVisibleByTeam		= 0;
t5		DrawParticleOnEntity= 0;
t6		FindOrCreateParticleOrSomething = 0;
t7		stricmp_valve		= 0; //test required. Snezh, i'm serious. Test it in nanoseconds. Dont forget, suka
t8		SelectableUnitCollidedWithCursor_Original = 0;
t9		ConVarMainProcessor_orig = 0;
t10		ConColorMsg			= 0;
t11		EnterScope_orig		= 0;