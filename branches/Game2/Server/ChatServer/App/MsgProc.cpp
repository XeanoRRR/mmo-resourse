#include "stdafx.h"
#include "../Net/Msg.h"
#include "RoleMgr.h"
#include "Role.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnChatMessage(Msg* msg)
{
    switch(msg->GetType())
    {
    case MSG_S2CS_PLAYER_ENTER_GAME://�������
        {
            CGUID roleId;
            msg->GetGUID(roleId);                   // role's id

            // the role exists?
            if (GetInst(RoleMgr).FindRole(roleId).Bound())
                break;

            HRole role(new Role);
            role->m_Id = roleId;
            role->m_SignCode = msg->GetLong();      // code used for validate
            char name[32];
            msg->GetStr(name, 32);                  // role's name
            role->m_Name = name;
            msg->GetGUID(role->m_TeamId);           // role's team id
            msg->GetGUID(role->m_FactionId);        // role's faction id
            role->m_RegionId = msg->GetLong();      // role's region id
            role->m_CountryId = msg->GetByte();     // role's country id
            role->m_WsId = msg->GetDWord();         // role's world server id
            role->m_GsSocketId = msg->GetSocketID();// role's game server socket id
            GetInst(RoleMgr).AddRole_Unchecked(role);
        }
        break;

    case MSG_S2CS_PLAYER_EXIT_GAME:// �������
        {
            CGUID roleId;
            msg->GetGUID(roleId);
            const HRole role = GetInst(RoleMgr).FindRole(roleId);
            if (role.Bound())
                GetInst(RoleMgr).DelRole(role);//��ɾ����ң������Ͽ�����
        }
        break;

    case MSG_S2CS_PLAYER_CHANGE_REGION:// ����л�����
        {
            CGUID roleId;
            msg->GetGUID(roleId);
            uint regionId = msg->GetDWord();
            GetInst(RoleMgr).ChangeRegion(roleId, regionId, msg->GetSocketID());
        }
        break;

    case MSG_S2CS_PLAYER_JOIN_TEAM:     // ��Ҽ������
    case MSG_S2CS_PLAYER_LEAVE_TEAM:    // ����뿪����
    case MSG_S2CS_PLAYER_JOIN_FACTION:  // ��Ҽ�����
    case MSG_S2CS_PLAYER_LEAVE_FACTION: // ����뿪���
    case MSG_S2CS_DISBAND_TEAM:         // �����ɢ
    case MSG_S2CS_DISBAND_FACTION:      // ����ɢ
        break;

    case MSG_C2CS_QUEST_VALIDATE:
        {
            // the role mustn't validate if it has checked
            if (GetInst(RoleMgr).FindRole(msg->GetSocketID()).Bound())
                break;

            CGUID roleId;
            msg->GetGUID(roleId);

            // checks the role
            HRole role = GetInst(RoleMgr).PopRole_Unchecked(roleId);
            if (role.Unbound() || role->m_SignCode != msg->GetDWord())
                break;

            // adds the role
            role->m_SocketId = msg->GetSocketID();
            GetInst(RoleMgr).AddRole(role);
        }
        break;

    case MSG_C2CS_CHAT:
        {
            const HRole sender = GetInst(RoleMgr).FindRole(msg->GetSocketID());
            if (sender.Unbound())
                break;

            eChatType chatType = (eChatType)msg->GetLong();
            char chatStr[1024];
            msg->GetStr(chatStr, 1024);
            switch (chatType)
            {
            case CHAT_NORMAL://˵
                {
                    size_t num = msg->GetDWord();
                    vector<CGUID> ids;
                    CGUID id;
                    for (size_t i=0; i<num; ++i)
                    {
                        msg->GetGUID(id);
                        ids.push_back(id);
                    }
                    GetInst(RoleMgr).ScreenChat(sender, chatStr, ids);
                }
                break;
            case CHAT_FACTION://���
                GetInst(RoleMgr).FactionChat(sender, chatStr);
                break;
            case CHAT_REGION://����
                GetInst(RoleMgr).RegionChat(sender, chatStr);
                break;
            case CHAT_TEAM://����
                GetInst(RoleMgr).TeamChat(sender, chatStr);
                break;
            case CHAT_WORLD://����
                GetInst(RoleMgr).WorldChat(sender, chatStr);
                break;
            case CHAT_PRIVATE://���Ļ�
                {
                    char receiverName[32];
                    msg->GetStr(receiverName, 32);
                    GetInst(RoleMgr).PrivateChat(sender, chatStr, receiverName);
                }
                break;
            }
        }
        break;

    }
}
