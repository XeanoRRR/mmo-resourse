


#ifndef _BLOCK_POOL_CPP_
#define _BLOCK_POOL_CPP_


#include "../include/BlockPool.h"

namespace XM_Tools
{
	//!------------------------------------------------------------------------------------------------------------------------------------------
	//! ��������
	//!------------------------------------------------------------------------------------------------------------------------------------------
	const unsigned long INIT_FLAG_VALUE = 0x35621458;

	enum eFlagType
	{
		eFT_Free,
		eFT_Head,
		eFT_Body,
	};


	//!------------------------------------------------------------------------------------------------------------------------------------------
	//! ��Աʵ��
	//!------------------------------------------------------------------------------------------------------------------------------------------
	template<typename _TLock, typename _TAllocator>
	CBlockPool<_TLock, _TAllocator>::CBlockPool(_TAllocator &Allocator)
		:m_Allocator(Allocator), m_uInitFlag(0), m_uFreeCount(0), m_uFirsthandCount(0), m_FreeBeginOffset(0xFFFFFFFF), m_FreeEndOffset(0xFFFFFFFF), m_pFlag(NULL), m_pBuf(NULL)
#ifdef LOG_ALLOC_FREE_INFO
		,m_PoolAllocCount(0)
#endif
	{

	}

	template<typename _TLock, typename _TAllocator>
	CBlockPool<_TLock, _TAllocator>::~CBlockPool(void)
	{
		Release();
	}


	//! ��ʼ������
	template<typename _TLock, typename _TAllocator>
	bool	CBlockPool<_TLock, _TAllocator>::Init(void)
	{
#ifdef USE_BLOCK_MANAGE
		m_pFlag = (unsigned char*)m_Allocator.Alloc(BLOCK_DEFAULT_PROVIDE_COUNT * (BLOCK_BSAE_SIZE + sizeof(unsigned char)));
		if (NULL == m_pFlag)
			RETURN_THROW(false);
		//! ʣ�����
		m_uFreeCount = BLOCK_DEFAULT_PROVIDE_COUNT;

		//! ��һ������ڴ��ƫ����
		m_FreeBeginOffset = 0;
		//! ���һ������ڴ��ƫ����
		m_FreeEndOffset = BLOCK_DEFAULT_PROVIDE_COUNT - 1;

		//! ��ʼ����־��
		memset(m_pFlag, eFT_Free, BLOCK_DEFAULT_PROVIDE_COUNT);

		m_pBuf = m_pFlag + BLOCK_DEFAULT_PROVIDE_COUNT;
#endif

		//! ��ʼ�����
		m_uInitFlag = INIT_FLAG_VALUE;
		return true;
	}

	//!
	template<typename _TLock, typename _TAllocator>
	void CBlockPool<_TLock, _TAllocator>::Release(void)
	{
		MM_ASSERY(0 == m_uFirsthandCount);
#ifdef LOG_ALLOC_FREE_INFO
		MM_ASSERY(0 == m_PoolAllocCount);
#endif
#ifdef DESTRUTOR_FREE_SYSMEM
		if(NULL != m_pFlag)
		{
			m_Allocator.Free(m_pFlag);
			m_pFlag = NULL;
		}
#endif
	}

	//! �������ʱ��Ϣ
	template<typename _TLock, typename _TAllocator>
	void CBlockPool<_TLock, _TAllocator>::OutInfo(char *pInfoBuf)
	{
#ifdef LOG_ALLOC_FREE_INFO
	#ifdef USE_BLOCK_MANAGE
		_snprintf(pInfoBuf, INFO_BUF_SIZE, "CBlockPool:\r\n	�ѷ��䡾%u KB�������С��%u byte��������%u���飬ʣ��%u���飬�ط��䡾%u���Σ�ֱ�ӷ��䡾%u����\r\n",
			BLOCK_DEFAULT_PROVIDE_COUNT * (BLOCK_BSAE_SIZE + sizeof(unsigned char)) / 1024, BLOCK_BSAE_SIZE, BLOCK_DEFAULT_PROVIDE_COUNT, m_uFreeCount, m_PoolAllocCount, m_uFirsthandCount);
	#else
		_snprintf(pInfoBuf, INFO_BUF_SIZE, "CBlockPool:\r\n	��ǰ�ѳ�û�н����ڴ���䣡\r\n");
	#endif
#endif
	}

