

#include "MemList.h"
#include "HeapPool.h"
#include "BlockPool.h"
#include "TilePool.h"


#ifndef _MEM_MANAGE_H_
#define _MEM_MANAGE_H_


namespace XM_Tools
{

	/*
	============================================================================
	============================================================================
	�ڴ������
	============================================================================
	============================================================================
	*/
	template<typename _TLock>
	class CMemManage
	{
	public:
		CMemManage(void);
		~CMemManage(void);

	public:
		//!											��ʼ��
		unsigned long								Init(void);
		//!											�ͷ�
		void										Release(void);

		//!											������Ϣ������������·��
		void										SetOutInfoSetup(OutInfoFun pOutInfoFun, const char *pFileName);
		//!											����ʱ��Ϣ���
		void										OutRunTimeObjInfo(void);

	public:
#ifdef _LOG_ALLOC_FILE_INFO_
		//!											�õ�һ���ڴ�
		void*										AsyncAlloc(unsigned long lSize, const char *pInfo);
#else
		//!											�õ�һ���ڴ�
		void*										AsyncAlloc(unsigned long lSize);
#endif
		//!											�ͷ�һ���ڴ�
		void										AsyncFree(void* pBuf, unsigned long lSize);


	private:
		//! �ڴ���Դ����
		enum eMemSourceType
		{
			//! ϵͳC����malloc����
			eMST_System_malloc,
			//! �ڴ�������Ķѳط���
			eMST_MemManage_Heap,
			//! �ڴ�������Ŀ�ط���
			eMST_MemManage_Block,
			//! �ڴ��������Ƭ�ط���
			eMST_MemManage_Tile,
		};

	private:
		typedef CHeapPool<_TLock>					THeapPool;
		typedef CBlockPool<_TLock, THeapPool>		TBlockPool;
		typedef CTilePool<_TLock, THeapPool>		TTilePool;

		//!											�ѳ�
		THeapPool									m_HeapPool;
		//!											���
		TBlockPool									m_BlockPool;
		//!											Ƭ��
		TTilePool									m_TilePool;

#ifdef _LOG_ALLOC_FILE_INFO_
		//!											�ѷ����ڴ�������Ϣ����
		std::map<void*, std::string>				m_mapAllocInfo;
		//!											����m_mapAllocInfo����
		_TLock										m_InfoLock;

#endif

		//!											�������
		OutInfoFun									m_OutInfoFun;
		//!											����ļ�
		const char									*m_pOutFile;

	};


	/*
	============================================================================
	============================================================================
	�ڴ������
	============================================================================
	============================================================================
	*/
	//!							��ȡ�ڴ�������
	template<typename _TLock>
	CMemManage<_TLock>&			GetMemManage(void);
};

#include "../cpp/MemManage.cpp"

#endif //! _MEM_MANAGE_H_