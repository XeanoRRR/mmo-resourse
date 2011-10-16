///////////////////////////////////////////////////////////////////////////////
///  State.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\WorldServer\AppWorld\State
///
///  CREATED: 06/06/2007 13:49:01 PM by ���Ƚ�
///
///  PURPOSE:   ״̬��
///
///  COPYRIGHT NOTICE:   Copyright (C) Aurora Game
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// State.h: interface for the CState class.
//
//////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_STATE_INCLUDED
#define _INC_STATE_INCLUDED
#include <vector>
#include <string>
#include <map>
using namespace std;


class CState
{
public:
	struct tagFettle
	{	
		string strFettleName;				//����״̬����		
		DWORD dwCondition;					//����ID		
		string strOpera;					//������
		DWORD dwValue;  					//����״̬������ֵ
		string strFormat;					//���㹫ʽ
	}; 
	struct tagEvent
	{
		DWORD dwCondition;
		string strScripte;
	};

	struct tagState
	{
		DWORD dwLevel;						//�ȼ�
		DWORD dwTime;						//���ʱ��
		DWORD dwCount;						//����

		vector<tagFettle*> vtgBegingFettle;	//��ʼ����״̬		
		string strBeginScript;				//�ű�

		vector<tagFettle*> vtgUpdateFettle;	//���»���״̬		
		string strUpdateScript;				//���½ű�


	
		vector<tagFettle*> vtgEndFettle;	//�����Ļ���״̬
		string strEndScript;				//�����ű�

		map<string,tagEvent*> mapEvent;		//�¼��ű�

	};

	struct tagGoodCondition
	{
		DWORD dwPlace;						//�ص�
		DWORD dwPos;						//λ��
		DWORD dwType;						//����
	};
	struct tagCondition
	{
		vector<tagGoodCondition> vtgGoodCondition;			//��Ʒ����
		vector<DWORD>			  vtgSkillID;		    	//��������
	};
	
private:
	
	DWORD						m_dwID;						//״̬ID	
	string						m_strName;					//״̬����
	DWORD						m_dwOnly;					//ʩ�ź���״̬�Ƿ�Ψһ��	
	DWORD						m_dwDeathIsClear;			//�ɼ�״̬�Ƿ�������ʱ�����
	map<DWORD,tagCondition>		m_mapCondition;				//����
	vector<tagState*>			m_vtgState;					//״̬
	
	



public:
	CState(void);
	CState(DWORD dwStateID);
	virtual ~CState(void);

	void 			AddState(tagState  *tgState);						//���״̬	
	void			SetCondition(DWORD dwConID,tagCondition tgCondition);//��������
	void			SetStateName(string strName);						//����״̬����
	void			SetStateID(long lStateID);							//����״̬ID
	void			SetStateOnly(DWORD dwOnly);							//����ʩ�ź���״̬�Ƿ�Ψһ��
	void			SetStateDeathIsClear(DWORD dwDeathIsClear);			//���ÿɼ�״̬�Ƿ�������ʱ�����
	void			SetStateLevel(long lLevel);							//״̬�ȼ�
	bool			AddStateToByteArray(vector<BYTE>& vRet);			//ѹ��״̬
	tagState*		GetState(DWORD dwLevel);							//��ȡ״̬

	void			Clear();											//���


};

#endif//_INC_STATE_INCLUDED