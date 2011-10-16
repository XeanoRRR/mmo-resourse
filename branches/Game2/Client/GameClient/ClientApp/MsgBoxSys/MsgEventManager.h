/** 
 *   brief   ���ڵ������ڵ������¼�����,�����жӹ������еĵ�������.
             ֻ�е���һ�ζԻ���رպ�Ž��д���һ��.
             
             ʹ�÷�ʽ: 1. ע��һ��ȫ�ֵ��¼����¼�����İ󶨺���
                        2. ���ÿ���ͨ��PushEvent��Ͷ���¼���֧��2�ַ�ʽ.
 *   author  Expter
 *   date    2010/06/12
     Modify  2010/06/25
 */

#pragma  once 

#include "../../../../Public/Common/BoxType.h"
#include "MsgEventDef.h"

#include <Queue>
#include <map>
#include <list>

class   MsgEventManager
{
public:
    MsgEventManager();
    ~MsgEventManager();
    
    void  Init();

public:
    void  PushEvent(eMsgBoxType Type,const char* str=NULL,
                    Msg_EventHandle Handle=NULL,void* pData=NULL,
                    bool Mode=false,bool Cache=false);

    void  PushEvent(MsgEvent& Event);

    void  GoProcessEvent();

private:
    void _PushEvent(MsgEvent& event);

    void ProcesEvent();
 
private:
    /// �����¼�ӳ���
    /// �����Ի�������
    std::list<MsgEvent>                    m_MsgBoxList;

private:
    bool                                   m_IsRuning;
};

#define GetMsgBox() GetInst(MsgEventManager)