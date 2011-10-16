//======================================================================================
/**
*	@file		GoodsSetup.h
*	@data		
*	@author		
*/
//======================================================================================

#pragma once

#include "../Common/BaseDef.h"
#include "../Common/GoodsDef.h"

/// �˵����
/// ��ҪΪ��Դģ�� (�����ֲ���ģ��), ����WS��ռ��GS����������.
/// Ϊ��˵�����ͳһ���, ����ʹ��ͬһ��ö�� (��˳����ͬ).
enum MENU_INDEX
{
    // �����˵�                    ����(_L)
    LOAD_SETUP = MY_MENU * 2,      // ����
    LOAD_LOGSYS,                   // ��־
    LOAD_TEXTRES,                  // �ı���Դ
    LOAD_BROADCAST,                // ϵͳ�㲥
    LOAD_GLOBALSETUP,              // ȫ�ֲ���
    LOAD_GAMESETUP,                // ��Ϸ����
    LOAD_GMLIST,                   // GM�б�
    LOAD_SPEECH,                   // ���۹���
    LOAD_PLAYER,                   // ���
    LOAD_GOODSLIST,                // ԭʼ��Ʒ�б�
	LOAD_GOODSEX,                  // ��Ʒ��չ����
	LOAD_RELIVEGOODS,              // �������
	LOAD_INCREMENTSHOP,            // ��ֵ�̵�
    LOAD_NPCSHOP,                  // NPC�̵�
    LOAD_SKILL,                    // ����
	LOAD_HITLEVEL,                 // �����ȼ�
    LOAD_QUEST,                    // ����
    LOAD_MONSTER,                  // ����
    LOAD_MONSTERAI,                // ����AI
	LOAD_SCRIPTAI,                 // �ű�AI
    LOAD_PET,                      // ����
	LOAD_CONTACTS,		           // ����
	LOAD_SUIT,                     // ��װ
	LOAD_SCRIPT,                   // һ�������ű�
    LOAD_SCRIPTS,                  // ���нű�
	LOAD_GENVARS,                  // ͨ�ñ���
    LOAD_REGIONLIST,               // �����б�
    LOAD_REGIONSETUP,              // ��������
    LOAD_SYSMAIL,                  // ϵͳ�ʼ�
    LOAD_GENIUSSYS,                // �츳

    LOAD_FACTION = MY_MENU * 3,    // ���
	LOAD_GATES,                    // ����
	LOAD_COUNTRY,                  // ����
    LOAD_DOCCULIST,                // ��ְҵ�б�
	LOAD_GOODSTAILOR,			   // �����䷽
    LOAD_PLAYERMGR,                // ��ҹ���
    LOAD_ANTICHEAT,                // �����
    LOAD_PHASE,                    // ��λ
    LOAD_CONTRIBUTE,               // ����
    LOAD_AHCONF,                   // �ɱ�����
    LOAD_REPUTATION,               // ����
	LOAD_PROFESSION,               // רҵ����
    LOAD_INSTANCE,                 // ����
	LOAD_DEPOT,                    // �ֿ�
	LOAD_EMOTE,                    // ��������
	LOAD_ENCHASE,                  // ǿ��
	LOAD_BUSINESS,                 // ��ҵ
    LOAD_FBUSINESS,                // ��������
	LOAD_SPRITE,                   // ����

	SYNC_INCLIMIT = MY_MENU * 4,   // ��ֵ�̵깺������
    SYNC_REGION,                   // ��������
    SYNC_REGIONS,                  // ���г���
	SYNC_BRANCHRGN,			       // ���߳�������
	SYNC_INSTANCESELF,			   // ���˷��ݸ�������
	SYNC_INSTANCEDATA,	           // ����ģ��
    SYNC_SCRIPT,                   // �ű�
	SYNC_VARIABLES,                // ����
    SYNC_GSINFO,                   // GS��Ϣ
	SYNC_WSINFO,                   // WS��Ϣ
    SYNC_END,

