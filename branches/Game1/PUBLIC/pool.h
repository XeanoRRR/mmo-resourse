
/*
*	file:		pool.h
*	Brief:		���̰߳�ȫ�ڴ��MemPool
*	detail:		������ö���μ�pooldef.h�ļ�
*				
*				
*	Authtor:	�ſ���
*	Datae:		2007
*/


#include "pooldef.h"

#pragma once


//! �ڴ����
/////////////////////////////////////////////////////////////////////////////
class MemPool
{
#pragma region �����ʼ��

public:
	//! ���캯��
	//!	��ʼ����Ԥ�����ڴ�
	MemPool(VOID);
	//! ��������
	~MemPool(VOID);

public:
	//!				Ĭ�ϳ�ʼ������
	//!				��Ĭ�ϵķ�ʽԤ�����ڴ�
	VOID			DefaultInit(VOID);

	//!				ָ��һ���ߴ���ڴ�Ԥ�ȷ�����ٿ�
	BOOL			SetInitialTileNum(LONG lSize, LONG lNum);

	//!				�ͷ������ڴ�
	VOID			Release(VOID);


#pragma  endregion


#pragma region ����Ӧ��

public:
	//! 			�õ�һ��ָ����С���ڴ�
	//! 			���ڴ汻0���
	VOID*			Alloc(LONG lSize);

	//!				�ͷ�һ��ָ����С���ڴ�
	BOOL			Free(VOID *pBuf, LONG lSize);


#pragma  endregion


#pragma region ˽�г�Ա

private:
	//!				Ϊĳһ�����ڴ�����һ���ڴ�
	BOOL			AddBlock(LONG lIndex, LONG lTileNum = 0);

	//!				�ͷ�һ���ߴ������������ڴ��
	VOID			FreeAllBlockByIndex(LONG lIndex);

	//!				�õ�һ���ߴ������
	LONG			GetSizeIndex(LONG lSize);

private:
	struct tagBlockNode
	{
		LONG lBlockSize;
	};

	struct tagHeap
	{
		LONG					_lTileNum;		//! ��Ƭ��
		LONG					_lLastAddNum;	//! ��һ����ӵ�Ƭ��
		MyList<VOID>			_TileFreeList;	//! δʹ�õ��ڴ�Ƭ�б�
		MyList<tagBlockNode>	_BlockList;		//! ��������ڴ���б�

		tagHeap(VOID):_lTileNum(0),_lLastAddNum(0){}
	};

	tagHeap						m_arrFreeList[MAX_TILE_BUF_SZIE];

#ifdef MEM_POOL_DEBUG
	set<LPVOID>					m_setBigMem;
#endif

#pragma endregion


};

