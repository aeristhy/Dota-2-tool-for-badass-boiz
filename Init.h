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
#ifdef _DEBUG
    printf("\nEntity added: %s [%d]%s", typeName,ptr->GetTeam(), TeamName(ptr->GetTeam()));
#endif
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
                printf("\n[%d]%s %llx", meow, typeName,ptr);
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

    TrampoToBreak = hk.set_hook((char*)d3d9Device[42], 15, (char*)hkEndScene, (char**)&oEndScene);
                    //hk.set_hook((char*)GetProcAddress(GetModuleHandleA("d3dx9_43.dll"), "D3DXMatrixPerspectiveFovLH"), 15, (char*)hk_D3DXMatrixPerspectiveFovLH,(char**)&oHkCreateSomethingElseAboutSomethingElseSoShutYourFace);
    


#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$", "a+", stdout);
#endif
    //printf("\n%llx", PatternFinder::PatternScan((char*)"client.dll", "48 8B 01 48 8B 51 ?? 48 FF")); //-- робит, юзаем
    auto qq = PatternFinder::PatternScan((char*)"client.dll", "48 8B 01 48 8B 51 ?? 48 FF");
    auto q2 = PatternFinder::PatternScan((char*)"engine2.dll", "48 89 ?? ?? ?? ?? ?? 49 03 ?? 48 8B");

#ifdef _DEBUG
    printf("\n%llx", q2);
#endif

    hk.set_reg_stealer          ((char*)qq,11,  (char*)&StolenVar,      r::rcx); 
    hk.set_reg_stealer_reverse  ((char*)q2, 16, (char*)&fuckingMatrix,  r::rax);

    /*
    //////////////////////////////////////////////
    Address of signature = client.dll + 0x0099E5DF
"\xFF\x90\x00\x00\x00\x00\x84\xC0\x74\x00\xFF\x83", "xx????xxx?xx"
"FF 90 ? ? ? ? 84 C0 74 ? FF 83" 16 bytes

Address of signature = client.dll + 0x0099E590
"\x48\x89\x00\x00\x00\x56\x48\x83\xEC\x00\x48\x8B\x00\x41\x8B\x00\xB9\x00\x00\x00\x00\x48\x8B", "xx???xxxx?xx?xx?x????xx"
"48 89 ? ? ? 56 48 83 EC ? 48 8B ? 41 8B ? B9 ? ? ? ? 48 8B"
/////////////////////////////


Address of signature = client.dll + 0x0099E630
"\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x41\x8B\x00\x25", "xx???xxxx?xx?xx?x"
"48 89 ? ? ? 57 48 83 EC ? 48 8B ? 41 8B ? 25"


    */

    auto OnAddEntityFunc = PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 56 48 83 EC ?? 48 8B ?? 41 8B ?? B9 ?? ?? ?? ?? 48 8B");
    auto OnRemoveEntityFunc = PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 41 8B ?? 25");
    CalculateCastRange = (t2)PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 49 8B ?? 48 8B ?? FF 90");
#ifdef _DEBUG
    if (OnAddEntityFunc == nullptr)
        printf("\nERROR: OnAddEntity sig not found");
    else
        printf("\nOnAddEntity: %llx", OnAddEntityFunc);
    if (OnRemoveEntityFunc == nullptr)
        printf("\nERROR: OnRemoveEntity sig not found");
    else
        printf("\nOnRemoveEntity: %llx", OnRemoveEntityFunc);
    if (CalculateCastRange == nullptr)
        printf("\nERROR: GetCastRange sig not found");
    else
        printf("\nGetCastRange: %llx", CalculateCastRange);


#endif
    
    hk.set_hook((char*)OnAddEntityFunc, 16, (char*)OnAddEntity, (char**)&OnAddEntityRet);
    hk.set_hook((char*)OnRemoveEntityFunc, 16, (char*)OnRemoveEntity, (char**)&OnRemoveEntityRet);

    //GetModules();
    //entityVMT = new VMT(entity); //loads CGameEntitySystem VMT into vmt.entity
    ////entityVMT->HookVMT(OnAddEntity, 14);//14
    //entityVMT->HookVMT(OnRemoveEntity, 15);//15
    //entityVMT->ApplyVMT(entity);

    ProcessOldWndProc = hk.set_WndProc_hook(ProcessWindowHandle, (__int64)WndProc);
}