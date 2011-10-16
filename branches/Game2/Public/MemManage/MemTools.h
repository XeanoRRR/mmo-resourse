


/**
*	�ڴ��������
*		ͨ������USE_IN_SYNC_SYSTEM�꣬ȷ���ڴ����ϵͳ����ͬ�������첽
*		ͨ������SHRINK_SCALE���ֵ�����Լ���Ԥ�����ڴ�Ĵ�С�����ֵ����Ӧ��0~31֮�䣬Ԥ������ڴ潫���ٵ�2��SHRINK_SCALE�η���
*		ͨ������USE_C_LIB_ALLOC�꣬���Խ��ڴ����������C��malloc\free������ģ��ֻ�������Թ���
*
*	�ڴ�����߷�Ϊ�ĸ����֣���Ϣ���������ڴ�������ڴ�ع���Ķ��󡢶��󹤳�ģ��
*		���У������������䲿�ֵķ�����ͷŶ�������ԳɶԵ��ã����ܻ�����
*/


#include "include/Tools.h"

using namespace XM_Tools;

#ifndef _MEM_TOOLS_H_
#define _MEM_TOOLS_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ��Ϣ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!					���������Ϣ������Param_OutFun����Ϊ��void OutInfoFun(const char *pFileName, const char *pText)��ʽ��
#define				SET_OUT_INFO_SETUP(Param_OutFun, Param_FileName)				__SET_OUT_INFO_SETUP__((Param_OutFun), (Param_FileName))
//!					�������ʱ��Ϣ
#define				OUT_RUNNING_INFO(Param_Detailed)								__OUT_RUNNING_INFO__(Param_Detailed)



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! �����ڴ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//!					�첽�����ڴ�
//!					��Param_Size ���ͣ�unsigned long��
#define				M_ALLOC(Param_Size)												__ALLOC__(Param_Size)
//!					�첽�ͷ��ڴ�
//!					��Param_Pointer ���ͣ�void*��Param_Size ���ͣ�unsigned long��
#define				M_FREE(Param_Pointer, Param_Size)								__FREE__((Param_Pointer), (Param_Size))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ���ڴ�ع���Ķ���
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define				MP_NEW															__NEW__
#define				MP_DELETE(Param_Pointer)										__SAFE_DELETE__(Param_Pointer)
#define				MP_DELETE_ARRAY(Param_Pointer)									__SAFE_DELETE_ARRAY__(Param_Pointer)

//!					����Ҫ���ڴ�ع�����Ӵ��࿪ʼ�̳�
typedef 			CMemObject														BaseMemObj;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ���󹤳�ģ��
//! �м̳й�ϵ���󣬱�������������������������������ȫ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!					���������޲ι��캯����
//!					���������ͣ�class/struct��
#define				OBJ_CREATE(_TClassName)											__OBJ_ALLOC__(_TClassName)
//!					��������LPVOID���Ͳ������캯����
//!					��_TClassName��class/struct��Param_ConstructParam��LPVOID���Ͳ���ָ�룩
#define				OBJ_CREATE_PVOID(_TClassName, Param_ConstructParam)				__OBJ_ALLOC_PVOID__(_TClassName, (Param_ConstructParam))
//!					�ͷŶ���
#define				OBJ_RELEASE(_TClassName, Param_Pointer)							__OBJ_FREE__(_TClassName, (Param_Pointer))


#endif //! _MEM_TOOLS_H_