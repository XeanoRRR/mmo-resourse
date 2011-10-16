//		�ļ���:		CDeputyOccu..h
//		����:		heliangchen
//		ʱ��:		2008/1/26
//		���:		��ְҵϵͳ: ��ְҵϵͳ������,���ڸ�ְҵ���ݵĲ�ѯ
#pragma once
#include"BaseDef.h"
#include "tinystr.h"
#include "tinyxml.h"

const int GOODSNAME_LEN_ =50;	//��Ʒ���ֳ���
const int NPCNAME_LEN_	=50;	//npc���ֵĳ���
const int E_DOCCU_NUM= 3;		//��ְҵ����

struct tgGoods 
{
	char	cGoodsName[GOODSNAME_LEN_];
	DWORD	dwGoodsIndex;
	int		iGoodNum;
	BYTE	bMaker;
	tgGoods(){bMaker = 0;}
};

struct tgOddsGoods 
{
	char	cGoodsName[GOODSNAME_LEN_];
	DWORD	dwGoodsIndex;
	int		iGoodNum;
	float	fOdds;
};

struct tgConditionGoods
{
	char	cGoodsName[GOODSNAME_LEN_];
	DWORD	dwGoodsIndex;
	int		iNumType;
	int		iNum;
};

struct tgNpcDistance 
{
	char 	cNpcName[NPCNAME_LEN_];
	int		iDistace;
};
	
struct tagQuestGoods
{
	int nQuestId;
	DWORD dwRoleStep;
};
//�ɼ�������
class CCollectionLayout
{
public:
	CCollectionLayout():m_byBlockType(0),m_dwActionID(1){}
	~CCollectionLayout(){}
	BOOL Init(TiXmlElement*);
	BOOL AddToByteArray(vector<BYTE>* pByteArray);
	BOOL DecordFromByteArray(BYTE* pByte, long& pos);
public:
	const string&	GetOrginName()const			{return m_strOrginName;}
	const string&	GetCollectionName()const	{return m_strCollectionName;}
	const string&	GetStartScriptName()const	{return m_strStartScriptName;}
	const string&	GetEndScriptName()const		{return m_strEndScriptName;}
	const string&   GetSuccScriptName()const	{return m_strSuccScriptName;}
	DWORD			GetCollectionID()const		{return m_dwCollectionID;}
	eDeputyOccu		GetOccuID()const			{return m_eOccuID;}
	DWORD			GetPicID()const				{return m_dwPicID;}
	DWORD			GetRadarID()const			{return m_dwRadarID;}
	DWORD			GetMinTimes()const			{return	m_dwMixTimes;}
	DWORD			GetMaxTime()const			{return m_dwMaxTimes;}
	DWORD			GetCollectLevel()const		{return m_dwCollectLevel;}
	DWORD			GetRoleID()const				{return 0;}
	DWORD			GetRoleStep()const			{return 0;}
	BYTE			GetBringBlock()const		{return m_byBlockType;}
	DWORD			GetActionID()const			{return m_dwActionID;}
public:
	const vector<tagQuestGoods>&	GetRoleInfo()const			{return m_vecQuestGoods;}
	const list<tgConditionGoods>&	GetConditionList()const		{return m_listGoodsCondition;}
	const list<tgOddsGoods>&		GetGainedList()const		{return m_listGainedGoods;}
protected:
	//ԭʼ��
	string			m_strOrginName;
	//�ɼ�������
	string			m_strCollectionName;	
	//�ɼ���id
	DWORD			m_dwCollectionID;
	//��ְҵid
	eDeputyOccu		m_eOccuID;
	//ͼ��id
	DWORD			m_dwPicID;
	//С��ͼ��־(�״�id)
	DWORD			m_dwRadarID;
	//��С�ɼ�����
	DWORD			m_dwMixTimes;
	//���ɼ�����
	DWORD			m_dwMaxTimes;
	//�ɼ��ȼ�
	DWORD			m_dwCollectLevel;
	////��Ҫ����ID
	//int				m_iRoleID;
	////��Ҫ������
	//DWORD			m_dwRoleStep;
	vector<tagQuestGoods>	m_vecQuestGoods;
	//�赲����
	BYTE			m_byBlockType;
	//�ɼ���ʼʱִ�нű���
	string			m_strStartScriptName;
	//�ɼ����ʱִ�нű�����
	string			m_strEndScriptName;
	//�ɼ�����ɹ�ʱִ�еĽű�
	string			m_strSuccScriptName;
	//����ɼ�ʱ����ID
	DWORD			m_dwActionID;
protected:
	//��Ʒ�����б�
	list<tgConditionGoods>		m_listGoodsCondition;	
	//�����Ʒ�б�
	list<tgOddsGoods>			m_listGainedGoods;
public:
	typedef list<tgConditionGoods>::iterator CONDITIONGOODS_ITTER;
	typedef list<tgOddsGoods>::iterator ODDSGOODS_ITTER;
	typedef vector<tagQuestGoods>::iterator QUESTGOODS_ITER;
};

