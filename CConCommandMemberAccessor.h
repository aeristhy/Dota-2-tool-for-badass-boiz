#pragma once

class CConCommand
{
public:
	__inline char* GetConCommandName()
	{
		return *(char**)((char*)this + 0x18);
	}
	__inline char* GetConCommandDescription()
	{
		return *(char**)((char*)this + 0x20);
	}
};