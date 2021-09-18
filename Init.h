#pragma once
#include "CustomTypes.h"

#include "gh_d3d9.h"

#include <Windows.h>
#include <d3d9.h>
#include "Include/d3dx9.h"
#include "connector.h"
#include "WndProc.h"
#include "Render.h"
#include "interfaces.h"
#include "PatternFinder.h"
#pragma warning(disable : 4996)


#include <iostream>
#include <fstream>

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
                printf("\n[%d]%s \t%llx || i:%X", meow, typeName, ptr, index);
#endif
                break;
            }

        }
    }
#ifdef _DEBUG
    else
    {
        if (!strstr(typeName, "Portrait") && !strstr(typeName, "C_World") && !strstr(typeName, "C_DynamicPropClientside")
            && !strstr(typeName, "C_CaptureCallbackHandler") && !strstr(typeName, "C_GlobalLight") && !strstr(typeName, "C_DOTAGamerulesProxy")
            && !strstr(typeName, "C_InfoPlayerStartBadGuys") && !strstr(typeName, "C_FogController") && !strstr(typeName, "C_TonemapController2")
            && !strstr(typeName, "C_DOTAGameManagerProxy") && !strstr(typeName, "C_DOTA_MapTree") && !strstr(typeName, "C_DOTACameraBounds")
            && !strstr(typeName, "C_InfoPlayerStartGoodGuys") && !strstr(typeName, "C_RagdollManager") && !strstr(typeName, "CAmbientCreatures")
            && !strstr(typeName, "C_DOTAWorldParticleSystem") && !strstr(typeName, "C_DOTATeam") && !strstr(typeName, "C_DOTABaseGameMode")
            && !strstr(typeName, "C_EnvDeferredLight") && !strstr(typeName, "C_DOTA_LightInfo") && !strstr(typeName, "C_DOTAFogOfWarTempViewers")
            && !strstr(typeName, "C_EnvSky") && !strstr(typeName, "C_PointCamera") && !strstr(typeName, "C_DOTAReflectionSkybox")
            && !strstr(typeName, "C_ParticleSystem") && !strstr(typeName, "C_DOTA_Ability") && !strstr(typeName, "C_Env")
            && !strstr(typeName, "C_DOTA_Item") && !strstr(typeName, "C_DOTABaseAbility") && !strstr(typeName, "CDOTA_Ability_")
            && !strstr(typeName, "C_DOTAWearableItem") && !strstr(typeName, "C_DOTASceneEntity") && !strstr(typeName, "CInfoTarget")
            && !strstr(typeName, "C_DOTA_Roshan") && !strstr(typeName, "CInfoWorldLayer") && !strstr(typeName, "C_DynamicProp")
            && !strstr(typeName, "C_TriggerBuoyancy") && !strstr(typeName, "C_DOTA_NeutralSpawner")
            && !strstr(typeName, "CDOTA_Item_Ward_Dispenser") && !strstr(typeName, "CAdditionalWearable")


            )
            for (int meow = 0; meow < xlam_slots; meow++)
            {
                if (xlam[meow])
                    continue;
                xlam[meow] = ptr;
                break;
            }
    }

#endif
    return OnAddEntityRet(ecx, ptr, index);
}

