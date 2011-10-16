//WorldServerSession.h/////////////////////////////////////////////////////////////////////
//������:�Ự����ʵ����
//Author:������
//Create Time:2008.11.03
#ifndef WORLD_SERVER_SESSION_H
#define WORLD_SERVER_SESSION_H
#include "CSession.h"
#include <vector>

class Account;
class CMail;
class CEntityGroup;
class CEntityProperty;
class CTempEntityProperty;

//##ModelId=42266D9200FA
class CWorldServerSession : public CSession
{
public:

	enum WORLD_SESSION_STEP_DEF
	{
		WORLD_SESSION_STEP_NULL = 0,
		WORLD_SESSION_CREATE_ACCOUNT,
		WORLD_SESSION_CREATE_LOGINPLAYER,
		WORLD_SESSION_CREATE_PLAYER,
		WORLD_SESSION_CREATE_FACTION,
		WORLD_SESSION_CREATE_PLAYER_GOODS,
		WORLD_SESSION_DEL_FACTION,
		WORLD_SESSION_DEL_UNION,

		WORLD_SESSION_SAVE_ACCOUNT,		
		WORLD_SESSION_SAVE_PLAYER,
		WORLD_SESSION_SAVE_PLAYER_PHRGN,
		WORLD_SESSION_SAVE_LOGINPLAYER,
		WORLD_SESSION_SAVE_LOGINPLAYER_DETIME,
		WORLD_SESSION_RESTORE_LOGINPLAYER_DETIME,
		WORLD_SESSION_SAVE_REGION,
		WORLD_SESSION_SAVE_LINKMAN,
		WORLD_SESSION_SAVE_FACTION,
		WORLD_SESSION_SAVE_UNION,
		WORLD_SESSION_SAVE_PHRGN,
		WORLD_SESSION_SAVE_GLOBALVAR,

		WORLD_SESSION_LOAD_ACCOUNT,
		WORLD_SESSION_LOAD_PLAYER,
		WORLD_SESSION_LOAD_REGION,
		WORLD_SESSION_LOAD_LINKMAN,
		WORLD_SESSION_LOAD_FACTION,
		WORLD_SESSION_LOAD_UNION,
		WORLD_SESSION_LOAD_COUNTRY,
		WORLD_SESSION_LOAD_PHRGN,
		WORLD_SESSION_LOAD_GLOBALVAR,
		WORLD_SESSION_LOAD_PLAYER_MAILS,

		WORLD_SESSION_SAVE_ONLINE_PLAYER_MAILS,

		WORLD_SESSION_INSERT_PLAYER_MAIL,

		WORLD_SESSION_INSERT_OFFLINE_PLAYER_MAILS,

		WORLD_SESSION_INSERT_FACTION,
		WORLD_SESSION_INSERT_UNION,

		WORLD_SESSION_DELETE_ONLINE_PLAYER_MAILS,

		// �������ǰ�ȼ���Ƿ�Ϸ�����
		WORLD_SESSION_CREATE_PLAYER_CHECK_CONDITION,

		WORLD_SESSION_DELETE_INSERT_PLAYER_GOODS,
		WORLD_SESSION_DELETE_INSERT_ONLINE_PLAYER_MAILS,

		WORLD_SESSION_SYSTEM_DELETE_OFFLINE_PLAYER_MAILS,

		WORLD_SESSION_CHECK_OFFLINE_PLAYER_NAME,

		WORLD_SESSION_CREATE_REGION,

		WORLD_SESSION_BAN_PLAYER,

		WORLD_SESSION_SAVE_BUSINESS,
		WORLD_SESSION_LOAD_BUSINESS,
		
		WORLD_SESSION_CHANGE_NAME,

		WORLD_SESSION_LOAD_GOODS_LIMIT_RECORD
	};

	CWorldServerSession(DWORD dwLifeTime);
	~CWorldServerSession();

	//##ModelId=41DE28EC0242
	virtual BOOL IsSessionEnded();

	//##ModelId=41DE197F0290
	virtual VOID Release();

	//##ModelId=41DDF8FA00EA
	virtual BOOL IsSessionAvailable();

	virtual BOOL IsSessionShouldBeenRemoved();

	//##ModelId=41DCD273036B
	virtual VOID AI();

	//������ʼ
	//##ModelId=41DCD60D0119
	virtual BOOL Start( BOOL bFlag = FALSE );

	//��������
	//##ModelId=41DCD61A00DA
	virtual BOOL End( BOOL bFlag = FALSE );

	//�쳣����
	//##ModelId=41DCD62A0186
	virtual BOOL Abort( BOOL bFlag = FALSE );

	virtual BOOL OnSessionStarted( BOOL bFlag );

	virtual BOOL OnSessionEnded( BOOL bFlag );

	virtual BOOL OnSessionAborted( BOOL bFlag );

	void SendChangeNameProc(const CGUID& guid, const char* szName, long gsscoketid);
	void ProcessChangeNameProcMsg(long retFlag, CEntityGroup* pEntity);

	// �Ự������Ϣ�ӿ�
	void SendLoadAccountMsg(Account* acc);
	void SendLoadCountryMsg();
	void SendLoadFactionGroupMsg();
	void SendLoadUnionGroupMsg();
	void SendCreatePlayerCheckProcMsg(const char* szCdkey, const char* szName, BYTE nOccu, 
		BYTE sex,BYTE nHead, BYTE nFace, BYTE btCountry, BYTE btMaxCharactersNum,
		BYTE byConstellation,bool bSelectRecommCountry);
	BOOL SendDelFactionProcMsg(const CGUID &OrganGuid);
	void SendDelUnionProcMsg(const CGUID &OrganGuid);
	void SendLoadPlayerDataMsg(const char* szCdkey, const CGUID& guid);

