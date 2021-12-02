#pragma once
#include "CustomTypes.h"
#include "D3D11Renderer.h"
#include "ESP.h"
#include "Menu.h"

#pragma region nomatter

bool boolka = false;



ImVec4 clear_color;

struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices


HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
    HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

    //if (SUCCEEDED(ret))
        (*ppDevice)->GetImmediateContext(ppContext);

    return ret;
}

#pragma endregion

HRESULT STDMETHODCALLTYPE meow(IDXGISwapChain* a, int b, int c)
{

    if (!boolka)
    {
        
        if (!RenderInitiated && ProcessWindowHandle)
		{
            GetDeviceAndCtxFromSwapchain(a, &pDevice, &pContext);
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
                //auto q = FindWindowA("Dota 2", "SDL_app");
                ImGui_ImplWin32_Init(ProcessWindowHandle);
                ImGui_ImplDX11_Init(pDevice, pContext);
                clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
                ImGui::SetNextWindowSize(ImVec2(200.0, 400));
                RenderInitiated = 1;
                boolka = true;
                renderer = new D3D11Renderer(a);
                renderer->Initialize();
            }
		}
    }
    if (GetDeviceAndCtxFromSwapchain(a, &pDevice, &pContext) >= 0 && RenderInitiated)
    //if (RenderInitiated)
    {
        
        //esp(pContext, renderer);
        menu();
        
        //renderer->BeginScene();
        //renderer->DrawString(L"xui 228", 13, 200.0, 400.0, 0xFFAAEEFF);
        //renderer->EndScene();

    }
    return render_orig(a, b, c);
}