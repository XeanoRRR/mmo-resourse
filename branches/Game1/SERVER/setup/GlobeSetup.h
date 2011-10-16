#pragma once
#include "PlayerList.h"

// ���ñ��
enum SETUP_INDEX
{
	SI_GOODSLIST,
	SI_SUITSETUP,
	SI_ENCHASESETUP,
	SI_TRADEAREASETUP,
	SI_PLAYERLIST,
	SI_MONSTERLIST,
	SI_TRADELIST,
	SI_INCREMENTSHOPLIST,
	SI_INCLIMITGOODSREC,
	SI_CONTRIBUTEITEM,
	SI_SKILLLIST,	
	SI_GLOBESETUP,
	SI_LOGSYSTEM,
	SI_GMLIST,
	SI_FUNCTIONLIST,
	SI_VARIABLELIST,
	SI_GENERALVARIABLELIST,
	SI_SCRIPTFILE,
	SI_REGION,
	SI_PROXY_REGION,
	SI_REGIONSETUP,
	SI_REGIONLEVELSETUP,
	SI_REGIONRELOAD,
	SI_GOODSEX,
	SI_DEPOT,
	SI_LINKMAN, //! ����ϵͳ
	SI_GAMESERVERINDEX,
	SI_HELP,
	SI_HITLEVEL,
	SI_EMOTION,
	SI_QUEST,
	SI_PLAYERRANKS,
	SI_COUNTRYPARAM,
	SI_COUNTRY,
	SI_DUPLIREGIONSETUP,
	SI_ATTACKCITYSYS_SETUP,
	SI_STATELIST,
	SI_PHDUPRGN, // ���˷��ݸ�������
	SI_DEPUTYOCCU,
	SI_GAMESERVER_SETUP, // GS��������
	SI_GLOBALSTRING,
	SI_SPRITE,
	SI_SensitiveWord,	//���д�
	SI_SYSMAIL,			//ϵͳ�ʼ�
	SI_SCRIPTAI,
	SI_GATELIST,
	SI_PLAYER_MAN,				// Fox@20081120 ��ҹ���
	SI_PET_LIST,				// Fox@20081119 ����
	SI_LINED_RGN_INFO,			// ���߳�������
	SI_GOODSTAILOR,				//! �Ǹ�ְҵ�����䷽
	SI_DUPRGNDATA,				// ������������
	SI_WS_INFO,
	SI_ANTICHEAT,               // BaiYun@2009525 �����
	SI_RELIVEGOODS,				//������Ʒ
	SI_CREDITSYSTEM,			// ��������
	SI_FBUSINESS,				// ������������
	SI_COUNTTIMERLIST,			// ��Ҷ�ʱ������
	SI_COSMESYSTEM,				// ����ϵͳ����
    SI_QUESTINDEX,              // ������������
	SI_Lottery,                 // ��Ʊϵͳ���� 
	SI_END
};

