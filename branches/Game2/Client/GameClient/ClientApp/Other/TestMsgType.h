/***********************************************************************************
/* ����     : ����
/* ����ʱ�� : 2009��2��2��
/* ժҪ     : �����˿ͻ�������Գ���֮�����Ϣͨ�����ͣ��ڴ�Ӱ�䷽ʽ��
/*			  ע�����ﶨ�����Ϣֵ�Ƕ�Ӧ��MsgType��Ϣֵ�ģ���Ҫ��Ϊ�˸���ݵ�ת��
/**********************************************************************************/
#pragma once

enum eTMsgNum
{
	TMAX_SUB			= 0x100,				// һ��С���͵������Ϣ����
	TMAX_TYPE			= TMAX_SUB * 0xff,		// һ�������͵������Ϣ����
};

enum eTMsgType
{

	//////////////////////
	//				    //
	//	   FILE_R_W		//
	//				    //
	//////////////////////

	//MSG_FILE						= TMAX_TYPE * 37,			// �ļ�����
	MSG_FILE_OPEN	=	1,										// ���ļ�
	MSG_FILE_WRITE,												// д�ļ�
	MSG_FILE_CLEAR,												// ����ļ�
	MSG_FILE_CLOSE,												// �ر��ļ�

	//////////////////////
	//				    //
	//	 ClientToTest	//
	//				    //
	//////////////////////
	//========================================================================================================
	MSG_L2T							= TMAX_TYPE * 11,			// Log -> Test(��ӦMSG_L2C = 718080)
	//========================================================================================================
	MSG_L2T_LOG						= MSG_L2T + TMAX_SUB * 0,	// ��½����(��ӦMSG_L2C_LOG = 718080)
	// ----------------------------------------------------
	MSG_L2T_LOG_ANSWERCDKEY,								// ����������CDKEY
	MSG_L2T_LOG_ANSWER_PLAYERBASELIST,						// ������������һ��������б�
	MSG_L2T_LOG_ANSWER_PLAYERDATA,							// ���������ϸ����
	MSG_L2T_LOG_ANSWERCREATE,								// ���������ؽ�����ɫ����Ϣ
	MSG_L2T_LOG_ANSWERDELETE,								// �����������Ƿ�����ɾ����Ϣ
	MSG_L2T_LOG_ANSWERRESTORE,								// �Ƿ�ɹ��ظ���ɫ
	MSG_L2T_LOG_UPDATEQUEUENUM,								// ���µ�½������е�ʣ������

	MSG_L2T_LOG_SENDACCOUNTINFO,							// �����ʺŵ���Ϣ���ͻ���
	MSG_L2T_LOG_UPDATEWORLDSERVERINFO,						// ����WorldServer��״̬��Ϣ


	//========================================================================================================
	MSG_C2T							= TMAX_TYPE * 12,			// Client --> Test(��ӦMSG_S2C = 783360)
	//========================================================================================================
	MSG_C2T_LOG						= MSG_S2C + MAX_SUB * 0,
	// ----------------------------------------------------
	MSG_C2T_LOG_ANSWER_PLAYERDATA,							// GameServer��GameClient����player��ϸ����
	MSG_C2T_LOG_GAMESERVERTIME,								// ����Server��ʱ��
	MSG_C2T_LOG_SEQUENCESTRING,								// ��֤��Ϣ�Ϸ��Ե��ַ�������

	// ----------------------------------------------------
	MSG_C2T_RGN						= MSG_C2T + TMAX_SUB * 1,	// ������Ϣ(��ӦMSG_S2C_RGN = 783616)
	// ----------------------------------------------------
	MSG_C2T_RGN_ADDSHAPE = 783618,							// �ж�����볡��(��ӦMSG_S2C_RGN_ADDSHAPE = 783618)
	MSG_C2T_RGN_DELSHAPE = 783620,							// �ж����뿪����(��ӦMSG_S2C_RGN_DELSHAPE = 783620)
	MSG_C2T_RGN_CHANGE   = 783621,							// ��������������л�����(��ӦMSG_S2C_RGN_CHANGE)
	MSG_C2T_RGN_CHANGESERVER = 783622,						// ��������������л�����������(��ӦMSG_S2C_RGN_CHANGESERVER)
	MSG_C2T_RGN_CHANGEWEATHER = 783623,						// �ı�����
	MSG_C2T_RGN_PLAYACTION = 783624,						// ���Ŷ���
	MSG_C2T_RGN_CHANGENAME = 783627,						// �ı䳡������(��ӦMSG_S2C_RGN_CHANGENAME)
	MSG_C2T_RGN_CLEARDUPRGNPLAYER = 783629,					// ����������Ϣ(��ӦMSG_S2C_RGN_CLEARDUPRGNPLAYER)


	// ----------------------------------------------------
	MSG_C2T_SHAPE					= MSG_C2T + TMAX_SUB * 2,			// ������Ϣ(��ӦMSG_S2C_SHAPE = 783872)
	// ----------------------------------------------------
	MSG_C2T_SHAPE_CHANGEDIR = 783873,						// �ı䷽��
	MSG_C2T_SHAPE_SETPOS = 783875,							// ˲��
	MSG_C2T_SHAPE_FORCEMOVE,								// ǿ���ƶ�
	MSG_C2T_SHAPE_MOVE,										// �ظ��ƶ���Ϣ
	MSG_C2T_SHAPE_STOPMOVE,									// ֹͣ�ƶ���Ϣ
	MSG_C2T_SHAPE_CHANGEMOVEMODE,							// �ƶ�ģʽ�ı�
	MSG_C2T_SHAPE_CHANGESTATE = 783881,						// �ı�״̬(��ӦMSG_S2C_SHAPE_CHANGESTATE = 783881)
	MSG_C2T_SHAPE_ATK_DIED = 783884,						// ����
	MSG_C2T_SHAPE_SINSTATE = 783887,						// ֪ͨclient������˳�����״̬
	MSG_C2T_SHAPE_Refresh = 783888,							// ˢ�½���״̬ 
	MSG_C2T_SHAPE_EMOTION = 783890,							// ����

	MSG_C2T_SHAPE_ATK_DAMAGE,								// ��ͨ������Ϣ
	MSG_C2T_SHAPE_ATK_BREAK,								// ������
	MSG_C2T_SHAPE_ATK_FULLMISS,								// ����
	MSG_C2T_SHAPE_ATK_PARRY,								// �м�
	MSG_C2T_SHAPE_ATK_BLOCK,								// ��
	MSG_C2T_SHAPE_ATK_RESIST,								// �ֿ�
	MSG_C2T_SHAPE_ATK_REBOUND, 							    // ����
	MSG_C2T_SHAPE_ATK_SUCK,									// ����
	MSG_C2T_SHAPE_RELIVE,									// ����һ�����
	MSG_C2T_SHAPE_CHANGE_PROPERTIES,						// ���¿��ƶ����������
	MSG_C2T_SHAPE_ATK_LOSE,									// ��ʧ
	MSG_C2T_SHAPE_CHANGE_FIRSTATTACKER,						// �ı��һ�������Լ�����
	MSG_C2T_SHAPE_LEAVEFIGHT,								// ����ս��
	MSG_C2T_SHAPE_SLIPPOS,									// ����λ��
	MSG_C2T_SHAPE_ATK_ADDBLOOD = 783906,					// ��Ѫ
	MSG_C2T_SHAPE_GETTSELECTEDSHAPE,						// ��õ�ǰѡ���Ŀ��


	// ----------------------------------------------------
	MSG_C2T_PLAYER					= MSG_C2T + TMAX_SUB * 3,			// �����Ϣ(��ӦMSG_S2C_PLAYER = 784128)
	// ----------------------------------------------------
	MSG_C2T_PLAYER_RESUME,									// �ָ�HP��MP��YP������
	MSG_C2T_PLAYER_ANSWER_RELIVE = 784131,					// �ָ�����
	MSG_C2T_PLAYER_ADDEXP,									// ���Ӿ���ֵ
	MSG_C2T_PLAYER_LEVELUP,									// ����
	MSG_C2T_PLAYER_ADDOCCUEXP,								// ����ְҵ����ֵ
	MSG_C2T_PLAYER_OCCULEVELUP,								// ְҵ�ȼ�����
	MSG_C2T_PLAYER_ADDSP,									// ����sp
	MSG_C2T_PLAYER_CREDIT_UPDATE,							// ��������
	MSG_C2T_PLAYER_SETLEVEL = 784138,						// ������ҵȼ�
	MSG_C2T_PLAYER_USEITEM,									// gameserver��ͻ��˷������ĵ��ߵ�ʹ�ý��
	MSG_C2T_PLAYER_CHANGEMAXATK,							// �����Ե���������󹥻���
	MSG_C2T_PLAYER_CHANGEATKSPEED,							// �����Ե������������ٶ�
	MSG_C2T_PLAYER_CHANGEDEF,								// �����Ե���������������
	MSG_C2T_PLAYER_SYNC_PKCOUNT = 784144,					// ͬ��ɱ����
	MSG_C2T_PLAYER_TRADEREQUEST,							// GSת������������ͻ�
	MSG_C2T_PLAYER_TRADEBEGIN,								// ���׿�ʼ
	MSG_C2T_PLAYER_TRADELOCK,
	MSG_C2T_PLAYER_WAITFORTRADE = 784154,					// ��ҽ��뽻�׵ȴ�״̬�ķ�����Ϣ
	MSG_C2T_PLAYER_TRADECANCEL = 784156,					// ������֪ͨ��ҶԷ��Ѿ�ȡ������
	MSG_C2T_PLAYER_TRADERESULT,								// ���׵����ս��
	MSG_C2T_PLAYER_ADDSKILL = 784159,						// ��Ӽ���
	MSG_C2T_PLAYER_DELSKILL,								// ɾ������
	MSG_C2T_PLAYER_CHANGE_EQUIPMENT = 784162,
	MSG_C2T_PLAYER_CHANGE_PROPERTIES,						// ��Ҹı�����
	MSG_C2T_PLAYER_UpdateDisplayHeadPiece,					// ��������Ƿ���ʾͷ����Ϣ
	MSG_C2T_PLAYER_AUTOMOVE,								// �Զ��ƶ���ָ���ĵ�
	MSG_C2T_PLAYER_CHANGE_CONTRIBUTE,						// ���ҹ��׸ı�
	MSG_C2T_PLAYER_FORCE_TARGET,							// ����ǿ�ƹ���Ŀ��(һ�㴦���������Ƽ��ܵ����)

	MSG_C2T_PLAYER_COLLECTION_LEVELUP,						// ��Ҳɼ�����
	MSG_C2T_PLAYER_FACTURE_LEVELUP,							// �����������
	MSG_C2T_PLAYER_RELIVE_RESULT,							// ����ʧ��
	MSG_C2T_PLAYER_BUYRELIVEITEM_RESULT,					// ���򸴻���߽��
	MSG_C2T_PLAYER_SKILL_RELIVE,							// ���ܸ���
	MSG_C2T_PLAYER_UPDATE_TITLE_TYPE,						// ��Ҹ��³ƺ�
	MSG_C2T_PLAYER_COLLECTION_ADDEXP,						// ��Ҳɼ���������
	MSG_C2T_PLAYER_FACTURE_ADDEXP,							// ���������������


