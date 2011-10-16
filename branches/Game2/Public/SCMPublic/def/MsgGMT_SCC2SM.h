



#pragma once

/*
��Ϣ����Լ�������ĸ����ʣ�ͳһʹ�ã�
	Notify��֪ͨ��
	Request������
	Re���ظ���
*/


//! SCC��SM��Ϣ
enum 
{
	//! 
	MSG_SCC2SM_LOGIN	= MSG_SCC2SM + MAX_SUB * 0,
	//!-----------------------------------------------------
	MSG_SCC2SM_LOGIN_Re_Login,			//! ͬ���½


	//! ��Ϣ���
	MSG_SCC2SM_INFO		= MSG_SCC2SM + MAX_SUB * 1,
	//!-----------------------------------------------------
	MSG_SCC2SM_INFO_SUC_Request_GameInfo,	//! ת��SUC�������µ���Ϸ��Ϣ
	

	//! �������
	MSG_SCC2SM_CTRL		= MSG_SCC2SM + MAX_SUB * 2,
	//!-----------------------------------------------------
	MSG_SCC2SM_CTRL_Request_SHUTDOWN,
	MSG_SCC2SM_CTRL_Request_START,
	MSG_SCC2SM_CTRL_Request_SMAction,		//  ��SMִ�еĶ���
	//Ӳ��
	MSG_SCC2SM_CTRL_Request_RESTARTCOM,		//! ���������

	MSG_SCC2SM_CTRL_Request_UpdateGapp,		//! �������һ��GAPP
	MSG_SCC2SM_CTRL_Request_UpdateTypeGapp,	//! �������һ�����͵�����GAPP

	MSG_SCC2SM_CTRL_Re_File,				//! �ظ��ļ�
	MSG_SCC2SM_CTRL_Re_ServerPack,			//! �ظ�һ��GAPP�������ļ���

	MSG_SCC2SM_CTRL_Request_GappAction,		//! ����Gapp��Ϊ����GAPP����һ����Ϊö��ֵ��

	//! �������
	MSG_SCC2SM_MANAGE	= MSG_SCC2SM + MAX_SUB * 3,
	//!-----------------------------------------------------
	MSG_SCC2SM_MANAGE_WATCH,				//! �������ĳ�����
	MSG_SCC2SM_MANAGE_Report_Result,		//! �ٱ�������
	MSG_SCC2SM_MANAGE_Notify_Affiche,		//! ���ͷ���������
	MSG_SCC2SM_MANAGE_Request_Gm_Command,	//! ת������������ҵ�GM������������ö�٣�DWORD��+ ��Ϣ�ַ�����512 STR����

	/////////////////////////////////////////
	MSG_SCC2SM_END = MSG_SCC2SM + MAX_TYPE,
};

//! SM��SCC��Ϣ
enum 
{
	//! ��½���
	MSG_SM2SCC_LOGIN	= MSG_SM2SCC + MAX_SUB * 0,
	//!-----------------------------------------------------
	MSG_SM2SCC_LOGIN_Request_Login,				//! ��½����


	//! ��Ϣ���
	MSG_SM2SCC_INFO		= MSG_SM2SCC + MAX_SUB * 1,
	//!-----------------------------------------------------
	MSG_SM2SCC_INFO_Notify_HardInfo,			//! һ��Ӳ����Ϣ
	MSG_SM2SCC_INFO_Notify_HardInfoList,		//! һ��Ӳ����Ϣ
	MSG_SM2SCC_INFO_Notify_Gapp_BaseInfo,		//! Gapp������Ϣ
	MSG_SM2SCC_INFO_Notify_Gapp_ChangeRunState,	//! Gapp����״̬�ı�
	MSG_SM2SCC_INFO_Notify_ServerInfo,			//��������Ϣ
	MSG_SM2SCC_INFO_Notify_CTRL_GappAction,		//! Gapp��Ϊ֪ͨ
	MSG_SM2SCC_INFO_Notify_Error,				//������Ϣ
	MSG_SM2SCC_INFO_Notify_DELAYTIME,	


	//! �������
	MSG_SM2SCC_CTRL		= MSG_SM2SCC + MAX_SUB * 2,
	//!-----------------------------------------------------
	MSG_SM2SCC_CTRL_Notify_SHUTDOWN,
	MSG_SM2SCC_CTRL_Notify_START,
	MSG_SCC2SM_CTRL_Notify_RESTARTCOM,

	MSG_SM2SCC_CTRL_Notify_UpdateGapp,			//! �ظ�����Gapp���

	MSG_SM2SCC_CTRL_Request_File,				//! �����ļ�
	MSG_SM2SCC_CTRL_Request_ServerPack,			//! ����һ��GAPP�������ļ���
	MSG_SM2SCC_CTRL_Notify_ServerPack,			//  ��ȡԭʼ�����
	MSG_SM2SCC_CTRL_Notify_ReloadFile,			//  �ض���Դ���

	MSG_SM2SCC_CTRL_Notify_SMAction,			//  SMִ�ж������ؽ��
	//! �������
	MSG_SM2SCC_MANAGE	= MSG_SM2SCC + MAX_SUB * 3,
	//!-----------------------------------------------------
	MSG_SM2SCC_MANAGE_Notify_Report,				//�ٱ���Ϣ
	MSG_SM2SCC_MANAGE_WATCH_Reply,				//��������Ӧ��
	MSG_SM2SCC_MANAGE_Report_Result_Replay,		//���ط�ŵȴ����Ƿ�ɹ�
	MSG_SM2SCC_MANAGE_Notify_Gm_Command_Info,	//���ͷ�������GM��SUC��������Ϣ������ö�٣�DWORD��+ ��Ϣ�ַ�����512 STR����
	/////////////////////////////////////////
	MSG_SM2SCC_END = MSG_SM2SCC + MAX_TYPE, 
};
