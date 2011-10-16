#pragma once
#include <iostream>
#include <map>
#include <vector>
using namespace std;


#define  ROEL_MAIN  1

const LONG  QUEST_TEXT_SUM= 10;							//��������ı�����
const LONG  QUEST_STEP_SUM=20;							//�����������
const LONG  QUEST_TRACE_SUM=10;
const LONG  MAINQUEST_SUM=1024*16;						//������������
const LONG  CHAPTER_MAINQUEST_NUM = 6;					// ÿ���������������


//��������
class CQuestAttribute
{
public:
	CQuestAttribute();
	~CQuestAttribute(){}
	BOOL Init(TiXmlElement*p,long lTextTypeID);
public:
	LONG GetID()const		{ return m_lID;}			//ȡ�þ�����id

	LONG GetTextTypeID()const{return m_lTextTypeID;}	// ���úͻ���ı����ͱ��

	LONG GetRoleType()const{return m_lRoleType;}		//ȡ����������

	//ȡ��  �����Ƿ���ʾ
	LONG GetShowAble()const{return m_bShowAble;}
	//�������С�ȼ�����
	LONG GetMinGrade()const{return m_lMinGrade;}
	//��������ȼ�����
	LONG GetMaxGrade()const{return m_lMaxGrade;}

	//ȡ��ְҵ����
	BOOL GetOccupationRestrict(INT index)const
	{
		if (index>=0 && index< OCC_Max)
		{
			return m_Occupation[OCC_Max] & (1<<(index%64)) ;
		}
		else 
			return FALSE;
	}
	//ȡ���½���
	LONG GetChapterIndex()const{return m_lChapterIndex;}
	//ȡ����������
	const string& GetQuestName() const {return m_strQuestName;}
	//ȡ������  ͼ��
	LONG GetIcon() const {return m_lIcon;}
	//ȡ������ͼƬ
	LONG GetPicture() const {return m_lPicture;}
	//ȡ�����������Ч��
	LONG GetEmbracerEffect() const {return m_lEmbracerEffect;}
	//
	LONG GetFinishEmbracerEffect()const{return m_lFinishEmbracerEffect;}
	//ȡ����������ߵ�ͼЧ��
	LONG GetEmbracerMapEffect() const {return m_lEmbracerMapEffect;}
	//ȡ�ù�������
	LONG GetStoryDescIndex() const {return m_lStoryDescIndex;} 
	//ȡ������������ı�ID
	LONG GetDescribTextID() const {return m_lDescribTextID;}

	//ȡ�� �������ߵ�id�����⣩
	//const CGUID& GetNPCID()const {return m_NPCID;}
	const string& GetNPCName()const{return m_strNPCName;}
	LONG GetNPCShowType()const{return m_lNPCShowType;}
	LONG GetNPCClewType()const{return m_lNPCClewType;}
	LONG GetHardFactor()const{return m_lHardFactor;}
private:
	VOID ParseOccupation(const char *str);
	int  GetNumInParse(char*&);
private:
	LONG			m_lID;							//����id
	LONG			m_lTextTypeID;					// ��Ӧ���ı�����
	LONG			m_lRoleType;					//��������
	unsigned int	m_Occupation[OCC_Max/32+1];		//ְҵ��ʶ
	BOOL			m_bShowAble;					//�ܷ���ʾ
	LONG			m_lMinGrade;					//��Сְҵ����	
	LONG			m_lMaxGrade;					//���ְҵ����

	LONG			m_lChapterIndex;				//�½���
	string			m_strNPCName;						//������npc ID ����������
	LONG			m_lNPCShowType;					//npc �ɽ�������ʾ����������
	LONG			m_lNPCClewType;					//npc ������ʾ(��ʱ���ܽӣ�

	string			m_strQuestName;					//������
	LONG			m_lEmbracerEffect;				//����Ч����ʶ
	LONG			m_lFinishEmbracerEffect;		//�������Ч����ʶ
	LONG			m_lEmbracerMapEffect;			//�����ͼ��ʶ
	LONG			m_lIcon;						//����ͼ�꣨���ߣ�
	LONG			m_lPicture;						//����ͼƬ��ʶ�����ߣ�
	LONG			m_lStoryDescIndex;				//�������������ߣ�

	LONG			m_lDescribTextID;				//����������ı�ID
	LONG			m_lHardFactor;					//������Ѷ�ϵ��
};
//����׷��
class CQuestTrace
{
public:
	CQuestTrace();
	~CQuestTrace();
	BOOL Init(TiXmlElement*);
public:
	//	����׷�ٵ�����
	LONG GetDescIndex()const {return m_lDescIndex;}
	//	�Ƿ���ʾ�����
	BOOL IsShowNum()const {return m_bShowNum;}

	// ���ø�׷���Ƿ�ﵽ�������
	//void SetTraceIsFinish(bool IsFinish) {m_bIsFinish = IsFinish;}
	//BOOL IsTraceFinish()const;

	//	����׷����Ҫ�������
	LONG GetSum()const{return m_lSum;}
	//	��Ӧ�ı�������
	LONG GetParam()const{return m_lParam-1;}
	//	��Ӧ�Ķ���ԭʼ��ID
	string GetGoodsStrID()const{return m_strID;}
	//	ȡ��npc��id
	//LONG GetNPCID()const{return	m_lNPCID;}
	string GetNPCName()const{return m_strNPCName;}
	//	ȡ��npc����ʾЧ����1��
	LONG GetNPCEffect_1()const{return m_lNPCEffect_1;}
	//	ȡ��npc����ʾЧ����2��
	LONG GetNPCEffect_2()const{return m_lNPCEffect_2;}
private:
	LONG	m_lDescIndex;
	BOOL	m_bShowNum;
	//BOOL	m_bIsFinish;
	LONG	m_lSum;
	LONG	m_lParam;
	string	m_strID;
	string	m_strNPCName;
	LONG	m_lNPCEffect_1;
	LONG	m_lNPCEffect_2;
public:
	CQuestTrace* next;

};
//���� ����
class CQuestStep
{
public:
	CQuestStep(){
		m_lID=0;m_lQuestTraceNum=0;m_QuestTrace=0;m_lRoleTraceStartRow=0;
		next = NULL;
	}
	~CQuestStep();
	BOOL Init(TiXmlElement *);  
public:
	//ȡ�ò���id
	LONG  GetID() const {return m_lID;}
	//ȡ�ò�������
	LONG GetDiscrib() const {return m_lDiscrib;}
	//ȡ�ò���׷��
	//const char *GetRoleTrace() const {return m_strRoleTrace.c_str();}
	const CQuestTrace* GetRoleTraceHead() const {return m_QuestTrace;}

	// �������׷�ٵ���ʼ��ʾλ��
	LONG GetTraceStartRow() const {return m_lRoleTraceStartRow;}

	// �жϸò����Ƿ����꣬��Ҫ�жϸò����ÿ������׷���Ƿ�ﵽ�������
	//BOOL IsStepFinish() const;

	//ȡ�� index ��Ӧ���ı�
	LONG GetText(LONG index) const
	{
		if (index >=0 && index <QUEST_TEXT_SUM)
		{
			return m_lTextIndex[index];
		}
		else
			return 0;
	}
	BOOL AddQuestTrace(CQuestTrace*&);
	const string & GetNPCName() const {return m_strNPCName;}
	LONG GetNPCEffect_1()const{return m_lNPCEffect_1;}
	LONG GetNPCEffect_2()const{return m_lNPCEffect_2;}
private:
	LONG			m_lID;						//����id
	LONG			m_lDiscrib;				//��������
	LONG			m_lRoleTraceStartRow;			//����׷�ٵ���ʼ��ʾλ��
	LONG			m_lQuestTraceNum;
	CQuestTrace*	m_QuestTrace;
	LONG			m_lTextIndex[QUEST_TEXT_SUM];	//�����ı�
	string			m_strNPCName;
	LONG			m_lNPCEffect_1;
	LONG			m_lNPCEffect_2;
public:
	CQuestStep*		next;
};