	// ----------------------------------------------------
	MSG_C2T_OTHER					= MSG_C2T + TMAX_SUB * 4,			// ������Ϣ(��ӦMSG_S2C_OTHER = 784384)
	// ----------------------------------------------------
	MSG_C2T_OTHER_TALK,										// �Ի���Ϣ
	MSG_C2T_OTHER_FACTIONTALK,								// �л�����
	MSG_C2T_OTHER_BROADCAST,								// �㲥
	MSG_C2T_OTHER_AddTopInfo,								// ��Ӷ�����ʾ��Ϣ
	MSG_C2T_OTHER_OPENTALKBOX,								// �ÿͻ��˵���TalkBox
	MSG_C2T_OTHER_OPENPIXYTALKBOX,							// �ÿͻ��˵���PixyTalkBox
	MSG_C2T_OTHER_ADDINFO,									// ����ҶԻ������һ����Ϣ
	MSG_C2T_OTHER_ADDINFO_BYINDEX,							// ����ҶԻ������һ����Ϣ(�����������)
	MSG_C2T_OTHER_ADDSYSTEMINFO,							// ����Ļ�Ϸ�����һ����Ϣ
	MSG_C2T_OTHER_GAMEMESSAGE,								// ������Ϣ�Ի���
	MSG_C2T_OTHER_SENDOBJECTPROPERTIY = 784396,				// ����Shape����������
	MSG_C2T_OTHER_SCRIPTTIMERRUN = 784400,					// �ű���ʱ������
	MSG_C2T_OTHER_ANSWERNPCLIST = 784405,					// ����NPC�б���ͻ���
	MSG_C2T_OTHER_OPENSTORYTALKBOX = 784415,				// �ÿͻ��˵���StoryTalkBox
	MSG_C2T_OTHER_EXPATC = 784417,							// ��������	
	MSG_C2T_OTHER_GoodsInfo_Re,								// ��Ʒ��Ϣ�ظ�
	MSG_C2T_OTHER_PLAY_VIDEO = 784420,						// ������Ƶ
	MSG_C2T_OTHER_STOP_VIDEO,								// �ر���Ƶ
	MSG_C2T_OTHER_REPEATLOGIN = 784423,						// �˺��ظ���¼
	MSG_C2T_OTHER_UPDATEVARVALUEOPEN,						// ���޸Ľű�����ֵ�Ĵ���


	// ----------------------------------------------------
	MSG_C2T_GOODS					= MSG_C2T + TMAX_SUB * 5,			// ��Ʒ��Ϣ(��ӦMSG_S2C_GOODS = 784640)
	// ----------------------------------------------------
	MSG_C2T_PICKGOOD = 784645,								// gameserver���ؼ���Ʒ���
	MSG_C2T_DROPMONEY,										// ��Ǯ						
	MSG_C2T_PICKMONEY,										// ��Ǯ
	MSG_C2T_SETHOTKEY,										// gameserver��������HOTKEY���
	MSG_C2T_CLEARHOTKEY,						
	MSG_C2T_EXCHANGEHOTKEY,						
	MSG_C2T_EQUIPDAMAGE,                                    // װ������
	MSG_C2T_EQUIPDURUPDATE,									// ����װ�����;�
	MSG_C2T_EQUIP_REFINE_UPDATE,							// ���ȸ���
	MSG_C2T_DELETEEQUIP,									// ɾ��װ��
	MSG_C2T_DELETEGOODS = 784656,							// ɾ��������ָ��λ��ָ����������Ʒ							
	MSG_C2T_DELETEALLGOODS,									// ɾ��������ָ��λ�õ�ȫ����Ʒ	
	MSG_C2T_OPENGOODSUPGRADE = 784659,						// gameserver֪ͨ��Ҵ���Ʒ��������
	MSG_C2T_OPENGOODSENCHASE,								// �򿪿�Ƭ��Ƕ����ҳ��
	MSG_C2T_ENCHASE_OPEN_HOLE,								// ������Ϣ
	MSG_C2T_ENCHASE_CLOSE_HOLE,								// �¿���Ϣ
	MSG_C2T_ENCHASE_ENCHASECARD,							// �⿨��Ϣ
	MSG_C2T_ENCHASE_REMOVECARD,								// ����Ϣ
	MSG_C2T_ENCHASE_ARRANGE,								// ��С����ҳ����Ϣ
	MSG_C2T_GOODSUPGRADE_UPGRADE = 784670,					// gameserver��������һ����Ʒ�Ľ��
	MSG_C2T_GOODS_UPDATEPROPERTY,							// ������Ʒ����

	MSG_C2T_SET_YUANBAO = 784674,							// ��������
	MSG_C2T_SET_TICKET,										// ���õ�ȯ

	//! GoodsExManageӦ��
	////////////////////////////////////////////////////////////////////////
	MSG_C2T_GOODS_OPEN_WINDOWS,								//������Ʒ�������ڣ�ָ�����ڵ����ͣ�
	MSG_C2T_GOODS_CLOSE_WINDOWS,							//���ر�������Ʒ��������
	//! �ֽ���Ʒ
	MSG_C2T_GOODS_RE_DISASSEMBLE_PREVIEW,					//! ����Ԥ�����
	MSG_C2T_GOODS_RE_DISASSEMBLE_DO,						//! ���طֽ���Ʒ�ɰ�
	//! �ϳ���Ʒ
	MSG_C2T_GOODS_RE_SYNTHESIZE_PREVIEW,					//! ����Ԥ�����
	MSG_C2T_GOODS_RE_SYNTHESIZE_DO,							//! ���غϳɳɰ�
	//! ��������
	MSG_C2T_GOODS_RE_REINFORCE_POWER,						//! ������������
	MSG_C2T_GOODS_POWER_WARN,								//! ��������ʣ��֪ͨ
	//! �󶨶���
	MSG_C2T_GOODS_BIND,										//! ��Ʒ�󶨣����֪ͨ
	MSG_C2T_GOODS_FROST,									//! ��Ʒ���ᣨ�ⶳ��֪ͨ

