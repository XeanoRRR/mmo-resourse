#include "stdafx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"

#include "commui.h"
#include "commdb.h"
#include "map.h"
#include "cmdmgr.h"
#include "editwnd.h"
#include "dooreditwnd.h"
#include "changewnd.h"

extern const char *szChangePointType[];

WndDoorEdit::WndDoorEdit(void)
{
}

WndDoorEdit::~WndDoorEdit(void)
{
}


BOOL WndDoorEdit::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent)
{
	//��������
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);

	//�����б��
	RECT rcListBox = {5,6,132,6 + 275};
	m_pListBox = new ui::ListBox;
	m_pListBox->Create("DoorLIST",&rcListBox,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//�����б��
	RECT rcListDetail= {141,76,274,280};
	m_pListDetail = new ui::ListBox;
	m_pListDetail->Create("DetailLIST",&rcListDetail,FWS_VISIBLE | FWS_DISABLE,this);

	//��Ӱ�ť
	RECT rcAddRECT = {138,8,202,29};
	m_pBtnAdd = new ui::Button;
	m_pBtnAdd->Create("���",&rcAddRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);

	//������ť
	RECT rcDelRECT = {210,8,278,29};
	m_pBtnDel= new ui::Button;
	m_pBtnDel->Create("ɾ��",&rcDelRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG  ,this);

	//������ť
	RECT rcModifyRECT = {138,34,203,34 + 20};
	m_pBtnModify= new ui::Button;
	m_pBtnModify->Create("�޸�",&rcModifyRECT,FWS_VISIBLE | FWS_OUTLINE | FWS_FILLBKG ,this);


	//�������ܱ༭

	return TRUE;
}

void WndDoorEdit::Destroy()
{
	for(DWORD n = 0 ; n < m_vDoorList.size(); n++)
	{
		SAFEDELETE(m_vDoorList[n]);
	}
	m_vDoorList.clear();

	//���ٿؼ�
	SAFEDESTROY(m_pListDetail);
	SAFEDESTROY(m_pBtnModify);
	SAFEDESTROY(m_pBtnDel);
	SAFEDESTROY(m_pBtnAdd);
	SAFEDESTROY(m_pListBox);

	//���ٴ���
	ui::Wnd::Destroy();
}

