//=============================================================================
/**
 *  file: TimerQueue.cpp
 *
 *  Brief:ͨ��ʱ����ԭ��ʵ�ֶ�ʱ�����ܡ�
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-5-15
 */
//=============================================================================
#include "StdAfx.h"
#ifndef __TIMER_QUEUE_CPP__
#define __TIMER_QUEUE_CPP__
#pragma once
#include "TimerQueue.h"
#include "GameEvent.h"

//TimerQueueʵ��˵��:

//ÿһ����ʱ������ͨ��TimerNode����ʾ
//ʱ���ֱ��ֳ�һ��������"spokes",��ʱ�������ܹ���m_nSpokeCount��"spokes",
//ÿһ����ʱ����ɢ�е���Ӧ��spokes��,���մ�����ʱ����������
//��ÿһ��"spoke"�����еĶ�ʱ���໥֮��ͨ��˫����������,
//ÿһ��"spoke"�ĵ�һ���ڵ���Ϊ"����ڵ�",������ʾ����Ľ���.���е�timerid���ڼ�����

//�����ȵĶ�ʱ����ID,��ͨ��spoke��λ�úͱ����뵽�����е�����ֵ��ɵġ�
//32bit��m_dwTimerID������Nλ����ȷ��spoke,Mλ��Ϊ����ֵ����һ���µĽڵ���뵽
//spoke�к�"����ڵ���"��timerid���1,�������������������,��"spoke"��������
//���нڵ�,�Ա��ڲ���һ�����ظ���timerid,Ϊ�˱����ʱ������,����������������"����ڵ�"
//��.

///////////////////////////////////////////////////////////////////////////
// TimerNode

template <class TYPE>
TimerNode<TYPE>::TimerNode (void)
{  
}

template <class TYPE>
TimerNode<TYPE>::~TimerNode (void)
{
}

template <class TYPE> inline
void TimerNode<TYPE>::Set(const TYPE &type,
						  const	void* arg,
						  unsigned long 	dwTimeValue,
						  unsigned long 	dwInterval,
						  unsigned long 	dwTimerID,
						  TimerNode<TYPE> *p,
						  TimerNode<TYPE> *n)
{
	m_Type = type;
	m_pArg = arg;
	m_dwTimeValue=dwTimeValue;
	m_dwInterval=dwInterval;
	m_dwTimerID=dwTimerID;
	m_pRev=p;
	m_pNext=n;
}

