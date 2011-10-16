
#pragma once
#include "..\..\public\BaseDef.h"
#include "..\..\public\tinystr.h"
#include "..\..\public\tinyxml.h"


//enum ROLE_TYPE{MOSTLY_ROLE=1,BAYOU_ROLE,ENGAGEARMS_ROLE};//�������ͣ����ߣ�֧�ߣ�Ӷ����

const int LABELPREFIX_NUM=5;							//ǰ����������

enum AIM_MODEL_TYPE{AMT_KILL=0,AMT_COLLECT,AMT_TALK,AMT_ESCORT,AMT_KILL_COLLECT};//Ŀ��ģʽ����

const LONG	PREPARE_PARAM_NUM=10;						//�󱸱�������aim��
const LONG  STEP_AIM_NUM=10;							//����Ŀ���������
const LONG  STEP_ENCOURAGEMENT_NUM=10;					//�����ȡ��Ʒ�������
const LONG  QUEST_STEP_NUM=20;							//�����������
const LONG  QUEST_ENCOURAGEMENT_NUM=20;					//�������������

const LONG  GOODS_TYPE_INDEX=1;							//���������е���Ʒ���ͱ�ʶ

const int QUEST_MAX_ODDS = 20;						//��������ϡ�ж�ֵ
const int MAX_MERC_QUEST_NUM = 4;					//�����б�ID������ٸ����������
//��������
class CQuestAttribute
{
public:
	CQuestAttribute();
	~CQuestAttribute(){}
	BOOL Init(TiXmlElement*);
public:
//ȡ��ֵ�Ľӿ�
	//ȡ�����������
	const char* GetName()const{return m_cName;}
	//ȡ������id
	LONG GetID()const{return m_lID;}
	//�Ƿ񱻼�¼
	LONG GetBeRecord()const{return m_bBeRecord;}
	//��������
	LONG GetRoleType()const{return m_lRoleType;}
	//�������npc��id
	LONG GetEmbracerNPCID()const{return m_lEmbracerNPCID;}
	//�Ƿ���ʾ
	LONG GetShowAble()const{return m_bShowAble;}
	//�Ƿ��ܷ�����
	LONG GetRepeated()const{return m_bRepeated;}
	//�����Ƿ�ʧ��
	LONG GetDeadLost()const{return m_bDeadLost;}
	//����ʧ�ܺ��ܷ����½�
	LONG GetLostRepeated()const{return m_bLostRepeated;}
	//��С�ȼ�����
	LONG GetMinGrade()const{return m_lMinGrade;}
	//���ȼ�����
	LONG GetMaxGrade()const{return m_lMaxGrade;}
	//�Ƿ��ܾ��鱶��Ӱ��
	LONG GetExpRatio()const{return m_bExpRatio;}
	//ְҵ����
	BOOL GetOccupationRestrict(INT index)const
	{
		if (index>=0 && index< OCC_Max)
		{
			return (m_Occupation[index/32] & (1<<(index%32)))==0 ? 0:1 ;
		}
		else 
			return FALSE;
	}
	//������
	LONG GetOdds()const{return m_lOdds;}
	//��ҪӶ������
	LONG GetEngageArmsRepute()const{return m_lEngageArmsRepute;}
	//��һ�������id
	LONG GetNextRoleID()const{return m_lNextRoleID;}
	//�½���
	LONG GetChapterNum()const{return m_lChapterNum;};
	//
	LONG GetFinishEffect()const{return m_lFinishEffect;}
	//
	const char* GetDeleteScript()const{return m_cDeleteScript;}

	LONG GetMassPoint() const	{return m_lMassPoint;}
private:
	//����   ְҵ����
	VOID ParseOccupation(const char *str);
	int  GetNumInParse(char*&);
private:
	char		m_cName[50];
	LONG		m_lID;							//����id
	LONG		m_lRoleType;					//��������
	LONG		m_lEmbracerNPCID;				//�������npc��id

