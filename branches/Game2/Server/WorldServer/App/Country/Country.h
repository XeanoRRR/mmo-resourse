// ������
// ��Ҫ�ǹ��ɹ��Ҷ���
// �������һ������ԺͲ���
// bugs
// 06/6/27

#pragma once
#ifndef _Country_h_
#define _Country_h_

#include "CountryParam.h"

class CCountry
{
public:
	CCountry(void);

	virtual ~CCountry(void);


public:
	enum eState
	{
		PEACE,									// ��ƽ
		WAR,									// ս��
	};

private:
	BYTE			m_lID;						// ����ID  // ���ID�ǹ̶��˵ġ�
												// ���ܴ������ļ����ȡ�����������ݿ��ֵ�����ȡ��
	long			m_lPower;					// ���� ����Ϊ1��
	CGUID			m_King;					// ����ID
	long			m_lTechLevel;				// �Ƽ��ȼ�

	eState			m_eState;					// ���ҵ�ǰ״̬

	//vector<long>	m_lRegions;					// ������س���ID
	//vector<long>	m_lFactions;				// ����ӵ�а��
public:
	void			SetID(BYTE id)				{ m_lID = id; }
	BYTE			GetID()						{ return m_lID; }

	void			SetPower(long power)
	{
		if( power > GetCountryParam()->m_lMaxCountryPower ) m_lPower = GetCountryParam()->m_lMaxCountryPower;
		else m_lPower = power;
	}
	long			GetPower()					{ return m_lPower; }

	void			SetKing(const CGUID& king)	{ m_King = king; }
	const CGUID&	GetKing()					{ return m_King; }

	void			SetTechLevel(long level)	{ m_lTechLevel = level; }
	long			GetTechLevel()				{ return m_lTechLevel;}

	void			SetState(eState state)		{ m_eState = state; }
	eState			GetState()					{ return m_eState; }

public:
	bool			IsWar()						{ return m_eState == WAR; }

	CCountry*		CloneSaveData();													//��¡��������

	bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};

#endif