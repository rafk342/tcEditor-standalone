#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include <string>
#include <array>
#include <iostream>
#include <chrono>
#include <thread>

#include "gui_handler.h"
#include "../timecycle_handler.h"
#pragma execution_character_set("utf-8")
#include "../resource.h"


#define fonts0 false
#define build_tc_editor true
#define fps_check false



static ID3D11Device*			g_pd3dDevice = nullptr;
static ID3D11DeviceContext*		g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*			g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*	g_mainRenderTargetView = nullptr;



bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int StartImguiRender();
void mStyle();
