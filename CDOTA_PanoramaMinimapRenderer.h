#pragma once
#include "vec2.h"
#include "offsets.h"

class PanoramaMinimapRenderer
{
public:
	vec2 GetMinimapSizeInPixel()
	{
		vec2 meow;
		meow.x = *(__int32*)((char*)this + MinimapBoundsInPixel_x);
		meow.y = *(__int32*)((char*)this + MinimapBoundsInPixel_y);
		return meow;
	}
};