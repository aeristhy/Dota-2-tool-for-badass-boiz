#pragma once

#ifdef CrashCatcher
#include "Logger.h"
#endif

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
#include "SelectableUnitCollidedWithCursor.h"
#include "CVarChangeCallback.h"
#include "CDOTAInventoryManager.h"
#include "EnterScope.h"
#pragma warning(disable : 4996)


#include <iostream>
#include <fstream>

void SetRenderingEnabled(void*, bool);

//void QuerySignatureFind
bool fkenMonkey = 0;
void OnAddEntity(CGameEntitySystem* ecx, CBaseEntity* ptr, EntityHandle index)
{
    bool monkey2 = false;
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
                heroes_index[meow] = index;
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
            && !strstr(typeName, "CDOTA_Item_Ward_Dispenser") && !strstr(typeName, "CAdditionalWearable") && !strstr(typeName, "CDOTA_Item_")


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
                heroes_index[meow] = 0;
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
        if ((__int64)heroes[i] > (__int64)1)
        {
            IsNoHeroes = 0;
            break;
        }
    if (IsNoHeroes)
    {
        LocalPlayer = 0;
        LocalPlayerID = 0;
        for (char i = 0; i < heroes_slots; i++)
            LastModifiersCount[i][0] = 0;
        fkenMonkey = 0;
//#ifdef _DEBUG
//        for (char i = 0; i < heroes_slots; i++)
//            for (char q = 0; q < Modifiers_Cap; q++)
//                Modifiers[i][q] = 0;
//#endif
    }
    return OnRemoveEntityRet(ecx, ptr, index);
}



__int64 GetAddressFromInstruction(__int64 instruction,char offset,char len, int extra_offset = 0)
/*example: 7FF965D651E2  movzx r14d,word ptr [7FF968B606D8]
                               44 0F B7 35    |EE B4 DF 02| (it's i32 value == 02 DF B4 EE)
                                              """""""""""""\---> this part is relative address
                                                                i should calculate instruction addres + this part
                                                                instruction_address = a
                                                                relative_address_part_offset = a + 4
                                                                relative_address    = c
                                                                absolute_address = *(__int32*)relative_address_part_offset + instruction_address + instruction_size
so 
instruction_address = (__int64)7FF965D651E2
relative address    = (__int32)02DFB4EE
target address      = (__int64)7FF968B606D8
7FF965D651E2 + 02DFB4EE + 8 =  7FF968B606D8
*/

{
    auto meow = instruction;
    if (extra_offset != 0)
        meow += extra_offset;

    auto relative_address = *(__int32*)(meow + offset);
    return meow + relative_address + len;
}

#define cstrAndLen(var) var,sizeof(var)

