#pragma once

#include "pch.h"
#include "CGameEntitySystem.h"
#include "IAppSystem.h"
#include "vmt.h"
#include "sig.h"

void* GetInterface(const char* dllname, const char* interfacename) {
	auto mhandle = GetModuleHandleA(dllname);
	if (!mhandle)
	{
		printf("\nnot found %s loaded, trying to load ... ", dllname);
		mhandle = LoadLibraryA(dllname);
		if (!mhandle)
		{
			printf("fail. Sleeping~");
			Sleep(-1);
		}
		else
			printf("ok");
	}
	CreateInterfaceFn CreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(mhandle, "CreateInterface"));

	int returnCode = 0;
	void* _interface = CreateInterface(interfacename, &returnCode);

	return _interface;
}

//IEngineClient* engine;
////IVPanel* panel;
//CParticleSystemMgr* particleMgr;


