#pragma once

// ͨ������
class CGlobeSetup
{
public:
	struct tagSetup
	{
		float		fMoneyBaseRate;			// ��Ʒ�����̵�۸�����۾���
		float		fMoneyRate;				// ��Ʒ�����̵�۸��۾���
		BYTE		btMaxCharactersNum;		// ��ɫ�����
		DWORD		dwMaxPlayerLevel;		// ��ɫ���ȼ�		
		DWORD		dwMoneyIndex;			// ��ҵ�����
		DWORD		dwSilverMoneyIndex;		// ���ҵ�����
		D3DCOLOR	colorWarningText;		// ������Ϣ��ɫ
		D3DCOLOR	colorMouseShape;		// ���ѡ�е�shape
		D3DCOLOR	colorWhiteName;			// ����
		D3DCOLOR	colorWhiteNameBack;		// �����߿�
		D3DCOLOR	colorPurpleName;		// ����
		D3DCOLOR	colorPurpleNameBack;	// �����߿�
		D3DCOLOR	colorRedName;			// ����
		D3DCOLOR	colorRedNameBack;		// �����߿�
		D3DCOLOR	colorOrangeName;		// ����
		D3DCOLOR	colorOrangeNameBack;	// �����߿�
		D3DCOLOR	colorBlueName;			// ����
		D3DCOLOR	colorBlueNameBack;		// �����߿�
		D3DCOLOR	colorYellowName;		// ����
		D3DCOLOR	colorYellowNameBack;	// �����߿�
		D3DCOLOR	colorGrayName;			// ����
		D3DCOLOR	colorGrayNameBack;		// �����߿�

		D3DCOLOR	colorNoShowCountryName;		//û�д򿪹��ұ�־ʱ,������ҿͻ���Ӧ����ʾ��������ɫ
		D3DCOLOR	colorNoShowCountryNameBack;	//û�д򿪹��ұ�־ʱ,������ҿͻ���Ӧ����ʾ��������Ӱ��ɫ
		D3DCOLOR	colorAllianceName;		// ͬ��Ӫ��
		D3DCOLOR	colorAllianceNameBack;	// ͬ��Ӫ�߿�
		D3DCOLOR	colorEnemyName;			// ��ͬ��Ӫ��
		D3DCOLOR	colorEnemyNameBack;		// ��ͬ��Ӫ�߿�
		D3DCOLOR	colorTeammateName;		// ������
		D3DCOLOR	colorTeammateNameBack;	// �������߿�
		D3DCOLOR	colorBossName;			// BOSS����
		D3DCOLOR	colorBossNameBack;		// BOSS���ֱ߿�

		D3DCOLOR	colorNpcName;			// NPC����
		D3DCOLOR	colorNpcNameBack;		// NPC���ֱ߿�
		D3DCOLOR	colorMonsterName[7];	// Monster����
		D3DCOLOR	colorMonsterNameBack[7];// Monster���ֱ߿�
		D3DCOLOR	colorMonsterTitelName[8];	// Monster�ĳ�ν������ɫ
		D3DCOLOR	colorMonsterTitelNameBack[8];// Monster�ĳ�ν���ֱ߿���ɫ
		D3DCOLOR	colorDiedMonsterName;		// ������Monster����
		D3DCOLOR	colorDiedMonsterNameBack;	// ������Monster���ֱ߿�
		D3DCOLOR	colorGoodsName;			// Goods����
		D3DCOLOR	colorGoodsNameBack;		// Goods���ֱ߿�
		D3DCOLOR	colorCityGateName;		// CityGate����
		D3DCOLOR	colorCityGateNameBack;	// CityGate���ֱ߿�

		D3DCOLOR	colorKnight;				//��λ�ƺ���ɫ
		D3DCOLOR	colorFaction;				//���ƺ���ɫ
		D3DCOLOR	colorMedal;					//ѫ�³ƺ���ɫ

		D3DCOLOR	colorBloodTextMonster;			// �����Ѫ��ֵ
		D3DCOLOR	colorBloodTextMonsterBack;
		D3DCOLOR	colorBloodTextPlayer;			// ��Ҽ�Ѫ��ֵ
		D3DCOLOR	colorBloodTextPlayerBack;
		D3DCOLOR	colorAddBloodTextPlayer;		// ��Ҽ�Ѫ��ֵ��ɫ
		D3DCOLOR	colorAddBloodTextPlayerBack;	

		D3DCOLOR	colorExpTipText;				//����ֵ������ʾ��ɫ
		D3DCOLOR	colorExpTipTextBack;			//����ֵ������ʾ������ɫ
		D3DCOLOR	colorConTipText;				//��ѫֵ������ʾ��ɫ
		D3DCOLOR	colorConTipTextBack;			//��ѫֵ������ʾ������ɫ
		DWORD		dwExpTipTextTimeLength;			//����ֵ��ʾ����ʱ��
		DWORD		dwExpTipTextLineMax;			//����ֵ��ʾ�����ʾ����
		float		fExpTipTextLineSpace;			//����ֵ��ʾ�м��
		float		fExpTipTextNumberScale;			//����ֵ��ʾ�������ű���

		float fPlayerInfoHeight;				//���ͷ����Ϣ���Ƹ߶�����
		float fMonsterInfoHeight;				//����ͷ����Ϣ���Ƹ߶�����(����NPC)

		// ͬ������		0
		// ͬ��������	1
		// ��������		2
		// �л�����		3
		// ��������		4
		// ����˽��		5
		// ��������		6
		// �˹�����		7
		// ��������		8
		// ϵͳ��Ϣ		9
		// �㲥			10
		// GM����		11

		DWORD		dwChatItemNum;		// ���컺����Ŀ����

		D3DCOLOR	colorChatBackBar;		// ��������ɫ
		D3DCOLOR	colorChatText[12];		// ������ɫ
		D3DCOLOR	colorChatTextBack[12];
		D3DCOLOR	colorChatTextShadow[12];

		D3DCOLOR	colorTopChat;				// �ö�������ɫ
		D3DCOLOR	colorTopChatShadow;			// �ö����������ɫ
		DWORD		dwTopChatPropIndex;			// �ö��������
		DWORD		dwTopChatMaxNum;			// �ö�������������
		DWORD		dwTopChatTime;				// �ö�����ͣ��ʱ��
		LONG		lTopChatY;					// �ö������в�Y����

		D3DCOLOR	colorTopChatInBox;			// �ö��������������ɫ
		D3DCOLOR	colorTopChatInBoxBack;
		D3DCOLOR	colorTopChatInBoxShadow;	

		D3DCOLOR	colorBroadcast;				// ����� ��ɫ
		D3DCOLOR	colorBroadcastShadow;		// ����� �����ɫ
		DWORD		dwBroadcastSpeed;			// ����� �ٶ�
		DWORD		dwBroadcastLoop;			// ����� ѭ������
		LONG		lBroadcastY;				// ����� Y����

		D3DCOLOR	colorScriptBroadcast;		// �ű����� ��ɫ
		D3DCOLOR	colorScriptBroadcastBack;	// �ű����� ������ɫ
		D3DCOLOR	colorScriptBroadcastShadow;	// �ű����� �����ɫ

		//��Ʒ˵����ɫ
		D3DCOLOR	colorGoodsWhite;		// ��ɫ
		D3DCOLOR	colorGoodsBlue;			// ��ɫ
		D3DCOLOR	colorGoodsRed;			// ��ɫ
		D3DCOLOR	colorGoodsDrop;			// ��Ʒ���½�������ɫ
		D3DCOLOR	colorGoodsGold;			// ��ɫ(�������Գ���4��)
		D3DCOLOR	colorGoodsDarkGold;		// ����ɫ(��������)
		D3DCOLOR	colorGoodsGod;			// ��Ʒ 100�������!!!!????
		D3DCOLOR	colorGoodsFuMo;			// ��ħ
		D3DCOLOR	colorGoodsDesc;			// ����
		D3DCOLOR	colorGoodsPrice;		// �ۼ�
		D3DCOLOR	colorGoodsPurpleBaseDesp;		// ��������������ɫ
		D3DCOLOR	colorGoodsPurpleAddtionDesp;		// ׷������������ɫ
		D3DCOLOR	colorGoodsOrangeBaseDesp;		// ��������������ɫ
		D3DCOLOR	colorGoodsOrangeAddtionDesp;		// ׷������������ɫ

		D3DCOLOR	colorShopBar;			// �����̵�װ���ɫ
		D3DCOLOR	colorShopWireBar;		// �����̵�װ�߿���ɫ
		D3DCOLOR	colorShopName;			// �����̵�����
		D3DCOLOR	colorShopNameBack;		// �����̵����ֱ߿�

		D3DCOLOR	colorCriticalBloodText;		// ����һ����Ѫ��ֵ
		D3DCOLOR	colorCriticalBloodTextBack;
		float		fCriticalTextScale;			// ����һ�������ִ�С����

		D3DCOLOR	colorFriendOnlineOffline;	// ������������ʾ��ɫ
		D3DCOLOR	colorFriendOnlineOfflineBack;
		D3DCOLOR	colorFriendOnlineOfflineShadow;

		D3DCOLOR	colorLowerLevel;
		D3DCOLOR	colorMiddleLevel;
		D3DCOLOR	colorUpperLevel;

