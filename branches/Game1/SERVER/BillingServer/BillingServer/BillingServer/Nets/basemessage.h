#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgQueue.h"

class CMySocket;
class CMessage;
class CGUID;

// ������Ϣ�б�
enum
{
	LOGIN_OK = 1,					// ��½�ɹ�
	LOGIN_LOGINSERVEROK,
	LOGIN_GAMESERVEROK,
	LOGIN_VERSIONERROR,				// ��Ϸ�汾����
	LOGIN_NOTCDKEY,					// û�����CDKEY
	LOGIN_ACC_LOCKED,				// �ʺű�����
	LOGIN_PASSWORDERROR,			// �������
	LOGIN_CHECKOUT,					// ������ҵ�½��Ϸ
	LOGIN_NOTLOGINSERVER,           //û�е�½��½������,

	LOGIN_PLAYER_ALREAD_IN_GAME,	// ������Ѿ�����Ϸ����
	LOGIN_PLAYER_LOAD_ERROR,		// ��ȡ�������ʧ��
	LOGIN_PLAYER_IN_OTHER_SERVER,	// ��ұ������������ǳ���
	LOGIN_PLAYER_NO_REGION,			// û���ҵ���ҵĳ���
	WORLD_REQUESTRGIONSERVEROK,
	LOGIN_CDKEY_IN_OTHER_SERVER,	// CDKEY�������������ǳ���
	LOGIN_BEBANED,					// ��CDKEY����
	LOGIN_CDKEYDISABLE,				// cdkey������
	LOGIN_IP_FORBID,				// ip������
	LOGIN_DELETEROLE_FAILED,        //ɾ����ɫ�ɹ�
	LOGIN_DELETEROLE_SUCCEED,       //ɾ����ɫʧ��
	LOGIN_RESTOREROLE_SUCCEED,		// �ָ���ɫ�ɹ�
	LOGIN_RESTOREROLE_FAILED,		// �ָ���ɫʧ��
	LOGIN_CREATEROLE_NAMEEXIST,     //������ɫʧ��(��������Ѿ�����)
	LOGIN_CREATEROLE_DBERROR,		//������ɫʧ��(���ݿ����ʧ��)
	LOGIN_CREATEROLE_FULL,			//������ɫʧ��(��ɫ����ʧ��)
	LOGIN_CREATEROLE_NAMEINVALID,	//����INVALID������ɫ��������ϵ�INVALID�ַ���
	LOGIN_CREATEROLE_SUCCEED,		//������ɫ�ɹ�
	LOGIN_QUEST_PLAYERDATA_FAILED,	//������ϸ��Ϣʧ��
	LOGIN_QUEST_PLAYERDATA_SUCCEED,	//������ϸ��Ϣ�ɹ�
	GOODS_FETCH_SUCCEED,			//����Ʒ�ɹ�
	GOODS_FETCH_FAILED,				//����Ʒʧ��
	GOODS_PUTDOWN_SUCCEED,			//����Ʒ�ɹ�
	GOODS_PUTDOWN_FAILED,			//����Ʒʧ��
	GOODS_CHANGE_SUCCEED,			//������Ʒ�ɹ�
	GOODS_CHANGE_FAILED,			//������Ʒʧ��
	GOODS_DROP_SUCCEED,				//����Ʒ�ɹ�
	GOODS_DROP_FAILED,				//����Ʒʧ��
	GOODS_PICK_SUCCEED,				//����Ʒ�ɹ�
	GOODS_PICK_NOTSPACE,			//����Ʒʧ��(ԭ��:������û���㹻�ռ�)
	GOODS_PICK_NOTFOUND,			//����Ʒʧ��(ԭ��:�Ҳ���Ҫʰȡ����Ʒ)
	GOODS_PICK_PROTECTED,			//����Ʒʧ��(ԭ��:��Ʒ������)
	GOODS_PICK_TOOHEAVY,			//����Ʒʧ��(ԭ��:��������)
	GOODS_PICK_NEW,
	GOODS_PICK_OVERLAP,
	HOTKEY_SET_SUCCEED,				//�ȼ����óɹ�
	HOTKEY_SET_FAILED,				//�ȼ�����ʧ��
	HOTKEY_CLEAR_SUCCEED,			//�ȼ�ȡ���ɹ�
	HOTKEY_CLEAR_FAILED,			//�ȼ�ȡ��ʧ��
	HOTKEY_EXCHANGE_SUCCEED,		//�ȼ������ɹ�
	HOTKEY_EXCHANGE_FAILED,			//�ȼ�����ʧ��
	PLAYER_USEITEM_SUCCEED,			//���ʹ�õ��߳ɹ�
	PLAYER_USEITEM_FAILED,			//���ʹ�õ���ʧ��

	PLAYER_USEITEM_SKILLTOOLOW,		//���ʹ�õ���ѧϰ���ܣ���ȼ��������м��ܵȼ���ʧ��
	PLAYER_USEITEM_SKILLTOOHIGH,	//���ʹ�õ���ѧϰ���ܣ���������ѧϰ���ܶ�ʧ��
	SHOP_BUY_SUCCEED,				
	SHOP_BUY_MONEYNOTENOUGH,
	SHOP_BUY_GOODSNOTFOUND,
	SHOP_REPAIR_SUCCEED,
	SHOP_REPAIR_MONEYNOTENOUGH,
	SHOP_REPAIR_FAILED,
	TRADE_SUCCEED,
	TRADE_FAILED,


	LOGINDB_ERROR,
	WORLDDB_ERROR,

	LOGIN_CREATEROLE_FAILED,		//������ɫʧ��

	LOGIN_MATRIX_POS,				//�Ϸ���֤������,
	LOGIN_MATRIX_RES_OK,
	LOGIN_MATRIX_RES_FAILED,
	LOGIN_MATRIX_RES_TIMEOUT,
	LOGIN_MATRIX_RES_KICKOUT,

	LOGIN_EKey_Err,
	LOGIN_TimeOut,  

    LOGIN_VALID_CODE,           // ������֤��
    LOGIN_VALID_CODE_OK,        // ��֤����ȷ
    LOGIN_VALID_CODE_FAILED,    // ��֤�����
    LOGIN_VALID_CODE_OVERTIME,  // ��֤��ʱ
    LOGIN_VALID_CODE_KICKOUT,   // ͬ�ʺ��������ط���¼��������֤�����
    LOGIN_VALID_CODE_SEND_FREQUENT, // ��֤�뷢��Ƶ��
    LOGIN_VALID_CODE_SEND_FAILED,   // ��֤�뷢��ʧ��

    LOGIN_FAILED_MANY_TIMES,    // ��֤ʧ��̫���

	// gongran sina 2008-3-3
	//LOGIN_UTG_PASSWORD_ERROR,	// �������
	//LOGIN_UTG_ACC_FORBIDDEN,	// �û���ͣȨ
	//LOGIN_UTG_ACC_INEXISTENT,	// �û�������
	//LOGIN_UTG_SERVER_INFO_ERROR,// ��������Ϣ����
	//LOGIN_UTG_TIME_OUT,			// ������ʱ
};

/* -----------------------------------------------------
[CMessage] ��Ϣ����

���ܣ�
��Ϸ����Ϣ���ӱ�������
����ÿ����Ϣ�Ļ������ݣ���С�����ͺ�ʱ�䣩
�ṩ��������װ��Ϣ��������Ϣ����

-------------------------------------------------------*/
class CBaseMessage
{
public:

	// ��������һ����Ϣ
	// CMessage msg(QUEST_JOIN);
	// msg.Add(long)��						// ��������
	// msg.Add(char*);
	// msg.Send();							// ����

	enum
	{
		MAX_SUB			= 0x100,				// һ��С���͵������Ϣ����
		MAX_TYPE		= MAX_SUB * 0xff,		// һ�������͵������Ϣ����
	};

	enum
	{
		//////////////////
		//				//
		//	LoginServer	//
		//				//
		//////////////////

		//========================================================================================================
		MSG_L2L = MAX_TYPE * 1,			// LoginServer --> LoginServer
		//========================================================================================================

		MSG_L2L_SERVER				= MSG_L2L + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_L2L_SERVER_WORLDSERVERLOST,							// LoginServer��⵽WorldServer�Ͽ�

		MSG_L2L_LOG					= MSG_L2L + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_L2L_LOG_CLIENTCLOSE,								// LoginServer��⵽Client�Ͽ�

		//========================================================================================================
		MSG_W2L = MAX_TYPE * 2,			// WorldServer --> LoginServer
		//========================================================================================================

		MSG_W2L_SERVER				= MSG_W2L + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_W2L_SERVER_SENDWORLDINFO,							// ����WORLD��Ϣ��LoginServer
		MSG_W2L_SERVER_SENDCDKEYLIST,							// ����ʱ����World�ϵ�Cdkeylist��LoginServer
		MSG_W2L_SERVER_CLEARCDKEYS,								// gameserver�Ͽ�,worldserver��loginserver������Ҫ�����CDKEY

		MSG_W2L_SERVER_ANSWER_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����

		MSG_W2L_SERVER_LOG_GS_CONNECT,
		MSG_W2L_SERVER_LOG_RELOAD_CONF,

		MSG_W2L_SERVER_Response_Info,							// ��������
		MSG_W2L_SERVER_Log_Error,								// ��¼������������־

		MSG_W2L_LOG					= MSG_W2L + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_W2L_LOG_ANSWER_PLAYERDATA,							// worldserver��loginserver������ϸ��Ϣ��ȡ�Ƿ�ɹ�
		MSG_W2L_LOG_ANSWER_PLAYERBASELIST,						// worldserver��loginserver������һ�����Ϣ�б�
		MSG_W2L_LOG_DELETEROLE,									// worldserver���ؽ�ɫ�Ƿ�ɹ�ɾ��
		MSG_W2L_LOG_RESTOREROLE,								// WORLDSERVER���ؽ�ɫ�Ƿ�ָ�
		MSG_W2L_LOG_CREATEROLE,									// worldserver���ؽ�ɫ�Ƿ�ɹ�����
		MSG_W2L_LOG_PLAYERQUIT,									// worldserver��loginserver�����˳���ҵ�cdkey

		MSG_W2L_LOG_SENDACCOUNTINFO,							// �����ʺŵ���Ϣ���ͻ���

		MSG_W2L_GM					= MSG_W2L + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_W2L_GM_BANPLAYER,

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool BEGIN
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MSG_W2L_GMA = MSG_W2L + MAX_SUB * 3,

		MSG_W2L_GMA_RESPONSE_KICKPLAYER,
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool END
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//========================================================================================================
		MSG_C2L = MAX_TYPE * 3,			// Client --> LoginServer
		//========================================================================================================

		MSG_C2L_LOG					= MSG_C2L + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_C2L_LOG_QUESTCDKEY,									// �ͻ��˵�½ʱ����CDKEY
		MSG_C2L_LOG_QUEST_PLAYERBASELIST,						// �ͻ��˵�½ʱ�����ʺ��µ���һ��������б�
		MSG_C2L_LOG_QUEST_PLAYERDATA,							// ��LOGINSERVER������ϸ����
		MSG_C2L_LOG_QUESTCREATE,								// �ͻ���Ҫ����һ���µĽ�ɫ
		MSG_C2L_LOG_QUESTDELETE,								// �ͻ���Ҫ��ɾ��һ����ɫ
		MSG_C2L_LOG_QUESTRESTORE,								// CLIENT ����ָ�һ����ɫ
		MSG_C2L_LOG_QUESTEXIT,									// ����GameServer�Ľ��
		MSG_C2L_LOG_VALIDATE_MATRIX,							// ������֤����
        MSG_C2L_LOG_QUEST_VALID_CODE,                           // �����������֤��֤��
        MSG_C2L_LOG_QUEST_VALID_CODE_ANOTHER,                   // ����������ط���֤��


		//////////////////
		//				//
		//	WorldServer	//
		//				//
		//////////////////

		//========================================================================================================
		MSG_W2W = MAX_TYPE * 4,			// WorldServer --> WorldServer
		//========================================================================================================

		MSG_W2W_SERVER					= MSG_W2W + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_W2W_SERVER_LOGINSERVERCLOSE,						// WorldServer��⵽LoginServer�ر�
		MSG_W2W_SERVER_GAMESERVERLOST,							// WorldServer��⵽GameServer�Ͽ�
		MSG_W2W_RECONNECTLOGINSERVEROK,							// ����LoginServer�ɹ�

		//========================================================================================================
		MSG_L2W = MAX_TYPE * 5,			// LoginServer --> WorldServer
		//========================================================================================================

