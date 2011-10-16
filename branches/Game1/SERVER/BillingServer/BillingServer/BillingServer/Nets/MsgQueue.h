#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
using namespace std;

class CBaseMessage;
typedef deque<CBaseMessage*> msgQueue;

/* -------------------------------------------
  [CMsgQueue] ��Ϣ����

  ����:
	�洢���յ�����Ϣ���ȴ�����

---------------------------------------------*/
class CMsgQueue  
{
private:
	msgQueue m_msgQueue;	// ����
	CRITICAL_SECTION m_CriticalSectionMsgQueue;

public:
	CMsgQueue();
	virtual ~CMsgQueue();

	bool PushMessage(CBaseMessage* pMsg);	// ѹ����Ϣ
	CBaseMessage* PopMessage();				// ������Ϣ
	void	GetAllMessage(msgQueue& pTemptMsgQueue);

	long GetSize();							// �õ���Ϣ���г���
	void Clear();							// �����Ϣ
};

#endif