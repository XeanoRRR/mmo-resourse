/**
*File:  ThreadDataDef.cpp
*brief: ������:�ö�����һ�������߳�Ͷ�ݵ������߳�ʹ�õ���Ϣ����
*Author:������
Date:   2008.11.03
*/
#include "StdAfx.h"
#include "ThreadDataDef.h"

#include "../Entity/Entitygroup.h"
#include "../Entity/EntityManager.h"

//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ThreadData::ThreadData()
:lThreadOpType(0),retFlag(0),pEntity(NULL)
{
	
}
ThreadData::~ThreadData()
{
	SAFE_DELETE(pEntity);
}
ThreadData& ThreadData::operator=(const ThreadData& right)
{
	if(this != &right)
	{
		lThreadOpType = right.lThreadOpType;
		retFlag = right.retFlag;
		pEntity = right.pEntity;
	}

	return *this;
}
