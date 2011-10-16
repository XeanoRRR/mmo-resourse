// chat prompt: ���������

#include <wctype.h>//for towlower()
#include "StdAfx.h"
#include "../UIDef.h"
#include "../../../Public/Common/MsgType.h"
#include "../../Net/Message.h"
#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/ClientApp/Player.h"

using namespace CEGUI;

static map<eChatType, String>   s_ChatTypeToName;
static map<String, eChatType>   s_ChatNameToType;
static map<String, String>      s_ChatCmdToName;

static void InitChatTypes()
{
    s_ChatTypeToName[CHAT_NORMAL]   = ToCEGUIString("˵");
    s_ChatTypeToName[CHAT_FACTION]  = ToCEGUIString("���");
    s_ChatTypeToName[RUN_SCRIPT]    = ToCEGUIString("�ű�");
    s_ChatTypeToName[CHAT_REGION]   = ToCEGUIString("����");
    s_ChatTypeToName[CHAT_TEAM]     = ToCEGUIString("����");
    s_ChatTypeToName[CHAT_WORLD]    = ToCEGUIString("����");
    s_ChatTypeToName[CHAT_PRIVATE]  = ToCEGUIString("���Ļ�");

    s_ChatNameToType[ToCEGUIString("˵")]       = CHAT_NORMAL;
    s_ChatNameToType[ToCEGUIString("���")]     = CHAT_FACTION;
    s_ChatNameToType[ToCEGUIString("�ű�")]     = RUN_SCRIPT;
    s_ChatNameToType[ToCEGUIString("����")]     = CHAT_REGION;
    s_ChatNameToType[ToCEGUIString("����")]     = CHAT_TEAM;
    s_ChatNameToType[ToCEGUIString("����")]     = CHAT_WORLD;
    s_ChatNameToType[ToCEGUIString("���Ļ�")]   = CHAT_PRIVATE;

    s_ChatCmdToName[ToCEGUIString("/s")]        = ToCEGUIString("˵");
    s_ChatCmdToName[ToCEGUIString("/say")]      = ToCEGUIString("˵");
    s_ChatCmdToName[ToCEGUIString("/f")]        = ToCEGUIString("���");
    s_ChatCmdToName[ToCEGUIString("/faction")]  = ToCEGUIString("���");
    s_ChatCmdToName[ToCEGUIString("/sc")]       = ToCEGUIString("�ű�");
    s_ChatCmdToName[ToCEGUIString("/script")]   = ToCEGUIString("�ű�");
    s_ChatCmdToName[ToCEGUIString("/r")]        = ToCEGUIString("����");
    s_ChatCmdToName[ToCEGUIString("/region")]   = ToCEGUIString("����");
    s_ChatCmdToName[ToCEGUIString("/t")]        = ToCEGUIString("����");
    s_ChatCmdToName[ToCEGUIString("/team")]     = ToCEGUIString("����");
    s_ChatCmdToName[ToCEGUIString("/w")]        = ToCEGUIString("����");
    s_ChatCmdToName[ToCEGUIString("/world")]    = ToCEGUIString("����");
    s_ChatCmdToName[ToCEGUIString("/p")]        = ToCEGUIString("���Ļ�");
    s_ChatCmdToName[ToCEGUIString("/private")]  = ToCEGUIString("���Ļ�");
    s_ChatCmdToName[ToCEGUIString("/wisper")]   = ToCEGUIString("���Ļ�");
}

Window* InitChatPrompt();
static bool OnKeyDown_eChatPrompt(const EventArgs& args);
static bool OnCharacterKey_eChatPrompt(const EventArgs& args);
static bool OnShown_dwChatPrompt(const EventArgs& args);
static bool OnClicked_bType(const EventArgs& args);
static bool OnClicked_miSay(const EventArgs& args);
static bool OnClicked_miFaction(const EventArgs& args);
static bool OnClicked_miScript(const EventArgs& args);
static bool OnClicked_miRegion(const EventArgs& args);
static bool OnClicked_miTeam(const EventArgs& args);
static bool OnClicked_miWorld(const EventArgs& args);
static bool OnClicked_miPrivate(const EventArgs& args);


