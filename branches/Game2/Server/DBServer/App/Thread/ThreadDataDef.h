/**
*File:  ThreadDataDef.h
*brief: ������:�ö�����һ�������߳�Ͷ�ݵ������߳�ʹ�õ���Ϣ����
*Author:������
Date:   2008.11.03
*/
#ifndef THREAD_DATA_DEF_H
#define THREAD_DATA_DEF_H


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