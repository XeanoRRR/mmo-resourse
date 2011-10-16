
#include "../Lock.h"
#include "ObjectFactory.h"

#ifndef _TOOLS_H_
#define _TOOLS_H_


namespace XM_Tools
{

#ifndef USE_IN_SYNC_SYSTEM
	#define	AsyncLock	LockOfWinWhileSleep
	#define GET_CLASS_NAME(_TClassName)	(""#_TClassName)
#else
	#define	AsyncLock	LockOfWinWhileSleep
	#define GET_CLASS_NAME(_TClassName)	(""#_TClassName)
#endif

	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! �����ڴ����
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define						__SET_OUT_INFO_SETUP__(Param_OutFun, Param_FileName)	{\
																							GetObjInfoManageLock<AsyncLock>().Lock();\
																							GetObjInfoManage().SetOutInfoSetup((Param_OutFun), (Param_FileName));\
																							GetObjInfoManageLock<AsyncLock>().UnLock();\
																							GetMemManage<AsyncLock>().SetOutInfoSetup((Param_OutFun), (Param_FileName));\
																						}

#ifdef LOG_ALLOC_FREE_INFO

	//! �ڴ����
	#define						__ALLOC__(_PSize)										GetMemManage<AsyncLock>().AsyncAlloc((_PSize), _FILE_AND_LINE_)
	#define						__FREE__(Param_Pointer, Param_Size)						do{\
																							if(Param_Pointer)\
																							{\
																								GetMemManage<AsyncLock>().AsyncFree(((void*)Param_Pointer), (Param_Size), _FILE_AND_LINE_);\
																								Param_Pointer = NULL;\
																							}\
																						} while(false)

	
	#define						__OUT_RUNNING_INFO__(Param_Detailed)					{\
																							GetObjInfoManageLock<AsyncLock>().Lock();\
																							GetObjInfoManage().OutRunTimeObjInfo();\
																							GetObjInfoManageLock<AsyncLock>().UnLock();\
																							GetMemManage<AsyncLock>().OutRunTimeObjInfo(Param_Detailed);\
																						}
	
#else

	//! �ڴ����
	#define						__ALLOC__(_PSize)										GetMemManage<AsyncLock>().AsyncAlloc((_PSize))

	#ifdef FREE_CHECK
		#define						__FREE__(Param_Pointer, Param_Size)						do{\
																								if(Param_Pointer)\
																								{\
																									GetMemManage<AsyncLock>().AsyncFree(((void*)Param_Pointer), (Param_Size), _FILE_AND_LINE_);\
																									Param_Pointer = NULL;\
																								}\
																							} while(false)

		#define						__FREE_INFO__(Param_Pointer, Param_Size, Param_Info)	do{\
																								if(Param_Pointer)\
																								{\
																									GetMemManage<AsyncLock>().AsyncFree(((void*)Param_Pointer), (Param_Size), (Param_Info));\
																									Param_Pointer = NULL;\
																								}\
																							} while(false)

	#else
		#define						__FREE__(Param_Pointer, Param_Size)						do{\
																								if(Param_Pointer)\
																								{\
																									GetMemManage<AsyncLock>().AsyncFree(((void*)Param_Pointer), (Param_Size));\
																									Param_Pointer = NULL;\
																								}\
																							} while(false)

		#define						__FREE_INFO__(Param_Pointer, Param_Size, Param_Info)	do{\
																								if(Param_Pointer)\
																								{\
																									GetMemManage<AsyncLock>().AsyncFree(((void*)Param_Pointer), (Param_Size));\
																									Param_Pointer = NULL;\
																								}\
																							} while(false)

	#endif

	#define						__OUT_RUNNING_INFO__(Param_Detailed)

#endif


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! ���ڴ�ع���Ķ���
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//! �����MM_DEBUG_NEW��������operator new/delete��ʹ�������һЩ���еĲ������ͻ�ӿڵ��ã�������ϵͳ�������ⶨ���operator new/delete��
	//!	����û�м̳���CMemObject����ʹ��MP_NEW����̳�CMemObject�Ķ���ʹ��new/delete������������������
	#define MM_DEBUG_NEW
	//! new/delete
#ifdef LOG_ALLOC_FREE_INFO

	#ifdef MM_DEBUG_NEW
		#define						__NEW__													new(_FILE_AND_LINE_, CMemObject::CDebugNew())
	#else
		#define						__NEW__													new(_FILE_AND_LINE_)
	#endif
#else
	#define						__NEW__													new
