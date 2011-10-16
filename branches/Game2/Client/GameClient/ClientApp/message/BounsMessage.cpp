/*
* BounsMessage.cpp
* ����:������Ϸ������ϵͳ�����Ϣ
* ժҪ:
* ����:���
* ��������:2009.10.23
*/

#include "stdafx.h"
#include "Message.h"
#include "../GameClient/Game.h"
#include "Other/AudioList.h"
#include "../../../Public/Common/BounsDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnBounsMessage(CMessage* pMsg)
{
	//CPresentPageEx* pPage = GetGame()->GetCGuiEx()->GetPresentPageEx();
	//if(pPage == NULL)
	//{
	//	return;
	//}

	switch(pMsg->GetType())
	{
	// ���ؽ�����Ϣ
	case MSG_S2C_BOUNS_INFO_RESPOND:
		{
			//pPage->GetAwardVector().clear();
			//pPage->GetDesVector().clear();

			// �����ĸ���
			long size = pMsg->GetLong();

			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			for(int i = 0; i < size; ++i)
			{
				tagBounsInfo stBouns;
				tagBounsDes stDes;
				stBouns.DecodeFromDataBlock(setReadDB);
				stDes.DecodeFromDataBlock(setReadDB);
				//pPage->GetAwardVector().push_back(stBouns);
				//map<DWORD, tagBounsDes>::iterator it = pPage->GetDesVector().find(stBouns.dwBounsID);
				//if(it == pPage->GetDesVector().end())
				//{
					//pPage->GetDesVector()[stBouns.dwBounsID] = stDes;
				//}
			}
		}
		break;

	// �콱�������
	case MSG_S2C_BOUNS_GET_RESULT:
		{
			// ������GUID
			CGUID guid;
			pMsg->GetGUID(guid);

			// ���ؽ�����ͣ�0:ʧ�ܣ�1:�ɹ���2:���ᣬ3:����ȡ��4:����
			long lRet = pMsg->GetLong();

			//CMessageBoxPageEx* pMsgBox = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
			switch(lRet)
			{
			case 0:
				{
					//if(pMsgBox != NULL)
					//{
						GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");

						//pMsgBox->Open(pPage, CStringReading::LoadText(eIDS_PRESENT, ePRESENT_UNSUCCESS), CMessageBoxPageEx::MSG_OK, true);
					//}
				}
				break;

			case 1:
				{
					//vector<tagBounsInfo>::iterator it = pPage->GetAwardVector().begin();
					//for(; it != pPage->GetAwardVector().end(); ++it)
					//{
					//	if(it->guid == guid)
					//	{
					//		pPage->GetAwardVector().erase(it);
					//		pPage->UpdateAward(pPage->GetCurPage());

					//		break;
					//	}
					//}

					//// �콱�ɹ���ʾ
					//CMainBarPageEx* pMainBar = GetGame()->GetCGuiEx()->GetMainBarPageEx();
					//if(pMainBar != NULL)
					//{
					//	pMainBar->AddText(eNOTIFYPOS_CENTER, CStringReading::LoadText(eIDS_PRESENT, ePRESENT_SUCCESS), 0xffff0000);
					//}
				}
				break;

			case 2:
				{
					//if(pMsgBox != NULL)
					//{
					//	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");

					//	pMsgBox->Open(pPage, CStringReading::LoadText(eIDS_PRESENT, ePRESENT_FROZE), CMessageBoxPageEx::MSG_OK, true);
					//}
				}
				break;

			case 3:
				{
					//if(pMsgBox != NULL)
					//{
					//	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");

					//	pMsgBox->Open(pPage, CStringReading::LoadText(eIDS_PRESENT, ePRESENT_DREW), CMessageBoxPageEx::MSG_OK, true);
					//}
				}
				break;

			case 4:
				{
					//if(pMsgBox != NULL)
					//{
					//	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");

					//	pMsgBox->Open(pPage, CStringReading::LoadText(eIDS_PRESENT, ePRESENT_OVERDUE), CMessageBoxPageEx::MSG_OK, true);
					//}
				}
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}
}