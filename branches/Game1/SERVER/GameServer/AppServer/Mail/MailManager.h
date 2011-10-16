///////////////////////////////////////////////////////////////////////////////
///  MailManager.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\GameServer\AppServer\Mail
///
///  CREATED: 09/12/2008  by ���Ƚ�
///
///  PURPOSE:   �ʼ�������
///
//////////////////////////////////////////////////////////////////////
#include <map>
#include "Mail.h"
#include "../BaseObject.h"
#pragma once

class CMailManager : public BaseMemObj
{
public:
	CMailManager();
	~CMailManager();

	bool		AddMail(CMail *pMail,CPlayer *pPlayer=NULL);							//����
	bool		SendMailToWS(CMail *pMail);												//��ws����
	void	    SendMailToClient(CGUID guid,CMail *pMail);								//��ͻ��˷��� const CGUID &guid,
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
	bool		CheckInvalidCharacter(string strText);									//����Ƿ��зǷ��ַ�
	bool		CheckPostage(CMail *pMail, CPlayer *pPlayer);							//���Ϳ۳�����
	bool		CheckManyReceiver(CMail *pMail);										//�����������Ƿ������Ʒ��Ǯ	
	bool		CheckReceiver(CMail *pMail,CPlayer *pPlayer);							//����������Ƿ����Լ�	
	void		SendPayMailMsg(const CGUID &guid,BYTE bValue,CPlayer *pPlayer);			//���͸���������Ϣ���ͻ���
	long		ReMoveGoldAndGoods(CMail *pMail,CPlayer *pPlayer);						//�����ʼ���������
	void		SendMailToReceiver(CGUID &guid,long lRet);								//���Ϳ۳���Ʒ��Ǯ�Ľ��
	void		SendGetGoodsGSToClient(CGUID &MailGuid,CGUID &PlayerGuid,long lIndex);	//����ȡ��Ʒ���ͻ���

	void		SendMailFailedToClient(CGUID guid,long lValue);							//�ʼ�����ʧ��	
	void		DecordFromByteArray(BYTE* pByte, long& pos,tagMailParam *pMailParam);	//����
	void		DecordMailMsgFromByteArray(BYTE* pByte, long& pos,tagMailParam *pMailParam);//�����ʼ���Ϣ	
	long		GetYuanBaoNum(long lIndex,long lNum,CPlayer *pPlayer);					//��ȡԪ������	
	void		SendPMail(CGUID &MailGuid,CPlayer *pPlayer,long lRet);					//�������ʼ�
	void		SendPresentMailToWS(CMail *pMail,long lRet);							//���������ʼ���WS
	void		SendReceiveMail(CPlayer *pPlayer);										//����
	void		AgreeToPresent(CMail *pMail,CPlayer *pPlayer,BYTE bType);				//ͬ������
	bool	    CheckPresentGoods(CMail *pMail,CPlayer *pPlayer,long &lNum);			//�����ֵ��Ʒ
	void		BuyItemByMail(CMail *pMail,CPlayer *pPlayer);							//ͨ���ʼ�������ֵ����	
	bool		CheckDeleteMail(list<CGUID> &lguid,CPlayer *pPlayer);									//���ɾ��	
	bool		SetMapMail(CMail *pMail);												//����

	void		SendReadMailRet(CGUID &MailGuid,const CGUID &PlayerGuid, BYTE bRet);			//���Ͷ�ȡ�ʼ����	
	eMAILRESULT	CheckMailCondition(CMail *pMail, CPlayer *pPlayer);						//����ʼ���������	
	CMail		*GetMail(CGUID &guid);													//��ȡ�ʼ�

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
private:

	map<CGUID,CMail*>	m_mapMail;  
	map<long,CMail*>	m_mapSysMail;													//ϵͳ�ʼ�
	//map<CGUID,CMail*>   m_mapExSysMail;													//ϵͳ�����ʼ�
};