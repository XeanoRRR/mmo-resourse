#pragma once
#include "stdafx.h"
#define MOUSEBUFFERSIZE			16

extern HANDLE              g_hMouseEvent;

enum {
	NONE = 0,
	LB_DOWN = 2,
	LB_UP = 3,					// ���I
	LB_DCLICK = 6,

	RB_DOWN = 4,
	RB_UP = 5,					// ���I
	RB_DCLICK = 7,

	MB_DOWN = 8,
	MB_UP = 9,					// ���I
	MB_DCLICK = 10,

	MB_WHEEL,					// ����
};

struct tagMouseKey
{
	long lX, lY;
	long lKey;
};


class CMouse  
{
public:
	CMouse();
	virtual ~CMouse();

public:
	bool m_bLeftDown;										// �������Ƿ���
	bool m_bRightDown;										// ����Ҽ��Ƿ���
	bool m_bMiddleDown;										// ����м��Ƿ���

	long					m_lX;							// ����ƶ�ʱ���X
	long					m_lY;							// ����ƶ�ʱ���Y
	long					m_lOldX;						// �����һ��ʱ���X
	long					m_lOldY;						// �����һ��ʱ���Y
	DWORD					m_dwDelta;						// ����ƫ��

private:
	queue <tagMouseKey*>		m_queueMouseKey;			// �����Ϣ���ڲ�����

public:

	bool Create(HINSTANCE hInst, HWND hWnd);
	void RecieveMouseInput();
	void SetPosition(long x, long y);

	long GetMouseX(void) { return m_lX; }
	long GetMouseY(void) { return m_lY; }
	long GetOldMouseX(void) { return m_lOldX; }
	long GetOldMouseY(void) { return m_lOldY; }
	long GetMouseMoveX()	{return m_lX-m_lOldX;}
	long GetMouseMoveY()	{return m_lY-m_lOldY;}

	bool IsLeftButtonDown(void) { return m_bLeftDown; }
	bool IsRightButtonDown(void) { return m_bRightDown; }
	bool IsMiddleButtonDown(void) { return m_bMiddleDown; }
	void SetCurrentButton(long value);
	long GetCurrentButton(void);
	DWORD GetWheelDelta()	{return m_dwDelta;}

// ����/̎�� windowϵ�y��ϵ�y�����Ϣ
public:
	bool MouseMessageProc(long lMessage, POINT pointMousePos, WPARAM wParam);
	void PushMouseKey(long lMouseX, long lMouseY, long lValue);

	void ClearMessage();
};
