#pragma once

const DWORD ROLE_PRAME_NUM=10;
const DWORD ROLE_NUM_MAX=65535;
const DWORD ROLE_ERRO=0xffffffff;
const DWORD ROLE_NULL=0xffffffff - 1;

#include <list>
#include <set>
using namespace std;

enum eQuestState
{
	eQS_No=0,//û�и�����
	eQS_Complete,//��������ɸ�����
	eQS_NotComplete,//���ڵ�δ���
};

struct sRoleParam
{
	sRoleParam()
	{
		m_wParam=0;
		m_wExcessRecordIndex=0;
		m_lAccParam=0;
	}
	WORD m_wParam;
	WORD m_wExcessRecordIndex;
	LONG m_lAccParam;
};

struct sExcessRecord
{
	sExcessRecord()
	{
		m_lRegionID=0;
		m_lRegionX=0;
		m_lRegionY=0;
	}
	LONG m_lRegionID;
	LONG m_lRegionX;
	LONG m_lRegionY;
};

//һ�������¼
class CRoleRecord
{
public:
	CRoleRecord();
	CRoleRecord(LONG RoleID,LONG StepID);
	~CRoleRecord();
	// ѹ����
	BOOL AddDatatoByteArray(vector<BYTE>* pByteArray);
	//��������
	BOOL DecordDataFromByteArray(BYTE* pByte, long& pos);

	BOOL CodeToDataBlock(DBWriteSet& setWriteDB);
	BOOL DecodeFromDataBlock(DBReadSet& setReadDB);

	//ȡ������id
	LONG GetRoleID(){return m_lRoleID;}

	//ȡ�õ�ǰ�Ĳ���id
	WORD GetRoleStep(){return m_cRoleStep;}

	//����������������
	VOID JumpStepTo(BYTE stepnum){m_cRoleStep=stepnum;	memset(m_Param,0,sizeof(m_Param));m_cAimNum=0;}

	//����ֵ
	//ȡ�ñ���ֵ
	LONG GetParam(LONG Index);						
	
	//���ñ���ֵ
	BOOL SetParam(LONG Index, LONG val);			
	
	//���ӱ�����ֵ
	BOOL AddParam(LONG Index, LONG addval);

	//ȡ�ø��ӱ���
	LONG GetAccParam(LONG Index);						

	//���ø��ӱ���ֵ
	BOOL SetAccParam(LONG Index, LONG val);			

	//���Ӹ��ӱ�����ֵ
	BOOL AddAccParam(LONG Index, LONG addval);

	//ȡ�ö����¼����
	WORD GetExcessRecordIndex(LONG aimIndex);

	//���ö����¼����
	BOOL SetExcessRecordIndex(LONG aimIndex,WORD IndexVal);
	//�����������е�m_wExcessRecordIndex
	BOOL IsExist(WORD wIndex);
private:
	LONG m_lRoleID;									//����id
	BYTE m_cRoleStep;								//������еĲ���
	BYTE m_cAimNum;
	sRoleParam m_Param[ROLE_PRAME_NUM];					//�������
};

