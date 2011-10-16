#pragma once
#include "..\myadobase.h"
#include "../public/GUID.h"
#include <map>
#include "../public/DBDefine.h"
//#include "..\..\server\dbserver\appworld\DBCityWar.h"
class CBaseDBEntity;
class CDBMail;
class CDBMailGoods;
class CDBMailGoodsGroup;
class CDBMailGroup;

class CRsMail :
	public CMyAdoBase
{
public:
	CRsMail(void);
	~CRsMail(void);

public:

	//! ��ѯ�ʼ�
	bool	QueryPlayerMails(long curTime, CDBMailGroup* mailGroup, _ConnectionPtr& cn);
	//! ��ȡ��ҵ������ʼ���Ϣ
	bool	LoadPlayerMails(const CGUID& playerID, CDBMailGroup* mailGroup, _ConnectionPtr& cn);
	//! ��ȡ�ʼ���Ϣ
	void    LoadMail(CDBMail* mail, _RecordsetPtr& pRs, _ConnectionPtr& cn);
	//! ������ҵ������ʼ���Ϣ
	DB_OPERATION_RESULT_TYPE	SavePlayerMails(CDBMailGroup* mailGroup, _ConnectionPtr& cn);
	//! �����ʼ���Ϣ
	void    SaveMail(CDBMail* mail, _ConnectionPtr& cn);
	//! ɾ����ҵ������ʼ���Ϣ
	DB_OPERATION_RESULT_TYPE	DeletePlayerMails(const CGUID& playerID, _ConnectionPtr& cn);
	//! ɾ���ʼ���Ϣ
	DB_OPERATION_RESULT_TYPE	DeleteMails(CDBMailGroup* mailsGroup, _ConnectionPtr& cn);
	//! ɾ���ʼ���Ϣ
	void    DeleteMail(const CGUID& guid, _ConnectionPtr& cn);

	//! ��ȡ�ʼ�����Ʒ��Ϣ
	DB_OPERATION_RESULT_TYPE		LoadMailGoods(const CGUID& mailGUID, CDBMailGoodsGroup* goodsGroup, _ConnectionPtr& cn);
	//! �����ʼ�����Ʒ��Ϣ
	static DB_OPERATION_RESULT_TYPE SaveMailGoods(const CGUID& mailGUID, set<CDBMailGoods*>& goodsGroup, _ConnectionPtr& cn);

	//! ɾ����ҵ������ʼ���Ʒ��Ϣ
	bool	DeletePlayerMailsGoods(set<CGUID>& mailSet, _ConnectionPtr& cn);
	//! ɾ���ʼ�����Ʒ��Ϣ
	void    DeleteMailGoods(const CGUID& mailID);

	//! ���ݿ������ϵͳ�ż�
	void	InsertSysMailToGroup(CDBMail* pMail, MAIL_LEVEL_OPER_FLAG levelOpFlag, long level, long sex, long* countryID, long countryNum, _ConnectionPtr& cn);
	//��Buffer
	static bool SaveBufferField(CBaseDBEntity *pGoods, const char* szFiledName, _RecordsetPtr &pRs);
	//��Buffer
	bool LoadBufferField(CBaseDBEntity *pGoods, const char* szFiledName, _RecordsetPtr &pRs);
};