		MSG_L2W_LOG					= MSG_L2W + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_L2W_LOG_QUEST_PLAYERBASELIST,						// loginserver��world������һ�����Ϣ�б�
		MSG_L2W_LOG_DELETEROLE,									// loginserver��worldserverת��ɾ����ɫ����
		MSG_L2W_LOG_RESTOREROLE,								// LOGINSERVER��worldserverת���ָ���ɫ����
		MSG_L2W_LOG_CREATEROLE,									// loginserver��worldserverת��������ɫ����
		MSG_L2W_LOG_QUEST_PLAYERDATA,							// loginserver��worldserverת����ȡ��ϸ��Ϣ����
		MSG_L2W_LOG_KICKPLAYER,                                 // loginServer��WorldServer����һ��Ҫ���߳�ĳ����ҵ���Ϣ
		MSG_L2W_LOG_REPEAT_LOGIN,								// Login Server֪ͨWorld Server��һ���������½һ�ʺŲ��滻��ԭ��ʹ�ø��ʺŵ����ӡ�

		MSG_L2W_SERVER				= MSG_L2W + MAX_SUB * 1,
		MSG_L2W_SERVER_QUEST_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����
		MSG_L2W_SERVER_Request_Info,							// ��ȡGS����
		MSG_L2W_SERVER_SendSetup,

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool BEGIN
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MSG_L2W_GMA = MSG_L2W + MAX_SUB * 2,
		
		MSG_L2W_GMA_KICKPLAYER,
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool END
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//========================================================================================================
		MSG_S2W = MAX_TYPE * 6,			// GameServer --> WorldServer
		//========================================================================================================

		MSG_S2W_SERVER					= MSG_S2W + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_S2W_SERVER_SENDSERVERINFO,							// ����server��Ϣ��WorldServer
		MSG_S2W_SERVER_CHANGEREGION,							// �����л�������
		MSG_S2W_SERVER_SAVERESPONSE,							// ���ʹ��̵���Ϣ
		MSG_S2W_SERVER_ADDINFO,									// ��Ҳ��ڵ�ǰ������������World Server����ҶԻ������һ����Ϣ
		MSG_S2W_SERVER_CHANGE_SCRIPT_VARIABLE,					// �ı�ű�������ֵ
		MSG_S2W_SERVER_REPORT_MURDERER,							// ����ɱ����
		MSG_S2W_SERVER_UpdateRegionInfo,						// ���泡����Ϣ
		MSG_S2W_SERVER_UPDATEGOODSID,							// ͬ��GoodsID

		MSG_S2W_SERVER_UPDATEPLAYERDATA,						// ͬ������������ҵ�����
		MSG_S2W_SERVER_ANSWER_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����
		MSG_S2W_SERVER_CREATEMONSTER,							// ��������
		MSG_S2W_SERVER_Response_Info,

		MSG_S2W_SERVER_Log_Error,								// ������������־

		MSG_S2W_LOG						= MSG_S2W + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_S2W_LOG_QUEST_PLAYERDATA,							// GameServer��WorldServer����player��ϸ����
		MSG_S2W_LOG_PLAYERQUIT,									// gameserver��worldserver�����˳���ҵ�playerid

		MSG_S2W_PLAYER					= MSG_S2W + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_S2W_PLAYER_ADDSKILL,							
		MSG_S2W_PLAYER_DELSKILL,
		MSG_S2W_PLAYER_DELGOODS,
		MSG_S2W_PLAYER_SETLEVEL,



		MSG_S2W_OTHER					= MSG_S2W + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_S2W_OTHER_TALK,										// �Ի���Ϣ
		MSG_S2W_OTHER_ADDINFO,							
		MSG_S2W_OTHER_GOODSLINK,                                // gameserver����һ����Ʒ������worldserver
		MSG_S2W_OTHER_VIEWGOODSLINK,				

		MSG_S2W_OTHER_CHANGENAME,								// Gsת��C�����ĸ�������Ws
		MSG_S2W_OTHER_BROADCAST,								// �㲥��Ϣ

		MSG_S2W_OTHER_TALK_WORLD,								// ���纰��
		MSG_S2W_OTHER_TALK_COUNTRY,								// ���Һ���

		MSG_S2W_OTHER_TALK_NOTIFY,								// ����֪ͨ

		MSG_S2W_OTHER_GETLOG_REQUEST,							// ������ֵ��ֵ

		MSG_S2W_DEPOT					= MSG_S2W + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_S2W_DEPOT_OPEN,										// ���ֿ�

		MSG_S2W_GM						= MSG_S2W + MAX_SUB * 5,
		// ----------------------------------------------------
		MSG_S2W_GM_QUERY_PLAYERCOUNT,							//��ѯ�û�����
		MSG_S2W_GM_QUERY_PLAYERDATA,							//��ѯ�û�����
		MSG_S2W_GM_ANSWER_PLAYERDATA,					
		MSG_S2W_GM_QUERY_FINDREGION,							//��ѯ��ͼ		
		MSG_S2W_GM_QUERY_PLAYERONLINE,							//��ѯ�û�����״��
		MSG_S2W_GM_RELOAD,										// �ض������ļ�
		MSG_S2W_GM_KICKPLAYER,									// ���������
		MSG_S2W_GM_KICKPLAYEREX,								// ������ gameserver 2 worldserver
		MSG_S2W_GM_ANSWER_KICKPLAYER,							// Ӧ�����������
		MSG_S2W_GM_KICK_ALLPLAYER,								// �������������
		MSG_S2W_GM_KICK_REGIONPLAYER,							// ��һ�������е��������
		MSG_S2W_GM_SILENCE_PLAYER,								// ����һ����ҽ���
		MSG_S2W_GM_ANSWER_SILENCE_PLAYER,						// �𸴽�һ����ҽ���
		MSG_S2W_GM_WORLD_MESSAGE,								// �������������ҷ�����Ϣ
		MSG_S2W_GM_QUERY_SILENCE_PLAYERS,						// �������������ѯ���Ե����
		MSG_S2W_GM_ANSWER_QUERY_SILENCE_PLAYERS,				// ��������������ؽ��Ե�����б�
		MSG_S2W_GM_CHANGEREGION,
		MSG_S2W_GM_BANPLAYER,
		MSG_S2W_GM_SAVEPLAYER,
		MSG_S2W_GM_QUERYGM,
		MSG_S2W_GM_ANSWERGM,
		//MSG_S2W_GM_TEAMCHANGEREGION,							//�����л�����
		MSG_S2W_GM_COUNTRY_MESSAGE,								// ���������ҵ���ҷ�����Ϣ

		MSG_S2W_TEAM					= MSG_S2W + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_S2W_TEAM_CREATE,
		MSG_S2W_TEAM_DESTORY,
		MSG_S2W_TEAM_TEAMATE_JOIN,
		MSG_S2W_TEAM_TEAMATE_LEAVE,
		MSG_S2W_TEAM_TEAMATE_CHANGE_REGION,
		MSG_S2W_TEAM_CHANGE_LEAER,									//�ı�ӳ�
		MSG_S2W_TEAM_KICK_PLAYER,
		MSG_S2W_TEAM_QUEST_SYNCHRONIZE,								//����ͬ��
		MSG_S2W_TEAM_QUERY_PLAYER_RESULT,							//��ѯ����Ƿ���ڵĽ������.
		MSG_S2W_TEAM_CHANGE_ALLOCATION_SCHEME,						//�ı���䷽ʽ.
		MSG_S2W_TEAM_CHAT,
		MSG_S2W_TEAM_CHANGE_STATE,									//ѧ���ı�

		//MSG_S2W_TEAM_QUERY_TEAMID,								//Ѱ�ʶ���ID ֻ������������
		//MSG_S2W_TEAM_TEAMMATE_JOIN,								//��Ա����
		//MSG_S2W_TEAM_TEAMMATE_QUIT,								//��Ա���
		//MSG_S2W_TEAM_TEAMMATE_KICK,								//����
		//MSG_S2W_TEAM_TEAMMATE_MIGRATE_OUT,						//ת��
		//MSG_S2W_TEAM_TEAMMATE_MIGRATE_IN,						//ת��
		//MSG_S2W_TEAM_UPLOAD,									//�ϴ�������Ϣ	
		//MSG_S2W_TEAM_QUERY_SIZE,								//ѯ�ʶ����С
		//MSG_S2W_TEAM_MISS_CAPTAIN_INFO,							//��ʧ�ӳ���Ϣ	
		//MSG_S2W_TEAM_CHANGE_CAPTAIN,							//�����ӳ�	
		//MSG_S2W_TEAM_SPREAD_CAPTAIN_CHANGE,						//�����ӳ���Ϣ
		//MSG_S2W_TEAM_CHAT,
		//MSG_S2W_TEAM_CHANGEREGION,								//�����л�����

		MSG_S2W_ORGSYS					= MSG_S2W + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_S2W_ORGSYS_OnPlayerDied,
		MSG_S2W_ORGSYS_OnDeleteRole,
		MSG_S2W_ORGSYS_CREATEFACTION,							//�������
		MSG_S2W_ORGSYS_GetOrganizing,							//�õ���ᡢͬ��
		MSG_S2W_ORGSYS_GetFaction,								//�õ����
		MSG_S2W_ORGSYS_GetUnion,								//�õ����
		MSG_S2W_ORGSYS_GETFACTIONLIST,							//�õ�����б�
		MSG_S2W_ORGSYS_APPLYJOINFACTION,						//���������
		MSG_S2W_ORGSYS_CANCELJOINFACTION,						//ȡ��������
		MSG_S2W_ORGSYS_DOJOINFACTION,							//�����ᴦ��
		MSG_S2W_ORGSYS_FireOutFactionMember,					//������Ա
		MSG_S2W_ORGSYS_FireOutUnionMember,						//������Ա
		MSG_S2W_ORGSYS_ExitFaction,								//�˳����
		MSG_S2W_ORGSYS_ExitUnion,								//�˳�ͬ��
		MSG_S2W_ORGSYS_Demise,									//������λ
		MSG_S2W_ORGSYS_UnionDemise,								//������λ
		MSG_S2W_ORGSYS_Disband,									//��ɢ���
		MSG_S2W_ORGSYS_DisbandUnion,							//��ɢͬ��
		MSG_S2W_ORGSYS_DubJobLvl,								//����ƺźͰ��ְ��
		MSG_S2W_ORGSYS_EndueRight,								//����Ȩ��
		MSG_S2W_ORGSYS_AbolishRight,							//ȡ��Ȩ��
		MSG_S2W_ORGSYS_RequestEstaUnion,						//������ͬ��
		MSG_S2W_ORGSYS_AnswerAffirmEstaUnion,					//�ظ�ȷ�Ͻ���ͬ��
		MSG_S2W_ORGSYS_InviteJoinUnion,							//�������ͬ��
		MSG_S2W_ORGSYS_AnswerAffirmJoinUnion,					//�ظ�ȷ������ͬ��
		MSG_S2W_ORGSYS_TurnOnLeaveWord,							//�������Թ���
		MSG_S2W_ORGSYS_LeaveWord,								//����
		MSG_S2W_ORGSYS_DelLeaveWord,							//ɾ������
		MSG_S2W_ORGSYS_EditPronounce,							//�༭����
		MSG_S2W_FACTIONWAR_GETFACTIONLIST,						//��ս�õ�����б�
		MSG_S2W_ORGSYS_DigUpTheHatchet,							//��ս
		MSG_S2W_ORGSYS_AnswerAffirmDigUpTheHatchet,				//�ظ�ȷ����ս
		MSG_S2W_ORGSYS_MakePeace,								//���
		MSG_S2W_ORGSYS_AnswerAffirmMakePeace,					//�ظ�ȷ�����
		MSG_S2W_ORGSYS_Surrender,								//Ͷ��
		MSG_S2W_ORGSYS_AnswerAffirmSurrender,					//�ظ�ȷ��Ͷ��
		MSG_S2W_ORGSYS_RequestGetFactionBillboard,				//�õ�������а�
		MSG_S2W_ORGSYS_UpgradeFaction,							//�����������
		MSG_S2W_ORGSYS_UploadFactionIcon,						//�ϴ����ձ�
		MSG_S2W_ORGSYS_SetFactionControbute,					//���ð��Ĺ�����
		MSG_S2W_ORGSYS_FactionIncExp,							//��������Ӿ���
		MSG_S2W_ORGSYS_PlayerInfoChange,						//��ҵ������Ϣ�ı���
		MSG_S2W_CITY_RequestObtainTaxPayment,					//��ȡ˰��
		MSG_S2W_CITY_RequestAdjustTaxRate,						//����˰��
		MSG_S2W_CITY_UpdateTaxRate,								//����˰��
		MSG_S2W_CITY_AddTaxMoney,								//��ĳ���������˰��
		MSG_S2W_CITY_OperatorCityGate,							//��������
		MSG_S2W_CITY_Transfer,									//ת������
		MSG_S2W_CITY_AnswerAffirmTransfere,						//�ظ�ȷ��ת������
		MSG_S2W_CITY_SetPermit,									//�����Ƿ�����ͬ�ˡ���ս
		MSG_S2W_ATTACKCITY_OnResult,							//���ͱ�־��ݻ�
		MSG_S2W_ATTACKCITY_AskAttackCityTime,					//ѯ�ʹ���ʱ��
		MSG_S2W_ApplyForVillageWar,								//����μ�����ݵ�ս
		MSG_S2W_WAR_FactionWinVillage,							//��ս��һ������ʤ��
		MSG_S2W_CITYWAR_DeclareWarc,							//��������ս��ս
		MSG_S2W_WAR_FactionWinCity,								//��������ս��һ������ʤ��

		//�������
		MSG_S2W_QUEST_PlayerAddQuest,							//��ĳ��������һ������
		MSG_S2W_QUEST_PlayerRemoveQuest,						//��ĳ������Ƴ�һ������
		MSG_S2W_QUEST_PlayerRunScript,							//��һ����Ҵ���һ���ű�

		MSG_S2W_SetFacParam_ByMasterPlayerID,					//���ð�����ֵ
		MSG_S2W_SetFacParam_ByFactionID,
		MSG_S2W_SetFacParam_ByFactionName,
		MSG_S2W_ChangeFacParam_ByMasterPlayerID,				//�ı������ֵ
		MSG_S2W_ChangeFacParam_ByFactionID,
		MSG_S2W_ChangeFacParam_ByFactionName,


		// ��־
		MSG_S2W_WRITELOG				= MSG_S2W + MAX_SUB * 8,//��־
		// ----------------------------------------------------
		MSG_S2W_WRITELOG_GOODSTRADE,							//������Ʒ
		MSG_S2W_WRITELOG_GOODS,									//��Ʒ�䶯������ʰ�𣬶���������
		MSG_S2W_WRITELOG_GOODSUPGRADE,							// ��Ʒ����
		MSG_S2W_WRITELOG_GOODSGEMEXCHANGE,						// �Ի���ʯ
		MSG_S2W_WRITELOG_GOODSJEWELRYMADE,						// ��������

		MSG_S2W_WRITELOG_PLAYERLEVEL,							//��ɫ����
		MSG_S2W_WRITELOG_PLAYEREXP,								//����䶯��������ӣ��������٣�
		//MSG_S2W_WRITELOG_PLAYERDELETED,						//��ɫɾ��
		MSG_S2W_WRITELOG_PLAYERDIED,							//��ҽ�ɫ������־

		MSG_S2W_WRITELOG_TEAM,									//������־�������ͽ�ɢ��
		MSG_S2W_WRITELOG_PLAYERKILLER,							//���PK

		MSG_S2W_WRITELOG_CHAT,									//������־
		MSG_S2W_WRITELOG_CHANGEMAP,								//�л�������־

		MSG_S2W_WRITELOG_INCREMENT,								// ��ֵ��־

		MSG_S2W_WRITELOG_LOG,									// �Զ�����־

		// ����
		MSG_S2W_COUNTRY					= MSG_S2W + MAX_SUB * 9,
		// -----------------------------------------------------
		MSG_S2W_COUNTRY_CHANGE,									// �ı����
		MSG_S2W_COUNTRY_POWER_CHANGE,
		MSG_S2W_COUNTRY_TECHLEVEL_CHANGE,
		MSG_S2W_COUNTRY_CHANGE_CI,

		//////////////////////////////////////////////////////////////////
		// ����Ȩ�� S2W yulefox
		MSG_S2W_COUNTRY_KING_GETSTATE,							// �������� : ��ȡ��ǰ״̬���Ƿ��ǹ�������ս״̬��
		MSG_S2W_COUNTRY_KING_GetInfo,							// �������� : ��ȡ���й�����Ϣ
		MSG_S2W_COUNTRY_KING_GetPlayerList,						// �������� : ��ȡ��������б�
		MSG_S2W_COUNTRY_KING_Demise,							// �������� : ����
		MSG_S2W_COUNTRY_KING_Appoint,							// �������� : ����
		MSG_S2W_COUNTRY_KING_Depose,							// �������� : ��ְ
		MSG_S2W_COUNTRY_KING_Absolve,							// �������� : ����
		MSG_S2W_COUNTRY_KING_Silence,							// �������� : ����
		MSG_S2W_COUNTRY_KING_Exile,								// �������� : ����
		MSG_S2W_COUNTRY_KING_SuccessExiled,						// �������� : ���Ž��
		MSG_S2W_COUNTRY_KING_Convone,							// �������� : �ټ�
		MSG_S2W_COUNTRY_COUNTRY_Message,						// ������Ϣ����
		MSG_S2W_COUNTRY_WORLD_Message,							// ������Ϣ����
		MSG_S2W_COUNTRY_PRIVATE_Message,						// ������Ϣ����
		MSG_S2W_COUNTRY_SetNewDay,								// �����µ�һ��
		MSG_S2W_COUNTRY_ChangeAttribute,						// �����µ�һ��
		MSG_S2W_COUNTRY_ReleaseQuest,							// ������������


		// ��ս����
		MSG_S2W_COUNTRY_WAR_Declare_Quest,						// ĳ����������ս
		MSG_S2W_COUNTRY_WAR_FlagDestroy,						// ���ı�����

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool BEGIN
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MSG_S2W_GMA = MSG_S2W + MAX_SUB * 10,
		MSG_S2W_GMA_RESPONSE_KICKPLAYER, 
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool END
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////
		//				//
		//	GameServer	//
		//				//
		//////////////////

		//========================================================================================================
		MSG_S2S = MAX_TYPE * 7,			// GameServer --> GameServer
		//========================================================================================================

		MSG_S2S_SERVER					= MSG_S2S + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_S2S_SERVER_WORLDSERVERCLOSE,						// GameServer��⵽WorldServer�ر�
		MSG_S2S_SERVER_RECONNECTWORLDSERVER,					// ��������WorldServer
		MSG_S2S_SERVER_BILLINGSERVERCLOSE,						// GameServer��⵽BillingServer�ر�
		MSG_S2S_SERVER_RECONNECTBILLINGSERVER,					//��������BillingServer

		MSG_S2S_LOG						= MSG_S2S + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_S2S_LOG_PLAYER_ONLOST,								// �������ڽ��ܵ��ͻ��˶�����Ϣʱ���Լ���һ����Ϣ	

		MSG_S2S_TEAM					= MSG_S2S + MAX_SUB * 2,
		// ----------------------------------------------------
		//MSG_S2S_TEAM_TEAMMATE_MIGRATE_IN,						//ת��
		//MSG_S2S_TEAM_TEAMMATE_MIGRATE_OUT,						//ת��
		//MSG_S2S_TEAM_TEAMMATE_LOST,								//��Ա���

		// ---------------------------------------------------- ͳһ�Ʒ���Ϣ
		MSG_S2S_UNIBILL					= MSG_S2S + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_S2S_UNIBILL_AccountAuthenRes,						// �ʺ���Ϣ��ѯ
		MSG_S2S_UNIBILL_ConsignTransferRes,						// C2C���׳ɹ��۷�
		MSG_S2S_UNIBILL_AccountLockRes,							// C2C���׳ɹ�Ԥ��
		MSG_S2S_UNIBILL_AccountLockExRes,						// B2C���׳ɹ�Ԥ��
		MSG_S2S_UNIBILL_AccountUnlockRes,						// C2C���׳ɹ��ⶳ
		MSG_S2S_UNIBILL_AccountUnlockExRes,						// B2C���׳ɹ��ⶳ
		MSG_S2S_UNIBILL_AwardAuthenRes,							// �콱��ѯ
		MSG_S2S_UNIBILL_AwardAckRes,							// �콱ȷ��

		//========================================================================================================
		MSG_W2S = MAX_TYPE * 8,			// WorldServer --> GameServer
		//========================================================================================================

		MSG_W2S_SERVER					= MSG_W2S + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_W2S_SERVER_SENDSETUP,								// WorldServer����setup������Ϣ��GameServer
		MSG_W2S_SERVER_CHANGEREGION,							// �����л�������
		MSG_W2S_SERVER_SAVENOTIFY,								// ���̵�֪ͨ
		MSG_W2S_SERVER_ADDINFO,									// ����World Server��GAME SERVER���ؽ��
		MSG_W2S_SERVER_CHANGE_SCRIPT_VARIABLE,					// �ı�ű�������ֵ
		MSG_W2S_SERVER_SYNC_MURDERER_COUNT,						// ͬ��ɱ����
		MSG_W2S_UPDATE_STRINGTABLE,								// ��������GameServer�ϵ����԰���

		MSG_W2S_SERVER_QUEST_COLLECT_PLAYERDATA,				// ����GameServer��������ҵ����ݷ��͹���
		MSG_W2S_SERVER_QUEST_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����
		MSG_W2S_SERVER_CREATEMONSTER,							// ��������
		MSG_W2S_SERVER_Request_Info,							// ��������

		MSG_W2S_LOG						= MSG_W2S + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_W2S_LOG_ANSWER_PLAYERDATA,							// WorldServer��GameServer����player��ϸ����
		MSG_W2S_LOG_KICKPLAYER,									//worldserver��gameserver����һ��Ҫ���߳�ĳ����ҵ���Ϣ
		MSG_W2S_LOG_REPEAT_LOGIN,								//World Server֪ͨGame Server��һ���������½һ�ʺŲ��滻��ԭ��ʹ�ø��ʺŵ����ӡ�

		MSG_W2S_LOG_PLAYER_ONLINE,								// ��Һ�������
		MSG_W2S_LOG_PLAYER_OFFLINE,								// ��Һ�������

		MSG_W2S_OTHER					= MSG_W2S + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_W2S_OTHER_TALK,										// �Ի���Ϣ
		MSG_W2S_OTHER_FACTIONTALK,								// �������
		MSG_W2S_OTHER_BROADCAST,								// worldserver֪ͨgameserver���㲥��client
		MSG_W2S_OTHER_AddTopInfo,								// ��ͻ�����Ӷ�����ʾ��Ϣ
		MSG_W2S_OTHER_ADDINFO,
		MSG_W2S_OTHER_GOODSLINK,							
		MSG_W2S_OTHER_VIEWGOODSLINK,
		MSG_W2S_OTHER_ADDSKILL,
		MSG_W2S_OTHER_DELSKILL,
		MSG_W2S_OTHER_DELGOODS,
		MSG_W2S_OTHER_SETLEVEL,
		MSG_W2S_OTHER_KICKALL,									// ��WorldServerֱ�������������ˣ�����GM


		MSG_W2S_OTHER_CHANGENAME,								// Ws�������ͽ����Gs
		MSG_W2S_OTHER_TALK_WORLD,								// ���纰��
		MSG_W2S_OTHER_TALK_COUNTRY,								// ���纰��

		MSG_W2S_OTHER_TALK_NOTIFY,								// ����֪ͨ

		MSG_W2S_OTHER_GETLOG_RESPONSE,							// ���ظ�gs��ֵ��־

		MSG_W2S_OTHER_TIMETORETURN,								// ������������Ҷ�ʱ�س�

		MSG_W2S_DEPOT					= MSG_W2S + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_W2S_DEPOT_OPEN,