//��������
class CFacture
{
public:
	CFacture():m_fQuotiety(1){}
	BOOL Init(TiXmlElement*);
	BOOL AddToByteArray(vector<BYTE>* pByteArray);
	BOOL DecordFromByteArray(BYTE* pByte, long& pos);
public:
	DWORD			GetFactureID()const		{return m_dwFactureID;}
	const string&	GetFactureName()const	{return m_strFactureName;}
	eDeputyOccu		GetArtisanID()const		{return m_eArtisanID;}
	DWORD			GetType()const			{return m_wType;}
	DWORD			GetFactureLevel()const	{return m_dwFactureLevel;}
	float			GetQuotiety()const		{return m_fQuotiety;}
	const list<tgGoods>&		GetToolList()const			{return m_listTools;}
	const list<tgNpcDistance>&	GetNpcList()const			{return m_listNpcDis;}
	const list<tgGoods>&		GetMaterialList()const		{return m_listMaterial;}
	const list<tgGoods>&		GetGainedList()const		{return m_listGainedGoods;}
	const list<tgGoods>&		GetAdvanceGainedList()const	{return m_listAdvancedGainedGood;}
protected:
	//������id
	DWORD		m_dwFactureID;
	//��������
	string		m_strFactureName;
	//����id
	eDeputyOccu m_eArtisanID;
	//��������Ʒ����
	DWORD		m_wType;
	//��Ҫ�������ܵȼ�
	DWORD		m_dwFactureLevel;
	//����ϵ��
	float		m_fQuotiety;
protected:
	//�����б�
	list<tgGoods> m_listTools;
	//npc�����б�
	list<tgNpcDistance> m_listNpcDis;
	//�����б�
	list<tgGoods> m_listMaterial;
	//��ȡ��Ʒ�б�
	list<tgGoods> m_listGainedGoods;
	//�߼���ȡ��Ʒ�б�
	list<tgGoods> m_listAdvancedGainedGood;
public:
	typedef list<tgGoods>::iterator GOODS_ITTER;
	typedef list<tgNpcDistance>::iterator NPC_ITTER;
};
struct CGapExp
{
public:
	CGapExp()
	{
		MinLevelGap	=0;
		MaxLevelGap	=0;
		GapExp		=0;
		GapCredit	=0;
		Color		=0;
	}
	DWORD TranslateColorCode(const char* chr);

public:
	LONG MinLevelGap;	//�ȼ�����Сֵ
	LONG MaxLevelGap;	//�ȼ������ֵ
	LONG GapExp;		//����ֵ
	LONG GapCredit;	//����ֵ
	DWORD Color;
};


struct CGradeExp 
{
	CGradeExp()
	{
		Level		=0;
		NeededExp	=0;
	}
	DWORD Level;		//�ȼ�
	DWORD NeededExp;	//�õȼ�����������
};


