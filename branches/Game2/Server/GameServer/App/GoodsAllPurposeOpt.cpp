

#include "StdAfx.h"
#include "GoodsAllPurposeOpt.h"
#include "Player.h"
//#include "Script/Script.h"



GoodsAllPurposeOpt::GoodsAllPurposeOpt(void)
:m_OwnerGuid(NULL_GUID)
,m_dwTitleID(0)
,m_dwButtonID(0)
,m_lRadius(0L)
,m_lPosX(0L)
,m_lPosY(0L)
,m_RegionGuid(NULL_GUID)
,m_dwMaxPushNum(0)
{
	memset(m_szText, 0, STR_TXT_SIZE);
	memset(m_szOnPushScript, 0, STR_INFO_SIZE);
	memset(m_szOnFinishScript, 0, STR_INFO_SIZE);
	memset(m_arrGoodsGuid, 0, sizeof(CGUID) * MAX_ALLPURPOSE_CONTAINER_SIZE);
}
GoodsAllPurposeOpt::~GoodsAllPurposeOpt(void)
{
	Release(FALSE);
}

//!
void GoodsAllPurposeOpt::Release(BOOL bSendCloseMsg)
{
	memset(m_arrGoodsGuid, 0, sizeof(CGUID) * MAX_ALLPURPOSE_CONTAINER_SIZE);
	m_RegionGuid	= NULL_GUID;
	m_dwMaxPushNum	= 0;

	memset(m_szText, 0, STR_TXT_SIZE);
	memset(m_szOnPushScript, 0, STR_INFO_SIZE);
	memset(m_szOnFinishScript, 0, STR_INFO_SIZE);

	if(bSendCloseMsg)
	{
		CMessage msg(MSG_S2C_GOODS_AllPurpose_Close);
		msg.SendToPlayer(m_OwnerGuid);
	}
}

//! ��ʼ��Ʒ����
BOOL GoodsAllPurposeOpt::BeginOpt(
	 const	CGUID	&OwnerGuid,			//! �������GUID
	 DWORD			dwTitleID,			//! ����ID
	 DWORD			dwButtonID,			//! ��ťID
	 LPCSTR			pText,				//! ����˵������
	 LPCSTR			pOnPushScript,		//! ��Ӧ��Ʒ����Ľű���
	 LPCSTR			pOnFinishScript,	//! ��Ӧ��ť���µĽű���
	 LONG			lRadius,			//! �Ựʱ����Ļ�뾶
	 LONG			lPosX,				//! ���Χ����X����
	 LONG			lPosY,				//! ���Χ����Y����
	 const	CGUID	&RegionGuid,		//! ���ڵ�ͼGUID
	 DWORD			dwMaxPushNum			//! ��������Ʒ��������������MAX_ALLPURPOSE_CONTAINER_SIZE��
)
{
	Release(FALSE);

	if(NULL == pText || NULL == pOnPushScript || NULL == pOnFinishScript || 0 >= lRadius || 0 > lPosX || 0 > lPosY || MAX_ALLPURPOSE_CONTAINER_SIZE < dwMaxPushNum || 0 == dwMaxPushNum)
		return FALSE;

	//if(NULL == GetGame()->GetScriptFileData(pOnPushScript) || GetGame()->GetScriptFileData(pOnFinishScript))
		//return FALSE;

	m_OwnerGuid			= OwnerGuid;
	m_dwTitleID			= dwTitleID;
	m_dwButtonID		= dwButtonID;
	m_dwMaxPushNum		= dwMaxPushNum;

	memcpy(m_szText, pText, min(STR_TXT_SIZE - 1, strlen(pText)));
	memcpy(m_szOnPushScript, pOnPushScript, min(STR_INFO_SIZE - 1, strlen(pOnPushScript)));
	memcpy(m_szOnFinishScript, pOnFinishScript, min(STR_INFO_SIZE - 1, strlen(pOnFinishScript)));

	m_lRadius			= lRadius;
	m_lPosX				= lPosX;
	m_lPosY				= lPosY;
	m_RegionGuid		= RegionGuid;

	//! 
	CMessage msgOpen(MSG_S2C_GOODS_AllPurpose_Open);

	DBWriteSet OpendbWriteSet;
	msgOpen.GetDBWriteSet(OpendbWriteSet);
	OpendbWriteSet.AddToByteArray(dwMaxPushNum);
	OpendbWriteSet.AddToByteArray(m_dwTitleID);
	OpendbWriteSet.AddToByteArray(m_dwButtonID);
	OpendbWriteSet.AddToByteArray(m_lRadius);
	OpendbWriteSet.AddToByteArray(m_lPosX);
	OpendbWriteSet.AddToByteArray(m_lPosY);

	msgOpen.SendToPlayer(OwnerGuid, false);

	//! 
	CMessage msgTxt(MSG_S2C_GOODS_AllPurpose_UpdateTxt);

	DBWriteSet TxtdbWriteSet;
	msgTxt.GetDBWriteSet(TxtdbWriteSet);
	TxtdbWriteSet.AddToByteArray(m_szText);

	msgTxt.SendToPlayer(m_OwnerGuid, false);

	return TRUE;
}

