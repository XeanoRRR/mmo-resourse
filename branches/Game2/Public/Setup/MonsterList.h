//======================================================================================
/**
*	@file		MonsterList.h
*	@data		
*	@author		
*/
//======================================================================================
#pragma once

#include "../Common/StateDef.h"
#include "../Common/PlayerDef.h"
#include "../Common/MonsterDef.h"

// �������Ժ͵�����Ʒȫ�ֻ�����
const int DROPINDEX_NULL = -1;
const int DROPINDEX_ERRO = -2;

/**
*	@class CMonsterList
*
*	@brief �����б�������
*/
class CMonsterList
{
public:
	/// ����
	CMonsterList(void);

	/// ����
	~CMonsterList(void);

public:

	/// ���ܽṹ
	struct tagSkill
	{
		WORD	wID;					///<	���
		WORD	wLevel;					///<	�ȼ�
		WORD	wOdds;					///<	�ͷŵļ���
	};
	/// �������ýṹ
	struct tagSkillInfo
	{
		tagSkillInfo():dwSkillID(0),dwSkillLevel(0),dwCoolDownTime(0),iActModifyTime(0){}
		DWORD dwSkillID;			///< ����id
		DWORD dwSkillLevel;			///< ���ܵȼ�
		DWORD dwCoolDownTime;		///< ��ȴʱ��
		int   iActModifyTime;		///< ��������
	};

	/// ��������
	struct tagMonster
	{
		DWORD		dwIndex;				///< ���
		char		cOriginalName[50];		///< ԭʼ��
		char		cName[50];				///< ����
		uchar		byBlockType;			///< �赲����
		uchar		byIsunbeatable;			///< �Ƿ����޵�״̬
		uchar		DropFlag;				///< ������־��0:Ĭ�ϣ�1:ֻ��DropList.xmlӰ��
		char		cCollectionName[50];	///< ��Ӧ�ɼ�������
		char		cTitleName[50];			///< ��ν����
		WORD		dwTitleColor;			///< ��ν��ʾ��ɫ
		WORD		dwTitlePicID;			///< ��νͼƬid
		uchar		byCountry;				///< ����ID
		eNpcType	eNType;					///< Npc��� (NNPC,Monster,Guard)
		WORD		wBossLevel;				///< boss�ȼ�
		DWORD		dwPicID;				///< ͼ��ID
		DWORD		dwPicLevel;				///< ͼ�εȼ�
		DWORD		dwNameColor;			///< ������ɫ���,��Ӧ���ͻ�������
		DWORD		dwColorOfLevel;         ///< �ȼ���ɫ
		DWORD		dwHpBarColor;			///< Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
		DWORD		dwSoundID;				///< ����ID
    	uchar		byCatchable;			///< �Ƿ���Բ���
		DWORD		dwMaxCatchFastness;		///< ��󲶻���
		ulong		dwServantID;			///< ��Ӧ�̴�ID
		WORD		wFigure;				///< ����(0:1x1 1:3*3 2:5*5)*
		WORD		wLevel;				    ///< �ȼ�
		DWORD		dwExp;					///< ����ֵ
		DWORD		dwDExp;					///< ��Ϊ����ֵ
		DWORD		dwOccuPoint;			///< ְҵ����
		DWORD		dwMaxHP;                ///< ���hp
		WORD		wMinAtk;				///< ��С������		    
		WORD		wMaxAtk;				///< ��󹥻���
		WORD		wMinMAtk;				///< ��С����������
		WORD		wMaxMAtk;				///< �����������
		WORD		wHit;					///< ��������
		WORD		wDef;					///< ������
		WORD		wMDef;					///< ħ��������
		WORD		wBlock;					///< ��
		WORD		wParry;					///< �м�
		WORD		wImmunity;				///< ����
		WORD		wPierce;				///< ��͸
		WORD		wDodge;					///< ��������
		float		fMoveSpeed;				///< �ƶ��ٶ�
		float		fFightMoveSpeed;		///< ս���ƶ��ٶ�
		float       fTurningSpeed;			///< ת���ٶ�
		WORD		wWillDef;				///< ��־����
		WORD		wElemDef[ET_Max];		///< ����Ԫ�ؿ���
		WORD		wHpRecoverSpeed;		///< �����ָ��ٶ�
		WORD		wMPRecoverSpeed;		///< ħ���ָ��ٶ�
		WORD		wBaseConstiDef;			///< ���ʿ���
		WORD		wDeBuffDef[DET_Max];	///< Debuff����

		WORD		wMinStopTimes;			///< ����ͣ������
		WORD		wMaxStopTimes;			///< ���ͣ������
		WORD		wMinRunTimes;			///< �����ƶ�����
		WORD		wMaxRunTimes;			///< ����ƶ�����

		DisType		GuardRange;			///< ���䷶Χ*
		DisType		ChaseRange;			///< ׷����Χ*
		DisType		PeaceMoveRange;		///< ��ƽ�ƶ���Χ
		WORD        wAIModel;               ///< AIģʽ
		WORD		wCanTalk;				///< �ɷ�Ի�
		eClass		eClass_;				///< ��������
		eEleType	eEType;					///< Ԫ�����
		WORD		wStopTime;				///< ͣ����ʱ��(��һ�������м�ͣ��֡��)
		WORD		wAIHangupType;			///< AI����ʽ
		WORD		wClassDam[C_Max];		///< �Ը��������˺�
		WORD		wElemDam[ET_Max];		///< �Ը���Ԫ���˺�
		WORD		wMoveRandomValue;		///< �����ƶ��ĸ���(����)
		WORD		wCri;					///< ��������
		WORD		wCriDamg;				///< �������˺�
		WORD		wCriDef;				///< ����������
		WORD		wMCri;					///< ħ��������
		WORD		wMCriDamg;				///< ħ�������˺�
		WORD		wMCriDef;				///< ħ����������
		float       fHurtProportion;        ///< ���˱���
		DWORD       dwGuardTime;            ///< ����ʱ��
		float       fEscapePoint;           ///< ���ܵ��ߣ����ܵ㣩
        WORD        wSkillProportion;       ///< ���ܸ���,һ��С�ڵ���100�����ڵ���0�����͡�
		DWORD       dwSkillSpace;           ///< ���ܼ��
		uchar		byIsFlyToPeace;			///< �Ƿ���ת��Ϊ��ƽ��ʱ��ɻ�ȥ
		uchar		byIsResumeAll;			///< �Ƿ���ת��Ϊ��ƽ��ʱ����ȫ�ָ�
		DisType		ReturnRange;			///< ��λ����
		DWORD		dwChaseTime;			///< ׷��ʱ�䣬��׷����Χ���û�ܵ�����ת��Ϊ��ƽ��ʱ��
		DWORD       dwAIReferrence[MONSTER_AI_REFERRENCE_NUM];      ///< AI�ο�

		//��������
		tagSkillInfo BaseSkillList[MONSTER_SKILL_SUM];
		tagSkillInfo CommonSkillList[MONSTER_SKILL_SUM];
		tagSkillInfo CycleSkillList[MONSTER_SKILL_SUM];

		char		cBornScriptName[MAX_PATH];				///< �����ű�
		char        cFightScriptName[MAX_PATH];				///< ս���ű�
		char        cGBHScriptName[MAX_PATH];				///< ���˽ű�
		char		cDeadScriptName[MAX_PATH];				///< �����ű�
		char		cCycleScriptName[MAX_PATH];				///< ���ڽű�
		char		cSearchEnemyScriptName[MAX_PATH];		///< ѡ���ܽű�
		char		cFindTargetScriptName[MAX_PATH];		///< Ѱ��Ŀ��ű�
		char		cKillTargetScriptName[MAX_PATH];		///< ɱ��Ŀ��ű�
		char		cCatchScriptName[MAX_PATH];				///< ����ű�

		tagPlayerElementProperty	m_arrElementProperty[MT_Max];	///< Ԫ������
        int tauntSize; ///< ����б���󳤶�
	};

