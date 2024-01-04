
#include "gui.h"



void load_fonts_from_folder(std::string font_path_name, float fontSize);

void ui_font_select();
void load_windows_font();


int StartImguiRender()
{

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGuiwin", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Timecycle editor", WS_OVERLAPPEDWINDOW, 100, 100, 900, 900, nullptr, nullptr, wc.hInstance, nullptr);

    HICON hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = NULL;
    io.LogFilename = NULL;


    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBackground
        | !ImGuiWindowFlags_MenuBar
        | ImGuiWindowFlags_NoSavedSettings;

    ImGui::StyleColorsDark();
    mStyle();




#if fonts0

    load_fonts_from_folder("f:\\source\\uc1\\uc1\\fonts0\\22\\" , 15.f);
#else
    load_windows_font();

#endif

    


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    int targetFPS = 300;
    const float background_color[4] = {27/255.0, 29/255.0, 31/255.0, 1.0};


#if build_tc_editor

    static gui_builder guiObj;

#endif


    bool done = false;
    while (!done)
    {
      
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }


        
        
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

       
       
#if build_tc_editor

        static const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y));
        ImGui::SetNextWindowPos(ImVec2(0.0, 0.0), ImGuiCond_Once);
        
        ImGui::Begin("main_window", NULL, window_flags);

#if fonts0
        ui_font_select();
#endif

        guiObj.file_section();
       

#if fps_check

        ImGui::SameLine();
        ImGui::Text(std::to_string(1.0f / io.DeltaTime).c_str());
#endif 

        if (!guiObj.get_file_load_state())
        {
            ImGui::BeginChild("Temp_params_window", ImVec2(-FLT_MIN, -FLT_MIN), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
            ImGui::EndChild();
        }
        else
        {
            switch (config_params::categories)
            {
            case(true):
                guiObj.categories_main_params_window();
                break;

            case(false):
                guiObj.main_params_window();
                break;
            }
        }

        ImGui::End();
        
#else

        ImGui::ShowDemoWindow();
#endif 
       
        ImGui::Render();


        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, background_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
 
        g_pSwapChain->Present(0, 0); 


        //fps limiter
        std::this_thread::sleep_for(std::chrono::milliseconds{static_cast<int>(1000/targetFPS)});

    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}







void mStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    style->GrabRounding = 4.0f;

    ImVec4* colors = style->Colors;

    style->CellPadding = { 0, 3 };
    style->FramePadding = { 4, 4 };

    style->Colors[ImGuiCol_PopupBg] = { 45.f / 255, 45.f / 255, 45.f / 255, 1.0f };
}








void load_windows_font()
{
    ImGuiIO& io = ImGui::GetIO();

    // latin& cyrillic& chinese
    static const ImWchar fontRange[] =
    {
        //Latin
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        //Chinese
         0x2000, 0x206F, // General Punctuation
         0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
         0x31F0, 0x31FF, // Katakana Phonetic Extensions
         0xFF00, 0xFFEF, // Half-width characters
         0xFFFD, 0xFFFD, // Invalid
         0x4e00, 0x9FAF, // CJK Ideograms

       0,
    };

    ImFontConfig regularConfig{};
    {
        regularConfig.RasterizerMultiply = 1.3f;
        regularConfig.OversampleH = 2;
        regularConfig.OversampleV = 1;
        regularConfig.RasterizerDensity = 1.2f;
    }

    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\calibri.ttf", 15.0f, &regularConfig, fontRange);

}





void load_fonts_from_folder(std::string font_path_name , float fontSize)
{
    ImGuiIO& io = ImGui::GetIO();

    static const ImWchar fontRange[] =
    {
        //Latin
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        //Chinese
         0x2000, 0x206F, // General Punctuation
         0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
         0x31F0, 0x31FF, // Katakana Phonetic Extensions
         0xFF00, 0xFFEF, // Half-width characters
         0xFFFD, 0xFFFD, // Invalid
         0x4e00, 0x9FAF, // CJK Ideograms

       0,
     };

     ImFontConfig regularConfig{};
     {
         regularConfig.RasterizerMultiply = 1.2f;
         regularConfig.OversampleH = 2;
         regularConfig.OversampleV = 1;
         regularConfig.RasterizerDensity = 1.2f;
     }

     file_handler font_files;
     auto font_map = font_files._get_files_map(font_path_name, ".ttf");
     auto oft_map = font_files._get_files_map(font_path_name, ".otf");




     for (auto [v1, v2] : font_map)
     {
         io.Fonts->AddFontFromFileTTF((font_path_name + v1).c_str(), fontSize, &regularConfig, fontRange);

     }
     for (auto [v1, v2] : oft_map)
     {
         io.Fonts->AddFontFromFileTTF((font_path_name + v1).c_str(), fontSize, &regularConfig, fontRange);
     }

      int index0 = 0;
      ImFont* font_current = ImGui::GetFont();

      for (ImFont* font : io.Fonts->Fonts)
      {

          std::cout << index0 << " Loaded font: " << font->GetDebugName() << std::endl;
          index0++;

      }
}






void ui_font_select()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font_current = ImGui::GetFont();

    bool btn_left = false;
    bool btn_rigt = false;
    
    size_t current_index = 0;

    for (size_t i = 0; i < io.Fonts->Fonts.size(); i++)
    {
        if (io.Fonts->Fonts[i]->GetDebugName() == font_current->GetDebugName())
        {
            current_index = i;
            break;
        }
    }


    ImGui::Text(std::to_string(current_index).c_str());
    ImGui::SameLine();
    ImGui::Text((" / " + std::to_string(io.Fonts->Fonts.size() - 1)).c_str());
    ImGui::Text("\t Строка \t");

    if (ImGui::Button("   <   "))
    {
        if (current_index != 0)
        {
            io.FontDefault = io.Fonts->Fonts[--current_index];
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("   >   "))
    {
        if (current_index < io.Fonts->Fonts.size() - 1)
        {
            io.FontDefault = io.Fonts->Fonts[++current_index];
        }
    }

    char str[255];
    strcpy_s(str, font_current->GetDebugName());

    ImGui::InputText("##window_text11", str, sizeof(str), ImGuiInputTextFlags_ReadOnly);

    if (ImGui::BeginCombo("##123", font_current->GetDebugName()))
    {
        for (ImFont* font : io.Fonts->Fonts)
        {
            ImGui::PushID((void*)font);

            if (ImGui::Selectable(font->GetDebugName(), font == font_current))
            {
                io.FontDefault = font;
            }

            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    ImGui::Text(" ");
}

















// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}