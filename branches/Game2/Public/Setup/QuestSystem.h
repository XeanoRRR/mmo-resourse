#pragma once

#include "../Common/BaseDef.h"


//enum ROLE_TYPE{MOSTLY_ROLE=1,BAYOU_ROLE,ENGAGEARMS_ROLE};//�������ͣ����ߣ�֧�ߣ�Ӷ����

const int LABELPREFIX_NUM=5;							//ǰ����������

enum AIM_MODEL_TYPE{AMT_KILL=0,AMT_COLLECT,AMT_TALK,AMT_ESCORT,AMT_KILL_COLLECT};//Ŀ��ģʽ����

const long	PREPARE_PARAM_NUM=10;						//�󱸱�������aim��
const long  STEP_AIM_NUM=10;							//����Ŀ���������
const long  STEP_ENCOURAGEMENT_NUM=10;					//�����ȡ��Ʒ�������
const long  QUEST_STEP_NUM=20;							//�����������
const long  QUEST_ENCOURAGEMENT_NUM=20;					//�������������
const long  GOODS_TYPE_INDEX=1;							//���������е���Ʒ���ͱ�ʶ
const int QUEST_MAX_ODDS = 20;						    //��������ϡ�ж�ֵ
const int MAX_MERC_QUEST_NUM = 4;					    //�����б�ID������ٸ����������

//��������
class CQuestAttribute
{
public:
	CQuestAttribute();
	~CQuestAttribute(){}
	bool Init(TiXmlElement*);
public:
//ȡ��ֵ�Ľӿ�
	//ȡ������id
	long GetID()const{return m_lID;}
	//�Ƿ񱻼�¼
	long GetBeRecord()const{return m_bBeRecord;}
	//��������
	long GetQuestType()const{return m_lQuestType;}
    //�����Ƿ�ɹ���
    long GetQuestIsShare()const{ return m_lShareQuest;}
	//�Ƿ���ʾ
	long GetShowAble()const{return m_bShowAble;}
	//�Ƿ��ܷ�����
	long GetRepeated()const{return m_bRepeated;}
	//�����Ƿ�ʧ��
	long GetDeadLost()const{return m_bDeadLost;}
	//����ʧ�ܺ��ܷ����½�
	long GetLostRepeated()const{return m_bLostRepeated;}
	//��С�ȼ�����
	long GetMinGrade()const{return m_lMinGrade;}
	//���ȼ�����
	long GetMaxGrade()const{return m_lMaxGrade;}
	//ְҵ����
	bool GetOccupationRestrict(INT index)const
	{
		if (index>=0 && index< OCC_Max)
		{
			return (m_Occupation[index/32] & (1<<(index%32)))==0 ? 0:1 ;
		}
		else 
			return false;
	}
	//������
	long GetOdds()const{return m_lOdds;}
	//��һ�������id
	long GetNextRoleID()const{return m_lNextRoleID;}
	//�½���
	long GetChapterNum()const{return m_lChapterNum;};
	// �����Ч
	long GetFinishEffect()const{return m_lFinishEffect;}
	// ����ɾ���ű�
	const char* GetDeleteScript()const{return m_cDeleteScript;}
    // �õ������
	long GetMassPoint() const	{return m_lMassPoint;}
    // �Ա�Ҫ��
    uchar GetQuestSex()const     {return m_QuestSex; }
    // ��ҪЯ������ID
    long GetTotePet()const      {return m_lTotePet; }
    // ����������
    long GetReqSkill()const     {return m_ReqSkill; }
private:

    /// ע��ԭ�н����㷨���������������������ͬ�������� by:expter

	//���� ְҵ����
	void ParseOccupation(const char *str);
	int  GetNumInParse(char*&);

private:   
    bool		m_bBeRecord;					//�Ƿ񱻼�¼
	bool		m_bShowAble;					//�ܷ���ʾ
	bool		m_bRepeated;					//�ܷ񷴸�����
	bool		m_bDeadLost;					//��������ʧ��
	bool		m_bLostRepeated;				//ʧ���ؽӿ���
    uchar        m_QuestSex;                     //�Ա�Ҫ�� 0�У�1Ů, 2��Ҫ��

	long		m_lID;							//����id
	long		m_lQuestType;					//��������
    long        m_lShareQuest;                  //�Ƿ���Թ���	
	long		m_lMinGrade;					//�ȼ�����
	long		m_lMaxGrade;					//�ȼ�����	
	long		m_lOdds;						//����ϡ�г̶�
	long		m_lNextRoleID;					//��һ�������id
	long		m_lFinishEffect;				//�������Ч��
	long		m_lChapterNum;					//�½���	  
    long        m_lTotePet;                     //��ҪЯ������ID
    long        m_ReqSkill;                     //������
	long		m_lMassPoint;					//�����   

