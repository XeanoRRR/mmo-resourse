#include "stdafx.h"
#include "DeputyOccuRecord.h"
#include "public.h"
#include "..\public\DeputyOccu.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDeputyOccuRecord::CDeputyOccuRecord()
{
	m_lCollectionExp=0;
	m_lCollectionGrade=0;
	m_lFactureExp=0;
	m_lFactureGrade=0;
	m_lFectureSuccRatio=0;
}
CDeputyOccuRecord::~CDeputyOccuRecord()
{
}
//ѹ��ְҵ����
BOOL CDeputyOccuRecord::AddDeputyOccuDataByteArray(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray,m_lCollectionExp) ;
	_AddToByteArray(pByteArray,m_lCollectionGrade);
	_AddToByteArray(pByteArray,m_lFactureExp);
	_AddToByteArray(pByteArray,m_lFactureGrade);
	_AddToByteArray(pByteArray,m_lFectureSuccRatio);
	_AddToByteArray(pByteArray,(LONG)m_setFactureList.size());
	set<DWORD>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		_AddToByteArray(pByteArray,*it);
	}
	return TRUE;
}
//�⸱ְҵ����
BOOL CDeputyOccuRecord::DecordDeputyOccuDataFromByteArray(BYTE* pByte, long& pos)
{
	m_setFactureList.clear();
	m_lCollectionExp=_GetLongFromByteArray(pByte,pos);
	m_lCollectionGrade=_GetLongFromByteArray(pByte,pos);
	m_lFactureExp=_GetLongFromByteArray(pByte,pos);
	m_lFactureGrade=_GetLongFromByteArray(pByte,pos);
	m_lFectureSuccRatio=_GetLongFromByteArray(pByte,pos);
	LONG size =_GetLongFromByteArray(pByte,pos);
	for (int a=0; a<size; a++)
	{
		LONG l =_GetLongFromByteArray(pByte,pos);
		m_setFactureList.insert(l);
	}
	return TRUE;
}

//Ϊ�ͻ���ѹ����
BOOL CDeputyOccuRecord::AddDeputyOccuDataForClient(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray,m_lCollectionExp) ;
	_AddToByteArray(pByteArray,m_lCollectionGrade);
	_AddToByteArray(pByteArray,m_lFactureExp);
	_AddToByteArray(pByteArray,m_lFactureGrade);
	_AddToByteArray(pByteArray,(LONG)m_setFactureList.size());
	set<DWORD>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		_AddToByteArray(pByteArray,*it);
	}
	return TRUE;
}

//Ϊ�ͻ��˽�����
BOOL CDeputyOccuRecord::DecordDeputyOccuDataForClient(BYTE* pByte, long& pos)
{
	m_setFactureList.clear();
	m_lCollectionExp=_GetLongFromByteArray(pByte,pos);
	m_lCollectionGrade=_GetLongFromByteArray(pByte,pos);
	m_lFactureExp=_GetLongFromByteArray(pByte,pos);
	m_lFactureGrade=_GetLongFromByteArray(pByte,pos);

	LONG size =_GetLongFromByteArray(pByte,pos);
	for (int a=0; a<size; a++)
	{
		LONG l =_GetLongFromByteArray(pByte,pos);
		m_setFactureList.insert(l);
	}
	return TRUE;
}