    OPER_CONNECTLS = MY_MENU * 5,  // ����LoginServer
    OPER_CONNECTDBS,               // ����DBServer
    OPER_CONNECTLOGS,              // ����LogServer
    OPER_COLLECTPLAYERDATA,        // �ռ�GS����
    OPER_SAVENOW,                  // ���̴浱ǰ����
    OPER_SAVEDATA,                 // ��������
    OPER_KICKALL,                  // �߳��������
    OPER_SHOWSAVEINFO,             // ��ʾ������Ϣ����
};

/**
*	@class GlobalSetup
*
*	@brief ȫ��������
*/
class GlobalSetup
{
public:
	/// ����
	GlobalSetup(void);

	/// ����
	~GlobalSetup(void);

public:
	/// ���ýṹ����
	struct tagSetup
	{
		short	btMaxCharactersNum;				// ��ɫ�����
		// ���Ի���
		float	fStr2MaxAtk[OCC_Max];
		float	fStr2Burden[OCC_Max];
		float	fDex2MinAtk[OCC_Max];
		float	fDex2Stiff[OCC_Max];
		float	fCon2MaxHp[OCC_Max];
		float	fCon2Def[OCC_Max];
		float	fInt2Element[OCC_Max];
		float	fInt2MaxMp[OCC_Max];
		float	fInt2Resistant[OCC_Max];

		// ս����ʽ
		long	lBaseHit;						// ��������
		float	fBaseAtcHitcMagnify;			// ���м����Ŵ�ϵ��
		float	fHitLvRate;						// ���еĵȼ���Ӱ��
		long	lMinHitMonster;					// ������б���(����)
		long	lMaxHitMonster;					// �����������(����)
		long	lMinHit[OCC_Max];				// ������б���
		long	lMaxHit[OCC_Max];				// �����������
		float	fCriticalRate;					// ����һ�����˺�ϵ��
		float	fLossExp_Normal;				// ��ʧ����ֵ����(��ͨ����)
		float	fLossExp_Game;					// ��ʧ����ֵ����(��������)
		float	fLossExp_War;					// ��ʧ����ֵ����(��ս����)

		// ����Ʒ����(diedLosts[A][B][C]
        // AΪװ����ö��ID��BΪս��״̬����ͨ����ս����CΪPK״̬�������ơ��졢�ң�
		float diedLosts[EC_TOTAL][2][4]; 

		//��Ʒ��ֵ��
		float	fBasePriceRate;

		//��Ʒ�ۼ���
		FLOAT	fTradeInRate;

		//��Ʒ����۸�ϵ��
		float	fRepairFactor;

		long	lGoodsDurFray;
		long	lGoodsDurSpecialFray;

		
		long	lHeadWaste;
		long	lBodyWaste;
		long	lGloveWaste;
		long	lBootWaste;

		long	lBackWaste;
		long	lDecorateWaste;
		long	lHeadgearWaste;
		long	lTalismanWaste;
		long	lFrockWaste;
		long	lMedalWaste;
		long	lWingWaste;
		long	lManteauWaste;
		long	lFairyWaste;
		long	lNeckWaste;
		long	lRingWaste;
		long	lShieldWaste;		

		//��Ʒ�����׶�ϵ���ͳɹ�����
		WORD	wGoodsBlessPhase[3];
		float	fGoodsUpgradeOdds[4];

		// ����
		DWORD	dwGoodsDisappearTimer;			// ��Ʒ��ʧʱ��
		DWORD	dwGoodsProtectedTimer;			// ��Ʒ����ʱ��
		long	lResumeTimer;					// HP/MP�ָ�ʱ����
		long	lResumeFactor;					// �ָ�ϵ��
		float	fResumeFactorMini;				// ���׻ָ�ϵ��
		
		// ����ظ�ʱ����
		long	lMonsterResumeTimer;

