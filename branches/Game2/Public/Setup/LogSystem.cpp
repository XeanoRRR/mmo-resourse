#include "StdAfx.h"
#include "LogSystem.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// ��̬��Ա��ʼ��
LogSystem::tagLogSystem LogSystem::m_stSetting;

/// ����
LogSystem::LogSystem(void)
{
}

/// ����
LogSystem::~LogSystem(void)
{
}



/// ���ļ�������־ϵͳ����
bool LogSystem::Load(const char* filename)
{
	ifstream stream;
	stream.open(filename);
	if(!stream.is_open())
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!",__FUNCTION__, filename);
		return false;
	}

	string strTemp;
	stream	
		// ��Ʒ���
		>> strTemp >> GetSetting()->bGoodsTradeLog			//������־
		>> strTemp >> GetSetting()->bGoodsShopLog			//�̵꽻��

		>> strTemp >> GetSetting()->bGoodsGetFromRegion		//ʰȡ
		>> strTemp >> GetSetting()->bGoodsBuyFromNPC		//��NPC����

		>> strTemp >> GetSetting()->bGoodsSellToNPC			//���۸�NPC
		>> strTemp >> GetSetting()->bGoodsDropToRegion		//������ʧ
		>> strTemp >> GetSetting()->bGoodsLostByBroken		//����ʧ
		>> strTemp >> GetSetting()->bGoodsLostByUpgrade		//������ʧ
		>> strTemp >> GetSetting()->bGoodsLostByDead		//������ʧ
		//�ֿ�����
		>> strTemp >> GetSetting()->bGoodsDepotSet
		>> strTemp >> GetSetting()->bGoodsDepotGet
		>> strTemp >> GetSetting()->bGoodsBankSet
		>> strTemp >> GetSetting()->bGoodsBankGet

		>> strTemp >> GetSetting()->bGoodsSplit
		>> strTemp >> GetSetting()->bGoodsEnhange
		>> strTemp >> GetSetting()->bGoodsMake
		>> strTemp >> GetSetting()->bGoodsUesed

		>> strTemp >> GetSetting()->bGoodsUpgradeSuccess	// �����ɹ�
		>> strTemp >> GetSetting()->bGoodsUpgradeFailure	// ����ʧ��

		>> strTemp >> GetSetting()->bGoodsGemExchange		// ��ʯ�һ�
		>> strTemp >> GetSetting()->bGoodsJewelryMade		// ��������

		>> strTemp >> GetSetting()->bGoodsLargess			// ������Ʒ


		//����ȼ�
		>> strTemp >> GetSetting()->bExpDecrease			//��������
		>> strTemp >> GetSetting()->bExpIncrease			//�������

		>> strTemp >> GetSetting()->bLevelLog				//�ȼ��仯

		>> strTemp >> GetSetting()->bTitileLog				//��ݱ仯

		>> strTemp >> GetSetting()->bDeleteLog				//ɾ����־

		>> strTemp >> GetSetting()->bDiedLog				//������־

		//�������
		>> strTemp >> GetSetting()->bTeamJion				//�����Ա
		>> strTemp >> GetSetting()->bTeamLeave				//��Ա�뿪

		//pk���
		>> strTemp >> GetSetting()->bPlayerKiller
		>> strTemp >> GetSetting()->bPlayerKilled

		// ������
		>> strTemp >> GetSetting()->bFactionCreate
		>> strTemp >> GetSetting()->bFactionDisband
		>> strTemp >> GetSetting()->bFactionApply
		>> strTemp >> GetSetting()->bFactionQuit

		>> strTemp >> GetSetting()->bFactionJoin
		>> strTemp >> GetSetting()->bFactionFireOut

		>> strTemp >> GetSetting()->bFactionTitleChanged
		>> strTemp >> GetSetting()->bFactionPurviewAdd
		>> strTemp >> GetSetting()->bFactionPurviewSub
		>> strTemp >> GetSetting()->bFactionMasterChnaned

		>> strTemp >> GetSetting()->bFactionExp
		>> strTemp >> GetSetting()->bFactionLel

		// ����
		>> strTemp >> GetSetting()->bNormalChat				//��Ļ����
		>> strTemp >> GetSetting()->bRegionChat				//��������
		>> strTemp >> GetSetting()->bFactionChat			//�������
		>> strTemp >> GetSetting()->bUnionChat				//��������
		>> strTemp >> GetSetting()->bTeamChat				//��������
		>> strTemp >> GetSetting()->bPrivateChat			//˽��
		>> strTemp >> GetSetting()->bGMCommand				//GMָ��

		>> strTemp >> GetSetting()->bChMap0
		>> strTemp >> GetSetting()->bChMap1
		>> strTemp >> GetSetting()->bChMap2

		>> strTemp >> GetSetting()->bCardDealing

		>> strTemp >> GetSetting()->bIncrement
		; // end ;


	stream.close();
	return true;
}

/// ����
bool LogSystem::AddToByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray, GetSetting(), sizeof(tagLogSystem));
	return true;
}

/// ����
bool LogSystem::DecordFromByteArray(uchar* pByte, long& pos)
{
	_GetBufferFromByteArray(pByte, pos, GetSetting(), sizeof(tagLogSystem));
	return true;
}
