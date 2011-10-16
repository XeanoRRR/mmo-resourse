/*LinkmanMessage.cpp
*����: ����
*��������:2008-2-14
*/
#include "stdafx.h"
#include "../Net/Message.h"

#include "Player.h"

// ���� ����ϵͳ����Ϣ
void OnLinkmanMessage( CMessage *pMsg )
{
	//CFriendsPageEx*	pFriendsPageEx = GetGame()->GetCGuiEx()->GetFriendsPageEx();

	switch( pMsg->GetType() ) 
	{
		// ��ʼ������ϵ���б�
		case MSG_S2C_LINKMAN_INIT:
			{
				//pFriendsPageEx->RecvInitMsg( pMsg );
			}
			break;

		// �����ϵ��
		case MSG_S2C_LINKMAN_ADD:
			{
				//pFriendsPageEx->RecvAddMsg( pMsg );
			}
			break;

		// ɾ����ϵ��
		case MSG_S2C_LINKMAN_DEL:
			{
				//pFriendsPageEx->RecvDelMsg( pMsg );
			}
			break;

		// �ƶ���ϵ��
		case MSG_S2C_LINKMAN_MOVE:
			{
				//pFriendsPageEx->RecvMoveMsg( pMsg );
			}
			break;

		// �������Э��
		case MSG_S2C_LINKMAN_REQUEST_ADD:
			{
				//pFriendsPageEx->RecvRequestMsg( pMsg );
			}
			break;

		// ����
		case MSG_S2C_LINKMAN_LOGIN:
			{
				//pFriendsPageEx->RecvLoginMsg( pMsg );
			}
			break;

		// ����
		case MSG_S2C_LINKMAN_LOGOUT:
			{
				//pFriendsPageEx->RecvLogoutMsg( pMsg );
			}
			break;

		// ��ϵ��ɾ����ɫ
		case MSG_S2C_LINKMAN_DELSTATE_CHANGE:
			{
				//pFriendsPageEx->RecvLinkmanDelMsg( pMsg );
			}
			break;

		// ��ϵ����Ϣ����
		case MSG_S2C_LINKMAN_INFO_CHANGE:
			{
				//pFriendsPageEx->RecvChangeInfoMsg( pMsg );
			}
			break;

		// ����
		case MSG_S2C_LINKMAN_LEAVE_WORD:
			{
				//pFriendsPageEx->RecvLeaveWordMsg( pMsg );
			}
			break;

		// ���Խ��
		case MSG_S2C_LEAVEWORD_SAVE_RESULT:
			{
				//pFriendsPageEx->RecvLWResultMsg( pMsg );
			}
			break;

		// ������ϵ�˶��Լ��Ĳ�����Ϣ
		case MSG_S2C_LINKMAN_OTHER_HANDLE:
			{
				//pFriendsPageEx->RecvOtherHandleMsg( pMsg );
			}
			break;
	}
}