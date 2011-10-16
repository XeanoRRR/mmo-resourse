/** 
 * @file   PhaseMsg.cpp  
 * @brief  ��λ���ܵ���Ϣ����ģ�飬�����г�
 * @date   2010/03/18 

 * @modify 2010/05/31  ���³�ʱ,����CSessionFactory��ȡ���Զ�����
 */
#pragma  once    

#include "StdAfx.h"
#include "PhaseManager.h"
#include "PhaseOtherDef.h"
#include "../Player.h"   
#include "../session/CSessionFactory.h"
#include "../session/CSession.h"
#include "../OrganizingSystem/GameOrganizingCtrl.h"
#include "../OrganizingSystem/GSTeam.h"
#include "../../../../Public/Setup/PhaseSetup.h"

/** 
 *  Ԥ����Ϣ����
 *  ����GS,WS,GC֮���ͨ��
 */  
void  PhaseManager::OnMessage(CMessage* pMsg)
{                   
    /// GS=GC��ͨ��
    C2SPhaseOperType type = (C2SPhaseOperType)pMsg->GetLong();

    switch( type )
    {
        /// �������� 
    case C2S_Invite:
        {
            CGUID  first,second;
            /// ������
            pMsg->GetGUID( first ); 
            /// ��������
            pMsg->GetGUID( second );

            /// ��Ҫ��������
            if ( !AddPhaseInviteMapTable(first,second) )
            {
                return ;
            }

            const CGUID&  PSessionID = CSessionFactory::CreateSession( 0 , 2 , PhaseTimer*1000 );
            CSession*  session = const_cast<CSession*>(CSessionFactory::QuerySession( PSessionID ));
            
            if( session && session->Start() )
            {
                m_InviteList.insert( std::make_pair<CGUID,ApplyList>(first,ApplyList(second,session) ) );

                CMessage msg( MSG_S2C_PHASE_OPER );
                msg.Add( (long) S2C_InVitee );
                msg.Add( first );
                msg.Add( second);
                msg.Add( PSessionID );
                msg.Add( pMsg->GetPlayer()->GetName() );
                msg.SendToPlayer( second );
            }
            else  if( session )
            {
                  session->Abort();
            }

        }
        break;

        /// ͬ�����pk��λ
    case C2S_Invitee:
        {
            CGUID  first,second,PSessionID;
            /// ������
            pMsg->GetGUID( first );
            /// ��������
            pMsg->GetGUID( second );
            pMsg->GetGUID( PSessionID );
            BYTE ret = pMsg->GetByte();

            CSession*  session = const_cast<CSession*>(CSessionFactory::QuerySession( PSessionID ));
            AppItr Aitr = m_InviteList.find( first );
            if( Aitr == m_InviteList.end() )
                break;
            if ( ret == 1 && session )
            {    
                /// �õ���������
                if( Aitr->second.first == second && Aitr->second.second == session )
                {                
                    CreatePhase(first,second,10,20,eActive,"");
                }  
                session->End();
            }
            else if( ret == 0 && session )
            {
                CMessage msg( MSG_S2C_PHASE_OPER );
                msg.Add( (long) S2C_RefuseJoin );
                msg.SendToPlayer( first );
            }
            else
            {
                CMessage msg( MSG_S2C_PHASE_OPER );
                msg.Add( (long) S2C_JoinTimeOut );
                msg.SendToPlayer( second );
            }          
            m_InviteList.erase( Aitr );
        }
        break;

        /// �����뿪��λ
    case C2S_Leave:
        {
            CGUID  first;
            ///> ���ID
            pMsg->GetGUID( first ); 
            OnPhaseOper( first , GetGame()->FindPlayer( first) , eLeave );
        }
        break;

    case C2S_Escape:
        {
            CGUID  first;   
            ///> ���ID
            pMsg->GetGUID( first ); 
            OnPhaseOper( first , GetGame()->FindPlayer( first) , eEscape );
        }
        break;

        /// �����Ӳ���
    case C2S_ReJoin:
        {
            CGUID guid;
            pMsg->GetGUID( guid );

            CPlayer* player = GetGame()->FindPlayer( guid );
            if ( player!=NULL && player->GetTeamID()!= NULL_GUID )
            {
                GSTeam *teamf = GetOrganiCtrl()->GetGameTeamByID( player->GetTeamID() );
                /// ������ڶ������Ҳ��Ƕӳ�
                if( teamf != NULL && teamf->GetLeader() != player->GetExID() )
                {
                      CPlayer* leader = GetGame()->FindPlayer(  teamf->GetLeader() );
                      /// ����ӳ�����λ�Ͷ��ѵ���λID���ҵ�ͼ�����ӿ���ֱ�Ӵ���,���Խ��й�Ӳ���
                      if ( leader!=NULL && leader->GetPhaseID() == player->GetPhaseID()  &&
                           static_cast<CServerRegion*>(leader->GetFather())->GetRgnRejoinTeam() 
                           == CRegion::PhaseJoin_YES )
                      {
                          player->ChangeRegion( RGN_NORMAL,static_cast<CServerRegion*>(leader->GetFather())->GetExID(),
                             leader->GetTileX(), leader->GetTileY(), 0 );
                          Log4c::Error(ROOT_MODULE,"%-15s Player %s Request Rejoin team successed��now site x,y: %f,%f",
                              __FUNCTION__, player->GetName(),leader->GetPosX(), leader->GetPosY() );
                      }
                }
            }
            Log4c::Error(ROOT_MODULE,"%-15s Player %s  Request Rejoin  Team failed, bacause Leader is in Phase States",__FUNCTION__,player->GetName());
        }
        break;
    }
}