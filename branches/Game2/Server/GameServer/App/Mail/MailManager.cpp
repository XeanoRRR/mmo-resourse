#include "StdAfx.h"
#include "MailManager.h"

#include "../Goods/CGoods.h"
#include "../Goods/CGoodsFactory.h"
#include "../Player.h"

#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"
#include "../../../../Public/Setup/MailList.h"
#include "../../../../Public/ServerPublic/Server/Common/WordsFilter.h"



extern void CancelIncrementTrading( CPlayer* pPlayer );
CMailManager::CMailManager()
{	
	m_mapMail.clear();	
}

CMailManager::~CMailManager()
{
	for (map<CGUID,CMail*>::iterator it=m_mapMail.begin(); it!=m_mapMail.end(); ++it)
	{
		it->second->DeleteMailGoods();
		OBJ_RELEASE(CMail, it->second);
	}
	m_mapMail.clear();

	for (map<long,CMail*>::iterator it=m_mapSysMail.begin(); it!=m_mapSysMail.end(); ++it)
	{
		OBJ_RELEASE(CMail, it->second);
	}
	m_mapSysMail.clear();
}



//����
bool CMailManager::SetMapMail(CMail *pMail)
{
   	time_t t_CurrentTime = time(NULL);
	map<CGUID,CMail*>::iterator it=m_mapMail.find(pMail->GetExID());
	if (it!=m_mapMail.end())//�ҵ�����ʼ��˾���ɾ��
	{
		CMail *pMail = (*it).second;
		it=m_mapMail.erase(it);	
		pMail->DeleteMailGoods();
		OBJ_RELEASE(CMail, pMail);
	}

	if ((pMail->GetType()==SYSTEM_MAIL || pMail->GetType()>=MAX_MAIL) && !stricmp(pMail->GetWriter().c_str(),AppFrame::GetText("GS_SYSMAIL_THIRD"/*�����Ƽ�*/)))
	{
		char strSubject[256];
		_snprintf(strSubject, 256, AppFrame::GetText("GS_SYSMAIL_FIFTH"/*ϵͳ:%s*/),pMail->GetSubject().c_str());
		pMail->SetSubject(strSubject);
	}
	if (pMail->GetType()>=MAX_MAIL)
	{
		long lSysMailID = pMail->GetType();
		string strText;
		map<long,CMail*>::iterator mit = m_mapSysMail.find(lSysMailID);
		if (mit!=m_mapSysMail.end())
		{
			strText = (*mit).second->GetText();
			pMail->SetText(strText.c_str());
			pMail->SetType(SYSTEM_MAIL);
		}
		else
		{			
			m_mapMail[pMail->GetExID()] = pMail;
			return false;
		}
	}

	m_mapMail[pMail->GetExID()] = pMail;
	return true;	
}
bool CMailManager::AddMail(CMail *pMail,CPlayer *pPlayer)
{
	CGUID guid;
	CGUID::CreateGUID(guid);
	pMail->SetExID(guid);

	if (SendMailToWS(pMail))
	{
		OBJ_RELEASE(CMail, pMail);
		return true;
	}
	OBJ_RELEASE(CMail, pMail);
	return false;

}

//���Ϳ۳�����
bool CMailManager::CheckPostage(CMail *pMail, CPlayer *pPlayer)
{
	if (pMail && pPlayer)
	{
		BYTE bType = (BYTE)pMail->GetType();
		long lPostage =  GlobalSetup::GetSetup()->lPostage[bType] * (long)(pMail->GetReceiverList().size());
		long lCurrentMoney = pPlayer->GetSilver();

		if (lCurrentMoney<lPostage)
		{
			//���ʲ���			
			return false;
		}		
	}
	return true;
}
//����Ǯ
bool CMailManager::CheckMoney(CMail *pMail, CPlayer *pPlayer)
 {
	if (pMail && pPlayer)
	{
		BYTE bType = (BYTE)pMail->GetType();		
		long lCurrentMoney = pPlayer->GetMoney();

		//�Ǹ����ʼ�
		if (pMail->GetGold()>GlobalSetup::GetSetup()->lMaxMoney || pMail->GetGold()>lCurrentMoney)
		{
			//������Ǯ��������
			return false;
		}			
	}
	return true;	
}