		long	lFightStateTimer;				// ս��״̬����ʱ��
		long	lSinStateTimer;					// ����״̬����ʱ��
		DWORD	dwAttackMonsterProtectedTimer;	// ��������ı���ʱ��
		DWORD	dwMonsterKeepDeadTime;			//����ʬ�屣��ʱ��
		DWORD	dwCanCollectMonsterKeepDeadTime;//�ɲɼ�����ʬ�屣��ʱ��
		DWORD   dwBeneficiaryTime;				//�������汣��ʱ��
		DWORD   dwCollectionKeepDeadTime;		//�ɼ��ﱣ������״̬ʱ��
		DWORD	dwMonsterTransitionTime;		//���ﱣ�ַ���״̬ʱ��
		DWORD	dwMonsterKindlyTime;			//����ճ���ʱ���Ѻ�ʱ�䣬����������
		DWORD	dwReturnPeaceEffectTime;		//�ָ���ƽ״̬ʱ��
		float	fReturnPeaceSpeed;				//�ָ���ƽʱ��Ļ����ٶ�
		DWORD	dwReturnPeaceStandTime;			//�ָ���ƽʱ���վ��ʱ��
		DWORD	dwFightStateTime;				//����ң�ս��״̬����ʱ��
		DWORD	dwDHFQueueSize;					//����DHF(Distance and Hurted First)����ά����Ŀ�������󳤶�
		// ŭֵ������
		WORD	wBaseRp_Lv1;					// ��һ��ŭֵ���ֵȼ�
		WORD	wBaseRp_Lv2;					// ��һ��ŭֵ���ֵȼ�
		WORD	wBaseMaxRp_Lv1;					// ��һ��ŭֵ����
		WORD	wBaseMaxRp_Lv2;					// �ڶ���ŭֵ����
		WORD	wBaseAtcRp_Sword;				// ÿ�ι������ӵ�ŭֵ
		float	fRpAdd_Lv1[6];					// ����������ŭֵ����Ѫ����
		WORD	wRpAdd_Lv1[6];					// ����������ŭֵ��ֵ

		long	lHitBaseLv;						// (�����ȼ���)			5
		long	lHitTime;						// (����ʱ��-����)			10000
		float	fHitExpPrize;					// (��������ֵ����ϵ��)		0.1
		float	fMaxHitExpPrize;				// ���齱��ϵ�����ֵ		1.0
		DWORD	dwHitTopLogNum[12];				// ���������¼
		DWORD	dwHitTopLogPrizeExp[12];		// �ﵽ���������¼�Ľ�������ֵ

		// ����������������
		long	lHpLiveUp[OCC_Max];				//	���Hp��������
		long	lMpLiveUp[OCC_Max];				//	���Mp��������
		long	lBurdenLiveUp[OCC_Max];			//	�������������

		float	fExpScale;						// ��ȡ����ֵ�ñ���
		float	fExpScriptScale;				// �ű���ȡ����ֵ�ñ���
		float	fOccuExpScale;					// ��ȡְҵ�������
		float	fOccuExpScriptScale;			// �ű���ȡְҵ�������
		float	fSpScale;						// ��ȡ���ܵ����ı���
		float	fSpScriptScale;					// �ű���ȡ���ܵ����ı���
		float	fMonsterDropGoodsScale;			// ���ﱬ��Ʒ�ı���

		//���;������
		DWORD	dwPresentExpCooldown;			// ���;�����ȴʱ��(����)
		float	fPresentExpScaleInPersonalHouse;// ���˷��ݸ��������;���ϵ��
		float	fPresentExpScaleInSafeArea;		// ��ȫ�����;���ϵ��
		float	fPresentExpScale;				// �������;���ϵ��
		long	lPresentExpBuffId;				// ��ʾ���;����buffid
		long	lPresentExpBuffLvl;				// ��ʾ���;����bufflevel


		//##Ӳֱϵͳ
		WORD	wNumStiffSetup;					// ���ټ�
		FLOAT	fStiff_Damage[4];				// �˺�
		WORD	wStiff_Probability[4];			// ����
		DWORD	dwStiffDelayTime;				// ÿ��Ӳֱ���ӳ�ʱ��
		DWORD	dwStiffBoundTime;				// ��λʱ�䳤��
		DWORD	dwStiffLimit;					// ÿ��λʱ������������Ӳֱ����

