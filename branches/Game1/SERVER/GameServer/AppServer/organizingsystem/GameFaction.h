#pragma once
#include "..\public\OrganizingDef.h"
#include "S2CSession.h"
//Faction.h
//Function:�ṩ�������ӿڡ���ɰ�Ṧ�ܡ�
//Authou:Joe
//Create Time:2004.6.11

class GameFaction : public BaseMemObj
{
public:
	GameFaction(void);
	~GameFaction(void);

public:
	//!							����
	BOOL						AddToByteArray(vector<BYTE>& ByteArray);
	//!							����(pMapApplicant���ⲿͳ���������Ƿ��ظ����õ�map)
	BOOL						DecodeFromMsgSet(DBReadSet &DbReadSet, map<CGUID, CGUID> *pMapApplicant = NULL);

	//!							���������ݵ�bufĩβ
	void						AddIconData_ToArray(vector<BYTE> *pByteArray);
	//!							���빫����Ϣ����Ϣĩβ
	void						AddPublicData_ToMsgBack(CMessage *pMsg);
	void						AddPublicData_ToArray(vector<BYTE> *pByteArray);
	void						AddPublicData_ToDataBlock(DBWriteSet& setWriteDB);
	//!							����������ϵ���Ϣĩβ
	void						AddBaseData_ToMsgBack(CMessage *pMsg);
	//!							�����Ա���ϵ���Ϣĩβ
	void						AddMember_ToMsgBack(CMessage *pMsg, const CGUID &AimGUID);
	//!							�����Ա����
	void						AddMember_ToMsgBack(CMessage* pMsg);
	//!							����һ����Ա���ϵ���Ϣĩβ
	void						AddOneMember_ToMsgBack(const CGUID &MemeberGuid, CMessage *pMsg, const CGUID &AimGUID);
	//!							�������������ϵ���Ϣĩβ
	void						AddApply_ToMsgBack(CMessage *pMsg);
	//!							���빫�浽��Ϣĩβ
	void						AddPronounce_ToMsgBack(CMessage *pMsg);
	//!							�������Ե���Ϣĩβ
	void						AddLeaveWord_ToMsgBack(CMessage *pMsg);
	//!							�õ�������������Ϣ
	void						GetApplyInfo(tagApplyInfo &ApplyInfo);

	//!							����һ����Ա��Player����Ĺ�����Ϣ
	void						UpdateMemberFacInfo(map<CGUID, tagFacMemInfo>::const_iterator &iteMemInfo);
	void						UpdateMemberFacInfo(CPlayer *pPlayer);

	//!							��Ӧ��������
	void						OnJoinUnion(GameUnion *pUnion);
	//!							��Ӧ�˳�����
	void						OnQuitUnion(void);
	//!							��Ӧ���˽�ɢ
	void						OnDisbandUnion(void);
	//!							Ϊ��Ա��ʼ��������Ϣ
	void						SetMemberUnionInfo(const CGUID &UnionGuid);
	//!							��Ӧִ�и�������
	void						OnBoonRun(void);
	

	//!							�õ���֯ID
	const		CGUID&			GetExID(void);					
	//!							������֯ID
	const		CGUID&			SetExID(const CGUID& guid) ;
	//!							�õ���֯������
	const		char*			GetName(void)const;				
	//!							���ð�������
	void						SetName(const char* pName);
	//!							�õ�����
	BYTE						GetCountry(void);

	//!							��Ա�ж�
	const		CGUID&			IsMember(const CGUID& MemberGuid);
	bool						IsMember( const std::string &name ) const;
	//!							�������ж�
	BOOL						HasApplicant(const CGUID& ApplicantGuid);
	//!							��û᳤
	const		CGUID&			GetMaster(void);
	//!							�õ���ǰ�л�ȼ�
	LONG						GetLevel(DWORD dwUpgradeType);
	//!							�õ���ǰ�л�ȼ�
	BOOL						SetLevel(LONG lLvl, DWORD dwUpgradeType);
	//!							�õ���Ա�ٽ�
	LONG						GetMemberJobLvl(const CGUID& MemberGuid);
	//!							������ļ״̬
	BOOL						SetRecruitState(BOOL bOpen);

