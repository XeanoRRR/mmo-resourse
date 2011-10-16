#include "stdafx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\render.h"
#include "..\..\frostengine\ui.h"

#include "WndAbout.h"

WndAbout::WndAbout(void)
{
	m_pListUpdateInfo = NULL;
	m_strEditionTile = "Model Viewer 4.2.1.0";
}

WndAbout::~WndAbout(void)
{
}

BOOL WndAbout::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent)
{
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);

	RECT rc = {0,0,80,20};
	m_pBtnOK = new ui::Button;
	VERIFY(m_pBtnOK->Create("ȷ��",&rc,FWS_VISIBLE,this));
	m_pBtnOK->CenterWndTo((m_tWindowInfo.rcRect.right - m_tWindowInfo.rcRect.left)/2,pRect->bottom - 24);	

	RECT rcList = {0,50,300,470};
	m_pListUpdateInfo = new ui::ListBox;
	m_pListUpdateInfo->Create("UpdateInfo",&rcList,FWS_VISIBLE,this);

	// ��һ�Σ����ܸĽ�����ܼ��𣩣��ڶ��������µ�С���ܣ��������޸�Ŀǰ����������BUG�����Ķ�С�ķ�BUG���͵ĳ����޸�
	static vector<string> vecInfo;
	vecInfo.push_back("<�汾����>");
	vecInfo.push_back("1.0.0.0 (2003 ~ 2004.1 ����)");
	vecInfo.push_back("2.0.0.0 (2007.12.6 lpf)");
	vecInfo.push_back("  �����µ��첽��ȡģʽ");
	vecInfo.push_back("2.0.5.2 (2007.12.14)");
	vecInfo.push_back("  ���� - ��������ѡ����ʾģ�Ͳ�����Ӧ�ȼ�ʱ,��");
	vecInfo.push_back("         ��ȷ���Եı���");
	vecInfo.push_back("  lpf  - �����˾��еȼ����ԵĲ����޷���ʾ��ɫ��");
	vecInfo.push_back("         ���ͽ�������");
	vecInfo.push_back("3.0.0.0 (2008.01.04 lpf)");
	vecInfo.push_back("  �޸���ģ����������");
	vecInfo.push_back("  ��Ҫ�����˰�ģ���б�ͽ�ɫģ���б�");
	vecInfo.push_back("  ��������Ⱦģ�ͽ�UI�ؼ��ڵ�����");
	vecInfo.push_back("  ȫ������˿�ݼ�����");
	vecInfo.push_back("3.1.0.3 (2008.01.07 lpf)");
	vecInfo.push_back("  ��������ʾ����ͼƬ�Ĺ���");
	vecInfo.push_back("  ������ģ�ͼ���ʱ���ļ��������˴�����ʾ�ж�");
	vecInfo.push_back("3.1.1.3 (2008.01.17 lpf)");
	vecInfo.push_back("  �������������ֹ�����ģ���ļ�ʱ,��������.");
	vecInfo.push_back("3.2.2.3 (2008.01.18 ��ǫ)");						//line: 20"
	vecInfo.push_back("  ������ͶӰ������ʾ�Ĺ���");
	vecInfo.push_back("  ��ͶӰ����������ļ�������ʱ,�������ᱨ��");
	vecInfo.push_back("3.2.3.3 (2008.01.24 lpf)");
	vecInfo.push_back("  �ڰ󶨵�Ի�����������һ��[����ԭ��]�󶨵�");
	vecInfo.push_back("3.2.5.3 (2008.01.30 ��ǫ)");
	vecInfo.push_back("  �����˹���ͶӰ����Ķ���Ч������");
	vecInfo.push_back("  F5ˢ�¶���ʱ,ͶӰ������Ҳ�ᱻ����");
	vecInfo.push_back("3.3.4.3 (2008.02.02 lpf)");
	vecInfo.push_back("  ������������F5ˢ�¶���ʱ����");
	vecInfo.push_back("  ��F5ˢ�¶���ʱ,����ǰ����ģ�Ͷ�����ͷ����");
	vecInfo.push_back("3.4.5.4 (2008.02.28 lpf)");
	vecInfo.push_back("  ��������βģ�͵�������������ʾ");
	vecInfo.push_back("  ��������ģ�ͺ�סCtrl,�ƶ������ƶ�ģ��");
	vecInfo.push_back("3.4.6.4 (2008.03.26 ��ǫ)");
	vecInfo.push_back("  �����˹����Ƶ����Ź���");
	vecInfo.push_back("3.5.6.4 (2008.04.02 ��ǫ)");
	vecInfo.push_back("  �޸��˹�������Ч���Ĺ���");
	vecInfo.push_back("3.6.7.5 (2008.04.02 lpf)");
	vecInfo.push_back("  �����˶���֡��������");
	vecInfo.push_back("  ������ģ��β����ʾ");
	vecInfo.push_back("3.8.7.5 (2008.06.17 lpf)");
	vecInfo.push_back("  ͬ�����µ�����ײ�");
	vecInfo.push_back("   ������ȡ��������");				
	vecInfo.push_back("4.0.0.0 (2008.07.15)");							//line:44
	vecInfo.push_back("  �߿� - �����˶�ģ��Shader�ı༭����");
	vecInfo.push_back("  ��ǫ - ���ӷ������������µĹ���");
	vecInfo.push_back("4.1.0.0 (2008.07.16 �߿�)");
	vecInfo.push_back("  �༭Shaderʱ,�������滻������");
	vecInfo.push_back("4.2.0.0 (2008.07.28 �߿�)");
	vecInfo.push_back("  ������Model��part��trial��particle�ļ���");
	vecInfo.push_back("  �༭����");
	vecInfo.push_back("4.2.1.0 (2008.09.24 lpf)");
	vecInfo.push_back("  �����˵�������֡ʱ,�����ο�ģ�Ͷ���");

	for (size_t st = 0; st < vecInfo.size(); ++st)
	{
		m_pListUpdateInfo->InsertItem(vecInfo[st].c_str());
	}

	return TRUE;
}

