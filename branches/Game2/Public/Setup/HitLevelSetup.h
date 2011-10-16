//======================================================================================
/**
*	@file		HitLevelSetup.h
*	@data		
*	@author		
*/
//======================================================================================
#pragma once

/**
*	@class HitLevelSetup
*
*	@brief �����ȼ�������
*/
class HitLevelSetup
{
public:
	/// ����
	HitLevelSetup(void);

	/// ����
	~HitLevelSetup(void);

public:
	/// �����ȼ��ṹ
	struct tagHitLevel
	{
		DWORD dwLevel;				// �ȼ�
		DWORD dwHit;				// ������
		DWORD dwExp;				// ����ֵ
	};

private:
	/// �����ȼ������б�
	static vector<tagHitLevel> m_vecHitLevel;

public:
	/// ȡ�������ȼ������б�
	static vector<tagHitLevel>*	GetHitLevelVec(){return &m_vecHitLevel;}

	/// ���ļ����������ȼ�����
	static bool LoadHitLevelSetup(const char* filename);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);	
};
