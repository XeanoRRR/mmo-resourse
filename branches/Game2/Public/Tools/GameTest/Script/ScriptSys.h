
//=============================================================================
/**
 *  file: ScriptSys.h
 *
 *  Brief:ʵ�ֶ�<lua_State* >״̬�Ĺ���
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-7-25
 */
//=============================================================================
#ifndef __SCRIPT_SYSTEM_H__
#define __SCRIPT_SYSTEM_H__

#pragma once
#include "..\Utility\TimerDef.h"

class CMessage;
class CGameTestDlg;
class CTestScript;

class CScriptSys:
		public CGameEvent
{
public:
	virtual ~CScriptSys(void);

	struct tagScriptTimer
	{
		//����1
		unsigned long arg;
	};
private:
	CScriptSys(void);
	//	�̻߳���
	HANDLE m_hStateMutex;
//	static CScriptSys* instance;
	//�ű�״̬��
	lua_State* m_luaState;
	//TestDialog
	CGameTestDlg* m_pTestDialog;

	//ͳһ����ű���Ķ�ʱ��
	typedef map<long,tagScriptTimer*>	ScriptTimers;
	typedef ScriptTimers::iterator		itScriTimer;

	ScriptTimers	m_ScriptTimers;

	//Ϊ�ű��ṩ�Ļ������ݿ�
	BYTE* m_pDB;
	//������С
	int	m_nDBSize;
public:
	static CScriptSys* getInstance();

	static bool Release();

	bool Init(CGameTestDlg* pTestDialog);
	
	bool UnInitial(); 

	lua_State* GetLuaState()	{return m_luaState;}
	BYTE* GetDB(int minsize);
	
	CGameTestDlg* GetTestDialog()	{return m_pTestDialog;}
public:
	//
	void RunMessage(CMessage *pMsg);
	//����ָ����lua�ļ�
	BOOL LoadOneFileOrString(const char* strFileName);
	//����Lua��Ҫ�Ľű��������ظ�����
	//bool LoadLuaRequireFile(const char* fileName);
	//void StartTest(const char* strFielName);		
	void PerformString(const char* strContent);			
	//void EndTest(const char* strFielName);
	
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//����ʱ����ȡ���ĵ���
	virtual void OnTimerCancel(DWORD timerid,const void* var);
	
	//������Ϣ��Ӧ
	virtual void OnMessage(CMessage*){}
	//����ʵ�����Ϣ����
	//virtual long Proc(DWORD type,const void* var1,const void* var2){return 0;}
    virtual long Proc(eEventType type,const tagProcVar *pVar) { return 0; }
	//����һ����ʱ��
	long Schedule(	unsigned long arg,
					 unsigned long futuretime,
					 unsigned long interval = 0);
	//ȡ��һ����ʱ��
	int Cancel(long timerid);
	//���ӵ�������
	int ConnectLoginServer();
	int ConnectServer(const char* strIP,int nPort,long lFlag);
	int ShutDownSocket(long lSocket);
private:
	//ͨ��TESTDIALOG������������������Ҫ���ӵ�IP��ַ
	string m_strIp;
	//ͨ��TESTDIALOG������������������Ҫ�Ĳ�������
	int m_TestType;
	//ͨ��TESTDIALOG�ı༭��õ��ķ�����Ϣ��ʱ����
	long m_lTestTimer;
};

#endif