void Init()
{ 

    bool res = false;
	while (!res)
	{
        res = GetD3D9Device(d3d9Device, sizeof(d3d9Device));
	}
#ifdef CrashCatcher
    _log.Append(cstrAndLen("start of log\n\n"));
#endif

    while (!ProcessWindowHandle)
        ProcessWindowHandle = FindWindowA("SDL_app", 0);

#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$", "a+", stdout);
#endif
    /*
    
    void (Class_Name::* FuncPointer)() = &Class_Name::Func_Name;

FuncPointer ������ �� ����� ������.
    
    */
    
    //printf("\n\n\n%llx\n\n\n", zalupka.GetHeroIndex);

    color[0] = 0;
    color[1] = 0xEA;
    color[2] = 0xAE;
    color[3] = 0xFF;
    char* xuinya = (char*)PatternFinder::PatternScan((char*)"client.dll", "45 33 ?? 0F 10 ?? ?? 66 48");

    /*__int32 ptr = *(__int32*)(xuinya - 10);
    char* instr = ((char*)xuinya - 13);
    */
#ifndef _DEBUG
    CDOTAInventoryManager* CDOTAInventoryMgr = (CDOTAInventoryManager*)GetAddressFromInstruction((__int64)xuinya, 3, 7, -13);

    while (1)
        if (CDOTAInventoryMgr->GetSomeSharedShit()->GetSomeUnnamedShit()->GetMoreUnnamedShit()->GetCDOTAGameAccountPlus()->MakeItWork())
            break;
#else
    CDOTAInventoryMgr = (CDOTAInventoryManager*)GetAddressFromInstruction((__int64)xuinya, 3, 7, -13);
#endif

    //auto test = PatternFinder::PatternScan((char*)"client.dll", "13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 13 37 ");//crash test
    
    long long SomeKindOfPool_offset = (long long)PatternFinder::PatternScan("client.dll", "48 8B ? ? ? ? ? 8B 0D ? ? ? ? BE");
    auto SomeKindOfPool_ptr = GetAddressFromInstruction(SomeKindOfPool_offset, 3, 7);
    
    while (!SomeKindOfPool_ptr)
        Sleep(100);

    GameEntitySystem = *(CGameEntitySystem**)SomeKindOfPool_ptr;

    auto C_DOTAPlayer_HeroIndexOffset_sign = PatternFinder::PatternScan("client.dll", "8B 93 ?? ?? ?? ?? 41 BE");

    
    
   /* while (!GameEntitySystem)
        Sleep(100);
    
    auto q = GameEntitySystem->GetPlayerPool();

    auto w = q->GetPlayerHighestIndex();*/


    /*
    
    World is size 1000x1000. 
    Minimap is size 2x2. 
    Ignore z-axis. 
    Player is in position 230x740. 
    Calculate x-position on minimap as 230*2/1000, y-position as 740*2/1000.
    
    */

    auto WTGSelectableUnitCollidedWithCursor = (t8)PatternFinder::PatternScan("client.dll", "4C 8B ?? 55 57 41 ?? 48 81 EC ?? ?? ?? ?? 48 8B");
    auto InBattleCameraFunc         = PatternFinder::PatternScan("client.dll",  "48 8B 01 48 8B 51 ?? 48 FF");
    auto WTGViewMatrix              = PatternFinder::PatternScan("engine2.dll", "48 89 ?? ?? ?? ?? ?? 49 03 ?? 48 8B");
    auto OnAddEntityFunc            = PatternFinder::PatternScan("client.dll",  "48 89 ?? ?? ?? 56 48 83 EC ?? 48 8B ?? 41 8B ?? B9 ?? ?? ?? ?? 48 8B");
    auto OnRemoveEntityFunc         = PatternFinder::PatternScan("client.dll",  "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 41 8B ?? 25");
    auto WTGCParticleSystemMgr      = PatternFinder::PatternScan("client.dll",  "41 0F ?? ?? 48 8B ?? 4C 8B ?? 41 B1");
    IsVisibleByTeam             = (t4)PatternFinder::PatternScan("client.dll",  "48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? ?? ?? ?? ?? 8B FA 48 8B ? 48 85");
    auto WTGCvarProcessor_main      = PatternFinder::PatternScan("engine2.dll", "48 89 ?? ?? ?? 48 89 ?? ?? ?? 44 89 ?? ?? ?? 57 41");
    ConColorMsg                = (t10)PatternFinder::PatternScan("tier0.dll",   "4C 8B ?? 49 89 ?? ?? 4D 89 ?? ?? 4D 89 ?? ?? 53 55");

    auto WTGEnterScope = (t11)PatternFinder::PatternScan("tier0.dll", "48 89 ?? ?? ?? 48 89 ?? ?? ?? 56 41 ?? 41 ?? 48 83 EC ?? 4D 8B");

    CalculateCastRange              =   (t3)PatternFinder::PatternScan("client.dll",     "48 89 ?? ?? ?? 48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 49 8B ?? 48 8B ?? FF 90");
    DrawParticleOnEntity            =   (t5)PatternFinder::PatternScan("client.dll",     "48 89 ?? ?? ?? 48 89 ?? ?? ?? 48 89 ?? ?? ?? 55 41 ?? 41 ?? 48 8D ?? ?? ?? 48 81 EC ?? ?? ?? ?? 4C 8B ?? 45 8B");
    FindOrCreateParticleOrSomething =   (t6)PatternFinder::PatternScan("particles.dll",  "48 8B ? 57 48 81 EC ? ? ? ? 48 8B");
    PingCoordinateWriter        = (__int64*)PatternFinder::PatternScan("client.dll",     "F3 41 ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 41 ?? ?? ?? ?? ?? ?? ?? FF 90");
    auto ParticleNameCutter         =       PatternFinder::PatternScan("particles.dll",  "0F B6 ?? 4C 8B ?? 44 8B");
    stricmp_valve                   = (t7)  PatternFinder::PatternScan("tier0.dll",      "4C 8B ?? 48 3B ?? 74 ?? 48 85");
    auto WTGCvarProcessor_particle  =       PatternFinder::PatternScan("particles.dll",  "4C 8B ?? 53 57 48 81 EC");
    auto WTGCvarProcessor_client    =       PatternFinder::PatternScan("client.dll",     "4C 8B ?? 53 57 48 81 EC ?? ?? ?? ?? 0F 29");
    auto LocalPLayerOrSomething     =       PatternFinder::PatternScan("client.dll",     "44 0F ?? ?? ?? ?? ?? ?? 41 BC ?? ?? ?? ?? 66 45 ?? ?? 0F 84 ?? ?? ?? ?? 4C 89");
    auto MinimapBounds_class        =       PatternFinder::PatternScan("client.dll",     "4C 8B ?? 55 56 57 48 83 EC");
    MinimapBounds_offset            =(char*)PatternFinder::PatternScan("client.dll",     "F3 0F ?? ?? ?? 45 33 ?? 41 BA");//"8B 41 ?? 89 02 8B 41 ?? 89 42 ?? 8B 41 ?? 89 42 ?? 8B 41 ?? 41 89 ?? 8B 81");
    //printf("\nCDOTAInventoryManager: %llx\nxuinya: %llx", CDOTAInventoryMgr,xuinya);
        //00007FFCBFF4DE55 + 01C1D644 + 7 = 7FFCC1B6B4A0

    
    //auto CInputService_StaticAddress PatternFinder::PatternScan((char*)"client.dll", "48 8B ?? ?? ?? ?? ?? 4C 8D ?? ?? 48 8D ?? ?? 48 8B ?? FF 90 ?? ?? ?? ?? 4C 8B");
    //auto attach_hitloc = PatternFinder::PatternScan((char*)"client.dll", "F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? ?? F2 0F ?? ?? ?? 4C 8D");
    /*
    new mask for WTGSelectableUnitCollidedWithCursor
    Address of signature = client.dll + 0x00A3DB30
"\x4C\x8B\x00\x55\x57\x41\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B", "xx?xxx?xxx????xx"
"4C 8B ? 55 57 41 ? 48 81 EC ? ? ? ? 48 8B"
    
    
    
    .................
    CInputService
Address of signature = client.dll + 0x00CBFE44
"\x48\x8B\x00\x00\x00\x00\x00\x4C\x8D\x00\x00\x48\x8D\x00\x00\x48\x8B\x00\xFF\x90\x00\x00\x00\x00\x4C\x8B", "xx?????xx??xx??xx?xx????xx"
"48 8B ? ? ? ? ? 4C 8D ? ? 48 8D ? ? 48 8B ? FF 90 ? ? ? ? 4C 8B"


7FFBF2ADFE44 mov reg,[7FFBF2ADFE44 + 0000029FF785+7]
7FFBF54DF5D0 CInputService

0000029FF785+7

.................
attach_hitloc

Address of signature = client.dll + 0x00CBFE89
"\x48\x8D\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\x49\x8B", "xx?????x????xxx?xx"
"48 8D ? ? ? ? ? E8 ? ? ? ? 84 C0 75 ? 49 8B"

zapaska 
Address of signature = client.dll + 0x00CBFEAC
"\xF3\x0F\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF2\x0F\x00\x00\x00\x4C\x8D", "xx??xx???xx???xx???xx???xx???xx???xx"
"F3 0F ? ? F3 0F ? ? ? F3 0F ? ? ? F3 0F ? ? ? F3 0F ? ? ? F3 0F ? ? ? F2 0F ? ? ? 4C 8D"

-----------------

   Address of signature = client.dll + 0x00931950
"\x40\x00\x56\x57\x48\x83\xEC\x00\x48\x8B\x00\x00\x00\x00\x00\x49\x8B\x00\x48\x8B\x00\x48\x8B\x00\xFF\x50\x00\x48\x8B\x00\x48\x8B\x00\x00\x00\x00\x00\x48\x85\x00\x0F\x84\x00\x00\x00\x00\x48\x83\x38\x00\x0F\x84\x00\x00\x00\x00\x48\x89\x00\x00\x00\x33\xDB\x48\x85\x00\x74\x00\x38\x1F", "x?xxxxx?xx?????xx?xx?xx?xx?xx?xx?????xx?xx????xxx?xx????xx???xxxx?x?xx"
"40 ? 56 57 48 83 EC ? 48 8B ? ? ? ? ? 49 8B ? 48 8B ? 48 8B ? FF 50 ? 48 8B ? 48 8B ? ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 48 83 38 ? 0F 84 ? ? ? ? 48 89 ? ? ? 33 DB 48 85 ? 74 ? 38 1F"

    Address of signature = engine2.dll + 0x000C7F30
"\x48\x89\x00\x00\x00\x44\x89\x00\x00\x00\x48\x89\x00\x00\x00\x55\x56\x57\x41\x00\x41\x00\x41\x00\x41\x00\x48\x81\xEC", "xx???xx???xx???xxxx?x?x?x?xxx"
"48 89 ? ? ? 44 89 ? ? ? 48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 81 EC"

__int64 __fastcall sub_7FFA7FC67F30(__int64 *CNetworkStringTable, __int64 CServerSideClient, __int64 a3, int a4)


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
    bool ok = 1;
#pragma region check

	if (InBattleCameraFunc == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: InBattleCameraFunc sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nInBattleCameraFunc: \t%llx", InBattleCameraFunc);
#endif
	if (WTGViewMatrix == nullptr)/////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: WTGViewMatrix sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nWTGViewMatrix: \t%llx", WTGViewMatrix);
#endif
	if (OnAddEntityFunc == nullptr)//////////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: OnAddEntityFunc sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nOnAddEntity: \t%llx", OnAddEntityFunc);
#endif
	if (OnRemoveEntityFunc == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: OnRemoveEntity sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nOnRemoveEntity: \t%llx", OnRemoveEntityFunc);
#endif
	if (CalculateCastRange == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: GetCastRange sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nGetCastRange: \t%llx", CalculateCastRange);
#endif
	if (IsVisibleByTeam == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: IsVisibleByTeam sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nIsVisibleByTeam: \t%llx", IsVisibleByTeam);
#endif
	if (DrawParticleOnEntity == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: DrawParticleOnEntity sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nDrawParticleOnEntity: \t%llx", DrawParticleOnEntity);
#endif
	if (WTGCParticleSystemMgr == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: WTGCParticleSystemMgr sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nWTGCParticleSystemMgr: \t%llx", WTGCParticleSystemMgr);
#endif
	if (FindOrCreateParticleOrSomething == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: FindOrCreateParticleOrSomething sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nFindOrCreateParticleOrSomething: \t%llx", FindOrCreateParticleOrSomething);
#endif
	if (PingCoordinateWriter == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: PingCoordinateWriter sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nPingCoordinateWriter: \t%llx", PingCoordinateWriter);
#endif
	if (ParticleNameCutter == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: ParticleNameCutter sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nParticleNameCutter: \t%llx", ParticleNameCutter);
#endif
	if (stricmp_valve == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: stricmp_valve sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nstricmp_valve: \t%llx", stricmp_valve);
#endif
	if (WTGSelectableUnitCollidedWithCursor == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: WTGSelectableUnitCollidedWithCursor sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\n WTGSelectableUnitCollidedWithCursor: \t%llx", WTGSelectableUnitCollidedWithCursor);
#endif
	if (WTGCvarProcessor_particle == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: WTGCvarProcessor_particle sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nWTGCvarProcessor_particle: \t%llx", WTGCvarProcessor_particle);
#endif
	if (WTGCvarProcessor_client == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: WTGCvarProcessor_client sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nWTGCvarProcessor_client: \t%llx", WTGCvarProcessor_client);
#endif
	if (WTGCvarProcessor_main == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: WTGCvarProcessor_main sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nWTGCvarProcessor_main: \t%llx", WTGCvarProcessor_main);
#endif
	if (LocalPLayerOrSomething == nullptr)///////////////////////
	{
#ifdef _DEBUG 
		printf("\nERROR: LocalPLayerOrSomething sig not found");
#endif
		ok = 0;
	}
#ifdef _DEBUG
	else 
		printf("\nLocalPLayerOrSomething: \t%llx", LocalPLayerOrSomething);
#endif
    if (C_DOTAPlayer_HeroIndexOffset_sign == nullptr)///////////////////////
    {
#ifdef _DEBUG 
        printf("\nERROR: C_DOTAPlayer_HeroIndexOffset_sign sig not found");
#endif
        ok = 0;
    }
#ifdef _DEBUG
    else
        printf("\nC_DOTAPlayer_HeroIndexOffset_sign: \t%llx", C_DOTAPlayer_HeroIndexOffset_sign);
#endif
    if (MinimapBounds_class == nullptr)///////////////////////
    {
#ifdef _DEBUG 
        printf("\nERROR: MinimapBounds_class sig not found");
#endif
        ok = 0;
    }
#ifdef _DEBUG
    else
        printf("\n MinimapBounds_class: \t%llx", MinimapBounds_class);
#endif


#pragma endregion

//    if (test == nullptr)///////////////////////
//    {
//#ifdef _DEBUG 
//        printf("\nERROR: test sig not found");
//#endif
//        ok = 0;
//    }
//#ifdef _DEBUG
//	else 
//        printf("\test: \t%llx", test);
//#endif

	if (!ok)
	{
#ifdef _DEBUG 
		printf("Something is wrong. Stuff need some update");
#else
        FreezeGame = 1;
#endif
		Sleep(-1);
	}

    hk.set_reg_stealer((char*)MinimapBounds_class, 16, (char*)&MinimapBoundsManipulator, r::rcx);

    

    C_DOTAPlayer_HeroIndexOffset = *(short*)((char*)C_DOTAPlayer_HeroIndexOffset_sign +2);

    hk.set_hook((char*)OnAddEntityFunc, 16, (char*)OnAddEntity, (char**)&OnAddEntityRet);
    hk.set_hook((char*)OnRemoveEntityFunc, 16, (char*)OnRemoveEntity, (char**)&OnRemoveEntityRet);
    //hk.set_reg_stealer((char*)InBattleCameraFunc, 11, (char*)&StolenVar, r::rcx);
    hk.set_reg_stealer_reverse((char*)WTGViewMatrix, 16, (char*)&fuckingMatrix, r::rax);
    //hk.set_reg_stealer((char*)WTGCParticleSystemMgr, 17, (char*)&CParticleSystemMgrPtr, r::rcx);
    TrampoToBreak = hk.set_hook((char*)d3d9Device[42], 15, (char*)hkEndScene, (char**)&oEndScene);
    hk.set_hook((char*)WTGSelectableUnitCollidedWithCursor, 14, (char*)&SelectableUnitCollidedWithCursor, (char**)&SelectableUnitCollidedWithCursor_Original);
    //(05.09.2021) 15 -> 14 (27.09.2021)
    hk.set_hook((char*)WTGCvarProcessor_main, 15, (char*)ConVarMainProcessor, (char**)&ConVarMainProcessor_orig);
#ifdef _DEBUG
    hk.set_hook((char*)WTGEnterScope, 15, (char*)&EnterScope, (char**)&EnterScope_orig);
#endif
    ProcessOldWndProc = hk.set_WndProc_hook(ProcessWindowHandle, (__int64)WndProc);

    
    ConColorMsg(color, "\n\n\n\n*****************we are in*****************\n\n\n\n");
}