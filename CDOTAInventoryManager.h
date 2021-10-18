#pragma once

class CDOTAGameAccountPlus
{
public:
	__inline bool MakeItWork()
	{
		if (this)
		{	
			*(__int64*)((char*)this + 0x2c) = 4;
			return true;
		}
		return false;
	}
};

class MoreUnnamedShit
{
public:
	__inline CDOTAGameAccountPlus* GetCDOTAGameAccountPlus()
	{
		if (this)
		return *(CDOTAGameAccountPlus**)((char*)this + 0x60);
	}
};

class UnnamedShit
{
public:
	__inline MoreUnnamedShit* GetMoreUnnamedShit()
	{
		if (this)
		return *(MoreUnnamedShit**)((char*)this + 0x28);
	}
};

class CGCClientSharedObjectCache
{
public:
	__inline UnnamedShit* GetSomeUnnamedShit()
	{
		if (this)
		return *(UnnamedShit**)((char*)this + 0x18);
	}
};

class CDOTAInventoryManager
{
public:
	__inline CGCClientSharedObjectCache* GetSomeSharedShit()
	{
		if (this)
		return (CGCClientSharedObjectCache*)*(__int64*)((char*)this + 0x1C8);
	}
};