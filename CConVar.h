#pragma once

class CConVar
{
public:
	__inline char* GetConVarName()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return *(char**)((char*)this + 0x18);
	}
	__inline char* GetConVarDescription()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return *(char**)((char*)this + 0x20);
	}
	__inline char* GetCurrentValue_string()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return *(char**)((char*)this + 0x48);
	}
	__inline int isCConCommand()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		int priznaki = 0;
		if (*(__int64*)((char*)this + 0x58) > 0x7FFF00000000)
		{
			priznaki++;
			if (this->GetCurrentValue_float() == 0)
				priznaki++;

			if (*(__int64*)((char*)this + 0x48) > 0x7FFF00000000)
				priznaki++;
		}
		return priznaki;
	}
	__inline __int32 GetCurrentValue__int32()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return *(__int32*)((char*)this + 0x58);
	}
	__inline float GetCurrentValue_float()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return *(float*)((char*)this + 0x54);
	}
	//Different ConVars uses different types of value. 
	//That means bools like cl_particle_log_creates can be switched by SetValue(1 or 0)
	//But r_particle_timescale uses float value and SetValue(10) will not affect this CVar, but SetValue(10.0) will do
	__inline void SetValue(char* value)
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		char* q;
		auto w = strlen(value);
		for (char i = 0; i < w; i++)
		{
			q = *(char**)((char*)this + 0x48);
			q[i] = value[i];
		}
		q[w + 1] = 0;
	}
	__inline void SetValue(float value)
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		*(float*)((char*)this + 0x54) = value;
	}
	__inline void SetValue(__int32 value) 
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		*(__int32*)((char*)this + 0x58) = value;
	}
	__inline char* GetSomeString()
	{
#ifdef CrashCatcher
		if (!this)
			_log.Append(__func__, "this", "is null", hazard_level::NamPizda);
#endif
		return *(char**)((char*)this + 0xA8);
	}
};