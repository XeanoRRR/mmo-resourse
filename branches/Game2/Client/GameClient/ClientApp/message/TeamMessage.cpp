#include "stdafx.h"
#include "Clientregion.h"
#include "Player.h"
#include "../Goods/Goods.h"
#include "Message.h"
#include "../GameClient/Game.h"
#include "../GameClient/GameControl.h"
#include "Other/Chat.h"
#include "Other/AudioList.h"          
#include "../MsgBoxSys/MsgEventDef.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../Team/Team.h"
#include "../../../Public/Common/Public.h"

///���������Ķ�����Ϣ����

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace  TeamMessage
{
    struct  tagTeam
    {
        CGUID  SourID;
        CGUID  DestID;
    };

    struct  tagRecommend
    {
        string name;
    };
}

void  InviteeJoinTeam(MsgEvent& event,eMsgResult UserId)
{
    TeamMessage::tagTeam* pTeam = (TeamMessage::tagTeam*)event.pData;
    if ( !pTeam ) return ;
    
    CMessage msg( MSG_C2S_TEAM_ANSWER_JOIN );
    msg.Add( pTeam->SourID );
    msg.Add( pTeam->DestID );
    /// ��ʾ�Է���Ϸ����ͬ���벻ͬ���������
    BYTE  ret = ( UserId == S_Ok ) ? 1 : 0;
    msg.Add((BYTE)ret); 
    msg.Send();

    SAFE_DELETE( pTeam );
}

void  RecommendJoinTeam(MsgEvent& event,eMsgResult UserId)
{
    TeamMessage::tagRecommend* Recommend = (TeamMessage::tagRecommend*)event.pData;
    if ( !Recommend ) return ;
    
    /// ��ͬ��ֱ�ӷ���
    if( UserId != S_Ok )   
    {
        SAFE_DELETE( Recommend ); 
        return ; 
    }

    CMessage  msg(MSG_C2S_TEAM_QUERY_JOIN_BYNAME);
    msg.Add( Recommend->name.c_str() );
    msg.Add( GetGame()->GetMainPlayer()->GetExID() );
    msg.Send();

    SAFE_DELETE( Recommend );
}