		MSG_W2S_GM						= MSG_W2S + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_W2S_GM_ANSWER_PLAYERCOUNT,							//�����û�����
		MSG_W2S_GM_ANSWER_PLAYERDATA,							//�����û�����
		MSG_W2S_GM_QUERY_PLAYERDATA,
		MSG_W2S_GM_ANSWER_FINDREGION,
		MSG_W2S_GM_ANSWER_PLAYERONLINE,							//�����û�����״��
		MSG_W2S_GM_KICKPLAYER,									// ���������
		MSG_W2S_GM_KICKPLAYEREX,								// ������ worldserver 2 gameserver
		MSG_W2S_GM_ANSWER_KICKPLAYER,							// Ӧ�����������
		MSG_W2S_GM_KICK_ALLPLAYER,								// �������������
		MSG_W2S_GM_KICK_REGIONPLAYER,							// ��һ�������е��������
		MSG_W2S_GM_SILENCE_PLAYER,								// ����һ����ҽ���
		MSG_W2S_GM_ANSWER_SILENCE_PLAYER,						// �𸴽�һ����ҽ���
		MSG_W2S_GM_WORLD_MESSAGE,								// �������������ҷ�����Ϣ
		MSG_W2S_GM_QUERY_SILENCE_PLAYERS,						// ����Ϸ��������ѯ���Ե����
		MSG_W2S_GM_ANSWER_QUERY_SILENCE_PLAYERS,				// ��������������ؽ��Ե�����б�
		MSG_W2S_GM_CHANGEREGION,
		MSG_W2S_GM_QUERYGM,
		MSG_W2S_GM_ANSWERGM,
		MSG_W2S_GM_COUNTRY_MESSAGE,								// ��ָ�����ҷ�����Ϣ

		MSG_W2S_TEAM					= MSG_W2S + MAX_SUB * 5,
		// ----------------------------------------------------
		MSG_W2S_TEAM_CREATE,
		MSG_W2S_TEAM_DESTORY,
		MSG_W2S_TEAM_TEAMATE_JOIN,
		MSG_W2S_TEAM_TEAMATE_LEAVE,
		MSG_W2S_TEAM_TEAMATE_CHANGE_REGION,
		MSG_W2S_TEAM_KICK_PLAYER,
		MSG_W2S_TEAM_CHANGE_LEAER,
		MSG_W2S_TEAM_SYNCHRONIZE,
		MSG_W2S_TEAM_QUERY_PLAYER,									//��ѯ����Ƿ����
		MSG_W2S_TEAM_CHANGE_ALLOCATION_SCHEME,						//�ı���䷽ʽ
		MSG_W2S_TEAM_CHAT,
		MSG_W2S_TEAM_CHANGE_STATE,

		//MSG_W2S_TEAM_ANSWER_TEAMID,								//���ض���ID
		//MSG_W2S_TEAM_TEAMMATE_JOIN,								//��Ա����
		//MSG_W2S_TEAM_TEAMMATE_QUIT,								//��Ա���
		//MSG_W2S_TEAM_OVER,										//ɢ��	
		//MSG_W2S_TEAM_TEAMMATE_KICK,								//����
		//MSG_W2S_TEAM_DOWNLOAD,									//�´�������Ϣ
		//MSG_W2S_TEAM_ANSWER_SIZE,								//���ض����С
		//MSG_W2S_TEAM_MISS_CAPTAIN_INFO,							//��ʧ�ӳ���Ϣ	
		//MSG_W2S_TEAM_CHANGE_CAPTAIN,							//�����ӳ�	
		//MSG_W2S_TEAM_SPREAD_CAPTAIN_CHANGE,						//...
		//MSG_W2S_TEAM_CHAT,
		//MSG_W2S_TEAM_CHANGEREGION,								//�����л�����

		MSG_W2S_ORGSYS					= MSG_W2S + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_W2S_ORGSYS_AnswerCREATEFACTION,
		MSG_W2S_ORGSYS_ADDFACTION,								//��Ӱ��
		MSG_W2S_ORGSYS_DELFACTION,								//ɾ�����
		MSG_W2S_ORGSYS_ADDUNION,								//���ͬ��
		MSG_W2S_ORGSYS_DELUNION,								//ɾ��ͬ��
		MSG_W2S_ORGSYS_UPDATEPLAYERFACTIONINFO,					//���°����Ϣ
		MSG_W2S_ORGSYS_ANSWERFACTIONLIST,						//���ذ���б�
		MSG_W2S_ORGSYS_AddAllFactions,							//�������еİ����ͻ���
		MSG_W2S_ORGSYS_ANSWERAPPLYJOINFACTION,
		MSG_W2S_ORGSYS_UpdateApplyMember,						//����������Ա
		MSG_W2S_ORGSYS_UpdateFactionPropert,					//���°������
		MSG_W2S_ORGSYS_UpdateFactionBasePropert,				//���»����������
		MSG_W2S_ORGSYS_UpdateMemberInfo,						//���³�Ա��Ϣ
		MSG_W2S_ORGSYS_UpdateUnionMemberInfo,					//����ͬ�˳�Ա��Ϣ
		MSG_W2S_ORGSYS_UpdateLeaveWord,							//����������Ϣ
		MSG_W2S_ORGSYS_UpdatePronounce,							//���¹�����Ϣ
		MSG_W2S_ORGSYS_UpdateEnemyFaction,						//���µж԰����Ϣ
		MSG_W2S_ORGSYS_UpdateCityWarEnemyFaction,				//���¹�������ս�ж԰����Ϣ
		MSG_W2S_ORGSYS_UpdateOwnedCity,							//����ӵ�е�������Ϣ
		MSG_W2S_ORGSYS_UpdateFactionExp,						//���°��ľ�����ʾ���ͻ���
		MSG_W2S_ORGSYS_UpdateFactionInfo,						//���°����Ϣ���ͻ���
		MSG_W2S_ORGSYS_RequestAffirmEstaUnion,					//����ȷ�Ͻ���ͬ��
		MSG_W2S_ORGSYS_RequestAffirmJoinUnion,					//����ȷ������ͬ��
		MSG_W2S_FACTIONWAR_ANSWERFACTIONLIST,					////���ذ�ս����б�
		MSG_W2S_ORGSYS_ANSWERDigUpTheHatchet,					//�ָ���ս���
		MSG_W2S_ORGSYS_RequestAffirmDigUpTheHatchet,			//����ȷ����ս
		MSG_W2S_ORGSYS_RequestAffirmMakePeace,					//����ȷ�����
		MSG_W2S_ORGSYS_RequestAffirmSurrender,					//����ȷ��Ͷ��
		MSG_W2S_ORGSYS_AnswerGetFactionBillboard,				//�õ�������а�
		MSG_W2S_ORGSYS_AnswerUpgradeFaction,					//�ָ������������
		//MSG_S2W_ORGSYS_AnswerUpgradeFaction,					//�ָ������������
		MSG_W2S_ATTACKCITY_Declar,								//����������ս��ս��ʼ
		MSG_W2S_ATTACKCITY_Start,								//����������ս��ʼ
		MSG_W2S_ATTACKCITY_TimeOut,								//����ʱ�䵽
		MSG_W2S_ATTACKCITY_End,									//����������ս����
		MSG_W2S_ATTACKCITY_Mass,								//����������ս���Ὺʼ
		MSG_W2S_ATTACKCITY_ClearOtherPlayer,					//�������������
		MSG_W2S_ATTACKCITY_RefreshRegion,						//����������ս���Ὺʼ
		MSG_W2S_ATTACKCITY_GetTodayTax,							//�ռ�����˰��
		MSG_W2S_ATTACKCITY_RefreshOwnedCityOrg,					//ˢ��ӵ�����ǵ���֯
		MSG_W2S_CITY_AnswerObtainTaxPayment,					//��ȡ˰��
		MSG_W2S_CITY_AnswerOAdjustTaxRate,						//����˰��
		MSG_W2S_CITY_OperatorCityGate,							//��������
		MSG_W2S_CITY_RequestAffirmTransfere,					//����ȷ��ת������
		MSG_W2S_CITY_RefreshState,								//ˢ������״̬
		MSG_W2S_CITY_AddTaxMoney,								//��ĳ���������˰��
		MSG_W2S_CITY_RefreshProxyRegionTax,						//��ĳ���������˰��

		MSG_W2S_VILLAGE_DeclareWar,								//��ս��ʼ
		MSG_W2S_VILLAGE_WarStart,								//��ս��ʼ
		MSG_W2S_VILLAGE_TimeOut,								//�ݵ�ʱ�䵽
		MSG_W2S_VILLAGE_WarEnd,									//���ڽ���
		MSG_W2S_AVILLAGE_ClearPlayer,							//�ݵ�ս����ݵ����
		MSG_W2S_ANSWERApplyForVillageWar,						//�ָ������μӾݵ�����ս
		MSG_W2S_CITYWAR_UpdateDecWarFacs,						//��������ս���İ���б�
		MSG_W2S_VIILWAR_UpdateDecWarFacs,						//��������ս���İ���б�
		MSG_W2S_CITYWAR_DeclareWar,								//��ս

		MSG_W2S_QUEST_PlayerAddQuest,							//��ĳ��������һ������
		MSG_W2S_QUEST_PlayerRemoveQuest,						//��ĳ������Ƴ�һ������
		MSG_W2S_QUEST_PlayerRunScript,							//��ĳ���������һ���ű�

		// ����
		MSG_W2S_COUNTRY					= MSG_W2S + MAX_SUB * 7,
		//---------------------------------------------------------------------------
		MSG_W2S_COUNTRY_CHANGE,									// ���Ĺ���
		MSG_W2S_COUNTRY_POWER_CHANGE,
		MSG_W2S_COUNTRY_TECHLEVEL_CHANGE,
		MSG_W2S_COUNTRY_CHANGE_CI,
		//////////////////////////////////////////////////////////////////
		// ����Ȩ�� W2S yulefox
		MSG_W2S_COUNTRY_PreSetKingID,							// Ԥ�����
		MSG_W2S_COUNTRY_KING_GETSTATE,							// �������� : ��ȡ��ǰ״̬���Ƿ��ǹ�������ս״̬��
		MSG_W2S_COUNTRY_KING_GetInfo,							// �������� : ��ȡ���й�����Ϣ
		MSG_W2S_COUNTRY_KING_GetPlayerList,						// �������� : ��ȡ��������б�
		MSG_W2S_COUNTRY_KING_Demise,							// �������� : ����
		MSG_W2S_COUNTRY_KING_Appoint,							// �������� : ����
		MSG_W2S_COUNTRY_KING_Depose,							// �������� : ��ְ
		MSG_W2S_COUNTRY_KING_Absolve,							// �������� : ����
		MSG_W2S_COUNTRY_KING_Silence,							// �������� : ����
		MSG_W2S_COUNTRY_KING_Exile,								// �������� : ����
		MSG_W2S_COUNTRY_KING_Convone,							// �������� : �ټ�
		MSG_W2S_COUNTRY_KING_ChangeControl,						// �����y����׃��
		MSG_W2S_COUNTRY_COUNTRY_Message,						// ������Ϣ����
		MSG_W2S_COUNTRY_WORLD_Message,							// ������Ϣ����
		MSG_W2S_COUNTRY_PRIVATE_Message,						// ������Ϣ����
		MSG_W2S_COUNTRY_ReleaseQuest,							// ������������

		// ��ս����
		MSG_W2S_COUNTRY_WAR_Declare_Response,					// �ش���ս
		MSG_W2S_COUNTRY_WAR_Declare_Begin,						//ս����ս��ʼ
		MSG_W2S_COUNTRY_WAR_Declare_End,						//ս����ս����
		
		MSG_W2S_COUNTRY_WAR_Prepare_Begin,						//ս����ս��ʼ
		MSG_W2S_COUNTRY_WAR_Prepare_End,						//ս����ս����

		MSG_W2S_COUNTRY_WAR_Start,								//ս����ʼ
		MSG_W2S_COUNTRY_WAR_TimeOut,							//ս��ʱ�䵽
		MSG_W2S_COUNTRY_WAR_End,								//ս������
		MSG_W2S_COUNTRY_WAR_Clear,								//�峡
		MSG_W2S_COUNTRY_WAR_UpdateCountrys,

		MSG_W2S_COUNTRY_WAR_FlagDestory,						//���ı���

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool BEGIN
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MSG_W2S_GMA = MSG_W2S + MAX_SUB * 8,
		MSG_W2S_GMA_KICKPLAYER,
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// GMA tool END
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//========================================================================================================
		MSG_C2S = MAX_TYPE * 9,			// Client --> GameServer
		//========================================================================================================

		MSG_C2S_LOG						= MSG_C2S + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_C2S_LOG_PLAYEREXITGAME,								// ������GameServer
		MSG_C2S_LOG_PLAYERENTER,								// ���������Ϸ������

		MSG_C2S_RGN						= MSG_C2S + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_C2S_RGN_ENTER,										// ����������������볡��
		MSG_C2S_RGN_EXIT,										// ��������뿪����

		MSG_C2S_SHAPE					= MSG_C2S + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_C2S_SHAPE_CHANGEDIR,								// �ı䷽��
		MSG_C2S_SHAPE_SETPOS,									// ˲��
		MSG_C2S_SHAPE_MOVESTEP,									// �ƶ�һ��
		MSG_C2S_SHAPE_QUESTINFO,								// ���������������shape����Ϣ
		MSG_C2S_SHAPE_EMOTION,									// ���鶯��

		MSG_C2S_PLAYER					= MSG_C2S + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_C2S_PLAYER_ADDPOINT,								// �ӵ�
		MSG_C2S_PLAYER_QUEST_RELIVE,							// ���󸴻�
		MSG_C2S_PLAYER_TOUCHNPC,								// ����NPC
		MSG_C2S_PLAYER_USEITEM,									// client��gameserver��֤һ�����ĵ��ߵ�ʹ��
		MSG_C2S_PLAYER_PKSWITCH,								// �л�PK����
		MSG_C2S_PLAYER_TRADEREQUEST,							//��������
		MSG_C2S_PLAYER_TRADERESPONSE,							//�ͻ���ͬ���ܾ���������
		MSG_C2S_PLAYER_PUTGOODSTOTRADEPAGE,						//�ͻ��˷�һ����Ʒ������չʾ��
		MSG_C2S_PLAYER_FETCHGOODSFROMTRADEPAGE,					//��Ҵ�չʾ���ó�һ����Ʒ
		MSG_C2S_PLAYER_TRADECHANGEMONEY,						//�ͻ������������Ǯ
		MSG_C2S_PLAYER_WAITFORTRADE,							//�ͻ���֪ͨ���뽻�׵ȴ�״̬
		MSG_C2S_PLAYER_TRADECANCEL,								//�ͻ�������ȡ������
		MSG_C2S_PLAYER_ADDFRIENDREQUEST,						//�ͻ��˷�����Ӻ��ѵ�����GameServer
		MSG_C2S_PLAYER_ADDFRIENDRESPONSE,						//��������Ҹ�GameServer��ͬ���ܾ�����Ϣ��Ӧ
		MSG_C2S_PLAYER_DELFRIEND,								//client��gameserver����ɾ��һ�����ѵ�����
		MSG_C2S_PLAYER_HELPREQUEST,								// ����������
		MSG_C2S_PLAYER_UpdateDisplayHeadPiece,					//�����Ƿ���ʾͷ����Ϣ
		MSG_C2S_PLAYER_CHANGE_QUEST_TIMEREQUEST,


		MSG_C2S_OTHER					= MSG_C2S + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_C2S_OTHER_TALK,										// �Ի���Ϣ
		MSG_C2S_OTHER_SENDTALKBOXSELECT,						// �ͻ��˷���TalkBox��ѡ��
		MSG_C2S_OTHER_VIEWGOODSLINK,							// �ͻ�������鿴һ����Ʒ������
		MSG_C2S_OTHER_CONNECTGAMESERVER,						// ����GameServer�Ľ��
		MSG_C2S_OTHER_CHANGENAME,								// ��������������
		MSG_C2S_OTHER_QUESTNPCLIST,								// ����NPC�б�

		MSG_C2S_OTHER_TALK_WORLD,								// ���纰��
		MSG_C2S_OTHER_TALK_COUNTRY,								// ���纰��

		//MSG_C2S_OTHER_TALK_NOTIFY,							// ����֪ͨ��������֪ͨ��

		MSG_C2S_OTHER_REQUEST_LOANTIMELIMIT,					// ���󻹿�����

		MSG_C2S_OTHER_QUESTGOD = MSG_C2S_OTHER + MAX_SUB - 7,	// ����������

		MSG_C2S_GOODS					= MSG_C2S + MAX_SUB * 5,
		// ----------------------------------------------------
		MSG_C2S_FETCHGOOD,										//playerȡһ����Ʒ
		MSG_C2S_PUTDOWNGOOD,									//player����һ����Ʒ
		MSG_C2S_CHANGEGOOD,										//player������Ʒλ��
		MSG_C2S_DROPGOOD,										//player����������Ʒ
		MSG_C2S_PICKGOOD,										//player�������Ʒ
		MSG_C2S_DROPMONEY,							
		MSG_C2S_PICKMONEY,							
		MSG_C2S_SETHOTKEY,										//player��������HOTKEY
		MSG_C2S_CLEARHOTKEY,									//ȡ���ȼ�����
		MSG_C2S_EXCHANGEHOTKEY,									//�滻�ȼ�����
		MSG_C2S_VIEWEQUIP,										//�鿴���װ��

		MSG_C2S_GOODSUPGRADE_PUT,								//����һ����Ʒ����������
		MSG_C2S_GOODSUPGRADE_FETCH,								//������������ȡ��һ����Ʒ
		MSG_C2S_GOODSUPGRADE_CHANGE,							//�����������н���һ����Ʒ
		MSG_C2S_GOODSUPGRADE_UPGRADE,                           //client��������һ����Ʒ
		MSG_C2S_GOODSUPGRADE_CLOSE,								// �ر���������(����session)
		MSG_C2S_GOODS_CONTAINER,								// ��Ʒ��������
		MSG_C2S_GOODS_PRECIOUBOX_ITEM,							// ��ȡ�ٱ������

		// gongran fruitmachine 2008-3-3
		//MSG_C2S_FRUIT_MACHINE_START,							// ��������-��ʼ
		//MSG_C2S_FRUIT_MACHINE_RECEIVE,							// ��������-�콱
		//MSG_C2S_FRUIT_MACHINE_CLOSE,							// ��������-�ر�

		MSG_C2S_SHOP					= MSG_C2S + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_C2S_SHOP_BUY,										//player��������
		MSG_C2S_SHOP_SELL,										//player��һ����Ʒ���̵�
		MSG_C2S_SHOP_REPAIR,									//���̵�������һ����Ʒ
		MSG_C2S_SHOP_REPAIRALL, 								//�̵�������ȫ����Ʒ
		MSG_C2S_SHOP_DURUPDATE,									//�;ø���
		MSG_C2S_SHOP_CLOSE,										//�رո����̵�

		MSG_C2S_DEPOT					= MSG_C2S + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_C2S_DEPOT_PUTDOWN,
		MSG_C2S_DEPOT_FETCH,
		MSG_C2S_DEPOT_CHANGE,
		MSG_C2S_DEPOT_SAVEMONEY,
		MSG_C2S_DEPOT_FETCHMONEY,
		MSG_C2S_DEPOT_CHECKPASSWORD,
		MSG_C2S_DEPOT_CHANGEPASSWORD,
		MSG_C2S_DEPOT_CLOSE,

		MSG_C2S_TEAM					= MSG_C2S + MAX_SUB * 8,
		// ----------------------------------------------------
		MSG_C2S_TEAM_QUERY_JOIN,									//�������
		MSG_C2S_TEAM_ANSWER_JOIN,									//�ظ��������
		MSG_C2S_TEAM_TEAMATE_JOIN,
		MSG_C2S_TEAM_TEAMATE_LEAVE,
		MSG_C2S_TEAM_CHANGE_LEAER,									//�ı�ӳ�
		MSG_C2S_TEAM_KICK_PLAYER,									//����
		MSG_C2S_TEAM_DESTORY,
		MSG_C2S_TEAM_RECRUIT,										//��ļ����
		MSG_C2S_TEAM_CHANGE_ALLOCATION_SCHEME,
		MSG_C2S_TEAM_CHAT,
		MSG_C2S_TEAM_QUERY_JOIN_BYNAME,									//ͨ����������������
		//MSG_C2S_TEAM_TEAMMATE_QUIT,						        //��Ա���
		//MSG_C2S_TEAM_TEAMMATE_KICK,								//����
		//MSG_C2S_TEAM_UPLOAD,									//�ϴ�������Ϣ	
		//MSG_C2S_TEAM_ANSWER_CAPTAIN_INFO,						//���ضӳ���Ϣ
		//MSG_C2S_TEAM_CHAT,										//����

		MSG_C2S_SKILL					= MSG_C2S + MAX_SUB * 9,
		// ----------------------------------------------------
		MSG_C2S_SKILL_USE,										// ʹ�ü���
		MSG_C2S_SKILL_USE_END,									// ʹ�ü��ܽ�������Щ�����ɿ���ݼ���ʱ����
		MSG_C2S_SKILL_STUDY,									// ѧϰ����

		MSG_C2S_ORGSYS					= MSG_C2S + MAX_SUB * 10,
		// ----------------------------------------------------
		MSG_C2S_ORGSYS_CREATEFACTION_AnswerName,				//��������İ������
		MSG_C2S_ORGSYS_GetOrganizing,							//�õ���ᡢͬ��
		MSG_C2S_ORGSYS_GetFaction,								//�õ����
		MSG_C2S_ORGSYS_GetUnion,								//�õ����
		MSG_C2S_ORGSYS_GETFACTIONLIST,
		MSG_C2S_ORGSYS_APPLYJOINFACTION,						//���������
		MSG_C2S_ORGSYS_DOJOINFACTION,							//�����ᴦ��
		MSG_C2S_ORGSYS_FireOutFactionMember,					//������Ա
		MSG_C2S_ORGSYS_FireOutUnionMember,						//������Ա
		MSG_C2S_ORGSYS_ExitFaction,								//�˳����
		MSG_C2S_ORGSYS_ExitUnion,								//�˳�ͬ��
		MSG_C2S_ORGSYS_Demise,									//������λ 
		MSG_C2S_ORGSYS_UnionDemise,								//������λ 
		MSG_C2S_ORGSYS_Disband,									//��ɢ��� 
		MSG_C2S_ORGSYS_DisbandUnion,							//��ɢͬ�� 
		MSG_C2S_ORGSYS_DubJobLvl,								//����ƺźͰ��ְ��
		MSG_C2S_ORGSYS_EndueRight,								//����Ȩ��
		MSG_C2S_ORGSYS_AbolishRight,							//ȡ��Ȩ��
		MSG_C2S_ORGSYS_RequestEstaUnion,						//������ͬ��
		MSG_C2S_ORGSYS_AnswerAffirmEstaUnion,					//�ظ�ȷ�Ͻ���ͬ��
		MSG_C2S_ORGSYS_InviteJoinUnion,							//�������ͬ��
		MSG_C2S_ORGSYS_AnswerAffirmJoinUnion,					//�ظ�ȷ������ͬ��
		MSG_C2S_ORGSYS_LeaveWord,								//����
		MSG_C2S_ORGSYS_DelLeaveWord,							//ɾ������
		MSG_C2S_ORGSYS_EditPronounce,							//�༭����
		MSG_C2S_FACTIONWAR_GETFACTIONLIST,
		MSG_C2S_ORGSYS_DigUpTheHatchet,							//��ս
		MSG_C2S_ORGSYS_AnswerAffirmDigUpTheHatchet,				//�ظ�ȷ����ս
		MSG_C2S_ORGSYS_MakePeace,								//���
		MSG_C2S_ORGSYS_AnswerAffirmMakePeace,					//�ظ�ȷ�����
		MSG_C2S_ORGSYS_Surrender,								//Ͷ��
		MSG_C2S_ORGSYS_AnswerAffirmSurrender,					//�ظ�ȷ��Ͷ��
		MSG_C2S_ORGSYS_SetFactionControbute,					//���ð��Ĺ�����
		MSG_C2S_CITY_AnswerInputTaxPayment,						//��ȡ˰��
		MSG_C2S_CITY_AnswerInputTaxRate,						//����˰��
		MSG_C2S_CITY_Transfer,									//ת������
		MSG_C2S_CITY_AnswerAffirmTransfere,						//�ظ�ȷ��ת������
		MSG_C2S_CITY_SetPermit,									//�����Ƿ�����ͬ�ˡ���ս

		MSG_C2S_QUEST_Complete,									//�ύ�������
		MSG_C2S_QUEST_Disband,									//�ύ��������

		MSG_C2S_QUEST_PlayerRanks,								//������ҵ����а�

		MSG_C2S_PLAYERSHOP				= MSG_C2S + MAX_SUB * 11,
		// ----------------------------------------------------
		MSG_C2S_PLAYERSHOP_CREATE,								//���󴴽������̵�
		MSG_C2S_PLAYERSHOP_GOODSPRICE,
		MSG_C2S_PLAYERSHOP_BUYITEM,								//������Ʒ
		MSG_C2S_PLAYERSHOP_OPENFORBUSINESS,						//����
		MSG_C2S_PLAYERSHOP_CLOSEDOWN,							//����
		MSG_C2S_PLAYERSHOP_LOOKGOODS,							//�쿴��Ʒ
		MSG_C2S_PLAYERSHOP_CLOSE,								//�رո����̵�
		MSG_C2S_PLAYERSHOP_QUIT,								//�������뿪�����̵�
		//MSG_C2S_PLAYERSHOP_FETCHGOODS,
		//MSG_C2S_PLAYERSHOP_PUTGOODS,
		//MSG_C2S_PLAYERSHOP_EXCHANGEITEM,

