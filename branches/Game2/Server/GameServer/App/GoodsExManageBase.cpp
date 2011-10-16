/*
	��Ʒ��չ���ܹ������������壻
	
	2007-12-11 �ſ���
*/


#include "StdAfx.h"
#include "GoodsExManage.h"

#include "Player.h"

#include "Goods/CGoods.h"
#include "Goods/CGoodsFactory.h"

#include "MessagePackage/CS2CContainerObjectMove.h"
#include "MessagePackage/CS2CContainerObjectAmountChange.h"

#include "PlayerDepot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//! Ҫ�����İ����б�
static long arr_lSearchAim[] = 
{
	CS2CContainerObjectMove::PEI_PACKET,//��ͨ����
	CS2CContainerObjectMove::PEI_PACK1,	//�ӱ�����λ1
	CS2CContainerObjectMove::PEI_PACK2,	//�ӱ�����λ2
	CS2CContainerObjectMove::PEI_PACK3,	//�ӱ�����λ3
	CS2CContainerObjectMove::PEI_PACK4,	//�ӱ�����λ4
	CS2CContainerObjectMove::PEI_PACK5	//�ӱ�����λ5
};

//! �õ���̬ʵ��
GoodsExManage& GoodsExManage::GetInstance(void)
{
	return c_GoodsExManage;
}

//! ��̬ʵ��
GoodsExManage GoodsExManage::c_GoodsExManage;


GoodsExManage::GoodsExManage(void)
{
	m_Immobile.fAmendIT = 1.0f;
	m_Immobile.fAmendMA = 1.0f;
	m_Immobile.fAmendMC = 1.0f;
	m_Immobile.fAmendPA = 1.0f;
	m_Immobile.fAmendPC = 1.0f;
}

GoodsExManage::~GoodsExManage(void)
{

}

//! ͨ����Ϣ��ȡ������Ϣ����Ϣ���ͣ�MSG_W2S_SERVER_SENDSETUP�����ݱ�ţ�LOAD_GOODSEX
bool GoodsExManage::InitSetup(BYTE *pData, long &lPos)
{
	if (NULL == pData || 0 > lPos) return false;
	
	return m_GoodsExSetup.Unserialize(pData, lPos);
}

//! �򿪿ͻ�����Ʒ��չ��������
bool GoodsExManage::OpenGoodsExWindow(CPlayer *pPlayer, tagGoodsExWndSession& GoodsExWndSession)
{
	if (eGWT_End <= GoodsExWndSession.uWndType || 0 == GoodsExWndSession.uRadius || NULL == pPlayer) return false;

	//! �ֿ�Ự��������Ʒ��չ����
	if (eGWT_DepotWindow == GoodsExWndSession.uWndType)
	{
		if(!pPlayer->GetPlayerDepot()->CreateSession(GoodsExWndSession.uRadius, GoodsExWndSession.lPosX, GoodsExWndSession.lPosY)) 
			return false;
	}
	else
	{
		//! �жϴ򿪵�ʱ���Ƿ�Ϸ�
		long lRadius = GoodsExWndSession.uRadius;
		//! ���Ự�뾶����ΪС��0����ʾ��������
		if (0 < lRadius)
		{
			long lPlayerPosX = pPlayer->GetTileX();
			long lPlayerPosY = pPlayer->GetTileY();

			//! �ж�����Ƿ��ڻỰ��Χ��
			long lDeltaX	= abs(lPlayerPosX - GoodsExWndSession.lPosX); //! X����
			long lDeltaY	= abs(lPlayerPosY - GoodsExWndSession.lPosY); //! Y����

			if (lDeltaX > lRadius || lDeltaY > lRadius)
			{
				return false;
			}
		}

		//! �ر�ԭ�д���
		map<CGUID, tagGoodsExWndSession>::iterator ite = m_mapOpendGEWnd.find(pPlayer->GetExID());
		if (m_mapOpendGEWnd.end() != ite)
		{
			CloseGoodsExWindow(pPlayer);
		}
		//! �����󴰿�
		m_mapOpendGEWnd[pPlayer->GetExID()] = GoodsExWndSession;
	}

	CMessage msg(MSG_S2C_GOODS_OPEN_WINDOWS);
	msg.Add(GoodsExWndSession.uWndType);
	msg.Add(GoodsExWndSession.uRadius);
	msg.Add(GoodsExWndSession.lPosX);
	msg.Add(GoodsExWndSession.lPosY);
	msg.SendToPlayer(pPlayer->GetExID());

	return true;
}