//����ʼ�
eMAILRESULT CMailManager::CheckMailCondition(CMail *pMail, CPlayer *pPlayer)
{
	//�������(����ظ����ʼ����������)
	if(!CheckPostage(pMail,pPlayer) && pMail->GetType()!=SENDGOLD_MAIL)
	{
		return MAIL_SENDING_FAILED_INVALID_POSTAGE;

	}
	//����Ǯ
	if (pMail->GetType()!=PAYGOLD_MAIL && !CheckMoney(pMail,pPlayer) )
	{
		return MAIL_SENDING_FAILED_INVALID_GOLD;
	}
	//�����Ʒ
	if(!pMail->CheckGoodsContainer(pPlayer) || !pMail->AddGoodsToContainer(pPlayer))
	{
		return MAIL_SENDING_FAILED_INVALID_GOODS;
	}
	//����ʼ������Ƿ�һ��
	if ((pMail->GetType()==COMMON_MAIL && (pMail->GetGold()>0 || pMail->GetGoodsList().size()>0 )) || pMail->GetType()<SYSTEM_MAIL	&& (pMail->GetGoodsList().size()>0 || pMail->GetGold()>0))
	{
		return MAIL_SENDING_FAILED_INVALID_TYPE;
	}

 	//if (pMail->GetSubject().size()==0 || pMail->GetReceiverList().size()==0)
 	//{
 	//	//�������������
 	//	return MAIL_SENDING_FALLED_INVALID_SUBJECTORRECEIVER;
 	//}
	//
	//if (!CheckInvalidCharacter(pMail->GetSubject()))
	//{
	//	//�ʼ����������зǷ��ַ�		
	//	return MAIL_SENDING_FAILED_INVALID_TEXT;
	//}

	//if (!CheckInvalidCharacter(pMail->GetExText()))
	//{
	//	//�ʼ��������������зǷ��ַ�	
	//	return MAIL_SENDING_FAILED_INVALID_EXTEXT;
	//}
	//if (!CheckManyReceiver(pMail))
	//{
	//	//���Ⱥ���ʼ����Ƿ�������Ǯ
	//	return MAIL_SENDING_FAILED_INVALID_MANYRECEIVER;
	//}
	//�����˲����Ƿ�����
	//if (!CheckReceiver(pMail,pPlayer))
	//{
	//	return MAIL_SENDING_FAILED_INVALID_SELF;
	//}

	return MAIL_SENDING_SUCCESS;
}
//�ʼ�����ʧ��
void CMailManager::SendMailFailedToClient(CGUID guid,long lValue)
{
	CMessage msg(MSG_S2C_MAIL_SEND_FAILED);
	msg.Add(lValue);
	msg.SendToPlayer(guid);
}



//���Ⱥ�����ʼ���������
bool	CMailManager::CheckMailCountryToEverybody(CMail *pMail,CPlayer *pPlayer)
{
	long lCountry = 0;
	list<tagMailCondition*> lMailCondition = pMail->GetMailCondition();
	list<tagMailCondition*>::iterator it=lMailCondition.begin();
	for (;it!=lMailCondition.end();++it)
	{
		if ((*it)->strName=="Country")
		{
			++lCountry;
			if ((*it)->lValue==pPlayer->GetCountry())	
			{
				return true;
			}
		}
	}

	if (lCountry>0)
	{
		return false;
	}
	return true;
}


//���Ⱥ�����ʼ�����
bool	CMailManager::CheckMailToEverybody(CMail *pMail,CPlayer *pPlayer)
{
	list<tagMailCondition*> lMailCondition = pMail->GetMailCondition();
	list<tagMailCondition*>::iterator it=lMailCondition.begin();
	for (;it!=lMailCondition.end();++it)
	{
		if ((*it)->strName=="Level")
		{
			if ((*it)->lValue>pPlayer->GetLevel())			
			{
				return false;
			}
		}

		else if ((*it)->strName=="Sex")
		{
			if ((*it)->lValue!=pPlayer->GetSex())		
			{
				return false;
			}
		}
	}	
	return true;
}

//����ϵͳ�ʼ���������
void	CMailManager::SendSysMailMsgToReceiver(long lSysID)
{
	CMail *pMail = GetSysMail(lSysID);
	long lBeginTime = 0;
	long lEndTime = 0;

	if (pMail)
	{
		pMail->GetSysSendTime(lBeginTime,lEndTime);
		GetGame()->SendSysMailMsgToOnlinePlayer(pMail);	
	}
}
//����ϵͳ�ʼ���Ϣ
void	CMailManager::SendSysMailMsg(CMail *pMail,CPlayer *pPlayer)
{
	if (pPlayer && CheckMailCountryToEverybody(pMail,pPlayer) && CheckMailToEverybody(pMail,pPlayer))
	{
		CMessage msg(MSG_S2C_MAIL_SYSMAILMSG);
		msg.Add((char*)pMail->GetSubject().c_str());
		msg.SendToPlayer(pPlayer->GetExID());
	}

}
//����ϵͳ�ʼ�
void	CMailManager::SendSysMail(CMail *pMail,CPlayer *pPlayer,long lSysID,long lEndTime)	
{
	if (pPlayer && CheckMailCountryToEverybody(pMail,pPlayer) && CheckMailToEverybody(pMail,pPlayer))
	{
		pMail->GetReceiverList().clear();
		pMail->SetReceiverList(pPlayer->GetName());
		pMail->AddGoodsToContainerBySys();
		pMail->SetReject(0);
		SendMailToWS(pMail);

		pPlayer->InsertSysMail(lSysID,lEndTime);
	}		
}
//ͨ���ű�����ϵͳ�ʼ�
void	CMailManager::SendSysMailByScript(CMail *pMail)
{
	if (pMail)
	{
		list<string>::iterator it = pMail->GetReceiverList().begin();
		CPlayer *pPlayer=GetGame()->FindPlayer((*it).c_str());
		pMail->AddGoodsToContainerBySys();//---
		pMail->SetReject(0);
		SendMailToWS(pMail);
		pMail->DeleteMailGoods();
		OBJ_RELEASE(CMail, pMail);

	}	
}
void   CMailManager::SendMailToReceiver(CGUID &guid,long lRet)
{
	CMessage msg(MSG_S2W_MAIL_SEND_RECEIVER);
	msg.Add(guid);
	msg.Add(lRet);
	msg.Send();
}

