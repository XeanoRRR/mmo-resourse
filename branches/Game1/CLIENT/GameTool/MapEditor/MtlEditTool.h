#pragma once



const DWORD dwMtlColor[10] =
{
	 0xa0000000//ȱʡ	
	,0xa000FF00//�ݵ�
	,0xa0FF0000//���
	,0xa0FFFF00//ɳĮ
	,0xa0FF00FF//ľ��
	,0xa0808080//ʯ��
	,0xa00000FF//ˮ��
	,0xa0FFFFFF//ѩ��
	,0xa000FFFF//����
};


//�˴�����MtlEdit�༭��

//���ݺͷ���
//������״̬
//1 ѡ��״̬

//2 ����״̬

class EditWnd;

class ToolMtlEdit
{
	int m_iBrushSize;

	DWORD m_dwCurSelGrid;

public:
	ToolMtlEdit(void);
	~ToolMtlEdit(void);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
	void _ShowStatus(void);
	
	void DrawBrush(void);
	void PaintBrush(DWORD dwMtl);

	DWORD GetSelectGrid(void);

};
