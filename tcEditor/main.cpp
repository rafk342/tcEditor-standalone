#include <iostream>
#include "ui/gui.h"
#include "Preload.h"


#define show_console false;


int main() 
{ 

    HWND hwnd_console = GetConsoleWindow();

#if show_console
    ShowWindow(hwnd_console, 1);
#else
    ShowWindow(hwnd_console, 0);
#endif

   

    configHandler::readCfg();
    CategoriesHandler::LoadCategories();
    TooltipsHandler::load_tooltips();

    StartImguiRender();

    return 0;
}

int timecycle_item::id = 0;