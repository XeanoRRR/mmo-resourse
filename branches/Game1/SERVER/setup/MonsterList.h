#pragma once

//class CMonster;
#include "..\..\PUBLIC\PlayerDef.h"
#include "..\..\PUBLIC\statedef.h"
#include "..\..\public\tinyxml.h"
// �������Ժ͵�����Ʒȫ�ֻ�����
const int DROPINDEX_NULL=-1;
const int DROPINDEX_ERRO=-2;

class CGoods;

class CMonsterList
{
public:
	CMonsterList(void);
	virtual ~CMonsterList(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:

	// ����
	struct tagSkill
	{
		WORD	wID;					//	���
		WORD	wLevel;					//	�ȼ�
		WORD	wOdds;					//	�ͷŵļ���
	};
	//�������ýṹ
	struct tagSkillInfo
	{
		tagSkillInfo():dwSkillID(0),dwSkillLevel(0),dwCoolDownTime(0),iActModifyTime(0){}
		DWORD dwSkillID;			//����id
		DWORD dwSkillLevel;			//���ܵȼ�
		DWORD dwCoolDownTime;		//��ȴʱ��
		int   iActModifyTime;		//��������
	};

	// ��������
	struct tagMonster
	{
		DWORD		dwIndex;
		char		cOriginalName[50];     //ԭʼ��
		char		cName[50];             //����
		BYTE		byBlockType;			//�赲����
		BYTE		byIsunbeatable;			//�Ƿ����޵�״̬
		BYTE		DropFlag;				//������־��0:Ĭ�ϣ�1:ֻ��DropList.xmlӰ��
		char		cCollectionName[50];	//��Ӧ�ɼ�������
		char		cTitleName[50];			//��ν����
		WORD		dwTitlePicID;			//��νͼƬid
		WORD		dwTitleColor;			//��ν��ʾ��ɫ
		BYTE		byCountry;				//����ID
		eNpcType	eNType;				//Npc��� (NNPC,Monster,Guard)
		WORD		wBossLevel;				//boss�ȼ�
		DWORD		dwPicID;				//ͼ��ID
		DWORD		dwPicLevel;				//ͼ�εȼ�
		DWORD		dwNameColor;			//������ɫ���,��Ӧ���ͻ�������]
		DWORD		dwColorOfLevel;         //�ȼ���ɫ
		DWORD		dwHpBarColor;			//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
		DWORD		dwSoundID;				//����ID
    	BYTE		byTamable;				//�Ƿ����ѱ��
		DWORD		dwMaxTameCount;			//�����ѱ������
		WORD		wFigure;				//����(0:1x1 1:3*3 2:5*5)*
		WORD		wLevel;				    //�ȼ�
		DWORD		dwExp;					//����ֵ*
		DWORD		dwOccuPoint;			//ְҵ����
		DWORD		dwMaxHP;                //���hp
		WORD		wMinAtk;				//��С������		    
		WORD		wMaxAtk;				//��󹥻���
		WORD		wMinMAtk;				//��С����������
		WORD		wMaxMAtk;				//�����������
		WORD		wHit;					//��������
		long		lDef;					//������
		long		lMDef;					//ħ��������
		WORD		wBlock;					//��
		WORD		wParry;					//�м�
		long		lImmunity;				//����
		WORD		wPierce;				//��͸
		WORD		wDodge;					//��������
		float		fMoveSpeed;				//�ƶ��ٶ�
		float		fFightMoveSpeed;		//ս���ƶ��ٶ�
		float       fTurningSpeed;			//ת���ٶ�
		long		lWillDef;				//��־����
		long		lElemDef[ET_Max];		//����Ԫ�ؿ���
		WORD		wHpRecoverSpeed;		//�����ָ��ٶ�
		WORD		wMPRecoverSpeed;		//ħ���ָ��ٶ�
		WORD		lBaseConstiDef;			//���ʿ���
		long		lDeBuffDef[DET_Max];	//Debuff����

		WORD		wMinStopTimes;			//����ͣ������
		WORD		wMaxStopTimes;			//���ͣ������
		WORD		wMinRunTimes;			//�����ƶ�����
		WORD		wMaxRunTimes;			//����ƶ�����

		WORD		wGuardRange;			//���䷶Χ*
		WORD		wChaseRange;			//׷����Χ*
		WORD		wPeaceMoveRange;		//��ƽ�ƶ���Χ
		WORD        wAIModel;               //AIģʽ
		WORD		wCanTalk;				//�ɷ�Ի�
		eClass		eClass_;				//��������
		eEleType	eEType;				//Ԫ�����
		WORD		wStopTime;				//ͣ����ʱ��(��һ�������м�ͣ��֡��)
		WORD		wAIHangupType;			//AI����ʽ
		WORD		wClassDam[C_Max];		//�Ը��������˺�
		WORD		wElemDam[ET_Max];		//�Ը���Ԫ���˺�
		WORD		wMoveRandomValue;		//�����ƶ��ĸ���(����)
		WORD		wCri;					//��������
		WORD		wCriDamg;				//�������˺�
		long		lCriDef;				//����������
		WORD		wMCri;					//ħ��������
		WORD		wMCriDamg;				//ħ�������˺�
		long		lMCriDef;				//ħ����������
		//..............add ....5/16.......
		float       fHurtProportion;        //���˱���
		DWORD       dwGuardTime;            //����ʱ��
		float       fEscapePoint;           //���ܵ��ߣ����ܵ㣩
        WORD        wSkillProportion;       //���ܸ���,һ��С�ڵ���100�����ڵ���0�����͡�
		DWORD       dwSkillSpace;           //���ܼ��
		BYTE		byIsFlyToPeace;			//�Ƿ���ת��Ϊ��ƽ��ʱ��ɻ�ȥ
		BYTE		byIsResumeAll;			//�Ƿ���ת��Ϊ��ƽ��ʱ����ȫ�ָ�
		WORD		wReturnRange;			//��λ����
		DWORD		dwChaseTime;			//׷��ʱ�䣬��׷����Χ���û�ܵ�����ת��Ϊ��ƽ��ʱ��
		DWORD       dwAIReferrence[MONSTER_AI_REFERRENCE_NUM];      //AI�ο�

		//��������
		tagSkillInfo BaseSkillList[MONSTER_SKILL_SUM];
		tagSkillInfo CommonSkillList[MONSTER_SKILL_SUM];
		tagSkillInfo CycleSkillList[MONSTER_SKILL_SUM];

		char		cBornScriptName[MAX_PATH];
		char        cFightScriptName[MAX_PATH];
		char        cGBHScriptName[MAX_PATH];
		char		cDeadScriptName[MAX_PATH];
		char		cCycleScriptName[MAX_PATH];
		char		cSearchEnemyScriptName[MAX_PATH];
		char		cFindTargetScriptName[MAX_PATH];
		char		cKillTargetScriptName[MAX_PATH];

		tagPlayerElementProperty	m_arrElementProperty[MT_Max];	//Ԫ������
		//end add..........................
	};

	// �����Ӧ�ĵ�����Ʒ����
	struct tagDropGoods
	{
		struct tagDrop
		{
			long lGoodsIndex;			// ��Ʒ���
			long lOdds;					// ���伸��
			long lMaxOdds;				// ���ֵ
			long lMinMoney;				// ����СǮ
			long lMaxMoney;				// �����Ǯ
			long lMinLevel;				// ������Ʒ����С�ȼ�  0��ʾԭʼ��Ʒ
			long lMaxLevel;				// ������Ʒ�����ȼ�(������)
			float fLevelAttenuation;		// �ȼ���˥�� (���ÿ���ڹ���1�������伸�ʽ���һ������)
			float fLevelAttenuationLimit;	// �ȼ���˥������

			string strName;				// ��Ʒ����
		};
		// add by chenaly 07/6/21
		struct tagOneGoods 
		{
			long	lGoodsIndex;
			long	lProbability;		//Ȩֵ��������أ�
			long	lMinLevel;			// ������Ʒ����С�ȼ�  0��ʾԭʼ��Ʒ
			long	lMaxLevel;			// ������Ʒ�����ȼ�(������)
			string	strName;			//��Ʒ����
		};
		struct tagFixedDrop 
		{
			INT iMaxDropNum;			//��������
			INT	iMinDropNum;			//��С������
			long lOdds;					//������伸��
			long lMaxOdds;				//������������
			float fLevelAttenuation;	//����ȼ�˥������
			float fLevelAttenuationLimit;
			vector<tagOneGoods>	vecOneDropGroup;		//һ�������������Ʒ
		};
		//end change...

		//string strMonsterOrginName;		// ����ԭʼ��
		vector<tagDrop>	vectorDrop;		// �����б�
		vector<tagFixedDrop> vecFixedDrop;//�̶�����Ʒ����
	};

private:

	static	map<string, tagMonster>		m_mapMonsterList;		// ԭʼMonster�б�(strOrginName, tagMonster)
	//static	map<string, tagDropGoods>	m_mapDropGoods;			// ����Ʒ����(strMonsterOrginName, tagDropGoods)
	//static  vector<vector<POINT>>		m_vecRoadsList;			// ·����
	struct  tgMonsterDrop
	{
		tgMonsterDrop(){ShareDropIndex=DROPINDEX_NULL;OwnDropIndex=DROPINDEX_NULL;ActivityIndex=DROPINDEX_NULL;}
		INT ShareDropIndex;
		INT OwnDropIndex; 
		INT ActivityIndex;
	};
	static vector<tagDropGoods*>		m_vecDropList;					//drop�Ĺ���
	static map<string , tgMonsterDrop>	m_mapMonsterDrop;				//monster����
	static map<int, int>				m_mapRegionDrop;				//region����
	static vector<int>					m_vecLevelDrop;					//�ȼ�����
	static int							m_iWordDrop;		//������䣨������	
public:

//	static BOOL LoadRoadsList(const char * filename);			// ���ļ����·��
//	static BOOL SaveRoadsList(const char * filename);			// ���浽�ļ�
	static BOOL LoadMonsterList(const char* filename);			// ���ļ���ȡԭʼ��Ʒ�б�
	static BOOL SaveMonsterList(const char* filename);			// ���浽�ļ�
	//static bool LoadDropGoodsList(const char* filename);		// ���ļ���ȡ������Ʒ�б�
	//static bool SaveDropGoodsList(const char* filename);		// ���浽�ļ�
	static BOOL LoadMonsterDrop(const char* filename);
	static BOOL LoadActivityDrop(const char* filename);
	static BOOL LoadRegionDrop(const char* filename, long regionID);
private:
	static tagDropGoods* LoadOneDrop(TiXmlElement* p);
	static BOOL GetTwoNum(const char* str, long &first, long &second);
public:
	static bool LoadSkillList(const char* filename);            // ���ļ���ȡ�����б�,*******�˺���Ӧ����LoadMonsterList����֮�����
	static bool SaveSkillList(const char*filename);             // ���漼���б�

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

public:
	static tagMonster* GetProperty(DWORD dwPicID);				// ����ָ����NPC����
	static tagMonster* GetPropertyByName(const char* strName);
	static tagMonster* GetPropertyByOrginName(const char* strName);
	static tagMonster* GetPropertyByOrginIndex(DWORD dwIndex);
	//static bool SetProperty(CMonster* pMonster, const char* strOrginName=NULL);		// ����ָ����NPC����

	//static tagDropGoods* GetDropGoods(const char* strName);		// ����ָ���ĵ���Ʒ��
	// drop_flag : 1:�������ֻ��DropList.xml���ƣ�0:ͬһ������ȡ��ʽ ��CMonsterList::GetPropertyByOrginName(strOrginName)->DropFlag
	static BOOL CheckDropGoods(string MonsterName, INT RegionIndex, INT Level, INT LevelBalances,vector<CGoods*> &pOut, BYTE drop_flag = 0 );
	static BOOL CheckOneDrop(tagDropGoods* pDrop,int LevelBalance, vector<CGoods*> &pOut);
	static bool RefeashAllMonster();							// ˢ��WORLD�е�����Monster����ָ��
public:
	static void Release();
};
