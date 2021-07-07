#pragma once
#include "GlobalVars.h"

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
engine2.dll+1BAB02 - 48 89 45 B0           - mov [rbp-50],rax //rax хранит указатель лишь одного класса стабильно rax+0x288==ViewMatrix
engine2.dll+1BAB06 - E8 459FFFFF           - call engine2.dll+1B4A50 //вызывает функцию которая ниже---\
engine2.dll+1BAB0B - 48 8B 55 68           - mov rdx,[rbp+68]											|
engine2.dll+1BAB0F - 44 8B CB              - mov r9d,ebx												|
engine2.dll+1BAB12 - 48 8B 9D E0000000     - mov rbx,[rbp+000000E0]										|
engine2.dll+1BAB19 - 48 81 C2 00150000     - add rdx,00001500											|
engine2.dll+1BAB20 - 49 03 D6              - add rdx,r14												|
																										|
																										|
																										|
	Address of signature = engine2.dll + 0x002EE2D6<<<<<<<<<<<<<<<<-----------------------------------./ вот она, слева, да
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

	for (char i = 0; i < heroes_slots; i++)
		if (heroes[i])
		{
			auto pos = heroes[i]->GetSleleton()->GetPos();
			D3DVECTOR vec3;
			vec3.x = pos[0];
			vec3.y = pos[1];
			vec3.z = pos[2];
			vec2 screen;
			screen.x = 0;
			screen.y = 0;


			heroes_isSeen[i] = VisibleByEnemy((VBE**)heroes[i], (heroes[i]->GetTeam() == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT));

			if (fuckingMatrix && fuckingMatrixValid)
			{
				long long temp = fuckingMatrix + 0x288;
				WorldToScreen(vec3, &screen, (float*)temp, view.Width, view.Height);

				int c = heroes[i]->GetModifiersCount();
				for (int w = 0; w < c; w++)
				{
					if (!memcmp(heroes[i]->GetModifiersPool()->GetModifier(w)->Name(), "modifier_truesight", 18))
					{
						DrawFilledRect11(screen.x, screen.y, 50, 50, quad_color, pDevice);
					}
				}

				if (heroes_isSeen[i])
					DrawFilledRect11(screen.x, screen.y, 50, 25, quad_color2, pDevice);
			}

			if (heroes_isSeen[i])
				Choosen = heroes[i];
			if (heroes[i] != Choosen)
				continue;
			const char* nick = heroes[i]->Schema_DynamicBinding()->binaryName + 17;
			if (heroes_isSeen[i])
			{
				DWORD temp = GetTickCount();
				int seconds = (temp - heroes_times[i]) / 1000;
				if (seconds > 60)
					ImGui::Text("[%d]%s\tunseen %d:%d", heroes[i]->GetTeam(), nick, (((temp - heroes_times[i]) / 1000) / 60), (((temp - heroes_times[i]) / 1000) % 60) - 1 /*(*time_symbol[i] == 'm' ? 60000 :1000)*/, time_symbol[i]);
				else
					ImGui::Text("[%d]%s\tunseen %d", heroes[i]->GetTeam(), nick, ((temp - heroes_times[i]) / 1000) - 1/*(*time_symbol[i] == 'm' ? 60000 :1000)*/);
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0, 0, 0, 1.0), "%s \tSEEN", nick);
				heroes_times[i] = GetTickCount();
			}
#ifdef _DEBUG
			ImGui::Text("[%d]%s\t%s\t%llx", heroes[i]->GetTeam(), heroes[i]->Schema_DynamicBinding()->binaryName + 17, heroes_isSeen[i] ? "seen" : "0", (__int64*)heroes[i]);
#endif
		}
}