// ȫ�ֲ���
class CGlobeSetup
{
public:
	CGlobeSetup(void);
	virtual ~CGlobeSetup(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		short	btMaxCharactersNum;		// ��ɫ�����
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
		long	lMinHit[OCC_Max];	// ������б���
		long	lMaxHit[OCC_Max];	// �����������
		float	fCriticalRate;					// ����һ�����˺�ϵ��
		float	fLossExp_Normal;				// ��ʧ����ֵ����(��ͨ����)
		float	fLossExp_Game;					// ��ʧ����ֵ����(��������)
		float	fLossExp_War;					// ��ʧ����ֵ����(��ս����)

		// ����Ʒ����(�����ơ��졢��)[0][]:��ͨ [1][]:��ս		
		float fDropEquip_Head[2][4];//(ͷ���������)
		float fDropEquip_Necklace[2][4];//(�����������)	
		float fDropEquip_Wing[2][4];//(������)	
		float fDropEquip_Body[2][4];//(���׵������)	
		float fDropEquip_Back[2][4];//(�����������)	
		float fDropEquip_Glove[2][4];//(���׵������)	
		float fDropEquip_Boot[2][4];//(Ь�ӵ������)	
		float fDropEquip_Headgear[2][4];//(ͷ�ε������)
		float fDropEquip_Frock[2][4];//(���׵������)	
		float fDropEquip_LRing[2][4];//(���ָ�������)
		float fDropEquip_RRing[2][4];//(�ҽ�ָ�������)
		float fDropEquip_Medal1[2][4];//(ѫ��1�������)
		float fDropEquip_Medal2[2][4];//(ѫ��2�������)
		float fDropEquip_Medal3[2][4];//(ѫ��3�������)
		float fDropEquip_Fairy[2][4];//(С����������)
		float fDropEquip_Weapon[2][4];//(�����������)
		float fDropEquip_Weapon2[2][4];//(���������������)	
		float fDropEquip_Cloak[2][4];//(����������)	
		float fDropEquip_Horse[2][4];//(����������)
		float fDropEquip_Bag[2][4];//(������Ʒ�������)
		float fDropEquip_Money[2][4];//(��Ǯ�������)	
		float fDropEquip_MoneyPercent[2][4];//(����Ǯ��������)

		//��Ʒ��ֵ��
		float	fBasePriceRate;

		//��Ʒ�ۼ���
		FLOAT	fTradeInRate;

		//��Ʒ����۸�ϵ��
		float	fRepairFactor;
		//��Ʒ�־�Ϊ0ʱ������۸�ϵ��.
		float   fRepairFactor2;

		LONG	lGoodsDurFray;
		LONG	lGoodsDurSpecialFray;

		
		LONG	lHeadWaste;
		LONG	lBodyWaste;
		LONG	lGloveWaste;
		LONG	lBootWaste;

		LONG	lBackWaste;
		LONG	lDecorateWaste;
		LONG	lHeadgearWaste;
		LONG	lTalismanWaste;
		LONG	lFrockWaste;
		LONG	lMedalWaste;
		LONG	lWingWaste;
		LONG	lManteauWaste;
		LONG	lFairyWaste;
		LONG	lNeckWaste;
		LONG	lRingWaste;
		LONG	lShieldWaste;		

		//��Ʒ�����׶�ϵ���ͳɹ�����
		WORD	wGoodsBlessPhase[3];
		float	fGoodsUpgradeOdds[4];

		// ����
		DWORD	dwGoodsDisappearTimer;		// ��Ʒ��ʧʱ��
		DWORD	dwGoodsProtectedTimer;		// ��Ʒ����ʱ��
		long	lResumeTimer;				// HP/MP�ָ�ʱ����
		long	lResumeFactor;				// �ָ�ϵ��
		float	fResumeFactorMini;			// ���׻ָ�ϵ��
		
		// 2006��4��28�� bugs ����ظ�ʱ����
		long	lMonsterResumeTimer;

		long	lFightStateTimer;			// ս��״̬����ʱ��
		long	lSinStateTimer;				// ����״̬����ʱ��
		DWORD	dwAttackMonsterProtectedTimer;	// ��������ı���ʱ��
		DWORD	dwMonsterKeepDeadTime;				//����ʬ�屣��ʱ��
		DWORD	dwCanCollectMonsterKeepDeadTime;	//�ɲɼ�����ʬ�屣��ʱ��
		DWORD   dwBeneficiaryTime;					//�������汣��ʱ��
		DWORD   dwCollectionKeepDeadTime;			//�ɼ��ﱣ������״̬ʱ��
		DWORD	dwMonsterTransitionTime;			//���ﱣ�ַ���״̬ʱ��
		DWORD	dwMonsterKindlyTime;				//����ճ���ʱ���Ѻ�ʱ�䣬����������
		DWORD	dwReturnPeaceEffectTime;			//�ָ���ƽ״̬ʱ��
		float	fReturnPeaceSpeed;					//�ָ���ƽʱ��Ļ����ٶ�
		DWORD	dwReturnPeaceStandTime;				//�ָ���ƽʱ���վ��ʱ��
		DWORD	dwFightStateTime;					//����ң�ս��״̬����ʱ��
		DWORD	dwDHFQueueSize;						//����DHF(Distance and Hurted First)����ά����Ŀ�������󳤶�
		// ŭֵ������
		WORD	wBaseRp_Lv1;		// ��һ��ŭֵ���ֵȼ�
		WORD	wBaseRp_Lv2;		// ��һ��ŭֵ���ֵȼ�
		WORD	wBaseMaxRp_Lv1;		// ��һ��ŭֵ����
		WORD	wBaseMaxRp_Lv2;		// �ڶ���ŭֵ����
		WORD	wBaseAtcRp_Sword;	// ÿ�ι������ӵ�ŭֵ
		float	fRpAdd_Lv1[6];		// ����������ŭֵ����Ѫ����
		WORD	wRpAdd_Lv1[6];		// ����������ŭֵ��ֵ

		long	lHitBaseLv;			// (�����ȼ���)			5
		long	lHitTime;			// (����ʱ��-����)			10000
		float	fHitExpPrize;		// (��������ֵ����ϵ��)		0.1
		float	fMaxHitExpPrize;	// ���齱��ϵ�����ֵ		1.0
		DWORD	dwHitTopLogNum[12];			// ���������¼
		DWORD	dwHitTopLogPrizeExp[12];	// �ﵽ���������¼�Ľ�������ֵ

		// ����������������
		long	lHpLiveUp[OCC_Max];		//	���Hp��������
		long	lMpLiveUp[OCC_Max];		//	���Mp��������
		long	lBurdenLiveUp[OCC_Max];	//	�������������

		float	fExpScale;				// ��ȡ����ֵ�ñ���
		float	fExpScriptScale;		// �ű���ȡ����ֵ�ñ���
		float	fOccuExpScale;			// ��ȡְҵ�������
		float	fOccuExpScriptScale;	// �ű���ȡְҵ�������
		float	fSpScale;				// ��ȡ���ܵ����ı���
		float	fSpScriptScale;			// �ű���ȡ���ܵ����ı���
		float	fMonsterDropGoodsScale;	//���ﱬ��Ʒ�ı���

		//���;������
		DWORD	dwPresentExpCooldown;	//���;�����ȴʱ��(����)
		float	fPresentExpScaleInPersonalHouse;//���˷��ݸ��������;���ϵ��
		float	fPresentExpScaleInSafeArea;//��ȫ�����;���ϵ��
		float	fPresentExpScale;//�������;���ϵ��
		long	lPresentExpBuffId;//��ʾ���;����buffid
		long	lPresentExpBuffLvl;//��ʾ���;����bufflevel


		//##Ӳֱϵͳ
		WORD	wNumStiffSetup;						//##���ټ�
		FLOAT	fStiff_Damage[4];					//##�˺�
		WORD	wStiff_Probability[4];				//##����
		DWORD	dwStiffDelayTime;					//##ÿ��Ӳֱ���ӳ�ʱ��
		DWORD	dwStiffBoundTime;					//##��λʱ�䳤��
		DWORD	dwStiffLimit;						//##ÿ��λʱ������������Ӳֱ����

		//##ɱ��������ʱ��
		DWORD	dwCriminalTime;
		DWORD	dwMurdererTime;	//����
		DWORD	dwRipperTime;	//����
		DWORD	dwPkCountPerKill;	// ɱһ�����ӵ�PKֵ
		DWORD   dwPkCountPerAttack;	//����һ�����ӵ�PKֵ
		DWORD	dwOnePkCountTime;	// ����һ��PKֵ��ʱ��
		DWORD	dwActTime;			//ʩ�Ŷ���ʱ��
		DWORD   dwHpMpRestoreTime;	//�Զ��ָ�HpMp�ļ��ʱ��
		long	lPostage[MAX_MAIL]; //����
		long	lMaxName;			//������������
		long	lMaxSubject;		//�����������
		long    lMaxText;			//�ʼ������������
		long    lMaxExText;			//�ʼ������ı����������
		long	lMaxMoney;			//����Ǯ
		float	fMaxRemainTime[MAX_MAIL];		//�ʼ���󱣴�ʱ��
		long	lPrisonIslandPkValue;//��������PKֵ
		long	lMailDelayTime;		 //�ʼ�������ʱʱ��
		long	lBaseModuleTime;	//����״̬��ʱ���ʱ��
		long    lTotalMaxSkillNum;	//�ܼ���Խ�����
		float   fOneSecMaxSkillNum;	//ÿ�뼼��Խ�����

		long    lTotalMaxNpcScriptsNum;	//��NPC�ű�Խ�����
		float   fOneSecMaxNpcScriptsNum;	//ÿ��NPC�ű�Խ�����
		//long	lMaxMailGroup;		 //Ⱥ�������˵��������

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

		float	fMonsterNumScale;		// ����������������
		bool	bAllowClientRunScript;	// �Ƿ�����clientִ�нű�
		bool	bAllowClientChangePos;	// �Ƿ�����clientִ��˲��

		//! ��ʾ��ż���ֵĲ���ֵ
		long	lShowSpouseNameStep;

		//ϵͳ���̵���
		DWORD	dwSavePointTime;

		long	lAreaWidth;				// �����С���ӿ�
		long	lAreaHeight;			// �����С���Ӹ�

		DWORD   dwDelDays;				// n�������ɾ��
		DWORD	dwDelLevelCondition;	// ɾ������ĵȼ�����
		char	strSpeStr[16];			// ����������������뺬�е��ַ���
		long	lRegionChatLevelLimit;	// ���������ĵȼ�����

		DWORD   lOfferExpPercent;	// ��ṱ���߹��׾���ı���
		float	fDecTimeParam;		// ����״̬�ܹ�������ʱ��ϵ��
		long	lDiedStateTime;		// ����״̬ʱ��S


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

		float	fBlastAttackScale;			// �﹥�����Ŵ�ϵ��
		float	fBlastDefendScale;			// �������˥��ϵ��
		float	fElementBlastAttackScale;	// ���������Ŵ�ϵ��
		float	fElementBlastDefendScale;	// ��������˥��ϵ��
		float	fElementFullMissScale;		// ��������˥��ϵ��
		
		char	szTalkWorldGoodsName[32];	// ���纰����Ʒԭʼ��
		int		nTalkWorldGoodsNum;			// ���纰����Ʒ����
		LONG	lTalkWorldGold;				// ���纰���������
		LONG	lTalkWorldSilver;			// ���纰����������

		char	szTalkUnionGoodsName[32];	// ��Ӫ������Ʒԭʼ��
		int		nTalkUnionGoodsNum;			// ��Ӫ������Ʒ����
		LONG	lTalkUnionGold;				// ��Ӫ������Ǯ����
		LONG	lTalkUnionSilver;			// ��Ӫ������������

		char	szTalkCountryGoodsName[32];	// ���Һ�����Ʒԭʼ��
		int		nTalkCountryGoodsNum;		// ���Һ�����Ʒ����
		LONG	lTalkCountryGold;			// ���Һ�����Ǯ����
		LONG	lTalkCountrySilver;			// ���Һ�����������

		char	szTalkTopWorldGoodsName[32];	// �ö����纰����Ʒԭʼ��
		int		nTalkTopWorldGoodsNum;			// �ö����纰����Ʒ����
		LONG	lTalkTopWorldGold;				// �ö����纰���������
		LONG	lTalkTopWorldSilver;			// �ö����纰����������

		char	szReliveGoodsName[32];	//ԭ�ظ���ԭʼ��
		LONG	lRelivePrice;			//ԭ�ظ���۸�

		float	fPlayerWalkSpeed;			// ������ٶ�<����/ÿ����>
		float	fPlayerRunSpeed;			// ������ٶ�<����/ÿ����>

		float   fRidePlayerWalkSpeed;		// ���������ٶ�
		float   fRidePlayerRunSpeed;		// ���������ٶ�

		//�ۻ����ȡ���鹫ʽ����ɫ���ջ�ȡ����ֵ����n1/100����ɫ����ȼ�/n2����n1�ݶ�=10��n2�ݶ���1428��n1��n2��������ȫ�ֲ����������á�
		long	lExpBallFactor1;			// �������ȡ�������1
		long	lExpBallFactor2;			// �������ȡ�������2

		DWORD	dwAutoProtectTime;	
		DWORD	dwIncrementLogDays;
		long	lMaxHelpHintNum;		//��������ʾ����

		//��ҵ�½ʱ�򴥷��Ľű�
		char	pszPlayerLoginScript[MAX_PATH];
		//�������ʱ�����Ľű�
		char	pszPlayerDiedScript[MAX_PATH];
		//�������ʱ�򴥷��Ľű�
		char	pszPlayerUpLevelScript[MAX_PATH];
		//���ʹ������ʱ�����Ľű�
		char    pszPlayerUseDiamondScript[MAX_PATH];
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
		//����
		char	pszHelpRequest[MAX_PATH];

		//! ��������
		long	lCorrelativeSkillID;		//����ӹ����ļ���ID
		long	lMinSkillLevel;				//������ӵ���С���ܵȼ�
		long	lMinTradeLevel;				//���Խ�����С�ȼ�
		long	new_lGoodsAllot;			//Ĭ�Ϸ��䷽ʽ
		long	new_lMaxMemberNum;			//Ĭ���������
		long	new_lSessionTimeoutS;		//�Ự��ʱ����
		float	new_fIncreasedExp_2;		//2�˾���ӳɱ���
		float	new_fIncreasedExp_3;		//3�˾���ӳɱ���
		float	new_fIncreasedExp_4;		//4�˾���ӳɱ���
		float	new_fIncreasedExp_5;		//5�˾���ӳɱ���
		float	new_fIncreasedExp_6;		//6�˾���ӳɱ���
		long	new_lExperienceRadii;		//�������뾶����λ����
		long	new_lExpDiff;				//�����
		long	new_lExpLimit;				//��Ҿ���ֵ�ݶ��������
		float	new_fExpAmerce;				//����ͷ�ϵ��
		float	new_fExpHortation;			//���齱��ϵ��
		float	new_fExpMinLvlDiff;			//���齱����ʼ����С�ȼ���
		
		float	vigour_fIncreasedExp_2;		//2��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_3;		//3��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_4;		//4��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_5;		//5��Ԫ���ӳɱ���
		float	vigour_fIncreasedExp_6;		//6��Ԫ���ӳɱ���
		long	vigour_lExpDiff;			//Ԫ����
		long	vigour_lExpLimit;			//Ԫ������ֵ�ݶ��������
		float	vigour_fExpAmerce;			//Ԫ���ͷ�ϵ��
		float	vigour_fExpHortation;		//Ԫ������ϵ��
		float	vigour_fExpMinLvlDiff;		//Ԫ��������ʼ����С�ȼ���
		float	vigour_fOccExpSpRatio;		//ְҵ�����SP��ֵ

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
		//��Ҿٱ�ʱ����(��)
		DWORD	dwReportTime;
		
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
		
		long	lTimerVarNum;	//Ԥ���䶨ʱ������������
		long    lPreAllocedScriptNum; // Ԥ����Ľű��ڴ������
		long    lMaxPreAllocedScriptNum;// Ԥ����Ľű��ڴ���������
		long    lDupRgnQueueNum;  // ���������Ŷ���
		long    lPDupRgnMaxNum;  // �������˸��������
		long    lPHDupRgnMaxNum;  // �������˷��ݸ��������
		long    lTDupRgnMaxNum;  // ������Ӹ��������
		long    lDupRgnQueueDeltaTime;  // ���������Ŷ�ʱ����
		long    lDupRgnQueueMaxNum;  // ���������Ŷ������
		long	lPDupRgnDelTime;			// ���˸���ɾ��ʱ��
		long	lCreatePDupRgnMaxNum;	//��Ҵ������˸��������
		long	lCreateTDupRgnMaxNum;	//��Ҵ�����Ӹ��������
		long	lCreateDupRgnDeltaTime; //��Ҵ�������ʱ����
		long    lSaveTime;				// ��ұ������ݵ�ʱ����
		long	lWSSaveTime;			// WS�������ݵ�ʱ����

		long	lMaxGoodInfoNum;	//���[��������Ʒ��Ϣ]��������

		long	lReliveDebuffId;	// ��Ҹ���DebuffID
		long	lReliveDebuffLevel; // ��Ҹ���DebuffLevel

		long	lCreateRgnPlayerLoadMax; //GS���������������������
		long	lCreateRgnMonsterLoadMax; //GS����������������������
		long    lPDupRgnMaxPlayerNum;  // ���˸����������
		long    lPHDupRgnMaxPlayerNum;  // ���˷��ݸ����������
		long    lTDupRgnMaxPlayerNum;  // ��Ӹ����������
		long	lRgnKickPlayerDeltaTime;	// ��������ʱ��
		long    lMLevelEnablePlayerSearch;  // ���ݵȼ����������ҹ���

		char	szHideName[32];	// ���������
		ULONG	uUpgradeTo10FontColor;		//! �����ɹ�����10���ϣ�������ʾ��������ɫ
		ULONG	uUpgradeTo20BackIdx;		//! �����ɹ�����10���ϣ��������ʾ�ı�������
		ULONG	uUpgrade10LostFontColor;	//! 10������������ʧ�ܣ�������ʾ��������ɫ

		//��Ʊϵͳ�ű�
		char	pszLottery[MAX_PATH];
	};

private:
	static tagSetup	m_stSetup;

public:
	static BOOL Load(const char* filename);			// ���ļ���ȡ�б�
	static BOOL LoadGameSetup(const char* filename);		// ��setup\gamesetup.ini��ȡ
	static BOOL Save(const char* filename);			// ���浽�ļ��б�

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

	static tagSetup* GetSetup()	{return &m_stSetup;}

	static WORD GetBaseMaxRp(BYTE lOccupation, BYTE lLevel);	// ��ȡ���ŭֵ����
};
