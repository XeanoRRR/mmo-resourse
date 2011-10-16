



/*
���ļ�����������MemManage�ļ�����
	��Ч����
	win32����

*/

#include "assert.h"
#include <Windows.h>

#ifndef _LOCK_H_
#define _LOCK_H_


namespace XM_Tools
{
	/////////////////////////////////////////////////////////////////////////////
	//! һ����Ч���������ڲ���Ҫͬ���Ķ���
	class InvalidLock
	{
	public:
		InvalidLock(void)			{}
		~InvalidLock(void)			{}
	public:
		void	Lock(void)			{}
		void	UnLock(void)		{}
	};

	/////////////////////////////////////////////////////////////////////////////
	//! һ��WIN32�ٽ���ʵ�ֵ���
	class LockOfWin32
	{
	public:
		LockOfWin32(void)			{InitializeCriticalSection(&m_CriticalSection);}
		~LockOfWin32(void)			{DeleteCriticalSection(&m_CriticalSection);}
	public:
		void	Lock(void)			{EnterCriticalSection(&m_CriticalSection);}
		void	UnLock(void)		{LeaveCriticalSection(&m_CriticalSection);}
	private:
		CRITICAL_SECTION			m_CriticalSection;
	};

	/////////////////////////////////////////////////////////////////////////////
	//! һ������ԭ�Ӳ�WIN32API��ʵ�ֵ�������ʹ�����߳����϶࣬��ײ���ҵĳ��ϣ�
	//! ���ڼ���ʱ��϶̵Ĳ���
	class LockOfWinWhile
	{
	public:
		LockOfWinWhile(void)		:m_lLockFlag(0){}
		~LockOfWinWhile(void)		{}
	public:
		void	Lock(void)			{while(1 == InterlockedCompareExchange(&m_lLockFlag, 1, 0));}
		void	UnLock(void)		{if(0 == InterlockedCompareExchange(&m_lLockFlag, 0, 1))assert(false);}
	private:
		volatile long				m_lLockFlag;
	};

	/////////////////////////////////////////////////////////////////////////////
	//! LockOfWinAtom��һ���Ż�������ʹ�����߳����϶࣬��ײ���ҵĳ��ϣ�
	//! ���ڼ���ʱ��ϳ��Ĳ���
	class LockOfWinWhileSleep
	{
	public:
		LockOfWinWhileSleep(void)	:m_lLockFlag(0){}
		~LockOfWinWhileSleep(void)	{}
	public:
		void	Lock(void)			{while(1 == InterlockedCompareExchange(&m_lLockFlag, 1, 0))Sleep(1);}
		void	UnLock(void)		{if(0 == InterlockedCompareExchange(&m_lLockFlag, 0, 1))assert(false);}
	private:
		volatile long				m_lLockFlag;
	};
};

#endif // _LOCK_H_