long	CMailManager::ReMoveGoldAndGoods(CMail *pMail,CPlayer *pPlayer)
{
	if (pPlayer)
	{
		BYTE bType = (BYTE)pMail->GetType();
		long lGold = 0;
		long lSilver = 0;
		char strGoodsInfo[512]="";

		if(!pMail->CheckGoodsContainer(pPlayer))
		{
			SendMailFailedToClient(pMail->GetExID(),MAIL_SENDING_FAILED_INVALID_GOODS);
			return 0;
		}	

		if (pMail->GetType()!=SENDGOLD_MAIL)
		{
			lSilver = pPlayer->GetSilver() - GlobalSetup::GetSetup()->lPostage[bType];
			if (lSilver<0)
				return 0;
		}


		if (pMail->GetType()!=PAYGOLD_MAIL)
		{
			lGold = pPlayer->GetMoney() - pMail->GetGold();
			if (lGold<0)
				return 0;			
		}	

		for (list<listGoods*>::iterator it=pMail->GetGoodsList().begin(); it!=pMail->GetGoodsList().end(); ++it)
		{
			CGoods *pGoods =pPlayer->FindGoodByGuid((*it)->goodsguid);
			if (pGoods)
			{
				char strNameNum[64];
				sprintf_s(strNameNum, 64, "-%s-%d----",pGoods->GetName(),(*it)->lNum);
				strcat(strGoodsInfo,strNameNum);				
			}			
		}

		if(!pMail->DeleteGoodsInContainer(pPlayer))
		{
			SendMailFailedToClient(pMail->GetExID(),MAIL_SENDING_FAILED_INVALID_GOODS);
			return 0;
		}		

		if (pMail->GetType()!=PAYGOLD_MAIL)
		{
			pPlayer->SetMoney(lGold,1);
		}	

		//����ظ����ʼ����۳�����	if (pMail->GetType()!=SENDGOLD_MAIL)
		{
			pPlayer->SetSilver(lSilver);
		} 		

	
		long lGoodsNum = 0;
		for (list<listGoods*>::iterator it=pMail->GetGoodsList().begin(); it!=pMail->GetGoodsList().end(); ++it)
		{
			lGoodsNum += (*it)->lNum;
		}



		//���ŵ���־��¼
		GetLogicLogInterface()->logT601_Send_mail_log(pMail->GetExID(),pMail->GetReceiver().c_str(),pMail->GetWriter().c_str(),pMail->GetType(),pMail->GetGold(),lGoodsNum/*,pMail->GetSGood().size()*/,strGoodsInfo);
		return 1;		
	}
	return 0;
	
}
//���ŵ�WSconst CGUID &guid,
bool CMailManager::SendMailToWS(CMail *pMail)
{
	
	CGUID playerguid = NULL_GUID;
	CPlayer *pPlayer = GetGame()->FindPlayer(pMail->GetWriterGuid());


	//  [6/6/2009 chenxianj]
	//////////////////////////////////////////////////////////////////////////
	
	if (NULL!=pMail && NULL==pPlayer && pMail->GetType()!=SYSTEM_MAIL && pMail->GetType()<=SENDGOLD_MAIL)
	{
		//�ѻ����ʹ����ҷ����ʼ�
		char szWriterGuid[256];
		pMail->GetWriterGuid().tostring(szWriterGuid); 			
		Log4c::Trace(ROOT_MODULE,"�ѻ��������:%s,GUID:%sʹ����ҷ����ʼ�,����:%d",pMail->GetWriter().c_str(),szWriterGuid,pMail->GetType());					
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	if (pPlayer)
	{
		//����ʼ�����	
		long lRet = CheckMailCondition(pMail,pPlayer);
		if (lRet!=0)
		{
			if (pMail->GetType()!=SYSTEM_MAIL && pMail->GetType()<MAX_MAIL)
			{		
				SendMailFailedToClient(pPlayer->GetExID(),lRet);
				return false;
			}			
		}
	}


	if (pMail)
	{	
		CMessage msg(MSG_S2W_MAIL_SEND);		
		msg.Add(pMail->GetWriter().c_str());		//������	
		msg.Add(pMail->GetWriterGuid());
		msg.Add(pMail->GetSubject().c_str());		//����
		msg.Add(pMail->GetText().c_str());			//�ʼ�����
		msg.Add(pMail->GetRead());					//��ȡ��־��
		msg.Add(pMail->GetType());					//�ʼ�����
		msg.Add(pMail->GetGold());					//��Ǯ		
		msg.Add(pMail->GetReject());				//���ű�־��

		if (pPlayer)
		{
			msg.Add(pPlayer->GetExID());
			playerguid = NULL_GUID;
		} 
		else
		{
			msg.Add(NULL_GUID);
		}		

		//������
		long lReceiverNum = (long)pMail->GetReceiverList().size();
		msg.Add(lReceiverNum);
		for (list<string>::iterator it=pMail->GetReceiverList().begin(); it!=pMail->GetReceiverList().end(); ++it)
		{
			msg.Add((*it).c_str());
		}


		//��ͨ��Ʒ�б�
		long lCGoodsNum = (long)pMail->GetGoodsList().size();
		msg.Add(lCGoodsNum);
		list<listGoods*>::iterator it=pMail->GetGoodsList().begin();
		for (;it!=pMail->GetGoodsList().end();++it)
		{
			msg.Add((*it)->goodsguid);
			msg.Add((*it)->lNum);
			msg.Add((*it)->lContainerID);
			msg.Add((*it)->lPos);
		}
		

		long lCGoodNum = (long)pMail->GetMGoodsContainer().size();//--��ȡ��Ʒ����
		msg.Add(lCGoodNum);
		int nPos = 0;
		vector<BYTE> pBA;
		for (vector<CGoods*>::iterator it=pMail->GetMGoodsContainer().begin(); it!=pMail->GetMGoodsContainer().end(); ++it)
		{				
			(*it)->Serialize(&pBA);
			msg.Add((LONG)pBA.size());//  ?
			msg.AddEx(&pBA[0], (long)pBA.size());//  ?			
		}
		msg.Send();
		return true;
	}
	return false;

}


//���͸���������Ϣ���ͻ���                
void	CMailManager::SendPayMailMsg(const CGUID &guid,BYTE bValue,CPlayer *pPlayer)
{	
	if (pPlayer==NULL)
		return;
	CMessage msg(MSG_S2C_MAIL_AGREED_PAY);
	msg.Add(guid);
	msg.Add(bValue);	
	msg.SendToPlayer(pPlayer->GetExID());
}

//����
void   CMailManager::SendMailToClient(CMail *pMail)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(pMail->GetReceiver().c_str());

	if (pPlayer && pMail)
	{
		bool bAdd = pPlayer->AddReceiveMailList(pMail->GetExID());
		bool bSet = SetMapMail(pMail);
		
		if(bAdd && bSet)//--����
		{
			CMessage msg(MSG_S2C_MAIL_SEND);			
			msg.SendToPlayer(pPlayer->GetExID());
			long lGoodsNum = 0;
			for (vector<CGoods*>::iterator it=pMail->GetMGoodsContainer().begin(); it!=pMail->GetMGoodsContainer().end(); ++it)
			{
				lGoodsNum += (*it)->GetAmount();
			}
			GetLogicLogInterface()->logT600_Receive_mail_log(pPlayer,pMail->GetExID(),pMail->GetWriter().c_str(),pMail->GetType(),pMail->GetGold(),lGoodsNum/*,pMail->GetSGood().size()*/,2);
      
		}		
	}
	else
	{
		pMail->DeleteMailGoods();
		OBJ_RELEASE(CMail, pMail);
	}		
}
//�����ʼ���Ϣ
void	CMailManager::DecordMailMsgFromByteArray(BYTE* pByte, long& pos,tagMailParam *pMailParam)
{
	char strTemp[2048];
	if (pMailParam==NULL)
		return;
	_GetBufferFromByteArray(pByte,pos,pMailParam->guid);	
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strSubject = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strWriter = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strText = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strExText = strTemp;

	pMailParam->lGold = _GetLongFromByteArray(pByte,pos);
	pMailParam->tRemainTime = time_t(_GetLongFromByteArray(pByte,pos));		
	pMailParam->bRead = _GetByteFromByteArray(pByte,pos);
	pMailParam->lType = _GetLongFromByteArray(pByte,pos);
	pMailParam->bReject = _GetByteFromByteArray(pByte,pos);
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strReceiver = strTemp;
	_GetBufferFromByteArray(pByte,pos,pMailParam->ReceiverGuid);

	long lCNum = _GetLongFromByteArray(pByte,pos);

	for (int i=0; i<lCNum; ++i)
	{
		listGoods *ptgCGoods = OBJ_CREATE(listGoods);
		_GetBufferFromByteArray(pByte,pos,ptgCGoods->goodsguid);
		ptgCGoods->lNum = _GetLongFromByteArray(pByte,pos);
		ptgCGoods->lContainerID = _GetLongFromByteArray(pByte,pos);
		ptgCGoods->lPos = _GetLongFromByteArray(pByte,pos);
		pMailParam->Goodslist.push_back(ptgCGoods);
	}
}
//����
void	CMailManager::DecordFromByteArray(BYTE* pByte, long& pos,tagMailParam *pMailParam)
{
	char strTemp[2048];
	if (pMailParam==NULL)
		return;
	_GetBufferFromByteArray(pByte,pos,pMailParam->guid);	
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strSubject = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strWriter = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strText = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strExText = strTemp;

	pMailParam->lGold = _GetLongFromByteArray(pByte,pos);
	pMailParam->tRemainTime = time_t(_GetDwordFromByteArray(pByte,pos));		
	pMailParam->bRead = _GetByteFromByteArray(pByte,pos);
	pMailParam->lType = _GetLongFromByteArray(pByte,pos);
	pMailParam->bReject = _GetByteFromByteArray(pByte,pos);
	_GetStringFromByteArray(pByte,pos,strTemp);
	pMailParam->strReceiver = strTemp;
	_GetBufferFromByteArray(pByte,pos,pMailParam->ReceiverGuid);

	long lCNum = _GetLongFromByteArray(pByte,pos);

	for (int i=0; i<lCNum; ++i)
	{
		listGoods *ptgCGoods = OBJ_CREATE(listGoods);
		_GetBufferFromByteArray(pByte,pos,ptgCGoods->goodsguid);
		ptgCGoods->lNum = _GetLongFromByteArray(pByte,pos);
		ptgCGoods->lContainerID = _GetLongFromByteArray(pByte,pos);
		ptgCGoods->lPos = _GetLongFromByteArray(pByte,pos);
		pMailParam->Goodslist.push_back(ptgCGoods);
	}

	long lGNum = _GetLongFromByteArray(pByte,pos);
	for (int i=0; i<lGNum; ++i)
	{
		CGoods *pGoods = CGoodsFactory::UnserializeGoods(pByte,pos,25);
		if( pGoods )
		{
			CGoodsFactory::UpdateEquipmentAttr(pGoods);
			pMailParam->vMailGoods.push_back(pGoods);
		}
	}

}


