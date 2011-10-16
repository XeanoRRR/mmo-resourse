#include "stdafx.h"
#include "FactionPageOper.h"
#include "ClientRegion.h"
#include "BaseObject.h"
#include "Player.h"
#include "MsgBoxSys/MsgEventManager.h"
#include "../GameClient/Game.h"
#include "../../../ui/GamePage/ChatList/ChatListPage.h"

namespace  uFactionOper
{
    void  ExitFaction(MsgEvent& event,eMsgResult UserId)
    {
        if( UserId  !=  S_Ok ) return ;

        if ( GetGame()->GetFactionManager()->GetMyJob() == -1 )
        {
            GetGame()->GetFactionManager()->SendDisbandMsg();
            return;
        }   
        GetGame()->GetFactionManager()->SendQuitMsg();
    }

    void  JoinFaction(MsgEvent& event,eMsgResult UserId)
    {
        CGUID * guid = static_cast<CGUID*>(event.pData);
        if( UserId  !=  S_Ok ) 
        { 
            SAFE_DELETE( guid );
            return ;
        }

        CMessage msg( MSG_C2S_ORGSYS_JoinFac_Request );
        msg.Add( (long)1 );
        msg.Add( *guid );
        msg.Send();

        SAFE_DELETE( guid );
    }

    void FactionInviteAnswer(MsgEvent& event,eMsgResult UserId)
    {
        FactionOper::SInviterInfo*	pst = static_cast<FactionOper::SInviterInfo*>(event.pData); 
        if ( UserId != S_Ok )
        {
            SAFE_DELETE( pst );
            return;
        }          

        /// ͬ����� 


    }

}

FactionOper::FactionOper()
{
    m_pFactionManager = GetGame()->GetFactionManager();
    m_IsOpen = m_Factionlist = false;
}
FactionOper::~FactionOper()
{
    m_Factionlist = false;
}

void FactionOper::Switch()
{
    CPlayer* player = GetGame()->GetMainPlayer();
    if ( !player)
        return ;

    // "����δ������壡"
    if( !m_pFactionManager->AmIJoinedFaction() )
    {
        GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("FA_301")); //����δ�����лᣡ
        return ;
    }
    m_IsOpen = !m_IsOpen;
    FireUIEvent("FactionPage",EVENT_SWITCH);
    if ( m_IsOpen )
    {
        m_pFactionManager->SendBaseInfoRequestMsg();
    }        
}

void  FactionOper::RecvCreateMsg(CMessage * pMsg)
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        CGUID  facGuid;
        pMsg->GetGUID( facGuid );
        GetGame()->GetMainPlayer()->SetFactionID( facGuid );

        // ���ùٽ�,�᳤
        m_pFactionManager->SetMyJob( Exceptive_JobLvl );

        // "��ᴴ���ɹ���"
        GetInst(MsgEventManager).PushEvent(Msg_Ok,"�������ɹ�!");
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::ExitFaction()
{
    const char* pStr = NULL;AppFrame::GetText("FA_401"); //��ȷ��Ҫ��ɢ�л���
    if ( GetGame()->GetFactionManager()->GetMyJob() != -1 )
    {
        pStr = AppFrame::GetText("FA_402");      //��ȷ��Ҫ�˳��л���
    }
    else
    {
        pStr = AppFrame::GetText("FA_eIDS_FAC_AreYouSureQuitFac");
    }

    /// ȷ���˳����
    GetInst(MsgEventManager).PushEvent(Msg_YesNo,pStr,uFactionOper::ExitFaction);
}

void  FactionOper::JoinFactionList(const CGUID& guid)
{   
    CGUID * id = new CGUID(guid.tostring());
    GetInst(MsgEventManager).PushEvent(Msg_YesNo,AppFrame::GetText("FA_409"),uFactionOper::JoinFaction,id);//307
}

void  FactionOper::OpenCountryFaction()
{
     m_Factionlist =!m_Factionlist;
     if (m_Factionlist)
     {
         m_pFactionManager->SendFactionList();
     }
}

