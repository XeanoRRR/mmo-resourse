


#pragma once



//! ��Ϸ������Ȩ������ö��
enum eGMTPurviewType
{
	ePT_Penniless			= 0				,//! ��Ȩ��

	ePT_GmNormal			= 0x01			,//! ��Ϸ����Ա������ٱ���
	ePT_GmAffiche			= 0x02			,//! ������Ϸ����
	ePT_ViewGmOpt			= 0x04			,//! �鿴GM�Ĳ�����¼

	ePT_Scout1				= 0x0100		,//! �鿴��Ϣ1
	ePT_Scout2				= 0x0200		,//! �鿴��Ϣ2

	ePT_ServerControl1		= 0x010000		,//! ������Ϸ������1
	ePT_ServerControl2		= 0x020000		,//! ������Ϸ������2

	ePT_Update				= 0x01000000	,//! �ļ�����
	ePT_UserManage			= 0x02000000	,//! �û�����

	ePT_AllPurview			= 0xFFFFFFFF	,//! ����Ȩ��
};


//! һ���û����͵�Ȩ��
struct tagGMTPurviewSetup
{
	//!						���õ�ID
	DWORD					dwUserType;
	//!						���õ�Ȩ��
	DWORD					dwPurview;
	//!						�û�������
	char					szName[MAX_USER_GROUP_NAME_SZIE];
	/////
	tagGMTPurviewSetup(VOID){memset(this, 0, sizeof(tagGMTPurviewSetup));}
};