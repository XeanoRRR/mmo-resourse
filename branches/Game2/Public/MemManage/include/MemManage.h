

#include "MemList.h"
#include "HeapPool.h"
#include "BlockPool.h"
#include "TilePool.h"
#include "MemInfo.h"


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
		void										OutRunTimeObjInfo(bool Detailed);

	public:
#ifdef LOG_ALLOC_FREE_INFO
		//!											�õ�һ���ڴ�
		inline void*								AsyncAlloc(unsigned long lSize, const char *pInfo);
		//!											�ͷ�һ���ڴ�
		inline void									AsyncFree(void* pBuf, unsigned long lSize, const char *pInfo);
#else
		//!											�õ�һ���ڴ�
		inline void*								AsyncAlloc(unsigned long lSize);
		//!											�ͷ�һ���ڴ�
		inline void									AsyncFree(void* pBuf, unsigned long lSize);
#endif
		


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

#ifdef LOG_ALLOC_FREE_INFO
		//!
		CMemInfo									m_MemInfo;
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
	inline CMemManage<_TLock>&			GetMemManage(void);
};

#include "../cpp/MemManage.tcc"

#endif //! _MEM_MANAGE_H_