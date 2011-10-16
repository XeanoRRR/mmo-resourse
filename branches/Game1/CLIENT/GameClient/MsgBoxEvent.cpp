#include "StdAfx.h"
#include "Game.h"
#include ".\msgboxevent.h"

#include "..\nets\netclient\message.h"
//#include "..\appclient\interface\interface.h"
#include "..\appclient\other\GlobeSetup.h"

//////////////////////////////////////////////////////////////////////////
//MsgBoxEvent.h
//Func:MsgBox���е��¼���
//Author:joe
//Create Time:2005.2.2
//////////////////////////////////////////////////////////////////////////


CMsgBoxEvent::CMsgBoxEvent(void)
{
	ClearAllEvent();
}

CMsgBoxEvent::~CMsgBoxEvent(void)
{
}

void	CMsgBoxEvent::ClearAllEvent()
{
	//������ڣ���ı�
	list<tagMsgBoxEvent*>::iterator it = m_MsgBoxEvents.begin();
	for(;it != m_MsgBoxEvents.end();it++)
	{
		tagMsgBoxEvent* pEvent = (*it);

		switch(pEvent->eEvent)
		{
		case 0:
			{
			}
			break;
		default:
			{
			}
		}
		delete pEvent;
	}
	m_MsgBoxEvents.clear();
}


void	CMsgBoxEvent::AddEvent(eMsgBoxEvent eEvent,...)
{
	va_list va;
	va_start(va,eEvent);

	//������ڣ���ı�
	tagMsgBoxEvent *ptagEvent=NULL;

	list<tagMsgBoxEvent*>::iterator it = m_MsgBoxEvents.begin();
	for(;it != m_MsgBoxEvents.end();it++)
	{
		if((*it)->eEvent == eEvent)
		{
			ptagEvent = (*it);
			break;
		}
	}

	if(ptagEvent == NULL)
	{
		ptagEvent = new tagMsgBoxEvent;
		ptagEvent->eEvent=eEvent;
		m_MsgBoxEvents.push_back(ptagEvent);
	}

	switch(eEvent)
	{
	case MBE_DIEDSELECT:
		{
		}
		break;
	default:
		{
			return;
		}
		break;
	}		
}

//���� MsgBox ��������
void	CMsgBoxEvent::Run()
{
	if(m_MsgBoxEvents.size()<=0)
		return;
	char	strMsg[256];
	tagMsgBoxEvent* pMsgBoxEvent = m_MsgBoxEvents.front();
	switch(pMsgBoxEvent->eEvent)
	{
	case MBE_DIEDSELECT:		//����ѡ���¼�
		{
			sprintf(strMsg,"�Ƿ��ڸ���㸴��?");
			//long lRet = GetGame()->GetInterface()->GetMsgBox()->RunByNoModal(strMsg,
			//												CGameMsgBox::MWS_NO | CGameMsgBox::MWS_YES);
			//if(lRet ==  CGameMsgBox::MWS_YES)
			//{
			//	delete pMsgBoxEvent;
			//	m_MsgBoxEvents.pop_front();

			//	//CMessage msg(MSG_C2S_PLAYER_QUEST_RELIVE);
			//	//msg.Send();
			//}
			//else if(lRet ==  CGameMsgBox::MWS_NO)
			//{
			//	delete pMsgBoxEvent;
			//	m_MsgBoxEvents.pop_front();

			//	GetGame()->GetInterface()->AddText("������������Escѡ�񸴻����������Ĵ�ׯ����򸴻", CGlobeSetup::GetSetup()->colorWarningText, D3DCOLOR_ARGB(255,255,255,255));
			//	GetGame()->GetInterface()->AddOtherInfo("������������Escѡ�񸴻����������Ĵ�ׯ����򸴻", CGlobeSetup::GetSetup()->colorWarningText);
			//}
		}
		break;
	}
}

//��ʾMsgBox
void	CMsgBoxEvent::Display()
{
	if(m_MsgBoxEvents.size()<=0)
		return;
	//GetGame()->GetInterface()->GetMsgBox()->Display();
}
