#include "StdAfx.h"
#include "Country.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCountry::CCountry(void)
:m_lID(0), m_lPower(0), m_King(0), m_lTechLevel(0),m_eState(PEACE)
{
}

CCountry::~CCountry(void)
{
}



//BYTE			m_lID;						// ����ID  // ���ID�ǹ̶��˵ġ�
//											// ���ܴ������ļ����ȡ�����������ݿ��ֵ�����ȡ��
//long			m_lPower;					// ���� ����Ϊ1��
//long			m_King;					// ����ID
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