void OnRemoveEntity(CGameEntitySystem* ecx, CBaseEntity* ptr, EntityHandle index)
{
    const char* typeName = ptr->Schema_DynamicBinding()->binaryName;

    if (strstr(typeName, "DOTA_Unit_Hero"))
    {
        for (int meow = 0; meow < heroes_slots; meow++)
        {
            if (!heroes[meow])
                continue;
            if (heroes[meow] == ptr)
            {
                heroes[meow] = 0;
#ifdef _DEBUG
                for (char i = 0; i < Modifiers_Cap; i++)
                    Modifiers[meow][i] = 0;
#endif
                break;
            }
        }
    }
#ifdef _DEBUG
    else
    {
        for (int meow = 0; meow < xlam_slots; meow++)
        {
            if (!xlam[meow])
                continue;
            if (xlam[meow] == ptr)
            {
                xlam[meow] = 0;
                break;
            }
        }
    }
    /*if (strstr(typeName, "C_DOTAWorldParticleSystem"))
    {
        printf("\nRemoved WorldParticleSystem: %llx", ptr);
        WorldParticleSystem = 0;
    }*/
#endif
    bool IsNoHeroes = 1;
    for (char i = 0; i < heroes_slots; i++)
        if (heroes[i])
        {
            IsNoHeroes = 0;
            break;
        }
    if (IsNoHeroes)
    {
        LocalPlayer = 0;
        LocalPlayerID = 0;
//#ifdef _DEBUG
//        for (char i = 0; i < heroes_slots; i++)
//            for (char q = 0; q < Modifiers_Cap; q++)
//                Modifiers[i][q] = 0;
//#endif
    }
    return OnRemoveEntityRet(ecx, ptr, index);
}