//! �رտͻ�����Ʒ��չ�������ڣ������ػỰ��
void GoodsExManage::CloseGoodsExWindow(CPlayer *pPlayer)
{
	m_mapOpendGEWnd.erase(pPlayer->GetExID());
	//CMessage msg(MSG_S2C_GOODS_CLOSE_WINDOWS);
	//msg.SendToPlayer(pPlayer->GetExID());
}

//!	��֤��Ϣ�Ự����
bool GoodsExManage::TestSession(const CGUID& guid, eGoodsExWindowType eGoodsWinType)
{
	map<CGUID, tagGoodsExWndSession>::iterator ite = m_mapOpendGEWnd.find(guid);
	if(m_mapOpendGEWnd.end() == ite) return false;

	CPlayer *pPlayer = GetGame()->FindPlayer(guid);
	assert(NULL != pPlayer);
	if(NULL == pPlayer) return false;
	
	long lRadius	= ite->second.uRadius;
	//! ���Ự�뾶����ΪС��0����ʾ��������
	if (0 >= lRadius) return true;

	long lPlayerPosX = pPlayer->GetTileX();
	long lPlayerPosY = pPlayer->GetTileY();

	//! �ж�����Ƿ��ڻỰ��Χ��
	long lDeltaX	= abs(lPlayerPosX - ite->second.lPosX); //! X����
	long lDeltaY	= abs(lPlayerPosY - ite->second.lPosY); //! Y����
	

	if (lDeltaX > lRadius || lDeltaY > lRadius)
	{
		m_mapOpendGEWnd.erase(ite);
		return false;
	}

	return (eGoodsWinType == ite->second.uWndType);
}

//! ��Ӧ�ͻ��˹ر���Ʒ��չ����
void GoodsExManage::OnClientCloseGoodsExWindow(CMessage *pMsg)
{
	m_mapOpendGEWnd.erase(pMsg->GetPlayerID());
}

//! ����������ҵ�һ������
CGoodsContainer* GoodsExManage::GetGoodsContainer(CPlayer *pPlayer, long lContianerId)
{
	switch(lContianerId)
	{
	case CS2CContainerObjectMove::PEI_PACKET:
		return pPlayer -> getOriginPack();
		break;
	//case CS2CContainerObjectMove::PEI_EQUIPMENT:
	//	return &( pPlayer -> m_cEquipment );
	//	break;
	//case CS2CContainerObjectMove::PEI_HAND:
	//	return &( pPlayer -> m_cHand );
	//	break;				
	//case CS2CContainerObjectMove::PEI_DEPOT:										
	//	return &( pPlayer -> m_cDepot);					
	//	break;				
	//case CS2CContainerObjectMove::PEI_PACK:
	//	return pPlayer ->getSubpackContainer();
	//	break;
	case CS2CContainerObjectMove::PEI_PACK1:
		return pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK2:
		return pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK3:
		return pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK4:
		return pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK5:
		return pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
		break;
	}
	return NULL;
}

//! Ϊ��Ҽ������߽�����Ʒ
BOOL GoodsExManage::LockGoods(CPlayer *pPlayer, const CGUID& GoodsGuid, BOOL bLock)
{
	static DWORD arrContainerType[] = 
	{
		CS2CContainerObjectMove::PEI_PACKET,
		CS2CContainerObjectMove::PEI_PACK1,
		CS2CContainerObjectMove::PEI_PACK2,
		CS2CContainerObjectMove::PEI_PACK3,
		CS2CContainerObjectMove::PEI_PACK4,
		CS2CContainerObjectMove::PEI_PACK5
	};

	assert(NULL != pPlayer);
	for (LONG i = 0; i < sizeof(arrContainerType) / sizeof(DWORD); ++i)
	{
		CAmountLimitGoodsContainer *pContainer = (CAmountLimitGoodsContainer*)GetGoodsContainer(pPlayer, arrContainerType[i]);
		if(NULL != pContainer)
		{
			CGoods *pGoods = (CGoods*)pContainer->FindIgnoreLock(GoodsGuid);
			if(NULL != pGoods)
			{
				if(bLock)
					pContainer->Lock(pGoods);
				else
					pContainer->Unlock(pGoods);
			}
		}
	}

	return FALSE;
}

