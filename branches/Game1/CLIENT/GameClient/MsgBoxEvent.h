#pragma once
#include <vector>
#include <list>

using namespace std;

//////////////////////////////////////////////////////////////////////////
//MsgBoxEvent.h
//Func:MsgBox���е��¼���
//Author:joe
//Create Time:2005.2.2
//////////////////////////////////////////////////////////////////////////

enum eMsgBoxEvent
{
	MBE_DIEDSELECT,				//����֮��ѡ��ĸ����¼�
};

class CMsgBoxEvent
{
public:
	struct tagMsgBoxEvent
	{
		eMsgBoxEvent			eEvent;		//ʱ������
		vector<LONG_PTR>		Params;		//�����б�
	};
	list<tagMsgBoxEvent*>::iterator itEvent;

public:
	CMsgBoxEvent(void);
	~CMsgBoxEvent(void);
private:
	list<tagMsgBoxEvent*>		m_MsgBoxEvents;
public:
	void	AddEvent(eMsgBoxEvent eEvent,...);
	void	ClearAllEvent();
	
	void	Run();							//���� MsgBox ��������
	void	Display();						//��ʾMsgBox
};
