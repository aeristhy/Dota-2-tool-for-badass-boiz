#pragma once

class CConVar
{
public:
	__inline char* GetConVarName()
	{
		return *(char**)((char*)this + 0x18);
	}
	__inline char* GetConVarDescription()
	{
		return *(char**)((char*)this + 0x20);
	}
	__inline char* GetCurrentValue_string()
	{
		return *(char**)((char*)this + 0x48);
	}
	__inline int isCConCommand()
	{
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
		return *(__int32*)((char*)this + 0x58);
	}
	__inline float GetCurrentValue_float()
	{
		return *(float*)((char*)this + 0x54);
	}
	//Different ConVars uses different types of value. 
	//That means bools like cl_particle_log_creates can be switched by SetValue(1 or 0)
	//But r_particle_timescale uses float value and SetValue(10) will not affect this CVar, but SetValue(10.0) will do
	__inline void SetValue(char* value)
	{
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
		*(float*)((char*)this + 0x54) = value;
	}
	__inline void SetValue(__int32 value) 
	{
		*(__int32*)((char*)this + 0x58) = value;
	}
	__inline char* GetSomeString()
	{
		return *(char**)((char*)this + 0xA8);
	}
};