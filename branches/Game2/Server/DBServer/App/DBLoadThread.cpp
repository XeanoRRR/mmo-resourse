#include "stdafx.h"
#include "DBLoadThread.h"
#include "DBAdoBase.h"
#include <process.h>
#include "DBentityManager.h"
#include "../dbaccess/WorldDB/RsPlayer.h"
#include "../dbaccess/WorldDB/RsDupRgn.h"
#include "../dbaccess/WorldDB/RsRegion.h"
#include "../dbaccess/WorldDB/rscountry.h"
#include "../dbaccess/WorldDB/RsMail.h"
#include "../dbaccess/WorldDB/RsGenVar.h"
#include "ThreadDataDef.h"
#include "DBLoginPlayer.h"
#include "DBDupRgn.h"
#include "DBRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern long lCurLoadThread1QueueSize;
extern long lCurLoadThread2QueueSize;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DBLoadThread::DBLoadThread(string strProvider, 
						 string strDataSource, 
						 string strInitialCatalog,
						 string strUserID, 
						 string strPassword
						 )
:
m_LoadRunning(false),
m_hLoadWaitThread(0)
{
	m_pConnPtr = new DBAdoBase(string(	"Provider=" + strProvider +
									"; Server=" + strDataSource + 
									"; Database=" + strInitialCatalog + 
									"; UID=" + strUserID + 
									"; PWD=" + strPassword + 
									"; OLE DB Services=-1; Driver={SQL Server}"
									));
	m_strInitialCatalog = strInitialCatalog;
	m_hQuitWait =	CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_hQuitWait);
}

//-----------------------------------------------------------------------------------------------------------------
DBLoadThread::~DBLoadThread(void)
{
	Exit();
	delete m_pConnPtr;
}

_ConnectionPtr DBLoadThread::GetConnPtr(void)
{
	return m_pConnPtr->GetConnPtr();
}
bool DBLoadThread::OpenCn(void)
{
	return m_pConnPtr->OpenCn();
}
//-----------------------------------------------------------------------------------------------------------------
bool DBLoadThread::Begin(void)
{
	if (0 != m_hLoadWaitThread)
		return false;

	m_LoadRunning	= true;

	if(0 == (m_hLoadWaitThread = (HANDLE)_beginthreadex(NULL, 0, RunThread, (void*)this, 0, &m_lThreadID)))
	{	
		m_LoadRunning	= false;
		return false;
	}
	return true;
}


//-----------------------------------------------------------------------------------------------------------------
void DBLoadThread::Exit(void)
{
	if (0 == m_hLoadWaitThread)
		return;

	m_hLoadWaitThread = 0;
}