Window* InitChatPrompt()
{
    Window* def = LoadUI("ChatPrompt");
    def->subscribeEvent(Window::EventShown, OnShown_dwChatPrompt);

    Window* edit = GetWndMgr().getWindow("ChatPrompt/eChatPrompt");
    edit->subscribeEvent(Window::EventKeyDown, OnKeyDown_eChatPrompt);
    edit->subscribeEvent(Window::EventCharacterKey, OnCharacterKey_eChatPrompt);

    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->subscribeEvent(PushButton::EventClicked, OnClicked_bType);

    PopupMenu* popup = CreatePopupMenu("ChatPrompt/bChatType/pmTypes");
    popup->setPosition(UVector2(UDim(0.0f,20.0f),UDim(0.0f,200.0f)));
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miSay", ToCEGUIString("˵ /s /say"), OnClicked_miSay);
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miFaction", ToCEGUIString("��� /f /faction"), OnClicked_miFaction);
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miScript", ToCEGUIString("�ű� /sc /script"), OnClicked_miScript);
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miRegion", ToCEGUIString("���� /r /region"), OnClicked_miRegion);
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miTeam", ToCEGUIString("���� /t /team"), OnClicked_miTeam);
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miWorld", ToCEGUIString("���� /w /world"), OnClicked_miWorld);
    CreateMenuItem(popup, "ChatPrompt/bChatType/pmTypes/miPrivate", ToCEGUIString("���Ļ� /p /private /wisper"), OnClicked_miPrivate);

    InitChatTypes();

    def->setVisible(false);
    return def;
}
static bool OnKeyDown_eChatPrompt(const EventArgs& args)
{
    static deque<pair<String, String> > s_PreChats;
    static const size_t s_PreChatsCap = 5;
    static size_t s_PreChatsIdx = 0;

    switch (static_cast<const KeyEventArgs&>(args).scancode)
    {
    case Key::ArrowLeft:
        {
            Editbox* edit = WEditBox(WEArgs(args).window);
            size_t beg = edit->getSelectionStartIndex();
            size_t end = edit->getSelectionEndIndex();
            if (beg < end)
            {
                edit->setCaratIndex(beg+1);
            }
        }
        break;

    case Key::ArrowRight:
        {
            Editbox* edit = WEditBox(WEArgs(args).window);
            size_t beg = edit->getSelectionStartIndex();
            size_t end = edit->getSelectionEndIndex();
            if (beg < end)
            {
                edit->setCaratIndex(end-1);
            }
        }
        break;

    case Key::ArrowUp:
        {
            if (s_PreChatsIdx == 0)
                return false;
            s_PreChatsIdx -= 1;
            Editbox* edit = WEditBox(WEArgs(args).window);
            edit->setText(s_PreChats[s_PreChatsIdx].second);
            edit->setSelection(0, s_PreChats[s_PreChatsIdx].second.size());
            Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
            btn->setText(s_PreChats[s_PreChatsIdx].first);
        }
        break;

    case Key::ArrowDown:
        {
            if (s_PreChats.empty() || s_PreChatsIdx >= s_PreChats.size()-1)
                break;
            s_PreChatsIdx += 1;
            Editbox* edit = WEditBox(WEArgs(args).window);
            edit->setText(s_PreChats[s_PreChatsIdx].second);
            edit->setSelection(0, s_PreChats[s_PreChatsIdx].second.size());
            Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
            btn->setText(s_PreChats[s_PreChatsIdx].first);
        }
        break;

    case Key::NumpadEnter:
    case Key::Return:
        {
            Window* edit = WEArgs(args).window;
            if (edit->getText().empty())
            {
                s_PreChatsIdx = s_PreChats.size();
                GetWndMgr().getWindow("ChatPrompt")->hide();
                return true;
            }

            Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
            map<String, eChatType>::iterator it = s_ChatNameToType.find(btn->getText());
            if (it == s_ChatNameToType.end())
            {
                s_PreChatsIdx = s_PreChats.size();
                GetWndMgr().getWindow("ChatPrompt")->hide();
                return true;
            }
            eChatType chatType = it->second;

            //��ǰ�����������һ�����Բ�ͬ�����¼֮
            if (s_PreChats.empty()
                || !s_PreChats.empty()
                && (s_PreChats.rbegin()->first != btn->getText()
                || s_PreChats.rbegin()->second != edit->getText()))
            {
                s_PreChats.push_back(make_pair(btn->getText(), edit->getText()));
            }
            while (s_PreChats.size() > s_PreChatsCap)
            {
                s_PreChats.pop_front();
            }

            const char* txt = CEGUIStringToAnsiChar(edit->getText());

            //����ѡ��ʲô�������ͣ�ֻҪ˫б�ܿ�ͷ���ͱ�ʾ�ű�����
            if (strlen(txt) > 2 && txt[0] == '/' && txt[1] == '/')
            {
                CPlayer *player = GetGame()->GetMainPlayer();
                CMessage msg(MSG_C2S_OTHER_TALK);
                msg.Add((BYTE)0);
                msg.Add((long)RUN_SCRIPT);
                msg.Add(player->GetType()); // ������type
                msg.Add(player->GetExID()); // ������ID
                msg.Add((long)0);
                msg.Add(&(txt[2]));//�ı����ӵ������ַ���ʼ��
                msg.Send();
            }
            else
            {
                switch (chatType)
                {
                case RUN_SCRIPT://"�ű�"
                    {
                        CPlayer *player = GetGame()->GetMainPlayer();
                        CMessage msg(MSG_C2S_OTHER_TALK);
                        msg.Add((BYTE)0);
                        msg.Add((long)RUN_SCRIPT);
                        msg.Add(player->GetType()); // ������type
                        msg.Add(player->GetExID()); // ������ID
                        msg.Add((long)0);
                        msg.Add(txt);//�ı�
                        msg.Send();
                    }
                    break;
                case CHAT_FACTION://"���"
                case CHAT_REGION://"����"
                case CHAT_TEAM://"����"
                case CHAT_WORLD://"����"
                    {
                        CMessage msg(MSG_C2CS_CHAT);
                        msg.Add((long)chatType);
                        msg.Add(txt);
                        msg.SendToChatServer();
                    }
                    break;
                case CHAT_NORMAL://"˵"
                    {
                        CMessage msg(MSG_C2CS_CHAT);
                        msg.Add((long)chatType);
                        msg.Add(txt);
                        list<CBaseObject*>* objs = GetGame()->GetRegion()->GetObjectList();
                        vector<CGUID> players;
                        for (list<CBaseObject*>::iterator it = objs->begin();
                            it != objs->end(); ++it)
                        {
                            if ((*it)->GetType() == TYPE_PLAYER)
                                players.push_back((*it)->GetExID());
                        }

                        msg.Add((unsigned long)players.size());
                        for (vector<CGUID>::iterator it = players.begin();
                            it != players.end(); ++it)
                        {
                            msg.Add(*it);
                        }
                        msg.SendToChatServer();
                    }
                    break;
                case CHAT_PRIVATE://"���Ļ�"
                    {
                        string s(txt);
                        size_t idx = s.find_first_of(' ');
                        if (idx > 16)//��������������8�������ַ���16��Ӣ���ַ�
                        {
                            //��ʾ����Է�����
                            break;
                        }
                        string name(s, 0, idx);
                        if (name == GetGame()->GetMainPlayer()->GetName())
                        {
                            //�������Լ�˽��
                            break;
                        }
                        string talk(s, idx+1, s.size());
                        if (talk == "")
                        {
                            //���ܷ��Ϳմ�
                            break;
                        }
                        CMessage msg(MSG_C2CS_CHAT);
                        msg.Add((long)chatType);
                        msg.Add(talk.c_str());
                        msg.Add(name.c_str());
                        msg.SendToChatServer();
                    }
                    break;
                }//switch (chatType)
            }//else
            edit->setText("");
            GetWndMgr().getWindow("ChatPrompt")->hide();
            s_PreChatsIdx = s_PreChats.size();
            return true;
        }
        break;
    case Key::Escape:
        {
            GetWndMgr().getWindow("ChatPrompt")->hide();
            return true;
        }
    }
    return false;
}
static bool OnCharacterKey_eChatPrompt(const EventArgs& args)
{
    switch (static_cast<const KeyEventArgs&>(args).codepoint)
    {
    case ' ':
        {
            Editbox* edit = WEditBox(WEArgs(args).window);
            const String& s = edit->getText();
            if (s.empty() 
                || s[0] != '/'              //���������'/'��ͷ
                || edit->getCaratIndex() > 8)  //�������8���ַ�
                break;

            String cmd(s, 0, edit->getCaratIndex());
            for (size_t i=0; i<cmd.size(); ++i)
            {
                cmd[i] = towlower(cmd[i]);
            }
            map<String, String>::iterator it = s_ChatCmdToName.find(cmd);
            if (it == s_ChatCmdToName.end())
                break;

            Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
            btn->setText(it->second);
            String talk(s, edit->getCaratIndex(), s.size());
            edit->setText(talk);
            edit->setCaratIndex(0);
            return true;
        }
    }
    return false;
}

static bool OnShown_dwChatPrompt(const EventArgs& args)
{
    Window* edit = GetWndMgr().getWindow("ChatPrompt/eChatPrompt");
    edit->activate();
    return false;
}

static bool OnClicked_bType(const EventArgs& args)
{
    Window* popup = GetWndMgr().getWindow("ChatPrompt/bChatType/pmTypes");
    popup->show();
    return false;
}

static bool OnClicked_miSay(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[CHAT_NORMAL]);
    return false;
}
static bool OnClicked_miFaction(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[CHAT_FACTION]);
    return false;
}
static bool OnClicked_miScript(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[RUN_SCRIPT]);
    return false;
}
static bool OnClicked_miRegion(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[CHAT_REGION]);
    return false;
}
static bool OnClicked_miTeam(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[CHAT_TEAM]);
    return false;
}
static bool OnClicked_miWorld(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[CHAT_WORLD]);
    return false;
}
static bool OnClicked_miPrivate(const EventArgs& args)
{
    Window* menuItem = WEArgs(args).window;
    Window* btn = GetWndMgr().getWindow("ChatPrompt/bChatType");
    btn->setText(s_ChatTypeToName[CHAT_PRIVATE]);
    return false;
}