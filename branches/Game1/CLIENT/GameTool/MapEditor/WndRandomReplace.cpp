#include "stdafx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"

#include "commui.h"
#include "commdb.h"
#include "map.h"
#include "cmdmgr.h"
#include "editwnd.h"
#include "dooreditwnd.h"
#include "wndrandomreplace.h"

extern const char *szChangePointType[];

WndRandomReplace::WndRandomReplace(void)
{
	m_bIsPopupCheckFlag = FALSE;
	m_bIsOK = FALSE;
}

WndRandomReplace::~WndRandomReplace(void)
{
}


BOOL WndRandomReplace::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent)
{
	//��������
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);

	//�����б��
	RECT rcListBox = {10,44,138,206};
	m_pListBox = new ui::ListBox;
	m_pListBox->Create("�滻��ѡ",&rcListBox,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//��Ӱ�ť
	RECT rcAddRECT = {151,79,216,101};
	m_pBtnAdd = new ui::Button;
	m_pBtnAdd->Create("���",&rcAddRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//ɾ����ť
	RECT rcDelRECT = {150,116,216,138};
	m_pBtnDel= new ui::Button;
	m_pBtnDel->Create("ɾ��",&rcDelRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG  ,this);

	//ȷ����ť
	RECT rcOkRECT = {45,225,111,246};
	m_pBtnOk= new ui::Button;
	m_pBtnOk->Create("�滻",&rcOkRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//������ť
	RECT rcCancelRECT = {129,225,195,246};
	m_pBtnCancel= new ui::Button;
	m_pBtnCancel->Create("ȡ��",&rcCancelRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//������ť
	RECT rcTargetRECT = {47,11,218,32};
	m_pEditTarget= new ui::EditBox;
	m_pEditTarget->Create("Ŀ��",&rcTargetRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	RECT rcSourceRECT = {151,44,217,65};
	m_pEditSource = new ui::EditBox;
	m_pEditSource->Create("Դ",&rcSourceRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//������ť
	RECT rcMinDistanceRECT = {153,153,220,176};
	m_pEditMinDistance= new ui::EditBox;
	m_pEditMinDistance->Create("Min",&rcMinDistanceRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	RECT rcMaxDistanceRECT = {153,184,220,206};
	m_pEditMaxDistance = new ui::EditBox;
	m_pEditMaxDistance->Create("Max",&rcMaxDistanceRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	return TRUE;
}

void WndRandomReplace::Destroy()
{
	//���ٿؼ�
	SAFEDESTROY(m_pEditMinDistance);
	SAFEDESTROY(m_pEditMaxDistance);
	SAFEDESTROY(m_pEditSource);
	SAFEDESTROY(m_pEditTarget);
	SAFEDESTROY(m_pBtnOk);
	SAFEDESTROY(m_pBtnCancel);
	SAFEDESTROY(m_pBtnDel);
	SAFEDESTROY(m_pBtnAdd);
	SAFEDESTROY(m_pListBox);

	//���ٴ���
	ui::Wnd::Destroy();
}

void WndRandomReplace::Update()
{
	//UPDATE DEFAULT
	ui::Wnd::UpdateDefault();

	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	if (dwStyle & FWS_VISIBLE && !(dwStyle & FWS_DISABLE))
	{
		if (m_pBtnAdd->IsClicked())
		{
			char szTemp[256];
			m_pEditSource->GetEditText(szTemp,256);
			if (lstrlen(szTemp))
			{
				m_pListBox->InsertItem(szTemp);
			}
			m_pEditSource->Clear();
		}

		if (m_pBtnDel->IsClicked())
		{
			//ɾ��
			int iSelItem = m_pListBox->GetSelItem();
			if (iSelItem >= 0 )
			{
				m_pListBox->DeleteItem(iSelItem);
			}
		}

		if (m_pBtnOk->IsClicked())
		{
			EndPopUp();
			m_bIsOK = TRUE;
			m_bIsPopupCheckFlag= TRUE;
		}
		if (m_pBtnCancel->IsClicked())
		{
			EndPopUp();
			m_bIsOK = FALSE;
			m_bIsPopupCheckFlag = TRUE;
		}

		if (m_pListBox->IsSelChanged())
		{
			int iSelItem = m_pListBox->GetSelItem();
			if (iSelItem >= 0 )
			{
				//��ģ��
			}
		}

		ui::Wnd::UpdateChild();
	}

}

void WndRandomReplace::BeginPopUp(void)
{
	ModifyStyle(FWS_VISIBLE,0);
	ui::Manager::GetInstance()->BeginPopupWnd(this);
}

void WndRandomReplace::EndPopUp()
{
	ModifyStyle(0,FWS_VISIBLE);
	ui::Manager::GetInstance()->EndPopupWnd();
}