//��������
void	CMailManager::PayMoneyMail(CMail *pMail,CPlayer *pPlayer)							
{
	char   strSysMsg[128];	
	if (pPlayer==NULL || pMail->GetGold()==0)
		return;
	_snprintf(strSysMsg, 128, AppFrame::GetText("GS_SYSMAIL_SIXTH"));

	long lPayGold = pMail->GetGold();
	string strReceiver;
	string strSubject = strSysMsg;
	strReceiver = pMail->GetWriter();
	tagMailParam *ptgMailParam = OBJ_CREATE(tagMailParam);

	ptgMailParam->lType = SENDGOLD_MAIL;
	
	ptgMailParam->lGold = lPayGold;
	ptgMailParam->strWriter = pPlayer->GetName();
	ptgMailParam->strReceiverList.push_back(strReceiver);
	ptgMailParam->WriterGuid = pPlayer->GetExID();
	ptgMailParam->strSubject=strSubject;

	CMail *pPayMail = OBJ_CREATE_PVOID(CMail, (void*)ptgMailParam);
	if(AddMail(pPayMail,pPlayer))
	{
		pMail->SetGold(0);
		if (pMail->GetMGoodsContainer().size()>0)
		{
			pMail->SetType(AFFIX_MAIL);
		}
		else
		{
			pMail->SetType(COMMON_MAIL);
		}

		SendPayMailMsg(pMail->GetExID(),BYTE(AFFIX_MAIL),pPlayer);//���͡����ѳɹ������ͻ���

		CMessage msg(MSG_S2W_MAIL_PAY);
		msg.Add(pMail->GetExID());
		msg.Send();
	}	
}

