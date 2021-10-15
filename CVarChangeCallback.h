#pragma once
#include "CustomTypes.h"
#include "CConVar.h"
#include "CConCommandMemberAccessor.h"
#include <iostream>

/*
Address of signature = particles.dll + 0x0026B220
"\x4C\x8B\x00\x53\x57\x48\x81\xEC", "xx?xxxxx"
"4C 8B ? 53 57 48 81 EC"

__int64 __usercall (rdx char* new cvar value, rcx ConVar, xm0 double FigureOutWhat)
*/
#ifdef _DEBUG
void __fastcall ConVarChangeCallback__particledll(char* value, CConVar* ConVar, double FigureOutWhat)
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
	return; //Function is not finished. This is just announce or something like that. Just like "Yo dude look im doing nice?"
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
#endif


void __fastcall ConVarMainProcessor(__int64 CInputService, int a2, int a3, int a4, unsigned int a5, CConVar* ConVar, char* SomethingICantExplain)
{
	if (!SomethingICantExplain)
	{
#ifdef _DEBUG
		auto ConVar_string = (SomethingICantExplain + 8);
		if (ConVar_string)
			printf("\n%s", ConVar_string);
		else
			printf("\n!SomethingICantExplain && no string (wtf?)");
#endif
		return ConVarMainProcessor_orig(CInputService, a2, a3, a4, a5, ConVar, (__int64*)SomethingICantExplain);
	}
	auto ConVar_elements = *(__int32*)SomethingICantExplain; //dota_use_particle_fow 1337 ura govno == 4 elements
#ifdef _DEBUG
	if (!ConVar_elements)
		printf("\n[zero elements]");
	else
		printf("\n");
#endif
	auto ConVar_strlen = *(__int32*)(SomethingICantExplain + 4);//strlen("dota_use_particle_fow")


	auto ConVar_string = (SomethingICantExplain + 8); //"dota_use_particle_fow 1337 ura govno"
	auto ConVar_command = ConVar_string;//"dota_use_particle_fow"
	auto tempchar = *ConVar_string;
	if (!ConVar_strlen && ((tempchar >= 65 && tempchar <= 90) || (tempchar >= 97 && tempchar <= 122)))
		ConVar_strlen = strlen(ConVar_string);
	
	auto ConVar_args = (char*)SomethingICantExplain + ConVar_strlen + 8;//" 1337 ura govno"

	char temp[200];
	memset(temp, 0, 200);
	if (ConVar_strlen)
		memcpy(temp, ConVar_string, ConVar_strlen);
	else
		strcat(temp, ConVar_args);
	if (ConVar_elements == 1)
	{
#ifdef _DEBUG
		if (!ConVar)
			printf("[no  ConVar]");
		else
		{
			auto q = ConVar->GetConVarName();
			if (*q)
				printf("[ _unknown ]");
			else
				printf("[no name]");
		}
		printf("\t%s get read or executed", temp);
#endif
	}
	else
		if (!ConVar_elements || !ConVar)
			return ConVarMainProcessor_orig(CInputService,a2,a3,a4,a5,ConVar, (__int64*)SomethingICantExplain);
#ifdef _DEBUG
		else
		//printf("\n%s\t%s\t--->\t%s", ConVar->GetConVarName(), ConVar->GetCurrentValue_string(), ConVar_args);
#endif
//		//if (WhatIsThisBullshit(first_argument) == dec)
		if (ConVar)
		{
			
			if (ConVar->isCConCommand())
			{	
#ifdef _DEBUG
				auto ConCommand = (CConCommand*)ConVar;
				printf("[ConCommand]\t%s %s", temp, ConVar_args);
#endif
			}
			else
			{
				if (!strstr(temp, "bind") && !strstr(temp, "say") && !strstr(temp, "say_team"))
				{
					auto command = strstr(ConVar_args, "force");		 //allows you to change protected CVars
					if (command && *(char*)(command + 5) == '\0')		//example: dota_use_particle_fow 0 force
					{													//will cause you to see enemy spells and teleports in Fog Of War
						char w[8];										//however, dota engine will still think it is '1' and will show it
						memset(w, 0, 8);								//dont belive it feature is not working. It will set value that checks
						memcpy(w, ConVar_args, command - ConVar_args);	//so either hack working either your game crashed :D
						ConVar->SetValue((int)std::strtol(w, 0, 10));		//for now working only for CVars that check __int32 value and only with decimal arguments. 
						ConColorMsg((__int64*)&color, "\n\nPssst~\nConVar value secretly changed.\nEnjoy~~\n\n");
					}
				}
#ifdef _DEBUG
				auto q = ConVar->GetCurrentValue_string();
				if (!*q)
					printf("[ConVar val]\t%s (no value) ---> %s", temp, ConVar_args);
				else
					printf("[ConVar val]\t%s %s ---> %s", temp,q, ConVar_args);
#endif
			}
		}
#ifdef _DEBUG
		else
			printf("%s %s",temp, ConVar_args);
#endif

	return ConVarMainProcessor_orig(CInputService, a2, a3, a4, a5, ConVar, (__int64*)SomethingICantExplain);
}