	//! ��Ʒ����
	MSG_C2T_GOODS_RE_UPGRADE_VIEW,							//! �ظ�Ԥ������
	MSG_C2T_GOODS_RE_UPGRADE_DO,							//! �ظ�ִ������
	//! ��Ʒħ��
	MSG_C2T_GOODS_RE_STUFF_SYNTHESIZE,						//! �ظ����Ϻϳ�
	MSG_C2T_GOODS_RE_PLUG_EVIL_VIEW,						//! �ظ�ħ��Ԥ������
	MSG_C2T_GOODS_RE_PLUG_EVIL_DO,							//! �ظ�ִ������
	MSG_C2T_GOODS_RE_EVIL_BACKOUT_VIEW,						//! �ظ����Ԥ������
	MSG_C2T_GOODS_RE_EVIL_BACKOUT,							//! ħ��װ�����

	//! �ֿ�
	MSG_C2T_DEPOT_OPEN,										//!	֪ͨ�ͻ��˴򿪲ֿ�
	MSG_C2T_DEPOT_RE_INPUT_PWD,								//! �ظ��ͻ�����������
	MSG_C2T_DEPOT_RE_CLOSE,									//! �ظ��رղֿ⣬Ҳ��Ϊ�ر�֪ͨ
	MSG_C2T_DEPOT_RE_CHANGE_PWD,							//! �ظ��޸�����
	MSG_C2T_DEPOT_RE_BUY_SUBPACK,							//! �����ӱ���
	MSG_C2T_DEPOT_FROSTED,									//! ֪ͨ�ֿⱻ����

	//! ѫ��
	MSG_C2T_MEDAL_ADD_NOTIFY,								//! ���ѫ��֪ͨ
	MSG_C2T_MEDAL_DEL_NOTIFY,								//! ɾ��ѫ��֪ͨ

	//! �Ǹ�ְҵ����
	MSG_C2T_MAKE_BEGIN,										//! ��ʼһ������
	MSG_C2T_MAKE_RESULT,									//! ����һ���������


	// ----------------------------------------------------
	MSG_C2T_SHOP					= MSG_C2T + TMAX_SUB * 6,			// �̵���Ϣ(��ӦMSG_S2C_SHOP = 784896)
	// ----------------------------------------------------
	MSG_C2T_SHOP_OPEN,										// ���̵�
	MSG_C2T_SHOP_BUY_BAT = 784899,							// gameserver���ش��̵�������Ʒ�Ľ��
	MSG_C2T_SHOP_SELL,										// ����ȷ��������Ʒ
	MSG_C2T_SHOP_SELL_BAT,									// gameserver��������Ʒ���̵�Ľ��
	MSG_C2T_SHOP_GOODSCHANGEGOODS,							// �����������̵����Ϣ
	MSG_C2T_SHOP_REPAIR,									// gameserver�������̵�������һ����Ʒ�Ľ��
	MSG_C2T_SHOP_REPAIRALL,
	MSG_C2T_SHOP_REPAIRALLCOST,								// ������������������ȫ����Ʒ�Ļ���
	MSG_C2T_BUSINESS_OPEN = 784908,
	MSG_C2T_BUSINESS_BUY,
	MSG_C2T_BUSINESS_SELL,
	MSG_C2T_INVEST_OPEN,
	MSG_C2T_SHOP_BUYBACK = 784913,
	MSG_C2T_BUSINESS_UPDATEPRICE,							// ��ҵ����۸����


	// ----------------------------------------------------
	MSG_C2T_DEPOT					= MSG_C2T + TMAX_SUB * 7,
	// ----------------------------------------------------

	// ----------------------------------------------------
	MSG_C2T_GM						= MSG_C2T + TMAX_SUB * 8,
	// ----------------------------------------------------

	// ----------------------------------------------------
	MSG_C2T_TEAM					= MSG_C2T + TMAX_SUB * 9,			// ������Ϣ(��ӦMSG_S2C_TEAM = 785664)
	// ----------------------------------------------------
	MSG_C2T_TEAM_TeamCreate = 785666,						//! ���ݸ�ʽͬMSG_S2C_TEAM_TeamData����ֻ�ڽ���ʱ����
	MSG_C2T_TEAM_TeamData,									//! ���ж������ݵ���Ա
	MSG_C2T_TEAM_MemberData,								//! һ����Ա�Ķ�������
	MSG_C2T_TEAM_TeamDataChange,							//! �������ݸı�

	MSG_C2T_TEAM_MemberAllIdioinfo,							//! һ����Ա���зǶ����Ա����
	MSG_C2T_TEAM_UpdateIdioinfo,							//! ����ĳ���Ƕ����Ա����
	MSG_C2T_TEAM_MembereExStateChange,						//! ��Ա�쳣״̬�ı�
	MSG_C2T_TEAM_MemberStateChange,							//! ��Ա����״̬�ı�

	MSG_C2T_TEAM_MemberLeave,								//! ��Ա���
	MSG_C2T_TEAM_MemberLeaveHere,							//! ��Ա�뿪����(GS)
	MSG_C2T_TEAM_MastterChange,								//! �ӳ��ı�

	MSG_C2T_TEAM_Disband,									//! �����ɢ
	MSG_C2T_TEAM_GoodsSetChange,							//! �޸���Ʒ��������

	MSG_C2T_TEAM_MemberPos,									//! ���³�Աλ��
	MSG_C2T_TEAM_RECRUIT,									//! ������ļ״̬
	MSG_C2T_TEAM_RECRUITED_NUM,								//! ��������ļ������
	//! [add end]

	//! old used
	MSG_C2T_TEAM_QUERY_JOIN,								// �������
	MSG_C2T_TEAM_CHAT,										// ����

	//! old unused
	MSG_C2T_TEAM_ANSWER_JOIN,								//�ظ��������
	MSG_C2T_TEAM_SYNCHRONIZE,								//ͬ���������Ϣ
	MSG_C2T_TEAM_TEAMATE_JOIN,
	MSG_C2T_TEAM_TEAMATE_LEAVE,
	MSG_C2T_TEAM_TEAMATE_CHANGE_REGION,
	MSG_C2T_TEAM_CHANGE_LEAER,									//�ı�ӳ�
	MSG_C2T_TEAM_CHANGE_ALLOCATION_SCHEME,

	MSG_C2T_TEAM_CHANGE_STATE,


	// -----------------------------------------------------
	MSG_C2T_SKILL					= MSG_C2T + TMAX_SUB * 10,			// ������Ϣ(��ӦMSG_S2C_SKILL = 785920)
	// -----------------------------------------------------
	MSG_C2T_SKILL_USE,										// ʹ�ü���
	MSG_C2T_SKILL_STATE_CHANGE,								// ʹ�ü��ܺ���HP/MP/RP/YP״̬�ı�
	MSG_C2T_SKILL_ADD_EX_STATE,								// ʹ�ü��ܺ�����쳣״̬
	MSG_C2T_SKILL_END_EX_STATE,								// �쳣״̬����ʧ,������֪ͨ�ͻ���
	MSG_C2T_SKILL_UPDATE_EX_STATE,							// ����״̬ʱ��
	MSG_C2T_SKILL_EX_STATE_ADDITIONAL_DATA,					// �쳣״̬�ĸ�������
	MSG_C2T_SKILL_AFFECT_FIELD,								// ������Ӱ���������ͻ���
	MSG_C2T_SKILL_STUDY_FAILED,								// ѧϰ����ʧ��
	MSG_C2T_SKILL_STUDY_BEGIN,								// �ͻ��˷��ش򿪼���ѧϰ��ʦҳ��
	MSG_C2T_OCCUSKILL_STUDY_BEGIN,							// �յ��򿪸�ְҵ����ѧϰҳ����Ϣ
	MSG_C2T_OCCUSKILL_STUDY,								// �յ�ѧϰ��ְҵ������Ϣ
	MSG_C2T_OCCUSKILL_DELETE,								// ��ͻ��˷���ɾ��������Ŀ��Ϣ
	MSG_C2T_OCCUSKILL_NPCFACTURE_BEGIN,						// ��ͻ��˷���NPC�����б�
	MSG_C2T_OCCUSKILL_NPCFACTURE,							// ����NPC������Ϣ
	MSG_C2T_SKILL_UNLOCK_GOODS,								// ���ͽ����Ʒ������Ϣ
	MSG_C2T_SKILL_LOCK_GOODS,								// ������Ʒ��Ϣ

	MSG_C2T_SKILL_ADD_PET,
	MSG_C2T_SKILL_DEL_PET,
	MSG_C2T_SKILL_USE_PET,
	MSG_C2T_SKILL_END_PET,


	// -----------------------------------------------------
	MSG_C2T_ORGSYS					= MSG_C2T + TMAX_SUB * 11,			// ������Ϣ(��ӦMSG_S2C_ORGSYS = 786176)
	// -----------------------------------------------------

	// �л�Э��-------------------------------------------------
	MSG_C2T_ORGSYS_InitOrgSys_Notify,					//! ��ʼ����֯ϵͳ֪ͨ
	MSG_C2T_ORGSYS_UpdatePlayerFacInfo,					//! ����һ����ҵĹ�����Ϣ

	//! �᳤����
	MSG_C2T_ORGSYS_CreateFaction_Notify,				//! �����л�֪ͨ(���������ϣ����ͻ���Ӧ����һ�����л�����)
	MSG_C2T_ORGSYS_CreateFaction_Re,					//! �����л��Ӧ
	MSG_C2T_ORGSYS_UpgradeFaction_Notify,				//! �����л�֪ͨ
	MSG_C2T_ORGSYS_UpgradeFaction_Re,					//! �����л�ظ�
	MSG_C2T_ORGSYS_DisbandFaction_Notify,				//! ��ɢ�л�֪ͨ
	MSG_C2T_ORGSYS_DisbandFaction_Re,					//! ��ɢ�л��Ӧ	
	MSG_C2T_ORGSYS_SetPlacard_Re,						//! �޸Ĺ����Ӧ
	MSG_C2T_ORGSYS_SetLeaveword_Re,						//! �޸ı�����Ӧ
	MSG_C2T_ORGSYS_SetJobName_Re,						//! �޸Ĺٽ����ֻ�Ӧ
	MSG_C2T_ORGSYS_SetJobPurview_Re,					//! �޸Ĺٽ�Ȩ�޻�Ӧ
	MSG_C2T_ORGSYS_SetJobPurview_Notify,				//! �޸Ĺٽ�Ȩ�޻�Ӧ
	MSG_C2T_ORGSYS_SetMemberJob_Re,						//! ���ó�Ա�ٽ׻�Ӧ
	MSG_C2T_ORGSYS_SetMemberJob_Notify,					//! ���ó�Ա�ٽ�֪ͨ���¼ӣ�
	MSG_C2T_Faction_SetMemberTitle_Re,					//! ���ó�Ա�ƺŻ�Ӧ
	MSG_C2T_Faction_SetMemberTitle_Notify,				//! ���ó�Ա�ƺ�֪ͨ���¼ӣ�
	MSG_C2T_Faction_SetChairman_Re,						//! ��λ�ظ�
	MSG_C2T_Faction_SetChairman_Notify,					//! ��λ֪ͨ
	MSG_C2T_Faction_ChangeRecruitState_Notify,			//! ��ļ״̬�ı�֪ͨ

