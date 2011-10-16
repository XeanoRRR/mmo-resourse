#include "stdafx.h"
#include "Mail.h"

#include "MailDef.h"
#include "../../Game.h"
#include "../../../UI/UIDef.h"
#include "../../../Public/Common/Public.h"
#include "../Other/GlobeSetup.h"
#include "../ClientWordsFilter.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../../../Engine/Utility.h"

namespace mail{
void  DoAgreePaySel(MsgEvent& event,eMsgResult UserId);
void  DeleteMail(MsgEvent& event,eMsgResult UserId);
void  DoAgreeSend(MsgEvent& event,eMsgResult UserId);

CMail::CMail(void)
{
	m_guidCurMailBox = CGUID::GUID_INVALID;
	m_nGoodsIndex = -1;
	m_bIsPayMail = false;
	m_lGoldNum = 0;
	m_nGoodsIndex = -1;
}

CMail::~CMail(void)
{
	/*list<CPlayer::tagGoods>::iterator it = m_listGoods.begin();
	for (int i = 0; it != m_listGoods.end(); ++it,++i)
	{
		(*it).pGoods->UnLock();
		SAFEDELETE(it->pGoods);
	}*/
	ClearData();
}

// CMail *CMail::m_pInstance = NULL;
// CMail *CMail::Instance()
// {
// 	if ( !m_pInstance )
// 	{
// 		m_pInstance = new CMail;
// 	}
// 	return m_pInstance;
// }

// ��Ӹ�����Ʒ(PackageEvent.cpp����)
void CMail::AddGoods( CPlayer::tagGoods* pstGoods )
{
	// ��Ʒ�����Ҳ���������
	if( pstGoods->pGoods && !pstGoods->pGoods->IsLocked())
	{
		// ��һ�������Ʒ
		if(m_listGoods.empty())
		{
			if(m_vecRecver.size() > 1)
			{
				//���и�����Ʒ���ҵ��ż�����Ⱥ����
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_24"));
				return;
			}			
		}
		if(m_listGoods.size() == 4)
		{		
			GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_33"));
			return;
		}
		pstGoods->pGoods->Lock();
		CPlayer::tagGoods stGoods;
		stGoods.pGoods = pstGoods->pGoods;
		stGoods.lPos = pstGoods->lPos;
		stGoods.lType = pstGoods->lType;	
		m_listGoods.push_back(stGoods);
		
		FireUIEvent(GETGOODSCONTAINER,EVENT_ADDGOODS);
	}
}


void CMail::DoSendMail()
{
	eMAILTYPE eMailType = GetEMailType();	
	if(!CheckSendCondition( eMailType ))
	{	
		FireUIEvent(ADDGOODSCONTAINER,EVENT_UPDATA);
		ClearData();
		return;
	}

	if(m_vecRecver.size() > 1)
	{	
		char stText[256]="";
		DWORD dwPostage = GetPostage(eMailType);	
		sprintf(stText, AppFrame::GetText("Mail_29"),dwPostage);
		//��ȷ��Ҫ����%d����Ⱥ�����ʼ���
		GetInst(MsgEventManager).PushEvent(Msg_YesNo,stText,DoAgreeSend);
		return;
	}

	SendMail();
	FireUIEvent(ADDGOODSCONTAINER,EVENT_UPDATA);
	ClearData();
	
	return;
}

void  DoAgreeSend(MsgEvent& event,eMsgResult UserId)
{
	if (UserId == S_Ok)
	{
		GetInst(CMail).SendMail();
	}
	FireUIEvent(ADDGOODSCONTAINER,EVENT_UPDATA);
	GetInst(CMail).ClearData();
}


void CMail::SendMail()
{
	eMAILTYPE eMailType = GetEMailType();
	long lGoodsNum = (long)m_listGoods.size();
	CMessage msg(MSG_C2S_MAIL_SEND);
	msg.Add(m_guidCurMailBox);
	size_t size = m_vecRecver.size();
	msg.Add((long)size);
	// ����������
	for (size_t i = 0; i < size;i++)
	{
		msg.Add(m_vecRecver[i].c_str());
	}

	msg.Add(m_strTitle.c_str());
	msg.Add(m_strContent.c_str());
	msg.Add((long)m_lGoldNum);
	msg.Add((byte)eMailType);
	msg.Add(lGoodsNum);
	if(!m_listGoods.empty())
	{
		list<CPlayer::tagGoods>::iterator it = m_listGoods.begin();	
		for (; it != m_listGoods.end(); ++it)
		{
			msg.Add((*it).pGoods->GetExID());
			msg.Add((long)(*it).pGoods->GetNum());
			msg.Add((DWORD)(*it).lType);
			msg.Add((DWORD)(*it).lPos);
		}		
	}
	msg.Send();
	GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_37"));
}
void CMail::ClearData()
{
	m_vecRecver.clear();
	m_strTitle.clear();
	m_strContent.clear();
	
	for (list<CPlayer::tagGoods>::iterator it = m_listGoods.begin();it != m_listGoods.end();it++)
	{
		it->pGoods->UnLock();
	}
	m_listGoods.clear();
	m_lGoldNum = 0;
	m_nGoodsIndex = -1;
	m_bIsPayMail = false;
}
eMAILTYPE CMail::GetEMailType() const
{
	if ( m_bIsPayMail )
	{
		return PAYGOLD_MAIL;
	}
	else if(!m_listGoods.empty() || m_lGoldNum)
	{
		return AFFIX_MAIL;
	}
	else
	{
		return COMMON_MAIL;
	}
}
void CMail::DoRecvMail()
{
	CMessage msg(MSG_C2S_MAIL_RECEIVER);
	msg.Send();
}


// �ʼ����ݽ���
void CMail::DecordDataFromByteArray(stMailData &stData,BYTE* pByte, long& pos)
{
	char str[102400]="";
	_GetBufferFromByteArray(pByte,pos,stData.gMailID);
	stData.dwTime = _GetDwordFromByteArray(pByte, pos);
	stData.dwGoldNum = _GetDwordFromByteArray(pByte, pos);
	stData.strMailName = _GetStringFromByteArray(pByte, pos, str);
	stData.strMailDesc = _GetStringFromByteArray(pByte, pos, str);
	stData.strMailDescEx = _GetStringFromByteArray(pByte, pos, str);
	stData.strSenderName = _GetStringFromByteArray(pByte, pos, str);
	stData.bIsReaded = _GetByteFromByteArray(pByte, pos)>0?true:false;
	stData.eMailType = (eMAILTYPE)_GetByteFromByteArray(pByte, pos);
	stData.bIsCanReBack = _GetByteFromByteArray(pByte, pos)>0?true:false;
	// ��ͨ����
// 	else
// 	{
		long lGoodssize = _GetLongFromByteArray(pByte, pos);
		for (int i=0; i<lGoodssize; ++i)
		{
			CGoods *pGoods=new CGoods;
			if(pGoods)
			{
				if( pGoods->DecordFromByteArray(pByte,pos) )
				{
					stData.GoodsList.push_back(pGoods);
				}
				else
				{
					SAFE_DELETE(pGoods);
				}
			}
		}
//	}
}
// ���һ���ʼ���Ϣ
void CMail::AddMail(stMailData& tagData)
{
	vector<stMailData>::iterator it = m_vecMailData.begin();
	for (; it!=m_vecMailData.end(); ++it)
	{
		if(it->gMailID == tagData.gMailID)
			return;
	}
	FormatMailInfo(tagData);
	stMailData stData;
	stData.gMailID			= tagData.gMailID;
	stData.eMailType		= tagData.eMailType;
	stData.bIsReaded		= tagData.bIsReaded;
	stData.bIsCanReBack		= tagData.bIsCanReBack;
	stData.dwGoldNum		= tagData.dwGoldNum;
	stData.dwTime			= tagData.dwTime;
	
	stData.GoodsList		= tagData.GoodsList;
	
	stData.strMailDesc		= tagData.strMailDesc;
	stData.strMailDescEx	= tagData.strMailDescEx;
	stData.strMailName		= tagData.strMailName;
	stData.strSenderName	= tagData.strSenderName;
	stData.colorTitle		= tagData.colorTitle;
	stData.colorSender		= tagData.colorSender;
	stData.colorGoodsEx		= tagData.colorGoodsEx;
	stData.dwTimeStart		= timeGetTime()/1000;
	// δ�Ķ��ķ�����ǰ��
	if(!stData.bIsReaded)
		m_vecMailData.insert(m_vecMailData.begin(),stData);
	else
		m_vecMailData.push_back(stData);

	
	// ��������Ķ��ʼ��������Ķ�������ʼ���Ϣ��ʾ
	//CReadMailPageEx * pReadMailPage = GetGame()->GetCGuiEx()->GetReadMailPageEx();
	//if(pReadMailPage && pReadMailPage->IsOpen())
	//	pReadMailPage->UpdataMailData(m_guidCurReadMailID);
}


// ȷ���ʼ���Ϣ����ʾ����������ɫ�ͱ���ĸ���������Ϣ��
void CMail::FormatMailInfo(stMailData &tagData)
{
	if(tagData.gMailID == CGUID::GUID_INVALID)
	{
		return;
	}
	// Ĭ�ϰ�ɫ
	tagData.colorSender = tagData.colorTitle = 0xffffffff;
	tagData.colorGoodsEx = 0x00ffffff;

	if(tagData.eMailType == SYSTEM_MAIL)
	{
		tagData.colorSender = 0xfffff600;
	}
	// �ʼ����Ķ���������Ϊ��ɫ
	if(tagData.bIsReaded)
	{
		tagData.colorTitle = 0xff949494;
		// ��ϵͳ�ʼ�������ʾΪ��ɫ����
		if(tagData.eMailType != SYSTEM_MAIL)
		{
			tagData.colorGoodsEx = 0x64949494;
		}
	}
}

void CMail::DoReadMail(const unsigned int index)
{
	CMessage msg(MSG_C2S_MAIL_READ);
	msg.Add(m_guidCurMailBox);

	if ( m_vecMailData.size() >= index + 1 )
	{
		msg.Add(m_vecMailData[index].gMailID);
		msg.Send();
	}	
}
// ���ָ��ID���ʼ�
stMailData* CMail::GetMailByGUID(const CGUID& gMailID)
{
	vector<stMailData>::iterator it = m_vecMailData.begin();
	for (; it!=m_vecMailData.end(); ++it)
	{
		if(it->gMailID == gMailID)
		{
			return &(*it);
		}
	}
	return NULL;
}

void CMail::DoDelMail( )
{
	WORD wSize = (WORD)m_vecDelMail.size();


	if ( wSize < 0 )
	{
		//to do: ��ѡ����Ҫɾ�����ʼ� 
	}
	for ( int i = 0; i < wSize; i++ )
	{
		if (m_vecDelMail[i].eMailType == PAYGOLD_MAIL )
		{
			//�������ż��޷�ɾ����
			GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_15")); 
			m_vecDelMail.clear();
			return;
		}
	}

	for ( int i = 0; i < wSize; i++ )
	{
		if (m_vecDelMail[i].GoodsList.size() > 0)
		{
			//��ѡ��Ҫɾ�����ż����к�����Ʒ��������ȷ���Ƿ�ɾ����
			GetInst(MsgEventManager).PushEvent(Msg_YesNo,AppFrame::GetText("Mail_16"),DeleteMail); 
			return;
		}
	}
	
	DelMail();
	
}

void DeleteMail(MsgEvent& event,eMsgResult UserId)
{
	if (UserId == S_Ok)
	{
		GetInst(CMail).DelMail();
	}
	GetInst(CMail).GetDelMail().clear();
}

void CMail::DelMail()
{
	CMessage msg(MSG_C2S_MAIL_DELETE);
	msg.Add(m_guidCurMailBox);
	WORD wSize = (WORD)m_vecDelMail.size();
	msg.Add(wSize);
	vector<stMailData>::iterator it = m_vecDelMail.begin();
	for (; it!=m_vecDelMail.end(); ++it)
	{
		msg.Add(it->gMailID);
	}
	msg.Send();
	m_vecDelMail.clear();
}
// ɾ���ʼ�����Ʒ
void CMail::ReleaseMailGoods(stMailData &stData)
{
	// �����ǰ�ĸ���
	list<CGoods *>::iterator it = stData.GoodsList.begin();
	for (; it != stData.GoodsList.end(); ++it)
	{
		SAFE_DELETE((*it));
	}
	stData.GoodsList.clear();
}



// ɾ��ָ��ID���ʼ�
void CMail::DelMailByGuid(CGUID gMailID)
{
	vector<stMailData>::iterator it = m_vecMailData.begin();
	for (; it!=m_vecMailData.end(); ++it)
	{
		if(it->gMailID == gMailID)
		{
			// ɾ���ʼ��ǵ�ǰ�����Ķ����ʼ����ر��Ķ�����
			//if(m_guidCurReadMailID == gMailID)
			//	GetGame()->GetCGuiEx()->GetReadMailPageEx()->Close();
			ReleaseMailGoods((*it));
			m_vecMailData.erase(it);
			return;
		}
	}
}

void CMail::AddMailToDel(const unsigned int id)
{
	if ( m_vecMailData.size() >= id+1 )
	{
		m_vecDelMail.push_back( m_vecMailData[id] );
	}
}

bool CMail::GetCurReadMailData(stMailData& mailData)
{
	for ( unsigned int i = 0; i < m_vecMailData.size(); i++ )
	{
		if ( m_vecMailData[i].gMailID == m_guidCurReadMail )
		{
			mailData =  m_vecMailData[i];
			return true;
		}
	}
	return false;
}

// ���ָ�������ĸ�����Ʒ��������ң�
void CMail::DoGetGoods(uint nMailIndex,uint nGoodsIndex)
{
	if ( m_vecMailData.size() <= nMailIndex )
	{
		//to do:add log
		return;
	}
	if(nGoodsIndex < 0 || nGoodsIndex >= m_vecMailData[nMailIndex].GoodsList.size())
	{
		//to do:add log
		return;
	}
	// ����������
	if(m_vecMailData[nMailIndex].eMailType == PAYGOLD_MAIL && m_vecMailData[nMailIndex].dwGoldNum > 0)//�ǲ��ǵ���ͬ�⸶�ѾͰ� dwGoldNum ��0 �ˣ�
	{
		// ���ܻ�ȡ��Ʒ�������������Ű�ť��
		 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_5"));
		 return;
	}
	// ������ͨ������ϵͳ�ʼ�
	else if(m_vecMailData[nMailIndex].eMailType == AFFIX_MAIL || m_vecMailData[nMailIndex].eMailType == SYSTEM_MAIL)
	{
		CMessage msg(MSG_C2S_MAIL_GET_GOODS);
		msg.Add(m_guidCurMailBox);
		msg.Add(m_vecMailData[nMailIndex].gMailID);
		msg.Add((long)nGoodsIndex);
		msg.Send();
	}
}


