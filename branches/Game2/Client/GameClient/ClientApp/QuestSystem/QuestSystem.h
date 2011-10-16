/**
 * @file  QuestSystem.h
 * @brief ������߼���
 *
 * @modify expter  2010/07/05
 */

#pragma once

//#define  ROEL_MAIN44  1

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
	VOID InitPreQuest(TiXmlElement*p);				// ��ʼ��ǰ������
public:
//ȡ��ֵ�Ľӿ�
	//ȡ�þ�����id
	LONG GetID()const{return m_lID;}
	// ���úͻ���ı����ͱ��
	LONG GetTextTypeID()const{return m_lTextTypeID;}
	//ȡ����������
	LONG GetQuestType()const{return m_lQuestType;}
	//ȡ��  �����Ƿ���ʾ
	LONG GetShowAble()const{return m_bShowAble;}
	//�������С�ȼ�����
	LONG GetMinGrade()const{return m_lMinGrade;}
	//��������ȼ�����
	LONG GetMaxGrade()const{return m_lMaxGrade;}

	//ȡ��ְҵ����
	BOOL GetOccupationRestrict(BYTE bOccu)const
	{
		// ��ְҵ���ƣ�ֱ�ӷ�����
		if(m_vecOccu.empty())
			return TRUE;
		for (BYTE i=0;i<m_vecOccu.size();++i)
		{
			if(m_vecOccu[i] == bOccu)
				return TRUE;
		}
		return FALSE;
	}
	//ȡ���½���
	LONG GetChapterIndex()const{return m_lChapterIndex;}
	//ȡ����������
	const char* GetQuestName() const;
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
	const string& GetNPCName()const{return m_strNPCName;}
	LONG GetNPCShowType()const{return m_lNPCShowType;}
	LONG GetNPCClewType()const{return m_lNPCClewType;}
	LONG GetHardFactor()const{return m_lHardFactor;}
	LONG GetOdds()const{return m_lOdds;}
    BOOL GetIsShareQuest()const { return m_lIsShareQuest == 1; }

    /// �õ�ǰ�������б�
    const vector<LONG>& GetPreQuestList()const { return m_vecPreQuest; }

private:

	VOID ParseOccupation(const char *str);
	int  GetNumInParse(char*&);
	LONG			m_lID;							//����id
	LONG			m_lTextTypeID;					// ��Ӧ���ı�����
	LONG			m_lQuestType;					//��������
	BOOL			m_bShowAble;					//�ܷ���ʾ
	LONG			m_lMinGrade;					//��Сְҵ����	
	LONG			m_lMaxGrade;					//���ְҵ����

	LONG			m_lChapterIndex;				//�½���

	string			m_strNPCName;					//������npc ID ����������
	LONG			m_lNPCShowType;					//npc �ɽ�������ʾ����������
	LONG			m_lNPCClewType;					//npc ������ʾ(��ʱ���ܽӣ�
													//���Ǽ����Ժ���Խӵ�������npc���ϵ���ʾ����������
    LONG            m_lIsShareQuest;                //�Ƿ��ǹ�������  

	LONG			m_lEmbracerEffect;				//����Ч����ʶ
	LONG			m_lFinishEmbracerEffect;		//�������Ч����ʶ
	LONG			m_lEmbracerMapEffect;			//�����ͼ��ʶ
	LONG			m_lIcon;						//����ͼ�꣨���ߣ�
	LONG			m_lPicture;						//����ͼƬ��ʶ�����ߣ�
	LONG			m_lStoryDescIndex;				//�������������ߣ�

	LONG			m_lDescribTextID;				//����������ı�ID
	LONG			m_lHardFactor;					//������Ѷ�ϵ��
	LONG            m_lOdds;                        //����ϡ�ж�

	/*****************************08.11.6�������***********************************
		�������ĸ�ְҵ���ƣ�����������������Լ������������Ӧ�Ĳ�ѯ�����Ͳ�ѯ����
	********************************************************************************/
	vector<LONG>	m_vecPreQuest;					// ǰ�������б�
	vector<BYTE>	m_vecOccu;						// ְҵ�����б�
	vector<BYTE>	m_vecDOccu;						// ��ְҵ�����б�
	long			m_lDescTextIDBySearch;			// ��ѯ����ID
	long			m_lSpDescTextIDBySearch;		// ��ѯ����ID
