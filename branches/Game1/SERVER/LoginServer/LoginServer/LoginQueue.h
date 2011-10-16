#pragma once
//////////////////////////////////////////////////////////////////////////
//LoginQueue.h
//Fun:	��½�������
//Author:Joe
//Create Time:2005.1.31
//Modify Time:
//////////////////////////////////////////////////////////////////////////
#include "..\public\GUID.h"
#include "Lock.h"

#include <set>
using namespace std;

class CLoginQueue
{
public:
	//������֤Cdkey����
	struct tagQuestCdkey
	{
		long	lSocketID;				// socketID
		DWORD	dwIP;					// IP
		long	lVersion;				// �汾��
		string	strCdkey;				// Cdkey
		vector<BYTE>	strPassWord;	// ����
		string	strWSName;				// WorldServer������
		long	lChallengeCode;			// ��ս��
		long	lEkey;					// �ܱ�
		DWORD   dwSendMessageTime;		// ������Ϣ��ʱ��
		bool	bLimit;					// δ���꣨�����ԣ�0��δ���꣬1�����꣩
		int		nRes;					// ��֤���
		bool	bISCPTAuthen;			// �Ƿ�ͨ����PT��֤����
        tagQuestCdkey()
			: lSocketID(0), dwIP(0), lVersion(0), strCdkey(0) ,strPassWord(0), strWSName(0),
			lChallengeCode(0), lEkey(0), dwSendMessageTime(0), bLimit(false), nRes(0),bISCPTAuthen(false)
		{
		}
		tagQuestCdkey( long SocketID, DWORD IP, long Version, string Cdkey, vector<BYTE> PassWord,
			string WSName, long ChallengeCode, long Ekey, DWORD SendMessageTime, bool Limit, int Res )
			: lSocketID(SocketID), dwIP(IP), lVersion(Version),strCdkey(Cdkey) ,
			strPassWord(PassWord), strWSName(WSName), lChallengeCode(ChallengeCode),
			lEkey(Ekey), dwSendMessageTime(SendMessageTime), bLimit(Limit), nRes(Res),bISCPTAuthen(false)
		{}
       tagQuestCdkey( const tagQuestCdkey& rQuestCdkey )
			: lSocketID(rQuestCdkey.lSocketID), dwIP(rQuestCdkey.dwIP), lVersion(rQuestCdkey.lVersion),
			strCdkey(rQuestCdkey.strCdkey) ,strPassWord(rQuestCdkey.strPassWord), strWSName(rQuestCdkey.strWSName),
			lChallengeCode(rQuestCdkey.lChallengeCode), lEkey(rQuestCdkey.lEkey), dwSendMessageTime(rQuestCdkey.dwSendMessageTime),
			bLimit(rQuestCdkey.bLimit), nRes(rQuestCdkey.nRes),bISCPTAuthen(false)
		{}
	};

	//���������Ϣ����
	struct tagQuestPlayerList
	{
		long	lSocketID;				// SocketID
		string	strCdkey;				// Cdkey
		string	strWSName;				// WSName
		DWORD   dwSendMessageTime;		// ������Ϣ��ʱ��
	};
	//���������Ϣ�б�
	struct tagQuestPlayerData
	{
		long	lSocketID;				// socketID
		string	strCdkey;				// Cdkey
		CGUID	PlayerID;				// �����ID
		DWORD	dwIP;					// �ͻ���IP
		DWORD   dwSendMessageTime;		// ������Ϣ��ʱ��
		DWORD   dwSignCode;				// ��֤��
	};
	
private:

	std::set<string>						NoQueueCDkeyList;	//���Ŷӵ�CDKey�б�

	typedef std::list<tagQuestCdkey>	    QuestCdkey;			//��֤Cdkey�����б�
	typedef std::list<tagQuestCdkey *>	    QuestPtCheck;		//��֤Cdkey�����б�
	typedef std::list<tagQuestPlayerList>   QuestPlayerList;	//��ȡ������Ϣ�б����
	typedef std::list<tagQuestPlayerData>	QuestPlayerData;	//���������ϸ��Ϣ�б�

	typedef std::list<tagQuestCdkey>::iterator itCdkey;
	typedef std::list<tagQuestPlayerList>::iterator itPlayerList;
	typedef std::list<tagQuestPlayerData>::iterator itPlayerData;


	QuestCdkey								m_QuestCdkey;		//��֤Cdkey�����б�
	set<string>								m_setQuestCdkey;
	std::map<string,QuestPlayerList>		m_QuestPlayerList;
	map<string, set<string>>				m_mapQuestBaseInfo;
	std::map<string,QuestPlayerData>		m_QuestPlayerData;

	QuestCdkey								m_NoQueueQuestCdkey;		//�����Ŷӵ���֤Cdkey�����б�
	std::map<string,QuestPlayerList>		m_NoQueueQuestPlayerList;
	std::map<string,QuestPlayerData>		m_NoQueueQuestPlayerData;

	typedef std::map<string,QuestPlayerList>::iterator itPlayerListMap;
	typedef std::map<string,QuestPlayerData>::iterator itPlayerDataMap;

	long m_nWordNum;				//World��������
	DWORD m_dwInter;				//������еļ��ʱ��
	DWORD m_dwSendMessageInter;		//���ͻ��˷�����Ϣ�ļ��
	long m_nWorldMaxPlayerNum;		//WorldServer������½����

	DWORD m_dwDoLogQueueTime;		//������е�ʱ��
	DWORD m_dwWorldQueueTime;		//�����½World��ʱ��
public:
	CLoginQueue(void);
	~CLoginQueue(void);

	void	OnInitial();			//��ʼ��
	void	LoadNoQueueCdkeyList();
public:

	BOOL	IsWaitCdKey(const char* szCdkey){return m_setQuestCdkey.end() != m_setQuestCdkey.find(szCdkey);}
	bool	IsInNoQueueList(const char* szCdkey);	//��CDKey�Ƿ��ڲ����Ŷӵ��б���
	void	SetWorlNum(int nNum)		{m_nWordNum = nNum;}

	void	AddQuestCdkey(long lSocketID,DWORD dwIP,long lVersion,const char* szCdkey,
					  vector<BYTE>& bytePassWord,char* szWSName, long lChallengeCode, long lEkey);
	void	AddQuestPlayerList(long lSocketID,const char* szCdkey,const char* szWSName);
	void	AddQuestPlayerData(long lSocketID,const char* szCdkey,CGUID& playerid,DWORD dwIP, DWORD dwSignCode);

	void	Run();

	void	OnQuestCdkey(tagQuestCdkey& QuestCdkey);
	void	OnQuestPlayerList(tagQuestPlayerList& QuestPlayerList);
	void	OnQuestPlayerData(tagQuestPlayerData& QuestPlayerData);

	void	OnClientLost(const char* szCdkey);

private:
	// playerid, lastTime(curTime)
	typedef map<CGUID,DWORD,guid_compare> LoginList;
	typedef map<CGUID,DWORD,guid_compare>::iterator itLogList;
	LoginList			m_mLoginList;				//�Ѿ���½����

public:
	// �ѽ�ɫ�����½��
	bool	PushLoginList(CGUID playerid);
	// ����Ƿ�Ϸ�����
	bool	IsValidQuest(CGUID playerid);

	void	ClearTimeoutList();

private:
	QuestPtCheck				m_lstPtChecked;		// ִ����PT��֤�ʺŶ���
    Lock						lockPtChecked;		// ִ����PT��֤�ʺŶ��е���

public:
    tagQuestCdkey* PushBackPtChecked( tagQuestCdkey* pAcc );	// ��һ���˺�ѹ���β
	tagQuestCdkey* PopFrontPtChecked();							// ��һ���˺ŵ�������
	void CLoginQueue::ClearPtChecked(QuestPtCheck &lstChecked);	// ����ʺ���֤����
    bool IsPtCheckedEmpty();									// �ʺŶ����Ƿ�Ϊ��
    void HandlePtChecked();										// �����Ѿ�ͨ��������֤���ʺ�
};
