/**************************************************
*		�ļ���  ��Mail.h
*		��Ҫ�������ʼ������߼�

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�10/10/18
*		�޶�˵����
****************************************************/
#pragma once

#include "../../../Public/Common/MailDef.h"

class CMail
{
public:
	struct stTimerParam
	{
		//��ʱ��ID
		long lTimerID;
		//Ԥ������ʱ��
		long lFuture;
		//���ô���
		long lCount;
		//�ѵ��ô���
		long lCalledCount;
		//��ʼʱ��
		DWORD nStartTime;			
	};

	struct tagMail 
	{
		CGUID PlayerGuid;
		CGUID MailGuid;		
		long  lMailID;
	};
public:
	CMail(void);
	CMail(tagMailParam *pMailParam);
	virtual ~CMail(void);

	long	 				GetType()				{ return m_pBaseParam->lType;}	//�ʼ�����
	string&	  				GetWriter()				{ return m_pBaseParam->strWriter;}		    //������	
	string&	  				GetSubject()			{ return m_pBaseParam->strSubject;}			//����
	string&	  				GetText()				{ return m_pBaseParam->strText;}			//�ʼ�����
	string&    				GetExText()				{ return m_pBaseParam->strExText;}		    //�ʼ���������
	time_t	  				GetRemainTime()			{ return m_pBaseParam->tRemainTime;}		//����ʱ��
	long	  				GetGold()				{ return m_pBaseParam->lGold;}				//���
	BYTE	  				GetRead()				{ return m_pBaseParam->bRead;}				//�ż����ı�־��
	BYTE					GetReject()				{ return m_pBaseParam->bReject;}			//���ű�־��
	BYTE					GetSave()				{ return m_pBaseParam->bSave;}				//�����־��
	CGUID&					GetWriterExID()		    { return m_pBaseParam->WriterGuid;}			//������GUID
	CGUID&					GetReceiverExID()		{ return m_pBaseParam->ReceiverGuid;}		//������GUID
	list<string>&			GetReceiverList()		{ return m_pBaseParam->strReceiverList;} 	//�������б�
	list<listGoods*>&		GetCGood()				{ return m_pBaseParam->Goodslist;}			//��ͨ��Ʒ�б�	
	vector<CGoods*>&		GetMGoodsContainer()	{ return m_pBaseParam->vMailGoods;}			//��ȡ��Ʒ�б�
	list<tagMailCondition*> GetMailCondition()		{ return m_pBaseParam->lMailCondition;}		//�ʼ�����
	string&					GetReceiver()			{ return m_pBaseParam->strReceiver;}		//������	
	tagMailParam*			GetBaseParm()			{ return m_pBaseParam;}

	void					SetType(long lType)							{ m_pBaseParam->lType = lType;}
	void					SetWriter(const char* szWriter)				{ if(szWriter) m_pBaseParam->strWriter = szWriter;		}
	void					SetWriterID(const CGUID &guid)				{ m_pBaseParam->WriterGuid = guid;}
	void					SetSubject(const char* szSubject)			{ if(szSubject) m_pBaseParam->strSubject = szSubject;	}
	void					SetText(const char* szText)					{ if(szText) m_pBaseParam->strText = szText;			}
	void					SetExText(const char* szExText)				{ if(szExText) m_pBaseParam->strExText = szExText;}
	void					SetRemainTime(time_t	lRemainTime)	 	{m_pBaseParam->tRemainTime = lRemainTime;}	
	void					SetGold(long lGold)					 		{m_pBaseParam->lGold = lGold;}
	void					SetRead(BYTE bRead)					 		{m_pBaseParam->bRead = bRead;}
	void					SetReject(BYTE bReject)						{m_pBaseParam->bReject = bReject;}
	void					SetSave(BYTE bSave)							{m_pBaseParam->bSave = bSave;}
	void					SetReceiverList(const char* szRecer)		{ if(szRecer) m_pBaseParam->strReceiverList.push_back(string(szRecer));}
	void					SetReceiver(string strReceiver)				{m_pBaseParam->strReceiver = strReceiver;}
	void					SetReceiverExID(const CGUID &guid)			{m_pBaseParam->ReceiverGuid = guid;}
	void					SetMGoodsContainer(vector<CGoods*> &vGoods) {m_pBaseParam->vMailGoods = vGoods;}	
	void					SetWriteTime(time_t lTime)					{m_pBaseParam->tWriteTime = lTime;}			
	void					ClearMailGoodsContainer();
	bool					ClearMailGoodsByIndex(unsigned long lIndex);
	void					AddToByteArrayToGS(vector<BYTE>& vRet);
	void					AddMailMsgByteArrayToGS(vector<BYTE>& vRet);
	const CGUID&			GetExID(void)	{ return m_pBaseParam->guid; }
	void					SetExID(const CGUID& guid) { m_pBaseParam->guid = guid; }

private:
	tagMailParam *m_pBaseParam;			
};
