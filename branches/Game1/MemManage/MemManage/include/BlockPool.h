


/*
����ڴ��
	��ֻ����ͬһ���ߴ���ڴ����󣬲�ͬ�ߴ����������㹻��Ŀ����㷵��
	λͼ��ʽ����
	����̬�����ڴ棬�ڲ�����������ʱ��ֱ�ӵ���_TAllocator�����ڴ�

*/

#include "mmdef.h"

#ifndef _BLOCK_POOL_H_
#define _BLOCK_POOL_H_


namespace XM_Tools
{
	template<typename _TLock, typename _TAllocator>
	class CBlockPool
	{
	public:
		CBlockPool(_TAllocator &Allocator);
		~CBlockPool(void);

	public:
		//!								��ʼ������
		bool							Init(void);
		//!
		void							Release(void);
		//!								�������ʱ��Ϣ
		void							OutInfo(char *pInfoBuf);

	public:
		//!		
		inline void*					Alloc(unsigned long uSize);
		//!		
		inline void						Free(void *pMem);

	private:
		_TAllocator						&m_Allocator;
		//!								��ʼ�����
		unsigned long					m_uInitFlag;
		//!								ʣ��ռ�(byte)
		unsigned long					m_uFreeCount;
#ifdef LOG_ALLOC_FREE_INFO
		//!								�ط����ڴ����
		unsigned long					m_PoolAllocCount;
#endif
		//!								��¼ֱ�ӷ�����ڴ����
		unsigned long					m_uFirsthandCount;

		//!								��һ������ڴ��ƫ����
		unsigned long					m_FreeBeginOffset;
		//!								���һ������ڴ��ƫ����
		unsigned long					m_FreeEndOffset;

		//!								
		_TLock							m_FlagLock;

		//!
		unsigned char					*m_pFlag;
		//!
		unsigned char					*m_pBuf;
		
		
	private:

		
		
	};
};


#include "../cpp/BlockPool.cpp"


#endif //! _BLOCK_POOL_H_