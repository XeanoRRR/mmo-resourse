#include "stdafx.h"
#include "AinMessage.h"


extern void OnAinMessage(AinMessage* pAimMsg);

MultiObjPool<AinMessage, LockOfWin32> AinMessage::c_ObjPool;

AinMessage* AinMessage::Create(LONG lType)
{
	AinMessage* pRe = c_ObjPool.Multi_Alloc(_FILE_AND_LINE_);
	if(NULL != pRe)
		new(pRe)AinMessage(lType);
	return pRe;

}
VOID AinMessage::Release(AinMessage **ppAinMsg)
{
	if(NULL == ppAinMsg || NULL == *ppAinMsg)return;
	(*ppAinMsg)->~AinMessage();
	c_ObjPool.Multi_Free(*ppAinMsg);
	*ppAinMsg=NULL;
}

VOID AinMessage::AddByte(BYTE cData)
{
	m_vData.push_back(cData);
}
VOID AinMessage::AddWord(WORD wData)
{
	size_t lSize = m_vData.size();
	m_vData.resize(lSize + sizeof(WORD));
	memcpy(&m_vData[lSize], &wData, sizeof(WORD));
}
VOID AinMessage::AddDword(DWORD dwData)
{
	size_t lSize = m_vData.size();
	m_vData.resize(lSize + sizeof(DWORD));
	memcpy(&m_vData[lSize], &dwData, sizeof(DWORD));
}
VOID AinMessage::AddStr(LPCSTR pStr)
{
	if(NULL == pStr)
	{
		assert(false);
		return;
	}
	WORD wStrLen = (WORD)strlen(pStr);
	AddWord(wStrLen);
	AddEx(pStr, wStrLen);		
}
VOID AinMessage::AddEx(LPCVOID pData, DWORD dwAddSize)
{
	size_t lSize = m_vData.size();
	m_vData.resize(lSize + dwAddSize);
	memcpy(&m_vData[lSize], pData, dwAddSize);
}

BYTE AinMessage::GetByte(VOID)
{
	BYTE cRe = 0;
	size_t lSize = m_vData.size();
	if(m_dwPos + sizeof(BYTE) <= lSize)
	{
		cRe = m_vData[m_dwPos];
		m_dwPos += sizeof(BYTE);
	}
	else
		assert(false);
	return cRe;
}
WORD AinMessage::GetWord(VOID)
{
	WORD wRe = 0;
	size_t lSize = m_vData.size();
	if(m_dwPos + sizeof(WORD) <= lSize)
	{
		wRe = *(WORD*)(&m_vData[m_dwPos]);
		m_dwPos += sizeof(WORD);
	}
	else
		assert(false);
	return wRe;
}
DWORD AinMessage::GetDword(VOID)
{
	DWORD dwRe = 0;
	size_t lSize = m_vData.size();
	if(m_dwPos + sizeof(DWORD) <= lSize)
	{
		dwRe = *(DWORD*)(&m_vData[m_dwPos]);
		m_dwPos += sizeof(DWORD);
	}
	else
		assert(false);
	return dwRe;
}
BOOL AinMessage::GetStr(LPSTR pStr, WORD dwBufMaxSize)
{
	BOOL bRe = FALSE;
	WORD wStrLen = GetWord();
	WORD wLoadLen = min(wStrLen, dwBufMaxSize - 1);

	size_t lSize = m_vData.size();
	if(m_dwPos + wStrLen <= lSize)
	{
		memcpy(pStr, &(m_vData[m_dwPos]), wLoadLen);
		m_dwPos += wStrLen;
		bRe = TRUE;
	}
	else
		assert(false);
	return bRe;
}
BOOL AinMessage::GetEx(LPVOID pData, DWORD lGetSize)
{
	BOOL bRe = FALSE;
	size_t lSize = m_vData.size();
	if(m_dwPos + lGetSize <= lSize)
	{
		memcpy(pData, &(m_vData[m_dwPos]), lGetSize);
		m_dwPos += lGetSize;
		bRe = TRUE;
	}
	else
		assert(false);
	return bRe;
}

