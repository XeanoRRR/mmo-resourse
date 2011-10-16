
/*
��Ʒ�ֽⷽ�����塣

2007-12-11 �ſ���
*/

#include "stdafx.h"
#include "GoodsExManage.h"

#include "Player.h"
#include "Script/Script.h"

#include "Goods\CGoods.h"
#include "goods/CGoodsFactory.h"
#include "goods/CGoodsBaseProperties.h"
#include "container\CTestContainer.h"

#include "..\..\..\nets\netserver\message.h"
#include "..\..\..\public\GUID.h"


//! ��Ӧ��Ʒ�ֽ�����
//! 
/*	�ͻ�����Ϣ�ṹ
long ���ֽ������GUID
long ���ֽ���λ��ö�٣�CS2CContainerObjectMove::PLAYER_EXTEND_ID��
long ��Ʒ���ڰ���λ��
*/
void GoodsExManage::OnGoodsDisassemble(CMessage *pMsg)
{
	CMessage reMsg(MSG_S2C_GOODS_RE_DISASSEMBLE_DO);

	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_DisassembleWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		CGUID goodsGuid;
		pMsg->GetGUID(goodsGuid);

		long lContianerId	= pMsg->GetLong(); //����ID
		long lPos			= pMsg->GetLong(); //λ��

		//! ȡ�ñ��ֽ���Ʒ������֤�Ϸ���
		CGoods *pGoods = TestGoodsHere(pPlayer, goodsGuid, lContianerId, lPos);
		if(NULL == pGoods)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		long lGoodsTypeID = pGoods->GetBasePropertiesIndex();
		tagReValue arr_result[MAX_DISASSEMBLE_NUM];
		//! �����Ʒ���ֽܷ�
		long lOutNum = GetGoodsDisassembleResult(lGoodsTypeID, arr_result);
		if (0 == lOutNum || IsFrost(pGoods))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_CantDisassemble);
			break;
		}

		//! �õ������б�
		vector<CGoods*> vAddGoods;
		for (long i = 0; i < MAX_DISASSEMBLE_NUM; ++i)
		{
			if (0 < arr_result[i].lGoodsNum)
			{
				vector<CGoods*> vTmpAddGoods;
				CGoodsFactory::CreateGoods( arr_result[i].lGoodsIndex, arr_result[i].lGoodsNum, vTmpAddGoods,42);
				for (long j = 0; j < vTmpAddGoods.size(); ++j)
				{
					vAddGoods.push_back(vTmpAddGoods[j]);
				}
			}
		}		

		//! �õ�ɾ���б�
		vector<tagGoodsOperate> vDelGoods;
		tagGoodsOperate GoodsOperate;
		GoodsOperate.lContianerPos	= lContianerId;
		GoodsOperate.lGoodsPos		= lPos;
		GoodsOperate.lOperateNum	= 1;
		vDelGoods.push_back(GoodsOperate);

		//! ��������ܷ�װ�²���
		long lSpareGrilleNum = TestPutWay(pPlayer, vDelGoods, vAddGoods);
		if(0 > lSpareGrilleNum)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_NoEnoughSpace);
			reMsg.Add(lSpareGrilleNum);

			for (LONG i = 0; i < vAddGoods.size(); ++i)
			{
#ifdef _GOODSLOG1_
				if(vAddGoods[i])
				{
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					vAddGoods[i]->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[��Ʒ����:exmanage206][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						vAddGoods[i]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(vAddGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vAddGoods[i]->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
				}			
#endif	
				CGoodsFactory::GarbageCollect(&vAddGoods[i], 520);
			}
			break;
		}

		//! ���ɱ��β�����ID
		CGUID optGuid;
		CGUID::CreateGUID(optGuid);

		//! ɾ���ֽ���
		if(!DelGoods(pPlayer, vDelGoods, 380, optGuid))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SrcIsLock);

			for (LONG i = 0; i < vAddGoods.size(); ++i)
			{
#ifdef _GOODSLOG1_
				if(vAddGoods[i])
				{
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					vAddGoods[i]->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[��Ʒ����:exmanage234][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						vAddGoods[i]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(vAddGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vAddGoods[i]->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
				}			
#endif	
				CGoodsFactory::GarbageCollect(&vAddGoods[i], 520);
			}
			break;
		}

		//! �����Ʒ
		vector<long> vOptTye(vAddGoods.size());
		vector<long> vOptNum(vAddGoods.size());
		AddGoods(pPlayer, vAddGoods, vOptTye, vOptNum);


		//! �����Ʒ��־
		for(LONG i = 0; i < vAddGoods.size(); ++i)
		{
			GetGameLogInterface()->logT380_goods_disassemble_log(pPlayer, vAddGoods[i], vOptNum[i], optGuid, TRUE);
		}			

		reMsg.Add((long)eGTC_Succeed);

	} while(false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! ��Ӧ��Ʒ�ϳ�����
//! 
/*	�ͻ�����Ϣ�ṹ
long				�ϳ��䷽����
tagSynthesizeGoods	�����б�
*/
void GoodsExManage::OnGoodsSynthesize(CMessage *pMsg)
{
	CMessage reMsg(MSG_S2C_GOODS_RE_SYNTHESIZE_DO);
	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_PrescriptionWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		//! ��÷������䷽
		long lPrescriptionIndex = pMsg->GetLong();
		const GoodsExSetup::tagSynthesizePrescription *pSynthesizePrescription 
			= m_GoodsExSetup.GetSynthesizePrescription(lPrescriptionIndex);
		if (NULL == pSynthesizePrescription)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_NoPrescription);
			break;
		}

		//! ɾ���б�
		vector<tagGoodsOperate> vDelGoods;

		//! ��֤�ͻ��˷�������Ʒ
		tagSynthesizeGoods SynthesizeGoods;
		pMsg -> GetEx(&SynthesizeGoods, sizeof(tagSynthesizeGoods));

		bool pass = true;
		for (long i = 0; i < MAX_STUFF_NUM; ++i)
		{
			if (0 == SynthesizeGoods.arrContainerFlag[i]) break;

			CGoods *pGoods = TestGoodsHere(	pPlayer, 
				SynthesizeGoods.arrGoodsGuid[i], 
				SynthesizeGoods.arrContainerFlag[i],
				SynthesizeGoods.arrPos[i]);

			if(NULL == pGoods)
			{
				pass = false;
				break;
			}
			for (long j = i + 1; j < MAX_STUFF_NUM; ++j)
			{
				if(SynthesizeGoods.arrContainerFlag[i] == SynthesizeGoods.arrContainerFlag[j] && 
					SynthesizeGoods.arrPos[i] == SynthesizeGoods.arrPos[j])
				{
					reMsg.Add((long)eGTC_Failed);
					reMsg.Add((long)eGTC_ErrSP);
					reMsg.SendToPlayer(pMsg->GetPlayerID());
					return;
				}
			}

			tagGoodsOperate GoodsOperate;
			GoodsOperate.lContianerPos	= SynthesizeGoods.arrContainerFlag[i];
			GoodsOperate.lGoodsPos		= SynthesizeGoods.arrPos[i];
			if(SynthesizeGoods.arrNum[i] > pGoods->GetAmount())
			{
				reMsg.Add((long)eGTC_Failed);
				reMsg.Add((long)eGTC_ErrSP);
				reMsg.SendToPlayer(pMsg->GetPlayerID());
				return;
			}
			GoodsOperate.lOperateNum	= SynthesizeGoods.arrNum[i];
			GoodsOperate.lReserved		= pGoods->GetBasePropertiesIndex();
			GoodsOperate.pGoods			= pGoods;
			vDelGoods.push_back(GoodsOperate);
		}
		if (!pass)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		//! ��֤�䷽����ȡ����������
		LONG lGoodsLevel = 0;
		LONG lGoodsEvilLevel = 0;
		long arrPass[MAX_STUFF_NUM] = {0};
		for (long i = 0; i < MAX_STUFF_NUM; ++i)
		{
			if (0 == pSynthesizePrescription->arr_lStuffId[i])
			{
				break;
			}

			long lNeedNum = pSynthesizePrescription->arr_lStuffNum[i];
			for (long j = 0; j < vDelGoods.size(); ++j)
			{
				if (vDelGoods[j].lReserved == pSynthesizePrescription->arr_lStuffId[i])
				{
					if (0 >= lNeedNum)
					{
						//! �䷽����
						reMsg.Add((long)eGTC_Failed);
						reMsg.Add((long)eGTC_ErrSP);
						reMsg.SendToPlayer(pMsg->GetPlayerID());
						return;
					}
					else if(vDelGoods[j].lOperateNum > lNeedNum)
					{
						vDelGoods[j].lOperateNum = lNeedNum;
						lNeedNum = 0;
						vDelGoods[j].lOperateWay = CS2CContainerObjectMove::OT_CHANGE_AMOUNT;
					}
					else
					{
						lNeedNum -= vDelGoods[j].lOperateNum;
						vDelGoods[j].lOperateWay = CS2CContainerObjectMove::OT_DELETE_OBJECT;
					}

					if(pSynthesizePrescription->bHasPrimary && 0 == i)
					{
						lGoodsLevel = vDelGoods[j].pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL, 1);
						lGoodsEvilLevel = vDelGoods[j].pGoods->GetAddonPropertyValues(GAP_PLUG_EVIL_LEVEL, 1);
					}
				}
			}

			if(0 != lNeedNum)
			{
				//! �䷽����
				reMsg.Add((long)eGTC_Failed);
				reMsg.Add((long)eGTC_ErrSP);
				reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
				return;
			}
		}

		//! �õ������б�
		vector<CGoods*> vAddGoods;
		CGoodsFactory::CreateGoods( 
			pSynthesizePrescription->lOutGoodsId, 
			pSynthesizePrescription->lOutGoodsNum, vAddGoods,44);

		//! ��������ܷ�װ�²���
		long lSpareGrilleNum = TestPutWay(pPlayer, vDelGoods, vAddGoods);
		if(0 > lSpareGrilleNum)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_NoEnoughSpace);
			reMsg.Add(lSpareGrilleNum);

			for (LONG i = 0; i < vAddGoods.size(); ++i)
			{
#ifdef _GOODSLOG1_
				if(vAddGoods[i])
				{
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					vAddGoods[i]->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[��Ʒ����:exmanage424][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						vAddGoods[i]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(vAddGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vAddGoods[i]->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
				}			
#endif	
				CGoodsFactory::GarbageCollect(&vAddGoods[i], 520);
			}
			break;
		}

		//! ���ɱ��β���ID
		CGUID optGuid;
		CGUID::CreateGUID(optGuid);

		//! ɾ���ֽ���
		if(!DelGoods(pPlayer, vDelGoods, 390, optGuid))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SrcIsLock);

			for (LONG i = 0; i < vAddGoods.size(); ++i)
			{
#ifdef _GOODSLOG1_
				if(vAddGoods[i])
				{
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					vAddGoods[i]->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[��Ʒ����:exmanage452][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						vAddGoods[i]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(vAddGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vAddGoods[i]->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
				}			
#endif	
				CGoodsFactory::GarbageCollect(&vAddGoods[i], 520);
			}
			break;
		}

		//! �̳�����
		if(pSynthesizePrescription->bHasPrimary)
		{
			CGoodsFactory::SetEquipmentLevel(vAddGoods[0], lGoodsLevel);
			CGoodsFactory::UpdateEquipmentAttr(vAddGoods[0]);
		}

		//! �����Ʒ
		vector<long> vOptTye(vAddGoods.size());
		vector<long> vOptNum(vAddGoods.size());
		AddGoods(pPlayer, vAddGoods, vOptTye, vOptNum);


		//! ��¼��־
		for (long i = 0; i < vAddGoods.size(); ++i)
		{
			GetGameLogInterface()->logT390_goods_synthesize_log(pPlayer, vAddGoods[i], vOptTye[i], optGuid, TRUE);
		}

		reMsg.Add((long)eGTC_Succeed);
		reMsg.Add((long)vAddGoods.size());
		for (LONG i = 0; i < vAddGoods.size(); ++i)
		{
			reMsg.Add(vOptNum[i]);
			reMsg.Add(vAddGoods[i]->GetExID());
		}


	} while(false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}



