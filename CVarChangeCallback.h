#pragma once
#include "GlobalVars.h"

#include "CConVar.h"

/*
Address of signature = particles.dll + 0x0026B220
"\x4C\x8B\x00\x53\x57\x48\x81\xEC", "xx?xxxxx"
"4C 8B ? 53 57 48 81 EC"

__int64 __usercall (rdx char* new cvar value, rcx ConVar, xm0 double FigureOutWhat)
*/

__int64 __fastcall ConVarChangeCallback__particledll(char* value, CConVar* ConVar, double FigureOutWhat)
{
	switch (*value)
	{
	case '0':
	{	//on something disabled
		//if (!strcmp(ConVar->GetConVarName(), "cl_particle_log_creates"))
		//	*value = '1'; //<-----------------useless sample hook. Different dlls have own processors for own CVars. 
		break;
	}	
	case '1':
	{	//on something enabled
		break;
	}
	default:
	{	break;
		
	}
	return 0; //Function is not finished. This is just announce or something like that. Just like "Yo dude look im doing nice?"
}
