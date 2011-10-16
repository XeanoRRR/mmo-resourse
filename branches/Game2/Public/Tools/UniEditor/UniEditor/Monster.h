#pragma once
#include "info.h"

// ���������Ϣ
class CMonsterInfo
{
public:
	CMonsterInfo(void);
	~CMonsterInfo(void);

	// ��Monster�ļ� "*.monster"
	friend ifstream &operator>>(ifstream &rstream, CMonsterInfo &rMonsterInfo); 
	friend ofstream &operator<<(ofstream &rstream, const CMonsterInfo &rMonsterInfo); 

// ���ݶ�д
public:
	const inline string& GetName()const						{return m_strName;}
	const inline unsigned GetProbability()const				{return m_uProbability;}
	const inline unsigned GetIdentifier()const				{return m_uIdentifier;}
	const inline unsigned GetLeader()const					{return m_uLeader;}

	const inline unsigned GetDistance()const				{return m_uDistance;}
	const inline unsigned GetPathIdentifier()const				{return m_uPathID;}
	const inline string& GetScript()const					{return m_strScript;}
	const inline string& GetTalkScript()const					{return m_strTalkScript;}

	inline void SetName(const string &strName)				{m_strName = strName;}
	inline void SetProbability(const unsigned uProbability)	{m_uProbability = uProbability;}
	inline void SetIdentifier(const unsigned uIdentifier)	{m_uIdentifier = uIdentifier;}
	inline void SetLeader(const unsigned uLeader)			{m_uLeader = uLeader;}
	inline void SetDistance(const unsigned uDistance)		{m_uDistance = uDistance;}
	inline void SetPathIdentifier(const unsigned uIdentifier)	{m_uPathID = uIdentifier;}
	inline void SetScript(const string &strScript)			{m_strScript = strScript;}
	inline void SetTalkScript(const string &strScript)			{m_strTalkScript = strScript;}
	
	void MonsterInfo2String(CString &str);

// ���������Ϣ
protected:
	string		m_strName;			// ��������
	unsigned	m_uProbability;		// ��������
	unsigned	m_uIdentifier;		// ��ʶ
	unsigned	m_uLeader;			// �쵼
	unsigned	m_uDistance;		// ����
	unsigned	m_uPathID;			//·����ʶ
	string		m_strScript;		// �����ű�
	string		m_strTalkScript;	//�Ի��ű�
};

typedef vector<CMonsterInfo *>	VECMONINFO;
typedef VECMONINFO::iterator	VECMONINFOITR;

class CMonster :
	public CInfo
{
public:
	CMonster(void);
	CMonster(const CPoint pt);
	CMonster(const long x, const long y);
	CMonster(CMonster &rMonster);
public:
	~CMonster(void);

public:
	// ��Monster�ļ� "*.monster"
	friend ifstream &operator>>(ifstream &rstream, CMonster &rMonster);
	friend ofstream &operator<<(ofstream &rstream, const CMonster &rMonster); 

// ���ݶ�д
public:
	const inline unsigned GetTimeBorn()const				{return m_uTimeBorn;}
	const inline unsigned GetTimeStart()const				{return m_uTimeStart;}
	inline VECMONINFO &GetMonsterInfo()						{return m_vecInfo;}

	inline void SetTimeBorn(const unsigned utime)			{m_uTimeBorn = utime;}
	inline void SetTimeStart(const unsigned utime)			{m_uTimeStart = utime;}

	// ���MONSTERINFO
	bool AddMonsterInfo(CMonsterInfo *pMonInfo = NULL);

	// ɾ��MONSTERINFO
	bool DelMonsterInfo(CMonsterInfo *pMonInfo);

	// ��MONSTER��Ϣת��Ϊ�ַ���
	void Monster2String(CString &str);

	// ���������Ϣ
	VECMONINFO	m_vecInfo;			// ���������Ϣ

// ������Ϣ
protected:
	unsigned	m_uTimeBorn;		// ����ʱ��
	unsigned	m_uTimeStart;		// ��ʼʱ��
};

//ifstream &operator>>(ifstream &rstream, CMonsterInfo &rMonsterInfo); 
//ofstream &operator<<(ofstream &rstream, const CMonsterInfo &rMonsterInfo); 
//
//ifstream &operator>>(ifstream &rstream, CMonster &rMonster);
//ofstream &operator<<(ofstream &rstream, const CMonster &rMonster);
