#pragma once

#include "GlobalVars.h"

char __fastcall SelectableUnitCollidedWithCursor(CBaseEntity* Hero, const char* FigureOutWhat, float* Coordinate)
{
	if (UnderCursor && fuckingMatrix && fuckingMatrixValid)
	{
		undercursor_screen.x = 0;
		undercursor_screen.y = 0;

		long long temp = fuckingMatrix + 0x288;

		WorldToScreen(*(D3DVECTOR*)Coordinate, &undercursor_screen, (float*)temp, view.Width, view.Height);

	
		memset(undercursor_buff, 0, 256);

		strcat(undercursor_buff, FigureOutWhat);
		
		undercursor_screen_point.x = undercursor_screen.x - 100;
		undercursor_screen_point.y = undercursor_screen.y - 50;

		

		
	}
	return SelectableUnitCollidedWithCursor_Original(Hero, FigureOutWhat, Coordinate);
}