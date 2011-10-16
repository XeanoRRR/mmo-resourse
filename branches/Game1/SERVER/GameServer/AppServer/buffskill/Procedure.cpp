#include "StdAfx.h"
#include "Procedure.h"
#include "Step.h"
#include "EffectObjects.h"
#include "OpRegister.h"
#include "..\MoveShape.h"
#include "..\ServerRegion.h"
#include "..\public\GameEvent.h"
#include "..\..\GameServer\GameManager.h"


CProcedure::CProcedure(stModuParam *pParam)
:m_pParam(pParam)
{
	m_pBegin = NULL;
	m_pTimeOut = NULL;
	m_pEvent = NULL;
	m_pEnd = NULL;
}
CProcedure::~CProcedure(void)
{	
	MP_DELETE(m_pBegin);
	MP_DELETE(m_pTimeOut);
	MP_DELETE(m_pEvent);
	MP_DELETE(m_pEnd);
}
//��ʼ����������
void CProcedure::InitBaseData(string strStepName,CStep *pStep)
{
	if (strStepName=="Begin")
	{
		m_pBegin = pStep;
	}
	else if (strStepName=="TimeOut")
	{
		m_pTimeOut = pStep;
	}
	else if (strStepName=="Event")
	{
		m_pEvent = pStep;
	}
	else if (strStepName=="End")
	{
		m_pEnd = pStep;
	}
}
//��ʼ
eModuleRet CProcedure::Begin(stModuParam *pParam)
{
	eModuleRet eRet = MR_Continue;

	SetParam(pParam);
	if(m_pBegin)
	{

		eRet = m_pBegin->DoSome(m_pParam);
	}
	else
	{

	}
	return eRet;
}
//�¼�����ӿ�
eModuleRet CProcedure::Proc(DWORD type,const tagProcVar *pVar,stModuParam *pParam)
{
	eModuleRet eRet = MR_Proc_End;

	SetParam(pParam);
	if(m_pEvent)
		eRet = m_pEvent->DoSome(m_pParam);
	//����ʱȡ����ʱ�����¼�
	return eRet;
}
//��ʱ����ӿ�
eModuleRet CProcedure::OnTimeOut(DWORD timerid,DWORD curtime,const void* var,stModuParam *pParam)
{
	eModuleRet eRet = MR_Continue;

	SetParam(pParam);
	if(m_pTimeOut)
		eRet = m_pTimeOut->DoSome(m_pParam);	

	m_pParam->OnTimerOut(timerid);

	if(m_pParam->Timers.size() == 0)
		eRet = MR_Proc_Next;
	return eRet;
}


//�������ýӿ�
eModuleRet CProcedure::End(stModuParam *pParam)
{
	SetParam(pParam);
	if(m_pEnd)
		m_pEnd->DoSome(m_pParam);
	
	//�ж��Ƿ��ж�ʱ��
	if(m_pParam->Timers.size() > 0 )
	{
		COpUnRegisterTimer *pUnRegister = MP_NEW COpUnRegisterTimer(m_pParam, 0);
		pUnRegister->Execute(NULL);
		MP_DELETE(pUnRegister);
	}
	//�ж��Ƿ����¼�
	if(m_pParam->Events.size() > 0)
	{
		COpUnRegisterEvent *pUnRegister = MP_NEW COpUnRegisterEvent(m_pParam, 0);
		pUnRegister->Execute(NULL);
		MP_DELETE(pUnRegister);
	}
	return MR_Proc_End;
}
