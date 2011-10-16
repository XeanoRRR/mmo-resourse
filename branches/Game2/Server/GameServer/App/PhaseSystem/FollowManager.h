/** 
 * @file   FollowManager.h  
 * @author Expter
 * @brief  ��Ҹ���ӳ�����д�˹������Ŀ�ľ���Ϊ��Ԥ���Ժ�ӳ������Ա
           ����ӳ���һЩ������Ϣ��
 * @date   2010/03/18        
 */

#pragma  once

#include <hash_map>

class   FollowManager : public CGameEvent
{
    typedef  std::map<long,CGUID>   lTIMER;
    typedef  stdext::hash_map<CGUID,CGUID,hash_guid_compare>  INVITETABLE;

public:
    FollowManager();
    ~FollowManager();

    /// ���ֹͣ����״̬
    void  StopFollow( CPlayer* player );

    /// ��������
    void  Invite();

    /// ͬ�������жϳ�ʱ
    bool  IsTimeOut(const CGUID& ID);

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

private:
    lTIMER          m_ltimer;
    INVITETABLE     m_InviteTable;

};