//!		���������ָ����λ���ҵ�һ����Ʒ
CGoods* GoodsExManage::FindGoodsFromPlayer(CPlayer *pPlayer, long lContianerID, long lPos)
{
	CGoodsContainer *pContainer = GetGoodsContainer(pPlayer, lContianerID);

	if( NULL != pContainer )
	{
		return pContainer->GetGoods(lPos);
	}
	return NULL;
}


//! ����һ����Ʒ�Ƿ���ָ����λ����
CGoods* GoodsExManage::TestGoodsHere(CPlayer *pPlayer, const CGUID &GoodsGuid, long lContianerID, long lPos, BOOL bTestFrost)
{
	CGoods *pGoods = FindGoodsFromPlayer(pPlayer, lContianerID, lPos);

	if( NULL != pGoods )
	{
		if (pGoods->GetExID() == GoodsGuid)
		{
			if(bTestFrost && IsFrost(pGoods))
				return NULL;
			return pGoods;
		}
	}

	return NULL;
}

//!		������Ʒ����ռ��Ƿ��㹻
long GoodsExManage::TestPutWay(CPlayer *pPlayer, vector<tagGoodsOperate> &vecDelGoods, vector<CGoods*> &vPutGoods)
{
	if (NULL == pPlayer) return false;

	long NullGrilleNum	= 0; //! �ո���

	//! ����ɾ���Ķ����ճ����ĸ���
	for (long i = 0; i < (long)vecDelGoods.size(); ++i)
	{
		CGoods			*pGoods		= NULL;
		CGoodsContainer *pContainer = GetGoodsContainer(pPlayer, vecDelGoods[i].lContianerPos);
		if( NULL == pContainer ) return false;
		
		pGoods = pContainer->GetGoods(vecDelGoods[i].lGoodsPos);
		if( NULL == pGoods ) return false;


		vecDelGoods[i].lOperateWay = CS2CContainerObjectMove::OT_DELETE_OBJECT;
		if (!pGoods->HasAddonProperty(GAP_MAXOVERLAP))
		{
			++NullGrilleNum;	
		}
		else
		{
			long lCurrOverlapNum = pGoods->GetAmount();
			if(vecDelGoods[i].lOperateNum == lCurrOverlapNum)
				++NullGrilleNum;
			else 
				vecDelGoods[i].lOperateWay = CS2CContainerObjectMove::OT_CHANGE_AMOUNT;
		}
	}
	
	LONG lCanPutNum = NullGrilleNum;
	for (long i = 0; i < 6; ++i)
	{
		CVolumeLimitGoodsContainer* pCGoodsContainer = (CVolumeLimitGoodsContainer*)GetGoodsContainer(pPlayer, arr_lSearchAim[i]);
		if (NULL == pCGoodsContainer)
			continue;
		
		lCanPutNum += pCGoodsContainer->GetCanPutNum(vPutGoods);
		if (lCanPutNum >= (long)vPutGoods.size())
		{
			return 1;
		}
	}

	return lCanPutNum - (LONG)vPutGoods.size();
}


//!				����������Ʒ
bool GoodsExManage::AddGoods(CPlayer *pPlayer, vector<CGoods*>& vAddGoods, vector<long>& vOptType, vector<long>& vOptNum)
{
	if(NULL == pPlayer || vAddGoods.size() == 0 || vAddGoods.size() != vOptType.size()) return false;

	for( size_t i = 0; i < vAddGoods.size(); i ++ )
	{
		if( vAddGoods[i] )
		{
			if( vAddGoods[i] )
			{
				vOptNum[i] = vAddGoods[i]->GetAmount();
				BOOL bAdded=pPlayer->AddAndReturnGoodsWithProtect(&vAddGoods[i], TRUE, &vOptType[i]);					
			}
		}
	}
	return true;
}