	//! ��Ա����
	MSG_C2T_ORGSYS_Quit_Re,								//! �˳��л��Ӧ
	MSG_C2T_ORGSYS_Quit_Notify,							//! �˳��л�֪ͨ
	MSG_C2T_ORGSYS_Kick_Re,								//! �߳��л��Ӧ
	MSG_C2T_ORGSYS_Kick_Notify,							//! �߳��л�֪ͨ
	MSG_C2T_ORGSYS_CheckRequisition_Re,					//! ��������Ӧ
	MSG_C2T_ORGSYS_CheckRequisition_Notify,				//! ���������������֪ͨ���¼ӣ�
	MSG_C2T_Faction_InvitePlayer_Re,					//! ������Ҽ����Ӧ
	MSG_C2T_Faction_InvitePlayer_Notify,				//! ������Ҽ���֪ͨ
	MSG_C2T_Faction_AddMember_Notify,					//! ��ӳ�Ա֪ͨ
	MSG_C2T_Faction_ApplyBoardOpen_Notify,				//! ������������֪ͨ

	MSG_C2T_ORGSYS_JoinFac_Re,							//! ��������ظ�

	//! ��������
	MSG_C2T_Faction_PublicData_Notify,					//! �л����ϣ�tagFacPublicData�ṹ��
	MSG_C2T_Faction_BaseData_Notify,					//! �л����ϣ�tagFacBaseData�ṹ��
	MSG_C2T_Faction_Member_Notify,						//! ��Ա��������
	MSG_C2T_Faction_Apply_Notify,						//! ��Ա��������
	MSG_C2T_Faction_Placard_Notify,						//! �������ϻ�Ӧ
	MSG_C2T_Faction_Leaveword_Notify,					//! �������ϻ�Ӧ


	MSG_C2T_Faction_IconBoardOpen_Notify,				//! ��ͼ���ϴ����֪ͨ
	MSG_C2T_Faction_IconData_Re,						//! �ϴ�һ������ͼ�����ݻظ�
	MSG_C2T_Faction_IconData_Notify,					//! һ�������ͼ�����ݸ���֪ͨ
	MSG_C2T_Faction_AllIconData_Re,						//! �������й���ͼ����Ϣ						

	MSG_C2T_Faction_MemberMap_Notifye,					//! ���³�Ա��ͼ
	MSG_C2T_Faction_MemberLevel_Notifye,				//! ���³�Ա�ȼ�
	MSG_C2T_Faction_MemberOccu_Notifye,					//! ���³�Աְҵ
	MSG_C2T_Faction_MemberOnline_Notifye,				//! ���³�Ա����״̬

	MSG_C2T_Faction_ChangeRes_Notify,					//! ��Դֵ���
	MSG_C2T_Faction_ChangeExp_Notify,					//! ����ֵ���
	MSG_C2T_Faction_ChangeMemberContribute_Notify,		//! ��Ա���ױ��
	// End���л�------------------------------------------------



	// ����Э��-------------------------------------------------
	MSG_C2T_UNION_Script_Creation,						//! �������˽ű�����֪ͨ�������˴�������
	MSG_C2T_UNION_Re_Creation,							//! �������˻ظ�
	MSG_C2T_UNION_Notify_Creation,						//! ��������֪ͨ
	MSG_C2T_UNION_Notify_Disband,						//! ��ɢ����֪ͨ��������
	MSG_C2T_UNION_Notify_Quit,							//! �л��˳�����֪ͨ��������
	MSG_C2T_UNION_Notify_Kick,							//! �������л��߳�����֪ͨ
	MSG_C2T_UNION_Re_Kick,								//! �������л��߳�����֪ͨ
	MSG_C2T_UNION_Notify_SetChairman,					//! ����ת��֪ͨ��������
	MSG_C2T_UNION_Re_SetRights,							//! ��������᳤Ȩ�޻ظ�
	MSG_C2T_UNION_Notify_GetRights,						//! �᳤һ���˻����������Ȩ�޵�֪ͨ
	MSG_C2T_UNION_Response_Invite,						//! ���� �յ� �л�᳤ �ظ���������
	MSG_C2T_UNION_Notify_Anwser_Invite,					//! �л�᳤ �յ� ��������������˵�����
	MSG_C2T_UNION_Notify_Join,							//! �л��������֪ͨ��������
	MSG_C2T_UNION_Notify_BaseData,						//! һ�����˵Ļ�������֪ͨ
	MSG_C2T_UNION_Notify_MemberData,					//! һ�����˵�һ����Ա������ӻ���µ�֪ͨ
	MSG_C2T_UNION_Notify_AllMemberData,					//! һ�����˵����г�Ա����֪ͨ
	// End������------------------------------------------------




