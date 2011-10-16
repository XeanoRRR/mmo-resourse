#pragma once
#include "..\..\frostengine\ui.h"

class WndMiniMap :
	public ui::Wnd
{

	DWORD m_dwReferenceTextureHandle;//�ο�ͼ

	//����
	DWORD m_dwMapTextureHandle;

	//����������ʵ����ʾ����Ϳ��
	int m_iMiniMapX,m_iMiniMapY,m_iMiniMapWidth,m_iMiniMapHeight;

	ui::Button *m_pBtnShowMapStatus;
	DWORD m_dwShowMapStatus;

public:
	WndMiniMap(void);
	~WndMiniMap(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent);
	void Destroy();

    virtual void Update();
	virtual void Render();

	void DrawArrow(int iX0,int iY0,int iX1,int iY1,DWORD dwColor);
	void SetMapPixel(DWORD dwGridIndex,DWORD dwColor);
	void UpdateMiniPixel();

	void CalculateMiniMapPosition();

	void LoadReference(const char *pszFileName);
	DWORD NewReference();
};