//��������б�
class CRoleQueue
{
public:
	CRoleQueue();
	~CRoleQueue();
	//����һ��id  ��ȡһ������ָ��
	CRoleRecord *GetRecordByID(LONG ID);
	//��ȡĳһ����ǰ�Ĳ�����
	LONG GetRoleStepNum(LONG ID);
	//Ѱ��ĳ�������ĳһ�����Ƿ����
	BOOL FindRoleStep(LONG ID , LONG Step);
	
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	LONG GetParam(LONG RoleID, LONG Step ,LONG Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	BOOL SetParam(LONG RoleID,  LONG Step,LONG Index, LONG val);

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	BOOL AddParam(LONG RoleID, LONG Step ,LONG Index, LONG val);
	//*********************************************************
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	LONG GetAccParam(LONG RoleID, LONG Step ,LONG Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ı���ֵ
	BOOL SetAccParam(LONG RoleID,  LONG Step,LONG Index, LONG val);

	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	BOOL AddAccParam(LONG RoleID, LONG Step ,LONG Index, LONG val);

	//*********************************************************
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	LONG GetRegionID(LONG RoleID, LONG Step ,LONG Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	BOOL SetRegionID(LONG RoleID,  LONG Step,LONG Index, LONG val);

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	LONG GetRegionX(LONG RoleID, LONG Step ,LONG Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	BOOL SetRegionX(LONG RoleID,  LONG Step,LONG Index, LONG val);

	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	LONG GetRegionY(LONG RoleID, LONG Step ,LONG Index);

	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	BOOL SetRegionY(LONG RoleID,  LONG Step,LONG Index, LONG val);
	//**********************************************************


	//ȡ�������б����������
	LONG GetRoleSum(){return (LONG)m_listRoleRecord.size();}

	//����һ������
	BOOL AddOneRecord(CRoleRecord*& ptr);
	//
	BOOL AddOneRecord(LONG roleid,LONG step);
	//ɾ��һ������
	BOOL DelOneRecord(LONG ID);

	//����һ������ĸ��Ӽ�¼
	BOOL ClearOneRecordEx(CRoleRecord*);
	//�����������n��
	VOID JumpStepTo(LONG lRoleID,BYTE stepnum);

	//ȡ���������������½�index
	LONG GetChapter(){return m_lChapter;}

	//�������������½���
	VOID SetChapter(LONG num){m_lChapter=num;}

	//�ƶ����������½�������һ�½�
	BOOL JumpChapterNext(){m_lChapter++;return TRUE;}

	//���������world server  ѹ���ݣ�
	BOOL AddQuestDataByteArray(vector<BYTE>* pByteArray);
	//��ͻ��� ѹ����
//	bool AddQuestDataByteArray_ForClient(vector<BYTE>* pByteArray);
	//�����Է�����������
	BOOL DecordQuestDataFromByteArray(BYTE* pByte, long& pos);
	//�ͻ��˽����Է�����������
	BOOL CodeToDataBlock(DBWriteSet& setWriteDB);
	BOOL DecodeFromDataBlock(DBReadSet& setReadDB);

	//��������¼
	BOOL AddFinishedRoleRecord(LONG index);
	//ɾ�������¼
	BOOL DelFinishedRoleRecord(LONG index);

	//ȡ��һ���������������״̬���Ƿ���ɣ�
	BOOL GetRoleFinishedRecord(LONG index);
	// ��������б�
	list<CRoleRecord*> * GetRoleRecordList()	{return &m_listRoleRecord;}
	//ɾ����������¼���������
	BOOL DeleteExcessRecord();
private:
	sExcessRecord* FindsExcessRecord(WORD index);
	WORD	CreatExcessRecord(LONG regionID,LONG regionX,LONG regionY);
	BOOL	DelExcessRecord(WORD index);

	VOID Clean();
private:
	list<CRoleRecord*>		m_listRoleRecord;					//�����б�
	CRoleRecord*			m_pCurrentPtr;						//����ָ�룬�Ż��ٶȡ�
	LONG					m_lChapter;							//�������������½�
	set<LONG>				m_setRoleRecord;					//�����¼
	map<WORD , sExcessRecord> m_mapExcessRecord;				//�����¼
};

//ٸ������ϵͳ��
class CMercenaryQuestSys
{
public:
	CMercenaryQuestSys();
	~CMercenaryQuestSys();

	//����һ��ٸ�������б�ṹ��,����4���ɽ��ܵ�����
	struct tagMerQuest
	{
		tagMerQuest():dwLastUpdateTime(0){}
		DWORD dwLastUpdateTime;
		vector<long>	Quests;
	};
private:	
	set<long>	DisbandQuests;
	typedef map<long,tagMerQuest>	mapMerQuests;
	typedef map<long,tagMerQuest>::iterator itMerQ;
	//����ٸ�������б�
	mapMerQuests	m_MerQuests;
	//��ǰ���ܵ�ٸ������
	long	m_lCurQuestID;
	//���һ����ɵ��ͽ�����
	long	m_lLastQuestID;
	//�ӳ����������
	long	m_lMasterQuestID;
public:
	//ѹ����
	bool AddDataByteArray(vector<BYTE>* pByteArray);
	//������
	bool DecordDataFromByteArray(BYTE* pByte, long& pos);

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
