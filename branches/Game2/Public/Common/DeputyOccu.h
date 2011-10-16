/// ��ְҵϵͳ: ��д��ģ��ʱ��������ƹ淶һ��
/// ��ְҵ��WOW�д���רҵ����, ʹ�� PROFESSION ��ʾ
/// --FOX SEAL 07212010--

/**
 *		�ļ���:		CDeputyOccu..h
 *		����:		heliangchen
 *		ʱ��:		2008/1/26
 *		���:		��ְҵϵͳ: ��ְҵϵͳ������,���ڸ�ְҵ���ݵĲ�ѯ
 *      Modify By:  Expter [1/25/ 2010 ] 
 */
#pragma once   

#include "BaseDef.h"
const int GOODSNAME_LEN_ =50;	//��Ʒ���ֳ���
const int NPCNAME_LEN_	 =50;	//npc���ֵĳ���
const int E_DOCCU_NUM    = 3;	//��ְҵ����

struct tgGoods 
{
	char	cGoodsName[GOODSNAME_LEN_];
	ulong	dwGoodsIndex;
	int		iGoodNum;
	uchar	bMaker;
	tgGoods(){bMaker = 0;}
};

struct tgOddsGoods 
{
	char	cGoodsName[GOODSNAME_LEN_];
	ulong	dwGoodsIndex;
	int		iGoodNum;
	float	fOdds;
};

struct tgConditionGoods
{
	char	cGoodsName[GOODSNAME_LEN_];
	ulong	dwGoodsIndex;
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
	ulong dwRoleStep;
};

///
/// �ɼ�������
/// 
class CCollectionLayout
{

    typedef list<tgConditionGoods>::iterator CONDITIONGOODS_ITTER;
    typedef list<tgOddsGoods>::iterator ODDSGOODS_ITTER;
    typedef vector<tagQuestGoods>::iterator QUESTGOODS_ITER;

public:
	CCollectionLayout():m_byBlockType(0),m_dwActionID(1){}
	~CCollectionLayout(){}

	bool Init(TiXmlElement*);

	bool AddToByteArray(vector<uchar>* pByteArray);
	bool DecordFromByteArray(uchar* pByte, long& pos);

public:
	const string&	GetOrginName()const			{return m_strOrginName;}
	const string&	GetCollectionName()const	{return m_strCollectionName;}
	const string&	GetStartScriptName()const	{return m_strStartScriptName;}
	const string&	GetEndScriptName()const		{return m_strEndScriptName;}
	const string&   GetSuccScriptName()const	{return m_strSuccScriptName;}
	
	eDeputyOccu		GetOccuID()const			{return m_eOccuID;}

	ulong			GetPicID()const				{return m_dwPicID;}
	ulong			GetRadarID()const			{return m_dwRadarID;}
	ulong			GetMinTimes()const			{return	m_dwMixTimes;}
	ulong			GetMaxTime()const			{return m_dwMaxTimes;}
	ulong			GetCollectLevel()const		{return m_dwCollectLevel;}
	ulong			GetRoleID()const			{return 0;}
	ulong			GetRoleStep()const			{return 0;}
	uchar			GetBringBlock()const		{return m_byBlockType;}
	ulong			GetActionID()const			{return m_dwActionID;}
    ulong			GetCollectionID()const		{return m_dwCollectionID;}

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
	ulong			m_dwCollectionID;
	//��ְҵid
	eDeputyOccu		m_eOccuID;
	//ͼ��id
	ulong			m_dwPicID;
	//С��ͼ��־(�״�id)
	ulong			m_dwRadarID;
	//��С�ɼ�����
	ulong			m_dwMixTimes;
	//���ɼ�����
	ulong			m_dwMaxTimes;
	//�ɼ��ȼ�
	ulong			m_dwCollectLevel;
	vector<tagQuestGoods>	m_vecQuestGoods;
	//�赲����
	uchar			m_byBlockType;
	//�ɼ���ʼʱִ�нű���
	string			m_strStartScriptName;
	//�ɼ����ʱִ�нű�����
	string			m_strEndScriptName;
	//�ɼ�����ɹ�ʱִ�еĽű�
	string			m_strSuccScriptName;
	//����ɼ�ʱ����ID
	ulong			m_dwActionID;

protected:
	//��Ʒ�����б�
	list<tgConditionGoods>		m_listGoodsCondition;	
	//�����Ʒ�б�
	list<tgOddsGoods>			m_listGainedGoods;

};