template <class TYPE> inline
TYPE &TimerNode<TYPE>::GetType(void)
{
	return m_Type;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetType(TYPE &type)
{
	m_Type = type;
}

template <class TYPE> inline
const void *TimerNode<TYPE>::GetArg(void)
{
	return m_pArg;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetArg(void *arg)
{
	m_pArg = arg;
}

template <class TYPE> inline
unsigned long TimerNode<TYPE>::GetTimerValue(void) const
{
	return m_dwTimeValue;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetTimerValue(unsigned long timervalue)
{
	m_dwTimeValue=timervalue;
}

template <class TYPE> inline
unsigned long TimerNode<TYPE>::GetInterval(void) const
{
	return m_dwInterval;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetInterval(unsigned long interval)
{
	m_dwInterval=interval;
}

template <class TYPE> inline
TimerNode<TYPE> *TimerNode<TYPE>::GetPrev(void)
{
	return m_pRev;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetPrev(TimerNode<TYPE> *prev)
{
	m_pRev = prev;
}

template <class TYPE> inline
TimerNode<TYPE> *TimerNode<TYPE>::GetNext(void)
{
	return m_pNext;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetNext(TimerNode<TYPE> *next)
{
	m_pNext = next;
}

template <class TYPE> inline
long TimerNode<TYPE>::GetTimerID(void) const
{
	return m_dwTimerID;
}

template <class TYPE> inline
void TimerNode<TYPE>::SetTimerID(long timerid)
{
	m_dwTimerID= timerid;
}

template <class TYPE> inline
void TimerNode<TYPE>::OnTimerOut(unsigned long curtime)
{
	m_Type->OnTimeOut(m_dwTimerID,curtime,m_pArg);
}
//ȡ����ʱ��ʱ�����������
template <class TYPE> inline
void TimerNode<TYPE>::OnTimerCancel()
{
	m_Type->OnTimerCancel(m_dwTimerID,m_pArg);
}

template <class TYPE>
void TimerNode<TYPE>::OutpuInfo(void) const
{
	//OUTPUTLOG("NodeStart");
	char str[64];
	//sprintf(str,"m_dwTimerID = %d",m_dwTimerID);
	//OUTPUTLOG(str);
	_snprintf(str, 64, "m_dwTimeValue = %d",m_dwTimeValue);
	OUTPUTLOG(str);
	//sprintf(str,"m_dwInterval = %d",m_dwInterval);
	//OUTPUTLOG(str);
	//sprintf(str,"m_pArg = %d",(int)m_pArg);
	//OUTPUTLOG(str);
	//OUTPUTLOG("NodeEnd");
}

///////////////////////////////////////////////////////////////////////////
// CTimerQueueIterator

template <class TYPE>
CTimerQueueIterator<TYPE>::CTimerQueueIterator(CTimerQueue<TYPE> &queue)
:m_TimerQueue(queue)
{
	First();
}

template <class TYPE>
CTimerQueueIterator<TYPE>::~CTimerQueueIterator(void)
{
}

//��λ�ڵ�һ��spoke�ĵ�һ���ڵ�
template <class TYPE>
void CTimerQueueIterator<TYPE>::First (void)
{
	GoToNext(0);
}

//ָ��ǰ�ڵ����һ���ڵ�
template <class TYPE>
void CTimerQueueIterator<TYPE>::Next (void)
{
	if (IsDone())
		return;
	TimerNode<TYPE>* n = m_pCurNode->GetNext();
	TimerNode<TYPE>* root = m_TimerQueue.m_pSpokes[m_nSpoke];
	if (n == root)
		GoToNext(m_nSpoke + 1);
	else
		m_pCurNode = n;
}

//���ڱ�����������Ҳû�нڵ�ʱ,����true.
template <class TYPE>
int CTimerQueueIterator<TYPE>::IsDone (void) const
{
	return m_pCurNode == 0;
}

template <class TYPE>
TimerNode<TYPE>* CTimerQueueIterator<TYPE>::Item (void)
{
	return m_pCurNode;
}

//����First()��Next()�Ĺ���ʹ�ò���
template <class TYPE>
void CTimerQueueIterator<TYPE>::GoToNext(unsigned long nSartSpoke)
{
	//���ҵ�һ���ǿյ���Ŀ
	unsigned long sc = m_TimerQueue.m_nSpokeCount;
	for (unsigned long  i = nSartSpoke; i < sc; ++i)
	{
		TimerNode<TYPE>* root = m_TimerQueue.m_pSpokes[i];
		TimerNode<TYPE>* n = root->GetNext ();
		if (n != root)
		{
			m_nSpoke= i;
			m_pCurNode = n;
			return;
		}
	}
	//����ǿ�
	m_nSpoke = sc;
	m_pCurNode = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//CTimerQueue
/**
* ���캯��,����wheel��С��spoke������Ԥ����һЩ�ڵ㡣
*
* spokecount	��ʱ���б������
* resolution:   ��hansh����ʹ��ʱ��ֱ���,�ú����ʾ��
* prealloc:     Ԥ����ڵ������
*
*/
template <class TYPE>
CTimerQueue<TYPE>::CTimerQueue(	unsigned long spokecount,
							   unsigned long resolution,
							   unsigned long prealloc)
							   :m_pSpokes(0)
							   ,m_nSpokeCount(0) //��OpenI�м���
							   ,m_nSpokeBits(0)
							   ,m_nResBits(0)
							   ,m_nEarliestSpoke(0)
							   ,pIterator(0)
							   ,m_nTimerCount(0)
{
	OpenI(prealloc, spokecount, resolution);
}

/**
* ��������,�����ͷ���Ӧ���ڴ�.
*/
template <class TYPE>
CTimerQueue<TYPE>::~CTimerQueue(void)
{
	delete pIterator;
	unsigned long i = 0;
	for (; i < this->m_nSpokeCount; ++i)
	{
		//�Ӹ��ڵ㿪ʼ���ͷ����нڵ�
		TimerNode<TYPE>* root = m_pSpokes[i];
		for (TimerNode<TYPE>* n = root->GetNext (); n != root;)
		{
			TimerNode<TYPE>* next = n->GetNext ();
			FreeNode(n);
			n = next;
		}
		delete root;
	}
	delete[] m_pSpokes;

	list<TimerNode<TYPE>*>::iterator it = m_FreeList.begin();
	for(;it != m_FreeList.end();it++)
	{
		TimerNode<TYPE>* pNode = (*it);
		if(pNode)
			delete pNode;
	}
	m_FreeList.clear();
}

/**
* �õ�һ��2����ֵ
*
* n			Ҫ�õ��ݵ���ֵ
* minbits	�������Сλ��
* maxbits   ��������λ��
*
*/
template <class TYPE>
int CTimerQueue<TYPE>::Power2Bits (int n, int minbits, int maxbits)
{
	int max = (1 << maxbits) - 1;
	if (n > max)
		return maxbits;

	//����N��λ��
	int i = 0;
	int tmp = n;
	do
	{
		tmp >>= 1;
		++i;
	}
	while (tmp != 0);

	if (i <= minbits)
		return minbits;

	//ѡ��������Ǹ�
	int a = (1 << i) - n;
	int b = (1 << (i - 1)) - n;
	if (b < 0)
		b = -b;
	if (b < a)
		return i - 1;
	return i;
}


// ����һ���ڵ㣬�ȴ�Ԥ��������в��ҡ����û������new
template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::AllocNode()
{
	if(m_FreeList.size() == 0)
		return new TimerNode<TYPE>;
	TimerNode<TYPE>* pNode = m_FreeList.front();
	m_FreeList.pop_front();
	return pNode;
}

template <class TYPE>
void CTimerQueue<TYPE>::FreeNode(TimerNode<TYPE>* pNode)
{
	if(m_FreeList.size() <= 10240)
		m_FreeList.push_back(pNode);
	else
		delete pNode;
}


// ʹ���Ѿ�ȷ������Ϣ������ʼ����ʱ�����С�

template <class TYPE>
void CTimerQueue<TYPE>::OpenI(size_t prealloc, unsigned long spokes, unsigned long res)
{
	//����spokes�ĸ���Ϊ2��N����-1;(����:1,3,7,15,..)
	const int MIN_SPOKE_BITS = 3;  // ���� 8 �� 4096 ��spokes
	const int MAX_SPOKE_BITS = 12;
	const int MAX_RES_BITS = 20; 

	m_nSpokeBits = Power2Bits(spokes, MIN_SPOKE_BITS, MAX_SPOKE_BITS);
	m_nResBits = Power2Bits(res, 1, MAX_RES_BITS);

	m_nSpokeCount = 1 << m_nSpokeBits;

	unsigned long i = 0;
	for(;i < prealloc + m_nSpokeCount;i ++)
	{
		TimerNode<TYPE> *pNode = new TimerNode<TYPE>;
		m_FreeList.push_back(pNode);
	}

	m_nWheelTime = (1 << (m_nResBits + m_nSpokeBits));
	m_pSpokes = new TimerNode<TYPE>*[m_nSpokeCount];
	//�������ڵ㣬����ڵ���������ʹ�á�
	for (i = 0; i < m_nSpokeCount; ++i)
	{
		TimerNode<TYPE>* root = AllocNode();
		root->Set(0, 0, 0, 0, 0,root, root);
		m_pSpokes[i] = root;
	}

	pIterator = new CTimerQueueIterator<TYPE>(*this);
}

//����timerid,��ĳ��ָ����spoke������,�������ҵ��Ľڵ�.
template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::FindSpokeNode(unsigned long spoke, long timerid) const
{
	if(spoke >= m_nSpokeCount)
	{
		char strError[256]="";
		_snprintf(strError,256,"Timer Wheel Varialbe Overflow(spoke:%d,m_nSpokeCount:%d).",spoke,m_nSpokeCount);
		PutDebugString(strError);
		return 0;
	}

	TimerNode<TYPE>* root = m_pSpokes[spoke];
	for (TimerNode<TYPE>* n = root->GetNext();
		n != root;
		n = n->GetNext())
	{
		if (n->GetTimerID() == timerid)
			return n;
	}
	return 0;
}
//����timerid,�������е�spokes,Ѱ����Ӧ�Ľڵ㡣
template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::FindNode(long timerid) const
{
	if (timerid == -1)
		return 0;
	// ����Ѱԭʼ���ȵ�timerid�������spoke
	unsigned long spokemask = m_nSpokeCount - 1;
	unsigned long start = timerid & spokemask;
	TimerNode<TYPE>* n = FindSpokeNode (start, timerid);
	if (n != 0)
		return n;
	// ��Ѱʣ���spokes
	for (unsigned long i = 0; i < m_nSpokeCount; ++i)
	{
		if (i != start)
		{
			n = FindSpokeNode(i, timerid);
			if (n != 0)
				return n;
		}
	}
	return 0;
}

//���wheel�Ƿ�Ϊ��
template <class TYPE>
int CTimerQueue<TYPE>::IsEmpty(void) const
{
	return m_nTimerCount == 0;
}

//�õ���һ�������ʱ��ֵ
template <class TYPE>
unsigned long CTimerQueue<TYPE>::EarliestTime(void) const
{
	TimerNode<TYPE>* n = GetFirstI();
	if (n != 0)
		return n->GetTimerValue();
	return 0;
}

//��������Ҫ��expireֵ������һ���򵥵�hash�㷨�������Ӧ��spoke
template <class TYPE>
unsigned long CTimerQueue<TYPE>::CalculateSpoke(unsigned long expire) const
{
	return static_cast<unsigned long> ((expire >> m_nResBits) & (m_nSpokeCount- 1));
}

//����������spoke,����һ����һ�޶���timerid,������ڵ�ļ��������,
//���ʱ���Ҫ�������еĽڵ㣬�Եõ�һ��û��ʹ�õ�timerid.
template <class TYPE>
long CTimerQueue<TYPE>::GenerateTimerID(unsigned long spoke)
{
	int cntbits = sizeof (long) * 8 - m_nSpokeBits;
	long maxcnt = ((long)1 << cntbits) - 1;
	if (spoke == m_nSpokeCount)
		--maxcnt;

	TimerNode<TYPE>* root = m_pSpokes[spoke];
	if (root == root->GetNext ())
		root->SetArg(0);
	//ʹ������ֶ���������һ������ֵ
	long nextcnt = reinterpret_cast<long> (root->GetArg());
	//����ֶ����ڼ�����������timerid.
	long cnt = root->GetTimerID();

	if (cnt >= maxcnt && root == root->GetNext())
	{
		OUTPUTLOG("In a space queue the counter overflow .");
		//�����һ���յ�spoke�������ʱ��
		root->SetTimerID(1);
		return spoke;
	}
	else if (cnt >= maxcnt)
	{ //�������0��ʼѰ��
		cnt = 0; 
	}
	else if (nextcnt == 0 || cnt < nextcnt)
	{
		root->SetTimerID(cnt + 1);
		return (cnt << m_nSpokeBits) | spoke;
	}

	OUTPUTLOG("remove counter, not alloc TimerID.");
	//�������ˡ�Ѱ���������У��ҳ�һ��δʹ�õ�timerid
	for (; cnt < maxcnt - 1; ++cnt)
	{
		long id = (cnt << m_nSpokeBits) | spoke;
		TimerNode<TYPE>* n = FindSpokeNode(spoke, id);
		if (n == 0)
		{
			root->SetTimerID(cnt + 1);
			nextcnt = 0;
			n=root->GetNext();
			for (; n != root; n = n->GetNext())
			{
				long tmp = n->GetTimerID() >> m_nSpokeBits;
				if (tmp > cnt && (tmp < nextcnt || nextcnt == 0))
					nextcnt = tmp;
			}
			root->SetArg (reinterpret_cast<void*> (nextcnt));
			return id;
		}
	}
	OUTPUTLOG("queue full,not alloc TimerID.");
	//��spokeȫ��,����IDʧ��.
	return -1;
}


/**
*���ݴ���Ĳ���������һ��TimerNode�ڵ㣬������һ��timerid
*Ȼ�󷵻�һ��timerid
*
* type			��ʱ���õ�ʵ�ʶ���
* arg			�û����������,������ʱ�ص�����
* futuretime	��ʱ��Ԥ���ĳ�ʱʱ��(����),�þ���ʱ���ʾ
* interval		��ʾ�Ƿ���һ�������ԵĶ�ʱ��,���������0,��ʾֻ����һ��.
*				�������0,��Ҫ�������ֵ���������Ե���
*/
template <class TYPE>
long CTimerQueue<TYPE>::Schedule(const TYPE& type,
								 const void* arg,
								 unsigned long futuretime,
								 unsigned long interval)
{
	TimerNode<TYPE>* n = AllocNode ();
	if (n != 0)
	{
		unsigned long spoke = CalculateSpoke(futuretime);
		long id = GenerateTimerID(spoke);

		if (id != -1)
		{
			n->Set(type, arg, futuretime, interval,id, 0, 0);
			this->ScheduleI(n, spoke, futuretime);
		}
		return id;
	}
	OUTPUTLOG("Schedule() failed,not enough memory.");
	return -1;
}

/**
*����Ԥ��һ����ʱ��,�ҳ���Ӧ��spoke,�����뵽��ȷ��λ��
*
* n		��Reschedule�Ľڵ�
*/
template <class TYPE>
void CTimerQueue<TYPE>::Reschedule(TimerNode<TYPE> *n)
{
	unsigned long expire = n->GetTimerValue ();
	unsigned long spoke = CalculateSpoke (expire);
	ScheduleI(n, spoke, expire);
}

//�ṩ�� Schedule()��Reschedule()ʹ�õĹ��ú���
template <class TYPE>
void CTimerQueue<TYPE>::ScheduleI(TimerNode<TYPE>* n, unsigned long spoke, unsigned long expire)
{
	//����Ƿ���������ʱ��
	if (IsEmpty() || expire < EarliestTime ())
		m_nEarliestSpoke = spoke;

	TimerNode<TYPE>* root = m_pSpokes[spoke];
	TimerNode<TYPE>* last = root->GetPrev();

	++m_nTimerCount;

	//������spoke�ǿյ�
	if (last == root) {
		n->SetPrev(root);
		n->SetNext(root);
		root->SetPrev(n);
		root->SetNext(n);
		return;
	}
	//�ȽϽڵ�ʱ�䣬������뵽��Ӧ��λ��
	TimerNode<TYPE>* p = root->GetPrev ();
	while (p != root && p->GetTimerValue () > expire)
		p = p->GetPrev ();

	n->SetPrev(p);
	n->SetNext(p->GetNext ());
	p->GetNext()->SetPrev (n);
	p->SetNext(n);
}

/**
* ����ID���ҳ���Ӧ�Ľڵ�ָ�룬ʹ��SetInterval()�����������ö�ʱ���ļ��ֵ
* timerid	��ʱ����Ψһ��ʾ
* interval	�µĶ�ʱ�����ֵ
*
* ����1��ʾ�ɹ�,-1��ʾʧ�ܡ�.
*/
template <class TYPE>
int CTimerQueue<TYPE>::ResetInterval(long timerid,unsigned long interval)
{
	TimerNode<TYPE>* n = FindNode (timerid);
	if (n != 0)
	{
		n->SetInterval(interval);
		return 0;
	}
	return -1;
}

/**
*
*����ʱ���������еĽڵ�,ƥ����Ӧ��typeֵ�����������Ƴ�������Ƴ�������Ķ�ʱ��
*�����¼������紥���Ķ�ʱ��
*
* type	��ʾ��Ѱ��ֵ
*
* ����ȡ���Ķ�ʱ������
*/
template <class TYPE>
int CTimerQueue<TYPE>::Cancel(const TYPE& type)
{
	int numcanceled = 0;

	if(!IsEmpty())
	{
		TimerNode<TYPE>* first = GetFirst();
		unsigned long last = first->GetTimerValue ();
		int recalc = 0;
		
		//�������е�spokes
		unsigned long i = 0;
		for (; i < this->m_nSpokeCount; ++i)
		{
			TimerNode<TYPE>* root = m_pSpokes[i];
			for (TimerNode<TYPE>* n = root->GetNext (); n != root; )
			{
				if (n->GetType() == type)
				{
					++numcanceled;
					if (n == first)
						recalc = 1;

					TimerNode<TYPE>* tmp = n;
					n = n->GetNext ();

					//����ȡ������
					tmp->OnTimerCancel();

					CancelI(tmp);
				}
				else
				{
					n = n->GetNext();
				}
			}
		}
		//���¼������紥���Ķ�ʱ��
		if (recalc)
			RecalcEarliest(last);
	}
	return numcanceled;
}

/**
*
*����timerid,�Ƴ������Ķ�ʱ����
*
* timerid   ��ʱ����Ψһ��ʶ
* arg		���������û����������,���arg!=0,��Ҫ����argֵ���ò���
*
* ����1��ʾ�ɹ�,-1��ʾʧ��
*/
//
template <class TYPE>
int CTimerQueue<TYPE>::Cancel(long timerid, const void** arg)
{
	TimerNode<TYPE>* n = FindNode(timerid);
	//�Ƿ��ҵ���Ӧ�Ľڵ�
	if (n != 0)
	{
		unsigned long last = n->GetTimerValue();

		int recalc = (GetFirstI() == n);
		if (arg != 0)
			*arg = n->GetArg();

		//����ȡ������
		n->OnTimerCancel();

		CancelI(n);

		if (recalc)
			RecalcEarliest(last);
		return 1;
	}
	return 0;
}

//����Cancel()�����Ĺ����Ӽ�
template <class TYPE>
void CTimerQueue<TYPE>::CancelI(TimerNode<TYPE>* n)
{
	Unlink(n);
	FreeNode(n);
}

//�������һ��expire�Ķ�ʱ��
template <class TYPE>
void CTimerQueue<TYPE>::RecalcEarliest(unsigned long last)
{
	if (IsEmpty ())
		return;

	unsigned long et = 0;
	unsigned long es = 0;
	unsigned long spoke = m_nEarliestSpoke;

	last = last>>m_nResBits;
	last = last<<m_nResBits;
	//����wheelһ��
	unsigned long i = 0;
	for (;i < m_nSpokeCount; ++i)
	{
		TimerNode<TYPE>* root = m_pSpokes[spoke];
		TimerNode<TYPE>* n = root->GetNext();
		if (n != root)
		{
			unsigned long t = n->GetTimerValue ();
			if (t < (last + m_nWheelTime))
			{
				m_nEarliestSpoke = spoke;
				return;
			}
			else if (et == 0 || t < et)
			{
				et = t;
				es = spoke;
			}
		}
		if (++spoke >= m_nSpokeCount)
			spoke = 0;
	}
	m_nEarliestSpoke = es;
}

//�Ƴ�����ڵ㣬Ѱ�����µ�<m_nEarliestSpoke>
template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::RemoveFirst(void)
{
	RemoveFirstExpired (MAX_TIME);
}

template <class TYPE>
void CTimerQueue<TYPE>::Unlink(TimerNode<TYPE>* n)
{
	--m_nTimerCount;
	n->GetPrev()->SetNext(n->GetNext());
	n->GetNext()->SetPrev(n->GetPrev());
	n->SetPrev(0);
	n->SetNext(0);
}

template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::RemoveFirstExpired(unsigned long now)
{
	TimerNode<TYPE>* n = GetFirst();
	if (n != 0 && n->GetTimerValue() <= now)
	{
		Unlink(n);
		RecalcEarliest(n->GetTimerValue());
		return n;
	}
	return 0;
}

//�õ�����Ľڵ㣬�����Ƴ�
template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::GetFirst(void)
{
	return GetFirstI();
}

template <class TYPE>
TimerNode<TYPE>* CTimerQueue<TYPE>::GetFirstI(void) const
{
	TimerNode<TYPE>* root = m_pSpokes[m_nEarliestSpoke];
	TimerNode<TYPE>* first = root->GetNext();
	if (first != root)
		return first;
	return 0;
}

template <class TYPE>
CTimerQueueIterator<TYPE>& CTimerQueue<TYPE>::Iter(void)
{
	pIterator->First();
	return *pIterator;
}


//������ʱ���ӿ�
//Ĭ�������ڵ�ʱ����������
template <class TYPE>
int CTimerQueue<TYPE>::Expire(void)
{
	unsigned long curtime = timeGetTime();
	return Expire(curtime);
}

/**
* �������curtimeΪ׼,Ϊ���ж�ʱ��С��curtimeʱ��Ĵ�����ʱ����
*
* curtime ��ʾ��ǰʱ��.
*
* ����ȡ���Ķ�ʱ�ڸ���
*/
template <class TYPE>
int CTimerQueue<TYPE>::Expire(unsigned long curtime)
{
	int expcount = 0;
	TimerNode<TYPE>* n = RemoveFirstExpired(curtime);

	while (n != 0)
	{
		bool cancel = true;
		++expcount;
		if (n->GetInterval() > 0)
		{
			do
			{
				n->SetTimerValue(n->GetTimerValue()+n->GetInterval());
			}
			while (n->GetTimerValue () <= curtime);

			cancel = false;
			Reschedule(n);
		}
		
		//������ʱ����
		n->OnTimerOut(curtime);
		//�����Ƴ�����
		if(cancel)
		{
			n->OnTimerCancel();
			FreeNode(n);
		}

		n = RemoveFirstExpired(curtime);
	}
	return expcount;
}

template <class TYPE>
void CTimerQueue<TYPE>::OutpuInfo(void) const
{
	OUTPUTLOG("ACE_Timer_Wheel_T::OutpuInfo");
	OUTPUTLOG("Start");

	char str[128];
	_snprintf(str, 128, "m_nSpokeCount = %d",m_nSpokeCount);
	OUTPUTLOG(str);
	_snprintf(str, 128, "m_nResBits = %d",m_nResBits);
	OUTPUTLOG(str);
	OUTPUTLOG("wheel = \n");

	unsigned long i = 0;
	for (; i < m_nSpokeCount; ++i)
	{
		_snprintf(str, 128, "%d",i);
		OUTPUTLOG(str);
		TimerNode<TYPE>* root = m_pSpokes[i];
		for (TimerNode<TYPE>* n = root->GetNext ();
			n != root;
			n = n->GetNext ())
		{
			n->OutpuInfo();
		}
	}
	OUTPUTLOG("End");
}

#endif