//!**************************************************************************
//! ��������
//!
//!
//!**************************************************************************


//! ����������Ʒ���õ����
//! ��Ʒ�Ϸ��ԡ�ʹ�ú����Ʒɾ���������ⲿִ��
bool GoodsExManage::ReinforcePower(CGoods *pArm, CGoods *pGoods, CPlayer *pPlayer)
{
	if (NULL == pGoods || NULL == pPlayer || NULL == pArm) return false;
	if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return false;

	const CGUID& PlayerGUID = pPlayer->GetExID();
	CMessage errMsg(MSG_S2C_GOODS_RE_REINFORCE_POWER);

	if (NULL == pArm || NULL == pGoods) 
	{
		errMsg.Add((long)eGTC_Failed);
		errMsg.Add((long)eGTC_NoArmInHand);
		errMsg.SendToPlayer(PlayerGUID, false);
		return false;
	}

	//! ������������			
	long lMaxPower_A = pArm->GetAddonPropertyValues(GAP_MAX_POWER, 1); 
	if (0 == lMaxPower_A) 
	{
		errMsg.Add((long)eGTC_Failed);
		errMsg.Add((long)eGTC_NoReinforceArm);
		errMsg.SendToPlayer(PlayerGUID, false);
		return false;
	}

	//! ��Ʒ����ǿ��
	long lPowerIntensity_G = pGoods->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1);
	if (0 == lPowerIntensity_G) 
	{
		//! ���߼����ϵĴ���Ӧ��¼������־
		return false;
	}

	//! ��Ʒ���ܵȼ�����
	long lMinLevel_G = pGoods->GetAddonPropertyValues(GAP_MIN_REINFORCE_LEVEL, 1);
	long lMaxLevel_G = pGoods->GetAddonPropertyValues(GAP_MAX_REINFORCE_LEVEL, 1);

	//! �����ȼ�
	long lLevel_A = pArm->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);
	if (lMinLevel_G > lLevel_A || lMaxLevel_G < lLevel_A)
	{
		errMsg.Add((long)eGTC_Failed);
		errMsg.Add((long)eGTC_LevelError);
		errMsg.SendToPlayer(PlayerGUID, false);
		return false;
	}

	//! ��������ǿ��
	long lPowerIntensity_A = pArm->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1);

	//! ������ǰ����ֵ
	long lCurrPower_A = pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1);


	//! �ж��Ƿ���Ҫ����
	long lMaxPercent = m_GoodsExSetup.GetReinforceMaxPercent();
	if (lPowerIntensity_A >= lPowerIntensity_G && lCurrPower_A * 100 >= lMaxPower_A * lMaxPercent)
	{
		errMsg.Add((long)eGTC_Failed);
		errMsg.Add((long)eGTC_ArmIsFull);
		errMsg.SendToPlayer(PlayerGUID, false);
		return false;
	}

	pArm->ChangeAttribute(GAP_POWER_INTENSITY, lPowerIntensity_G, 1, true);
	pArm->ChangeAttribute(GAP_POWER_INTENSITY, pGoods->GetBasePropertiesIndex(), 2, true);

	pArm->ChangeAttribute(GAP_CURR_POWER, lMaxPower_A, 1, true);
	pArm->UpdateAttribute(PlayerGUID);

	errMsg.Add((long)eGTC_Succeed);
	errMsg.Add(PlayerGUID);
	errMsg.Add(lPowerIntensity_G);
	errMsg.SendToAround(pPlayer);

	return true;
}

//! ��ʼ��ҵ������ӳ�Ч��
bool GoodsExManage::ArmReinforceBegin(CPlayer *pPlayer, CGoods *pArm, bool isMagic, bool isAttack, pair<float, float>& ReturnReinforce)
{
	if (NULL == pArm || NULL == pPlayer) return false;

	//! ��������ǿ��
	long lPowerIntensity = pArm->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1);

	//! ������ǰ����ֵ
	long lCurrPower = pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1);

	//! �����ȼ�
	long lLevel = pArm->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);

	//! ����
	const GoodsExSetup::tagPowerAccessorial* pPowerAccessorial = m_GoodsExSetup.GetPowerAccessorial(lLevel, lPowerIntensity);

	if (NULL != pPowerAccessorial) 
	{
		long lUsePower = 0;
		if (isMagic)
			lUsePower = pPowerAccessorial->lMExpend;
		else
			lUsePower = pPowerAccessorial->lPExpend;
		if (lCurrPower > 0)
		{
			//! ����ʣ�����
			long lLostPower = lCurrPower - lUsePower;
			lLostPower = (lLostPower > 0) ? lLostPower : 0;

			pArm->ChangeAttribute(GAP_CURR_POWER, lLostPower, 1, true);

			CMessage msg(MSG_S2C_GOODS_POWER_WARN);
			msg.Add(pPlayer->GetExID());
			msg.Add(lPowerIntensity);
			msg.Add(lLostPower);
			msg.SendToAround(pPlayer);


			if (isMagic)
			{
				ReturnReinforce.first = pPowerAccessorial->AmendCoefficient.fAmendIT;
				if(isAttack)
					ReturnReinforce.second = pPowerAccessorial->AmendCoefficient.fAmendMA;
				else
					ReturnReinforce.second = pPowerAccessorial->AmendCoefficient.fAmendMC;
			}
			else
			{
				ReturnReinforce.first = 1.0f;
				if(isAttack)
					ReturnReinforce.second = pPowerAccessorial->AmendCoefficient.fAmendPA;
				else
					ReturnReinforce.second = pPowerAccessorial->AmendCoefficient.fAmendPC;
			}
			return true;
		}
	}

	ReturnReinforce.first	= 1.0f;
	ReturnReinforce.second	= 1.0f;
	return false;
}

//!				��������������õ�һ�����õļӳ�ֵ
float GoodsExManage::GetReinforceByEnum(long lAimLevel, long lPowerIntensity, long eValuePos)
{
	const GoodsExSetup::tagPowerAccessorial* pPowerAccessorial = m_GoodsExSetup.GetPowerAccessorial(lAimLevel, lPowerIntensity);
	if (NULL != pPowerAccessorial)
	{
		switch(eValuePos)
		{
		case GoodsExSetup::eACT_AmendIT:
			return pPowerAccessorial->AmendCoefficient.fAmendIT;
		case GoodsExSetup::eACT_AmendMA:
			return pPowerAccessorial->AmendCoefficient.fAmendMA;
		case GoodsExSetup::eACT_AmendPA:
			return pPowerAccessorial->AmendCoefficient.fAmendPA;
		case GoodsExSetup::eACT_AmendMC:
			return pPowerAccessorial->AmendCoefficient.fAmendMC;
		case GoodsExSetup::eACT_AmendPC:
			return pPowerAccessorial->AmendCoefficient.fAmendPC;
		}
	}
	return 1.0f;
}

//!				�ж��Ƿ񻹿��Լ�����
long GoodsExManage::CanExpendPower(CGoods *pArm, BOOL isMagic)
{
	//! ��������ǿ��
	long lPowerIntensity = pArm->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1);

	//! ������ǰ����ֵ
	long lCurrPower = pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1);

	//! �����ȼ�
	long lLevel = pArm->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);

	//! ����
	const GoodsExSetup::tagPowerAccessorial* pPowerAccessorial = m_GoodsExSetup.GetPowerAccessorial(lLevel, lPowerIntensity);

	long lUsePower = 0;
	if (NULL != pPowerAccessorial) 
	{
		if (isMagic)
			lUsePower = pPowerAccessorial->lMExpend;
		else
			lUsePower = pPowerAccessorial->lPExpend;
	}

	return lCurrPower >= lUsePower;
}

//!				�۳�һ�����ĵ�����������
long GoodsExManage::GetExpendPower(CGoods *pArm, BOOL isMagic)
{
	//! ��������ǿ��
	long lPowerIntensity = pArm->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1);

	//! ������ǰ����ֵ
	long lCurrPower = pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1);

	//! �����ȼ�
	long lLevel = pArm->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);

	//! ����
	const GoodsExSetup::tagPowerAccessorial* pPowerAccessorial = m_GoodsExSetup.GetPowerAccessorial(lLevel, lPowerIntensity);

	long lUsePower = 0;
	if (NULL != pPowerAccessorial) 
	{
		if (isMagic)
			lUsePower = pPowerAccessorial->lMExpend;
		else
			lUsePower = pPowerAccessorial->lPExpend;
	}

	if (lCurrPower < lUsePower)
		lUsePower = lCurrPower;

	return lUsePower;
}


