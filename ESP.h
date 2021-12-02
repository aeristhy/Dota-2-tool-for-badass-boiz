#pragma once
#include "GlobalVars.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "../imgui/backends/imgui_impl_win32.h"
#include "vec2.h"
#include "CDOTA_PanoramaMinimapRenderer.h"
#include "D3D11Renderer.h"

void DrawFilledRect9(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void DrawFilledRect11(int x, int y, int w, int h, Color_class color, D3D11Renderer* dev)
{
	dev->FillRect(x,y,w,h, color);
}


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

void WorldToMinimap(float* pos, vec2 * screen)
{
	/*
	World is size 1000x1000.
	Minimap is size 2x2.
	Ignore z - axis.
	Player is in position 230x740.
	Calculate x - position on minimap as 230 * 2 / 1000,
	y - position as 740 * 2 / 1000.
	*/
	auto q = MinimapBoundsManipulator->GetBoundsMax();
	auto w = MinimapBoundsManipulator->GetBoundsMin();
	auto PixelBounds = MinimapPixelBoundsManipulator->GetMinimapSizeInPixel();

	if (w.x < 0.0)
		w.x *= -1;
	if (w.y < 0.0)
		w.y *= -1;

	auto wx = w.x;
	auto wy = w.y;


	auto xsum = (q.x + wx);
	auto ysum = (q.y + wy);

	pos[0] += wx;
	pos[1] += wy;


	auto x = pos[0] / xsum;
	auto y = pos[1] / ysum;

	pos[0] -= wx;
	pos[1] -= wy;

	PixelBounds.x += x_minimap_size; //10
	PixelBounds.y += y_minimap_size;


	auto yboundtemp = PixelBounds.y;

	x *= PixelBounds.x;
	y = yboundtemp * (-y);

	y += view.Height;
	screen->x = x -(x_minimap_offset);
	screen->y = y +(y_minimap_offset); //-20
}


void esp(ID3D11DeviceContext* pContext, D3D11Renderer* renderer)
{
	
	//pDevice->GetViewport(&view); rip (╥﹏╥)
	unsigned int viewports_num = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	pContext->RSGetViewports(&viewports_num, &view);

	if (!LocalPlayer)
	{
		Sleep(0);
		//for (char i = 0; i < 5; i++)
		//{
		//	if ((__int64)heroes[i] > (__int64)1)
		//		if (IsVisibleByTeam(heroes[i], (heroes[i]->GetTeam() == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT)))
		//		//if(heroes[i]->IsLocalPlayer())
		//		{
		//			LocalPlayer = heroes[i];
		//			LocalPlayerID = i;
		//			localPlayerTeam = LocalPlayer->GetTeam();
		//			EnemyTeam = localPlayerTeam == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT;
		//			break;
		//		}

		//}
	}
	else
	{
		if (DrawHealthPanel)
			ImGui::Begin("HealthPanel");

		auto q = GameEntitySystem->GetPlayerPool();
		
		for (int i = 0; i < q->GetPlayerHighestIndex(); i++)
		{
			auto p = q->GetPlayerByPlayerIndex(i);
			auto entIndex = p->GetHeroEntityIndex();

			CBaseEntity* ent = 0;
			for (int i = 0; i < heroes_slots; i++)
				if (heroes_index[i] == entIndex)
				{
					ent = heroes[i];
					break;
				}
			if (ent)
			{
				auto hero = ent;
				auto heroName = hero->Schema_DynamicBinding()->binaryName + 17;
				auto heroTeam = hero->GetTeam(); //do not compile with optimization enabled. This func fucks up all things
				auto heroVec3 = hero->GetSleleton()->GetPos();



				auto heroIsVisible = IsVisibleByTeam(hero, (heroTeam == 2 ? DOTATeam_t::DOTA_TEAM_DIRE : DOTATeam_t::DOTA_TEAM_RADIANT));

				vec2 screen;
				screen.x = 0;
				screen.y = 0;

				long long temp = fuckingMatrix + 0x288;
				WorldToScreen(*(D3DVECTOR*)heroVec3, &screen, (float*)temp, view.Width, view.Height);

				

				RECT rect;
				rect.left = screen.x;
				rect.top = screen.y;
				rect.right = screen.x + 500;
				rect.bottom = screen.y + 50;


				if (heroTeam != EnemyTeam)
				{
					int c = hero->GetModifiersCount();
					auto ModPool = hero->GetModifiersPool();
					LastModifiersCount[i][0] = c;
					for (int w = 0; w < c; w++)
					{
						Modifiers[i][w] = 0;
						auto Mod = ModPool->GetModifier(w);
						if (!Mod)
							continue;
						Modifiers[i][w] = Mod;
						if (!memcmp(Mod->Name(), "modifier_truesight", 18))
						{
							vec2 minimapper = { 0,0 };
							if (TrueSightIndicator3DWorld)
								DrawFilledRect11(screen.x, screen.y, 50, 50, Color_class(0xFF, 0x00, 0x00, 0xFF), renderer);
							if (TrueSightIndicatorMinimap)
							{
								WorldToMinimap(ent->GetSleleton()->GetPos(), &minimapper);

								DrawFilledRect11(minimapper.x, minimapper.y+5, 10, 5, Color_class(0xFF, 0x00, 0x00, 0xFF), renderer);
							}
						}
					}

					if (SelfVisibeByEnemy && heroIsVisible)
					{
						if (SelfVisibleByEnemy3DWorld)
							DrawFilledRect11(screen.x, screen.y, 50, 25, Color_class(0xFF, 0xFF, 0xFF, 0), renderer);
#ifdef _DEBUG
						vec2 minimapper = {0,0};
						if (SelfVisibleByEnemyOnMinimap)
						{
							WorldToMinimap(ent->GetSleleton()->GetPos(), &minimapper);

							DrawFilledRect11(minimapper.x, minimapper.y, 10, 5, Color_class(0xFF, 0xFF, 0xFF, 0), renderer);
						}
#endif
					}
				}
				else //if heroTeam==EnemyTeam
				{
#ifdef _DEBUG
					if (enemyCD)
					{
						char CDString[100];
						auto npcinf = hero->GetNpcInfo();
						for (char i = 0; i < 16; i++)
						{
							auto ab = npcinf->GetAbility(i);
							if (ab)
							{
								auto nm = ab->GetName();
								//auto lv = ab->GetLevel();
								auto cd = ab->GetLastCooldown();
								auto max_cd = ab->GetMaxCooldown();
								if (!nm)//|| !*nm
									continue;
								else
								{
									auto npm = npcinf->GetNpcName();
									//auto npm_len = strlen(npm);
									//nm += npm_len;
									if (!memcmp(nm, npm, strlen(npm)))
									{
										rect.top += 15;
										rect.bottom += 15;
										memset(CDString, 0, 100);
										sprintf(CDString, "%s [%0.f] %0.f", nm, cd, max_cd);
										//font->DrawTextA(0, CDString, 100, &rect, 0, D3DCOLOR_ARGB(255, 0, 255, 0));

									}
								}
							}

						}
						//rect.top += 50;
						//rect.bottom += 50;
						//font->DrawTextA(0, CDString, strlen(CDString), &rect, 0, D3DCOLOR_ARGB(255, 255, 0, 0));

					}
#endif
					if (DrawHealthPanel)
					{
						char HeroName[7];
						memcpy(HeroName, heroName, 6);
						HeroName[6] = 0;
						auto currentHealth = hero->GetCurrentHealth();
						ImVec4 color;
						color.x = (float)1.0;
						color.w = (float)0.2;
						color.y = (float)0.2;
						color.z = (float)0.2;
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
						ImGui::TextColored(color, "%s\t%d/%d", HeroName, currentHealth, hero->GetMaxHealth());
					}
					if (TrueHero && heroIsVisible)
					{
						if (TrueHeroIndicator3DWorld)
							DrawFilledRect11(screen.x, screen.y - 10, 50, 10, Color_class(0xFF, 0x11, 0xFF, 0x55), renderer);
						if (TrueHeroIndicatorMinimap)
						{
							vec2 minimapper = { 0,0 };
							WorldToMinimap(ent->GetSleleton()->GetPos(), &minimapper);

							DrawFilledRect11(minimapper.x, minimapper.y, 10, 5, Color_class(0xFF, 0x11, 0xFF, 0x55), renderer);
						}
					}
				}
			}
		}
			if (DrawHealthPanel)
				ImGui::End();
		
	}
#ifdef _DEBUG
	for (int i = 0; i < xlam_slots; i++)
	{
		

		if (!xlam[i])
			continue;
		auto xx = xlam[i]->GetTeam();
		if (xx != DOTA_TEAM_DIRE && xx != DOTA_TEAM_RADIANT)
			continue;
		auto xlamName = xlam[i]->Schema_DynamicBinding()->binaryName;
		if (!xlamName)
			continue;

		auto Skeleton = xlam[i]->GetSleleton();
		if (!Skeleton)
			continue;

		auto xlamVec3 = Skeleton->GetPos();

		vec2 screen;
		screen.x = 0;
		screen.y = 0;

		long long temp = fuckingMatrix + 0x288;
		WorldToScreen(*(D3DVECTOR*)xlamVec3, &screen, (float*)temp, view.Width, view.Height);

		RECT rect;
		rect.left = screen.x;
		rect.top = screen.y;
		rect.right = screen.x + 500;
		rect.bottom = screen.y + 50;

		if (screen.x && screen.y)
			if (IsVisibleByTeam(xlam[i], DOTATeam_t::DOTA_TEAM_RADIANT) && IsVisibleByTeam(xlam[i], DOTATeam_t::DOTA_TEAM_DIRE))
			{
				DrawFilledRect11(rect.left, rect.top, 5, 5, Color_class(255, 255, 0, 0), renderer);
				
				//font->DrawTextA(0, xlamName, (INT)strlen(xlamName), &rect, 0, D3DCOLOR_ARGB(255, 255, 0, 0));//works fine
			}
	}
#endif

}

void OutputModifiers()
{
	for (char i = 0; i < LastModifiersCount[LocalPlayerID][0]; i++)
	{
		auto Mod = Modifiers[LocalPlayerID][i];
		if (Mod && Mod->GetTeamOwner() != localPlayerTeam)
		{
			auto ModName = Mod->Name();
			char buff[256];
			memset(buff, 0, 256);
			memcpy(buff, ModName, strlen(ModName));
			ImGui::Text("[%d]%s", Mod->GetTeamOwner(), buff);
		}
	}
}

#ifdef _DEBUG


//void DrawQuad3D(float q, float w, float e, LPDIRECT3DDEVICE9 pDevice)
//{
//	vec2 screen;
//	screen.x = 0;
//	screen.y = 0;
//	D3DVECTOR qwe;
//	qwe.x = q;
//	qwe.y = w;
//	qwe.z = e;
//	long long temp = fuckingMatrix + 0x288;
//	WorldToScreen(qwe, &screen, (float*)temp, view.Width, view.Height);
//	DrawFilledRect11(screen.x, screen.y, 5, 5, D3DCOLOR_ARGB(255, 0xAA, 0xAA, 0xEE), pDevice);
//
//}

#endif


void debugWindow()
{
#ifdef _DEBUG
	for (char i = 0; i < heroes_slots; i++)
		if ((__int64)heroes[i] > (__int64)1)
		{
			auto heroName = heroes[i]->Schema_DynamicBinding()->binaryName + 17;
			auto heroTeam = heroes[i]->GetTeam();

			ImGui::Text("[%d]%s %llx", heroTeam, heroName, (__int64*)heroes[i]);
		}
#endif
}
