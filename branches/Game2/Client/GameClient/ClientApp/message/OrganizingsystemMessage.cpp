#include "stdafx.h"
#include "../GameClient/PlayerRanks.h"
#include "Player.h"
#include "Message.h"
#include "Organizingsystem/Union.h"
#include "Organizingsystem/Faction.h"
#include "Organizingsystem/FactionPageOper.h"
#include "Other/DeputyOccuSystemClient.h"
#include "CollectionGoods.h"
#include "Other/AudioList.h"
#include "QuestSystem/QuestSystem.h"
#include "../GameClient/Game.h"  
#include "../MsgBoxSys/MsgEventDef.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../../../Public/Common/QuestDef.h"

namespace  ShareQuest
{
    struct ShareData
    {
        BYTE  QuestResult;
        long  QuestId;
        CGUID SendID;
        char  Name[20];
        char  DestInfo[256];
    };
}
void  AnswerShareQuest(MsgEvent& event,eMsgResult UserId)
{                                       
    ShareQuest::ShareData* pShare = (ShareQuest::ShareData*)event.pData;
    if ( !pShare ) return ;             

    /// ��ʾ�Է���Ϸ����ͬ���벻ͬ���������
    BYTE  ret = ( UserId == S_Ok ) ? 1 : 0;
    CMessage msg( MSG_C2S_QUEST_OPER );
    msg.Add( (BYTE)QS::QS_SHARE );
    msg.Add((BYTE)ret); 
    msg.Add(pShare->QuestId);
    msg.Add(pShare->SendID);
    msg.Send();

    SAFE_DELETE( pShare );
}

