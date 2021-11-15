#pragma once
#ifdef CrashCatcher

enum hazard_level {
	NoAttention = 0,
	NamPizda	= 1
};

const char* LevelName(hazard_level q)
{
	switch (q)
	{
	case 0:
		return "No matter";
	case 1: 
		return "Nam pizda";
	default: return "xui znaet";
	}
}

class kycb 
{
	HANDLE file = 0;
	char ErrorLogBuffer[1024];
public:
	kycb(const char* name)
	{
		_OFSTRUCT q;
		q.cBytes = sizeof(q);
		file = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	}
	void Append(const char* text, int len)
	{
		WriteFile(file, text, len, 0,0);
	}
	void Append(const char* function, const char* object, const char* comment, hazard_level lvl)
	{
		memset(ErrorLogBuffer, 0, 1024);
		auto q = sprintf(ErrorLogBuffer, "Func:%s\n%s %s\n%s\n", function, object, comment, LevelName(lvl));
		WriteFile(file, ErrorLogBuffer, q, 0, 0);
	}
};

kycb _log("xui_log_cyka");

#endif