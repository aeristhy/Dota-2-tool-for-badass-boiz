#pragma once
#include "GlobalVars.h"
#include "ESP.h"
#include "../imgui/backends/imgui_impl_dx11.h"
#include "../imgui/backends/imgui_impl_win32.h"
#include "PatternFinder.h"
connector hk2;

#ifdef _DEBUG
void cutout()
{
	Sleep(1000);
	hk2.~connector();
	FreeLibraryAndExitThread(HackModuleHandle, 0);
}
#endif



void menu()
{
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

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (fuckingMatrix && fuckingMatrixValid)
		esp(pContext, renderer);

#ifdef _DEBUG
	if (NoPings)
	{
		if (!IsHookSet)
		{
			VirtualProtect(PingCoordinateWriter, 32, PAGE_EXECUTE_READWRITE, &trashbox);
			memcpy(PingCoordinatorOriginalCode, PingCoordinateWriter, 32);
			memset(PingCoordinateWriter, 0x90, 9);
			auto temp = (char*)PingCoordinateWriter + 14;
			memset(temp, 0x90, 18);
			hk2.set_reg_stealer((char*)PingCoordinateWriter, 14, (char*)&HudPtrStolen, r2::r13);
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
#endif

	if (DrawModifiersPanel)
	{
		ImGui::Begin("Modifiers panel");
		OutputModifiers();
		ImGui::End();
	}

	auto q = GameEntitySystem->GetPlayerPool();

	if (q->GetPlayerByPlayerIndex(-1))
		ImGui::Begin("(tv or local lobby)");
	else
		ImGui::Begin("Lick the dick :p");
	{
#ifdef _DEBUG
		ImGui::Text("off_x:");
		ImGui::SameLine();
		ImGui::InputInt("##x_minimap_offset", &x_minimap_offset);
		ImGui::Text("off_y:");
		ImGui::SameLine();
		ImGui::InputInt("##y_minimap_offset", &y_minimap_offset);

		ImGui::Text("size_x:");
		ImGui::SameLine();
		ImGui::InputInt("##x_minimap_size", &x_minimap_size);
		ImGui::Text("size_y:");
		ImGui::SameLine();
		ImGui::InputInt("##y_minimap_size", &y_minimap_size);
#endif

		char label[100];
		memset(label, 0, 100);

		if (!LocalPlayer)
		{
			ImGui::Text("Select your hero pls");
			auto roar = q->GetPlayerHighestIndex();
			for (int i = 0; i < roar; i++)
			{
				CBaseEntity* ent = 0;
				auto entIndex = q->GetPlayerByPlayerIndex(i)->GetHeroEntityIndex();
				int w = 0;
				for (int i = 0; i < heroes_slots; i++)
					if (heroes_index[i] == entIndex)
					{
						ent = heroes[i];
						w = i;
						break;
					}
				if (ent)
				{
					auto MyTeam = ent->GetTeam();
					if (MyTeam == DOTATeam_t::DOTA_TEAM_DIRE || MyTeam == DOTATeam_t::DOTA_TEAM_RADIANT)
					{
						sprintf(label, "[%s]%s", TeamName(MyTeam), ent->Schema_DynamicBinding()->binaryName + 17);
						if (ImGui::Button(label))
						{
							MinimapBounds_x_min = *(MinimapBounds_offset + 4);
							MinimapBounds_y_min = MinimapBounds_x_min + 4;
							MinimapBounds_x_max = MinimapBounds_x_min + 12;
							MinimapBounds_y_max = MinimapBounds_x_min + 16;
							LocalPlayer = ent;
							LocalPlayerID = w;
							localPlayerTeam = MyTeam;
							EnemyTeam = (ent->GetTeam() == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT);
							break;
						}
					}
				}

			}
		}
		else
			if (ImGui::Button("Reset selected hero"))
				LocalPlayer = 0;
#ifdef _DEBUG
		if (ImGui::Button("unload"))
		{
			FreeConsole();
			hk2.BreakTrampoline(TrampoToBreak);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&cutout, 0, 0, 0);
		}
#endif
		debugWindow();
	}
#ifdef _DEBUG
	ImGui::InputInt("##123123", &qwqe);
#endif
	ImGui::Text("HP Panel");
	ImGui::SameLine();
	ImGui::Checkbox("##Heath_Panel_checkbox", &DrawHealthPanel);
	if (SelfVisibeByEnemy)
	{
		ImGui::Separator();
		ImGui::Text("SelfVisibleByEnemy");
		ImGui::SameLine();
		ImGui::Checkbox("##SelfVisibleByEnemy", &SelfVisibeByEnemy);
		ImGui::Text("\\-->MinimapIndicator");
		ImGui::SameLine();
		ImGui::Checkbox("##MinimapIndicatorSelfVBE",&SelfVisibleByEnemyOnMinimap);
		ImGui::Separator();
	}
	else
	{
		ImGui::Text("SelfVisibleByEnemy");
		ImGui::SameLine();
		ImGui::Checkbox("##SelfVisibleByEnemy", &SelfVisibeByEnemy);
	}
	if (TrueSightIndicator)
	{
		ImGui::Separator();
		ImGui::Text("Truesight Indicator");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueSightIndicator", &TrueSightIndicator);

		ImGui::Text("\\-->3D Truesight indicator");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueSightIndicator3DWorld", &TrueSightIndicator3DWorld);

		ImGui::Text("\\-->MinimapIndicator");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueSightIndicatorMinimap", &TrueSightIndicatorMinimap);
		ImGui::Separator();
	}
	else
	{
		ImGui::Text("Truesight Indicator");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueSightIndicator", &TrueSightIndicator);
	}
	if (TrueHero)
	{
		ImGui::Separator();
		ImGui::Text("TrueHero");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueHero", &TrueHero);

		ImGui::Text("\\-->3D World indicator");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueHeroIndicator3DWorld", &TrueHeroIndicator3DWorld);

		ImGui::Text("\\-->MinimapIndicator");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueHeroMinimapIndicator", &TrueHeroIndicatorMinimap);
		ImGui::Separator();
	}
	else
	{
		ImGui::Text("TrueHero");
		ImGui::SameLine();
		ImGui::Checkbox("##TrueHero", &TrueHero);
	}
	ImGui::Text("Modifiers panel");
	ImGui::SameLine();
	ImGui::Checkbox("##xui", &DrawModifiersPanel);
	ImGui::Text("UnderCursor");
	ImGui::SameLine();
	ImGui::Checkbox("##undercursor", &UnderCursor);
#ifdef _DEBUG
	if (ImGui::Button("Force enable dota+"))
		CDOTAInventoryMgr->GetSomeSharedShit()->GetSomeUnnamedShit()->GetMoreUnnamedShit()->GetCDOTAGameAccountPlus()->MakeItWork();
	if (ImGui::Button("Force disable dota+"))
		CDOTAInventoryMgr->GetSomeSharedShit()->GetSomeUnnamedShit()->GetMoreUnnamedShit()->GetCDOTAGameAccountPlus()->Disable();
	ImGui::Text("Disable input");
	ImGui::SameLine();
	ImGui::Checkbox("if you need to input coords", &disableinput);

	/*ImGui::Text("Draw 3D box");
	ImGui::SameLine();
	ImGui::Checkbox("##d3dbox", &draw3dbox);*/


	//pofiksi blin
		//if (draw3dbox)
		//{


		//	CUSTOMVERTEX g_Vertices[] =
		//	{
		//		{ -10.0f,-10.0f, 200.0f,      0x00ff0000, },
		//		{ -10.0f,-10.0f, 510.0f,      0x0000ff00, },
		//		{  10.0f,-10.0f, 510.0f,      0x000000ff, },
		//		{  10.0f,-10.0f, 200.0f,      0x00ff0000, },
		//		{ -10.0f,-10.0f, 200.0f,      0x0000ff00, },
		//		{ -10.0f, 10.0f, 200.0f,      0x000000ff, },
		//		{ -10.0f, 10.0f, 510.0f,      0x00ff0000, },
		//		{ -10.0f, -10.0f, 510.0f,     0x00ff0000, },
		//		{ -10.0f, 10.0f, 510.0f,      0x00ff0000, },
		//		{ 10.0f, 10.0f, 510.0f,       0x00ff0000, },
		//		{ 10.0f, 10.0f, 200.0f,       0x000000ff, },
		//		{ 10.0f, -10.0f, 200.0f,      0x00ff0000, },
		//		{ 10.0f, 10.0f, 200.0f,       0x000000ff, },
		//		{ -10.0f, 10.0f, 200.0f,      0x000000ff, },
		//		{ -10.0f, 10.0f, 510.0f,      0x00ff0000, },
		//		{ 10.0f, 10.0f, 510.0f,       0x00ff0000, },
		//		{ 10.0f, -10.0f, 510.0f,      0x000000ff, },


		//	};

		//	// Create the vertex buffer.
		//	if (FAILED(pDevice->CreateVertexBuffer(16 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		//		ImGui::Text("failCreateVertex");

		//	// Fill the vertex buffer.
		//	VOID* pVertices;
		//	if (FAILED(g_pVB->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0)))
		//		ImGui::Text("faillockVertex");
		//	memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
		//	g_pVB->Unlock();

		//	pDevice->SetStreamSource(1337228, g_pVB, 0, sizeof(CUSTOMVERTEX));
		//	pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		//	pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 16);

		//}
		/*

		тут фигурируют координаты на миникарте в регистрах r8 и r9
		ј ещЄ что-то внутри говорит, что бывает и в r10. Ќе знаю, € уже потрачен, не могу подтвердить
		Address of signature = server.dll + 0x00B2B0F9
	"\x8B\x5C\x00\x00\x48\x8D\x00\x00\x00\x00\x00\x8B\x7C", "xx??xx?????xx"
	"8B 5C ? ? 48 8D ? ? ? ? ? 8B 7C"

		*/


		//ImGui::Text("Draw squares");
		//ImGui::SameLine();
		//ImGui::Checkbox("##draw_square_at_3d_coords", &draw_square_at_3d_coords);
		//if (draw_square_at_3d_coords)
		//{
		//	memset(PointsToDraw_Names, 0, blyaaaa);
		//	for (int i = blyaaaa; i >= 0; i -= 4)
		//	{
		//		memset(PointsToDraw_Names + i, (short)'##', 2);
		//	}
		//	ImGui::Text("How much:");
		//	ImGui::SameLine();
		//	ImGui::InputInt("##how_much_squares", &how_much_squares, 1, 0);
		//	if (how_much_squares > 20)
		//		how_much_squares = 20;
		//	for (int i = 1; i <= how_much_squares; i++)
		//	{
		//		auto pizdec = i * 4 - 2;
		//		PointsToDraw_Names[pizdec] = (char)i + 0x19;
		//		ImGui::InputFloat(PointsToDraw_Names + pizdec - 2, &pointstodraw[how_much_squares][0]);
		//		pizdec = i * 4 - 2 + 20;
		//		PointsToDraw_Names[pizdec] = (char)i + 0x39;
		//		ImGui::InputFloat(PointsToDraw_Names + pizdec - 2, &pointstodraw[how_much_squares][1]);
		//		pizdec = i * 4 - 2 + 40;
		//		PointsToDraw_Names[pizdec] = (char)i + 0x59;
		//		ImGui::InputFloat(PointsToDraw_Names + pizdec - 2, &pointstodraw[how_much_squares][2]);
		//	}
		//	if (fuckingMatrix && fuckingMatrixValid)
		//		for (int i = 1; i < how_much_squares + 1; i++)
		//			DrawQuad3D(pointstodraw[i][0], pointstodraw[i][1], pointstodraw[i][2], pDevice);
		//}
#endif
	if (fuckingMatrix && fuckingMatrixValid && UnderCursor && (GetTickCount() - undercursor_time) < 2000)
	{
		RECT r;
		r.left = undercursor_screen.x;
		r.right = r.left + 500;
		r.top = undercursor_screen.y;
		r.bottom = undercursor_screen.y + 100;

		//font->DrawTextA(0, undercursor_buff, (INT)strlen(undercursor_buff), &r, 0, D3DCOLOR_ARGB(255, 0xAA, 0xEE, 0xEE));
		DrawFilledRect11(undercursor_screen_point.x, undercursor_screen_point.y, 10, 10, Color_class(255, 0xAA, 0xEE, 0xEE), renderer);
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}