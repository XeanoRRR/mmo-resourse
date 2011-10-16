#include "stdafx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\render.h"
#include "..\..\frostengine\ui.h"

#include "wndhotkey.h"

WndHotkey::WndHotkey(void)
{
}

WndHotkey::~WndHotkey(void)
{
}

BOOL WndHotkey::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent)
{
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);

	RECT rc = {0,0,80,20};
	m_pBtnOK = new ui::Button;
	VERIFY(m_pBtnOK->Create("ȷ��",&rc,FWS_VISIBLE,this));
	m_pBtnOK->CenterWndTo((m_tWindowInfo.rcRect.right - m_tWindowInfo.rcRect.left)/2,pRect->bottom - 24);	

	return TRUE;
}

void WndHotkey::Destroy()
{
	SAFEDESTROY(m_pBtnOK);

	ui::Wnd::Destroy();
}

void WndHotkey::Update()
{
	ui::Wnd::UpdateDefault();
	
	if (m_pBtnOK->IsClicked())
	{
		//�ر��Լ�
		ModifyStyle(0,FWS_VISIBLE);
		ui::Manager::GetInstance()->EndPopupWnd();
	}

	ui::Wnd::UpdateChild();
}

void WndHotkey::Render()
{
	render::Interface		   * pInterface	= render::Interface::GetInstance();
	render::Interface::Layer2D * pLayer2D   = pInterface->GetLayer2D();

	DWORD &dwStyle = m_tWindowInfo.dwStyle;

	if (dwStyle & FWS_VISIBLE)
	{
		RECT &rc = m_tWindowInfo.rcRect;
		int iX	 = rc.left, iY = rc.top, iW = rc.right - rc.left + 1, iH = rc.bottom - rc.top + 1;

		static const char * szInfo[15] ={"�ȼ��б�",
										 "�򿪽�ɫģ�� Ctrl+O    �Ƿ����ƹ�       - L",
										 "�򿪰�ģ�� Ctrl+B    �Ƿ���ʾ�ƹ�ģ��   - I",
										 "�򿪲ο�ģ�� Ctrl+R    �༭�ƹ���ɫ       - E",
										 "�˳�         Alt+X     �༭������ɫ       - K",
										 "                       �л�����ͼƬ����ɫ - S",
										 "",
										 "��ʾ����     - G       ���Ŷ���       - SPACE",
										 "��ʾ�߿�     - W       ������һ������     - A",
										 "��ʾ����     - N       �Ƿ�ѭ������       - P",
										 "��ʾ����     - J       �Ƿ���ʾ�������Ϣ - C",
										 "��ʾģ��β�� - T       �������λ         - R",
										 "                       �����ȡ����       - V",
										 "",
										 "��������ģ�� - F5",
										 };

		// ���Ʊ���
		pLayer2D->DrawSolidQuad(iX + 4, iY + 4, iW, iH, m_tWindowInfo.colorLow);
		pLayer2D->DrawSolidQuad(iX, iY, iW, iH, m_tWindowInfo.colorBack);
		pLayer2D->DrawWireQuad(iX, iY, iW, iH, m_tWindowInfo.colorHigh);

		// ��������
		DWORD dwTextWidth = pLayer2D->CalcTextWidth(szInfo[0], lstrlen(szInfo[0]));
		int iStartX = iX + iW / 2 - dwTextWidth / 2;
		pLayer2D->DrawSolidQuad(iStartX - 3, iY + 12, dwTextWidth + 5, pLayer2D->GetFontSize() + 8, 0xf0001020);
		pLayer2D->TextOut(iStartX, iY + 15, szInfo[0], lstrlen(szInfo[0]), m_tWindowInfo.colorText);

		// ���ư汾��ʷ
		int iPosX = iX + 12;
		int iPosY = iY + 30;
		for (int i = 1; i < 15; ++i)
			pLayer2D->TextOut(iPosX, iPosY += (pLayer2D->GetFontSize() + 3), szInfo[i], lstrlen(szInfo[i]), m_tWindowInfo.colorText);
	}

	ui::Wnd::RenderChild();
}
void WndHotkey::BeginPopUp(void)
{
	ModifyStyle(FWS_VISIBLE,0);
	ui::Manager::GetInstance()->BeginPopupWnd(this);
}
void WndHotkey::EndPopUp(void)
{
	ModifyStyle(0,FWS_VISIBLE);
	ui::Manager::GetInstance()->EndPopupWnd();
}