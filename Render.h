#pragma once
#include <d3d9helper.h>
//#include "Include/d3d9.h"
#include "GlobalVars.h"
//#include "Eject.h"

#include "ESP.h"
#include <bitset>


connector hk;
ImVec4 clear_color;

struct CUSTOMVERTEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	DWORD color;        // The vertex color
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices

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
	pDevice_ = pDevice;
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
В плавающих сохраняются координаты партикла-пинга
В r13 хранится указатель на худ (?), но это двухбайтоывй регистр и имеющийся стилер владеет лишь однобайтовыми. 
TODO:
1) Разделить тип стила на один и два байта, т.к. разный размер и все дела
2) Апгрейднуть стилер до возможности красть x64
3) Добавить комментарии с описанием, каков миниимальный объём памяти для стилера в байтах
4) Добавить возможность красть содержимое нескольких регистров за одну инъекцию,чтобы не плодить лишние джампы

Внизу код, который заменит инструкции нопами. Перед этим надо скопировать значение r13, дабы записать координаты партикла
Иначе, после ноппинга, стилер перекинет нопы на новую страницу, и в этом не будет никакого смысла. Хотя игра и не вылетит. 
Это жест оптимизации, Снеж, тут не нужно искать ошибки. Всё ок. 
Крадём r13, переключаем флаг, ноппим, по повторному переключению удаляем нопы и нулим то, где хранится украденное значение r13 
	*/
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
			hk.set_reg_stealer((char*)PingCoordinateWriter, 14, (char*)&HudPtrStolen, r2::r13);
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

//#ifdef _DEBUG

	if (DrawModifiersPanel)
	{
		ImGui::Begin("Modifiers panel");
		OutputModifiers();
		ImGui::End();
	}
