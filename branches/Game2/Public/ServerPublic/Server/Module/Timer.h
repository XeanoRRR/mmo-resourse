////////////////////////////////////////////////////////////////////////////////
//Timer.h/////////////////////////////////////////////////////////////////////
//ʱ�䶨ʱ����:
//function:1,���ĳ��ʱ���Ķ�ʱ�¼�����2,��һ����ʱ������
//Author:Joe
//Create Time:2004.7.28
#pragma once
#include "../../../Module/MulTiThMap.h"

typedef LRESULT (CALLBACK* CallBackFun)(LPARAM lparam);
class CTimer
{
private:
	CTimer(void);
	~CTimer(void);
public:
	//ʱ���ʱ��
	struct tagTimer
	{
		ULONG	uID;
		bool	bOpen;				//�Ƿ�򿪴μ�ʱ��
		bool	bUseAITick;			//�Ƿ���AITick
		ULONG	uElapseTime;
		ULONG	uStartTime;
		ULONG	uElapseAITick;
		ULONG	uStartAITick;
		LPARAM  lparam;
		CallBackFun pCallBack;
	};
	//ʱ�䶨ʱ��
	struct tagTimeEvent
	{
		ULONG	uID;
		//bool	bTriger;			//��ʾ��ʱ���¼�������
		tagTime Time;				//�¼�������
		LPARAM  lparam;
		CallBackFun pCallBack;
	};
private:
	MulTiThMap<ULONG,tagTimer>			m_Timers;		//��ʱ������
	MulTiThMap<ULONG,tagTimeEvent>		m_TimeEvents;	//��ʱ�¼�����

	typedef	MulTiThMap<ULONG,tagTimer>::iterator itTimer;
	typedef MulTiThMap<ULONG,tagTimeEvent>::iterator itTimeEvent;

	static CTimer* instance;				//ȫ��Ψһ��ʵ��

private:
	ULONG	GetULONGID()
	{
		static	ULONG uID = 1;
		return uID++;
	}
public:
	static CTimer* getInstance();
	static void	Release();								//�ͷ���Դ

	bool	SetTimerParam(ULONG uID,LPARAM lparam);
	bool	SetTimerCallBackFun(ULONG uID,CallBackFun pCallBackFun);

	ULONG	SetTimeEvent(tagTime& Time,CallBackFun pCallBackFun,LPARAM  lparam);//����һ��ʱ���¼�	
	bool	KillTimeEvent(ULONG uID);
	bool	SetTimeEventParam(ULONG uID,LPARAM lparam);
	bool	SetTimeEventCallBackFun(ULONG uID,CallBackFun pCallBackFun);

	void	Run();
};

CTimer* GetTimer();