	MSG_C2T_Quest_Add,										//���һ������
	MSG_C2T_Quest_Complete,									//���һ������
	MSG_C2T_Quest_Remove,									//�Ƴ�һ������								
	MSG_C2T_Quest_UpdateStep,								//����һ������
	MSG_C2T_Quest_UpdateParamData,							//����һ����������б���
	MSG_C2T_Quest_UpdateData,								//����һ������ı���
	MSG_C2T_Quest_UpdateRecordData,							//�������һ�������¼
	MSG_C2T_Quest_Talk,
	MSG_C2T_MercQuest_UpdateData,							//���ͻ��˷���ٸ���������ݲ�ͬʱ�򿪽���
	MSG_C2T_MercQuest_OpenInterface,						//��ٸ���������
	
	MSG_C2T_ANSWER_PlayerRanks,								//��ͻ��˷���������а�
	//�ͽ�����
	MSG_C2T_SHARE_QUESTLIST_UPDATE,							//���¹��������б�
	MSG_C2T_SHARE_QUEST_OPEN,								//�򿪶������ڽ��е��������
	MSG_C2T_SHARE_CURRQUEST,								//���͵�ǰ���ڽ��е�����ID


	// -----------------------------------------------------
	MSG_C2T_PLAYERSHOP				= MSG_C2T + TMAX_SUB * 12,			// ����̵���Ϣ(��ӦMSG_S2C_PLAYERSHOP = 786432)
	// -----------------------------------------------------
	MSG_C2T_PALYERSHOP_CREATE,								// ��������̵�
	MSG_C2T_PLAYERSHOP_BUYITEM,
	MSG_C2T_PLAYERSHOP_ADDSUCCEED,
	MSG_C2T_PLAYERSHOP_OPENFORBUSINESS,
	MSG_C2T_PLAYERSHOP_CLOSEDOWN,
	MSG_C2T_PLAYERSHOP_LOOKGOODS,
	MSG_C2T_PLAYERSHOP_CLOSE,
	MSG_C2T_PLAYERSHOP_QUIT,
	MSG_C2T_PLAYERSHOP_TOBUYER,
	MSG_C2T_PLAYERSHOP_TOSELLER,
	MSG_C2T_PLAYERSHOP_UPDATE,
	MSG_C2T_PLAYERSHOP_REMOVE,



	// ----------------------------------------------------
	MSG_C2T_CONTAINER				= MSG_C2T + TMAX_SUB * 13,			// ������Ϣ
	// ----------------------------------------------------
	MSG_C2T_CONTAINER_OBJECT_MOVE,
	MSG_C2T_CONTAINER_OBJECT_UNLOCK,
	MSG_C2T_CONTAINER_OBJECT_AMOUNT_CHANGE,
	//MSG_C2T_CONTAINER_CLEAR,


	// ----------------------------------------------------
	MSG_C2T_PET						= MSG_C2T + TMAX_SUB * 14,			// ������Ϣ
	// ----------------------------------------------------
	MSG_C2T_PET_MOVE,										// �ƶ�
	MSG_C2T_PET_STOP,										// ֹͣ
	MSG_C2T_PET_CHANGE_MODE,								// �ı���ﵱǰģʽ
	MSG_C2T_PET_CHANGE_ACTION,								// �ı���ﵱǰ����
	MSG_C2T_PET_CHANGE_SPEED,								// �ı��ٶ�
	MSG_C2T_PET_CHANGE_MASTER,								// �ı�����
	MSG_C2T_PET_CHANGE_LEVEL,								// �ı�ȼ�



	// -----------------------------------------------------
	MSG_C2T_COST					= MSG_C2T + TMAX_SUB * 15,			//�Ʒ�
	// -----------------------------------------------------
	MSG_C2T_COST_NOTENOUGHCOST,

	//���۵㿨 s->c
	// -----------------------------------------------------
	MSG_C2T_CARD					= MSG_C2T + TMAX_SUB * 16, 
	// -----------------------------------------------------
	

	//����
	// -----------------------------------------------------
	MSG_C2T_COUNTRY					= MSG_C2T + TMAX_SUB * 17, 
	// -----------------------------------------------------
	MSG_C2T_COUNTRY_CHANGE,									// ���Ĺ���



	// ----------------------------------------------------
	MSG_C2T_INCREMENTSHOP			= MSG_C2T + TMAX_SUB * 18,			// ��ֵ�̵�
	// ----------------------------------------------------
	MSG_C2T_INCREMENTSHOP_OPEN,								// ���̵�			
	MSG_C2T_INCREMENTSHOP_BUY,								// gameserver���ش��̵��й���һ����Ʒ�Ľ��
	MSG_C2T_INCREMENTSHOP_CLOSE,							// ##�رո����̵�
	MSG_C2T_INCREMENTSHOP_AFFICHE_RESPONSE,					// ���洰�ڿ�

	MSG_C2T_INCREMENTSHOP_GETLOG_RESPONSE,					// ������ֵ��־


	// ----------------------------------------------------
	MSG_C2T_LINKMAN					= MSG_C2T + TMAX_SUB * 19,			// ����ϵͳ
	// ----------------------------------------------------
	MSG_C2T_LINKMAN_INIT,										//! ��ʼ����ϵ���б�
	MSG_C2T_LINKMAN_ADD,										// S ֪ͨ A �����ϵ�� B �Ľ��
	MSG_C2T_LINKMAN_DEL,										// S ֪ͨ A ɾ����ϵ�� B �Ľ��
	MSG_C2T_LINKMAN_MOVE,										// S ֪ͨ A �ƶ���ϵ�� B �Ľ��

	MSG_C2T_LINKMAN_OTHER_HANDLE,								// S ֪ͨ B�� A �� B �� ��ӡ�ɾ�����ƶ� �Ȳ������

	MSG_C2T_LINKMAN_REQUEST_ADD,								// S ֪ͨ B�� A �� B ���������

	MSG_C2T_LINKMAN_LOGIN,										// ��ϵ������
	MSG_C2T_LINKMAN_LOGOUT,										// ��ϵ������

	MSG_C2T_LINKMAN_INFO_CHANGE,								// ��Ϣ�ı䣺��ͼ���ȼ���ְҵ
	MSG_C2T_LINKMAN_DELSTATE_CHANGE,							// ɾ��״̬�ı�  

	MSG_C2T_LINKMAN_LEAVE_WORD,									//! �������Ե��ͻ���
	MSG_C2T_LEAVEWORD_SAVE_RESULT,								//! ���Դ�����


	// ----------------------------------------------------
	MSG_C2T_SPRITE					= MSG_C2T + TMAX_SUB * 20,			//! С����
	// ----------------------------------------------------
	MSG_C2T_SPRITE_AutoSupply_OnOff_Re,							//! �Զ�����ϵͳ���ظ����ܿ���
	MSG_C2T_SPRITE_AutoSupply_ChangeSetup_Re,					//! �Զ�����ϵͳ���ظ��޸�һ����������
	MSG_C2T_SPRITE_AutoSupply_UseItem_Notify,					//! �Զ�����ϵͳ��֪ͨʹ����һ��ҩƷ
	MSG_C2T_SaveNewSetupData_Re,								//! ��ұ����µ���Ϸ���ûظ�



	// ----------------------------------------------------
	MSG_C2T_MAIL					= MSG_C2T + TMAX_SUB * 29,			//! �ʼ�ϵͳ
	// ----------------------------------------------------
	MSG_C2T_MAIL_RECEIVER,										//!����ʼ�����
	MSG_C2T_MAIL_SEND_FAILED,									//!����ʧ��
	MSG_C2T_MAIL_SEND,											//!���ʼ�����
	MSG_C2T_MAIL_READ,											//!��ȡ�ʼ�
	MSG_C2T_MAIL_FAILED_MOVE_GOODS,								//!�ʼ�ȡ��Ʒʧ��
	MSG_C2T_MAIL_MOVE_GOODS_RESULT,								//!�ʼ�ȡ��Ʒ���
	MSG_C2T_MAIL_DELETE_RESULT,									//!ɾ���и����ʼ���ȷ����Ϣ
	MSG_C2T_MAIL_DELETE_OK,										//!ɾ���ʼ�
	MSG_C2T_MAIL_AGREED_PAY,									//!�������Ž�����Ϣ
	MSG_C2T_MAIL_AGREED_PRESENT,								//!ͬ������
	MSG_C2T_MAIL_GET_GOLD_RESULT,								//!����ȡ��Ǯ���
	MSG_C2T_MAIL_PRESENT_FAILED,								//����ʧ��
	MSG_C2T_MAIL_PRESENT_OK,									//������������
	MSG_C2T_MAIL_SYSMAILMSG,									//ϵͳ�ʼ�������Ϣ

	// Fox@20081107----------------------------------------------
	// ��ҹ���ϵͳ( PlayerManager )
	MSG_C2T_PM						= MSG_C2T + TMAX_SUB * 30,
	// -----------------------------------------------------
	MSG_C2T_PM_ACK,							// GSӦ���ѯ�����Ϣ


	MSG_C2T_LOGIC					= MSG_C2T + TMAX_SUB * 40,
	MSG_C2T_LOGIC_UIISOPENED,				// ָ���Ľ����Ƿ���






	//////////////////////
	//				    //
	//	 TestToClient	//
	//				    //
	//////////////////////

	//========================================================================================================
	MSG_T2C							= TMAX_TYPE * 38,			// Test -> Client
	//========================================================================================================
	MSG_T2C_MOUSE					= MSG_T2C + TMAX_SUB * 0,	// �����Ϣ

	MSG_T2C_MOUSE_MOVE,											// ����ƶ�
	MSG_T2C_MOUSE_LBUTTONDOWN,									// �������
	MSG_T2C_MOUSE_LBUTTONUP,									// ����ͷ�
	MSG_T2C_MOUSE_LBUTTONDBLCLK,								// ���˫��

	MSG_T2C_MOUSE_RBUTTONDOWN,									// �Ҽ�����
	MSG_T2C_MOUSE_RBUTTONUP,									// �Ҽ��ͷ�
	MSG_T2C_MOUSE_RBUTTONDBLCLK,								// �Ҽ�˫��
	
	MSG_T2C_MOUSE_MOUSEWHEEL,									// ����м����

	// -----------------------------------------------------
	MSG_T2C_KEY						= MSG_T2C + TMAX_SUB * 1,	// ������Ϣ
	// -----------------------------------------------------
	MSG_T2C_KEY_DOWN,											// ���̰���
	MSG_T2C_KEY_UP,												// �����ͷ�
	MSG_T2C_KEY_CHAR,											// ���̲���

	// -----------------------------------------------------
	MSG_T2C_LOGIC					= MSG_T2C + TMAX_SUB * 2,	// �߼�������Ϣ
	// -----------------------------------------------------
	MSG_T2C_LOGIC_MOVE,											// �ƶ���ĳ�������
	MSG_T2C_LOGIC_SETPOS,										// ˲�Ƶ�ĳ�������

	MSG_T2C_LOGIC_SELECTEDSHAPE,								// ѡ��ĳ������
	MSG_T2C_LOGIC_UIISOPENED,									// ����ͻ��˷���һ��ָ�������Ƿ�������Ϣ
	
	MSG_T2C_LOGIC_PICKGOODS,									// ����ͻ���ʰȡ��Ʒ

	MSG_T2C_LOGIC_CALLSCREAPT,									// ����ͻ�������������ͽű����ù���
	MSG_T2C_LOGIC_SPLITGOODS,									// ����ͻ�������������Ͳ����Ʒ��Ϣ
};