//#endif
	auto q = GameEntitySystem->GetPlayerPool();
	if (q->GetPlayerByPlayerIndex(-1))
		ImGui::Begin("(tv or local lobby)");
	else
		ImGui::Begin("Lick the dick :p");                      
	{
		char label[100];
		memset(label, 0, 100);
		
		if (!LocalPlayer)
		{
			ImGui::Text("Select your hero pls");
			for (int i = 0; i < q->GetPlayerHighestIndex(); i++)
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
						
						sprintf(label, "[%s]%s", TeamName(MyTeam), ent->Schema_DynamicBinding()->binaryName+17);
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
			hk.BreakTrampoline(TrampoToBreak);
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
	ImGui::Text("Truehero");	
	ImGui::SameLine();
	ImGui::Checkbox("##True_hero_checkbox", &TrueHero);
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

	ImGui::Text("Draw 3D box");
	ImGui::SameLine();
	ImGui::Checkbox("##d3dbox",&draw3dbox);
	ImGui::Text("Disable input");
	ImGui::SameLine();

	if (draw3dbox)
	{
		

		CUSTOMVERTEX g_Vertices[] =
		{
			{ -10.0f,-10.0f, 200.0f,      0x00ff0000, },
			{ -10.0f,-10.0f, 510.0f,      0x0000ff00, },
			{  10.0f,-10.0f, 510.0f,      0x000000ff, },
			{  10.0f,-10.0f, 200.0f,      0x00ff0000, },
			{ -10.0f,-10.0f, 200.0f,      0x0000ff00, },
			{ -10.0f, 10.0f, 200.0f,      0x000000ff, },
			{ -10.0f, 10.0f, 510.0f,      0x00ff0000, },
			{ -10.0f, -10.0f, 510.0f,     0x00ff0000, },
			{ -10.0f, 10.0f, 510.0f,      0x00ff0000, },
			{ 10.0f, 10.0f, 510.0f,       0x00ff0000, },
			{ 10.0f, 10.0f, 200.0f,       0x000000ff, },
			{ 10.0f, -10.0f, 200.0f,      0x00ff0000, },
			{ 10.0f, 10.0f, 200.0f,       0x000000ff, },
			{ -10.0f, 10.0f, 200.0f,      0x000000ff, },
			{ -10.0f, 10.0f, 510.0f,      0x00ff0000, },
			{ 10.0f, 10.0f, 510.0f,       0x00ff0000, },
			{ 10.0f, -10.0f, 510.0f,      0x000000ff, },


		};

		// Create the vertex buffer.
		if (FAILED(pDevice->CreateVertexBuffer(16 * sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &g_pVB, NULL)))
			ImGui::Text("failCreateVertex");

		// Fill the vertex buffer.
		VOID* pVertices;
		if (FAILED(g_pVB->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0)))
			ImGui::Text("faillockVertex");
		memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
		g_pVB->Unlock();

		pDevice->SetStreamSource(1337228, g_pVB, 0, sizeof(CUSTOMVERTEX));
		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 16);

	}
	/*
	
	тут фигурируют координаты на миникарте в регистрах r8 и r9
	А ещё что-то внутри говорит, что бывает и в r10. Не знаю, я уже потрачен, не могу подтвердить
	Address of signature = server.dll + 0x00B2B0F9
"\x8B\x5C\x00\x00\x48\x8D\x00\x00\x00\x00\x00\x8B\x7C", "xx??xx?????xx"
"8B 5C ? ? 48 8D ? ? ? ? ? 8B 7C"
	
	*/

	ImGui::Checkbox("if you need to input coords", &disableinput);
	ImGui::Text("Draw squares");
	ImGui::SameLine();
	ImGui::Checkbox("##draw_square_at_3d_coords", &draw_square_at_3d_coords);
	if (draw_square_at_3d_coords)
	{
		memset(PointsToDraw_Names, 0, blyaaaa);
		for (int i = blyaaaa; i >= 0; i -= 4)
		{
			memset(PointsToDraw_Names + i, (short)'##', 2);
		}
		ImGui::Text("How much:");
		ImGui::SameLine();
		ImGui::InputInt("##how_much_squares", &how_much_squares, 1, 0);
		if (how_much_squares > 20)
			how_much_squares = 20;
		for (int i = 1; i <= how_much_squares; i++)
		{
			auto pizdec = i * 4 - 2;
			PointsToDraw_Names[pizdec] = (char)i + 0x19;
			ImGui::InputFloat(PointsToDraw_Names + pizdec -2,&pointstodraw[how_much_squares][0]);
			pizdec = i * 4 - 2 + 20;
			PointsToDraw_Names[pizdec] = (char)i + 0x39;
			ImGui::InputFloat(PointsToDraw_Names + pizdec -2, &pointstodraw[how_much_squares][1]);
			pizdec = i * 4 - 2 + 40;
			PointsToDraw_Names[pizdec] = (char)i + 0x59;
			ImGui::InputFloat(PointsToDraw_Names + pizdec -2, &pointstodraw[how_much_squares][2]);
		}
		if (fuckingMatrix && fuckingMatrixValid)
			for (int i = 1; i < how_much_squares + 1; i++)
				DrawQuad3D(pointstodraw[i][0], pointstodraw[i][1], pointstodraw[i][2], pDevice);
	}
#endif
	if (fuckingMatrix && fuckingMatrixValid && UnderCursor && (GetTickCount() - undercursor_time) < 2000 )
	{
		RECT r;
		r.left = undercursor_screen.x;
		r.right = r.left + 500;
		r.top = undercursor_screen.y;
		r.bottom = undercursor_screen.y + 100;

		font->DrawTextA(0, undercursor_buff, (INT)strlen(undercursor_buff), &r, 0, D3DCOLOR_ARGB(255, 0xAA, 0xEE, 0xEE));
		//DrawFilledRect11(undercursor_screen_point.x, undercursor_screen_point.y, 10, 10, D3DCOLOR_ARGB(255, 0xAA, 0xEE, 0xEE), pDevice_);
	}
	/*ImGui::Text("No Pings");
	ImGui::SameLine();
	ImGui::Checkbox("(not full disabled yet)", &NoPings);
	ImGui::Text("Spell CD");
	ImGui::SameLine();
	ImGui::Checkbox("##", &enemyCD);*/

	ImGui::End();


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	
	return oEndScene(pDevice);
}