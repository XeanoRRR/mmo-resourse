#include "StdAfx.h"
#include ".\globesetup.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGlobeSetup::tagSetup	CGlobeSetup::m_tagSetup;
CGlobeSetup::CGlobeSetup(void)
{
}

CGlobeSetup::~CGlobeSetup(void)
{
}

// ��ȡ
bool CGlobeSetup::Load(const char* filename)
{
	stringstream stream;	
	CRFile* prfile = rfOpen(filename);
	if( !prfile )
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return FALSE;
	}

	prfile->ReadToStream(stream);
	rfClose(prfile);

	char strTemp[256];
	long a,r,g,b;

	// ��Ʒ�����̵�۸�����۾���
	stream >> strTemp >> GetSetup()->fMoneyBaseRate;

	// ��Ʒ�����̵�۸��۾���
	stream >> strTemp >> GetSetup()->fMoneyRate;

	// ��ɫ�����
	stream >> strTemp >> a;
	GetSetup()->btMaxCharactersNum = (BYTE)a;
	
	//��ɫ���ȼ�
	stream >> strTemp >> a;
	GetSetup()->dwMaxPlayerLevel = (DWORD)a;
	// ��ҵ�����
	stream >> strTemp >> a;
	GetSetup()->dwMoneyIndex = (DWORD)a;

	// ���ҵ�����
	stream >> strTemp >> a;
	GetSetup()->dwSilverMoneyIndex = (DWORD)a;

	// ������Ϣ��ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorWarningText = D3DCOLOR_ARGB(a,r,g,b);

	// ���ѡ�е�shape
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorMouseShape = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorWhiteName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorWhiteNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorPurpleName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorPurpleNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorRedName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorRedNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorOrangeName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorOrangeNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBlueName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBlueNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorYellowName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorYellowNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGrayName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGrayNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// û�д򿪹��ұ�־ʱ,������ҿͻ���Ӧ����ʾ��������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorNoShowCountryName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorNoShowCountryNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ͬ��Ӫ��
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorAllianceName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorAllianceNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ��ͬ��Ӫ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ��������
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTeammateName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTeammateNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// Boss����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBossName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBossNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// NPC����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorNpcName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorNpcNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// Monster����
	for (int i=0;i<7;i++)
	{
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorMonsterName[i] = D3DCOLOR_ARGB(a,r,g,b);
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorMonsterNameBack[i] = D3DCOLOR_ARGB(a,r,g,b);
	}
	// Monster�ĳ�ν����
	for (int i=0;i<8;i++)
	{
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorMonsterTitelName[i] = D3DCOLOR_ARGB(a,r,g,b);
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorMonsterTitelNameBack[i] = D3DCOLOR_ARGB(a,r,g,b);
	}

	// DiedMonster����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorDiedMonsterName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorDiedMonsterNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// Goods����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// CityGate����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorCityGateName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorCityGateNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ��λ�ƺ���ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorKnight = D3DCOLOR_ARGB(a,r,g,b);

	// ���ƺ���ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorFaction = D3DCOLOR_ARGB(a,r,g,b);

	// ѫ�³ƺ���ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorMedal = D3DCOLOR_ARGB(a,r,g,b);

	// �����Ѫ��ֵ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBloodTextMonster = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBloodTextMonsterBack = D3DCOLOR_ARGB(a,r,g,b);

	// ��Ҽ�Ѫ��ֵ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBloodTextPlayer = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBloodTextPlayerBack = D3DCOLOR_ARGB(a,r,g,b);

	// ��Ҽ�Ѫ��ֵ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorAddBloodTextPlayer = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorAddBloodTextPlayerBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����ֵ������ʾ��ɫ
	stream >> strTemp >> r >> g >> b;
	GetSetup()->colorExpTipText = D3DCOLOR_ARGB(0, r, g, b);

	// ����ֵ������ʾ������ɫ
	stream >> strTemp >> r >> g >> b;
	GetSetup()->colorExpTipTextBack = D3DCOLOR_ARGB(0, r, g, b);

	// ��ѫֵ������ʾ��ɫ
	stream >> strTemp >> r >> g >> b;
	GetSetup()->colorConTipText = D3DCOLOR_ARGB(0, r, g, b);

	// ��ѫֵ������ʾ������ɫ
	stream >> strTemp >> r >> g >> b;
	GetSetup()->colorConTipTextBack = D3DCOLOR_ARGB(0, r, g, b);

	// ����ֵ��ʾ����ʱ��
	stream >> strTemp >> GetSetup()->dwExpTipTextTimeLength;

	// ����ֵ��ʾ�����ʾ����
	stream >> strTemp >> GetSetup()->dwExpTipTextLineMax;

	// ����ֵ��ʾ�м��
	stream >> strTemp >> GetSetup()->fExpTipTextLineSpace;

	// ����ֵ��ʾ�������ű���
	stream >> strTemp >> GetSetup()->fExpTipTextNumberScale;

	// ���ͷ����Ϣ���Ƹ߶�����
	stream >> strTemp >> GetSetup()->fPlayerInfoHeight;
	stream >> strTemp >> GetSetup()->fMonsterInfoHeight;

	// ���컺����Ŀ����
	stream >> strTemp >> a;
	GetSetup()->dwChatItemNum = (DWORD)a+1;

	// ������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorChatBackBar = D3DCOLOR_ARGB(a,r,g,b);

	for (int i=0;i<12;i++)
	{
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorChatText[i] = D3DCOLOR_ARGB(a,r,g,b);
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorChatTextBack[i] = D3DCOLOR_ARGB(a,r,g,b);
		stream >> strTemp >> a >> r >> g >> b;
		GetSetup()->colorChatTextShadow[i] = D3DCOLOR_ARGB(a,r,g,b);
	}

	// �ö�������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTopChat = D3DCOLOR_ARGB(a,r,g,b);

	// �ö����������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTopChatShadow = D3DCOLOR_ARGB(a,r,g,b);

	// �ö��������
	stream >> strTemp >> a;
	GetSetup()->dwTopChatPropIndex = (DWORD)a;

	// �ö�������������
	stream >> strTemp >> a;
	GetSetup()->dwTopChatMaxNum = (DWORD)a;

	// �ö�����ͣ��ʱ��
	stream >> strTemp >> a;
	GetSetup()->dwTopChatTime = (DWORD)a;

	// �ö������в�Y����
	stream >> strTemp >> a;
	GetSetup()->lTopChatY = a;

	// �ö��������������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTopChatInBox = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTopChatInBoxBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorTopChatInBoxShadow = D3DCOLOR_ARGB(a,r,g,b);

	// ����� ��ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBroadcast = D3DCOLOR_ARGB(a,r,g,b);

	// ����� �����ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorBroadcastShadow = D3DCOLOR_ARGB(a,r,g,b);

	// ����� �ٶ�
	stream >> strTemp >> a;
	GetSetup()->dwBroadcastSpeed = (DWORD)a;

	// ����� ѭ������
	stream >> strTemp >> a;
	GetSetup()->dwBroadcastLoop = (DWORD)a;

	// ����� Y����
	stream >> strTemp >> a;
	GetSetup()->lBroadcastY = a;

	// �ű����� ��ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorScriptBroadcast = D3DCOLOR_ARGB(a,r,g,b);

	// �ű����� ������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorScriptBroadcastBack = D3DCOLOR_ARGB(a,r,g,b);

	// �ű����� �����ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorScriptBroadcastShadow = D3DCOLOR_ARGB(a,r,g,b);

	// ��ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsWhite = D3DCOLOR_ARGB(a,r,g,b);

	// ��ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsBlue = D3DCOLOR_ARGB(a,r,g,b);

	// ��ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsRed = D3DCOLOR_ARGB(a,r,g,b);

	// ��Ʒ���½�������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsDrop = D3DCOLOR_ARGB(a,r,g,b);

	// ��ɫ(�������Գ���4��)
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsGold = D3DCOLOR_ARGB(a,r,g,b);

	// ����ɫ(��������)
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsDarkGold = D3DCOLOR_ARGB(a,r,g,b);

	// ��Ʒ 100�������!!!!????
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsGod = D3DCOLOR_ARGB(a,r,g,b);

	// ��ħ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsFuMo = D3DCOLOR_ARGB(a,r,g,b);

	// ��ħ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsDesc = D3DCOLOR_ARGB(a,r,g,b);

	// ��ħ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsPrice = D3DCOLOR_ARGB(a,r,g,b);

	//��������������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsPurpleBaseDesp = D3DCOLOR_ARGB(a,r,g,b);

	//׷������������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsPurpleAddtionDesp = D3DCOLOR_ARGB(a,r,g,b);

	//��������������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsOrangeBaseDesp = D3DCOLOR_ARGB(a,r,g,b);

	//׷������������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorGoodsOrangeAddtionDesp = D3DCOLOR_ARGB(a,r,g,b);

	// �����̵�װ���ɫ
	// �����̵�װ�߿���ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorShopBar = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorShopWireBar = D3DCOLOR_ARGB(a,r,g,b);

	// �����̵�����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorShopName = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorShopNameBack = D3DCOLOR_ARGB(a,r,g,b);

	// ����һ����Ѫ��ֵ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorCriticalBloodText = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorCriticalBloodTextBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> GetSetup()->fCriticalTextScale;

	// ������������ʾ����
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorFriendOnlineOffline = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorFriendOnlineOfflineBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorFriendOnlineOfflineShadow = D3DCOLOR_ARGB(a,r,g,b);

	// �ȼ�����ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorLowerLevel = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorMiddleLevel = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorUpperLevel = D3DCOLOR_ARGB(a,r,g,b);

	// ��Ʒ�ı���
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorItemBlueBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorItemRedBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorHandItemCanPlaceBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorHandItemCanChangeBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorHandItemCannotChangeBack = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorInShopMirrorBack = D3DCOLOR_ARGB(a,r,g,b);
	
	//�й����������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyPlayerNameGreen = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyPlayerNameGray = D3DCOLOR_ARGB(a,r,g,b);
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyPlayerNamePurple = D3DCOLOR_ARGB(a,r,g,b);

	// �������ж���Ӫ������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyNameInBattleZone = D3DCOLOR_ARGB(a,r,g,b);

	// �������ж���Ӫ������ɫ����ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyNameInBattleZoneBack = D3DCOLOR_ARGB(a,r,g,b);

	// ս�����Եж���Ӫ������ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyNameInWarZone = D3DCOLOR_ARGB(a,r,g,b);

	// ս�����Եж���Ӫ������ɫ����ɫ
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorEnemyNameInWarZoneBack = D3DCOLOR_ARGB(a,r,g,b);

	// ���������Ч
	stream >> strTemp >> a;
	GetSetup()->dwLevelUpEffectID = (DWORD)a;

	// ��Ҽ���������Ч
	stream >> strTemp >> a;
	GetSetup()->dwSkillLevelUpEffectID = (DWORD)a;

	// �������������Ч
	stream >> strTemp >> a;
	GetSetup()->dwReputeLevelUpEffectID = (DWORD)a;

	//���ְҵ������Ч
    stream >> strTemp >> a;
    GetSetup()->dwOccuLevelUpEffectID = (DWORD)a;

	// ���񱣻��ֵ�������ɫ�������ߣ�
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorQuestMstByCreator = D3DCOLOR_ARGB(a,r,g,b);

	// ���񱣻��ֵ�������ɫ���Ǵ����ߣ�
	stream >> strTemp >> a >> r >> g >> b;
	GetSetup()->colorQuestMstByOther = D3DCOLOR_ARGB(a,r,g,b);

	// ���͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
	stream >> strTemp >> a;
	GetSetup()->dwPlayerAlphaFadeTimeLength = (DWORD)a;

	// ����͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
	stream >> strTemp >> a;
	GetSetup()->dwMonsterAlphaFadeTimeLength = (DWORD)a;

	// �ɼ���͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
	stream >> strTemp >> a;
	GetSetup()->dwCollectionAlphaFadeTimeLength = (DWORD)a;

	// ��Ʒ͸������ʱ�䳤��(Ϊ0���ʾû��͸������)
	stream >> strTemp >> a;
	GetSetup()->dwGoodsAlphaFadeTimeLength = (DWORD)a;

	// ��ͨ�ʼ�������
	stream >> strTemp >> a;
	GetSetup()->dwNormalMailPrice = (DWORD)a;

	// ��ͨ�����ʼ�������
	stream >> strTemp >> a;
	GetSetup()->dwNormalGoodsMailPrice = (DWORD)a;

	// �����ʼ�������
	stream >> strTemp >> a;
	GetSetup()->dwPresentMailPrice = (DWORD)a;

	// ��Ҫ�ʼ�������
	stream >> strTemp >> a;
	GetSetup()->dwRequestMailPrice = (DWORD)a;

	// ���������ʼ�������
	stream >> strTemp >> a;
	GetSetup()->dwPayGoldMailPrice = (DWORD)a;

	// Tab�����Ҷ���ķ�Χ
	stream >> strTemp >> a;
	GetSetup()->lAreaByTab = (long)a;

	// loading���ӳ�ʱ��
	stream >> strTemp >> a;
	GetSetup()->dwLoadingDelayTime = (DWORD)a;

	// ����ʹ�ü��ܷ�����Ϣ����
	stream >> strTemp >> a;
	GetSetup()->lUseSkillMsgCount = (long)a;

	// �����ƶ�������Ϣ����
	stream >> strTemp >> a;
	GetSetup()->lMoveMsgCount = (long)a;

	// �����������Ϣ�����ӳ�ʱ��
	stream >> strTemp >> a;
	GetSetup()->dwSendFinishBagDelayTime = (DWORD)a;

	//����ҽ������ʱ��
	stream >> strTemp >> a;
	GetSetup()->dwAnswerTime = (DWORD)a;
	//����ҽ����Զ���ʱ��
	stream >> strTemp >> a;
	GetSetup()->dwAutoOpenTime = (DWORD)a;
	//����Ҿ�����Ϣ��ʾ��ID
	stream >> strTemp >> a;
	GetSetup()->dwSonudID	= (DWORD)a;

	stream >> strTemp >> m_tagSetup.lEncryptType;

	//�����������ܵĵȼ�����
	stream >> strTemp >> a;
	GetSetup()->dwLimit=(DWORD)a;
	//�����������ܵĵȼ�����
	stream >> strTemp >> a;
	GetSetup()->lUseSkillTime = (long)a;
	//�������ʹ�����Ƶ�ʱ��
	stream >> strTemp >> a;
	GetSetup()->lReliveTime = (long)a;
	//Ĭ��ʹ�ø�����Ʒ��id
	stream >> strTemp >> a;
	GetSetup()->lReliveItem = (long)a;
	//CPֵ����ʾ�ȼ�����
	stream >> strTemp >> a;
	GetSetup()->lPKCPShowLevel = (long)a;
	//˽�������������λ��Ŀ
	stream >> strTemp >> a;
	GetSetup()->dwMaxNumPrivateChat = (DWORD)a;
	//˽����ʷÿҳ��ʾ��Ŀ��
	stream >> strTemp >> a;
	GetSetup()->dwPrivateChatHistoryMaxItem = (DWORD)a;
	//˽����Ϣ���������Ŀ����
	stream >> strTemp >> a;
	GetSetup()->dwMaxPriChatMsgBoxTotalItem = (DWORD)a;

	//���̻��ҵ�ID
	stream >> strTemp >> a;
	GetSetup()->dwFBCoinID = (DWORD)a;
    //��С��������
    stream >> strTemp >> GetSetup()->fminimumOpDistance;

    //ֱ�߷�������������ƶ�����
    stream >> strTemp >> GetSetup()->fLineMaxDistanceAllowToMove;
    float abb = GetSetup()->fLineMaxDistanceAllowToMove;
	/***********************************************************************/
	/* zhaohang add */
	//�������ʶλ�����Ƶı���
	stream >> strTemp >> GetSetup()->strWeiMianName;
	//�ɱ����������ѷ���
	stream >> strTemp >> GetSetup()->agentExtracust;
	//�ɱ�����һ����Ӧ��λ����
	stream >> strTemp >> GetSetup()->oderPerNum;
	//�����İ������ļ�����
	stream >> strTemp >> GetSetup()->strSkillDocCon;
	//������ʾ�����ļ�����
	stream >> strTemp >> GetSetup()->strSkillShowIni;
	//������̬�����ļ�����
	stream >> strTemp >> GetSetup()->strSkillPoseIni;
	//�������������ļ�����
	stream >> strTemp >> GetSetup()->strSkillAttrIniSetup;
	/***********************************************************************/
	return true;
}