public:
	void			InitOccu(vector<BYTE>& vecOccu,const char *str);	// ��ʼ��ְҵ���Ƽ���(��ְҵ����ְҵ)
	bool			GetDOccuRestrict(BYTE bDOccuID)const;	// ����ID������Ӧ�ĸ�ְҵ�Ƿ�ɽӸ�����
	long			GetDescTextIDBySearch()	const			// ��ò�ѯ����ID
					{return m_lDescTextIDBySearch;}
	long			GetSpDescTextIDBySearch() const			// ��ò�ѯ����ID
					{return m_lSpDescTextIDBySearch;}
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
	string GetDescIndex()const {return m_lDescIndex;}
	//	�Ƿ���ʾ�����
	BOOL IsShowNum()const {return m_bShowNum;}

	//	����׷����Ҫ�������
	LONG GetSum()const{return m_lSum;}
	//	��Ӧ�ı�������
	LONG GetParam()const{return m_lParam-1;}
	//	��Ӧ�Ķ���ԭʼ��ID
	string GetGoodsStrID()const{return m_strID;}
	//	ȡ��npc��id
	string GetNPCName()const{return m_strNPCName;}
	//	ȡ��npc����ʾЧ����1��
	LONG GetNPCEffect_1()const{return m_lNPCEffect_1;}
	//	ȡ��npc����ʾЧ����2��
	LONG GetNPCEffect_2()const{return m_lNPCEffect_2;}
    //  ȡ�õ�ǰ׷��������ı�
    const char* GetTraceText() { return NULL;}
private:
	string	m_lDescIndex;
	BOOL	m_bShowNum;
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
	CQuestStep()
    {
        m_lID = 0;
        m_lQuestTraceNum = 0;
        m_QuestTrace = 0;
        m_lQuestTraceStartRow = 0;
        m_lNPCEffect_1 = 0;
        m_lNPCEffect_2 = 0;
    }

    ~CQuestStep();
	BOOL Init(TiXmlElement*);

	//ȡ�ò���id
	LONG  GetID() const {return m_lID;}
	//ȡ�ò�������
	string GetDiscrib() const {return m_lDiscrib;}
	//ȡ�ò���׷��
	const CQuestTrace* GetQuestTraceHead() const {return m_QuestTrace;}

	// �������׷�ٵ���ʼ��ʾλ��
	LONG GetTraceStartRow() const {return m_lQuestTraceStartRow;}

	BOOL AddQuestTrace(CQuestTrace*&);
	const string & GetNPCName() const {return m_strNPCName;}
	LONG GetNPCEffect_1()const{return m_lNPCEffect_1;}
	LONG GetNPCEffect_2()const{return m_lNPCEffect_2;}
private:
	LONG			m_lID;						//����id
	string			m_lDiscrib;				//��������
	LONG			m_lQuestTraceStartRow;			//����׷�ٵ���ʼ��ʾλ��
	LONG			m_lQuestTraceNum;
	CQuestTrace*	m_QuestTrace;
	string			m_strNPCName;
	LONG			m_lNPCEffect_1;
	LONG			m_lNPCEffect_2;
public:
	CQuestStep*		next;
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
    long GetType()const{return m_lType;}
    //�����ľ���id
    long GetID()const{return m_lID;}
    //��������
    long GetNum()const{return m_lNum;}
    //��ñ���
    float GetOdds()const {return m_fOdds;}
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

    static BOOL  Init(const char* filename);

    static BOOL  LoadAward(const char* filename);

    static long  GetEncourCell( long Qid );

    static long  GetEncourSum( long Qid ) ;

    static CQusetEncouragement* GetEncouragement(long Qid);

    static string GetAwardText(long Qid);
private:

    static std::map<long,QuestAward*>  m_Award;

};

