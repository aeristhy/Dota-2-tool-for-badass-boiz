#pragma once
#include "GlobalVars.h"
#include "../imgui/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool FreezeGame = 0;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//ProcessWindowHandle = hWnd;

	if (FreezeGame)
	{
		FreezeGame = 0;
		MessageBoxA(0, "One or more signature is invalid. Game can crash anytime.\nPress ok and quit, or use on own risk", "whoops", MB_OK);
	}

	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	if (!disableinput)
		return CallWindowProc((WNDPROC)ProcessOldWndProc, hWnd, msg, wParam, lParam);
}