void DBLoadThread::SetLoadWaitThread(void)
{
	SetEvent(m_hLoadWaitThread);
}
//-----------------------------------------------------------------------------------------------------------------
bool DBLoadThread::ProcessData(void)
{
	bool ret = true;
	char outStr[1024];
	LoadThreadData* tempLoadData = NULL;
	// ��������Ԫ�ص���ʱ������������
	sprintf(outStr, "ID[%d]:��ȡ�߳�׼������Load����...\r\n", m_lThreadID);
	OutputDebugStr(outStr);
	GetGame()->GetDBEntityManager()->PopLoadThreadData(&tempLoadData,m_lThreadID);
	sprintf(outStr, "ID[%d]:��ȡ�߳����ڴ���Load����...\r\n", m_lThreadID);
	OutputDebugStr(outStr);

	if( tempLoadData && tempLoadData->dbObj && tempLoadData->loadObj )
	{
		// ִ�ж�ȡ����
		switch(tempLoadData->loadObj->objType)
		{
		case DB_GENVARGROUP:
			{
				// ��ȡ����
				LoadThreadData* loadData = new LoadThreadData(tempLoadData->loadObj->objGUID, DB_GENVARGROUP);
				*(loadData->loadObj) = *(tempLoadData->loadObj);
				if(DB_OPER_RESULT_ERROR == GetGame()->GetRsGenVar()->Load((CDBGenVarGroup*)(loadData->dbObj), GetConnPtr()))
				{
					char szPlayerID[128];
					tempLoadData->loadObj->objGUID.tostring(szPlayerID);
					AddLogText("Load DB_GENVARGROUP Failed.");
					(loadData->loadObj)->objNum = -1;
				}
				GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(loadData);
			}
			break;
		case DB_LOGINPLAYER:
			{
				// �������ݱ�����CDBLoginPlayer����
				std::list<LoadThreadData*> LoginPlayerList;
				bool ret = GetGame()->GetRsPlayer()->OpenPlayerBase(tempLoadData->loadObj->szCDKEY, LoginPlayerList, GetConnPtr());	

				if(ret == false) // ��ȡʧ��
				{
					CGUID tempGUID;
					CGUID::CreateGUID(tempGUID);
					LoadThreadData* tempLoginPlayer = new LoadThreadData(tempGUID, DB_LOGINPLAYER);	
					*(tempLoginPlayer->loadObj) = *(tempLoadData->loadObj);
					tempLoginPlayer->loadObj->objNum = -1;
					GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(tempLoginPlayer);
					break;
				}

				std::list<LoadThreadData*>::iterator lItr = LoginPlayerList.begin();
				if(LoginPlayerList.size() == 0) // û�н�ɫ
				{
					CGUID tempGUID;
					CGUID::CreateGUID(tempGUID);
					LoadThreadData* tempLoginPlayer = new LoadThreadData(tempGUID, DB_LOGINPLAYER);	
					*(tempLoginPlayer->loadObj) = *(tempLoadData->loadObj);
					tempLoginPlayer->loadObj->objNum = 0;
					GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(tempLoginPlayer);
				}
				else
				{
					for(; lItr != LoginPlayerList.end(); lItr++)
					{
						(*lItr)->loadObj->objNum = LoginPlayerList.size();
						(*lItr)->loadObj->objType = tempLoadData->loadObj->objType;
						strcpy((*lItr)->loadObj->szCDKEY, tempLoadData->loadObj->szCDKEY);
						(*lItr)->loadObj->objType = tempLoadData->loadObj->objType;
					}
					GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(LoginPlayerList);
					LoginPlayerList.clear();
				}
			}
			break;
		case TYPE_PLAYER:
			{
				// ��ȡ����
				LoadThreadData* loadData = new LoadThreadData(tempLoadData->loadObj->objGUID, DB_PLAYER);
				*(loadData->loadObj) = *(tempLoadData->loadObj);
				if(0 == GetGame()->GetRsPlayer()->LoadPlayer((CDBPlayer*)loadData->dbObj, GetConnPtr()))
				{
					char szPlayerID[128];
					(CDBPlayer*)loadData->dbObj->GetExID().tostring(szPlayerID);
					AddLogText("Load PLAYER Failed.ID[%s].", szPlayerID);
					(loadData->loadObj)->objNum = -1;
				}
				
				GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(loadData);
			}
			break;
		case TYPE_REGION:
			{
				LoadThreadData* loadData=new LoadThreadData(tempLoadData->loadObj->objGUID,DB_REGION);
				GetGame()->GetRsRegion()->LoadRegionParam((CDBRegion*)tempLoadData->dbObj);
				*(loadData->loadObj) = *(tempLoadData->loadObj);
				CDBRegion* pRegionObj=dynamic_cast<CDBRegion*>(loadData->dbObj);
				CDBRegion* pTempRegionObj=dynamic_cast<CDBRegion*>(tempLoadData->dbObj);
				*pRegionObj=*pTempRegionObj;
				GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(loadData);					
			}
			break;
		case DB_PHDUPRGN:
			{
				// ��ȡ����
				LoadThreadData* loadData = new LoadThreadData(tempLoadData->loadObj->objGUID, DB_PHDUPRGN);
				GetGame()->GetRsDupRgn()->LoadDupRgn((CDBDupRgn*)loadData->dbObj, GetConnPtr());
				*(loadData->loadObj) = *(tempLoadData->loadObj);
				GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(loadData);
			}
			break;
		case DB_PHRGNGOODSGROUP:
			{
				AddLogText("��ȡ���˸�����Ʒ��Ӧ�ý�����̶߳�ȡ��");
			}
			break;
		case DB_COUNTRY:
			{
				std::list<LoadThreadData*> countryList;
				// ��ȡ����
				GetGame()->GetRSCountry()->Load(countryList, GetConnPtr());
			
				std::list<LoadThreadData*>::iterator lItr = countryList.begin();
				for(; lItr != countryList.end(); lItr++)
				{
					(*lItr)->loadObj->objType = tempLoadData->loadObj->objType;
					(*lItr)->loadObj->objType = tempLoadData->loadObj->objType;
					(*lItr)->loadObj->objNum = countryList.size();
					GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(*lItr);
				}
				countryList.clear();
			}
			break;
		case DB_MAILGROUP:
			{
				// ��ȡ����
				LoadThreadData* loadData = new LoadThreadData(tempLoadData->loadObj->objGUID, DB_MAILGROUP);
				*(loadData->loadObj) = *(tempLoadData->loadObj);
				if(tempLoadData->loadObj->opType == DB_OPER_QUERY)
				{
					if(false == GetGame()->GetRsMail()->QueryPlayerMails(tempLoadData->loadObj->lQueryTime, (CDBMailGroup*)(loadData->dbObj), GetConnPtr()))
					{
						AddLogText("Query DB_MAILGROUP Failed.");
						(loadData->loadObj)->objNum = -1;
					}
				}
				else
				{
					if(false == GetGame()->GetRsMail()->LoadPlayerMails(tempLoadData->loadObj->objGUID, (CDBMailGroup*)(loadData->dbObj), GetConnPtr()))
					{
						char szPlayerID[128];
						tempLoadData->loadObj->objGUID.tostring(szPlayerID);
						AddLogText("Load DB_MAILGROUP Failed.ownerID[%s].", szPlayerID);
						(loadData->loadObj)->objNum = -1;
					}
				}
				GetGame()->GetDBEntityManager()->PushDisposedLoadThreadData(loadData);
			}
			break;
		}
	} 
	else if(tempLoadData
		&& ( NULL == tempLoadData->dbObj && NULL == tempLoadData->loadObj ) )// ����һ���ն����ʾ�߳��˳�
	{
		ret = false;
	}
	// �����Ϣ����
	SAFE_DELETE(tempLoadData);
	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DBAdoBase* DBLoadThread::GetAdoBasePtr(void)
{
	return m_pConnPtr;
}
unsigned __stdcall DBLoadThread::RunThread(void *pThis)
{
#ifndef _DEBUG
	__try
	{
#endif
		HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
		DBLoadThread *pLoadThread = (DBLoadThread*)pThis;
		if (NULL != pLoadThread)
		{
			pLoadThread->GetAdoBasePtr()->OpenCn();

			while(pLoadThread->ProcessData())
			{

			}

			pLoadThread->GetAdoBasePtr()->CloseCn();

			char outStr[1024];
			sprintf(outStr, "ID:[%d] �̳߳ɹ��˳�!\r\n", pLoadThread->GetThreadID());
			OutputDebugStr(outStr);
			SetEvent(pLoadThread->GetQuitWaitHandle());
		}
		::CoUninitialize();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		//	MessageBox(g_hWnd, "������ִ�����鿴���µĴ��󱨸��ļ�", "Error", MB_OK);

		PutDebugString("������ִ�����鿴���µĴ��󱨸��ļ�");
		AddErrorLogText("������ִ�����鿴���µĴ��󱨸��ļ�");

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

		Sleep(5000);

		WinExec("DBServer.exe", SW_SHOW);

//		PostMessage(g_hWnd, WM_DESTROY, 0, 0);
		SetEvent(g_hGameThreadExitEvent);
		_endthreadex( 0 );

		return 0;
	}
#endif

	_endthreadex(0);		
	return 0;
}