 void CMail::SetRecver( string& strName )
 {
	unsigned int pos = 0;
	while (!strName.empty())
	{
		pos = (unsigned int)strName.find(";");
		if(string::npos != pos)
		{
			string strName = strName.substr(0,pos);
			if(!strName.empty())
			{
				m_vecRecver.push_back(strName);
			}
			strName = strName.substr(pos+1);
		}
		else
		{
			if(!strName.empty())
			{
				m_vecRecver.push_back(strName);
				break;
			}
		}
	}
 }

 // ����ָ�����ʼ���Ϣ(������ʼ����ڱ��鿴����Ҫ�����Ķ��������ʾ)
 void CMail::UpdataMail(stMailData& tagData)
 {
	 vector<stMailData>::iterator it = m_vecMailData.begin();
	 for (int i=0; it!=m_vecMailData.end(); ++it,++i)
	 {
		 if(it->gMailID == tagData.gMailID)
		 {
			 FormatMailInfo(tagData);
			 it->eMailType		= tagData.eMailType;
			 it->bIsReaded		= tagData.bIsReaded;
			 it->dwGoldNum		= tagData.dwGoldNum;
			 it->dwTime			= tagData.dwTime;
			 it->GoodsList		= tagData.GoodsList;
			 it->strMailDesc	= tagData.strMailDesc;
			 it->strMailDescEx	= tagData.strMailDescEx;
			 it->strMailName	= tagData.strMailName;
			 it->strSenderName	= tagData.strSenderName;
			 it->colorTitle		= tagData.colorTitle;
			 it->colorSender	= tagData.colorSender;
			 it->colorGoodsEx	= tagData.colorGoodsEx;
			 // �����ռ����б�Ľ�����ʾ
			// UpdataInBoxMailItem((*it),i);
			 return;
		 }
	 }
 }
 // ����ʼ����������Ƿ��ɣ���ɷ��ţ����򲻷���
 bool CMail::CheckSendCondition(eMAILTYPE eMailType)
 {
	 CPlayer *pPlayer = GetGame()->GetMainPlayer();
	 if( pPlayer )
	 {
		 DWORD dwPostage = GetPostage(eMailType);	
		 //if(pPlayer->GetSilverMoney() < dwPostage)
		 //{	
			// //���ʲ���������ʧ��
			// GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_51"));
			// return false;
		 //}
		 // ����ż�����ַ��Ƿ���ڷǷ��ַ� 
		 if(m_strTitle.empty() || m_vecRecver.size() == 0 )
		 {
			//û�������˻����⣬����ʧ�ܣ�			 
			GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_61"));
			return false;
		 }
		 if( ! CClientWordsFilter::GetInstance()->Check(m_strTitle, 1, 1) )
		 {
			 //��������Ƿ��ַ�������ʧ�ܣ�
			 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_52"));
			 return false;
		 }
		 if( ! CClientWordsFilter::GetInstance()->Check(m_strContent, 1, 1) )
		 {
			 //���ݰ����Ƿ��ַ�������ʧ�ܣ� 
			 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_53"));
			 return false;
		 }
		 vector<string>::iterator it = m_vecRecver.begin();
		 for( ;it != m_vecRecver.end();it++ )
		 {
			 if ((*it) == pPlayer->GetName())
			 {
				 //�Բ��𣬲��ܸ��Լ������ʼ���			 
				 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_27"));
				 return false;
			 }		
		 }

		 if(eMailType == PAYGOLD_MAIL)
		 {
			 if ( m_lGoldNum <= 0 )
			 {
				 //����ȷ���븶�����ŵĽ�	
				 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_32"));
				 return false;
			 }
			 return true;
		 }
		 // ��ͨ����
		 if(eMailType == AFFIX_MAIL)
		 {
			 if(pPlayer->GetMoney() < m_lGoldNum)
			 {
				 // ���Ľ�Ҳ��㣬����ʧ�ܣ�				 
				 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_13"));
				 return false;
			 }		
			 return true;
		 }
		 // Ⱥ�����ʼ�
		 size_t size = m_vecRecver.size();
		 //Ⱥ�����ʼ�����������Ʒ���Ǯ
		 if (size >= 2)
		 {
			 if (m_lGoldNum > 0 || m_listGoods.size() > 0 ) 
			 {
				 //Ⱥ�����ʼ�����������Ʒ���Ǯ��
				  GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_63"));
				  return false;
			 }
		 }
		 if (size > 100)
		 {
			 //�ռ��������������ƣ�����ʧ��		 
			 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_62"));
			 return false;
		 }
	 }
	 return true;
 }


const int CMail::GetPostage(const eMAILTYPE eMailType)
 {
	 unsigned int dwPostage = 0;

	 CGlobeSetup::tagSetup *pSetup = CGlobeSetup::GetSetup();
	 switch(eMailType)
	 {
	 case COMMON_MAIL:	//��ͨ�ʼ�
		 dwPostage = pSetup->dwNormalMailPrice;
		 break;
	 case AFFIX_MAIL:	//��ͨ�����ʼ�
		 dwPostage = pSetup->dwNormalGoodsMailPrice;
		 break;
	 case PAYGOLD_MAIL:	//�����ʼ�
		 dwPostage = pSetup->dwPayGoldMailPrice;
		 break;
	 }
	 // ������Ҹ�������������
	 dwPostage *= (unsigned int)m_vecRecver.size();

	 return dwPostage;
 }
// ��ý�Ҹ���
void CMail::DoGetGold()
{

	//CPlayer *pPlayer = GetGame()->GetMainPlayer();//del
	//char str2[256]="";//del
	//sprintf_s(str2,"��ȡ���ǰ�Ľ�ǮΪ%d",pPlayer->GetMoney());//del
	//GetInst(MsgEventManager).PushEvent(Msg_Ok,str2);//del

	stMailData MailData;
	GetCurReadMailData(MailData);
	if(MailData.dwGoldNum > 0)
	{
	// �Ǹ���������
		if(MailData.eMailType != PAYGOLD_MAIL)
		{
			CMessage msg(MSG_C2S_MAIL_GET_GOLDS);
			msg.Add(m_guidCurMailBox);
			msg.Add(m_guidCurReadMail);
			msg.Send();
		}
	}
}

void CMail::DoAgreePay(const unsigned int index)
{
	if ( m_vecMailData.size() < index )
	{
		return;
	}
	CPlayer *pPlayer = GetGame()->GetMainPlayer();//del
	if ( m_vecMailData[index].eMailType == PAYGOLD_MAIL )
	{
		if(pPlayer->GetMoney() >=  m_vecMailData[index].dwGoldNum)
		{

			//char str2[256]="";//del
			//sprintf_s(str2,"����ǰ�Ľ�ǮΪ%d",pPlayer->GetMoney());//del
			//GetInst(MsgEventManager).PushEvent(Msg_Ok,str2);//del

	
			char str[256]="";
			sprintf_s(str,AppFrame::GetText("Mail_18"),m_vecMailData[index].dwGoldNum);
			//"�Ƿ�ͬ����%d��Ҽ۸���ȡ�ż��еĸ�����"
			GetInst(MsgEventManager).PushEvent(Msg_Agree,str,DoAgreePaySel);//��ʹ�á�&CMail::DoAgreePaySel������ָ���Ա��ָ��
		}
		else
		{
			//"���Ľ�Ҳ��㣬���ܸ��"
			GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_19"));
		}
	}
}

void  DoAgreePaySel(MsgEvent& event,eMsgResult UserId)
{
	if (UserId == S_Ok)
	{
		CMessage msg(MSG_C2S_MAIL_AGREED_PAY);
		msg.Add(GetInst(CMail).GetMailBoxGUID());
		msg.Add(GetInst(CMail).GetCurReadMailGUID());
		msg.Send();
	}	
}
// ��������ʼ�����
void CMail::ClearAllMailData()
{
	m_vecDelMail.clear();
	for (unsigned int i = 0; i < m_vecMailData.size(); i++)
	{
		ReleaseMailGoods(m_vecMailData[i]);
	}
	m_vecMailData.clear();
}

// ɾ��ָ�������ĸ�����Ʒ
void CMail::DoRemoveGoods(const unsigned int index)
{
	list<CPlayer::tagGoods>::iterator it = m_listGoods.begin();
	for (int i = 0; it != m_listGoods.end(); ++it,++i)
	{
		if(i == index)
		{
			(*it).pGoods->UnLock();
			//SAFEDELETE(it->pGoods);
			m_listGoods.erase(it);
			return;
		}
	}
}
// ���Ų���
void CMail::DoRejectMail(const unsigned int index)
{
	if (m_vecMailData.size() < index+1)
	{
		return;
	}
	if (m_vecMailData[index].eMailType == SYSTEM_MAIL)
	{
		//ϵͳ�ʼ��������ţ�
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_38"));
		return;
	}
	CMessage msg(MSG_C2S_MAIL_REJECT);
	msg.Add(m_guidCurMailBox);
	msg.Add(m_vecMailData[index].gMailID);
	msg.Send();
}



}