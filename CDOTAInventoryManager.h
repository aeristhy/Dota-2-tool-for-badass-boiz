#pragma once

class CDOTAGameAccountPlus
{
public:
	__inline void MakeItWork()
	{
		*(__int64*)((char*)this + 0x2c) = 4;
	}
};

class MoreUnnamedShit
{
public:
	__inline CDOTAGameAccountPlus* GetCDOTAGameAccountPlus()
	{
		return *(CDOTAGameAccountPlus**)((char*)this + 0x60);
	}
};

class UnnamedShit
{
public:
	__inline MoreUnnamedShit* GetMoreUnnamedShit()
	{
		return *(MoreUnnamedShit**)((char*)this + 0x28);
	}
};

class CGCClientSharedObjectCache
{
public:
	__inline UnnamedShit* GetSomeUnnamedShit()
	{
		return *(UnnamedShit**)((char*)this + 0x18);
	}
};

class CDOTAInventoryManager
{
public:
	__inline CGCClientSharedObjectCache* GetSomeSharedShit()
	{
		return (CGCClientSharedObjectCache*)*(__int64*)((char*)this + 0x190);
	}
};