//ThreadDataDef.h/////////////////////////////////////////////////////////////////////
//������:�ö�����һ�������߳�Ͷ�ݵ������߳�ʹ�õ���Ϣ����
//Author:������
//Create Time:2008.11.03
#ifndef THREAD_DATA_DEF_H
#define THREAD_DATA_DEF_H
#include "../public/GUID.h"
#include "../public/MsgType.h"
#include "../public/DBDefine.h"


class CBaseEntity;

// ���̶߳�ȡ����������Ϣ
struct ThreadData 
{
	LONG				lThreadOpType;
	LONG				retFlag;
	CBaseEntity*		pEntity;

	ThreadData();
	~ThreadData();
	ThreadData& operator=(const ThreadData& right);
};

#endif//THREAD_DATA_DEF_H