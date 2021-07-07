#pragma once
//#include "Include/d3d9.h"
#include "GlobalVars.h"
#include "sig.h"
//#include "Eject.h"
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_dx9.h"
#include "../imgui/backends/imgui_impl_win32.h"
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

	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Lick the dick :p");                      
	{
#ifdef _DEBUG
		/*ImGui::InputInt("#quadcolor", (int*)&quad_color, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
		ImGui::InputInt("#quad_x", &quad_x, 1, 0);
		ImGui::InputInt("#quad_y", &quad_y, 1, 0);
		ImGui::Text("%llx", StolenVar);*/
		if (ImGui::Button("unload"))
		{
			FreeConsole();
			hk.BreakTrampoline(TrampoToBreak);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&cutout, 0, 0, 0);
		}
#endif
		//ImGui::InputDouble("", (double*)&fuckingMatrix, 0, 0,"%.16f", ImGuiInputTextFlags_CharsHexadecimal);
		//ImGui::InputText("...", (char*)xui.c_str(), 256, ImGuiInputTextFlags_CharsHexadecimal);
		//fuckingMatrix = std::strtoull(xui.c_str(), nullptr, 16);
		
		//ImGui::SameLine();
		//ImGui::Checkbox("#IsFuckingMatrixValid", &fuckingMatrixValid);
		
		esp(pDevice);
		
		

	}
	ImGui::End(); //main window end

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	
	return oEndScene(pDevice);
}