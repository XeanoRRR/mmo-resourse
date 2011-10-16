//=============================================================================
/**
 *  file: DataBlock.cpp
 *
 *  Brief:�й̶���С���ڴ�죬��̬�������ݿ�Ļ���������
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-6-13
 */
//=============================================================================
#include "StdAfx.h"
#include "DataBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDataBlock::CDataBlock(long maxsize)
:m_nMaxSize(maxsize)
,m_nCurSize(0)
,m_bInitData(false)
{
	m_pBase = new uchar[maxsize];
	if(NULL == m_pBase)
	{
		Log4c::Warn(ROOT_MODULE, "�ں���CDataBlock::CDataBlock(...),�����ڴ����");
	}
}

CDataBlock::~CDataBlock(void)
{
	SAFE_DELETE_ARRAY(m_pBase);
}

void CDataBlock::SetCurSize(long lSize)
{
	m_nCurSize = (lSize > 0 && lSize <= m_nMaxSize) ? lSize : 0;
}

/*
* ʹ���ڴ�ع���Ĵ���

CDataBlock::CDataBlock(long maxsize)
:m_nMaxSize(maxsize)
,m_nCurSize(0)
,m_bInitData(false)
{
m_pBase = (uchar*)M_ALLOC(m_nMaxSize);
if(NULL == m_pBase)
{
Log4c::Trace(ROOT_MODULE,"�ں���CDataBlock::CDataBlock(...),�����ڴ����");
}
}

CDataBlock::CDataBlock(void *maxsize)
:m_nMaxSize((long)maxsize)
,m_nCurSize(0)
,m_bInitData(false)
{
m_pBase = (uchar*)M_ALLOC(m_nMaxSize);
if(NULL == m_pBase)
{
Log4c::Trace(ROOT_MODULE, "�ں���CDataBlock::CDataBlock(...),�����ڴ����");
}
}

CDataBlock::~CDataBlock(void)
{
M_FREE(m_pBase, m_nMaxSize);
}

void CDataBlock::SetCurSize(long lSize)
{
m_nCurSize = (lSize > 0 && lSize <= m_nMaxSize) ? lSize : 0;
}
*/