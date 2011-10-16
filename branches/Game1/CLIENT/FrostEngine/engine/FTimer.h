#pragma once

//��ʱ��
#include "fsingleton.h"

#define MAX_TIME_COUNTER  0x20

class FTimer
{
	DECLARE_SINGLETON(FTimer);

	//����FrostingEngine.Update()��
	//���㵱ǰʱ�������
	//�ṩ�̶���ˢ��֡��
	
	//��Ϣ����-����


	//��Ⱦ����

	FTimer(void);
	~FTimer(void);

	//__��ʱ__
	struct _tagCounter
	{
		char  szName[256];
		DWORD dwBegineTime;
		DWORD dwEndTime;
		DWORD dwUsedTime;

		BOOL  bUsed;
	};

	_tagCounter m_pCounterArray[MAX_TIME_COUNTER];

public:
	
	DWORD FrameRate(void);

	void Create();
	void Destroy();

	void BeginCount(const char *pszName,DWORD dwCountIndex);
	void EndCount(DWORD dwCountIndex);

	void OutputCounter();
};