VOID AinMessage::Run(VOID)
{
	OnAinMessage(this);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AinMsgQueue::AinMsgQueue(VOID)
:m_dwMinBeginTime(0xFFFFFFFF)
{
}
AinMsgQueue::~AinMsgQueue(VOID)
{

}
VOID AinMsgQueue::Release(VOID)
{
	//! ִ�������е���Ϣ
	list<AinMessage*> listMsg;
	PopMessage(listMsg);
	for(list<AinMessage*>::iterator ite = listMsg.begin(); listMsg.end() != ite; ite++)
	{
		if( NULL != *ite )
		{
			(*ite)->Run();
			AinMessage::Release(&(*ite));
		}
	}

	//! ��ʱΪ������Ϣ��ִ����
	map<DWORD, AinMessage*>::iterator ite = m_timeMsgQueue.begin();
	for (; m_timeMsgQueue.end() != ite; ++ite)
	{
		AinMessage::Release(&(ite->second));
	}
}
//! �õ���Ϣ��Ŀ
LONG AinMsgQueue::GetSize(VOID)
{
	LONG lRe = 0;
	m_Lock.Lock();
	{
		lRe = (LONG)m_msgQueue.size() + (LONG)m_timeMsgQueue.size();
	}
	m_Lock.UnLock();
	return lRe;
}
//! ѹ����Ϣ
BOOL AinMsgQueue::PushMessage(AinMessage* pMsg)
{
	if(NULL == pMsg) return FALSE;
	m_Lock.Lock();
	{
		m_msgQueue.push_back(pMsg);
	}
	m_Lock.UnLock();
	return TRUE;
}

//! ѹ��һ����ʱ��Ϣ
BOOL AinMsgQueue::PushTimeMessage(AinMessage* pMsg, DWORD dwMillisecond)
{
	if(NULL == pMsg) return FALSE;
	if(0 == dwMillisecond)
		return PushMessage(pMsg);

	DWORD dwCurrTime = timeGetTime();
	DWORD dwBeginTime = dwCurrTime + dwMillisecond;

	m_Lock.Lock();
	{
		do 
		{
			if(m_timeMsgQueue.end() != m_timeMsgQueue.find(dwBeginTime))
				++dwBeginTime;
			else
				break;
		} while(true);

		m_timeMsgQueue[dwBeginTime] = pMsg;
		m_dwMinBeginTime = min(m_dwMinBeginTime, dwBeginTime);
	}
	m_Lock.UnLock();
	return TRUE;
}

//! ������Ϣ
VOID	AinMsgQueue::PopMessage(list<AinMessage*> &listMsg)
{
	//! ��ͨ��Ϣ
	m_Lock.Lock();
	{
		listMsg = m_msgQueue;
		m_msgQueue.clear();
	}
	m_Lock.UnLock();

	//! ��ʱ��Ϣ
	DWORD dwCurrTime = timeGetTime();
	if(m_dwMinBeginTime <= dwCurrTime)
	{
		m_Lock.Lock();
		{
			while (0 != m_timeMsgQueue.size() && dwCurrTime < m_timeMsgQueue.begin()->first)
			{
				listMsg.push_back(m_timeMsgQueue.begin()->second);
				m_timeMsgQueue.erase(m_timeMsgQueue.begin());
			}

			if(0 == m_timeMsgQueue.size())
				m_dwMinBeginTime = 0xFFFFFFFF;
			else
				m_dwMinBeginTime = m_timeMsgQueue.begin()->first;
		}
		m_Lock.UnLock();
	}
}
//! �����Ϣ�����ͷ��ڴ棩
VOID AinMsgQueue::Clear(VOID)
{
	m_Lock.Lock();
	m_msgQueue.clear();
	m_Lock.UnLock();
}