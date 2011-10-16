#pragma once

#include <vector>
	

const DWORD dwWarColor[8] =
{
	0x00000000,	    //�ǳ�ս
	0x800d2242		//��ս
};


//�˴�����MtlEdit�༭��

//���ݺͷ���
//������״̬
//1 ѡ��״̬

//2 ����״̬

class EditWnd;

class ToolWarEdit
{
	int m_iBrushSize;

	DWORD m_dwCurSelGrid;

public:
	ToolWarEdit(void);
	~ToolWarEdit(void);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
	void _ShowStatus(void);
	
	void DrawBrush(void);
	void PaintBrush(DWORD dwMtl);

	DWORD GetSelectGrid(void);

};