		// ����
		MSG_C2S_CONTAINER				= MSG_C2S + MAX_SUB * 12,
		// ----------------------------------------------------
		MSG_C2S_CONTAINER_OBJECT_MOVE,

		// ����
		MSG_C2S_PET						= MSG_C2S + MAX_SUB * 13,
		// ----------------------------------------------------
		MSG_C2S_PET_CHANGE_MODE,								//�ı���ﵱǰģʽ
		MSG_C2S_PET_CHANGE_ACTION,								//�ı���ﵱǰ����

		// ����
		MSG_C2S_COUNTRY					= MSG_C2S + MAX_SUB * 14,
		// -----------------------------------------------------
		//////////////////////////////////////////////////////////////////
		// ����Ȩ�� C2S yulefox
		MSG_C2S_COUNTRY_KING_GETSTATE,							// �������� : ��ȡ��ǰ״̬���Ƿ��ǹ�������ս״̬��
		MSG_C2S_COUNTRY_KING_GetInfo,							// �������� : ��ȡ���й�����Ϣ
		MSG_C2S_COUNTRY_KING_GetPlayerList,						// �������� : ��ȡ��������б�
		MSG_C2S_COUNTRY_KING_Demise,							// �������� : ����
		MSG_C2S_COUNTRY_KING_Appoint,							// �������� : ����
		MSG_C2S_COUNTRY_KING_Depose,							// �������� : ��ְ
		MSG_C2S_COUNTRY_KING_Absolve,							// �������� : ����
		MSG_C2S_COUNTRY_KING_Silence,							// �������� : ����
		MSG_C2S_COUNTRY_KING_Exile,								// �������� : ����
		MSG_C2S_COUNTRY_KING_Convone,							// �������� : �ټ�

		MSG_C2S_COUNTRY_WAR_EntryRegionBySummon,				// ���ٻ������ս��ͼ
		// ��ֵ�̵�
		MSG_C2S_INCREMENTSHOP					= MSG_C2S + MAX_SUB * 15,
		// ----------------------------------------------------
		MSG_C2S_INCREMENTSHOP_OPEN,
		MSG_C2S_INCREMENTSHOP_LIST,										//player�����̳��б�
		MSG_C2S_INCREMENTSHOP_BUY,										//player��������
		MSG_C2S_INCREMENTSHOP_CLOSE,									//�رո����̵�

		MSG_C2S_INCREMENTSHOP_GETLOG_REQUEST,							// ������ֵ��־

		//MSG_C2S_GETLOG_RESPONSE,

		//////////////////
		//				//
		//	GameClient	//
		//				//
		//////////////////

		//========================================================================================================
		MSG_C2C = MAX_TYPE * 10,			// Client --> Client
		//========================================================================================================

		MSG_C2C_LOG						= MSG_C2C + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_C2C_LOG_SERVERLOST,									// CLIENT�ڽ��ܵ�Server������Ϣʱ���Լ���һ����Ϣ

		//========================================================================================================
		MSG_L2C = MAX_TYPE * 11,		// LoginServer --> Client
		//========================================================================================================

		MSG_L2C_LOG						= MSG_L2C + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_L2C_LOG_ANSWERCDKEY,								// ����������CDKEY
		MSG_L2C_LOG_ANSWER_PLAYERBASELIST,						// ������������һ��������б�
		MSG_L2C_LOG_ANSWER_PLAYERDATA,							// ���������ϸ����
		MSG_L2C_LOG_ANSWERCREATE,								// ���������ؽ�����ɫ����Ϣ
		MSG_L2C_LOG_ANSWERDELETE,								// �����������Ƿ�����ɾ����Ϣ
		MSG_L2C_LOG_ANSWERRESTORE,								// �Ƿ�ɹ��ظ���ɫ
		MSG_L2C_LOG_UPDATEQUEUENUM,								// ���µ�½������е�ʣ������

		MSG_L2C_LOG_SENDACCOUNTINFO,							// �����ʺŵ���Ϣ���ͻ���
		MSG_L2C_LOG_UPDATEWORLDSERVERINFO,						// ����WorldServer��״̬��Ϣ

		//========================================================================================================
		MSG_S2C = MAX_TYPE * 12,		// GameServer --> Client
		//========================================================================================================

		MSG_S2C_LOG						= MSG_S2C + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_S2C_LOG_ANSWER_PLAYERDATA,							// GameServer��GameClient����player��ϸ����
		MSG_S2C_LOG_GAMESERVERTIME,								// ����Server��ʱ��
		MSG_S2C_LOG_SEQUENCESTRING,								// ��֤��Ϣ�Ϸ��Ե��ַ�������

		MSG_S2C_LOG_PLAYER_ONLINE,								// ��Һ�������
		MSG_S2C_LOG_PLAYER_OFFLINE,								// ��Һ�������

		MSG_S2C_RGN						= MSG_S2C + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_S2C_RGN_ENTER,										// ����������������볡��
		MSG_S2C_RGN_ADDSHAPE,									// �ж�����볡��
		MSG_S2C_RGN_ADDSUMMONEDSHAPE,							// �ٻ���Shape���볡��
		MSG_S2C_RGN_DELSHAPE,									// �ж����뿪����
		MSG_S2C_RGN_CHANGE,										// ��������������л�����
		MSG_S2C_RGN_CHANGESERVER,								// ��������������л�����������
		MSG_S2C_RGN_CHANGEWEATHER,								// �ı�����
		MSG_S2C_RGN_PLAYACTION,									// ���Ŷ���
		MSG_S2C_RGN_PLAYSOUND,									// ��������
		MSG_S2C_RGN_PLAYEFFECT,									// ������Ч

		MSG_S2C_SHAPE					= MSG_S2C + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_S2C_SHAPE_CHANGEDIR,								// �ı䷽��
		MSG_S2C_SHAPE_CHECKPOS,									// ����Ч��
		MSG_S2C_SHAPE_SETPOS,									// ˲��
		MSG_S2C_SHAPE_FORCEMOVE,								// ǿ���ƶ�
		MSG_S2C_SHAPE_MOVESTEP,									// �ظ��ƶ�һ��
		MSG_S2C_SHAPE_MOVETILE,									// ������������Ϣ�ƶ�һ��
		MSG_S2C_SHAPE_CHANGESTATE,								// �ı�״̬
		MSG_S2C_SHAPE_ATK_LOCK,									// ָʾ����Ŀ��
		MSG_S2C_SHAPE_ATK_BEGIN,								// ������֪ͨ�ͻ��˿�ʼ����
		MSG_S2C_SHAPE_ATK_BREAK,								// ������ͣ��
		MSG_S2C_SHAPE_ATK_DIED,									// ����
		MSG_S2C_SHAPE_ATK_REJECT,								// ����ָ��ܾ�
		MSG_S2C_SHAPE_FIGHTSTATE,								// ֪ͨclient������˳�ս��״̬
		MSG_S2C_SHAPE_SINSTATE,									// ֪ͨclient������˳�����״̬
		MSG_S2C_SHAPE_Refresh,									// ˢ�½���״̬ 
		MSG_S2C_SHAPE_STIFFEN,									// ��ֱ״̬
		MSG_S2C_SHAPE_EMOTION,									// ��ֱ״̬
		MSG_S2C_SHAPE_ATK_FULLMISS,								// ����
		MSG_S2C_SHAPE_RELIVE,									// ����һ�����

		MSG_S2C_PLAYER					= MSG_S2C + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_S2C_PLAYER_RESUME,									// �ָ�HP��MP��YP������
		MSG_S2C_PLAYER_ADDPOINT,								// �ӵ�
		MSG_S2C_PLAYER_ANSWER_RELIVE,							// �ָ�����
		MSG_S2C_PLAYER_ADDEXP,									// ���Ӿ���ֵ
		MSG_S2C_PLAYER_LEVELUP,									// ����
		MSG_S2C_PLAYER_HITLEVELUPGRADE,							// ����������
		MSG_S2C_PLAYER_SYNC_HITS,								// ͬ��������
		MSG_S2C_PLAYER_SETLEVEL,
		MSG_S2C_PLAYER_USEITEM,									// gameserver��ͻ��˷������ĵ��ߵ�ʹ�ý��
		MSG_S2C_PLAYER_CHANGEMAXATK,							// �����Ե���������󹥻���
		MSG_S2C_PLAYER_CHANGEATKSPEED,							// �����Ե������������ٶ�
		MSG_S2C_PLAYER_CHANGEDEF,								// �����Ե���������������
		MSG_S2C_PLAYER_CHANGEELEMENTMODIFY,						// �����Ե��߸ı似�ܹ���ϵ��
		MSG_S2C_PLAYER_SYNC_PKCOUNT,							// ͬ��ɱ����
		MSG_S2C_PLAYER_TRADEREQUEST,							//GSת������������ͻ�
		MSG_S2C_PLAYER_TRADEBEGIN,								//���׿�ʼ
		MSG_S2C_PLAYER_PUTGOODSTOTRADEPAGE,						//���������ط�һ����Ʒ��չʾ���Ľ��
		MSG_S2C_PLAYER_GETGOODSFROMTRADEPAGE,					//��������һ����Ʒ��Ϣ���Է����
		MSG_S2C_PLAYER_FETCHGOODSFORMTRADEPAGE,					//���������ش�չʾ����һ����Ʒ����ؽ��
		MSG_S2C_PLAYER_DELGOODSFROMTRADEPAGE,					//������֪ͨ�Է�һ����Ʒ�ӽ�������ɾ��
		MSG_S2C_PLAYER_TRADECHANGEMONEY,						//���������ص�����Ǯ����ؽ��
		MSG_S2C_PLAYER_WAITFORTRADE,				
		MSG_S2C_PLAYER_TRADECANCEL,								//������֪ͨ��ҶԷ��Ѿ�ȡ������
		MSG_S2C_PLAYER_TRADERESULT,								//���׵����ս��
		MSG_S2C_PLAYER_ADDFRIENDREQUEST,						//GameServerת�����������Ϣ�����������
		MSG_S2C_PLAYER_ADDFRIENDRESPONSE,						//GameServerת����������ҵ���Ӧ���������������Ǹ����
		MSG_S2C_PLAYER_DELFRIEND,								//gameserver��client����ɾ��һ�����ѵĽ��
		MSG_S2C_PLAYER_HELPRESPONSE,							// ��Ӧ��Ұ���
		MSG_S2C_PLAYER_ADDSKILL,								// ��Ӽ���
		MSG_S2C_PLAYER_DELSKILL,								// ɾ������
		MSG_S2C_PLAYER_SETATTACKRANGE,				
		MSG_S2C_PLAYER_CHANGE_EQUIPMENT,
		MSG_S2C_PLAYER_CHANGE_PROPERTIES,						//��Ҹı�����
		MSG_S2C_PLAYER_UpdateDisplayHeadPiece,					//��������Ƿ���ʾͷ����Ϣ
		MSG_S2C_PLAYER_AUTOMOVE,								//�Զ��ƶ���ָ���ĵ�
		MSG_S2C_PLAYER_CHANGE_CONTRIBUTE,						// ���ҹ��׸ı�
		MSG_S2C_PLAYER_CHANGE_LOAN,
		MSG_S2C_PLAYER_CHANGE_LOANMAX,
		MSG_S2C_PLAYER_CHANGE_LOANTIME,
		MSG_S2C_PLAYER_CHANGE_QUEST_ON,
		MSG_S2C_PLAYER_CHANGE_QUEST_TIMEBEGIN,
		MSG_S2C_PLAYER_CHANGE_QUEST_TIMECLEAR,
		MSG_S2C_PLAYER_CHANGE_QUEST_TIMERESPONSE,
		MSG_S2C_PLAYER_CHANGE_ENERGY,							// ����ֵ�ĸı�
		MSG_S2C_PLAYER_CHANGE_MAXENERGY,						// ���þ���ֵ����
		MSG_S2C_PLAYER_CHANGE_Exploit,							// �ı� ����
		MSG_S2C_PLAYER_CHANGE_Kudos,							// �ı� ����
		MSG_S2C_PLAYER_UpdateOnlineTime,						// ��������ʱ��

		MSG_S2C_OTHER					= MSG_S2C + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_S2C_OTHER_TALK,										// �Ի���Ϣ
		MSG_S2C_OTHER_FACTIONTALK,								// �������
		MSG_S2C_OTHER_BROADCAST,								// �㲥
		MSG_S2C_OTHER_AddTopInfo,								// ��Ӷ�����ʾ��Ϣ
		MSG_S2C_OTHER_OPENTALKBOX,								// �ÿͻ��˵���TalkBox
		MSG_S2C_OTHER_ADDINFO,									// ����ҶԻ������һ����Ϣ
		MSG_S2C_OTHER_ADDSYSTEMINFO,							// ����Ļ�Ϸ�����һ����Ϣ
		MSG_S2C_OTHER_GAMEMESSAGE,								// ������Ϣ�Ի���
		MSG_S2C_OTHER_PING,										// ��������ʱ���Client�Ƿ�����ߣ�client����ͬ����Ϣ
		MSG_S2C_OTHER_SENDOBJECTPROPERTIY,						// ����Shape����������
		MSG_S2C_OTHER_CHANGESHAPEPROPERTIY,						// �ı�Shape��һ������
		MSG_S2C_OTHER_SETSHAPEPROPERTIY,						// ����Shape��һ������
		MSG_S2C_OTHER_VIEWGOODSLINK,			
		MSG_S2C_OTHER_SCRIPTTIMERRUN,							// �ű���ʱ������
		MSG_S2C_OTHER_CHANGENAME,								// ����Gsת��Ws�������
		MSG_S2C_OTHER_CHANGENAME_OPENUI,						// ������������
		MSG_S2C_OTHER_ADDPLAYERINFO,
		MSG_S2C_OTHER_SHOWREADMEWINDOW,
		MSG_S2C_OTHER_ANSWERNPCLIST,							// ����NPC�б���ͻ���

		MSG_S2C_OTHER_TALK_WORLD,								// ���纰��
		MSG_S2C_OTHER_TALK_COUNTRY,								// ���Һ���

		MSG_S2C_OTHER_TALK_NOTIFY,								// ����֪ͨ

		MSG_S2C_OTHER_RESPONSE_LOANTIMELIMIT,					// ���ػ�������
		
		

		MSG_S2C_GOODS					= MSG_S2C + MAX_SUB * 5,
		// ----------------------------------------------------
		MSG_S2C_FETCHGOOD,										//GAMESERVER����ȡһ����Ʒ�Ľ��
		MSG_S2C_PUTDOWNGOOD,									//gameserver���ط���һ����Ʒ�Ľ��
		MSG_S2C_CHANGEGOOD,										//gameserver������Ʒ�������
		MSG_S2C_DROPGOOD,										//gameserver��������Ʒ���
		MSG_S2C_PICKGOOD,										//gameserver���ؼ���Ʒ���
		MSG_S2C_DROPMONEY,							
		MSG_S2C_PICKMONEY,							
		MSG_S2C_SETHOTKEY,										//gameserver��������HOTKEY���
		MSG_S2C_CLEARHOTKEY,						
		MSG_S2C_EXCHANGEHOTKEY,						
		MSG_S2C_EQUIPDAMAGE,                                    //װ������
		MSG_S2C_EQUIPDURUPDATE,									//����װ�����;�
		MSG_S2C_DELETEEQUIP,									//ɾ��װ��
		MSG_S2C_DELETEBAGGOODS,									//ɾ��������Ʒ
		MSG_S2C_DELETEGOODS,									
		MSG_S2C_DELETEALLGOODS,						
		MSG_S2C_VIEWEQUIP,                                      
		MSG_S2C_OPENGOODSUPGRADE,								//gameserver֪ͨ��Ҵ���Ʒ��������
		MSG_S2C_CLOSEGOODSUPGRADE,
		MSG_S2C_GOODSUPGRADE_PUT,					
		MSG_S2C_GOODSUPGRADE_FETCH,					
		MSG_S2C_GOODSUPGRADE_CHANGE,				
		MSG_S2C_GOODSUPGRADE_UPGRADE,							//gameserver��������һ����Ʒ�Ľ��
		MSG_S2C_GOODS_UPDATEPROPERTY,							//������Ʒ����
		MSG_S2C_GOODS_OPEN_CONTAINER,							// ����Ʒ����

		MSG_S2C_GOODS_PRECIOUSBOX_OPEN,							// �򿪰ٱ���
		MSG_S2C_GOODS_PRECIOUSBOX_ITEM,							// �ٱ����ȡ������Ϣ
		MSG_S2C_GOODS_PRECIOUSBOX_CLOSE,

		// gongran fruitmachine 2008-3-3
		//MSG_S2C_FRUIT_MACHINE_OPEN,								// ��������-�򿪽���
		//MSG_S2C_FRUIT_MACHINE_RESULT,							// ��������-֪ͨ�ͻ�����Ϸ���


		MSG_S2C_SHOP					= MSG_S2C + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_S2C_SHOP_OPEN,										//���̵�			
		MSG_S2C_SHOP_BUY,									   //gameserver���ش��̵��й���һ����Ʒ�Ľ��
		MSG_S2C_SHOP_SELL,							
		MSG_S2C_SHOP_REPAIR,						
		MSG_S2C_SHOP_REPAIRALL,						
		MSG_S2C_SHOP_DURUPDATE,						
		MSG_S2C_SHOP_CLOSE,		//##�رո����̵�	

		MSG_S2C_DEPOT					= MSG_S2C + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_S2C_DEPOT_OPEN,							
		MSG_S2C_DEPOT_PUTDOWN,						
		MSG_S2C_DEPOT_FETCH,						
		MSG_S2C_DEPOT_CHANGE,						
		MSG_S2C_DEPOT_SAVEMONEY,					
		MSG_S2C_DEPOT_FETCHMONEY,					
		MSG_S2C_DEPOT_CHECKPASSWORD,				
		MSG_S2C_DEPOT_CHANGEPASSWORD,				

		MSG_S2C_GM						= MSG_S2C + MAX_SUB * 8,
		// ----------------------------------------------------
		MSG_S2C_GM_ANSWER_GMMODE,								// ��Ӧ�ͻ�������GMģʽ����Ϣ

		MSG_S2C_TEAM					= MSG_S2C + MAX_SUB * 9,
		// ----------------------------------------------------
		MSG_S2C_TEAM_QUERY_JOIN,								//�������
		MSG_S2C_TEAM_ANSWER_JOIN,								//�ظ��������
		MSG_S2C_TEAM_SYNCHRONIZE,								//ͬ���������Ϣ
		MSG_S2C_TEAM_TEAMATE_JOIN,
		MSG_S2C_TEAM_TEAMATE_LEAVE,
		MSG_S2C_TEAM_TEAMATE_CHANGE_REGION,
		MSG_S2C_TEAM_CHANGE_LEAER,									//�ı�ӳ�
		MSG_S2C_TEAM_CHANGE_ALLOCATION_SCHEME,
		MSG_S2C_TEAM_CHAT,
		MSG_S2C_TEAM_CHANGE_STATE,
		//MSG_S2C_TEAM_TEAMMATE_JOIN,								//��Ա����
		//MSG_S2C_TEAM_TEAMMATE_QUIT,								//��Ա���
		//MSG_S2C_TEAM_OVER,										//ɢ��	
		//MSG_S2C_TEAM_TEAMMATE_KICK,								//����
		//MSG_S2C_TEAM_DOWNLOAD,									//�´�������Ϣ	
		//MSG_S2C_TEAM_QUERY_CAPTAIN_INFO,						//ѯ�ʶӳ���Ϣ
		//MSG_S2C_TEAM_CHANGE_CAPTAIN,							//�����ӳ�	
		//MSG_S2C_TEAM_SPREAD_CAPTAIN_CHANGE,						//...
		//MSG_S2C_TEAM_CHAT,
		//MSG_S2C_TEAM_BID,										//�޶�����Ա���ܶ���

		MSG_S2C_SKILL					= MSG_S2C + MAX_SUB * 10,
		// ----------------------------------------------------
		MSG_S2C_SKILL_USE,										// ʹ�ü���
		MSG_S2C_SKILL_STATE_CHANGE,								// ʹ�ü��ܺ���HP/MP/RP/YP״̬�ı�
		MSG_S2C_SKILL_ADD_EX_STATE,								// ʹ�ü��ܺ�����쳣״̬
		MSG_S2C_SKILL_END_EX_STATE,								// �쳣״̬����ʧ,������֪ͨ�ͻ���
		MSG_S2C_SKILL_EX_STATE_ADDITIONAL_DATA,					// �쳣״̬�ĸ�������
		MSG_S2C_SKILL_AFFECT_FIELD,								// ������Ӱ���������ͻ���

		MSG_S2C_ORGSYS					= MSG_S2C + MAX_SUB * 11,
		// ----------------------------------------------------
		MSG_S2C_ORGSYS_CREATEFACTION_RequiestName,				//���������Ϣ,Ҫ��ͻ�������������
		MSG_S2C_ORGSYS_ADDFACTION,								//��Ӱ��
		MSG_S2C_ORGSYS_DELFACTION,								//ɾ�����
		MSG_S2C_ORGSYS_ADDUNION,								//���ͬ��
		MSG_S2C_ORGSYS_DELUNION,								//ɾ��ͬ��
		MSG_S2C_ORGSYS_UPDATEPLAYERFACTIONINFO,					//���°����Ϣ
		MSG_S2C_ORGSYS_ANSWERFACTIONLIST,						//���ذ���б�
		MSG_S2C_ORGSYS_AddAllFactions,							//�������еİ����ͻ���
		MSG_S2C_ORGSYS_ANSWERAPPLYJOINFACTION,					//���������
		MSG_S2C_ORGSYS_UpdateApplyMember,						//����������Ա				
		MSG_S2C_ORGSYS_UpdateFactionProperty,					//���°������
		MSG_S2C_ORGSYS_UpdateFactionBaseProperty,				//���°ﻧ��������
		MSG_S2C_ORGSYS_UpdateMemberInfo,						//���³�Ա��Ϣ
		MSG_S2C_ORGSYS_UpdateUnionMemberInfo,					//����ͬ�˳�Ա��Ϣ
		MSG_S2C_ORGSYS_UpdateLeaveWord,							//����������Ϣ
		MSG_S2C_ORGSYS_UpdatePronounce,							//���¹�����Ϣ
		MSG_S2C_ORGSYS_UpdateEnemyFaction,						//���µж԰����Ϣ
		MSG_S2C_ORGSYS_UpdateCityWarEnemyFaction,				//���¹�������ս�ж԰����Ϣ
		MSG_S2C_ORGSYS_UpdateOwnedCity,							//����ӵ�е�������Ϣ
		MSG_S2C_ORGSYS_UpdateFactionExp,						//���°��ľ�����ʾ���ͻ���
		MSG_S2C_ORGSYS_UpdateFactionInfo,						//���°����Ϣ���ͻ���
		MSG_S2C_ORGSYS_PlayerAddFactionExp,						//��ҹ����˰�ᾭ��
		MSG_S2C_ORGSYS_RequestAffirmEstaUnion,					//����ȷ�Ͻ���ͬ��
		MSG_S2C_ORGSYS_RequestAffirmJoinUnion,					//����ȷ������ͬ��
		MSG_S2C_FACTIONWAR_ANSWERFACTIONLIST,					//������ս����б�
		MSG_S2C_ORGSYS_RequestAffirmDigUpTheHatchet,			//����ȷ����ս
		MSG_S2C_ORGSYS_RequestAffirmMakePeace,					//����ȷ�����
		MSG_S2C_ORGSYS_RequestAffirmSurrender,					//����ȷ��Ͷ��
		MSG_S2C_ORGSYS_AnswerGetFactionBillboard,				//�õ�������а�
		MSG_S2C_ATTACKCITY_Start,								//����������ս��ʼ
		MSG_S2C_ATTACKCITY_End,									//����������ս����
		MSG_S2C_ATTACKCITY_Mass,								//����������ս���Ὺʼ
		MSG_S2C_ATTACKCITY_ClearOtherPlayer,					//�������������
		MSG_S2C_ATTACKCITY_RefreshRegion,						//����������ս���Ὺʼ
		MSG_S2C_ATTACKCITY_GetTodayTax,							//�ռ�����˰��
		MSG_S2C_CITY_RequestInputTaxPayment,					//��ȡ˰��
		MSG_S2C_CITY_RequestInputTaxRate,						//����˰��
		MSG_S2C_CITY_UpdateTaxRate,								//˰�ʸ���˰��
		MSG_S2C_CITY_RequestAffirmTransfer,						//����ȷ��ת������

		MSG_S2C_WAR_UpdateContendState,							//��������״̬
		MSG_S2C_WAR_UpdateContendTime,							//��������ʣ��ʱ��
		MSG_S2C_WAR_UpdateDiedState,							//��������״̬
		MSG_S2C_WAR_UpdateDiedStateTime,						//��������״̬ʣ��ʱ��

		MSG_S2C_Quest_Add,										//���һ������
		MSG_S2C_Quest_Complete,									//���һ������
		MSG_S2C_Quest_Remove,									//�Ƴ�һ������
		MSG_S2C_Quest_Update,									//����һ������

		MSG_S2C_ANSWER_PlayerRanks,								//��ͻ��˷���������а�

		MSG_S2C_ORGSYS_RESPONSE_DigUpTheHatchet,				//��ս�����Ӧ4.15.2008
		MSG_S2C_PLAYERSHOP				= MSG_S2C + MAX_SUB * 12,
		// ----------------------------------------------------
		MSG_S2C_PALYERSHOP_CREATE,
		MSG_S2C_PLAYERSHOP_BUYITEM,
		MSG_S2C_PLAYERSHOP_GOODSPRICE,
		MSG_S2C_PLAYERSHOP_OPENFORBUSINESS,
		MSG_S2C_PLAYERSHOP_CLOSEDOWN,
		MSG_S2C_PLAYERSHOP_LOOKGOODS,
		MSG_S2C_PLAYERSHOP_CLOSE,
		MSG_S2C_PLAYERSHOP_QUIT,
		//MSG_S2C_PLAYERSHOP_FETCHGOODS,
		//MSG_S2C_PLAYERSHOP_PUTGOODS,
		//MSG_S2C_PLAYERSHOP_EXCHANGEITEM,
		//MSG_S2C_PLAYERSHOP_HASCLOSED,
		//MSG_S2C_PLAYERSHOP_NOMOREMONEY,
		//MSG_S2C_PLAYERSHOP_NOSPACE,
		//MSG_S2C_PLAYERSHOP_NOTHISGOODS,
		//MSG_S2C_PLAYERSHOP_NOTSAFE,

		// ����
		MSG_S2C_CONTAINER				= MSG_S2C + MAX_SUB * 13,
		// ----------------------------------------------------
		MSG_S2C_CONTAINER_OBJECT_MOVE,
		MSG_S2C_CONTAINER_OBJECT_AMOUNT_CHANGE,
		MSG_S2C_CONTAINER_CLEAR,

		// ����
		MSG_S2C_PET						= MSG_S2C + MAX_SUB * 14,
		// ----------------------------------------------------
		MSG_S2C_PET_CHANGE_MASTER,								//�ı�����
		MSG_S2C_PET_CHANGE_MODE,								//�ı乥��ģʽ
		MSG_S2C_PET_CHANGE_EXPERIENCE,							//�ı�ȼ�
		MSG_S2C_PET_CHANGE_ACTION,								//�ı���ﵱǰ����
		MSG_S2C_PET_CARRIAGE,

		//����
		MSG_S2C_COUNTRY					= MSG_S2C + MAX_SUB * 15,
		// -----------------------------------------------------
		MSG_S2C_COUNTRY_CHANGE,									// ���Ĺ���

		MSG_S2C_COUNTRY_CHANGE_CI,								// ���ù���
		//////////////////////////////////////////////////////////////////
		// ����Ȩ�� S2C yulefox
		MSG_S2C_COUNTRY_KING_GETSTATE,							// �������� : ��ȡ��ǰ״̬���Ƿ��ǹ�������ս״̬��
		MSG_S2C_COUNTRY_KING_GetInfo,							// �������� : ��ȡ���й�����Ϣ
		MSG_S2C_COUNTRY_KING_GetPlayerList,						// �������� : ��ȡ��������б�
		MSG_S2C_COUNTRY_KING_Demise,							// �������� : ����
		MSG_S2C_COUNTRY_KING_Appoint,							// �������� : ����
		MSG_S2C_COUNTRY_KING_Depose,							// �������� : ��ְ
		MSG_S2C_COUNTRY_KING_Absolve,							// �������� : ����
		MSG_S2C_COUNTRY_KING_Silence,							// �������� : ����
		MSG_S2C_COUNTRY_KING_Exile,								// �������� : ����
		MSG_S2C_COUNTRY_KING_Convone,							// �������� : �ټ�
		MSG_S2C_COUNTRY_KING_Private,							// �������� : ������Ϣ
		MSG_S2C_COUNTRY_KING_ChangeControl,						// ����ͳ�������
		MSG_S2C_COUNTRY_COUNTRY_Message,						// ������Ϣ����
		MSG_S2C_COUNTRY_WORLD_Message,							// ������Ϣ����
		MSG_S2C_COUNTRY_PRIVATE_Message,						// ������Ϣ����

		// ��ս
		MSG_S2C_COUNTRY_WAR_Start,								//ս����ʼ
		MSG_S2C_COUNTRY_WAR_TimeOut,							//ս��ʱ�䵽
		MSG_S2C_COUNTRY_WAR_End,								//ս������
		MSG_S2C_COUNTRY_WAR_UpdateCountrys,

		// ��ֵ�̵�
		MSG_S2C_INCREMENTSHOP					= MSG_S2C + MAX_SUB * 16,
		// ----------------------------------------------------
		MSG_S2C_INCREMENTSHOP_OPEN,										//���̵�			
		MSG_S2C_INCREMENTSHOP_LIST,										//gameserver���ش��̵��е����б�
		MSG_S2C_INCREMENTSHOP_BUY,										//gameserver���ش��̵��й���һ����Ʒ�Ľ��
		MSG_S2C_INCREMENTSHOP_CLOSE,							//##�رո����̵�

		MSG_S2C_INCREMENTSHOP_GETLOG_RESPONSE,					// ������ֵ��־

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// For AuthServer messages, donot use these values between 
		// MSG_LS2AS_TYPE_START and MSG_LS2AS_TYPE_END.  - Kevin Lynx 3.6.2008
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MSG_LS2AS_TYPE_START = MAX_TYPE * 13,

		MSG_LS2AS_TYPE_END = MAX_TYPE * 14 - 1,
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//-----------------------------------------------------------------------------------------------------
		// BillingServer <--> GameServer
		// Fox, 2008/03/06
		//-----------------------------------------------------------------------------------------------------

		MSG_S2B = MAX_TYPE * 15,									// GameServer --> BillingServer

		MSG_S2B_SERVER = MSG_S2B + MAX_SUB * 0,
		//-----------------------------------------------------------------------------------------------------
		MSG_S2B_SERVER_SENDSERVERINFO,								// GS is connecting

		MSG_S2B_BILLING = MSG_S2B + MAX_SUB * 1,
		//-----------------------------------------------------------------------------------------------------
		MSG_S2B_BILLING_AUTHEN,										// Client request for account info
		MSG_S2B_BILLING_INCBUY,										// Incrementshop buying
		MSG_S2B_BILLING_TRADE,										// GS request lock client account

		MSG_B2S = MAX_TYPE * 16,									// BillingServer --> GameServer

		MSG_B2S_BILLING = MSG_B2S + MAX_SUB * 0,
		//-----------------------------------------------------------------------------------------------------
		MSG_B2S_BILLING_AUTHEN,										// BS respond with client's account info
		MSG_B2S_BILLING_INCBUY,										// Incrementshop buying
		MSG_B2S_BILLING_TRADE,										// BS respond to foot the bill

		MSG_B2B = MAX_TYPE * 17,									// BillingServer --> BillingServer
		MSG_B2B_SERVER = MSG_B2B + MAX_SUB * 0,
		//-----------------------------------------------------------------------------------------------------
		MSG_B2B_SERVER_GAMESERVERLOST,								// GS lost process

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// For GMA tool messages, do NOT use these values between 
		// MSG_GM2AS_TYPE_START and MSG_GM2AS_TYPE_END.  - Kevin Lynx 4.22.2008
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MSG_GM2AS_TYPE_START = MAX_TYPE * 17,

		MSG_GM2AS_TYPE_END = MAX_TYPE * 18 - 1,
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	};

	struct stMsg
	{
		unsigned long lSize;		// ��Ϣ��С
		unsigned long eType;		// ��Ϣ����
		unsigned long lTime;		// ʱ���(���͵�ʱ��)
		unsigned long lVerifyCode;	// У����
		//unsigned long ulReserveSize;// �������ݵĳ���
		//char *pReserveDate;				// �������ݣ�����һ�������������ݣ�


		//stMsg() {lSize=0;eType=0;lTime=0;ulReserveSize=(ULONG)strlen(pReserveDate)+1;pReserveDate=NULL;}
		//~stMsg() {}
	};

	////////////////////////////////////////////////////////////////////////
	//	��������
	////////////////////////////////////////////////////////////////////////

	vector<BYTE> m_MsgData;			// ��Ϣ������ͷָ��

protected:
	void SetSize(unsigned long l)	{((stMsg*)GetMsgBuf())->lSize = l;}

public:
	void SetType(unsigned long t)	{((stMsg*)GetMsgBuf())->eType = t;}
	void SetTime(unsigned long l)	{((stMsg*)GetMsgBuf())->lTime = l;}
	void SetCode(unsigned long l)	{((stMsg*)GetMsgBuf())->lVerifyCode = l;}
	unsigned long GetSize()			{return ((stMsg*)GetMsgBuf())->lSize;}
	unsigned long GetType()			{return ((stMsg*)GetMsgBuf())->eType;}
	unsigned long GetMsgSendTime()	{return ((stMsg*)GetMsgBuf())->lTime;}
	unsigned long GetCode()			{return ((stMsg*)GetMsgBuf())->lVerifyCode;}
	//void SetReserve(const char *pIn) { strcpy( ((stMsg*)GetMsgBuf())->pReserveDate , pIn); ((stMsg*)GetMsgBuf())->ulReserveSize = (ULONG)strlen((const char*)pIn)+1; }
	//ULONG GetReserve(char *pOut) { pOut = ((stMsg*)GetMsgBuf())->pReserveDate; return (ULONG)strlen(pOut)+1; }

	BYTE* GetMsgBuf()	{ return &m_MsgData[0]; }

	void Update()
	{
		SetSize((unsigned long)m_MsgData.size());
	}


	////////////////////////////////////////////////////////////////////////
	//	��Ӻͻ�ȡ����
	////////////////////////////////////////////////////////////////////////
public:
	void Add(char);
	void Add(BYTE);
	void Add(short);
	void Add(WORD);
	void Add(long);
	void Add(LONG64);
	void Add(DWORD);
	void Add(float);
	void Add(char*);
	void Add(const CGUID&	guid);
	void Add(void*, long size);
	void AddEx(void*, long size);//���Ӱ�ȫ�ԣ���GetEx���ʹ��
	void Reset(void);           //�����Ϣ����

	long m_lPtr;				// ��ǰ��ȡ��λ��

	char GetChar();
	BYTE GetByte();
	short GetShort();
	WORD GetWord();
	long GetLong();
	LONG64 GetLONG64();
	DWORD GetDWord();
	float GetFloat();
	bool  GetGUID(CGUID& guid);
	//char* Get(char*);
	void* Get(void*, long size);
	void* GetEx(void*, long size);
	char* GetStr( char* pStr,long lMaxLen );


	////////////////////////////////////////////////////////////////////////
	//	��Ϣ����
	////////////////////////////////////////////////////////////////////////

	CBaseMessage();
	virtual ~CBaseMessage();
	virtual long Run() = 0;            //���麯������Ϣ���к�����

	////////////////////////////////////////////////////////////////////////
	//	��Ϣѹ��
	////////////////////////////////////////////////////////////////////////
protected:
	//	static BYTE s_pRLEBuffer[524288];
	static DWORD s_dwRLEBufferLen;
	static BYTE *s_pRLEBuffer;
	static DWORD s_dwTemptBufferLen;
	static BYTE *s_pTemptBuffer;
	static CRITICAL_SECTION m_CriticalSectionCreateMSG;
	static CRITICAL_SECTION m_CSTemptBuffer;

public:
	static bool Initial();
	static bool Release();

	static int DoTemptBuffer(void* pIN,int nAddPos,int nLen);
	static int DoRLE(void* pIn, BYTE* pOut, int len);			// ѹ��
	static int DecodeRLE(BYTE* pIn, BYTE* pOut, int len);		// ��ѹ
	static int DecodeRLE_SAFE(BYTE* pIn, BYTE* pOut, int nInBufLen,int nOutBufLen);		//��ѹ,������߽���

	friend class CMsgQueue;
};

#endif
