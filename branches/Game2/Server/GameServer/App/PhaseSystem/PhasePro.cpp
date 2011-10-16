#include "StdAfx.h"
#include "PhasePro.h"
#include "PhaseList.h"
#include "PhaseOtherDef.h"
#include "FollowManager.h"
#include "../Player.h"
#include "../OrganizingSystem/GSTeam.h"
#include "../LuaScript/Script.h"
#include "../LuaScript/ScriptSys.h" 
#include "../RgnManager.h"
#include "../../Server/GameManager.h"
#include "../../../../Public/Setup/PhaseSetup.h"


PhasePro::PhasePro()
{
   m_PhaseList = NULL;
   m_ProTable.clear();
   m_lTimer = m_lTime = m_lExpires= 0;
   CGUID::CreateGUID( m_Id );
}

PhasePro::~PhasePro()
{
    Release();
}

void  PhasePro::Release()
{  
    for ( MITER itr (m_ProTable.begin()) ; itr != m_ProTable.end() ; itr ++ )
    {
        std::list<GSTeam*>::iterator it = itr->second.begin();
        for( ; it != itr->second.end() ; it++  )
            if(*it != NULL)   (*it)->SetPhaseID( NULL_GUID );
    }
    GameManager::GetInstance()->Cancel(m_lTimer);
    SAFE_DELETE( m_PhaseList );        
    m_ProTable.clear();
}

void  PhasePro::Init(CPlayer* first , GSTeam* firteam ,
                     long num,float size1,float size2,
                     ePhaseType type,eTrigger triggertype,
                     CPlayer* second,GSTeam* secteam)
{
    m_PhaseList = new PhaseList(this);

    m_Place.a_size = size1;
    m_Place.b_size = size2;
    m_Place.x_point= first->GetPosX();
    m_Place.y_point= first->GetPosY();
    m_Place.pRegion = static_cast<CServerRegion*>(first->GetFather());
    m_triggertype  = triggertype;
    m_type = type;

    if ( type == eFight )
    {   
        ///  һ������
        if ( firteam )
        {
            AddTeadMember( ePRest , firteam );
        }
        else
        {
            OnPhaseOper( first , eRemit );
        }
    }
    else if( type == ePk )
    {
        /// ��ȡ���еĶ�����Ϣ
        if( firteam && secteam)
        {
            AddTeadMember( ePRest , firteam );
            AddTeadMember( ePInVitee , secteam );
        }
        else
        {
            OnPhaseOper( first , eRemit );
            OnPhaseOper( second , eRemit );
        }
    }
    else
    {
        return ;
    }
    
    char  str[200];     
    sprintf_s( str, "��λ�����ɹ�(%d,%d,%f,%f,%f,%f,%d,%d)" ,type,triggertype,
                    m_Place.x_point,m_Place.y_point,m_Place.a_size,
                    m_Place.b_size,GetPlayerNum(),GetMonsterNum());
    PRINT_PHASE_TO_CLIENT(str,first->GetExID());
}

void  PhasePro::ActiveSelf(eTimerType etype,long lTimer,long lExipires)
{  
    m_lTime = lTimer;
    m_lExpires= lExipires;
    m_timertype= etype;
    SetPhaseStates( eRun );
    StarUpTimer( );
}