void  FactionOper::RecvApplyRestMsg(CMessage* pMsg)
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        // ���뻹��ȡ������
        if( pMsg->GetLong() )
        {
            // �����ļ���GUID
            CGUID	guidFaction;
            pMsg->GetGUID( guidFaction );

            // "���ļ����������ύ����ȴ�����������"
            GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("FA_313"));
        }
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}
void  FactionOper::RecvQuitMsg( CMessage* pMsg )
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::RecvQuitNotifyMsg(CMessage* pMsg)
{
    // �˳���Ա�ļ��� GUID
    CGUID	guidFaction;
    pMsg->GetGUID( guidFaction );

    // �˳���Ա�� GUID
    CGUID	guidMember;
    pMsg->GetGUID( guidMember );

    // �˳�����Ա ����
    TCHAR	nameMember[ MAX_MEMBER_NAME_SIZE ];
    pMsg->GetStr( nameMember, MAX_MEMBER_NAME_SIZE );

    // �Լ������GUID
    CGUID	guidMyFaction = GetGame()->GetMainPlayer()->GetFactionID();

    // ���������������е����
    std::list<CBaseObject*>::iterator p = GetGame()->GetRegion()->GetObjectList()->begin();
    for( ; p != GetGame()->GetRegion()->GetObjectList()->end(); p++ )
    {
        if( (*p)->GetType() == TYPE_PLAYER )
        {
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p);

            // ���˳���Ա�ļ�����Ϣ���
            if( pPlayer->GetExID() == guidMember )
            {
                // ����Լ��ļ����־��Ϣ
                pPlayer->SetFactionID( NULL_GUID );
                pPlayer->SetUnionID( NULL_GUID );
                pPlayer->SetFactionName( std::string() );
                pPlayer->SetFactionTitle( std::string() );

                break;
            }
        }
    }
    // ��������Լ�����ģ��򷵻�
    if( guidMyFaction != guidFaction )
        return;
    // �����Լ�
    if( GetGame()->GetMainPlayer()->GetExID() == guidMember )
    {
        FireUIEvent("FactionPage","Closed");

        // "���Ѿ��˳����壡"
        //GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-31.wav");
        // "���ļ����������ύ����ȴ�����������"
        GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("FA_307")); //���Ѿ��˳���ᣡ
    }
}

void  FactionOper::PushApplyInfo(const CGUID& guid,BOOL Agree)
{
     m_pFactionManager->SendCheckRequisitionMsg( Agree ,guid );
}

void  FactionOper::RecvCheckRequisitionMsg(CMessage* pMsg)
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        // "�����Լ���׼����ļ������룡"
        GetInst(MsgEventManager).PushEvent(Msg_Ok,"�����Լ���׼����ļ�������");
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::RecvCheckRequisitionMsgNotify(CMessage* pMsg)
{
    // ����GUID
    CGUID	guidFaction;
    pMsg->GetGUID( guidFaction );

    // true��ӣ�falseɾ��
    long	bAdd = pMsg->GetLong();

    if( CFaction* pMyFaction = m_pFactionManager->GetMyFaction() )
    {
        if( bAdd )
        {
            //�����߽ṹ
            tagFacApplyPerson	stFacApplyPerson;
            pMsg->GetEx( &stFacApplyPerson, sizeof(stFacApplyPerson) );

            // �������
            CFacMember::SMemberInfo	stMemberInfo;
            stMemberInfo.guidFac = GetGame()->GetMainPlayer()->GetFactionID();
            stMemberInfo.guidMember = stFacApplyPerson.PlayerGuid;
            stMemberInfo.strName = stFacApplyPerson.szName;
            stMemberInfo.lLevel = stFacApplyPerson.lLvl;
            stMemberInfo.lOccupation = stFacApplyPerson.lOccu;

            pMyFaction->AddApply( &stMemberInfo );
        }
        else
        {
            // ������GUID
            CGUID	guidRequisition;
            pMsg->GetGUID( guidRequisition );

            // ɾ��
            pMyFaction->DelApply( guidRequisition );
        }
    }
    FireUIEvent("FactionPage","UpdateFacAppList");
}

void FactionOper::RecvJoindNotifyMsg(CMessage* pMsg)
{
    // ��Ա�ṹ
    tagFacMemInfo	stFacMemInfo;
    pMsg->GetEx( &stFacMemInfo, sizeof(stFacMemInfo) );
    CGUID           FactionGuid;
    pMsg->GetGUID( FactionGuid );

    // ���һ����Ա
    CFacMember::SMemberInfo	stMemberInfo;
    stMemberInfo.guidFac = FactionGuid;
    stMemberInfo.guidMember = stFacMemInfo.MemberGuid;
    stMemberInfo.strName = stFacMemInfo.szName;
    stMemberInfo.strTitle = stFacMemInfo.szTitle;
    stMemberInfo.lLevel = stFacMemInfo.lLvl;
    stMemberInfo.lJob = stFacMemInfo.lJobLvl;
    stMemberInfo.lOccupation = stFacMemInfo.lOccu;
    stMemberInfo.lContribute = stFacMemInfo.lContribute;
    stMemberInfo.bOnline = stFacMemInfo.bIsOnline != 0;
    stMemberInfo.strRegion = stFacMemInfo.szRegion;

    
    m_pFactionManager->AddMyConfrere( &stMemberInfo );

    // �Ƿ����Լ�
    if( GetGame()->GetMainPlayer()->GetExID() == stFacMemInfo.MemberGuid )
    {
        GetGame()->GetMainPlayer()->SetFactionID(FactionGuid);

        m_pFactionManager->SetMyJob( stFacMemInfo.lJobLvl );

        // "���Ѿ�������壺 %s"
        TCHAR	szPrompt[ 1024 ];
        wsprintf( szPrompt, AppFrame::GetText("FA_308"), GetGame()->GetMainPlayer()->GetFactionName().c_str() );
        GetInst(MsgEventManager).PushEvent(Msg_Ok,szPrompt);
    }

    FireUIEvent("FactionPage","UpdateFacAppList");
}