	//!	
	template<typename _TLock, typename _TAllocator>
	void* CBlockPool<_TLock, _TAllocator>::Alloc(unsigned long uSize)
	{
#ifndef USE_BLOCK_MANAGE
		++m_uFirsthandCount;
		return m_Allocator.Alloc(uSize);
#endif

		MM_ASSERY(INIT_FLAG_VALUE == m_uInitFlag);
		
		void *pRe = NULL;
		//! ����094
		m_FlagLock.Lock();
		if(m_uFreeCount * BLOCK_BSAE_SIZE < uSize)
		{
			//! ��ϣ��Ԥ������ڴ治��ʹ��
			//MM_ASSERY(false);
			//! ֱ�ӵ����ϼ�����
			if(NULL != (pRe = m_Allocator.Alloc(uSize)))
			{
				MM_ASSERY(!(m_pBuf <= pRe && m_pBuf + BLOCK_DEFAULT_PROVIDE_COUNT * BLOCK_BSAE_SIZE >= pRe));
				++m_uFirsthandCount;
			}
		}
		else
		{
			unsigned long uAllocCount = uSize / BLOCK_BSAE_SIZE + ((uSize % BLOCK_BSAE_SIZE) ? 1 : 0);


			//! ����������1����β����
			if (1 == uAllocCount)
			{
				MM_ASSERY(0xFFFFFFFF != m_FreeEndOffset);
				MM_ASSERY(eFT_Free == m_pFlag[m_FreeEndOffset]);
				m_pFlag[m_FreeEndOffset] = eFT_Head;
				pRe = &m_pBuf[m_FreeEndOffset * BLOCK_BSAE_SIZE];
				--m_uFreeCount;
#ifdef LOG_ALLOC_FREE_INFO
				++m_PoolAllocCount;
#endif

				if(0 == m_uFreeCount)
				{
					m_FreeBeginOffset	= 0xFFFFFFFF;
					m_FreeEndOffset		= 0xFFFFFFFF;
				}
				else
				{
					for (; m_FreeEndOffset > 0; --m_FreeEndOffset)
					{
						if(eFT_Free == m_pFlag[m_FreeEndOffset])
							break;
					}
				}

				
			}
			//! ���򣬴�ͷ����
			else
			{
				unsigned long uSeriateCount = 0;
				unsigned long uFindIdx = m_FreeBeginOffset;

				for (; uFindIdx < BLOCK_DEFAULT_PROVIDE_COUNT; ++uFindIdx)
				{
					if(eFT_Free == m_pFlag[uFindIdx])
						++uSeriateCount;
					else
						uSeriateCount = 0;

					if(uSeriateCount >= uAllocCount)
						break;
				}

				//! �ҵ���
				if(uSeriateCount == uAllocCount)
				{
					unsigned long uFindBegin = uFindIdx + 1 - uAllocCount;	
					memset(&m_pFlag[uFindBegin], eFT_Body, uAllocCount);
					m_pFlag[uFindBegin] = eFT_Head;
					pRe = &m_pBuf[uFindBegin * BLOCK_BSAE_SIZE];
					MM_ASSERY(m_uFreeCount >= uAllocCount);
					m_uFreeCount -= uAllocCount;
#ifdef LOG_ALLOC_FREE_INFO
					++m_PoolAllocCount;
#endif
					if(0 == m_uFreeCount)
					{
						m_FreeBeginOffset	= 0xFFFFFFFF;
						m_FreeEndOffset		= 0xFFFFFFFF;
					}
					else
					{
						if(eFT_Free != m_pFlag[m_FreeEndOffset])
						{
							for (; m_FreeEndOffset > 0; --m_FreeEndOffset)
							{
								if(eFT_Free == m_pFlag[m_FreeEndOffset])
									break;
							}
							MM_ASSERY(eFT_Free == m_pFlag[m_FreeEndOffset]);
						}
						if (uFindBegin == m_FreeBeginOffset)
						{
							m_FreeBeginOffset = uFindIdx + 1;
							for (; m_FreeBeginOffset < BLOCK_DEFAULT_PROVIDE_COUNT; ++m_FreeBeginOffset)
							{
								if(eFT_Free == m_pFlag[m_FreeBeginOffset])
									break;
							}
							MM_ASSERY(eFT_Free == m_pFlag[m_FreeBeginOffset]);
						}
					}
				}
				//! û�ҵ�
				else
				{
					//! ֱ�ӵ����ϼ�����
					if(NULL != (pRe = m_Allocator.Alloc(uSize)))
					{
						MM_ASSERY(!(m_pBuf <= pRe && m_pBuf + BLOCK_DEFAULT_PROVIDE_COUNT * BLOCK_BSAE_SIZE >= pRe));
						++m_uFirsthandCount;
					}
				}
			}
		}
		m_FlagLock.UnLock();//! ����094
		
		return pRe;
	}

	//!		
	template<typename _TLock, typename _TAllocator>
	void CBlockPool<_TLock, _TAllocator>::Free(void *pMem)
	{
#ifndef USE_BLOCK_MANAGE
		--m_uFirsthandCount;
		m_Allocator.Free(pMem);
		return;
#endif

		MM_ASSERY(INIT_FLAG_VALUE == m_uInitFlag);
		unsigned char *pRe = (unsigned char*)pMem;

		m_FlagLock.Lock();//! ����196
		if(0 != m_uFirsthandCount)
		{
			if(!(m_pBuf <= pRe && m_pBuf + BLOCK_DEFAULT_PROVIDE_COUNT * BLOCK_BSAE_SIZE >= pRe))
			{
				m_Allocator.Free(pRe);
				--m_uFirsthandCount;
				m_FlagLock.UnLock();//! ����196
				return;
			}
		}

		unsigned long uFlagIdx = (unsigned long)(pRe - m_pBuf) / BLOCK_BSAE_SIZE;
		MM_ASSERY(eFT_Head == m_pFlag[uFlagIdx]);
		if(m_FreeBeginOffset > uFlagIdx || 0xFFFFFFFF == m_FreeBeginOffset)
			m_FreeBeginOffset = uFlagIdx;
		
		m_pFlag[uFlagIdx++] = eFT_Free;
		unsigned long uCount = 1;
		while (BLOCK_DEFAULT_PROVIDE_COUNT > uFlagIdx && eFT_Body == m_pFlag[uFlagIdx])
		{
			m_pFlag[uFlagIdx] = eFT_Free;
			++uCount;		
			++uFlagIdx;
		}

		--uFlagIdx;
		m_uFreeCount += uCount;
#ifdef LOG_ALLOC_FREE_INFO
		--m_PoolAllocCount;
#endif
		if(uFlagIdx > m_FreeEndOffset || 0xFFFFFFFF == m_FreeEndOffset)
			m_FreeEndOffset = uFlagIdx;

		m_FlagLock.UnLock();//! ����196
		
	}
};
#endif //! _BLOCK_POOL_CPP_