long  PhasePro::OnPhaseOper(CPlayer* player,ePhaseOper Oper,ePhaseRest ret)
{
    if( !player)     return 0;

    char  str[200]={0};
    switch( Oper )
    {
    case eRemit:
        {
            /// �������ͬһ������,���Ѿ�����һ����λ
            if ( player->GetFather() != m_Place.pRegion || player->GetPhaseID() != NULL_GUID )
            {
                sprintf_s( str, "��� %s ����ͬһ���������Ѿ�������λ�����ܽ������˲���" , player->GetName() );
                break;
            }

            float dLen = Distance( player->GetPosX() , player->GetPosY() , m_Place.x_point , m_Place.y_point );
            if ( dLen > m_Place.b_size )
            {
                sprintf_s( str, "��� %s �ڷ�Χ2֮�� ���ܽ�����λ����" , player->GetName() , m_Place.x_point ,m_Place.y_point );
                break;
            }

            m_PhaseList->AddPlayer( ret , player );
            
            if ( player->GetCurrentProgress() == CPlayer::PROGRESS_FOLLOW_LEADER )
            {
                player->SetCurrentProgress( CPlayer::PROGRESS_NONE );
               
                GetInst(FollowManager).StopFollow( player );
            }
            
            /// �������ǵ����˵�PK������˲������
            if ( m_type == ePk && player->GetTeamID() == NULL_GUID )
            {                                          
                break;
            }
            else if ( m_type == ePk )
            {
                /// ����������˲��λ��
            }

            if ( dLen > m_Place.a_size )
            {
                RunScript(player,(long) eRemit,eSc_Oper);
                sprintf_s( str, "��� %s �ڷ�Χ2�ڱ�˲�Ƶ�λ�� %f,%f" , player->GetName() , m_Place.x_point ,m_Place.y_point );
            }   
        }
        /// ����һ���ű�
        break;

        /// ��������뿪������߱��߿�����
    case eLeave:
        {  
            RemovePlayer( player );
            /// ���ͻ��˷�����Ϣ
            sprintf_s( str, "��� %s�뿪�˶��飬��T����λ " , player->GetName()  );
            /// ����һ���ű�
            RunScript(player,(long) eLeave ,eSc_Oper);

            if(IsEnd())
            {
                SetPhaseStates( eClose , true );
            }
        }
        break;

        /// �������ܲ���
    case eEscape:
        {
            RemovePlayer(player);
            /// ���ͻ��˷�����Ϣ
            sprintf_s( str, "��� %sͨ�������뿪����λ " , player->GetName() );
            /// ����һ���ű�
            RunScript(player,(long) eEscape ,eSc_Oper);

            if(IsEnd())
            {
                SetPhaseStates( eClose , true );
            }
        }     
        break;
        /// ��ҹ�Ӳ���
    case eReJoin:
        {
            m_PhaseList->AddPlayer( ret , player );
            if ( 1 )
            {
                player->ChangeRegion( RGN_NORMAL,m_Place.pRegion->GetExID(),
                (long)m_Place.x_point , (long)m_Place.y_point , 0 );
                sprintf_s( str, "��� %s �ڷ�Χ2�����߽��й�λ������λ�� %f,%f" , player->GetName() , m_Place.x_point ,m_Place.y_point );
            }
        }
        break;
    }
   Log4c::Trace(ROOT_MODULE,str);
    return 0;
}

long  PhasePro::OnPhaseOper(CMonster* Monster,ePhaseMOper Oper)
{    
    switch( Oper )
    {
    case eMonsterDie:
        {
            m_PhaseList->RemoveMonster( Monster );
            if ( m_PhaseList->GetPhaseMonsterNum() == 0)
            {
                SetPhaseStates( eEnd );
            }
        }
        break;

    case eAddToPhase:
        {
            m_PhaseList->AddMonster( Monster );
        }
        break;
    }
    return 0;
}

long  PhasePro::OnPhaseEvent(CMoveShape* shape, ePhaseEvent Event)
{
    switch( Event )
    {
        /// �����ű�����
    case eAgonal:
        {
            CPlayer* player = dynamic_cast<CPlayer*>(shape);
            if( !player ) return 0;
            /// ����һ�������Ľű�
            RunScript(player,(long) eAgonal,eSc_Event);
        }
        break;
        /// �����������
    case eDie:
        {
            CPlayer* player = dynamic_cast<CPlayer*>(shape);
            if( !player ) return 0;

            RemovePlayer(player);
            
            /// ����һ�����������Ľű�
            RunScript(player,(long) eDie ,eSc_Event);

            if(IsEnd())
            {
                SetPhaseStates(eClose ,true);
            }
        }
        break;

    case eSecKill:
        {
            CPlayer* player = dynamic_cast<CPlayer*>(shape);
            if( !player ) return 0;

            RemovePlayer(player);

            /// ����һ�����������Ľű�
            RunScript(player,(long) eSecKill ,eSc_Event);

            if(IsEnd())
            {
                SetPhaseStates( eClose , true );
            }
        }
        break;
    case eExit:
        {
            if( m_PhaseList != NULL )
            {
              SetPhaseStates( eEnd );
              return m_PhaseList->PhaseExitEvent( );
            }
        }
    }
     return 1;
}

