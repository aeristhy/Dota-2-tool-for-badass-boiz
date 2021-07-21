#pragma once
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
            && !strstr(typeName, "CDOTA_Item_Ward_Dispenser")


            )//0x00007ffbaf91a7b0 "C_DOTA_PortraitBaseModel"
            for (int meow = 0; meow < xlam_slots; meow++)
            {
                if (xlam[meow])
                    continue;
                xlam[meow] = ptr;
                break;
            }
    }
    /*if (strstr(typeName, "C_DOTAWorldParticleSystem"))
    {      
        WorldParticleSystem = (__int64*)ptr;
        printf("\nAdded WorldParticleSystem: %llx", WorldParticleSystem);
    }*/

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
    }
    return OnRemoveEntityRet(ecx, ptr, index);
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

    /*
    
Address of signature = client.dll + 0x00BB60D0
"\x48\x89\x00\x00\x00\x57\x48\x83\xEC\x00\x48\x8B\x00\x00\x00\x00\x00\x8B\xFA\x48\x8B\x00\x48\x85", "xx???xxxx?xx?????xxxx?xx"
"48 89 ? ? ? 57 48 83 EC ? 48 8B ? ? ? ? ? 8B FA 48 8B ? 48 85"
    */

    /* draws particle
Address of signature = client.dll + 0x01EB5B40
"\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x48\x89\x00\x00\x00\x55\x41\x00\x41\x00\x48\x8D\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\x00\x45\x8B", "xx???xx???xx???xx?x?xx???xxx????xx?xx"
"48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 55 41 ? 41 ? 48 8D ? ? ? 48 81 EC ? ? ? ? 4C 8B ? 45 8B"

rcx == ent->GetParticleMgr()
rdx == particles/ui_mouseactions/range_display.vpcf
rsi == вокруг чего рисуем?/hero class
rdi == radius
*/
    
    auto InBattleCameraFunc         = PatternFinder::PatternScan((char*)"client.dll", "48 8B 01 48 8B 51 ?? 48 FF");
    auto WTGViewMatrix              = PatternFinder::PatternScan((char*)"engine2.dll", "48 89 ?? ?? ?? ?? ?? 49 03 ?? 48 8B");
    auto OnAddEntityFunc            = PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 56 48 83 EC ?? 48 8B ?? 41 8B ?? B9 ?? ?? ?? ?? 48 8B");
    auto OnRemoveEntityFunc         = PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 41 8B ?? 25");
    auto WTGCParticleSystemMgr      = PatternFinder::PatternScan((char*)"client.dll", "41 0F ?? ?? 48 8B ?? 4C 8B ?? 41 B1");
    CalculateCastRange          = (t3)PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 49 8B ?? 48 8B ?? FF 90");
    IsVisibleByTeam             = (t4)PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? ?? ?? ?? ?? 8B FA 48 8B ? 48 85");
    DrawParticleOnEntity        = (t5)PatternFinder::PatternScan((char*)"client.dll", "48 89 ?? ?? ?? 48 89 ?? ?? ?? 48 89 ?? ?? ?? 55 41 ?? 41 ?? 48 8D ?? ?? ?? 48 81 EC ?? ?? ?? ?? 4C 8B ?? 45 8B");
    FindOrCreateParticleOrSomething = (t6)PatternFinder::PatternScan((char*)"particles.dll", "48 8B ? 57 48 81 EC ? ? ? ? 48 8B");
    PingCoordinateWriter  = (__int64*)PatternFinder::PatternScan((char*)"client.dll", "F3 41 ? ? ? ? ? ? ? F3 0F ? ? ? F3 41 ? ? ? ? ? ? ? F3 41 ? ? ? ? ? ? ? FF 90");
    /*
    
    Address of signature = particles.dll + 0x00038FD0
"\x48\x8B\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B", "xx?xxxx????xx"
"48 8B ? 57 48 81 EC ? ? ? ? 48 8B"
    */

    /*
   Address of signature = client.dll + 0x01EB625D
"\x41\x0F\x00\x00\x48\x8B\x00\x4C\x8B\x00\x41\xB1", "xx??xx?xx?xx"
"41 0F ? ? 48 8B ? 4C 8B ? 41 B1"
    */
    
    //"48 89 ?? ?? ?? 57 48 83 EC ?? 48 8D ?? ?? ?? ?? ?? 48 8B ?? 48 89 ?? 8B FA 48 83 C1 ?? F6 01 ?? 74 ?? 4C 8B ?? 41 F6 C0 ?? 75 ?? 49 83 E0 ?? EB ?? 41 8B ?? D1 E8 A8 ?? 75 ?? 49 83 E0 ?? 4D 8B ?? 4D 85 ?? 75 ?? E8 ?? ?? ?? ?? 40 F6 C7 ?? 74 ?? BA ?? ?? ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 48 8B ?? 48 8B ?? ?? ?? 48 83 C4 ?? 5F C3"
    //some prticle bullshit processor


    
    //i dont even remember what the //// is CalculateCastRange.....................
#ifdef _DEBUG
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
#endif
    

    hk.set_hook((char*)OnAddEntityFunc, 16, (char*)OnAddEntity, (char**)&OnAddEntityRet);
    hk.set_hook((char*)OnRemoveEntityFunc, 16, (char*)OnRemoveEntity, (char**)&OnRemoveEntityRet);
    hk.set_reg_stealer((char*)InBattleCameraFunc, 11, (char*)&StolenVar, r::rcx);
    hk.set_reg_stealer_reverse((char*)WTGViewMatrix, 16, (char*)&fuckingMatrix, r::rax);
    hk.set_reg_stealer((char*)WTGCParticleSystemMgr, 17, (char*)&CParticleSystemMgrPtr, r::rcx);
    TrampoToBreak = hk.set_hook((char*)d3d9Device[42], 15, (char*)hkEndScene, (char**)&oEndScene);

    ProcessOldWndProc = hk.set_WndProc_hook(ProcessWindowHandle, (__int64)WndProc);

    

}