//������ȡ��Ʒ�ŵ�����
void	CMailManager::GetGoodsFromMail(CGUID &guid,CPlayer *pPlayer,long lIndex)	
{
	CMail *pMail = GetMail(guid);
	if (pMail && pPlayer && pMail->GetType()!=PAYGOLD_MAIL &&  pMail->GetReceiverGuid()==pPlayer->GetExID())//strcmp(pMail->GetReceiver().c_str(),pPlayer->GetName())==0
	{
		pMail->SendGetGoodsMsg(pPlayer,lIndex);
	}
}
//����ȡ��Ʒ���ͻ���
void	CMailManager::SendGetGoodsGSToClient(CGUID &MailGuid,CGUID &PlayerGuid,long lIndex)
{
	CMail *pMail = GetMail(MailGuid);
	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
	if (pMail && pPlayer)
	{
		pMail->GetGoodsFromContainer(pPlayer,lIndex);
	}
}
//��WS����ȡ��Ǯ��Ϣ
void	CMailManager::SendGetGoldFromMailToWS(CGUID &guid,CPlayer *pPlayer)
{
	if (pPlayer)
	{
		CMail* pMail = GetMail(guid);
		if (pMail && (pMail->GetType()==AFFIX_MAIL || pMail->GetType()==SYSTEM_MAIL))
		{
			long lGold = pMail->GetGold();


			if(pPlayer->GetMoney()+lGold>GlobalSetup::GetSetup()->dwGoldCoinLimit || lGold<=0)
			{
				const char* strRes=AppFrame::GetText("38");
				if(strRes)
				{
					pPlayer->SendNotifyMessage(strRes);
				}
				return ;
			}	

			//��WS����ȡ��Ǯ��Ϣ
			CMessage OperaGoldmsg(MSG_S2W_MAIL_OPERA_GOLD);
			OperaGoldmsg.Add(guid);
			OperaGoldmsg.Send();

		}
	}
}
//��������ȡ��Ǯ
void CMailManager::GetGoldFromMail(CGUID &guid,CPlayer *pPlayer)
{
	if (pPlayer)
	{
		CMail* pMail = GetMail(guid);
		if (pMail && (pMail->GetType()==AFFIX_MAIL || pMail->GetType()==SYSTEM_MAIL))
		{
			long lGold = pMail->GetGold();


			if(pPlayer->GetMoney()+lGold>GlobalSetup::GetSetup()->dwGoldCoinLimit || lGold<=0)
			{
				const char* strRes=AppFrame::GetText("38");
				if(strRes)
				{
					pPlayer->SendNotifyMessage(strRes);
				}
				return ;
			}			
			
			if (pPlayer->SetMoney(lGold+pPlayer->GetMoney(),2))//���������ȡ���Ľ�Ǯ
			{
				pMail->SetReject(0);
				pMail->SetGold(0);
				if (pMail->GetMGoodsContainer().size()==0/* && pMail->GetSGood().size()==0 */&& pMail->GetType()!=SYSTEM_MAIL && pMail->GetType()<MAX_MAIL)
				{
					pMail->SetType(COMMON_MAIL);
				}				
			}
			else
			{	
				CMessage msg(MSG_S2C_MAIL_GET_GOLD_RESULT);
				msg.Add(guid);
				msg.Add((BYTE)0);
				msg.SendToPlayer(pPlayer->GetExID());
				return;
			}
			
			//ȡ��Ǯ��־
			long lGoodsNum = 0;
			for (vector<CGoods*>::iterator it=pMail->GetMGoodsContainer().begin(); it!=pMail->GetMGoodsContainer().end(); ++it)
			{
				lGoodsNum += (*it)->GetAmount();
			}
			GetLogicLogInterface()->logT603_GetGold_mail_log(pMail->GetExID(),pMail->GetReceiver().c_str(),pMail->GetWriter().c_str(),pMail->GetType(),lGold, lGoodsNum/*,pMail->GetSGood().size()*/);	

			CMessage msg(MSG_S2C_MAIL_GET_GOLD_RESULT);
			msg.Add(guid);
			msg.Add((BYTE)1);
			msg.SendToPlayer(pPlayer->GetExID());	

		}
	}

}

