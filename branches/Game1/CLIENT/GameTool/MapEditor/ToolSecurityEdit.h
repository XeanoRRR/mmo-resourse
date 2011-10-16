#pragma once

#include <vector>


const DWORD dwSecurityColor[8] =
{
	0x00000000,	    //��
	0x80004000,		//�ݵ�
	0xa0128013,		//���
};


//�˴�����MtlEdit�༭��

//���ݺͷ���
//������״̬
//1 ѡ��״̬

//2 ����״̬

class EditWnd;

class ToolSecurityEdit
{
	int m_iBrushSize;

	DWORD m_dwCurSelGrid;


public:
	ToolSecurityEdit(void);
	~ToolSecurityEdit(void);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
	void _ShowStatus(void);
	
	void DrawBrush(void);
	void PaintBrush(DWORD dwSecurity);

	DWORD GetSelectGrid(void);
};
