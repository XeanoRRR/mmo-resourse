/** 
 * @file   PhaseManager.h  
 * @author Expter
 * @brief  �������е���λ,�Լ���λ�赲,�ű��ӿ�,��Ϣ����Ӧ,
 *         �ṩ���ⲿ������λ���ܵ���
 *
 * @date   2010/03/18        
 */
#pragma  once

#include "../session/CSession.h"
#include "../OrganizingSystem/GameOrganizingCtrl.h"
#include "../../../../Public/Common/PhaseDef.h"
#include "../../../../Public/ServerPublic/Server/BaseDef/PhaseSeverDef.h"

#define  RNULL ;
#define  PHASEMAN  GetInst(PhaseManager)
#define  PhaseOpen(ret)   if(GetInst(PhaseSetup).PhaseEnable() == 0 )   return ret;

class PhasePro;
class PhaseManager
{
    struct  ApplyList 
    {
        CGUID first;    /// ��������ID
        CSession* second;   /// SessionsID
        ApplyList( CGUID f,CSession* s):first(f),second(s)
        {
        }
    };

    typedef    stdext::hash_map<CGUID,PhasePro*,hash_guid_compare>   PhaseTable;
    typedef    PhaseTable::iterator                                  PIter;
    typedef    stdext::hash_map<CGUID,CGUID,hash_guid_compare>       PhaseInvite;
    typedef    PhaseInvite::iterator                                 PIVter;
    typedef    std::map<CGUID,ApplyList>                             AppMap;
    typedef    AppMap::iterator                                      AppItr;

public:
    /// ���캯��
    PhaseManager();
    ~PhaseManager();

    /// �ͷ�������Դ
    void  Release();

    void  Release(const CGUID& GID);

    /// 
    /// ���һ���������λ���򴴽�(Ұ��ս������)
    /// @param  player  ���ڴ�����λ���ܵ����
    /// @param  size1   ��Χ1�Ĵ�С
    /// @param  size2   ��Χ2�Ĵ�С
    /// @param  trigger ��λ�������ͣ�����������
    /// @param  scriptfile ��λ���������Ҫ�����Ľű�
    /// 
    void   CreatePhase(CPlayer* player,float size1,
                       float size2,eTrigger trigger,
                       const char* Scriptfile);

    ///
    /// ����һ����������λ��������ǹ��ܴ���
    /// @param  first   �������󴴽�������λ���ܵ����ID
    /// @param  second  ���ڽ������봴����λ���ܵ����ID
    /// @param  size1   ��Χ1�Ĵ�С
    /// @param  size2   ��Χ2�Ĵ�С
    /// @param  trigger ��λ�������ͣ�����������
    /// @param  scriptfile ��λ���������Ҫ�����Ľű�
    /// 
    void   CreatePhase(const CGUID& first,const CGUID& second,
                       float size1,float size2,
                       eTrigger trigger,
                       const char* Scriptfile );

    /// 
    /// ����λ��������x_point , y_point ��������һֻ����
    /// @param   GID        ��λ��GUID
    /// @param   OrgName    ���������ԭʼ��
    /// @param   xy_point   ����������λ���ĵ������
    /// 
    void  CreatePhaseMonster(const CGUID& GID,const char* OrgName,
                             float x_point , float y_point);

    ///  ������λ��Ϣ
    void  OnMessage(CMessage* pMsg);
        
    ///  �õ���λ�ĵ�ǰʣ����������
    long  GetPhasePlayerNum(const CGUID& id);

    /// 
    /// ���ǰ��λ
    /// @param   GID        ��λ��GUID
    /// @param   eTimerType ��λ���������� Add,Sub
    /// @param   lTimer     ��λ����ʱ��
    /// @param   lExipires  ��λ��������
    ///   
    long  ActivePhase(const CGUID& GID,eTimerType etype,
                      long  lTimer , long lExipires);

    /// 
    /// �����λ�������˺����Ǹ�������ͬ�Ĵ����������������
    /// �����Ҹ�����λ�������ͽ��д���
    /// @param   Oper   ��������
    /// 
    long  OnPhaseOper(const CGUID& GID,CPlayer* player,ePhaseOper Oper);

    /// ��λ������Թ���
    long  OnPhaseOper(const CGUID& GID,CMonster* Monster,ePhaseMOper Oper);

    /// ��λ�¼�
    long  OnPhaseEvent(const CGUID& GID,CMoveShape*shape,ePhaseEvent Event);

    ///
    /// shape�����Ƿ����ҹ���
    /// @return  ����1��ʾ��Ҫ����ҹ��ˣ�����0��ʾ����Ҫ����
    /// 
    bool  PhasePlayerFilter(CMoveShape* player,CMoveShape* shape);

    /// 
    /// �����λ��������Ϣֵ
    /// @param  GID    ��λ��GUID
    /// @param  pName  ��ʾ��λ�Ĳ�������
    /// 
    long GetPhasePoint(const CGUID& GID,const char* pName);

    ///
    /// ���һ������ṩһ���ӿڱ�ʾ�����λ����������������Ҳ�����Ǳ���
    /// @param   GID   ��λ��GUID
    /// 
    bool  SetPhaseEnd(const CGUID& GID,CPlayer* player);

    /// �ж�ĳ�����λ�赲
    bool  IsPhaseBlock(CMoveShape* shape,long xNew,long yNew);

    ///
    /// Ϊ����ɽ����Լ���������Ҫ����λ��������һ���ű���
    /// @function   SetPhaseEndScriptName ������λ�������нŲ�
    /// @param   GID        ��λ��GUID
    /// @param   FileName   ��λ�������еĽű�
    /// 
    void  SetPhaseEndScriptName(const CGUID& GID,const char* FileName);

    /// 
    /// �ṩ���ű�һ�����з�����ϵͳ����������λ�Ľӿ�
    /// @param   GID     ��λID
    ///  
    void  ClosePhase(const CGUID& GID);

    /// 
    /// ������λ���ܵĶ�ʱ��ʱ��
    /// @param   GID    ��λID
    /// 
    long  GetCostTime(const CGUID& GID);

    /// 
    /// �ر���λ�Ķ�ʱ������
    /// @param   GID    ��λID
    /// 
    void  CancelPhaseTimer(const CGUID& GID);

    ///  
    /// ������λ������
    /// @param   GID    ��λID
    /// 
    ePhaseType  GetPhaseType(const CGUID& GID);

    ///  
    /// ����λͶ��һ����Ϣ
    /// @param   GID    ��λID
    /// @param   pMsg   ��Ϣ����
    /// 
    void   PostPhaseMsg(const CGUID& GID,const char* pMsg,ulong dwColor, ulong dwBkColor,long ePos);

    /// 
    /// ����ӳ���
    /// @param  first  ������ID
    /// @param  second ������ID
    ///
    bool   AddPhaseInviteMapTable(const CGUID& first,const CGUID& second);

    /// ������λ�������
private:
    void  ClearTimeOut();

private:
    /// ���е���λ�б�
    PhaseTable        m_PhaseList;
    /// �����ߺͱ�������ӳ���
    AppMap            m_InviteList;
};