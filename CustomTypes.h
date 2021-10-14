#pragma once
#include "Include/d3d9.h"
#include "Include/d3dx9math.h"
#include "GlobalVars.h"
#include "CConVar.h"
#define cint64 const __int64
#define cint   const int

typedef HRESULT	(APIENTRY* t0)	(LPDIRECT3DDEVICE9 pDevice);

typedef void	(__thiscall* t1)(CGameEntitySystem* ecx, CBaseEntity* ptr, __int32 index);	//OnAddEntity
typedef void	(__thiscall* t2)(CGameEntitySystem* ecx, CBaseEntity* ptr, __int32 index);	//OnRemoveEntity
typedef __int32	(__thiscall* t3)(__int64*		ecx);								//CastRange, mother of god...........
typedef bool	(__thiscall* t4)(CBaseEntity*	ecx, DOTATeam_t team);				//IsVisibleByTeam
typedef void	(__fastcall* t5)(ParticleMgr* ecx, const char* particleName, cint _1, cint64 _0, cint64 _0_2,float* zero, cint64 _0_3);
/*
error at 7FFFD22687AF
Code below. Rax == (__int64)-1

client.dll+1ED87AF - 0F10 00               - movups xmm0,[rax]
client.dll+1ED87B2 - 0F11 42 1C            - movups [rdx+1C],xmm0
client.dll+1ED87B6 - 0F10 48 10            - movups xmm1,[rax+10]
client.dll+1ED87BA - 0F11 4A 2C            - movups [rdx+2C],xmm1
client.dll+1ED87BE - 0F10 40 20            - movups xmm0,[rax+20]
client.dll+1ED87C2 - 0F11 42 3C            - movups [rdx+3C],xmm0

example

v38 = SetParticleOnEntity(rcx, "particles/ui_mouseactions/selected_ring.vpcf",	2 const, 0 const, 0 const, &v69,		0 const);// when mouse over ally struct
v31 = SetParticleOnEntity(rcx, "particles/speechbubbles/speech_voice.vpcf",		7 const, 0 const, 0 const, &v141,		0 const); 
	  SetParticleOnEntity(rcx, "particles/ui_mouseactions/range_display.vpcf",	1 const, 0 const, 0 const, &float_zero, 0 const);
 * client.dll+1AFFCE8 - 48 8D 8E 90040000     - lea rcx,[rsi+00000490] why ida thinks it is 0x124?
rsi == class of entity to draw around

*/


typedef __int64*(__fastcall* t6)(__int64* CParticleSystemMgrPtr, Nekto2C8 var,const char* particleName, char a4);
typedef __int64*(__fastcall* t7)(char* str0, char* str1);//test required. Snezh, i'm serious. Test it in nanoseconds. Dont forget, suka
typedef char	(__fastcall* t8)(NetworkVar_m_skeletonInstance* HeroSkeleton, char* reserved, char* Coordinate);
typedef void	(__fastcall* t9)(__int64 CInputService, int a2, int a3, int a4, unsigned int a5, CConVar* ConVar, __int64* SomethingICantExplain);
typedef void   (__fastcall* t10)(__int64* color, const char* format, ...);







t0		oEndScene			= 0;

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