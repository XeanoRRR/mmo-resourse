#include "StdAfx.h"
#include ".\globesetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;

CGlobeSetup::tagSetup CGlobeSetup::m_stSetup;

CGlobeSetup::CGlobeSetup(void)
{
	memset(&m_stSetup, 0, sizeof(tagSetup));
}

CGlobeSetup::~CGlobeSetup(void)
{
}

// ���ļ���ȡ�б�
BOOL CGlobeSetup::Load(const char* filename)
{
	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		char str[256];
		_snprintf(str, 256, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return FALSE;
	}


	stringstream stream;
	prfile->ReadToStream(stream);
	ZeroMemory(&m_stSetup, sizeof(tagSetup));

	string strTemp;
	stream	
		>> strTemp >> m_stSetup.btMaxCharactersNum
		>> strTemp >> m_stSetup.fStr2MaxAtk[0]
		>> strTemp >> m_stSetup.fStr2MaxAtk[1]
		>> strTemp >> m_stSetup.fStr2MaxAtk[2]
		>> strTemp >> m_stSetup.fStr2Burden[0]
		>> strTemp >> m_stSetup.fStr2Burden[1]
		>> strTemp >> m_stSetup.fStr2Burden[2]
		>> strTemp >> m_stSetup.fDex2MinAtk[0]
		>> strTemp >> m_stSetup.fDex2MinAtk[1]
		>> strTemp >> m_stSetup.fDex2MinAtk[2]
		>> strTemp >> m_stSetup.fDex2Stiff[0]
		>> strTemp >> m_stSetup.fDex2Stiff[1]
		>> strTemp >> m_stSetup.fDex2Stiff[2]
		>> strTemp >> m_stSetup.fCon2MaxHp[0]
		>> strTemp >> m_stSetup.fCon2MaxHp[1]
		>> strTemp >> m_stSetup.fCon2MaxHp[2]
		>> strTemp >> m_stSetup.fCon2Def[0]
		>> strTemp >> m_stSetup.fCon2Def[1]
		>> strTemp >> m_stSetup.fCon2Def[2]
		>> strTemp >> m_stSetup.fInt2MaxMp[0]
		>> strTemp >> m_stSetup.fInt2MaxMp[1]
		>> strTemp >> m_stSetup.fInt2MaxMp[2]
		>> strTemp >> m_stSetup.fInt2Element[0]
		>> strTemp >> m_stSetup.fInt2Element[1]
		>> strTemp >> m_stSetup.fInt2Element[2]
		>> strTemp >> m_stSetup.fInt2Resistant[0]
		>> strTemp >> m_stSetup.fInt2Resistant[1]
		>> strTemp >> m_stSetup.fInt2Resistant[2]

		>> strTemp >> m_stSetup.lBaseHit
		>> strTemp >> m_stSetup.fBaseAtcHitcMagnify
		>> strTemp >> m_stSetup.fHitLvRate
		>> strTemp >> m_stSetup.lMinHitMonster
		>> strTemp >> m_stSetup.lMaxHitMonster
		>> strTemp >> m_stSetup.lMinHit[0]
		>> strTemp >> m_stSetup.lMinHit[1]
		>> strTemp >> m_stSetup.lMinHit[2]
		>> strTemp >> m_stSetup.lMaxHit[0]
		>> strTemp >> m_stSetup.lMaxHit[1]
		>> strTemp >> m_stSetup.lMaxHit[2]
		>> strTemp >> m_stSetup.fCriticalRate
		>> strTemp >> m_stSetup.fLossExp_Normal
		>> strTemp >> m_stSetup.fLossExp_Game
		>> strTemp >> m_stSetup.fLossExp_War		

		// ����Ʒ����(�����ơ��졢��)		
		>>strTemp>>m_stSetup.fDropEquip_Head[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Wing[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Body[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Back[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Glove[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Boot[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Frock[0][0]
		>>strTemp>>m_stSetup.fDropEquip_LRing[0][0]
		>>strTemp>>m_stSetup.fDropEquip_RRing[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Horse[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Bag[0][0]
		>>strTemp>>m_stSetup.fDropEquip_Money[0][0]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[0][0]

		>>strTemp>>m_stSetup.fDropEquip_Head[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Wing[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Body[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Back[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Glove[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Boot[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Frock[0][1]
		>>strTemp>>m_stSetup.fDropEquip_LRing[0][1]
		>>strTemp>>m_stSetup.fDropEquip_RRing[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Horse[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Bag[0][1]
		>>strTemp>>m_stSetup.fDropEquip_Money[0][1]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[0][1]

		>>strTemp>>m_stSetup.fDropEquip_Head[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Wing[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Body[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Back[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Glove[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Boot[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Frock[0][2]
		>>strTemp>>m_stSetup.fDropEquip_LRing[0][2]
		>>strTemp>>m_stSetup.fDropEquip_RRing[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Horse[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Bag[0][2]
		>>strTemp>>m_stSetup.fDropEquip_Money[0][2]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[0][2]

		>>strTemp>>m_stSetup.fDropEquip_Head[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Wing[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Body[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Back[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Glove[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Boot[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Frock[0][3]
		>>strTemp>>m_stSetup.fDropEquip_LRing[0][3]
		>>strTemp>>m_stSetup.fDropEquip_RRing[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Horse[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Bag[0][3]
		>>strTemp>>m_stSetup.fDropEquip_Money[0][3]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[0][3]
		// ����Ʒ����(�����ơ��졢��)(��ս)
		>>strTemp>>m_stSetup.fDropEquip_Head[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Wing[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Body[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Back[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Glove[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Boot[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Frock[1][0]
		>>strTemp>>m_stSetup.fDropEquip_LRing[1][0]
		>>strTemp>>m_stSetup.fDropEquip_RRing[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Horse[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Bag[1][0]
		>>strTemp>>m_stSetup.fDropEquip_Money[1][0]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[1][0]

		>>strTemp>>m_stSetup.fDropEquip_Head[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Wing[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Body[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Back[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Glove[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Boot[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Frock[1][1]
		>>strTemp>>m_stSetup.fDropEquip_LRing[1][1]
		>>strTemp>>m_stSetup.fDropEquip_RRing[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Horse[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Bag[1][1]
		>>strTemp>>m_stSetup.fDropEquip_Money[1][1]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[1][1]

		>>strTemp>>m_stSetup.fDropEquip_Head[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Wing[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Body[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Back[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Glove[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Boot[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Frock[1][2]
		>>strTemp>>m_stSetup.fDropEquip_LRing[1][2]
		>>strTemp>>m_stSetup.fDropEquip_RRing[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Horse[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Bag[1][2]
		>>strTemp>>m_stSetup.fDropEquip_Money[1][2]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[1][2]

		>>strTemp>>m_stSetup.fDropEquip_Head[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Necklace[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Wing[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Body[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Back[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Glove[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Boot[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Headgear[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Frock[1][3]
		>>strTemp>>m_stSetup.fDropEquip_LRing[1][3]
		>>strTemp>>m_stSetup.fDropEquip_RRing[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Medal1[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Medal2[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Medal3[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Fairy[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Weapon[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Weapon2[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Cloak[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Horse[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Bag[1][3]
		>>strTemp>>m_stSetup.fDropEquip_Money[1][3]
		>>strTemp>>m_stSetup.fDropEquip_MoneyPercent[1][3]

		//��Ʒ��ֵ�Ⱥͼ۸�����ϵ��
		>> strTemp >> m_stSetup.fBasePriceRate
		>> strTemp >> m_stSetup.fTradeInRate
		>> strTemp >> m_stSetup.fRepairFactor
		>> strTemp >> m_stSetup.fRepairFactor2

		//##��Ʒ���
		>> strTemp >> m_stSetup.lGoodsDurFray
		>> strTemp >> m_stSetup.lGoodsDurSpecialFray
		>> strTemp >> m_stSetup.lHeadWaste
		>> strTemp >> m_stSetup.lBodyWaste
		>> strTemp >> m_stSetup.lGloveWaste
		>> strTemp >> m_stSetup.lBootWaste

		>> strTemp >> m_stSetup.lBackWaste
		>> strTemp >> m_stSetup.lDecorateWaste
		>> strTemp >> m_stSetup.lHeadgearWaste
		>> strTemp >> m_stSetup.lTalismanWaste
		>> strTemp >> m_stSetup.lFrockWaste
		>> strTemp >> m_stSetup.lMedalWaste
		>> strTemp >> m_stSetup.lWingWaste
		>> strTemp >> m_stSetup.lManteauWaste
		>> strTemp >> m_stSetup.lFairyWaste
		>> strTemp >> m_stSetup.lNeckWaste
		>> strTemp >> m_stSetup.lRingWaste
		>> strTemp >> m_stSetup.lShieldWaste

		//��Ʒ�����׶�ϵ���ͳɹ�����
		>> strTemp >> m_stSetup.wGoodsBlessPhase[0]
		>> strTemp >> m_stSetup.wGoodsBlessPhase[1]
		>> strTemp >> m_stSetup.wGoodsBlessPhase[2]
		>> strTemp >> m_stSetup.fGoodsUpgradeOdds[0]
		>> strTemp >> m_stSetup.fGoodsUpgradeOdds[1]
		>> strTemp >> m_stSetup.fGoodsUpgradeOdds[2]
		>> strTemp >> m_stSetup.fGoodsUpgradeOdds[3]

		// ����
		>> strTemp >> m_stSetup.dwGoodsDisappearTimer
		>> strTemp >> m_stSetup.dwGoodsProtectedTimer
		>> strTemp >> m_stSetup.lResumeTimer
		>> strTemp >> m_stSetup.lResumeFactor
		>> strTemp >> m_stSetup.fResumeFactorMini

		>> strTemp >> m_stSetup.lMonsterResumeTimer

		>> strTemp >> m_stSetup.lFightStateTimer
		>> strTemp >> m_stSetup.lSinStateTimer
		>> strTemp >> m_stSetup.dwAttackMonsterProtectedTimer
		>> strTemp >> m_stSetup.dwMonsterKeepDeadTime
		>> strTemp >> m_stSetup.dwCanCollectMonsterKeepDeadTime
		>> strTemp >> m_stSetup.dwBeneficiaryTime
		>> strTemp >> m_stSetup.dwCollectionKeepDeadTime
		>> strTemp >> m_stSetup.dwMonsterTransitionTime
		>> strTemp >> m_stSetup.dwMonsterKindlyTime
		>> strTemp >> m_stSetup.dwReturnPeaceEffectTime
		>> strTemp >> m_stSetup.fReturnPeaceSpeed
		>> strTemp >> m_stSetup.dwReturnPeaceStandTime
		>> strTemp >> m_stSetup.dwFightStateTime
		>> strTemp >> m_stSetup.dwDHFQueueSize
		// ŭֵ������
		>> strTemp >> m_stSetup.wBaseRp_Lv1
		>> strTemp >> m_stSetup.wBaseRp_Lv2
		>> strTemp >> m_stSetup.wBaseMaxRp_Lv1
		>> strTemp >> m_stSetup.wBaseMaxRp_Lv2
		>> strTemp >> m_stSetup.wBaseAtcRp_Sword

		>> strTemp >> m_stSetup.fRpAdd_Lv1[0]	>>	m_stSetup.wRpAdd_Lv1[0]
		>> strTemp >> m_stSetup.fRpAdd_Lv1[1]	>>	m_stSetup.wRpAdd_Lv1[1]
		>> strTemp >> m_stSetup.fRpAdd_Lv1[2]	>>	m_stSetup.wRpAdd_Lv1[2]
		>> strTemp >> m_stSetup.fRpAdd_Lv1[3]	>>	m_stSetup.wRpAdd_Lv1[3]
		>> strTemp >> m_stSetup.fRpAdd_Lv1[4]	>>	m_stSetup.wRpAdd_Lv1[4]
		>> strTemp >> m_stSetup.fRpAdd_Lv1[5]	>>	m_stSetup.wRpAdd_Lv1[5]

		>> strTemp >> m_stSetup.lHitBaseLv
		>> strTemp >> m_stSetup.lHitTime
		>> strTemp >> m_stSetup.fHitExpPrize
		>> strTemp >> m_stSetup.fMaxHitExpPrize

		>> strTemp >> m_stSetup.dwHitTopLogNum[0]	>>	m_stSetup.dwHitTopLogPrizeExp[0]
		>> strTemp >> m_stSetup.dwHitTopLogNum[1]	>>	m_stSetup.dwHitTopLogPrizeExp[1]
		>> strTemp >> m_stSetup.dwHitTopLogNum[2]	>>	m_stSetup.dwHitTopLogPrizeExp[2]
		>> strTemp >> m_stSetup.dwHitTopLogNum[3]	>>	m_stSetup.dwHitTopLogPrizeExp[3]
		>> strTemp >> m_stSetup.dwHitTopLogNum[4]	>>	m_stSetup.dwHitTopLogPrizeExp[4]
		>> strTemp >> m_stSetup.dwHitTopLogNum[5]	>>	m_stSetup.dwHitTopLogPrizeExp[5]
		>> strTemp >> m_stSetup.dwHitTopLogNum[6]	>>	m_stSetup.dwHitTopLogPrizeExp[6]
		>> strTemp >> m_stSetup.dwHitTopLogNum[7]	>>	m_stSetup.dwHitTopLogPrizeExp[7]
		>> strTemp >> m_stSetup.dwHitTopLogNum[8]	>>	m_stSetup.dwHitTopLogPrizeExp[8]
		>> strTemp >> m_stSetup.dwHitTopLogNum[9]	>>	m_stSetup.dwHitTopLogPrizeExp[9]
		>> strTemp >> m_stSetup.dwHitTopLogNum[10]	>>	m_stSetup.dwHitTopLogPrizeExp[10]
		>> strTemp >> m_stSetup.dwHitTopLogNum[11]	>>	m_stSetup.dwHitTopLogPrizeExp[11]

		>> strTemp >> m_stSetup.lHpLiveUp[0]
		>> strTemp >> m_stSetup.lHpLiveUp[1]
		>> strTemp >> m_stSetup.lHpLiveUp[2]
		>> strTemp >> m_stSetup.lMpLiveUp[0]
		>> strTemp >> m_stSetup.lMpLiveUp[1]
		>> strTemp >> m_stSetup.lMpLiveUp[2]
		>> strTemp >> m_stSetup.lBurdenLiveUp[0]
		>> strTemp >> m_stSetup.lBurdenLiveUp[1]
		>> strTemp >> m_stSetup.lBurdenLiveUp[2]

		//##Ӳֱ�Ĳ���
		>> strTemp >> m_stSetup.wNumStiffSetup
		>> strTemp >> m_stSetup.fStiff_Damage[0] >> m_stSetup.wStiff_Probability[0]
		>> strTemp >> m_stSetup.fStiff_Damage[1] >> m_stSetup.wStiff_Probability[1]
		>> strTemp >> m_stSetup.fStiff_Damage[2] >> m_stSetup.wStiff_Probability[2]
		>> strTemp >> m_stSetup.fStiff_Damage[3] >> m_stSetup.wStiff_Probability[3]

		>> strTemp >> m_stSetup.dwStiffDelayTime
		>> strTemp >> m_stSetup.dwStiffBoundTime
		>> strTemp >> m_stSetup.dwStiffLimit

		//##ʱ�����
		>> strTemp >> m_stSetup.dwCriminalTime
		>> strTemp >> m_stSetup.dwMurdererTime	// ����
		>> strTemp >> m_stSetup.dwRipperTime	// ����
		>> strTemp >> m_stSetup.dwPkCountPerKill	// ɱһ�����ӵ�PKֵ
		>> strTemp >> m_stSetup.dwOnePkCountTime	// ����һ��PKֵ��ʱ��

		//ʰȡ��Ʒʱ����
		>> strTemp >> m_stSetup.dwPickGoodsInterval
		//##�������
		>> strTemp >> m_stSetup.dwGoldCoinLimit
		//��������
		>> strTemp >> m_stSetup.dwSilverLimit
		//��ȯ����
		>> strTemp >> m_stSetup.dwTicketLimit

		//##���߼�Ѫ�����ʱ������
		>> strTemp >> m_stSetup.dwHealTimeInterval

		//##���߼�MP�����ʱ������
		>> strTemp >> m_stSetup.dwMpTimeInterval

		>> strTemp >> m_stSetup.fMonsterNumScale

		>> strTemp >> strTemp	//m_stSetup.bAllowClientRunScript

		>> strTemp >> strTemp	//m_stSetup.bAllowClientChangePos

		>> strTemp >> strTemp	//m_stSetup.dwSavePointTime

		>> strTemp >> m_stSetup.lAreaWidth
		>> strTemp >> m_stSetup.lAreaHeight

		// ��ɫ��������
		>> strTemp >> strTemp	//m_stSetup.dwDelDays
		>> strTemp >> strTemp	//m_stSetup.strSpeStr	// ���и������ַ������Ը���
		>> strTemp >> m_stSetup.lRegionChatLevelLimit

		>> strTemp >> m_stSetup.lOfferExpPercent
		>> strTemp >> m_stSetup.fDecTimeParam
		>> strTemp >> m_stSetup.lDiedStateTime

		>> strTemp >> m_stSetup.m_fPvPDamageFactor
		>> strTemp >> m_stSetup.m_btNewbieLevelLimit
		>> strTemp >> m_stSetup.m_btNewSoldierLevel
		
		>> strTemp >> m_stSetup.m_fWeaponPowerModifier
		>> strTemp >> m_stSetup.m_fWeaponMinPowerModifier


		>> strTemp >> m_stSetup.m_dwCard6Hours
		>> strTemp >> m_stSetup.m_dwCard12Hours
		>> strTemp >> m_stSetup.m_dwCard24Hours

		>> strTemp >> m_stSetup.m_dwCard6HoursChargePrice
		>> strTemp >> m_stSetup.m_dwCard12HoursChargePrice
		>> strTemp >> m_stSetup.m_dwCard24HoursChargePrice

		>> strTemp >> m_stSetup.m_dwCardLowerLimitPrice
		>> strTemp >> m_stSetup.m_dwCardUpperLimitPrice
		

		>> strTemp >> m_stSetup.m_dwCardMaxNum
		>> strTemp >> m_stSetup.m_dwCardWorldMaxNum

		>> strTemp >> m_stSetup.m_dwCardRetryNum
		>> strTemp >> m_stSetup.m_dwCardDenyTime

		>> strTemp >> m_stSetup.fBlastAttackScale
		>> strTemp >> m_stSetup.fBlastDefendScale
		>> strTemp >> m_stSetup.fElementBlastAttackScale
		>> strTemp >> m_stSetup.fElementBlastDefendScale
		>> strTemp >> m_stSetup.fElementFullMissScale
	
		>> strTemp >> m_stSetup.szTalkWorldGoodsName		// ���纰����Ʒԭʼ��
		>> strTemp >> m_stSetup.nTalkWorldGoodsNum			// ���纰����Ʒ����
		>> strTemp >> m_stSetup.lTalkWorldGold				// ���纰���������
		>> strTemp >> m_stSetup.lTalkWorldSilver			// ���纰����������

		>> strTemp >> m_stSetup.szTalkUnionGoodsName		// ��Ӫ������Ʒԭʼ��
		>> strTemp >> m_stSetup.nTalkUnionGoodsNum			// ��Ӫ������Ʒ����
		>> strTemp >> m_stSetup.lTalkUnionGold				// ��Ӫ������Ǯ����
		>> strTemp >> m_stSetup.lTalkUnionSilver			// ��Ӫ������������

		>> strTemp >> m_stSetup.szTalkCountryGoodsName		// ���Һ�����Ʒԭʼ��
		>> strTemp >> m_stSetup.nTalkCountryGoodsNum		// ���Һ�����Ʒ����
		>> strTemp >> m_stSetup.lTalkCountryGold			// ���Һ�����Ǯ����
		>> strTemp >> m_stSetup.lTalkCountrySilver			// ���Һ�����������

		>> strTemp >> m_stSetup.szTalkTopWorldGoodsName		// �ö����纰����Ʒԭʼ��
		>> strTemp >> m_stSetup.nTalkTopWorldGoodsNum		// �ö����纰����Ʒ����
		>> strTemp >> m_stSetup.lTalkTopWorldGold			// �ö����纰���������
		>> strTemp >> m_stSetup.lTalkTopWorldSilver			// �ö����纰����������

		>> strTemp >> m_stSetup.szReliveGoodsName
		>> strTemp >> m_stSetup.lRelivePrice


		>> strTemp >> m_stSetup.fPlayerWalkSpeed
		>> strTemp >> m_stSetup.fPlayerRunSpeed

		>> strTemp >> m_stSetup.fRidePlayerWalkSpeed
		>> strTemp >> m_stSetup.fRidePlayerRunSpeed

		>> strTemp >> m_stSetup.lExpBallFactor1
		>> strTemp >> m_stSetup.lExpBallFactor2

		>> strTemp >> m_stSetup.dwAutoProtectTime

		>> strTemp >> m_stSetup.dwIncrementLogDays
		>> strTemp >> m_stSetup.lMaxHelpHintNum

		>> strTemp >> m_stSetup.pszPlayerLoginScript
		>> strTemp >> m_stSetup.pszPlayerDiedScript
		>> strTemp >> m_stSetup.pszPlayerUpLevelScript
		>> strTemp >> m_stSetup.pszPlayerChangeOccuScript
		>> strTemp >> m_stSetup.pszPlayerUseDiamondScript
		>> strTemp >> m_stSetup.pszPVPKilled
		>> strTemp >> m_stSetup.pszPVPBeKilled

		>> strTemp >> m_stSetup.pszConValueNegative
		>> strTemp >> m_stSetup.pszConValueNotNegative

		>> strTemp >> m_stSetup.szReliveScriptName	// ��Ҹ�����ű���
		>> strTemp >> m_stSetup.pszCalendar
		>> strTemp >> m_stSetup.pszMagicBox
		>> strTemp >> m_stSetup.pszAnswerRequest
		>> strTemp >> m_stSetup.pszChangeLineRgn
		>> strTemp >> m_stSetup.pszHelpRequest

		//��������
		>> strTemp >> m_stSetup.lCorrelativeSkillID			//����ӹ����ļ���ID
		>> strTemp >> m_stSetup.lMinSkillLevel				//������ӵ���С���ܵȼ�
		>> strTemp >> m_stSetup.lMinTradeLevel

		>> strTemp >> m_stSetup.new_lGoodsAllot				//Ĭ�Ϸ��䷽ʽ
		>> strTemp >> m_stSetup.new_lMaxMemberNum			//Ĭ���������
		>> strTemp >> m_stSetup.new_lSessionTimeoutS		//�Ự��ʱ����

		>> strTemp >> m_stSetup.new_fIncreasedExp_2			//2�˾���ӳɱ���
		>> strTemp >> m_stSetup.new_fIncreasedExp_3			//3�˾���ӳɱ���
		>> strTemp >> m_stSetup.new_fIncreasedExp_4			//4�˾���ӳɱ���
		>> strTemp >> m_stSetup.new_fIncreasedExp_5			//5�˾���ӳɱ���
		>> strTemp >> m_stSetup.new_fIncreasedExp_6			//6�˾���ӳɱ���
		>> strTemp >> m_stSetup.new_lExperienceRadii		//�������뾶����λ����
		>> strTemp >> m_stSetup.new_lExpDiff				//�����
		>> strTemp >> m_stSetup.new_lExpLimit				//��Ҿ���ֵ�ݶ�������ϱ���
		>> strTemp >> m_stSetup.new_fExpAmerce				//����ͷ�ϵ��
		>> strTemp >> m_stSetup.new_fExpHortation			//���齱��ϵ��
		>> strTemp >> m_stSetup.new_fExpMinLvlDiff			//���齱����ʼ����С�ȼ���

		>> strTemp >> m_stSetup.vigour_fIncreasedExp_2		//2��Ԫ���ӳɱ���
		>> strTemp >> m_stSetup.vigour_fIncreasedExp_3		//3��Ԫ���ӳɱ���
		>> strTemp >> m_stSetup.vigour_fIncreasedExp_4		//4��Ԫ���ӳɱ���
		>> strTemp >> m_stSetup.vigour_fIncreasedExp_5		//5��Ԫ���ӳɱ���
		>> strTemp >> m_stSetup.vigour_fIncreasedExp_6		//6��Ԫ���ӳɱ���
		>> strTemp >> m_stSetup.vigour_lExpDiff				//Ԫ����
		>> strTemp >> m_stSetup.vigour_lExpLimit			//Ԫ������ֵ�ݶ��������
		>> strTemp >> m_stSetup.vigour_fExpAmerce			//Ԫ���ͷ�ϵ��
		>> strTemp >> m_stSetup.vigour_fExpHortation		//Ԫ������ϵ��
		>> strTemp >> m_stSetup.vigour_fExpMinLvlDiff		//Ԫ��������ʼ����С�ȼ���
		>> strTemp >> m_stSetup.vigour_fOccExpSpRatio		//ְҵ�����SP��ֵ
		

		>> strTemp >> m_stSetup.lCommoCoolDownTime		//������ȴʱ��
		>> strTemp >> m_stSetup.fPkToPvpValue			//pk/pvp����ֵ
		>> strTemp >> m_stSetup.pszPKToPvPScript		//pk/pvp���������Ľű�
		>> strTemp >> m_stSetup.dwPkCountPerAttack		//
		>> strTemp >> m_stSetup.dwActTime				//ʩ�Ŷ���ʱ��
		>> strTemp >> m_stSetup.dwHpMpRestoreTime
		>> strTemp >> m_stSetup.lPostage[0]				//ϵͳ�ʼ�
		>> strTemp >> m_stSetup.lPostage[1]				//��ͨ�ʼ�
		>> strTemp >> m_stSetup.lPostage[2]				//��ͨ�����ʼ�
		>> strTemp >> m_stSetup.lPostage[3]				//�����ʼ�����
		>> strTemp >> m_stSetup.lPostage[4]				//��ȡ�ʼ�����
		>> strTemp >> m_stSetup.lPostage[5]				//�����ʼ�����

		>> strTemp >> m_stSetup.lMaxName				//������������
		>> strTemp >> m_stSetup.lMaxSubject				//�����������
		>> strTemp >> m_stSetup.lMaxText				//�ʼ������������
		>> strTemp >> m_stSetup.lMaxExText				//�ʼ������ı����������
		>> strTemp >> m_stSetup.lMaxMoney				//����Ǯ

		>> strTemp >> m_stSetup.fMaxRemainTime[0]				//ϵͳ�ʼ�
		>> strTemp >> m_stSetup.fMaxRemainTime[1]				//��ͨ�ʼ�
		>> strTemp >> m_stSetup.fMaxRemainTime[2]				//��ͨ�����ʼ�
		>> strTemp >> m_stSetup.fMaxRemainTime[3]				//�����ʼ�
		>> strTemp >> m_stSetup.fMaxRemainTime[4]				//��ȡ�ʼ�
		>> strTemp >> m_stSetup.fMaxRemainTime[5]				//�����ʼ�
		>> strTemp >> m_stSetup.lPrisonIslandPkValue			//���ŵ���������PKֵ
		>> strTemp >> m_stSetup.lMailDelayTime					//�ʼ���ʱʱ��
	
		//>> strTemp >> m_stSetup.lMaxMailGroup					 //Ⱥ�������˵��������







		>> strTemp >> m_stSetup.dwGoldGoodsId
		>> strTemp  >> m_stSetup.dwSilverGoodsId

		>> strTemp  >> m_stSetup.dwWeimianGoodsId		//λ�澫�����ID

		>> strTemp >> m_stSetup.dwNpcSellGoldToSilver

		>> strTemp >> m_stSetup.lMaxGoodInfoNum			//���[��������Ʒ��Ϣ]��������

		>> strTemp >> m_stSetup.lReliveDebuffId		// ��Ҹ���DebuffID
		>> strTemp >> m_stSetup.lReliveDebuffLevel  // ��Ҹ���DebuffLevel	
		>> strTemp >> m_stSetup.pszLottery          // ��Ʊϵͳ���ýű�
		;
		// end ;

		LoadGameSetup("setup/gamesetup.ini");
	
	rfClose(prfile);
	return true;
}

// ��setup\gamesetup.ini��ȡ
BOOL CGlobeSetup::LoadGameSetup(const char* filename)
{
	//////////////////////////////////////////////////////////////////////////
	// �� setup\GameSetup.ini ��ȡ
	//////////////////////////////////////////////////////////////////////////
	
	CRFile *prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		char str[256];
		_snprintf(str, 256, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return FALSE;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	string strTemp;
	stream	
		>> strTemp >> m_stSetup.fExpScale
		>> strTemp >> m_stSetup.fExpScriptScale
		>> strTemp >> m_stSetup.fOccuExpScale
		>> strTemp >> m_stSetup.fOccuExpScriptScale
		>> strTemp >> m_stSetup.fSpScale
		>> strTemp >> m_stSetup.fSpScriptScale
		>> strTemp >> m_stSetup.fMonsterDropGoodsScale

		//���;������
		>> strTemp >> m_stSetup.dwPresentExpCooldown
		>> strTemp >> m_stSetup.fPresentExpScaleInPersonalHouse
		>> strTemp >> m_stSetup.fPresentExpScaleInSafeArea
		>> strTemp >> m_stSetup.fPresentExpScale
		>> strTemp >> m_stSetup.lPresentExpBuffId >> m_stSetup.lPresentExpBuffLvl

		>> strTemp >> m_stSetup.bAllowClientRunScript

		>> strTemp >> m_stSetup.bAllowClientChangePos

		>> strTemp >> m_stSetup.lShowSpouseNameStep

		>> strTemp >> m_stSetup.dwSavePointTime

		// ��ɫ��������
		>> strTemp >> m_stSetup.dwDelDays
		>> strTemp >> m_stSetup.dwDelLevelCondition		//��ɾ����ɫ��Ҫ�ﵽ�ĵȼ�
		>> strTemp >> m_stSetup.strSpeStr	// ���и������ַ������Ը���


		//���Ʒ���Ϣ������������
		>> strTemp >> m_stSetup.lSendMsgLimitPlayerNum
		//���Ʒ���Ϣʱ��������
		>> strTemp >> m_stSetup.lSendMsgLimitDistance
		//���Ʒ�������Ϣʱ����������
		>> strTemp >> m_stSetup.lSendSkillMsgLimitPlayerNum
		//�������Ե���Χ���ӳ�ʱ��
		>> strTemp >> m_stSetup.lUpdateOtherPropertyDelayTime
		//����AddShape��Ϣʱ�ļ��ʱ��
		>> strTemp >> m_stSetup.lSendAddShapeMsgInteTime
		//��������AddShape��Ϣ�ľ�������
		>> strTemp >> m_stSetup.lSendAddSahpeMsgDistance
		//����״̬��ʱ���ʱ��
		>> strTemp >> m_stSetup.lBaseModuleTime	
		//�ܼ���Խ�����
		>> strTemp >> m_stSetup.lTotalMaxSkillNum	
		//ÿ�뼼��Խ�����
		>> strTemp >> m_stSetup.fOneSecMaxSkillNum
		//��NPC�ű�Խ�����
		>> strTemp >> m_stSetup.lTotalMaxNpcScriptsNum	
		//ÿ��NPC�ű�Խ�����
		>> strTemp >> m_stSetup.fOneSecMaxNpcScriptsNum	

		>> strTemp >> m_stSetup.lCheckMsgCount
		>> strTemp >> m_stSetup.fMaxMsgCountPerSecond
		>> strTemp >> m_stSetup.fMaxMistakeAverSpeed
		>> strTemp >> m_stSetup.fMaxMistakeMaxSpeed
		>> strTemp >> m_stSetup.lPingChekTime
		>> strTemp >> m_stSetup.lPingErrorBanTime
		>> strTemp >> m_stSetup.lReLiveTime

		//! ��Ϸ��������������
		>> strTemp //����ע��

		>> strTemp >> m_stSetup.Goods_Policy.ObjNum				>> m_stSetup.Goods_Policy.blockNum
		>> strTemp >> m_stSetup.Monster_Policy.ObjNum			>> m_stSetup.Monster_Policy.blockNum
		>> strTemp >> m_stSetup.Collection_Policy.ObjNum		>> m_stSetup.Collection_Policy.blockNum
		>> strTemp >> m_stSetup.Npc_Policy.ObjNum				>> m_stSetup.Npc_Policy.blockNum
		>> strTemp >> m_stSetup.Player_Policy.ObjNum			>> m_stSetup.Player_Policy.blockNum
		>> strTemp >> m_stSetup.Region_Policy.ObjNum			>> m_stSetup.Region_Policy.blockNum
		>> strTemp >> m_stSetup.SummonedCreature_Policy.ObjNum	>> m_stSetup.SummonedCreature_Policy.blockNum
		>> strTemp >> m_stSetup.Build_Policy.ObjNum				>> m_stSetup.Build_Policy.blockNum
		>> strTemp >> m_stSetup.Citygate_Policy.ObjNum			>> m_stSetup.Citygate_Policy.blockNum
		
		>> strTemp >> m_stSetup.lTimerVarNum	//Ԥ���䶨ʱ������������
		>> strTemp >> m_stSetup.lPreAllocedScriptNum // Ԥ����Ľű��ڴ������
		>> strTemp >> m_stSetup.lMaxPreAllocedScriptNum// Ԥ����Ľű��ڴ���������
		>> strTemp >> m_stSetup.lPDupRgnMaxNum  // �������˸��������
		>> strTemp >> m_stSetup.lPHDupRgnMaxNum  // �������˷��ݸ��������
		>> strTemp >> m_stSetup.lTDupRgnMaxNum  // ������Ӹ��������
		>> strTemp >> m_stSetup.lPDupRgnDelTime			// ���˸���ɾ��ʱ��
		>> strTemp >> m_stSetup.lCreatePDupRgnMaxNum	//��Ҵ������˸��������
		>> strTemp >> m_stSetup.lCreateTDupRgnMaxNum	//��Ҵ�����Ӹ��������
		>> strTemp >> m_stSetup.lCreateDupRgnDeltaTime //��Ҵ�������ʱ����
		>> strTemp >> m_stSetup.lSaveTime				// ��ұ������ݵ�ʱ����
		>> strTemp >> m_stSetup.lWSSaveTime			// WS�������ݵ�ʱ����
		>> strTemp >> m_stSetup.lCreateRgnPlayerLoadMax	//GS�����������������
		>> strTemp >> m_stSetup.lCreateRgnMonsterLoadMax	//GS������������������
		>> strTemp >> m_stSetup.lPDupRgnMaxPlayerNum  // ���˸����������
		>> strTemp >> m_stSetup.lPHDupRgnMaxPlayerNum  // ���˷��ݸ����������
		>> strTemp >> m_stSetup.lTDupRgnMaxPlayerNum  // ��Ӹ����������
		>> strTemp >> m_stSetup.lRgnKickPlayerDeltaTime  // ��������ʱ��
		>> strTemp >> m_stSetup.szHideName				// ���������
		>> strTemp >> m_stSetup.lMLevelEnablePlayerSearch // ���ݵȼ�����������ҹ���
		>> strTemp >> m_stSetup.uUpgradeTo10FontColor		//! �����ɹ�����10���ϣ�������ʾ��������ɫ
		>> strTemp >> m_stSetup.uUpgradeTo20BackIdx		//! �����ɹ�����20���ϣ��������ʾ�ı�������
		>> strTemp >> m_stSetup.uUpgrade10LostFontColor	//! 10������������ʧ�ܣ�������ʾ��������ɫ
		>> strTemp >> m_stSetup.lWarningDeltaTime	//! ���б���ʱ����
		>> strTemp >> m_stSetup.lOperBounsTime		// ������Ʋ������ͼ��ʱ��(����)
		>> strTemp >> m_stSetup.dwReportTime		// �ٱ�ʱ����	

		; // end ;

	return true;
}

// ���浽�ļ��б�
BOOL CGlobeSetup::Save(const char* filename)
{
	return true;
}

// ��ӵ�CByteArray
bool CGlobeSetup::AddToByteArray(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray, &m_stSetup, sizeof(tagSetup));
	return true;
}

// ����
bool CGlobeSetup::DecordFromByteArray(BYTE* pByte, long& pos)
{
	_GetBufferFromByteArray(pByte, pos, &m_stSetup, sizeof(tagSetup));
	return true;
}

// ��ȡ���ŭֵ����
WORD CGlobeSetup::GetBaseMaxRp(BYTE lOccupation, BYTE lLevel)
{
	if (lOccupation==0)	// ��ʿ
	{
		if (lLevel>=GetSetup()->wBaseRp_Lv1 && lLevel<GetSetup()->wBaseRp_Lv2)
		{
			return GetSetup()->wBaseMaxRp_Lv1;	// ��һ��
		}
		else if (lLevel>=GetSetup()->wBaseRp_Lv2)
		{
			return GetSetup()->wBaseMaxRp_Lv2;	// �ڶ���
		}
	}

	return 0;
}


