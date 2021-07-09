#pragma once
#include "GlobalVars.h"
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_dx9.h"
#include "../imgui/backends/imgui_impl_win32.h"


typedef bool(__thiscall* VBE)(unsigned __int64*, DOTATeam_t);

bool VisibleByEnemyOkToUse = false;
bool VisibleByEnemyChecked = false;
int  VisibleByEnemySlot = 0;
bool VisibleByEnemy(VBE** ent, DOTATeam_t t)

{
	if (!VisibleByEnemyChecked)
	{
		if (CheckSigInVtable((unsigned __int64**)ent, 260, d::n::VBE, &VisibleByEnemySlot))
		{
			VisibleByEnemyOkToUse = true;
#ifdef _DEBUG
			printf("\nVBE func id == %d", VisibleByEnemySlot);
#endif
		}
		VisibleByEnemyChecked = true;
	}
	if (!VisibleByEnemyOkToUse)
		return 0;

	return ((*ent)[VisibleByEnemySlot])((unsigned __int64*)ent, t);
}


void DrawFilledRect11(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}
struct vec2 {
	int x;
	int y;
};


bool WorldToScreen(D3DVECTOR pos, vec2* screen, float matrix[16], int windowWidth, int windowHeight)
{
	/*

	Address of signature = engine2.dll + 0x001BAAF2
"\x48\x89\x00\x00\x00\x00\x00\x49\x03\x00\x48\x8B", "xx?????xx?xx"
"48 89 ? ? ? ? ? 49 03 ? 48 8B"

engine2.dll+1BAAEC - 48 05 C0260000        - add rax,000026C0
engine2.dll+1BAAF2 - 48 89 8D E0000000     - mov [rbp+000000E0],rcx
engine2.dll+1BAAF9 - 49 03 C6              - add rax,r14
engine2.dll+1BAAFC - 48 8B D1              - mov rdx,rcx
engine2.dll+1BAAFF - 48 8B C8              - mov rcx,rax
engine2.dll+1BAB02 - 48 89 45 B0           - mov [rbp-50],rax //rax ������ ��������� ���� ������ ������ ��������� rax+0x288==ViewMatrix
engine2.dll+1BAB06 - E8 459FFFFF           - call engine2.dll+1B4A50 //�������� ������� ������� ����---\
engine2.dll+1BAB0B - 48 8B 55 68           - mov rdx,[rbp+68]											|
engine2.dll+1BAB0F - 44 8B CB              - mov r9d,ebx												|
engine2.dll+1BAB12 - 48 8B 9D E0000000     - mov rbx,[rbp+000000E0]										|
engine2.dll+1BAB19 - 48 81 C2 00150000     - add rdx,00001500											|
engine2.dll+1BAB20 - 49 03 D6              - add rdx,r14												|
																										|
																										|
																										|
	Address of signature = engine2.dll + 0x002EE2D6<<<<<<<<<<<<<<<<-----------------------------------./ ��� ���, �����, ��
"\x0F\x11\x00\x0F\x11\x00\x00\x0F\x11\x00\x00\xC3", "xx?xx??xx??x"
"0F 11 ? 0F 11 ? ? 0F 11 ? ? C3"

engine2.dll+2ED670 - 0F10 4A 10            - movups xmm1,[rdx+10]
engine2.dll+2ED674 - 48 8B C1              - mov rax,rcx
engine2.dll+2ED677 - 0F10 52 20            - movups xmm2,[rdx+20]
engine2.dll+2ED67B - 0F10 5A 30            - movups xmm3,[rdx+30]
engine2.dll+2ED67F - 0F10 02               - movups xmm0,[rdx]
engine2.dll+2ED682 - 0F11 49 10            - movups [rcx+10],xmm1
engine2.dll+2ED686 - 0F11 01               - movups [rcx],xmm0 //rcx == ViewMatrix
engine2.dll+2ED689 - 0F11 51 20            - movups [rcx+20],xmm2
engine2.dll+2ED68D - 0F11 59 30            - movups [rcx+30],xmm3
engine2.dll+2ED691 - C3                    - ret


	*/


	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	float clipCoords[4];
	clipCoords[0] = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords[1] = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords[2] = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords[3] = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clipCoords[3] < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	D3DVECTOR NDC;
	NDC.x = clipCoords[0] / clipCoords[3];
	NDC.y = clipCoords[1] / clipCoords[3];
	NDC.z = clipCoords[2] / clipCoords[3];

	screen->x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen->y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

void esp(LPDIRECT3DDEVICE9 pDevice)
{
	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);
	//if (!IsLocalPlayerFound) //only works with real players. Bots and disconnected players counts as local player. (siet)
	//{
	//	bool hasBothTeam = 0;
	//	bool hasDire = 0;
	//	bool hasRadiant = 0;
	//	for (char i = 0; i < heroes_slots; i++)
	//	{
	//		if (!heroes[i])
	//			continue;
	//		auto heroTeam = heroes[i]->GetTeam();
	//		heroes_isSeen[i] = VisibleByEnemy((VBE**)heroes[i], (heroTeam == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT));
	//		if (heroes_isSeen[i])
	//		{
	//			if (!hasDire && heroTeam == DOTATeam_t::DOTA_TEAM_DIRE)
	//				hasDire = 1;
	//			if (!hasRadiant && heroTeam == DOTATeam_t::DOTA_TEAM_RADIANT)
	//				hasRadiant = 1;
	//			if (hasRadiant && hasDire)
	//			{
	//				hasBothTeam = 1;
	//				break;
	//			}
	//		}
	//	}
	//	if (hasBothTeam)
	//	{
	//		
	//		for (char i = 0; i < heroes_slots; i++)
	//		{
	//			if (!heroes[i])
	//				continue;
	//			bool AloneInTeam = 1;
	//			auto heroTeam = heroes[i]->GetTeam();
	//			for (char q = 0; q < heroes_slots; q++)
	//			{
	//				if (!heroes[q])
	//					continue;
	//				if (q != i && heroTeam == heroes[q]->GetTeam())
	//					AloneInTeam = 0;
	//			}
	//			if (AloneInTeam)
	//			{
	//				IsLocalPlayerFound = 1;
	//				LocalPlayer = heroes[i];
	//			}
	//		}
	//		
	//	}
	//}
	//else
	
	//������� ���������, ������� �������, �������� ������� ����� � �����. ���.
	if (!LocalPlayer)
	{
		for (char i = 0; i < 5; i++)
		{
			if (heroes[i])
				if (VisibleByEnemy((VBE**)heroes[i], (heroes[i]->GetTeam() == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT)))
					LocalPlayer = heroes[i];
		}
	}
	else
	{
		if (DrawHealthPanel)
			ImGui::Begin("HealthPanel");
		auto localPlayerTeam = LocalPlayer->GetTeam();
		auto EnemyTeam = localPlayerTeam == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT;
		for (char i = 0; i < heroes_slots; i++)
			if (heroes[i])
			{	
				auto heroName = heroes[i]->Schema_DynamicBinding()->binaryName + 17;
				auto heroTeam = heroes[i]->GetTeam();
				auto heroVec3 = heroes[i]->GetSleleton()->GetPos();

				vec2 screen;
				screen.x = 0;
				screen.y = 0;

				long long temp = fuckingMatrix + 0x288;
				WorldToScreen(*(D3DVECTOR*)heroVec3, &screen, (float*)temp, view.Width, view.Height);


				if (heroTeam != EnemyTeam)
				{
					heroes_isSeen[i] = VisibleByEnemy((VBE**)heroes[i], (heroTeam == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT));
					

					int c = heroes[i]->GetModifiersCount();
					auto ModPool = heroes[i]->GetModifiersPool();
					for (int w = 0; w < c; w++)
					{
						if (!memcmp(ModPool->GetModifier(w)->Name(), "modifier_truesight", 18))
						{
							DrawFilledRect11(screen.x, screen.y, 50, 50, quad_color, pDevice);
						}
					}

					if (heroes_isSeen[i])
						DrawFilledRect11(screen.x, screen.y, 50, 25, quad_color2, pDevice);
				}
				else //if heroTeam==EnemyTeam
				{
					if (DrawHealthPanel)
					{
						char HeroName[7];
						memcpy(HeroName, heroName, 6);
						HeroName[6] = 0;
						auto currentHealth = heroes[i]->GetCurrentHealth();
						ImVec4 color;
						color.x = 1.0;
						color.w = 0.2;
						color.y = 0.2;
						color.z = 0.2;
						if (currentHealth >= 1000)
						{
							color.y = 0.0;
							color.z = 1.0;
							
						}
						if (currentHealth <= 500)
						{
							color.y = 1.0;
							color.z = 0.0;
						}
						ImGui::TextColored(color,"%s\t%d/%d", HeroName, currentHealth, heroes[i]->GetMaxHealth());
					}
					if (TrueHero)
						DrawFilledRect11(screen.x, screen.y-10, 50, 10, 0xFF000000, pDevice);
				}
			}
		if (DrawHealthPanel)
			ImGui::End();
		
	}
}

#ifdef _DEBUG
void debugWindow()
{
	for (char i = 0; i < heroes_slots; i++)
		if (heroes[i])
		{
			auto heroName = heroes[i]->Schema_DynamicBinding()->binaryName + 17;
			auto heroTeam = heroes[i]->GetTeam();
			ImGui::Text("[%d]%s\t%s\t%llx", heroTeam, heroName, heroes_isSeen[i] ? "seen" : "0", (__int64*)heroes[i]);
		}
}
#endif