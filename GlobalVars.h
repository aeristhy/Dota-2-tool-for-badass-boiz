#pragma once
#include <libloaderapi.h>
#include "CEntityInstance.h"
#include "CGameEntitySystem.h"
#include "vector.h"
#include "CDOTAInventoryManager.h"
#include "MinimapBounds.h"

struct vec2 {
	int x;
	int y;
};
int qwqe = 520;



/*
Misc
*/
MinimapBounds* MinimapBoundsManipulator = 0;
char* MinimapBounds_offset = 0;
char undercursor_buff[256];
vec2 undercursor_screen;
vec2 undercursor_screen_point;
DWORD undercursor_time = 0;
LPDIRECT3DDEVICE9 pDevice_ = 0;
DWORD	trashbox = 0;

#define TeamSize		5
#define heroes_slots	100

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
bool draw_square_at_3d_coords = 0;
bool disableinput = 0;
bool draw3dbox = 0;
bool UnderCursor = 0;
#ifdef _DEBUG
CDOTAInventoryManager* CDOTAInventoryMgr;
#endif 

/*
Entity
*/
CGameEntitySystem* GameEntitySystem = 0;
int				heroes_index[heroes_slots];
CBaseEntity*	heroes[heroes_slots];
CBaseEntity*	LocalPlayer = 0;
char			LocalPlayerID = 0;
//#ifdef _DEBUG
#define Modifiers_Cap 16
Modifier*		Modifiers[heroes_slots][Modifiers_Cap];
char			LastModifiersCount[heroes_slots][1];
char*			xlamValidToVisibleCheck[100][256];
//#endif
DOTATeam_t	localPlayerTeam;
DOTATeam_t	EnemyTeam;


/*
settings visual
*/

#define blyaaaa 240
char	PointsToDraw_Names[blyaaaa];//60 names with 4 chars ##q\0
float   pointstodraw[20][3];
int		how_much_squares = 0;
DWORD	quad_color = 0x000000FF;
DWORD	quad_color2 = 0x00FFFF00;
D3DVIEWPORT9 view;
byte color[4];


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
