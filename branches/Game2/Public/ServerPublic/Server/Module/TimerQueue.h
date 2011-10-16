//=============================================================================
/**
*  file: TimerQueue.h
*
*  Brief:ͨ��ʱ����ԭ��ʵ�ֶ�ʱ�����ܡ�
*
*  Authtor:wangqiao
*	
*	Datae:2007-5-15
*/
//=============================================================================

#ifndef __TIMER_QUEUE_H__
#define __TIMER_QUEzUE_H__
#pragma once


template <class TYPE>
class CTimerQueue;

const int MAX_TIME = 0x8FFFFFFF;
//#define OUTPUTLOG(X) AppFrame::AddStdLog(X)

#define OUTPUTLOG(X) Log4c::Trace(ROOT_MODULE,X)

/**
* class: TimerNode
*
* Brief: �����˶�ʱ����һ���ڵ���������
*
*/

template <class TYPE>
class TimerNode
{
private:
    //�洢�����ݿ�Ķ�������
    TYPE			m_Type;
    //�Ͷ�ʱ�����������ɱ�ʾ
    const	void*	m_pArg;
    //������ʱ��ֵ(����)
    ulong 	m_dwTimeValue;
    //�����һ�������ԵĶ�ʱ����������һ�ν�Ҫ������ֵ
    ulong 	m_dwInterval;	
    //��ʱ��ID
    ulong 	m_dwTimerID;
    //ʵ��˫������
    TimerNode<TYPE> *m_pRev;
    TimerNode<TYPE> *m_pNext;
public:
    TimerNode(void);
    ~TimerNode(void);

    //������ص�����ֵ
    void Set(const TYPE &type,
        const	void* arg,
        ulong 	dwTimeValue,
        ulong 	dwInterval,
        ulong 	dwTimerID,
        TimerNode<TYPE> *p,
        TimerNode<TYPE> *n);

    //�õ�type
    TYPE &GetType(void);
    //����type
    void SetType(TYPE &type);
    //�õ��û�����
    const void *GetArg(void);
    //�����û�����
    void SetArg(void *arg);
    //�õ���ʱ��ʱ��ֵ
    ulong GetTimerValue(void) const;
    //���ö�ʱ��ʱ��ֵ
    void SetTimerValue(ulong timervalue);
    //�õ���ʱ�����
    ulong GetInterval(void) const;
    //���ö�ʱ�����
    void SetInterval(ulong interval);

    TimerNode<TYPE> *GetPrev(void);
    void SetPrev(TimerNode<TYPE> *prev);

    TimerNode<TYPE> *GetNext(void);
    void SetNext(TimerNode<TYPE> *next);
    //�õ���ʱ��ID
    long GetTimerID(void) const;
    //���ö�ʱ��ID
    void SetTimerID(long timerid);
    //ʱ������ʱ�����������
    void OnTimerOut(ulong);
    //ȡ����ʱ��ʱ�����������
    void OnTimerCancel();
    //�����ϸ��Ϣ
    void OutpuInfo(void) const;
};


/**
* class: CTimerQueue
*
* Brief: һ��Iterator,��������Wheel�е����нڵ�.
*			���ᰴ��ʱ�䳬ʱֵ������˳�����
*/

template <class TYPE>
class CTimerQueueIterator
{
private:
    //ָ��Ҫ�����Ķ�ʱ��
    CTimerQueue<TYPE>&	m_TimerQueue;
    //��ʱ�����еĵ�ǰλ��
    ulong	m_nSpoke;
    //ָ��ǰ����λ���ϵĽڵ�ָ��
    TimerNode<TYPE>	*m_pCurNode;
private:
    void GoToNext(ulong nSartSpoke);
public:
    typedef CTimerQueue<TYPE> Queue;
    typedef TimerNode<TYPE> Node;

    CTimerQueueIterator(CTimerQueue<TYPE> &);
    ~CTimerQueueIterator(void);

    //��λ�ڵ�һ��spoke�ĵ�һ���ڵ�
    void First (void);
    //ָ��ǰ�ڵ����һ���ڵ�
    void Next (void);
    //���ڱ�����������Ҳû�нڵ�ʱ,����true.
    int IsDone (void) const;
    //���ش��ڱ������еĵ�ǰ�ڵ�
    TimerNode<TYPE>* Item (void);
};


/**
* class:	CTimerQueueIterator
*
* Brief:	��ʱ�䶨ʱ��ԭ��ʵ�ֵ�һ����ʱ��.
*			ʵ��ʹ����һ�������Ծ���ʱ������˫�������һ��Hash��,
*			��ʱ�ڵ�Ҳ����Ԥ���书��
*
*/

