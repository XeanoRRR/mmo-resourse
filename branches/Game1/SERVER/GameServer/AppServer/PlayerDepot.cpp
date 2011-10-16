
#include "StdAfx.h"
#include "PlayerDepot.h"
#include "Player.h"


PlayerDepot::PlayerDepot(CPlayer *pPlayer)
:
m_pPlayer(pPlayer),
m_uSessionRadius(0),
m_lSessionPosX(0),
m_lSessionPosY(0)
{
	assert(NULL != m_pPlayer);
	m_Primary.SetContainerVolume(PRIMARY_SIZE);
	//m_Primary2.SetContainerVolume(PRIMARY2_SIZE);
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
	setWriteDB.AddToByteArray( m_GemDepot.GetSilverCoinsAmount() );
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
	char szNullPwd[PASSWORD_SIZE] = {0};
	PlayerDepotDataHead.lHasPassword = (0 == memcmp(m_Pwd.GetPwd(), szNullPwd, PASSWORD_SIZE)) ? 0 : 1;
	//! �ⶳʱ��
	PlayerDepotDataHead.lThawDate = m_Pwd.GetThawTime();

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
	setWriteDB.AddToByteArray((void*)m_Pwd.GetPwd(), PASSWORD_SIZE);
	//! ��չ��λ��ʶ
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			lFlag |= (1 << i);
	}
	setWriteDB.AddToByteArray(lFlag);
	//! ����ʱ��
	setWriteDB.AddToByteArray(m_Pwd.GetThawTime());
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
	setWriteDB.AddToByteArray((void*)m_Pwd.GetPwd(), PASSWORD_SIZE);
	//! ��չ��λ��ʶ
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			lFlag |= (1 << i);
	}
	setWriteDB.AddToByteArray(lFlag);
	//! ����ʱ��
	setWriteDB.AddToByteArray(m_Pwd.GetThawTime());
}
//!							����
void PlayerDepot::DecodeFromDataBlock_FromServer(DBReadSet& setReadDB)
{
#ifdef _RUNSTACKINFO1_
	char pszStackInfo[1024]="";
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"PlayerDepot::DecodeFromDataBlock_FromServer() Start");
#endif
	//! ����
	char szPassword[PASSWORD_SIZE] = {0};
	setReadDB.GetBufferFromByteArray(szPassword, PASSWORD_SIZE);
	m_Pwd.Init(c_DepotSetup.lMaxPwdErrCount, c_DepotSetup.lFrostTime, szPassword);

	//! ��չ��λ��ʶ
	long lFlag =setReadDB.GetLongFromByteArray();
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = (0 != (lFlag & (1 << i)));
	}
	//! ����ʱ��
	m_Pwd.SetThawTime(setReadDB.GetLongFromByteArray());

	//! ��Ʒ����
	m_Primary.Unserialize(setReadDB);
	m_Secondary.Unserialize(setReadDB);
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

