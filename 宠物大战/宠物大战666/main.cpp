#include <graphics.h>
//#include <windows.h>
//#include<stdio.h>
#include "Menu.h"

int main()
{
    //AllocConsole();
    //FILE* fp;
    //freopen_s(&fp, "CONOUT$", "w", stdout);
    initgraph(1280, 720);
    ShowMenu();
    closegraph();
    return 0;
}
