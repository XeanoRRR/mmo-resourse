 //-----------------------------------------------------------------------------
 // File: State.cpp
 //
 // Desc: ״̬��Ĵ���
 //
 // Copyright (c) Aurora Game. All rights reserved.
 //-----------------------------------------------------------------------------

#include "stdafx.h"
#include "State.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
NAME:CState()
DESC:Ĭ�Ϲ��캯��
PARA:��
*/

CState::CState()
{
	m_dwID = 0;	
}


/*
NAME:CState(long lStateID)
DESC:���캯��
PARA:lStateIDΪ״̬��ID
*/

CState::CState(DWORD dwStateID)
{
	m_dwID = dwStateID;	
}

/*
NAME:~CState()
DESC:���캯��
PARA:��
*/

CState::~CState()
{
	Clear();
}

/*
NAME:Clear()
DESC:���
PARA:��
*/
void CState::Clear()
{
	m_dwID = 0;	
	for (vector<tagState*>::iterator it=m_vtgState.begin(); it!=m_vtgState.end(); ++it)
	{	
		for (vector<tagFettle*>::iterator Fit=(*it)->vtgBegingFettle.begin(); Fit!=(*it)->vtgBegingFettle.end(); ++Fit)
		{
			SAFE_DELETE(*Fit);
		}
		(*it)->vtgBegingFettle.clear();

		for (vector<tagFettle*>::iterator Fit=(*it)->vtgUpdateFettle.begin(); Fit!=(*it)->vtgUpdateFettle.end(); ++Fit)
		{			
			SAFE_DELETE(*Fit);
		}
		(*it)->vtgUpdateFettle.clear();


		for (vector<tagFettle*>::iterator Fit=(*it)->vtgEndFettle.begin(); Fit!=(*it)->vtgEndFettle.end(); ++Fit)
		{			
			SAFE_DELETE(*Fit);
		}
		(*it)->vtgEndFettle.clear();

		

		for (map<string,tagEvent*>::iterator mit=(*it)->mapEvent.begin(); mit!=(*it)->mapEvent.end();++mit)
		{
			if (NULL != mit->second)
			{
				SAFE_DELETE((mit->second));
			}						//mit.clear();->second
		}
		(*it)->mapEvent.clear();

		SAFE_DELETE(*it);

	}
	m_vtgState.clear();
}

/*
NAME:SetStateName(string strName)
DESC:����״̬����
PARA:strName״̬����
*/
void CState::SetStateName(string strName)
{
	m_strName = strName;
}

/*
NAME:SetCondition
DESC:��������
PARA:dwConID����ID
	 tgConditon����
*/
void CState::SetCondition(DWORD dwConID,tagCondition tgCondition)
{
	m_mapCondition[dwConID] = tgCondition;
}

/*
NAME:SetStateName(string strName)
DESC:����״̬����
PARA:strName״̬����
*/
void CState::SetStateDeathIsClear(DWORD dwDeathIsClear)
{
	m_dwDeathIsClear = dwDeathIsClear;
}

/*
NAME:SetStateID(long lStateID)
DESC:����״̬ID
PARA:lStateID״̬ID
*/
void CState::SetStateID(long lStateID)
{
	m_dwID = lStateID;
}



/*
NAME:SetStateOnly(BYTE byOnly)
DESC:ʩ�ź���״̬�Ƿ�Ψһ��
PARA:byOnly״̬�Ƿ�Ψһ1�����ǣ�0�����
*/
void CState::SetStateOnly(DWORD dwOnly)
{
	m_dwOnly = dwOnly;
}



/*
NAME:AddState(tagState& tgState)
DESC:���״̬
PARA:tgStateΪ״̬
*/
void CState::AddState(tagState *tgState)
{
	m_vtgState.push_back(tgState);		
}



/*
NMAE:GetState(DWORD dwLevel)
DESC:��ȡ״̬
PARA:dwLevel״̬ID�ȼ�
*/
CState::tagState* CState::GetState(DWORD dwLevel)
{

	for(vector<tagState*>::iterator it=m_vtgState.begin(); it!=m_vtgState.end(); it++)
	{
		if ((*it)->dwLevel == dwLevel)
		{
			return (*it);
		}
	}

	return NULL;
}

