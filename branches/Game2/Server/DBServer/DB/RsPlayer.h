#pragma once
#include "..\myadobase.h"
#include "RSGoods.h"
#include "Largess.h"
class CDBPlayer;
class CDBLoginPlayer;
class CMessage;
class CDBGoods;
class CPlayerRanks;
struct LoadThreadData;
class CDBLinkmanGroup;


class CRsPlayer :
	public CMyAdoBase
{
public:

	CRsPlayer(void);
	virtual ~CRsPlayer(void);

	bool ValidatePlayerIDInCdkey(LPCSTR szCdkey, const CGUID& nID, _ConnectionPtr& cn);

	BYTE GetPlayerCountInCdkey(LPCSTR szCdkey);
	BYTE GetPlayerCountInDBbyCdkey(LPCSTR szCdkey,_ConnectionPtr& cn);

	bool IsNameExist(LPCSTR szName);

	CGUID GetPlayerExID(const char* strName);

	void GetPlayerNameByID(IN const CGUID& dwPlayerID, OUT char *szPlayerName);

	string GetCDKey(const char* strName);

	bool LoadPlayer(CDBPlayer *pPlayer, _ConnectionPtr& cn);//���������

	bool StatRanks(CPlayerRanks* pPlayerRanks,_ConnectionPtr& cn);			//ͳ��������а�


	// �����¼��ɫ��Ϣ
public:
	bool OpenPlayerBase(LPCSTR szCdkey, std::list<LoadThreadData*>& loadDataList, _ConnectionPtr& cn);//����һ�������
private:
	bool OpenPlayerBaseInDB(LPCSTR szCdkey, std::list<LoadThreadData*>& loadDataList, long &cnt, _ConnectionPtr& cn);//����һ�������



	// check list and database ,get resault of frist empty location (ranger 1-3, 0 no location)
	//BYTE GetFristEmptyLocation(LPCSTR szCdkey);
	//////////////////////////////////////////////////////////////////////////
	// ������ɫ
	//////////////////////////////////////////////////////////////////////////
public:
	bool CreatePlayer(CDBPlayer *pPlayer, _ConnectionPtr& cn);	// save thread call
private:
	bool CreatePlayerBase(CDBLoginPlayer *pPlayer, _ConnectionPtr& cn);
	bool CreatePlayerAbilities(CDBPlayer *pPlayer, _ConnectionPtr& cn);
	//////////////////////////////////////////////////////////////////////////

public:
	// �߼�ɾ��һ����ɫ
	bool DeletePlayer(const CGUID& nID, time_t tDelDate, _ConnectionPtr& cn);	// save thread call

	bool RestorePlayer(const CGUID& nPlayerID, _ConnectionPtr& cn);	// save thread call

	// ��ȡ�����ɾ���Ľ�ɫ��ɾ��ʱ��
	time_t GetPlayerDeletionDate(const CGUID& nID, _ConnectionPtr& cn);
	time_t GetPlayerDeletionDateByRs(const CGUID& nID, _RecordsetPtr& rs);
	// ���õ�¼ʱ��,��WS����MSG_W2DB_PASS_LOG_PLAYERBASELIST��Ϣʱ����
	void UpdateLogoutTime(const CGUID& nID, WORD* szLoginTime, _ConnectionPtr& cn);
	// ��ȡ����˵�¼�Ľ�ɫ���ϴ����ߵ�ʱ��(����)
	time_t GetPlayerLastLogoutDate(const CGUID& nID, _ConnectionPtr& cn);
	//////////////////////////////////////////////////////////////////////////
	//���������
	//////////////////////////////////////////////////////////////////////////
public:
	// �������ȫ������
	bool SavePlayer(CDBPlayer *pPlayer, _ConnectionPtr& cn);		// save thread call

	// ������һ�������
	bool SavePlayerBase(CDBLoginPlayer *pPlayer, _ConnectionPtr& cn);
	
	// ���������ͨ��������(������,״̬,��������,�ű����ݵ�)
	bool SavePlayerAbilities(CDBPlayer *pPlayer, _ConnectionPtr& cn);
	//////////////////////////////////////////////////////////////////////////

public:
	// Ԥ������
	void PreLoadPlayerBase(_ConnectionPtr& cn);
	// Ԥ������
	void PreLoadPlayerAttrs(_ConnectionPtr& cn);

	// only check database
	BYTE GetLocationStateInCdkey(LPCSTR szCdkey);

	//��Buffer
	bool SaveBufferField(CDBPlayer *pPlayer, const char* szFiledName, _RecordsetPtr &pRs);
	//��Buffer
	bool LoadBufferField(CDBPlayer *pPlayer, const char* szFiledName, _RecordsetPtr &pRs);

	//��ȡС��������
	bool LoadSpriteSystem(CDBPlayer* pPlayer, _ConnectionPtr &cn);
	//����С��������
	bool SaveSpriteSystem(CDBPlayer* pPlayer, _ConnectionPtr &cn);

	//��ȡ�����Ҫ�����ڷ�����������
	bool LoadSetupOnServer(CDBPlayer* pPlayer, _ConnectionPtr &cn);
	//���������Ҫ�����ڷ�����������
	bool SaveSetupOnServer(CDBPlayer* pPlayer, _ConnectionPtr &cn);

	//��ȡ��ϵ������
	bool LoadLinkmanData(CDBPlayer* pPlayer, CDBLinkmanGroup* linkmanGroup, _ConnectionPtr cn);
	//������ϵ����Ϣ
	static bool SaveLinkmanData(CDBLinkmanGroup* linkmanGroup, _ConnectionPtr cn);

	// ����������ֲ�����Ҷ���
	bool CheckPlayerIsExist(const char* szName, _ConnectionPtr& cn);
};
