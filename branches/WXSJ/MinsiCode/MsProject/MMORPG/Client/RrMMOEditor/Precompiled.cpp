#include "Precompiled.h"
BOOL MSCALL RrGameClientHookProc(MSG*);
LRESULT CALLBACK RrGameClientWndProc(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
CMsBaseRun s_CMsBaseRun(
    _T("%s/Log/Exe_RrClientEditor_Assertx.log"),
    _T("Exe_RrMMOClient"),
    _T("���MMOGAME"),
    RrGameClientHookProc,
    RrGameClientWndProc,
    NULL
    );
