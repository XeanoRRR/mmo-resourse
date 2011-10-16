


#pragma once

/*
��Ϣ����Լ�������ĸ����ʣ�ͳһʹ�ã�
	Notify��֪ͨ��
	Request������
	Re���ظ���
*/

//! SCC��SUC��Ϣ
enum 
{
	//! ��½���
	MSG_SCC2SUC_LOGIN = MSG_SCC2SUC + MAX_SUB * 0,
	//!-----------------------------------------------------
	MSG_SCC2SUC_LOGIN_Re_Login,						//! ��Ӧ��¼
	MSG_SCC2SUC_LOGIN_Re_ChangePwd,					//! �޸�����ظ�


	//! ��Ϣ���
	MSG_SCC2SUC_INFO	= MSG_SCC2SUC + MAX_SUB * 1,
	//!-----------------------------------------------------
	MSG_SCC2SUC_INFO_Notify_Sm_BaseInfo,			//! һ��SM�Ļ�����Ϣ
	MSG_SCC2SUC_INFO_Notify_Sm_Close,				//! һ��SM�ر�

	MSG_SCC2SUC_INFO_Notify_AddHardInfo,			//! ���һ��Ӳ����Ϣ
	MSG_SCC2SUC_INFO_Notify_HardInfoList,			//! ����һ��SM��һ��Ӳ����Ϣ

	MSG_SCC2SUC_INFO_Notify_Gapp_BaseInfo,			//! Gapp������Ϣ
	MSG_SCC2SUC_INFO_Notify_Gapp_ChangeVersion,		//! Gapp�İ汾�ı�
	MSG_SCC2SUC_INFO_Notify_Gapp_ChangeRunState,	//! Gapp������״̬�ı�
	MSG_SCC2SUC_INFO_Notify_Gapp_DELAYTIME,			//! ֪ͨSM ---->GAPP Echo����ʱ��

	MSG_SCC2SUC_INFO_Notify_PackList,				//! ���͵�ǰ�������ĸ��°��б�
	MSG_SCC2SUC_INFO_Notify_UpLoadPack,				//! �ϴ����°�֪ͨ
	MSG_SCC2SUC_INFO_Notify_Error,					//! һ������֪ͨ

	MSG_SCC2SUC_INFO_Re_Show,						//! �ظ�SUC���ҵ�����

	MSG_SCC2SUC_INFO_Re_UpdateFile,					//! �ظ������ļ��б�
	MSG_SCC2SUC_INFO_Notify_CTRL_GappAction,		//! Gapp��Ϊ֪ͨ

	//! �������
	MSG_SCC2SUC_CTRL	= MSG_SCC2SUC + MAX_SUB * 2,
	//!-----------------------------------------------------
	MSG_SCC2SUC_CTRL_Re_InCtrlRoom,					//! ������ƿռ�ظ� 
	MSG_SCC2SUC_CTRL_Re_SHUTDOWN,					//! �ر�GAPP
	MSG_SCC2SUC_CTRL_Re_START,						//! ����GAPP
	MSG_SCC2SUC_CTRL_Re_RESTART,					//! ����GAPP

	MSG_SCC2SUC_CTRL_Re_UpLoadPack,					//! һ�����°��ϴ����
	MSG_SCC2SUC_CTRL_Re_UpdateGapp,					//! �ظ�����һ��GAPP
	MSG_SCC2SUC_CTRL_Re_ReLoadGapp,					//! �ظ��ض�Gappԭʼ�ļ���
	MSG_SCC2SUC_CTRL_Re_UploadFile,					//! �ظ��ϴ��ļ����

	MSG_SCC2SUC_CTRL_Re_GappAction,					//! �ظ�����Gapp��Ϊ����GAPP����һ����Ϊö��ֵ��

	MSG_SCC2SUC_CTRL_Re_SMAction,					//	�ظ�һ��SM������Ϊ
	
	//! �������
	MSG_SCC2SUC_MANAGE	= MSG_SCC2SUC + MAX_SUB * 3,
	//!-----------------------------------------------------
	MSG_SCC2SUC_MANAGE_Notify_GmToWs,				//! Gm������˳�Ws��֪ͨ
	MSG_SCC2SUC_MANAGE_Notify_Watch,				//! GM�������״̬֪ͨ
	MSG_SCC2SUC_MANAGE_Notify_ReportList,			//! �ٱ��б�֪ͨ
	MSG_SCC2SUC_MANAGE_Notify_ResultList,			//! �������б�֪ͨ
	MSG_SCC2SUC_MANAGE_Notify_Gm_Command_Info,		//! ���ͷ�������GM��SUC��������Ϣ������ö�٣�DWORD��+ ��Ϣ�ַ�����512 STR����

	//! ����
	MSG_SCC2SUC_OTHER	= MSG_SCC2SUC + MAX_SUB * 4,
	//!-----------------------------------------------------
	MSG_SCC2SUC_OTHER_Notify_SendText,				//! ����һ���ı���Ϣ
	MSG_SCC2SUC_OTHER_Notify_NotReady,				//! SCCû��׼���ã�������SUC��Ϣ


	/////////////////////////////////////////
	MSG_SCC2SUC_END = MSG_SCC2SUC + MAX_TYPE,
};


//! SUC��SCC��Ϣ
enum 
{
	//! ��½���
	MSG_SUC2SCC_LOGIN	= MSG_SUC2SCC + MAX_SUB * 0,
	//!-----------------------------------------------------
	MSG_SUC2SCC_LOGIN_Request_Login,				//! �����¼
	MSG_SUC2SCC_LOGIN_Request_ChangePwdQuest,		//! �����޸�����

	//! ��Ϣ���
	MSG_SUC2SCC_INFO	= MSG_SUC2SCC + MAX_SUB * 1,
	//!-----------------------------------------------------
	MSG_SUC2SCC_INFO_Request_Ctrl,					//! ��Ϣ����	

	MSG_SUC2SCC_INFO_Request_Show,					//! ����һ��Gapp��ĳ������Ϣ����һ��ʱ�䣩
	MSG_SUC2SCC_INFO_Request_New,					//! ����һ��Gapp��ĳ����������Ϣ

	MSG_SUC2SCC_INFO_Request_UpdateFile,			//! ��ȡ�����ļ��б�

	//! �������
	MSG_SUC2SCC_CTRL	= MSG_SUC2SCC + MAX_SUB * 2,
	//!-----------------------------------------------------
	MSG_SUC2SCC_CTRL_Request_InCtrlRoom,			//! ���롢�˳����ƿռ�
	MSG_SUC2SCC_CTRL_Request_SHUTDOWN,				//! �ر�GAPP
	MSG_SUC2SCC_CTRL_Request_START,					//! ����GAPP
	MSG_SUC2SCC_CTRL_Request_RESTART,				//! ����GAPP

	MSG_SUC2SCC_CTRL_Request_UpLoadPack,			//! �ϴ�һ�����°�
	MSG_SUC2SCC_CTRL_Request_UpdateGapp,			//! ����һ��GAPP
	MSG_SUC2SCC_CTRL_Request_ReLoadGapp,			//! �ض�Gappԭʼ�ļ���

	MSG_SUC2SCC_CTRL_Request_UploadFile,			//	�ϴ�һ���ļ�

	MSG_SUC2SCC_CTRL_Request_GappAction,			//! ����Gapp��Ϊ����GAPP����һ����Ϊö��ֵ��

	MSG_SUC2SCC_CTRL_Request_SMAction,				//	SM��Ϊ
	

	//! �������
	MSG_SUC2SCC_MANAGE	= MSG_SUC2SCC + MAX_SUB * 3,
	//!-----------------------------------------------------
	MSG_SUC2SCC_MANAGE_Request_GmToWs,				//! Gm������˳�Ws������
	MSG_SUC2SCC_MANAGE_Request_VeiwReport,			//! �鿴�ٱ�����
	MSG_SUC2SCC_MANAGE_Request_VeiwResult,			//! �鿴����������
	MSG_SUC2SCC_MANAGE_Request_ScoutPlayer,			//! ���������Ϊ����
	MSG_SUC2SCC_MANAGE_Request_OptPlayer,			//! �����������
	MSG_SUC2SCC_MANAGE_Request_SendAffiche,			//! ���͹�������
	MSG_SUC2SCC_MANAGE_Request_Gm_Command,			//! ���Ͳ���������ҵ�GM������������ö�٣�DWORD��+ ��Ϣ�ַ�����512 STR����


	/////////////////////////////////////////
	MSG_SUC2SCC_END = MSG_SUC2SCC + MAX_TYPE,
};