#endif

	#ifdef MM_DEBUG_NEW
		#define						__SAFE_DELETE__(Param_Pointer)							if(NULL != (Param_Pointer)){(Param_Pointer)->TestNothing(); delete (Param_Pointer);(Param_Pointer) = NULL;}
		#define						__SAFE_DELETE_ARRAY__(Param_Pointer)					if(NULL != (Param_Pointer)){(Param_Pointer)->TestNothing(); delete (Param_Pointer);(Param_Pointer) = NULL;}
	#else
		#define						__SAFE_DELETE__(Param_Pointer)							if(NULL != (Param_Pointer)){delete (Param_Pointer);(Param_Pointer) = NULL;}
		#define						__SAFE_DELETE_ARRAY__(Param_Pointer)					if(NULL != (Param_Pointer)){delete[] (Param_Pointer);(Param_Pointer) = NULL;}
	#endif


	//! ���ڴ�ع���Ķ������
	class CMemObject
	{
	public:
		virtual ~CMemObject(void){}

#ifdef LOG_ALLOC_FREE_INFO

		#ifdef MM_DEBUG_NEW
		
		//! ���ڲ���MP_NEW�����Ƿ���ȷ������
		class CDebugNew
		{
		};
		//!
		static void* operator new(size_t Size, const char *pInfo, CDebugNew &DebugNew);
		static void* operator new[](size_t Size, const char *pInfo, CDebugNew &pDebugNew);

		//!
		static void operator delete(void *pMem, const char*, CDebugNew &);
		static void operator delete[](void *pMem, const char*, CDebugNew &);

		#else
		//!
		static void* operator new(size_t Size, const char *pInfo);
		static void* operator new[](size_t Size, const char *pInfo);

		//!
		static void operator delete(void *pMem, const char*);
		static void operator delete[](void *pMem, const char*);

		#endif

#else
		//!
		static void* operator new(size_t Size);
		static void* operator new[](size_t Size);
#endif

		#ifdef MM_DEBUG_NEW

		//!	���ڲ���MP_DELETE�����Ƿ���ȷ�Ľӿ�
		void	TestNothing(void){}

		#endif

		//!
		static void operator delete(void *pMem);
		static void operator delete[](void *pMem);

		
	};



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! ���󹤳�
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef LOG_ALLOC_FREE_INFO
	#define						__OBJ_ALLOC__(_TClassName)								GetFactory<_TClassName, AsyncLock>(GET_CLASS_NAME(_TClassName)).CreateObj(GET_CLASS_NAME(_TClassName), _FILE_AND_LINE_)
	#define						__OBJ_ALLOC_PVOID__(_TClassName, Param_ConstructParam)	GetFactory<_TClassName, AsyncLock>(GET_CLASS_NAME(_TClassName)).CreateObj((Param_ConstructParam), GET_CLASS_NAME(_TClassName), _FILE_AND_LINE_)
	#define						__OBJ_FREE__(_TClassName, Param_Pointer)				do{\
																							GetFactory<_TClassName, AsyncLock>(GET_CLASS_NAME(_TClassName)).ReleaseObj(dynamic_cast<_TClassName*>(Param_Pointer), GET_CLASS_NAME(_TClassName), _FILE_AND_LINE_);\
																							Param_Pointer = NULL;\
																						} while(false)
#else
	#define						__OBJ_ALLOC__(_TClassName)								GetFactory<_TClassName, AsyncLock>().CreateObj()
	#define						__OBJ_ALLOC_PVOID__(_TClassName, Param_ConstructParam)	GetFactory<_TClassName, AsyncLock>().CreateObj((Param_ConstructParam))

	#ifdef FREE_CHECK
		#define						__OBJ_FREE__(_TClassName, Param_Pointer)					do{\
																									GetFactory<_TClassName, AsyncLock>().ReleaseObj(dynamic_cast<_TClassName*>(Param_Pointer), _FILE_AND_LINE_);\
																									Param_Pointer = NULL;\
																								} while(false)

		#define						__OBJ_FREE_INFO__(_TClassName, Param_Pointer, Param_Info)	do{\
																									GetFactory<_TClassName, AsyncLock>().ReleaseObj(dynamic_cast<_TClassName*>(Param_Pointer), (Param_Info));\
																									Param_Pointer = NULL;\
																								} while(false)

	#else
		#define						__OBJ_FREE__(_TClassName, Param_Pointer)				do{\
																								GetFactory<_TClassName, AsyncLock>().ReleaseObj(dynamic_cast<_TClassName*>(Param_Pointer));\
																								Param_Pointer = NULL;\
																							} while(false)

		#define						__OBJ_FREE_INFO__(_TClassName, Param_Pointer, Param_Info)	__OBJ_FREE__(_TClassName, (Param_Pointer));
	#endif
#endif
};






#endif //! _TOOLS_H_