void Init()
{ 
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
  
    auto InBattleCameraFunc         = PatternFinder::PatternScan((char*)"client.dll",   "48 8B 01 48 8B 51 ?? 48 FF");
    auto WTGViewMatrix              = PatternFinder::PatternScan((char*)"engine2.dll",  "48 89 ?? ?? ?? ?? ?? 49 03 ?? 48 8B");
    auto OnAddEntityFunc            = PatternFinder::PatternScan((char*)"client.dll",   "48 89 ?? ?? ?? 56 48 83 EC ?? 48 8B ?? 41 8B ?? B9 ?? ?? ?? ?? 48 8B");
    auto OnRemoveEntityFunc         = PatternFinder::PatternScan((char*)"client.dll",   "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 41 8B ?? 25");
    auto WTGCParticleSystemMgr      = PatternFinder::PatternScan((char*)"client.dll",   "41 0F ?? ?? 48 8B ?? 4C 8B ?? 41 B1");
    IsVisibleByTeam = (t4)PatternFinder::PatternScan((char*)"client.dll",               "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? ?? ?? ?? ?? 8B FA 48 8B ? 48 85");
#ifdef _DEBUG
    CalculateCastRange                    = (t3)PatternFinder::PatternScan((char*)"client.dll",       "48 89 ?? ?? ?? 48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 49 8B ?? 48 8B ?? FF 90");
    DrawParticleOnEntity                  = (t5)PatternFinder::PatternScan((char*)"client.dll",       "48 89 ?? ?? ?? 48 89 ?? ?? ?? 48 89 ?? ?? ?? 55 41 ?? 41 ?? 48 8D ?? ?? ?? 48 81 EC ?? ?? ?? ?? 4C 8B ?? 45 8B");
    FindOrCreateParticleOrSomething       = (t6)PatternFinder::PatternScan((char*)"particles.dll",    "48 8B ? 57 48 81 EC ? ? ? ? 48 8B");
    PingCoordinateWriter            = (__int64*)PatternFinder::PatternScan((char*)"client.dll",       "F3 41 ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 41 ?? ?? ?? ?? ?? ?? ?? FF 90");
    auto ParticleNameCutter                   = PatternFinder::PatternScan((char*)"particles.dll",    "0F B6 ?? 4C 8B ?? 44 8B");
    stricmp_valve                         = (t7)PatternFinder::PatternScan((char*)"tier0.dll",        "4C 8B ?? 48 3B ?? 74 ?? 48 85");
    /*

Address of signature = tier0.dll + 0x00008E10
"\x4C\x8B\x00\x48\x3B\x00\x74\x00\x48\x85", "xx?xx?x?xx"
"4C 8B ? 48 3B ? 74 ? 48 85"

*/
 /*sub_7FFFD226A100((v5 + 0x124), "particles/ui_mouseactions/range_display.vpcf", 1u, 0i64, 0, &float_zero, 0i64);
 * client.dll+1AFFCE8 - 48 8D 8E 90040000     - lea rcx,[rsi+00000490] why ida thinks it is 0x124?
rsi == class of entity to draw around

Address of signature = client.dll + 0x01EDA100
"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x55\x41\x00\x41\x00\x48\x8D\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\x00\x45\x8B", "xx???xx???xx???xx?x?xx???xxx????xx?xx"
"48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 55 41 ? 41 ? 48 8D ? ? ? 48 81 EC ? ? ? ? 4C 8B ? 45 8B"


 */

#pragma warning(disable : 4477) //to prevent flood in IDE output

    if (InBattleCameraFunc == nullptr)///////////////////////
        printf("\nERROR: InBattleCameraFunc sig not found");
    else
        printf("\nInBattleCameraFunc: \t%llx", InBattleCameraFunc);
    if (WTGViewMatrix == nullptr)/////////////////////
        printf("\nERROR: WTGViewMatrix sig not found");
    else
        printf("\nWTGViewMatrix: \t%llx", WTGViewMatrix);
    if (OnAddEntityFunc == nullptr)//////////////////////////
        printf("\nERROR: OnAddEntityFunc sig not found");
    else
        printf("\nOnAddEntity: \t%llx", OnAddEntityFunc);
    if (OnRemoveEntityFunc == nullptr)///////////////////////
        printf("\nERROR: OnRemoveEntity sig not found");
    else
        printf("\nOnRemoveEntity: \t%llx", OnRemoveEntityFunc);
    if (CalculateCastRange == nullptr)///////////////////////
        printf("\nERROR: GetCastRange sig not found");
    else
        printf("\nGetCastRange: \t%llx", CalculateCastRange);
    if (IsVisibleByTeam == nullptr)///////////////////////
        printf("\nERROR: IsVisibleByTeam sig not found");
    else
        printf("\nIsVisibleByTeam: \t%llx", IsVisibleByTeam);
    if (DrawParticleOnEntity == nullptr)///////////////////////
        printf("\nERROR: DrawParticleOnEntity sig not found");
    else
        printf("\nDrawParticleOnEntity: \t%llx", DrawParticleOnEntity);
    if (WTGCParticleSystemMgr == nullptr)///////////////////////
        printf("\nERROR: WTGCParticleSystemMgr sig not found");
    else
        printf("\nWTGCParticleSystemMgr: \t%llx", WTGCParticleSystemMgr);
    if (FindOrCreateParticleOrSomething == nullptr)///////////////////////
        printf("\nERROR: FindOrCreateParticleOrSomething sig not found");
    else
        printf("\nFindOrCreateParticleOrSomething: \t%llx", FindOrCreateParticleOrSomething);
    if (PingCoordinateWriter == nullptr)///////////////////////
        printf("\nERROR: FindOrCreateParticleOrSomething sig not found");
    else
        printf("\nPingCoordinateWriter: \t%llx", PingCoordinateWriter);
    if (ParticleNameCutter == nullptr)///////////////////////
        printf("\nERROR: ParticleNameCutter sig not found");
    else
        printf("\nParticleNameCutter: \t%llx", ParticleNameCutter);
    if (stricmp_valve == nullptr)///////////////////////
        printf("\nERROR: stricmp_valve sig not found");
    else
        printf("\nstricmp_valve: \t%llx", stricmp_valve);
#endif
    

    hk.set_hook((char*)OnAddEntityFunc, 16, (char*)OnAddEntity, (char**)&OnAddEntityRet);
    hk.set_hook((char*)OnRemoveEntityFunc, 16, (char*)OnRemoveEntity, (char**)&OnRemoveEntityRet);
    //hk.set_reg_stealer((char*)InBattleCameraFunc, 11, (char*)&StolenVar, r::rcx);
    hk.set_reg_stealer_reverse((char*)WTGViewMatrix, 16, (char*)&fuckingMatrix, r::rax);
    //hk.set_reg_stealer((char*)WTGCParticleSystemMgr, 17, (char*)&CParticleSystemMgrPtr, r::rcx);
    TrampoToBreak = hk.set_hook((char*)d3d9Device[42], 15, (char*)hkEndScene, (char**)&oEndScene);

    ProcessOldWndProc = hk.set_WndProc_hook(ProcessWindowHandle, (__int64)WndProc);

    

}