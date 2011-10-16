///////////////////////////////////////////////////////////////////////////////
///  StateFactory.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\WorldServer\AppWorld\State
///
///  CREATED: 06/07/2007 12:50:01 PM by ���Ƚ�
///
///  PURPOSE:   ״̬������
///
///  COPYRIGHT NOTICE:   Copyright (C) Aurora Game
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// StateFactory.h: interface for the CStateFactory class.
//
//////////////////////////////////////////////////////////////////////
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_STATE_FACTORY_INCLUDED
#define _INC_STATE_FACTORY_INCLUDED
#include "../../../../public/tinyxml.h"
#include "State.h"
#include <map>
using namespace std;


class CState;

class CStateFactory
{

private:
	//״̬����ʱ�б�
	static map<DWORD, CState*>					g_mStateMap;

public:

	//��ȡ״̬�����ļ�
	static HRESULT			LoadConfigration	( const string p );
	//ͨ���ڵ�����
	static HRESULT			LoadConfigration	( TiXmlElement *pElem );
	//��������ȼ���״̬
	static HRESULT			LoadLevelConfigration( TiXmlElement *pElem, CState *pState );
	//���������ж�
	static HRESULT			LoadConditionConfigration( TiXmlElement *pElem, CState *pState );
	//���뿪ʼ����״̬����
	static HRESULT			LoadPropertyConfigration(TiXmlElement *pElem, CState::tagState *ptgState);
	//���뿪ʼ״̬����
	static HRESULT			LoadBeginConfig(TiXmlElement *pElem,CState::tagState *ptgState);
	//�����¼������ű�
	static HRESULT		    LoadEventConfig(TiXmlElement *pElem,CState::tagState *ptgState);

	//�������״̬����
	static CState::tagFettle *LoadBaseProperty(TiXmlElement *pElem);//,CState::tagState *ptgState
	//�������״̬����
	static HRESULT		    LoadTimeOutConfig(TiXmlElement *pElem,CState::tagState *ptgState);
	//�������״̬����
	static HRESULT		    LoadEndConfig(TiXmlElement *pElem,CState::tagState *ptgState);


	//���뿪ʼ�ű�
	static HRESULT			LoadScriptConfigration(TiXmlElement *pElem, CState::tagState *ptgState);
	//���״̬����
	static void				ClearStateCache		();								
	//����״̬����
	static bool				LoadStateCache		();
	//����
	static bool				CodeState			( vector<BYTE>& vRet );	

};

#endif//_INC__STATE_FACTORY_INCLUDED