#pragma once

#include <vector>
	

// �赲����
const DWORD dwBlockColor[5] = 
{
	0x00000000,		// ���赲
	0x80800000,		// �ɷ����赲
	0x80FF0000,		// ���ɷ����赲
	0x800000ff,		// ����
};

class EditWnd;


class ToolBlockEdit
{
	int m_iBrushSize;

	DWORD m_dwCurSelGrid;

	DWORD m_dwCurBLockType;//0 �ɷ����赲 1 ���ɷ����赲 2 ����

public:
	ToolBlockEdit(void);
	~ToolBlockEdit(void);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
	void _ShowStatus(void);
	
	void DrawBrush(void);
	void PaintBrush(DWORD dwBlock);

	DWORD GetSelectGrid(void);
};
