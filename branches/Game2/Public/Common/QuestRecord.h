#pragma once

const ulong ROLE_PRAME_NUM=10;
const ulong ROLE_NUM_MAX=65535;
const ulong ROLE_ERRO=0xffffffff;
const ulong ROLE_NULL=0xffffffff - 1;

enum eQuestState
{
	eQS_No=0,       //û�и�����
	eQS_Complete,   //��������ɸ�����
	eQS_NotComplete,//���ڵ�δ���
};

enum eShareResult
{
    eQsExist,         //������������б�
    eQsFinish,        //����ɴ������б�
    eQsNo,            //������
};

enum  eQuestTextId
{
    eNoQuestId,
    eNoQuest =1,
    eComQuest,
    eNoComQuest_No,
    eNoComQuest_Fin,
    eComQuestText,    //����������ɺ������ʾ�ı�
    /// �ͻ���
    eQuestDes,        //��������
    eQuestAward,
};

struct sQuestParam
{
	sQuestParam()
	{
		m_wParam=0;
		m_wExcessRecordIndex=0;
		m_lAccParam=0;
	}
	long m_wParam;
	long m_wExcessRecordIndex;
	long m_lAccParam;
};

struct sExcessRecord
{
	sExcessRecord()
	{
		m_lRegionID=0;
		m_lRegionX=0;
		m_lRegionY=0;
	}
	long m_lRegionID;
	long m_lRegionX;
	long m_lRegionY;
};

//һ�������¼
class CQuestRecord
{
public:
	CQuestRecord();
	CQuestRecord(long QuestID,long StepID);
	~CQuestRecord();

	// ѹ����
	bool AddDatatoByteArray(vector<uchar>* pByteArray);
	//��������
	bool DecordDataFromByteArray(uchar* pByte, long& pos);

    bool CodeToDataBlock(DBWriteSet& setWriteDB);
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	//ȡ������id
	long   GetQuestID(){return m_lQuestID;}

	//ȡ�õ�ǰ�Ĳ���id
	ushort GetQuestStep(){return m_cQuestStep;}

	//����������������
	void   JumpStepTo(uchar stepnum){m_cQuestStep=stepnum;	memset(m_Param,0,sizeof(m_Param));m_cAimNum=0;}

	//����ֵ
	//ȡ�ñ���ֵ
	long GetParam(long Index);						
	
	//���ñ���ֵ
	bool SetParam(long Index, long val);			
	
	//���ӱ�����ֵ
	bool AddParam(long Index, long addval);

	//ȡ�ø��ӱ���
	long GetAccParam(long Index);						

	//���ø��ӱ���ֵ
	bool SetAccParam(long Index, long val);			

	//���Ӹ��ӱ�����ֵ
	bool AddAccParam(long Index, long addval);

	//ȡ�ö����¼����
	ushort GetExcessRecordIndex(long aimIndex);

	//���ö����¼����
	bool SetExcessRecordIndex(long aimIndex,ushort IndexVal);

	//�����������е�m_wExcessRecordIndex
	bool IsExist(ushort wIndex);

private:

	long    m_lQuestID;									//����id
	uchar   m_cQuestStep;								//������еĲ���
	uchar   m_cAimNum;
	sQuestParam m_Param[ROLE_PRAME_NUM];				    //�������
};

//��������б�
class CQuestQueue
{

public:
	CQuestQueue();
	~CQuestQueue();

