
#include "stdafx.h"
#include "SpriteSystem.h"


SpriteSystem::SpriteSystem(void)
{

}

SpriteSystem::~SpriteSystem(void)
{

}

map<LONG, DWORD> SpriteSystem::c_mapCanUseGoodsIndex;
vector<LONG> SpriteSystem::c_vectorCanUseFightSkill;
vector<LONG> SpriteSystem::c_vectorCanUseAssistSkill;
vector<LONG> SpriteSystem::c_vectorCanUseAssistGoods;

//! ���ñ���
void SpriteSystem::SetupAddToByteArray(vector<BYTE> &ByteArray)
{
	_AddToByteArray(&ByteArray, (LONG)c_mapCanUseGoodsIndex.size());
	for (map<LONG, DWORD>::iterator ite = c_mapCanUseGoodsIndex.begin(); ite != c_mapCanUseGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(&ByteArray, ite->first);
		_AddToByteArray(&ByteArray, ite->second);
	}
    //ս������
    _AddToByteArray(&ByteArray, (LONG)c_vectorCanUseFightSkill.size());
    for (vector<LONG>::iterator ite = c_vectorCanUseFightSkill.begin(); ite != c_vectorCanUseFightSkill.end(); ++ite)
    {
        _AddToByteArray(&ByteArray, *ite);
    }
    //��������
    _AddToByteArray(&ByteArray, (LONG)c_vectorCanUseAssistSkill.size());
    for (vector<LONG>::iterator ite = c_vectorCanUseAssistSkill.begin(); ite != c_vectorCanUseAssistSkill.end(); ++ite)
    {
        _AddToByteArray(&ByteArray, *ite);
    }
    //��������
    _AddToByteArray(&ByteArray, (LONG)c_vectorCanUseAssistGoods.size());
    for (vector<LONG>::iterator ite = c_vectorCanUseAssistGoods.begin(); ite != c_vectorCanUseAssistGoods.end(); ++ite)
    {
        _AddToByteArray(&ByteArray, *ite);
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
    LONG lFightSkillNum = _GetLongFromByteArray(pByteArray, lPos);
    for (LONG l = 0; l < lFightSkillNum; ++l)
    {
        LONG lID        = _GetLongFromByteArray(pByteArray, lPos);
        c_vectorCanUseFightSkill.push_back(lID);
    }
    LONG lAssistSkillNum = _GetLongFromByteArray(pByteArray, lPos);
    for (LONG l = 0; l < lAssistSkillNum; ++l)
    {
        LONG lID        = _GetLongFromByteArray(pByteArray, lPos);
        c_vectorCanUseAssistSkill.push_back(lID);
    }
    LONG lAssistGoodsNum = _GetLongFromByteArray(pByteArray, lPos);
    for (LONG l = 0; l < lAssistGoodsNum; ++l)
    {
        LONG lID        = _GetLongFromByteArray(pByteArray, lPos);
        c_vectorCanUseAssistGoods.push_back(lID);
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

//! ����һ�����ܺ͸��������ܷ�ʹ��
BOOL SpriteSystem::TestAutoFightSkill(LONG lSkillID)
{
    vector<LONG>::iterator ite = c_vectorCanUseFightSkill.begin();
    for (; ite != c_vectorCanUseFightSkill.end(); ++ite)
    {
        if ((*ite) == lSkillID)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//!  ����һ��ս�����������ܷ�ʹ��
BOOL SpriteSystem::TestAutoAssistSkill(LONG lSkillID)
{
    vector<LONG>::iterator ite = c_vectorCanUseAssistSkill.begin();
    for (; ite != c_vectorCanUseAssistSkill.end(); ++ite)
    {
        if ( (*ite) == lSkillID)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//!   ����һ�����������ܷ�ʹ��
BOOL SpriteSystem::TestAutoAssistGoods(LONG lGoodsID)
{
    vector<LONG>::iterator ite = c_vectorCanUseAssistGoods.begin();
    for (; ite != c_vectorCanUseAssistGoods.end(); ++ite)
    {
        if ( (*ite) == lGoodsID)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//! ����������
void SpriteSystem::SetOwner(CPlayer *pMainPlayer)
{
	m_AutoSupply.SetOwner(pMainPlayer);
    m_AutoFight.SetOwner(pMainPlayer);
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
    //setWriteDB.AddToByteArray(m_AutoFight.GetStableTime());
    //�����ȶ�ʱ��ֵ����WS
    m_AutoFight.CodeToDataBlock(setWriteDB);
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
    //m_AutoFight.SetStableTime(setReadDB.GetLongFromByteArray());
    //��WS����ȶ�ʱ��
    m_AutoFight.DecodeFromDataBlock(setReadDB);
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

//! ��Ӧ�Զ��������
void SpriteSystem::OnChangeAutoFight(CMessage *pMsg)
{
    LONG eFightType = pMsg->GetLong();

    tagAutoFightNode AutoFightNode;
    pMsg->GetEx(&AutoFightNode, sizeof(tagAutoFightNode));

    LONG bReValue = eASET_NotErr;
    switch((eAutoHookType)eFightType)
    {
    case eAHT_COMMON:
    case eAHT_STABLE:
    case eAHT_REPAIR:
        {
            //�������߹رո����һ�
            bReValue = m_AutoFight.ChangeSetup(eFightType, AutoFightNode);

            CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
            if (bReValue == eASET_NotErr)
            {
                msg.Add( (LONG)bReValue);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            else
            {
                msg.Add((LONG)bReValue);
                msg.Add(eFightType);
            }
            msg.SendToPlayer(pMsg->GetPlayerID(), false);
        }
        break;
    case eAHT_FIGHTSKILL:
        {
            //����Զ�ս�������ܷ����
            bReValue = TestAutoFightSkill(AutoFightNode.lFightTypeIndex);

            CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
            if (bReValue)
            {
                msg.Add((LONG)eASET_CanUse);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            else
            {
                msg.Add((LONG)eASET_NoCan);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            msg.SendToPlayer(pMsg->GetPlayerID(), false);
        }
        break;
    case eAHT_ASSISTSKILL:
        {
            //����Զ����������ܷ����
            bReValue = TestAutoAssistSkill(AutoFightNode.lFightTypeIndex);

            CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
            if (bReValue)
            {
                msg.Add((LONG)eASET_CanUse);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            else
            {
                msg.Add((LONG)eASET_NoCan);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            msg.SendToPlayer(pMsg->GetPlayerID(),false);
        }
        break;
    case eAHT_ASSISTGOODS:
        {
            //����Զ����������ܷ����
            bReValue = TestAutoAssistGoods(AutoFightNode.lFightTypeIndex);

            CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
            if (bReValue)
            {
                msg.Add((LONG)eASET_CanUse);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            else
            {
                msg.Add((LONG)eASET_NoCan);
                msg.Add(eFightType);
                msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
            }
            msg.SendToPlayer(pMsg->GetPlayerID(), false);
        }
        break;
    default:
        break;
    }
}

//! �ж��Զ�����װ��������
bool SpriteSystem::CanAutoRepair(void)
{
   const tagAutoFightNode* pAutoFightNode;

   pAutoFightNode = m_AutoFight.GetOneSetup((LONG)eAHT_REPAIR);
    if (pAutoFightNode->bOpen)
    {
        //���Ҳ����ж�
        if (m_AutoFight.CanAutoRepair())
            return true;
        else
            return false;
    }
    return false;
}