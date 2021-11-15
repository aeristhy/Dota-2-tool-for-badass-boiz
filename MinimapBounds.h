#pragma once
#include "offsets.h"
struct Bounds2
{
	float x;
	float y;
};

class MinimapBounds
{
public:
	__inline Bounds2 GetBoundsMin()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		Bounds2 q;
		q.x = *(float*)((char*)this + MinimapBounds_x_min);
		q.y = *(float*)((char*)this + MinimapBounds_y_min);
		return q;
	}

	__inline Bounds2 GetBoundsMax()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		Bounds2 q;
		q.x = *(float*)((char*)this + MinimapBounds_x_max);
		q.y = *(float*)((char*)this + MinimapBounds_y_max);
		return q;
	}
};