	//����һ��id  ��ȡһ������ָ��
	CQuestRecord *GetRecordByID(long ID);
	//��ȡĳһ����ǰ�Ĳ�����
	long         GetQuestStepNum(long ID);
	//Ѱ��ĳ�������ĳһ�����Ƿ����
	bool         FindQuestStep(long ID , long Step);
	
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	long GetParam(long QuestID, long Step ,long Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	bool SetParam(long Quest,  long Step, long Index, long val);

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	bool AddParam(long Quest, long Step ,long Index, long val);
	//*********************************************************
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	long GetAccParam(long Quest, long Step ,long Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	bool SetAccParam(long Quest,  long Step,long Index, long val);

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	bool AddAccParam(long Quest, long Step ,long Index, long val);

	//*********************************************************
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	long GetRegionID(long Quest, long Step ,long Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	bool SetRegionID(long Quest,  long Step,long Index, long val);

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	long GetRegionX(long Quest, long Step ,long Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	bool SetRegionX(long Quest,  long Step,long Index, long val);

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	long GetRegionY(long Quest, long Step ,long Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	bool SetRegionY(long Quest,  long Step,long Index, long val);
	//**********************************************************


	//ȡ�������б����������
	long GetQuestSum(){return (long)m_listQuestRecord.size();}
	//����һ������
	bool AddOneRecord(CQuestRecord*& ptr);
	//
	bool AddOneRecord(long Quest,long step);
	//ɾ��һ������
	bool DelOneRecord(long ID);

	//����һ������ĸ��Ӽ�¼
	bool ClearOneRecordEx(CQuestRecord*);
	//�����������n��
	void JumpStepTo(long lQuest,uchar stepnum);

	//ȡ���������������½�index
	long GetChapter(){return m_lChapter;}

	//�������������½���
	void SetChapter(long num){m_lChapter=num;}

	//�ƶ����������½�������һ�½�
	bool JumpChapterNext(){m_lChapter++;return true;}

	//���������world server  ѹ���ݣ�
	bool AddQuestDataByteArray(vector<uchar>* pByteArray);
	//��ͻ��� ѹ����
    //bool AddQuestDataByteArray_ForClient(vector<uchar>* pByteArray);
	//�����Է�����������
	bool DecordQuestDataFromByteArray(uchar* pByte, long& pos);
	//�ͻ��˽����Է�����������
	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	//��������¼
	bool AddFinishedQuestRecord(long index);
	//ɾ�������¼
	bool DelFinishedQuestRecord(long index);

	//ȡ��һ���������������״̬���Ƿ���ɣ�
	bool GetQuestFinishedRecord(long index);
	// ��������б�
	list<CQuestRecord*> * GetQuestRecordList()	{return &m_listQuestRecord;}
	//ɾ����������¼���������
	bool DeleteExcessRecord();

private:
	sExcessRecord* FindsExcessRecord(ushort index);

	ushort	CreatExcessRecord(long regionID,long regionX,long regionY);
	bool	DelExcessRecord(ushort index);

	void Clean();

private:

	list<CQuestRecord*>		m_listQuestRecord;					//�����б�
	CQuestRecord*			m_pCurrentPtr;						//����ָ�룬�Ż��ٶȡ�
	long					m_lChapter;							//�������������½�
	set<long>				m_setQuestRecord;					//�����¼
	map<ushort , sExcessRecord> m_mapExcessRecord;				//�����¼
};

//ٸ������ϵͳ��
class CMercenaryQuestSys
{ 
public:
    //����һ��ٸ�������б�ṹ��,����4���ɽ��ܵ�����
    struct tagMerQuest
    {
        tagMerQuest():dwLastUpdateTime(0){}
        ulong dwLastUpdateTime;
        vector<long>	Quests;
    };

    typedef map<long,tagMerQuest>	mapMerQuests;
    typedef map<long,tagMerQuest>::iterator itMerQ;

public:

	CMercenaryQuestSys();
	~CMercenaryQuestSys();

private:

	set<long>	    DisbandQuests;
	//����ٸ�������б�
	mapMerQuests	m_MerQuests;
	//��ǰ���ܵ�ٸ������
	long	        m_lCurQuestID;
	//���һ����ɵ��ͽ�����
	long	        m_lLastQuestID;
	//�ӳ����������
	long	        m_lMasterQuestID;

public:
	//ѹ����
	bool AddDataByteArray(vector<uchar>* pByteArray);
	//������
	bool DecordDataFromByteArray(uchar* pByte, long& pos);
	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	//�õ���ǰ�Ľ��յ�ٸ������
	long GetCurQuestID()	{ return m_lCurQuestID; }
	void SetCurQuestID(long lID) { m_lCurQuestID=lID; }
	//���öӳ����������
	void SetMasterQuestID(long lID);
	long GetMasterQuestID()				{return m_lMasterQuestID;}		
	//�õ�������ɵ��ͽ�����
	long GetLastQuestID()	{return m_lLastQuestID;}
	void SetLastQuestID(long lID)	{ m_lLastQuestID = lID; }
	//�ж����б����Ƿ���ڸ�����
	long IsExistQuest(long lQuestID);
	//�ж����б����Ƿ���ڸ������б������
	bool IsExistQuestList(long lQuestListID);
	//�õ����б�ID��ٸ������
	bool GetMercQuests(long lListID,tagMerQuest& MercQuests);
	//�õ���ǰ���е�ٸ������
	void GetMercListQuests(set<long> &quests);
	//�������ٸ����������
	void Clear();
	//����ĳ���б��ٸ������
	void SetMercQuests(long lListID,const tagMerQuest& quests){m_MerQuests[lListID]=quests;}
	//�õ�����ֵ
	long GetData(long lQuestListID,long lPos);
	//���ò���ֵ 
	void SetData(long lQuestListID,long lPos,long lValue);
	//�����һ�������ʱ��
	long OnAddQuest(long lQuestID);
	//��ɾ��һ�������ʱ��
	bool OnDelQuest(long lQuestID);
	//���ˢ��ʱ��
	bool ClearTime(long MerListID);
	//��¼���ͽ������б���,����������.����lListID�����ж��Ƿ��ǵ�ǰ�б����������
	void DisbandQuestRecord(long lQuestID,bool bFlag = false);
	//���ҷ��������б�
	long FindDisbandQuest(long lQuestID);
	//�õ����������б�
	void GetDisbandQuest(set<long> &quests);
};