	/// �����Ӧ�ĵ�����Ʒ����
	struct tagDropGoods
	{
		struct tagDrop
		{
			long lGoodsIndex;			///< ��Ʒ���
			long lOdds;					///< ���伸��
			long lMaxOdds;				///< ���ֵ
			long lMinMoney;				///< ����СǮ
			long lMaxMoney;				///< �����Ǯ
			long lMinLevel;				///< ������Ʒ����С�ȼ�  0��ʾԭʼ��Ʒ
			long lMaxLevel;				///< ������Ʒ�����ȼ�(������)
			float fLevelAttenuation;		///< �ȼ���˥�� (���ÿ���ڹ���1�������伸�ʽ���һ������)
			float fLevelAttenuationLimit;	///< �ȼ���˥������

			string strName;				///< ��Ʒ����
		};
		struct tagOneGoods 
		{
			long	lGoodsIndex;
			long	lProbability;		///< Ȩֵ��������أ�
			long	lMinLevel;			///< ������Ʒ����С�ȼ�  0��ʾԭʼ��Ʒ
			long	lMaxLevel;			///< ������Ʒ�����ȼ�(������)
			string	strName;			///< ��Ʒ����
		};
		struct tagFixedDrop 
		{
			INT iMaxDropNum;			///< ��������
			INT	iMinDropNum;			///< ��С������
			long lOdds;					///< ������伸��
			long lMaxOdds;				///< ������������
			float fLevelAttenuation;	///< ����ȼ�˥������
			float fLevelAttenuationLimit;
			vector<tagOneGoods>	vecOneDropGroup;		///< һ�������������Ʒ
		};


		vector<tagDrop>	vectorDrop;		///< �����б�
		vector<tagFixedDrop> vecFixedDrop;///< �̶�����Ʒ����
	};

	/// �������ṹ
	struct  tgMonsterDrop
	{
		tgMonsterDrop(){ShareDropIndex=DROPINDEX_NULL;OwnDropIndex=DROPINDEX_NULL;ActivityIndex=DROPINDEX_NULL;}
		INT ShareDropIndex;
		INT OwnDropIndex; 
		INT ActivityIndex;
	};
private:
	/// ԭʼMonster�б�(strOrginName, tagMonster)
	static	map<string, tagMonster>		m_mapMonsterList;

	/// drop�Ĺ���
	static vector<tagDropGoods*>		m_vecDropList;

	/// monster����
	static map<string , tgMonsterDrop>	m_mapMonsterDrop;

	/// region����
	static map<int, int>				m_mapRegionDrop;

	/// �ȼ�����
	static vector<int>					m_vecLevelDrop;

	/// ������䣨������			
	static int							m_iWordDrop;			
public:
	/// ���ļ����ع����б�
	static BOOL LoadMonsterList(const char* filename);

	/// ���ļ���ȡ�����б�,*******�˺���Ӧ����LoadMonsterList����֮�����
	static bool LoadSkillList(const char* filename);

	/// ���ļ����ع����������
	static BOOL LoadMonsterDrop(const char* filename);

	/// ���ļ����ػ��������
	static BOOL LoadActivityDrop(const char* filename);

	/// ���ļ����س�����������
	static BOOL LoadRegionDrop(const char* filename, long regionID);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ����
	static void Release();

private:
	/// ����һ����������
	static tagDropGoods* LoadOneDrop(TiXmlElement* p);

	/// 
	static BOOL GetTwoNum(const char* str, long &first, long &second);

public:
	/// ����ͼ��ID����ָ����NPC����
	static tagMonster* GetProperty(DWORD dwPicID);

	/// �������Ʒ���ָ����NPC����
	static tagMonster* GetPropertyByName(const char* strName);

	/// ����ԭʼ���Ʒ���ָ����NPC����
	static tagMonster* GetPropertyByOrginName(const char* strName);

	/// ����ID����ָ����NPC����
	static tagMonster* GetPropertyByOrginIndex(DWORD dwIndex);

	/// ȡ�ù�����伯��
	static map<string , tgMonsterDrop>* GetMonsterDropList();

	/// ȡ��Drop������
	static vector<tagDropGoods*>* GetDropGoodsList();

	/// ȡ��region���伯��
	static map<int, int>* GetRegionDropList();

	/// ȡ�õȼ����伯��
	static vector<int>* GetLevelDropList();

	/// ȡ��������䣨������
	static int GetWorldDropIdx();

};
