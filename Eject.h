#pragma once
#include "GlobalVars.h"

#ifdef _DEBUG
#endif

//void unload_basic(connector hk)
//{
//
//	Sleep(1000);
//    hk.~connector();
//    FreeLibraryAndExitThread(HackModuleHandle, 0);
//
//}
//
//__inline void eject(connector hk)
//{
//    hk.BreakTrampoline(TrampoToBreak);
//    SetThreadPriority(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)unload_basic, 0, 0, 0), -1);
//}