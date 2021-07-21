#pragma once
#include <libloaderapi.h>
#include "CEntityInstance.h"
#include "CGameEntitySystem.h"
#include "vector.h"

bool	RenderInitiated			= 0;
HMODULE	HackModuleHandle		= 0;
HANDLE	HackThreadHandle_Init	= 0;
HWND	ProcessWindowHandle		= 0;
__int64	ProcessOldWndProc		= 0;

void*	d3d9Device[119];
int		TrampoToBreak = 0;
DWORD	trashbox = 0;

#define TeamSize		5
#define heroes_slots	10

#ifdef _DEBUG
#define xlam_slots		400
CBaseEntity* xlam[xlam_slots];
__int64* WorldParticleSystem = 0;
#endif
CBaseEntity*	heroes	[heroes_slots];
CBaseEntity*	LocalPlayer = 0;
ID3DXFont*		font;

bool	heroes_isSeen[heroes_slots];
__int64* CParticleSystemMgrPtr = 0;
__int64* StolenVar = 0;
__int64 fuckingMatrix = 0;
bool	fuckingMatrixChanged = 0;
bool	fuckingMatrixValid = 0;
__int64 fuckingMatrixLastValue = 0;

bool DrawHealthPanel = 0;
bool TrueHero = 0;

char PingCoordinatorOriginalCode[32];
char PingCoordinatorHooked[32];
__int64* PingCoordinateWriter = 0;
Hud* HudPtrStolen = 0; //CHudDOTABase
bool IsHookSet = 0;
int	 HookID = 0;
bool NoPings = 0;
bool PingsRemoved = 0;
bool enemyCD = 0;
bool siet = 0;

bool isMenuOpen = 0;

/*
settings visual
*/

DWORD	quad_color = 0x000000FF;
DWORD	quad_color2 = 0x00FFFF00;