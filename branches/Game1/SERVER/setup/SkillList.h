#pragma once

class CSkill;

class CSkillList
{
public:
	CSkillList(void);
	virtual ~CSkillList(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:
	//struct tagSkill
	//{
	//	DWORD	dwIndex;			//	���ܱ��
	//	DWORD	dwActionID;			//	����ͼ��ID
	//	DWORD	dwCastEffectID;		//	ʩ�ż��ܵ���Ч
	//	DWORD	dwHitEffectID;		//	������Ч
	//	DWORD	dwRange;			//	��Χ
	//	DWORD	dwObject;			//	Ŀ��
	//	DWORD	dwCastTimePlus;		//	����ʱ������
	//	DWORD	dwCastTimeMul;		//	����ʱ��ϵ��
	//	DWORD	dwDelayTime;		//	��ֱʱ��
	//	DWORD	dwCoolDownTime;		//	��ȴʱ��
	//	DWORD	dwInterrupt;		//	���
	//	DWORD	dwCostHp;			//	����Hp
	//	DWORD	dwCostHpPer;		//	����Hp��
	//	DWORD	dwCostMp;			//	����Mp
	//	DWORD	dwCostMpPer;		//	����Mp��
	//	DWORD	dwCostYao;			//	��������

	//	//	�ɱ����
	//	long	lParameter01;		//	����1
	//	long	lParameter02;		//	����2
	//	long	lParameter03;		//	����3
	//	long	lParameter04;		//	����4
	//	long	lParameter05;		//	����5
	//	long	lParameter06;		//	����6
	//	long	lParameter07;		//	����7
	//	long	lParameter08;		//	����8
	//	long	lParameter09;		//	����9
	//	long	lParameter10;		//	����10

	//	string	strName;			//	��������
	//	string	strDescribe;		//	����
	//};

	// ����Ŀ��
	enum eSkillDestType
	{
		SDT_SELF,				// �Լ�
		SDT_SHAPE,				// ����shape
		SDT_TILE,				// ��ͼ����
	};

	struct tagStep
	{
		WORD	wCommandID;			// ������
		BYTE	bStringParam;		// �Ƿ����ַ�������
		union{
			long	lParam;				// ���β���
			char	*pstrParam;			// �ַ�������
		};
	};

	struct tagLevel
	{
		WORD	wRange;					//	�ͷŷ�Χ
		vector<tagStep>	vectorSteps;	//	�����б�
	};

	struct tagSkill
	{
		DWORD	dwIndex;				//	���ܱ��
		string	strName;				//	��������
		string	strDescribe;			//	����
		eSkillDestType	lDestType;		//	Ŀ������

		vector<tagLevel>	vectorLevels;	//	�����ȼ�����ϸ����
	};

private:

	static	map<DWORD, tagSkill>		m_mapSkillList;		// ԭʼSkill�б�(dwIndex, tagSkill)
	static	map<string, WORD>			m_mapStepNameID;	// �������ƶ�Ӧ��ID

public:

	static BOOL LoadSkillList(const char* stepfilename, const char* dir_name);			// ��Ŀ¼��ȡԭʼ�����б�
	static BOOL LoadOneSkill(const char* filename);				// ��ȡһ������
	static void Release();

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

public:
	static tagSkill* GetPropertiy(DWORD dwIndex);				// ����ָ����Skill����
	static tagSkill* GetPropertiyByName(const char* strName);
	static bool SetPropertiy(CSkill* pSkill, const char* strOrginName=NULL);		// ����ָ��Skill������
};