		//##ɱ��������ʱ��
		DWORD	dwCriminalTime;
		DWORD	dwMurdererTime;					//����
		DWORD	dwRipperTime;					//����
		DWORD	dwPkCountPerKill;				// ɱһ�����ӵ�PKֵ
		DWORD   dwPkCountPerAttack;				//����һ�����ӵ�PKֵ
		DWORD	dwOnePkCountTime;				// ����һ��PKֵ��ʱ��
		DWORD	dwActTime;						//ʩ�Ŷ���ʱ��
		DWORD   dwHpMpRestoreTime;				//�Զ��ָ�HpMp�ļ��ʱ��
		long	lPostage[MAX_MAIL];				//����
		long	lMaxName;						//������������
		long	lMaxSubject;					//�����������
		long    lMaxText;						//�ʼ������������
		long    lMaxExText;						//�ʼ������ı����������
		long	lMaxMoney;						//����Ǯ
		float	fMaxRemainTime[MAX_MAIL];		//�ʼ���󱣴�ʱ��
		long	lPrisonIslandPkValue;			//��������PKֵ
		long	lMailDelayTime;					//�ʼ�������ʱʱ��
		long	lBaseModuleTime;				//����״̬��ʱ���ʱ��
		long    lTotalMaxSkillNum;				//�ܼ���Խ�����
		float   fOneSecMaxSkillNum;				//ÿ�뼼��Խ�����

		long    lTotalMaxNpcScriptsNum;			//��NPC�ű�Խ�����
		float   fOneSecMaxNpcScriptsNum;		//ÿ��NPC�ű�Խ�����

		//��Ǯ����
		DWORD   dwGoldGoodsId;
		//��������
		DWORD   dwSilverGoodsId;

		//λ�澫������
		DWORD   dwWeimianGoodsId;

		DWORD   dwNpcSellGoldToSilver;

		//##��Ǯ����
		DWORD	dwGoldCoinLimit;
		//��������
		DWORD   dwSilverLimit;
		//��ȯ����
		DWORD   dwTicketLimit;

		//##���߼�Ѫ�����ʱ������
		DWORD	dwHealTimeInterval;

		//##���߼�MP�����ʱ������
		DWORD	dwMpTimeInterval;

		//ʰȡ��Ʒʱ����
		DWORD   dwPickGoodsInterval;

		float	fMonsterNumScale;				// ����������������
		bool	bAllowClientRunScript;			// �Ƿ�����clientִ�нű�
		bool	bAllowClientChangePos;			// �Ƿ�����clientִ��˲��

		//! ��ʾ��ż���ֵĲ���ֵ
		long	lShowSpouseNameStep;

		//ϵͳ���̵���
		DWORD	dwSavePointTime;

		long	lAreaWidth;						// �����С���ӿ�
		long	lAreaHeight;					// �����С���Ӹ�

		DWORD   dwDelDays;						// n�������ɾ��
		char	strSpeStr[16];					// ����������������뺬�е��ַ���
		long	lRegionChatLevelLimit;			// ���������ĵȼ�����

		DWORD   lOfferExpPercent;				// ��ṱ���߹��׾���ı���
		float	fDecTimeParam;					// ����״̬�ܹ�������ʱ��ϵ��
		long	lDiedStateTime;					// ����״̬ʱ��S


		//##PK�˺�����
		FLOAT			m_fPvPDamageFactor;

		//##���˵ȼ�����
		DWORD			m_btNewbieLevelLimit;
		DWORD			m_btNewSoldierLevel;

		// �±������ȼ�


		//����ǿ������
		FLOAT			m_fWeaponPowerModifier;

		//����ǿ����С����ֵ��
		FLOAT			m_fWeaponMinPowerModifier;

		// ����ʱ����
		DWORD			m_dwCard6Hours;
		DWORD			m_dwCard12Hours;
		DWORD			m_dwCard24Hours;

		// ����ʱ��������
		DWORD			m_dwCard6HoursChargePrice;
		DWORD			m_dwCard12HoursChargePrice;
		DWORD			m_dwCard24HoursChargePrice;

		// ���۽��������
		DWORD			m_dwCardLowerLimitPrice;
		DWORD			m_dwCardUpperLimitPrice;
		

		// �ʺ��µ�����������
		DWORD			m_dwCardMaxNum;
		// ���������� 
		DWORD			m_dwCardWorldMaxNum;

		// ���۳��Դ���
		DWORD			m_dwCardRetryNum;
		// �ܾ�����ʱ��
		DWORD			m_dwCardDenyTime;

