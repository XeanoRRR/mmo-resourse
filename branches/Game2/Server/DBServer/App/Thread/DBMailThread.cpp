/**
*File:  DBMailThread.cpp
*brief: ������:�ö���ά��һ�����߳�ʹ�õ����ݿ�Cn����
*Author:������
*Date:  2009.02.17
*/
#include "stdafx.h"
#include "DBMailThread.h"

#include <process.h>
#include "DBAdoBase.h"

#include "../Entity/EntityManager.h"

#include "../../../../Public/AppFrame/TextRes.h"
using namespace AppFrame;
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

extern DWORD g_dwThisSaveStartTime;			// ���δ��̿�ʼʱ��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DBMailThread::DBMailThread(string strProvider, 
						 string strDataSource, 
						 string strInitialCatalog,
						 string strUserID, 
						 string strPassword
						 )
{
	m_pConnPtr = new DBAdoBase(string(	"Provider=" + strProvider +
		"; Server=" + strDataSource + 
		"; Database=" + strInitialCatalog + 
		"; UID=" + strUserID + 
		"; PWD=" + strPassword + 
		"; OLE DB Services=-1; Driver={SQL Server}"
		));
	m_strInitialCatalog = strInitialCatalog;
	m_hQuitWait = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_hQuitWait);
	GetGame()->GetEntityManager()->AddMailThreadQuitEventHandle(m_hQuitWait);
}

//-----------------------------------------------------------------------------------------------------------------
DBMailThread::~DBMailThread(void)
{
}

//-----------------------------------------------------------------------------------------------------------------
bool DBMailThread::ProcessData(void)
{
	bool ret = true;
	// ��������Ԫ�ص���ʱ������������
	list<ThreadData*> tempDataList;
	GetGame()->GetEntityManager()->GetMailThreadData(tempDataList, GetThreadID());

	long threadTime = timeGetTime();

	CEntityGroup* pBaseEntity = NULL;
	list<ThreadData*>::iterator thitr = tempDataList.begin();
	for(; thitr != tempDataList.end(); thitr++)
	{
		if((*thitr)->retFlag == -99999999) // ����һ��ֵ��ʾ�߳��˳�
		{
			SAFE_DELETE(*thitr);
			return false;
		}
		pBaseEntity = (CEntityGroup*)(*thitr)->pEntity;

		if(pBaseEntity)
		{
#ifdef _DEBUG_SM_INFO_
			char szOutPutStr[1024];
			char szGuid[128];
			pBaseEntity->GetGUID().tostring(szGuid);
			sprintf(szOutPutStr, "--mail ThreadID:%d TravelRootDbOper() Start:dbObj->GetExID[%s]--",GetThreadID(), szGuid);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),szOutPutStr);
#endif
			HRESULT hRet = TravelRootDbOper(pBaseEntity, GetAdoBasePtr()->GetConnPtr());

#ifdef _DEBUG_SM_INFO_
			sprintf(szOutPutStr, "--mail ThreadID:%d TravelRootDbOper() End:dbObj->GetExID[%s]--",GetThreadID(), szGuid);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),szOutPutStr);
#endif

			CMessage msg(MSG_DB2W_OPER_ENTITY);
			msg.Add(pBaseEntity->GetSessionID());
			msg.Add((long)hRet);
			msg.Add(pBaseEntity->GetCompositeFlag().c_str());
			msg.Add(pBaseEntity->GetGUID());
			msg.Add(pBaseEntity->GetName());
			msg.Add((BYTE)pBaseEntity->GetFindByNameFlag());

			// ������Ϣ
			vector<BYTE> pBA;
			if(pBaseEntity->GetCurDbOperType() == DB_OPER_LOAD)
				pBaseEntity->AddToByteArray(pBA, true);
			else
				pBaseEntity->AddToByteArray(pBA, false);

			msg.Add((long)pBA.size());
			if(pBA.size())
				msg.AddEx(&pBA[0], static_cast<long>(pBA.size()));
			msg.SendToSocket(GetGame()->GetWSSocket());
		}
		else
		{
			Log4c::Warn(ROOT_MODULE,"ProcMailData entity is null!");
			SAFE_DELETE(*thitr);
			return true;
		}
		SAFE_DELETE(*thitr);
	} 
	tempDataList.clear();

	ulong lviewTime = timeGetTime() - threadTime;
	if(GetGame()->GetSetup()->dwViewDbOperDeltaTime <= lviewTime)
		Log4c::Trace(ROOT_MODULE,"%s",FormatText("DB_DBTHREAD_0", GetThreadID(), lviewTime));//======mail ThreadID:%d ���ݿ������ʱ: %d======
	g_dwThisSaveStartTime = lviewTime;

	return ret;
}