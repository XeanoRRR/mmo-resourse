#include "StdAfx.h"
#include "BounsOperThread.h"
#include "BounsAdoBase.h"
#include "BounsOper.h"

CBounsOperThread::CBounsOperThread(string strProvider,
								string strDataSource,
								string strInitialCatalog,
								string strUserID,
								string strPassword
								)
								:m_bThreadRunning(false),
								m_hOperThread(NULL)
{
	m_pAdoBasePtr = new CBounsAdoBase(string("Provider=" + strProvider +
									"; Server=" + strDataSource + 
									"; Database=" + strInitialCatalog + 
									"; UID=" + strUserID + 
									"; PWD=" + strPassword + 
									"; OLE DB Services=-1; Driver={SQL Server}"
									));

	m_strInitialCatalog = strInitialCatalog;
	m_hQuitWait = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_hQuitWait);
}

CBounsOperThread::~CBounsOperThread(void)
{
	End();
	delete m_pAdoBasePtr;
}

_ConnectionPtr CBounsOperThread::GetConnPtr()
{
	return m_pAdoBasePtr->GetConnPtr();
}

bool CBounsOperThread::OpenCn()
{
	return m_pAdoBasePtr->OpenCn();
}

bool CBounsOperThread::Begin()
{
	if( m_hOperThread != NULL )
	{
		return false;
	}

	m_bThreadRunning = true;
	m_hOperThread = (HANDLE)_beginthreadex(NULL, 0, OperFun, (void*)this, 0, NULL);
	if( m_hOperThread == NULL )
	{
		m_bThreadRunning = false;
		return false;
	}

	return true;
}

void CBounsOperThread::End()
{
	if( m_hOperThread == NULL)
		return;

	m_hOperThread = NULL;
}

bool CBounsOperThread::ProcessData()
{
	bool bRet = true;

	list<CBounsOper*> listOpers;
	GetGame()->GetBounsDBManager()->GetOpers(listOpers);
	list<CBounsOper*>::iterator itor = listOpers.begin();
	for(; itor != listOpers.end(); ++itor)
	{
		if((*itor))
		{
			if((*itor)->GetOperType() != OT_BOUNS_EXIT_THREAD )
			{
DO_AGAIN:
				(*itor)->SetConnectionPtr(m_pAdoBasePtr->GetConnPtr());
				if((*itor)->DoOperator() == FALSE)
                {
                    // �����쳣 �޸�����
TRY_CONNECT:
                    m_pAdoBasePtr->CloseCn();
                    if(!m_pAdoBasePtr->OpenCn())                    
                    {
                        AddLogText("�������ݿ��Ƿ���������!!!!");
                        goto TRY_CONNECT;
                    }
                    else
                    {
                        AddLogText("���ݿ����ӻָ�����!!!");
                        goto DO_AGAIN;
                    }
                    
                }
			}
			else
			{
				CBounsOper *pOper = new CBounsOper(OT_BOUNS_EXIT_THREAD);
				GetGame()->GetBounsDBManager()->AddOper(pOper);
				bRet = false;
			}
		}
        SAFE_DELETE((*itor));
	}
	listOpers.clear();

	return bRet;
}

unsigned int __stdcall CBounsOperThread::OperFun(void *pArgument)
{
#ifndef _DEBUG
	__try
	{
#endif
		HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
		CBounsOperThread *pOperThread = (CBounsOperThread*)pArgument;
		if (NULL != pOperThread)
		{
			pOperThread->GetAdoBasePtr()->OpenCn();
			while(pOperThread->ProcessData())
			{
				//...
			}
			pOperThread->GetAdoBasePtr()->CloseCn();
			
			char outStr[1024];
			sprintf(outStr, "ID:[%d] �̳߳ɹ��˳�!\r\n", pOperThread->GetThreadID());
			SetEvent(pOperThread->GetQuitWaitHandle());
		}
		::CoUninitialize();

#ifndef _DEBUG
	}
	__except(_Sword3::CrashFilter(GetExceptionInformation(),GetExceptionCode()))
	{
		//	MessageBox(g_hWnd, "������ִ�����鿴���µĴ��󱨸��ļ�", "Error", MB_OK);

		PutDebugString("������ִ�����鿴���µĴ��󱨸��ļ�");		

		if(GetGame())
			GetGame()->Release();
		DeleteGame();

		Sleep(5000);

		WinExec("BounsServer.exe", SW_SHOW);

		SetEvent(g_hGameThreadExitEvent);
		_endthreadex( 0 );

		return 0;
	}
#endif

	_endthreadex(0);
	return 0;
}
