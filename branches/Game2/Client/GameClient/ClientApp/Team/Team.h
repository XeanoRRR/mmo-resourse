/**
* @filename:Team.h
* @date: 2010/6/7
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: ��Ҷ�����߼���
*/
#include "../../../Public/Module/GUID.h"
#include "../../../Public/Common/TeamDef.h"
class CPlayer;
///��������ݽṹ
class TeamData
{
public:
	/// ״̬�ṹ
	struct tagState
	{
		long lID;						 
		long lIconID;
		long lLevel;					//	����״̬�ȼ�
		DWORD dwLeftTime;
		DWORD dwAlpha;					// ��ɫ
		bool IsDeBuff;					// ״̬��־���Ƿ���DeBuff״̬
		tagState()
		{
			lID = 0;
			lIconID = 0;
			lLevel = 0;
			dwLeftTime = 0;
			dwAlpha = 0xffffffff;
			IsDeBuff = false;
		}
	};
	/// ����ṹ
	struct tagTeam
	{
		std::string	strPlayerName;			/// �������
		long	lNameColor;				/// ������ֵ�������ɫ
		CGUID	PlayerID;				/// ���ID
		long	lIconID;				/// ְҵͼ��
		long	lSex;					/// �Ա�
		DWORD	dwMaxHp;				/// ���HP
		DWORD	dwMaxMp;				/// ���MP
		DWORD	dwMaxRp;				/// ���Rp
		DWORD	dwHp;					/// HP
		DWORD	dwMp;					/// MP
		DWORD	dwRp;					/// Rp
		long	lLev;					/// �ȼ�
		long	lRegionID;				/// ����ID
		CGUID	RegionGuid;				/// ����GUID
		float	fPosX;					/// ����
		float	fPosY;
		bool	bIsOnline;				/// �Ƿ����߱�־
		vector<tagState>	m_vectorState;		/// Buff״̬�б�
		///���캯��
		tagTeam()
		{
			strPlayerName = "";
			lNameColor = 0xff000000;
			lIconID = -1;
			dwMaxHp = 0;						// HP
			dwMaxMp = 0;						// MP
			dwMaxRp = 0;						// Rp
			dwHp = 0;
			dwMp = 0;
			dwRp = 0;
			lLev = 0;
			lRegionID = 0;
			RegionGuid = CGUID::GUID_INVALID;
			fPosX = 0.0f;
			fPosY = 0.0f;
			bIsOnline = true;
		};
	};
public:
	///���캯��
	TeamData();
	///��������
	~TeamData();
	///֪ͨ�������������ʾ
	void FireUIUpdate();	
	///�������
	void Clear();							
	/// ��ӱ�����������
	void AddNameBySrc(std::string& strName);
	///
	void AddGUIDSrc(CGUID guid);
	///
	void UpdateHeadPic(tagTeam sTeam,long index);
	/// ���ö���ӳ�ͷ��
	void SetTeamCapPic(bool bIsCap = false);								
	bool UpdateTeamList();													/// ����״̬�б�
	bool RemoveTeamMember(const CGUID& MemberID);							/// ��ָ���Ķ��ѴӶ�����ɾ��

	void SetMemberInfo(const CGUID& MemberID, tagMemberIdioinfo sMemberinfo); /// Ϊָ���Ķ�Ա�趨�����Ϣ
	void UpdateMemberPos(const CGUID& MemberID, float fPosX,float fPosY);	/// ���¶��ѵ�����
	void UpdateMemberInfo(const CGUID& MemberID, long lType, long lValue);	/// ����ָ����ҵ�ָ�����Ե�ֵ
	void SetMemberIsOnline(const CGUID& PlayerID, long lState);				/// ����ָ�����ѵ�����״̬
	void SetTeamAllotMode(int iAllotMode);									/// ���ո����Ķ�����Ʒ���䷽ʽ��������Ϣ

	void AddMemberState(const CGUID& PlayerID, DWORD dwState, DWORD dwLev);	/// Ϊָ���Ķ������״̬
	void RemoveMemberState(const CGUID& PlayerID, DWORD dwState);			/// Ϊָ���Ķ���ɾ��ĳ��״̬

	bool ChangeCaptain(const CGUID& CapID);									/// �ı����Ķӳ�

	void MemberChangeServer(const CGUID& MemID);							/// ָ��ID�Ķ�Ա���

	tagTeam * GetMemberInfo(const CGUID& MemID);							/// ���ָ������ڶ����б������Ϣ
	vector<tagTeam> & GetTeamInfo() { return m_vectorTeam; }				/// �����Ҷ�����Ϣ

	void SetSelMember(uint index) { CurSelMemberIndex = index ; }			/// ���õ�ǰѡ�еĳ�Ա
	uint GetSelMember() { return CurSelMemberIndex;	}						/// ��õ�ǰѡ�еĳ�Ա
	const CGUID& GetSelMemberGUID();										/// ��õ�ǰѡ�еĳ�ԱGUID
	bool AddTeamMember(tagWSMemberInfo& sWSMemberInfo);						/// ���ݶ�����Ϣ��ӳ�Ա
private:
	
	vector<tagTeam>	m_vectorTeam;		/// team�б�
	vector<CGUID> m_vecIDSrc;			/// �����˵�GUID�б�
	vector<std::string> m_vecSrcName;	/// ���Ƽ��˵������б�

	uint CurSelMemberIndex;				///��ǰ��ѡ�еĳ�Ա����
};

///�������Ϣ����
class TeamMsgMgr
{
public:
	TeamMsgMgr(){}
	~TeamMsgMgr(){}

public:
	///
	static void  SendInviteJoinTeam(CPlayer* first,CPlayer* second);
	///
	static void  SendLeaveTeam(CPlayer* first);
	///
	static void  SendLeaveTeam(const CGUID& guid);
	///
	static void  SendSelfLeaveTeam();
	///
	static void  SendInviteJoinPhase(CPlayer* first,CPlayer* second);
	///
	static void  SendAddLinkMan(CPlayer* first,CPlayer* second);
	///
	static void  SendFollowLeader(CPlayer* first);
	///
	static void  SendSetCaptain(const CGUID& guid);
};