


/*
С���ڴ��
	һ����ֻ����ͬһ���ߴ���ڴ�����
	��������ʽʵ��
*/

#include "MemList.h"


#ifndef _TILE_POOL_H_
#define _TILE_POOL_H_


namespace XM_Tools
{
	template<typename _TLock, typename _TAllocator>
	class CTilePool
	{
	public:
		inline CTilePool(_TAllocator &Allocator, unsigned long uDefaultAddSize = TILE_DEFAULT_ADD_SIZE);
		inline ~CTilePool(void);

	public:
		//!											��ʼ������
		inline bool									InitForSize(unsigned long uSize, unsigned long uProvideCount);
		//!
		inline void									Release(void);

		//!											�������ʱ��Ϣ
		void										OutInfo(char *pInfoBuf);

	public:
		//!
		inline void*								Alloc(unsigned long uSize);
		//!
		inline void									Free(void *pMem);


	private:
		//! ����һ���ߴ��ڴ�Ľṹ
		struct tagTileManageInfo 
		{
			//! �������������
			unsigned long uIndex;
			//!	�ܴ�С������������Ϣ��
			unsigned long uSize;
			//! �ֿ�����
			unsigned long uTileSum;
			//! ��ʹ�ÿ���
			unsigned long uUseCount;
			//! �ϴ�����Ŀ���
			unsigned long uLastAllocCount;
			//!
			_TLock InfoLock;
			//! �ѻ�õ�ϵͳ�ڴ�
			CMemList<void, _TLock> SysMemList;
			//! ��������
			CMemList<unsigned char, _TLock> FreeList;
		};

	private:		
		//!											�õ��ߴ���õ�����
		inline unsigned long						GetListIndex(unsigned long uSize);
		//!											�õ�Ĭ��һ�����ӵĿ���
		inline unsigned long						GetDefaultAddCount(unsigned long uIndex);
		//!											�����µĿ��п�
		inline bool									AddNewTile(tagTileManageInfo &TileManageInfo, unsigned long uAddCount);

	private:
		//!											
		_TAllocator									&m_Allocator;
		//!											
		const unsigned long							m_uDefaultAddSize;
		//!											С���ڴ��ڴ������
		tagTileManageInfo							m_arrMemList[TILE_POOL_NUM];

#ifdef LOG_ALLOC_FREE_INFO
	public:
	private:
#endif

	};
};


#include "../cpp/TilePool.cpp"

#endif //! _TILE_POOL_H_