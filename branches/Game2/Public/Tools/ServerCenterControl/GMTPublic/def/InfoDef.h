


#pragma once
//! ��Ϣ�õ��Ĺ�������
////////////////////////////////////////////////////////////////////////////////////////////////////

//! ͨ��SCC���͵�SM���������
struct  tagScc2SmSetup
{
	DWORD dwGetHardWareInfoTime;
	DWORD dwSaveHardWareInfoSize;

	DWORD dwStartTimeOut;
	DWORD dwShutdownTimeOut;
	DWORD dwPingTimeOut;
	DWORD dwDelayTestTime;

	tagScc2SmSetup(VOID){memset(this, 0, sizeof(tagScc2SmSetup));}
};

//! SM������Ϣ
//!----------------------------------------------------------------------------------
struct tagSmBaseInfo
{
	//!				��Ϣ������ʱ��
	DWORD			dwTime;
	//!				ID
	DWORD			dwID;
	//!				����
	char			szName[MAX_SM_NAME_SZIE];
	//!				��ϸ˵��
	char			szExplain[BASE_TEXT_SIZE];
	//!				��ǰ״̬
	DWORD			dwState;

	tagSmBaseInfo(VOID){memset(this, 0, sizeof(tagSmBaseInfo));}
};

//! ����������Ϣ��SM
struct tagSmInfo : public tagSmBaseInfo
{
	//!				����ID
	DWORD			dwRegionID;
	//!				��������
	char			szRegionName[MAX_SM_NAME_SZIE];
	//!				��������ID
	DWORD			dwGroupID;
	//!				������������
	char			szGroupName[MAX_SM_NAME_SZIE];

	tagSmInfo(VOID){memset(this, 0, sizeof(tagSmInfo));}
};


//! GAPP������Ϣ
//!----------------------------------------------------------------------------------

//! GAPP״̬
enum eServerState
{
	eSS_Stop		= 0,
	eSS_Running,
	eSS_Busy,
};

//! �ṹ��
struct tagGappBaseInfo
{
	//!				��Ϣ������ʱ��
	DWORD			_dwGappTime;
	//!				ID
	DWORD			_dwGappID;
	//!				����
	DWORD			_dwGappType;
	//!				��ǰ�汾
	char			_szGappVersion[MAX_GAPP_NAME_SZIE];
	//!				��ʾ��
	char			_szGappName[MAX_GAPP_NAME_SZIE];
	//!				��ǰ״̬
	DWORD			_dwGappState;
	//!				��ǰ�ӳ�
	DWORD			_dwGappDelayTime;
	tagGappBaseInfo(VOID){memset(this, 0, sizeof(tagGappBaseInfo));}
};


//! Ӳ����Ϣ
//!----------------------------------------------------------------------------------

//!				һ����Ϣ��������Ӳ����Ϣ��
const DWORD		MAX_MESSAGE_INCLUDE_HARD_INFO			= HARD_INFO_GROUP_NUM * HARD_INFO_CACHE_GROUP_NUM;


struct tagCpuUsage
{
	UCHAR cUsage;//�ܵ�CPUʹ����
	UCHAR cSubUsage[MAX_CPU_NUM];//����CPUʹ����
	tagCpuUsage(){memset(this,0,sizeof(tagCpuUsage));}
};

struct tagHardwareInfo:public tagCpuUsage
{
	//!				��Ϣ������ʱ��
	DWORD			dwTime;
	//!				��ǰ�ڴ�(���������)
	MEMORYSTATUS	stMS;
	tagHardwareInfo(VOID){memset(this, 0, sizeof(tagHardwareInfo));}
}; 


//! ��Ϸ���ݿ���Ϣ
struct tagGameDbInfo
{
	//!				��ʾ����
	char			_szShowName[BASE_NAME_SIZE];
	//!				������ID
	DWORD			_dwSMID;
	//!				����ID
	DWORD			_dwID;
};

//SMҪִ�еĶ���
enum SMActionType
{
	SETPORT=0,		//�޸Ķ˿�
};


//! GM����
//!----------------------------------------------------------------------------------

enum	{	eUsedSys = 1,eUsedGM=2,};