//!**************************************************************************
//! ��Ʒ�󶨡�����
//!
//!
//!**************************************************************************

//! ������Ʒ�İ�����
long GoodsExManage::SetGoodsBindProperty_Player(CGoods *pGoods, long lBindType, const CGUID& playerGuid)
{
	if(NULL == pGoods) return 1;
	if(!pGoods->HasAddonProperty(GAP_BIND)) return 10;

	//! ֻ�С�δ���ᡱ����Ʒ���ܽ��С��󶨡���ز���
	if (eBT_Untie == lBindType || eBT_Bind == lBindType)
	{
		if (lBindType != pGoods->GetAddonPropertyValues(GAP_BIND, 1))
			pGoods->ChangeAttribute(GAP_BIND, lBindType, 1, true);
		else
			return 30;//! ����ֵ�뵱ǰֵ��ͬ
	}
	else
		return 40;

	pGoods->UpdateAttribute(playerGuid);
	return 0;
}


//! Ϊ�ű��ṩ�İ󶨺���
//������1����Ʒӵ����
//! ����2��3����Ʒ���ڰ���λ�ã�ֻ������������5���ӱ�����
//! ����4��0=��󶨡�1=�󶨡�����ֵ�Ƿ�
//! ����ֵ��0=ʧ�ܣ�1=�ɹ���
long GoodsExManage::SetGoodsBindProperty_Script(CPlayer *pPlayer, long lContianerID, long lPos, long lBindType)
{
	if (NULL == pPlayer || (eBT_Untie != lBindType && eBT_Bind != lBindType)) return 0;

	CGoods *pGoods = FindGoodsFromPlayer(pPlayer, lContianerID, lPos);
	if (NULL == pGoods) return 0;

	if(0 == SetGoodsBindProperty_Player(pGoods, lBindType, pPlayer->GetExID())) return 1;

	return 0;
}


//! ϵͳ������Ʒ�İ�����
long GoodsExManage::SetGoodsBindProperty_System(CGoods *pGoods, long lBindType, const CGUID& playerGuid)
{
	if(NULL == pGoods || NULL_GUID == playerGuid) return 1;
	if(!pGoods->HasAddonProperty(GAP_BIND)) return 10;

	pGoods->ChangeAttribute(GAP_BIND, lBindType, 1, true);
	pGoods->UpdateAttribute(playerGuid);

	return 0;
}




//! ���������Ʒ�Ķ�������
long GoodsExManage::SetGoodsFrostProperty_Player(CGoods *pGoods, long lFrostType, const CGUID& playerGuid)
{
	if(NULL == pGoods || NULL_GUID == playerGuid) return 1;
	if(!pGoods->HasAddonProperty(GAP_BIND)) return 10;

	//! ���ֻ��������ͽⶳ����
	if (eFT_UnFrost == lFrostType || eFT_Frost == lFrostType)
	{
		long lOldFrostType = pGoods->GetAddonPropertyValues(GAP_FREEZE, 1);
		if(lOldFrostType == lFrostType) return 30; //! ����ֵ�뵱ǰֵ��ͬ

		if(eFT_CantThaw == lFrostType)
		{
			return 70;
		}
		//! ����Ƕ��ᣬ��ֱ�Ӷ�����Ʒ
		else if (eFT_Frost == lFrostType)
		{
			pGoods->ChangeAttribute(GAP_FREEZE, eFT_Frost, 1, true);
			pGoods->ChangeAttribute(GAP_THAWTIME, 0, 1, true);
		}
		else 
		{
			//! �������ʱ��
			long lNow = 0;
			time(&lNow);

			//! ������ڽⶳ
			if (eFT_Thawing == lOldFrostType)
			{
				//! �쿴�ⶳ����ʱ��
				long lThawTime = pGoods->GetAddonPropertyValues(GAP_THAWTIME, 1);
				if (lNow > lThawTime) //! �ⶳʱ���ѵ�
				{
					pGoods->ChangeAttribute(GAP_FREEZE, eFT_UnFrost, 1, true);
					pGoods->ChangeAttribute(GAP_THAWTIME, 0, 1, true);
				}
				else
				{
					return 40;//! �ⶳʱ��δ��
				}
			}
			else
			{
				//! ��7������������Ϊ�ⶳ����ʱ��
				pGoods->ChangeAttribute(GAP_FREEZE, eFT_Thawing, 1,true);
				pGoods->ChangeAttribute(GAP_THAWTIME, lNow + 7 * 24 * 60 * 60, 1, true);
			}
		}
	}
	else
	{
		return 60;
	}

	pGoods->UpdateAttribute(playerGuid);
	return 0;
}

//! Ϊ�ű��ṩ�Ķ��ắ��
//������1����Ʒӵ����
//! ����2��3����Ʒ���ڰ���λ�ã�ֻ������������5���ӱ�����
//! ����4��0=�ⶳ��1=���ᡣ����ֵ�Ƿ�
//! ����ֵ��0=ʧ�ܣ�1=�ɹ���
long GoodsExManage::SetGoodsFrostProperty_Script(CPlayer *pPlayer, long lContianerID, long lPos, long lBindType)
{
	if (NULL == pPlayer || (eFT_UnFrost != lBindType && eFT_Frost != lBindType)) return 0;

	CGoods *pGoods = FindGoodsFromPlayer(pPlayer, lContianerID, lPos);
	if (NULL == pGoods) return 0;

	if(0 == SetGoodsFrostProperty_Player(pGoods, lBindType, pPlayer->GetExID())) return 1;

	return 0;
}

//!		�����Ʒ�Ƿ񶳽�
//!		�����Ʒ�ⶳʱ���ѵ��������ⶳ���Ե�����
bool	GoodsExManage::IsFrost(CGoods *pGoods)
{
	long lFrostType = pGoods->GetAddonPropertyValues(GAP_FREEZE, 1);
	if(eFT_Frost == lFrostType || eFT_CantThaw == lFrostType)
	{
		return true;
	}
	else if (eFT_Thawing == lFrostType)
	{
		//! �쿴�ⶳ����ʱ��
		long lThawTime = pGoods->GetAddonPropertyValues(GAP_THAWTIME, 1);
		if (time(NULL) > lThawTime) //! �ⶳʱ���ѵ�
		{
			//! ������������ݵ�����ͻ��˷��ͣ��ͻ�����ʱ�����ʾ�������Ѿ������ֽţ���Ҳ���о�����Ʒ���Եĸ��±��ӳ���
			pGoods->ChangeAttribute(GAP_FREEZE, eFT_UnFrost, 1, true);
			pGoods->ChangeAttribute(GAP_THAWTIME, 0, 1, true);
		}
		else
			return true;
	}

	return false;
}

//!		�����Ʒ�Ƿ��
bool	GoodsExManage::IsBind(CGoods *pGoods)
{
	long lBindType = pGoods->GetAddonPropertyValues(GAP_BIND, 1);
	if(eBT_Bind == lBindType || eBT_CantUntie == lBindType)
		return true;
	return false;
}

//! ��Ӧ��Ʒ��
void GoodsExManage::OnGoodsBind(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL == pPlayer) return;
	if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;
	BindAndFrost(true, pMsg);
}

//! ��Ӧ��Ʒ����
void GoodsExManage::OnGoodsFrost(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL == pPlayer) return;
	if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;
	BindAndFrost(false, pMsg);
}

//! ��Ӧװ������Ԥ��
void GoodsExManage::OnEquipmentUpgradeView(CMessage *pMsg)
{
	CMessage reMsg(MSG_S2C_GOODS_RE_UPGRADE_VIEW);
	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_UpgradeWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		//! ȡ�����
		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		//! ȡ�ò���Ŀ�꣬��֤Ŀ���Ƿ����
		CGUID goodsGuid;
		pMsg->GetGUID(goodsGuid);

		long lContianerId	= pMsg->GetLong(); //����ID
		long lPos			= pMsg->GetLong(); //λ��

		CGoods *pGoods = TestGoodsHere(pPlayer, goodsGuid, lContianerId, lPos);
		if(NULL == pGoods)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		//! ��֤Ŀ���Ƿ��������
		long lAimCurrLevel = pGoods->CanUpgrade();
		long UpgradeMaxLevel = pGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL_MAX, 1);// ��ø���Ʒ���������ȼ������û�����ã�Ĭ��Ϊ20��
		if(UpgradeMaxLevel==0)
		{
			UpgradeMaxLevel=20;
		}
		if (-1 == lAimCurrLevel || UpgradeMaxLevel <= lAimCurrLevel)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_LevelTiptop);
			break;
		}
		//! ��ȡ�䷽
		long lLimitLevel = pGoods->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);
		const GoodsExSetup::tagUpgradeSet* pUpgradeSet = m_GoodsExSetup.GetUpgradeSet(lLimitLevel, lAimCurrLevel);
		if (NULL == pUpgradeSet)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_LevelTiptop);
			break;
		}

		//! ��¡һ����Ʒ�Ա������޸�
		CGoods *pCloneGoods = CGoodsFactory::CreateGoods(pGoods->GetBasePropertiesIndex(),45);
		if(!pGoods->Clone((CBaseObject*)pCloneGoods))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_UnknownErr);

			//! ������ʱ��Ʒ
#ifdef _GOODSLOG1_
			if(pCloneGoods)
			{
				char pszGoodsLog[1024]="";			
				char srcGoodsGUID[50]="";
				pCloneGoods->GetExID().tostring(srcGoodsGUID);					
				_snprintf(pszGoodsLog,1024,"[��Ʒ����:GoodsExManage::OnEquipmentUpgradeView][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
					pCloneGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pCloneGoods->GetBasePropertiesIndex()),srcGoodsGUID,pCloneGoods->GetAmount());
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
			}			
#endif
			CGoodsFactory::GarbageCollect(&pCloneGoods,32);
			break;
		}

		//! ������¡����Ʒ�������뵽��Ϣ��
		if(!CGoodsFactory::ChangeEquipmentLevel(pCloneGoods, 1))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_UnknownErr);

			//! ������ʱ��Ʒ
