/** 
 *  @file   PhaseServerDef.h
 *  @brief  ��λ����:
 *          ������λ���ܵķ�������һЩö�ٺ����Զ���
 *  @date   2010/03/18
 */

#pragma  once 

enum ePhaseRest
{
    ePRest,              /// ������λ�������� 
    ePInVitee            /// ������λ��������
};

enum  ePhaseWSInfo
{   
    eUpdateGSPhaseID,
};

enum  S2WPhaseOper
{
    eUpdateWSPhaseID,
    eDeleteWSPhaseID,

};
enum  S2WPhaseMsg
{
    eUpdatePlayerPhaseID,
};

struct PhaseFace
{
    PhaseFace(CGUID id,ePhaseRest st):guid(id),Rest(st)
    {

    }

    CGUID       guid;
    ePhaseRest  Rest;
};