		D3DCOLOR	colorItemBlueBack;		// ��Ʒ����ɫ���������ã�
		D3DCOLOR	colorItemRedBack;		// ��Ʒ�ĺ�ɫ�����������ã�
		D3DCOLOR	colorHandItemCanPlaceBack;		// ������Ʒ�ܷ�������ɫ
		D3DCOLOR	colorHandItemCanChangeBack;		// ������Ʒ�ܽ�������ɫ
		D3DCOLOR	colorHandItemCannotChangeBack;	// ������Ʒ���ܽ�������ɫ
		D3DCOLOR	colorInShopMirrorBack;	// �����ڸ����̵���׿��еľ��񱳾�ɫ
		D3DCOLOR	colorEnemyPlayerNameGreen;	//�й������ɫ����
		D3DCOLOR	colorEnemyPlayerNameGray;	//�й���һ�ɫ����
		D3DCOLOR	colorEnemyPlayerNamePurple;	//�й������ɫ����
		D3DCOLOR	colorEnemyNameInBattleZone;		//�������ж���Ӫ������ɫ
		D3DCOLOR	colorEnemyNameInBattleZoneBack;	//�������ж���Ӫ������ɫ����ɫ
		D3DCOLOR	colorEnemyNameInWarZone;		//ս�����Եж���Ӫ������ɫ
		D3DCOLOR	colorEnemyNameInWarZoneBack;	//ս�����Եж���Ӫ������ɫ����ɫ

		DWORD		dwLevelUpEffectID;				// ���������Ч
		DWORD		dwSkillLevelUpEffectID;			// ��Ҽ���������Ч
		DWORD		dwReputeLevelUpEffectID;		// �������������Ч
		DWORD       dwOccuLevelUpEffectID;          // ���ְҵ������Ч

		D3DCOLOR	colorQuestMstByCreator;			// ���񱣻��ֵ�������ɫ�������ߣ�
		D3DCOLOR	colorQuestMstByOther;			// ���񱣻��ֵ�������ɫ���Ǵ����ߣ�

		DWORD dwPlayerAlphaFadeTimeLength;			//���͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
		DWORD dwMonsterAlphaFadeTimeLength;			//����͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
		DWORD dwCollectionAlphaFadeTimeLength;		//�ɼ���͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
		DWORD dwGoodsAlphaFadeTimeLength;			//��Ʒ͸������ʱ�䳤��(Ϊ0���ʾû��͸������)

		DWORD dwNormalMailPrice;					// ��ͨ�ʼ�������
		DWORD dwNormalGoodsMailPrice;				// ��ͨ�����ʼ�������
		DWORD dwPresentMailPrice;					// �����ʼ�������
		DWORD dwRequestMailPrice;					// ��Ҫ�ʼ�������
		DWORD dwPayGoldMailPrice;					// ���������ʼ�������

		long lAreaByTab;							// Tab�����Ҷ���ķ�Χ
		DWORD dwLoadingDelayTime;					// loading���ӳ�ʱ��
		long	lUseSkillMsgCount;					// ����ʹ�ü��ܷ�����Ϣ����
		long	lMoveMsgCount;						// �����ƶ�������Ϣ����

		DWORD dwSendFinishBagDelayTime;				// ���ͱ����������Ϣ���ӳ�ʱ��
		DWORD dwAnswerTime;							// ����Ҵ���ʱ��
		DWORD dwAutoOpenTime;						// ����ҽ����Զ���ʱ��
		DWORD dwSonudID;							// ����Ҿ�����Ϣ��ʾ��ID
		long	lEncryptType;						// ͨ�ż��ܿ���
		DWORD dwLimit;								// �����������ܵĵȼ�����
		long	lUseSkillTime;						// ����ʹ��Ƶ��
		long   lReliveTime;                         // ����ʹ�õ���ʱ��
		long   lReliveItem;                         // Ĭ��ʹ�ø�����Ʒ��id
		long   lPKCPShowLevel;                      // CPֵ����ʾ�ȼ�����
		DWORD  dwPrivateChatHistoryMaxItem;			// ˽����ʷÿҳ��ʾ��Ŀ��
		DWORD  dwMaxNumPrivateChat;					// ˽�������������λ��Ŀ
		DWORD  dwMaxPriChatMsgBoxTotalItem;			//  ˽����Ϣ���������Ŀ����
		DWORD  dwFBCoinID;							 // �������̻��ҵ�ID
        float  fminimumOpDistance;                  // ��С��������
        float  fLineMaxDistanceAllowToMove;         // ֱ���������Ч�ƶ�����
		/***********************************************************************/
		/* zhaohang fix 2010-9-3
		/* ����ɱ����۵�����
		/***********************************************************************/
		char   strWeiMianName[256];						// �ɱ�����ʱλ�������
		float  agentExtracust;							// �ɱ����������ѷ���
		int    oderPerNum;								// �ɱ�����һ����Ӧ��λ������
		/***********************************************************************/
		/* zhaohang add */
		char   strSkillDocCon[256];					///�����İ������ļ�����·������
		char   strSkillShowIni[256];				///������ʾ�����ļ�����·������
		char   strSkillPoseIni[256];				///������̬�����ļ�����·������
		char   strSkillAttrIniSetup[256];			///�������������ļ�����·������
		/***********************************************************************/
	};

protected:
	static tagSetup	m_tagSetup;

public:
	static bool		Load(const char* filename);			// ��ȡ
	static tagSetup*	GetSetup()	{return &m_tagSetup;}

public:
	CGlobeSetup(void);
	virtual ~CGlobeSetup(void);
};