#ifdef _GOODSLOG1_
			if(pCloneGoods)
			{
				char pszGoodsLog[1024]="";			
				char srcGoodsGUID[50]="";
				pCloneGoods->GetExID().tostring(srcGoodsGUID);					
				_snprintf(pszGoodsLog,1024,"[��Ʒ����:GoodsExManage::OnEquipmentUpgradeView2][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
					pCloneGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pCloneGoods->GetBasePropertiesIndex()),srcGoodsGUID,pCloneGoods->GetAmount());
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
			}			
#endif
			CGoodsFactory::GarbageCollect(&pCloneGoods,33);
			break;
		}

		vector<BYTE> vMsgData;
		pCloneGoods->SerializeForOldClient(&vMsgData);

		reMsg.Add((long)eGTC_Succeed);
		reMsg.Add((long)vMsgData.size());
		reMsg.AddEx(&(vMsgData[0]), (long)vMsgData.size());

		//! ������ʱ��Ʒ
#ifdef _GOODSLOG1_
		if(pCloneGoods)
		{
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			pCloneGoods->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:GoodsExManage::OnEquipmentUpgradeView3][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				pCloneGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pCloneGoods->GetBasePropertiesIndex()),srcGoodsGUID,pCloneGoods->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
		}		
#endif
		CGoodsFactory::GarbageCollect(&pCloneGoods,34);

	}while (false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! ��Ӧװ������
void GoodsExManage::OnEquipmentUpgrade(CMessage *pMsg)
{
	CMessage reMsg(MSG_S2C_GOODS_RE_UPGRADE_DO);
	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_UpgradeWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		//! ȡ�����
		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		//! ȡ�����в����������ɾ�����б�
		tagUpgradeStuff UpgradeStuff;
		pMsg->GetEx(&UpgradeStuff, sizeof(tagUpgradeStuff));

		vector<tagGoodsOperate> vDelGoods;

		//! ������в�����
		set<CGUID> setGoodsGuid;
		CGoods *arr_pGoods[IN_TAGUPGRADESTUFF_GOODS_NUM] = {0};
		for (long i = 0; i < IN_TAGUPGRADESTUFF_GOODS_NUM; ++i)
		{
			arr_pGoods[i] = TestGoodsHere(pPlayer, UpgradeStuff.arrGoodsGuid[i], UpgradeStuff.arrContainerFlag[i], UpgradeStuff.arrPos[i]);
			if (0 < i && NULL != arr_pGoods[i])
			{
				//! ��֤�ظ���Ʒ����
				if(setGoodsGuid.end() != setGoodsGuid.find(arr_pGoods[i]->GetExID()))
					return;
				else
					setGoodsGuid.insert(arr_pGoods[i]->GetExID());

				for (long j = i + 1; j < IN_TAGUPGRADESTUFF_GOODS_NUM; ++j)
				{
					if (arr_pGoods[i] == arr_pGoods[j])
					{
						return;
					}
				}
				tagGoodsOperate GoodsOperate;
				GoodsOperate.lContianerPos	= UpgradeStuff.arrContainerFlag[i];
				GoodsOperate.lGoodsPos		= UpgradeStuff.arrPos[i];
				if(1 < arr_pGoods[i]->GetAmount())
					GoodsOperate.lOperateWay = CS2CContainerObjectMove::OT_CHANGE_AMOUNT;
				else if(1 != arr_pGoods[i]->GetAmount())
				{
					reMsg.Add((long)eGTC_Failed);
					reMsg.Add((long)eGTC_NumError);
					break;
				}
				GoodsOperate.lOperateNum	= 1;
				vDelGoods.push_back(GoodsOperate);
			}
		}

		//! ��֤����Ŀ��������Ƿ����
		if(NULL == arr_pGoods[0] || NULL == arr_pGoods[1])
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		//! ��֤���ϵȼ��Ƿ�ƥ��
		bool bError = false;
		long lLimitLevel = arr_pGoods[0]->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);
		for (long i = 1; i < IN_TAGUPGRADESTUFF_GOODS_NUM; ++i)
		{
			if(NULL != arr_pGoods[i])
			{
				vector<long> vOutLimit;
				arr_pGoods[i]->GetAddonPropertyValues(GAP_UPGRADE_LIMIT, vOutLimit);
				if (2 != vOutLimit.size())
					continue;
				if(vOutLimit[0] > lLimitLevel || vOutLimit[1] < lLimitLevel)
				{
					bError = true;
					break;
				}
			}
		}
		if (bError)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_LimitTooLow);
			break;
		}

		//! ��֤Ŀ���Ƿ��������
		long lAimCurrLevel = arr_pGoods[0]->CanUpgrade();
		long lNewLevel = lAimCurrLevel;
		//���ҵ�ǰװ���Ƿ�������ǿ������
		long UpgradeMaxLevel = arr_pGoods[0]->GetAddonPropertyValues(GAP_WEAPON_LEVEL_MAX, 1);// ��ø���Ʒ���������ȼ������û�����ã�Ĭ��Ϊ20��
		if(UpgradeMaxLevel==0)
		{
			UpgradeMaxLevel=20;
		}
		if (-1 == lAimCurrLevel || UpgradeMaxLevel <= lAimCurrLevel)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_LevelTiptop);
			break;
		}

		//! ��ȡ�䷽
		const GoodsExSetup::tagUpgradeSet* pUpgradeSet = m_GoodsExSetup.GetUpgradeSet(lLimitLevel, lAimCurrLevel);
		if(NULL == pUpgradeSet)
		{
			AddErrorLogText("���[%s]ʹ��[%s]������Ʒ[%s]ʱ����ʧ�ܣ�û���ҵ������䷽!����Ʒ��ǰ�ȼ�Ϊ[%d]��!!", 
				pPlayer->GetName(),
				arr_pGoods[1]->GetName(),
				arr_pGoods[0]->GetName(), 
				lAimCurrLevel);
			break;
		}


		//! ��֤Ŀ������ϴ����Ƿ�Ϸ�
		bool FindStuff = false;
		for (list<long>::const_iterator itrStuff = pUpgradeSet->listStuffIndex.begin(); itrStuff != pUpgradeSet->listStuffIndex.end(); ++itrStuff)
		{
			if (*itrStuff == arr_pGoods[1]->GetBasePropertiesIndex())
			{
				FindStuff = true;
				break;
			}
		}
		if(!FindStuff)
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}

		//! ��֤��ҵĽ�Ǯ
		long lType		= arr_pGoods[0]->GetGoodsBaseType();
		long lChidlType = 1;
		if (GT_WEAPON == lType)
		{
			lChidlType = arr_pGoods[0]->GetAddonPropertyValues(GAP_WEAPON_CATEGORY, 1);
		}

		//�� ���������л�ȡ�۸��Ϊ����Ʒ�����л�ȡ�۸񣬵�λ���
		long lNeedMoney = arr_pGoods[0]->GetAddonPropertyValues(GAP_UPGRADE_PRICE, 1);

		if ( lNeedMoney > pPlayer->GetMoney() )
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_NotEnoughMoney);
			break;
		}

		//! ��ȡ��������
		map<const GoodsExSetup::tagExtraStuff*, long> mapAssistantFlag;
		for (long i = IN_TAGUPGRADESTUFF_GOODS_NUM - MAX_UPGRADE_ASSISTANT_NUM; i < IN_TAGUPGRADESTUFF_GOODS_NUM; ++i)
		{
			if (NULL != arr_pGoods[i])
			{
				long lAssistantIndex = arr_pGoods[i]->GetBasePropertiesIndex();
				const GoodsExSetup::tagExtraStuff *tmpExtraStuff = m_GoodsExSetup.GetExtraStuffIni(lAssistantIndex);
				if(NULL != tmpExtraStuff)
				{
					map<const GoodsExSetup::tagExtraStuff*, long>::iterator ite = mapAssistantFlag.find(tmpExtraStuff);
					if (mapAssistantFlag.end() != ite)
					{
						ite->second = ite->second + 1;
					}
					else
					{
						mapAssistantFlag[tmpExtraStuff] = 1;
					}

					//! ��֤������ϵ������Ƿ񳬹�����
					if ((mapAssistantFlag.find(tmpExtraStuff))->first->lMaxUseCount < mapAssistantFlag[tmpExtraStuff])
					{
						//! �Ƿ���Ϣ���������κ���Ϣ
						return;
					}
				}
				else
				{
					//! �Ƿ���Ϣ���������κ���Ϣ
					return;
				}
			}
		}

		//! �����ʵõ���������Ľ��
		bool bSucceed = false;

		long lOdds = pUpgradeSet->lOdds;

		for (map<const GoodsExSetup::tagExtraStuff*, long>::iterator ite = mapAssistantFlag.begin(); ite != mapAssistantFlag.end(); ++ite)
		{
			lOdds += (ite->first->lAddOdds) * (ite->second);
		}
		if (0 > lOdds) lOdds = 0;
		else if(100 < lOdds) lOdds = 100;

		long lRandom = random(100);

		if (lRandom < lOdds) bSucceed = true;

		//! �������������Ŀ��

		long lUpgradeResult = eUR_Succeed;

		if (bSucceed)
		{
			//! ������Ʒ
			if(!CGoodsFactory::ChangeEquipmentLevel(arr_pGoods[0], 1))
			{
				reMsg.Add((long)eGTC_Failed);
				reMsg.Add((long)eGTC_UnknownErr);

#ifdef _GOODSLOG1_
				if(arr_pGoods[0])
				{
					char pszGoodsLog[1024]="";			
					char srcGoodsGUID[50]="";
					arr_pGoods[0]->GetExID().tostring(srcGoodsGUID);					
					_snprintf(pszGoodsLog,1024,"[��Ʒ����:GoodsExManage::OnEquipmentUpgrade][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
						arr_pGoods[0]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(arr_pGoods[0]->GetBasePropertiesIndex()),srcGoodsGUID,arr_pGoods[0]->GetAmount());
					CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
				}				
#endif
				CGoodsFactory::GarbageCollect(&(arr_pGoods[0]),35);
				break;
			}
			lNewLevel += 1;

			//! ϵͳ����
			if(10 <= lAimCurrLevel)
			{
				if(20 > lAimCurrLevel + 1)
				{
					//! ��ͨ����
					char szShowInfo[512] = {0};
					//! "%s�ġ�%s���ܵ���֮Ů��ľ�ˣ��ɹ�����%s���ɡ�%d����ǿ��Ϊ��%d����"
					_snprintf(szShowInfo, 512, CStringReading::LoadString(20, 80),
						GetGame()->GetCountryName(pPlayer->GetCountry()),
						pPlayer->GetName(),
						arr_pGoods[0]->GetName(),
						lAimCurrLevel,
						lAimCurrLevel + 1
						);

					CMessage msg( MSG_S2W_GM_WORLD_MESSAGE );
					msg.Add( NULL_GUID );
					msg.Add( szShowInfo );
					msg.Add( CGlobeSetup::GetSetup()->uUpgradeTo10FontColor );
					msg.Add( (long)0x00FFFFFF );
					msg.Send();
				}
				else
				{
					char szShowInfo[512] = {0};
					//! "%s�ġ�%s���������漣����%d���ġ�%s���������貶�Ŀ�Ĺ�â��������һ��Ϊ��λ��ʿ�Ȳʰɣ�"
					_snprintf(szShowInfo, 512, CStringReading::LoadString(20, 81),
						GetGame()->GetCountryName(pPlayer->GetCountry()),
						pPlayer->GetName(),
						lAimCurrLevel + 1,
						arr_pGoods[0]->GetName()
						);
					//! �����
					CMessage msg(MSG_S2W_OTHER_NEWS_TOP_WORLD);
					msg.Add(1l);
					msg.Add(szShowInfo);
					msg.Add(CGlobeSetup::GetSetup()->uUpgradeTo20BackIdx);
					msg.Send();
				}
			}
		}
		else
		{
			long lFailedOperate = pUpgradeSet->lFailedOperate;
			long lOperateNum	= pUpgradeSet->lOperateNum;

			bool bIsAmended				= false;
			long lAmendFailedOperate	= eFO_Null;
			long lAmendOperateNum		= 0;

			lUpgradeResult = eUR_Failed_And_LevelNotChange;

			//! ���Ҫ��ʧ�������ͷ�
			if (eFO_Null != lFailedOperate)
			{
				//! ���Ȳ�����û�����޸ĳͷ�����ĸ�����Ʒ
				long lChangeResultGoodsNum = 0;
				for (map<const GoodsExSetup::tagExtraStuff*, long>::iterator ite = mapAssistantFlag.begin(); ite != mapAssistantFlag.end(); ++ite)
				{
					if(-1 < ite->first->lFailedMinishLevel)
					{
						lAmendFailedOperate = eFO_Reduce;
						//lAmendOperateNum	= ite->first->lFailedMinishLevel;
						bIsAmended			= true;

						//if (0 == lChangeResultGoodsNum)
						//{
						//	++lChangeResultGoodsNum;
						//}
						//else
						//{
						//	//! �����˶���޸ĳͷ�����Ʒ����������Ϣ
						//	return;
						//}
						//����ͷ���Ʒʱ�жϳͷ�
						if(lChangeResultGoodsNum==0)
						{
							lAmendOperateNum = ite->first->lFailedMinishLevel;
						}else{
							if(lAmendOperateNum>ite->first->lFailedMinishLevel)
							{
								lAmendOperateNum = ite->first->lFailedMinishLevel;
							}
						}
						++lChangeResultGoodsNum;
					}
				}

				//! �Ƚϳ���õĳͷ����
				long lResultLevel = 0;
				if (eFO_Reduce == lFailedOperate)
				{
					lResultLevel = lAimCurrLevel - lOperateNum;
				}
				else if (eFO_SetTo == lFailedOperate)
				{
					lResultLevel = lOperateNum;
				}

				if (bIsAmended)
				{
					long lAmendResult = lAimCurrLevel;

					if (eFO_Reduce == lAmendFailedOperate)
					{
						lAmendResult = lAimCurrLevel - lAmendOperateNum;
					}
					else if (eFO_SetTo == lAmendFailedOperate)
					{
						lAmendResult = lAmendOperateNum;
					}

					if (lAmendResult > lResultLevel) lResultLevel = lAmendResult;
				}

				//! ִ�в���
				if (lAimCurrLevel != lResultLevel)
				{
					assert(lResultLevel < lAimCurrLevel);
					lUpgradeResult = eUR_Failed_And_Degraded;
					//! ��Ʒ����
					if(!CGoodsFactory::SetEquipmentLevel(arr_pGoods[0], lResultLevel) && 0 != lAimCurrLevel)
					{
						reMsg.Add((long)eGTC_Failed);
						reMsg.Add((long)eGTC_UnknownErr);
						break;
					}
					lNewLevel = lResultLevel;
				}


				if(10 <= lAimCurrLevel)
				{				
					//! ϵͳ����
					//if(lAmendOperateNum==0)
					if(lAimCurrLevel==lResultLevel)
					{
						//! ��ͨ����
						char szShowInfo[512] = {0};
						//! "%s�ġ�%s����������ĬĬ��װ��ǿ���ɹ���Ȼ������ԸΥ����%s���ĵȼ�û���κα仯��"
						_snprintf(szShowInfo, 512, CStringReading::LoadString(20, 84),
							GetGame()->GetCountryName(pPlayer->GetCountry()),
							pPlayer->GetName(), 
							arr_pGoods[0]->GetName()
							);

						CMessage msg( MSG_S2W_GM_WORLD_MESSAGE );
						msg.Add( NULL_GUID );
						msg.Add( szShowInfo );
						msg.Add( CGlobeSetup::GetSetup()->uUpgrade10LostFontColor );
						msg.Add( (long)0x00FFFFFF );
						msg.Send();
					}else{
						//! ��ͨ����
						char szShowInfo[512] = {0};
						//! "%s�ġ�%s����������ĬĬ��װ��ǿ���ɹ���Ȼ������ԸΥ����%s���ɡ�%d���½�����%d������"
						_snprintf(szShowInfo, 512, CStringReading::LoadString(20, 82),
							GetGame()->GetCountryName(pPlayer->GetCountry()),
							pPlayer->GetName(),
							arr_pGoods[0]->GetName(),
							lAimCurrLevel,
							lResultLevel
							);

						CMessage msg( MSG_S2W_GM_WORLD_MESSAGE );
						msg.Add( NULL_GUID );
						msg.Add( szShowInfo );
						msg.Add( CGlobeSetup::GetSetup()->uUpgrade10LostFontColor );
						msg.Add( (long)0x00FFFFFF );
						msg.Send();
					}
				}
			}
		}

		//! ɾ����Ǯ
		pPlayer->SetMoney(pPlayer->GetMoney() - lNeedMoney,11);

		//! ���ɱ��β�����ID
		CGUID optGuid;
		CGUID::CreateGUID(optGuid);

		//! ɾ������
		if(!DelGoods(pPlayer, vDelGoods, 400, optGuid))
		{
			CGoodsFactory::SetEquipmentLevel(arr_pGoods[0], lAimCurrLevel);
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SrcIsLock);
			break;
		}

		//! ��¼��־
		GetGameLogInterface()->logT400_goods_upgrade_log(pPlayer, optGuid, arr_pGoods[0], lAimCurrLevel, lNewLevel);

		vector<BYTE> vMsgData;
		arr_pGoods[0]->SerializeForOldClient(&vMsgData);

		reMsg.Add((long)eGTC_Succeed);
		reMsg.Add(lUpgradeResult);
		if (eUR_Failed_And_LevelNotChange != lUpgradeResult)
		{
			reMsg.Add((long)vMsgData.size());
			reMsg.AddEx(&(vMsgData[0]), (long)vMsgData.size());
		}
		BOOL bUpgradeSucceed = (eUR_Succeed == lUpgradeResult);
		PlayUpgradeEffect(pPlayer, m_GoodsExSetup.GetUpgrade3DEffectID(bUpgradeSucceed));

	}while (false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! ����һ����ҵ�������Ʒ��Ч
void GoodsExManage::PlayUpgradeEffect(CPlayer *pPlayer, LONG lEffectID)
{
	assert(NULL != pPlayer);
	if(NULL == pPlayer) return;
	map<CGUID, tagGoodsExWndSession>::iterator ite = m_mapOpendGEWnd.find(pPlayer->GetExID());
	assert(ite != m_mapOpendGEWnd.end());
	if(m_mapOpendGEWnd.end() == ite) return;


	CMessage msg(MSG_S2C_RGN_PLAYEFFECT);
	msg.Add((BYTE)0);
	msg.Add((LONG)lEffectID);

	msg.Add((LONG)TYPE_MONSTER);
	msg.Add(ite->second.NpcGuid);

	msg.Add((FLOAT)0.0f);
	msg.Add((FLOAT)0.0f);
	msg.Add(1L);
	msg.SendToAround(pPlayer);
	//msg.SendToAround((CServerRegion*)pPlayer->GetFather(), ite->second.lPosX, ite->second.lPosY);
}






//! ��Ӧװ��ħ��
void GoodsExManage::OnEquipmentPlugEvil(CMessage *pMsg)
{
	CMessage reMsg(MSG_S2C_GOODS_RE_PLUG_EVIL_DO);
	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_PlusEvilWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		//! ȡ�����
		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		//! �õ�������λ
		DBReadSet dBReadSet;
		pMsg->GetDBReadSet(dBReadSet);
		long lItemType = dBReadSet.GetLongFromByteArray();
		if(GAP_ELEMENT_1 > lItemType || GAP_ELEMENT_3 < lItemType)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_SrcIsLock);
			break;
		}

		//! �õ�������Ʒ
		CGUID OptGoodsGuid;
		dBReadSet.GetBufferFromByteArray(OptGoodsGuid);
		CGoods *pOptGoods = pPlayer->FindGoodsByGuidInPack(OptGoodsGuid);
		if(NULL == pOptGoods)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		//! �õ�����
		CGUID StuffGuid;
		dBReadSet.GetBufferFromByteArray(StuffGuid);
		CGoods *pStuff = pPlayer->FindGoodsByGuidInPack(StuffGuid);
		if(NULL == pStuff)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}
		const char *pClientStuffName = CGoodsFactory::QueryGoodsOriginalName(pStuff->GetBasePropertiesIndex());
		if(NULL == pClientStuffName)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}

		//! �õ���������
		CGUID HelpfulStuffGuid;
		CGoods *pHelpfulStuff = NULL;
		const char *pUseHelpfulStuffName = NULL;

		bool bUseHelpfulStuff = (1 == dBReadSet.GetByteFromByteArray());
		if (bUseHelpfulStuff)
		{
			dBReadSet.GetBufferFromByteArray(HelpfulStuffGuid);
			pHelpfulStuff = pPlayer->FindGoodsByGuidInPack(HelpfulStuffGuid);
			if (NULL == pHelpfulStuff)
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_ErrAssistant);
				break;
			}
			pUseHelpfulStuffName = CGoodsFactory::QueryGoodsOriginalName(pHelpfulStuff->GetBasePropertiesIndex());
			if(NULL == pUseHelpfulStuffName)
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_ErrAssistant);
				break;
			}
		}

		//! 
		long lGoodsLevel = pOptGoods->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);
		//! �����Ʒ��ǿ���ȼ�
		long lUpgradeLevel = pOptGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL, 1);
		//! �õ���Ʒħ������
		vector<long> vPro;
		pOptGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)lItemType, vPro);
		if (2 > vPro.size() || eGET_Disabled == vPro[0])
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantOperateAim);
			break;
		}
		bool bFristUp = eGET_Enabled == vPro[0];

		if(lUpgradeLevel==0)
		{
			//ǿ���ȼ�Ϊ0���ܽ��и�ħ����
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_NotUpgrade);
			break;
		}else if(!bFristUp&&m_GoodsExSetup.GetMaxElementLevelByUpgrade(lUpgradeLevel)<=vPro[1])
		{
			//������ǵ�һ�θ�ħ�Ҹ�ħ�ȼ����ڵȼ���ǰǿ������֧�ֵ����ȼ������ܲ���
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_UpgradeLow);
			break;
		}

		//! �õ�����ħ������
		set<DWORD> setHasType;
		if (bFristUp)
		{
			for (DWORD i = GAP_ELEMENT_1; i <= GAP_ELEMENT_3; ++i)
			{
				vector<long> vTmpPro;
				pOptGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)i, vTmpPro);
				if(eGET_Enabled < vTmpPro[0])
					setHasType.insert(vTmpPro[0] % 10);
			}
		}

		bool bIsAttEquipment = (TRUE == CGoodsFactory::IsAttEquipment(pOptGoods));

		//! ��ȡ��������
		long lElementType	= vPro[0] % 10;
		long lNeedStuffNum	= 0;
		long lSucceedOdds	= 0;
		GoodsExSetup::LPReStuffName pStuffName = NULL;

		if(bFristUp)
		{
			vPro[1] = 0;
			lElementType = m_GoodsExSetup.GetElement(bIsAttEquipment, lGoodsLevel, pClientStuffName, lNeedStuffNum, lSucceedOdds);
			if(MT_Invalid == lElementType || setHasType.end() != setHasType.find(lElementType))
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_CantOperateAim);
				break;
			}
		}
		else
		{
			if(!m_GoodsExSetup.GetElement(bIsAttEquipment, lGoodsLevel, vPro[1], lElementType, pStuffName, lNeedStuffNum, lSucceedOdds))
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_CantOperateAim);
				break;
			}
		}

		//! �������͡����������ж�
		if(!bFristUp)
		{
			if(0 != strcmp(pClientStuffName, pStuffName) || pStuff->GetAmount() < lNeedStuffNum)
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_ErrStuff);
				break;
			}
		}

		//! ��ȡ������������
		if (bUseHelpfulStuff)
		{
			long lAddOdds = m_GoodsExSetup.GetHelpfulStuffOdds(vPro[1], pUseHelpfulStuffName);
			if (0 > lAddOdds)
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_ErrAssistant);
				break;
			}

			lSucceedOdds += lAddOdds;
		}

		//! �������
		bool bSucceed = (random(10000) < lSucceedOdds);
		if(bSucceed)
		{
			if(bFristUp)
			{
				if(bIsAttEquipment)
					pOptGoods->SetAddonPropertyBaseValues((GOODS_ADDON_PROPERTIES)lItemType, 1, lElementType + eGET_Attack);
				else
					pOptGoods->SetAddonPropertyBaseValues((GOODS_ADDON_PROPERTIES)lItemType, 1, lElementType + eGET_Defend);
			}

			pOptGoods->SetAddonPropertyBaseValues((GOODS_ADDON_PROPERTIES)lItemType, 2, vPro[1] + 1);
		}


		//! ���ɱ��β�����ID
		CGUID optGuid;
		CGUID::CreateGUID(optGuid);

		//! ɾ������
		//! �ȼ�¼һ������ɾ����־����ɾ��ʧ�ܣ��򲻻������Ʒħ���ȼ��ı����־
		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pStuff, 0, pStuff->GetAmount(), pStuff->GetAmount() - lNeedStuffNum);
		if(!pPlayer->DelGoods(StuffGuid, lNeedStuffNum))
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}
		//! ��������
		if (bUseHelpfulStuff)
		{
			//! �ȼ�¼һ������ɾ����־����ɾ��ʧ�ܣ��򲻻������Ʒħ���ȼ��ı����־
			GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pHelpfulStuff, 0, pHelpfulStuff->GetAmount(), pHelpfulStuff->GetAmount() - 1);
			if(!pPlayer->DelGoods(HelpfulStuffGuid, 1))
			{
				reMsg.Add((long)eGTC_ErrOperation);
				reMsg.Add((long)eGTC_ErrStuff);
				break;
			}
		}

		//! ħ�������־��ͨ��������־�����ж��Ƿ�ɾ���˲�����û��ִ��ħ�����
		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pOptGoods, lItemType, vPro[1], ((bSucceed) ? vPro[1] + 1 : vPro[1]));

		vector<BYTE> vMsgData;
		pOptGoods->SerializeForOldClient(&vMsgData);

		reMsg.Add((long)eGTC_Succeed);
		reMsg.Add((BYTE)bSucceed);
		reMsg.Add((long)vMsgData.size());
		reMsg.AddEx(&(vMsgData[0]), (long)vMsgData.size());

	}while (false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! �����������ڸ�����һ���ʾ���������䳤�ȵ����У��õ�һ���������������һ�������У���������������
DWORD RandBoundIndex(const long lBound[], long lSize)
{
	struct tagBound
	{
		long lBegin;
		long lEnd;
	};
	long lAllOdds = 0;
	vector<tagBound> vBound;
	long lCurr = 0;
	for (long i = 0; i < lSize; ++i)
	{
		lAllOdds += lBound[i];
		tagBound Bound;
		Bound.lBegin	= lCurr;
		Bound.lEnd		= lCurr + lBound[i];
		lCurr			+= lBound[i];
		vBound.push_back(Bound);
	}

	if (0 < lAllOdds)
	{
		long lPointOfFall = random(lAllOdds);
		for (long i = 0; i < vBound.size(); ++i)
		{
			if (vBound[i].lBegin != vBound[i].lEnd)
				if(vBound[i].lBegin <= lPointOfFall && vBound[i].lEnd > lPointOfFall)
					return i;
		}
	}

	return 0xFFFFFFFF;
}

//! ��Ӧ����ħ������
void GoodsExManage::OnEquipmentResetElementType(CMessage *pMsg)
{
	CMessage reMsg(MSG_C2S_GOODS_RE_RESET_ELEMENT_TYPE);
	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_ResetElementTypeWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		//! ȡ�����
		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		//! �õ�������λ
		DBReadSet dBReadSet;
		pMsg->GetDBReadSet(dBReadSet);
		long lItemType = dBReadSet.GetLongFromByteArray();
		if(GAP_ELEMENT_1 > lItemType || GAP_ELEMENT_3 < lItemType)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_SrcIsLock);
			break;
		}

		//! �õ�������Ʒ
		CGUID OptGoodsGuid;
		dBReadSet.GetBufferFromByteArray(OptGoodsGuid);
		CGoods *pOptGoods = pPlayer->FindGoodsByGuidInPack(OptGoodsGuid);
		if(NULL == pOptGoods)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		//! �õ�����
		CGUID StuffGuid;
		dBReadSet.GetBufferFromByteArray(StuffGuid);
		CGoods *pStuff = pPlayer->FindGoodsByGuidInPack(StuffGuid);
		if(NULL == pStuff)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}
		const char *pClientStuffName = CGoodsFactory::QueryGoodsOriginalName(pStuff->GetBasePropertiesIndex());
		if(NULL == pClientStuffName)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}

		//! 
		long lGoodsLevel = pOptGoods->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);
		//! �õ�ħ����λ����
		vector<long> vPro;
		pOptGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)lItemType, vPro);
		if (2 > vPro.size() || eGET_Disabled == vPro[0] || eGET_Enabled == vPro[0])
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantOperateAim);
			break;
		}
		//! �õ�����ħ������
		set<DWORD> setHasType;
		for (DWORD i = GAP_ELEMENT_1; i <= GAP_ELEMENT_3; ++i)
		{
			vector<long> vTmpPro;
			pOptGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)i, vTmpPro);
			if(eGET_Enabled < vTmpPro[0])
				setHasType.insert(vTmpPro[0] % 10);
		}

		//! ��ȡ��������
		const GoodsExSetup::tagResetStuff *pResetStuff = m_GoodsExSetup.GetResetElement(lGoodsLevel, pClientStuffName);
		if (NULL == pResetStuff)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantOperateAim);
			break;
		}


		DWORD lNewElementType = RandBoundIndex(pResetStuff->lOdds, MT_Max);
		long lCount = 0;
		while (setHasType.end() != setHasType.find(lNewElementType))
		{
			++lCount;
			if(20 <= lCount)
			{
				lNewElementType = 0xFFFFFFFF;
				break;
			}
			lNewElementType = RandBoundIndex(pResetStuff->lOdds, MT_Max);
		}
		if (0xFFFFFFFF == lNewElementType)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}
		//! ��������
		if(CGoodsFactory::IsAttEquipment(pOptGoods))
			lNewElementType = lNewElementType + eGET_Attack;
		else
			lNewElementType = lNewElementType + eGET_Defend;

		pOptGoods->SetAddonPropertyBaseValues((GOODS_ADDON_PROPERTIES)lItemType, 1, lNewElementType);

		//! ���ɱ��β�����ID
		CGUID optGuid;
		CGUID::CreateGUID(optGuid);

		//! ɾ������
		//! �ȼ�¼һ������ɾ����־����ɾ��ʧ�ܣ��򲻻������Ʒħ���ȼ��ı����־
		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pStuff, 0, pStuff->GetAmount(), pStuff->GetAmount() - 1);
		if(!pPlayer->DelGoods(StuffGuid, 1))
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}

		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pOptGoods, lItemType, vPro[0], lNewElementType);

		vector<BYTE> vMsgData;
		pOptGoods->SerializeForOldClient(&vMsgData);

		reMsg.Add((long)eGTC_Succeed);
		reMsg.Add((long)vMsgData.size());
		reMsg.AddEx(&(vMsgData[0]), (long)vMsgData.size());

	}while (false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! ��Ӧ����ħ������
void GoodsExManage::OnEquipmentResetXiangXing(CMessage *pMsg)
{
	CMessage reMsg(MSG_C2S_GOODS_RE_RESET_XIANG_XING);
	do 
	{
		if(!TestSession(pMsg->GetPlayerID(), eGWT_ResetElementXiangXingWindow))
		{
			reMsg.Add((long)eGTC_Failed);
			reMsg.Add((long)eGTC_SessionLapse);
			break;
		}

		//! ȡ�����
		CPlayer* pPlayer = pMsg -> GetPlayer();
		if (NULL == pPlayer) return;
		if (CPlayer::PROGRESS_NONE != pPlayer->GetCurrentProgress()) return;

		//! �õ�������λ
		DBReadSet dBReadSet;
		pMsg->GetDBReadSet(dBReadSet);
		long lItemType = dBReadSet.GetLongFromByteArray();
		if(GAP_ELEMENT_XIANG_XING_1 > lItemType || GAP_ELEMENT_XIANG_XING_3 < lItemType)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_SrcIsLock);
			break;
		}

		//! �õ�������Ʒ
		CGUID OptGoodsGuid;
		dBReadSet.GetBufferFromByteArray(OptGoodsGuid);
		CGoods *pOptGoods = pPlayer->FindGoodsByGuidInPack(OptGoodsGuid);
		if(NULL == pOptGoods)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantFindAim);
			break;
		}

		//! �õ�����
		CGUID StuffGuid;
		dBReadSet.GetBufferFromByteArray(StuffGuid);
		CGoods *pStuff = pPlayer->FindGoodsByGuidInPack(StuffGuid);
		if(NULL == pStuff)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}
		const char *pClientStuffName = CGoodsFactory::QueryGoodsOriginalName(pStuff->GetBasePropertiesIndex());
		if(NULL == pClientStuffName)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}

		//! 
		long lGoodsLevel = pOptGoods->GetAddonPropertyValues(GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1);
		//! �õ�ħ����λ����
		vector<long> vPro;
		pOptGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)lItemType, vPro);
		if (2 > vPro.size() || eGET_Disabled == vPro[0] || eGET_Enabled == vPro[0])
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantOperateAim);
			break;
		}

		//! �õ�����ħ������
		set<DWORD> setHasType;
		for (DWORD i = GAP_ELEMENT_1; i <= GAP_ELEMENT_3; ++i)
		{
			vector<long> vTmpPro;
			pOptGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)i, vTmpPro);
			if(eGET_Enabled < vTmpPro[0])
				setHasType.insert(vTmpPro[0] % 10);
		}

		//! ��ȡ��������
		GoodsExSetup::LPCXiangXingOdds pLPCXiangXingOdds = NULL;
		const GoodsExSetup::tagResetStuff *pResetStuff = m_GoodsExSetup.GetResetXiangXing(lGoodsLevel, pClientStuffName, pLPCXiangXingOdds);
		if (NULL == pResetStuff || NULL == pLPCXiangXingOdds)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_CantOperateAim);
			break;
		}

		DWORD lNewXiangXingType = RandBoundIndex(pResetStuff->lOdds, MT_Max);
		long lCount = 0;
		while (setHasType.end() != setHasType.find(lNewXiangXingType))
		{
			++lCount;
			if(20 <= lCount)
			{
				lNewXiangXingType = 0xFFFFFFFF;
				break;
			}
			lNewXiangXingType = RandBoundIndex(pResetStuff->lOdds, MT_Max);
		}
		DWORD lNewXiangXingValue = RandBoundIndex(pLPCXiangXingOdds->lOdds, eXXV_NUM);
		if (0xFFFFFFFF == lNewXiangXingType || 0xFFFFFFFF == lNewXiangXingValue)
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}
		//! ��������
		if(CGoodsFactory::IsAttEquipment(pOptGoods))
			lNewXiangXingType = lNewXiangXingType + eGET_Attack;
		else
			lNewXiangXingType = lNewXiangXingType + eGET_Defend;

		pOptGoods->SetAddonPropertyBaseValues((GOODS_ADDON_PROPERTIES)lItemType, 1, lNewXiangXingType);
		pOptGoods->SetAddonPropertyBaseValues((GOODS_ADDON_PROPERTIES)lItemType, 2, s_XiangXingValus[lNewXiangXingValue]);

		//! ���ɱ��β�����ID
		CGUID optGuid;
		CGUID::CreateGUID(optGuid);

		//! ɾ������
		//! �ȼ�¼һ������ɾ����־����ɾ��ʧ�ܣ��򲻻������Ʒħ���ȼ��ı����־
		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pStuff, 0, pStuff->GetAmount(), pStuff->GetAmount() - 1);
		if(!pPlayer->DelGoods(StuffGuid, 1))
		{
			reMsg.Add((long)eGTC_ErrOperation);
			reMsg.Add((long)eGTC_ErrStuff);
			break;
		}

		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pOptGoods, lItemType, vPro[0], lNewXiangXingType);
		GetGameLogInterface()->logT410_goods_plugevil_log(pPlayer, optGuid, pOptGoods, lItemType, vPro[1], s_XiangXingValus[lNewXiangXingValue]);

		vector<BYTE> vMsgData;
		pOptGoods->SerializeForOldClient(&vMsgData);

		reMsg.Add((long)eGTC_Succeed);
		reMsg.Add((long)vMsgData.size());
		reMsg.AddEx(&(vMsgData[0]), (long)vMsgData.size());

	}while (false);

	reMsg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! ˽�з���
///////////////////////////////////////////////////////////////////////////////////
/*
�õ�һ����Ʒ�ķֽ�������ID������
����result������pair<��ƷID, ��Ʒ����>
��������pGoods�ĺϷ���
*/
long GoodsExManage::GetGoodsDisassembleResult(long lGoodsID,  tagReValue arrReValue[MAX_DISASSEMBLE_NUM])
{
	long re = 0;

	CGoodsBaseProperties* pGoodsBaseProperties = CGoodsFactory::QueryGoodsBaseProperties(lGoodsID);
	if (NULL == pGoodsBaseProperties) return re;
	if (!pGoodsBaseProperties->IsHasAddonPropertie(GAP_CAN_DISASSEMBLE)) return re;

	//! ȡ�ÿɷ�ֽ�
	vector<CGoodsBaseProperties::tagAddonPropertyValue> vOut;
	pGoodsBaseProperties->GetAddonPropertyValues(GAP_CAN_DISASSEMBLE, vOut);
	long lAbleDisassemble = vOut[0].lBaseValue;
	if (0 == lAbleDisassemble) return re;

	//! ȡ�÷ֽ��ֵ
	vOut.clear();
	pGoodsBaseProperties->GetAddonPropertyValues(GAP_DISASSEMBLE_VALUE, vOut);
	long lDisassembleValue1 = vOut[0].lBaseValue;
	long lDisassembleValue2 = vOut[1].lBaseValue;

	if (0 == lDisassembleValue1 || 0 == lDisassembleValue2) return re;

	//! ȡ����ƷƷ��
	vOut.clear();
	pGoodsBaseProperties->GetAddonPropertyValues(GAP_QUALITY, vOut);
	if (1 > vOut.size()) return re;
	long lQuality = vOut[0].lBaseValue;


	//! ȡ������
	const GoodsExSetup::tagDisassemblePrescription  *pDisassemblePrescription = m_GoodsExSetup.GetDisassemblePrescription(lQuality);

	if (NULL == pDisassemblePrescription) 
		return re;

	for (long idx = 0; idx < MAX_DISASSEMBLE_NUM; ++idx)
	{
		if (0 != pDisassemblePrescription->arr_lOutGoodsId[idx])
		{
			//! �õ�������ID
			arrReValue[idx].lGoodsIndex		= pDisassemblePrescription->arr_lOutGoodsId[idx];
			if(0 == idx)
			{
				//! ��һ�в���
				arrReValue[idx].lGoodsNum	= m_GoodsExSetup.GetDisassembleNum(
					lDisassembleValue1,
					lDisassembleValue2,
					lQuality,
					idx
					);
			}
			else if(1 == idx)
			{
				//! �ڶ��в���
				vOut.clear();
				pGoodsBaseProperties->GetAddonPropertyValues(GAP_BLUE_CRYSTAL, vOut);
				if (0 < vOut.size())
					arrReValue[idx].lGoodsNum = vOut[0].lBaseValue;
				else
					arrReValue[idx].lGoodsNum = 0;

			}
			else if(2 == idx)
			{
				arrReValue[idx].lGoodsNum = 0;
			}
			++re;
		}
		else
		{
			arrReValue[idx].lGoodsIndex		= 0;
			arrReValue[idx].lGoodsNum		= 0;
		}
	}

	return re;
}

