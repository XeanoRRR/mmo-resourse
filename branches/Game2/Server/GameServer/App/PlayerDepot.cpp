
#include "StdAfx.h"
#include "PlayerDepot.h"
#include "Player.h"
#include "../../../Public/Setup/GoodsSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PlayerDepot::PlayerDepot(CPlayer *pPlayer)
:
m_pPlayer(pPlayer),
m_bIsFree(false),
m_lInputPwdErrCount(0),
m_lFirstErrTime(0),
m_lThawDate(0),
m_uSessionRadius(0),
m_lSessionPosX(0),
m_lSessionPosY(0),
m_GoldDepot(GoodsSetup::GetGoldCoinIndex()),
m_GemDepot(GoodsSetup::GetSilverIndex())
{
	assert(NULL != m_pPlayer);
	m_Primary.SetContainerVolume(PRIMARY_SIZE);
	//m_Primary2.SetContainerVolume(PRIMARY2_SIZE);
	memset(m_szPassword, 0, DEPOT_PASSWORD_ARRAY_LEN + 1);
	memset(m_arrHlodSubpack, 0, DEPOT_SUBPACK_NUM * sizeof(bool));
}

PlayerDepot::~PlayerDepot(void)
{
	Release();
}


///////////////////////////////////////////////////////////////////////////////////////////////
//!							����
void PlayerDepot::CodeToDataBlock_ForClient(DBWriteSet& setWriteDB)
{
	//! -------------------------------------��Ӳֿ��е���Ʒ��Ϣ-------------------------------------

	//! ���ֿ�
	{
		setWriteDB.AddToByteArray(LONG(m_Primary.GetGoodsAmount()));
		CPacketListener cPacketListener;
		cPacketListener.m_pDBWriteData = &setWriteDB;
		m_Primary.TraversingContainer( &cPacketListener );
	}
	//! ������λ����
	BYTE btPackAmout=(BYTE)(m_Secondary.GetGoodsAmount());
	setWriteDB.AddToByteArray(btPackAmout);

	for(BYTE i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	{			
		if(m_Secondary.GetSubpack(i)->pGoods)
		{
			m_Secondary.GetSubpack(i)->pGoods->SerializeForOldClient(setWriteDB);
			setWriteDB.AddToByteArray(i);
		}
	}

	//! ������λ�ֿ�
	for(int i=0; i < DEPOT_SUBPACK_NUM; i++)
	{
		LONG lGoodsNum = m_Secondary.GetSubpack(i)->pSubpackContainer->GetGoodsAmount();
		setWriteDB.AddToByteArray(lGoodsNum);
		CPacketListener cPacketListener;
		cPacketListener.m_pDBWriteData = &setWriteDB;
		m_Secondary.GetSubpack(i)->pSubpackContainer->TraversingContainer( &cPacketListener );
	}

	//! ��Ҳֿ�
	setWriteDB.AddToByteArray( m_GoldDepot.GetGoldCoinsAmount() );
	CGoods* pGold = m_GoldDepot.GetGoods( 0 );
	if( pGold )
		setWriteDB.AddToByteArray( static_cast<CGUID>(pGold -> GetExID()) );
	else
		setWriteDB.AddToByteArray(CGUID::GUID_INVALID );

	//! ��ʯ�ֿ�
	setWriteDB.AddToByteArray( m_GemDepot.GetGoldCoinsAmount() );
	CGoods* pGem = m_GemDepot.GetGoods( 0 );
	if( pGem )
		setWriteDB.AddToByteArray( static_cast<CGUID>(pGem -> GetExID()) );
	else
		setWriteDB.AddToByteArray( CGUID::GUID_INVALID );
}
//!							����ֿ���Ϣͷ
void PlayerDepot::CodeHeadToDataBlock_ForClient(DBWriteSet& setWriteDB)
{
	//! -------------------------------------��Ӳֿ���Ϣ-------------------------------------
	tagPlayerDepotDataHead PlayerDepotDataHead;

	//! �Ƿ�������
	char szNullPwd[DEPOT_PASSWORD_ARRAY_LEN] = {0};
	PlayerDepotDataHead.lHasPassword = (0 == memcmp(m_szPassword, szNullPwd, DEPOT_PASSWORD_ARRAY_LEN)) ? 0 : 1;
	//! �ⶳʱ��
	PlayerDepotDataHead.lThawDate = m_lThawDate;

	//! ���õģ��ѹ���ģ��Ӳ�λ
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			++PlayerDepotDataHead.lUsableSubbackNum;
		else
			break;
	}
	//! ��λ�۸�
	memmove(PlayerDepotDataHead.arr_lSubbackPrice, c_DepotSetup.arr_lSubpackPrice, sizeof(long) * DEPOT_SUBPACK_NUM);

	setWriteDB.AddToByteArray(&PlayerDepotDataHead, sizeof(tagPlayerDepotDataHead));
}
//!							����
void PlayerDepot::CodeToDataBlock_ForServer(DBWriteSet& setWriteDB)
{
	//! ����
	setWriteDB.AddToByteArray( m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			lFlag |= (1 << i);
	}
	setWriteDB.AddToByteArray( lFlag);
	//! ����ʱ��
	setWriteDB.AddToByteArray( m_lThawDate);
	//! ��Ʒ����
	m_Primary.Serialize( setWriteDB, true);
	m_Secondary.Serialize( setWriteDB, true);
	m_GoldDepot.Serialize(setWriteDB,true);
	m_GemDepot.Serialize(setWriteDB,true);
}
//!							�ֿ����ñ���
void PlayerDepot::CodeSetupToDataBlock(DBWriteSet& setWriteDB)
{
	//! ����
	setWriteDB.AddToByteArray(m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			lFlag |= (1 << i);
	}
	setWriteDB.AddToByteArray(lFlag);
	//! ����ʱ��
	setWriteDB.AddToByteArray(m_lThawDate);
}
//!							����
void PlayerDepot::DecodeFromDataBlock_FromServer(DBReadSet& setReadDB)
{
#ifdef _RUNSTACKINFO1_
	char pszStackInfo[1024]="";
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"PlayerDepot::DecodeFromDataBlock_FromServer() Start");
#endif
	//! ����
	setReadDB.GetBufferFromByteArray(m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
#ifdef _RUNSTACKINFO1_
	_snprintf(pszStackInfo,1024,"Password:%s", m_szPassword);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif
	//! ��չ��λ��ʶ
	long lFlag =setReadDB.GetLongFromByteArray();
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = (0 != (lFlag & (1 << i)));
	}
	//! ����ʱ��
	m_lThawDate = setReadDB.GetLongFromByteArray();

