// GameServer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "StdAfx.h"
#include "Game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine,
                       int nCmdShow)
{
    return AppFrame::InitApp(hInstance, nCmdShow, GameThreadFunc);
}
