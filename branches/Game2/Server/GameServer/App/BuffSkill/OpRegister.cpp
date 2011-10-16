#include "StdAfx.h"
#include "OpRegister.h"

#include "../Player.h"
#include "../ServerRegion.h"
#include "../../Server/GameManager.h"

#include "../../../../Public/Setup/GlobalSetup.h"
#include "../../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../../Public/ServerPublic/Server/Common/VarListUtils.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

COpRegisterTimer::COpRegisterTimer(stModuParam *pParam)
:COperator(pParam)
{
}
COpRegisterTimer::COpRegisterTimer(void* pParam)
:COperator((stModuParam*)pParam)
{

}
COpRegisterTimer::~COpRegisterTimer(void)
{

}

void COpRegisterTimer::Execute(CMoveShape* pShape)
{
    CMoveShape* pDest = pShape;
	pShape = GetOpShape(pShape);
	if(NULL == pShape)	return;

	stTimerParam *pTimerparam =
		m_pParam->FindTimerParam(m_lFlag,pShape->GetType(),pShape->GetExID());
	//��������
	if(pTimerparam)
	{
		pTimerparam->lInter = m_lInter;
		//�Ѿ�ע��
		if(pTimerparam->lTimerID == -1)
		{
			pTimerparam->nStartTime = timeGetTime();

			tagTimerVar* pTimvar = OBJ_CREATE(tagTimerVar);
			pTimvar->TimerType = (eChildObjType)m_eChildObjType;
			pTimvar->lvar = m_pParam->dwGlobleID;
			pTimvar->lvar1 = m_pParam->nID;
			m_pParam->lUseTime = pTimerparam->lFuture - GlobalSetup::GetSetup()->dwActTime;

			//ע�ᶨʱ��
			pTimerparam->lTimerID = TIMER_SCHEDULE(pShape,
				pTimvar,pTimerparam->nStartTime+pTimerparam->lFuture,
				m_lInter);

#ifdef _SKILL_INFO_D_
            DWORD dwCurTime = timeGetTime();
            Log4c::Trace(ROOT_MODULE,"COpRegisterTimer:����ID%d,��ֵ%d,ʱ��%d,ע��ʱ��\n",m_pParam->nID, m_pParam->dwGlobleID,m_pParam->lUseTime);
#endif
		}
	}
	else
	{
		//�����������
		stTimerParam *pTimerparam = OBJ_CREATE(stTimerParam);	
		//��ʼ������
		pTimerparam->lDestType = pShape->GetType();
		pTimerparam->DestID = pShape->GetExID();

		//  [5/4/2009 chenxianj]	
		if(m_strFutVarname != "")
			m_lFuture = (long)m_pParam->pVariableList->GetVarValue<double>(m_strFutVarname.c_str(),0);
		if(m_strInterVarName != "")
			m_lInter = (long)m_pParam->pVariableList->GetVarValue<double>(m_strInterVarName.c_str(),0);
		if(m_strCountVarName != "")
			m_nCount = (long)m_pParam->pVariableList->GetVarValue<double>(m_strCountVarName.c_str(),0);
		//  [5/4/2009 chenxianj]

        // by:expter ��ȡ����
        if (m_strFutureParamname!="")
        {
            m_lFuture = pShape->GetSkillValue(m_pParam->nID,m_pParam->nLvl,m_strFutureParamname,m_pParam->nMouseX,m_pParam->nMouseY);
        }
        if (m_strInterParamname!="")
        {
            m_lInter = pShape->GetSkillValue(m_pParam->nID,m_pParam->nLvl,m_strInterParamname,m_pParam->nMouseX,m_pParam->nMouseY);
        }
        if (m_strCountParamname!="")
        {
            m_nCount = pShape->GetSkillValue(m_pParam->nID,m_pParam->nLvl,m_strCountParamname,m_pParam->nMouseX,m_pParam->nMouseY);
        }

		m_pParam->lUseTime = m_lFuture + m_nCount*m_lInter ;//  - GlobalSetup::GetSetup()->dwActTime

		pTimerparam->lFuture = m_lFuture;
		pTimerparam->lInter = m_lInter;
		pTimerparam->nCount = m_nCount;

		pTimerparam->lFlag = m_lFlag;
		pTimerparam->nCalledCount=0;
		pTimerparam->nStartTime = timeGetTime();

		tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
		pTimvar->TimerType = (eChildObjType)m_eChildObjType;
		pTimvar->lvar = m_pParam->dwGlobleID;
		pTimvar->lvar1 = m_pParam->nID;
		//ע�ᶨʱ��
		pTimerparam->lTimerID = TIMER_SCHEDULE(pShape,
			pTimvar,pTimerparam->nStartTime+m_lFuture,
			m_lInter);

#ifdef _SKILL_INFO_D_
        DWORD dwCurTime = timeGetTime();
        Log4c::Trace(ROOT_MODULE, "COpRegisterTimer:����ID%d,��ֵ%d,ʱ��%d",m_pParam->nID, m_pParam->dwGlobleID,m_pParam->lUseTime);
#endif
		//���涨ʱ��
		m_pParam->Timers.push_back(pTimerparam);
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
COpRegisterEvent::COpRegisterEvent(stModuParam *pParam)
:COperator(pParam)
{
}
COpRegisterEvent::COpRegisterEvent(void* pParam)
:COperator((stModuParam*)pParam)
{

}
COpRegisterEvent::~COpRegisterEvent(void)
{
}

//ע���¼�
void COpRegisterEvent::Execute(CMoveShape* pShape)
{	
	stEventParam *pParam = OBJ_CREATE(stEventParam);
	pParam->lEventType = m_lEventType;
	pParam->lFlag = m_lFlag;

	long lEventID = pShape->GetEventIDCount();
	tagEvenRegVar RegVar = tagEvenRegVar(lEventID,pShape->GetType(),pShape->GetExID(),
		m_eChildObjType,m_pParam->dwGlobleID);

	pParam->RegVar = RegVar;
	pShape->RegisterEvent(eEventType(m_lEventType),RegVar);	
	//����ע����¼�
	m_pParam->Events.push_back(pParam);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

COpUnRegisterTimer::COpUnRegisterTimer(stModuParam *pParam,
									   long lFlag)
									   :COperator(pParam)
									   ,m_lFlag(lFlag)
{
}
COpUnRegisterTimer::COpUnRegisterTimer(void *pParam)
:COperator(((tagParam*)pParam)->pParam),m_lFlag(((tagParam*)pParam)->lFlag)
{

};
COpUnRegisterTimer::~COpUnRegisterTimer(void)
{
}

void COpUnRegisterTimer::Execute(CMoveShape* pShape)
{
	//ȡ����ͬ��ǵĶ�ʱ��
	stModuParam::itTimer it = m_pParam->Timers.begin();
	for(;it != m_pParam->Timers.end();)
	{
		stTimerParam* pParam = (*it);
		if(pParam && (pParam->lFlag==m_lFlag || m_lFlag==0))
		{
			if(pParam->lTimerID > 0)
				int nNum = TIMER_CANCEL(pParam->lTimerID);
			pParam->lTimerID = -1;

			it=m_pParam->Timers.erase(it);
			OBJ_RELEASE(stTimerParam, pParam);
		}
		else
		{
			it++;
		}
	}
	m_pParam->Timers.clear();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
COpUnRegisterEvent::COpUnRegisterEvent(stModuParam *pParam,
									   long lFlag)
									   :COperator(pParam)
									   ,m_lFlag(lFlag)
{
}
COpUnRegisterEvent::COpUnRegisterEvent(void* pParam)
:COperator(((tagParam*)pParam)->pParam)
,m_lFlag(((tagParam*)pParam)->lFlag)
{

}
COpUnRegisterEvent::~COpUnRegisterEvent(void)
{
}

//ע���¼�
void COpUnRegisterEvent::Execute(CMoveShape* pShape)
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pParam->pUser->GetFather());
	if (pRegion)
	{
		stModuParam::itEvent it = m_pParam->Events.begin();
		for(;it!=m_pParam->Events.end();)
		{
			stEventParam *pParam = (*it);
			if(pParam && (pParam->lFlag == m_lFlag || m_lFlag==0) )
			{
				CBaseObject *pObj = pRegion->FindChildObject((OBJECT_TYPE)(pParam->RegVar.lType1),pParam->RegVar.ID1);
				
				if(!pObj && pParam->RegVar.lType1 == TYPE_PLAYER)
					CBaseObject *pObj = GetGame()->FindPlayer(pParam->RegVar.ID1);
				if(pObj)
					pObj->UnRegisterEvent(eEventType(pParam->lEventType),pParam->RegVar.lEventID);
			
				it = m_pParam->Events.erase(it);
				OBJ_RELEASE(stEventParam, pParam);
			}
			else
			{
				it++;
			}
		}
	}
}