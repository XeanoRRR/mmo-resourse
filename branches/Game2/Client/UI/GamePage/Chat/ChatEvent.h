#pragma once

#include "../../PageEvent.h"
#include "../../../GameClient/ClientApp/other/Chat.h"
#include "../../../../Public/Common/BaseDef.h"

class ChatEvent:public PageEvent
{
public:
    ChatEvent(){}
    ChatEvent(EVENTTYPE type);
    ~ChatEvent(){}
    virtual void OnPageLoad(GamePage *pPage);
    virtual void OnPageOpen(GamePage *pPage);
    virtual void OnPageClose(GamePage *pPage);
    virtual void OnPageUpdate(GamePage *pPage);
    virtual void OnPageRelease(GamePage *pPage);
};
//////////////////////////////////////////////////////////////////////////
//zhaohang 2010-03-18
//
/*!
\brief
�������������ı�����,���ǳ���Chat.h �ļ��е�����CChat����
*/
class ChatInfo
{
public:
    ChatInfo();
    ~ChatInfo();

    /*!
    \brief
    �޸��ı�����
    */
    void ReSetChatContent(const char *content);
    /*!
    \brief
    �����ı�ǰ׺��ȡ�������ͣ���ǰ��Ƶ�����
    */
    eChatType GetChatType();

    /*!
    \brief
    �������������Ƴ���Ա������Ӧ���ı�ǰ׺�������Ƴ�����ַ���ָ��
    */
    const char* RemoveFlag(eChatType e);

private:
    string m_Content;
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//zhaohang 2010-03-18
//
/*!
\brief
����������߼�������,Ŀǰֻ�����˽ű����ò���
*/
class ChatLogicEventManager
{
public:
    ChatLogicEventManager();
    ~ChatLogicEventManager();

    /*!
    \brief
    ����������ͽű���������
    \param ScriptContent
    �ű�����
    */
    bool SendScript(const CEGUI::EventArgs &args);

    //�ع��������͵���Ϣ
    bool RollBackSendedText(const CEGUI::EventArgs &args);

    void SetRollPos(long pos)   {RollPos = pos;}

    long GetMsgQueSize()        {return (long)SendedMsgQue.size();}
protected:

private:
    ChatInfo m_chat;

    vector<string>      SendedMsgQue;

    long     RollPos;
    
};

