


#pragma once


/** ���ݻ���ģ��
*
*		��ģ��ֻ���������ݲ�����Ƶ��������������ݵ����ʱ����
*	Զ�������õ���󻺴�ʱ�䣬�����ֹ������ݲ��ܵõ���ʱ�����
*	���⣬��Ҫ�ֶ�����CheckCache�ӿ�
*
*	ע�⣺TDataӦ��֤��=������������ȷ��
*/

template<typename TData>
class DataCache
{
public:
	//! MaxCacheNum=��󻺴����� MaxCacheSec=��󻺴�ʱ��
	DataCache(DWORD MaxCacheNum, DWORD MaxCacheSec);
	~DataCache(VOID);

public:
	//! ���ڱ���ͷ��ص����ݽṹ
	struct tagDataBlock 
	{
		TData *pData;
		DWORD DataNum;
	};

	//! ���һ�����ݵ�����
	//! ������ֵ��Ϊ�գ���˵�������������ѳ�ʱ
	//! ���ص�ֻ������ʹ����Ϻ�Ӧͨ��Release�����ύ�ر��ദ���������й¶
	const tagDataBlock* AddData(TData &Data);

	//! ȡ�����������ѳ�ʱ���������
	//! ���ص�ֻ������ʹ����Ϻ�Ӧͨ��Release�����ύ�ر��ദ���������й¶
	const tagDataBlock* CheckCache(VOID);

	//! ��ǰȡ����������ݣ��������κ�����
	//! ���ص�ֻ������ʹ����Ϻ�Ӧͨ��Release�����ύ�ر��ദ���������й¶
	const tagDataBlock* GetCache(VOID);

	//! �ύ���ݲ�����ɽӿ�
	VOID Release(const tagDataBlock *pDataBlock);
	VOID Release(const TData *pData);

private:
	//! ��󻺴�����
	const DWORD m_MaxCacheNum;
	//! ��󻺴�ʱ��
	const DWORD m_MaxCacheSec;

	//! ��ǰ��Ĵ���ʱ��
	DWORD m_CurrBlokCreateTime;
	//! ���ⲿ���������ݿ��б�
	list<tagDataBlock> m_OptDataCache;
	//! ��ǰ����
	tagDataBlock m_CurrData;
};










///////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TData>
DataCache<TData>::DataCache(DWORD MaxCacheNum, DWORD MaxCacheSec)
:m_MaxCacheNum(MaxCacheNum), m_MaxCacheSec(MaxCacheSec), m_CurrBlokCreateTime(0)
{
}

template<typename TData>
DataCache<TData>::~DataCache(VOID)
{
	//! ���ⲿ������û�н������ȴ�30��
	if(0 != m_OptDataCache.size())
		Sleep(1000 * 30);

	//! ���ⲿ������û�н��������ͷ���Ӧ�ڴ����������
	if(0 == m_OptDataCache.size())
	{
		list<tagDataBlock>::iterator iteData = m_OptDataCache.begin();
		for (; m_OptDataCache.end() != iteData; ++iteData)
		{
			M_FREE(iteData->pData, m_MaxCacheNum * sizeof(TData));
		}
		m_OptDataCache.clear();
	}
	else
	{
		LogTools::OutErrLog("~DataCache()�����ⲿ����û�м�ʱ�黹ʹ�õ����ݣ����й¶!!!");
	}
	
	//! �ͷŵ��ⲿû���õ����ڴ�
	M_FREE(m_CurrData.pData, m_MaxCacheNum * sizeof(TData));
	m_CurrData.DataNum = 0;
	m_CurrBlokCreateTime = 0;
}

template<typename TData>
typename const DataCache<TData>::tagDataBlock* DataCache<TData>::AddData(TData &Data)
{
	const tagDataBlock *pRe = CheckCache();

	m_CurrData.pData[m_CurrData.DataNum] = Data;
	++m_CurrData.DataNum;

	if(1 == m_CurrData.DataNum)
	{
		m_CurrBlokCreateTime = (DWORD)time(NULL);
	}

	return pRe;
}

template<typename TData>
typename const DataCache<TData>::tagDataBlock* DataCache<TData>::CheckCache(VOID)
{
	if(m_MaxCacheSec <= time(NULL) - m_CurrBlokCreateTime
		|| m_MaxCacheNum <= m_CurrData.DataNum
		|| NULL == m_CurrData.pData)
	{
		return GetCache();
	}

	return NULL;
}

template<typename TData>
typename const DataCache<TData>::tagDataBlock* DataCache<TData>::GetCache(VOID)
{
	tagDataBlock *pRe = NULL;
	if(NULL != m_CurrData.pData)
	{
		m_OptDataCache.push_back(m_CurrData);
		pRe = &(*(--m_OptDataCache.end()));
	}

	m_CurrData.DataNum = 0;
	m_CurrBlokCreateTime = 0;
	m_CurrData.pData = (TData*)M_ALLOC(sizeof(TData) * m_MaxCacheNum);

	return pRe;
}

template<typename TData>
VOID DataCache<TData>::Release(const tagDataBlock *pDataBlock)
{
	list<tagDataBlock>::iterator iteData = m_OptDataCache.begin();
	for (; m_OptDataCache.end() != iteData; ++iteData)
	{
		if(&(*iteData) == pDataBlock)
		{
			M_FREE(iteData->pData, m_MaxCacheNum * sizeof(TData));
			m_OptDataCache.erase(iteData);
			return;
		}
	}
}

template<typename TData>
VOID DataCache<TData>::Release(const TData *pData)
{
	list<tagDataBlock>::iterator iteData = m_OptDataCache.begin();
	for (; m_OptDataCache.end() != iteData; ++iteData)
	{
		if(iteData->pData == pData)
		{
			M_FREE(pDataBlock->pData, m_MaxCacheNum * sizeof(TData));
			m_OptDataCache.erase(iteData);
			return;
		}
	}
}