    int 		m_Occupation[OCC_Max/32 +1];	//ְҵ���� b  
    char        m_cDeleteScript[64];			//ɾ������ű�
};

//ǰ������
class CQuestLabelPrefix
{
public:
	CQuestLabelPrefix();
	~CQuestLabelPrefix(){}
	bool Init(TiXmlElement*);
public:
	//ȡ��index��Ӧ��ǰ������
	long GetLabelPrefix(long Index)const;
	//����һ��ǰ������id��¼
	bool AddLabelPrefix(long val);
	//ȡ��ǰ����������
	long GetLabelSum()const{return m_lLabelSum;}
private:
	long		m_lLabelSum;						//ǰ����������
	long		m_lLabelPrefix[LABELPREFIX_NUM];	//ǰ�������б�
};
//����Ŀ��
class CQuestAim
{
public:
	CQuestAim();
	~CQuestAim(){}
	bool Init(TiXmlElement*);
public:
//ȡֵ�ӿ�
	//ȡ��id
	long GetID()const					{return m_lID;}
	//Ŀ��ģ������
	AIM_MODEL_TYPE	GetAMTType()const	{return m_AMTType;}
	//Ŀ������
	long GetAimType()const				{return m_lAimType;}
	//ȡ��Ŀ��id
	long GetAimId()const				{return m_lAimID;}
	//ȡ��Ŀ������
	long GetAimNum()const				{return m_lAimNum;}
	//ȡ�� ��������
	long GetParameIndex()const			{return m_lParamIndex;}
	//����������Ʒ���������ͣ�����Ʒ��
	long GetDropFrom()const				{return m_lDropFrom;}
	//����
	long GetDropOdds()const				{return m_lDropOdds;}
	//ȡ��Ŀ�������
	const char* GetName()const			{return (char*)&m_cAimName[0];}

	//ȡ�ñ�������
	long GetPrepareParam(long index)const
	{
		if (index>=0 && index <PREPARE_PARAM_NUM)
		{
			return  m_lPrepareParam[index];
		}
		else
			return 0;
	}

private:
	long			m_lID;					//id
	AIM_MODEL_TYPE	m_AMTType;				//ģ������
	long			m_lAimType;				//Ŀ������
	char			m_cAimName[50];			//Ŀ������
	long			m_lAimID;				//Ŀ��id
	long			m_lAimNum;				//Ŀ������
	long			m_lParamIndex;			//��������
	long			m_lDropFrom;			//�������
	long			m_lDropOdds;			//����
	long			m_lPrepareParam[PREPARE_PARAM_NUM];		//���������б�
public:
	CQuestAim* next;
};
//������
class CQusetEncouragement
{
public:
	CQusetEncouragement();
	~CQusetEncouragement(){}
	bool Init(TiXmlElement*);

    void AddToByteArray(vector<uchar>*& pucharArray);
    void DecodeQuestAward(uchar* puchar, long& pos);
public:
	//ȡ�ý���������
	const char* GetName()const{return m_Name;}
	//ȡ�ý�������
	long GetType()const{return m_lType;}
	//�����ľ���id
	long GetID()const{return m_lID;}
	//��������
	long GetNum()const{return m_lNum;}
private:
	char  m_Name[128];		//�����������
	long  m_lType;			//����
	long  m_lID;			//id
	long  m_lNum;			//����
    float m_fOdds;          //����
public:
	CQusetEncouragement *next;
};
// ����������
class  CQuestEncourMan
{
public:
    struct QuestAward
    {
        long  Sum;
        long  Cell;
        CQusetEncouragement *Encour;
        QuestAward()
        {
            Cell= 0;
            Sum = 0 ;
            Encour = NULL;
        }
    };
    
    CQuestEncourMan()  ;

    ~CQuestEncourMan() ;

    static void  Release();

    static bool  Init(const char* filename);

    static bool  LoadAward(const char* filename);
    
    static bool  DecodeQuestAward(uchar* puchar, long& pos);

    static bool  AddToByteArray(vector<uchar>* pucharArray);

    static long  GetEncourCell( long Qid );

    static long  GetEncourSum( long Qid ) ;

    static CQusetEncouragement* GetEncouragement(long Qid);
private:

    static std::map<long,QuestAward*>  m_Award;

};