	//!							�жϵ�ǰ�ܷ��ɢ
	eFactionOptErrInfo			CanDisban(void);
	//!							��ɢ
	void						Disband(void);
	//!							����ԱȨ��
	BOOL						TestPurview(const CGUID& MemberGuid, eMemberPurview ePurview);
	//!							�õ���ԱȨ��
	LONG						GetPurview(const CGUID& MemberGuid);
	//!							�õ���Ա������
	const tagFacMemInfo*		GetMemberData(const CGUID& MemberGuid);
	//!							��ӳ�Ա
	void						AddMember(tagFacMemInfo &FacMemInfo);
	//!							ɾ����Ա
	void						DelMember(const CGUID& MemberGuid);
	//!							���һ��������
	BOOL						AddApplicant(tagFacApplyPerson &FacApplyPerson);
	//!							ɾ��һ��������
	BOOL						DelApplicant(const CGUID& ApplicantGuid);
	//!							����һ��ְ��������
	BOOL						SetJobName(LONG lJobLvl, const char *pNewName);
	//!							����һ��ְ����Ȩ��
	BOOL						SetJobPurview(LONG lJobLvl, LONG lPurview);
	//!							���ù���
	BOOL						SetPronounce(tagOrgPronounceWord &OrgPronounceWord);
	//!							���ñ���
	BOOL						SetLeaveword(tagOrgPronounceWord &OrgPronounceWord);
	//!							���ó�Ա��ְ��
	BOOL						SetMemJobLvl(const CGUID& MemberGuid, LONG lJobLvl);
	//! 						���ó�Ա�ĳƺ�
	BOOL						SetMemberTitle(const CGUID& MemberGuid, const char *pNewTitle);
	//! 						�õ���Ա�ĳƺ�
	const char*					GetMemberTitle(const CGUID& MemberGuid);
	//!							���ó�Ա��ͼ
	BOOL						SetMemberMap(const CGUID& MemberGuid, const char *pMapName);
	//!							���ó�Ա�ȼ�
	BOOL						SetMemberLevel(const CGUID& MemberGuid, LONG lLevel);
	//!							���ó�Ա����
	BOOL						SetMemberHide(const CGUID& MemberGuid, BYTE cHide);
	//!							�õ���Ա�ȼ�
	LONG						GetMemberLevel(const CGUID& MemberGuid);
	//!							���ó�Աְҵ
	BOOL						SetMemberOccu(const CGUID& MemberGuid, LONG lOccu);
	//!
	BOOL						SetMemberName(const CGUID& MemberGuid, const char *pName);
	//!							�����»᳤
	BOOL						SetNewMaster(tagFacMemInfo &NewMaster, tagFacMemInfo &OldMaster);
	//!							����״̬�ı�
	BOOL						MemberOnlineChange(const CGUID& MemberGuid, BOOL bOnline, LONG lLastOnlineTime);
	//!							�ȼ��ı�
	BOOL						MemberLvlChange(const CGUID& MemberGuid, LONG lLvl);
	//!							��ù��ṫ��
	const tagOrgPronounceWord&	GetPronounceWord(void);

	//!							���û��
	BOOL						SetIcon(BYTE* IconData, long size);
	//!							�õ����
	void						GetIcon(vector<BYTE> &vIconData);
	//!
	DWORD						GetIconCRC32(VOID){return m_dwCRC32IconCode;}

public:
	//!							������Ϣ�����г�Ա
	void						SendToAll(CMessage &msg);

	//!							��ӹ�����Ϣ��ע�ߣ�ϵͳ��Թ�ע�ߵĹ�����Ϣ���ּ�ʱ����
	void						AddAttention(const CGUID &MemberGuid);
	//!							ɾ��������Ϣ��ע��
	void						DelAttention(const CGUID &MemberGuid);
	//!							������Ϣ�����й�ע������Ϣ�ĳ�Ա
	void						SendToAllConcern(CMessage &msg);

