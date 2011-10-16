/**
 *  @brief �ṩ�ͻ�����λ����Ϣ����ģ��
 *  @file  PhaseSys.h
 */

#pragma  once 

#include "../../../../Public/Common/PhaseDef.h"

class  CMessage;
class  CPlayer;
class  PhaseSys
{
public:
    ///
    /// ������λ��Ϣ�Ĵ���
    ///
    void  PhaseMsgPro(CMessage* pMsg);

    ///
    /// ������λ����ҵ�������Ϊ������
    /// @param Oper     C2SPhaseOperType
    /// 
    void  PhaseOper(CPlayer* player,C2SPhaseOperType Oper);

    ///
    /// ������λ����ҵ�������Ϊ������
    /// @param first    creator
    /// @param second   Inviteer
    /// @param Oper     C2SPhaseOperType
    /// 
    void  PhaseOper(const CGUID& first,const CGUID& second,C2SPhaseOperType Oper);
};

