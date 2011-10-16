#include "StdAfx.h"
#include "Procedure.h"

#include "EffectObjects.h"
#include "OpRegister.h"
#include "Step.h"
#include "../MoveShape.h"
#include "../ServerRegion.h"
#include "../../Server/GameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProcedure::CProcedure(stModuParam *pParam)
:m_pParam(pParam)
{
	m_pBegin = NULL;
	m_pTimeOut = NULL;
	m_pEvent = NULL;
	m_pEnd = NULL;
}
CProcedure::CProcedure(void * pParam)
:m_pParam((stModuParam*)pParam)
{
	m_pBegin = NULL;
	m_pTimeOut = NULL;
	m_pEvent = NULL;
	m_pEnd = NULL;
}
CProcedure::~CProcedure(void)
{	
	OBJ_RELEASE(CStep, m_pBegin);
	OBJ_RELEASE(CStep, m_pTimeOut);
	OBJ_RELEASE(CStep, m_pEvent);
	OBJ_RELEASE(CStep, m_pEnd);
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
		COpUnRegisterTimer::tagParam param;
		param.pParam = m_pParam;
		param.lFlag = 0;
		COpUnRegisterTimer *pUnRegister = OBJ_CREATE_PVOID(COpUnRegisterTimer, (void*)&param);
		pUnRegister->Execute(NULL);
		OBJ_RELEASE(COpUnRegisterTimer, pUnRegister);
	}
	//�ж��Ƿ����¼�
	if(m_pParam->Events.size() > 0)
	{
		COpUnRegisterEvent::tagParam param;
		param.pParam = m_pParam;
		param.lFlag = 0;
		COpUnRegisterEvent *pUnRegister = OBJ_CREATE_PVOID(COpUnRegisterEvent, (void*)&param);
		pUnRegister->Execute(NULL);
		OBJ_RELEASE(COpUnRegisterEvent, pUnRegister);
	}
	return MR_Proc_End;
}