#ifdef _RUNSTACKINFO1_
	_snprintf(pszStackInfo,1024,"lFlag:%d,m_lThawDate:%d", lFlag,m_lThawDate);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStackInfo);
#endif

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode Primary");
#endif
	//! ��Ʒ����
	m_Primary.Unserialize(setReadDB);
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode Secondary");
#endif
	m_Secondary.Unserialize(setReadDB);
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode GoldDepot");
#endif
	m_GoldDepot.Unserialize(setReadDB);
	m_GemDepot.Unserialize(setReadDB);
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"PlayerDepot::DecodeFromDataBlock_FromServer() End");
#endif
}

//! �󶨽�ɫ
void PlayerDepot::SetOwner(CPlayer *pPlayer)
{
	if (NULL != pPlayer)
	{
		m_Primary.SetOwner( pPlayer->GetType(), pPlayer->GetExID() );
		//m_Primary2.SetOwner( pPlayer->GetType(), pPlayer->GetExID() );
		m_Secondary.SetOwner( pPlayer->GetType(), pPlayer->GetExID() );
		{
			for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
			{
				CSubpackContainer::tagSubpack *pSubpack = m_Secondary.GetSubpack(i);
				if (NULL != pSubpack->pSubpackContainer)
				{
					pSubpack->pSubpackContainer->SetOwner(pPlayer->GetType(), pPlayer->GetExID());
				}
			}
		}
		m_GoldDepot.SetOwner( pPlayer->GetType(), pPlayer->GetExID() );
		m_GemDepot.SetOwner( pPlayer->GetType(), pPlayer->GetExID() );
	}
	
}
//! �ͷŶ���
void PlayerDepot::Release(void)
{
	m_pPlayer = NULL;
	m_Primary.Release();
	//m_Primary2.Release();
	m_Secondary.Release();
	m_GoldDepot.Release();
	m_GemDepot.Release();

}

