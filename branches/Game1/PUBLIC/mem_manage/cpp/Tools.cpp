

#include "../include/Tools.h"
#include "../include/ObjectFactory.h"


namespace XM_Tools
{
#ifdef _LOG_ALLOC_FILE_INFO_
	//! ͬ�������ڴ�
	void* SyncAlloc(unsigned long uSize, const char *pInfo)
	{
		return GetMemManage<InvalidLock>().AsyncAlloc(uSize, pInfo);
	}

	//! �첽�����ڴ�
	void* AsyncAlloc(unsigned long uSize, const char *pInfo)
	{
		return GetMemManage<AsyncLock>().AsyncAlloc(uSize, pInfo);;
	}

#else
	//! ͬ�������ڴ�
	void* SyncAlloc(unsigned long uSize)
	{
		return GetMemManage<InvalidLock>().AsyncAlloc(uSize);
	}
	//! �첽�����ڴ�
	void* AsyncAlloc(unsigned long uSize)
	{
		return GetMemManage<AsyncLock>().AsyncAlloc(uSize);
	}


#endif


	//! ͬ���ͷ��ڴ�
	void SyncFree(void *pMem, unsigned long uSize)
	{
		GetMemManage<InvalidLock>().AsyncFree(pMem, uSize);
	}

	//! �첽�ͷ��ڴ�
	void AsyncFree(void *pMem, unsigned long uSize)
	{
		GetMemManage<AsyncLock>().AsyncFree(pMem, uSize);
	}
	



};