void PlayerDepot::CheckGuardianGoods(std::vector<CGoods*>&GoodsList)
{
	//���ֿ��ӱ�������˥����Ʒ
	static DWORD arrContainerType[] = 
	{
		eDCT_Primary,
		eDCT_Secondary1,
		eDCT_Secondary2,
		eDCT_Secondary3,
		eDCT_Secondary4
	};

	for (LONG i = 0; i < sizeof(arrContainerType) / sizeof(DWORD); ++i)
	{
		CVolumeLimitGoodsContainer *pContainer = (CVolumeLimitGoodsContainer*)FindContainer(arrContainerType[i],true);
		if(NULL != pContainer)
		{
			for (long j = 0; j < pContainer->GetVolume(); ++j)
			{
				CGoods *pTmpGoods = pContainer->GetGoods(j);
				if (NULL != pTmpGoods)
				{
					//! ��������ʱ��
					if(m_pPlayer->TestGoodsTimeout(pTmpGoods))
					{
						//! ɾ����Ʒ
						pair<long, long> GoodsPosInfo;
						if(m_pPlayer->FindGoodsByGuid(pTmpGoods->GetExID(), GoodsPosInfo))
						{
							m_pPlayer->DelGoodsAndSendMsg(GoodsPosInfo.first, GoodsPosInfo.second);
						}
					}
					else
					{
						GoodsList.push_back( pTmpGoods );
					}
				}
			}
		}
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


//! ����һ��������λ�Ŀ�����رգ����ؿ�����Ҫ�Ľ�Ǯ
long PlayerDepot::SetSecondary(bool bOpen)
{
	if(!IsEnable()) return false;

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
		if(!IsEnable()) return NULL;
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
	if(!IsEnable()) return TRUE;

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
	m_Pwd.Lock();
}


//! �����ֿ����Session
bool PlayerDepot::CreateSession(ULONG uRadius, long lPosX, long lPosY)
{
	if(CPlayerPwd::ePS_Frost == m_Pwd.GetState())
	{
		CMessage msg(MSG_S2C_DEPOT_FROSTED);
		msg.Add(m_Pwd.GetThawTime());
		msg.SendToPlayer(m_pPlayer->GetExID());
		return false;
	}

	m_uSessionRadius	= uRadius;
	m_lSessionPosX		= lPosX;
	m_lSessionPosY		= lPosY;

	//! �ֿ�����
	if (CPlayerPwd::ePS_Free == m_Pwd.GetState() )
	{
		CMessage msg(MSG_S2C_DEPOT_RE_INPUT_PWD);
		msg.Add((long)eDMC_Succeed);
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);
		CodeToDataBlock_ForClient(setWriteDB);

		if(NULL == m_pPlayer)
		{
			AddErrorLogText("bind player is NULL");
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
	if(!TestSession()) return;

	if(m_pPlayer->GetSortSontainr())
	{
		//�������в��ܴ򿪲ֿ�
		return;
	}

	char szPassword[PASSWORD_SIZE] = {0};
	pMsg->GetEx(szPassword, PASSWORD_SIZE);

	CMessage msg(MSG_S2C_DEPOT_RE_INPUT_PWD);
	if(CPlayerPwd::ePS_Free == m_Pwd.Unlock(szPassword))
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
	m_Pwd.Lock();
	EndSession();
	CMessage msg(MSG_S2C_DEPOT_RE_CLOSE);
	msg.SendToPlayer(m_pPlayer->GetExID());

}
//! ��Ӧ�޸���������
void PlayerDepot::OnChangePwdRequest(CMessage *pMsg)
{
	if(!IsEnable()) return;
	if(!TestSession()) return;

	char szOldPassword[PASSWORD_SIZE] = {0};
	pMsg->GetEx(szOldPassword, PASSWORD_SIZE);
	szOldPassword[PASSWORD_SIZE - 1] = 0;

	char szNewPassword[PASSWORD_SIZE] = {0};
	pMsg->GetEx(szNewPassword, PASSWORD_SIZE);
	szNewPassword[PASSWORD_SIZE - 1] = 0;

	CMessage msg(MSG_S2C_DEPOT_RE_CHANGE_PWD);
	if(CPlayerPwd::ePS_Free == m_Pwd.ChangePassword(szOldPassword, szNewPassword))
		msg.Add((long)eDMC_Succeed);
	else
		msg.Add((long)eDMC_Failed);

	msg.SendToPlayer(m_pPlayer->GetExID());
	return;
}
//! ��Ӧ�����ӱ�������
void PlayerDepot::OnBuySubpackRequest(CMessage *pMsg)
{
	if(!IsEnable()) return;
	if(!TestSession()) return;

	long lPrice = SetSecondary(true);
	if (0 < lPrice)
	{
		if(m_pPlayer->GetMoney() >= lPrice)
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

		if (lDeltaX > m_uSessionRadius || lDeltaY > m_uSessionRadius)
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

	m_Pwd.Lock();
	return false;
}




//! ���Բֿ��Ƿ����
bool PlayerDepot::IsEnable(void)
{
	if(CPlayerPwd::ePS_Frost == m_Pwd.GetState())
	{
		CMessage msg(MSG_S2C_DEPOT_FROSTED);
		msg.Add(m_Pwd.GetThawTime());
		msg.SendToPlayer(m_pPlayer->GetExID());
	}
	
	return (CPlayerPwd::ePS_Free == m_Pwd.GetState());
}

//! �����Ự
void PlayerDepot::EndSession(void)
{
	m_uSessionRadius	= 0;
	m_lSessionPosX		= 0;
	m_lSessionPosY		= 0;
}