	//!							�õ���ǰ��Դ��
	LONG						GetRes(VOID);
	//!							�õ���ǰ������
	LONG						GetExp(VOID);
	//!							�õ���ǰ��Ա������
	LONG						GetMemberContribute(const CGUID &MemberGuid);

	//!							������Դ��ֵ
	VOID						SetRes(LONG lRes);
	//!							���þ���
	VOID						SetExp(LONG lExp);
	//!							���ó�Ա����
	VOID						SetMemberContribute(const CGUID &MemberGuid, LONG lContribute);
	//!							��ȡ�ݵ���Ʊ��
	LONG						GetTickets(char* cBaseName);
	//!							��ȡ��Ա����Ȩ
	BOOL						GetJoin(const CGUID& guid);
	//!							��ȡս��ֵ
	LONG						GetBattle(CGUID guid=NULL_GUID);
	//!							��ȡͶ��ֵ
	LONG						GetArmyInvest(CGUID guid=NULL_GUID);
	//!							�ı�ս��ֵ
	void						AddBattle(LONG lValue,CGUID PlayerGuid);
	//!							�ı�Ͷ��ֵ
	void						AddArmyInvest(LONG lVlue,CGUID PlayerGuid);
	//!							���ó�Ա����Ȩ
	void						SetJoin(CGUID& PlayerGuid,LONG lFlags);
	//!							���þݵ���Ʊ��
	void						SetTickets(char* cBaseName,long lNum);
	//!							���þݵ�ս��
	void						SetBaseBattle(char* cBaseName,long lNum);
	//!							��ȡ�ݵ�ս��
	LONG						GetBaseBattle(char* cBaseName);
	//!							��ӿƼ�
	void						AddTechLv(LONG lTechNo,LONG Lv){m_TechLv[lTechNo]=Lv;}	
	//!							��ȡ�Ƽ��ȼ�
	LONG						GetTechLv(LONG lTechNo){return m_TechLv.find(lTechNo)!=m_TechLv.end()?m_TechLv[lTechNo]:0;}
	typedef						map<CGUID, tagFacMemInfo>::iterator MEMBERITER;
	//!							�������ж���Ϣ���ͻ���
	BOOL						AddEnemyInfoToMsg(CMessage* pMsg);
	//!							������Ʊ��¼���ͻ���
	BOOL						AddBaseInfoToMsg(CMessage* pMsg);
	//!							�޸ĵжԼ�����Ϣ
	BOOL						ChangeEnemy(eBattleType eType,char* cName,DWORD dwTime,DWORD dwEndTime,LONG lFlags=0);
	//!							�޸�һ���ݵ����Ϣ
	BOOL						ChangeBaseInfo(char* cName);
	//!							���;ݵ���Ϣ���ͻ���
	void						SendBaseInfo();
	//!							��ȡ��ռ��ľݵ�
	LONG						GetBaseNum();
	//!							����ҷ��͹�����Ϣ
	void						FacDisplay(const CGUID& guid,long lValue1,long lValue2);
	//!							��վݵ�
	void						ClearBase();
	//�������
private:
	//!									������Ϣ
	tagFacPublicData					m_FacPublicData;
	//!									������Ϣ����
	tagFacBaseData						m_FacBaseData;
	//!									�Զ�����
	vector<BYTE>						m_vIconData;
	//!									�����֤��
	DWORD								m_dwCRC32IconCode;

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

	//!									�����л����ĳ�Ա
	S2CSession<CGUID>					m_OptingMember;
	//!									ӵ�и����ݵ����Ϣ
	map<string,tagBaseInfo>				m_mapBaseInfo;
	//!									����Ƽ��ȼ�
	map<LONG,LONG>						m_TechLv; 
	//!									����ս����Ϣ
	map<eBattleType,list<tagEnemy> >	m_Enemy;
};
