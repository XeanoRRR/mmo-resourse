
#pragma once

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
	long lCompleteQuest;	//����ɵĶ��鹲�������     
    ulong dwJoinTime;
	char szName[20];
};

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
	
	CGUID RegionGuid;		//���ڵ�ͼGUID
    long lRegionID;			//���ڵ�ͼID

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