//������
class CQuestStep
{
public:
	CQuestStep();
	~CQuestStep();
	bool Init(TiXmlElement*);
	bool AddToByteArray(vector<uchar>* pucharArray);
	bool DecordFromByteArray(uchar* puchar, long& pos);
private:
	struct SQuestText
	{
		long TextID;
		long TextIndex;
	};
public:
	//
	long GetStepAimNum()const{return m_lAimNum;}
	//ȡ�ò���id
	long GetStepID()const{return m_lStepID;}
	//��ȡ������Ʒ��ռ������
	long GetRolePropCell()const{return m_lRolePropCell;}
	//ȡ��Ŀ��
	const CQuestAim* GetAim(long ID)const;
	//ȡ�ý����б��ͷָ��
	const CQuestAim* GetAimHead()const{return m_Aim;}
private:
	//����һ��Ŀ��
	bool AddAim(CQuestAim*&);
private:
	long				m_lStepID;				//����id
	long				m_lRolePropCell;	    //������ռ������
	long				m_lAimNum;				//����Ŀ������
	CQuestAim*			m_Aim;					//����Ŀ��ͷָ��
public:
	CQuestStep* next;
};
//��������
class CQuest
{
public:
	CQuest();
	~CQuest();
	bool Init(TiXmlElement* ,long lTextType);
	bool AddToByteArray(vector<uchar>* pucharArray);
	bool DecordFromByteArray(uchar* puchar, long& pos);
public:
	long GetTextType()const{return m_lTextType;}
	long GetID()const{return m_Attribute.GetID();}
	const CQuestAttribute& GetAttribute()const{return m_Attribute;}
	const CQuestLabelPrefix& GetLabelPrefix()const{return m_LabelPrefix;}
	long GetStepSum()const{return m_lStepSum;}
	const CQuestStep*	GetStep(long ID=0x7fffffff)const;
private:
	bool AddStep(CQuestStep *& step);
	bool AddEncouragement(CQusetEncouragement *& Encouragement);
private:
	long					m_lTextType;				//�ı���������
	CQuestAttribute			m_Attribute;				//����
	CQuestLabelPrefix		m_LabelPrefix;				//ǰ������
	long					m_lStepSum;					//�����ܲ�����
	CQuestStep*				m_Step;						//�������б�};
};
//�����б�
class CQuestSystem
{
	CQuestSystem();
	~CQuestSystem();

	struct tagQuestList
	{
		long lTextType;   //�ַ���Դ����
		long lTitleTextID;//�����ַ���ԴID
		long lType;	      //���������ͣ� -1 ���󣨲����������飩 0 һ�������� 1���������� 2Ӷ�������� 
		ulong dwFreshTime;//ˢ��ʱ��
		//���б��µ���������
		map<long,CQuest*>	Quests;
	};
public:
	static bool Init(const char* filename);
	static bool InitAttribute(TiXmlElement* p);
	static bool InitOneFile(const char* filename,tagQuestList& QuestList);
	static bool Relese();
	static bool AddToByteArray(vector<uchar>* pucharArray);
	static bool DecordFromByteArray(uchar* puchar, long& pos);
	//����ӿ�
	static const  CQuest*			GetQuest(long ID);
	static		  long				GetMainRoleByChapterID(long chapterID);
	static const  CQuestStep*		GetStep(long ID,long stepID);
	static const  CQuestAim*	GetAim(long ID,long step,long aimID);
	//static const  CQusetEncouragement* GetEncouragementHead(long ID);
	//static const  CQusetEncouragement* GetStepEncouragementHead(long ID,long stepID);
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
	static long  GetQuestListType(long lListID);
	static ulong GetQuestListFreshTime(long lListID);
	static long  GetQuestListID(long lQuestID);

    static long  GetEncouragementCell(long lQuestID);
    static long  GetEncouragementSum(long lQuestID);
    static CQusetEncouragement* GetEncouragementHead(long lQuestID);
//    static CQusetEncouragement* GetRolePropHead(long lQuestID);
//    static long  GetRolePropSum(long lQuestID);

private:
	static bool AddQuest(tagQuestList& QuestList, CQuest* quest);
	static bool AddMainQuest(long index,long roleID);
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
	typedef map<long,CQuest*>	mapQuests;
	typedef map<long,CQuest*>::iterator itQuests;

	typedef map<long,tagQuestList> mapQuestList;
	typedef map<long,tagQuestList>::iterator itQuestList;

	static mapQuestList	m_mapQuestList;
	static vector<long>			m_vecMainQuestList;				//���������б�
};
