#pragma once

struct tagUserSetup
{
	DWORD	dwUserType;
	char	szName[MAX_USER_NAME_SZIE];
	char	szPwd[BASE_PASSWORD_SIZE];
	BOOL	bIsCmd;

	tagUserSetup(VOID){memset(this, 0, sizeof(tagUserSetup));}
};

////////////////////////////////////
class UserSetup
{
public:
	UserSetup(VOID);
	~UserSetup(VOID);

public:
	static			UserSetup&			GetInstance(VOID);

public:
	//!									��ȡ����
	BOOL								LoadSetup(const char *pXmlFilename);
	//!									�ҵ�һ���û�
	const			tagUserSetup*		FindUser(const char *pName) const;

	//!
	DWORD								GetTimeOutSec(VOID) const {return m_TimeOutSec;}

	//!									��ʼ��֤�û�������ͳ�ʼ�����Ƿ�һ��
	VOID								BeginUserPwdCmp(VOID);
	//!									��Ӧ�����ݿ��бȽ��û�����Ľ��
	VOID								OnUserPwdCmp(LONG lResult, const char *pName, const char *pPwd);
	//!									�鿴��֤�Ƿ����
	BOOL								IsFinishCmp(VOID);

	//!									�޸����루��Ӱ�������ļ���
	VOID								ChangePwd(const char *pName, const char *pPwd);

	//!									������������ת�����ַ���
	VOID								TransitionMD5ToStr(char *pOut, const byte *pIn);


private:
	//!									�û�����
	map<string, tagUserSetup>			m_mapUserSetup;
	//!									��ʱ����
	DWORD								m_TimeOutSec;


private:
	static	UserSetup	c_UserSetup;
};