	void SendLoadLinkmanMsg(const CGUID &PlayerGuid);
	void SendSaveLinkmanMsg(const CGUID &PlayerGuid);
	
	void SendSaveAccountMsg(Account* pAcc);
	void SendSaveLoginPlayerDataMsg(const CGUID& guid);
	void SendSavePlayerDataMsg(long bitValue, const CGUID& guid, bool isSaveDbPlayerFlag/*true:����ʹ��DBPlayer����*/);
	void SendLoadRegionGroupMsg(void);
	void SendSaveRegionMsg(CEntityGroup* pRegion);
	void SendCreateRegionMsg(CEntityGroup* pRegion);
	void SendSaveFactionMsg(vector<CEntityGroup*> &vCEntityGroup);
	void SendSaveUnionMsg(CEntityGroup* pUnion);
	void SendInsertFactionMsg(CEntityGroup* pFaction);
	void SendInsertUnionMsg(CEntityGroup* pUnion);
	void SendLoadPhRgnGroupMsg(void);
	void SendSavePhRgnMsg(CEntityGroup* pPhRgn);
	void SendLoadGlobalVarGroupMsg(void);
	void SendSaveGlobalVarGroupMsg(void);

	// ����ɾ������ʼ��Ĵ洢����
	void SendDelRoleMailProcMsg(const char* szName);

	// ����/��ȡ��ҵ����
	void SendSaveBusinessMsg();
	void SendLoadBusinessMsg();

	void SendLimitGoodsRecordUpdateMsg();
	void SendLoadLimitGoodsRecordUpdateMsg();
	void SendUpdateLoginPlayerTimeMsg(const char* szCdkey, const CGUID& guid);
	void SendRestoreLoginPlayerTimeMsg(const char* szCdkey, const CGUID& guid);

	// ��ҳ�ʼ����ȡ�ż�
	void SendInitLoadMail(const CGUID& guid);
	// �������ż�
	void SendInsertMailMsg(CMail* pMail);
	
	// �������ɾ���ż�
	void SendDeleteOnlineMailMsg(const CGUID& ownerID, vector<CMail*>& mail);
	// ������ұ����ż�
	void SendSaveOnlineMailMsg(const CGUID& ownerID, vector<CMail*>& mail);
	// ������������ּ��(����GUID)�洢����
	void SendCheckOfflinePlayerNameProcMsg(const char* szOwnerName, const CGUID& WriterGuid);
	// �������������ż�
	
	// ϵͳɾ������������ż�
	
	// Ban Player proc
	void SendBanPlayerProcMsg(const char* szName, long time);
	
	void NakeSendSaveLoginPlayerDataMsg(CPlayer* pPlayer, CEntityGroup* pLoginPlayer);
	void NakeSendSavePlayerDataMsg(long bitValue, CPlayer* pPlayer, CEntityGroup* pDbPlayer, bool isSaveDbPlayerFlag/*true:����ʹ��DBPlayer����*/);

	static void ProcessLoadAccount(long retFlag,CEntityGroup* pBaseEntity);
	void ProcessLoadCountry(long retFlag,CEntityGroup* pBaseEntity);
	void ProcessLoadFaction(long retFlag,CEntityGroup* pBaseEntity);
	void ProcessLoadUnion(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessSaveFaction(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessSaveUnion(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessInsertFaction(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessInsertUnion(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessDelFaction(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessDelUnion(long retFlag,CEntityGroup* pBaseEntity);
	void ProcessLoadRegion(long retFlag,CEntityGroup* pBaseEntity);
	void ProcessLoadGlobalVar(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessCreatePlayerCheck(long retFlag,CEntityGroup* pBaseEntity);
	
	
	bool ProcessLoadPlayerData(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessSaveLoginPlayerData(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessSavePlayerData(long retFlag,CEntityGroup* pBaseEntity);
	bool ProcessCreatePlayerGoodsData(long retFlag,CEntityGroup* pBaseEntity);

	bool ProcessInsertOnlineMail(long retFlag, CEntityGroup* pBaseEntity);
	bool ProcessDeleteOnlineMail(long retFlag, CEntityGroup* pBaseEntity);
	bool ProcessSaveOnlineMail(long retFlag, CEntityGroup* pBaseEntity);

	bool ProcessLoadPlayerMails(long retFlag, CEntityGroup* pBaseEntity);
	bool ProcessDeleteOnlineMails(long retFlag, CEntityGroup* pBaseEntity);

	// �����������������ִ洢���̷��ؽ��(GUID)
	bool ProcessCheckOfflinePlayerNameProc(long retFlag, CEntityGroup* pBaseEntity);
	// ����BanPlayer
	bool ProcessBanPlayer(long retFlag, CEntityGroup* pBaseEntity);

	bool ProcessSaveGlobalVarMsg(long retFlag, CEntityGroup* pBaseEntity);
	bool ProcessLoadPhRgnGroupMsg(long retFlag, CEntityGroup* pBaseEntity);
	// �Ự��ʱ��������ӿ�
	void TempDataRelease();
	// ���δλỰ����ʱ����
	vector<CTempEntityProperty*>& GetTempDataVec(void) { return m_pTempDataMgr; }

	long GetCurOperCount(void) { return m_lOperCount; }
	private:
		vector<CTempEntityProperty*> m_pTempDataMgr;
		long	m_lOperCount;
};
#endif
