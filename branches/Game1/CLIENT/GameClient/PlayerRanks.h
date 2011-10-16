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
		int		nPlayerID;		//���ID
		string	strName;		//�������
		WORD	wOccupation;	//ְҵ
		WORD	wLevel;		//�ȼ�
	
		string	strFacName;	//�����л�����
	};

private:
	static CPlayerRanks* instance;											//����Ψһʵ�����

	list<tagRank>	m_PlayerRanks;

public:
	static CPlayerRanks* getInstance();										//�õ�Ψһʵ�����
	bool   Initialize();													//��ʼ��
	void   Release();

	bool DecordFromByteArray(BYTE* pByte, long& lPos);

	list<tagRank> *GetPlayerRanksList()		
	{
		return &m_PlayerRanks;
	}
};

CPlayerRanks* GetPlayerRanks();
