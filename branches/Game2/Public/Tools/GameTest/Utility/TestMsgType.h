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










long MsgRad[1680] = {

		//////////////////
		//				//
		//	LoginServer	//
		//				//
		//////////////////

		//========================================================================================================
		MAX_TYPE * 1,			// LoginServer --> LoginServer
		//========================================================================================================

		MSG_L2L + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_L2L_SERVER_WORLDSERVERLOST,							// LoginServer��⵽WorldServer�Ͽ�

		MSG_L2L + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_L2L_LOG_CLIENTCLOSE,								// LoginServer��⵽Client�Ͽ�

		//========================================================================================================
		 MAX_TYPE * 2,			// WorldServer --> LoginServer
		//========================================================================================================

		MSG_W2L + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_W2L_SERVER_SENDWORLDINFO,							// ����WORLD��Ϣ��LoginServer
		MSG_W2L_SERVER_SENDCDKEYLIST,							// ����ʱ����World�ϵ�Cdkeylist��LoginServer
		MSG_W2L_SERVER_CLEARCDKEYS,								// gameserver�Ͽ�,worldserver��loginserver������Ҫ�����CDKEY

		MSG_W2L_SERVER_ANSWER_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����
		MSG_W2L_SERVER_ANSWER_GsPlayerNum,						// ���ط���������

		MSG_W2L_SERVER_QUEST_RECONNECTION,

		MSG_W2L + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_W2L_LOG_ANSWER_PLAYERDATA,							// worldserver��loginserver������ϸ��Ϣ��ȡ�Ƿ�ɹ�
		MSG_W2L_LOG_ANSWER_PLAYERBASELIST,						// worldserver��loginserver������һ�����Ϣ�б�
		MSG_W2L_LOG_DELETEROLE,									// worldserver���ؽ�ɫ�Ƿ�ɹ�ɾ��
		MSG_W2L_LOG_RESTOREROLE,								// WORLDSERVER���ؽ�ɫ�Ƿ�ָ�
		MSG_W2L_LOG_CREATEROLE,									// worldserver���ؽ�ɫ�Ƿ�ɹ�����
		MSG_W2L_LOG_PLAYERQUIT,									// worldserver��loginserver�����˳���ҵ�cdkey

		MSG_W2L_LOG_SENDACCOUNTINFO,							// �����ʺŵ���Ϣ���ͻ���

		MSG_W2L + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_W2L_GM_BANPLAYER,


		//========================================================================================================
		 MAX_TYPE * 3,			// Client --> LoginServer
		//========================================================================================================

		MSG_C2L + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_C2L_LOG_QUESTCDKEY,									// �ͻ��˵�½ʱ����CDKEY
		MSG_C2L_LOG_QUEST_PLAYERBASELIST,						// �ͻ��˵�½ʱ�����ʺ��µ���һ��������б�
		MSG_C2L_LOG_QUEST_PLAYERDATA,							// ��LOGINSERVER������ϸ����
		MSG_C2L_LOG_QUESTCREATE,								// �ͻ���Ҫ����һ���µĽ�ɫ
		MSG_C2L_LOG_QUESTDELETE,								// �ͻ���Ҫ��ɾ��һ����ɫ
		MSG_C2L_LOG_QUESTRESTORE,								// CLIENT ����ָ�һ����ɫ
		MSG_C2L_LOG_QUESTEXIT,									// ����GameServer�Ľ��
		MSG_C2L_LOG_VALIDATE_MATRIX,							// �ͻ����ύ�ܱ���

		MSG_C2L_LOG_SEND_PT_TOKEN,								// �ͻ��˷���ʢ��PT��token���ݣ������½��
		MSG_C2L_LOGIN_AND_QUEST_PLAYERBASELIST,					//! �ͻ�������ֱ�ӽ���ѡ�˽��棨С�ˣ�


		//////////////////
		//				//
		//	WorldServer	//
		//				//
		//////////////////

		//========================================================================================================
		MAX_TYPE * 4,			// WorldServer --> WorldServer
		//========================================================================================================

		MSG_W2W + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_W2W_SERVER_LOGINSERVERCLOSE,						// WorldServer��⵽LoginServer�ر�
		MSG_W2W_SERVER_GAMESERVERLOST,							// WorldServer��⵽GameServer�Ͽ�
		MSG_W2W_RECONNECTLOGINSERVEROK,							// ����LoginServer�ɹ�
		MSG_W2W_SERVER_COSTSERVERCLOSE,
		MSG_W2W_RECONNECTCOSTSERVEROK,
		//MSG_W2W_SERVER_CLEAROFFLINEPLAYER,					// �����߳��ڴ��̳ɹ�����Ϣ�����������������б�

		//========================================================================================================
		MAX_TYPE * 5,			// LoginServer --> WorldServer
		//========================================================================================================

		MSG_L2W + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_L2W_LOG_QUEST_PLAYERBASELIST,						// loginserver��world������һ�����Ϣ�б�
		MSG_L2W_LOG_DELETEROLE,									// loginserver��worldserverת��ɾ����ɫ����
		MSG_L2W_LOG_RESTOREROLE,								// LOGINSERVER��worldserverת���ָ���ɫ����
		MSG_L2W_LOG_CREATEROLE,									// loginserver��worldserverת��������ɫ����
		MSG_L2W_LOG_QUEST_PLAYERDATA,							// loginserver��worldserverת����ȡ��ϸ��Ϣ����
		MSG_L2W_LOG_KICKPLAYER,                                 // loginServer��WorldServer����һ��Ҫ���߳�ĳ����ҵ���Ϣ
		MSG_L2W_LOG_REPEAT_LOGIN,								// Login Server֪ͨWorld Server��һ���������½һ�ʺŲ��滻��ԭ��ʹ�ø��ʺŵ����ӡ�
		MSG_L2W_LOG_BACKTOLOGINSTEP,							// Login Server֪ͨWorld Server�ͻ����˻ص�¼���档
		MSG_L2W_LOG_FCM_BEGIN,									// ��ҽ��������״̬������WS�ߵ����
		MSG_L2W_LOG_FCM_TIME,									// ����WS֪ͨ��һ�ʣ�����ʱ��

		MSG_L2W + MAX_SUB * 1,
		MSG_L2W_SERVER_QUEST_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����
		MSG_L2W_SERVER_QUEST_RECONNECTION,

		//========================================================================================================
		MAX_TYPE * 6,			// GameServer --> WorldServer
		//========================================================================================================

		MSG_S2W + MAX_SUB * 0,
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
		MSG_S2W_SERVER_CREATEPHGOODS,							// �������˷�����Ʒ
		MSG_S2W_SERVER_UPDATEPHGOODS,							// ���¸��˷�����Ʒ
		MSG_S2W_SERVER_DELETEPHGOODS,							// ɾ�����˷��ݸ�����Ʒ
		MSG_S2W_SERVER_SAVEPERSONALHOUSE,								// ������˷�����Ʒ
		MSG_S2W_SERVER_LOADPERSONALHOUSE,								// ��ȡ���˷�����Ʒ

		// [AHC 080809 DBSMSG] 
		MSG_S2W_SERVER_QUEST_RECONNECTION,
		MSG_S2W_SERVER_PLAYER_PERSONALHOUSEGUID,
		MSG_S2W_JOE,

		MSG_S2W + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_S2W_LOG_QUEST_PLAYERDATA,							// GameServer��WorldServer����player��ϸ����
		MSG_S2W_LOG_PLAYERQUIT,									// gameserver��worldserver�����˳���ҵ�playerid

		MSG_S2W + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_S2W_PLAYER_ADDSKILL,							
		MSG_S2W_PLAYER_DELSKILL,
		MSG_S2W_PLAYER_DELGOODS,
		MSG_S2W_PLAYER_ADDCREDIT,	//�ı���������
		MSG_S2W_PLAYER_SETLEVEL,
		MSG_S2W_PLAYER_OCCU_CHANGE,


		MSG_S2W + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_S2W_OTHER_TALK,										// �Ի���Ϣ
		MSG_S2W_OTHER_ADDINFO,							
		MSG_S2W_OTHER_GOODSLINK,                                // gameserver����һ����Ʒ������worldserver
		MSG_S2W_OTHER_VIEWGOODSLINK,				

		MSG_S2W_OTHER_CHANGENAME,								// Gsת��C�����ĸ�������Ws
		MSG_S2W_OTHER_BROADCAST,								// �㲥��Ϣ

		MSG_S2W_OTHER_TALK_WORLD,								// ���纰��
		MSG_S2W_OTHER_TALK_COUNTRY,								// ���Һ���
		MSG_S2W_OTHER_TALK_COUNTRY_UNION,						// �˹�����
		MSG_S2W_OTHER_TALK_NOTIFY,								// ����֪ͨ
		MSG_S2W_OTHER_TALK_TOP_WORLD,							// �ö����纰��
		MSG_S2W_OTHER_GETLOG_REQUEST,							// ������ֵ��ֵ
		MSG_S2W_OTHER_GoodsInfo_Request,						// ������Ʒ��Ϣ
		MSG_S2W_OTHER_GoodsInfo_Create,							// �����µ���Ʒ��Ϣ
		MSG_S2W_OTHER_UPDATEGLOBALSCTIPTVARS,					// ����ȫ�ֽű�����]
		MSG_S2W_OTHER_NEWS_TOP_REGION,							// �����ö�����
		MSG_S2W_OTHER_NEWS_TOP_WORLD,							// ȫ���ö�����
		MSG_S2W_OTHER_UPDATE_HIDE_FLAG,							// �����������״̬
		MSG_S2W_OTHER_WARNING,									// ���б���ͨ��
		MSG_S2W_OTHER_DEL_SPOUSE,								//! ��������
		MSG_S2W_OTHER_LIMIT_GOODS_REQUEST,						// gs��ws�����޹���Ʒ��ѯ

		MSG_S2W + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_S2W_DEPOT_OPEN,										// ���ֿ�

		MSG_S2W + MAX_SUB * 5,
		// ----------------------------------------------------
		MSG_S2W_GM_QUERY_PLAYERCOUNT,							//��ѯ�û�����
		MSG_S2W_GM_QUERY_PLAYERDATA,							//��ѯ�û�����
		MSG_S2W_GM_QUERY_PLAYER,								//��ѯ�û�����(GUID)
		MSG_S2W_GM_QUERY_PLAYER_P,								//�����û�����(GUID)
		MSG_S2W_GM_QUERY_PLAYER_PH,								//�����û�����(GUID)
		MSG_S2W_GM_QUERY_PLAYER_T,								//�����û�����(GUID)

		MSG_S2W_GM_ANSWER_PLAYERDATA,	
		MSG_S2W_GM_ANSWER_PLAYER,								//�����û�����(GUID)
		MSG_S2W_GM_ANSWER_PLAYER_P,								//�����û�����(GUID)
		MSG_S2W_GM_ANSWER_PLAYER_PH,							//�����û�����(GUID)
		MSG_S2W_GM_ANSWER_PLAYER_T,								//�����û�����(GUID)

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
		MSG_S2W_GM_GETPLAYER_REGION,							// ����õ�Player�ĳ�����Ϣ
		MSG_S2W_GM_ANSWERGETPLAYER_REGION,						// �ظ�����õ�Player�ĳ�����Ϣ
		MSG_S2W_GM_CHANGETPLAYER_REGION,						// ����л�Player�ĳ���
		MSG_S2W_GM_ANSWERCHANGETPLAYER_REGION,					// �ظ�����л�Player�ĳ���
		MSG_S2W_GM_QUERY_REGIONISEXIT,							// ��ѯ�����Ƿ����

		MSG_S2W + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_S2W_TEAM_QUERY_JOIN,								//! �����������
		MSG_S2W_TEAM_QUERY_JOIN_BYNAME,							//! �����������
		MSG_S2W_TEAM_ANSWER_JOIN,								//! �����뷽�ظ�
		MSG_S2W_TEAM_CHANGE_LEADER,								//! �ı�ӳ�
		MSG_S2W_TEAM_SetGoods,									//! ��Ʒ��������
		MSG_S2W_TEAM_RECRUIT,									//! ��ļ����
		MSG_S2W_TEAM_BeRecruited,								//! Ӧ��
		MSG_S2W_TEAM_CHAT,										//! ����
		MSG_S2W_TEAM_MemberAllIdioinfo,							//! ������ת�Ķ�����ϸ��Ϣ
		MSG_S2W_TEAM_MemberPos,									//! ������תλ����Ϣ

		MSG_S2W_TEAM_SwapRgnID,									//! ����ת���ƽ����鸱��ID����
		MSG_S2W_TEAM_RequestSwapRgnID,							//! ���󽻻�������ҵ����ж��鸱��ID
		MSG_S2W_TEAM_AddRgnID,									//! ���һ������ĸ���ID
		MSG_S2W_TEAM_DelRgnID,									//! ɾ��һ������ĸ���ID
		MSG_S2W_TEAM_ClearRgnID,								//! ���һ������ĸ���ID

		MSG_S2W_TEAM_GetRgnNum,									//! ��ȡһ����ҵĶ��鸱������
		MSG_S2W_TEAM_ReturnGetRgnNum,							//! ����һ����ҵĶ��鸱������
		MSG_S2W_TEAM_GetRgnGuid,								//! ��ȡһ����ҵĶ��鸱��GUID by Index
		MSG_S2W_TEAM_ReturnGetRgnGuid,							//! ����һ����ҵĶ��鸱��GUID
		MSG_S2W_TEAM_GetRgnTemplateID,							//! ��ȡһ����ҵĶ��鸱��ģ��ID by Index
		MSG_S2W_TEAM_ReturnGetRgnTemplateID,					//! ����һ����ҵĶ��鸱��ģ��ID

		MSG_S2W_TEAM_KICK_PLAYER,								//! ����
		MSG_S2W_TEAM_INLVLDIFFERENCE,							//! �ж϶�Ա���ȼ���
		MSG_S2W_TEAM_INLVLSPACE,								//! �ж����ж�Ա�ĵȼ��Ƿ�������ķ�Χ��

		MSG_S2W_TEAM_Recommend_ByName,							//! �ͻ��������Ƽ�




		MSG_S2W + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_S2W_ORGSYS_CreateFaction_Reques,					//! ����WS��������
		MSG_S2W_ORGSYS_UpgradeFaction_Reques,					//! ����WS��������
		MSG_S2W_ORGSYS_DisbandFaction_Reques,					//! ����ɾ�����
		MSG_S2W_Faction_InvitePlayer_Reques,					//! �����͹�����������
		MSG_S2W_Faction_AddMember,								//! �������һ����Ա
		MSG_S2W_ORGSYS_SetJobName_Request,						//! ���ùٽ�����
		MSG_S2W_ORGSYS_SetJobPurview_Request,					//! ���ùٽ�Ȩ��
		MSG_S2W_ORGSYS_Quit_Request,							//! ��Ա�˳�
		MSG_S2W_Faction_SetPronounce_Reques,					//! �����޸Ĺ���
		MSG_S2W_Faction_SetLeaveword_Reques,					//! �����޸ı���
		MSG_S2W_Faction_Fire_Reques,							//! ���󿪳���Ա
		MSG_S2W_ORGSYS_SetMemberJob_Request,					//! ���ó�Ա�ٽ�
		MSG_S2W_Faction_SetMemberTitle_Reques,					//! ���ó�Ա�ٽ�
		MSG_S2W_Faction_SetNewMaster_Reques,					//! ���û᳤
		MSG_S2W_Faction_CheckRequisition_Reques,				//! �����б��������
		MSG_S2W_ORGSYS_JoinFac_Request,							//! ����������
		MSG_S2W_Faction_UpLoadIcon_Reques,						//! ����ϴ�����ͼ��

		MSG_S2W_Faction_ChangeRes_Reques,						//! �޸���Դ����
		MSG_S2W_Faction_ChangeExp_Reques,						//! �޸ľ�������
		MSG_S2W_Faction_ChangeMemberContribute_Reques,			//! �޸ĳ�Ա��������
		MSG_S2W_Faction_ChangeTickets,							//	�޸ľݵ���Ʊ��
		MSG_S2W_Faction_ChangeBaseBattle,						//	�޸ľݵ�ս��ֵ
		MSG_S2W_Faction_ChangeBase,								//	�ݵ���ɾ
		MSG_S2W_Faction_ChangeBattle_Reques,					//	�޸�ս����ֵ
		MSG_S2W_Faction_ChangeArmyInvest_Reques,				//	�޸ľ���Ͷ��ֵ
		MSG_S2W_Faction_ChangeJoinBattle,						//	�޸ĳ�Ա����Ȩ
		MSG_S2W_Faction_ChangeTechLevel,						//	�޸ļ���Ƽ��ȼ�
		MSG_S2W_Faction_ChangeEnemy,							//	�޸ĵжԼ�����Ϣ	

		//�������
		MSG_S2W_QUEST_PlayerAddQuest,							//��ĳ��������һ������
		MSG_S2W_QUEST_PlayerRemoveQuest,						//��ĳ������Ƴ�һ������
		MSG_S2W_QUEST_PlayerRunScript,							//��һ����Ҵ���һ���ű�

		MSG_S2W_QUEST_PLAYERRANKS,                              // ��ѯ����

		MSG_S2W_SetFacParam_ByMasterPlayerID,					//�����л����ֵ
		MSG_S2W_SetFacParam_ByFactionID,
		MSG_S2W_SetFacParam_ByFactionName,
		MSG_S2W_ChangeFacParam_ByMasterPlayerID,				//�ı��л����ֵ
		MSG_S2W_ChangeFacParam_ByFactionID,
		MSG_S2W_ChangeFacParam_ByFactionName,

		//! ����
		MSG_S2W_UNION_Request_Create,							//! ������������
		MSG_S2W_UNION_Request_Disband,							//! ��ɢ��������
		MSG_S2W_UNION_Request_Quit,								//! �˳���������
		MSG_S2W_UNION_Request_Kick,								//! ������������
		MSG_S2W_UNION_Request_SetChairman,						//! ��λ
		MSG_S2W_UNION_Request_SetPurview,						//! ����Ȩ��
		MSG_S2W_UNION_Request_Invit,							//! ����
		MSG_S2W_UNION_Request_AnswerInvit,						//! �ش�����

		//�ͽ�������
		MSG_S2W_SHARE_QUESTLIST_UPDATE,							//���¹��������б�
		MSG_S2W_SHARE_CURRQUEST,								//���µ�ǰ����
		MSG_S2W_SHARE_DELQUEST,									//ɾ�������б���ָ������
		MSG_S2W_SHARE_SETMEMBERQUEST,							//���ö�Ա��ǰ�ѳ���ɹ��������
		// ��־
		MSG_S2W + MAX_SUB * 8,//��־
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




		//�Ʒ�
		MSG_S2W + MAX_SUB * 9,
		// -----------------------------------------------------
		MSG_S2W_COST_CHANGEREGION,
		MSG_S2W_COST_CHANGELEVEL,

		// �㿨���� s->w
		MSG_S2W + MAX_SUB * 10,
		// -----------------------------------------------------
		MSG_S2W_CARD_SELL_REQUEST,								// ��������
		//MSG_S2W_CARD_SELL_ROLLBACK,								// ���������Ϣ
		MSG_S2W_CARD_RESERVE_REQUEST,
		MSG_S2W_CARD_BUY_REQUEST,								// ��������
		MSG_S2W_CARD_CANCEL_REQUEST,					// ȡ������
		MSG_S2W_CARD_LIST_REQUEST,
		//MSG_S2W_CARD_SALECOSTS_REQUEST,

		MSG_S2W_CARD_CHARGE_REQUEST,


		// ����
		MSG_S2W + MAX_SUB * 11,
		// -----------------------------------------------------
		MSG_S2W_COUNTRY_CHANGE,									// �ı����
		MSG_S2W_COUNTRY_POWER_CHANGE,
		MSG_S2W_COUNTRY_TECHLEVEL_CHANGE,
		MSG_S2W_COUNTRY_KING_CHANGE,

		// Region 
		MSG_S2W + MAX_SUB * 12,
		// -----------------------------------------------------
		MSG_S2W_RGN_CHANGE,									// 

		MSG_S2W_RGN_CREATE,
		MSG_S2W_RGN_CREATE_QUEST,  // ��GS0��GS1�ϴ�����GS1����GS0����Ϣ

		MSG_S2W_RGN_DELETE,
		MSG_S2W_RGN_DELETE_QUEST,  // ��GS0��GS1��ɾ����GS1����GS0����Ϣ

		MSG_S2W_RGN_VAR_ADD,
		MSG_S2W_RGN_VAR_DEL,
		MSG_S2W_RGN_VAR_SET,
		MSG_S2W_RGN_VAR_GET,

		// �������
		MSG_S2W_RGN_VAR_ADD_ANSWER,
		MSG_S2W_RGN_VAR_DEL_ANSWER,
		MSG_S2W_RGN_VAR_SET_ANSWER,
		MSG_S2W_RGN_VAR_GET_ANSWER,

		MSG_S2W_RGN_VIEWRGNVAR,
		MSG_S2W_RGN_VIEWRGNNUM,
		MSG_S2W_RGN_VIEWRGNDATA,
		MSG_S2W_RGN_VIEWRGNAVLID, // �鿴�����Ƿ����

		MSG_S2W_RGN_GETTEMPLATEID,	//����ָ�������ɣĵĸ�������ͼ�ɣ�
		MSG_S2W_RGN_GETDUPTYPE,		//����ָ�������ɣĵĸ���������
		MSG_S2W_RGN_GETDUPRGNONWER, //����ָ�������ɣĵĸ����������ߣɣġ�

		MSG_S2W_RGN_VIEWPLAYERPID, // �鿴��ҵĸ��˸���
		MSG_S2W_RGN_VIEWPLAYERPHID, // �鿴��ҵĸ��˷��ݸ���
		MSG_S2W_RGN_VIEWPLAYERTID, // �鿴��ҵ���Ӹ���
		MSG_S2W_RGN_VIEWGSPLAYERNUM,// �鿴ָ��GS��ĳ��������
		MSG_S2W_RGN_ANSWERVIEWGSPLAYERNUM,// ��ӦWS��MSG_W2S_RGN_QUESTVIEWGSPLAYERNUM�鿴ָ��GS��ĳ��������
		MSG_S2W_RGN_QUESTPLAYERRGNLIST,// ��ѯWS�ĸ����Ƿ����
		MSG_S2W_RGN_ANSWERWSQUESTPLAYERRGNLIST,// ��ӦWS��ѯ��Ҹ���list
		MSG_S2W_RGN_ANSWERGSUPDATEPLAYERRGNLIST,// ��Ӧ����GS��Ҹ���list

		MSG_S2W_RGN_UPDATE_PLAYER_TEAM_RGNGUID, //! ����һ����ҵĶ��鸱������
		MSG_S2W_RGN_CAMP_PLAYER_NUM_QUEST,	// ��ѯָ��������ָ����Ӫ�������
		MSG_S2W_RGN_CAMP_PLAYER_NUM_ANSWER,	// ��Ӧָ��������ָ����Ӫ�������

		// DBEntity Operation 
		MSG_S2W + MAX_SUB * 13, 
		// -----------------------------------------------------
		MSG_S2W_OPER_PLAYER,		// �������
		MSG_S2W_OPER_UNION,			// ��������

		// DBEntity Operation 
		MSG_S2W + MAX_SUB * 14,
		// -----------------------------------------------------
		// �����������(������¼���Ժ���ϸ����),ʹ��һ��32λ��־����ͬ������
		// 0x00000001:property,0x00000002:EQUIPMENT,
		// 0x00000004:ORIGINALPACK,0x00000008:SUBPACK
		// 0x00000008:VARLIST,0x00000010:QUESTLIST,
		// 0x00000020:SKILLLIST,0x00000040:STATELIST
		// ���ʹ��:
		// 0x0x00000006:ORIGINALPACK+EQUIPMENT...
		MSG_S2W_PLAYER_DETAIL_SAVE_PROPERTY,


		MSG_S2W + MAX_SUB*15,
		MSG_S2W_REGION_DETAIL_SAVE_PROPERTY,


		//! ����ϵͳ
		MSG_S2W + MAX_SUB * 16,
		// -----------------------------------------------------
		MSG_S2W_LINKMAN_ADD,					//! �����ϵ��
		MSG_S2W_LINKMAN_DEL,					//! ɾ����ϵ��

		MSG_S2W_LINKMAN_MAP_CHANGE,				//! ֪ͨWS��ҵ�ͼ�ı�
		MSG_S2W_LINKMAN_DATA,					//! һ����ҵ���ϵ������

		MSG_S2W_LINKMAN_SYSTEM_REQUEST_ADD,		//! ϵͳ�����ϵ������
		MSG_S2W_LINKMAN_PALYER_REQUEST_ADD,		//! ��������ϵ������
		MSG_S2W_LINKMAN_PALYER_RE_ADD,			//! ��һظ������ϵ������
		MSG_S2W_LINKMAN_PALYER_REQUEST_MOVE,	//! ����ƶ���ϵ������
		MSG_S2W_LINKMAN_PALYER_REQUEST_DEL,		//! ���ɾ����ϵ������
		//!�ʼ�ϵͳ
		MSG_S2W + MAX_SUB * 17,
		// -----------------------------------------------------
		MSG_S2W_MAIL_SEND,						//! ����
		MSG_S2W_MAIL_RECEIVE,					//! ����
		MSG_S2W_MAIL_OPERA_GOODS,				//! �ʼ�����Ʒ����
		MSG_S2W_MAIL_OPERA_GOLD,				//! �ʼ��еĽ�Ҵ���
		MSG_S2W_MAIL_DELETE,					//! ɾ���ʼ�
		MSG_S2W_MAIL_PAY,						//! ��������
		MSG_S2W_MAIL_REJECT,					//! ����
		MSG_S2W_MAIL_READ,						//! ����
		MSG_S2W_MAIL_SEND_RECEIVER,				//! ���Ÿ�ĳ�����
		//MSG_S2W_MAIL_PRESENT_RECEIVER,			//! �������ʼ�����


		// Fox@20081107----------------------------------------------
		// ��ҹ���ϵͳ( PlayerManager )
		 MSG_S2W + MAX_SUB * 18,
		// -----------------------------------------------------
		MSG_S2W_PM_ADD,							// GS�������������Ϣ
		MSG_S2W_PM_CHG,							// GS����ˢ�������Ϣ
		MSG_S2W_PM_REQ,							// GS�����ѯ�����Ϣ

		/// ��ҵϵͳ��Ϣ
		MSG_S2W + MAX_SUB * 19,

		MSG_S2W_BUSINESS_SAVE,	/// GS��������������ݿ�����

		MSG_S2W + MAX_SUB * 20,
		MSG_S2W_PLAYERPHASE,

		// Auction House 
		MSG_S2W + MAX_SUB * 21,
		MSG_S2W_AH_OPER,

		// 
		MSG_S2W + MAX_SUB * 22,
		MSG_S2W_QUEST_OPER,

		//////////////////
		//				//
		//	GameServer	//
		//				//
		//////////////////

		//========================================================================================================
		MAX_TYPE * 7,			// GameServer --> GameServer
		//========================================================================================================

		MSG_S2S + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_S2S_SERVER_WORLDSERVERCLOSE,						// GameServer��⵽WorldServer�ر�
		MSG_S2S_SERVER_RECONNECDTWORLDSERVER,					//��������WorldServer

		MSG_S2S + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_S2S_LOG_PLAYER_ONLOST,								// �������ڽ��ܵ��ͻ��˶�����Ϣʱ���Լ���һ����Ϣ	

		MSG_S2S + MAX_SUB * 2,
		// ----------------------------------------------------
		//MSG_S2S_TEAM_TEAMMATE_MIGRATE_IN,						//ת��
		//MSG_S2S_TEAM_TEAMMATE_MIGRATE_OUT,						//ת��
		//MSG_S2S_TEAM_TEAMMATE_LOST,								//��Ա���

		//========================================================================================================
		MAX_TYPE * 8,			// WorldServer --> GameServer
		//========================================================================================================

		MSG_W2S + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_W2S_SERVER_SENDSETUP,								// WorldServer����setup������Ϣ��GameServer
		MSG_W2S_SERVER_CHANGEREGION,							// �����л�������
		MSG_W2S_SERVER_SAVENOTIFY,								// ���̵�֪ͨ
		MSG_W2S_SERVER_ADDINFO,									// ����World Server��GAME SERVER���ؽ��
		MSG_W2S_SERVER_CHANGE_SCRIPT_VARIABLE,					// �ı�ű�������ֵ
		MSG_W2S_SERVER_SYNC_MURDERER_COUNT,						// ͬ��ɱ����
		MSG_W2S_SERVER_BUSINESS_UPDATE,
		MSG_W2S_SERVER_BUSINESS_BUYLIMIT_CLEAR,

		MSG_W2S_SERVER_QUEST_COLLECT_PLAYERDATA,				// ����GameServer��������ҵ����ݷ��͹���
		MSG_W2S_SERVER_QUEST_PING,								// ping����������Ϣ��Ҳ�Ǽ��������Ƿ�����
		MSG_W2S_SERVER_CREATEMONSTER,							// ��������
		MSG_W2S_SERVER_CREATEPHGOODS,							// �������˷��ݸ�����Ʒ
		MSG_W2S_SERVER_UPDATEPHGOODS,							// ���¸��˷�����Ʒ
		MSG_W2S_SERVER_DELETEPHGOODS,							// ɾ�����˷��ݸ�����Ʒ
		MSG_W2S_SERVER_SAVEPERSONALHOUSE,								// ������˷�����Ʒ
		MSG_W2S_SERVER_LOADPERSONALHOUSE,								// ��ȡ���˷�����Ʒ

		// [AHC DBSMSG]
		MSG_W2S_SERVER_QUEST_RECONNECTION,
		MSG_W2S_SERVER_PLAYER_PERSONALHOUSEGUID,               // ��ѯ��ҵĸ��˷�������

		MSG_W2S + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_W2S_LOG_ANSWER_PLAYERDATA,							// WorldServer��GameServer����player��ϸ����
		MSG_W2S_LOG_KICKPLAYER,									//worldserver��gameserver����һ��Ҫ���߳�ĳ����ҵ���Ϣ
		MSG_W2S_LOG_REPEAT_LOGIN,								//World Server֪ͨGame Server��һ���������½һ�ʺŲ��滻��ԭ��ʹ�ø��ʺŵ����ӡ�
		MSG_W2S_LOG_FCM_TIME,									// WS��GS֪ͨ��ҷ�����ʱ��

		MSG_W2S + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_W2S_OTHER_TALK,										// �Ի���Ϣ
		MSG_W2S_OTHER_FACTIONTALK,								// �л�����
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
		MSG_W2S_OTHER_KICKPLAYER,								// ������ϵͳ����
		MSG_W2S_OTHER_UPDATEGLOBALSCTIPTVARS,					// ����ȫ�ֽű�����


		MSG_W2S_OTHER_CHANGENAME,								// Ws�������ͽ����Gs
		MSG_W2S_OTHER_TALK_WORLD,								// ���纰��
		MSG_W2S_OTHER_TALK_COUNTRY,								// ���Һ���
		MSG_W2S_OTHER_TALK_COUNTRY_UNION,						// �˹�����
		MSG_W2S_OTHER_TALK_NOTIFY,								// ����֪ͨ
		MSG_W2S_OTHER_TALK_TOP_WORLD,							// �ö����纰��	
		MSG_W2S_OTHER_GETLOG_RESPONSE,							// ���ظ�gs��ֵ��־
		MSG_W2S_OTHER_GoodsInfo_Re,								// ������Ʒ��Ϣ
		MSG_W2S_OTHER_NEWS_TOP_REGION,							// ��������
		MSG_W2S_OTHER_NEWS_TOP_WORLD,							// ��������

		MSG_W2S_OTHER_WARNING,									// ���б���ͨ��
		MSG_W2S_OTHER_NEW_DAY,									// ֪ͨGS�µ�һ�쵽��
		MSG_W2S_OTHER_DEL_SPOUSE,								//! ���֪ͨ
		MSG_W2S_OTHER_LIMIT_GOODS_RESPONSE,						// ws�����޹���Ʒ��ѯ
		MSG_W2S_OTHER_LIMIT_GOODS_UPDATA,							//WS֪ͨ����GS����������Ϣ



	MSG_W2S + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_W2S_DEPOT_OPEN,

		MSG_W2S + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_W2S_GM_ANSWER_PLAYERCOUNT,							//�����û�����
		MSG_W2S_GM_ANSWER_PLAYERDATA,							//�����û�����
		MSG_W2S_GM_ANSWER_PLAYER,								//�����û�����(GUID)
		MSG_W2S_GM_ANSWER_PLAYER_P,								//�����û�����(GUID)
		MSG_W2S_GM_ANSWER_PLAYER_PH,							//�����û�����(GUID)
		MSG_W2S_GM_ANSWER_PLAYER_T,								//�����û�����(GUID)

		MSG_W2S_GM_QUERY_PLAYERDATA,
		MSG_W2S_GM_QUERY_PLAYER,								//�����û�����(GUID)
		MSG_W2S_GM_QUERY_PLAYER_P,								//�����û�����(GUID)
		MSG_W2S_GM_QUERY_PLAYER_PH,								//�����û�����(GUID)
		MSG_W2S_GM_QUERY_PLAYER_T,								//�����û�����(GUID)

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
		MSG_W2S_GM_GETPLAYER_REGION,
		MSG_W2S_GM_ANSWER_GETPLAYER_REGION,						// ���ػ����ҳ������
		MSG_W2S_GM_CHANGEPLAYER_REGION,
		MSG_W2S_GM_ANSWER_CHANGEPLAYER_REGION,					// ���ػ����ҳ������
		MSG_W2S_GM_ANSWER_REGIONISEXIT,							// ���س���ѯ�����Ƿ����

		MSG_W2S + MAX_SUB * 5,
		// ----------------------------------------------------
		MSG_W2S_TEAM_InitTeamSet,								//��ʼ����������

		MSG_W2S_TEAM_ToPlayerInfo,								//! ת����������Ϣ
		MSG_W2S_TEAM_QUERY_JOIN,								//! ת��������Ϣ
		MSG_W2S_TEAM_PlayerJoin,								//! ��Ҽ������
		MSG_W2S_TEAM_MastterChange,								//! �ӳ����
		MSG_W2S_TEAM_MemberStateChange,							//! ��Ա����״̬�ı�
		MSG_W2S_TEAM_ServerChange,								//! ��Ա�л�������
		MSG_W2S_TEAM_MemberExit,								//! ��Ա�˳�
		MSG_W2S_TEAM_Disband,									//! ��ɢ
		MSG_W2S_TEAM_GoodsSetChange,							//! ��Ʒ���������޸�
		MSG_W2S_TEAM_RECRUIT,									//! ��ļ״̬�ı�
		MSG_W2S_TEAM_MemberAllIdioinfo,							//! ת����Ա��ϸ��Ϣ
		MSG_W2S_TEAM_MemberPos,									//! ת����Աλ����Ϣ
		MSG_W2S_TEAM_LEVELCHANGE,								//! �㲥��Ա�ȼ��仯
		MSG_W2S_TEAM_OCCUCHANGE,								//! �㲥��Աְҵ�仯

		MSG_W2S_TEAM_SetupInit,									//! ��ʼ����������
		MSG_W2S_TEAM_DataInit,									//! ��ʼ����������

		MSG_W2S_TEAM_SwapRgnID,									//! ת�� ���鸱��ID���ݣ�Դ�����Ѱ�������Ϣ�У�
		MSG_W2S_TEAM_GetSrcSwapRgnID,							//! ת�� ��ȡ�������鸱��ID���ݵ�Դ����
		MSG_W2S_TEAM_AddRgnID,									//! ת�� �����ҵĶ��鸱��ID
		MSG_W2S_TEAM_DelRgnID,									//! ת�� ɾ����ҵĶ��鸱��ID
		MSG_W2S_TEAM_ClearRgnID,								//! ת�� �����ҵĶ��鸱��ID

		MSG_W2S_TEAM_GetRgnNum,									//! ת�� ��ȡһ����ҵĶ��鸱������
		MSG_W2S_TEAM_ReturnGetRgnNum,							//! ת�� ����һ����ҵĶ��鸱������
		MSG_W2S_TEAM_GetRgnGuid,								//! ת�� ��ȡһ����ҵĶ��鸱��GUID by Index
		MSG_W2S_TEAM_ReturnGetRgnGuid,							//! ת�� ����һ����ҵĶ��鸱��GUID
		MSG_W2S_TEAM_GetRgnTemplateID,							//! ת�� ��ȡһ����ҵĶ��鸱��ģ��ID by Index
		MSG_W2S_TEAM_ReturnGetRgnTemplateID,					//! ת�� ����һ����ҵĶ��鸱��ģ��ID

		MSG_W2S_TEAM_CREATE,									// ���齨��
		MSG_W2S_TEAM_CHAT,										// ����

		MSG_W2S_TEAM_INLVLDIFFERENCE,
		MSG_W2S_TEAM_INLVLSPACE,

		MSG_W2S_TEAM_Recommend_ByName,							//! ת���Ƽ����


	MSG_W2S + MAX_SUB * 6,
		// ----------------------------------------------------

		MSG_W2S_ORGSYS_InitGsFaction,							//! ��ʼ��GS�л���Ϣ
		MSG_W2S_ORGSYS_InitPlayerFacSys,						//! ֪ͨGS����ҷ��ͳ�ʼ��������Ϣ
		MSG_W2S_ORGSYS_CreateFaction_Re,						//! �ظ�GS�������
		MSG_W2S_ORGSYS_DelFaction_Notify,						//! ɾ���л�֪ͨ
		MSG_W2S_Faction_InvitePlayer_Re,						//! ����ظ�
		MSG_W2S_Faction_AddMember_Notify,						//! ����»�Ա֪ͨ
		MSG_W2S_ORGSYS_SetJobName_Notify,						//! �޸�ְ������֪ͨ
		MSG_W2S_ORGSYS_SetJobPurview_Notify,					//! �޸�ְ��Ȩ��֪ͨ
		MSG_W2S_ORGSYS_Quit_Notify,								//! ��Ա�˳�֪ͨ
		MSG_W2S_Faction_EditPronounce_Notify,					//! �༭����֪ͨ
		MSG_W2S_Faction_EditLeaveword_Notify,					//! �༭����֪ͨ
		MSG_W2S_ORGSYS_Fire_Notify,								//! ������Ա֪ͨ
		MSG_W2S_ORGSYS_SetMemJobLvl_Notify,						//! ���ó�Աְ��
		MSG_W2S_Faction_SetMemberTitle_Notify,					//! ���ó�Ա�ƺ�֪ͨ
		MSG_W2S_Faction_SetMaster_Notify,						//! �����»᳤֪ͨ
		MSG_W2S_Faction_OnlineChange_Notify,					//! ��Ա����״̬�ı�
		MSG_W2S_Faction_MemberMap_Notify,						//! ��Ա��ͼ֪ͨ
		MSG_W2S_Faction_MemberLevel_Notify,						//! ��Ա�ȼ�֪ͨ
		MSG_W2S_Faction_MemberHide_Notify,						//! ��Ա����֪ͨ
		MSG_W2S_Faction_MemberOccu_Notify,						//! ��Աְҵ֪ͨ
		MSG_W2S_Faction_Applicant_Notify,						//! ����������֪ͨ
		MSG_W2S_Faction_Join_Re,								//! �ظ�����������󣨴���Żظ���
		MSG_W2S_Faction_LvlChange_Notify,						//! ����ȼ��ı�֪ͨ
		MSG_W2S_Faction_IconChange_Notify,						//! ����ͼ�������޸�֪ͨ

		MSG_W2S_Faction_ChangeRes_Notify,						//! �޸���Դ֪ͨ
		MSG_W2S_Faction_ChangeExp_Notify,						//! �޸ľ���֪ͨ
		MSG_W2S_Faction_ChangeMemberContribute_Notify,			//! �޸ĳ�Ա����֪ͨ
		MSG_W2S_Faction_ChangeRes_Re,							//! �޸���Դ�ظ�
		MSG_W2S_Faction_ChangeExp_Re,							//! �޸ľ���ظ�
		MSG_W2S_Faction_ChangeMemberContribute_Re,				//! �޸ĳ�Ա���׻ظ�
		MSG_W2S_Faction_ChangeTickets_Re,						//	�޸ľݵ���Ʊ��
		MSG_W2S_Faction_ChangeBaseBattle_re,					//	�޸ľݵ�ս��ֵ
		MSG_W2S_Faction_ChangeBase_Re,							//	�ݵ���ɾ
		MSG_W2S_Faction_ChangeBattle_Re,						//	�޸�ս����ֵ
		MSG_W2S_Faction_ChangeArmyInvest_Re,					//	�޸ľ���Ͷ��ֵ
		MSG_W2S_Faction_ChangeJoinBattle_Re,					//	�޸ĳ�Ա����Ȩ
		MSG_W2S_Faction_ChangeTechLevel_Re,						//	�޸ļ���Ƽ��ȼ�
		MSG_W2S_Faction_ChangeEnemy_Re,							//	�޸ĵжԼ�����Ϣ

		MSG_W2S_Faction_BoonRun_Notify,							//! ֪ͨGSִ�и���

		MSG_W2S_QUEST_PlayerAddQuest,							//��ĳ��������һ������
		MSG_W2S_QUEST_PlayerRemoveQuest,						//��ĳ������Ƴ�һ������
		MSG_W2S_QUEST_PlayerRunScript,							//��ĳ���������һ���ű�

		MSG_W2S_QUEST_PLAYERRANKS,                              // ���а��ѯ

		MSG_W2S_Union_InitGs,									//! ��ʼ��GS�л���Ϣ
		MSG_W2S_Union_Create_Notify,							//! �ظ�GS�������
		MSG_W2S_Union_Disband_Notify,							//! ��ɢ
		MSG_W2S_Union_Quit_Notify,								//! �˳�
		MSG_W2S_Union_Kick_Notify,								//! ����
		MSG_S2W_Union_SetChairman_Notify,						//! ��λ
		MSG_S2W_Union_SetPurview_Notify,						//! ����Ȩ��
		MSG_S2W_Union_Invit_Re,									//! �ظ�������
		MSG_S2W_Union_TransferInvit,							//! ת������
		MSG_S2W_Union_TransferAnswer,							//! ת���ظ�
		MSG_S2W_Union_NewMemberJoin,							//! �³�Ա����

		//�ͽ�����
		MSG_W2S_SHARE_QUESTLIST_UPDATE,						//���¹��������б�
		MSG_W2S_SHARE_CURRQUEST,							//���µ�ǰ����
		MSG_W2S_SHARE_SETMEMBERQUEST,		
		////�Ʒ�
		//MSG_W2S_COST					= MSG_W2S + MAX_SUB * 7,  
		////---------------------------------------------------------------------------
		//MSG_W2S_COST_NOTENOUGHCOST,
		//MSG_W2S_COST_INFORMATION,

		// �㿨����  w->s
		 MSG_W2S + MAX_SUB * 8,  
		//---------------------------------------------------------------------------
		MSG_W2S_CARD_SELL_RESPONSE,								// ��Ӧ��������
		MSG_W2S_CARD_RESERVE_RESPONSE,
		MSG_W2S_CARD_BUY_RESPONSE,								// ��Ӧ��������
		MSG_W2S_CARD_CANCEL_RESPONSE,
		MSG_W2S_CARD_LIST_RESPONSE,
		//MSG_W2S_CARD_SALECOSTS_RESPONSE,

		MSG_W2S_CARD_CHARGE_RESPONSE,


		// ����
		MSG_W2S + MAX_SUB * 9,  
		//---------------------------------------------------------------------------
		MSG_W2S_COUNTRY_CHANGE,									// ���Ĺ���
		MSG_W2S_COUNTRY_POWER_CHANGE,
		MSG_W2S_COUNTRY_TECHLEVEL_CHANGE,
		MSG_W2S_COUNTRY_KING_CHANGE,

		//! ����ϵͳ
		MSG_W2S + MAX_SUB * 10,  
		//---------------------------------------------------------------------------
		MSG_W2S_LINKMAN_PLAYER_INITDATA,			//! һ����ҵĳ�ʼ������
		MSG_W2S_LINKMAN_PLAYER_RELEASEDATA,			//!	����������֪ͨ
		MSG_W2S_LINKMAN_PLAYER_LOGIN,				//! һ����ϵ�˵�½
		MSG_W2S_LINKMAN_PLAYER_LOGOUT,				//! һ����ϵ������
		MSG_W2S_LINKMAN_PLAYER_INFO_CHANGE,			//! һ����ҵ���Ϣ�ı�
		MSG_W2S_LINKMAN_PLAYER_DELSTATE_CHANGE,		//! һ����ҵ�ɾ��״̬�ı�

		MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_NOTIFY,	//! ������������ϵ��֪ͨ
		MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE,		//! ������������ϵ�˻ظ�
		MSG_W2S_LINKMAN_ADD_NOTIFY,					//! �����ϵ��֪ͨ
		MSG_W2S_LINKMAN_MOVE_NOTIFY,				//! �ƶ���ϵ��֪ͨ
		MSG_W2S_LINKMAN_DEL_NOTIFY,					//! ɾ����ϵ��֪ͨ

		MSG_W2S_LEAVEWORD_SAVE_RESULT,				//! ���Խ��
		MSG_W2S_LEAVEWORD_POP,						//! ȡ��������
		//! �ʼ�ϵͳ
		MSG_W2S + MAX_SUB * 11,
		MSG_W2S_MAIL_SEND_FAILED,					//! ����ʧ��
		MSG_W2S_MAIL_SEND,							//! ����
		MSG_W2S_MAIL_OPERA_GOODS,					//! ��Ʒ����
		MSG_W2S_MAIL_OPERA_GOLD,					//! ��Ǯ����
		MSG_W2S_MAIL_DELETE,						//! ɾ���ż�
		MSG_W2S_MAIL_RECEIVE,						//! ����
		MSG_W2S_MAIL_PAY,							//! ��������
		MSG_W2S_MAIL_SEND_RESULT,					//! ���Ž��
		MSG_W2S_MAIL_SEND_SYS,						//! Ⱥ��ϵͳ�ʼ�
		MSG_W2S_MAIL_CHANGE_TO_SERVER,  			//! �ʼ�ת��

		// Fox@20081107----------------------------------------------
		// ��ҹ���ϵͳ( PlayerManager )
		MSG_W2S + MAX_SUB * 12,
		// -----------------------------------------------------
		MSG_W2S_PM_CHG,							// WS��Ӧˢ�������Ϣ
		MSG_W2S_PM_ACK,							// WS��Ӧ��ѯ�����Ϣ

		// Region 
		MSG_W2S + MAX_SUB * 13,
		// -----------------------------------------------------
		MSG_W2S_RGN_CHANGE,									// 

		MSG_W2S_RGN_CREATE,
		MSG_W2S_RGN_CREATE_ANSWER,  // ��GS0��GS1�ϴ�����WS��GS1���͵���Ϣ


		MSG_W2S_RGN_DELETE,
		MSG_W2S_RGN_DELETE_ANSWER,  // ��GS0��GS1�ϴ�����WS��GS1���͵���Ϣ

		MSG_W2S_RGN_VAR_ADD,
		MSG_W2S_RGN_VAR_DEL,
		MSG_W2S_RGN_VAR_SET,
		MSG_W2S_RGN_VAR_GET,

		// �������
		MSG_W2S_RGN_VAR_ADD_QUEST,
		MSG_W2S_RGN_VAR_DEL_QUEST,
		MSG_W2S_RGN_VAR_SET_QUEST,
		MSG_W2S_RGN_VAR_GET_QUEST,

		MSG_W2S_RGN_VIEWRGNVAR,
		MSG_W2S_RGN_VIEWRGNNUM,
		MSG_W2S_RGN_VIEWRGNDATA,
		MSG_W2S_RGN_VIEWTOTALDUPRGNNUM,
		MSG_W2S_RGN_VIEWRGNAVLID, // �鿴�����Ƿ����

		MSG_W2S_RGN_GETTEMPLATEID,	//����ָ�������ɣĵĸ�������ͼ�ɣ�
		MSG_W2S_RGN_GETDUPTYPE,		//����ָ�������ɣĵĸ���������
		MSG_W2S_RGN_GETDUPRGNONWER, //����ָ�������ɣĵĸ����������ߣɣġ�

		MSG_W2S_RGN_VIEWPLAYERPID, // �鿴��ҵĸ��˸���
		MSG_W2S_RGN_VIEWPLAYERPHID, // �鿴��ҵĸ��˷��ݸ���
		MSG_W2S_RGN_VIEWPLAYERTID, // �鿴��ҵ���Ӹ���
		MSG_W2S_RGN_VIEWGSPLAYERNUM,// �鿴ָ��GS��ĳ��������
		MSG_W2S_RGN_QUESTVIEWGSPLAYERNUM, // WS��GS���Ͳ�ѯ��Ϣ
		MSG_W2S_RGN_ANSWERPLAYERRGNLIST,// ��ӦGS�ĸ����Ƿ����
		MSG_W2S_RGN_QUESTGSQUESTPLAYERRGNLIST,// ��ѯGS��Ҹ���list
		MSG_W2S_RGN_QUESTGSUPDATEPLAYERRGNLIST,// ��ѯ����GS��Ҹ���list

		MSG_W2S_RGN_UPDATE_PLAYER_TEAM_RGNGUID,//! ����һ����ҵĶ��鸱������
		MSG_W2S_RGN_CAMP_PLAYER_NUM_QUEST,	// ��ѯָ��������ָ����Ӫ�������
		MSG_W2S_RGN_CAMP_PLAYER_NUM_ANSWER,	// ��Ӧָ��������ָ����Ӫ�������

		// AntiCheat 
		MSG_W2S + MAX_SUB * 14,
		MSG_W2S_AC_UPDATEQUESTIONS,

		MSG_W2S + MAX_SUB * 15,
		MSG_W2S_PLAYERPHASE,

		// Auction House
		MSG_W2S + MAX_SUB * 16,
		MSG_W2S_AH_OPER,

		MSG_W2S + MAX_SUB * 17,
		MSG_W2S_QUEST_OPER,

		//========================================================================================================
		MAX_TYPE * 9,			// Client --> GameServer
		//========================================================================================================

		MSG_C2S + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_C2S_LOG_PLAYEREXITGAME,								// ������GameServer
		MSG_C2S_LOG_PLAYERENTER,								// ���������Ϸ������

		MSG_C2S + MAX_SUB * 1,
		// ----------------------------------------------------
		MSG_C2S_RGN_ENTER,										// ����������������볡��
		MSG_C2S_RGN_EXIT,										// ��������뿪����

		MSG_C2S + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_C2S_SHAPE_CHANGEDIR,								// �ı䷽��
		MSG_C2S_SHAPE_SETPOS,									// ˲��
		MSG_C2S_SHAPE_MOVE,										// �ƶ���Ϣ
		MSG_C2S_SHAPE_STOPMOVE,									// ֹͣ�ƶ���Ϣ
		MSG_C2S_SHAPE_CHANGEMOVEMODE,							// �ƶ�ģʽ�ı�
		MSG_C2S_SHAPE_QUESTINFO,								// ���������������shape����Ϣ
		MSG_C2S_SHAPE_EMOTION,									// ���鶯��
		MSG_C2S_SHAPE_CHANGELINEDRGN,							// �л����߳���

		MSG_C2S_SHAPE_SETSELECTEDSHAPE,							// �趨��ǰѡ���Ŀ��
		MSG_C2S_SHAPE_GETTSELECTEDSHAPE,						// ��õ�ǰѡ���Ŀ��

		MSG_C2S + MAX_SUB * 3,
		// ----------------------------------------------------
		//MSG_C2S_PLAYER_ADDPOINT,								// �ӵ�
		MSG_C2S_PLAYER_QUEST_RELIVE,							// ���󸴻�
		MSG_C2S_PLAYER_TOUCHNPC,								// ����NPC
		MSG_C2S_PLAYER_USEITEM,									// client��gameserver��֤һ�����ĵ��ߵ�ʹ��
		MSG_C2S_PLAYER_PKSWITCH,								// �л�PK����
		MSG_C2S_PLAYER_HELPREQUEST,								// ����������
		MSG_C2S_PLAYER_UpdateShowFashion,						//�����Ƿ���ʾͷ��,����,���׵�����
		//MSG_C2S_PLAYER_CHANGE_QUEST_TIMEREQUEST,
		MSG_C2S_PLAYER_SKILL_RELIVE,							//
		MSG_C2S_PLAYER_SKILL_CANCEL_RELIVE,						//ȡ�����ܸ���
		MSG_C2S_PLAYER_CHANGE_TITLE_TYPE,						// ���������ĳƺ���ʾ����




		MSG_C2S + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_C2S_OTHER_TALK,										// �Ի���Ϣ
		MSG_C2S_OTHER_SENDTALKBOXSELECT,						// �ͻ��˷���TalkBox��ѡ��
		MSG_C2S_OTHER_VIEWGOODSLINK,							// �ͻ�������鿴һ����Ʒ������
		MSG_C2S_OTHER_CONNECTGAMESERVER,						// ����GameServer�Ľ��
		MSG_C2S_OTHER_CHANGENAME,								// ��������������
		MSG_C2S_OTHER_QUESTNPCLIST,								// ����NPC�б�
		MSG_C2S_OTHER_TALK_WORLD,								// ���纰��
		MSG_C2S_OTHER_TALK_COUNTRY_UNION,						// �˹�����
		MSG_C2S_OTHER_TALK_COUNTRY,								// ���Һ���
		MSG_C2S_OTHER_TALK_TOP_WORLD,							// �ö����纰��
		MSG_S2C_OTHER_HITHELPHINT,								// ���ð�����ʾ
		MSG_S2C_OTHER_HITINTERFACEHINT,							// ���ý�����ʾ
		MSG_C2S_OTHER_FACEACTION,								// ���ͱ��鶯����Ϣ
		MSG_C2S_OTHER_GoodsInfo_Request,						// ������Ʒ��Ϣ
		MSG_C2S_OTHER_Message_Busyness,							// �ͻ�����Ϣ��æ

		MSG_C2S_OTHER + MAX_SUB - 7,	// ����������
		MSG_C2S_OTHER_UPDATEVARVALUE_QUEST,						// �����޸Ľű�����ֵ
		MSG_C2S_OTHER_UPDATENAMEANSWER,							//! �ͻ����޸����ַ�����Ϣ
		MSG_C2S_OTHER_QUITTOSELECT,                             //  �����˻ص�ѡ�˽���
		MSG_C2S_OTHER_PING,										// �ͻ��˶�ʱ����ping������


		MSG_C2S + MAX_SUB * 5,
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
		MSG_C2S_CLOSEGOODSUPGRADE,
		MSG_C2S_GOODSUPGRADE_PUT,								//����һ����Ʒ����������
		MSG_C2S_GOODSUPGRADE_FETCH,								//������������ȡ��һ����Ʒ
		MSG_C2S_GOODSUPGRADE_CHANGE,							//�����������н���һ����Ʒ
		MSG_C2S_GOODSUPGRADE_UPGRADE,                           //client��������һ����Ʒ									
		MSG_C2S_ENCHASE_OPEN_HOLE,								//��Ƕ�����еĿ�����Ϣ
		MSG_C2S_ENCHASE_CLOSE_HOLE,								//��Ƕ�����еĶ¿���Ϣ
		MSG_C2S_ENCHASE_ENCHASECARD,							//�⿨��Ϣ
		MSG_C2S_ENCHASE_REMOVECARD,								//����Ϣ
		MSG_C2S_ENCHASE_ENDSESSION,								// �����Ự
		MSG_C2S_GOODS_CONTAINER,								// ��Ʒ��������

		MSG_C2S_GOODS_LOCK,										// ��Ʒ����Э��

		//! GoodsExManageӦ��
		////////////////////////////////////////////////////////////////////////
		MSG_C2S_GOODS_CLOSE_EX_WND,								//! ��Ʒ��չ���ڹر�֪ͨ
		//! �ֽ���Ʒ
		MSG_C2S_GOODS_DISASSEMBLE_PREVIEW,						//! Ԥ���ֽ���
		MSG_C2S_GOODS_DISASSEMBLE_DO,							//! ����ֽ���Ʒ
		//! �ϳ���Ʒ
		MSG_C2S_GOODS_SYNTHESIZE_PREVIEW,						//! Ԥ���ϳɽ��
		MSG_C2S_GOODS_SYNTHESIZE_DO,							//! ����ϳ���Ʒ
		//! ��������
		//! ʹ��ͳһ��Ʒ��Ϣ��MSG_C2S_PLAYER_USEITEM

		//���󶨶���
		MSG_C2S_GOODS_BIND,										//! ������
		MSG_C2S_GOODS_FROST,									//! ��������

		//! ��Ʒ����
		MSG_C2S_GOODS_UPGRADE_VIEW,								//! Ԥ������
		MSG_C2S_GOODS_UPGRADE_DO,								//! ִ������
		//! ��Ʒħ��
		MSG_C2S_GOODS_PLUG_EVIL_DO,								//! ִ��ħ��
		MSG_C2S_GOODS_RESET_ELEMENT_TYPE,						//! ����ħ��Ԫ������
		MSG_C2S_GOODS_RESET_XIANG_XING,							//! ����ħ������

		//! �ֿ�
		MSG_C2S_DEPOT_INPUT_PWD,								//! �ͻ�������ֿ�����
		MSG_C2S_DEPOT_REQUEST_CLOSE,							//! Ҫ��رղֿ�
		MSG_C2S_DEPOT_REQUEST_CHANGE_PWD,						//! Ҫ���޸�����
		MSG_C2S_DEPOT_REQUEST_BUY_SUBPACK,						//! Ҫ�����ӱ���

		//! ͨ����Ʒ��������
		MSG_C2S_GOODS_AllPurpose_RequestPush,					//! ���������Ʒ
		MSG_C2S_GOODS_AllPurpose_RequestPop,					//! ����ȡ����Ʒ
		MSG_C2S_GOODS_AllPurpose_RequestClose,					//! ����ر�����
		MSG_C2S_GOODS_AllPurpose_RequestFinish,					//! ����ִ����������


		MSG_C2S + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_C2S_SHOP_BUY,										//player��������
		MSG_C2S_SHOP_SELL,										//player��һ����Ʒ���̵�
		MSG_C2S_SHOP_REPAIR,									//���̵�������һ����Ʒ
		MSG_C2S_SHOP_REPAIRALL, 								//�̵�������ȫ����Ʒ
		MSG_C2S_SHOP_REQUESTREPAIRALLPRICE,					//��ѯ����ȫ����Ʒ�ļ۸�
		MSG_C2S_SHOP_REQUESTREPAIRSINGLEPRICE,				//��ѯ��������Ʒ�ļ۸�
		MSG_C2S_SHOP_DURUPDATE,								//�;ø���
		MSG_C2S_SHOP_CLOSE,									//�رո����̵�

		MSG_C2S_NPCSHOP_OPEN,									//��NPC�̵꣨��ʱʹ�ã�
		MSG_C2S_BUSINESS_BUY,
		MSG_C2S_BUSINESS_SELL,
		MSG_C2S_BUSINESS_INVEST,	//��ҵó�׵�Ͷ��
		MSG_C2S_SHOP_BUYBACK,									// �ع���Ʒ

		MSG_C2S_ESSENCE_SHOP_OPEN,								// �����̵�
		MSG_C2S_FBUSINESS_OPER,		// �������̲���

		MSG_C2S + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_C2S_DEPOT_PUTDOWN,
		MSG_C2S_DEPOT_FETCH,
		MSG_C2S_DEPOT_CHANGE,
		MSG_C2S_DEPOT_SAVEMONEY,
		MSG_C2S_DEPOT_FETCHMONEY,
		MSG_C2S_DEPOT_CHECKPASSWORD,
		MSG_C2S_DEPOT_CHANGEPASSWORD,
		MSG_C2S_DEPOT_CLOSE,

		MSG_C2S + MAX_SUB * 8,
		// ----------------------------------------------------
		MSG_C2S_TEAM_QUERY_JOIN,									//�������
		MSG_C2S_TEAM_QUERY_JOIN_BYNAME,								//ͨ����������������
		MSG_C2S_TEAM_ANSWER_JOIN,									//�ظ��������
		MSG_C2S_TEAM_CHANGE_LEADER,									//�ı�ӳ�
		MSG_C2S_TEAM_KICK_PLAYER,									//����(�ӳ������������ˣ���Ա�������Լ�)
		MSG_C2S_TEAM_SetGoods,										//�޸���Ʒ��������
		MSG_C2S_TEAM_CHAT,											//����
		MSG_C2S_TEAM_RECRUIT,										//��ļ����
		MSG_C2S_TEAM_BeRecruited,									//Ӧ��

		MSG_C2S_TEAM_TEAMATE_LEAVE,									//�뿪����
		MSG_C2S_TEAM_TEAMATE_JOIN,									
		MSG_C2S_TEAM_DESTORY,
		MSG_C2S_TEAM_CHANGE_ALLOCATION_SCHEME,

		MSG_C2S_TEAM_Recommend_ByName,								//! �Ƽ����


		MSG_C2S + MAX_SUB * 9,
		// ----------------------------------------------------
		MSG_C2S_SKILL_USE,										// ʹ�ü���
		MSG_C2S_SKILL_USE_END,									// ʹ�ü��ܽ�������Щ�����ɿ���ݼ���ʱ����
		MSG_C2S_SKILL_STUDY,									// ѧϰ����
		MSG_C2S_SKILL_STUDY_BEGIN,								// ����������ʹ򿪼���ѧϰ��ʦҳ��
		MSG_C2S_DOCCUSKILL_USE,									// ʹ�ø�ְҵ����
		MSG_C2S_OCCUSKILL_STUDY_BEGIN,							// ����������ʹ򿪸�ְҵ����ѧϰҳ��
		MSG_C2S_OCCUSKILL_STUDY,								// �����������ѧϰ��ְҵ������Ϣ
		MSG_C2S_OCCUSKILL_DELETE,								// �����������ɾ��������Ŀ��Ϣ

		MSG_C2S_BUFF_END,										// �����������ɾ��״̬��Ϣ
		MSG_C2S_OCCUSKILL_NPCFACTURE_BEGIN,						// �յ��ͻ�NPC�����б���Ϣ
		MSG_C2S_OCCUSKILL_NPCFACTURE,							// �յ��ͻ���������Ϣ

		MSG_C2S_SKILL_ADD_PET,
		MSG_C2S_SKILL_DEL_PET,
		MSG_C2S_SKILL_USE_PET,
		MSG_C2S_SKILL_END_PET,
		MSG_C2S_SKILL_SAVE,

		MSG_C2S + MAX_SUB * 10,
		// ----------------------------------------------------

		// �л�Э��-------------------------------------------------
		//! �᳤����
		MSG_C2S_ORGSYS_CreateFaction_Reques,					//! �����л�����
		MSG_C2S_ORGSYS_DisbandFaction_Request,					//! ��ɢ�л�����	
		MSG_C2S_ORGSYS_SetPlacard_Request,						//! �޸Ĺ�������
		MSG_C2S_ORGSYS_SetLeaveword_Request,					//! �޸ı�������
		MSG_C2S_ORGSYS_SetJobName_Request,						//! �޸Ĺٽ���������
		MSG_C2S_ORGSYS_SetJobPurview_Request,					//! �޸Ĺٽ�Ȩ������
		MSG_C2S_ORGSYS_SetMemberJob_Request,					//! ���ó�Ա�ٽ�����
		MSG_C2S_Faction_SetMemberTitle_Request,					//! ���ó�Ա�ƺ�����
		MSG_C2S_Faction_SetChairman_Request,					//! ��λ����
		MSG_C2S_Faction_FindFactionList,                        //! ������ǰ�������еİ����Ϣ

		//! ��Ա����
		MSG_C2S_ORGSYS_Quit_Request,							//! �˳��л�����
		MSG_C2S_ORGSYS_Kick_Request,							//! �߳��л�����
		MSG_C2S_Faction_InvitePlayer_Request,					//! ������Ҽ�������
		MSG_C2S_Faction_InvitePlayer_Answer,					//! �ش�����
		MSG_C2S_ORGSYS_CheckRequisition_Request,				//! �����������
		MSG_C2S_Faction_UpLoadIcon_Request,						//! �ϴ�����ͼ��

		MSG_C2S_ORGSYS_JoinFac_Request,							//! �����ᣨ���뵽�����б�����

		//! ��������
		MSG_C2S_FacData_Request,								//! �л���������
		MSG_C2S_Faction_IconData_Request,						//! ���󵥸�����ͼ������
		MSG_C2S_Faction_AllIconData_Request,					//! ����ͼ�����������ݲ�ʹ���ˣ�

		MSG_C2S_ORGSYS_CloseFacBoard_Notifye,					//! ��ҹرչ������֪ͨ���¼ӣ�

		MSG_C2S_FACTION_Upgrade_Request,						//! ������������
		MSG_C2S_Faction_ChangeJoinBattle,						//	�޸ĳ�Ա����Ȩ
		// End���л�------------------------------------------------


		// ����Э��-------------------------------------------------
		MSG_C2S_UNION_Request_Creation,					//! ������������
		MSG_C2S_UNION_Request_Disband,					//! ��ɢ��������
		MSG_C2S_UNION_Request_Quit,						//! �л��˳���������
		MSG_C2S_UNION_Request_Kick,						//! �������л��߳���������
		MSG_C2S_UNION_Request_SetChairman,				//! ����ת������
		MSG_C2S_UNION_Request_SetRights,				//! ��������᳤Ȩ������
		MSG_C2S_UNION_Request_Invite,					//! ���� �����л�᳤ �������˵ķ�������
		MSG_C2S_UNION_Request_Anwser_Invite,			//! �л�᳤ ���ͻظ�����������
		// End������------------------------------------------------

		// ս��Э��-------------------------------------------------
		//MSG_S2C_ORGSYS_WAR_Script_Start,				//! �����л�ս���ű����󣬴�ս������

		//MSG_C2S_ORGSYS_WAR_Request_Start,				//! ��ս�� ����ս������ָ�������л����
		//MSG_C2S_ORGSYS_WAR_Response_Start,				//! ��ս����ӭս�� ���Ե����� ���ͻظ� �Ƿ����ս��
		//MSG_S2C_ORGSYS_WAR_Request_Start,				//! ��ս����ӭս�� ���Ե����� �յ���ս�� ����ս��
		//MSG_S2C_ORGSYS_WAR_Response_Start,				//! ��ս�������� �յ�ӭս���ظ��Ƿ����ս��

		//MSG_S2C_ORGSYS_WAR_Notify_Join,					//! ����ս�������г�Ա�õ� ��ս֪ͨ

		//MSG_S2C_ORGSYS_WAR_Notify_Over,					//! �����л�ս�� ֪ͨ���д�ս�۳�Ա
		// End��ս��------------------------------------------------



		MSG_C2S_QUEST_Accept,									//�ύ�������
		MSG_C2S_QUEST_Complete,									//�ύ�������
		MSG_C2S_QUEST_Disband,									//�ύ��������
		MSG_C2S_QUEST_SHARE,                                    //�ύ��������

		MSG_C2S_MERCQUEST_Update,								//ˢ��ٸ������

		MSG_C2S_QUEST_PLAYERRANKS,								//������ҵ����а�

		//�ͽ�������Ϣ
		MSG_C2S_SHARE_QUESTLIST_UPDATE,							//�յ��Ƽ��ͽ�������Ϣ						

		// �����̵�
		 MSG_C2S + MAX_SUB * 11,
		// ----------------------------------------------------
		MSG_C2S_PLAYERSHOP_CREATE,								//������󿪵�
		MSG_C2S_PLAYERSHOP_OPENFORBUSINESS,						//�������ʼ��̯
		MSG_C2S_PLAYERSHOP_CLOSEDOWN,							//��ҹرո����̵�
		MSG_C2S_PLAYERSHOP_LOOKGOODS,							//�鿴��Ʒ
		MSG_C2S_PLAYERSHOP_QUIT,								//�������뿪�����̵�
		MSG_C2S_PLAYERSHOP_BUYITEM,								//������Ʒ
		MSG_C2S_PLAYERSHOP_CLOSE,								//�رո����̵�

		MSG_C2S_PLAYERSHOP_MSGBOARDSET,							//�������԰���Ϣ(���������̵����ơ������Ƽ�)
		MSG_C2S_PLAYERSHOP_MSGBOARDCHAT,						//�ͻ��˷��͵����԰��е�������Ϣ


		// ����
		 MSG_C2S + MAX_SUB * 12,
		// ----------------------------------------------------
		MSG_C2S_CONTAINER_OBJECT_MOVE,
		MSG_C2S_CONTAINER_OBJECT_UNLOCK,

		// ����
		MSG_C2S + MAX_SUB * 13,
		// ----------------------------------------------------
		MSG_C2S_PET_MOVE,										// �ƶ�
		MSG_C2S_PET_STOP,										// ֹͣ
		MSG_C2S_PET_CHANGE_MODE,								// �ı���ﵱǰģʽ
		MSG_C2S_PET_CHANGE_ACTION,								// �ı���ﵱǰ����
		MSG_C2S_PET_CHANGE_SPEED,								// �ı��ٶ�
		MSG_C2S_PET_HOUSE,										// լ�س���

		MSG_C2S_PET_ADDSKILL,									// ������Ӽ��ܣ�������Ƕ��
		MSG_C2S_PET_UPGRADE,									// ��������
		MSG_C2S_PET_UPGRADE_DLEV,								// �����������еȼ�
		MSG_C2S_PET_CHANGE_STATE,								// ����ı�״̬
		MSG_C2S_PET_CHANG_NAME,									// �����޸�����
		MSG_C2S_PET_CANCEL,										// ɾ������
		MSG_C2S_PET_SET_FOCUS,									// ���ý������

		// �㿨���� c->s
		MSG_C2S + MAX_SUB * 14,
		// -----------------------------------------------------
		MSG_C2S_CARD_SELL_REQUEST,								// �ֿ���������۵㿨
		//MSG_C2S_CARD_RESERVE_REQUEST,							// �ֿ���������㿨
		MSG_C2S_CARD_BUY_REQUEST,								// �ֿ���������㿨
		MSG_C2S_CARD_CANCEL_REQUEST,							// ȡ�����۵㿨
		MSG_C2S_CARD_LIST_REQUEST,								//�����������ǰ���۵ĵ㿨�б�
		MSG_C2S_CARD_SALECOSTS_REQUEST,							//�����������ǰ���۵ĵ㿨������ 

		//�㿨��ֵ
		MSG_C2S_CARD_CHARGE_REQUEST,							// �����ֵ

		// ����
		MSG_C2S + MAX_SUB * 15,
		// -----------------------------------------------------
		//MSG_C2S_COUNTRY_INFO_GET,								// ��ȡ���й�����Ϣ

		// ��ֵ�̵�
		MSG_C2S + MAX_SUB * 16,
		// ----------------------------------------------------
		MSG_C2S_INCREMENTSHOP_OPEN,
		MSG_C2S_INCREMENTSHOP_BUY,										//player��������
		//MSG_C2S_INCREMENTSHOP_SELL,										//player��һ����Ʒ���̵�
		//MSG_C2S_INCREMENTSHOP_REPAIR,									//���̵�������һ����Ʒ
		//MSG_C2S_INCREMENTSHOP_REPAIRALL, 								//�̵�������ȫ����Ʒ
		//MSG_C2S_INCREMENTSHOP_DURUPDATE,									//�;ø���
		MSG_C2S_INCREMENTSHOP_CLOSE,									//�رո����̵�

		MSG_C2S_INCREMENTSHOP_AFFICHE_REQUEST,							// ���洰�ڿ�

		MSG_C2S_INCREMENTSHOP_REPAY,									// �������

		MSG_C2S_INCREMENTSHOP_GETLOG_REQUEST,							// ������ֵ��־
		//MSG_C2S_GETLOG_RESPONSE,
		MSG_C2S + MAX_SUB * 17,
		MSG_C2S_STATE_END,										//״̬����

		// ����ϵͳ
		 MSG_C2S + MAX_SUB * 18,
		// A��Դ�����ͻ���-------------------------------------
		// B���������ͻ���-------------------------------------
		MSG_C2S_LINKMAN_ADD,										// A �� S ���������ϵ�� B
		MSG_C2S_LINKMAN_DEL,										// A �� S ����ɾ����ϵ�� B
		MSG_C2S_LINKMAN_MOVE,										// A �� S �����ƶ���ϵ�� B

		MSG_C2S_LINKMAN_RESPONSE_ADD,								// B �� S ��Ӧ A ���������
		// ����ϵͳ end----------------------------------------

		// С����
		MSG_C2S + MAX_SUB * 19,
		// ----------------------------------------------------
		MSG_C2S_SPRITE_AutoSupply_OnOff_Request,					//! �Զ�����ϵͳ�����ܿ�������
		MSG_C2S_SPRITE_AutoSupply_ChangeSetup_Request,				//! �Զ�����ϵͳ���޸�һ��������������


		MSG_C2S_SaveNewSetupData_Request,							//! ��ұ����µ���Ϸ����
		MSG_C2S_Calendar_Request,                                   //! ������ճ̱�
		MSG_C2S_JanaweBox_Request,                                  //! ���������άħ��
		MSG_C2S_Answer_Request,                                     //! �ʴ��뽱��

		// ----------------------------------------------------
		//�ʼ�ϵͳ
		MSG_C2S + MAX_SUB * 20,
		MSG_C2S_MAIL_SEND,											//! ����
		MSG_C2S_MAIL_RECEIVER,										//! ����
		MSG_C2S_MAIL_READ,											//�����ʼ�
		MSG_C2S_MAIL_GET_GOODS,										//! ���ʼ���ȡ��Ʒ������
		MSG_C2S_MAIL_GET_GOLDS,										//! ���ʼ���ȡ���
		MSG_C2S_MAIL_DELETE,										//! ɾ���ʼ�
		MSG_C2S_MAIL_REJECT,										//! �����ʼ�
		MSG_C2S_MAIL_AGREED_PAY,									//! ͬ�⸶��
		//MSG_C2S_MAIL_AGREED_PRESENT,								//! ͬ������

		// Fox@20081107----------------------------------------------
		// ��ҹ���ϵͳ( PlayerManager )
		MSG_C2S + MAX_SUB * 21,
		// -----------------------------------------------------
		MSG_C2S_PM_REQ,							// CLIENT�����ѯ�����Ϣ

		/// Expter@20090527
		///======================================================================================================
		 MSG_C2S + MAX_SUB * 22,
		MSG_C2S_AC_ANSWER,

		// �������
		MSG_C2S + MAX_SUB * 23,
		MSG_C2S_BOUNS_INFO,					// ��ȡ������Ϣ
		MSG_C2S_BOUNS_GET,					// ȡ����Ʒ
		MSG_C2S_BOUNS_CLOSE,				// �ر��콱����


		// Auction House
		MSG_C2S + MAX_SUB * 24,
		MSG_C2S_AH_OPER,

		MSG_C2S + MAX_SUB*25,
		MSG_C2S_PHASE_OPER,
		MSG_C2S_FOLLOW_Leader,              // ����ӳ�

		// new Quest module
		 MSG_C2S + MAX_SUB * 26,
		MSG_C2S_QUEST_OPER,

		/// ��ҽ���
		MSG_C2S + MAX_SUB * 27,

		MSG_C2S_PLAYERTRADE_REQUEST,							//��������
		MSG_C2S_PLAYERTRADE_RESPONSE,							//�ͻ���ͬ���ܾ���������
		MSG_C2S_PLAYERTRADE_LOCK,								//��������״̬
		MSG_C2S_PLAYERTRADE_CHANGEMONEY,						//�ͻ������������Ǯ
		MSG_C2S_PLAYERTRADE_WAIT,							//�ͻ���֪ͨ���뽻�׵ȴ�״̬
		MSG_C2S_PLAYERTRADE_CANCEL,								//�ͻ�������ȡ������
		MSG_C2S_PLAYERTRADE_TRADING,									// ���ȷ����㽻��ʱ����

		//////////////////
		//				//
		//	GameClient	//
		//				//
		//////////////////

		//========================================================================================================
		MAX_TYPE * 10,			// Client --> Client
		//========================================================================================================

		MSG_C2C + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_C2C_LOG_SERVERLOST,									// CLIENT�ڽ��ܵ�Server������Ϣʱ���Լ���һ����Ϣ

		//========================================================================================================
		MAX_TYPE * 11,		// LoginServer --> Client
		//========================================================================================================

		 MSG_L2C + MAX_SUB * 0,
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
		MSG_L2C_LOG_WAITDATALOAD,								// ֪ͨ�ͻ������ڶ�ȡ��ɫ������Ϣ
		MSG_L2C_LOG_FROZEN_CDKEY,								// �������Թ����˺Ŷ���
		MSG_L2C_LOG_FCM_TIME,									// ֪ͨ��ҷ�����ʣ��ʱ�䣨�룩
		MSG_L2C_LOG_SECURITY_VALIDATE,							// ֪ͨ�ͻ��˽��а�ȫ��Ϣ����
		MSG_L2C_LOG_SECURITY_VALIDATINT,						// ֪ͨ�ͻ��ˣ���������������������ܱ�����Ϣ�����Ե�����

		//========================================================================================================
		 MAX_TYPE * 12,		// GameServer --> Client
		//========================================================================================================

		MSG_S2C + MAX_SUB * 0,
		// ----------------------------------------------------
		MSG_S2C_LOG_ANSWER_PLAYERDATA,							// GameServer��GameClient����player��ϸ����
		MSG_S2C_LOG_GAMESERVERTIME,								// ����Server��ʱ��
		MSG_S2C_LOG_SEQUENCESTRING,								// ��֤��Ϣ�Ϸ��Ե��ַ�������

		MSG_S2C + MAX_SUB * 1,
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
		MSG_S2C_RGN_CHANGENAME,									// �ı�����
		MSG_S2C_RGN_UPDATEEFFECT,								// ˢ����Ч
		//!MSG_S2C_RGN_CLEARDUPRGNPLAYER,							// ����������Ϣ������Ϣ��ΪMSG_S2C_OTHER_TIME_INFO_BOX��
		MSG_S2C_RGN_PLAY3DSOUND,                                // ����3D����
		MSG_S2C_RGN_PLAYMUSIC,                                  // ��������
		MSG_S2C_RGN_STOPMUSIC,                                  // ֹͣ��������
		MSG_S2C_RGN_STRONGPOINT_STATE_CHANGE,					// ������ͨ�泡���������ĳ���ݵ㱻�ı�

		//[071127 AHC]
		MSG_S2C_RGN_ANSWER_CREATE,
		MSG_S2C_RGN_ANSWER_DELETE,

		MSG_S2C + MAX_SUB * 2,
		// ----------------------------------------------------
		MSG_S2C_SHAPE_CHANGEDIR,								// �ı䷽��
		MSG_S2C_SHAPE_CHECKPOS,									// ����Ч��
		MSG_S2C_SHAPE_SETPOS,									// ˲��
		MSG_S2C_SHAPE_FORCEMOVE,								// ǿ���ƶ�
		MSG_S2C_SHAPE_MOVE,										// �ظ��ƶ���Ϣ
		MSG_S2C_SHAPE_STOPMOVE,									// ֹͣ�ƶ���Ϣ
		MSG_S2C_SHAPE_CHANGEMOVEMODE,							// �ƶ�ģʽ�ı�
		MSG_S2C_SHAPE_MOVETILE,									// ������������Ϣ�ƶ�һ��
		MSG_S2C_SHAPE_CHANGESTATE,								// �ı�״̬
		MSG_S2C_SHAPE_ATK_LOCK,									// ָʾ����Ŀ��
		MSG_S2C_SHAPE_ATK_BEGIN,								// ������֪ͨ�ͻ��˿�ʼ����
		MSG_S2C_SHAPE_ATK_DIED,									// ����
		MSG_S2C_SHAPE_ATK_REJECT,								// ����ָ��ܾ�
		MSG_S2C_SHAPE_FIGHTSTATE,								// ֪ͨclient������˳�ս��״̬
		MSG_S2C_SHAPE_SINSTATE,									// ֪ͨclient������˳�����״̬
		MSG_S2C_SHAPE_Refresh,									// ˢ�½���״̬ 
		MSG_S2C_SHAPE_STIFFEN,									// ��ֱ״̬
		MSG_S2C_SHAPE_EMOTION,									// ��ֱ״̬

		MSG_S2C_SHAPE_ATK_DAMAGE,								//��ͨ������Ϣ
		MSG_S2C_SHAPE_ATK_BREAK,								// ������
		MSG_S2C_SHAPE_ATK_FULLMISS,								// ����
		MSG_S2C_SHAPE_ATK_PARRY,								// �м�
		MSG_S2C_SHAPE_ATK_BLOCK,								// ��
		MSG_S2C_SHAPE_ATK_RESIST,								// �ֿ�
		MSG_S2C_SHAPE_ATK_REBOUND, 							    // ����
		MSG_S2C_SHAPE_ATK_SUCK,									// ����
		MSG_S2C_SHAPE_RELIVE,									// ����һ�����
		MSG_S2C_SHAPE_CHANGE_PROPERTIES,						// ���¿��ƶ����������
		MSG_S2C_SHAPE_ATK_LOSE,									// ��ʧ
		MSG_S2C_SHAPE_CHANGE_FIRSTATTACKER,						// �ı��һ�������Լ�����
		MSG_S2C_SHAPE_LEAVEFIGHT,								// ����ս��
		MSG_S2C_SHAPE_SLIPPOS,									// ����λ��
		MSG_S2C_SHAPE_LEAVEFIGHTEND,							// ����ս������
		//MSG_S2C_SHAPE_ATK_EFFECT,								// ��Ч����
		MSG_S2C_SHAPE_ATK_ADDBLOOD,								// ��Ѫ
		MSG_S2C_SHAPE_GETTSELECTEDSHAPE,						// ��õ�ǰѡ���Ŀ��


		MSG_S2C + MAX_SUB * 3,
		// ----------------------------------------------------
		MSG_S2C_PLAYER_RESUME,									// �ָ�HP��MP��YP������
		MSG_S2C_PLAYER_ADDPOINT,								// �ӵ�
		MSG_S2C_PLAYER_ANSWER_RELIVE,							// �ָ�����
		MSG_S2C_PLAYER_ADDEXP,									// ���Ӿ���ֵ
		MSG_S2C_PLAYER_LEVELUP,									// ����
		MSG_S2C_PLAYER_ADDOCCUEXP,								// ����ְҵ����ֵ
		MSG_S2C_PLAYER_OCCULEVELUP,								// ְҵ�ȼ�����
		MSG_S2C_PLAYER_ADDSP,									// ���Ӿ���ֵ
		MSG_S2C_PLAYER_CREDIT_UPDATE,							//��������
		MSG_S2C_PLAYER_HITLEVELUPGRADE,							// ����������
		MSG_S2C_PLAYER_SYNC_HITS,								// ͬ��������
		MSG_S2C_PLAYER_SETLEVEL,
		MSG_S2C_PLAYER_USEITEM,									// gameserver��ͻ��˷������ĵ��ߵ�ʹ�ý��
		MSG_S2C_PLAYER_CHANGEMAXATK,							// �����Ե���������󹥻���
		MSG_S2C_PLAYER_CHANGEATKSPEED,							// �����Ե������������ٶ�
		MSG_S2C_PLAYER_CHANGEDEF,								// �����Ե���������������
		MSG_S2C_PLAYER_CHANGEELEMENTMODIFY,						// �����Ե��߸ı似�ܹ���ϵ��
		MSG_S2C_PLAYER_SYNC_PKCOUNT,							// ͬ��ɱ����
		MSG_S2C_PLAYER_HELPRESPONSE,							// ��Ӧ��Ұ���
		MSG_S2C_PLAYER_ADDSKILL,								// ��Ӽ���
		MSG_S2C_PLAYER_DELSKILL,								// ɾ������
		MSG_S2C_PLAYER_SETATTACKRANGE,				
		MSG_S2C_PLAYER_CHANGE_EQUIPMENT,
		MSG_S2C_PLAYER_CHANGE_PROPERTIES,						//��Ҹı�����
		MSG_S2C_PLAYER_UpdateShowFashion,					//��������Ƿ���ʾͷ����Ϣ
		MSG_S2C_PLAYER_AUTOMOVE,								//�Զ��ƶ���ָ���ĵ�
		MSG_S2C_PLAYER_CHANGE_CONTRIBUTE,						// ���ҹ��׸ı�
		MSG_S2C_PLAYER_FORCE_TARGET,
		MSG_S2C_PLAYER_COLLECTION_LEVELUP,								// ��Ҳɼ�����
		MSG_S2C_PLAYER_FACTURE_LEVELUP,									// �����������
		MSG_S2C_PLAYER_RELIVE_RESULT,									// ����ʧ��
		MSG_S2C_PLAYER_BUYRELIVEITEM_RESULT,							// ���ؼ��ԭ�ػ���߽��
		MSG_S2C_PLAYER_SKILL_RELIVE,									// ���ܸ���
		MSG_S2C_PLAYER_UPDATE_TITLE_TYPE,								// ��Ҹ��³ƺ�
		MSG_S2C_PLAYER_COLLECTION_ADDEXP,								// ��Ҳɼ���������
		MSG_S2C_PLAYER_FACTURE_ADDEXP,									// ���������������
		MSG_S2C_PLAYER_ACTIVE_CHANGE,									// ��һ�����ı�
		MSG_S2C_PLAYER_SHOW_SPOUSENAME,									// ֪ͨ�ͻ����Ƿ���ʾ��ż����
		MSG_S2C_PLAYER_ELEMENT_PROPERTY_CHANGE,							// ���Ԫ�����Ըı�

		//MSG_S2C_MONSTER					= MSG_S2C + MAX_SUB * 19,

		MSG_S2C + MAX_SUB * 4,
		// ----------------------------------------------------
		MSG_S2C_OTHER_TALK,										// �Ի���Ϣ
		MSG_S2C_OTHER_FACTIONTALK,								// �л�����
		MSG_S2C_OTHER_BROADCAST,								// �㲥
		MSG_S2C_OTHER_AddTopInfo,								// ��Ӷ�����ʾ��Ϣ
		MSG_S2C_OTHER_OPENTALKBOX,								// �ÿͻ��˵���TalkBox
		MSG_S2C_OTHER_OPEN_PIC_TALKBOX,							//! �ÿͻ��˵������Զ���ͼƬ��TalkBox
		MSG_S2C_OTHER_OPENPIXYTALKBOX,							// �ÿͻ��˵���PixyTalkBox
		MSG_S2C_OTHER_ADDINFO,									// ����ҶԻ������һ����Ϣ
		MSG_S2C_OTHER_ADDINFO_BYINDEX,							// ����ҶԻ������һ����Ϣ(�����������)
		MSG_S2C_OTHER_ADDSYSTEMINFO,							// ����Ļ�Ϸ�����һ����Ϣ
		MSG_S2C_OTHER_GAMEMESSAGE,								// ������Ϣ�Ի���
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
		MSG_S2C_OTHER_TALK_COUNTRY_UNION,						// �˹�����
		MSG_S2C_OTHER_TALK_NOTIFY,								// ����֪ͨ
		MSG_S2C_OTHER_TALK_TOP_WORLD,							// �ö����纰��	
		MSG_S2C_OTHER_TRANSFORCAMERA,							// ת�������
		MSG_S2C_OTHER_OPERAINTERFACE,							// ��������
		MSG_S2C_OTHER_SETHELPHINT,								// ���ð�����ʾ
		MSG_S2C_OTHER_SETINTERFACEHINT,							// ���ý�����ʾ
		MSG_S2C_OTHER_OPENSTORYTALKBOX,							// �ÿͻ��˵���StoryTalkBox
		MSG_S2C_OTHER_EFFECT,									// �ÿͻ��˲���һ����Ч����
		MSG_S2C_OTHER_EXPATC,									// ��������	
		MSG_S2C_OTHER_GoodsInfo_Re,								// ��Ʒ��Ϣ�ظ�
		MSG_S2C_OTHER_Message_Busyness,							// ���ؿͻ��˷�æ��Ϣ
		MSG_S2C_OTHER_PLAY_VIDEO,								// ������Ƶ
		MSG_S2C_OTHER_STOP_VIDEO,								// �ر���Ƶ
		MSG_S2C_OTHER_PackGlint,								// ������˸
		MSG_S2C_OTHER_REPEATLOGIN,								// �˺��ظ���¼
		MSG_S2C_OTHER_UPDATEVARVALUEOPEN,						// ���޸Ľű�����ֵ�Ĵ���
		MSG_S2C_OTHER_SHOW_STRING,								//! �ڿͻ����Ҳ���ʾһ���ַ���
		MSG_S2C_OTHER_NEWS_TOP_WORLD,							// ��������
		MSG_S2C_OTHER_ADDINFO_AND_GOODSINFO,					//! һ������Ʒ��Ϣ����Ϣ
		MSG_S2C_OTHER_UPDATENAMEOPEN,							//! �ͻ����޸����ֶԻ��򵯳�
		MSG_S2C_OTHER_QUITTOSELECTANSEWR,                       // ���󷵻�ѡ�˽���Ļظ�
		MSG_S2C_OTHER_UPDATE_HIDE_FLAG,							// �������״̬�ñ䣬ͨ����Χ���
		MSG_S2C_OTHER_BILLURL,									//֪ͨ�Ʒ�URL
		MSG_S2C_OTHER_TIME_INFO_BOX,							//! ֪ͨ�ͻ�����ʾһ������ʱ����Ϣ��
		MSG_S2C_OTHER_WARNING,									// ��ұ�������Ӫ��ҹ���ͨ��
		MSG_S2C_OTHER_OPEN_RELIVEWINDOW,						//�򿪸������


		MSG_S2C + MAX_SUB * 5,
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
		MSG_S2C_EQUIP_REFINE_UPDATE,							//���ȸ���
		MSG_S2C_DELETEEQUIP,									//ɾ��װ��
		MSG_S2C_DELETEBAGGOODS,									//ɾ��������Ʒ
		MSG_S2C_DELETEGOODS,									
		MSG_S2C_DELETEALLGOODS,						
		MSG_S2C_VIEWEQUIP,                                      
		MSG_S2C_OPENGOODSUPGRADE,								//gameserver֪ͨ��Ҵ���Ʒ��������
		MSG_S2C_OPENGOODSENCHASE,
		MSG_S2C_ENCHASE_OPEN_HOLE,								//������Ϣ
		MSG_S2C_ENCHASE_CLOSE_HOLE,								//�¿���Ϣ
		MSG_S2C_ENCHASE_ENCHASECARD,							//�⿨��Ϣ
		MSG_S2C_ENCHASE_REMOVECARD,								//����Ϣ
		MSG_S2C_ENCHASE_ARRANGE,								//��С����ҳ����Ϣ

		MSG_S2C_CLOSEGOODSUPGRADE,
		MSG_S2C_GOODSUPGRADE_PUT,					
		MSG_S2C_GOODSUPGRADE_FETCH,					
		MSG_S2C_GOODSUPGRADE_CHANGE,				
		MSG_S2C_GOODSUPGRADE_UPGRADE,							//gameserver��������һ����Ʒ�Ľ��
		MSG_S2C_GOODS_UPDATEPROPERTY,							//������Ʒ����
		MSG_S2C_GOODS_OPEN_CONTAINER,							// ����Ʒ����
		MSG_S2C_SUIT_ACTIVE,									//��װ����

		MSG_S2C_SET_YUANBAO,  //����Ԫ��
		MSG_S2C_SET_TICKET,	  //���õ�ȯ

		//! GoodsExManageӦ��
		////////////////////////////////////////////////////////////////////////
		MSG_S2C_GOODS_OPEN_WINDOWS,								//������Ʒ�������ڣ�ָ�����ڵ����ͣ�
		MSG_S2C_GOODS_CLOSE_WINDOWS,							//���ر�������Ʒ��������
		//! �ֽ���Ʒ
		MSG_S2C_GOODS_RE_DISASSEMBLE_PREVIEW,					//! ����Ԥ�����
		MSG_S2C_GOODS_RE_DISASSEMBLE_DO,						//! ���طֽ���Ʒ�ɰ�
		//! �ϳ���Ʒ
		MSG_S2C_GOODS_RE_SYNTHESIZE_PREVIEW,					//! ����Ԥ�����
		MSG_S2C_GOODS_RE_SYNTHESIZE_DO,							//! ���غϳɳɰ�
		//! ��������
		MSG_S2C_GOODS_RE_REINFORCE_POWER,						//! ������������
		MSG_S2C_GOODS_POWER_WARN,								//! ��������ʣ��֪ͨ
		//! �󶨶���
		MSG_S2C_GOODS_BIND,										//! ��Ʒ�󶨣����֪ͨ
		MSG_S2C_GOODS_FROST,									//! ��Ʒ���ᣨ�ⶳ��֪ͨ

		//! ��Ʒ����
		MSG_S2C_GOODS_RE_UPGRADE_VIEW,							//! �ظ�Ԥ������
		MSG_S2C_GOODS_RE_UPGRADE_DO,							//! �ظ�ִ������
		//! ��Ʒħ��
		MSG_S2C_GOODS_RE_PLUG_EVIL_DO,							//! �ظ�ִ������
		MSG_C2S_GOODS_RE_RESET_ELEMENT_TYPE,					//! �ظ�ִ������ħ��Ԫ������
		MSG_C2S_GOODS_RE_RESET_XIANG_XING,						//! �ظ�ִ������ħ������

		//! �ֿ�
		MSG_S2C_DEPOT_OPEN,										//!	֪ͨ�ͻ��˴򿪲ֿ�
		MSG_S2C_DEPOT_RE_INPUT_PWD,								//! �ظ��ͻ�����������
		MSG_S2C_DEPOT_RE_CLOSE,									//! �ظ��رղֿ⣬Ҳ��Ϊ�ر�֪ͨ
		MSG_S2C_DEPOT_RE_CHANGE_PWD,							//! �ظ��޸�����
		MSG_S2C_DEPOT_RE_BUY_SUBPACK,							//! �����ӱ���
		MSG_S2C_DEPOT_FROSTED,									//! ֪ͨ�ֿⱻ����

		//! ѫ��
		MSG_S2C_MEDAL_ADD_NOTIFY,								//! ���ѫ��֪ͨ
		MSG_S2C_MEDAL_DEL_NOTIFY,								//! ɾ��ѫ��֪ͨ

		//! �Ǹ�ְҵ����
		MSG_S2C_MAKE_BEGIN,										//! ��ʼһ������
		MSG_S2C_MAKE_RESULT,									//! ����һ���������

		//! ͨ����Ʒ��������
		MSG_S2C_GOODS_AllPurpose_Open,							//! ��ͨ������
		MSG_S2C_GOODS_AllPurpose_Close,							//! �ر�����
		MSG_S2C_GOODS_AllPurpose_UpdateTxt,						//! ����˵���ı�
		MSG_S2C_GOODS_AllPurpose_RePush,						//! �ظ�������Ʒ
		MSG_S2C_GOODS_AllPurpose_RePop,							//! �ظ�ȡ����Ʒ
		MSG_S2C_GOODS_UPDATE_SCRIPTDESC,						//! ��Ʒ���������ı�
		MSG_S2C_GOODS_AllPurpose_UpdateGold,					//! ����������ʾǮ��

		MSG_S2C + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_S2C_SHOP_OPEN,										//���̵�			
		MSG_S2C_SHOP_BUY,
		MSG_S2C_SHOP_BUY_BAT,
		MSG_S2C_SHOP_SELL,	
		MSG_S2C_SHOP_SELL_BAT,
		MSG_S2C_SHOP_GOODSCHANGEGOODS,
		MSG_S2C_SHOP_REPAIR,						
		MSG_S2C_SHOP_REPAIRALL,	
		MSG_S2C_SHOP_REPAIRALLCOST,					// ������������������ȫ����Ʒ�Ļ���
		MSG_S2C_SHOP_REPAIRSINGLECOST,				// �����������������е�����Ʒ�Ļ���
		MSG_S2C_SHOP_DURUPDATE,						
		MSG_S2C_SHOP_CLOSE,		//##�رո����̵�	

		MSG_S2C_BUSINESS_OPEN,
		MSG_S2C_BUSINESS_BUY,
		MSG_S2C_BUSINESS_SELL,
		MSG_S2C_INVEST_OPEN,
		MSG_S2C_BUSINESS_INVEST,	//��ҵó�׵�Ͷ��
		MSG_S2C_SHOP_BUYBACK,

		MSG_S2C_BUSINESS_UPDATEPRICE, // ��ҵ����۸����
		MSG_S2C_FBUSINESS_OPER,		// �������̲���

		MSG_S2C + MAX_SUB * 7,
		// ----------------------------------------------------


		MSG_S2C + MAX_SUB * 8,
		// ----------------------------------------------------
		MSG_S2C_GM_ANSWER_GMMODE,								// ��Ӧ�ͻ�������GMģʽ����Ϣ
		MSG_S2C_GM_VIEW_HIDENAME_AROUND,						// ��ӦGM�鿴��Χ����������

		MSG_S2C + MAX_SUB * 9,
		// ----------------------------------------------------
		MSG_S2C_TEAM_SKILL_ABOUT,								//! ���������Ϣ����ʼ�����ͣ�
		MSG_S2C_TEAM_TeamCreate,								//! ���ݸ�ʽͬMSG_S2C_TEAM_TeamData����ֻ�ڽ���ʱ����
		MSG_S2C_TEAM_TeamData,									//! ���ж������ݵ���Ա
		MSG_S2C_TEAM_MemberData,								//! һ����Ա�Ķ�������
		MSG_S2C_TEAM_TeamDataChange,							//! �������ݸı�

		MSG_S2C_TEAM_MemberAllIdioinfo,							//! һ����Ա���зǶ����Ա����
		MSG_S2C_TEAM_UpdateIdioinfo,							//! ����ĳ���Ƕ����Ա����
		MSG_S2C_TEAM_MembereExStateChange,						//! ��Ա�쳣״̬�ı�
		MSG_S2C_TEAM_MemberStateChange,							//! ��Ա����״̬�ı�

		MSG_S2C_TEAM_MemberLeave,								//! ��Ա���
		MSG_S2C_TEAM_MemberLeaveHere,							//! ��Ա�뿪����(GS)
		MSG_S2C_TEAM_MastterChange,								//! �ӳ��ı�

		MSG_S2C_TEAM_Disband,									//! �����ɢ
		MSG_S2C_TEAM_GoodsSetChange,							//! �޸���Ʒ��������

		MSG_S2C_TEAM_MemberPos,									//! ���³�Աλ��
		MSG_S2C_TEAM_RECRUIT,									//! ������ļ״̬
		MSG_S2C_TEAM_RECRUITED_NUM,								//! ��������ļ������

		MSG_S2C_TEAM_QUERY_JOIN,								//�������
		MSG_S2C_TEAM_CHAT,										//����

		MSG_S2C_TEAM_Recommend_ByName,							//! �Ƽ����֪ͨ




		MSG_S2C + MAX_SUB * 10,
		// ----------------------------------------------------
		MSG_S2C_SKILL_USE,										// ʹ�ü���
		MSG_S2C_SKILL_STATE_CHANGE,								// ʹ�ü��ܺ���HP/MP/RP/YP״̬�ı�
		MSG_S2C_SKILL_ADD_EX_STATE,								// ʹ�ü��ܺ�����쳣״̬
		MSG_S2C_SKILL_END_EX_STATE,								// �쳣״̬����ʧ,������֪ͨ�ͻ���
		MSG_S2C_SKILL_UPDATE_EX_STATE,							// ����״̬ʱ��
		MSG_S2C_SKILL_EX_STATE_ADDITIONAL_DATA,					// �쳣״̬�ĸ�������
		MSG_S2C_SKILL_AFFECT_FIELD,								// ������Ӱ���������ͻ���
		MSG_S2C_SKILL_STUDY_FAILED,								// ѧϰ����ʧ��
		MSG_S2C_SKILL_STUDY_BEGIN,								// �ͻ��˷��ش򿪼���ѧϰ��ʦҳ��
		MSG_S2C_OCCUSKILL_STUDY_BEGIN,							// �յ��򿪸�ְҵ����ѧϰҳ����Ϣ
		MSG_S2C_OCCUSKILL_STUDY,								// �յ�ѧϰ��ְҵ������Ϣ
		MSG_S2C_OCCUSKILL_DELETE,								// ��ͻ��˷���ɾ��������Ŀ��Ϣ
		MSG_S2C_OCCUSKILL_NPCFACTURE_BEGIN,						// ��ͻ��˷���NPC�����б�
		MSG_S2C_OCCUSKILL_NPCFACTURE,							// ����NPC������Ϣ
		MSG_S2C_SKILL_UNLOCK_GOODS,								// ���ͽ����Ʒ������Ϣ
		MSG_S2C_SKILL_LOCK_GOODS,								// ������Ʒ��Ϣ

		MSG_S2C_SKILL_ADD_PET,
		MSG_S2C_SKILL_DEL_PET,
		MSG_S2C_SKILL_USE_PET,
		MSG_S2C_SKILL_END_PET,

		MSG_S2C + MAX_SUB * 11,
		// ----------------------------------------------------

		// �л�Э��-------------------------------------------------
		MSG_S2C_ORGSYS_InitOrgSys_Notify,					//! ��ʼ����֯ϵͳ֪ͨ
		MSG_S2C_ORGSYS_CreateORGSysUI,                      //! ����������գ�
		MSG_S2C_ORGSYS_UpdatePlayerFacInfo,					//! ����һ����ҵĹ�����Ϣ

		//! �᳤����
		MSG_S2C_ORGSYS_CreateFaction_Notify,				//! �����л�֪ͨ(���������ϣ����ͻ���Ӧ����һ�����л�����)
		MSG_S2C_ORGSYS_CreateFaction_Re,					//! �����л��Ӧ
		MSG_S2C_ORGSYS_UpgradeFaction_Notify,				//! �����л�֪ͨ
		MSG_S2C_ORGSYS_UpgradeFaction_Re,					//! �����л�ظ�
		MSG_S2C_ORGSYS_DisbandFaction_Notify,				//! ��ɢ�л�֪ͨ
		MSG_S2C_ORGSYS_DisbandFaction_Re,					//! ��ɢ�л��Ӧ	
		MSG_S2C_ORGSYS_SetPlacard_Re,						//! �޸Ĺ����Ӧ
		MSG_S2C_ORGSYS_SetLeaveword_Re,						//! �޸ı�����Ӧ
		MSG_S2C_ORGSYS_SetJobName_Re,						//! �޸Ĺٽ����ֻ�Ӧ
		MSG_S2C_ORGSYS_SetJobPurview_Re,					//! �޸Ĺٽ�Ȩ�޻�Ӧ
		MSG_S2C_ORGSYS_SetJobPurview_Notify,				//! �޸Ĺٽ�Ȩ�޻�Ӧ
		MSG_S2C_ORGSYS_SetMemberJob_Re,						//! ���ó�Ա�ٽ׻�Ӧ
		MSG_S2C_ORGSYS_SetMemberJob_Notify,					//! ���ó�Ա�ٽ�֪ͨ���¼ӣ�
		MSG_S2C_Faction_SetMemberTitle_Re,					//! ���ó�Ա�ƺŻ�Ӧ
		MSG_S2C_Faction_SetMemberTitle_Notify,				//! ���ó�Ա�ƺ�֪ͨ���¼ӣ�
		MSG_S2C_Faction_SetChairman_Re,						//! ��λ�ظ�
		MSG_S2C_Faction_SetChairman_Notify,					//! ��λ֪ͨ
		MSG_S2C_Faction_ChangeRecruitState_Notify,			//! ��ļ״̬�ı�֪ͨ

		//! ��Ա����
		MSG_S2C_ORGSYS_Quit_Re,								//! �˳��л��Ӧ
		MSG_S2C_ORGSYS_Quit_Notify,							//! �˳��л�֪ͨ
		MSG_S2C_ORGSYS_Kick_Re,								//! �߳��л��Ӧ
		MSG_S2C_ORGSYS_Kick_Notify,							//! �߳��л�֪ͨ
		MSG_S2C_ORGSYS_CheckRequisition_Re,					//! ��������Ӧ
		MSG_S2C_ORGSYS_CheckRequisition_Notify,				//! ���������������֪ͨ���¼ӣ�
		MSG_S2C_Faction_InvitePlayer_Re,					//! ������Ҽ����Ӧ
		MSG_S2C_Faction_InvitePlayer_Notify,				//! ������Ҽ���֪ͨ
		MSG_S2C_Faction_AddMember_Notify,					//! ��ӳ�Ա֪ͨ
		MSG_S2C_Faction_ApplyBoardOpen_Notify,				//! ������������֪ͨ

		MSG_S2C_ORGSYS_JoinFac_Re,							//! ��������ظ�

		//! ��������
		MSG_S2C_Faction_PublicData_Notify,					//! �л����ϣ�tagFacPublicData�ṹ��
		MSG_S2C_Faction_BaseData_Notify,					//! �л����ϣ�tagFacBaseData�ṹ��
		MSG_S2C_Faction_Member_Notify,						//! ��Ա��������
		MSG_S2C_Faction_Apply_Notify,						//! ��Ա��������
		MSG_S2C_Faction_Placard_Notify,						//! �������ϻ�Ӧ
		MSG_S2C_Faction_Leaveword_Notify,					//! �������ϻ�Ӧ


		MSG_S2C_Faction_IconBoardOpen_Notify,				//! ��ͼ���ϴ����֪ͨ
		MSG_S2C_Faction_IconData_Re,						//! �ϴ�һ������ͼ�����ݻظ�
		MSG_S2C_Faction_IconData_Notify,					//! һ�������ͼ�����ݸ���֪ͨ
		MSG_S2C_Faction_AllIconData_Re,						//! �������й���ͼ����Ϣ						

		MSG_S2C_Faction_MemberMap_Notifye,					//! ���³�Ա��ͼ
		MSG_S2C_Faction_MemberLevel_Notifye,				//! ���³�Ա�ȼ�
		MSG_S2C_Faction_MemberOccu_Notifye,					//! ���³�Աְҵ
		MSG_S2C_Faction_MemberOnline_Notifye,				//! ���³�Ա����״̬

		MSG_S2C_Faction_ChangeRes_Notify,					//! ��Դֵ���
		MSG_S2C_Faction_ChangeExp_Notify,					//! ����ֵ���
		MSG_S2C_Faction_ChangeMemberContribute_Notify,		//! ��Ա���ױ��
		MSG_S2C_Faction_Beachheads_Notify,					//	�ݵ�ս��Ϣ
		MSG_S2C_Faction_ChangeBattle_Notify,				//	�޸�ս����ֵ
		MSG_S2C_Faction_ChangeArmyInvest_Notify,			//	�޸ľ���Ͷ��ֵ
		MSG_S2C_Faction_ChangeJoinBattle_Notify,			//	�޸ĳ�Ա����Ȩ
		MSG_S2C_Faction_ChangeEnemy_Notify,					//	�޸ĵжԼ�����Ϣ
		MSG_S2C_Faction_OpenGUI,							//	�ű�����,�򿪼���ָ������
		// End���л�------------------------------------------------



		// ����Э��-------------------------------------------------
		MSG_S2C_UNION_Script_Creation,						//! �������˽ű�����֪ͨ�������˴�������
		MSG_S2C_UNION_Re_Creation,							//! �������˻ظ�
		MSG_S2C_UNION_Notify_Creation,						//! ��������֪ͨ
		MSG_S2C_UNION_Notify_Disband,						//! ��ɢ����֪ͨ��������
		MSG_S2C_UNION_Notify_Quit,							//! �л��˳�����֪ͨ��������
		MSG_S2C_UNION_Notify_Kick,							//! �������л��߳�����֪ͨ
		MSG_S2C_UNION_Re_Kick,								//! �������л��߳�����֪ͨ
		MSG_S2C_UNION_Notify_SetChairman,					//! ����ת��֪ͨ��������
		MSG_S2C_UNION_Re_SetRights,							//! ��������᳤Ȩ�޻ظ�
		MSG_S2C_UNION_Notify_GetRights,						//! �᳤һ���˻����������Ȩ�޵�֪ͨ
		MSG_S2C_UNION_Response_Invite,						//! ���� �յ� �л�᳤ �ظ���������
		MSG_S2C_UNION_Notify_Anwser_Invite,					//! �л�᳤ �յ� ��������������˵�����
		MSG_S2C_UNION_Notify_Join,							//! �л��������֪ͨ��������
		MSG_S2C_UNION_Notify_BaseData,						//! һ�����˵Ļ�������֪ͨ
		MSG_S2C_UNION_Notify_MemberData,					//! һ�����˵�һ����Ա������ӻ���µ�֪ͨ
		MSG_S2C_UNION_Notify_AllMemberData,					//! һ�����˵����г�Ա����֪ͨ
		// End������------------------------------------------------




		MSG_S2C_Quest_Add,										//���һ������
		MSG_S2C_Quest_Complete,									//���һ������
		MSG_S2C_Quest_Remove,									//�Ƴ�һ������
		//MSG_S2C_Quest_Update,									
		MSG_S2C_Quest_UpdateStep,								//����һ������
		MSG_S2C_Quest_UpdateParamData,							//����һ����������б���
		MSG_S2C_Quest_UpdateData,								//����һ������ı���
		MSG_S2C_Quest_UpdateRecordData,							//�������һ�������¼
		MSG_S2C_Quest_Talk,
		MSG_S2C_MercQuest_UpdateData,							//���ͻ��˷���ٸ���������ݲ�ͬʱ�򿪽���
		MSG_S2C_MercQuest_OpenInterface,						//��ٸ���������

		MSG_S2C_ANSWER_PLAYERRANKS,								//��ͻ��˷���������а�
		//�ͽ�����
		MSG_S2C_SHARE_QUESTLIST_UPDATE,							//���¹��������б�
		MSG_S2C_SHARE_QUEST_OPEN,								//�򿪶������ڽ��е��������
		MSG_S2C_SHARE_CURRQUEST,								//���͵�ǰ���ڽ��е�����ID

		// �����̵�
		MSG_S2C + MAX_SUB * 12,
		// ----------------------------------------------------
		MSG_S2C_PALYERSHOP_CREATE,								// ������󿪵�
		MSG_S2C_PLAYERSHOP_OPENFORBUSINESS,						// �������ʼ��̯
		MSG_S2C_PLAYERSHOP_CLOSEDOWN,							// ��ҹرո����̵�
		MSG_S2C_PLAYERSHOP_LOOKGOODS,							// �鿴��Ʒ
		MSG_S2C_PLAYERSHOP_BUYITEM,								// ������Ʒ
		MSG_S2C_PLAYERSHOP_CLOSE,								// �رո����̵�
		MSG_S2C_PLAYERSHOP_QUIT,								// �������뿪�����̵�
		MSG_S2C_PLAYERSHOP_TOBUYER,								// ��������
		MSG_S2C_PLAYERSHOP_TOSELLER,							// ������
		MSG_S2C_PLAYERSHOP_UPDATE,								// �̵����

		MSG_S2C_PLAYERSHOP_MSGBOARDGET,							// �������������õ����԰���Ϣ���̵����ơ������Ƽ��Լ�������Ϣ��
		MSG_S2C_PLAYERSHOP_MSGBOARDCHAT,						// ���������ص������԰��е�������Ϣ


		// ����
		 MSG_S2C + MAX_SUB * 13,
		// ----------------------------------------------------
		MSG_S2C_CONTAINER_OBJECT_MOVE,
		MSG_S2C_CONTAINER_OBJECT_UNLOCK,
		MSG_S2C_CONTAINER_OBJECT_AMOUNT_CHANGE,
		MSG_S2C_CONTAINER_CLEAR,

		// ����
		MSG_S2C + MAX_SUB * 14,
		// ----------------------------------------------------
		MSG_S2C_PET_AI,											// ����AI�¼�
		MSG_S2C_PET_MOVE,										// �ƶ�
		MSG_S2C_PET_STOP,										// ֹͣ
		MSG_S2C_PET_CHANGE_MODE,								// �ı���ﵱǰģʽ
		MSG_S2C_PET_CHANGE_ACTION,								// �ı���ﵱǰ����
		MSG_S2C_PET_CHANGE_SPEED,								// �ı��ٶ�
		MSG_S2C_PET_HOUSE,										// լ�س���
		MSG_S2C_PET_CANCEL,										// ��������
		MSG_S2C_PET_CHANGE_MASTER,								// �ı�����
		MSG_S2C_PET_CREATE_PET,									// ��ӳ���

		MSG_S2C_PET_CHANGE_STATE_SLEEP,							// ����ı�Ϊ��˯״̬
		MSG_S2C_PET_CHANGE_STATE_AWAIT,							// ����ı�Ϊ����״̬
		MSG_S2C_PET_CHANGE_STATE_FIGHT,							// ����ı�Ϊս��״̬
		MSG_S2C_PET_CHANGE_STATE_ACTIVE,						// ����ı�Ϊ����״̬
		MSG_S2C_PET_CHANGE_STATE,                               // ����ı�״̬

		//�Ʒ�
		MSG_S2C + MAX_SUB * 15, 
		// -----------------------------------------------------
		MSG_S2C_COST_NOTENOUGHCOST,

		//���۵㿨 s->c
		MSG_S2C + MAX_SUB * 16, 
		// -----------------------------------------------------
		MSG_S2C_CARD_SHOWWINDOW,								//��֪ͨ�򿪵㿨���۴���
		MSG_S2C_CARD_SELL_RESPONSE,								//��gs��Ӧ����������
		MSG_S2C_CARD_BUY_RESPONSE,
		MSG_S2C_CARD_CANCEL_RESPONSE,
		MSG_S2C_CARD_LIST_RESPONSE,								//	���ص�ǰ���۵ĵ㿨�б�
		MSG_S2C_CARD_SALECOSTS_RESPONSE,						//	���ص�ǰ���۵ĵ㿨�б�

		MSG_S2C_CARD_CHARGE_RESPONSE,

		//����
		MSG_S2C + MAX_SUB * 17, 
		// -----------------------------------------------------
		MSG_S2C_COUNTRY_CHANGE,									// ���Ĺ���


		// ��ֵ�̵�
		 MSG_S2C + MAX_SUB * 18,
		// ----------------------------------------------------
		MSG_S2C_INCREMENTSHOP_OPEN,										//���̵�			
		MSG_S2C_INCREMENTSHOP_BUY,										//gameserver���ش��̵��й���һ����Ʒ�Ľ��
		//MSG_S2C_INCREMENTSHOP_SELL,							
		//MSG_S2C_INCREMENTSHOP_REPAIR,						
		//MSG_S2C_INCREMENTSHOP_REPAIRALL,						
		//MSG_S2C_INCREMENTSHOP_DURUPDATE,						
		MSG_S2C_INCREMENTSHOP_CLOSE,							//##�رո����̵�
		MSG_S2C_INCREMENTSHOP_AFFICHE_RESPONSE,					// ���洰�ڿ�

		MSG_S2C_INCREMENTSHOP_GETLOG_RESPONSE,					// ������ֵ��־
		MSG_S2C_INCREMENTSHOP_UPDATA,							// ���ݸ���

		// ����ϵͳ
		MSG_S2C + MAX_SUB * 19,
		// ----------------------------------------------------
		MSG_S2C_LINKMAN_INIT,										//! ��ʼ����ϵ���б�
		MSG_S2C_LINKMAN_ADD,										// S ֪ͨ A �����ϵ�� B �Ľ��
		MSG_S2C_LINKMAN_DEL,										// S ֪ͨ A ɾ����ϵ�� B �Ľ��
		MSG_S2C_LINKMAN_MOVE,										// S ֪ͨ A �ƶ���ϵ�� B �Ľ��

		MSG_S2C_LINKMAN_OTHER_HANDLE,								// S ֪ͨ B�� A �� B �� ��ӡ�ɾ�����ƶ� �Ȳ������

		MSG_S2C_LINKMAN_REQUEST_ADD,								// S ֪ͨ B�� A �� B ���������

		MSG_S2C_LINKMAN_LOGIN,										// ��ϵ������
		MSG_S2C_LINKMAN_LOGOUT,										// ��ϵ������

		MSG_S2C_LINKMAN_INFO_CHANGE,								// ��Ϣ�ı䣺��ͼ���ȼ���ְҵ
		MSG_S2C_LINKMAN_DELSTATE_CHANGE,							// ɾ��״̬�ı�  

		MSG_S2C_LINKMAN_LEAVE_WORD,									//! �������Ե��ͻ���
		MSG_S2C_LEAVEWORD_SAVE_RESULT,								//! ���Դ�����

		//! С����
		MSG_S2C + MAX_SUB * 20,
		// ----------------------------------------------------
		MSG_S2C_SPRITE_AutoSupply_OnOff_Re,							//! �Զ�����ϵͳ���ظ����ܿ���
		MSG_S2C_SPRITE_AutoSupply_ChangeSetup_Re,					//! �Զ�����ϵͳ���ظ��޸�һ����������
		MSG_S2C_SPRITE_AutoSupply_UseItem_Notify,					//! �Զ�����ϵͳ��֪ͨʹ����һ��ҩƷ

		MSG_S2C_SaveNewSetupData_Re,								//! ��ұ����µ���Ϸ���ûظ�

		//�� ��ҽ���
		MSG_S2C + MAX_SUB * 21,

		MSG_S2C_PLAYERTRADE_REQUEST,							//GSת������������ͻ�
		MSG_S2C_PLAYERTRADE_BEGIN,								//���׿�ʼ
		MSG_S2C_PLAYERTRADE_LOCK,
		//MSG_S2C_PLAYERTRADE_GETGOODS,							//��������һ����Ʒ��Ϣ���Է����
		//MSG_S2C_PLAYERTRADE_FETCHGOODS,							//���������ش�չʾ����һ����Ʒ����ؽ��
		//MSG_S2C_PLAYERTRADE_CHANGEMONEY,						//���������ص�����Ǯ����ؽ��
		MSG_S2C_PLAYERTRADE_WAIT,	
		//MSG_S2C_PLAYER_TRADING,									// ��ҵ㽻��ʱ���ʹ���Ϣ
		MSG_S2C_PLAYERTRADE_CANCEL,								//������֪ͨ��ҶԷ��Ѿ�ȡ������
		MSG_S2C_PLAYERTRADE_RESULT,								//���׵����ս��

		//! �ʼ�ϵͳ
		MSG_S2C + MAX_SUB * 29,
		MSG_S2C_MAIL_RECEIVER,										//!����ʼ�����
		MSG_S2C_MAIL_SEND_FAILED,									//!����ʧ��
		MSG_S2C_MAIL_SEND,											//!���ʼ�����
		MSG_S2C_MAIL_READ,											//!��ȡ�ʼ�
		MSG_S2C_MAIL_FAILED_MOVE_GOODS,								//!�ʼ�ȡ��Ʒʧ��
		MSG_S2C_MAIL_MOVE_GOODS_RESULT,								//!�ʼ�ȡ��Ʒ���
		//MSG_S2C_MAIL_DELETE_RESULT,									//!ɾ���и����ʼ���ȷ����Ϣ
		MSG_S2C_MAIL_DELETE_OK,										//!ɾ���ʼ�
		MSG_S2C_MAIL_AGREED_PAY,									//!�������Ž�����Ϣ
		//MSG_S2C_MAIL_AGREED_PRESENT,								//!ͬ������
		MSG_S2C_MAIL_GET_GOLD_RESULT,								//!����ȡ��Ǯ���
		//MSG_S2C_MAIL_PRESENT_FAILED,								//����ʧ��
		//MSG_S2C_MAIL_PRESENT_OK,									//������������
		MSG_S2C_MAIL_SYSMAILMSG,									//ϵͳ�ʼ�������Ϣ

		// Fox@20081107----------------------------------------------
		// ��ҹ���ϵͳ( PlayerManager )
		 MSG_S2C + MAX_SUB * 30,
		// -----------------------------------------------------
		MSG_S2C_PM_ACK,							// GSӦ���ѯ�����Ϣ

		// Expter@20090527
		//=======================================================================================================
		MSG_S2C + MAX_SUB * 31,
		MSG_S2C_AC_QUESTION,
		MSG_S2C_AC_ANSWER_RET,
		MSG_S2C_AC_LOGINFAILED,

		// �������
		MSG_S2C + MAX_SUB * 32,
		MSG_S2C_BOUNS_INFO_RESPOND,
		MSG_S2C_BOUNS_GET_RESULT,

		// Auction Hosue
		MSG_S2C + MAX_SUB * 33,
		MSG_S2C_AH_OPER,

		// Phase_S2C
		MSG_S2C + MAX_SUB * 34,
		MSG_S2C_PHASE_OPER,
		MSG_S2C_FOLLOW_Leader,                  // GS��������ӳ�ģʽ

		//���ĺ������ģ��
		 MSG_S2C + MAX_SUB * 35,
		MSG_S2C_OPER,

		//�츳---
		MSG_S2C + MAX_SUB * 36, 
		MSG_S2C_GENIUS_SKILLUPDATE,
		//////////////////
		//				//
		//	CostServer	//
		//				//
		//////////////////

		//========================================================================================================
		MAX_TYPE * 13,			// World Server -> Cost Server
		//========================================================================================================

		//�Ʒ�
		MSG_W2COST + MAX_SUB * 0,
		// -------------------------------------------------------
		MSG_W2COST_COST_LOGIN,
		MSG_W2COST_COST_CHANGE,
		MSG_W2COST_COST_COST,
		MSG_W2COST_COST_OFFLINE,
		MSG_W2COST_COST_ALLOFFLINE,						//��WorldServer�����󣬷��ʹ���Ϣ
		//MSG_W2COST_COST_BATCHLOGIN,					//��WorldServer��GameServer�����ɹ�ʱ�����ʹ���Ϣ
		MSG_W2COST_COST_QUESTINFO,						// �����ʺ���Ϣ

		// �㿨����
		 MSG_W2COST + MAX_SUB * 1,
		// ------------------------------------------------------
		MSG_W2COST_CARD_VALIDATE_REQUEST,				// ת�����۵㿨��Ϣ��COST�����������е㿨��֤
		MSG_W2COST_CARD_SELL_REQUEST,					// ���ͼ�������
		MSG_W2COST_CARD_RESERVE_REQUEST,					// ���͹�������
		MSG_W2COST_CARD_BUY_REQUEST,					// ���͹�������
		MSG_W2COST_CARD_CANCEL_REQUEST,					// ȡ������
		MSG_W2COST_CARD_TIMEOUT,						// ���ۿ���ʱ�¼�

		MSG_W2COST_CARD_CHARGE_REQUEST,

		//========================================================================================================
		MAX_TYPE * 14,		// Cost Server -> World Server
		//========================================================================================================

		//�Ʒ�
		 MSG_COST2W + MAX_SUB * 0,
		//---------------------------------------------------------
		MSG_COST2W_COST_NOTENOUGHCOST,
		MSG_COST2W_COST_INFORMATION,		
		MSG_COST2W_COST_ISEVERCONNECT,
		MSG_COST2W_COST_ANSWERINFO,						// �����ʺ���Ϣ

		// �㿨����
		 MSG_COST2W + MAX_SUB * 1,  
		// --------------------------------------------------------
		MSG_COST2W_CARD_VALIDATE_RESPONSE,				// �㿨��֤���
		MSG_COST2W_CARD_SELL_RESPONSE,					// ���ؼ�������
		MSG_COST2W_CARD_RESERVE_RESPONSE,
		MSG_COST2W_CARD_BUY_RESPONSE,					// ���ع�������
		MSG_COST2W_CARD_CANCEL_RESPONSE,					

		MSG_COST2W_CARD_CHARGE_RESPONSE,

		//////////////////
		//				//
		//	DBServer	//
		//				//
		//////////////////

		//========================================================================================================
		MAX_TYPE * 15,			// DBServer --> DBServer
		//========================================================================================================

		MSG_DB2DB + MAX_SUB * 0,
		// ----------------------------------------------------

		//========================================================================================================
		MAX_TYPE * 16,			// WS --> DBS
		//========================================================================================================

		MSG_W2DB + MAX_SUB * 0, //WS->DBS
		// ----------------------------------------------------
		MSG_W2DB_SERVER_SENDSERVERINFO,							// ����server��Ϣ��DBServer
		MSG_W2DB_SERVER_WORLDSERVERCLOSE,						// DBS��⵽WS�ر�
		MSG_W2DB_SERVER_GAMESERVERLOST,							// DB��⵽GS�Ͽ�
		MSG_W2DB_RECONNECTWORLDSERVEROK,							// ����LoginServer�ɹ�
		MSG_W2DB_SERVER_COSTSERVERCLOSE,
		MSG_W2DB_RECONNECTCOSTSERVEROK,
		MSG_W2DB_SERVER_ANSWER_PING,
		MSG_W2DB_SERVER_QUEST_RECONNECTION,
		MSG_W2DB_SERVER_SENDSETUP,
		//MSG_W2W_SERVER_CLEAROFFLINEPLAYER,					// �����߳��ڴ��̳ɹ�����Ϣ�����������������б�


		MSG_W2DB_SERVER_UNREGENTITY,							// ���DBS����Ķ���
		MSG_W2DB_SERVER_PLAYER_PERSONALHOUSEGUID,               // ��ѯ��ҵĸ��˷�������
		MSG_W2DB_SERVER_SAVEDATAEND,							// ����DoSaveData��Ϣ���ͽ�����־

		MSG_W2DB_SERVER_QUEST_PING,


		MSG_W2DB + MAX_SUB * 1, //WS->DBS
		// ----------------------------------------------------
		MSG_W2DB_SAVE_PLAYER,									// ����Player����
		MSG_W2DB_SAVE_PLAYER_GOOD,								// ����Player��Good����
		MSG_W2DB_SAVE_COUNTRY,							// ����LoginServer�ɹ�
		MSG_W2DB_SAVE_CREATE_PLAYER,                    // ���洴���Ľ�ɫ
		MSG_W2DB_SAVE_DELETE_PLAYER,                    // ����ɾ���Ľ�ɫ
		MSG_W2DB_SAVE_CARDS_LARGESS,	
		MSG_W2DB_SAVE_ALL_CITY_WAR_PARAM,
		MSG_W2DB_SAVE_ALL_VILLAGE_WAR_PARAM,
		MSG_W2DB_SAVE_ALL_ENEMY_FACTIONS,
		MSG_W2DB_SAVE_REGION_PARAM,
		MSG_W2DB_SAVE_CONFEDERATION,
		MSG_W2DB_SAVE_FACTION,
		MSG_W2DB_SAVE_PERSONALHOUSE,

		MSG_W2DB + MAX_SUB * 2, //WS->DBS
		// ----------------------------------------------------
		MSG_W2DB_LOAD_INIT_PLAYER,								// WS��ʼ����DB��ȡPlayer
		MSG_W2DB_LOAD_PLAYER,									// ����Player����
		MSG_W2DB_LOAD_PLAYER_GOOD,								// ����Player��Good����
		MSG_W2DB_LOAD_COUNTRY,								// ����LoginServer�ɹ�
		MSG_W2DB_LOAD_INIT_ENEMY_FACTION,					// ��ȡ�ж��л�����
		MSG_W2DB_LOAD_INIT_COUNTRY,
		MSG_W2DB_LOAD_PLAYER_WORLD_ID,
		MSG_W2DB_LOAD_REGION_PARAM,
		MSG_W2DB_LOAD_CONFEDERATION,
		MSG_W2DB_LOAD_ALL_CONFEDERATIONS,
		MSG_W2DB_LOAD_FACTION,
		MSG_W2DB_LOAD_ALL_FACTIONS,
		MSG_W2DB_LOAD_CARDS_LARGESS,
		MSG_W2DB_LOAD_ALL_CITY_WAR_PARAM,
		MSG_W2DB_LOAD_ALL_VILLAGE_WAR_PARAM,
		MSG_W2DB_LOAD_ALL_ENEMY_FACTIONS,
		MSG_W2DB_LOAD_PERSONALHOUSE,
		MSG_WSDB_LOAD_UNISTRINGRES,

		 MSG_W2DB + MAX_SUB * 3, //WS->DBS
		// ----------------------------------------------------
		MSG_W2DB_PASS_LOG_PLAYERBASELIST,				// WS����LS������
		MSG_W2DB_PASS_LOG_QUEST_PLAYERDATA,				// WS����LS������
		MSG_W2DB_PASS_LOG_DELETEROLE,					// WS����ɾ��һ����ɫ
		MSG_W2DB_PASS_LOG_RESTOREROLE,					// WS����ָ�һ����ɫ
		MSG_W2DB_PASS_LOG_CREATEROLE,					// WS���󴴽�һ����ɫ
		MSG_W2DB_PASS_CARD_VALIDATE_RESPONSE,			// ���ֿ�
		MSG_W2DB_PASS_GM_BANPLAYER,						// ����S2W��GM_BANPLAYER��Ϣ
		MSG_W2DB_PASS_LOAD_ALL_DUPRGN,					// ��ʼ����ȡ����PH����
		MSG_W2DB_PSASS_OTHER_GETLOG_REQUEST,			// WS����GS���������������־��Ϣ
		MSG_W2DB_PSASS_OTHER_WRITELOG,					// WS����GS����д������־��Ϣ
		MSG_W2DB_PASS_LOAD_ALL_FACTION,					// �������й�������
		MSG_W2DB_PASS_DelOneFaction,					// ����ɾ��һ����������
		MSG_W2DB_PASS_LOAD_ALL_SCRIPTVAR,				// ��ʼ����ȡ����ȫ�ֽű���������

		MSG_W2DB + MAX_SUB * 4, //WS->DBS
		// ----------------------------------------------------
		MSG_W2DB_OPER_ENTITY,

		MSG_W2DB + MAX_SUB * 5, //DBS->WS
		// ----------------------------------------------------
		MSG_W2DB_RGN_LOAD_PERSONALHOUSE,
		MSG_W2DB_RGN_SAVE_PERSONALHOUSE,

		//! ����ϵͳ
		MSG_W2DB + MAX_SUB * 6, 
		// ----------------------------------------------------
		MSG_W2DB_LEAVEWORD_SETUP,								//! ������������
		MSG_W2DB_LEAVEWORD_SAVE,								//! ������������
		MSG_W2DB_LEAVEWORD_POP,									//! ȡ��һ����ҵ�����
		MSG_W2DB_LEAVEWORD_CLEAR_OUTTIME,						//! ����ʱ�ļ�¼

		//! �ʼ�ϵͳ
		MSG_W2DB + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_W2DB_MAIL_INITLOAD,
		MSG_W2DB_MAIL_INSERT_OFFLINE, // ���һ������������ʼ�Group
		MSG_W2DB_MAIL_DELETE_OFFLINE, // ɾ��һ������������ʼ�Group
		MSG_W2DB_MIAL_QUERY,
		MSG_W2DB_MIAL_ADDOFFLINEPLAYERCOSTGOLD, // ��Ӳ�������ҵĸ����ż����
		MSG_W2DB_MIAL_SENDSAVEGROUPMAILS,

		//========================================================================================================
		MAX_TYPE * 17,			// GS --> DBS
		//========================================================================================================

		MSG_S2DB + MAX_SUB * 0, //WS->DBS
		// ----------------------------------------------------
		MSG_S2DB_SERVER_SENDSERVERINFO,							// ����server��Ϣ��DBServer
		MSG_S2DB_SERVER_WORLDSERVERCLOSE,						// DBS��⵽WS�ر�
		MSG_S2DB_SERVER_GAMESERVERCLOSE,							// DB��⵽GS�Ͽ�
		MSG_S2DB_RECONNECTGAMESERVEROK,							// ����LoginServer�ɹ�
		MSG_S2DB_SERVER_ANSWER_PING,
		MSG_S2DB_SERVER_QUEST_RECONNECTION,


		MSG_S2DB + MAX_SUB * 1, //WS->DBS
		// ----------------------------------------------------
		MSG_S2DB_SAVE_PLAYER,									// ����Player����
		MSG_S2DB_SAVE_PLAYER_GOOD,								// ����Player��Good����
		MSG_S2DB_SAVE_COUNTRY,									// ����LoginServer�ɹ�
		MSG_S2DB_SAVE_FACTION,				
		MSG_S2DB_SAVE_CONFEDERATION,


		MSG_S2DB + MAX_SUB * 2, //WS->DBS
		// ----------------------------------------------------
		MSG_S2DB_LOAD_INIT_PLAYER,								// DB����WS��ʼ����ȡ�������
		MSG_S2DB_LOAD_PLAYER,									// ����Player����
		MSG_S2DB_LOAD_PLAYER_GOOD,								// ����Player��Good����
		MSG_S2DB_LOAD_COUNTRY,								// ����LoginServer�ɹ�
		MSG_S2DB_LOAD_FACTION,				
		MSG_S2DB_LOAD_CONFEDERATION,

		MSG_S2DB + MAX_SUB * 3, //GS->DBS
		// ----------------------------------------------------
		MSG_S2DB_OPER_CARD,
		MSG_S2DB_OPER_CITY_WAR,
		MSG_S2DB_OPER_ENEMY_FACTION,
		MSG_S2DB_OPER_FACTION,
		MSG_S2DB_OPER_GEN_VAR,
		MSG_S2DB_OPER_GOOD,
		MSG_S2DB_OPER_PLAYER,
		MSG_S2DB_OPER_DUPRGN,
		MSG_S2DB_OPER_REGION,
		MSG_S2DB_OPER_SETUP,
		MSG_S2DB_OPER_UNION,
		MSG_S2DB_OPER_VILLAGEWAR,
		MSG_S2DB_OPER_COUNTRY,
		//========================================================================================================
		MAX_TYPE * 18,			// DBS --> WS 
		//========================================================================================================

		MSG_DB2W + MAX_SUB * 0, //DBS --> WS 
		// ----------------------------------------------------
		MSG_DB2W_SERVER_QUEST_PING,
		MSG_DB2W_SERVER_QUEST_RECONNECTION,
		MSG_DB2W_SERVER_SENDSETUP,
		MSG_DB2W_SERVER_INIT_PLAYERDATABUFFER, // ��ʼ��CLoginPlayer��CPlayer���ݻ�����,��߲����ٶ�
		MSG_DB2W_SERVER_PLAYER_PERSONALHOUSEGUID,               // ��ѯ��ҵĸ��˷�������

		MSG_DB2W + MAX_SUB * 1, //DBS --> WS 
		// ----------------------------------------------------
		MSG_DB2W_PASS_LOG_PLAYERBASELIST,        // ��ӦLS��WS���ݵ�����
		MSG_DB2W_PASS_LOG_PLAYERBASELIST_DELTIMER, // ���ͻ�ӦLS��WS���ݵ�����ɾ����ʱ����Ϣ
		MSG_DB2W_PASS_LOG_QUEST_PLAYERDATA,				// ��ӦLS��WS���ݵ�����
		MSG_DB2W_PASS_LOG_CREATEROLE,
		MSG_DB2W_PASS_LOG_DELETEROLE,
		MSG_DB2W_PASS_LOG_RESTOREROLE,
		MSG_DB2W_PASS_GM_BANPLAYER,						// ����S2W��GM_BANPLAYER��Ϣ
		MSG_DB2W_PASS_LOAD_ALL_DUPRGN,
		MSG_DB2W_PSASS_OTHER_GETLOG_REQUEST,			// DBS��ӦWS��GS���������������־��Ϣ
		MSG_DB2W_PASS_LOAD_ALL_FACTION_FINISH,			// ��ȡ������Ϣ���
		MSG_DB2W_PASS_DelFaction_Finish,				// ɾ���������
		MSG_DB2W_PASS_LOAD_ALL_SCRIPTVAR,				// ��ʼ����ȡ����ȫ�ֽű���������

	 MSG_DB2W + MAX_SUB * 2, //DBS --> WS 
		// ----------------------------------------------------
		MSG_DB2W_SAVE_PLAYER,
		MSG_DB2W_SAVE_PLAYER_GOOD,
		MSG_DB2W_SAVE_COUNTRY,
		MSG_DB2W_SAVE_CREATE_PLAYER,                    // ���洴���Ľ�ɫ
		MSG_DB2W_SAVE_DELETE_PLAYER,                    // ����ɾ���Ľ�ɫ
		MSG_DB2W_SAVE_CARDS_LARGESS,
		MSG_DB2W_SAVE_ALL_CITY_WAR_PARAM,
		MSG_DB2W_SAVE_ALL_VILLAGE_WAR_PARAM,
		MSG_DB2W_SAVE_ALL_ENEMY_FACTIONS,
		MSG_DB2W_SAVE_REGION_PARAM,
		MSG_DB2W_SAVE_CONFEDERATION,
		MSG_DB2W_SAVE_FACTION,
		MSG_DB2W_SAVE_PERSONALHOUSE,

		MSG_DB2W + MAX_SUB * 3, //DBS --> WS 
		// ----------------------------------------------------
		MSG_DB2W_LOAD_INIT_ENEMY_FACTION,
		MSG_DB2W_LOAD_INIT_COUNTRY,
		MSG_DB2W_LOAD_PLAYER,
		MSG_DB2W_LOAD_INIT_PLAYER,
		MSG_DB2W_LOAD_PLAYER_GOOD,
		MSG_DB2W_LOAD_COUNTRY,
		MSG_DB2W_LOAD_PLAYER_WORLD_ID,
		MSG_DB2W_LOAD_REGION_PARAM,
		MSG_DB2W_LOAD_CONFEDERATION,
		MSG_DB2W_LOAD_ALL_CONFEDERATIONS,
		MSG_DB2W_LOAD_FACTION,
		MSG_DB2W_LOAD_ALL_FACTIONS,
		MSG_DB2W_LOAD_CARDS_LARGESS,
		MSG_DB2W_LOAD_ALL_CITY_WAR_PARAM,
		MSG_DB2W_LOAD_ALL_VILLAGE_WAR_PARAM,
		MSG_DB2W_LOAD_ALL_ENEMY_FACTIONS,
		MSG_DB2W_LOAD_PERSONALHOUSE,

		 MSG_DB2W + MAX_SUB * 4, //WS->DBS
		// ----------------------------------------------------
		MSG_DB2W_OPER_ENTITY,

		MSG_DB2W + MAX_SUB * 5, //DBS->WS
		// ----------------------------------------------------
		MSG_DB2W_RGN_LOAD_PERSONALHOUSE,
		MSG_DB2W_RGN_SAVE_PERSONALHOUSE,

		//! ����ϵͳ
		MSG_DB2W + MAX_SUB * 6,
		// ----------------------------------------------------
		MSG_DB2W_LINKMAN_PLAYER_DATA_SAVE_NOTIFY,					//! �����ϵ�����ݱ���֪ͨ

		MSG_DB2W_LEAVEWORD_SAVE_RESULT,								//! �ظ����Ա�����
		MSG_DB2W_LEAVEWORD_RE_POP,									//! �ظ�ȡ���������

		//! �ʼ�ϵͳ
		MSG_DB2W + MAX_SUB * 7,
		// ----------------------------------------------------
		MSG_DB2W_MAIL_INITLOAD,
		MSG_DB2W_MAIL_OPERATION_RESLUT,
		MSG_DB2W_MIAL_QUERY,
		MSG_DB2W_MIAL_ADDOFFLINEPLAYERCOSTGOLD, // ��Ӧ��Ӳ�������ҵĸ����ż����


		//========================================================================================================
		MAX_TYPE * 19,			// DBS --> GS 
		//========================================================================================================

		MSG_DB2S + MAX_SUB * 0, //DBS --> GS 
		// ----------------------------------------------------
		MSG_DB2S_SERVER_QUEST_PING,
		MSG_DB2S_SERVER_QUEST_RECONNECTION,
		MSG_DB2S_SERVER_PLAYER_PERSONALHOUSEGUID,               // ��ѯ��ҵĸ��˷�������


		MSG_DB2S + MAX_SUB * 1, //DBS --> GS 
		// ----------------------------------------------------
		MSG_DB2S_SAVE_PLAYER,
		MSG_DB2S_SAVE_FACTION,				
		MSG_DB2S_SAVE_CONFEDERATION,

		MSG_DB2S + MAX_SUB * 2, //DBS --> GS 
		// ----------------------------------------------------
		MSG_DB2S_LOAD_PLAYER,
		MSG_DB2S_LOAD_FACTION,				
		MSG_DB2S_LOAD_CONFEDERATION,


		MSG_DB2S + MAX_SUB * 3, //DBS --> WS 
		// ----------------------------------------------------
		MSG_DB2S_PASS_LOG_PLAYERBASELIST,        // ��ӦLS��WS���ݵ�����
		MSG_DB2S_PASS_LOG_QUEST_PLAYERDATA,				// ��ӦLS��WS���ݵ�����

		MSG_DB2S + MAX_SUB * 4, //DBS --> WS 
		// ----------------------------------------------------
		MSG_DB2S_LOG_PLAYER_ONLINE,

		 MSG_DB2S + MAX_SUB * 5, //GS->DBS
		// ----------------------------------------------------
		MSG_DB2S_OPER_CARD,
		MSG_DB2S_OPER_CITY_WAR,
		MSG_DB2S_OPER_ENEMY_FACTION,
		MSG_DB2S_OPER_FACTION,
		MSG_DB2S_OPER_GEN_VAR,
		MSG_DB2S_OPER_GOOD,
		MSG_DB2S_OPER_PLAYER,
		MSG_DB2S_OPER_REGION,
		MSG_DB2S_OPER_DUPRGN,
		MSG_DB2S_OPER_SETUP,
		MSG_DB2S_OPER_UNION,
		MSG_DB2S_OPER_VILLAGEWAR,
		MSG_DB2S_OPER_COUNTRY,

		//========================================================================================================
	MAX_TYPE * 20,			// OtherServer --> LogServer
		//========================================================================================================
		MSG_02LOG + MAX_SUB * 0, //! һ����Ϣ
		// ----------------------------------------------------
		MSG_02LOG_SERVER_LOGIN,			//! ��½��Ϣ����
		MSG_02LOG_SERVER_LOG,			//! ������Ҫ��¼����־
		MSG_02LOG_SERVER_LOG_SQLString, //! ������Ҫ��¼����־����SQL�����ʽ����



		//MSG_02LOG_LOG					= MSG_02LOG + MAX_SUB * 1, //! ��־����
		// ----------------------------------------------------


		//========================================================================================================
		MAX_TYPE * 21,			// LogServer --> OtherServer
		//========================================================================================================
		MSG_LOG20 + MAX_SUB * 1, // һ����Ϣ
		// ----------------------------------------------------
		MSG_LOG20_SERVER_TABLE,			//���ṹ��Ϣ
		MSG_LOG20_SERVER_CONDITION,		//��Ϣɸѡ����
		MSG_LOG20_SERVER_AFFIRM,		//ȷ�Ϸ�������LogServer�������õ����ݿ�

		///
		/// WorldServer��MonsterServer֮�����Ϣֵ��������Ϣֵ�����������ļ��С�
		///
	MAX_TYPE * 22,
		/// Do NOT insert any values here
		MAX_TYPE * 23 - 1,

		MAX_TYPE * 23,
		/// Do NOT insert any values here
		MAX_TYPE * 24 - 1,

		///
		/// GameServer��Monster֮�����Ϣֵ��������Ϣֵ�����������ļ��С�
		///
		MAX_TYPE * 24,
		/// Do NOT insert any values here
	MAX_TYPE * 25 - 1,

		MAX_TYPE * 25,
		/// Do NOT insert any values here
		 MAX_TYPE * 26 - 1,

		///
		/// GameServer��WorldServer֮�����Ϣֵ(���ڼ������������߼�)��������Ϣ�����������ļ��С�
		///
	MAX_TYPE * 26,
		/// Do NOT insert any values here
		 MAX_TYPE * 27 - 1,

	MAX_TYPE * 27,
		/// Do NOT insert any values here
		 MAX_TYPE * 28 - 1,

	MAX_TYPE * 30,			// BillingServer --> GameServer
		MSG_B2S_GET_BALANCE,
		MSG_B2S_ACCOUNT_LOCK,
		MSG_B2S_ACCOUNT_UNLOCK,
		MSG_B2S_ACCOUNT_UPDATED,



		//========================================================================================================



	 MAX_TYPE * 31,			// GameServer --> BillingServer
		MSG_S2B_SENDINFO,
		MSG_S2B_GET_BALANCE,
		MSG_S2B_ACCOUNT_LOCK,
		MSG_S2B_ACCOUNT_UNLOCK,
		MSG_S2B_ACCOUNT_LOCK_ROLLBACK,

		//========================================================================================================
		 MAX_TYPE * 31,			//! OtherServer --> AccountServer
		//========================================================================================================
		MSG_O2A + MAX_SUB * 0,	//! ��½��Ϣ
		// ----------------------------------------------------
		MSG_O2A_LOGIN_Affirm_Request,								//! ��½ȷ������
		MSG_O2A_LOGIN_SendOnlineInfo,								//! ���������˺���Ϣ


		MSG_O2A + MAX_SUB * 1,	//! �˺Ų�����Ϣ
		// ----------------------------------------------------
		MSG_O2A_ACCOUNT_Validate_Request,							//! �˺���֤����
		MSG_O2A_ACCOUNT_Reject_Request,								//! �������
		MSG_O2A_ACCOUNT_Logout,										//! ֪ͨAS�˺��˳�
		MSG_O2A_ACCOUNT_ListLogout,									//! ֪ͨASһ���˺��˳�
		MSG_O2A_SERVER_RUN_ERROR,									//! ������Ϸ���������д���





		//========================================================================================================
		MAX_TYPE * 32,			//! AccountServer --> OtherServer
		//========================================================================================================
	 MSG_A2O + MAX_SUB * 0,	//! ��½��Ϣ
		// ----------------------------------------------------
		MSG_A2O_LOGIN_Affirm_Re,									//! ��½ȷ�ϻ�Ӧ


	MSG_A2O + MAX_SUB * 1,	//! �˺Ų�����Ϣ
		// ----------------------------------------------------
		MSG_A2O_ACCOUNT_Validate_Re,								//! �˺���֤��Ӧ
		MSG_A2O_ACCOUNT_FcmTime_Notify,								//! ������ʣ��ʱ��֪ͨ
		MSG_A20_SECURITY_DATA,										//! �����˺ŵİ�ȫ����


		//========================================================================================================
		 MAX_TYPE * 33,			//! SCC --> SM
		//========================================================================================================

		//========================================================================================================
		MAX_TYPE * 34,			//! SM --> SCC
		//========================================================================================================

		//========================================================================================================
		 MAX_TYPE * 34,			//! SCC --> SUC
		//========================================================================================================

		//========================================================================================================
		MAX_TYPE * 35,			//! SUC --> SCC
		//========================================================================================================

		//////////////////////////////////////////////////////////////////////////
		// GameServer ---> BounsServer
		//////////////////////////////////////////////////////////////////////////
		MAX_TYPE * 36,
		MSG_S2BDS_SENDSERVERINFO,				// GameServer��Ϣ
		MSG_S2BDS_BOUNS_LOAD,					// ��ѯ������Ϣ
		MSG_S2BDS_BOUNS_GET,					// ��ȡ����
		MSG_S2BDS_BOUNS_ROLLBACK,				// ���ݻع�

		//////////////////////////////////////////////////////////////////////////
		// BounsServer ----> GameServer
		//////////////////////////////////////////////////////////////////////////
		MAX_TYPE * 37,
		MSG_BDS2S_BOUNS_LOAD_RESPOND,
		MSG_BDS2S_BOUNS_GET_RESPOND,
		MSG_BDS2S_BOUNS_DES_UPDATE,				// ���½���������Դ
	
};