//��ͻ��˷��������ʼ���Ϣ
void	CMailManager::SendAllMailToClient(vector<BYTE> &vectorByte, CPlayer *pPlayer)
{	
	if (pPlayer==NULL)
		return;
	CMessage msg(MSG_S2C_MAIL_RECEIVER);
	msg.Add((long)vectorByte.size());
	if (vectorByte.size())	
		msg.AddEx(&vectorByte[0], (long)vectorByte.size());			
	msg.SendToPlayer(pPlayer->GetExID());
}

//ϵͳɾ���ʼ�
void 	CMailManager::DeleteMail(list<CGUID> &guid,CPlayer* pPlayer)
{
	if (pPlayer==NULL)
		return;
	long lNum = (long)guid.size();
	if (lNum>0 && pPlayer)
	{
		for (list<CGUID>::iterator it=guid.begin();it!=guid.end();++it)
		{
			map<CGUID,CMail*>::iterator mit=m_mapMail.find((*it));
			if (mit!=m_mapMail.end())
			{
				pPlayer->DeleteReceiveMailList(*it);

				long lGoodsNum = 0;
				for (vector<CGoods*>::iterator tit=((*mit).second)->GetMGoodsContainer().begin(); tit!=((*mit).second)->GetMGoodsContainer().end(); ++tit)
				{
					lGoodsNum += (*tit)->GetAmount();
				}
				//ɾ���ʼ���־
				GetLogicLogInterface()->logT605_delete_mail_log((*mit).first, ((*mit).second)->GetReceiver().c_str(),
																			  ((*mit).second)->GetWriter().c_str(),
																			  ((*mit).second)->GetType(),
																			  ((*mit).second)->GetGold(),
																			  lGoodsNum);
				(*mit).second->DeleteMailGoods();
				OBJ_RELEASE(CMail, (*mit).second);
				m_mapMail.erase(mit);
			}
		}
	}
}

//����ɾ���ʼ���WS
void	CMailManager::DeleteMailToWS(list<CGUID> &guid,CPlayer *pPlayer)
{
	if (pPlayer==NULL)
		return;
	long lNum = (long)guid.size();
	CMessage msg(MSG_S2W_MAIL_DELETE);	
	msg.Add(pPlayer->GetExID());
	msg.Add(lNum);
	for (list<CGUID>::iterator it=guid.begin(); it!=guid.end(); ++it)
	{
		msg.Add((*it));
	}
	msg.Send();
}
//����ɾ���ʼ���Client
void	CMailManager::DeleteMailToClient(list<CGUID> &guid,CPlayer *pPlayer)
{
	if (pPlayer==NULL)
		return;
	long lNum = (long)guid.size();
	CMessage msg(MSG_S2C_MAIL_DELETE_OK);	
	msg.Add(lNum);
	for (list<CGUID>::iterator it=guid.begin(); it!=guid.end(); ++it)
	{
		msg.Add((*it));
	}
	msg.SendToPlayer(pPlayer->GetExID());
	DeleteMail(guid,pPlayer);
}


