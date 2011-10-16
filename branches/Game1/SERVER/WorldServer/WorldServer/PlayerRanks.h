//������а�
//joe
//20006-3-16

#pragma once
#include "..\public\Date.h"

class CPlayer;

class CPlayerRanks
{
public:
	CPlayerRanks(void);
	~CPlayerRanks(void);

	struct tagRank
	{
		CGUID	PlayerID;		//���ID
		string	strName;		//�������
		WORD	wOccupation;	//ְҵ
		WORD	wLevel;		//�ȼ�
	
		string	strFacName;	//���ڰ������
	};

private:
	static CPlayerRanks* instance;											//����Ψһʵ�����

	bool	m_bStat;														//�Ƿ����ͳ��
	tagTime m_StatTime;														//ͳ������ʱ���
	ULONG	m_lStatRanksEventID;											//ͳ��������ʱ���ʱ��

	int				m_nMaxNum;												//�������а�����
	list<tagRank>	m_PlayerRanks;

public:
	static CPlayerRanks* getInstance();										//�õ�Ψһʵ�����
	bool   Initialize();													//��ʼ��
	void   Release();

	bool AddToByteArray(vector<BYTE>& ByteArray);							// ��ӵ�CByteArray
	bool DecordFromByteArray(BYTE* pByte, long& lPos);

	bool	GetIsStat()						{return m_bStat;}
	void	SetIsStat(bool b)				{m_bStat = b;}

	int		GetMaxNum()						{return m_nMaxNum;}

	tagTime& GetStatTime()					{return m_StatTime;}
	void	SetStatRanksEventID(ULONG lID)	{m_lStatRanksEventID = lID;}

	void Clear()		{m_PlayerRanks.clear();}
	void StatPlayerRanks();
	void AddRank(const CGUID& PlayerID, string& strName,WORD wOccu,WORD wLevl);
	void UpdateRanksToGameServer();

	static	void CALLBACK OnStatRanks(LPARAM lparam);						//ͳ������
};

CPlayerRanks* GetPlayerRanks();
