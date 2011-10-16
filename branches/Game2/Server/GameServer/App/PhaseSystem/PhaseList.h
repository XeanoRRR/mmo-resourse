/** 
 * @file   PhaseList.h  
 * @author Expter
 * @brief  ��¼��ǰ��λ����Һ͹����б�,�Լ��¼���Ӧ
 * @date   2010/03/18        
 */
#pragma once 

#include "../../../../Public/Common/PhaseDef.h"
#include "../../../../Public/ServerPublic/Server/BaseDef/PhaseSeverDef.h"

class  PhasePro;
class  PhaseList
{
    typedef std::map<ePhaseRest,std::list<CPlayer*> > MPlayer;
    typedef MPlayer::iterator                    MPIER;
    
public:
     PhaseList(PhasePro* );
     ~PhaseList();

public:

     ///  �����Ϣ
     void Release();

     ///  ����һ��Player
     void AddPlayer(ePhaseRest e , CPlayer* player);

     ///  �Ƴ�һ��Player
     void RemovePlayer(ePhaseRest e,CPlayer* player);
     
     ///  ����һ��Monster
     void AddMonster(CMonster* Monster);

     ///  �Ƴ�һ��Monster
     void RemoveMonster(CMonster* monster);

     ///  �õ�Player��Ŀ
     long GetPhasePlayerNum( ePhaseRest e ) const;

     ///  �õ�Monster��Ŀ
     long GetPhaseMonsterNum() const;

     ///  Ͷ��һ��pMsg
     void PostPhaseMsg(const char* pMsg,ulong dwColor, ulong dwBkColor,long ePos);

     ///  ÿһ����Ҵ���һ���ű�
     long PhaseExitEvent( );

private:

    MPlayer               m_PlayerList;
    std::list<CMonster*>  m_MonsterList;
    PhasePro*             m_PhasePro;
};