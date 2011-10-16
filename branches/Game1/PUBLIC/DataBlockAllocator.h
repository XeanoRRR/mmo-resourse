//=============================================================================
/**
 *  file: DataBlockAllocator.h
 *
 *  Brief:�̶���С���ڴ�������
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-6-13
 */
//=============================================================================

#ifndef __DATABLOCK_ALLOCATOR__H__
#define __DATABLOCK_ALLOCATOR__H__

#pragma once

class CDataBlock;
class CGUID;

struct tagDBOpParam
{
	//��ǰ��д���ݿ���
	long	nCurNum;
	//��ǰ��д���ݿ�Ĵ�С
	long	nCurDBSize;
	//��ǰ��д���ݵ�λ��
	long	nCurPos;
	//��ǰ��д���ݿ��ָ��
	BYTE	*pDBPtr;
};

class CDataBlockAllocator
{
public:
	CDataBlockAllocator(int nDBNum,int nDBSize,int bIniShareDB = false);
public:
	~CDataBlockAllocator(void);

private:
	typedef list<CDataBlock*>	FreeDataBlocks;
	typedef list<CDataBlock*>::iterator itFreeDB;

	FreeDataBlocks m_FreeDataBlocks;
	CRITICAL_SECTION m_CSFreeDB;
	int	m_nMaxDBNum;
	const int	m_nDBSize;
	//�����߼�����ʱʹ�õ����ݿ�
	//�������࣬ʹ�ýϴ�����ݿ���������ִ�С������
	typedef multimap<long,BYTE*>	MMapShareDB;
	typedef pair <long, BYTE*> ShareDBPair;
	typedef multimap<long,BYTE*>::iterator itShareDB;
	MMapShareDB m_ShareDataBlocks;

	typedef map<BYTE*,long>	mapAllocedRecord;
	typedef map<BYTE*,long>::iterator itAllocR;
	mapAllocedRecord	m_AllocedShareDB;
	//�������������ݿ����
	map<long,long>	m_MapTest;
private:
	void Initial(int bIniShareDB = false);
	void Release();

public:
	CDataBlock*	AllocDB(long lTestFlag=0);
	void FreeDB(CDataBlock*);

	BYTE* AllockShareDB(long lSize);
	void FreeShareDB(BYTE* pData);

	void PutAllocInfo(const char* pszFileName);
};

#endif
