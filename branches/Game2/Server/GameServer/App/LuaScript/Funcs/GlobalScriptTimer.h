///
/// @file GlobalScriptTimer.h
/// @brief �ű��ڲ�ֱ��ʹ��
/// @author wangqiao
///
#ifndef __GLOABAL_SCRIPT_TIMER_H__
#define __GLOABAL_SCRIPT_TIMER_H__
#pragma once

class CGlobalScriptTimer:public CGameEvent
{
	typedef enum eTimerScriptType
	{
		TST_Wait=0,
		TST_Timer,
	}eTST;
public:
	CGlobalScriptTimer(void);
	~CGlobalScriptTimer(void);

private:
	static CGlobalScriptTimer* instance;

public:
	static CGlobalScriptTimer* GetInstance();
	static void Release();

private:
	struct tagScriptTimer
	{
		long lUserData;
		string	strScript;
		bool bLoop;//�Ƿ�ѭ��
	};
	//ע��Ľű���ʱ������
	typedef map<long,tagScriptTimer>	mapScriptTimers;
	typedef map<long,tagScriptTimer>::iterator itST;
	//��ע��Ľű���ʱ��
	mapScriptTimers m_ScriptTimers;


	//���ű�ִ�е�wait����ʱ�����ƵĶ�ʱ��
	typedef map<long,CGUID>	mapWaitScripts;
	typedef map<long,CGUID>::iterator itWS;
	//��ע��Ľű���ʱ��
	mapWaitScripts m_WaitScripts;

public:
	//����һ���ű���ʱ��
	long RegisterTimer(long lFutureTime,long lInterTime,
							const char* pszScriptName,long lUserData);
	//ע��һ���ű���ʱ��
	long UnRegisterTimer(long lUserData,long lTimerID);

	//����ִ��wait�����Ľű�
	void RegisterWaitScript(const CGUID& guScriptID,long lFutureTime);
	//������нű���ʱ��
	void ClearScriptTimer();

	//������Ϣ��Ӧ
	virtual void OnMessage(CMessage*)	{ return; }
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//����ʱ����ȡ���ĵ���
	void OnTimerCancel(DWORD timerid,const void* var){return;}
	//����ʵ�����Ϣ����
	virtual long Proc(eEventType type,const tagProcVar *pVar){return 0;}
};

#endif