//! ��Ӧ��Ʒ����
GoodsAllPurposeOpt::eOptResult GoodsAllPurposeOpt::OnGoodsPut(const CGUID &GoodsGuid, DWORD dwIdx)
{
	if(m_dwMaxPushNum > dwIdx)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(m_OwnerGuid);
		if(NULL != pPlayer)
		{
			if(CanOpt(pPlayer))
			{
				//! �ҵ�����Ʒ
				if(NULL != pPlayer->FindGoodsByGuidInPack(GoodsGuid))
				{
					m_arrGoodsGuid[dwIdx] = GoodsGuid;

					//! ִ�нű�
					pPlayer->PlayerRunScript(m_szOnPushScript);

					CMessage msg(MSG_S2C_GOODS_AllPurpose_RePush);
					DBWriteSet dbWriteSet;
					msg.GetDBWriteSet(dbWriteSet);
					dbWriteSet.AddToByteArray(dwIdx);
					dbWriteSet.AddToByteArray(GoodsGuid);
					msg.SendToPlayer(m_OwnerGuid, false);

					return eOR_Succeed;
				}
				else
				{
					return eOR_Failed;
				}
			}
		}
	}

	return eOR_Lawless;
}

//! ��Ӧ��Ʒ�Ƴ�
GoodsAllPurposeOpt::eOptResult GoodsAllPurposeOpt::OnGoodsPop(DWORD dwIdx)
{
	if(m_dwMaxPushNum > dwIdx || 0xFFFFFFFF == dwIdx)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(m_OwnerGuid);
		if(NULL != pPlayer)
		{
			if(CanOpt(pPlayer))
			{
				if(0xFFFFFFFF == dwIdx)
					memset(m_arrGoodsGuid, 0, sizeof(CGUID) * MAX_ALLPURPOSE_CONTAINER_SIZE);
				else
					m_arrGoodsGuid[dwIdx] = NULL_GUID;
				CMessage msg(MSG_S2C_GOODS_AllPurpose_RePop);
				DBWriteSet dbWriteSet;
				msg.GetDBWriteSet(dbWriteSet);
				dbWriteSet.AddToByteArray(dwIdx);
				msg.SendToPlayer(m_OwnerGuid, false);

				CMessage msgTxt(MSG_S2C_GOODS_AllPurpose_UpdateTxt);
				DBWriteSet TxtdbWriteSet;
				msgTxt.GetDBWriteSet(TxtdbWriteSet);
				TxtdbWriteSet.AddToByteArray(m_szText);
				msgTxt.SendToPlayer(m_OwnerGuid, false);

				return eOR_Succeed;
			}
		}
	}

	return eOR_Lawless;
}