void  FactionOper::RecvKickNotifyMsg(CMessage* pMsg)
{
    TCHAR	strPrompt[ 1024 ];

    // �߳���Ա�ļ��� GUID
    CGUID	guidFaction;
    pMsg->GetGUID( guidFaction );

    // ���ߵ���Ա GUID
    CGUID	guidMember;
    pMsg->GetGUID( guidMember );

    // ���ߵ���Ա ����
    TCHAR	nameMember[ MAX_MEMBER_NAME_SIZE ];
    pMsg->GetStr( nameMember, MAX_MEMBER_NAME_SIZE );

    // �Լ������GUID
    CGUID	guidMyFaction = GetGame()->GetMainPlayer()->GetFactionID();

    // ���������������е����
    for( std::list<CBaseObject*>::iterator p = GetGame()->GetRegion()->GetObjectList()->begin(); p != GetGame()->GetRegion()->GetObjectList()->end(); p++ )
    {
        if( (*p)->GetType() == TYPE_PLAYER )
        {
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p);

            // ���˳���Ա�ļ�����Ϣ���
            if( pPlayer->GetExID() == guidMember )
            {
                // ����Լ��ļ����־��Ϣ
                pPlayer->SetFactionID( NULL_GUID );
                pPlayer->SetUnionID( NULL_GUID );
                pPlayer->SetFactionName( std::string() );
                pPlayer->SetFactionTitle( std::string() );

                break;
            }
        }
    }

    // ��������Լ�����ģ��򷵻�
    if( guidMyFaction != guidFaction )
        return;

    // ���Ƿ����Լ�����
    if( GetGame()->GetMainPlayer()->GetExID() == guidMember )
    {   
        // �رս���
        FireUIEvent("FactionPage","Closed");

        // "���Ѿ����߳����壡"
        wsprintf( strPrompt, AppFrame::GetText("FA_304"), GetGame()->GetMainPlayer()->GetFactionName().c_str() ); //���Ѿ����߳���ᣡ
        GetInst(MsgEventManager).PushEvent(Msg_Ok,strPrompt);

    }
    // �������
    else if( GetGame()->GetMainPlayer()->GetFactionID() == guidFaction )
    {
        // "%s �Ѿ����߳����壡"
        wsprintf( strPrompt, AppFrame::GetText("FA_111"), nameMember );   //%s �Ѿ����߳��лᣡ
        //GetMainBar()->AddOtherInfo( strPrompt );
        GetInst(ChatListPage).AddChatWords(strPrompt,0xffffffff,0,0xff000000,"",CHAT_BOARDCAST);

        // ɾ��
        m_pFactionManager->GetMyFaction()->DelMember( guidMember );
        FireUIEvent("FactionPage","UpdateMem");
    }
}

void FactionOper::RecvKickMsg(CMessage* pMsg)
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        // ���ߵ���Ա GUID
        CGUID	guidMember;
        pMsg->GetGUID( guidMember );

        //
        if( CFacMember* pMember = m_pFactionManager->GetMyConfrere( guidMember ) )
        {
            // "%s �Ѿ����߳������壡"
            TCHAR	strPrompt[ 256 ];
            wsprintf( strPrompt, AppFrame::GetText("FA_107"), pMember->GetName());  //%s �Ѿ����߳�����ᣡ
            GetInst(ChatListPage).AddChatWords(strPrompt,0xffffffff,0,0xff000000,"",CHAT_SYSTEM);
        }
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::RecvDisbandMsg(CMessage* pMsg)
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        // �رս���
        FireUIEvent("FactionPage","Closed");

        // "�����ڼ����Ѿ���ɢ��"
        const char* pStr =AppFrame::GetText("FA_309") ;
        GetInst(ChatListPage).AddChatWords(pStr,0xffffffff,0,0xff000000,"",CHAT_SYSTEM);

        // ����ü������Ϣ
        m_pFactionManager->DelFaction( GetGame()->GetMainPlayer()->GetFactionID() );
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::RecvDisbandNotifyMsg(CMessage* pMsg)
{
    // ��ɢ�ļ��� GUID
    CGUID	guidFaction;
    pMsg->GetGUID( guidFaction );

    // �����Լ�
    if( m_pFactionManager->AmIJoinedFaction( guidFaction ) )
    {
        // �رս���
        FireUIEvent("FactionPage","Closed");

        // "�����ڵļ����Ѿ���ɢ��"
        const char* pStr =AppFrame::GetText("FA_309") ;
        GetInst(ChatListPage).AddChatWords(pStr,0xffffffff,0,0xff000000,"",CHAT_SYSTEM);   
    }

    // ����ü������Ϣ
    m_pFactionManager->DelFaction( guidFaction );
}

