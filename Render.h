#pragma once
//#include "Include/d3d9.h"
#include "GlobalVars.h"
#include "sig.h"
//#include "Eject.h"

#include "ESP.h"
#include <bitset>

connector hk;
ImVec4 clear_color;
LPDIRECT3DDEVICE9 _pDevice;
#ifdef _DEBUG
void cutout()
{
	Sleep(1000);
	hk.~connector();
	FreeLibraryAndExitThread(HackModuleHandle, 0);
}
#endif


HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	_pDevice = pDevice;
	if (!RenderInitiated)
	{
		if (D3DXCreateFontA(pDevice, 15, 11, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas", &font) == S_OK)
			printf("\nfont created");
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(ProcessWindowHandle);
		ImGui_ImplDX9_Init(pDevice);
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGui::SetNextWindowSize(ImVec2(200.0, 400));
		RenderInitiated = 1;
	}

	/*
	DrawFilledRect11(0, 0, 50, 20, 0xFF000000, pDevice); black
	DrawFilledRect11(0, 50, 50, 20, 0x00FF0000, pDevice); red
	DrawFilledRect11(0, 100, 50, 20, 0x0000FF00, pDevice); green
	DrawFilledRect11(0, 150, 50, 20, 0x000000FF, pDevice); blue
	*/
	//RECT rect;
	//rect.left	= 0;
	//rect.top	= 0;
	//rect.right	= 100;
	//rect.bottom = 100;
	//font->DrawTextA(0, "xuinya", -1, &rect, 0, D3DCOLOR_ARGB(255, 255, 0, 0));//works fine

	if (fuckingMatrix == fuckingMatrixLastValue)
		fuckingMatrixChanged = false;
	else
	{
		fuckingMatrixChanged = true;
		fuckingMatrixLastValue = fuckingMatrix;
	}
	DWORD trashbox;
	if (fuckingMatrixChanged)
	{
		if (VirtualProtect((__int64*)fuckingMatrix, 64, PAGE_EXECUTE_READWRITE, &trashbox) || VirtualProtect((__int64*)fuckingMatrix, 64, PAGE_READWRITE, &trashbox))
			fuckingMatrixValid = true;
		else
			fuckingMatrixValid = false;
		fuckingMatrixChanged = false;
	}
	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (fuckingMatrix && fuckingMatrixValid)
		esp(pDevice);

	/*
	
	Address of signature = client.dll + 0x00D6A133
"\xF3\x41\x00\x00\x00\x00\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x00\x00\x00\xFF\x90", "xx???????xx???xx???????xx???????xx"
"F3 41 ? ? ? ? ? ? ? F3 0F ? ? ? F3 41 ? ? ? ? ? ? ? F3 41 ? ? ? ? ? ? ? FF 90"
� ��������� ����������� ���������� ��������-�����
� r13 �������� ��������� �� ��� (?), �� ��� ������������ ������� � ��������� ������ ������� ���� �������������. 
TODO:
1) ��������� ��� ����� �� ���� � ��� �����, �.�. ������ ������ � ��� ����
2) ����������� ������ �� ����������� ������ x64
3) �������� ����������� � ���������, ����� ������������ ����� ������ ��� ������� � ������
4) �������� ����������� ������ ���������� ���������� ��������� �� ���� ��������,����� �� ������� ������ ������

����� ���, ������� ������� ���������� ������. ����� ���� ���� ����������� �������� r13, ���� �������� ���������� ��������
�����, ����� ��������, ������ ��������� ���� �� ����� ��������, � � ���� �� ����� �������� ������. ���� ���� � �� �������. 
��� ���� �����������, ����, ��� �� ����� ������ ������. �� ��. 
����� r13, ����������� ����, ������, �� ���������� ������������ ������� ���� � ����� ��, ��� �������� ���������� �������� r13 
	*/
	if (NoPings)
	{
		if (!IsHookSet)
		{
			VirtualProtect(PingCoordinateWriter, 32, PAGE_EXECUTE_READWRITE, &trashbox);
			memcpy(PingCoordinatorOriginalCode, PingCoordinateWriter, 32);
			memset(PingCoordinateWriter, 0x90, 9);
			auto temp = (char*)PingCoordinateWriter + 14;
			memset(temp, 0x90, 18);
			HookID = hk.set_reg_stealer((char*)PingCoordinateWriter, 14, (char*)&HudPtrStolen, r2::r13);
			memcpy(PingCoordinatorHooked, PingCoordinateWriter, 32);
			IsHookSet = 1;
		}
		if (HudPtrStolen && !siet)
		{
			memcpy(PingCoordinateWriter, PingCoordinatorOriginalCode, 32);
			memset(PingCoordinateWriter, 0x90, 9);
			auto temp = (char*)PingCoordinateWriter + 14;
			memset(temp, 0x90, 18);
			HudPtrStolen->GetLastPingCoord()[0] = -20000.0; //kick annoying ping's ass out of map
			siet = 1;
		}
		if (!siet)
			memcpy(PingCoordinateWriter, PingCoordinatorHooked, 32);
	}
	
	if (!NoPings && IsHookSet)
	{
		HudPtrStolen = 0;
		siet = 0;
		memcpy(PingCoordinateWriter, PingCoordinatorOriginalCode, 32);
	}

#ifdef _DEBUG
	ImGui::Begin("Lick the dick :p");                      
	{

		if (ImGui::Button("unload"))
		{
			FreeConsole();
			hk.BreakTrampoline(TrampoToBreak);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&cutout, 0, 0, 0);
		}
		debugWindow();
	}
	ImGui::End();
#endif
	ImGui::Begin("##Placeholer");
	ImGui::Text("HP Panel");
	ImGui::SameLine();
	ImGui::Checkbox("##Heath_Panel_checkbox", &DrawHealthPanel);
	ImGui::Text("Truehero");
	ImGui::SameLine();
	ImGui::Checkbox("##True_hero_checkbox", &TrueHero);
	/*ImGui::Text("No Pings");
	ImGui::SameLine();
	ImGui::Checkbox("(not full disabled yet)", &NoPings);*/
	ImGui::Text("Spell CD");
	ImGui::SameLine();
	ImGui::Checkbox("##", &enemyCD);

	ImGui::End();


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	
	return oEndScene(pDevice);
}