void  PhasePro::StarUpTimer( )
{
    int lTime = m_lTime * 1000;
    tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
    pTimvar->TimerType = (eChildObjType)m_timertype;
    m_EndTime = m_StartTime = timeGetTime();
    m_lTimer= GameManager::GetInstance()->Schedule(this,(void*)pTimvar, m_StartTime + lTime,m_lExpires*1000);

    Log4c::Trace(ROOT_MODULE,"Phase Timer is %d,TimeOut Time:%d,Expires:%d" ,m_timertype,m_lTime,m_lExpires);
    return ;
}


long  PhasePro::OnTimeOut(ulong timerid,ulong curtime,const void* var)
{
      const tagTimerVar *pTimervar = (const tagTimerVar *)var	;
      assert(pTimervar);
      switch( pTimervar->TimerType )
      {
      case  eAdd:
          {
              Log4c::Trace(ROOT_MODULE,"Phase %s TimeOut Execute !",DisPlay(eSub));

              RunScript( GetInst(PhaseSetup).GetScript( eAdd,eSc_Ttype));
          }
          break;

      case eSub:
          {   
              Log4c::Warn(ROOT_MODULE,"Phase End ,Phase %s TimeOut Execute !",DisPlay(eSub));

              RunScript( GetInst(PhaseSetup).GetScript( eSub,eSc_Ttype));
          }
          break;
      }
      return 0;
}

void  PhasePro::CancelTimer()
{
      GameManager::GetInstance()->Cancel( m_lTimer );
      m_lTimer = -1;
      m_EndTime = timeGetTime();
}

void  PhasePro::OnTimerCancel(DWORD timerid,const void *var)
{
    tagTimerVar *pTimervar = (tagTimerVar*)var;
    OBJ_RELEASE(tagTimerVar, pTimervar );
}

long  PhasePro::GetCostTime( )const
{
    if( m_lTimer > 0)
        return timeGetTime() - m_StartTime; 
    else
        return m_EndTime - m_StartTime;        
}

long  PhasePro::GetPlayerNum( ) const
{
    return m_PhaseList->GetPhasePlayerNum( ePRest ) + m_PhaseList->GetPhasePlayerNum( ePInVitee );
}

long  PhasePro::GetPlayerNum(ePhaseRest e) const 
{
    return m_PhaseList->GetPhasePlayerNum( e );
}

long  PhasePro::GetMonsterNum() const 
{
    return m_PhaseList->GetPhaseMonsterNum();
}

/// ����Id
void  PhasePro::RunScript(CPlayer* player ,long id , long type)
{
    assert(player);
    Script::RunEnv env( const_cast<CPlayer*>( player ), const_cast<CPlayer*>( player ), 
        m_Place.pRegion, NULL ,m_Id );

    GetInst( ScriptSys ).RunScript( env, GetInst(PhaseSetup).GetScript( id ,type));
}

void  PhasePro::RunScript(CPlayer* player , const char *filename)
{
    assert(player);
    Script::RunEnv env( const_cast<CPlayer*>( player ), const_cast<CPlayer*>( player ), 
        m_Place.pRegion, NULL ,m_Id );

    GetInst( ScriptSys ).RunScript( env, filename );
}

void  PhasePro::RunScript(const char* filename)
{
    Script::RunEnv env( NULL, NULL, m_Place.pRegion , NULL , m_Id );

    GetInst( ScriptSys ).RunScript( env, filename );
}

