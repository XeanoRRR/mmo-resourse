#pragma once

// -------------------------------------------------------------------------------------

#include <queue>

class CMySocket;
class CMsgQueue;

struct stSend
{
	CMySocket* pSocket;
	DWORD dwSocketID;
	BYTE* pBuffer;
	int size;
};

extern CMsgQueue* g_MsgQeue;
extern queue<int> g_MsgTime;
extern BOOL g_bNetDelay;
extern long g_lNetDelayInMin;
extern long g_lNetDelayInRange;
extern long g_lNetDelayOutMin;
extern long g_lNetDelayOutRange;

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")

CMsgQueue* GetMsgQeue();

// ��ȡģ������
void InitNetDelay();

void ExitNetDelay();


// ---- TEST ----
// ѹ��Ϣ������߳�
unsigned __stdcall PushMessageThread(void* pArguments);
// ---- TEST ----
// ������Ϣ�߳�
unsigned __stdcall SendThread(void* pArguments);
