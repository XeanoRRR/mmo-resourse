#include "stdafx.h"
#include "Region.h"
#include "Player.h"
#include "../Goods/Goods.h"
#include "Message.h"
#include "../GameClient/game.h"
#include "ClientRegion.h"
#include "SequenceString.h"
#include "Other/AudioList.h"
#include "Other/GlobeSetup.h"
#include "Depot.h"
#include "../../ui/UIManager.h"
#include "../../ui/GamePage/Login/LoginPage.h"
#include "../../ui/GamePage/SelectWorldServer/WorldServerPage.h"
#include "../../ui/GamePage/SelectRole/SelectRolePage.h"
#include "../../ui/GamePage/CreateRole/CreateRolePage.h"
#include "../MsgBoxSys/MsgEventManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern string g_strLastWisper;

bool g_bHandleSocketClose = true;
char strCostMsg[256];

void OnCostMessage(CMessage *pMsg)
{
	if(pMsg->GetType()==MSG_S2C_COST_NOTENOUGHCOST)
	{
		pMsg->GetStr(strCostMsg,sizeof(strCostMsg));
		g_bHandleSocketClose=false;
		if(g_bDebug)
			MessageBox(g_hWnd,strCostMsg,"cost",0);
	}
}



/*
 * ����: ��ӦLOG��Ϣ
 * ժҪ: -
 * ����: pMsg - ���������͵���Ϣ
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.03.20 - lpf
 *		��MSG_L2C_LOG_ANSWER_PLAYERBASELIST��Ϣ��������"��Һ�ƽ״̬"��"ͬ���������ģ��"���趨;
 *	2008.04.01 - lpf
 *		��MSG_L2C_LOG_ANSWER_PLAYERBASELIST��Ϣ�������˹���Hair��Face���Ե��趨;
 *		��MSG_L2C_LOG_ANSWERCREATE��Ϣ�������˹���Hair��Face���Ե��趨;
 *	2008.10.16 - lpf
 *		��MSG_L2C_LOG_ANSWER_PLAYERBASELIST��Ϣ�������˲���ʾAlphaFadeЧ��;
 *	2008.10.18 - lpf
 *		��MSG_S2C_LOG_ANSWER_PLAYERDATA��Ϣ�������˶Խ��볡���������AlphaFade���趨;
 *	2009.03.01 - lpf
 *		��MSG_L2C_LOG_ANSWER_PLAYERBASELIST��Ϣ������������װ���ڵ�½ҳ�����ʾ�趨;
 *	2009.03.13 - lpf
 *		��MSG_L2C_LOG_ANSWER_PLAYERBASELIST��Ϣ�����������Ĭ����ʾ�����趨;
 *		��MSG_S2C_LOG_ANSWER_PLAYERDATA��Ϣ�������˳���SetChangeEquip()��ʾ�趨;
 *	2009.11.13 - lpf
 *		��MSG_S2C_LOG_ANSWER_PLAYERDATA��Ϣ�������˶Ե�ǰ�����Ƿ�ΪPK������־�Ķ�ȡ,���Գ����������˶�Ӧ���趨;
 */