template <class TYPE>
class CTimerQueue
{
private:
    //ʱ����
    TimerNode<TYPE>** m_pSpokes;
    //ʱ���ִ�С
    ulong	m_nSpokeCount;
    //�����ַ���TimerID��λ��
    ulong	m_nSpokeBits;
    //ÿ���ַ��Ķ�ʱ���������
    ulong	m_nMaxPerSpoke;
    //ʱ���ֵĽ�����(��΢��)
    long			m_nResBits;
    //���ʱ����б�����
    ulong	m_nEarliestSpoke;
    //��ʱ���ĵ�����
    CTimerQueueIterator<TYPE>* pIterator;
    //�ظ�һ��ʱ����������ʱ������(resolution * m_nSpokeCount)
    ulong	m_nWheelTime;
    //��ǰ��ʱ�䶨ʱ������
    ulong	m_nTimerCount;

    //����Ԥ����ڵ���б�
    list<TimerNode<TYPE>*> m_FreeList;
private:
    //�õ�һ��2����ֵ
    int Power2Bits (int n, int minbits, int maxbits);
    TimerNode<TYPE>* AllocNode();
    void FreeNode(TimerNode<TYPE>*);

    //������ֹʹ�ÿ������캯���͸�ֵ������
    CTimerQueue(const CTimerQueue &);
    void operator=(const CTimerQueue&);
private:

    TimerNode<TYPE>* GetFirstI(void) const;
    TimerNode<TYPE>* RemoveFirstExpired(ulong now);
    void OpenI(	size_t prealloc,
        ulong spokes,
        ulong res);
    void Reschedule(TimerNode<TYPE> *);
    TimerNode<TYPE>* FindSpokeNode(	ulong spoke,
        long timerid) const;
    TimerNode<TYPE>* FindNode(long timerid) const;
    ulong CalculateSpoke(ulong expire) const;
    long GenerateTimerID(ulong spoke);
    void ScheduleI(TimerNode<TYPE>* n,
        ulong spoke,
        ulong expire);
    void CancelI(TimerNode<TYPE>* n);
    void Unlink(TimerNode<TYPE>* n);
    void RecalcEarliest(ulong last);

    long ScheduleI(const TimerNode<TYPE>& type,
        const void* arg,
        ulong futuretime,
        ulong interval);
public:

    //��������spoke��С��resolution��Ԥ����ڵ��С���캯��
    CTimerQueue (ulong spokecount,
        ulong resolution,
        ulong prealloc = 0);
    ~CTimerQueue (void);

    //�������Ϊ�շ���true,����false
    int IsEmpty(void) const;
    //�������紥���Ķ�ʱ��ID
    ulong EarliestTime(void) const;
    //�ı�һ����ʱ���ļ��(interval==0:���óɷ������ԵĶ�ʱ��,
    //��֮���óɸü��ֵ�������Զ�ʱ�ڵ�)
    int ResetInterval(long timerid,ulong interval);

    //ȡ�����е���<type>�Ķ�ʱ��
    //����ֵ��ʾȡ���Ķ�ʱ������
    int Cancel(const TYPE& type);

    //ȡ������<m_dwTimerID>����<timerid>�Ķ�ʱ��
    //���arg�����,��Ҫ���õ���ʱ�����õ�<arg>����
    //����ֵ��ʾȡ���Ķ�ʱ������
    int Cancel(long timerid, const void** arg = 0);

    //����timerGetTime()����,Ϊ���ж�ʱ��С��curtimeʱ��Ĵ�����ʱ����
    //����ȡ���Ķ�ʱ�ڸ���
    int Expire(void);
    //�������curtimeΪ׼,Ϊ���ж�ʱ��С��curtimeʱ��Ĵ�����ʱ����
    //����ȡ���Ķ�ʱ�ڸ���
    int Expire(ulong);

    //��<type>����Ԥ��һ����ʱ��,�����ʱ����Ҫ�ھ���ʱ��futuretime����
    //interval:�������0,��ʾ����һ�������Զ�ʱ��,��һ�����ʱ��
    //��ʾ��ʱ����һ�δ����ļ��ʱ��,�ú�������-1��ʾ����ʧ��
    //arg��һ���û��Զ������	
    long Schedule(	const TYPE &type,
        const void *arg,
        ulong futuretime,
        ulong interval = 0);

    //����һ��CTimerQueue��Iteratorָ��
    CTimerQueueIterator<TYPE>& Iter(void);
    //�Ƴ����紥����ʱ��ڵ�,��������
    TimerNode<TYPE>* RemoveFirst(void);
    //�õ����紥����ʱ��ڵ�,��������
    TimerNode<TYPE>* GetFirst(void);
    //�����ϸ��Ϣ
    void OutpuInfo(void) const;

public:
    //����CTimerQueueIteratorΪ��Ԫ��
    friend class CTimerQueueIterator<TYPE>;
};

//��������
#include "TimerQueue.tcc"

#endif