// LoginServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Game.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    return AppFrame::InitApp(hInstance, nCmdShow, GameThreadFunc);
}
