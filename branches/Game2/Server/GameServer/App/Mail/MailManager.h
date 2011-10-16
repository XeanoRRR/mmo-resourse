/**************************************************
*		�ļ���  ��MailManager.h
*		��Ҫ�������ʼ�������

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�10/10/18
*		�޶�˵����
****************************************************/
#include "Mail.h"
#include "../BaseObject.h"
#pragma once

class CMailManager
{
public:
	CMailManager();
	~CMailManager();

	bool		AddMail(CMail *pMail,CPlayer *pPlayer=NULL);							//����
	bool		SendMailToWS(CMail *pMail);												//��ws����
	void	    SendMailToClient(CMail *pMail);											//��ͻ��˷��� const CGUID &guid,
	void		SendAllMailToClient(vector<BYTE> &vectorByte,CPlayer *pPlayer);			//��ͻ��˷��������ʼ���Ϣ
	void		DeleteMail(list<CGUID> &guid,CPlayer *pPlayer);							//����ɾ���ʼ���ws
	void		DeleteMailToClient(list<CGUID> &guid,CPlayer *pPlayer);					//����ɾ���ʼ���Client
	void		DeleteMailToWS(list<CGUID> &guid,CPlayer *pPlayer);						//����ɾ���ʼ���WS
	void		ReadMail(CGUID &guid,CPlayer *pPlayer);									//��ȡ�ʼ�
	void		GetGoodsFromMail(CGUID &guid,CPlayer *pPlayer,long lIndex);				//������ȡ��Ʒ�ŵ�����
	void		GetGoldFromMail(CGUID &guid,CPlayer *pPlayer);							//��������ȡ��Ǯ
	void		SendGetGoldFromMailToWS(CGUID &guid,CPlayer *pPlayer);					//��WS����ȡ��Ǯ��Ϣ
	void		PayMoneyMail(CMail *pMail,CPlayer *pPlayer);							//��������	
	bool		CheckMoney(CMail *pMail, CPlayer *pPlayer);								//����Ǯ	
	bool		CheckPostage(CMail *pMail, CPlayer *pPlayer);							//���Ϳ۳�����	
	void		SendPayMailMsg(const CGUID &guid,BYTE bValue,CPlayer *pPlayer);			//���͸���������Ϣ���ͻ���
	long		ReMoveGoldAndGoods(CMail *pMail,CPlayer *pPlayer);						
	void		SendMailToReceiver(CGUID &guid,long lRet);								//���Ϳ۳���Ʒ��Ǯ�Ľ��
	void		SendGetGoodsGSToClient(CGUID &MailGuid,CGUID &PlayerGuid,long lIndex);	//����ȡ��Ʒ���ͻ���
	void		SendMailFailedToClient(CGUID guid,long lValue);							//�ʼ�����ʧ��	
	void		DecordFromByteArray(BYTE* pByte, long& pos,tagMailParam *pMailParam);	//����
	void		DecordMailMsgFromByteArray(BYTE* pByte, long& pos,tagMailParam *pMailParam);//�����ʼ���Ϣ		
	void		SendReceiveMail(CPlayer *pPlayer);										//����	
	bool		SetMapMail(CMail *pMail);												//����
	void		SendReadMailRet(CGUID &MailGuid,const CGUID &PlayerGuid, BYTE bRet);	//���Ͷ�ȡ�ʼ����	
	eMAILRESULT	CheckMailCondition(CMail *pMail, CPlayer *pPlayer);						//����ʼ���������	
	CMail*		GetMail(CGUID &guid);													//��ȡ�ʼ�

	//ϵͳ�ʼ�
	void		InitSysMail();															//��ʼ��ϵͳ�ʼ�
	void		InitSysMailParam(tagMailParam *pParam,tagMailParam *pPamramList);		//��ʼ��ϵͳ�ʼ�����
	bool		AddSysMail(long lMailid, CMail *pMail);									//���ϵͳ�ʼ�
	void		ClearSysMail();															//���ϵͳ�ʼ�
	void		SendSysMailMsgToReceiver(long lSysID);									//����ϵͳ�ʼ���WS
	void		SendSysMail(CMail *pMail,CPlayer *pPlayer,long lSysID,long lEnd);		//����ϵͳ�ʼ�
	bool		CheckMailToEverybody(CMail *pMail,CPlayer *pPlayer);					//���Ⱥ�����ʼ�����
	bool		CheckMailCountryToEverybody(CMail *pMail,CPlayer *pPlayer);				//���Ⱥ�����ʼ���������
	void		SendSysMailMsg(CMail *pMail,CPlayer *pPlayer);							//����ϵͳ�ʼ���Ϣ
	void		SendSysMailToReciver(CPlayer *pPlayer);									//����ҵ�һ������ʱ��û���յ���ϵͳ�ʼ�����
	void		SendSysMailMsgToReciver(CPlayer *pPlayer);								//����ҽ�����Ϸʱ�����ϵͳ�ʼ����͵���Ϣ��ʾ	
	void		SendSysMailByScript(CMail *pMail);										//ͨ���ű�����ϵͳ�ʼ�
	CMail		*GetSysMail(long  lMailID);												//ͨ��ID�õ�ϵͳ�ʼ�
	void		ReleaseMail(list<CGUID> &lMailGuid);									//�ͷ��ʼ�

	//bool		CheckInvalidCharacter(string strText);									//����Ƿ��зǷ��ַ�
	//bool		CheckManyReceiver(CMail *pMail);										//�����������Ƿ������Ʒ��Ǯ	
	//bool		CheckReceiver(CMail *pMail,CPlayer *pPlayer);							//����������Ƿ����Լ�	
	//void		SendPMail(CGUID &MailGuid,CPlayer *pPlayer,long lRet);					//�������ʼ�
	//void		SendPresentMailToWS(CMail *pMail,long lRet);							//���������ʼ���WS
	//void		BuyItemByMail(CMail *pMail,CPlayer *pPlayer);							//ͨ���ʼ�������ֵ����	
private:

	map<CGUID,CMail*>	m_mapMail;  
	map<long,CMail*>	m_mapSysMail;													//ϵͳ�ʼ�
};