void OnLogMessage(CMessage* pMsg)
{
	
	char * pszStr = NULL;
	
	g_LoginTime = 0;
	switch(pMsg->GetType())
	{
	case MSG_L2C_LOG_ANSWERCDKEY:	// �����������������
		{
			GetGame()->SetQueuePos(-1);

			// ����
			GetGame()->ReNew();
			
			//long Result = pMsg->GetChar(); 
			long Result = pMsg->GetLong();
			switch( Result )
			{
			case LOGIN_LOGINSERVEROK:
				{
					string strCDKey= GetInst(LoginPage).GetCDkey();
                    GetInst(LoginPage).Close();
					GetInst(WorldServerPage).ClearWorldInfo();
					int wsNum=pMsg->GetShort();					
					char wsName[20];
					for(int i=0;i<wsNum;i++)
					{
						long lStateLvl = pMsg->GetLong();
						pMsg->GetStr(wsName,20);
                        GetInst(WorldServerPage).AddWorldInfo(i,wsName,lStateLvl);
					}
                    GetInst(WorldServerPage).UpdateWSList();
					GetInst(WorldServerPage).SetCDKey(strCDKey.c_str());
					GetGame()->SetGameState(GS_SELECTWS);
					return;
				}
				break;

			case LOGIN_VERSIONERROR:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav"); 
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_3"));
					GetGame()->SetAutoLogin(false);
				}
				break;

			case LOGIN_NOTCDKEY:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_4"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_BEBANED:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");

					char outStr[256];
					char szBanTime[32] = "";
					pMsg->GetStr(szBanTime, 32);
					sprintf(outStr, AppFrame::GetText("Base_5"), szBanTime);
					pszStr = outStr;
					GetInst(MsgEventManager).PushEvent(Msg_Ok,pszStr);
					GetGame()->SetAutoLogin(false);
				}
				break;

			case LOGIN_CDKEYDISABLE:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_6"));
					GetGame()->SetAutoLogin(false);
				}
				break;

			case LOGIN_IP_FORBID:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_7"));
					GetGame()->SetAutoLogin(false);
				}
				break;

			case LOGIN_PASSWORDERROR:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");

					// ��ʾ�������
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_8"));
					GetGame()->SetAutoLogin(false);
				}
				break;

			case LOGIN_CHECKOUT:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_9"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGINDB_ERROR:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_10"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_ACCOUNT_NOT_ACTIVE:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_39"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_ACCOUNT_IS_LOCKED:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_40"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_REJECTED:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_41"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_WAIT_INIT_FINISH:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_42"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_DB_COM_ERROR:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_43"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_CANT_CONN_DB:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_44"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_DB_OPT_FAILED:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_45"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_LOCAL_DB_FAILED:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_46"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_LOGINSERVERPLAYERFULL:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_38"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_PLAYER_ALREAD_IN_GAME:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_9"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_WORLD_SERVER_BE_CLOSED:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_25"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case  LOGIN_NOTENOUGH_HEALTHY_TIME:
				{
					char outStr[256]="";
					DWORD dwTime = pMsg->GetDWord();
					//��ʣ�಻����ʱ��ת��Ϊ��ʾʱ��
					//int iSec = dwTime%60;
			  //      int m1 = dwTime/60;
			  //      int iMin = m1%60;
		   //      	int ihour = m1/60%60; 
					sprintf(outStr, AppFrame::GetText("Base_49"));
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					pszStr = outStr; 
					GetInst(MsgEventManager).PushEvent(Msg_Ok,pszStr);
					GetGame()->SetAutoLogin(false);
				}
				break;
			case SECURITY_OPERATE_LOST:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_407"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_IP_LOGIN_MAX_SAME_CDKEY:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_413"));
					GetGame()->SetAutoLogin(false); 
				}
				break;
			case LOGIN_IP_LOGIN_NIMIETY_CDKEY:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_414"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_IP_CREATE_MAX:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_415"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			case LOGIN_MODE_ERROR:
				{
					GetGame()->GetNetClient()->Close();
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
					GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_420"));
					GetGame()->SetAutoLogin(false);
				}
				break;
			default:
				{
					GetGame()->GetNetClient()->Close();
					char outStr[256]="";
					sprintf(outStr,"%s%d",AppFrame::GetText("Base_408"),Result);
					pszStr = outStr; 
					GetInst(MsgEventManager).PushEvent(Msg_Ok,pszStr);
					GetGame()->SetAutoLogin(false);
				}
				break;
			}
			DWORD dwCurTime = timeGetTime();
            //GetGame()->GetCGuiEx()->GetLoginPageEx()->SetLastTime(dwCurTime);
			g_LoginTime = 0;
			GetGame()->SetGameState(GS_LOGIN);
			GetGame()->SDOLogOut();
		}
		break;

	case MSG_L2C_LOG_ANSWER_PLAYERBASELIST:	// �����������������
		{
			GetInst(WorldServerPage).Close();
			GetGame()->SetQueuePos(-1);

			char strCDKey[20],strName[50];
			BYTE nRoleCnt=0;

			//======================================
					WORD nLevel=0;
					DWORD nOccupation=0,
					nSex=0,
					nCountry=0,
					nHead=0,
					nHair=0,
					nFace=0,
					nNecklace=0,
					nWing=0,
					nBody=0,
					nBack=0,
					nGlove=0,
                    nShoulder=0,
					nWrist=0,
					nLeg=0,
					nBoot=0,
					nLring=0,
					nRring=0,
					nWeapon=0,
					nCard=0,
					nGodGoods=0,
					nEffigy=0,
					nLifeCowry=0,
					nSoulGuard=0,
					nRider=0,

                    nHeadLevel=0,
                    nNecklaceLevel=0,
                    nWingLevel=0,
                    nBodyLevel=0,
                    nBackLevel=0,
                    nGloveLevel=0,
                    nShoulderLevel=0,
                    nWristLevel=0,
                    nLegLevel=0,
                    nBootLevel=0,
                    nLringLevel=0,
                    nRringLevel=0,
                    nWeaponLevel=0,
                    nCardLevel=0,
                    nGodGoodsLevel=0,
                    nEffigyLevel=0,
                    nLifeCowryLevel=0,
                    nSoulGuardLevel=0,
                    nRiderLevel=0,

					nRgnID = 0,
					nField = 0;

			//��ȡ��������֤��
			long lSignCode = pMsg->GetLong();
			GetGame()->SetSignCode(lSignCode);
			
			long nRemainDays;
			CGUID playerid;
			BYTE bRet=0;
			bRet=pMsg->GetByte();
			int i;

			if(bRet == 2)
			{
				GetGame()->GetNetClient()->Close();
				GetGame()->SetGameState(GS_LOGIN);
				GetGame()->SetAutoLogin(false);
				GetGame()->SDOLogOut();
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_11"));
				return;
			}
			if(bRet != 1)
			{
				GetGame()->GetNetClient()->Close();
				GetGame()->SetGameState(GS_LOGIN);
				GetGame()->SetAutoLogin(false);
				GetGame()->SDOLogOut();
				GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Base_12"));
				return;
			}
            
			pMsg->GetStr(strCDKey,20);

			// ������ɫʱ��������ҷ�Χ
			BYTE nRandCountryRange = pMsg->GetByte();

            CreateRolePage::SetRandCountryRange( nRandCountryRange );

			nRoleCnt=BYTE(pMsg->GetShort());
			if(nRoleCnt==0)
			{
				GetGame()->SetGameState(GS_CREATE);
				return;
			}

			// �����ǽ���3����ɫ bugs

			// ����ɵĽ�ɫ
            GetInst(SelectRolePage).ClearPlayer();
			for(i=0;i<nRoleCnt;i++)
			{
				// ���
				nField=BYTE(pMsg->GetShort());//��λֵ
				pMsg->GetGUID(playerid);
				pMsg->GetStr(strName,50);//�õ�����

					nLevel			=pMsg->GetShort();
					nOccupation		=pMsg->GetShort();
					nSex			=pMsg->GetShort();
					nCountry		=pMsg->GetShort();
					nHead			=pMsg->GetShort();
					nHair			=pMsg->GetShort();
					nFace			=pMsg->GetLong();
					nNecklace		=pMsg->GetShort();
					nWing			=pMsg->GetShort();
					nBody			=pMsg->GetShort();
					nBack			=pMsg->GetShort();
					nGlove			=pMsg->GetShort();
					nShoulder		=pMsg->GetShort();
					nWrist		    =pMsg->GetShort();
					nLeg			=pMsg->GetShort();
                    nBoot           =pMsg->GetShort();
					nLring			=pMsg->GetShort();
					nRring			=pMsg->GetShort();
					nWeapon			=pMsg->GetShort();
					nCard			=pMsg->GetShort();
					nGodGoods		=pMsg->GetShort();
					nEffigy			=pMsg->GetShort();
					nLifeCowry		=pMsg->GetShort();
					nSoulGuard		=pMsg->GetShort();
					nRider			=pMsg->GetShort();

					nHeadLevel		=pMsg->GetShort();
					nNecklaceLevel	=pMsg->GetShort();
					nWingLevel		=pMsg->GetShort();
					nBodyLevel		=pMsg->GetShort();
					nBackLevel		=pMsg->GetShort();
					nGloveLevel		=pMsg->GetShort();
                    nShoulderLevel	=pMsg->GetShort();
                    nWristLevel	    =pMsg->GetShort();
                    nLegLevel		=pMsg->GetShort();
                    nBootLevel      =pMsg->GetShort();
                    nLringLevel		=pMsg->GetShort();
                    nRringLevel		=pMsg->GetShort();
                    nWeaponLevel	=pMsg->GetShort();
                    nCardLevel		=pMsg->GetShort();
                    nGodGoodsLevel	=pMsg->GetShort();
                    nEffigyLevel	=pMsg->GetShort();
                    nLifeCowryLevel	=pMsg->GetShort();
                    nSoulGuardLevel	=pMsg->GetShort();
                    nRiderLevel		=pMsg->GetShort();

					nRemainDays		=(char)pMsg->GetLong();
					nRgnID			=pMsg->GetLong(); 

				CPlayer *pPlayer = new CPlayer;
				pPlayer->SetGraphicsID(nSex+1);
				pPlayer->SetExID(playerid);
				pPlayer->SetOccupation((eOccupation)nOccupation);

				pPlayer->SetName(strName);
				pPlayer->SetLevel(nLevel);
				pPlayer->SetSex((BYTE)nSex);
				pPlayer->SetCountry((BYTE)nCountry);
				pPlayer->SetHairPic((BYTE)nHair);
				pPlayer->SetFacePic((BYTE)nFace);
				pPlayer->SetShowFashion(FASHION_TYPE_MANTEAU);

				//===========================
				// -- ���װ�� 
				//pPlayer->SetEquipIndex(CPlayer::EQUIP_HEAD, nHead);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_NECKLACE, nNecklace);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_WING,  nWing);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_BODY, nBody);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_BACK, nBack);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_HAND, nGlove);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_SHOULDER, nShoulder);

				// -- �ұ�װ��
				pPlayer->SetEquipIndex(CPlayer::EQUIP_WRIST, nWrist);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_LEG, nLeg);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_BOOT,  nBoot);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_LRING, nLring);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_RRING, nRring);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_WEAPON, nWeapon);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_CARD, nCard);

				// -- �±�װ��
				pPlayer->SetEquipIndex(CPlayer::EQUIP_GODGOODS, nGodGoods);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_EFFIGY, nEffigy);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_LIFECOWRY, nLifeCowry);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_SOULGUARD, nSoulGuard);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_RIDER, nRider);

				// --װ���ȼ�
				// -- ���װ��
				pPlayer->SetEquipLevel(CPlayer::EQUIP_HEAD, (BYTE)nHeadLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_NECKLACE, nNecklaceLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_WING,  nWingLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_BODY, nBodyLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_BACK, nBackLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_HAND, nGloveLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_SHOULDER, nShoulderLevel);

                // -- �ұ�װ��
                pPlayer->SetEquipIndex(CPlayer::EQUIP_WRIST, nWristLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_LEG, nLegLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_BOOT,  nBootLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_LRING, nLringLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_RRING, nRringLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_WEAPON, nWeaponLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_CARD, nCardLevel);

                // -- �±�װ��
                pPlayer->SetEquipIndex(CPlayer::EQUIP_GODGOODS, nGodGoodsLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_EFFIGY, nEffigyLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_LIFECOWRY, nLifeCowryLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_SOULGUARD, nSoulGuardLevel);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_RIDER, nRiderLevel);

				pPlayer->SetDisplayModel(false);
				pPlayer->SetDisplayModelGroup();
				pPlayer->SetRegionID(nRgnID);
				pPlayer->SetPlayerRemainDays((char)nRemainDays);
				pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WEAPON);
				pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WING);

                GetInst(SelectRolePage).AddPlayer(pPlayer);

				pPlayer->SetState(CShape::STATE_PEACE);
				pPlayer->SetIsNeedAlphaFade(false);
			}

			/*if(pPage->GetPlayerVector()->size())
			{
				pPage->SetSelectedNum( 0 );
			}

			pPage->SetCurPage(0);*/
			GetGame()->SetGameState(GS_SELECT);
			FireUIEvent("SelectRolePage","UpdateDelRemainTime");
		}
		break;		

	case MSG_L2C_LOG_ANSWERDELETE://loginserver����ɾ����ɫ�Ľ��
		{
			char strCDKey[20]="";	
			int result=pMsg->GetChar();
			if(result==LOGIN_DELETEROLE_SUCCEED)
			{
				CGUID PlayerID;
				pMsg->GetGUID(PlayerID);
				pMsg->GetStr(strCDKey,20);
				char nRemainDays = pMsg->GetChar();
				SelectRolePage& srp = GetInst(SelectRolePage);
				
				ulong cnt = srp.GetPlayerCount();
				for(ulong c = 0 ; c < cnt ; ++c)
				{
					CPlayer* player = srp.GetPlayer(c);
					if(player && player->GetExID() == PlayerID)
					{
						player->SetPlayerRemainDays(nRemainDays);
						char delInfo[256];
						sprintf(delInfo,"����%d�챻ɾ��",nRemainDays);
						GetInst(MsgEventManager).PushEvent(Msg_Ok,delInfo);
						FireUIEvent("SelectRolePage","UpdateDelRemainTime");
						return;
					}
				}
			}
			else
			{
				int nID=pMsg->GetLong();
				pMsg->GetStr(strCDKey,20);
				long lFlag = pMsg->GetLong();
				char str[256]="";
				if(lFlag == 0)
				{
					sprintf(str,AppFrame::GetText("Base_14"));
					GetInst(MsgEventManager).PushEvent(Msg_Ok,str);
				}
				else if(lFlag == 1)
				{
					sprintf(str,AppFrame::GetText("Base_15"));
					GetInst(MsgEventManager).PushEvent(Msg_Ok,str);
				}
				else if(lFlag == 2)
				{
					sprintf(str,AppFrame::GetText("Base_16"));
					GetInst(MsgEventManager).PushEvent(Msg_Ok,str);
				}
				else if(lFlag == 3)
				{
					sprintf(str,AppFrame::GetText("Base_17"));
					GetInst(MsgEventManager).PushEvent(Msg_Ok,str);
				}
			}
		}
		break;

	case MSG_L2C_LOG_ANSWERRESTORE:
		{
			char res = pMsg->GetChar();

			if(res == LOGIN_RESTOREROLE_SUCCEED)
			{
				CGUID PlayerID;
				pMsg->GetGUID(PlayerID);
				SelectRolePage& srp = GetInst(SelectRolePage);
				ulong cnt = srp.GetPlayerCount();
				for(ulong c = 0 ; c < cnt ; ++c)
				{
					CPlayer* player = srp.GetPlayer(c);
					if(player && player->GetExID() == PlayerID)
					{
						player->SetPlayerRemainDays(-1);
						GetInst(MsgEventManager).PushEvent(Msg_Ok,
							AppFrame::GetText("Base_18"));
						FireUIEvent("SelectRolePage","UpdateDelRemainTime");
						return;
					}
				}
			}
			else
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_19"));
			}
		}
		break;

	case MSG_L2C_LOG_ANSWERCREATE://loginserver���ش�����ɫ�Ľ��
		{
			int result=pMsg->GetChar();
			if(result==LOGIN_CREATEROLE_SUCCEED)
			{
				char strCDKey[20],strName[50];
				short nLevel;
				char nOccupation;
				char nSex;
				BYTE nHead;
				BYTE nHair;
				long lFace;
				CGUID Playerid;
				long lRegionID;
				BYTE byCountry;
				
                DWORD dwHead=0,
                    dwNecklace=0,
                    dwWing=0,
                    dwBody=0,
                    dwBack=0,
                    dwGlove=0,
                    dwShoulder=0,
                    dwWrist=0,
                    dwLeg=0,
                    dwBoot=0,
                    dwLring=0,
                    dwRring=0,
                    dwWeapon=0,
                    dwCard=0,
                    dwGodGoods=0,
                    dwEffigy=0,
                    dwLifeCowry=0,
                    dwSoulGuard=0;

               BYTE nHeadLevel=0,
                    nNecklaceLevel=0,
                    nWingLevel=0,
                    nBodyLevel=0,
                    nBackLevel=0,
                    nGloveLevel=0,
                    nShoulderLevel=0,
                    nWristLevel=0,
                    nLegLevel=0,
                    nBootLevel=0,
                    nLringLevel=0,
                    nRringLevel=0,
                    nWeaponLevel=0,
                    nCardLevel=0,
                    nGodGoodsLevel=0,
                    nEffigyLevel=0,
                    nLifeCowryLevel=0,
                    nSoulGuardLevel=0;
	
				pMsg->GetStr(strCDKey,20);
				pMsg->GetGUID(Playerid);
				pMsg->GetStr(strName,50);
				nLevel=pMsg->GetShort();
				nOccupation=pMsg->GetChar();
				nSex=pMsg->GetChar();
				byCountry = pMsg->GetByte();
				nHead=pMsg->GetByte();
				nHair=pMsg->GetByte();
				lFace=pMsg->GetLong();

				// ���װ����
                dwHead=pMsg->GetDWord();
                dwNecklace=pMsg->GetDWord();
                dwWing=pMsg->GetDWord();
                dwBody=pMsg->GetDWord();
                dwBack=pMsg->GetDWord();
                dwGlove=pMsg->GetDWord();
                dwShoulder=pMsg->GetDWord();
                dwWrist=pMsg->GetDWord();
                dwLeg=pMsg->GetDWord();
                dwBoot=pMsg->GetDWord();
                dwLring=pMsg->GetDWord();
                dwRring=pMsg->GetDWord();
                dwWeapon=pMsg->GetDWord();
                dwCard=pMsg->GetDWord();
                dwGodGoods=pMsg->GetDWord();
                dwEffigy=pMsg->GetDWord();
                dwLifeCowry=pMsg->GetDWord();
                dwSoulGuard=pMsg->GetDWord();
				// -- װ���ȼ�
				// ���װ����
                nHeadLevel=pMsg->GetByte();
                nNecklaceLevel=pMsg->GetByte();
                nWingLevel=pMsg->GetByte();
                nBodyLevel=pMsg->GetByte();
                nBackLevel=pMsg->GetByte();
                nGloveLevel=pMsg->GetByte();
                nShoulderLevel=pMsg->GetByte();
                nWristLevel=pMsg->GetByte();
                nLegLevel=pMsg->GetByte();
                nBootLevel=pMsg->GetByte();
                nLringLevel=pMsg->GetByte();
                nRringLevel=pMsg->GetByte();
                nWeaponLevel=pMsg->GetByte();
                nCardLevel=pMsg->GetByte();
                nGodGoodsLevel=pMsg->GetByte();
                nEffigyLevel=pMsg->GetByte();
                nLifeCowryLevel=pMsg->GetByte();
                nSoulGuardLevel=pMsg->GetByte();

				lRegionID = pMsg->GetLong();

				CPlayer *pPlayer = new CPlayer;
				pPlayer->SetGraphicsID(nSex+1);
				pPlayer->SetExID(Playerid);
				pPlayer->SetOccupation((eOccupation)nOccupation);
				pPlayer->SetName(strName);
				pPlayer->SetLevel(nLevel);
				pPlayer->SetSex(nSex);
				pPlayer->SetCountry(byCountry);
				pPlayer->SetHairPic(nHair);
				pPlayer->SetFacePic((BYTE)lFace);

				// -- ���װ�� 
				pPlayer->SetEquipIndex(CPlayer::EQUIP_HEAD, dwHead);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_NECKLACE, dwNecklace);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_WING,  dwWing);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_BODY, dwBody);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_BACK, dwBack);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_HAND, dwGlove);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_SHOULDER, dwShoulder);

				// -- �ұ�װ��
				pPlayer->SetEquipIndex(CPlayer::EQUIP_WRIST, dwWrist);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_LEG, dwLeg);
                pPlayer->SetEquipIndex(CPlayer::EQUIP_BOOT, dwBoot);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_LRING,  dwLring);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_RRING, dwRring);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_WEAPON, dwWeapon);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_CARD, dwCard);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_GODGOODS, dwGodGoods);

				// -- �±�װ��
				pPlayer->SetEquipIndex(CPlayer::EQUIP_EFFIGY, dwEffigy);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_LIFECOWRY, dwLifeCowry);
				pPlayer->SetEquipIndex(CPlayer::EQUIP_SOULGUARD, dwSoulGuard);

				// --װ���ȼ�
				// -- ���װ��
				pPlayer->SetEquipLevel(CPlayer::EQUIP_HEAD, nHeadLevel);
				pPlayer->SetEquipLevel(CPlayer::EQUIP_NECKLACE, nNecklaceLevel);
				pPlayer->SetEquipLevel(CPlayer::EQUIP_WING,  nWingLevel);
				pPlayer->SetEquipLevel(CPlayer::EQUIP_BODY, nBodyLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_BACK, nBackLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_HAND, nGloveLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_SHOULDER, nShoulderLevel);

                // -- �ұ�װ��
                pPlayer->SetEquipLevel(CPlayer::EQUIP_WRIST, nWristLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_LEG, nLegLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_BOOT, nBootLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_LRING,  nLringLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_RRING, nRringLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_WEAPON, nWeaponLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_CARD, nCardLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_GODGOODS, nGodGoodsLevel);

                // -- �±�װ��
                pPlayer->SetEquipLevel(CPlayer::EQUIP_EFFIGY, nEffigyLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_LIFECOWRY, nLifeCowryLevel);
                pPlayer->SetEquipLevel(CPlayer::EQUIP_SOULGUARD, nSoulGuardLevel);

				pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WEAPON);
		//		pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WEAPON2);
				pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WING);
				pPlayer->UpdateWeaponShow(CPlayer::EQUIP_P_HEADGEAR);
				

				// --
				lRegionID = pMsg->GetLong();
				pPlayer->SetPlayerRemainDays(-1);

				// add by lpf - 2007.12.13
				pPlayer->SetDisplayModel();
				pPlayer->SetDisplayModelGroup();

                GetInst(SelectRolePage).AddPlayer(pPlayer);
                GetGame()->SetGameState(GS_SELECT);
				FireUIEvent("SelectRolePage","UpdateDelRemainTime");
				
			}
			else if(result==LOGIN_CREATEROLE_DBERROR)
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
				GetGame()->SetGameState(GS_LOGIN);
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_20"));
			}
			else if(result==LOGIN_CREATEROLE_NAMEEXIST)
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
                GetGame()->SetGameState(GS_LOGIN);
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_21"));
			}
			else if(result==LOGIN_CREATEROLE_FULL)
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");	
                GetGame()->SetGameState(GS_LOGIN);
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_22"));
			}
			else if (result == LOGIN_CREATEROLE_NAMEINVALID)
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
                GetGame()->SetGameState(GS_LOGIN);
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_23"));
			}
			else
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
                GetGame()->SetGameState(GS_LOGIN);
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_24"));
			}
		}
		break;

	case MSG_L2C_LOG_ANSWER_PLAYERDATA://loginserver���ػ�ȡ��ϸ��Ϣ�Ľ��,������gs��IP�Ͷ˿�
		{
#ifdef _RUNSTACKINFO_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MSG_L2C_LOG_ANSWER_PLAYERDATA Start.");
#endif
			Log4c::Trace(ROOT_MODULE,"MSG_L2C_LOG_ANSWER_PLAYERDATA");
			GetGame()->SetQueuePos(-1);

			long lChangeServer = pMsg->GetLong();
			GetGame()->SetChangeServerCode(lChangeServer);
			int result=pMsg->GetChar();
			if(result==LOGIN_QUEST_PLAYERDATA_SUCCEED)
			{
			#ifdef _RUNSTACKINFO_
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"result==LOGIN_QUEST_PLAYERDATA_SUCCEED Start.");
			#endif
				char strGsIp[20],strCDKey[20];
				long lGsPort;
				pMsg->GetStr(strCDKey,20);
				pMsg->GetStr(strGsIp,20);
				lGsPort=pMsg->GetLong();

                CGUID PlayerID = GetInst(SelectRolePage).GetSelPlayer()->GetExID();

				CClient* pTemptClient = GetGame()->GetNetClient();
				GetGame()->SetGameCDKEY(strCDKey);
				CClient* pNetClient = GetGame()->ConnectGameServer(strGsIp,lGsPort,PlayerID);				

				//����GameServer�ɹ�
				if(pNetClient != NULL)
				{
					GetGame()->SetNetClient(pNetClient);
					CMessage::SetClient(pNetClient);

					pTemptClient->Exit();
					pTemptClient->Close();
					SAFE_DELETE(pTemptClient);
					
					// �ͷŲֿ���Ʒ����
					CGCMana::Inst()->Clear( true );

					// ��λ��������
					/*CMainBarPageEx* pMainBarPageEx = GetGame()->GetCGuiEx()->GetMainBarPageEx();
					if( pMainBarPageEx )
					{
						pMainBarPageEx->SetChannel( CHAT_NORMAL );
						pMainBarPageEx->SetChatType( CHAT_NORMAL );
						g_strLastWisper = "";
					}*/
				}
				else
				{
					GetGame()->SetNetClient(NULL);
					CMessage::SetClient(NULL);

					//����GameServerʧ��
					CMessage msg(MSG_C2L_LOG_QUESTEXIT);
					msg.Add((long)0);
					msg.Send(pTemptClient);

					//��ģ�����������ʱ����ɾ����������̲߳����Ƿ�		
					pTemptClient->Exit();
					pTemptClient->Close();
					SAFE_DELETE(pTemptClient);					

					GetGame()->SetGameState(GS_LOGIN);
					GetInst(MsgEventManager).PushEvent(Msg_Ok,
						AppFrame::GetText("Base_25"));
				}

			#ifdef _RUNSTACKINFO_
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"result==LOGIN_QUEST_PLAYERDATA_SUCCEED End.");
			#endif
			}
			else if(result == LOGIN_LOGINSERVERPLAYERFULL)    //�����������ﵽ����
			{
				GetGame()->ReNew();
				GetGame()->SetGameState(GS_LOGIN);
				g_bHandleSocketClose=true;
                GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_38"));
			}
			else
			{
				GetInst(MsgEventManager).PushEvent(Msg_Ok,
					AppFrame::GetText("Base_26"));
			}
