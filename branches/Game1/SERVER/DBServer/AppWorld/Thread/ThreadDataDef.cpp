//ThreadDataDef.cpp/////////////////////////////////////////////////////////////////////
//������:�ö�����һ�������߳�Ͷ�ݵ������߳�ʹ�õ���Ϣ����
//Author:������
//Create Time:2008.11.03
#include "StdAfx.h"
#include "../entity/entityManager.h"
#include "ThreadDataDef.h"
#include "../entity/entitygroup.h"

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
