#pragma once
#include "GlobalVars.h"
#include "../imgui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//ProcessWindowHandle = hWnd;
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam); 

	if (!disableinput)
		return CallWindowProc((WNDPROC)ProcessOldWndProc, hWnd, msg, wParam, lParam);
}