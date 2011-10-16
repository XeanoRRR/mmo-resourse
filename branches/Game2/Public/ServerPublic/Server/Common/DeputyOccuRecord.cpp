#include "StdAfx.h"
#include "DeputyOccuRecord.h"


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
bool CDeputyOccuRecord::AddDeputyOccuDataByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray,m_lCollectionExp) ;
	_AddToByteArray(pByteArray,m_lCollectionGrade);
	_AddToByteArray(pByteArray,m_lFactureExp);
	_AddToByteArray(pByteArray,m_lFactureGrade);
	_AddToByteArray(pByteArray,m_lFectureSuccRatio);
	_AddToByteArray(pByteArray,(long)m_setFactureList.size());
	set<ulong>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		_AddToByteArray(pByteArray,*it);
	}
	return true;
}
//�⸱ְҵ����
bool CDeputyOccuRecord::DecordDeputyOccuDataFromByteArray(uchar* pByte, long& pos)
{
	m_setFactureList.clear();
	m_lCollectionExp=_GetLongFromByteArray(pByte,pos);
	m_lCollectionGrade=_GetLongFromByteArray(pByte,pos);
	m_lFactureExp=_GetLongFromByteArray(pByte,pos);
	m_lFactureGrade=_GetLongFromByteArray(pByte,pos);
	m_lFectureSuccRatio=_GetLongFromByteArray(pByte,pos);
	long size =_GetLongFromByteArray(pByte,pos);
	for (int a=0; a<size; a++)
	{
		long l =_GetLongFromByteArray(pByte,pos);
		m_setFactureList.insert(l);
	}
	return true;
}

//Ϊ�ͻ���ѹ����
bool CDeputyOccuRecord::AddDeputyOccuDataForClient(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray,m_lCollectionExp) ;
	_AddToByteArray(pByteArray,m_lCollectionGrade);
	_AddToByteArray(pByteArray,m_lFactureExp);
	_AddToByteArray(pByteArray,m_lFactureGrade);
	_AddToByteArray(pByteArray,(long)m_setFactureList.size());
	set<ulong>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		_AddToByteArray(pByteArray,*it);
	}
	return true;
}

//Ϊ�ͻ��˽�����
bool CDeputyOccuRecord::DecordDeputyOccuDataForClient(uchar* pByte, long& pos)
{
	m_setFactureList.clear();
	m_lCollectionExp=_GetLongFromByteArray(pByte,pos);
	m_lCollectionGrade=_GetLongFromByteArray(pByte,pos);
	m_lFactureExp=_GetLongFromByteArray(pByte,pos);
	m_lFactureGrade=_GetLongFromByteArray(pByte,pos);

	long size =_GetLongFromByteArray(pByte,pos);
	for (int a=0; a<size; a++)
	{
		long l =_GetLongFromByteArray(pByte,pos);
		m_setFactureList.insert(l);
	}
	return true;
}

bool CDeputyOccuRecord::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lCollectionExp) ;
	setWriteDB.AddToByteArray(m_lCollectionGrade);
	setWriteDB.AddToByteArray(m_lFactureExp);
	setWriteDB.AddToByteArray(m_lFactureGrade);
	setWriteDB.AddToByteArray(m_lFectureSuccRatio);
	setWriteDB.AddToByteArray((long)m_setFactureList.size());
	set<ulong>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		setWriteDB.AddToByteArray(*it);
	}
	return true;
}
bool CDeputyOccuRecord::CodeToDataBlockForClient(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lCollectionExp) ;
	setWriteDB.AddToByteArray(m_lCollectionGrade);
	setWriteDB.AddToByteArray(m_lFactureExp);
	setWriteDB.AddToByteArray(m_lFactureGrade);
	setWriteDB.AddToByteArray((long)m_setFactureList.size());
	set<ulong>::iterator it= m_setFactureList.begin();
	for (; it!= m_setFactureList.end(); it++)
	{
		setWriteDB.AddToByteArray(*it);
	}
	return true;
}
bool CDeputyOccuRecord::DecodeFromDataBlock(DBReadSet& setReadDB)
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
	long size =setReadDB.GetLongFromByteArray();
	for (int a=0; a<size; a++)
	{
		long l =setReadDB.GetLongFromByteArray();
		m_setFactureList.insert(l);
	}
	return true;
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
	while (lExp > (long)CDeputyOccuSystem::GetFactureNeededExp(level) && level<maxlevel)
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
	while (lExp > (long)CDeputyOccuSystem::GetCollectionNeededExp(level) && level<maxlevel)
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

bool CDeputyOccuRecord::AddFacture(ulong l)
{
	if (!FindFacture(l))
	{
		m_setFactureList.insert(l);
		return true;
	}
	return false;
}
bool CDeputyOccuRecord::FindFacture(ulong l)
{
	set<ulong>::iterator it= m_setFactureList.find(l);
	if (it!=m_setFactureList.end())
	{
		return true;
	}
	return false;
}

bool CDeputyOccuRecord::DeleteFacture(ulong l)
{
	set<ulong>::iterator it= m_setFactureList.find(l);
	if (it!=m_setFactureList.end())
	{
		m_setFactureList.erase(it);
		return true;
	}
	return false;
}
//���������Ŀ
bool CDeputyOccuRecord::CleanFacture()
{
	m_setFactureList.clear();
	return true;
}