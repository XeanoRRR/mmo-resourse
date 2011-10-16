 
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
/*
 tanhaowen 2009.1.23 �޸���� For AutoDriveClient
*/
#ifndef __MAP_SCRIPT_SYSTEM_H__
#define __MAP_SCRIPT_SYSTEM_H__

#pragma once
// #include "..\Utility\TimerDef.h"
// #include "..\Utility\GameEvent.h"
// #include "..\Utility\TimerDef.h"
// #include "../Utility//GUID.h"
// 
// class CMessage;
// class CGameTestDlg;
// 
// class CMapScriptSystem : public CGameEvent
// {
// public:
// 	CMapScriptSystem(void);
// 	virtual ~CMapScriptSystem(void);
// 
// 	struct tagMapScriptTimer
// 	{
// 		//����1
// 		unsigned long arg;
// 	};
// private:
// 	//Ϊ�ű��ṩ�Ļ������ݿ�
// 	BYTE* m_pDB;
// 	//������С
// 	int	m_nDBSize;
// 	//MapFile�ű�״̬��ʵ��
// 	static CMapScriptSystem *m_MapSYSInstance;
// 	//�ű�״̬��
// 	lua_State			    *m_lMapLuaState;
// 	//TestDialog
// 	CGameTestDlg		    *m_pTestDialog;
// 	//ͳһ����ű���Ķ�ʱ��
// 	typedef map<long, tagMapScriptTimer*>	MapScriptTimer;
// 	map<long, tagMapScriptTimer*>::iterator itScriptTimer;
// 	MapScriptTimer	m_mapScriptTimers;							
// 
// public:
// 	//��������ȡMapFile�ű�״̬��ʵ��
// 	static CMapScriptSystem		*getInstance();
// 	//�ͷ���Դ
// 	static bool					 Release();
// 	//��ʼ��
// 	bool						 Init(CGameTestDlg *pTestDialog, const char *strScriptFile=NULL);
// 	//����Lua_state
// 	bool						 UnInitial(); 
// 	//�ռ��ڴ�����
// 	int							 LuaGC(int nGcType);
// 	//��������ȡ�ű���������
// 	BYTE						*GetDB(int minsize);
// 	//��ȡLua״̬��
// 	lua_State					*GetLuaState()			{ return m_lMapLuaState;}
// 	//��ȡ��ǰmfc�����
// 	CGameTestDlg				*GetTestDialog()		{ return m_pTestDialog;}
// public:
// 	////���������� ������
// 	//virtual long				 OnTimerRead(/*CMapFile * pMap*/);
// 	////���������� д����
// 	//virtual void				 OnTimerWrite();
// 
// 	int							 RunMessage(CMessage *pMsg);
// 	//����Lua��Ҫ�Ľű��������ظ�����
// 	bool				         LoadLuaRequireFile(const char *fileName);
// 	void						 StartTest(const char *strFielName);		
// 	void						 PerformString(const char *strContent);			
// 	void						 EndTest(const char *strFielName);
// 	//��ʱ��Ӧ
// 	virtual long				 OnTimeOut(DWORD timerid, DWORD curtime, const void *var);
// 	//����ʱ����ȡ���ĵ���
// 	virtual void				 OnTimerCancel(DWORD timerid, const void *var);
// 	//������Ϣ��Ӧ
// 	virtual void				 OnMessage(CMessage*) {}
// 	//����ʵ�����Ϣ����
// 	virtual long				 Proc(DWORD type, const void *var1, const void *var2)		{ return 0; }
// 	//����һ����ʱ��
// 	long						 Schedule(unsigned long arg,
// 								          unsigned long futuretime,
// 										  unsigned long interval = 0);
// 	//ȡ��һ����ʱ��
// 	int							 Cancel(long timerid);
// 	//ִ�нű�
// 	void						 RunScript(string ScriptName);
// 	//
// 	long						 SendInfo(string strCDkey, CMessage *msg);
// 	long						 AcceptInfo(string strCDkey, CMessage *msg);
// 	//��̬�¼Ӳ��Խű�
// 	bool						 AddScriptFile(const char *strScriptFileName);
// 	bool						 AddScriptString(const char *strString);
// 	//����ָ����lua�ļ�
// 	bool						 LoadOneScriptFile(const char *strFileName);
// };

#endif //__MAP_SCRIPT_SYSTEM_H__