//! ��Ӧ��ʼ��Ʒ����
GoodsAllPurposeOpt::eOptResult GoodsAllPurposeOpt::FinishOpt(void)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(m_OwnerGuid);
	if(NULL != pPlayer)
	{
		if(CanOpt(pPlayer))
		{
			BOOL bPass = TRUE;
			//! �ҵ���Ʒ
			for (DWORD i = 0; i < m_dwMaxPushNum; ++i)
			{
				if(NULL == pPlayer->FindGoodsByGuidInPack(m_arrGoodsGuid[i]))
				{
					bPass = FALSE;
					break;
				}
			}
			
			if(bPass)
			{
				//! ִ�нű�
				pPlayer->PlayerRunScript(m_szOnFinishScript);

				return eOR_Succeed;
			}
			else
			{
				memset(m_arrGoodsGuid, 0, sizeof(CGUID) * MAX_ALLPURPOSE_CONTAINER_SIZE);
				CMessage msg(MSG_S2C_GOODS_AllPurpose_RePop);
				DBWriteSet dbWriteSet;
				msg.GetDBWriteSet(dbWriteSet);
				dbWriteSet.AddToByteArray((DWORD)0xFFFFFFFF);
				msg.SendToPlayer(m_OwnerGuid, false);

				CMessage msgTxt(MSG_S2C_GOODS_AllPurpose_UpdateTxt);
				DBWriteSet TxtdbWriteSet;
				msgTxt.GetDBWriteSet(TxtdbWriteSet);
				TxtdbWriteSet.AddToByteArray(m_szText);
				msgTxt.SendToPlayer(m_OwnerGuid, false);

				return eOR_Failed;
			}
		}
	}

	return eOR_Lawless;
}

//! ˢ��������ʾ����
GoodsAllPurposeOpt::eOptResult GoodsAllPurposeOpt::SetTxt(LPCSTR pTxt)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(m_OwnerGuid);
	if(NULL != pPlayer)
	{
		if(CanOpt(pPlayer))
		{
			if(NULL == pTxt)
				return eOR_Failed;
			else
			{
				CMessage msgTxt(MSG_S2C_GOODS_AllPurpose_UpdateTxt);
				DBWriteSet TxtdbWriteSet;
				msgTxt.GetDBWriteSet(TxtdbWriteSet);
				TxtdbWriteSet.AddToByteArray(pTxt);
				msgTxt.SendToPlayer(m_OwnerGuid, false);

				return eOR_Succeed;
			}
		}
	}

	return eOR_Lawless;
}

//! ˢ��������ʾ��Ǯ��
GoodsAllPurposeOpt::eOptResult GoodsAllPurposeOpt::SetGold(DWORD dwGoldNum)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(m_OwnerGuid);
	if(NULL != pPlayer)
	{
		if(CanOpt(pPlayer))
		{
			CMessage msgTxt(MSG_S2C_GOODS_AllPurpose_UpdateGold);
			DBWriteSet TxtdbWriteSet;
			msgTxt.GetDBWriteSet(TxtdbWriteSet);
			TxtdbWriteSet.AddToByteArray(dwGoldNum);
			msgTxt.SendToPlayer(m_OwnerGuid, false);

			return eOR_Succeed;
		}
	}

	return eOR_Lawless;
}

//! �ж���Ҳ����ĺϷ���
BOOL GoodsAllPurposeOpt::CanOpt(CPlayer *pPlayer)
{
	assert(NULL != pPlayer);

	if(m_RegionGuid != pPlayer->GetRegionGUID())
		return FALSE;

	if (abs(pPlayer->GetPosX() - m_lPosX) > m_lRadius || abs(pPlayer->GetPosY() - m_lPosY) > m_lRadius)
		return FALSE;

	return TRUE;
}