#ifdef _RUNSTACKINFO_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"MSG_L2C_LOG_ANSWER_PLAYERDATA End.");
#endif
		}
		break;

	case MSG_S2C_LOG_ANSWER_PLAYERDATA://gameserver�������ϸ���ݵķ���
		{
			Log4c::Trace(ROOT_MODULE,"MSG_S2C_LOG_ANSWER_PLAYERDATA");
			//get the detail data from gameserver
			long lCode = pMsg->GetLong();
			long lFlag = pMsg->GetLong();
			// �ɹ�������Ϸ����
			if( lFlag )
			{
				// �ͷ�ѡ��������Դ
                UIManager::CloseAllPage();
                //UIManager::ReleaseAllPage();
                UIManager::ReleasePage("Login");
                UIManager::ReleasePage("WorldServer");
                UIManager::ReleasePage("SelectRolePage");
                UIManager::ReleasePage("CreateRolePage");
                UIManager::ReleasePage("SelectLS");

                //�ͷ�ǰһ����ҵĽ���__gongran
                CloseAllWindow();
                DestroyAllWindow();

				CGUID PlayerID;
				pMsg->GetGUID(PlayerID);
				//GetGame()->GetCGuiEx()->GetSelectRolePageEx()->StopSound();
				CPlayer* pPlayer = GetGame()->GetMainPlayer();

				if (pPlayer==NULL)
				{
					GetGame()->NewMainPlayer();
					pPlayer = GetGame()->GetMainPlayer();
				}

				if( pPlayer )
				{
					// ����ת������
					pMsg->GetEx(&GetGame()->GetSetup()->fStr2MinAtk[0], sizeof(float)*33);
					GetGame()->GetSetup()->lHitTime = pMsg->GetLong();
					GetGame()->GetSetup()->wBaseRp_Lv1=pMsg->GetWord();
					GetGame()->GetSetup()->wBaseRp_Lv2=pMsg->GetWord();
					GetGame()->GetSetup()->wPkCountPerKill=pMsg->GetWord();
//////////////////////////////////////////���緶Χ�ڵ�����///////////////////////////////////
					GetGame()->GetSetup()->fBasePriceRate=pMsg->GetFloat();
					GetGame()->GetSetup()->fTradeInRate=pMsg->GetFloat();
					GetGame()->GetSetup()->fRepairFactor=pMsg->GetFloat();
					pMsg->GetStr(GetGame()->GetSetup()->szTalkCountryGoodsName, 32);
					GetGame()->GetSetup()->nTalkCountryGoodsNum = pMsg->GetLong();
					GetGame()->GetSetup()->dwTalkCountryMoney = pMsg->GetDWord();
					pMsg->GetStr(GetGame()->GetSetup()->szTalkWorldGoodsName, 32);
					GetGame()->GetSetup()->nTalkWorldGoodsNum = pMsg->GetLong();
					GetGame()->GetSetup()->dwTalkWorldMoney = pMsg->GetDWord();
					// �˹�
					//pMsg->GetStr(GetGame()->GetSetup()->szTalkCountryUnionGoodsName, 32);
					GetGame()->GetSetup()->nTalkCountryUnionGoodsNum = 0;//pMsg->GetLong();
					GetGame()->GetSetup()->dwTalkCountryUnionMoney = 0;//pMsg->GetDWord();
/////////////////////////////////////////////////////////////////////////////////////////////
					pPlayer->SetEnemyPlayerLevelFirstQuotiety(pMsg->GetByte());
					pPlayer->SetEnemyPlayerLevelSecondQuotiety(pMsg->GetByte());

					if (CGlobeSetup::GetSetup()->dwPlayerAlphaFadeTimeLength != 0)
					{
						pPlayer->SetIsNeedAlphaFade(true);
						pPlayer->SetAlphaFadeInTimeLength(CGlobeSetup::GetSetup()->dwPlayerAlphaFadeTimeLength);
						pPlayer->SetAlphaFadeMode(CAlphaFadeObjectList::ALPHA_FADE_IN);
					}

					extern long AREA_WIDTH;
					extern long AREA_HEIGHT;
					AREA_WIDTH = pMsg->GetLong();
					AREA_HEIGHT = pMsg->GetLong();

					if (pPlayer->GetFather())
					{
						((CClientRegion*)pPlayer->GetFather())->RemoveObject(pPlayer);
					}
					
					DBReadSet setReadDB;
					pMsg->GetDBReadSet(setReadDB);
					pPlayer->DecodeFromDataBlock(setReadDB);

					// ��ͼ����
					char strRegionName[256];
					pMsg->GetStr(strRegionName,256);
					long lTaxRate = pMsg->GetLong();							//�õ�˰��
					eRWT eWarRegionType = (eRWT)pMsg->GetByte();//�õ�ս����������
					eRST eSpaceType=(eRST)pMsg->GetByte();//�Ƿ���ˮ�³���
					bool bChangeEquip = pMsg->GetByte()==0?false:true;
					CGUID RegionID;
					pMsg->GetGUID(RegionID);
					long lRegionID = pMsg->GetLong();
					long lResourceID = pMsg->GetLong();
					float fExpScale = pMsg->GetFloat();
					eRPKT ePKRegionType = (eRPKT)pMsg->GetByte();

					pPlayer->SetDisplayModel();
					GetGame()->QuestEnterRegion(lRegionID,RegionID,lResourceID,strRegionName, pPlayer->GetTileX(), pPlayer->GetTileY(), pPlayer->GetDir());
					//����˰��
					CClientRegion * pRegion = GetGame()->GetRegion();
					if(pRegion)
					{
						pRegion->SetTaxRate(lTaxRate);
						pRegion->SetWarType(eWarRegionType);
						pRegion->SetExpScale(fExpScale);
						pRegion->SetSpaceType(eSpaceType);
						pRegion->SetChangeEquip(bChangeEquip);
						pRegion->SetPKRegionType(ePKRegionType);

						//�����ǽ��볡��
						pRegion->OnMainPlayerEnter();						
					}
					//�򿪳���Ѫ�� �����볡��shape����
					//@todo ������
					//long lPetCount = pPlayer->GetFightPetNum();
					//if ( lPetCount > 0)
					//{
					//	for (int i = 0; i < lPetCount; i++)
					//	{
					//		CPet *pPet = pPlayer->GetActivePet(i+1);
					//		if (pPet)
					//			GetGame()->GetRegion()->AddObject((CBaseObject*)pPet);
					//	}
					//	//GetGame()->GetCGuiEx()->GetPetNamHpPage()->Open();
					//	//GetGame()->GetCGuiEx()->GetPetHotKeyBarPage()->Update();
					//}
					//////////////////////////��÷�����Ϣ////////////////////////////////////
					//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->SetCurRegionID(lRegionID);
					//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->DecodeFromDataBlock(setReadDB);

					// ��������ʱ��
					DWORD dwCurTime = pMsg->GetDWord();

					// **************************�����ľݵ������Ϣ��09.9.29���ƣ�*********************
					BYTE	bOpenFlag = pMsg->GetByte();
					long	lPointNum = pMsg->GetLong();
					char	szPointName[256] = "";
					if(pRegion)
					{
						//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetIsOpenSPoint(bOpenFlag>0);
						CClientRegion::tagSPointInfo stSPInfo;
						for ( long num = 0; num < lPointNum; ++num)
						{
							stSPInfo.strPointName = pMsg->GetStr(szPointName,256);
							stSPInfo.lPosX		= pMsg->GetLong();
							stSPInfo.lPosY		= pMsg->GetLong();
							stSPInfo.bState		= (BYTE)pMsg->GetLong();
							pRegion->AddSPointInfo(stSPInfo);
						}
					}
					// ********************************************************************************

					//��ʼ�����ֹ�������
					//���ݷ������ý����ϵķ��߰�ť
					//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->SetSelectBotton();

					//�رչ��������Ч
					GetGame()->GetAudioList()->SetOppearSoundEnble(false);

					//��ʼ�������ѫ��
					//GetGame()->GetCGuiEx()->GetCharacterPageEx()->InitMedalGoods(); 

					//���ÿͻ�����ʾ��ʱ��
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetServerTime(dwCurTime);

					//�����½ʢ��ȦȦ������ȦȦ�Ľ�ɫ��Ϣ
					//GetGame()->SetPlayerInfo(pPlayer->GetName(),pPlayer->GetSex());

				}
			}
			else
			{
				GetGame()->CloseNetClient();
				GetGame()->SetGameState(GS_LOGIN);				
			}
		}
		break;

		// �������Ͽ�������
	case MSG_C2C_LOG_SERVERLOST:
		{			
			GetGame()->ReNew();	// ���¿�ʼ

			GetGame()->SetGameState( GS_LOGIN );
			GetGame()->	SDOLogOut();
			g_bHandleSocketClose=true;
		}
		break;
	case MSG_L2C_LOG_UPDATEQUEUENUM:
		{
			long lNumber = pMsg->GetLong();
			GetGame()->SetQueuePos(lNumber);
		}
		break;

		// �յ�Serverʱ��
	case MSG_S2C_LOG_GAMESERVERTIME:
		{
			long lPlayerID = pMsg->GetLong();
			unsigned long ulServerTime = pMsg->GetDWord();
			//CGuiExPage::m_lC2SDeltaTime = (long)pMsg->GetDWord() - (long)time( NULL );

			// ����
			GetGame()->m_ulServerTime = ulServerTime;
			GetGame()->m_ulClientTime = /*(ULONG)time(NULL)*/ timeGetTime();
			GetGame()->m_bMsgTimeStart = true;


#ifdef _DEBUG
			char debug[128];
			sprintf(debug, "Recieve Init Server time %d\r\n", ulServerTime);
			OutputDebugString(debug);
#endif // _DEBUG
		}
		break;

		// ����������֤��Ϣ�������ַ���
	case MSG_S2C_LOG_SEQUENCESTRING:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			CSequenceString::Uninitialize();

			// ��ʼ ������
			BYTE ByteData[10240];
			pMsg->GetEx(ByteData,10240);
			long lPos = 0;
			CSequenceString::Initialize(ByteData,lPos);
		}
		break;

	// �����ʺŵ���Ϣ����ʾ��ѡ���ɫ����
	case MSG_L2C_LOG_SENDACCOUNTINFO:
		{
			char strCdkey[256];
			char strInfo[256];
			pMsg->GetStr(strCdkey, 256);
			pMsg->GetStr(strInfo, 256);
		}
		break;
	case MSG_L2C_LOG_UPDATEWORLDSERVERINFO:
		{
			if( GetGame()->GetGameState() == GS_SELECTWS)
			{
				int wsNum=pMsg->GetShort();					
				char wsName[20];
				for(int i=0;i<wsNum;i++)
				{
					long lStateLvl = pMsg->GetLong();
					pMsg->GetStr(wsName,20);
				}
			}
		}
		break;
	case MSG_L2C_LOG_WAITDATALOAD:
		{
			char str[100]="";
			sprintf(str,AppFrame::GetText("Base_47"));
			//GetInst(MsgEventManager).PushEvent(GE_Box_Default_Window,str);
			GetGame()->SetQueuePos(-1);
		}
		break;
	case MSG_L2C_LOG_FROZEN_CDKEY:
		{
			DWORD dwTime = pMsg->GetDWord();
			GetGame()->GetNetClient()->Close();
			GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav"); 
			int h = 0;                   //Сʱ
			int m = 0;                   //����
			int s = 0;                   //��
			DWORD dwLastTimeTemp = dwTime;
			s = dwLastTimeTemp%60;
			DWORD m1 = dwLastTimeTemp/60;
			m = m1%60;
			DWORD h1 = m1/60;
			h = h1%60; 
			char pszStr[256] = "";
			sprintf(pszStr,AppFrame::GetText("Base_48"),h,m,s);
			GetGame()->SetAutoLogin(false);
			//if (GetGame()->GetGameState() == GS_LOGIN)
			//pMsgBox->Open(pLoginPage,pszStr,CMessageBoxPageEx::MSG_OK,true);
			//else if (GetGame()->GetGameState() == GS_PWD_CARD)
			//	pMsgBox->Open(GetGame()->GetCGuiEx()->GetMatrixCardPageEx(),pszStr,CMessageBoxPageEx::MSG_OK,true);
		}
		break;
	case MSG_L2C_LOG_FCM_TIME:   //������֪ͨ������ʣ��ʱ��
		{
			DWORD dwTime = pMsg->GetDWord();    //��
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetIsIndulgence(true);
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetIndulgenceTime(dwTime*1000);
			if(GetGame()->GetGameState()==GS_RUN)
			{
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->OpenIndulgenceComp(1); 
                //GetGame()->GetCGuiEx()->GetMainBarPageEx()->FcmTimeNote(true);
			}
		}
		break;
	case MSG_L2C_LOG_SECURITY_VALIDATE:		//֪ͨ�ͻ��˽��а�ȫ��Ϣ���루�ܱ�����
		{
			//CMatrixCardPageEx *pMatrixPage = GetGame()->GetCGuiEx()->GetMatrixCardPageEx();
			//CMessageBoxPageEx *pMsgPage    = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
			//if (pMatrixPage && pMsgPage)
			//{
			//	WORD wVertifyCount = pMatrixPage->GetVerticyCount();
			//	if (wVertifyCount>=1)
			//	{
			//		pMsgPage->Open(pMatrixPage,CStringReading::LoadText(1,404),CMessageBoxPageEx::MSG_OK,true);
			//	}
			//	
			//	//��ȡ����3��λ��
			//	unsigned char *cPos = new unsigned char[3];
			//	pMsg->GetEx(cPos,3);
			//	pMatrixPage->ReSetPage();
			//	pMatrixPage->SetMatrixPositon(cPos,3);
			//	SAFE__DELETE_ARRAY(cPos);
			//	pMatrixPage->TIME_OUT = pMsg->GetLong();
			//}
			
			//���õ�ǰ��Ϸ״̬Ϊ �ܱ�������״̬
			GetGame()->SetGameState(GS_PWD_CARD);
		}
		break;
	case MSG_L2C_LOG_SECURITY_VALIDATINT:
		{
			//CMessageBoxPageEx *pMsgPage    = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
			//CLoginPageEx	  *pLoginPage  = GetGame()->GetCGuiEx()->GetLoginPageEx();
			DWORD dwCurTime = timeGetTime();
           // GetGame()->GetCGuiEx()->GetLoginPageEx()->SetLastTime(dwCurTime);
			g_LoginTime = 0;
			GetGame()->SetGameState(GS_LOGIN);
			GetInst(MsgEventManager).PushEvent(Msg_Ok,
				AppFrame::GetText("Base_406"));
			//if (pMsgPage)
			//{
			//	pMsgPage->Open(pLoginPage,CStringReading::LoadText(1,406),CMessageBoxPageEx::MSG_OK,true);
			//}
		}
	}
}

