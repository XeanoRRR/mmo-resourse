#pragma once
#include "resource.h"

class CMsInputTest
    : public CMs2DEngine
{
public:
    // ����
    virtual BOOL Ms2D_CallBack_Create(OUT S_2DEngineWndInfo& s_ewi);

    // ����
    virtual BOOL Ms2D_CallBack_Destroy(VOID);

    // ����
    virtual VOID Ms2D_CallBack_Render(HWND hWnd, HDC hDc);

    // �߼�
    virtual VOID Ms2D_CallBack_LogicTick(CMs2DInputEvent* pEvent);

    // ����
    virtual VOID Ms2D_CallBack_Activate(VOID);


};