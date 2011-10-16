/** 
 *  @file   PhaseDef.h
 *  @author
 *
 *  @brief  ������λ���ܵĿͻ������������һЩö�ٺ����Զ���
 *  @date   2010/03/18
 */

#pragma  once 

enum   C2SPhaseOperType
{
    C2S_Invite=0,               /// �����߷�����������
    C2S_Invitee,                /// ��������ͬ�����Pk��λ
    C2S_Leave,                  /// �뿪
    C2S_Escape,                 /// ��������
    C2S_ReJoin,                 /// �������
    S2C_FollowLeader,           /// ����ӳ�����(����)
};

enum   S2CPhaseOperType
{
    S2C_ChangeRgn,              /// �����л�
    S2C_ChangeState,            /// ״̬�ı�
    S2C_EnterPhase,             /// ������λ
    S2C_LeavePhase,             /// �뿪��λ
    S2C_InVitee,                /// ���������λ
    S2C_JoinTimeOut,            /// ���볬ʱ
    
    S2C_ReInvite,               /// �ظ�����
    S2C_TooFar,                 /// ̫Զ
    S2C_RefuseJoin,             /// �ܾ�������λ
};

/// ����������
enum eTrigger
{   
    eUnAct,                     /// ��������
    eActive,                    /// ��������   
};

/// ��λ����
enum  ePhaseType
{
    eInValid = -1,               
    eFight,                     /// Ұս����λ
    ePk                         /// Pk��λ
};

/// ��λ״̬
enum  ePhaseState
{
    eRun,                       /// ������
    eEnd,                       /// ��λ����
    eClose                      /// ��λ�˳�
};

/// ��λ�¼�
enum  ePhaseEvent
{
    eExit=1,                   /// ���������¼�������������ͨ��ɱ��ĳ������������ĳ������
    eDie,                      /// ��������
    eSecKill,                  /// ��ɱ
    eAgonal,                   /// ��ұ���״̬
};


/// ��λ���������¼�
enum  ePhaseMOper
{
    eMonsterDie,
    eAddToPhase,
};

/// ��λ�д��ڵĻ�������
enum   ePhaseOper
{
    eRemit=1,                 /// ����
    eLeave,                   /// �뿪(��ӻ��߶ӳ�����) ���˳���Ϸ����
    eEscape,                  /// ����
    eReJoin,                  /// ��Ӳ���
    eReceive,                 /// ����Pk��λ�Ĳ���
};


/// ����������
enum   eTimerType
{
    eAdd    =1,
    eSub
};

/// �ű���������  
enum   eWScriptType
{
    eSc_Oper = 1,
    eSc_Ttype,
    eSc_Event
};