void  FactionOper::RecvMemberOnlineNotifyMsg(CMessage* pMsg)
{
    // ��ԱGUID
    CGUID	guidMember;
    pMsg->GetGUID( guidMember );

    bool bIsOnline = pMsg->GetLong() != 0;

    // ��������
    if( CFacMember*	pMember = m_pFactionManager->GetMyConfrere( guidMember ) )
    {
        pMember->SetOnline( bIsOnline );
    }
    FireUIEvent("FactionPage","UpdateMem");
}

void  FactionOper::RecvMemberMapNotifyeMsg(CMessage *pMsg)
{
    // �����ùٽ׵���ԱGUID
    CGUID	guidMember;
    pMsg->GetGUID( guidMember );

    // ��ͼ
    TCHAR	szRegion[ MAX_MEMBER_NAME_SIZE ];
    pMsg->GetStr( szRegion, MAX_MEMBER_NAME_SIZE );

    // ��������
    if( CFacMember*	pMember = m_pFactionManager->GetMyConfrere( guidMember ) )
    {
        pMember->SetRegion( szRegion );
    }
    FireUIEvent("FactionPage","UpdateMem");
}

void  FactionOper::RecvSetChairmanMsg(CMessage* pMsg)
{
    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        m_pFactionManager->SetMyJob( 0 );
        FireUIEvent("FactionPage","UpdateMem");
    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::RecvSetChairmanNotifyMsg(CMessage* pMsg)
{
    TCHAR	szPrompt[ 256 ];

    // ���峤 ����
    TCHAR	nameOldMaster[ MAX_MEMBER_NAME_SIZE ];
    pMsg->GetStr( nameOldMaster, MAX_MEMBER_NAME_SIZE );

    // ���峤 ����
    TCHAR	nameNewMaster[ MAX_MEMBER_NAME_SIZE ];
    pMsg->GetStr( nameNewMaster, MAX_MEMBER_NAME_SIZE );

    // ����������峤
    if( ! lstrcmp( GetGame()->GetMainPlayer()->GetName(), nameOldMaster ) )
    {
        // "���Ѿ����峤ְλת�ø� %s��"
        wsprintf( szPrompt, AppFrame::GetText("FA_104") , nameNewMaster );
        GetInst(MsgEventManager).PushEvent(Msg_Ok,szPrompt);

        // �޸Ĺٽ�
        m_pFactionManager->SetMyJob( 0 );
    }
    // ����������峤
    else if( ! lstrcmp( GetGame()->GetMainPlayer()->GetName(), nameNewMaster ) )
    {
        // "%s ������Ϊ %s �����峤��"
        wsprintf( szPrompt, AppFrame::GetText("FA_302") , nameOldMaster, m_pFactionManager->GetMyFaction()->GetName() );
        GetInst(MsgEventManager).PushEvent(Msg_Ok,szPrompt);

        // ������Ϊ�峤���������
        m_pFactionManager->SetMyJob( Exceptive_JobLvl );
    }
    // ������ҵĻ���
    else if( m_pFactionManager->GetMyFaction() )
    {
        // "%s ���� %s Ϊ %s �����峤��"
        wsprintf( szPrompt, AppFrame::GetText("FA_303") , nameOldMaster, m_pFactionManager->GetMyFaction()->GetName() );
        GetInst(ChatListPage).AddChatWords(szPrompt,0xffffffff,0,0xff000000,"",CHAT_SYSTEM);
    }

    // ���� Member ����
    if( CFacMember* pMember = m_pFactionManager->GetMyConfrere( nameOldMaster ) )
    {
        pMember->SetJob( 0 );
    }

    if( CFacMember* pMember = m_pFactionManager->GetMyConfrere( nameNewMaster ) )
    {
        pMember->SetJob( Exceptive_JobLvl );
    }

    // ˢ�½���
    FireUIEvent("FactionPage","UpdateMem");
}

void  FactionOper::RecvMemberLevelNotifyMsg(CMessage* pMsg)
{
    // �����ùٽ׵���ԱGUID
    CGUID	guidMember;
    pMsg->GetGUID( guidMember );

    long lLevel = pMsg->GetLong();

    // ��������
    if( CFacMember*	pMember = m_pFactionManager->GetMyConfrere( guidMember ) )
    {
        pMember->SetLevel( lLevel );
    }

    FireUIEvent("FactionPage","UpdateMem");
}

void  FactionOper::RecvMemberOccuNotifyMsg(CMessage* pMsg)
{
    // �����ùٽ׵���ԱGUID
    CGUID	guidMember;
    pMsg->GetGUID( guidMember );

    long lOccu = pMsg->GetLong();

    // ��������
    if( CFacMember*	pMember = m_pFactionManager->GetMyConfrere( guidMember ) )
    {
        pMember->SetOccupation( lOccu );
    }

    // ˢ��
    FireUIEvent("FactionPage","UpdateMem");
}

void  FactionOper::RecvOpenMsg(CMessage* pMsg)
{

}

void  FactionOper::RecvUpdatePlayerFacInfoMsg(CMessage* pMsg)
{
    // ��� GUID
    CGUID guidPlayer;
    pMsg->GetGUID( guidPlayer );

    // ���ڼ��� GUID
    CGUID guidFaction;
    pMsg->GetGUID( guidFaction );

    // ��Ҽ���ƺ�
    TCHAR szFacTitle[ MAX_MEMBER_NAME_SIZE ];
    pMsg->GetStr( szFacTitle, MAX_MEMBER_NAME_SIZE );

    // ���������������е����
    for( std::list<CBaseObject*>::iterator p = GetGame()->GetRegion()->GetObjectList()->begin(); p != GetGame()->GetRegion()->GetObjectList()->end(); p++ )
    {
        if( (*p)->GetType() == TYPE_PLAYER )
        {
            CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p);

            // ������µ����
            if( pPlayer->GetExID() == guidPlayer )
            {
                pPlayer->SetFactionID( guidFaction );
                pPlayer->SetFactionTitle( std::string( szFacTitle ) );
                pPlayer->SetFactionName( std::string( m_pFactionManager->GetFaction( guidFaction )->GetName() ) );
            }
        }
    }

    // ˢ�½���
    FireUIEvent("FactionPage","UpdateMem");
}

