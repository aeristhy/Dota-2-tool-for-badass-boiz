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
	if (!SomethingICantExplain)
	{
		auto ConVar_string = (SomethingICantExplain + 8);
		if (ConVar_string)
			printf("\n%s", ConVar_string);
		else
			printf("\n!SomethingICantExplain && no string (wtf?)");

		return ConVarMainProcessor_orig(CInputService, a2, a3, a4, a5, ConVar, (__int32*)SomethingICantExplain);
	}
	auto ConVar_elements = *(__int32*)SomethingICantExplain; //dota_use_particle_fow 1337 ura govno == 4 elements
	if (!ConVar_elements)
		printf("\n[zero elements]");
	else
		printf("\n");
	auto ConVar_strlen = *(__int32*)(SomethingICantExplain + 4);//strlen("dota_use_particle_fow")


	auto ConVar_string = (SomethingICantExplain + 8); //"dota_use_particle_fow 1337 ura govno"
	auto ConVar_command = ConVar_string;//"dota_use_particle_fow"
	auto tempchar = *ConVar_string;
	if (!ConVar_strlen && ((tempchar >= 65 && tempchar <= 90) || (tempchar >= 97 && tempchar <= 122)))
		ConVar_strlen = strlen(ConVar_string);

	auto ConVar_args = (char*)SomethingICantExplain + ConVar_strlen + 8;//" 1337 ura govno"

	char temp[200];
	memset(temp, 0, 200);
	memcpy(temp, ConVar_string, ConVar_strlen);

	if (ConVar_elements == 1)
	{
		printf("%s get read or executed", temp);
		if (!*ConVar_args)
			printf("\t(no args)");
		else
			printf("--> %s", ConVar_args);
	}
	else
		//if (!ConVar_elements || !ConVar)
		//	return ConVarMainProcessor_orig(CInputService,a2,a3,a4,a5,ConVar, (__int32*)SomethingICantExplain);
		//else
		//	printf("\n%s\t%s\t--->\t%s", ConVar->GetConVarName(), ConVar->GetCurrentValue_string(), ConVar_args);


		//if (WhatIsThisBullshit(first_argument) == dec)
		if (ConVar)
			printf("%s %s ---> %s", temp, ConVar->GetCurrentValue_string(), ConVar_args);
		else
			printf("%s %s",temp, ConVar_args);


	return ConVarMainProcessor_orig(CInputService, a2, a3, a4, a5, ConVar, (__int32*)SomethingICantExplain);
}
#endif