///////////////////////////////////////////////////////////////////////////////////////////////
//! 	����
void PlayerDepot::Lock(void)
{
	m_bIsFree = false;
}

//! 	����
bool PlayerDepot::Unlock(const char *pPassword)
{
	if (NULL == m_pPlayer) return false;

	if (0 == strcmp(pPassword, m_szPassword))
	{	m_bIsFree = true;
		m_lFirstErrTime = 0;
	}
	else
	{
		++m_lInputPwdErrCount;
		if (1 == m_lInputPwdErrCount)
		{
			time(&m_lFirstErrTime);
		}
		else
		{
			if (c_DepotSetup.lMaxPwdErrCount < m_lInputPwdErrCount)
			{
				if (time(NULL) > m_lFirstErrTime)
				{
					frost();
				}
				else
				{
					time(&m_lFirstErrTime);
				}
			}
		}
	}

	return m_bIsFree;
}

//! 	�޸Ĳֿ�����
bool PlayerDepot::ChangePassword(const char *pOldPwd, const char *pNewPwd)
{
	if (IsFrost() || IsLock()) return false;

	if (0 == strcmp(pOldPwd, m_szPassword))
	{
		memmove(m_szPassword, pNewPwd, DEPOT_PASSWORD_ARRAY_LEN);
		return true;
	}
	return false;
}



//! ����һ��������λ�Ŀ�����رգ����ؿ�����Ҫ�Ľ�Ǯ
long PlayerDepot::SetSecondary(bool bOpen)
{
	if (IsFrost() || IsLock()) return false;

	if (bOpen)
	{
		for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
		{
			if (!m_arrHlodSubpack[i])
			{
				m_arrHlodSubpack[i] = true;
				return c_DepotSetup.arr_lSubpackPrice[i];
			}
		}
	}
	else
	{
		for (long i = DEPOT_SUBPACK_NUM - 1; i >= 0; --i)
		{
			if (m_arrHlodSubpack[i])
			{
				m_arrHlodSubpack[i] = false;
				return 0;
			}
		}
	}

	return -1;
}


//! �ҵ���������
CGoodsContainer* PlayerDepot::FindContainer(long eContainerId, bool bNotTest)
{
	if(!bNotTest)
		if ((IsFrost() || IsLock()) ) return NULL;
	switch(eContainerId)
	{
	case eDCT_Gold:
		return &m_GoldDepot;
	case eDCT_Silver:
		return &m_GemDepot;
	case eDCT_Primary:
		return &m_Primary;
	//case eDCT_Primary2:
	//	return &m_Primary2;
	case eDCT_Secondary:
		return &m_Secondary;
	case eDCT_Secondary1:
		if (m_arrHlodSubpack[0] || bNotTest)
			return m_Secondary.GetSubpack(0)->pSubpackContainer;
		break;
	case eDCT_Secondary2:
		if (m_arrHlodSubpack[1] || bNotTest)
			return m_Secondary.GetSubpack(1)->pSubpackContainer;
		break;
	case eDCT_Secondary3:
		if (m_arrHlodSubpack[2] || bNotTest)
			return m_Secondary.GetSubpack(2)->pSubpackContainer;
		break;
	case eDCT_Secondary4:
		if (m_arrHlodSubpack[3] || bNotTest)
			return m_Secondary.GetSubpack(3)->pSubpackContainer;
		break;
	}

	return NULL;
}