void  FactionOper::RecvInviteRequestMsg(CMessage* pMsg)
{
    // 
    TCHAR	szPrompt[ 256 ];

    // ��� �ɹ�ʧ��
    if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
    {
        TCHAR	namePlayer[ MAX_MEMBER_NAME_SIZE ];
        pMsg->GetStr( namePlayer, MAX_MEMBER_NAME_SIZE );

        // "%s �Ѿ�ͬ���������������"
        wsprintf( szPrompt, AppFrame::GetText("FA_108" ) ,namePlayer);
        GetInst(ChatListPage).AddChatWords(szPrompt,0xffffffff,0,0xff000000,"",CHAT_SYSTEM);

    }
    else
    {
        // �������
        m_pFactionManager->HandleErrorMsg( pMsg );
    }
}

void  FactionOper::RecvInviteAnswerMsg(CMessage* pMsg)
{
    SInviterInfo*	pstInviterInfo = new SInviterInfo;

    // ������GUID
    pMsg->GetGUID( pstInviterInfo->guidInviter );

    // ����������
    pMsg->GetStr( pstInviterInfo->nameInviter, MAX_MEMBER_NAME_SIZE );

    // ����GUID
    pMsg->GetGUID( pstInviterInfo->guidFaction );

    // ��������
    pMsg->GetStr( pstInviterInfo->nameFaction, MAXSIZE_FactionName );

    // "%s ������������壺%s"
    TCHAR	strPrompt[ 1024 ];
    wsprintf( strPrompt, AppFrame::GetText("FA_106"), pstInviterInfo->nameInviter, pstInviterInfo->nameFaction );
    GetInst(MsgEventManager).PushEvent(Msg_Agree,strPrompt,uFactionOper::FactionInviteAnswer,pstInviterInfo);
}

void  FactionOper::CreateFactionUi()
{
    // "����δ������壡"
    if( !m_pFactionManager->AmIJoinedFaction() )
    {
        FireUIEvent("CreateFaction","Opened");
        return ;
    }
    else
    {

    }
    
}