class CFactureStudy
{
public:
	CFactureStudy();
	~CFactureStudy();
	BOOL Init(TiXmlElement*);
	BOOL AddToByteArray(vector<BYTE>* pByteArray);
	BOOL DecordFromByteArray(BYTE* pByte, long& pos);
	BOOL FindFacture(DWORD dwFactureID)const;
	DWORD FindFactureCost(DWORD dwFactureID)const;
public:
	const string& GetNpcName()const			{return m_strFactureNpcName;}
	DWORD		  GetStudySum()const		{return m_vecFactureList.size();}
	DWORD		  GetFactureID(DWORD studynum)const
	{
		if (m_vecFactureList.size()<= studynum)return 0;
		return m_vecFactureList[studynum].FactureID;
	}
	DWORD		  GetFactureCost(DWORD studynum)const
	{
		if (m_vecFactureList.size()<=studynum)return 0;
		return m_vecFactureList[studynum].Cost;
	}
private:
	struct study  
	{
		DWORD FactureID;
		DWORD Cost;
	};
	string m_strFactureNpcName;
	vector<study> m_vecFactureList;
};

struct DeputyOccuSkillRecord 
{
	DeputyOccuSkillRecord()
	{
		memset(CollectiontSkill,0,sizeof(CollectiontSkill));
		memset(FactureSkill,0,sizeof(FactureSkill));
		TaskSkill=0;
	}
	DWORD CollectiontSkill[E_DOCCU_NUM];	//�ɼ������б�
	DWORD FactureSkill[E_DOCCU_NUM];		//���������б�
	DWORD TaskSkill;						//�������б�
};

class DOccuTypeLevel
{
public:
	DOccuTypeLevel(){Type=0; Level=0;}
	void Set(DWORD type,DWORD level){Type= type; Level= level;}
	bool operator <(const DOccuTypeLevel& m)const
	{
		if (Type==m.Type)
		{
			return Level<m.Level;
		}
		return Type< m.Type;
	}
private:
	DWORD Type;
	DWORD Level;
};


class CDeputyOccuSystem
{
public:
	// ��ְҵ��ӵ�е���Ϊ����
	enum OccuBeavioral
	{
		OB_Collection = 1,
		OB_Facture,
		OB_TaskCollection,

		OB_NUM
	};

	CDeputyOccuSystem();
	~CDeputyOccuSystem();
	static BOOL Init(const char* filename);
	static BOOL Relese();
	static BOOL AddToByteArray(vector<BYTE>* pByteArray);
	static BOOL DecordFromByteArray(BYTE* pByte, long& pos);
public:

	//��ȡһ���ɼ�������ñ��ɼ������֣�
	static const CCollectionLayout* GetCollectionLayout(const char* Name);
	//��ȡһ�����������ñ����������֣�
	static const CFacture*			GetFacture(DWORD ID);
	//��ȡ�����и߼���Ʒ��õĻ�������
	static		 DWORD				GetAdvancedGainedBacePro(){return m_dwAdvancedGainedBacePro;}