void  PhasePro::AddTeadMember(ePhaseRest type , GSTeam* team )
{
   if ( !team ) return ;
   m_ProTable[ type ].push_back( team );

   list<CGUID> members;
   team->GetMemberList(members);
   team->SetPhaseID( m_Id );
   for (list<CGUID>::iterator ite = members.begin(); ite != members.end(); ++ite)
   {
       OnPhaseOper( GetGame()->FindPlayer( *ite ) , eRemit , type);
   }
}

void   PhasePro::RemovePlayer(CPlayer* player)
{
    m_PhaseList->RemovePlayer(player->GetPhaseRest(),player );
}

bool    PhasePro::IsEnd()
{
    /// ���������˵����һ�������������=0��ʱ�������λ����
    if ( m_type == eFight )
    {
         long num = m_PhaseList->GetPhasePlayerNum( ePRest )  ;
         if( num <= 0)
            return true;
    }
    else 
    {
         long num1 = m_PhaseList->GetPhasePlayerNum( ePRest )  ;
         long num2 = m_PhaseList->GetPhasePlayerNum( ePInVitee);
         if ( num1<=0 || num2 <=0 )
             return true;
    }

    return false;
}

long   PhasePro::GetPointInfo(const char* pName) const
{
    if ( strcmp("x_point" ,pName) == 0 )
        return (long)m_Place.x_point;
    else if ( strcmp("y_point",pName)== 0)
        return (long)m_Place.y_point;
    else if ( strcmp("a_size" ,pName)== 0)
        return (long)m_Place.a_size;
    else if ( strcmp("b_size" ,pName)== 0)
        return (long)m_Place.b_size;
    else if ( strcmp("PhaseType",pName)== 0)
        return (long)m_type;
    else if ( strcmp("RegionID",pName)==0 )
    {
        if( m_Place.pRegion != NULL )
            return (long)m_Place.pRegion->GetID();
    }
   
    return 0;
}

void   PhasePro::SetPhaseStates(ePhaseState state , bool flag )
{
    if ( m_state == state )
        return ;
    m_state = state;  
    if ( (state == eEnd && m_triggertype == eActive) ||  flag )
        CancelTimer();
}

void   PhasePro::CreateMonster(const char* OrgName , float x_point , float y_point)
{
    CMonster* pMonster = m_Place.pRegion->AddMonster( OrgName , (long)(m_Place.x_point +  x_point), (long)(m_Place.y_point + y_point), 0 );

    m_PhaseList->AddMonster( pMonster );
}

bool  PhasePro::IsPhaseBlock(CMoveShape* shape,long xNew,long yNew,long Type)
{
    float dLen = Distance( (float)xNew, (float)yNew , m_Place.x_point , m_Place.y_point );
    /// �����߳���Χ1��  
    if ( dLen >= m_Place.a_size && Type == 0)
    {
      Log4c::Warn(ROOT_MODULE,"���%s��ͼ�뿪���ڵ���λ,��λ�赲������ֹ" ,shape->GetName());
       return true;
    }
    else if ( dLen <= m_Place.b_size && Type == 1)
    {
        if( !shape ) return false;

        float wLen = Distance( shape->GetPosX() , shape->GetPosY() , m_Place.x_point , m_Place.y_point );
        /// ���������ǿ��Ե�
        if (  wLen < dLen )
        {
            Log4c::Warn(ROOT_MODULE,"���%s��ͼԶ���Ѿ���ʼ�Ķ�����λ,��������λ�赲" ,shape->GetName());
            return false;
        }

        Log4c::Warn(ROOT_MODULE,"���%s��ͼ�����Ѿ���ʼ�Ķ�����λ,��λ�赲������ֹ" ,shape->GetName());
        return true;
    }
    return false;

}

void   PhasePro::PostSelfMsg(const char* Msg,ulong dwColor, ulong dwBkColor,long ePos)
{
    m_PhaseList->PostPhaseMsg( Msg ,dwColor,dwColor,ePos);
}