//��ȡ�ʼ�
void	CMailManager::ReadMail(CGUID &guid,CPlayer *pPlayer)
{	
	if (pPlayer==NULL)
		return;
	map<CGUID,CMail*>::iterator it = m_mapMail.find(guid);

	if (it!=m_mapMail.end())
	{		
		if((*it).second->GetReceiverGuid()!=pPlayer->GetExID())
		{
			GetLogicLogInterface()->logT604_read_mail_log(pPlayer,guid,0,0,0/*,0*/,0);
			return;
		}

		if ((*it).second->GetRead()==0)//--�����δ������WS����  MSG_S2W_MAIL_READ ��Ϣ�� WS��Ϊ�Ѷ�
		{
			CMessage msg(MSG_S2W_MAIL_READ);
			msg.Add(guid);
			msg.Send();
		}
		//��ȡ�ʼ��ɹ���־
		long lGoodsNum = 0;
		for (vector<CGoods*>::iterator mit=(*it).second->GetMGoodsContainer().begin(); mit!=(*it).second->GetMGoodsContainer().end(); ++mit)
		{
			lGoodsNum += (*mit)->GetAmount();
		}

		GetLogicLogInterface()->logT604_read_mail_log(pPlayer,guid,(*it).second->GetType(),(*it).second->GetGold(),lGoodsNum/*,(*it).second->GetSGood().size()*/,1);
		(*it).second->SetRead(1);				
	}
	else
	{
		//��ȡ�ʼ�ʧ����־
		GetLogicLogInterface()->logT604_read_mail_log(pPlayer,guid,0,0,0/*,0*/,0);
		SendReadMailRet(guid,pPlayer->GetExID(),0);
		return;
	}
	SendReadMailRet(guid,pPlayer->GetExID(),1);
}
//���Ͷ�ȡ�ʼ����	
void	CMailManager::SendReadMailRet(CGUID &MailGuid,const CGUID &PlayerGuid, BYTE bRet)
{
	CMessage msg(MSG_S2C_MAIL_READ);
	msg.Add(bRet);
	msg.Add(MailGuid);

	msg.SendToPlayer(PlayerGuid);
}
//��ʼ��ϵͳ�ʼ�����
void	CMailManager::InitSysMailParam(tagMailParam *pParam,tagMailParam *pPamramList)
{
	pParam->bRead = pPamramList->bRead;
	pParam->lType = pPamramList->lType;

	pParam->strExText = pPamramList->strExText;
	pParam->strText  = pPamramList->strText;
	pParam->bReject = 0;
	pParam->tRemainTime = pPamramList->tRemainTime;
	pParam->strWriter = pPamramList->strWriter;
	pParam->WriterGuid = NULL_GUID;
	pParam->strSubject = pPamramList->strSubject;
	pParam->strReceiverList = pPamramList->strReceiverList;
	pParam->lGold = pPamramList->lGold;

	for (list<tagMailEvent*>::iterator it=pPamramList->ltgMailEvent.begin(); it!=pPamramList->ltgMailEvent.end(); ++it)
	{
		tagMailEvent *ptgMailEvent = OBJ_CREATE(tagMailEvent);
		ptgMailEvent->strEventName = (*it)->strEventName;
		ptgMailEvent->lBeginTime = (*it)->lBeginTime;
		ptgMailEvent->lEndTime = (*it)->lEndTime;
		pParam->ltgMailEvent.push_back(ptgMailEvent);
	}

	for (list<tagMailCondition*>::iterator it=pPamramList->lMailCondition.begin(); it!=pPamramList->lMailCondition.end(); ++it)
	{
		tagMailCondition *ptgMailCondition = OBJ_CREATE(tagMailCondition);
		ptgMailCondition->strName = (*it)->strName;
		ptgMailCondition->lValue = (*it)->lValue;
		pParam->lMailCondition.push_back(ptgMailCondition);
	}

}
//��ʼ��ϵͳ�ʼ�
void	CMailManager::InitSysMail()
{
	ClearSysMail();
	map<DWORD,tagMailParam*> mapMail = MailList::GetSysMail();
	map<DWORD,tagMailParam*>::iterator it=mapMail.begin();
	for (;it!=mapMail.end();++it)
	{		
		CGUID guid;
		CGUID::CreateGUID(guid);
		tagMailParam *ptgMailParam = OBJ_CREATE(tagMailParam);
		InitSysMailParam(ptgMailParam,(*it).second);
		ptgMailParam->guid =guid;
		ptgMailParam->lType = (*it).first;

		CMail *pMail = OBJ_CREATE_PVOID(CMail, (void*)ptgMailParam);
		AddSysMail((*it).first,pMail);	
	}
}
//����ҽ�����Ϸʱ�����ϵͳ�ʼ����͵���Ϣ��ʾ
void CMailManager::SendSysMailMsgToReciver(CPlayer *pPlayer)
{
	if (pPlayer==NULL)
		return;
	time_t t_CurrentTime;
	t_CurrentTime = time(NULL);
	long lCurrentTime = (long)t_CurrentTime;
	map<long,CMail*>::iterator it=m_mapSysMail.begin();

	for (;it!=m_mapSysMail.end();++it)
	{
		CMail *pSysMail = (*it).second;
		long lBeingTime = 0;
		long lEndTime = 0;
		pSysMail->GetSysSendTime(lBeingTime,lEndTime);
		if (!pPlayer->CheckSysMail((*it).first) && lCurrentTime>lBeingTime && lCurrentTime<=lEndTime)
		{			
			SendSysMailMsg(pSysMail,pPlayer);
		}
	}

}
//����
void CMailManager::SendReceiveMail(CPlayer *pPlayer)
{
	list<CGUID> lReceiveMail = pPlayer->GetReceiveMailList();
	if (lReceiveMail.size()>0 && pPlayer->GetReceiveMail())
	{		
		vector<BYTE> vectorByte;
		vectorByte.clear();


		_AddToByteArray(&vectorByte,(long)lReceiveMail.size());// "��CByteArray�������"
		for (list<CGUID>::iterator it=lReceiveMail.begin();it!=lReceiveMail.end();++it)
		{
			map<CGUID,CMail*>::iterator mit = m_mapMail.find((*it));
			if (mit!=m_mapMail.end())
			{
                CMail *pMail = mit->second;
				pMail->AddToByteArrayForClient(vectorByte,pPlayer);

				long lGoodsNum = 0;
				for (vector<CGoods*>::iterator tit=pMail->GetMGoodsContainer().begin(); tit!=pMail->GetMGoodsContainer().end(); ++tit)
				{
					lGoodsNum += (*tit)->GetAmount();
				}

				GetLogicLogInterface()->logT600_Receive_mail_log(pPlayer,(*mit).second->GetExID(),(*mit).second->GetWriter().c_str(),
                    (*mit).second->GetType(),(*mit).second->GetGold(),lGoodsNum/*,(*mit).second->GetSGood().size()*/,1);
			}
		}
 		SendAllMailToClient(vectorByte,pPlayer);
	} 
	else
	{
		CMessage msg(MSG_S2W_MAIL_RECEIVE);
		msg.Add(pPlayer->GetExID());
		msg.Send();
	}	
}
//����ҵ�һ������ʱ��û���յ���ϵͳ�ʼ�����
void CMailManager::SendSysMailToReciver(CPlayer *pPlayer)
{
	if (pPlayer==NULL)
		return;
	time_t t_CurrentTime;
	t_CurrentTime = time(NULL);
	long lCurrentTime = (long)t_CurrentTime;
	map<long,CMail*>::iterator it=m_mapSysMail.begin();

	for (;it!=m_mapSysMail.end();++it)
	{
		CMail *pSysMail = (*it).second;
		long lBeingTime = 0;
		long lEndTime = 0;
		pSysMail->GetSysSendTime(lBeingTime,lEndTime);
		if (!pPlayer->CheckSysMail((*it).first) && lCurrentTime>lBeingTime && lCurrentTime<=lEndTime)
		{
			SendSysMail(pSysMail,pPlayer,(*it).first,lEndTime);	
			pSysMail->DeleteMailGoods();
		}
	}
	pPlayer->CheckSysMailTime();
}

