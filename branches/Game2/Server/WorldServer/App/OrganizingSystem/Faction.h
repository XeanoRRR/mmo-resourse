#pragma once
#include "organizing.h"
#include "OrganBuffSys.h"
//Faction.h
//Function:�ṩ�������ӿڡ���ɰ�Ṧ�ܡ�
//Authou:Joe
//Create Time:2004.6.11

class CMessage;
class CUnion;
class CEntityGroup;

class CFaction
	: public COrganizing 
{
public:
	//! ���ڴ�DB���ݹ����Դ��ڵĶ���
	CFaction(void);
	//! ������Ϸ����Ҵ����µĶ���
	CFaction(const CGUID &FacGuid, const char *pName, CPlayer *pMaster,const char *pPro);

	~CFaction(void);

	//��صĲ���
public:
	void Insert(void);
	void Save(CEntityGroup** ppTmpFaction);												//! ����
	BOOL CodeToMsgSet(DBWriteSet &DbWriteSet);										//! ����
	BOOL DecodeFromMsgSet(DBReadSet &DbReadSet);												//! ����
	//! �̳еĽӿ�
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const CGUID&	GetExID(void);														//! �õ���֯ID
	const CGUID&	SetExID(const CGUID& guid);											//! ������֯ID
	const char*		GetName(void)const;													//! �õ���֯������
	void			SetName(const char* pName);											//! ���ð�������


	BOOL ApplyForJoin(const CGUID& PlayerGuid);											//! �������
	BOOL ClearApplyList(void);															//! ��������б�
	BOOL AddMeber(CPlayer *pPlayer, const CGUID &InviterGuid = NULL_GUID);				//! ��ӳ�Ա
	BOOL Exit(const CGUID& PlayerGuid);													//! �˳�
	BOOL FireOut(const CGUID& FireID,const CGUID& BeFiredID);							//! ����
	BOOL Demise(const CGUID& MasterID,const CGUID& ID);									//! ��λ
	BOOL DubAndSetJobLvl(const CGUID& DubID,const CGUID& BeDubedID,string& strTitle);	//! ����ƺ�
	const CGUID& GetMasterID(void);														//! �õ��쵼��ID
	const CGUID& IsMember(const CGUID& guid);											//! ��Ա�ж�
	LONG GetMemberNum(void);															//! �õ���Ա��

	BOOL Disband(const CGUID& guid = NULL_GUID);										//! ��ɢ

	BYTE GetCountry(void);																//! �õ�����ID
	void SetCountry(BYTE c);															//! ���ù���ID
	const CGUID&  GetSuperior(void);													//! �õ��ϼ�
	void  SetSuperior(CUnion *pUnion);													//! �����ϼ�
	LONG GetEstablishedTime(void) const;												//! �õ��ý���ʱ��

	BOOL Pronounce(CPlayer *pPlayer, const char *pPro);									//! ����
	BOOL Leaveword(CPlayer *pPlayer, const char *pPro);									//! ����
	void Talk(const CGUID& PlayerGuid, const char *pSourceName, const char *pTxt);		//! ��֯����

	BOOL Initial(void);																	//! ��ʼ��


	//!										�õ��쵼��ID
	const char*								GetMasterName(void);													
	//!										�õ�������Ϣ
	tagFacPublicData&						GetPublicData(void){return m_FacPublicData;}
	//!										�õ�������Ϣ����
	tagFacBaseData&							GetBaseData(void){return m_FacBaseData;}
	//!										���û��
	BOOL									SetIcon(BYTE* IconData, long size);
	//!										�õ����
	void									GetIcon(vector<BYTE> &vIconData);
	//!										�õ�����
	tagOrgPronounceWord&					GetPronounceWord(void){return m_Pronounce;}
	//!										�õ�����
	tagOrgPronounceWord&					GetLeaveword(void){return m_LeaveWord;}

	//!										�õ���Ա��Ϣ�б�
	map<CGUID, tagFacMemInfo>&				GetMemberMap(void){return m_mapMemInfo;}
	//!										�õ������б�
	map<CGUID, tagFacApplyPerson>&			GetApplyMap(void){return m_mapFacApplyPerson;}

	//! ϵͳ����
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//!										���Ͷ���GS
	void									SendToGameServer(LONG lSocketID);

	//!										����һ����Ա������Ϸ
	void									OnMemberEnterGame(CPlayer *pPlayer);
	//!										����һ����Ա�뿪��Ϸ
	void									OnMemberExitGame(CPlayer *pPlayer);

	enum
	{
		Nothing,
		ImmediatelyDisband,
	};
	//!										ִ�й�����֤����
	DWORD									OnValidate(DWORD dwTime=0);



	//! ������
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//!										�����Ա����
	VOID									ResetMember(VOID);
	//!										���ĳIDִ��ĳ�������ĺϷ���
	BOOL									CheckOperValidate(const CGUID& ExecuteGuid,eMemberPurview eMP);				
	//!										����һ��ְ��������
	BOOL									SetJobName(LONG lJobLvl, const char *pNewName);
	//!										����һ��ְ����Ȩ��
	BOOL									SetJobPurview(LONG lJobLvl, LONG lPurrview);
	//!										���ó�Ա��ְ��
	BOOL									SetJobLvl(const CGUID& MemberGuid, LONG lJobLvl);
	//!										���ó�Ա�ƺ�
	BOOL									SetMemberTitle(const CGUID& OptGuid, const CGUID& MemberGuid, const char *pNewTitle);
	//!										���ó�Ա��ͼ
	BOOL									MemberMapChange(const CGUID& MemberGuid, const char *pMapName);
	//!										���ó�Ա�ȼ�
	BOOL									MemberLevelChange(const CGUID& MemberGuid, LONG lLevel);
	//!										���ó�Ա����״̬
	BOOL									MemberHideChange(const CGUID& MemberGuid, BYTE cHide);
	//!										���ó�Աְҵ
	BOOL									MemberOccuChange(const CGUID& MemberGuid, LONG lOccu);
	//!										���һ��������
	BOOL									AddApplicant(const CGUID& ApplicantGuid);
	//!										�ҵ�һ��������
	const	tagFacApplyPerson*				FindApplicant(const CGUID& ApplicantGuid);
	//!										ɾ��һ��������
	BOOL									DelApplicant(const CGUID& ApplicantGuid);
	//!										�������߼��뵽һ���ⲿ������б�
	void									AddApplicantToMap(map<CGUID, CGUID> &mapaLLApplicant);

	//!										��ӳ�Ա
	BOOL									AddMeber(const tagFacApplyPerson& FacApplyPerson, const CGUID &InviterGuid = NULL_GUID);

	//!										�õ���ǰ��Դ��
	LONG									GetRes(VOID);
	//!										�õ���ǰ������
	LONG									GetExp(VOID);
	//!										�õ���ǰ��Ա������
	LONG									GetMemberContribute(const CGUID &MemberGuid);

	//!										������Դ�����ʹ�������
	BOOL									AddRes(LONG lDeltaRes);
	//!										���Ӿ��飨���ʹ�������
	BOOL									AddExp(LONG lDeltaExp);
	//!										���ӳ�Ա���ף����ʹ�������
	BOOL									AddMemberContribute(const CGUID &MemberGuid, LONG lDeltaContribute);

	//!										�õ��ȼ�
	LONG									GetLvl(DWORD dwUpgradeType);
	//!										���õȼ�
	BOOL									SetLvl(LONG lLvl, DWORD dwUpgradeType);
	//!										�ճ�����
	DWORD									DailyConsume();
	//!										����Ա�������ʱ��
	BOOL									CheckLastTime();
	//!										���ݼ���ȼ���������Ƽ��ȼ�
	BOOL									ChangeTechLv();
	//!										���ĳ���ݵ����Ʊ
	LONG									AddTickets(char* cBaseName,LONG lNum);
	//!										���ĳ��ռս��ֵ
	LONG									AddBaseBattle(char* cBaseName,LONG lNum);
	//!										�޸�ս��ֵ
	LONG									AddBattle(long lValue,CGUID& PlayerGuid);	
	//!										�޸ľ���Ͷ��ֵ
	LONG									AddArmyInvest(long lValue,CGUID& PlayerGuid);
	//!										������Ҳ���Ȩ
	BYTE									SetJoin(CGUID& PlayerGuid,LONG lFlags);
	//!										����һ���Ƽ��ȼ�
	BOOL									AddTechLv(LONG lTechNo,LONG Lv);									
	map<string,tagBaseInfo>&				GetBaseInfo(){return m_mapBaseInfo;}
	//										�洢������ռ�ݵ�
	void									AddBaseInfoToByteArray(vector<BYTE>*pByte);
	void									DecodeBaseInfoFromByteArray(BYTE* pByte,long& pos);
	//										�����ͥ�Ƽ�
	void									AddTechLvToByteArray(vector<BYTE>* pByte);
	void									DecodeTechLvFromByteArray(BYTE* pBYTE,long& pos);
	//										���ӻ���ٵжԼ���ս����Ϣ,lFlags=0������
	BOOL									ChangeEnemy(eBattleType eType,char* cName,DWORD dwTime,LONG lFlags=0);
	//										�޸�һ���ݵ����Ϣ
	BOOL									ChangeBaseInfo(char* cName);
	//										��ȡ��ռ��ľݵ���
	LONG									GetBaseNum();
	//										��ռ���ռ��ľݵ�
	void									ClearBase();
private:
	//!										���һ���»�Ա
	void									AddNewMember(CPlayer *pPlayer, BOOL bIsMaster = FALSE);
				
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�������
private:
	//!									������Ϣ
	tagFacPublicData					m_FacPublicData;
	//!									������Ϣ����
	tagFacBaseData						m_FacBaseData;
	//!									�Զ�����
	vector<BYTE>						m_vIconData;

	//!									��Ա��Ϣ�б�
	map<CGUID, tagFacMemInfo>			m_mapMemInfo;
	//!									�����б�
	map<CGUID, tagFacApplyPerson>		m_mapFacApplyPerson;
	//!									�����б�
	//vector<tagOrgLeaveWord>				m_vFacLeaveWord;
	//!									��������
	tagOrgPronounceWord					m_Pronounce;
	//!									��������
	tagOrgPronounceWord					m_LeaveWord;
	//!									����Ƽ��ȼ�
	map<LONG,LONG>						m_TechLv;
	//!									ӵ�и����ݵ����Ϣ
	map<string,tagBaseInfo>				m_mapBaseInfo;
	//!									����ս����Ϣ
	map<eBattleType,list<tagEnemy>>		m_Enemy;
public:
	//!									���ݱ�����
	DWORD								m_dwDataChanged;
	//!									��ɢ��ʱ
	DWORD								m_dwDisbandTime;
	//!									��ԴΪ����ʱ
	DWORD								m_dwNegativeTime;
	//!									����Դ��ʱ��
	DWORD								m_dwSubResTime;

};
