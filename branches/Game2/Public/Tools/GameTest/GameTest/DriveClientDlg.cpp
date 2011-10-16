/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CDriveClientDlg.cpp
*  ���ܣ���Ҫ���ڹ������DriveClient Dlg
*  ��ǰ�汾��1.1.0
*  ���ߣ� MartySa
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�  2009.2.2
*
============================================================
*/

#include "stdafx.h"
#include "GameTest.h"
#include "DriveClientDlg.h"
#include <process.h>
#include "GameTestDlg.h"
//#include "..\App\AutoDriveClient\AutoDriveClient.h"
//#include "..\Script\MapScriptSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDriveClientDlg, CDialog)

CDriveClientDlg::CDriveClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDriveClientDlg::IDD, pParent),
	 m_DriveClientManager(NULL)
	 , m_lStartCDkey(0)
	 , m_lEndCDkey(0)
{
	m_DriveClientManager = NULL;
	m_pTimer = NULL;
}

CDriveClientDlg::~CDriveClientDlg()
{
	//CloseHandle(m_stOutputInfo.hEvent);
	//CloseHandle(m_stOutputInfo.hThrd);
	//m_stOutputInfo.bExit = TRUE;
}

void CDriveClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	m_lStartCDkey = 1111;
	m_lEndCDkey   = 1111;
	DDX_Text(pDX, IDC_EDIT1, m_lStartCDkey);
	DDX_Text(pDX, IDC_EDIT2, m_lEndCDkey);
}


