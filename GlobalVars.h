#pragma once
#include <libloaderapi.h>
#include "CustomTypes.h"
#include "CEntityInstance.h"
#include "CGameEntitySystem.h"
#include "vector.h"
#include "vmt.h"

bool	RenderInitiated			= 0;

HMODULE	HackModuleHandle		= 0;
HANDLE	HackThreadHandle_Init	= 0;
HWND	ProcessWindowHandle		= 0;
__int64	ProcessOldWndProc		= 0;
ES		oEndScene				= 0;
qq		oHkCreateSomethingElseAboutSomethingElseSoShutYourFace = 0;
t0		OnAddEntityRet			= 0;
t1		OnRemoveEntityRet		= 0;
t2		CalculateCastRange		= 0;
void*	d3d9Device[119];
int		TrampoToBreak = 0;
DWORD	trashbox = 0;

#define TeamSize 5
#define heroes_slots 10
CBaseEntity*	heroes[heroes_slots];
CBaseEntity*	procceed[heroes_slots];
CBaseEntity* Choosen = 0;
DWORD	heroes_lasttimeseen[heroes_slots];
bool	heroes_isSeen[heroes_slots];
bool	heroes_allies[TeamSize];
DWORD	heroes_times[heroes_slots];
const char*	time_symbol[4];
__int64* StolenVar = 0;
__int64 fuckingMatrix = 0;
bool	fuckingMatrixChanged = 0;
bool	fuckingMatrixValid = 0;
__int64 fuckingMatrixLastValue = 0;



/*
settings visual
*/

DWORD	quad_color = 0x990000FF;
DWORD	quad_color2 = 0x99FFFF00;
int		quad_x = 0;
int		quad_y = 0;


//D3DXMATRIX* _pOut;
FLOAT      _fovy;
//FLOAT      _Aspect;
//FLOAT      _zn;
//FLOAT      _zf;

bool edit = false;

CGameEntitySystem* entity;

VMT* entityVMT;