		float	fBlastAttackScale;				// �﹥�����Ŵ�ϵ��
		float	fBlastDefendScale;				// �������˥��ϵ��
		float	fElementBlastAttackScale;		// ���������Ŵ�ϵ��
		float	fElementBlastDefendScale;		// ��������˥��ϵ��
		float	fElementFullMissScale;			// ��������˥��ϵ��
		
		char	szTalkWorldGoodsName[32];		// ���纰����Ʒԭʼ��
		int		nTalkWorldGoodsNum;				// ���纰����Ʒ����
		long	lTalkWorldGold;					// ���纰���������
		long	lTalkWorldSilver;				// ���纰����������

		char	szTalkUnionGoodsName[32];		// ��Ӫ������Ʒԭʼ��
		int		nTalkUnionGoodsNum;				// ��Ӫ������Ʒ����
		long	lTalkUnionGold;					// ��Ӫ������Ǯ����
		long	lTalkUnionSilver;				// ��Ӫ������������

		char	szTalkCountryGoodsName[32];		// ���Һ�����Ʒԭʼ��
		int		nTalkCountryGoodsNum;			// ���Һ�����Ʒ����
		long	lTalkCountryGold;				// ���Һ�����Ǯ����
		long	lTalkCountrySilver;				// ���Һ�����������

		char	szTalkTopWorldGoodsName[32];	// �ö����纰����Ʒԭʼ��
		int		nTalkTopWorldGoodsNum;			// �ö����纰����Ʒ����
		long	lTalkTopWorldGold;				// �ö����纰���������
		long	lTalkTopWorldSilver;			// �ö����纰����������

		char	szReliveGoodsName[32];			//ԭ�ظ���ԭʼ��
		long	lRelivePrice;					//ԭ�ظ���۸�

		float	fPlayerWalkSpeed;				// ������ٶ�<����/ÿ����>
		float	fPlayerRunSpeed;				// ������ٶ�<����/ÿ����>

		float   fRidePlayerWalkSpeed;			// ���������ٶ�
		float   fRidePlayerRunSpeed;			// ���������ٶ�

		//�ۻ����ȡ���鹫ʽ����ɫ���ջ�ȡ����ֵ����n1/100����ɫ����ȼ�/n2����n1�ݶ�=10��n2�ݶ���1428��n1��n2��������ȫ�ֲ����������á�
		long	lExpBallFactor1;				// �������ȡ�������1
		long	lExpBallFactor2;				// �������ȡ�������2

		DWORD	dwAutoProtectTime;	
		DWORD	dwIncrementLogDays;
		long	lMaxHelpHintNum;				//��������ʾ����

		//��ҵ�½ʱ�򴥷��Ľű�
		char	pszPlayerLoginScript[MAX_PATH];
		//�������ʱ�����Ľű�
		char	pszPlayerDiedScript[MAX_PATH];
		//�������ʱ�򴥷��Ľű�
		char	pszPlayerUpLevelScript[MAX_PATH];
		//תְʱ�����ű�
		char	pszPlayerChangeOccuScript[MAX_PATH];
		//���PVPɱ��ʱ�����ű�
		char	pszPVPKilled[MAX_PATH];
		//���PVP��ɱʱ�����ű�
		char	pszPVPBeKilled[MAX_PATH];
		//���PKֵΪ��ʱ�����Ľű�
		char	pszConValueNegative[MAX_PATH];
		//���PKֵΪ�Ǹ�ʱ�����Ľű�
		char	pszConValueNotNegative[MAX_PATH];
		 //��Ҹ���
		char	szReliveScriptName[MAX_PATH];
		//�ճ̱�
		char	pszCalendar[MAX_PATH];
		//����άħ��
		char	pszMagicBox[MAX_PATH];
		//�ʴ��뽱��
		char	pszAnswerRequest[MAX_PATH];
		//�л�����
		char	pszChangeLineRgn[MAX_PATH];

