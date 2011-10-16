

/*
*	file:		LinkmanDef.h
*	Brief:		���ں��ѵ�һЩC/S�˹��õĶ���
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-18
*/

#pragma once

#include "GUID.h"

//! ��������ֵ��
//!-------------------------------------------------------------------------------

//!			��ϵ����ƺ���󳤶�
const ULONG MAX_APPELLATIVE_SIZE		= 16 + 1;
//!			����ļ�·������
const ULONG MAX_FILE_PATH_SIZE			= 256;

//!			������ֳ���
const ULONG MAX_PLAYER_NAME_SIZE		= 20 + 1;
//!			��ͼ���ֳ���
const ULONG MAX_PLAYER_TITLE_SIZE		= 256 + 1;



//!			�����ϵ�˲��������Ựʱ�䣨�룩
const ULONG	MAX_LINKMAN_OPT_TIME_SEC	= 60;
//!			��Ч����ϵ��ID
const ULONG INVALID_GROUP_ID			= 0xFFFFFFFF;


//! ö��
//!-------------------------------------------------------------------------------
//!		��ϵ������
enum eLinkmanGroupType
{
	//! ������
	eLGT_Friend,
	//! ��ż��
	eLGT_Spouse,
	//! İ������
	eLGT_Stranger,
	//! ������
	eLGT_Hellion,
	//! ������
	eLGT_Blacklist,

	//! ��С��Чֵ
	eLGT_Invalid,
};


//! �������ȼ�
enum eLeaveWordPRI
{
	//! ��������
	eLWPRI_Cant				= 0,

	//! ���ȶ���
	eLWPRI_Steadyest		= 1,
	//! ��ȶ���
	eLWPRI_UnSteadyest		= 999,

	
};



//! ����Ƶ�����
enum eSecretTalkChannel
{
	//! Ƶ������ֵ
	eSTC_BaseValue			= 0xFF00,
	//! Ƶ�����ֵ
	eSTC_MaxValue			= 0xFFFF,
	//! ��С��Чֵ(��Ƶ��)
	eSTC_Invalid			= 0,
};


//! �洢��ʽ
enum eKeepWay
{
	//! ����������
	eKW_ServerKeep,
	//! �ͻ��˱���
	eKW_ClientKeep,
	//! ������
	eKW_NotKeep,
};

//! ������ʾ��Ϣ(��λ��ʾ)
enum eTimelyInfoType
{
	//! ��ʾ������ڵ�ͼ
	eTIT_Show_MapName		= 1 << 0,
	//! ��ʾ�ȼ�
	eTIT_Show_Level			= 1 << 1,
	//! ��ʾְҵ
	eTIT_Show_Occupation	= 1 << 2,

	//! ����ʾ������Ϣ
	eTIT_Hide_All			= 0,
	//! ��ʾ������Ϣ
	eTIT_Show_All			= 0xFFFFFFFF,
};


//! ���ι��ܣ���λ��ʾ��
enum eFunctionFiltrateType
{
	//! ������������
	eFFT_Filtrate_Chat		= 1 << 0,
	//! �����������
	eFFT_Filtrate_Team		= 1 << 1,

	//! ����������
	eFFT_UuFiltrate_All		= 0,
	//! ��������
	eFFT_Filtrate_All		= 0xFFFFFFFF,
};

//! �Զ���Ӱ취
enum eAutoAddType
{
	//! ���Զ����
	eAAT_NotAdd,
	//! ������Ĺ�����
	eAAT_SecretTalk,
	//! ��ӹ�����
	eAAT_Attacker,
	//! ��ż
	eAAT_Spouse,
};

//! ��ϵ�����ɾ���Ĳ�����
enum eGroupOperatorType
{
	eGOT_Client,
	eGOT_Server,
};


//! ��Ա�ƶ����ȼ�
enum eOnlyPRT
{
	//! ���ȶ���
	eOPRI_Steadyest			= 1,
	//! ��ȶ���
	eOPRI_UnSteadyest		= 999,
};

//! ��ϵ��״̬
enum eLinmanStateType
{
	//! δ֪״̬
	eLST_Unkown,
	//! ɾ��״̬
	eLST_Deleted,
	//! ����״̬
	eLST_Offline,
	//! ����״̬
	eLST_Online,
};



//! ��ϵ����Ϣö��
enum eLinmanInfoType
{
	//! ��ͼ
	eLIT_Map,
	//! �ȼ�
	eLIT_Level,
	//! ְҵ
	eLIT_Occupation,
};



//! ��ϵ�˲�������ö��
enum eLinmanHandleType
{
	//! ���
	eLHT_Add,
	//! ɾ��
	eLHT_Del,
};



//! ���ѵ�ɾ��״̬
enum eLinmanDelState
{
	//! δɾ��
	eLDS_NotDel,
	//! �����Ϊɾ��
	eLDS_FlagDel,
	//! DB��������ʵɾ��
	eLDS_DataDel,
};



//! �ͻ��˲�������ԭ��
enum eOptErrorType
{
	//! ����Ŀ�겻�ڱ�������
	eOET_AimNotBeing,
	//! �����ͬһ����ҷ��ͺ�����ص�����
	eOET_OverfullToOne,
	//! �Է��ܾ����
	eOET_AimReject,
	//! �Է��б�����
	eOET_ListFull,
	//! �Է��������������
	eOET_AimInOther,
	//! �Է�����
	eOET_AimHideName,
};




//! �ṹ
//!-------------------------------------------------------------------------------

//! A
struct tagLinkmanBase_data
{
	CGUID		guid;								// GUID
	CHAR		szName[ MAX_PLAYER_NAME_SIZE ];		// ����
	ULONG		uState;								// ״̬��eLinmanStateType��
};

//! A\B
//! ���ڱ������ϵ�����ݽṹ
struct tagLinkman_data : public tagLinkmanBase_data
{
	CHAR		szMapName[ MAX_PLAYER_TITLE_SIZE ];	// ���ڵ�ͼ����
	LONG		lLevel;								// �ȼ�
	LONG		lOccupation;						// ְҵ
	
	tagLinkman_data(void){memset(this, 0, sizeof(tagLinkman_data));}
};

//! A\C
//! ������Ϣ����ϵ�����ݽṹ
struct tagLinkmanBase : public tagLinkmanBase_data
{
	BOOL		bMutual;							//! ���ӹ�ϵ��ʾ
};

//! A\B\C
//! ������Ϣ����ϵ�����ݽṹ
struct tagLinkman : public tagLinkman_data
{
	tagLinkman(void){memset(this, 0, sizeof(tagLinkman));}
	BOOL		bMutual;							//! ���ӹ�ϵ��ʾ
};





//! DB��ϵ���ݽṹ
struct  tagLinkman_DB
{
	tagLinkman_DB(void){memset(this, 0, sizeof(tagLinkman_DB));}
	CGUID		OwnerGUID;								//! ���GUID
	CGUID		AimGUID;								//! �������GUID
	ULONG		uGroupID;								//! ��ϵ������ID
	LONG		uDelState;								//! ɾ��״̬��eLinmanDelState��
	char		szAimName[MAX_PLAYER_NAME_SIZE];		//! �����������

	inline bool operator ==(const tagLinkman_DB& right) const
	{
		return (OwnerGUID == right.OwnerGUID && AimGUID == right.AimGUID && uGroupID == right.uGroupID);
	}
	inline bool operator <(const tagLinkman_DB& right) const
	{
		return OwnerGUID < right.OwnerGUID;
	}
};



struct tagMeAddPlayer
{
	BOOL			_IsMutual;	//! �໥��ϵ
	tagLinkman_data	*_date;		//! ����
};
