/** 
 * @file   PhaseOper.h  
 * @author Expter
 * @brief  ����һ����λȫ�����ܼ���,��λ�����в���,��λ�赲����ű�����,
 *         ͬʱ������λ�е���Һ͹��
 *
 * @date   2010/03/18        
 */

#pragma once 

#include "../../../../Public/Common/PhaseDef.h"
#include "../../../../Public/ServerPublic/Server/BaseDef/PhaseSeverDef.h"


class Script;
class CPlayer;
class GSTeam;
class CServerRegion;
class PhaseList;

class PhasePro : public CGameEvent
{
    struct   PhasePlace
    {
        /// ����
        float  x_point;
        float  y_point;
        /// ��Χ1��2�Ĵ�С
        float  a_size;
        float  b_size;
        CServerRegion*   pRegion;        
    };

    /// Typedef 
    typedef PhasePro self;
    typedef std::map<ePhaseRest,list<GSTeam*> >  MTEAMTABLE;
    typedef MTEAMTABLE::iterator                 MITER;
    friend  class  PhaseList;

public:
    /// ���캯��
    PhasePro();
    ~PhasePro();

    /// �ͷ�
    void  Release();

    /// 
    /// ��λ������ʼ��firteam��ʾ��һ������ָ��,secteamΪ�ڶ�������ָ��
    /// ˵��:(��ǰ�����ǵ�����һ����2�������Ķ��������λ����)
    ///       ���Ϊ�������Ժ�Ķ�����鴥����ʽֻ���޸Ľӿ�Ϊlist<GSteam*>����.
    /// 
    void  Init(CPlayer* first,GSTeam* firteam ,
               long num,float size1,float size2,
               ePhaseType type,eTrigger triggertype,
               CPlayer* second=NULL,
               GSTeam* secteam=NULL);

    /// ����
    void  ActiveSelf(eTimerType etype,long lTimer,long lExipires);

    /// ����������Ҳ���
    long  OnPhaseOper(CPlayer* player,ePhaseOper Oper,ePhaseRest Rest = ePRest);

    /// ��Ӧ����Ĳ����¼�
    long  OnPhaseOper(CMonster* monster,ePhaseMOper Oper);

    /// ��λ�¼�
    long  OnPhaseEvent(CMoveShape* shape,ePhaseEvent Event);
  
    /// ���нű��ļ�
    void  RunScript(CPlayer* player,const char *filename);

    /// �ű�����
    void  RunScript(CPlayer* player,long id,long type);

    /// �ű�����
    void  RunScript(const char* filename);

    /// ��������
    void  CreateMonster(const char* OrgName,float x_point,float y_point);

    /// �������赲�ж�
    bool  IsPhaseBlock(CMoveShape* shape,long xNew,long yNew,long type=0);

    /// ��ʱ��Ӧ
    long  OnTimeOut(ulong timerid,ulong curtime,const void* var);

    /// ��ʱ��ע��ʱ���õĺ�����������ɾ��һЩ���Ӳ���
    void  OnTimerCancel( DWORD timerid,const void *var);

    /// ��ʱ����Դ�ͷ�
    void  CancelTimer( ) ;

    /// ��Ϣ����
    void  OnMessage(CMessage* pMsg){    }

    /// ����
    long  Proc(eEventType type,const tagProcVar* pVar){ return 0; }

    /// ����ǰ��λ��������ҷ�����Ϣ������ΪMsg
    void  PostSelfMsg(const char* Msg,ulong dwColor, ulong dwBkColor,long ePos);

    /// Get/Set��������
public:
  
    /// ������λ��ID
    const CGUID& GetID() const         { return m_Id ;      }

    /// ������λ����
    ePhaseType   GetPhaseType() const  { return m_type;     }

    /// ������λ��״̬
    ePhaseState  GetPhaseStates()const {  return m_state;   } 
    
    ///  ������λ�������Ǳ���
    eTrigger     GetPhaseTrigger()const{ return m_triggertype;}

    /// Get/Set��ҵĽ������нű�
    void  SetEndScriptFile(const char* filename){  m_Endfile = filename;}

    const char* GetEndScriptFile() const   {  return m_Endfile.c_str();}

    /// ������λ����ʱ��
    long  GetCostTime() const  ;

    /// �����������
    long  GetPlayerNum() const ;

    /// ���ض����������
    long  GetPlayerNum(ePhaseRest e) const;

    /// ���ع�������
    long  GetMonsterNum()const ;
   
    /// ������λ������Ϣ
    long  GetPointInfo(const char* pName)const;

    void  SetPointInfo(const char*pName,double Value);

    /// �����¼���״̬
    void  SetPhaseStates(ePhaseState state,bool flag = false);
  
    /// ˽�к���
private:
    
    /// ����һ������ӳ��
    void  AddTeadMember(ePhaseRest type,GSTeam* team);

    /// �Ƴ���λ���
    void  RemovePlayer(CPlayer* player);

    /// �Ƴ���λ����
    void  RemoveMonster(CPlayer* Monster);

    /// ������ʱ��
    void  StarUpTimer();

    /// ���˲���
    void  RemitPlayer();

    /// �ж��Ƿ������־
    bool  IsEnd();

private:
    /// ��λ����(Ұս,����)
    ePhaseType      m_type;
    /// ��ʱ������(Sub,Add)
    eTimerType      m_timertype;
    /// ��ʱ������(����,����)
    eTrigger        m_triggertype;
    /// ��λ����״̬,������,���н���
    ePhaseState     m_state;    
    /// ��λ�Ĵ���ʱ��
    long            m_lTime;
    /// ��λ�ĳ�ʱʱ��
    long            m_lExpires;
    /// ��λ��ʱ��Timer
    long            m_lTimer;
    /// ��ʼʱ��
    long            m_StartTime;
    /// ����ʱ��
    long            m_EndTime;
    /// ����ִ�нű�
    std::string     m_Endfile;
    /// shape�б�
    PhaseList*      m_PhaseList;
    /// ��λ��ID
    CGUID           m_Id;
    /// ��λ��Place��Ϣ
    PhasePlace      m_Place;
    /// ��λ�����( Ԥ���������Ĵ�����ʽ )
    MTEAMTABLE      m_ProTable; 
 };