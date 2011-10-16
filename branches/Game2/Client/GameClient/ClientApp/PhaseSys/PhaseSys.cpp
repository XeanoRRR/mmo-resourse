#include "stdafx.h"
#include "PhaseSys.h"
#include "../Player.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../../GameClient/Game.h"
#include "../../../Net/Message.h"

namespace  PhaseSysBox
{
    struct  tagPhaseBox
    {
        CGUID  first;
        CGUID  second;
        CGUID  SessionID;
    };
}

void  InviteeJoinPhase(MsgEvent& event,eMsgResult UserId)
{
    /// �Ƿ������λ
    BYTE  ret = ( UserId == S_Ok ) ? 1 : 0;
    PhaseSysBox::tagPhaseBox* PhaseBox = (PhaseSysBox::tagPhaseBox*)event.pData; 
    if ( !PhaseBox ) return ;
   
    CMessage  Msg( MSG_C2S_PHASE_OPER );
    Msg.Add( (long) C2S_Invitee );
    Msg.Add( PhaseBox->first );
    Msg.Add( PhaseBox->second );
    Msg.Add( PhaseBox->SessionID);
    Msg.Add( (BYTE) ret );
    Msg.Send();

    SAFE_DELETE( PhaseBox );
}

void   PhaseSys::PhaseMsgPro(CMessage* pMsg)
{
    S2CPhaseOperType  Type = (S2CPhaseOperType)pMsg->GetLong();
    
    switch( Type )
    {   
    case S2C_ChangeState:
        {
            /// �����Χ��� 
            CGUID  PhaseID,PlayerID;
            pMsg->GetGUID( PhaseID );
            pMsg->GetGUID( PlayerID);
            CPlayer* player = GetGame()->GetMainPlayer();
            /// ������Լ�
            if ( player->GetExID() == PlayerID )
            {
                player->SetPhaseID(PhaseID);
            }
            else 
            {
                /// �Լ�UI����Ч�ı�
                /// ͬ��������ΧShape
            } 
        }
        break;

    case S2C_EnterPhase:
        {
            CGUID  PhaseID;
            /// �õ���λID
            pMsg->GetGUID( PhaseID ); 
            CPlayer* player = GetGame()->GetMainPlayer();
            player->SetPhaseID(PhaseID);
            {
                /** 
                 * /// �Լ�UI����Ч�ı�
                 * /// ͬ������������ΧShape
                 */
                float  xpoint,ypoint,asize,bsize;
                xpoint = pMsg->GetFloat();
                ypoint = pMsg->GetFloat();
                asize = pMsg->GetFloat();
                bsize = pMsg->GetFloat();
                CPlayer::PhasePlace  place;
                place.x_point = xpoint;
                place.y_point = ypoint;
                place.a_size  = asize;
                place.b_size  = bsize;
                player->SetPhasePlace( place );
                GetInst(MsgEventManager).PushEvent(Msg_Ok,"������λ");
            }
        }
        break;

    case S2C_LeavePhase:
        {
            CPlayer* player = GetGame()->GetMainPlayer();
            player->SetPhaseID(NULL_GUID);
            {
                /// �Լ�UI����Ч�ı�
                /// ͬ��������ΧShape
            }
            GetInst(MsgEventManager).PushEvent(Msg_Ok,"�뿪��λ");
        }
        break;

    case S2C_InVitee:
        {
            CGUID  first,second,SessionID;
            pMsg->GetGUID( first ); ///> ������
            pMsg->GetGUID( second );///> ��������
            pMsg->GetGUID( SessionID);/// SessionID

            CPlayer *pPlayer = GetGame()->GetMainPlayer();

            char name[256];
            pMsg->GetStr(name,256);
            strcat(name," �����Ƿ������λ? ");

            PhaseSysBox::tagPhaseBox* PhaseBox = new PhaseSysBox::tagPhaseBox;
            PhaseBox->first = first;
            PhaseBox->second= second;
            PhaseBox->SessionID= SessionID;
            GetInst(MsgEventManager).PushEvent(Msg_YesNo,name,InviteeJoinPhase,PhaseBox);
        }
        break;

    case S2C_JoinTimeOut:
        {
            GetInst(MsgEventManager).PushEvent(Msg_Ok,"������λ��ʱ!");
        }
        break;
    case S2C_RefuseJoin:
        {
            GetInst(MsgEventManager).PushEvent(Msg_Ok,"�ܾ�������λ!");
        }
        break;
    }
}

void   PhaseSys::PhaseOper(CPlayer* player,C2SPhaseOperType Oper)
{
    CMessage  Msg( MSG_C2S_PHASE_OPER );
    Msg.Add( (long) Oper );
    Msg.Add( player->GetExID() );
    Msg.Send();
}

void   PhaseSys::PhaseOper(const CGUID& first,const CGUID& second,C2SPhaseOperType Oper)
{
     CMessage  Msg( MSG_C2S_PHASE_OPER );
     Msg.Add( (long) Oper );
     Msg.Add( first );
     Msg.Add( second);
     Msg.Send();
}