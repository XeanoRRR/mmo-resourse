#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\player.h"
#include "..\goods\cgoods.h"
#include "..\..\..\Setup\GlobeSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//void OnDepotMessage(CMessage* pMsg)
//{
//	CPlayer* pPlayer=pMsg->GetPlayer();
//	if (pPlayer==NULL)
//		return;
//
//	if( pPlayer -> IsInChangingServer() ||
//		pPlayer -> IsInChangingRegion() )
//	{
//		return;
//	}
//
//	if( pPlayer -> GetCurrentProgress() != CPlayer::PROGRESS_BANKING )
//	{
//		return;
//	}
//
//	switch(pMsg->GetType())
//	{		
//	case MSG_C2S_DEPOT_CHECKPASSWORD://��ҽ��вֿ��������֤
//		{
//			char strPassWord[7];
//			pMsg->GetStr(strPassWord,7);
//			if(strcmp(pPlayer->GetDepotPassword(),strPassWord)==0)
//			{
//				CMessage msg(MSG_S2C_DEPOT_CHECKPASSWORD);
//				msg.Add((BYTE)1);
//				msg.SendToPlayer(pPlayer->GetExID());
//				pPlayer -> m_cBank.Unlock( strPassWord );
//				pPlayer -> m_cDepot.Unlock( strPassWord );
//				//pMsg->GetPlayer()->SetDepotPassWordCheckFlag(true);
//				//pMsg->GetPlayer()->SetSavedDepotPassWord(strPassWord);
//			}
//			else
//			{
//				CMessage msg(MSG_S2C_DEPOT_CHECKPASSWORD);
//				msg.Add((BYTE)0);
//				msg.SendToPlayer(pPlayer -> GetExID());
//			}
//		}
//		break;
//	case MSG_C2S_DEPOT_CHANGEPASSWORD://��Ҹı�ֿ�����
//		{
//			char strOldPassWord[7];
//			char strNewPassWord[7];
//			pMsg->GetStr(strOldPassWord,7);
//			pMsg->GetStr(strNewPassWord,7);
//			//�������Ƿ���ȷ
//			if(strcmp(pPlayer->GetDepotPassword(),strOldPassWord)==0)
//			{
//				//##������볤�ȴ���6λ���򷵻ء�
//				if( strlen( strNewPassWord ) > 6 )
//				{
//					CMessage msg(MSG_S2C_DEPOT_CHANGEPASSWORD);
//					msg.Add( static_cast<BYTE>(3) /*##3��ʾ�ַ����������6λ*/ );
//					msg.SendToPlayer(pPlayer -> GetExID());
//				}
//				else 
//				{
//					//##���Ƿ��зǷ��ַ����ж�.
//					BOOL bHasInvalidWord = FALSE;
//					for( size_t i = 0; i < strlen( strNewPassWord ); i ++ )
//					{
//						if( !( (strNewPassWord[i] >= 30 && strNewPassWord[i] <= 39) ||
//							(strNewPassWord[i] >= 65 && strNewPassWord[i] <= 90) ||
//							(strNewPassWord[i] >= 48 && strNewPassWord[i] <= 57) ||
//							(strNewPassWord[i] >= 97 && strNewPassWord[i] <= 122) ) )
//						{
//							bHasInvalidWord = TRUE;
//						}
//					}
//
//					if( bHasInvalidWord )
//					{
//						//##�зǷ��ַ���
//						CMessage msg(MSG_S2C_DEPOT_CHANGEPASSWORD);
//						msg.Add( static_cast<BYTE>(2) /*##2��ʾ�ַ����зǷ��ַ�*/ );
//						msg.SendToPlayer(pPlayer -> GetExID());
//					}
//					else 
//					{
//						//##û�зǷ��ַ���
//						CMessage msg(MSG_S2C_DEPOT_CHANGEPASSWORD);
//						msg.Add( static_cast<BYTE>(0) /*##0��ʾ�޸ĳɹ�*/ );
//						msg.SendToPlayer(pPlayer -> GetExID());
//						//����������
//						pPlayer->SetDepotPassword(strNewPassWord);
//					}
//				}
//			}
//			else
//			{
//				CMessage msg(MSG_S2C_DEPOT_CHANGEPASSWORD);
//				msg.Add( static_cast<BYTE>(1) /*##1��ʾԭ�������*/ );
//				msg.SendToPlayer(pPlayer -> GetExID());
//			}
//		}
//		break;
//	case MSG_C2S_DEPOT_CLOSE://��ҹر��˲ֿ����
//		{
//			pPlayer -> m_cBank.Lock();
//			pPlayer -> m_cDepot.Lock();
//
//			//##���õ�ǰ�Ĳ���Ϊ��
//			pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_NONE );
//		}
//		break;
//	}
//}