void WndDoorEdit::Update()
{
	//UPDATE DEFAULT
	ui::Wnd::UpdateDefault();

	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	if (dwStyle & FWS_VISIBLE && !(dwStyle & FWS_DISABLE))
	{
		if (m_pBtnAdd->IsClicked())
		{
			//Comm
			CommUI *pCommUI = CommUI::GetInstance();
			EditWnd *pEditWnd = pCommUI->GetEditWnd();
			WndPopupDoorEdit *pDoorWnd = pEditWnd->GetWndPopupDoorEdit();
			pDoorWnd->BeginPopUp();

		}

		if (m_pBtnDel->IsClicked())
		{
			//ɾ�����
			int iSelItem = m_pListBox->GetSelItem();
			if (iSelItem >= 0 )
			{
				tagDoor *pDoor = m_vDoorList[iSelItem];
				for(std::vector<tagDoor*>::iterator it = m_vDoorList.begin();
					it != m_vDoorList.end(); it++)
				{
					if (pDoor == *it)
					{
						m_vDoorList.erase(it);
						break;
					}
				}
				//�������ListBox
				m_pListBox->DeleteAllItem();
				for(std::vector<tagDoor*>::iterator it = m_vDoorList.begin();
					it != m_vDoorList.end(); it++)
				{
					m_pListBox->InsertItem((*it)->szName);					
				}
			}

			//ͬʱ�������еĵ�ͼ����������
			EditMap *pMap = CommDB::GetInstance()->GetMap();
			for(int i = 0; i < pMap->GetWidth(); i++)
			{
				for(int j = 0; j < pMap->GetDepth(); j++)
				{
					Grid *pGrid = pMap->GetGrid(i,j);
					DWORD dwDoorIndex = pGrid->GetDoorIndex();
					if (dwDoorIndex)//��1��ʼ��
					{
						dwDoorIndex--;
						if (dwDoorIndex < (DWORD)iSelItem)
						{
							//��ǰ���index�����		 
						}
						else
							if (dwDoorIndex > (DWORD)iSelItem)
							{
								//�����ͳͳ��ȥһ
								pGrid->SetDoorIndex(dwDoorIndex);
							}
							else
							{
								//��ȵĹ���,�����
								pGrid->SetDoorIndex(0);
							}
					}
				}

			}
			//ͬʱ��������б�
			CmdMgr::GetInstance()->Clear();
		}

		if (m_pBtnModify->IsClicked())
		{
			 //Comm
			CommUI *pCommUI = CommUI::GetInstance();
			EditWnd *pEditWnd = pCommUI->GetEditWnd();
			WndPopupDoorEdit *pDoorWnd = pEditWnd->GetWndPopupDoorEdit();

			//ɾ�����
			int iSelItem = m_pListBox->GetSelItem();
			if (iSelItem >= 0 )
			{
				tagDoor *pDoor = m_vDoorList[iSelItem];
				for(std::vector<tagDoor*>::iterator it = m_vDoorList.begin();
					it != m_vDoorList.end(); it++)
				{
					if (pDoor == *it)
					{
						pDoorWnd->SetDoor(pDoor);
						pDoorWnd->BeginPopUp();
						break;
					}
				}
			}

			

		}


		if (m_pListBox->IsSelChanged())
		{
			int iSelItem = m_pListBox->GetSelItem();
			if (iSelItem >= 0 )
			{
				tagDoor *pDoor = m_vDoorList[iSelItem];
				//������ݵ�ListDetail
				char szTemp[256];
				m_pListDetail->DeleteAllItem();
				_snprintf(szTemp,255,"Name: %s\n",pDoor->szName);
				m_pListDetail->InsertItem(szTemp);
				_snprintf(szTemp,255,"X: %d Y: %d\n",pDoor->tSwitch.lCoordX,pDoor->tSwitch.lCoordY);
				m_pListDetail->InsertItem(szTemp);			
				_snprintf(szTemp,255,"��ͼ���: %d\n",pDoor->tSwitch.lRegionID);
				m_pListDetail->InsertItem(szTemp);			
				_snprintf(szTemp,255,"����: %d\n",pDoor->tSwitch.lDir);
				m_pListDetail->InsertItem(szTemp);			
				_snprintf(szTemp,255,"����: %s\n",szChangePointType[pDoor->tSwitch.lState]);
				m_pListDetail->InsertItem(szTemp);			
			}
		}

		ui::Wnd::UpdateChild();
	}

}
void WndDoorEdit::InsertDoor(tagDoor *pDoor)
{
	//����door
	for(int i = 0; i <(int)m_vDoorList.size(); i++)
	{
		tagDoor *pTempDoor = m_vDoorList[i];

		if (strcmp(pTempDoor->szName,pDoor->szName) == 0)
		{
			//�滻
			memcpy(pTempDoor,pDoor,sizeof(tagDoor));
			return ;
		}
	}
	//�������
	m_vDoorList.push_back(pDoor);
	m_pListBox->InsertItem(pDoor->szName);
}

int WndDoorEdit::GetCurDoorIndex()
{
	return m_pListBox->GetSelItem();
}
tagDoor *WndDoorEdit::GetDoor(DWORD dwIndex)
{ 
	return m_vDoorList[dwIndex];
}

void WndDoorEdit::ClearAllDoor()
{
	for(DWORD n = 0 ; n < m_vDoorList.size(); n++)
	{
		SAFEDELETE(m_vDoorList[n]);
	}
	m_vDoorList.clear();	
	m_pListBox->DeleteAllItem();
}
