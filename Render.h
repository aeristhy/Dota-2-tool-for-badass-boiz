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
	ImGui::Text("Health panel");
	ImGui::SameLine();
	ImGui::Checkbox("##Heath_Panel_checkbox", &DrawHealthPanel);
	ImGui::Text("True hero");
	ImGui::SameLine();
	ImGui::Checkbox("##True_hero_checkbox", &TrueHero);
	ImGui::End();


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	
	return oEndScene(pDevice);
}