class CQuest
{
public:
	CQuest(){m_lStepNum=0;m_pQuestStep=NULL;} 
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
class CQuestRecord;
class CQuestSystem
{
	CQuestSystem();
	~CQuestSystem();

	struct tagQuestList
	{
		long lTextType;//�ַ���Դ����
		long lTitleTextID;//�����ַ���ԴID
		bool bIsCanBeSearch;	//���б��µ����������Ƿ��ܱ�����
		//���б��µ���������
		map<LONG,CQuest*>	Quests;
	};

	struct tagOddsColor
	{
		long OddsValue; //����ϡ�ж�
		long OddsColor; //ϡ�жȶ�Ӧ���ֵ���ɫֵ
	};
public:
	static BOOL Init(const char* filename);
	static BOOL InitAttribute(TiXmlElement* p);
	static VOID Relese();

    /// ȡ��ָ������id��ָ�������£�ָ������������׷��
	static const CQuestTrace* GetQuestTrace(LONG lQuestID,
        LONG lStep,
        LONG lIndex);

    /// ȡ������id��Ӧ������
	static const CQuest* GetQuest(LONG id);

    /// ȡ���½�����Ӧ����������
	static const LONG GetMainQuest(LONG chapterindex);

    /// ȡ����������npc�����������б�
	static vector<LONG> GetSpecialQuestByNPCID(string npcName);
	static long GetMercQuestUpdateInter() { return m_lMercQuestUpdateInter; }
	static void GetmapQuestList(LONG nID, long& TextType, long& lTitleTextID);

    /// Ӷ��ˢ����Ʒ�б�
    static vector<int> m_vecMerItemList;                   

    /// ȡ�������ܲ�ѯ�������б�
	static vector<CQuest*>& GetBeSearchQuestList()
    {
        return m_vecCanBeSearchQuestList;
    }

private:
	//���һ�����������б���
	static BOOL AddQuest(tagQuestList& QuestList, CQuest* quest);
	//����һ����������
	static BOOL AddMainQuest(LONG chapterindex, LONG questID);
	//����һ����������npc ������Ӧ��һ������id
	static BOOL AddSpecialQuest(string npcName,LONG questID);
	static BOOL InitOneFile(const char* filename,tagQuestList& QuestList);
	static long	m_lMercQuestUpdateInter;		//ٸ������ˢ�¼۸�ʱ��

	typedef map<LONG,CQuest*>	mapQuests;
	typedef map<LONG,CQuest*>::iterator itQuests;

	typedef map<LONG,tagQuestList> mapQuestList;
	typedef map<LONG,tagQuestList>::iterator itQuestList;

	static vector<CQuest*>	m_vecCanBeSearchQuestList;				// �����ܲ�ѯ�������б�

	static mapQuestList	m_mapQuestList;							// ���������б�
	static vector<LONG>			m_vecMainQuestList;				//���������б�

	static multimap<string,LONG> m_mmapSpecialQuestList;

public:
	typedef map<LONG,tagOddsColor> mapQuestOddsColor;
	typedef map<LONG,tagOddsColor>::iterator itQuestOddsColor;
	static bool InitOdds(TiXmlElement* p);       //��ʼ��ϡ�жȶ�Ӧ����ɫֵ
	static mapQuestOddsColor m_mapOddsColor;     //ϡ�ж��б�

private:
    static CQuestRecord* m_TempQuestRecord;
public:
    static CQuestRecord* GetTempQuestRecord() {return m_TempQuestRecord;}
    static void SetTempQuestRecord(CQuestRecord* q) {m_TempQuestRecord = q;}
};		

/////////////////////////////////////////////////
// zhaohang  2010/5/27 
// ������Ϣͨ�ŷ�װ,ʹ�õ���ģʽ
//��������ö��ֵ
enum eQuestType{
	QUEST_TYPE_NOT_MERCENARY = 0,		//��Ӷ������
	QUEST_TYPE_MERCENARY = 1,			//Ӷ������
	QUEST_TYPE_UNKNOW					//δ֪����
};

//������Ϣͨ��
class QuestMsgMgr
{
public:
	QuestMsgMgr();
	~QuestMsgMgr();

