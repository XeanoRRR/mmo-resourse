/** 
 * @file   PhaseMsg.cpp  
 * @brief  ����ӳ���Ϣ����
 * @date   2010/03/18        
 */

#pragma  once

#include "StdAfx.h"
#include "../Player.h"
#include "../OrganizingSystem/GSTeam.h"
#include "../OrganizingSystem/GameOrganizingCtrl.h"
#include "../../Server/Game.h"
#include "../../../../Public/Common/FollowDef.h"
#include "FollowManager.h"

/** 
 *   Ԥ���ӳ��������������ƽӿڴ���
 */

void  OnFollowMessage( CMessage* pMsg)
{
    /// GS=GC��ͨ��
    TEAM_FOLLOW_TYPE type = (TEAM_FOLLOW_TYPE)pMsg->GetLong();

    switch( type )
    {
        /// �ӳ�������Ѹ���ӳ�
    case  TF_Team_Leader_Invite:
        {
            CPlayer* pLeader =  pMsg->GetPlayer();
            if ( pLeader != NULL && pLeader->GetTeamID() != NULL_GUID )
            {
                GSTeam *team =  GetOrganiCtrl()->GetGameTeamByID( pLeader->GetTeamID() );
                /// ����Ƕӳ���Ϣ
                if ( team != NULL && team->GetLeader() == pLeader->GetExID() )
                {
                    /// ����Ա������������
                }
                else
                {
                    //// �Ƕӳ��ķǷ���Ϣ
                }
            }

        }
        break;

    case  TF_Team_Return_TeamFollow:
        {
            CGUID  first,second;  
            ///> ������
            pMsg->GetGUID( first ); 
            ///> ��������
            pMsg->GetGUID( second );   
        }
        break;

    case  TF_Team_Member_Follow_Leader:
        {
            CPlayer* player =  pMsg->GetPlayer();
            if ( player != NULL && player->GetTeamID() != NULL_GUID )
            {
                GSTeam *team =  GetOrganiCtrl()->GetGameTeamByID( player->GetTeamID() );
                CPlayer* Leader = GetGame()->FindPlayer( team->GetLeader() );
                if( !team || !Leader || Leader==player ) 
                    return ;
                /// ������Ƕӳ���Ϣ 

                player->SetCurrentProgress( CPlayer::PROGRESS_FOLLOW_LEADER );          
                long  lRegionID = static_cast<CServerRegion*>(Leader->GetFather())->GetID();
                long  lPointx   = Leader->GetTileX();
                long  lPointy   = Leader->GetTileY();
                /// ����Ա������������
                CMessage  Msg( MSG_S2C_FOLLOW_Leader );
                Msg.Add( (long) TF_Team_Member_Follow_Leader);
                Msg.Add( (long) TF_Result_Follow_Success );
                Msg.Add( lRegionID );
                Msg.Add( lPointx );
                Msg.Add( lPointy );
                Msg.SendToPlayer( player->GetExID() );
            }
        }
        break;

    case  TF_Team_Stop_TeamFollow:
        {
            CPlayer* player =  pMsg->GetPlayer();
            if ( !player )  return ;
            
            GetInst(FollowManager).StopFollow( player );
        }
        break;

    case TF_Team_Member_ChangeRgn:
        {

        }
        break;
    }
}