
#pragma once

#include "GUID.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//! ��Ա�Ķ���״̬
enum eMemberState
{
	eMS_ONLINE,		//����
	eMS_LEAVE,		//����
};

//! ��Ʒ���䷽ʽ
enum eGoodsAllot
{
	eGA_FREE,	//����ʰȡ
	eGA_QUEUE,	//����ʰȡ
	eGA_RANDOM, //���ʰȡ
};

//! �������ݸı�
enum eTeamChange
{
	eTC_NewMaster,		//�¶ӳ�
	eTC_NewGoodsAllot,	//�µ���Ʒ�������
	eTC_PlayerLogin,	//��Ա����
	eTC_PlayerLeave,	//��Ա����
};

//! �����������
enum eTeamOptControlType
{
	eTOCT_CantJoin		= 0x1, //! ���ܼ���
	eTOCT_CantExit		= 0x2, //! �����뿪
	eTOCT_CantDisband	= 0x4, //! ���ܽ�ɢ

	eTOCT_CantAll		= 0xFFFFFFFF,//! ȫ����ֹ
};

//! ��Ա������������
#define	MAX_LUCRATIVE_BOUND		21U

//! ��ļ�ı�����󳤶ȣ�������β/0�ַ���
#define	MAX_RECRUIT_TEXT_SIZE	32U
//! ��ļ�����ı�����󳤶ȣ�������β/0�ַ���
#define	MAX_RECRUIT_PWD_SIZE	32U

//! ��ļ״̬
struct tagRecruitState
{
	bool bIsRecruiting;
	char szRecruitName[MAX_RECRUIT_TEXT_SIZE];
	char szPassword[MAX_RECRUIT_PWD_SIZE];
	tagRecruitState()
	{
		bIsRecruiting = false; 
		memset(szRecruitName, 0, MAX_RECRUIT_TEXT_SIZE);
		memset(szPassword, 0, MAX_RECRUIT_PWD_SIZE);
	}
};

//! ��Ա��Ϣ
struct tagWSMemberInfo
{
	CGUID guid;
	long lState;
	long lOccupation;
	long lSex;				//�Ա�
	DWORD dwJoinTime;
	long lCompleteQuest;	//����ɵĶ��鹲�������
	char szName[20];
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//! Ҫ���µķ������Ϣ˵��
enum eUpdateIdioinfo
{
	eMIS_Pos			,//����λ��

	eMIS_CurrHp			,//���µ�ǰHP
	eMIS_CurrMp			,//���µ�ǰMP
	eMIS_CurrEnergy		,//���µ�ǰCPֵ(Ϊ������ͻ��˱������û���޸�ö��ֵ����)

	eMIS_MaxHp			,//�������HP
	eMIS_MaxMp			,//�������HP
	eMIS_MaxEnergy		,//������CPֵ(Ϊ������ͻ��˱������û���޸�ö��ֵ����)

	eMIS_Level			,//���µȼ�
	eMIS_Occupation		,//����ְҵ
	eMIS_Sex			,//�Ա�

	eMIS_State			,//����״̬
};

//! �Ƕ����Ա��Ϣ��������Ϣ���ͣ�
struct tagMemberIdioinfo
{
	float fPosX;
	float fPosY;
	long lRegionID;			//���ڵ�ͼID
	CGUID RegionGuid;		//���ڵ�ͼGUID

	long lCurrHp;			//��ǰHP
	long lCurrMp;			//��ǰMP
	long lCurrEnergy;		//��ǰ����

	long lMaxHp;			//���HP
	long lMaxMp;			//���HP
	long lMaxEnergy;		//�������

	long lLevel;			//��ǰ�ȼ�
	long lOccupation;		//ְҵ
	tagMemberIdioinfo()
	{
		memset(this,0,sizeof(tagMemberIdioinfo));
	}
};


//! ս��ϵͳ����ֵ
enum eSentaiErrInfo
{
	eSEI_NoTeam,			//! �㻹û�����
	eSEI_NotLeader,			//! �㲻�Ƕӳ�
	eSEI_IsIn,				//! �����Ѿ��μ�ս���Ŷ���
	eSEI_NotJoin,			//! ���ǻ�û�вμ�ս���Ŷ�
	eSEI_AllOnline, 		//! �Ŷ�ǰӦ��֤���ж�Ա����
	eSEI_Entering,			//! ���ڽ�������ʱ���������˲���
	eSEI_Battle,			//! ����ս���У��������˲���
	eSEI_MemberNumErr,		//! ��Ա������Ҫ��
	eSEI_MemberLvlRrr,		//! ��Ա�ȼ�����Ҫ��
	eSEI_Waiting,			//! �Ѿ���ʼ����������ԣ��������˲���
	eSEI_NotInSameRegion,	//! ��Ա����ͬһ�ŵ�ͼ
};