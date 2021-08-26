#pragma once
#include <libloaderapi.h>
#include "CEntityInstance.h"
#include "CGameEntitySystem.h"
#include "vector.h"


/*
Misc
*/

DWORD	trashbox = 0;

#define TeamSize		5
#define heroes_slots	10

#ifdef _DEBUG
#define xlam_slots		400
CBaseEntity* xlam[xlam_slots];
#endif
ID3DXFont* font;

bool	RenderInitiated = 0;
HMODULE	HackModuleHandle = 0;
HANDLE	HackThreadHandle_Init = 0;
HWND	ProcessWindowHandle = 0;
__int64	ProcessOldWndProc = 0;

void* d3d9Device[119];


/*
Menu vars
*/

bool DrawModifiersPanel = 0;
bool DrawHealthPanel = 0;
bool TrueHero = 0;
bool NoPings = 0;
bool enemyCD = 0;

/*
Entity
*/
CBaseEntity*	heroes[heroes_slots];
CBaseEntity*	LocalPlayer = 0;
char			LocalPlayerID = 0;
#ifdef _DEBUG
#define Modifiers_Cap 16
Modifier*		Modifiers[heroes_slots][Modifiers_Cap];
char			LastModifiersCount[heroes_slots][1];
char*			xlamValidToVisibleCheck[100][256];
#endif
DOTATeam_t	localPlayerTeam;
DOTATeam_t	EnemyTeam;


/*
settings visual
*/

DWORD	quad_color = 0x000000FF;
DWORD	quad_color2 = 0x00FFFF00;

/*
Hooks and it's stuff
*/
int		TrampoToBreak = 0;
char	PingCoordinatorOriginalCode[32];
char	PingCoordinatorHooked[32];
__int64* PingCoordinateWriter = 0;
Hud* HudPtrStolen = 0; //CHudDOTABase
bool	IsHookSet = 0;
bool	PingsRemoved = 0;
bool	siet = 0;
__int64 fuckingMatrix = 0;
bool	fuckingMatrixChanged = 0;
bool	fuckingMatrixValid = 0;
__int64 fuckingMatrixLastValue = 0;