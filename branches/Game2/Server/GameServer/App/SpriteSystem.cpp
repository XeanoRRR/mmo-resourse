
#include "StdAfx.h"
#include "SpriteSystem.h"


SpriteSystem::SpriteSystem(void)
{

}

SpriteSystem::~SpriteSystem(void)
{

}

map<LONG, DWORD> SpriteSystem::c_mapCanUseGoodsIndex;

//! ���ñ���
void SpriteSystem::SetupAddToByteArray(vector<BYTE> &ByteArray)
{
	_AddToByteArray(&ByteArray, (LONG)c_mapCanUseGoodsIndex.size());
	for (map<LONG, DWORD>::iterator ite = c_mapCanUseGoodsIndex.begin(); ite != c_mapCanUseGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(&ByteArray, ite->first);
		_AddToByteArray(&ByteArray, ite->second);
	}
}

//! ���ý���
void SpriteSystem::SetupGetFormByteArray(BYTE *pByteArray, long &lPos)
{
	LONG lGoodsNum = _GetLongFromByteArray(pByteArray, lPos);
	for (LONG i = 0; i < lGoodsNum; ++i)
	{
		LONG lIndex		= _GetLongFromByteArray(pByteArray, lPos);
		DWORD dwBound	= _GetLongFromByteArray(pByteArray, lPos); 
		c_mapCanUseGoodsIndex[lIndex] = dwBound;
	}
}

//!							���ñ���
void SpriteSystem::CodeSetupToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray( (LONG)c_mapCanUseGoodsIndex.size());
	for (map<LONG, DWORD>::iterator ite = c_mapCanUseGoodsIndex.begin(); ite != c_mapCanUseGoodsIndex.end(); ++ite)
	{
		setWriteDB.AddToByteArray( ite->first);
		setWriteDB.AddToByteArray( ite->second);
	}
}
	//!							���ý���
void SpriteSystem::DecodeSetupFromDataBlock(DBReadSet& setReadDB)
{
	LONG lGoodsNum = setReadDB.GetLongFromByteArray();
	for (LONG i = 0; i < lGoodsNum; ++i)
	{
		LONG lIndex		= setReadDB.GetLongFromByteArray();
		DWORD dwBound	= setReadDB.GetLongFromByteArray(); 
		c_mapCanUseGoodsIndex[lIndex] = dwBound;
	}
}

//! ����һ����Ʒ�ܷ������Զ�����
BOOL SpriteSystem::TestAutoSupplyGoods(LONG lGoodsIdx)
{
	return (c_mapCanUseGoodsIndex.end() != c_mapCanUseGoodsIndex.find(lGoodsIdx));
}


//! ����������
void SpriteSystem::SetOwner(CPlayer *pMainPlayer)
{
	m_AutoSupply.SetOwner(pMainPlayer);
}

//! ����
void SpriteSystem::AddToByteArray(vector<BYTE> &ByteArray)
{
	m_AutoSupply.AddToByteArray(ByteArray);
}

//! ����
void SpriteSystem::GetFormByteArray(BYTE *pByteArray, long &lPos)
{
	//! ----------------�Զ�����----------------
	m_AutoSupply.GetFormByteArray(pByteArray, lPos);
	//! ������������������֮��ĳ�ͻ
	const tagAotoSupplyNode *pAotoSupplyNode = NULL;
	for (LONG i = 0; i < AotoSupplyTypeNum; ++i)
	{
		pAotoSupplyNode = m_AutoSupply.GetOneSetup(i);
		if (TestAutoSupplyGoods(pAotoSupplyNode->lGoodsIndex))
		{
			//! �������ýӿ�ChangeSetup���ܱ�֤���ݸı��Ǳ����£�����������˶���Ķ��󿽱�
			tagAotoSupplyNode AotoSupplyNode;
			memmove(&AotoSupplyNode, pAotoSupplyNode, sizeof(tagAotoSupplyNode));
			m_AutoSupply.ChangeSetup(i, AotoSupplyNode, TRUE);
		}
	}
	
	//! ----------------   ----------------
}

//!							����
void SpriteSystem::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	m_AutoSupply.CodeToDataBlock(setWriteDB);
}
//!							����
void SpriteSystem::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	//! ----------------�Զ�����----------------
	m_AutoSupply.DecodeFromDataBlock(setReadDB);
	//! ������������������֮��ĳ�ͻ
	const tagAotoSupplyNode *pAotoSupplyNode = NULL;
	for (LONG i = 0; i < AotoSupplyTypeNum; ++i)
	{
		pAotoSupplyNode = m_AutoSupply.GetOneSetup(i);
		if (TestAutoSupplyGoods(pAotoSupplyNode->lGoodsIndex))
		{
			//! �������ýӿ�ChangeSetup���ܱ�֤���ݸı��Ǳ����£�����������˶���Ķ��󿽱�
			tagAotoSupplyNode AotoSupplyNode;
			memmove(&AotoSupplyNode, pAotoSupplyNode, sizeof(tagAotoSupplyNode));
			m_AutoSupply.ChangeSetup(i, AotoSupplyNode, TRUE);
		}
	}
}


//! ��Ӧ�Զ����������޸�
void SpriteSystem::OnChangeAutoSupply(CMessage *pMsg)
{
	LONG eSupplyType = pMsg->GetLong();
	tagAotoSupplyNode AotoSupplyNode;
	pMsg->GetEx(&AotoSupplyNode, sizeof(tagAotoSupplyNode));

	BOOL bReValue = eASET_NotErr;
	if(TestAutoSupplyGoods(AotoSupplyNode.lGoodsIndex) || 0 == AotoSupplyNode.lGoodsIndex)
	{
		bReValue = m_AutoSupply.ChangeSetup(eSupplyType, AotoSupplyNode);
	}
	else
	{
		bReValue = eASET_ValueErr;
	}

	CMessage msg(MSG_S2C_SPRITE_AutoSupply_ChangeSetup_Re);
	if(bReValue)
	{
		msg.Add((LONG)bReValue);
		msg.Add(eSupplyType);
		msg.AddEx(&AotoSupplyNode, sizeof(tagAotoSupplyNode));
	}
	else
	{
		msg.Add((LONG)bReValue);
		msg.Add(eSupplyType);
		//msg.Add(lReValue);
	}

	msg.SendToPlayer(pMsg->GetPlayerID(), false);
}