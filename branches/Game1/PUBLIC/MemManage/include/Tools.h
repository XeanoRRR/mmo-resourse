
#include "../Lock.h"
#include "ObjectFactory.h"

#ifndef _TOOLS_H_
#define _TOOLS_H_


namespace XM_Tools
{

	typedef LockOfWinWhile AsyncLock;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! �ڴ����
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _LOG_ALLOC_FILE_INFO_
	//!							ͬ�������ڴ�
	void*						SyncAlloc(unsigned long uSize, const char *pInfo);
	//!							�첽�����ڴ�
	void*						AsyncAlloc(unsigned long uSize, const char *pInfo);
	
#else
	//!							ͬ�������ڴ�
	void*						SyncAlloc(unsigned long uSize);
	//!							�첽�����ڴ�
	void*						AsyncAlloc(unsigned long uSize);

#endif

	//!							ͬ���ͷ��ڴ�
	void						SyncFree(void *pMem, unsigned long uSize);
	//!							�첽�ͷ��ڴ�
	void						AsyncFree(void *pMem, unsigned long uSize);


#ifdef _LOG_ALLOC_FILE_INFO_
	#define						__SYNC_ALLOC__(_PSize)									SyncAlloc((_PSize), _FILE_AND_LINE_);	
	#define						__ASYNC_ALLOC__(_PSize)									AsyncAlloc((_PSize), _FILE_AND_LINE_);
#else
	#define						__SYNC_ALLOC__(_PSize)									SyncAlloc(_PSize);
	#define						__ASYNC_ALLOC__(_PSize)									AsyncAlloc(_PSize)
#endif

	#define						__SYNC_FREE__(Param_Pointer, Param_Size)				SyncFree((Param_Pointer), (Param_Size));
	#define						__ASYNC_FREE__(Param_Pointer, Param_Size)				AsyncFree((Param_Pointer), (Param_Size));


#ifndef USE_IN_SYNC_SYSTEM

	#define						__SET_OUT_INFO_SETUP__(Param_OutFun, Param_FileName)	{\
																							GetObjInfoManageLock<AsyncLock>().Lock();\
																							GetObjInfoManage().SetOutInfoSetup((Param_OutFun), (Param_FileName));\
																							GetObjInfoManageLock<AsyncLock>().UnLock();\
																							GetMemManage<AsyncLock>().SetOutInfoSetup((Param_OutFun), (Param_FileName));\
																						}

	#define						__OUT_RUNNING_INFO__									{\
																							GetObjInfoManageLock<AsyncLock>().Lock();\
																							GetObjInfoManage().OutRunTimeObjInfo();\
																							GetObjInfoManageLock<AsyncLock>().UnLock();\
																							GetMemManage<AsyncLock>().OutRunTimeObjInfo();\
																						}
#else

	#define						__SET_OUT_INFO_SETUP__(Param_OutFun, Param_FileName)	{\
																							GetObjInfoManage().SetOutInfoSetup((Param_OutFun), (Param_FileName));\
																							GetMemManage<InvalidLock>().SetOutInfoSetup((Param_OutFun), (Param_FileName));\
																						}

	#define						__OUT_RUNNING_INFO__									{\
																							GetObjInfoManage().OutRunTimeObjInfo();\
																							GetMemManage<InvalidLock>().OutRunTimeObjInfo();\
																						}

#endif

	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! ���󹤳��ӿ�
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//!							�õ����󹤳�
#define							__SYNC_OBJECT_FACTORY__(_TClassName)		GetFactory<_TClassName, InvalidLock>()
	//!							�õ����󹤳�
#define							__ASYNC_OBJECT_FACTORY__(_TClassName)		GetFactory<_TClassName, AsyncLock>()



};



#endif //! _TOOLS_H_