		//! ��������
		long	lCorrelativeSkillID;			//����ӹ����ļ���ID
		long	lMinSkillLevel;					//������ӵ���С���ܵȼ�
		long	lMinTradeLevel;					//���Խ�����С�ȼ�
		long	new_lGoodsAllot;				//Ĭ�Ϸ��䷽ʽ
		long	new_lMaxMemberNum;				//Ĭ���������
		long	new_lSessionTimeoutS;			//�Ự��ʱ����
		float	new_fIncreasedExp_2;			//2�˾���ӳɱ���
		float	new_fIncreasedExp_3;			//3�˾���ӳɱ���
		float	new_fIncreasedExp_4;			//4�˾���ӳɱ���
		float	new_fIncreasedExp_5;			//5�˾���ӳɱ���
		float	new_fIncreasedExp_6;			//6�˾���ӳɱ���
		long	new_lExperienceRadii;			//�������뾶����λ����
		long	new_lExpDiff;					//�����
		long	new_lExpLimit;					//��Ҿ���ֵ�ݶ��������
		float	new_fExpAmerce;					//����ͷ�ϵ��
		float	new_fExpHortation;				//���齱��ϵ��
		float	new_fExpMinLvlDiff;				//���齱����ʼ����С�ȼ���
		
		float	vigour_fIncreasedExp_2;			//2��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_3;			//3��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_4;			//4��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_5;			//5��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_6;			//6��Ԫ���ӳɱ���
		long	vigour_lExpDiff;				//Ԫ����
		long	vigour_lExpLimit;				//Ԫ������ֵ�ݶ��������
		float	vigour_fExpAmerce;				//Ԫ���ͷ�ϵ��
		float	vigour_fExpHortation;			//Ԫ������ϵ��
		float	vigour_fExpMinLvlDiff;			//Ԫ��������ʼ����С�ȼ���
		float	vigour_fOccExpSpRatio;			//ְҵ�����SP��ֵ

		//������ȴʱ��
		long	lCommoCoolDownTime;
		//pk/pvp����ֵ�趨
		float	fPkToPvpValue;
		//��PK/PVP�����ýű�
		char	pszPKToPvPScript[MAX_PATH];

		//���Ʒ���Ϣ������������
		short	lSendMsgLimitPlayerNum;
		//���Ʒ���Ϣʱ��������
		short	lSendMsgLimitDistance;
		//���Ʒ�������Ϣʱ����������
		short	lSendSkillMsgLimitPlayerNum;
		//�������Ե���Χ���ӳ�ʱ��
		long	lUpdateOtherPropertyDelayTime;
		//����AddShape��Ϣʱ�ļ��ʱ��
		long	lSendAddShapeMsgInteTime;
		//��������AddShape��Ϣ�ľ�������
		long	lSendAddSahpeMsgDistance;
		//��֤�ƶ���Ϣ��������
		long	lCheckMsgCount;
		//�������ÿ�뷢���ƶ���Ϣ�ĸ���
		float	fMaxMsgCountPerSecond;
		//��֤�ƶ�ʱ��ƽ���ƶ����ٶ������������
		float	fMaxMistakeAverSpeed;
		//��֤�ƶ��ٶȵ�ʱ������ٶȵ����
		float	fMaxMistakeMaxSpeed;
		//Ping���Ƽ��ʱ��(����)
		long	lPingChekTime;
		//�ͻ���Ping������ʱ��(����)
		long	lPingErrorBanTime;
		//���������ʱ(����)
		long	lReLiveTime;
		// ����ʱ�������룩
		long	lWarningDeltaTime;
		// ������Ʋ������ͼ��ʱ��(����)
		long	lOperBounsTime;
       
        
        ///  ����ֵ�½�ʱ��
        long    lDistractTime;
        /// ÿ���½�����
        long    lDistractValue;
        /// ʹ�ü�������������
        long    lAddDistractValue;
        /// ����ʹ�ü���ʱ��
        long    lInterUseSkillTime;
        /// ����ʹ�ü�����������ֵ
        long    lAddInterDistractValue;
        /// �������������
        long    lDryCount;
        /// ��״̬��������ֵ
        long    lDryValue;
        /// ��������ʱ��
        long    lDryTime;

		
		//! ��Ϸ��������������
		struct ObjAllotPolicy
		{
			long	ObjNum;			//ÿԤ����һ���ڴ棬�����Ķ�������
			long	blockNum;	//��ʼ�����ڴ��ǣ�Ԥ����Ŀ���
			ObjAllotPolicy():ObjNum(0),blockNum(0){}
		};
		ObjAllotPolicy Goods_Policy;			//!��Ʒ
		ObjAllotPolicy Monster_Policy;			//!����
		ObjAllotPolicy Collection_Policy;		//!�ɼ���
		ObjAllotPolicy Npc_Policy;				//!NPC
		ObjAllotPolicy Player_Policy;			//!���
		ObjAllotPolicy Region_Policy;			//!����
		ObjAllotPolicy SummonedCreature_Policy;	//!�ٻ���
		ObjAllotPolicy Build_Policy;			//!����
		ObjAllotPolicy Citygate_Policy;			//!����
		
