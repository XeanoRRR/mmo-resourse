#pragma once

enum timeType
{
	GAPP_LOADRES=0,//������Դ��ʱ
	GAPP_MAINLOOP,//����ʱ��ѭ����ʱ
	GAPP_SHUTDOWN,//�رճ�ʱ
	GAPP_PING,//����������ʱ
};

//��ʱ����(void* var)�����ṹ
struct tagSMTimerVar
{
	tagSMTimerVar(){lvar=0;lvar1=0;pvar=NULL;}
	timeType TimerType;
	long lvar;
	long lvar1;
	void *pvar;
};