//!				ɾ�������Ʒ
bool GoodsExManage::DelGoods(CPlayer *pPlayer, vector<tagGoodsOperate> &vDelGoods, DWORD dwLogType, const CGUID &optGuid)
{
	if (NULL == pPlayer) return false;
	
	for (long i = 0; i < (long)vDelGoods.size(); ++i)
	{
		//! �ҵ�����Ʒ������
		CVolumeLimitGoodsContainer* pCGoodsContainer = (CVolumeLimitGoodsContainer*)GetGoodsContainer(pPlayer, vDelGoods[i].lContianerPos);
		if( NULL != pCGoodsContainer )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( pCGoodsContainer -> GetGoods(vDelGoods[i].lGoodsPos) );
			assert(NULL != pGoods);
			if (NULL != pGoods)
			{
				long lAddType = 1;

				if( CS2CContainerObjectMove::OT_CHANGE_AMOUNT == vDelGoods[i].lOperateWay)
				{
					//! �ı�����
					lAddType = 2;
					pGoods -> SetAmount( pGoods -> GetAmount() - vDelGoods[i].lOperateNum);

					CS2CContainerObjectAmountChange coacMsg;
					coacMsg.SetSourceContainer( TYPE_PLAYER, pPlayer->GetExID(), vDelGoods[i].lGoodsPos );
					coacMsg.SetSourceContainerExtendID( vDelGoods[i].lContianerPos );
					coacMsg.SetObject( pGoods -> GetType(), pGoods -> GetExID() );
					coacMsg.SetObjectAmount( pGoods -> GetAmount());
					coacMsg.Send( pPlayer->GetExID() );
				}
				else
				{
					//##ɾ��
					CS2CContainerObjectMove comMsg;
					if( pCGoodsContainer -> Remove(pGoods, &comMsg) )
					{
						pPlayer->OnLoseGoods(pGoods);
						comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
						comMsg.SetSourceContainerExtendID( vDelGoods[i].lContianerPos );
						comMsg.Send( pPlayer->GetExID() );

						//##ɾ������Ʒ?
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:GoodsExManage::DelGoods][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
					}
				}

				switch(dwLogType)
				{
				case 380:
					GetLogicLogInterface()->logT380_goods_disassemble_log(pPlayer, pGoods, lAddType, optGuid, FALSE);
					break;
				case 390:
					GetLogicLogInterface()->logT390_goods_synthesize_log(pPlayer, pGoods, lAddType, optGuid, FALSE);
				    break;
				case 400:
					GetLogicLogInterface()->logT400_goods_upgrade_log(pPlayer, optGuid, pGoods, 0, 0);
					break;
				default:
				    break;
				}

				if(1 == lAddType)
				{
#ifdef _GOODSLOG1_
					if(pGoods)
					{
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:exmanageBase418][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
					}			
#endif	
					CGoodsFactory::GarbageCollect( &pGoods,42 );
				}
			}
			
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//! GoodsExSetup�Ľ��붨�壬��Ӧ����ӿڣ�Serialize��λ�ã�WS Game.cpp
bool GoodsExSetup::Unserialize(BYTE* pStream, long& lOffset)
{
	//! �ֽ�����
	m_mapDisassemblePrescription.clear();

	long lDpNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lDpNum; ++i)
	{
		tagDisassemblePrescription DisassemblePrescription;
		_GetBufferFromByteArray(pStream, lOffset, &DisassemblePrescription, sizeof(tagDisassemblePrescription));
		m_mapDisassemblePrescription[DisassemblePrescription.lOutGoodsQuality] = DisassemblePrescription;
	}

	//! �ϳ�����
	m_mapSynthesizePrescription.clear();

	long lSpNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lSpNum; ++i)
	{
		tagSynthesizePrescription SynthesizePrescription;
		_GetBufferFromByteArray(pStream, lOffset, &SynthesizePrescription, sizeof(tagSynthesizePrescription));
		m_mapSynthesizePrescription[SynthesizePrescription.lIndex] = SynthesizePrescription;
	}

	//! ��������
	m_lReinforceMaxPercent = _GetLongFromByteArray(pStream, lOffset);

	m_vecPowerIni.clear();

	long lPowerIniNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lPowerIniNum; ++i)
	{
		tagLevelBoundPA LevelBoundPA;
		LevelBoundPA.lMinLevel = _GetLongFromByteArray(pStream, lOffset);
		LevelBoundPA.lMaxLevel = _GetLongFromByteArray(pStream, lOffset);
		long lPANum = _GetLongFromByteArray(pStream, lOffset);
		for (long j = 0; j < lPANum; ++j)
		{
			tagPowerAccessorial PowerAccessorial;
			_GetBufferFromByteArray(pStream, lOffset, &PowerAccessorial, sizeof(tagPowerAccessorial));
			LevelBoundPA.mapPowerAccessorial[PowerAccessorial.lIntensity] = PowerAccessorial;
		}
		m_vecPowerIni.push_back(LevelBoundPA);
	}

	//! ����Ʒ�����б�
	long lIndexNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lIndexNum; ++i)
	{
		m_setBindGoodsIndex.insert(_GetLongFromByteArray(pStream, lOffset));
	}
	//! �����Ʒ�����б�
	lIndexNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lIndexNum; ++i)
	{
		m_setUntieGoodsIndex.insert(_GetLongFromByteArray(pStream, lOffset));
	}
	//! ������Ʒ�����б�
	lIndexNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lIndexNum; ++i)
	{
		m_setFrostGoodsIndex.insert(_GetLongFromByteArray(pStream, lOffset));
	}
	//! �ⶳ��Ʒ�����б�
	lIndexNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lIndexNum; ++i)
	{
		m_setThawGoodsIndex.insert(_GetLongFromByteArray(pStream, lOffset));
	}

	//! װ����������
	long lUpgradeSetNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lUpgradeSetNum; ++i)
	{
		tagArmUpgradeSet ArmUpgradeSet;
		ArmUpgradeSet.lMinLimitLevel = _GetLongFromByteArray(pStream, lOffset);
		ArmUpgradeSet.lMaxLimitLevel = _GetLongFromByteArray(pStream, lOffset);

		long lUpgradeSetNum = _GetLongFromByteArray(pStream, lOffset);
		for (long j = 0; j < lUpgradeSetNum; ++j)
		{
			tagUpgradeSet UpgradeSet;
			_GetBufferFromByteArray(pStream, lOffset, &UpgradeSet, sizeof(tagUpgradeSet));
			ArmUpgradeSet.mapUpgradeSet[UpgradeSet.ArmLevel] = UpgradeSet;
		}
		m_vecUpgradeSetList.push_back(ArmUpgradeSet);
	}

	//! ������Ʒ����
	long lExtraStuffNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lExtraStuffNum; ++i)
	{
		tagExtraStuff ExtraStuff;
		_GetBufferFromByteArray(pStream, lOffset, &ExtraStuff, sizeof(tagExtraStuff));
		m_vecExtraStuff.push_back(ExtraStuff);
	}

	//! װ����������
	long lUpgradeMoneySetNum = _GetLongFromByteArray(pStream, lOffset);
	for (long i = 0; i < lUpgradeMoneySetNum; ++i)
	{
		tagArmUpgradeMoneySet ArmUpgradeMoneySet;
		ArmUpgradeMoneySet.lMinLimitLevel = _GetLongFromByteArray(pStream, lOffset);
		ArmUpgradeMoneySet.lMaxLimitLevel = _GetLongFromByteArray(pStream, lOffset);

		long lUpgradeSetNum = _GetLongFromByteArray(pStream, lOffset);
		for (long j = 0; j < lUpgradeSetNum; ++j)
		{
			long id = _GetLongFromByteArray(pStream, lOffset);
			ArmUpgradeMoneySet.mapUpgradeMoneySet[id] = _GetLongFromByteArray(pStream, lOffset);
			
		}
		m_vecUpgradeMoneySetList.push_back(ArmUpgradeMoneySet);
	}

	m_lUpgradeLimit = _GetLongFromByteArray(pStream, lOffset);

	//! ħ������
	{
		//! װ���ȼ���Χ����
		m_ElementSetup.vEquipmentLevelBound.clear();
		//! ħ����������
		m_ElementSetup.vElementBound.clear();
		//! ���ʸ�����������
		m_ElementSetup.vHelpfulSetup.clear();
		//! ħ���ȼ���Ӧħ������ֵ
		m_ElementSetup.vElementValue.clear();
		//! Ԫ����������
		m_ElementSetup.vResetElement.clear();
		//! ������������
		m_ElementSetup.vResetXiangXing.clear();


		//! װ���ȼ���Χ����
		long lElementBoundSize = _GetLongFromByteArray(pStream, lOffset);
		for (long i = 0; i < lElementBoundSize; ++i)
		{
			tagElementBound ElementBound;
			long lStuffSize = _GetLongFromByteArray(pStream, lOffset);
			if(0 < lStuffSize)
			{
				ElementBound.vElementStuff.resize(lStuffSize);
				_GetBufferFromByteArray(pStream, lOffset, &ElementBound.vElementStuff[0], sizeof(tagElementStuff) * lStuffSize);
			}
			long lNeedNumSize = _GetLongFromByteArray(pStream, lOffset);
			if(0 < lNeedNumSize)
			{
				ElementBound.vNeedNum.resize(lNeedNumSize);
				_GetBufferFromByteArray(pStream, lOffset, &ElementBound.vNeedNum[0], sizeof(tagNeedNum) * lNeedNumSize);
			}

			m_ElementSetup.vElementBound.push_back(ElementBound);
		}

		//! ħ����������
		long lEquipmentLevelBoundSize = _GetLongFromByteArray(pStream, lOffset);
		if(0 < lEquipmentLevelBoundSize)
		{
			m_ElementSetup.vEquipmentLevelBound.resize(lEquipmentLevelBoundSize);
			_GetBufferFromByteArray(pStream, lOffset, &m_ElementSetup.vEquipmentLevelBound[0], sizeof(tagEquipmentLevelBound) * lEquipmentLevelBoundSize);
		}

		//! ���ʸ�����������
		long lHelpfulSetupSize = _GetLongFromByteArray(pStream, lOffset);
		for (long i = 0; i < lHelpfulSetupSize; ++i)
		{
			tagHelpfulSetup HelpfulSetup;
			HelpfulSetup.lMinLevel = _GetLongFromByteArray(pStream, lOffset);
			HelpfulSetup.lMaxLevel = _GetLongFromByteArray(pStream, lOffset);
			long lStuffSize = _GetLongFromByteArray(pStream, lOffset);
			if(0 < lStuffSize)
			{
				HelpfulSetup.vHelpfulStuff.resize(lStuffSize);
				_GetBufferFromByteArray(pStream, lOffset, &HelpfulSetup.vHelpfulStuff[0], sizeof(tagHelpfulStuff) * lStuffSize);
			}
			m_ElementSetup.vHelpfulSetup.push_back(HelpfulSetup);
		}

		//! ħ���ȼ���Ӧħ������ֵ
		long lElementValueSize = _GetLongFromByteArray(pStream, lOffset);
		if(0 < lElementValueSize)
		{
			m_ElementSetup.vElementValue.resize(lElementValueSize);
			_GetBufferFromByteArray(pStream, lOffset, &m_ElementSetup.vElementValue[0], sizeof(tagElementValue) * lElementValueSize);
		}

		//! Ԫ����������
		long lResetElementSize = _GetLongFromByteArray(pStream, lOffset);
		for (long i = 0; i < lResetElementSize; ++i)
		{
			tagResetElement ResetElement;
			ResetElement.lMinLevel = _GetLongFromByteArray(pStream, lOffset);
			ResetElement.lMaxLevel = _GetLongFromByteArray(pStream, lOffset);
			long lStuffSize = _GetLongFromByteArray(pStream, lOffset);
			if(0 < lStuffSize)
			{
				ResetElement.vResetStuff.resize(lStuffSize);
				_GetBufferFromByteArray(pStream, lOffset, &ResetElement.vResetStuff[0], sizeof(tagResetStuff) * lStuffSize);
			}
			m_ElementSetup.vResetElement.push_back(ResetElement);
		}

		//! ������������
		long lResetXiangXingSize = _GetLongFromByteArray(pStream, lOffset);
		for (long i = 0; i < lResetXiangXingSize; ++i)
		{
			tagResetXiangXing ResetXiangXing;
			ResetXiangXing.lMinLevel = _GetLongFromByteArray(pStream, lOffset);
			ResetXiangXing.lMaxLevel = _GetLongFromByteArray(pStream, lOffset);
			_GetBufferFromByteArray(pStream, lOffset, &ResetXiangXing.XiangXingOdds, sizeof(tagXiangXingOdds));
			long lStuffSize = _GetLongFromByteArray(pStream, lOffset);
			if(0 < lStuffSize)
			{
				ResetXiangXing.vResetStuff.resize(lStuffSize);
				_GetBufferFromByteArray(pStream, lOffset, &ResetXiangXing.vResetStuff[0], sizeof(tagResetStuff) * lStuffSize);
			}
			m_ElementSetup.vResetXiangXing.push_back(ResetXiangXing);
		}
	}
	
	return true;
}