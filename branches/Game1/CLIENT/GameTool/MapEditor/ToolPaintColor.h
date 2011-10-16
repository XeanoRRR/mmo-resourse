//---------------------------------------------------------------------------------
//��дby ����
//---------------------------------------------------------------------------------
#pragma once
#include "CmdPaintColor.h"

// �༭״̬

enum PAINTCOLORSTATUETYPE
{
	PCST_IDLE = 0,					// ����
	PCST_PAINT,						// Ϳɫ
	PCST_PICKFORECOLOR,				// ѡǰ��ɫ
	PCST_PICKBACKCOLOR				// ѡ��ɫ
};

class ToolPaintColor
{
private:
	RECT	m_rcColorDialog;													// ��¼ѡɫ�Ի����rc
	RECT	m_rcForeColorQuad, m_rcBackColorQuad;								// ��¼��ɫ����rc
	DWORD	m_dwForeColor, m_dwBackColor;										// ѡ�е���ɫ������ǰ��ɫ�ͺ�ɫ��
	DWORD	m_dwTextureIndex;													// ѡɫ�����ͼ��index

	float   m_fBrushRadius;															// ��ˢ�Ĵ�С

	DWORD   m_dwHoverVertex;

	float m_fColorChroma;														// ��ɫŨ��


	PAINTCOLORSTATUETYPE	m_eEditStatus;										// �༭״̬
	DWORD					m_dwBrushType;										// ��������(0 Ǧ�ʣ�1ë��)

public:
	ToolPaintColor(void);
	~ToolPaintColor(void);

	void Create(TCHAR *colorfile, RECT &rcColorDialog, RECT &rcForeColorQuad, RECT &rcBackColorQuad);
	void Destroy();
	void MessageProc(BOOL bActive);
	void Render();
	void ShowColorDialog();
	DWORD PickColor(DWORD x, DWORD y);
	DWORD  GetHoverVertex();

	void _ShowStatus();
	void DrawBrush(void);
};
