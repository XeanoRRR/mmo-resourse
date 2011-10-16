


/*
	�ڴ��������
		ͨ������USE_IN_SYNC_SYSTEM�꣬ȷ���ڴ����ϵͳ����ͬ�������첽
		ͨ������SHRINK_SCALE���ֵ�����Լ���Ԥ�����ڴ�Ĵ�С�����ֵ��ö���Ϊ2�Ĵη�������Ĭ������£�Ԥ�����ڴ��СԼΪ1.1GB
*/


#include "include/Tools.h"

using namespace XM_Tools;

#ifndef _MEM_TOOLS_H_
#define _MEM_TOOLS_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! �ڴ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!					���������Ϣ������Param_OutFun����Ϊ��void OutInfoFun(const char *pFileName, const char *pText)��ʽ��
#define				SET_OUT_INFO_SETUP(Param_OutFun, Param_FileName)				__SET_OUT_INFO_SETUP__((Param_OutFun), (Param_FileName))
//!					�������ʱ��Ϣ
#define				OUT_RUNNING_INFO()												__OUT_RUNNING_INFO__


	#ifndef USE_IN_SYNC_SYSTEM

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! �ڴ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//!					�첽�����ڴ�
//!					��Param_Size ���ͣ�unsigned long��
#define				M_ALLOC(Param_Size)												__ASYNC_ALLOC__(Param_Size)
//!					�첽�ͷ��ڴ�
//!					��Param_Pointer ���ͣ�void*��Param_Size ���ͣ�unsigned long��
#define				M_FREE(Param_Pointer, Param_Size)								__ASYNC_FREE__((Param_Pointer), (Param_Size));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ���󹤳��ӿ�
//!		�÷���
//!			class CTestClase;
//!				�޲ι���	OBJECT_FACTORY(CTestClase).CreateObj(void);
//!				��������	OBJECT_FACTORY(CTestClase).CreateObj(void* pConstructParam);
//!				�ͷ�		OBJECT_FACTORY(CTestClase).ReleaseObj(CTestClase *pCTestClase);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//!					�õ����󹤳�
//!					���������ͣ�class/struct��
#define				OBJECT_FACTORY(_TClassName)										__ASYNC_OBJECT_FACTORY__(_TClassName)

	#else //! USE_IN_SYNC_SYSTEM

	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! �ڴ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//!					ͬ�������ڴ�
//!					��Param_Size ���ͣ�unsigned long��
#define				M_ALLOC(Param_Size)												__SYNC_ALLOC__(Param_Size)
//!					ͬ���ͷ��ڴ�
//!					��Param_Pointer ���ͣ�void*��Param_Size ���ͣ�unsigned long��
#define				M_FREE(Param_Pointer, Param_Size)								__SYNC_FREE__((Param_Pointer), (Param_Size));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ���󹤳��ӿ�
//!		�÷���
//!			class CTestClase;
//!			ͬ��:
//!				�޲ι���	SYNC_OBJECT_FACTORY(CTestClase).CreateObj(void);
//!				��������	SYNC_OBJECT_FACTORY(CTestClase).CreateObj(void* pConstructParam);
//!				�ͷ�		SYNC_OBJECT_FACTORY(CTestClase).ReleaseObj(CTestClase *pCTestClase);
//!			�첽:
//!				�޲ι���	ASYNC_OBJECT_FACTORY(CTestClase).CreateObj(void);
//!				��������	ASYNC_OBJECT_FACTORY(CTestClase).CreateObj(void* pConstructParam);
//!				�ͷ�		ASYNC_OBJECT_FACTORY(CTestClase).ReleaseObj(CTestClase *pCTestClase);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//!					�õ����󹤳�
//!					���������ͣ�class/struct��
#define				OBJECT_FACTORY(_TClassName)										__SYNC_OBJECT_FACTORY__(_TClassName)



	#endif //! USE_IN_SYNC_SYSTEM

#endif //! _MEM_TOOLS_H_