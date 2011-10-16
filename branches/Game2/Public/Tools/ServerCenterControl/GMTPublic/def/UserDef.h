

#pragma once

struct tagUserLogin 
{
	DWORD dwVersion;
	char szName[MAX_USER_NAME_SZIE];
	byte szMD5Pwd[MD5_16_PASSWORD_SIZE];			//md5_16���ܺ��

	tagUserLogin(VOID){memset(this, 0, sizeof(tagUserLogin));}
};

struct tagChangePWD 
{
	byte szMD5PwdOld[MD5_16_PASSWORD_SIZE];
	byte szMD5PwdNew[MD5_16_PASSWORD_SIZE];
	tagChangePWD()	{ memset(this,0,sizeof(tagChangePWD)); }
};


enum eUserLoginError
{
	eULE_NoError,		//! û�д���

	eULE_VersionErr,	//! SUC��SM��Ӧ�汾����
	eULE_Nobody,		//! �޴��û�
	eULE_PwdErr,		//! �������

	eULE_SystemErr,		//! ϵͳ����
};


//! �û���������
enum eUserGappCtrlType
{
	eUGCT_Close,		//! �ر�
	eUGCT_ReStart,		//! ��������
};