///
/// ��������
///
class CFacture
{
public:
	CFacture():m_fQuotiety(1){}
	bool Init(TiXmlElement*);
	bool AddToByteArray(vector<uchar>* pByteArray);
	bool DecordFromByteArray(uchar* pByte, long& pos);
public:
	ulong			GetFactureID()const		{return m_dwFactureID;}
	const string&	GetFactureName()const	{return m_strFactureName;}
	eDeputyOccu		GetArtisanID()const		{return m_eArtisanID;}
	ulong			GetType()const			{return m_wType;}
	ulong			GetFactureLevel()const	{return m_dwFactureLevel;}
	float			GetQuotiety()const		{return m_fQuotiety;}
	const list<tgGoods>&		GetToolList()const			{return m_listTools;}
	const list<tgNpcDistance>&	GetNpcList()const			{return m_listNpcDis;}
	const list<tgGoods>&		GetMaterialList()const		{return m_listMaterial;}
	const list<tgGoods>&		GetGainedList()const		{return m_listGainedGoods;}
	const list<tgGoods>&		GetAdvanceGainedList()const	{return m_listAdvancedGainedGood;}
protected:
	//������id
	ulong		m_dwFactureID;
	//��������
	string		m_strFactureName;
	//����id
	eDeputyOccu m_eArtisanID;
	//��������Ʒ����
	ulong		m_wType;
	//��Ҫ�������ܵȼ�
	ulong		m_dwFactureLevel;
	//����ϵ��
	float		m_fQuotiety;
protected:
	// �����б�
	list<tgGoods> m_listTools;
	// npc�����б�
	list<tgNpcDistance> m_listNpcDis;
	// �����б�
	list<tgGoods> m_listMaterial;
	// ��ȡ��Ʒ�б�
	list<tgGoods> m_listGainedGoods;
	// �߼���ȡ��Ʒ�б�
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
	ulong TranslateColorCode(const char* chr);

public:
	long MinLevelGap;	//�ȼ�����Сֵ
	long MaxLevelGap;	//�ȼ������ֵ
	long GapExp;		//����ֵ
	long GapCredit;	    //����ֵ
	ulong Color;
};


struct CGradeExp 
{
	CGradeExp()
	{
		Level		=0;
		NeededExp	=0;
	}
	ulong Level;		//�ȼ�
	ulong NeededExp;	//�õȼ�����������
};


class CFactureStudy
{
public:
	CFactureStudy();
	~CFactureStudy();
	
    bool  Init(TiXmlElement*);
	bool  AddToByteArray(vector<uchar>* pByteArray);
	bool  DecordFromByteArray(uchar* pByte, long& pos);
	bool  FindFacture(ulong dwFactureID)const;
	ulong FindFactureCost(ulong dwFactureID)const;


public:
	const string& GetNpcName()const			{return m_strFactureNpcName;}
	ulong		  GetStudySum()const		{return (ulong)m_vecFactureList.size();}
	ulong		  GetFactureID(size_t studynum)const
	{
		if (m_vecFactureList.size()<= studynum)return 0;
		return m_vecFactureList[studynum].FactureID;
	}
	ulong		  GetFactureCost(ulong studynum)const
	{
		if (m_vecFactureList.size()<=studynum)return 0;
		return m_vecFactureList[studynum].Cost;
	}
private:
	struct study  
	{
		ulong FactureID;
		ulong Cost;
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
	ulong CollectiontSkill[E_DOCCU_NUM];	//�ɼ������б�
	ulong FactureSkill[E_DOCCU_NUM];		//���������б�
	ulong TaskSkill;						//�������б�
};

class DOccuTypeLevel
{
public:
	DOccuTypeLevel(){Type=0; Level=0;}
	void Set(ulong type,ulong level)
    {   Type= type; Level= level;   }
	bool operator <(const DOccuTypeLevel& m)const
	{
		if (Type==m.Type)
		{
			return Level<m.Level;
		}
		return Type< m.Type;
	}

private:
	ulong Type;
	ulong Level;
};


class CDeputyOccuSystem
{ 

public:
    /// ��ְҵ��ӵ�е���Ϊ����
	enum OccuBeavioral
	{
		OB_Collection = 1,
		OB_Facture,
		OB_TaskCollection,
		OB_NUM
	};

public:
	CDeputyOccuSystem();
	~CDeputyOccuSystem();
	static bool Init(const char* filename);
	static bool Relese();

	static bool AddToByteArray(vector<uchar>* pByteArray);
	static bool DecordFromByteArray(uchar* pByte, long& pos);

public:

	/// ��ȡһ���ɼ�������ñ��ɼ������֣�
	static const CCollectionLayout* GetCollectionLayout(const char* Name);
	/// ��ȡһ�����������ñ����������֣�
	static const CFacture*			GetFacture(ulong ID);
	//��ȡ�����и߼���Ʒ��õĻ�������
	static		 ulong				GetAdvancedGainedBacePro(){return m_dwAdvancedGainedBacePro;}
	/// ��ȡ�ɼ�һ�εľ��飨����ɼ��ȼ���ɼ���ĵȼ��
	static		 long				GetCollectionExp(long level);
	/// ��ȡ�ɼ������ɫ������ɼ��ȼ���ɼ���ĵȼ��
	static		 ulong				GetCollectionColor(long level);
	/// ��ȡ�˲ɼ��ȼ���Ҫ�ľ��飨��ǰ������ɼ��ȼ���
	static		 ulong				GetCollectionNeededExp(ulong level);
	/// ȡ��һ�������ľ��飨����������������ĵȼ��
	static		 long				GetFactureExp(long level);
	/// ȡ��һ������������������������������ĵȼ��
	static		 long				GetFactureCredit(long level);
	/// ȡ����������ɫ������������������ĵȼ��
	static		 ulong				GetFactureColor(long level);
	/// ȡ�õ�ǰ�����ȼ���������Ҫ���飨��ǰ���������ȼ���
	static		 ulong				GetFactureNeededExp(ulong level);
	/// ȡ��һ��npc��������Ŀ��npc���֣�
	static const CFactureStudy*		GetFactureStudy(const char*NpcName);
	/// ȡ��ĳ�ȼ�
	static const vector<string>*	GetStudyStudyClew(ulong dwDOccuType,ulong dwLevel);		
	/// ȡ��ĳһ��������id��Ӧ������������֣��������id��
	static const string*			GetFactureSortName(ulong FactureType);
	/// ȡ�òɼ���ߵȼ�
	static		ulong				GetMaxCollectionLevel(){return m_dwCollectMaxGrade;}
	/// ȡ��������ߵȼ�
	static		ulong				GetMaxFactureLevel(){return m_dwFactureMaxGrade;}
	/// ��ȡ��ְҵ����
	static		ulong				GetDeputyOccuSkillID(OccuBeavioral type,eDeputyOccu DOccuType);
	/// ��ȡ�ɼ��ɹ�ϵͳ����
	static      string&				GetCollectionSuccSpring(){return m_strCollectionSuccSpring;}
	/// ��ȡ�����ɹ�ϵͳ����
	static		string&				GetFactureSuccSpring(){return m_strFactureSuccSpring;}


protected:

	/// ��ȡ��ְҵϵͳ�����ݣ��ֱ��ʼ������
	static bool InitCollectionList(const char* filename);
	static bool InitFactureList(const char* filename);
	static bool InitCollectionGapExpList(const char* filename);
	static bool InitFactureGapExpList(const char* filename);
	static bool InitCollectionGradeExpList(const char* filename);
	static bool InitFactureGradeExpList(const char* filename);
	static bool InitFactureStudyList(const char* filename);
	static bool	InitFactureSortList(const char* filename);
	static bool InitDeputyOccuSkillID(const char* filename);

protected:
	/// �ͷ���Դ
	static bool ReleseCollectionList();
	static bool ReleseFactureList();
	static bool ReleseCollectionGapExpList();
	static bool ReleseFactureGapExpList();
	static bool ReleseCollectionGradeExpList();
	static bool ReleseFactureGradeExpList();
	static bool ReleseFactureStudyList();
	static bool ReleseFactureSortList();
	static bool ReleseDeputyOccuSkillID();

protected:
	//�ɼ��б�string�ǲɼ�������֣�
	static map<string,CCollectionLayout*>		m_mapCollectionList;
	//�����б�
	static map<ulong,CFacture*>			m_mapFactureList;
	//�����߼������Ʒ�Ļ�������
	static ulong						m_dwAdvancedGainedBacePro;
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
	static map<ulong,string>			m_mapFactureSort;
	//�ɼ�����ߵȼ�
	static ulong						m_dwCollectMaxGrade;
	//��������ߵȼ�
	static ulong						m_dwFactureMaxGrade;
	//��ְҵ���ܼ�¼
	static DeputyOccuSkillRecord		m_tgDOccuSkillRecord;
	//�ɼ��ɹ�ϵͳ����
	static string						m_strCollectionSuccSpring;
	//�����ɹ�ϵͳ����
	static string						m_strFactureSuccSpring;
};
