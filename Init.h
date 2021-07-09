#pragma once
#include "GlobalVars.h"
#include "CustomTypes.h"

#include "gh_d3d9.h"

#include <Windows.h>
#include <d3d9.h>
#include "Include/d3dx9.h"
#include "connector.h"
#include "WndProc.h"
#include "Render.h"
#include "sig.h"
#include "interfaces.h"
#include "PatternFinder.h"
#pragma warning(disable : 4996)

void SetRenderingEnabled(void*, bool);

void OnAddEntity(CGameEntitySystem* ecx, CBaseEntity* ptr, EntityHandle index)
{
    
    bool alreadyExists = false;
    const char* typeName = ptr->Schema_DynamicBinding()->binaryName;
    if (strstr(typeName, "DOTA_Unit_Hero"))
    {

        for (int meow = 0; meow < heroes_slots; meow++)
        {
            if (!heroes[meow])
                continue;
#ifndef _DEBUG
            if (strstr(heroes[meow]->Schema_DynamicBinding()->binaryName, ptr->Schema_DynamicBinding()->binaryName))
            {
                alreadyExists = true;
                break;
            }

#endif
        }

        if (!alreadyExists)
        {
            for (int meow = 0; meow < heroes_slots; meow++)
            {
                if (heroes[meow])
                    continue;
                heroes[meow] = ptr;
#ifdef _DEBUG
                printf("\n[%d]%s %llx || i:%X", meow, typeName,ptr, index);
#endif
                break;
            }

        }
    }
    return OnAddEntityRet((__int64*)ecx, (__int64*)ptr, index);
}

void OnRemoveEntity(CGameEntitySystem* ecx, CBaseEntity* ptr, EntityHandle index)
{
    const char* typeName = ptr->Schema_DynamicBinding()->binaryName;

    //if (strstr(typeName, "DOTA_Unit_Hero")) 

    if (strstr(typeName, "DOTA_Unit_Hero"))
    {
        for (int meow = 0; meow < heroes_slots; meow++)
        {
            if (!heroes[meow])
                continue;
            if (heroes[meow] == ptr)
            {
                heroes[meow] = 0;
                break;
            }
        }
    }
    bool IsNoHeroes = 1;
    for (char i = 0; i < heroes_slots; i++)
        if (heroes[i])
        {
            IsNoHeroes = 0;
            break;
        }
    if (IsNoHeroes)
        LocalPlayer = 0;

    return OnRemoveEntityRet((__int64*)ecx, (__int64*)ptr, index);
}

void Init()
{
	d::Init();
 
    bool res = false;
	while (!res)
	{
        res = GetD3D9Device(d3d9Device, sizeof(d3d9Device));
	}
   
    while (!ProcessWindowHandle)
        ProcessWindowHandle = FindWindowA("SDL_app", 0);

#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$", "a+", stdout);
#endif

    auto InBattleCameraFunc     = PatternFinder::PatternScan((char*)"client.dll", "48 8B 01 48 8B 51 ?? 48 FF");
    auto WhereToGetViewMatrix   = PatternFinder::PatternScan((char*)"engine2.dll", "48 89 ?? ?? ?? ?? ?? 49 03 ?? 48 8B");
    auto OnAddEntityFunc        = PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 56 48 83 EC ?? 48 8B ?? 41 8B ?? B9 ?? ?? ?? ?? 48 8B");
    auto OnRemoveEntityFunc     = PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 41 8B ?? 25");
    CalculateCastRange      = (t2)PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 49 8B ?? 48 8B ?? FF 90");
    //i dont even remember what the //// is CalculateCastRange.....................
#ifdef _DEBUG
    if (InBattleCameraFunc == nullptr)///////////////////////
        printf("\nERROR: InBattleCameraFunc sig not found");
    else
        printf("\nInBattleCameraFunc: %llx", InBattleCameraFunc);
    if (WhereToGetViewMatrix == nullptr)/////////////////////
        printf("\nERROR: WhereToGetViewMatrix sig not found");
    else
        printf("\nWhereToGetViewMatrix: %llx", WhereToGetViewMatrix);
    if (OnAddEntityFunc == nullptr)//////////////////////////
        printf("\nERROR: OnAddEntityFunc sig not found");
    else
        printf("\nOnAddEntity: %llx", OnAddEntityFunc);
    if (OnRemoveEntityFunc == nullptr)///////////////////////
        printf("\nERROR: OnRemoveEntity sig not found");
    else
        printf("\nOnRemoveEntity: %llx", OnRemoveEntityFunc);
    if (CalculateCastRange == nullptr)///////////////////////
        printf("\nERROR: GetCastRange sig not found");
    else
        printf("\nGetCastRange: %llx", CalculateCastRange);
#endif
    
    hk.set_hook((char*)OnAddEntityFunc, 16, (char*)OnAddEntity, (char**)&OnAddEntityRet);
    hk.set_hook((char*)OnRemoveEntityFunc, 16, (char*)OnRemoveEntity, (char**)&OnRemoveEntityRet);
    hk.set_reg_stealer((char*)InBattleCameraFunc, 11, (char*)&StolenVar, r::rcx);
    hk.set_reg_stealer_reverse((char*)WhereToGetViewMatrix, 16, (char*)&fuckingMatrix, r::rax);
    TrampoToBreak = hk.set_hook((char*)d3d9Device[42], 15, (char*)hkEndScene, (char**)&oEndScene);

    ProcessOldWndProc = hk.set_WndProc_hook(ProcessWindowHandle, (__int64)WndProc);
}