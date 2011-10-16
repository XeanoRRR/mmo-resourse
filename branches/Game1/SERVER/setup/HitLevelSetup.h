#pragma once

class CHitLevelSetup
{
public:
	CHitLevelSetup(void);
	virtual ~CHitLevelSetup(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:
	struct tagHitLevel
	{
		DWORD dwLevel;
		DWORD dwHit;
		DWORD dwExp;
	};
private:
	static vector<tagHitLevel> m_vecHitLevel;
public:	
	static vector<tagHitLevel>*	GetHitLevelVec(){return &m_vecHitLevel;}
	static bool LoadHitLevelSetup(const char* filename);

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};