	//��ȡ�ɼ�һ�εľ��飨����ɼ��ȼ���ɼ���ĵȼ��
	static		 LONG				GetCollectionExp(LONG level);
	//��ȡ�ɼ������ɫ������ɼ��ȼ���ɼ���ĵȼ��
	static		 DWORD				GetCollectionColor(LONG level);
	//��ȡ�˲ɼ��ȼ���Ҫ�ľ��飨��ǰ������ɼ��ȼ���
	static		 DWORD				GetCollectionNeededExp(DWORD level);
	//ȡ��һ�������ľ��飨����������������ĵȼ��
	static		 LONG				GetFactureExp(LONG level);
	//ȡ��һ������������������������������ĵȼ��
	static		 LONG				GetFactureCredit(LONG level);
	//ȡ����������ɫ������������������ĵȼ��
	static		 DWORD				GetFactureColor(LONG level);
	//ȡ�õ�ǰ�����ȼ���������Ҫ���飨��ǰ���������ȼ���
	static		 DWORD				GetFactureNeededExp(DWORD level);
	//ȡ��һ��npc��������Ŀ��npc���֣�
	static const CFactureStudy*		GetFactureStudy(const char*NpcName);
	//ȡ��ĳ�ȼ�
	static const vector<string>*	GetStudyStudyClew(DWORD dwDOccuType,DWORD dwLevel);		
	//ȡ��ĳһ��������id��Ӧ������������֣��������id��
	static const string*			GetFactureSortName(DWORD FactureType);
	//ȡ�òɼ���ߵȼ�
	static		DWORD				GetMaxCollectionLevel(){return m_dwCollectMaxGrade;}
	//ȡ��������ߵȼ�
	static		DWORD				GetMaxFactureLevel(){return m_dwFactureMaxGrade;}
	//��ȡ��ְҵ����
	static		DWORD				GetDeputyOccuSkillID(OccuBeavioral type,eDeputyOccu DOccuType);
	//��ȡ�ɼ��ɹ�ϵͳ����
	static      string&				GetCollectionSuccSpring(){return m_strCollectionSuccSpring;}
	//��ȡ�����ɹ�ϵͳ����
	static		string&				GetFactureSuccSpring(){return m_strFactureSuccSpring;}
protected:
	//��ȡ��ְҵϵͳ�����ݣ��ֱ��ʼ������

	static BOOL InitCollectionList(const char* filename);
	static BOOL InitFactureList(const char* filename);
	static BOOL InitCollectionGapExpList(const char* filename);
	static BOOL InitFactureGapExpList(const char* filename);
	static BOOL InitCollectionGradeExpList(const char* filename);
	static BOOL InitFactureGradeExpList(const char* filename);
	static BOOL InitFactureStudyList(const char* filename);
	static BOOL	InitFactureSortList(const char* filename);
	static BOOL InitDeputyOccuSkillID(const char* filename);
protected:
	//�ͷ���Դ

	static BOOL ReleseCollectionList();
	static BOOL ReleseFactureList();
	static BOOL ReleseCollectionGapExpList();
	static BOOL ReleseFactureGapExpList();
	static BOOL ReleseCollectionGradeExpList();
	static BOOL ReleseFactureGradeExpList();
	static BOOL ReleseFactureStudyList();
	static BOOL ReleseFactureSortList();
	static BOOL ReleseDeputyOccuSkillID();
protected:
	//�ɼ��б�string�ǲɼ�������֣�
	static map<string,CCollectionLayout*>		m_mapCollectionList;
	//�����б�
	static map<DWORD,CFacture*>			m_mapFactureList;
	//�����߼������Ʒ�Ļ�������
	static DWORD						m_dwAdvancedGainedBacePro;
	//�ɼ������б�
	static list<CGapExp>				m_listCollectionGapExpList;
	//���������б�
	static list<CGapExp>				m_listFacturenGapExpList;
	//�ɼ��ȼ��б�
	static vector<CGradeExp>			m_vecCollectionGradeExpList;
	//�����ȼ��б�
	static vector<CGradeExp>			m_vecFactureGradeExpList;
	//����ѧϰ�б�string��NPC�����֣�
	static map<string,CFactureStudy*>	m_mapStudyList;
	//����ѧϰ��ʾ�б�
	static map<DOccuTypeLevel,vector<string>>		m_mapStudyClew;
	//��������
	static map<DWORD,string>			m_mapFactureSort;
	//�ɼ�����ߵȼ�
	static DWORD						m_dwCollectMaxGrade;
	//��������ߵȼ�
	static DWORD						m_dwFactureMaxGrade;
	//��ְҵ���ܼ�¼
	static DeputyOccuSkillRecord		m_tgDOccuSkillRecord;
	//�ɼ��ɹ�ϵͳ����
	static string						m_strCollectionSuccSpring;
	//�����ɹ�ϵͳ����
	static string						m_strFactureSuccSpring;
	

};