/////////////////////////////////////////////////
// zhaohang  2010/3/23 
// �������а�ϵͳͷ�ļ�
#include "../RankSystem/RankSystem.h"
#include "../../../ui/GamePage/RankPage/RankPage.h"
/////////////////////////////////////////////////
void OnOrgasysMessage(CMessage* pMsg)
{
	BYTE *ByteData = new BYTE[102400];

	// �����߼��������
	CFactionManager*	pFactionManager = GetGame()->GetFactionManager();

	switch(pMsg->GetType())
	{

		/* ���壺-----------------------------    ���������Ϣ ----------------------------*/

		// ��ʼ������Э��
	case MSG_S2C_ORGSYS_InitOrgSys_Notify:
		{
			pFactionManager->RecvInitMsg( pMsg );
		}
		break;

		// ��������Ľű��¼�Э��
	case MSG_S2C_ORGSYS_CreateFaction_Notify:
		{
			pFactionManager->RecvCreateNotifyMsg( pMsg );
		}
		break;

    case MSG_S2C_ORGSYS_CreateORGSysUI:
        {
           GetInst(FactionOper).CreateFactionUi( );
        }
        break;

		// ��������Э��
	case MSG_S2C_ORGSYS_CreateFaction_Re:
		{
			GetInst(FactionOper).RecvCreateMsg( pMsg );
		}
		break;

		// �򿪼���Э��
	case MSG_S2C_Faction_OpenGUI:
		{
			GetInst(FactionOper).RecvOpenMsg( pMsg );
		}
		break;

		// ��ɢ����Э��
	case MSG_S2C_ORGSYS_DisbandFaction_Re:
		{
			GetInst(FactionOper).RecvDisbandMsg( pMsg );
		}
		break;

		// ��ɢ����֪ͨЭ��
	case MSG_S2C_ORGSYS_DisbandFaction_Notify:
		{
			GetInst(FactionOper).RecvDisbandNotifyMsg( pMsg );
		}
		break;

		// �˳�����Э��
	case MSG_S2C_ORGSYS_Quit_Re:
		{
			GetInst(FactionOper).RecvQuitMsg( pMsg );
		}
		break;

		// �˳�����֪ͨ
	case MSG_S2C_ORGSYS_Quit_Notify:
		{
			GetInst(FactionOper).RecvQuitNotifyMsg( pMsg );
		}
		break;

		// �߳�����Э��
	case MSG_S2C_ORGSYS_Kick_Re:
		{
			GetInst(FactionOper).RecvKickMsg( pMsg );
		}
		break;

		// �߳�����֪ͨЭ��
	case MSG_S2C_ORGSYS_Kick_Notify:
		{
			GetInst(FactionOper).RecvKickNotifyMsg( pMsg );
		}
		break;

		// �������幦�ܻظ�Э��
	case MSG_S2C_ORGSYS_UpgradeFaction_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvUpgradeMsg( pMsg );
		}
		break;

		// �������幦��֪ͨЭ��
	case MSG_S2C_ORGSYS_UpgradeFaction_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvUpgradeNotifyMsg( pMsg );
		}
		break;

		// һ������Ĺ�������֪ͨ
	case MSG_S2C_Faction_PublicData_Notify:
		{
			pFactionManager->RecvFacPublicDataMsg( pMsg );
		}
		break;

		// ����һ����ҵļ�����Ϣ
	case MSG_S2C_ORGSYS_UpdatePlayerFacInfo:
		{
			GetInst(FactionOper).RecvUpdatePlayerFacInfoMsg( pMsg );
		}
		break;

		// ����������ϻ�Ӧ
	case MSG_S2C_Faction_BaseData_Notify:
		{
			pFactionManager->RecvBaseInfoMsg( pMsg );
		}
		break;

		// ��Ա���ϻ�Ӧ
	case MSG_S2C_Faction_Member_Notify:
		{
			pFactionManager->RecvMemberInfoMsg( pMsg );
		}
		break;

		// ���������ϻ�Ӧ
	case MSG_S2C_Faction_Apply_Notify:
		{
			pFactionManager->RecvRequisitionInfoMsg( pMsg );
		}
		break;

		// �������ϻ�Ӧ
	case MSG_S2C_Faction_Placard_Notify:
		{
			pFactionManager->RecvPlacardInfoMsg( pMsg );
		}
		break;

		// ����¼���ϻ�Ӧ
	case MSG_S2C_Faction_Leaveword_Notify:
		{
			pFactionManager->RecvMemoInfoMsg( pMsg );
		}
		break;

		// �޸Ĺ����Ӧ
	case MSG_S2C_ORGSYS_SetPlacard_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetPlacardMsg( pMsg );
		}
		break;

		// �޸ı���¼��Ӧ
	case MSG_S2C_ORGSYS_SetLeaveword_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetMemoMsg( pMsg );
		}
		break;

		// �ٽ�Ȩ�����ϻ�Ӧ
	case MSG_S2C_ORGSYS_SetJobPurview_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvJobPurviewMsg( pMsg );
		}
		break;

		// �޸Ĺٽ����ֻ�Ӧ
	case MSG_S2C_ORGSYS_SetJobName_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetJobNameMsg( pMsg );
		}
		break;

		// �޸Ĺٽ�Ȩ�޻�Ӧ
	case MSG_S2C_ORGSYS_SetJobPurview_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetJobPurviewMsg( pMsg );
		}
		break;

		// ���ó�Ա�ٽ׻�Ӧ
	case MSG_S2C_ORGSYS_SetMemberJob_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetMemberJobMsg( pMsg );
		}
		break;

		// ��������������Ա��֪ͨ
	case MSG_S2C_ORGSYS_CheckRequisition_Notify:
		{
            GetInst(FactionOper).RecvCheckRequisitionMsgNotify( pMsg );
		}
		break;

		// �������
	case MSG_S2C_ORGSYS_CheckRequisition_Re:
		{
            GetInst(FactionOper).RecvCheckRequisitionMsg( pMsg );
		}
		break;

		// �������
	case MSG_S2C_Faction_InvitePlayer_Re:
		{
			GetInst(FactionOper).RecvInviteRequestMsg( pMsg );
		}
		break;

		// �������֪ͨ
	case MSG_S2C_Faction_InvitePlayer_Notify:
		{
			GetInst(FactionOper).RecvInviteAnswerMsg( pMsg );
		}
		break;

		// �������
	case MSG_S2C_Faction_AddMember_Notify:
		{
			GetInst(FactionOper).RecvJoindNotifyMsg( pMsg );
		}
		break;

		// ���û�Ա�ƺ�
	case MSG_S2C_Faction_SetMemberTitle_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetTitleMsg( pMsg );
		}
		break;

		// ת���峤
	case MSG_S2C_Faction_SetChairman_Re:
		{
			GetInst(FactionOper).RecvSetChairmanMsg( pMsg );
		}
		break;

		// ��ת�ó��峤��֪ͨ
	case MSG_S2C_Faction_SetChairman_Notify:
		{
			GetInst(FactionOper).RecvSetChairmanNotifyMsg( pMsg );
		}
		break;


		// ���ó�Ա�ٽ�֪ͨ
	case MSG_S2C_ORGSYS_SetMemberJob_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetMemberJobNotifyMsg( pMsg );
		}
		break;

		// ���ó�Ա�ƺ�֪ͨ
	case MSG_S2C_Faction_SetMemberTitle_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvSetMemberTitleNotifyMsg( pMsg );
		}
		break;

		// ���³�Ա��ͼ
	case MSG_S2C_Faction_MemberMap_Notifye:
		{
			GetInst(FactionOper).RecvMemberMapNotifyeMsg( pMsg );
		}
		break;

		// ���³�Ա�ȼ�
	case MSG_S2C_Faction_MemberLevel_Notifye:
		{
			GetInst(FactionOper).RecvMemberLevelNotifyMsg( pMsg );
		}
		break;

		// ���³�Աְҵ
	case MSG_S2C_Faction_MemberOccu_Notifye:
		{
			GetInst(FactionOper).RecvMemberOccuNotifyMsg( pMsg );
		}
		break;

		// ���³�Ա����״̬
	case MSG_S2C_Faction_MemberOnline_Notifye:
		{
			GetInst(FactionOper).RecvMemberOnlineNotifyMsg( pMsg );
		}
		break;

		// ������ļ״̬
	case MSG_S2C_Faction_ChangeRecruitState_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvChangeRecruitStateMsg( pMsg );
		}
		break;

		// ���³�Ա����
	case MSG_S2C_Faction_ChangeMemberContribute_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvMemberContributeNotifyMsg( pMsg );
		}
		break;

		// ������Դ
	case MSG_S2C_Faction_ChangeRes_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvResNotifyMsg( pMsg );
		}
		break;

		// ���¾���
	case MSG_S2C_Faction_ChangeExp_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvExpNotifyMsg( pMsg );
		}
		break;

		// �򿪼����б����
	case MSG_S2C_Faction_ApplyBoardOpen_Notify:
		{
            pFactionManager->RecvFactionList( pMsg );
		}
		break;

		// �������ظ�
	case MSG_S2C_ORGSYS_JoinFac_Re:
		{
             GetInst(FactionOper).RecvApplyRestMsg( pMsg );
		}
		break;

		// �򿪻ձ��ϴ��� �ļ��������
	case MSG_S2C_Faction_IconBoardOpen_Notify:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvInputFacIconNotifyMsg( pMsg );
		}
		break;

		// �ϴ�һ������ͼ�����ݻظ�
	case MSG_S2C_Faction_IconData_Re:
		{
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->RecvUploadFacIconMsg( pMsg );
		}
		break;

		// һ�������ͼ�����ݸ���֪ͨ
	case MSG_S2C_Faction_IconData_Notify:
		{
			pFactionManager->RecvUpdateFacIconNotifyMsg( pMsg );
		}
		break;

		// �������м���ͼ����Ϣ
	case MSG_S2C_Faction_AllIconData_Re:
		{
			pFactionManager->RecvUpdateAllFacIconMsg( pMsg );
		}
		break;

		// ����ս���жԼ�����Ϣ
	case MSG_S2C_Faction_ChangeEnemy_Notify:
		{
			pFactionManager->RecvWarInfoMsg( pMsg );
		}
		break;

		// ���²�ս��Ϣ
	case MSG_S2C_Faction_ChangeJoinBattle_Notify:
		{
			pFactionManager->RecvMemberWarMsg( pMsg );
		}
		break;

		// ������Ա��ѫ
	case MSG_S2C_Faction_ChangeBattle_Notify:
		{
			pFactionManager->RecvMemberAchievementMsg( pMsg );
		}
		break;

		// ������Ա΢��
	case MSG_S2C_Faction_ChangeArmyInvest_Notify:
		{
			pFactionManager->RecvMemberWarFundMsg( pMsg );
		}
		break;

		// ���¾ݵ���Ϣ
	case MSG_S2C_Faction_Beachheads_Notify:
		{
			pFactionManager->RecvBeachheadInfoMsg( pMsg );
		}
		break;

		/* End������ --------------------------    ���������Ϣ --------------------------*/
		/* ���壺-----------------------------    ���������Ϣ ----------------------------*/

		// �������˽ű�����֪ͨ�������˴�������
	case MSG_S2C_UNION_Script_Creation:
		{
			GetGame()->GetUnionManager()->RecvScriptCreationMsg( pMsg );
		}
		break;

		// �������˵� ����ظ�
	case MSG_S2C_UNION_Re_Creation:
		{
			GetGame()->GetUnionManager()->RecvCreationMsg( pMsg );
		}
		break;

		// �������˵� ֪ͨ
	case MSG_S2C_UNION_Notify_Creation:
		{
			GetGame()->GetUnionManager()->RecvCreationNotifyMsg( pMsg );
		}
		break;

		// ��ɢ����֪ͨ��������
	case MSG_S2C_UNION_Notify_Disband:
		{
			GetGame()->GetUnionManager()->RecvDisbandMsg( pMsg );
		}
		break;

		// �����˳�����֪ͨ��������
	case MSG_S2C_UNION_Notify_Quit:
		{
			GetGame()->GetUnionManager()->RecvQuitMsg( pMsg );
		}
		break;

		// �����������߳�����֪ͨ
	case MSG_S2C_UNION_Notify_Kick:
		{
			GetGame()->GetUnionManager()->RecvKickMsg( pMsg );
		}
		break;

		// ����ת��֪ͨ��������
	case MSG_S2C_UNION_Notify_SetChairman:
		{
			GetGame()->GetUnionManager()->RecvSetChairmanMsg( pMsg );
		}
		break;

		// ���������峤Ȩ�޻ظ�
	case MSG_S2C_UNION_Re_SetRights:
		{
			GetGame()->GetUnionManager()->RecvSetRightsMsg( pMsg );
		}
		break;

		// �峤һ���˻����������Ȩ�޵�֪ͨ
	case MSG_S2C_UNION_Notify_GetRights:
		{
			GetGame()->GetUnionManager()->RecvGetRightsMsg( pMsg );
		}
		break;

		// ���� �յ� �����峤 �ظ���������
	case MSG_S2C_UNION_Response_Invite:
		{
			GetGame()->GetUnionManager()->RecvResponseMsg( pMsg );
		}
		break;

		// �����峤 �յ� ��������������˵�����
	case MSG_S2C_UNION_Notify_Anwser_Invite:
		{
			GetGame()->GetUnionManager()->RecvInviteMsg( pMsg );
		}
		break;

		// �����������֪ͨ��������
	case MSG_S2C_UNION_Notify_Join:
		{
			GetGame()->GetUnionManager()->RecvJoinMsg( pMsg );
		}
		break;

		// һ�����˵Ļ�������֪ͨ
	case MSG_S2C_UNION_Notify_BaseData:
		{
			GetGame()->GetUnionManager()->RecvUnionInfoMsg( pMsg );
		}
		break;

		// һ�����˵ĳ�Ա����֪ͨ
	case MSG_S2C_UNION_Notify_MemberData:
		{
			GetGame()->GetUnionManager()->RecvUnionMemberMsg( pMsg );
		}
		break;

		/* End������ --------------------------    ���������Ϣ --------------------------*/


		/* ���壺-----------------------------    ս�������Ϣ ----------------------------*/
		/* End������ --------------------------    ս�������Ϣ --------------------------*/



		//���������Ϣ--------------------------------------------
	case MSG_S2C_Quest_Add:
		{
			//------07.9.11���liuke-------
			DWORD dwQuestID = pMsg->GetDWord();
			long lStep = pMsg->GetLong();

			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			if(pMainPlayer)
			{
				pMainPlayer->AddQuest((long)dwQuestID,lStep);
				// ��Mainbar�ϵ�����ť�������Ч��
				/*CMainBarPageEx * pMainbar = GetGame()->GetCGuiEx()->GetMainBarPageEx();
				CQuestPageEx * pQuest = GetGame()->GetCGuiEx()->GetQuestPageEx();
				if(pQuest && !pQuest->IsOpen() && 
				pMainbar && pMainbar->GetInterfaceComp())
				{
				CButton *pQuestBtn = (CButton *)pMainbar->GetInterfaceComp()->GetComponent("Quest");
				if(pQuestBtn && pQuestBtn->GetSpecialIcon())
				((CImageIcon *)pQuestBtn->GetSpecialIcon())->SetIsChangeAlpha(true);
				}*/
			} 
			if(!GetGame()->GetAudioList()->IsPlaying("SOUNDS\\quest\\q-03.wav",-1))  //�������Чδ���ʱ�������Ž�����Ч
			{
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\quest\\q-01.wav");
			}
			CDeputyOccuSystemClient::UpdateCollectionEffectByQuest(dwQuestID,true);
		}
		break;
	case MSG_S2C_Quest_Complete:
		{
			DWORD dwQuestID = pMsg->GetDWord();
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			if(pMainPlayer)
			{
				const CQuest *pQuest = CQuestSystem::GetQuest((long)dwQuestID);
				// �����������񣬸��������������������־
				if(pQuest && pQuest->GetAttribute().GetQuestType()==QUEST_MAIN)
				{
					pMainPlayer->GetRoleQueue()->AddFinishedQuestRecord((long)dwQuestID);
					long lChapter = pQuest->GetAttribute().GetChapterIndex();
					if(lChapter > 0)
						pMainPlayer->GetRoleQueue()->SetChapter(lChapter);
					//GetGame()->GetCGuiEx()->GetQuestPageEx()->UpdateMainQuestList();
				}	

				CDeputyOccuSystemClient::UpdateCollectionEffectByQuest(dwQuestID,false);

				pMainPlayer->RemoveQuest((long)dwQuestID);
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\quest\\q-03.wav");
			}
		}
		break;
	case MSG_S2C_Quest_Remove:
		{
			DWORD dwQuestID = pMsg->GetDWord();
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			if(pMainPlayer)
			{
				CDeputyOccuSystemClient::UpdateCollectionEffectByQuest(dwQuestID,false);
				pMainPlayer->RemoveQuest((long)dwQuestID);
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\quest\\q-02.wav");
			}
		}
		break;
	case MSG_S2C_Quest_UpdateStep:
		{
			DWORD dwQuestID = pMsg->GetDWord();
			long lStep = pMsg->GetLong();
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			if(pMainPlayer)
			{
				pMainPlayer->UpdataQuestStep((long)dwQuestID,lStep);
			}
		}
		break;
		//����һ����������б���
	case MSG_S2C_Quest_UpdateParamData:
		{
			DWORD dwQuestID = pMsg->GetDWord();
			long lIndex = pMsg->GetLong();
			long lValue = pMsg->GetLong();
			long lRegionID = pMsg->GetLong();
			long lRegionX = pMsg->GetLong();
			long lRegionY = pMsg->GetLong();
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			if(pMainPlayer)
			{
				pMainPlayer->UpdataQuestTarget((long)dwQuestID,lIndex,lValue,lRegionID,lRegionX,lRegionY);
			}
		}
		break;
	case MSG_S2C_Quest_UpdateData:
		{
			DWORD dwQuestID = pMsg->GetDWord();
			long lIndex = pMsg->GetLong();
			long lValue = pMsg->GetLong();
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			if(pMainPlayer)
			{
				pMainPlayer->UpdataQuestTarget((long)dwQuestID,lIndex,lValue);
			}
		}
		break;
		//�������һ�������¼(һ������������)
	case MSG_S2C_Quest_UpdateRecordData:
		{
			long lQuestID = pMsg->GetLong();
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			// Ϊ������һ���Ѿ���ɵ�����
			if(pMainPlayer)
				pMainPlayer->GetRoleQueue()->AddFinishedQuestRecord(lQuestID);
		}
		break;
	case MSG_S2C_Quest_Talk:
		{
			long lQuestID = pMsg->GetLong();
			long lTextID = pMsg->GetLong();

		}
		break;
	case MSG_S2C_MercQuest_UpdateData:
		{
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			// Ϊ������һ���Ѿ���ɵ�����
			if(NULL == pMainPlayer)	break;

			long lCurMercQuestID = pMsg->GetLong();  //��ȡ��ǰ����ID
			//��õ�ǰ�ķ��������б�
			//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->ClearDelQuestList();
			long lNum =  pMsg->GetLong();  //��÷���������

			for(int i=0;i<lNum;i++)
			{
				long lDelQuestID =  pMsg->GetLong();  //��÷���������
				//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->AddMerQuestDel(lDelQuestID);
			}
			long lType = pMsg->GetLong();
			pMainPlayer->GetMercQuestSys()->SetCurQuestID(lCurMercQuestID);
			if(lType==0)
			{
				pMainPlayer->GetMercQuestSys()->SetCurQuestID(lCurMercQuestID);

				//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->UpdateCurQuest();
			}
			else if(lType==1)
			{
				//long lCurMercQuestID = pMsg->GetLong();  //��ȡ��ǰ����ID
				long lListID = pMsg->GetLong();           //��ȡ�����б�ID 
				long dwLastUpdateTime = pMsg->GetDWord(); //��ȡ�������ʣ��ʱ��
				if(dwLastUpdateTime<0)    dwLastUpdateTime = 0;
				//pMainPlayer->GetMercQuestSys()->SetCurQuestID(lCurMercQuestID);
				//pMainPlayer->GetMercQuestSys()->SetLastUpdateTime(dwLastUpdateTime);
				//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->SetUpdateTime(dwLastUpdateTime);

				if(lListID != 0)
				{
					long lSize = pMsg->GetWord();
					CMercenaryQuestSys::tagMerQuest MerQuests;
					MerQuests.dwLastUpdateTime=dwLastUpdateTime;
					MerQuests.Quests.clear();
					//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->ClearMerQuestLast();
					MerQuests.dwLastUpdateTime = dwLastUpdateTime;
					for(int i=0;i<lSize;i++)
					{
						long lID = pMsg->GetLong();
						long lIsAccept = pMsg->GetLong();  //�������Ƿ�������
						if(lID!=0)
							MerQuests.Quests.push_back(lID);
						//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->AddMerQuestLast(lIsAccept);
					}

					//pMainPlayer->GetMercQuestSys()->SetMercQuests(lListID,MerQuests);
					//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->SetMerQuestList(MerQuests);
					//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->SetListID(lListID);

				}

			}
			//if(GetGame()->GetCGuiEx()->GetMercenaryPageEx()->IsOpen())
			//{
			//	GetGame()->GetCGuiEx()->GetMercenaryPageEx()->UpdatePage();
			//	if(lType==0)
			//	{
			//		GetGame()->GetCGuiEx()->GetMercenaryPageEx()->UpdateQuestNote(-1);
			//	}
			//}
		}
		break;
	case MSG_S2C_MercQuest_OpenInterface:
		{
			long lListID = pMsg->GetLong();
			long lOpenType = pMsg->GetLong(); 
			//GetGame()->GetCGuiEx()->GetMercenaryPageEx()->Open();
		}
		break;
	case MSG_S2C_SHARE_QUESTLIST_UPDATE:
		{

            ShareQuest::ShareData * pShare = new  ShareQuest::ShareData;
            pShare->QuestId = pMsg->GetLong();
            char strName[MAX_PATH] = "";
            pMsg->GetStr(pShare->Name,MAX_PATH);
            CGUID  SendID;
            pMsg->GetGUID( pShare->SendID );
            pShare->QuestResult = pMsg->GetByte();
            char DQuest[16];
            sprintf_s( DQuest ,"Q%d", pShare->QuestId );
            sprintf_s( pShare->DestInfo, AppFrame::GetText("TM_41"), pShare->Name,AppFrame::GetText( DQuest ));
            GetInst(MsgEventManager).PushEvent(Msg_YesNo,pShare->DestInfo,AnswerShareQuest,pShare);

		}
		break;
	case  MSG_S2C_SHARE_QUEST_OPEN:
		{
			//long lQuestId = pMsg->GetLong();
			//CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			//// Ϊ������һ���Ѿ���ɵ�����
			//if(NULL == pMainPlayer)	break;

			//long lCurQuest = pMainPlayer->GetMercQuestSys()->GetCurQuestID();
			//long lCurTQuest = GetGame()->GetCGuiEx()->GetMercenaryPageEx()->GetTeamQuestId();
			//if(lCurQuest!=lCurTQuest)
			//{
			//GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->Open();
			//GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->UpdateQuestNote();
			//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\quest\\q-01.wav");
			//if(GetGame()->GetCGuiEx()->GetMercenaryPageEx()->IsOpen())
			//{
			//	GetGame()->GetCGuiEx()->GetMercenaryPageEx()->Close();
			//}
			//}
		}
		break;
	case MSG_S2C_SHARE_CURRQUEST:
		{
			long lCurTeamQuest = pMsg->GetLong();
			/*GetGame()->GetCGuiEx()->GetMercenaryPageEx()->SetTeamQuestId(lCurTeamQuest);
			GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->SetShowQuestId(lCurTeamQuest);
			if(GetGame()->GetCGuiEx()->GetMercenaryPageEx()->IsOpen())
			{
			GetGame()->GetCGuiEx()->GetMercenaryPageEx()->UpdateTeamCurQuest();
			}
			if(GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->IsOpen())
			{
			GetGame()->GetCGuiEx()->GetMercenaryNotePageEx()->UpdateQuestNote(lCurTeamQuest);
			}*/
			//��ʾ��ʾ
			const CQuest *pQuest = CQuestSystem::GetQuest(lCurTeamQuest);
			if(pQuest == NULL)
				break;
			//��������Ƿ��Ƕӳ�
			CPlayer *pPlayer = GetGame()->GetMainPlayer();
			if(!pPlayer)	return;
			if(pPlayer->GetCaptainID() == pPlayer->GetExID())
			{
				char chText[256]="";
				sprintf(chText,AppFrame::GetText("TM_20"),pQuest->GetAttribute().GetQuestName());
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,chText,0xff00ffff);
			}else{
				char chText[256]="";
				sprintf(chText,AppFrame::GetText("TM_21"),pQuest->GetAttribute().GetQuestName());
				// GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,chText,0xff00ffff);
			}
		}
		break;
		///���а�ϵͳ
	case MSG_S2C_ANSWER_PLAYERRANKS:
		{
			//pMsg->GetEx(ByteData,102400);
			//long lPos = 0;
			//GetPlayerRanks()->DecordFromByteArray(ByteData, lPos);
			/////////////////////////////////////////////////
			// zhaohang  2010/3/22 
			// �¹������а�ϵͳ,��������������ǰ����Ϣͷ�����Ǳ����ı��ˣ����ע�͵���ǰ���룬���½����ⲿ�ֵı��� 
			long lPos = 0;
			CGUID guid;
			long lBufSize;
			pMsg->GetGUID(guid);
			char eRt = pMsg->GetChar();
			RankPage& page = GetInst(RankPage);
			RankMsgProc &RankMsg = GetInst(RankMsgProc);
			switch(eRt)
			{
			case RT_NOMATCH:	// ��ƥ�������
				RankMsg.SetRankState(RT_NOMATCH);
				break;	
			case RT_UPDATE:		// ���������ڸ���
				RankMsg.SetRankState(RT_UPDATE);
				break;
			case RT_OWNER:
				RankMsg.SetRankState(RT_OWNER);
				lBufSize = pMsg->GetLong();
				pMsg->GetEx(ByteData, lBufSize);
				RankMsg.ReceiveMsg(ByteData,lPos);
				page.UpdatePage();
				break;
			case RT_OTHER:
			case RT_RANK:
				lBufSize = pMsg->GetLong();
				pMsg->GetEx(ByteData, lBufSize);
				RankMsg.ReceiveMsg(ByteData,lPos);
				page.UpdatePage();
				break;
			}
			/////////////////////////////////////////////////
		}
		break;
	}
	SAFE_DELETE_ARRAY(ByteData);
}