	BOOL		m_bBeRecord;					//�Ƿ񱻼�¼
	BOOL		m_bShowAble;					//�ܷ���ʾ
	BOOL		m_bRepeated;					//�ܷ񷴸�����
	BOOL		m_bDeadLost;					//��������ʧ��
	BOOL		m_bLostRepeated;				//ʧ���ؽӿ���
	BOOL		m_bExpRatio;					//�Ƿ��ܾ��鱶��Ӱ��

	LONG		m_lMinGrade;					//�ȼ�����
	LONG		m_lMaxGrade;					//�ȼ�����
	int 		m_Occupation[OCC_Max/32 +1];	//ְҵ���� b
	LONG		m_lOdds;						//����ϡ�г̶�
	LONG		m_lEngageArmsRepute;			//��ҪӶ������
	
	LONG		m_lNextRoleID;					//��һ�������id
	LONG		m_lFinishEffect;				//�������Ч��
	LONG		m_lChapterNum;					//�½���
	char        m_cDeleteScript[50];			//ɾ������ű�

	LONG		m_lMassPoint;					//�����
};

//ǰ������
class CQuestLabelPrefix
{
public:
	CQuestLabelPrefix();
	~CQuestLabelPrefix(){}
	BOOL Init(TiXmlElement*);
public:
	//ȡ��index��Ӧ��ǰ������
	LONG GetLabelPrefix(LONG Index)const;
	//����һ��ǰ������id��¼
	BOOL AddLabelPrefix(LONG val);
	//ȡ��ǰ����������
	LONG GetLabelSum()const{return m_lLabelSum;}
private:
	LONG		m_lLabelSum;						//ǰ����������
	LONG		m_lLabelPrefix[LABELPREFIX_NUM];	//ǰ�������б�
};
//����Ŀ��
class CQuestAim
{
public:
	CQuestAim();
	~CQuestAim(){}
	BOOL Init(TiXmlElement*);
public:
//ȡֵ�ӿ�
	//ȡ��id
	LONG GetID()const					{return m_lID;}
	//Ŀ��ģ������
	AIM_MODEL_TYPE	GetAMTType()const	{return m_AMTType;}
	//Ŀ������
	LONG GetAimType()const				{return m_lAimType;}
	//ȡ��Ŀ��id
	LONG GetAimId()const				{return m_lAimID;}
	//ȡ��Ŀ������
	LONG GetAimNum()const				{return m_lAimNum;}
	//ȡ�� ��������
	LONG GetParameIndex()const			{return m_lParamIndex;}
	//����������Ʒ���������ͣ�����Ʒ��
	LONG GetDropFrom()const				{return m_lDropFrom;}
	//����
	LONG GetDropOdds()const				{return m_lDropOdds;}
	//ȡ��Ŀ�������
	const char* GetName()const			{return (char*)&m_cAimName[0];}

