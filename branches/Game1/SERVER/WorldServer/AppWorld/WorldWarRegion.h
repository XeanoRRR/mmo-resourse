//////////////////////////////////////////////////////////////////////////
//worldwarregion.h/
//ս�����᳡���Ļ���
//Author��Joe
//Create Date:2005.7.5
//////////////////////////////////////////////////////////////////////////


#pragma once
#include "worldregion.h"


class CWorldWarRegion :
	public CWorldRegion
{
public:
	CWorldWarRegion(void);
	~CWorldWarRegion(void);
protected:

	long	m_lSymbolTotalNum;					//��־��������
	long	m_lWinVicSymbolNum;					//��ʤ��ȡ��־�������
	long	m_lVicSymbolNum;					//��ֵ�����ʤ�ı�־������
	
	//////////////////////////////////////////////////////////////////////////
	//	�ӿ�
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL Load();	// ��ȡ	
	BOOL Save();	// ��ȡ

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
};