	/// @brief
	/// ���ͽ��ܸ�������
	/// @param questID
	///  ����ID
	/// @param Type
	/// Ĭ��Ϊ�ͽ�����
	/// @return
	/// 
	void Send_Accept(uint questID,eQuestType Type = QUEST_TYPE_MERCENARY);

	/// @brief
	/// ���ͽ����Ŷ�����
	/// @param
	/// 
	/// @return
	/// 
	void Send_Accept_TeamQuest(uint questID,eQuestType Type = QUEST_TYPE_MERCENARY);
	/// @brief
	///  ���ͷ���������Ϣ 
	/// @param questID
	/// ����������ID
	/// @param questType
	/// ��������
	/// @return
	/// 
	void Send_Disband(uint questID,eQuestType Type = QUEST_TYPE_NOT_MERCENARY);

	/// @brief
	/// ���͹���������Ϣ
	/// @param questID
	/// ����ID
	/// @return
	/// 
	void Send_Shared(uint questID);

	/// @brief
	/// ���Ͷ��������������
	/// @param questID
	/// ����ID
	/// @return
	/// 
	void Send_TeamQuestUpdate(uint questID);
	/// @brief
	/// �������������Ϣ
	/// @param questID
	/// ����ID
	/// @return
	/// 
	void Send_Complete(uint questID,eQuestType Type = QUEST_TYPE_MERCENARY);

	/// @brief
	/// //���͸��¸��������б�
	/// @param questID
	/// ����ID
	/// @param isUseGoods
	/// �Ƿ�ʹ����Ʒ
	/// @return
	/// 
	void Send_Update(uint questID,bool isUseGoods);
private:
	/// @brief
	/// //�����Ƽ�����
	/// @param type
	/// 0��ʾ�Ƽ�һ������1��ʾ������������б�
	/// @return
	/// 
	void Send_Commend(uint questID,BYTE type);
};

/////////////////////////////////////////////////
// zhaohang  2010/6/29 
// ��Ҷ�������������֯
////////////////////////////////////////////////
class TeamQuestData
{
public:
	struct tagTeamQuest
	{
		long lQuestID;					//����ID
		string strCommenderName;		//�Ƽ�������
		tagTeamQuest()
		{
			lQuestID = 0;
			strCommenderName = "";
		}
	};
	TeamQuestData();
	~TeamQuestData();
	/// @brief
	/// //����ID���Ƽ����������һ������,�������Ѿ��Ƽ���������֮ǰ�Ƽ����񱻵�ǰ�����滻
	/// @param ID
	/// ����ID
	/// @param srcPlayerName
	/// �Ƽ�����������
	/// @return
	///
	void Add(long ID,const char* srcPlayerName);	

	/// @brief
	/// �����Ƽ��ߣ�ɾ����Ӧ�Ķ�������
	/// @param srcPlayerName
	/// �Ƽ���(���)����
	/// @return
	/// 
	void Del(const char* srcPlayerName);

	/// @brief
	/// ָ��ID�������Ƿ��Ѿ��Ƽ���
	/// @param ID
	/// ����ID
	/// @return
	/// �Ѿ��Ƽ�������true�����򷵻�false
	bool IsCommended(long ID);
	void NotifyUpdateUI();						//֪ͨ���������ʾ
	void NotifyUIQuestRunning(long ID);			//֪ͨ���������ʾ���ڽ��е�����

	tagTeamQuest* Get(uint index);				//��ȡָ�������Ķ�������
	void Clear(){ vecTeamQuest.clear();}		//��ն��������б�
	//void SetTeamQuestID(long id) { teamQuestID = id; }
	//const long GetTeamQuestID() const { return teamQuestID;}
	const long GetRunningID() const { return curRunningID;}		//��ȡ�������ڽ��е�����ID
private:
	vector<tagTeamQuest> vecTeamQuest;		//���������б�
	//long teamQuestID;                       //ȫ������ID
	long curRunningID;						//��ǰ�������ڽ��е�����
};