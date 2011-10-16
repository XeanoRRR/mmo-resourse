/**************************************************
*		�ļ���  ��Mail.h
*		��Ҫ�������ʼ���

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�10/10/18
*		�޶�˵����
****************************************************/
#pragma once

#include "../BaseObject.h"
#include "../../../../Public/Common/MailDef.h"

class CMailManager;

class CMail : public CBaseObject
{
public:
	struct tagMail 
	{
		CGUID MailGuid;
	};

public:
	CMail(tagMailParam *pMailParam);
	CMail(void *pParam);
	virtual ~CMail(void);

	 CGUID					  GetExID()			{ return m_pBaseParam->guid;}			 //GUID
	long					  GetType()			{ return m_pBaseParam->lType;}			 //�ʼ�����
	string					  GetWriter()		{ return m_pBaseParam->strWriter;}	     //������	
	string					  GetSubject()		{ return m_pBaseParam->strSubject;}		 //����
	string					  GetText()			{ return m_pBaseParam->strText;}		 //�ʼ�����
	string				      GetExText()		{ return m_pBaseParam->strExText;}	     //�ʼ���������
	time_t					  GetRemainTime()	{ return m_pBaseParam->tRemainTime;}	 //����ʱ��
	long					  GetCost()			{ return m_pBaseParam->lCost;}   		 //����
	long					  GetGold()			{ return m_pBaseParam->lGold;}			 //���
	BYTE					  GetRead()			{ return m_pBaseParam->bRead;}			 //�ż����ı�־��
	BYTE					  GetReject()		{ return m_pBaseParam->bReject;}		 //���ű�־��	
	list<tagMailEvent*>		  GetMailEvent()	{return m_pBaseParam->ltgMailEvent;}	 //ϵͳ�ʼ��¼�
	CGUID&					  GetWriterGuid()  { return m_pBaseParam->WriterGuid;}		 //д���˵�GUID
	list<string>&			  GetReceiverList()		{return m_pBaseParam->strReceiverList;} //�������б�
	list<listGoods*>&		  GetGoodsList()				{return m_pBaseParam->Goodslist;}			//��ͨ��Ʒ�б�
	vector<CGoods*>&		  GetMGoodsContainer() {return m_pBaseParam->vMailGoods;}			//��ȡ��Ʒ����
	list<tagMailCondition*>   GetMailCondition()	{return m_pBaseParam->lMailCondition;}		//��������
	string					  GetReceiver()			{return m_pBaseParam->strReceiver;}			//������
	CGUID					  GetReceiverGuid()		{return m_pBaseParam->ReceiverGuid;}		//������GUID										
	void				SetType(long lType)							{ m_pBaseParam->lType = lType;}
	void				SetReject(BYTE bReject)						{ m_pBaseParam->bReject = bReject;}
	void				SetGold(long lGold)					 		{m_pBaseParam->lGold = lGold;}
	void				SetRead(BYTE bRead)					 		{m_pBaseParam->bRead = bRead;}
	void				SetExID(const CGUID& guid)					{m_pBaseParam->guid = guid;}
	void				SetReceiverList(const char* szRecer);			
	void				SetWriter(const char* szWriter)	;			
	void				SetSubject(const char* szSubject);			
	void				SetText(const char* szText);					
	void				SetExText(const char* szExText);				
	void				SetReceiver(string strName)					{m_pBaseParam->strReceiver = strName;}
	void				ClearSGoods();
	bool				AddGoodsToContainer(CPlayer *pPlayer);											//�����Ʒ���ʼ���Ʒ����
	bool				DeleteGoodsInContainer(CPlayer *pPlayer);										//ɾ�������е���Ʒ
	bool				CheckGoodsContainer(CPlayer *pPlayer);											//�����Ʒ����
	void				AddGoodsToContainerBySys();														//ϵͳ���ʼ�����Ʒ�����������Ʒ
	void				SendGetGoodsMsg(CPlayer *pPlayer, long lIndex);									//���ʹ��ʼ���Ʒ������ȡ��Ʒ��������Ϣ
	void				SendGetGoodsToClient(CGUID &MailGuid,const CGUID &PlayerGuid,long lIndex,BYTE bResult);//��ͻ��˷���ȡ��Ʒ��Ϣ
	void				GetGoodsFromContainer(CPlayer *pPlayer, long lIndex);							//���ʼ���Ʒ������ȡ��Ʒ������
	void				DeleteMailGoods();																//ɾ���ʼ��е���Ʒ
	void				AddToByteArrayForClient(vector<BYTE>& vRet,CPlayer *pPlayer);					//Ϊ�ͻ��˱���			
	void				AddToByteArrayForWS(vector<BYTE>& vRet,CPlayer* pPlayer);	
	void				CodeToDataBlockForClient(DBWriteSet& setWriteDB,CPlayer *pPlayer);				
	void				CodeToDataBlockForWS(DBWriteSet& setWriteDB,CPlayer *pPlayer);																
	bool				JudgeSysMailEventTime();														//�ж�ϵͳ�ʼ��¼�ʱ������	
	void				GetSysSendTime(long &lBeginTime,long &lEndTime);								//��ȡϵͳȺ����ķ���ʱ���
	
private:
	tagMailParam *m_pBaseParam;	

};