//!					�жϲֿ������Ƿ�����
BOOL PlayerDepot::IsFull (long eContainerId)
{
	if (IsFrost() || IsLock()) return TRUE;

	CGoodsContainer *pCGoodsContainer = NULL;
	switch(eContainerId)
	{
	case eDCT_Gold:
		pCGoodsContainer = &m_GoldDepot;
		break;
	case eDCT_Silver:
		pCGoodsContainer = &m_GemDepot;
		break;
	case eDCT_Primary:
		pCGoodsContainer = &m_Primary;
		break;
	//case eDCT_Primary2:
	//	pCGoodsContainer = &m_Primary2;
		break;
	case eDCT_Secondary:
		pCGoodsContainer = &m_Secondary;
		break;
	case eDCT_Secondary1:
		if (m_arrHlodSubpack[0])
			pCGoodsContainer = m_Secondary.GetSubpack(0)->pSubpackContainer;
		break;
	case eDCT_Secondary2:
		if (m_arrHlodSubpack[1])
			pCGoodsContainer = m_Secondary.GetSubpack(1)->pSubpackContainer;
		break;
	case eDCT_Secondary3:
		if (m_arrHlodSubpack[2])
			pCGoodsContainer = m_Secondary.GetSubpack(2)->pSubpackContainer;
		break;
	case eDCT_Secondary4:
		if (m_arrHlodSubpack[3])
			pCGoodsContainer = m_Secondary.GetSubpack(3)->pSubpackContainer;
		break;
	}

	if (NULL == pCGoodsContainer)
		return TRUE;
	return pCGoodsContainer->IsFull();
}


//! ��Ӧ����뿪
void PlayerDepot::OnPlayerLeave(void)
{
	Lock();
}