//!		��������Ϣ��Ӧ�ڴ�ִ�к���
void	GoodsExManage::BindAndFrost(bool isBind, CMessage *pMsg)
{
	CMessage msg;
	if (isBind)
		msg.Init(MSG_S2C_GOODS_BIND);
	else
		msg.Init(MSG_S2C_GOODS_FROST);

	do 
	{
		//! ���ԻỰ
		if (!TestSession(pMsg->GetPlayerID(), eGWT_PrescriptionWindow))
		{
			msg.Add((long)eGTC_Failed);
			msg.Add((long)eGTC_SessionLapse);
			break;
		}

		//! ���ȵõ������ֶ�
		long lOperation = pMsg->GetLong();
		if (eFT_Frost != lOperation && eFT_UnFrost != lOperation)
		{
			msg.Add((long)eGTC_Failed);
			msg.Add((long)eGTC_ErrOperation);
			break;
		}

		//! �õ��������
		CPlayer *pPlayer = pMsg->GetPlayer();
		if(NULL == pPlayer) return;

		CGUID	arrGoodGuid[2];				//! ��ƷID
		CGoods	*arr_pGoods[2]		= {NULL};
		long	arrlContianerID[2]	= {0};	//! ��Ʒ��������
		long	arrlPos[2]			= {0};	//! ��Ʒ����λ��

		//! ��һ����Ʒ������Ʒ
		pMsg->GetGUID(arrGoodGuid[0]);
		arrlContianerID[0]	= pMsg->GetLong();
		arrlPos[0]			= pMsg->GetLong();

		arr_pGoods[0] = TestGoodsHere(pPlayer, arrGoodGuid[0], arrlContianerID[0], arrlPos[0]);
		if (NULL == arr_pGoods[0])
		{
			msg.Add((long)eGTC_Failed);
			msg.Add((long)eGTC_ErrAssistant);
			break;
		}

		//! �ڶ�����Ʒ��Ŀ��
		pMsg->GetGUID(arrGoodGuid[1]);
		arrlContianerID[1]	= pMsg->GetLong();
		arrlPos[1]			= pMsg->GetLong();

		arr_pGoods[1] = TestGoodsHere(pPlayer, arrGoodGuid[1], arrlContianerID[1], arrlPos[1]);
		if (NULL == arr_pGoods[1] || arr_pGoods[1] == arr_pGoods[0])
		{
			msg.Add((long)eGTC_Failed);
			msg.Add((long)eGTC_CantFindAim);
			break;
		}

		//����֤����������
		LONG lLogType = 0;
		if (isBind)
		{
			if (eBT_Bind == lOperation)
			{
				if (!m_GoodsExSetup.TestBindGoodsIndex(arr_pGoods[0]->GetBasePropertiesIndex()))
				{
					msg.Add((long)eGTC_Failed);
					msg.Add((long)eGTC_ErrAssistant);
					break;
				}
			}
			else
			{
				if (!m_GoodsExSetup.TestUntieGoodsIndex(arr_pGoods[0]->GetBasePropertiesIndex()))
				{
					msg.Add((long)eGTC_Failed);
					msg.Add((long)eGTC_ErrAssistant);
					break;
				}
				lLogType = 1;
			}
		}
		else
		{	
			if (eFT_Frost == lOperation)
			{
				if (!m_GoodsExSetup.TestFrostGoodsIndex(arr_pGoods[0]->GetBasePropertiesIndex()))
				{
					msg.Add((long)eGTC_Failed);
					msg.Add((long)eGTC_ErrAssistant);
					break;
				}
				lLogType = 2;
			}
			else
			{
				if (!m_GoodsExSetup.TestThawGoodsIndex(arr_pGoods[0]->GetBasePropertiesIndex()))
				{
					msg.Add((long)eGTC_Failed);
					msg.Add((long)eGTC_ErrAssistant);
					break;
				}
				lLogType = 3;
			}
		}


		//! ִ�в���
		if (isBind)
		{
			if(0 != SetGoodsBindProperty_Player(arr_pGoods[1], lOperation, pMsg->GetPlayerID()))
			{
				msg.Add((long)eGTC_Failed);
				msg.Add((long)eGTC_CantOperateAim);
				break;
			}
		}
		else
		{
			if(0 != SetGoodsFrostProperty_Player(arr_pGoods[1], lOperation, pMsg->GetPlayerID()))
			{
				msg.Add((long)eGTC_Failed);
				msg.Add((long)eGTC_CantOperateAim);
				break;
			}
		}

		GetGameLogInterface()->logT421_goods_bind_and_frost(pPlayer, arr_pGoods[1], arr_pGoods[0], lLogType);

		//! ɾ��������
		vector<tagGoodsOperate> vDelGoods;
		tagGoodsOperate GoodsOperate;
		GoodsOperate.lContianerPos	= arrlContianerID[0];
		GoodsOperate.lGoodsPos		= arrlPos[0];
		GoodsOperate.lOperateNum	= 1;
		GoodsOperate.lOperateWay	= (arr_pGoods[0]->GetAmount() > 1) ? CS2CContainerObjectMove::OT_CHANGE_AMOUNT : CS2CContainerObjectMove::OT_DELETE_OBJECT;
		vDelGoods.push_back(GoodsOperate);

		if (!DelGoods(pPlayer, vDelGoods, 0, NULL_GUID))
		{
			msg.Add((long)eGTC_Failed);
			msg.Add((long)eGTC_DelAssistantFailed);
			break;
		}

		msg.Add((long)eGTC_Succeed);


	} while(false);

	msg.SendToPlayer(pMsg->GetPlayerID(), false);
}

