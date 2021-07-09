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

	if (fuckingMatrix && fuckingMatrixValid)
		esp(pDevice);
	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
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
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	
	return oEndScene(pDevice);
}