//! �����ֿ����Session
bool PlayerDepot::CreateSession(ULONG uRadius, long lPosX, long lPosY)
{
	if (IsFrost()) return false;
	m_uSessionRadius	= uRadius;
	m_lSessionPosX		= lPosX;
	m_lSessionPosY		= lPosY;
	if (0 == m_szPassword[0])
	{
		m_bIsFree = true;
	}

	if(m_bIsFree)
	{
		//! �ֿ�����
		CMessage msg(MSG_S2C_DEPOT_RE_INPUT_PWD);
		msg.Add((long)eDMC_Succeed);
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);
		CodeToDataBlock_ForClient(setWriteDB);

		if(NULL == m_pPlayer)
		{
			Log4c::Error(ROOT_MODULE,"%-18s %s",__FUNCTION__,"bind player is NULL");
			return false;
		}
		msg.SendToPlayer(m_pPlayer->GetExID());

	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//! ��Ӧ��������
void PlayerDepot::OnInputPwdRequest (CMessage *pMsg)
{
	if(IsFrost())return;
	if(!TestSession()) return;

	char szPassword[DEPOT_PASSWORD_ARRAY_LEN] = {0};
	pMsg->GetEx(szPassword, DEPOT_PASSWORD_SIZE);

	CMessage msg(MSG_S2C_DEPOT_RE_INPUT_PWD);
	if(Unlock(szPassword))
	{
		msg.Add((long)eDMC_Succeed);
		//! �ֿ�����
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);
		CodeToDataBlock_ForClient(setWriteDB);
	}
	else
		msg.Add((long)eDMC_Failed);

	msg.SendToPlayer(m_pPlayer->GetExID());
	return;
}
//! ��Ӧ�ֿ���������
void PlayerDepot::OnLockRequest(CMessage *pMsg)
{
	Lock();
	EndSession();
	CMessage msg(MSG_S2C_DEPOT_RE_CLOSE);
	msg.SendToPlayer(m_pPlayer->GetExID());

}
//! ��Ӧ�޸���������
void PlayerDepot::OnChangePwdRequest(CMessage *pMsg)
{
	if(IsFrost())return;
	if(!TestSession()) return;

	char szOldPassword[DEPOT_PASSWORD_ARRAY_LEN] = {0};
	pMsg->GetEx(szOldPassword, DEPOT_PASSWORD_SIZE);

	char szNewPassword[DEPOT_PASSWORD_ARRAY_LEN] = {0};
	pMsg->GetEx(szNewPassword, DEPOT_PASSWORD_SIZE);

	CMessage msg(MSG_S2C_DEPOT_RE_CHANGE_PWD);
	if(ChangePassword(szOldPassword, szNewPassword))
		msg.Add((long)eDMC_Succeed);
	else
		msg.Add((long)eDMC_Failed);

	msg.SendToPlayer(m_pPlayer->GetExID());
	return;
}
//! ��Ӧ�����ӱ�������
void PlayerDepot::OnBuySubpackRequest(CMessage *pMsg)
{
	if(IsFrost())return;
	if(!TestSession()) return;

	long lPrice = SetSecondary(true);
	if (0 < lPrice)
	{
		if( (long)m_pPlayer->GetMoney() >= lPrice)
		{
			m_pPlayer->SetMoney(m_pPlayer->GetMoney() - lPrice,9);
			CMessage msg(MSG_S2C_DEPOT_RE_BUY_SUBPACK);
			msg.Add((long)eDMC_Succeed);
			msg.SendToPlayer(m_pPlayer->GetExID());
			return;
		}
	}

	if (-1 != lPrice)
	{
		SetSecondary(false);
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////
tagDepotSetup PlayerDepot::c_DepotSetup;
//! �õ�����
tagDepotSetup* PlayerDepot::GetDepotSetup(void)
{
	return &c_DepotSetup;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//! ���ԻỰ����ʧ���������ֿ�
bool PlayerDepot::TestSession(void)
{
	if (0 != m_uSessionRadius)
	{
		long lCurrX = m_pPlayer->GetTileX();
		long lCurrY = m_pPlayer->GetTileY();

		//! �ж�����Ƿ��ڻỰ��Χ��
		long lDeltaX	= abs(lCurrX - m_lSessionPosX); //! X����
		long lDeltaY	= abs(lCurrY - m_lSessionPosY); //! Y����

		if (lDeltaX > (long)m_uSessionRadius || lDeltaY > (long)m_uSessionRadius)
		{
			m_uSessionRadius	= 0;
			m_lSessionPosX		= 0;
			m_lSessionPosY		= 0;
		}
		else
		{
			return true;
		}
	}

	Lock();
	return false;
}

//! ��������
bool PlayerDepot::IsLock(void)
{
	return !m_bIsFree;
}


//! ���Զ���
bool PlayerDepot::IsFrost(void)
{
	if(0 == m_lThawDate) return false;
	//! �������ⶳʱ�䣬��ⶳ
	if (time(NULL) > m_lThawDate)
	{
		m_lThawDate = 0;
		return false;
	}
	CMessage msg(MSG_S2C_DEPOT_FROSTED);
	msg.Add(m_lThawDate);
	msg.SendToPlayer(m_pPlayer->GetExID());
	return true;
}


void PlayerDepot::frost(void)
{
	Lock();
	EndSession();
	m_lThawDate = (long)time(NULL) + c_DepotSetup.lFrostTime;
	m_lFirstErrTime = 0;
}

//! �����Ự
void PlayerDepot::EndSession(void)
{
	m_uSessionRadius	= 0;
	m_lSessionPosX		= 0;
	m_lSessionPosY		= 0;
}