	//ȡ�ñ�������
	LONG GetPrepareParam(LONG index)const
	{
		if (index>=0 && index <PREPARE_PARAM_NUM)
		{
			return  m_lPrepareParam[index];
		}
		else
			return 0;
	}

private:
	LONG			m_lID;					//id
	AIM_MODEL_TYPE	m_AMTType;				//ģ������
	LONG			m_lAimType;				//Ŀ������
	char			m_cAimName[50];			//Ŀ������
	LONG			m_lAimID;				//Ŀ��id
	LONG			m_lAimNum;				//Ŀ������
	LONG			m_lParamIndex;			//��������
	LONG			m_lDropFrom;			//�������
	LONG			m_lDropOdds;			//����
	LONG			m_lPrepareParam[PREPARE_PARAM_NUM];		//���������б�
public:
	CQuestAim* next;
};
//������
class CQusetEncouragement
{
public:
	CQusetEncouragement();
	~CQusetEncouragement(){}
	BOOL Init(TiXmlElement*);
public:
	//ȡ�ý���������
	const char* GetName()const{return m_Name;}
	//ȡ�ý�������
	LONG GetType()const{return m_lType;}
	//�����ľ���id
	LONG GetID()const{return m_lID;}
	//��������
	LONG GetNum()const{return m_lNum;}
private:
	char m_Name[50];		//�����������
	LONG m_lType;			//����
	LONG m_lID;				//id
	LONG m_lNum;			//����
public:
	CQusetEncouragement *next;
};
//������
class CQuestStep
{
public:
	CQuestStep();
	~CQuestStep();
	BOOL Init(TiXmlElement*);
	BOOL AddToByteArray(vector<BYTE>* pByteArray);
	BOOL DecordFromByteArray(BYTE* pByte, long& pos);
private:
	struct SQuestText
	{
		LONG TextID;
		LONG TextIndex;
	};
public:
	//
	LONG GetStepAimNum()const{return m_lAimNum;}
	//ȡ�ò���id
	LONG GetStepID()const{return m_lStepID;}
	//ȡ��textid ��Ӧ��text������
	LONG GetTextIndex(LONG lTextID);
	//
	LONG GetRolePropSum()const{return m_lRolePropNum;}
	//ȡ�ý����б��ͷָ��
	const CQusetEncouragement* GetRolePropHead()const{return m_RoleProp;}
	//��ȡ������Ʒ��ռ������
	LONG GetRolePropCell()const{return m_lRolePropCell;}
	//ȡ��Ŀ��
	const CQuestAim* GetAim(LONG ID)const;
	//ȡ�ý����б��ͷָ��
	const CQuestAim* GetAimHead()const{return m_Aim;}
private:
	//����һ��Ŀ��
	BOOL AddAim(CQuestAim*&);
	//����һ�������Ʒ��
	BOOL AddEncouragement(CQusetEncouragement*&);
private:
	LONG				m_lStepID;				//����id
	LONG				m_lRolePropNum;			//����������
	LONG				m_lRolePropCell;	//������ռ������
	CQusetEncouragement*m_RoleProp;				//���轱���б�ͷ
	LONG				m_lAimNum;				//����Ŀ������
	CQuestAim*			m_Aim;					//����Ŀ��ͷָ��
	vector<SQuestText>   m_vecQuestText;		//����Ŀ����ı�
public:
	CQuestStep* next;
};
//��������
class CQuest
{
public:
	CQuest();
	~CQuest();
	BOOL Init(TiXmlElement* ,LONG lTextType);
	BOOL AddToByteArray(vector<BYTE>* pByteArray);
	BOOL DecordFromByteArray(BYTE* pByte, long& pos);
public:
	LONG GetTextType()const{return m_lTextType;}
	LONG GetID()const{return m_Attribute.GetID();}
	const CQuestAttribute& GetAttribute()const{return m_Attribute;}
	const CQuestLabelPrefix& GetLabelPrefix()const{return m_LabelPrefix;}
	LONG GetStepSum()const{return m_lStepSum;}
	const CQuestStep*	GetStep(LONG ID=0x7fffffff)const;
	LONG GetEncouragementSum()const{return m_lEncouragementSum;}
	CQusetEncouragement* GetEncouragementHead()const{return m_Encouragement;}
	LONG GetEncouragementCell()const{return m_lEncouragementCell;}
private:
	BOOL AddStep(CQuestStep *& step);
	BOOL AddEncouragement(CQusetEncouragement *& Encouragement);
private:
	LONG					m_lTextType;				//�ı���������
	CQuestAttribute			m_Attribute;				//����

	CQuestLabelPrefix		m_LabelPrefix;				//ǰ������

	LONG					m_lStepSum;					//�����ܲ�����
	CQuestStep*				m_Step;						//�������б�

	LONG					m_lEncouragementSum;		//�����ܽ�����
	LONG					m_lEncouragementCell;		//��������ռ�ĸ�������
	CQusetEncouragement*	m_Encouragement;			//�������б�
};

//�����б�
class CQuestSystem
{
	CQuestSystem();
	~CQuestSystem();

