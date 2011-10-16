#pragma once
#include "MsgType.h"

enum eManageToolMsgType
{
	//SM->GAPP-------------------------
	MSG_SM2GAPP = MAX_TYPE * 36,
	MSG_SM2GAPP_OPERATOR = MSG_SM2GAPP + MAX_SUB * 0,
	MSG_SM2GAPP_TESTCONN,//����ӳ�
	MSG_SM2GAPP_SHUTDOWN,

	MSG_SM2GAPP_GET_GSPLAYERS,//��ȡGS�������														
	
	MSG_SM2GAPP_ACTION,
	MSG_SM2GAPP_GET_MAINAITICK,
	MSG_SM2GAPP_WATCH,//����ĳ�����
	MSG_SM2GAPP_REPORT_Result,//�ٱ�������
	MSG_SM2GAPP_WS_SENDTOPNEWS,//������
	MSG_SM2GAPP_MANAGE_Request_Gm_Command,
	//SM->GAPP-------------------------




	//GAPP->SM-------------------------
	MSG_GAPP2SM = MAX_TYPE * 37,
	MSG_GAPP2SM_OPERATOR = MSG_GAPP2SM + MAX_SUB * 0,
	MSG_GAPP2SM_TESTCONN,
	MSG_GAPP2SM_WRITEFILE,//д�ļ�
	MSG_GAPP2SM_CLEARFILE,//����ļ�
	MSG_GAPP2SM_OPENFILE,//���ļ�
	MSG_GAPP2SM_CLOSEFILE,//�ر��ļ�


	MSG_GAPP2SM_GAPP_SENDINFO,//ͨ����������SM
	MSG_GAPP2SM_GAPP_INITOK,//GAPP��ʼ���ɹ�
	MSG_GAPP2SM_GAPP_STARTOK,//GAPP�����ɹ�
	
	MSG_GAPP2SM_SHUTDOWN,//GAPP�ر�

	MSG_GAPP2SM_ACTION,

	MSG_GAPP2SM_GET_GSPLAYERS,
	MSG_GAPP2SM_SEND_MAINAITICK,
	MSG_GAPP2SM_SENDINFO,
	MSG_GAPP2SM_REPORTINFO,//�ٱ���Ϣ
	MSG_GAPP2SM_WATCH_INFO,//���ӽ��
	MSG_GAPP2SM_WATCH_INFO_Reply,//��������Ӧ��
	MSG_GAPP2SM_REPORT_Result_Reply,//�ٱ�������ִ�����
	MSG_GAPP2SM_MANAGE_Request_Gm_Command,
	//GAPP->SM-------------------------
};