


/*
�ڴ˶���MemManage�õ��ģ�
	��������
	��������
	��
	ö��

*/

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
	#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#pragma warning(disable:4996; disable:4311; disable:4312; disable:4345)

#include <map>
#include <string>
#include <Windows.h>


#ifndef _MEM_MANAGE_DEF_
#define _MEM_MANAGE_DEF_



namespace XM_Tools
{
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//! ��
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef NULL
	#define						NULL							0
#endif

	//!							����ʱ�ͷ�ϵͳ�ڴ�
//#define							DESTRUTOR_FREE_SYSMEM

//!								����ʱ����ڴ�
//#define							FILL_MEM
//!								���Կ���
//#define							ASSERT_ENABLE


#ifndef MEM_LOG_FILE
	#define MEM_LOG_FILE			"log\mem_info.log"
#endif //! MEM_LOG_FILE
	
//!								��¼���䡢�ͷ���Ϣ
//#define							LOG_ALLOC_FREE_INFO

//!								ʹ��Ƭ����
#define							USE_TILE_MANAGE
//!								ʹ�ÿ����
//#define							USE_BLOCK_MANAGE
//!								ʹ�öѹ���
//#define							USE_HEAP_MANAGE



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ASSERT_ENABLE
	#define						MM_ASSERY(bValue)				if(!(bValue))throw
	#define						FILL_VALUE						(0xbb)
#else
	#define						MM_ASSERY(bValue)
	#define						FILL_VALUE						(0)
#endif

	//!								�ڷ���һ��ֵǰ�׳��쳣���ܿ���ASSERT_ENABLE��
#define							RETURN_THROW(ReturnValue)		{MM_ASSERY(false);return (ReturnValue);}
#define							RETURN_THROW_VOID				{MM_ASSERY(false);}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef LOG_ALLOC_FREE_INFO

	#define						FILE_LINE_TEMP2(x, y)			"file["x"]��line["#y"]"
	#define						FILE_LINE_TEMP(x, y)			FILE_LINE_TEMP2(x, y)
	#define						_FILE_AND_LINE_					FILE_LINE_TEMP(__FILE__, __LINE__)

	//!							������־��¼��һ���꣨�������������������ڵ��ļ������к����ӳ�һ���ַ�����
	#define						LINK_OBJ_INFO(ClassName)		(#ClassName ## _FILE_AND_LINE_ )

#else

	#define						_FILE_AND_LINE_					NULL

	//!							������־��¼��һ���꣨�������������������ڵ��ļ������к����ӳ�һ���ַ�����
	#define						LINK_OBJ_INFO(ClassName)		NULL

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! ���ֶ���
	inline unsigned long		SNAP_TO_GRID(unsigned long uSrcNum, unsigned long uGrid)		
	{
		return (uSrcNum / uGrid + ((uSrcNum % uGrid) ? 1 : 0)) * uGrid;
	}

	//! ��Ϣ�����������
	typedef	void				(*OutInfoFun)(const char *pFileName, const char *pText)	;

	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//! ��������
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	#ifndef SHRINK_SCALE
		#define					SHRINK_SCALE					0
	#endif
	//! ������
	#ifndef CACHE_LINES
		#define					CACHE_LINES						64
	#endif

	//!							��Ϣ�����С
	const unsigned long			INFO_BUF_SIZE					= 1024 * 8;

	//!							CPU��byte��
	const unsigned long 		CPU_SIZE						= sizeof(long);
	//!							��С���ȣ�byte�������ֵ����趨ΪCPU��λ���������2��N���ݣ�
	const unsigned long			MIN_GRANULARITY					= CPU_SIZE * 2;
	//!							�ڴ������ʼֵ��byte�������ֵ����趨ΪCPUλ���2�����������2��N���ݣ�
	const unsigned long 		MIN_ALLOC_SIZE					= CPU_SIZE * 2;
	//!							������ȣ�����ڴ���������
	const unsigned long			MAX_GRANULARITY					= 1024 * 1024 * 1024;



	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//! С���ڴ����
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	//!							�ߴ����
	const unsigned char 		TILE_POOL_NUM					= 32;
	//!							����ߴ�
	const unsigned long 		TILE_MAX_SIZE					= TILE_POOL_NUM * MIN_GRANULARITY;
	//!							Ĭ�Ͻ���һ�ο��п����ʱ�����ӵĿ�ʹ���ڴ�
	const unsigned long 		TILE_DEFAULT_ADD_SIZE			= (1024 * 1024 * 4) >> SHRINK_SCALE;


	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//! ����ڴ����
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//!							�������С
	const unsigned long 		BLOCK_BSAE_SIZE					= TILE_MAX_SIZE * CPU_SIZE;
	//!							Ĭ�������׼��������������CPU���������
	const unsigned long 		BLOCK_DEFAULT_PROVIDE_COUNT		= ( MAX_GRANULARITY / BLOCK_BSAE_SIZE ) >> SHRINK_SCALE;
	//!							�������������ڴ�
	const unsigned long			BLOCK_MAX_ALLOC_SIZE			= 1024 * 512;


	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//! ���ڴ����
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//!							�ѵ���С�������ȣ�byte��
	const unsigned long			HEAP_MIN_GRANULARITY			= BLOCK_BSAE_SIZE * CPU_SIZE;
	//!							����������ڴ����
	const unsigned long			MAX_MANAGE_COUNT				= 128;
	//!							ÿ���ͷ�8����
	const unsigned long			FREE_COUNT						= 8;


	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//! �������
	//!------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//!							
	const unsigned long			CLASS_NAME_SIZE					= 512;


};



#endif // _MEM_MANAGE_DEF_