void WndAbout::Destroy()
{
	SAFEDESTROY(m_pBtnOK);
	SAFEDESTROY(m_pListUpdateInfo);
	ui::Wnd::Destroy();
}

void WndAbout::Update()
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

void WndAbout::Render()
{
	render::Interface		   * pInterface	= render::Interface::GetInstance();
	render::Interface::Layer2D * pLayer2D   = pInterface->GetLayer2D();

	DWORD &dwStyle = m_tWindowInfo.dwStyle;

	if (dwStyle & FWS_VISIBLE)
	{
		RECT &rc = m_tWindowInfo.rcRect;
		int iX	 = rc.left, iY = rc.top, iW = rc.right - rc.left + 1, iH = rc.bottom - rc.top + 1;

		

		// ���Ʊ���
		pLayer2D->DrawSolidQuad(iX + 4, iY + 4, iW, iH, m_tWindowInfo.colorLow);
		pLayer2D->DrawSolidQuad(iX, iY, iW, iH, m_tWindowInfo.colorBack);
		pLayer2D->DrawWireQuad(iX, iY, iW, iH, m_tWindowInfo.colorHigh);

		// ��������
		DWORD dwTextWidth = pLayer2D->CalcTextWidth(m_strEditionTile.c_str(), (int)m_strEditionTile.length());
		int iStartX = iX + iW / 2 - dwTextWidth / 2;
		pLayer2D->DrawSolidQuad(iStartX - 3, iY + 12, dwTextWidth + 5, pLayer2D->GetFontSize() + 8, 0xf0001020);
		pLayer2D->TextOut(iStartX, iY + 14, m_strEditionTile.c_str(), (int)m_strEditionTile.length(), m_tWindowInfo.colorText);

		// ���ư汾��ʷ
		/*int iPosX = iX + 12;
		int iPosY = iY + 30;
		for (int i = 1; i < 52; ++i)
			pLayer2D->TextOut(iPosX, iPosY += (pLayer2D->GetFontSize() + 3), szInfo[i], lstrlen(szInfo[i]), m_tWindowInfo.colorText);*/
	}

	ui::Wnd::RenderChild();
}
void WndAbout::BeginPopUp(void)
{
	ModifyStyle(FWS_VISIBLE,0);
	ui::Manager::GetInstance()->BeginPopupWnd(this);
}
void WndAbout::EndPopUp(void)
{
	ModifyStyle(0,FWS_VISIBLE);
	ui::Manager::GetInstance()->EndPopupWnd();
}