//! ��־�ӿ�
///////////////////////////////////////////////////////////////////////////////////

/* --------------------------------------------------------------------
<!--��Ʒ�ֽ���־-->
<item type="client_head"     head_name=""></item>
<item type="player"   head_name=""></item>
<item type="goods"    head_name=""></item><!--��Ʒ-->
<item type="alone"    Item_name="opt_id"            data_type="guid"></item><!--�ֽ������ID��-->
<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=ɾ���ģ�1=��ӵ�-->
<item type="alone"    Item_name="change_type"       data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
</Table>
*/
bool LogicLogInterface::logT380_goods_disassemble_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, const CGUID& guid, BOOL bIsAdd)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pGoods) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		380, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
		&guid, bIsAdd, lAddType
		);
}


/* --------------------------------------------------------------------
<!--��Ʒ�ϳ���־-->
<Table name="t390_goods_synthesize_log" log_no="39" log_pos="1">
<item type="client_head"     head_name=""></item>
<item type="player"   head_name=""></item>
<item type="goods"    head_name=""></item><!--��Ʒ-->
<item type="alone"    Item_name="opt_id"            data_type="guid"></item><!--�ֽ������ID��-->
<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=ɾ���ģ�1=��ӵ�-->
<item type="alone"    Item_name="change_type"       data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
</Table>
*/
bool LogicLogInterface::logT390_goods_synthesize_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, const CGUID& guid, BOOL bIsAdd)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pGoods) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		390, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(), 
		&guid, bIsAdd, lAddType
		);
}