// ��Ӧ�����Ϣ
void OnTeamMessage(CMessage* pMsg)
{
	static bool bTeam = false; // ������ӣ�

	if (GetGame()->GetMainPlayer()==NULL)
		return;
	TeamData& td = GetInst(TeamData);
	switch(pMsg->GetType())
	{
	//---------07.8.3�¶��������Ϣ���������ƣ�----------
	case MSG_S2C_TEAM_MemberPos:
		{
			long bufsize = pMsg->GetLong();
			vector<BYTE> v_buff(bufsize);
			pMsg->GetEx(&v_buff[0],bufsize);
			//CTeamPageEx *pTeamPage = GetGame()->GetCGuiEx()->GetTeamPageEx();
			long pos = 0;
			long size = _GetLongFromByteArray(&v_buff[0],pos);
            CGUID  Captain = NULL_GUID;
            CPlayer* player = GetGame()->GetMainPlayer();
            long  Captainx = -1;
            long  Captainy = -1;
			for(long i=0; i<size; ++i)
			{
				CGUID PlayerID;
				_GetBufferFromByteArray(&v_buff[0],pos,PlayerID);
				long x = _GetLongFromByteArray(&v_buff[0],pos);
				long y = _GetLongFromByteArray(&v_buff[0],pos);  
                if ( player->GetCaptainID() == PlayerID )
                {
                        Captain  = PlayerID ;
                        Captainx = x;
                        Captainy = y;
                }
				td.UpdateMemberPos(PlayerID,(float)x,(float)y);
			}
            
            /// ���ͬһ����
            if ( player->GetCaptainID() == Captain )
            {
                CGameControl *pGameCtl = GetGame()->GetGameControl();
                if( pGameCtl->GetFollowType() )
                {
                    pGameCtl->AutoMoveTo((float)Captainx,(float)Captainy);
                }
            }
		}
		break;
	// ��ļ
	case MSG_S2C_TEAM_RECRUIT:
		{
			// ��ļ��Ϣ�����ߵ�ID
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long lFlag = pMsg->GetLong();
			CPlayer *pPlayer = (CPlayer *)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
			if(!pPlayer)	return;
			// ����ļ��ť���أ���ʾȡ����ļ��ť
			//CPopMenuPageEx *pPage = GetGame()->GetCGuiEx()->GetCPopMenuPageEx();
			//if(!pPage)	return;
			// ȡ����ļ
			if(lFlag==0)
			{
				pPlayer->SetRecruitingTeam(false);
				pPlayer->SetTeammateAmount(1);
				//CPassWordBoxPageEx *pPassPage = GetGame()->GetCGuiEx()->GetPassWordBoxPageEx();
				//if(pPassPage && pPassPage->IsOpen() && pPassPage->GetRecruitPlayer() == pPlayer)
				//	pPassPage->Close();
				return;				
			}
			char strName[20];
			pMsg->GetStr(strName,20);
			long lPass = pMsg->GetLong();
			pPlayer->SetRecruitingTeam(true);
			pPlayer->SetTeamname(strName);
			pPlayer->SetTeamHasPassword(lPass==0?false:true);
		}
		break;
	case MSG_S2C_TEAM_RECRUITED_NUM:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long lNum = pMsg->GetLong();     // ����ҵ�ǰ��������
			CPlayer *pPlayer = (CPlayer *)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
			if(!pPlayer)	return;
			pPlayer->SetTeammateAmount(lNum);
		}
		break;
	case MSG_S2C_TEAM_MastterChange:	// �ı�ӳ�
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			//if(GetGame()->GetCGuiEx()->GetTeamPageEx()->ChangeCaptain(PlayerID))
				//GetGame()->GetMainPlayer()->SetCaptainID(PlayerID);
			if(td.ChangeCaptain(PlayerID))
			{
				GetGame()->GetMainPlayer()->SetCaptainID(PlayerID);
			}
		}
		break;
	case MSG_S2C_TEAM_GoodsSetChange:	// ������Ʒʰȡ���䷽ʽ
		{
			long lAllotMade = pMsg->GetLong();
			char strText[64];
			sprintf(strText,AppFrame::GetText("TM_5"),
				AppFrame::LoadAppTextRes("TM_",30+lAllotMade));
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,strText,D3DCOLOR_ARGB(255,255,255,0));
			CPlayer *pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer)
				pPlayer->SetAllotMade(lAllotMade);
		}
		break;
	case MSG_S2C_TEAM_MemberStateChange:	// ����Ƿ����ߵ�״̬�ı�
		{
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			if(PlayerID == pMainPlayer->GetExID())
				return;
			long lState = pMsg->GetLong();
			vector<tagWSMemberInfo> sMemInfo = pMainPlayer->GetTeamMateInfo();
			for(uint i=0; i<pMainPlayer->GetTeamMateInfo().size();++i)
			{
				if((pMainPlayer->GetTeamMateInfo())[i].guid == PlayerID)
				{
					//char strText[128];
					(pMainPlayer->GetTeamMateInfo())[i].lState = lState;
					//GetGame()->GetCGuiEx()->GetTeamPageEx()->SetMemberIsOnline(PlayerID,lState);
					td.SetMemberIsOnline(PlayerID,lState);
					//td.UpdateTeamList();
					//if(lState == eMS_LEAVE)
					//	sprintf(strText,AppFrame::GetText("TM_eIDS_TEAM_MEMBERLOADEX"),(pMainPlayer->GetTeamMateInfo())[i].szName);
					//else
					//	sprintf(strText,AppFrame::GetText("TM_eIDS_TEAM_MEMBERRELOAD"),(pMainPlayer->GetTeamMateInfo())[i].szName);
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,strText,D3DCOLOR_ARGB(255,255,255,0));
				}
			}
		}
		break;
	case MSG_S2C_TEAM_TeamCreate:			// ��������
		{
			CGUID TeamID;
			pMsg->GetGUID(TeamID);
			CGUID LeaderId;
			pMsg->GetGUID(LeaderId);
			long lGoodsAllot = pMsg->GetLong(); //��Ʒ����
			long lMemberCount = pMsg->GetLong();
			GetGame()->GetMainPlayer()->GetTeamMateInfo().clear();
			for(long i=0; i<lMemberCount; ++i)
			{
				tagWSMemberInfo sMemberInfo;
				pMsg->GetEx(&sMemberInfo, sizeof(tagWSMemberInfo));
				CPlayer *pPlayer = (CPlayer *)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,sMemberInfo.guid);
				if(pPlayer)// �ö����ڳ����ڿɼ�
				{
					pPlayer->SetTeamID(TeamID);
					pPlayer->SetCaptainID(LeaderId);
					pPlayer->SetTeammateAmount(lMemberCount);
				}
				GetGame()->GetMainPlayer()->GetTeamMateInfo().push_back(sMemberInfo);
				//if(pPlayer != GetGame()->GetMainPlayer())
				//{
				//	char strText[128];
				//	sprintf(strText,"%s%s",sMemberInfo.szName,AppFrame::GetText("TM_eIDS_TEAM_JOIN"));
				//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,strText,D3DCOLOR_ARGB(255,255,255,0));
				//}
			}
			td.UpdateTeamList();
			//if(GetGame()->GetCGuiEx()->GetMercenaryPageEx()->IsOpen())   //�ͽ�����򿪣��ȹر�
			//{
				//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->Close();
			//}
		}
		break;
	
	case MSG_S2C_TEAM_TeamData:				// ���ж������ݵ���Ա
		{
			CGUID TeamID;
			pMsg->GetGUID(TeamID);
			CGUID LeaderId;
			pMsg->GetGUID(LeaderId);
			long lGoodsAllot = pMsg->GetLong();
			long lMemberCount = pMsg->GetLong();
			GetGame()->GetMainPlayer()->GetTeamMateInfo().clear();
			for(long i=0; i<lMemberCount; ++i)
			{
				tagWSMemberInfo sMemberInfo;
				pMsg->GetEx(&sMemberInfo, sizeof(tagWSMemberInfo));
				CPlayer *pPlayer = (CPlayer *)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,sMemberInfo.guid);
				if(pPlayer)// �ö����ڳ����ڿɼ�
				{
					pPlayer->SetTeamID(TeamID);
					pPlayer->SetCaptainID(LeaderId);
					pPlayer->SetTeammateAmount(lMemberCount);
				}
				GetGame()->GetMainPlayer()->GetTeamMateInfo().push_back(sMemberInfo);
			}
			//GetGame()->GetCGuiEx()->GetTeamPageEx()->UpdataTeamList();
			td.UpdateTeamList();
			long lTeamQuestID = pMsg->GetLong();
			//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->SetTeamQuestId(lTeamQuestID);
		}
		break;
	case MSG_S2C_TEAM_MemberAllIdioinfo:// �����Ա����ϸ��Ϣ
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			// ������Ϣ������ʾ
			if(PlayerID==GetGame()->GetMainPlayer()->GetExID())
				return;
			long lFlag = pMsg->GetLong();
			tagMemberIdioinfo sMemberInfo;
			if(lFlag)
				pMsg->GetEx(&sMemberInfo, sizeof(tagMemberIdioinfo));
			td.SetMemberInfo(PlayerID,sMemberInfo);
		}
		break;
	case MSG_S2C_TEAM_UpdateIdioinfo:	// ����ĳ�����ѵ�ĳ������
		{
			//CTeamPageEx *pTeamPage = GetGame()->GetCGuiEx()->GetTeamPageEx();
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long lType = pMsg->GetLong();
			if(lType==eMIS_Pos)	// ����
			{
				float x=pMsg->GetFloat();
				float y=pMsg->GetFloat();
				td.UpdateMemberPos(PlayerID,x,y);
			}
			else	// ��������
			{
				long lValue = pMsg->GetLong();
				td.UpdateMemberInfo(PlayerID,lType,lValue);
			}
		}
		break;
		//	������Ҽ�������
	case MSG_S2C_TEAM_MemberData:
		{
			CPlayer *pPlayer = GetGame()->GetMainPlayer();
			tagWSMemberInfo sMemberInfo;
			pMsg->GetEx(&sMemberInfo, sizeof(tagWSMemberInfo));
			// ����
			if(pPlayer->GetExID()==sMemberInfo.guid)
				return;
			pPlayer->GetTeamMateInfo().push_back(sMemberInfo);
			pPlayer->SetTeammateAmount(pPlayer->GetTeammateAmount()+1);
			td.AddTeamMember(sMemberInfo);
			//char strText[128];
			//sprintf(strText,"%s%s",sMemberInfo.szName,AppFrame::GetText("TM_eIDS_TEAM_JOIN"));
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,strText,D3DCOLOR_ARGB(255,255,255,0));
		}
		break;
	case MSG_S2C_TEAM_MemberLeaveHere:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			td.MemberChangeServer(PlayerID);
		}
		break;
		// ������
	case MSG_S2C_TEAM_MemberLeave:
		{
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			char strText[128];
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			// �Լ��˶�
			if(PlayerID==pMainPlayer->GetExID())
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-31.wav");
				pMainPlayer->SetTeamID(CGUID::GUID_INVALID);
				pMainPlayer->SetCaptainID(CGUID::GUID_INVALID);
				pMainPlayer->GetTeam().clear();
				pMainPlayer->GetTeamMateInfo().clear();
				//GetGame()->GetInterface()->GetTeamPage()->Reset();
				td.Clear();
				sprintf(strText,AppFrame::GetText("TM_8"));
				pMainPlayer->SetTeammateAmount(1);
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,strText,D3DCOLOR_ARGB(255,255,255,0));
				//GetGame()->GetCGuiEx()->GetTeamPageEx()->UpdataTeamList();
				td.UpdateTeamList();
				//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
				//if(pObjPage && (pObjPage->GetObjType() == TYPE_PLAYER))
				//	pObjPage->SetObjectHpInfo();
                CShape* shape = GetGame()->GetGameControl()->GetSelectedShape();
                if ( shape && shape->GetType() == TYPE_PLAYER )
                {
                    FireUIEvent("ObjectPage", "ObjectAttr");
                }
				// �˵�����
				//CPopMenuPageEx *pPopPage = GetGame()->GetCGuiEx()->GetCPopMenuPageEx();
				//if(pPopPage && pPopPage->GetMenuType() >= CPopMenuPageEx::LEADER_MYSELF_MENU && 
				//				pPopPage->GetMenuType() <= CPopMenuPageEx::SELECT_PLAYER_MENU)
				//	pPopPage->Close();
				//��������ͽ�������棬�ر�
				/*if(GetGame()->GetCGuiEx()->GetMercenaryPageEx()->IsOpen())
				{
					GetGame()->GetCGuiEx()->GetMercenaryPageEx()->Close();
				}
				if(GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->IsOpen())
				{
					GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->Close();
				}
				GetGame()->GetCGuiEx()->GetMercenaryPageEx()->SetTeamQuestId(0);*/
			}
			else
			{
				vector<tagWSMemberInfo>::iterator it;
				for(it=pMainPlayer->GetTeamMateInfo().begin();it!=pMainPlayer->GetTeamMateInfo().end();it++)
				{
					if(it->guid==PlayerID)
					{
						sprintf(strText,AppFrame::GetText("TM_9"),it->szName);
						pMainPlayer->GetTeamMateInfo().erase(it);
						pMainPlayer->SetTeammateAmount(pMainPlayer->GetTeammateAmount()-1);
						CPlayer *pPlayer = (CPlayer *)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
						if(pPlayer)
						{
							pPlayer->SetTeamID(CGUID::GUID_INVALID);
							pPlayer->SetCaptainID(CGUID::GUID_INVALID);
							pPlayer->SetTeammateAmount(1);
						}
						//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,strText,D3DCOLOR_ARGB(255,255,255,0));
						//GetGame()->GetCGuiEx()->GetTeamPageEx()->RemoveTeamMember(PlayerID);
						td.RemoveTeamMember(PlayerID);
						break;
					}
				}
			}
			
		}
		break;
	case MSG_S2C_TEAM_MembereExStateChange:	// �����쳣״̬�ı�
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);// ���ID
			if(PlayerID == GetGame()->GetMainPlayer()->GetExID())
				return;
			DWORD dwFlag = pMsg->GetDWord();	// 0:��ʼ
			DWORD dwStateID = pMsg->GetDWord();	// ״̬ID
			DWORD dwLev = pMsg->GetDWord();		// ״̬�ȼ�
			//if(dwFlag==0)
				//GetGame()->GetCGuiEx()->GetTeamPageEx()->AddMemberState(PlayerID,dwStateID,dwLev);
			//else
				//GetGame()->GetCGuiEx()->GetTeamPageEx()->RemoveMemberState(PlayerID,dwStateID);
			if(dwFlag==0)
				td.AddMemberState(PlayerID,dwStateID,dwLev);
			else
				td.RemoveMemberState(PlayerID,dwStateID);

		}
		break;
	// ���뷽��������
	case MSG_S2C_TEAM_QUERY_JOIN:
		{
			CGUID PlayerID,IDSrc;
			// ������ʾ
			pMsg->GetGUID(PlayerID);// ����ID
			pMsg->GetGUID(IDSrc);// ���뷽ID
			//CGameConfigPageEx *pGamePage = GetGame()->GetCGuiEx()->GetGameConfigPageEx();
			CPlayer *pPlayer = GetGame()->GetMainPlayer();
			
            if ( pPlayer )
            {
                 TeamMessage::tagTeam*  pTeam = new TeamMessage::tagTeam;
                 pTeam->SourID = PlayerID;
                 pTeam->DestID = IDSrc;
                 char str[255];
                 pMsg->GetStr(str,255); 
                 strcat(str,AppFrame::GetText("TM_11"));
                 GetInst(MsgEventManager).PushEvent(Msg_YesNo,str,InviteeJoinTeam,pTeam);
            }
            return ;

			// ��ҹر����
			if ( pPlayer/* && pGamePage && !pGamePage->GetOption( CGameConfigPageEx::OPTION_AllowJoinTeam )*/)
			{
				CMessage msg( MSG_C2S_TEAM_ANSWER_JOIN );
				msg.Add(PlayerID);
				msg.Add(IDSrc);
				msg.Add((BYTE)1); // ��ʾ�Է���Ϸ����ͬ���� �������
				msg.Send(GetGame()->GetNetClient());
				bTeam = false;
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,AppFrame::GetText("TM_eIDS_TEAM_ON_OFF"),D3DCOLOR_ARGB(255,255,0,0));
				break;
			}

			GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-13.wav");
			CPlayer * pPlayerSrc = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,IDSrc));
			//if (pPlayerSrc==NULL)
			//	return;
			char name[255];
			string strName = pMsg->GetStr(name,255);

			//CTeamPageEx * pPage = GetGame()->GetCGuiEx()->GetTeamPageEx();
			//CMessageBoxPageEx * pMsgBox = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
			//if(!pPage || !pMsgBox)	return;
			//pPage->AddGUIDSrc(IDSrc);
			td.AddGUIDSrc(IDSrc);

			//char name[255];
            //strcpy(name,pPlayerSrc->GetName());
			strcat(name,AppFrame::GetText("TM_11"));        //������������飬�Ƿ�ͬ�⣿
			// ������Ϣ��ʾ������ɹ�����ӡ��ʾ��Ϣ
			//if(pMsgBox->Open(pPage,name,CMessageBoxPageEx::MSG_OK_NO,false))
			//{
			//	sprintf(name,AppFrame::GetText("TM_eIDS_TEAM_BEGJOIN"),strName.c_str());
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,name,D3DCOLOR_ARGB(255,255,255,0));	
			//}
			//pMsg->Reset();		

		}
		break;
	
	//case MSG_S2C_TEAM_ANSWER_JOIN:
	//	{
	//		CGUID IDDst,IDScr;
	//		pMsg->GetGUID(IDDst);
	//		pMsg->GetGUID(IDScr); // ���뷽ID
	//		CPlayer * pPlayerDst = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,IDDst));

	//		if( !pPlayerDst ) return;

	//		char name[255];
	//		strcpy(name,pPlayerDst->GetName());
	//		BYTE b = pMsg->GetByte();
	//		
	//		// bΪ�ظ�
	//		if ( b == 1 )
	//		{
	//			GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-14.wav");
	//			strcat(name,AppFrame::GetText("TM_eIDS_TEAM_JOIN"));
	//		}
	//		else if ( b == 2 )
	//		{
	//			strcat(name,AppFrame::GetText("TM_eIDS_TEAM_OBJNOTJOIN"));
	//		}
	//		else
	//		{
	//			strcat(name,AppFrame::GetText("TM_eIDS_TEAM_NOJOIN"));
	//		}
	//		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,name,D3DCOLOR_ARGB(255,255,255,0));
	//	}
	//	break;
		//! �Ƽ����֪ͨ
	case MSG_S2C_TEAM_Recommend_ByName:
		{
			// ���Ƽ�������
			char strDName[256] = "";
			pMsg->GetStr(strDName,256);
			// �Ƽ�������
			char strSName[256] = "";
			pMsg->GetStr(strSName,256);
			char str[256]="";
			sprintf_s(str,AppFrame::GetText("TM_23"),strSName,strDName);
            TeamMessage::tagRecommend*  recommend = new TeamMessage::tagRecommend;
            recommend->name = strDName;

            GetInst(MsgEventManager).PushEvent(Msg_YesNo,str,RecommendJoinTeam,recommend);
		}
		break;
	// ������Ƽ���
	case MSG_S2C_TEAM_SKILL_ABOUT:
		{
			//long lLev = pMsg->GetLong();
			//long lID = pMsg->GetLong();
			//GetGame()->GetMainPlayer()->SetLimtSkill(lID,lLev);
		}
		break;
	//---------------------------------------------------

	//case MSG_S2C_TEAM_SYNCHRONIZE:
	//	{
	//		for(int i=0;i<4;i++)
	//			pMsg->GetLong();
	//		CGUID TeamID,LeaderID;
	//		pMsg->GetGUID(TeamID);
	//		char strTeamname[64];
	//		char strPassword[64];
	//		pMsg->GetStr(strTeamname,23);
	//		pMsg->GetStr(strPassword,9);
	//		pMsg->GetGUID(LeaderID);
	//		DWORD dwAmount=pMsg->GetDWord();
	//		GetGame()->GetMainPlayer()->GetTeam().clear();
	//		GetGame()->GetMainPlayer()->SetTeamID(TeamID);
	//		GetGame()->GetMainPlayer()->SetCaptainID(LeaderID);
	//		//if(lLeaderID!=GetGame()->GetMainPlayer()->GetID())
	//		//	GetGame()->GetMainPlayer()->SetTeamHasPassword(strlen(strPassword)>0?true:false);
	//		//if(lLeaderID!=GetGame()->GetMainPlayer()->GetID())
	//		//	GetGame()->GetMainPlayer()->SetTeamname(strTeamname);
	//		//GetGame()->GetInterface()->GetTeamPage()->Reset();
	//		for(int i=0;i<dwAmount;i++)
	//		{
	//			//
	//			pMsg->GetLong();
	//			long lType=pMsg->GetLong();
	//			CGUID ID;
	//			pMsg->GetGUID(ID);
	//			pMsg->GetLong();
	//			//
	//			CGUID RegionID;
	//			pMsg->GetGUID(RegionID);
	//			char strTeammate[32];
	//			pMsg->GetStr(strTeammate,17);
	//			//
	//			CPlayer::tagTeammate sTeam;
	//			sTeam.ID=ID;
	//			sTeam.RegionID=RegionID;
	//			sTeam.strName=strTeammate;
	//			GetGame()->GetMainPlayer()->GetTeam().push_back(sTeam);
	//		}
	//	}
	//	break;

	//case MSG_S2C_TEAM_TEAMATE_JOIN:
	//	{
	//		CGUID teamid;
	//		pMsg->GetGUID(teamid);
	//		long lType=pMsg->GetLong();
	//		CGUID playerid;
	//		pMsg->GetGUID(playerid);
	//		CGUID regionid;
	//		pMsg->GetGUID(regionid);
	//		float fHPPercent=pMsg->GetFloat();
	//		char strName[32];
	//		pMsg->GetStr(strName,17);
	//		vector<CPlayer::tagTeammate>::iterator it;
	//		for(it=GetGame()->GetMainPlayer()->GetTeam().begin();it!=GetGame()->GetMainPlayer()->GetTeam().end();it++)
	//		{
	//			if(it->ID==playerid)
	//			{
	//				return;
	//			}
	//		}
	//		CPlayer::tagTeammate sTeam;
	//		sTeam.ID=playerid;
	//		sTeam.wHP=(WORD)(fHPPercent*100);
	//		sTeam.wMaxHP=100;
	//		sTeam.strName=strName;
	//		sTeam.RegionID=regionid;
	//		GetGame()->GetMainPlayer()->SetTeamID(teamid);
	//		GetGame()->GetMainPlayer()->GetTeam().push_back(sTeam);
	//		char strText[128];
	//		sprintf(strText,"%s%s",strName,AppFrame::GetText("TM_eIDS_TEAM_JOIN"));
	//	}
	//	break;
	//	
	//case MSG_S2C_TEAM_CHANGE_STATE:
	//	{
	//		CGUID TeamID;
	//		pMsg->GetGUID(TeamID);
	//		long lType=pMsg->GetLong();
	//		CGUID PlayerID;
	//		pMsg->GetGUID(PlayerID);
	//		float fHp=pMsg->GetFloat();
	//		vector<CPlayer::tagTeammate>::iterator it;
	//		for(it=GetGame()->GetMainPlayer()->GetTeam().begin();it!=GetGame()->GetMainPlayer()->GetTeam().end();it++)
	//		{
	//			if(it->ID==PlayerID)
	//			{
	//				it->wHP=(WORD)(100*fHp);
	//				it->wMaxHP=100;
	//				break;
	//			}
	//		}
	//	}
	//	break;

	//case MSG_S2C_TEAM_TEAMATE_CHANGE_REGION:
	//	{
	//		CGUID TeamID,PlayerID,RegionID;
	//		pMsg->GetGUID(TeamID);
	//		long lType=pMsg->GetLong();
	//		pMsg->GetGUID(PlayerID);
	//		pMsg->GetGUID(RegionID);
	//		vector<CPlayer::tagTeammate>::iterator it;
	//		for(it=GetGame()->GetMainPlayer()->GetTeam().begin();it!=GetGame()->GetMainPlayer()->GetTeam().end();it++)
	//		{
	//			if(it->ID==PlayerID)
	//			{
	//				it->RegionID=RegionID;
	//				break;
	//			}
	//		}
	//	}
	//	break;

	case MSG_S2C_TEAM_CHAT:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			char strText[512]={0};
			pMsg->GetStr(strText,511);
			//=========================�Ի���Ϣ�е�������Ϣ=======================
			long lAdjustLen = 0;
			//CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//list<CEditField::stSpecialTextInfo> * pTextInfoList = pMainPage->GetSpecialText();
			//pTextInfoList->clear();
			char strGoodsName[1024] = "";
			long lNum = pMsg->GetLong();
			for(long i=0; i<lNum; ++i)
			{
				//CEditField::stSpecialTextInfo stInfo;
				//pMsg->GetEx(&stInfo,256/* sizeof(CEditField::stSpecialTextInfo)*/);
				//stInfo.str = pMsg->GetStr(strGoodsName,1024);
				//pTextInfoList->push_back(stInfo);
			}
			//====================================================================
			CPlayer *pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer)
			{
				tagWSMemberInfo* pstWSMemberInfo = pPlayer->GetWsMemberInfo( PlayerID );
				if( pstWSMemberInfo )
				{
					string str= pstWSMemberInfo->szName;
					str+=":";
					lAdjustLen = (long)str.length();
					str+=strText;
					//pMainPage->AdjGoodsInfoPos(lAdjustLen);
					/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords((char *)str.c_str(),
						CGlobeSetup::GetSetup()->colorChatText[CHAT_TEAM],
						CGlobeSetup::GetSetup()->colorChatTextBack[CHAT_TEAM],
						CGlobeSetup::GetSetup()->colorChatTextShadow[CHAT_TEAM],
						pstWSMemberInfo->szName,
						CHAT_TEAM);*/
					break;
				}
			}
		}
		break;
	/*case MSG_S2C_TEAM_CHANGE_LEAER:
		{
			CGUID TeamID,LeaderID;
			pMsg->GetGUID(TeamID);
			pMsg->GetGUID(LeaderID);
			GetGame()->GetMainPlayer()->SetCaptainID(LeaderID);
			vector<CPlayer::tagTeammate>::iterator it;
			for(it=GetGame()->GetMainPlayer()->GetTeam().begin();it!=GetGame()->GetMainPlayer()->GetTeam().end();it++)
			{
				if(it->ID==LeaderID)
				{
					char strText[128];
					sprintf(strText,AppFrame::GetText("TM_eIDS_TEAM_CAPMEMBER"),it->strName.c_str());
					if(GetGame()->GetMainPlayer()->GetTeam().begin()!=it)
					{
						CPlayer::tagTeammate sTeam=(*it);
						GetGame()->GetMainPlayer()->GetTeam().erase(it);
						GetGame()->GetMainPlayer()->GetTeam().insert(GetGame()->GetMainPlayer()->GetTeam().begin(),sTeam);
					}
					break;
				}
			}
		}
		break;

	case MSG_S2C_TEAM_CHANGE_ALLOCATION_SCHEME:
		{
			long lTeamID=pMsg->GetLong();
			long lAs = pMsg->GetLong();
		}
		break;*/

	}
}