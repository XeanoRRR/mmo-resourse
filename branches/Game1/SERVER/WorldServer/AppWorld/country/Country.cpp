#include "StdAfx.h"
#include ".\country.h"

CCountry::CCountry(void)
:m_lID(0), m_lPower(0), m_lTechLevel(0),m_eState(PEACE)
{
}

CCountry::~CCountry(void)
{
}

//��¡��������,ֻ�ǿ�¡�仯�˵�����
CCountry* CCountry::CloneSaveData()
{
	CCountry* pCloneCountry= MP_NEW CCountry();
	if( pCloneCountry ) 
	{
		pCloneCountry->m_lID		= m_lID;
		pCloneCountry->m_lPower		= m_lPower;
		pCloneCountry->m_King		= m_King;
		pCloneCountry->m_lTechLevel	= m_lTechLevel;
	}
	return pCloneCountry;
}

//BYTE			m_lID;						// ����ID  // ���ID�ǹ̶��˵ġ�
//											// ���ܴ������ļ����ȡ�����������ݿ��ֵ�����ȡ��
//long			m_lPower;					// ���� ����Ϊ1��
//long			m_lKing;					// ����ID
//long			m_lTechLevel;				// �Ƽ��ȼ�

//eState			m_eState;					// ���ҵ�ǰ״̬

// ��ӵ�CByteArray
bool CCountry::AddToByteArray(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray, m_lID);
	_AddToByteArray(pByteArray, m_lPower);
	_AddToByteArray(pByteArray, m_King);
	_AddToByteArray(pByteArray, m_lTechLevel);
	_AddToByteArray(pByteArray, (long)m_eState);
	return true;
}

// ����
bool CCountry::DecordFromByteArray(BYTE* pByte, long& pos)
{
	m_lID = _GetByteFromByteArray(pByte, pos);
	m_lPower = _GetLongFromByteArray(pByte, pos);
	_GetBufferFromByteArray(pByte, pos, m_King);
	m_lTechLevel = _GetLongFromByteArray(pByte, pos);
	m_eState = (eState)_GetLongFromByteArray(pByte, pos);
	return true;
}