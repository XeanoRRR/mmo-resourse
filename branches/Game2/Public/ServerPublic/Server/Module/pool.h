
/*
*	file:		pool.h
*	Brief:		���̰߳�ȫ�ڴ��MemPool
*	detail:		������ö���μ�pooldef.h�ļ�
*				
*				
*	Authtor:	�ſ���
*	Datae:		2007
*/


#include "../BaseDef/pooldef.h"

#pragma once


//! �ڴ����
/////////////////////////////////////////////////////////////////////////////
class MemPool
{
#pragma region �����ʼ��

public:
	//! ���캯��
	//!	��ʼ����Ԥ�����ڴ�
	MemPool(void);
	//! ��������
	~MemPool(void);

public:
	//!				Ĭ�ϳ�ʼ������
	//!				��Ĭ�ϵķ�ʽԤ�����ڴ�
	void			DefaultInit(void);

	//!				ָ��һ���ߴ���ڴ�Ԥ�ȷ�����ٿ�
	bool			SetInitialTileNum(long lSize, long lNum);

	//!				�ͷ������ڴ�
	void			Release(void);


#pragma  endregion


#pragma region ����Ӧ��

public:
	//! 			�õ�һ��ָ����С���ڴ�
	//! 			���ڴ汻0���
	void*			Alloc(long lSize);

	//!				�ͷ�һ��ָ����С���ڴ�
	bool			Free(void *pBuf, long lSize);


#pragma  endregion


#pragma region ˽�г�Ա

private:
	//!				Ϊĳһ�����ڴ�����һ���ڴ�
	bool			AddBlock(long lIndex, long lTileNum = 0);

	//!				�ͷ�һ���ߴ������������ڴ��
	void			FreeAllBlockByIndex(long lIndex);

	//!				�õ�һ���ߴ������
	long			GetSizeIndex(long lSize);

private:
	struct tagBlockNode
	{
		long lBlockSize;
	};

	struct tagHeap
	{
		long					_lTileNum;		//! ��Ƭ��
		long					_lLastAddNum;	//! ��һ����ӵ�Ƭ��
		MyList<void>			_TileFreeList;	//! δʹ�õ��ڴ�Ƭ�б�
		MyList<tagBlockNode>	_BlockList;		//! ��������ڴ���б�

		tagHeap(void):_lTileNum(0),_lLastAddNum(0){}
	};

	tagHeap						m_arrFreeList[MAX_TILE_BUF_SZIE];

#ifdef MEM_POOL_DEBUG
	set<LPVOID>					m_setBigMem;
#endif

#pragma endregion


};

