//OrganizingsystemMessage.cpp
#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\player.h"
#include "..\serverregion.h"
#include "..\..\..\Setup\QuestSystem.h"
#include "..\script\script.h"
#include "..\script\VariableList.h"


// [2007-7-6-add]
#include "../OrganizingSystem/GameOrganizingCtrl.h"
#include "../OrganizingSystem/GameFaction.h"
#include "../OrganizingSystem/GameUnion.h"
// [2007-7-6-add end]
#include "../RgnManager.h"
#include "../../GameServer/GameManager.h"
#include "../Organizingsystem/GSTeam.h"


void OnOrgasysMessage(CMessage* pMsg)
{
	if(0 == CMessage::GetWSSocket())
	{
		CMessage msg( MSG_S2C_OTHER_ADDINFO );
		msg.Add( (DWORD)0xffff0000 );
		msg.Add( (DWORD)0 );
		msg.Add((BYTE)eNOTIFYPOS_CENTER);
		msg.Add( CStringReading::LoadString(209, 70) );
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	switch(pMsg->GetType())
	{
		//! 2008 04 16 ���
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_W2S_ORGSYS_InitGsFaction: //! ��ʼ��GS����ϵͳ
		{
			GetOrganiCtrl()->OnInitGsFaction(pMsg);
		}
		break;

	case MSG_W2S_ORGSYS_CreateFaction_Re: //! �ظ����ᴴ��
		{
			GetOrganiCtrl()->OnCreateFactionRe(pMsg);
		}
		break;
	case MSG_W2S_ORGSYS_InitPlayerFacSys:
		{
		}
		break;
	case MSG_W2S_ORGSYS_DelFaction_Notify:
		{
			GetOrganiCtrl()->OnWsDelFaction(pMsg);
		}
		break;
	case MSG_W2S_Faction_InvitePlayer_Re:
		{
			GetOrganiCtrl()->OnWsInvitePlayerRe(pMsg);
		}
		break;
	case MSG_W2S_Faction_AddMember_Notify:
		{
			GetOrganiCtrl()->OnWsAddMember(pMsg);
		}
		break;
	case MSG_W2S_ORGSYS_SetJobName_Notify:
		{
			GetOrganiCtrl()->OnWsSetJobName(pMsg);
		}
		break;
	case MSG_W2S_ORGSYS_SetJobPurview_Notify:
		{
			GetOrganiCtrl()->OnWsSetJobPurview(pMsg);
		}
		break;
	case MSG_W2S_ORGSYS_Quit_Notify:
		{
			GetOrganiCtrl()->OnWsMemberQuit(pMsg);
		}
		break;
	case MSG_W2S_Faction_EditPronounce_Notify:
		{
			GetOrganiCtrl()->OnWsEditPronounce(pMsg);
		}
		break;
	case MSG_W2S_Faction_EditLeaveword_Notify:
		{
			GetOrganiCtrl()->OnWsEditLeaveword(pMsg);
		}
		break;
	case MSG_W2S_ORGSYS_Fire_Notify:
		{
			GetOrganiCtrl()->OnWsFire(pMsg);
		}
		break;
	case MSG_W2S_ORGSYS_SetMemJobLvl_Notify:
		{
			GetOrganiCtrl()->OnWsSetMemJobLvl(pMsg);
		}
		break;
	case MSG_W2S_Faction_SetMemberTitle_Notify:
		{
			GetOrganiCtrl()->OnWsSetMemberTitle(pMsg);
		}
		break;
	case MSG_W2S_Faction_MemberMap_Notify:
		{
			GetOrganiCtrl()->OnWsMemberMap(pMsg);
		}
		break;
	case MSG_W2S_Faction_MemberLevel_Notify:
		{
			GetOrganiCtrl()->OnWsMemberLevel(pMsg);
		}
		break;
	case MSG_W2S_Faction_MemberHide_Notify:
		{
			GetOrganiCtrl()->OnWsMemberHide(pMsg);
		}
		break;
	case MSG_W2S_Faction_MemberOccu_Notify:
		{
			GetOrganiCtrl()->OnWsMemberOccu(pMsg);
		}
		break;
	case MSG_W2S_Faction_SetMaster_Notify:
		{
			GetOrganiCtrl()->OnWsSetMaster(pMsg);
		}
		break;
	case MSG_W2S_Faction_OnlineChange_Notify:					//! ��Ա����״̬�ı�
		{
			GetOrganiCtrl()->OnWsMemOnlineChange(pMsg);
		}
		break;
	case MSG_W2S_Faction_Applicant_Notify:
		{
			GetOrganiCtrl()->OnWsApplicantChange(pMsg);
		}
		break;
	case MSG_W2S_Faction_Join_Re:
		{
			pMsg->GetLong();
			pMsg->GetLong();
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			pMsg->SetType(MSG_S2C_ORGSYS_JoinFac_Re);
			pMsg->SendToPlayer(PlayerGuid);
		}
		break;
	case MSG_W2S_Faction_LvlChange_Notify: //! ����ȼ��ı�֪ͨ
		{
			GetOrganiCtrl()->OnWsFacLvlChange(pMsg);
		}
		break;
	case MSG_W2S_Faction_IconChange_Notify:
		{
			GetOrganiCtrl()->OnWsIconChange(pMsg);
		}
		break;
	case MSG_S2W_Faction_ChangeRes_Reques:					//! �޸���Դ����
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if(NULL != pFaction)
			{
				LONG lDeltaRes = pMsg->GetLong();//! ���ֵGS��ʹ��
				LONG lCurrRes = pMsg->GetLong();
				pFaction->SetRes(lCurrRes);
			}	
		}
		break;
	case MSG_S2W_Faction_ChangeExp_Reques:					//! �޸ľ�������
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if(NULL != pFaction)
			{
				LONG lDeltaExp = pMsg->GetLong();//! ���ֵGS��ʹ��
				LONG lCurrExp = pMsg->GetLong();
				pFaction->SetExp(lCurrExp);
			}
		}
		break;
	case MSG_S2W_Faction_ChangeMemberContribute_Reques:		//! �޸ĳ�Ա��������
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if(NULL != pFaction)
			{
				CGUID MemberGuid;
				pMsg->GetGUID(MemberGuid);
				LONG lDeltaContribute = pMsg->GetLong();//! ���ֵGS��ʹ��
				LONG lCurrContribute = pMsg->GetLong();
				pFaction->SetMemberContribute(MemberGuid, lCurrContribute);
			}
		}
		break;
	case MSG_W2S_Faction_ChangeRes_Re:						//! �޸���Դ�ظ�
	case MSG_W2S_Faction_ChangeExp_Re:						//! �޸ľ���ظ�
	case MSG_W2S_Faction_ChangeMemberContribute_Re:			//! �޸ĳ�Ա���׻ظ�
		{
			CGUID ScriptGuid;
			pMsg->GetGUID(ScriptGuid);
			char szRet[128] = {0};
			pMsg->GetStr(szRet, 128);
			LONG lRet = pMsg->GetLong();

			//ȡ������Ľű�
			CVariableList* stackVarList = CScript::GetStackVarListInst();
			if(stackVarList)
			{
				stackVarList->Release();
				stackVarList->AddVar(szRet, lRet);
			}
			ContinueRunScript(ScriptGuid, stackVarList);
		}
		break;

	case MSG_W2S_Faction_ChangeRes_Notify:					//! �޸���Դ֪ͨ
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (NULL == pFaction) return;

			LONG lDeltaValue = pMsg->GetLong();
			LONG lCurrValue = pMsg->GetLong();
			pFaction->SetRes(lCurrValue);

			CMessage msg(MSG_S2C_Faction_ChangeRes_Notify);
			msg.Add(lCurrValue);
			pFaction->SendToAllConcern(msg);

		}
		break;
	case MSG_W2S_Faction_ChangeExp_Notify:					//! �޸ľ���֪ͨ
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (NULL == pFaction) return;

			LONG lDeltaValue = pMsg->GetLong();
			LONG lCurrValue = pMsg->GetLong();
			pFaction->SetExp(lCurrValue);

			CMessage msg(MSG_S2C_Faction_ChangeExp_Notify);
			msg.Add(lCurrValue);
			pFaction->SendToAllConcern(msg);
		}
		break;
	case MSG_W2S_Faction_ChangeMemberContribute_Notify:		//! �޸ĳ�Ա����֪ͨ
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (NULL == pFaction) return;

			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			LONG lDeltaValue = pMsg->GetLong();
			LONG lCurrValue = pMsg->GetLong();
			pFaction->SetMemberContribute(PlayerGuid, lCurrValue);

			CMessage msg(MSG_S2C_Faction_ChangeMemberContribute_Notify);
			msg.Add(PlayerGuid);
			msg.Add(lCurrValue);
			pFaction->SendToAllConcern(msg);
		}
		break;
	case MSG_W2S_Faction_BoonRun_Notify:
		{
			GetOrganiCtrl()->OnBoonRun();
		}
		break;

	//! ------------------------------------------------------------------------------------------------------
	case MSG_C2S_ORGSYS_CreateFaction_Reques:
		{
			GetOrganiCtrl()->OnPlayerCreateFactionReques(pMsg);
		}
		break;
	case MSG_C2S_ORGSYS_DisbandFaction_Request:
		{
			GetOrganiCtrl()->OnPlayerDisbandFactionReques(pMsg);
		}
		break;
	case MSG_C2S_ORGSYS_Quit_Request: //! �˳��л�����
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (NULL != pPlayer)
			{
				GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
				if (NULL != pFaction)
				{
                    //��鳡�������
                    if (GetOrganiCtrl()->TestRegionForbidMember(pPlayer))
                    {
                        //������
                     /*   CMessage msg(MSG_S2C_ORGSYS_Quit_Re);
                        msg.Add(PLAYER_OPT_FAILED);
                        msg.Add((LONG)eFOEI_Err_RegionForbid);
                        msg.SendToPlayer(pPlayer->GetExID(), false);*/
                        return;
                    }

					if (pFaction->GetMaster() != pPlayer->GetExID())
					{
						CMessage msg(MSG_S2W_ORGSYS_Quit_Request);
						msg.Add(pFaction->GetExID());
						msg.Add(pPlayer->GetExID());
						msg.Send();
					}
				}
			}
		}
		break;
	case MSG_C2S_ORGSYS_Kick_Request: //! �߳��л�����
		{
			GetOrganiCtrl()->OnPlayerKickReques(pMsg);
		}
		break;
	case MSG_C2S_Faction_UpLoadIcon_Request: //! �ϴ�����ͼ������
		{
			GetOrganiCtrl()->OnPlayerUpLoadIconReques(pMsg);
		}
		break;
	case MSG_C2S_FacData_Request: //! �л������������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (NULL != pPlayer)
			{
				GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
				if (NULL != pFaction)
				{
					pFaction->AddAttention(pPlayer->GetExID());

					//! public����
					CMessage msg(MSG_S2C_Faction_PublicData_Notify);
					pFaction->AddPublicData_ToMsgBack(&msg);
					msg.SendToPlayer(pPlayer->GetExID());
					//! base����
					CMessage msgB(MSG_S2C_Faction_BaseData_Notify);
					pFaction->AddBaseData_ToMsgBack(&msgB);
					msgB.SendToPlayer(pPlayer->GetExID());	
					//! ��Ա����
					CMessage msgM(MSG_S2C_Faction_Member_Notify);
					pFaction->AddMember_ToMsgBack(&msgM, pPlayer->GetExID());
					msgM.SendToPlayer(pPlayer->GetExID());
					//! ����������
					CMessage msgA(MSG_S2C_Faction_Apply_Notify);
					pFaction->AddApply_ToMsgBack(&msgA);
					msgA.SendToPlayer(pPlayer->GetExID());
					//! ����
					CMessage msgP(MSG_S2C_Faction_Placard_Notify);
					pFaction->AddPronounce_ToMsgBack(&msgP);
					msgP.SendToPlayer(pPlayer->GetExID());
					//! ����
					CMessage msgL(MSG_S2C_Faction_Leaveword_Notify);
					pFaction->AddLeaveWord_ToMsgBack(&msgL);
					msgL.SendToPlayer(pPlayer->GetExID());
					//! �ݵ�ս��Ʊ��¼
					CMessage msgTickets(MSG_S2C_Faction_Beachheads_Notify);
					pFaction->AddBaseInfoToMsg(&msgTickets);
					msgTickets.SendToPlayer(pPlayer->GetExID());
					//! �жԼ�����Ϣ
					CMessage msgEnemy(MSG_S2C_Faction_ChangeEnemy_Notify);
					pFaction->AddEnemyInfoToMsg(&msgEnemy);
					msgEnemy.SendToPlayer(pPlayer->GetExID());
				}
			}
		}
		break;
	case MSG_C2S_Faction_AllIconData_Request:
		{
		}
		break;
	case MSG_C2S_Faction_IconData_Request:
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (NULL != pFaction)
			{
				if(0 != pFaction->GetIconCRC32())
				{
					vector<BYTE> vIconData;
					pFaction->GetIcon(vIconData);
					CMessage msgRadiate(MSG_S2C_Faction_IconData_Notify);
					msgRadiate.Add(FacGuid);
					msgRadiate.Add((LONG)vIconData.size());
					msgRadiate.AddEx(&vIconData[0], vIconData.size());
					msgRadiate.SendToSocket(pMsg->GetSocketID());
				}	
			}
		}
		break;
	case MSG_C2S_ORGSYS_SetPlacard_Request: //! �޸Ĺ�������
		{
			GetOrganiCtrl()->OnPlayerFacSetPlacardRequest(pMsg);
		}
		break;
	case MSG_C2S_ORGSYS_SetLeaveword_Request: //! �޸ı�������
		{
			GetOrganiCtrl()->OnPlayerFacSetLeavewordRequest(pMsg);
		}
		break;
	case MSG_C2S_ORGSYS_CloseFacBoard_Notifye: //! ��ҹرչ������֪ͨ
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (NULL != pPlayer)
			{
				GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
				if (NULL != pFaction)
					pFaction->DelAttention(pPlayer->GetExID());
			}
		}
		break;
	case MSG_C2S_ORGSYS_SetJobName_Request: //! �޸Ĺٽ���������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (NULL != pPlayer)
			{
				GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
				if (NULL != pFaction)
				{
					if (pPlayer->GetExID() == pFaction->GetMaster())
					{
						LONG lJobLvl = pMsg->GetLong();
						if (0 < lJobLvl && MAXNUM_JobLvl >= lJobLvl)
						{
							char szNewName[MAX_MEMBER_NAME_SIZE] = {0};
							pMsg->GetStr(szNewName, MAX_MEMBER_NAME_SIZE);
							CMessage msg(MSG_S2W_ORGSYS_SetJobName_Request);
							msg.Add(pFaction->GetExID());
							msg.Add(lJobLvl);
							msg.Add(szNewName);
							msg.Send();
						}
					}
				}
			}
		}
		break;
	case MSG_C2S_ORGSYS_SetJobPurview_Request: //! �޸Ĺٽ�Ȩ������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (NULL != pPlayer)
			{
				GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
				if (NULL != pFaction)
				{
					if (pPlayer->GetExID() == pFaction->GetMaster())
					{
						LONG lJobLvl = pMsg->GetLong();
						if (0 < lJobLvl && MAXNUM_JobLvl >= lJobLvl)
						{
							LONG lPurview = pMsg->GetLong();
							if(0 != ((~eMP_MemberTiptop) & lPurview))
								return;
							CMessage msg(MSG_S2W_ORGSYS_SetJobPurview_Request);
							msg.Add(pFaction->GetExID());
							msg.Add(lJobLvl);
							msg.Add(lPurview);
							msg.Send();
						}
					}
				}
			}
		}
		break;
	case MSG_C2S_ORGSYS_SetMemberJob_Request: //! ���ó�Ա�ٽ�����
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if (NULL != pPlayer)
			{
				GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
				if (NULL != pFaction)
				{
					CGUID MemberGuid;
					pMsg->GetGUID(MemberGuid);

					if (pPlayer->GetExID() == pFaction->GetMaster() && pPlayer->GetExID() != MemberGuid)
					{
						if (NULL_GUID != pFaction->IsMember(MemberGuid))
						{
							LONG lJobLvl = pMsg->GetLong();
							if (0 <= lJobLvl && MAXNUM_JobLvl >= lJobLvl)
							{
								CMessage msg(MSG_S2W_ORGSYS_SetMemberJob_Request);
								msg.Add(pFaction->GetExID());
								msg.Add(MemberGuid);
								msg.Add(lJobLvl);
								msg.Send();
							}
						}
					}
				}
			}
		}
		break;
	case MSG_C2S_Faction_InvitePlayer_Request: //! ������Ҽ�������
		{
			GetOrganiCtrl()->OnPlayerFacInviteReques(pMsg);
		}
		break;
	case MSG_C2S_Faction_InvitePlayer_Answer: //! �ش�����
		{
			GetOrganiCtrl()->OnPlayerFacInviteAnswer(pMsg);
		}
		break;
	case MSG_C2S_Faction_SetMemberTitle_Request: //! ���ó�Ա�ƺ�����
		{
			GetOrganiCtrl()->OnPlayerFacSetMemberTitleReques(pMsg);
		}
		break;
	case MSG_C2S_Faction_SetChairman_Request: //! ��λ����
		{
			GetOrganiCtrl()->OnPlayerFacSetChairmanReques(pMsg);
		}
		break;
	case MSG_C2S_ORGSYS_CheckRequisition_Request:
		{
			GetOrganiCtrl()->OnPlayerFacCheckRequisition(pMsg);
		}
		break;
	case MSG_C2S_ORGSYS_JoinFac_Request:
		{
			GetOrganiCtrl()->OnPlayerJoinFac(pMsg);
		}
		break;
		//! ����
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_W2S_Union_InitGs:
		{
			LONG lDataSize = pMsg->GetLong();
			vector<BYTE> vData(lDataSize + 1);
			pMsg->GetEx(&vData[0], lDataSize);

			GetOrganiCtrl()->OnInitGsUnion(vData);
		}
		break;
	case MSG_C2S_UNION_Request_Creation:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL == pPlayer) return;

			char szNewUnionName[MAXSIZE_FactionName] = {0};
			pMsg->GetStr(szNewUnionName, MAXSIZE_FactionName);

			GetOrganiCtrl()->OnPlayerCreateUnion(pPlayer, szNewUnionName);
		}
		break;
	case MSG_W2S_Union_Create_Notify:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			LONG lResult = pMsg->GetLong();
			if(PLAYER_OPT_FAILED == lResult)
			{
				CMessage msgRe(MSG_S2C_UNION_Re_Creation);
				msgRe.Add(PLAYER_OPT_FAILED);
				msgRe.Add(pMsg->GetLong());
				msgRe.SendToPlayer(PlayerGuid, false);
				return;
			}

			LONG lDataSize = pMsg->GetLong();
			vector<BYTE> vData(lDataSize + 1);
			pMsg->GetEx(&vData[0], lDataSize);

			GetOrganiCtrl()->OnCreateNewUnion(vData);
		}
		break;
	case MSG_C2S_UNION_Request_Disband:					//! ��ɢ��������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
				GetOrganiCtrl()->OnPlayerDisbandUnion(pPlayer);
		}
		break;
	case MSG_W2S_Union_Disband_Notify:
		{
			CGUID UnionGuid;
			pMsg->GetGUID(UnionGuid);
			
			GetOrganiCtrl()->OnWsDisbandUnion(UnionGuid);
		}
		break;
	case MSG_C2S_UNION_Request_Quit:					//! �л��˳���������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
				GetOrganiCtrl()->OnPlayerQuitUnion(pPlayer);
		}
		break;
	case MSG_W2S_Union_Quit_Notify:
		{
			CGUID FactionGuid;
			pMsg->GetGUID(FactionGuid);
			CGUID UnionGuid;
			pMsg->GetGUID(UnionGuid);

			GetOrganiCtrl()->OnWsQuitUnion(FactionGuid, UnionGuid);
		}
		break;
	case MSG_C2S_UNION_Request_Kick:					//! ���л��߳���������
		{
			CGUID AimFactionGuid;
			pMsg->GetGUID(AimFactionGuid);
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
				GetOrganiCtrl()->OnPlayerKick(pPlayer, AimFactionGuid);
		}
		break;
	case MSG_W2S_Union_Kick_Notify:
		{
			CGUID OptGuid;
			pMsg->GetGUID(OptGuid);
			CGUID AimGuid;
			pMsg->GetGUID(AimGuid);
			CGUID UnionGuid;
			pMsg->GetGUID(UnionGuid);

			GetOrganiCtrl()->OnWsKickUnion(OptGuid, AimGuid, UnionGuid);
		}
		break;
	case MSG_C2S_UNION_Request_SetChairman:				//! ����ת������
		{
			CGUID AimFactionGuid;
			pMsg->GetGUID(AimFactionGuid);
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
				GetOrganiCtrl()->OnPlayerSetChairman(pPlayer, AimFactionGuid);
		}
		break;
	case MSG_S2W_Union_SetChairman_Notify:
		{
			CGUID UnionGuid;
			pMsg->GetGUID(UnionGuid);
			CGUID AimFacGuid;
			pMsg->GetGUID(AimFacGuid);
			char szNewMasterName[MAX_MEMBER_NAME_SIZE] = {0};
			pMsg->GetStr(szNewMasterName, MAX_MEMBER_NAME_SIZE);

			GetOrganiCtrl()->OnWsSetChairman(UnionGuid, AimFacGuid, szNewMasterName);
		}
		break;
	case MSG_C2S_UNION_Request_SetRights:				//! ���������ԱȨ������
		{
			CGUID AimFactionGuid;
			pMsg->GetGUID(AimFactionGuid);
			DWORD dwPurview = pMsg->GetDWord();
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
				GetOrganiCtrl()->OnPlayerSetPurview(pPlayer, AimFactionGuid, dwPurview);
		}
		break;
	case MSG_S2W_Union_SetPurview_Notify:
		{
			CGUID UnionGuid;
			pMsg->GetGUID(UnionGuid);
			CGUID AimFacGuid;
			pMsg->GetGUID(AimFacGuid);
			DWORD dwPurview = pMsg->GetDWord();

			GetOrganiCtrl()->OnWsSetPurviewUnion(UnionGuid, AimFacGuid, dwPurview);
		}
		break;
	case MSG_C2S_UNION_Request_Invite:					//! �����л�᳤ �������˵ķ�������
		{
			char szAimPlayerName[MAX_MEMBER_NAME_SIZE];
			pMsg->GetStr(szAimPlayerName, MAX_MEMBER_NAME_SIZE);
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
				GetOrganiCtrl()->OnPlayerUnionInvite(pPlayer, szAimPlayerName);
		}
		break;
	case MSG_S2W_Union_Invit_Re:
		{
			//! ����ǰ�����Ϣ����...
			if(PLAYER_OPT_SUCCEED == pMsg->GetLong())
			{
				char szTmp[MAX_MEMBER_NAME_SIZE] = {0};
				pMsg->GetStr(szTmp, MAX_MEMBER_NAME_SIZE);
			}
			else
				pMsg->GetLong();
			//! ...�ҳ���Ϣ������
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			//! ת����Ϣ
			pMsg->SetType(MSG_S2C_UNION_Response_Invite);
			pMsg->SendToPlayer(PlayerGuid, false);
		}
		break;
	case MSG_S2W_Union_TransferInvit:
		{
			CGUID PlayerGuid;
			char szTemp[MAX_MEMBER_NAME_SIZE];
			//! ����ǰ�����Ϣ����...
			pMsg->GetGUID(PlayerGuid);
			pMsg->GetStr(szTemp, MAX_MEMBER_NAME_SIZE);
			pMsg->GetGUID(PlayerGuid);
			pMsg->GetStr(szTemp, MAX_MEMBER_NAME_SIZE);
			//! ...�ҳ���Ϣ������
			pMsg->GetGUID(PlayerGuid);

			//! ת����Ϣ
			pMsg->SetType(MSG_S2C_UNION_Notify_Anwser_Invite);
			pMsg->SendToPlayer(PlayerGuid, false);
		}
		break;
	case MSG_C2S_UNION_Request_Anwser_Invite:			//! �л�᳤ ���ͻظ�����������
		{
			pMsg->SetType(MSG_S2W_UNION_Request_AnswerInvit);
			pMsg->Send(false);
		}
		break;

	case MSG_C2S_FACTION_Upgrade_Request:				//! ������������
		{
			DWORD dwUpgradeType = pMsg->GetLong();
			GetOrganiCtrl()->OnPlayerUpgradeFac(pMsg->GetPlayer(), dwUpgradeType);
		}
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_W2S_OTHER_FACTIONTALK:
		{

		}
	//! [add end]
	case MSG_W2S_QUEST_PlayerAddQuest:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			DWORD dwQuestID  = pMsg->GetDWord();
			CPlayer* pPlayer = GetGame()->FindPlayer(PlayerID);
			if(pPlayer)
			{
				pPlayer->AddQuest(dwQuestID);
			}
		}
		break;
	case MSG_W2S_QUEST_PlayerRemoveQuest:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			DWORD dwQuestID  = pMsg->GetDWord();
			CPlayer* pPlayer = GetGame()->FindPlayer(PlayerID);
			if(pPlayer)
			{
				pPlayer->RemoveQuest(dwQuestID);
			}
		}
		break;
	case MSG_W2S_QUEST_PlayerRunScript:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			char str[256]={0};
			pMsg->GetStr(str,256);
			CPlayer* pPlayer = GetGame()->FindPlayer(PlayerID);
			if(pPlayer && pPlayer->IsDied() == false)
			{
				pPlayer->PlayerRunScript(str);
			}
		}
		break;
		//�������
	case MSG_C2S_QUEST_Complete:
		{
			if(pMsg->GetPlayer() == NULL)	break;

			DWORD wQuestID   = pMsg->GetDWord();//>GetWord();
			CPlayer* pPlayer = pMsg->GetPlayer();
			if(pPlayer /*&& pPlayer->GetQuestState(wQuestID) == eQS_NotComplete*/)
			{
				const CQuest* pQuest = CQuestSystem::GetQuest((LONG)wQuestID);
				if(NULL == pQuest)	break;
				const char* strScript =CQuestSystem::GetCompleteQuestScript();
				if(strScript == NULL)	break;

				stRunScript sScript;
				sScript.pszFileName = (char*)strScript;
				sScript.desShape = NULL;
				sScript.pRegion =  pMsg->GetRegion();
				sScript.srcShape	=pPlayer ;

				RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ), wQuestID ,0);
			}
		}
		break;
		//��������
	case MSG_C2S_QUEST_Accept:
		{
			if (pMsg->GetPlayer()==NULL)
				break;
			CPlayer* pPlayer = pMsg->GetPlayer();
			DWORD wQuestID   = pMsg->GetDWord();
			BYTE bMercQuest = pMsg->GetByte();
			BYTE bFlag = pMsg->GetByte();
			BYTE bReAccept = pMsg->GetByte();
			const CQuest* pQuest = CQuestSystem::GetQuest((LONG)wQuestID);
			if(NULL == pQuest)	break;
			
			stRunScript sScript;
			sScript.desShape = NULL;
			sScript.pRegion =  pMsg->GetRegion();
			sScript.srcShape	=pPlayer;
			//�ӳ��������Ϊȫ������
			if (bFlag!=0)
			{
				CGUID teamid=pPlayer->GetTeamID();
				if (teamid == NULL_GUID)
					break;
				GSTeam* pTeam=GetOrganiCtrl()->getInstance()->GetGameTeamByID(teamid);
				if (pTeam == NULL)
					break;
				//���ҹ����б����Ƿ��д�����
				if ( pTeam->FindShareQuest(wQuestID)!= 0 )
				{
					const char* strScript =CQuestSystem::GetAddQuestScript();
					if (strScript == NULL)
						break;
					sScript.pszFileName = (char*)strScript;
					//ִ�жӳ���ȫ������Ľű�
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ), wQuestID ,1);
				}
				
			}
			//���ܹ���������ڶ����н��ܸ����б��е�����
			else
			{
				//���ܹ�������
				if (bReAccept)
				{
					const char* strReAccept = CQuestSystem::GetQuestAcceptScript();
					if (strReAccept == NULL)
						break;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strReAccept) ), wQuestID);
					break;
				}
				//���ܸ����б��е�����
				if( pPlayer->IsMercListQuest(wQuestID) && !pPlayer->IsDisbandQuest(wQuestID))
				{
					const char* strScript =CQuestSystem::GetAddQuestScript();
					if (strScript == NULL)
						break;
					sScript.pszFileName = (char*)strScript;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ), wQuestID ,0);
				}
				
			}
			
			//}
		}
		break;
		//��������
	case MSG_C2S_QUEST_Disband:
		{
			//*
			if(pMsg->GetPlayer() == NULL)	break;

			DWORD wQuestID   = pMsg->GetDWord();//>GetWord();
			CPlayer* pPlayer = pMsg->GetPlayer();
			if(pPlayer)
			{
				const CQuest* pQuest = CQuestSystem::GetQuest((LONG)wQuestID);
				if(NULL == pQuest)	break;
				const char* strScript =pQuest->GetAttribute().GetDeleteScript();
				if(strScript == NULL)	break;

				stRunScript sScript;
				sScript.pszFileName = (char*)strScript;
				sScript.desShape = NULL;
				sScript.pRegion =  pMsg->GetRegion();
				sScript.srcShape	=pPlayer ;

				RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ), wQuestID ,0);
			}
			//*/
		}
		break;
		//ˢ��ٸ������
	case MSG_C2S_MERCQUEST_Update:
		{
			if(pMsg->GetPlayer())
			{
				int ListID= pMsg->GetLong();
				BYTE byUseGoods= pMsg->GetByte();
				CPlayer* pPlayer= pMsg->GetPlayer();
				if(pPlayer)
				pPlayer->FreshMercQuest(ListID,byUseGoods!=0);
			}
		}
		break;
	case MSG_W2S_SHARE_QUESTLIST_UPDATE:
		{
			CGUID teamGuid=NULL_GUID;
			pMsg->GetGUID(teamGuid);
			DWORD dwTime=pMsg->GetDWord();
			GSTeam* pTeam=GetOrganiCtrl()->GetGameTeamByID(teamGuid);
			if (pTeam)
			{
				long lsize=pMsg->GetLong();
				if ( 0 == lsize)
				{
					pTeam->ClearShareQuestList();
				}
				else
				{
					pTeam->SetAcceptTime(dwTime);
					pTeam->ClearShareQuestList();
					for (int i=0;i<lsize;i++)
					{
						GSTeam::tagShareQuestList quest;
						quest.lQuestID=pMsg->GetLong();
						pMsg->GetStr(quest.szName,20);
						pTeam->UpdateShareQuestList(&quest);
					}
				}
				pTeam->UpdateShareQuestList(NULL,true);
			}
		}
		break;
	case MSG_C2S_SHARE_QUESTLIST_UPDATE:
		{
			//���GUID
			CGUID guid=NULL_GUID;
			GSTeam::tagShareQuestList quest;
			pMsg->GetGUID(guid);
			BYTE btype=pMsg->GetByte();
			quest.lQuestID=pMsg->GetLong();
			CPlayer* pPlayer=pMsg->GetPlayer();
			if (pPlayer)
			{
				//0��ʾ�Ƽ�����
				if (0 == btype)
				{
					//�ж��ͽ������б����Ƿ��д���������ɾ��
					if ( !pPlayer->UpdateTeamQuestList(quest.lQuestID) )
					{
						break;
					}
				}
			}
			pMsg->SetType(MSG_S2W_SHARE_QUESTLIST_UPDATE);
			pMsg->Send(false);
		}
		break;
	case MSG_W2S_SHARE_CURRQUEST:
		{
			CGUID teamGuid=NULL_GUID;
			pMsg->GetGUID(teamGuid);
			DWORD dwTime=pMsg->GetDWord();
			long lQuestID=pMsg->GetLong();
			GSTeam* pTeam=GetOrganiCtrl()->GetGameTeamByID(teamGuid);
			if (pTeam)
			{	
				pTeam->SetAcceptTime(dwTime);
				pTeam->SetCurrShareQuest(lQuestID);
			}
		}
		break;
	case MSG_W2S_SHARE_SETMEMBERQUEST:
		{
			CGUID teamGuid=NULL_GUID;
			CGUID PlayerGuid=NULL_GUID;
			pMsg->GetGUID(PlayerGuid);
			pMsg->GetGUID(teamGuid);
			long lQuestID=pMsg->GetLong();
			GSTeam* pTeam=GetOrganiCtrl()->GetGameTeamByID(teamGuid);
			if (pTeam)
			{
				pTeam->SetMemberQuest(PlayerGuid,lQuestID);
			}
		}
		break;
	case MSG_W2S_Faction_ChangeTickets_Re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			char cBaseName[32]={0};
			pMsg->GetStr(cBaseName,32);
			BYTE bySucc=pMsg->GetByte();
			LONG leftTickets=pMsg->GetLong();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->SetTickets(cBaseName,leftTickets);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}	
				ContinueRunScript(ScriptGuid, stackVarList);
			}				
		}
		break;
	case MSG_W2S_Faction_ChangeBase_Re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			char cBaseName[32]={0};
			pMsg->GetStr(cBaseName,32);
			BYTE bySucc=pMsg->GetByte();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->ChangeBaseInfo(cBaseName);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}	
				ContinueRunScript(ScriptGuid, stackVarList);
			}
		}
		break;
	case MSG_W2S_Faction_ChangeBattle_Re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			CGUID PlayerGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			pMsg->GetGUID(PlayerGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			BYTE bySucc=pMsg->GetByte();
			LONG leftBattle=pMsg->GetLong();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->AddBattle(leftBattle,PlayerGuid);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}
				ContinueRunScript(ScriptGuid, stackVarList);
			}
		}
		break;
	case MSG_W2S_Faction_ChangeArmyInvest_Re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			CGUID PlayerGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			pMsg->GetGUID(PlayerGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			BYTE bySucc=pMsg->GetByte();
			LONG leftArmyInvest=pMsg->GetLong();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->AddArmyInvest(leftArmyInvest,PlayerGuid);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}
				ContinueRunScript(ScriptGuid, stackVarList);	
			}
		}
		break;
	case MSG_W2S_Faction_ChangeJoinBattle_Re:
		{	
			CGUID FacGuid=NULL_GUID;
			CGUID PlayerGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(PlayerGuid);
			LONG lFlags=pMsg->GetLong();
			
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction==NULL)
				break;
			pFaction->SetJoin(PlayerGuid,lFlags);
		}
		break;
	case MSG_W2S_Faction_ChangeBaseBattle_re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			char cBaseName[32]={0};
			pMsg->GetStr(cBaseName,32);
			BYTE bySucc=pMsg->GetByte();
			LONG leftTickets=pMsg->GetLong();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->SetBaseBattle(cBaseName,leftTickets);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}	
				ContinueRunScript(ScriptGuid, stackVarList);
			}
		}
		break;
	case MSG_W2S_Faction_ChangeTechLevel_Re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			LONG lTechNo=pMsg->GetLong();
			LONG lLv=pMsg->GetLong();
			BYTE bySucc=pMsg->GetByte();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->AddTechLv(lTechNo,lLv);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}	
				ContinueRunScript(ScriptGuid, stackVarList);	
			}
		}
		break;
	case MSG_W2S_Faction_ChangeEnemy_Re:
		{
			CGUID FacGuid=NULL_GUID;
			CGUID ScriptGuid=NULL_GUID;
			pMsg->GetGUID(FacGuid);
			pMsg->GetGUID(ScriptGuid);
			char cRet[32]={0};
			pMsg->GetStr(cRet,32);
			char szFacName[32]={0};
			pMsg->GetStr(szFacName,32);
			DWORD dwStartTime=pMsg->GetDWord();
			DWORD dwEndTime=pMsg->GetDWord();
			LONG lBattleType=pMsg->GetLong();
			LONG lFlag=pMsg->GetLong();
			BYTE bySucc=pMsg->GetByte();
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction!=NULL && bySucc==1)
			{
				pFaction->ChangeEnemy((eBattleType)lBattleType,szFacName,dwStartTime,dwEndTime,lFlag);
			}
			//����ִ�нű�
			if (g_Scripts.find(ScriptGuid)!=g_Scripts.end())
			{
				CVariableList* stackVarList = CScript::GetStackVarListInst();
				if(stackVarList)
				{
					stackVarList->Release();
					stackVarList->AddVar(cRet, (double)bySucc);
				}	
				ContinueRunScript(ScriptGuid, stackVarList);
			}
		}
		break;
	case MSG_C2S_Faction_ChangeJoinBattle:
		{
			CPlayer* pPlayer=pMsg->GetPlayer();
			if (pPlayer==NULL)
				break;
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
			if (pFaction==NULL)
				break;
			//���ǻ᳤
			if(pFaction->GetMaster()!=pPlayer->GetExID())
				break;
			
			//���޸ĳ�Ա��GUID
			CGUID PlayerGuid=NULL_GUID;
			pMsg->GetGUID(PlayerGuid);
			CMessage msg(MSG_S2W_Faction_ChangeJoinBattle);
			msg.Add(PlayerGuid);
			msg.Add(pPlayer->GetFactionID());
		
			msg.Add(pMsg->GetByte());
			msg.Send();
		}
		break;
	case MSG_W2S_Faction_ClearBase_Notify:
		{
			CGUID FacGuid;
			pMsg->GetGUID(FacGuid);
			GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(FacGuid);
			if (pFaction==NULL)
				break;
			pFaction->ClearBase(); 

		}
		break;
	}
}