		long	lTimerVarNum;					//Ԥ���䶨ʱ������������
		long    lPreAllocedScriptNum;			// Ԥ����Ľű��ڴ������
		long    lMaxPreAllocedScriptNum;		// Ԥ����Ľű��ڴ���������
		long    lDupRgnQueueNum;				// ���������Ŷ���
		long    lPDupRgnMaxNum;					// �������˸��������
		long    lPHDupRgnMaxNum;				// �������˷��ݸ��������
		long    lTDupRgnMaxNum;					// ������Ӹ��������
		long    lDupRgnQueueDeltaTime;			// ���������Ŷ�ʱ����
		long    lDupRgnQueueMaxNum;				// ���������Ŷ������
		long	lPDupRgnDelTime;				// ���˸���ɾ��ʱ��
		long	lCreatePDupRgnMaxNum;			//��Ҵ������˸��������
		long	lCreateTDupRgnMaxNum;			//��Ҵ�����Ӹ��������
		long	lCreateDupRgnDeltaTime;			//��Ҵ�������ʱ����
		long    lSaveTime;						// ��ұ������ݵ�ʱ����
		long	lWSSaveTime;					// WS�������ݵ�ʱ����

		long	lMaxGoodInfoNum;				//���[��������Ʒ��Ϣ]��������

		long	lReliveDebuffId;				// ��Ҹ���DebuffID
		long	lReliveDebuffLevel;				// ��Ҹ���DebuffLevel

		long	lCreateRgnPlayerLoadMax;		//GS���������������������
		long	lCreateRgnMonsterLoadMax;		//GS����������������������
		long    lPDupRgnMaxPlayerNum;			// ���˸����������
		long    lPHDupRgnMaxPlayerNum;			// ���˷��ݸ����������
		long    lTDupRgnMaxPlayerNum;			// ��Ӹ����������
		long	lRgnKickPlayerDeltaTime;		// ��������ʱ��
		long    lMLevelEnablePlayerSearch;		// ���ݵȼ����������ҹ���

		char	szHideName[32];					// ���������
		ULONG	uUpgradeTo10FontColor;			//! �����ɹ�����10���ϣ�������ʾ��������ɫ
		ULONG	uUpgradeTo20BackIdx;			//! �����ɹ�����10���ϣ��������ʾ�ı�������
		ULONG	uUpgrade10LostFontColor;		//! 10������������ʧ�ܣ�������ʾ��������ɫ
	};

    /// �ƶ��ж�/ͬ������
    struct MoveCheckConf
    {
        int reqMax; ///< ������Ϣ�����������ڴ�ֵ���
        float disPerStep; ///< ��������ÿ���ƶ��ľ���
        float syncMaxDis; ///< ͬ��ʱ��������
        float errMaxDis;
        float errMinDis;
        float reqMaxDis;
    };

private:
	/// ȫ�����ýṹ
	static tagSetup	m_stSetup;

    static MoveCheckConf m_moveConf;
public:
	/// ���ļ�����ȫ������
	static bool Load(const char* filename);

	/// ����Setup/gamesetup.ini����
	static bool LoadGameSetup(const char* filename);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ȡ��ȫ�����ñ�
	static tagSetup* GetSetup()	{return &m_stSetup;}

	/// ��ȡ���ŭֵ����
	static WORD GetBaseMaxRp(uchar lOccupation, uchar lLevel);

    static bool LoadMoveConf( stringstream &stream );

    static const MoveCheckConf &MoveConf()  { return m_moveConf; }

    /// �����֮��ɫϵͳ
    static bool LoadBuddhism( stringstream& stream );
};
