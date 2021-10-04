#pragma once
#include "CustomTypes.h"
#include "CConVar.h"
#include <iostream>

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
	}
	return 0; //Function is not finished. This is just announce or something like that. Just like "Yo dude look im doing nice?"
}


enum VarType
{
	wtf = 0,
	dec,	//decimal
	point,	//float
	string	//char
};

VarType WhatIsThisBullshit(char* bullshit)
{
	return wtf;
}
#ifdef _DEBUG
void __fastcall ConVarMainProcessor(__int64 CInputService, int a2, int a3, int a4, unsigned int a5, CConVar* ConVar, char* SomethingICantExplain)
{
	if (!SomethingICantExplain || !(*SomethingICantExplain) || *SomethingICantExplain <= 1 || !ConVar)
		return ConVarMainProcessor_orig(CInputService,a2,a3,a4,a5,ConVar, (__int32*)SomethingICantExplain);

	auto orig = SomethingICantExplain;

	auto ConVar_elements	= *(__int32*)SomethingICantExplain; 
	auto ConVar_strlen		= *(__int32*)(SomethingICantExplain + 4);
	auto ConVar_string		= *(SomethingICantExplain + 8); //"dota_use_particle_fow 1337 ura govno"
	auto ConVar_command		= ConVar_string;//"dota_use_particle_fow"
	auto ConVar_args		= (char*)SomethingICantExplain + ConVar_strlen + 8;//" 1337 ura govno"
	
	//if (WhatIsThisBullshit(first_argument) == dec)
	printf("\n%s %d ---> %s", ConVar->GetConVarName(), ConVar->GetCurrentValue__int32(), ConVar_args);


	return ConVarMainProcessor_orig(CInputService, a2, a3, a4, a5, ConVar, (__int32*)orig);
}
#endif