BEGIN_MESSAGE_MAP(CDriveClientDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDriveClientDlg::OnBnClickedButtonStart)
	ON_EN_CHANGE(IDC_EDIT1, &CDriveClientDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

//
////���̵߳���
//UINT WINAPI OutputThread(void* lParam)
//{
//	CGameTestDlg* pTestDlg = (CGameTestDlg*)lParam;
//	if( pTestDlg != NULL )
//	{	
//		CDriveClientDlg *pDriveDlg = pTestDlg->GetDriveDlg();
//		//SetEvent(pDriveDlg->GetstOutPutInfo()->hEvent);
//		for(;;)
//		{
//			//extern m_cs;
//			//WaitForSingleObject(pDriveDlg->GetstOutPutInfo()->hEvent, INFINITE);
//			// MSG_FILE Loop 
//			if(pDriveDlg->GetThreadExit())
//				break;
//			EnterCriticalSection( pTestDlg->GetTestCS());
//			// do some thing
//			//��ȡ��Ϣ����
//			
//
//			//������Ϣ����
//			
//			CDriveClient *DriveClientManager = pTestDlg->GetDriveClientManager();
//			CDriveGameClient *gameclient = (CDriveGameClient*)DriveClientManager->_FindGameClient("GM1111");
//			msgQueue *MsgQueue = gameclient->GetMsgQueueMSG_FILE();
//			if ( DriveClientManager && gameclient && MsgQueue)
//			{
//				if ( MsgQueue->size()>0 )
//				{	
//					msgQueue::iterator it = MsgQueue->begin();
//					for ( ; it!=MsgQueue->end(); ++it)
//					{
//						CMessage *pMsg = (CMessage*)(*it);		
//						if ( pMsg )
//						{	
//							long lType = pMsg->GetType();
//							lType = lType - (lType&0x00000ff);	
//
//							switch( lType )
//							{	
//								//for�ڴ�ӳ���ļ� �ļ�����
//								case MSG_FILE:
//									{	
//										long lType = pMsg->GetType();
//									
//										switch(lType)
//										{				
//											case MSG_FILE_WRITE:
//											{
//												char strFileName[256] = " ";
//												pMsg->GetStr(strFileName, 256);
//
//												char strFiletxt[256] = " ";
//												pMsg->GetStr(strFiletxt, 256);
//												
//												if ( ::strlen(strFileName)>0 && ::strlen(strFiletxt)>0 )
//												{
//													pTestDlg->GetMapFileManager()->WriteFile(strFileName, strFiletxt);
//												}
//												
//											}
//											break;
//
//											case MSG_FILE_CLEAR:
//											{
//												char strFileName[256] = " ";
//												pMsg->GetStr(strFileName, 256);
//												
//												if ( ::strlen(strFileName)>0 )
//												{											
//													pTestDlg->GetMapFileManager()->ClearFile(strFileName);
//												}
//											}
//											break;
//
//											case MSG_FILE_OPEN:
//											{
//												char strFileName[256] = " ";
//												pMsg->GetStr(strFileName, 256);
//												
//												if ( ::strlen(strFileName)>0 )
//												{
//													pTestDlg->GetMapFileManager()->OpenFile(strFileName);
//												}
//											}
//											break;
//
//											case MSG_FILE_CLOSE:
//											{
//												char strFileName[256] = " ";
//												pMsg->GetStr(strFileName, 256);
//
//												if (::strlen(strFileName)>0)
//												{
//													pTestDlg->GetMapFileManager()->CloseFile(strFileName);
//												}
//												
//											}
//											break;
//											
//											default:
//												break;															
//										}
////										CBaseMessage::FreeMsg(pMsg);
//									}
//									break;
//								default:
//									break;
//							}
//						}
//						pTestDlg->FreeMsg(pMsg);
//					}
//					MsgQueue->clear();
//				}
//			}
//
//			LeaveCriticalSection(pTestDlg->GetTestCS());
//			Sleep(100);
//		}
//	}
//	return 0;
//}
// CDriveClientDlg ��Ϣ�������

void CDriveClientDlg::OnBnClickedButtonStart()
{
	if ( m_DriveClientManager != NULL )
	{

		//�����ڴ�ӳ���ļ�
		unsigned uId = m_DriveClientManager->_CreateFileMapping("GameClient", "MemServer", "Run//");		
		UpdateData(true);
		//����DriveGameClient,����������ʺ�
		_CreatDriveGameClient(m_lStartCDkey, m_lEndCDkey, uId);		
		
		//�������̣߳�����MapFile �ļ�����
		//m_stOutputInfo.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
		//CGameTestDlg* pDlg = CMapScriptSystem::getInstance()->GetTestDialog();
		//m_stOutputInfo.hThrd = (HANDLE)_beginthreadex(NULL,0,OutputThread,(void*)pDlg,0,NULL);

		//::ShellExecute(NULL, "open", "Luvinia.exe", NULL, NULL, SW_SHOW);
	}
	else
	{
		::MessageBox(NULL, "���������ڴ�ӳ���ļ�ʧ��,DriveClientManager = NULL����", "ERRO!",MB_OK);
	}
}

void CDriveClientDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	
}

//����DriveGameClient
void  CDriveClientDlg::_CreatDriveGameClient(long startcdkey, long endcdkey, unsigned uID)
{
	if ( startcdkey > endcdkey )
	{	
		::MessageBox(NULL, "�������ʼ�ʺ�<<С��>>�����ʺ�,�����������ʺ�","ERRO!", MB_YESNO);
		return ;
	}
	if ( startcdkey < 1000 )
	{
		::MessageBox(NULL, "�������ʼ�ʺ�<<����>> 1000 ","ERRO!", MB_YESNO);
		return ; 
	}
	if ( startcdkey == endcdkey )
	{
		::MessageBox(NULL, "�������ʼ�ʺŵ��ڽ����ʺ�,Ϊһ���ʺ�","OK!", MB_YESNO);
	}
	CString strPat = "GM";
	CString strCDkey;
	for ( int i = startcdkey; i <= endcdkey; ++i)
	{
		strCDkey.Format("%d", i);
		strCDkey = strPat+strCDkey;

		//��ʼ����DriveClient
		m_DriveClientManager->_CreateGameClient(strCDkey.GetBuffer(), uID, m_pTimer);
		strCDkey.ReleaseBuffer();
	}
	
}