	struct tagQuestList
	{
		long lTextType;//�ַ���Դ����
		long lTitleTextID;//�����ַ���ԴID
		long lType;	//���������ͣ� -1 ���󣨲����������飩 0 һ�������� 1���������� 2Ӷ�������� 
		DWORD dwFreshTime;//ˢ��ʱ��
		//���б��µ���������
		map<LONG,CQuest*>	Quests;
	};
public:
	static BOOL Init(const char* filename);
	static BOOL InitAttribute(TiXmlElement* p);
	static BOOL InitOneFile(const char* filename,tagQuestList& QuestList);
	static BOOL Relese();
	static BOOL AddToByteArray(vector<BYTE>* pByteArray);
	static BOOL DecordFromByteArray(BYTE* pByte, long& pos);
	//����ӿ�
	static const  CQuest*			GetQuest(LONG ID);
	static		  LONG				GetMainRoleByChapterID(LONG chapterID);
	static const  CQuestStep*		GetStep(LONG ID,LONG stepID);
	static const  CQuestAim*	GetAim(LONG ID,LONG step,LONG aimID);
	static const  CQusetEncouragement* GetEncouragementHead(LONG ID);
	static const  CQusetEncouragement* GetStepEncouragementHead(LONG ID,LONG stepID);
	//
	//static const char* GetPlayerLoginScript()	{return 0;}//m_strPlayerLoginScript.c_str();}	//�õ�����ҵĵ�½�ű�
	//static const char* GetPlayerUpLevelScrip()	{return 0;}//m_strPlayerUpLevelScript.c_str();}	//�õ���ҵ������ű�
	//static const char* GetPlayerDiedScrip()	{return 0;}//m_strPlayerDiedScript.c_str();}		//�õ���������ű�
	static const char* GetCompleteScripById(WORD wQuestID);
	static map<long,long>&		GetMerItem()	{return m_mapMerItem;}

	static long	GetMaxQuestNum()	{return m_lMaxQuestNum;}
	static long GetMercQuestUpdateInter() {return m_lMercQuestUpdateInter;}
	static const char* GetAddQuestScript(){return m_strAddQuestScript.c_str();}
	static const char* GetCompleteQuestScript(){return m_strCompleteQuestScript.c_str();}
	static const char* GetQuestAcceptScript(){return m_strQuestAccept.c_str();}
	static const char* GetMercUpdateScript() {return m_pszUpdateScript;}
	static const char* GetQuestUpdate() {return m_strQuestUpdate.c_str();}
	//��ȡٸ������
	static bool GetMercQuests(long lListID,long lPlayerLvl,long lPlayerOccu,
								long lRankOfMercenary,set<long>& BesidesQuests,
								vector<long>& RequireQuests,long lMaxRequireNum,
								bool bUseGoods=false);
	static long GetQuestListType(long lListID);
	static DWORD GetQuestListFreshTime(long lListID);
	static long GetQuestListID(long lQuestID);
private:
	static BOOL AddQuest(tagQuestList& QuestList, CQuest* quest);
	static BOOL AddMainQuest(LONG index,LONG roleID);
	//�õ������ϡ�ж�
	static long GetRadomOdds(bool bUseGoods = false);
private:
	static long	m_lMaxQuestNum;					//������������
	static long	m_lMercQuestUpdateInter;		//ٸ������ˢ�¼۸�ʱ��
	static char m_pszUpdateScript[MAX_PATH];	//ٸ������ˢ�½ű�
	static long m_lQuestLvelSpan;				//����ȼ����
	static long m_lMaxRandomNum;				//��������
	static long m_lOddsRandomNum[QUEST_MAX_ODDS];//ϡ�ж���ص������ֵ(ϡ�ж����ֵΪ4)
	static long m_lOddsRandomNumUseGoods[QUEST_MAX_ODDS];//ϡ�ж���ص������ֵ(ϡ�ж����ֵΪ4),ʹ�õ���ˢ��ʱ
	static map<long,long> m_mapMerItem;

	static string m_strAddQuestScript;
	static string m_strCompleteQuestScript;
	static string m_strQuestAccept;
	static string m_strQuestUpdate;
	static map<long,vector<CQuest*>>	m_mapQuestsWithOdds[QUEST_MAX_ODDS];//��ϡ�жȷ�����������ݽṹ
	typedef map<LONG,CQuest*>	mapQuests;
	typedef map<LONG,CQuest*>::iterator itQuests;

	typedef map<LONG,tagQuestList> mapQuestList;
	typedef map<LONG,tagQuestList>::iterator itQuestList;

	static mapQuestList	m_mapQuestList;
	static vector<LONG>			m_vecMainQuestList;				//���������б�
};