//���ϵͳ�ʼ�
bool	CMailManager::AddSysMail(long lMailid, CMail *pMail)
{
	if (pMail)
	{
		map<long,CMail*>::iterator it=m_mapSysMail.find(lMailid);
		if (it==m_mapSysMail.end())
		{			
			m_mapSysMail[lMailid]=pMail;	
			return true;
		}
		else
		{
			pMail->DeleteMailGoods();
			OBJ_RELEASE(CMail, pMail);
			return false;
		}
	}	
	return false;
}
//��ȡ�ʼ�
CMail	*CMailManager::GetMail(CGUID &guid)	
{
	map<CGUID,CMail*>::iterator it=m_mapMail.find(guid);
	if (it!=m_mapMail.end())
	{
		return (*it).second;
	}
	return NULL;
}

//ͨ��guid�õ�ϵͳ�ʼ�
CMail*  CMailManager::GetSysMail(long  lMailID)												
{
	map<long,CMail*>::iterator it=m_mapSysMail.find(lMailID);
	if (it!=m_mapSysMail.end())
	{
		return (*it).second;
	}
	return NULL;
}
//���ϵͳ�ʼ�
void	CMailManager::ClearSysMail()															
{
	map<long,CMail*>::iterator it=m_mapSysMail.begin();
	for (;it!=m_mapSysMail.end();)
	{
		OBJ_RELEASE(CMail, (*it).second);
		it = m_mapSysMail.erase(it);
	}
}
//�ͷ��ʼ�
void	CMailManager::ReleaseMail(list<CGUID> &lMailGuid)
{
	list<CGUID>::iterator it = lMailGuid.begin();
	for (; it!=lMailGuid.end(); ++it)
	{
		map<CGUID,CMail*>::iterator mit=m_mapMail.find(*it);
		if (mit!=m_mapMail.end())
		{
			(*mit).second->DeleteMailGoods();
			OBJ_RELEASE(CMail, (*mit).second);
			mit = m_mapMail.erase(mit);
		}
	}
}
//����������Ƿ����Լ�
//bool CMailManager::CheckReceiver(CMail *pMail,CPlayer *pPlayer)
//{
//	if (pPlayer && pMail->GetType()!=SYSTEM_MAIL && pMail->GetType()<MAX_MAIL)
//	{
//		list<string>::iterator it = pMail->GetReceiverList().begin();
//		for (;it!=pMail->GetReceiverList().end();++it)
//		{
//			if (!stricmp((*it).c_str(),pPlayer->GetName()) || !CheckInvalidCharacter((*it)))
//			{
//				return false;
//			}
//		}
//	}
//	return true;
//}

//�����������Ƿ������Ʒ��Ǯ
//bool CMailManager::CheckManyReceiver(CMail *pMail)
//{
//
//	if ((pMail->GetType()!=SYSTEM_MAIL || pMail->GetType()<MAX_MAIL) && pMail->GetReceiverList().size()>1)
//	{
//		if (pMail->GetGold()>0 || pMail->GetSGood().size()>0 || pMail->GetGoodsList().size()>0)
//		{
//			return false;
//		}
//		return true;
//	}
//	return true;
//}
//����Ƿ��зǷ��ַ�
//bool	CMailManager::CheckInvalidCharacter(string strText)							
//{
//	if(!CWordsFilter::GetInstance()->Check(strText,false,true))//
//	{
//		return false;
//	}
//	return true;
//}
//���ɾ��
//bool	CMailManager::CheckDeleteMail(list<CGUID> &lguid,CPlayer *pPlayer)
//{
//	for (list<CGUID>::iterator dit=lguid.begin(); dit!=lguid.end(); ++dit)
//	{
//		map<CGUID,CMail*>::iterator it=m_mapMail.find((*dit));
//		if (it!=m_mapMail.end())
//		{	
//			if((*it).second->GetType()==PAYGOLD_MAIL || (*it).second->GetReceiverGuid()!=pPlayer->GetExID())
//				return false;	
//		}
//	}
//	return true;
//}