class CQuest
{
public:
	CQuest(){m_lStepNum=0;m_pQuestStep=0;} 
	~CQuest();
	BOOL Init(TiXmlElement*p,long lTextTypeID);
public:
	//ȡ������id
	LONG GetID() const {return m_Attribute.GetID();}
	//ȡ����������
	LONG GetStepNum() const {return m_lStepNum;}
	//ȡ����������
	const CQuestAttribute & GetAttribute() const { return m_Attribute;}
	//ȡ��������id ��Ӧ�� ���� 
	const CQuestStep* GetStep(LONG ID)const;
private:
	//����һ������
	BOOL AddStep(CQuestStep *&step);
private:
	LONG m_lStepNum;				//������
	CQuestAttribute m_Attribute;	//����
	CQuestStep* m_pQuestStep;		//�������б�ͷ
};



//-----------------------------------------------------------
class CQuestSystem
{
public:
	CQuestSystem();
	~CQuestSystem();
private:
	struct tagQuestList
	{
		long lTextType;//�ַ���Դ����
		long lTitleTextID;//�����ַ���ԴID
		//���б��µ���������
		map<LONG,CQuest*>	Quests;
	};
public:
	static BOOL Init(const char* filename);
	static BOOL InitAttribute(TiXmlElement* p);
	//static BOOL InitText(const char* filename);
	static VOID Relese();
public:
	//ȡ��ָ������id��ָ�������£�ָ������������׷��
	static const CQuestTrace* GetQuestTrace(LONG lQuestID,LONG lStep,LONG lIndex);
	//ȡ������id��Ӧ������
	static const CQuest* GetQuest(LONG id);
	//ȡ���½�����Ӧ����������
	static const LONG GetMainRole(LONG chapterindex);
	//ȡ����������npc�����������б�
	static vector<LONG> GetSpecialQuestByNPCID(string npcName);
	//
	//static string* GetTextByIndex(LONG index);
	static long GetMercQuestUpdateInter() {return m_lMercQuestUpdateInter;}
	static void GetmapQuestList(LONG nID,long &TextType,long &lTitleTextID);
private:
	//���һ�����������б���
	static BOOL AddQuest(tagQuestList& QuestList, CQuest* quest);
	//����һ����������
	static BOOL AddMainQuest(LONG chapterindex, LONG questID);
	//����һ����������npc ������Ӧ��һ������id
	static BOOL AddSpecialQuest(string npcName,LONG questID);
	static BOOL InitOneFile(const char* filename,tagQuestList& QuestList);
	//static BOOL InitOneText(const char* filename);
private:
	static long	m_lMercQuestUpdateInter;		//ٸ������ˢ�¼۸�ʱ��

	typedef map<LONG,CQuest*>	mapQuests;
	typedef map<LONG,CQuest*>::iterator itQuests;

	typedef map<LONG,tagQuestList> mapQuestList;
	typedef map<LONG,tagQuestList>::iterator itQuestList;

	static mapQuestList	m_mapQuestList;
	static vector<LONG>			m_vecMainQuestList;				//���������б�

	static multimap<string,LONG> m_mmapSpecialQuestList;
};

class CQuestContent
{
public:
	CQuestContent()	{};
	~CQuestContent()	{};

	struct tagQuestContent 
	{
		string	strName;		//��������
		long	lRegionID;
		long	lPosX;
		long	lPosY;
	};
private:
	static map<long,tagQuestContent*>		m_mapMainQuest;
public:
	static BOOL	InitQuest(const char* filename);
	static void Release();

	static tagQuestContent* GetQuest(long lQuestID);
};