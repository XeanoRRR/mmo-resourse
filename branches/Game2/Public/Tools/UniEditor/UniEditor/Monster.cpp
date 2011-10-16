#include "StdAfx.h"
#include "Monster.h"
#include <stdlib.h>

CMonsterInfo::CMonsterInfo(void)
{
	m_strName		= _T("0");		// ��������
	m_uProbability	= 0;			// ��������
	m_uIdentifier	= 0;			// ��ʶ
	m_uLeader		= 0;			// �쵼
	m_uDistance		= 0;			// ����
	m_uPathID		= 0;			//·����ʶ
	m_strScript		= _T("0");		// �����ű�
	m_strTalkScript	= _T("0");		//�Ի��ű�
}

CMonsterInfo::~CMonsterInfo(void)
{

}

void CMonsterInfo::MonsterInfo2String(CString &str)
{
	str.Format(_T("\t%-14s %2u%% ��ʶ:%-3u �쵼:%-3u ����:%-2u %s\n"),
		m_strName.c_str(), m_uProbability, m_uIdentifier, m_uLeader, m_uDistance,
		m_strScript.c_str());
}

CMonster::CMonster(void)
{
	m_rcCoor		=  CRect(1, 1, 20, 20);			//����
	m_uTimeBorn		= 0;								// ����ʱ��
	m_uTimeStart	= 0;								// ��ʼʱ��
}

CMonster::CMonster(const CPoint pt)
{
	m_rcCoor		=  CRect(1, 1, pt.x, pt.y);		// ����
	m_uTimeBorn		= 0;								// ����ʱ��
	m_uTimeStart	= 0;								// ��ʼʱ��
}

CMonster::CMonster(const long x, const long y)
{
	m_rcCoor		=  CRect(1, 1, x, y);			// ����
	m_uTimeBorn		= 0;								// ����ʱ��
	m_uTimeStart	= 0;								// ��ʼʱ��
}

CMonster::CMonster(CMonster &rMonster)
{
	m_uNo			= rMonster.GetNo();					// ���
	m_rcCoor		= rMonster.GetCoordinate();			// ����
	m_uQuantity		= rMonster.GetQuantity();			// ����
	m_iDirection	= rMonster.GetDirection();			// ����

	m_uTimeBorn		= rMonster.GetTimeBorn();			// ����ʱ��
	m_uTimeStart	= rMonster.GetTimeStart();			// ��ʼʱ��
	VECMONINFO &vecInfo = rMonster.GetMonsterInfo();
	for(VECMONINFOITR itr=vecInfo.begin(); itr!=vecInfo.end(); ++itr)
	{
		CMonsterInfo *pMonInfo = new CMonsterInfo(**itr);
		m_vecInfo.push_back(pMonInfo);
	}
}

CMonster::~CMonster(void)
{
	for(VECMONINFOITR itr=m_vecInfo.begin(); itr!=m_vecInfo.end(); ++itr)
	{
		delete *itr;
	}
}

// ���MONSTERINFO
bool CMonster::AddMonsterInfo(CMonsterInfo *pMonInfo)
{
	if(pMonInfo != NULL)
	{
		m_vecInfo.push_back(pMonInfo);
	}
	else
	{
		CMonsterInfo *pMonInfo = new CMonsterInfo;
		m_vecInfo.push_back(pMonInfo);
	}
	return true;
}

// ɾ��MONSTERINFO
bool CMonster::DelMonsterInfo(CMonsterInfo *pMonInfo)
{
	return true;
}

// ��MONSTER��Ϣת��Ϊ�ַ���
void CMonster::Monster2String(CString &str)
{
	str.Format(_T("\t(%-3u, %-3u; %-3u, %-3u) ����:%-3u ����ʱ��:%-3u ��ʼʱ��:%-3u ����:%-3d\n"),
		m_rcCoor.left, m_rcCoor.top, m_rcCoor.right, m_rcCoor.bottom, m_uQuantity, m_uTimeBorn,
		m_uTimeStart, m_iDirection);
}

ifstream & operator>>(ifstream &rstream, CMonsterInfo &rMonsterInfo)
{
	rstream >> rMonsterInfo.m_strName >> rMonsterInfo.m_uProbability
		>> rMonsterInfo.m_uIdentifier >> rMonsterInfo.m_uLeader
		>> rMonsterInfo.m_uDistance >> rMonsterInfo.m_uPathID
		>>rMonsterInfo.m_strScript>>rMonsterInfo.m_strTalkScript;

	return rstream;
}

ofstream & operator<<(ofstream &rstream, const CMonsterInfo &rMonsterInfo)
{

	rstream <<rMonsterInfo.m_strName<< strSep << rMonsterInfo.m_uProbability << strSep
	<< rMonsterInfo.m_uIdentifier << strSep << rMonsterInfo.m_uLeader << strSep 
	<< rMonsterInfo.m_uDistance << strSep <<rMonsterInfo.m_uPathID<<strSep
	<<rMonsterInfo.m_strScript<< strSep << rMonsterInfo.m_strTalkScript<< endl;

	return rstream;
}

ifstream & operator>>(ifstream &rstream, CMonster &rMonster)
{
	rstream >> rMonster.m_rcCoor.left >> rMonster.m_rcCoor.top
		>> rMonster.m_rcCoor.right >> rMonster.m_rcCoor.bottom >> rMonster.m_uQuantity
		>> rMonster.m_uTimeBorn >> rMonster.m_uTimeStart >> rMonster.m_iDirection;

	return rstream;
}

ofstream & operator<<(ofstream &rstream, const CMonster &rMonster)
{
	if (rMonster.m_bSwitch)
	{
		rstream << strSharp;
	} 
	else
	{
		rstream << strSep;
	}
	rstream << rMonster.m_uNo << strSep << rMonster.m_rcCoor.left << strSep
		<< rMonster.m_rcCoor.top << strSep << rMonster.m_rcCoor.right << strSep 
		<< rMonster.m_rcCoor.bottom << strSep << rMonster.m_uQuantity << strSep 
		<< rMonster.m_uTimeBorn << strSep << rMonster.m_uTimeStart << strSep 
		<< rMonster.m_iDirection << endl << endl;

	return rstream;
}