BOOL CDeputyOccuRecord::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lCollectionExp) ;
	setWriteDB.AddToByteArray(m_lCollectionGrade);
	setWriteDB.AddToByteArray(m_lFactureExp);
	setWriteDB.AddToByteArray(m_lFactureGrade);
	setWriteDB.AddToByteArray(m_lFectureSuccRatio);
	setWriteDB.AddToByteArray((LONG)m_setFactureList.size());
	set<DWORD>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		setWriteDB.AddToByteArray(*it);
	}
	return TRUE;
}
BOOL CDeputyOccuRecord::CodeToDataBlockForClient(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lCollectionExp) ;
	setWriteDB.AddToByteArray(m_lCollectionGrade);
	setWriteDB.AddToByteArray(m_lFactureExp);
	setWriteDB.AddToByteArray(m_lFactureGrade);
	setWriteDB.AddToByteArray((LONG)m_setFactureList.size());
	set<DWORD>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		setWriteDB.AddToByteArray(*it);
	}
	return TRUE;
}
BOOL CDeputyOccuRecord::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_setFactureList.clear();
	m_lCollectionExp=setReadDB.GetLongFromByteArray();
	m_lCollectionGrade=setReadDB.GetLongFromByteArray();
	m_lFactureExp=setReadDB.GetLongFromByteArray();
	m_lFactureGrade=setReadDB.GetLongFromByteArray();
	//090410 by tanglei �߻��޸������ļ���,��Ҿ������
	//�ɼ�
	long maxlevel = CDeputyOccuSystem::GetMaxCollectionLevel(); 
	if (m_lCollectionGrade>maxlevel)
	{
		m_lCollectionGrade = maxlevel;
	}
	long maxexp = CDeputyOccuSystem::GetCollectionNeededExp(m_lCollectionGrade);
	if(m_lCollectionExp > maxexp)
	{
		ModifyCollectionExp(m_lCollectionGrade,m_lCollectionExp);
	}
	//����
	maxlevel = CDeputyOccuSystem::GetMaxFactureLevel(); 
	if (m_lFactureGrade>maxlevel)
	{
		m_lFactureGrade = maxlevel;
	}
	maxexp = CDeputyOccuSystem::GetFactureNeededExp(m_lFactureGrade);
	if (m_lFactureExp > maxexp)
	{
		ModifyFactureExp(m_lFactureGrade,m_lFactureExp);
	}
	//090410 end
	m_lFectureSuccRatio=setReadDB.GetLongFromByteArray();
	LONG size =setReadDB.GetLongFromByteArray();
	for (int a=0; a<size; a++)
	{
		LONG l =setReadDB.GetLongFromByteArray();
		m_setFactureList.insert(l);
	}
	return TRUE;
}

void CDeputyOccuRecord::ModifyFactureExp(long& level,long& lExp)
{
	//���ȼ�
	long maxlevel = CDeputyOccuSystem::GetMaxFactureLevel();
	//���ȼ����ܾ���
	long maxlevelexp = CDeputyOccuSystem::GetFactureNeededExp(maxlevel);
	if (level == maxlevel && lExp == maxlevelexp)
	{
		return;
	}
	long lastexp = lExp;
	while (lExp > CDeputyOccuSystem::GetFactureNeededExp(level) && level<maxlevel)
	{
		lExp -=CDeputyOccuSystem::GetFactureNeededExp(level);
		if (lExp>0)
		{
			lastexp = lExp;
			level ++;
		}
		else
			break;
	}
	lExp = lastexp>maxlevelexp?maxlevelexp:lastexp;
}

void CDeputyOccuRecord::ModifyCollectionExp(long& level,long& lExp)
{
	//���ȼ�
	long maxlevel = CDeputyOccuSystem::GetMaxCollectionLevel();
	//���ȼ����ܾ���
	long maxlevelexp = CDeputyOccuSystem::GetCollectionNeededExp(maxlevel);
	if (level == maxlevel && lExp == maxlevelexp)
	{
		return;
	}
	long lastexp = lExp;
	while (lExp > CDeputyOccuSystem::GetCollectionNeededExp(level) && level<maxlevel)
	{
		lExp -=CDeputyOccuSystem::GetCollectionNeededExp(level);
		if (lExp>0)
		{
			lastexp = lExp;
			level ++;
		}
		else
			break;
	}
	//�Դ������ȼ��ľ���,��ȥ���ྭ��
	lExp = lastexp>maxlevelexp?maxlevelexp:lastexp;
}

BOOL CDeputyOccuRecord::AddFacture(DWORD l)
{
	if (!FindFacture(l))
	{
		m_setFactureList.insert(l);
		return TRUE;
	}
	return FALSE;
}
BOOL CDeputyOccuRecord::FindFacture(DWORD l)
{
	set<DWORD>::iterator it= m_setFactureList.find(l);
	if (it!=m_setFactureList.end())
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDeputyOccuRecord::DeleteFacture(DWORD l)
{
	set<DWORD>::iterator it= m_setFactureList.find(l);
	if (it!=m_setFactureList.end())
	{
		m_setFactureList.erase(it);
		return TRUE;
	}
	return FALSE;
}
//���������Ŀ
BOOL CDeputyOccuRecord::CleanFacture()
{
	m_setFactureList.clear();
	return TRUE;
}