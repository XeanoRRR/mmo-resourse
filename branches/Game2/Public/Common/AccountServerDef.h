

/*
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-15
*/


#pragma once


//!	�ͻ�������	
enum eAccoutClientType
{
	//! İ����
	eACT_Stranger,
	//!	ֻ���û�
	eACT_ReadOnly,
	//! ���Է�ŵ��û�
	eACT_Admin,

};

//! ��½��������
enum eAccoutLoginErrorType
{
	//! δע���
	eALET_NotRegister, 
	//! IP����
	eALET_IpDiffer,
	//! �ظ���½
	eALET_ReLogin,
};

//! ��½״̬
enum eAccoutLoginState
{
	//! ����
	eLS_OffLine,
	//! ����
	eLS_OnLine,
	//! 
};

const ULONG RESOURCE_VERSION_SIZE		= 128;
const ULONG PT_TOKEN_SIZE				= 128;
const ULONG CDKEY_SIZE					= 32;
const ULONG PWD_SIZE					= 33;
const ULONG WS_NAME_SIZE				= 20;
const ULONG SECURITY_CARD_WIDTH			= 10;
const ULONG SECURITY_CARD_HEIGHT		= 8;
const ULONG SECURITY_CARD_SIZE			= SECURITY_CARD_WIDTH * SECURITY_CARD_HEIGHT;
const ULONG SECURITY_VALIDATE_POS_NUM	= 3;


//! ʢ��PT��֤���
struct tagSdoaValidateResult 
{
	int		nResult;
	int		nUniqueId;
	char	szSndaID[CDKEY_SIZE];
	bool	bNotFcm;
	bool	bForbidden;

	tagSdoaValidateResult(void){memset(this, 0, sizeof(tagSdoaValidateResult));}
};

enum	eValidateType
{
	eVT_Not_Pwd		= 0x1,
	eVT_Not_Safe	= 0x2,
	eVT_Not_Fcm		= 0x4,
};
//! ���͵�AS���˺���Ϣ
struct tagAccountInfo_ToAS
{
	char	_szCdkey[CDKEY_SIZE];
	char	_szPwd[PWD_SIZE];
	char	_szWSName[WS_NAME_SIZE];
	long	_lWsID;
	ulong	_dwIp;
	long	_lSocket;
	ulong	_dwEndTime;
	uchar	_cValidateType;
	bool	_bLocalValidate;

	/////////////
	tagAccountInfo_ToAS(void)
	{
		memset(this, 0, sizeof(tagAccountInfo_ToAS));
		_lWsID		= -1;
	}
	tagAccountInfo_ToAS(const char *pCdKey, const char *pPwd, const char *pWSName, ulong dwIp, long lSocket)
	{
		memset(this, 0, sizeof(tagAccountInfo_ToAS));
		if(NULL != pCdKey)	strcpy_s(_szCdkey, CDKEY_SIZE, pCdKey);
		if(NULL != pPwd)	strcpy_s(_szPwd, PWD_SIZE, pPwd);
		if(NULL != pWSName)	strcpy_s(_szWSName, WS_NAME_SIZE, pWSName);
		_lWsID		= -1;
		_dwIp		= dwIp;
		_lSocket	= lSocket;
	}
	tagAccountInfo_ToAS(const tagAccountInfo_ToAS& src)
	{
		memcpy(this, &src, sizeof(tagAccountInfo_ToAS));
	}
private:

};


enum ePlayerLoginResult
{
	//! ��½ʧ��
	ePLR_Lost,
	//! ��ͨ��½
	ePLR_Normal,
	//! 
};

//! ��½���
struct tagLoginRerultType
{
	//!		��½�ɰ�
	ulong	_dwLoginResult;
	//!		��Ϣ����
	long	_InfoCode;
};

//! ��ȫ��֤������
struct tagSecurityData
{
	char		_arrSecurityCard[SECURITY_CARD_SIZE];
	long		_lWorkFlag;

	tagSecurityData(void){memset(this, 0, sizeof(tagSecurityData));}
	tagSecurityData(const tagSecurityData &srcObj){memcpy(this, &srcObj, sizeof(tagSecurityData));}
};

//! AS���ص��˺���Ϣ
struct tagAccountInfo_FromAS : public tagAccountInfo_ToAS
{
	tagLoginRerultType	_LoginRerult;

	/////////////
	tagAccountInfo_FromAS(void)
	{
		memset(this, 0, sizeof(tagAccountInfo_FromAS));
	}
	tagAccountInfo_FromAS(tagAccountInfo_ToAS &ToAs)
	{
		memset(&_LoginRerult, 0, sizeof(tagLoginRerultType));
		memmove(this, &ToAs, sizeof(tagAccountInfo_ToAS));
	}
};

struct tagLSSecurityData : public tagSecurityData
{
	char arrPos[SECURITY_VALIDATE_POS_NUM];
	tagAccountInfo_FromAS _AccountInfo_FromAS;

	tagLSSecurityData(void){memset(this, 0, sizeof(tagLSSecurityData));}
	explicit tagLSSecurityData(const tagSecurityData& BaseData, const tagAccountInfo_FromAS AccountInfo_FromAS)
		:tagSecurityData(BaseData), _AccountInfo_FromAS(AccountInfo_FromAS)
	{
		memset(arrPos, 0, SECURITY_VALIDATE_POS_NUM);
	}
};
