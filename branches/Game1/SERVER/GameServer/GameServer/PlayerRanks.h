//������а�
//joe
//20006-3-16

#pragma once


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

	list<tagRank>	m_PlayerRanks;

	map<CGUID,DWORD>				m_PlayerGetTime;							//����������һ�ȡ�����ݵļ��

public:
	static CPlayerRanks* getInstance();										//�õ�Ψһʵ�����
	bool   Initialize();													//��ʼ��
	void   Release();

	bool AddToByteArray(vector<BYTE>& ByteArray,int nAddNum);				// ��ӵ�CByteArray
	bool DecordFromByteArray(BYTE* pByte, long& lPos);

	void OnPlayerGetRanks(const CGUID& lPlayerID,long lMaxNum);
};

CPlayerRanks* GetPlayerRanks();