/*
NMAE:Serialize( BYTE** ppBuffer, size_t* pLength )
DESC:���л�
PARA:ppBuffer����
	 pLength ����
*/
bool CState::AddStateToByteArray(vector<BYTE>& vRet)	//ѹ��״̬
{
	_AddToByteArray(&vRet , m_dwID);
	_AddToByteArray(&vRet , (char*)(m_strName.c_str()));
	_AddToByteArray(&vRet , m_dwOnly);
	_AddToByteArray(&vRet , m_dwDeathIsClear);
	
	_AddToByteArray(&vRet , (long)m_mapCondition.size());//��������

	for (map<DWORD,tagCondition>::iterator it=m_mapCondition.begin(); it!=m_mapCondition.end(); ++it)
	{
		_AddToByteArray(&vRet,it->first);
		_AddToByteArray(&vRet,(long)((it->second).vtgGoodCondition.size()));
			for(vector<tagGoodCondition>::iterator goodsit = (it->second.vtgGoodCondition.begin()); goodsit!=it->second.vtgGoodCondition.end(); ++goodsit)
			{
				_AddToByteArray(&vRet, (*goodsit).dwPlace);
				_AddToByteArray(&vRet, (*goodsit).dwPos);
				_AddToByteArray(&vRet, (*goodsit).dwType);
			}

		_AddToByteArray(&vRet,(long)((it->second).vtgSkillID.size()));
		for(vector<DWORD>::iterator skillit = (it->second.vtgSkillID.begin());skillit!=it->second.vtgSkillID.end();++skillit)
		{
			_AddToByteArray(&vRet,*skillit);
		}
	}

	//״̬����
	_AddToByteArray(&vRet, (long)m_vtgState.size());

	for (vector<tagState*>::iterator it=m_vtgState.begin(); it!=m_vtgState.end(); ++it )
	{
		_AddToByteArray(&vRet, (*it)->dwLevel);
		_AddToByteArray(&vRet, (*it)->dwTime);
		_AddToByteArray(&vRet, (*it)->dwCount);
		_AddToByteArray(&vRet, (char*)(*it)->strBeginScript.c_str());
		_AddToByteArray(&vRet, (char*)(*it)->strUpdateScript.c_str());
		_AddToByteArray(&vRet, (char*)(*it)->strEndScript.c_str());
		//��ʼ����״̬����
		_AddToByteArray(&vRet, (long)(*it)->vtgBegingFettle.size());
		for (vector<tagFettle*>::iterator Fit = (*it)->vtgBegingFettle.begin(); Fit!=(*it)->vtgBegingFettle.end(); ++Fit)
		{
			_AddToByteArray(&vRet, (char*)((*Fit)->strFettleName.c_str()));
			_AddToByteArray(&vRet, (*Fit)->dwCondition);
			_AddToByteArray(&vRet, (char*)((*Fit)->strOpera.c_str()));
			_AddToByteArray(&vRet, (*Fit)->dwValue);
			_AddToByteArray(&vRet, (char*)((*Fit)->strFormat.c_str()));
		}

		//���»���״̬����
		_AddToByteArray(&vRet, (long)(*it)->vtgUpdateFettle.size());
		for (vector<tagFettle*>::iterator Fit = (*it)->vtgUpdateFettle.begin(); Fit!=(*it)->vtgUpdateFettle.end(); ++Fit)
		{
			_AddToByteArray(&vRet, (char*)((*Fit)->strFettleName.c_str()));
			_AddToByteArray(&vRet, (*Fit)->dwCondition);
			_AddToByteArray(&vRet, (char*)((*Fit)->strOpera.c_str()));
			_AddToByteArray(&vRet, (*Fit)->dwValue);
			_AddToByteArray(&vRet, (char*)((*Fit)->strFormat.c_str()));
		}

		//��������״̬����
		_AddToByteArray(&vRet, (long)(*it)->vtgEndFettle.size());
		for (vector<tagFettle*>::iterator Fit = (*it)->vtgEndFettle.begin(); Fit!=(*it)->vtgEndFettle.end(); ++Fit)
		{
			_AddToByteArray(&vRet, (char*)((*Fit)->strFettleName.c_str()));
			_AddToByteArray(&vRet, (*Fit)->dwCondition);
			_AddToByteArray(&vRet, (char*)((*Fit)->strOpera.c_str()));
			_AddToByteArray(&vRet, (*Fit)->dwValue);
			_AddToByteArray(&vRet, (char*)((*Fit)->strFormat.c_str()));
		}
		//�¼�����
		_AddToByteArray(&vRet,(long)(*it)->mapEvent.size());

		for (map<string,tagEvent*>::iterator mit = (*it)->mapEvent.begin();
			 mit!=(*it)->mapEvent.end(); ++mit)
		{
			_AddToByteArray(&vRet,(char*)(mit->first.c_str()));//.c_str()
			_AddToByteArray(&vRet,(mit->second)->dwCondition);
			_AddToByteArray(&vRet,(char*)((mit->second)->strScripte.c_str()));
		}

	}


	return true;
}