/* --------------------------------------------------------------------
<!--��Ʒ������־-->
<Table name="t400_goods_upgrade_log" log_no="40" log_pos="1">
<item type="client_head"     head_name=""></item>
<item type="player"   head_name=""></item>
<item type="alone"    Item_name="opt_id"            data_type="guid"></item>  <!--������ID��-->
<item type="goods"    head_name=""></item><!--����������Ʒ-->
<item type="alone"    Item_name="old_level"      data_type="int32"></item><!--����ǰ�ȼ�-->
<item type="alone"    Item_name="new_level"      data_type="int32"></item><!--������ȼ�-->
</Table>
*/
bool LogicLogInterface::logT400_goods_upgrade_log(CPlayer *pPlayer, const CGUID& guid, CGoods *pGoods, long lOldLevel, long lNewLevel)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pGoods) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		400, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		&guid,
		pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
		lOldLevel, lNewLevel
		);
}


/* --------------------------------------------------------------------
<!--��Ʒħ����־-->
<Table name="t41_goods_plugevil_log" log_no="41" log_pos="1">
<item type="client_head"     head_name=""></item>
<item type="player"   head_name=""></item>
<item type="alone"    Item_name="opt_id"            data_type="guid"></item>  <!--������ID��-->
<item type="goods"    head_name=""></item><!--����������Ʒ-->
<item type="alone"    Item_name="item_index"     data_type="int32"></item> <!--�޸ĵ�ħ����λ-->
<item type="alone"    Item_name="old_level"      data_type="int32"></item><!--����ǰ�ȼ�-->
<item type="alone"    Item_name="new_level"      data_type="int32"></item><!--������ȼ�-->
</Table>
*/
bool LogicLogInterface::logT410_goods_plugevil_log(CPlayer *pPlayer, const CGUID& guid, CGoods *pGoods, long lItemIndex, long lOldLevel, long lNewLevel)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pGoods) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		410, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		&guid,
		pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
		lItemIndex, lOldLevel, lNewLevel
		);
}

/*
<!--ħ�����ϻ����־-->
<Table name="t420_evil_stuff_synthesize_log" log_no="42" log_pos="1">
<item type="client_head"     head_name=""></item>
<item type="player"   head_name=""></item>
<item type="goods"    head_name=""></item><!--���ϳɵ���Ʒ-->
<item type="alone"    Item_name="add_type"      data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
<item type="alone"    Item_name="get_num"      data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
</Table>
*/
bool LogicLogInterface::logT420_evil_stuff_synthesize_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, long lGetNum)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pGoods) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		420, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(), lAddType, lGetNum
		);
}

/* --------------------------------------------------------------------
<!--��Ʒ�󶨺Ͷ�����־-->
<Table name="t421_goods_bind_and_frost" log_no="421" log_pos="1">
<item type="client_head"     head_name=""></item>
<item type="player"   head_name=""></item>
<item type="goods"    head_name=""></item><!--����������Ʒ-->
<item type="goods"    head_name=""></item><!--���ĵ���Ʒ-->
<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=�󶨣�1=����󶨣� 2=���ᣬ3=�������-->
</Table>
*/
bool LogicLogInterface::logT421_goods_bind_and_frost(CPlayer *pPlayer, CGoods *pAim, CGoods *pStuff, long lType)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pAim || NULL == pStuff) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		421, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		pAim->GetBasePropertiesIndex(), &(pAim->GetExID()), pAim->GetName(), 
		pStuff->GetBasePropertiesIndex(), &(pStuff->GetExID()), pStuff->GetName(), 
		lType
		);
}

