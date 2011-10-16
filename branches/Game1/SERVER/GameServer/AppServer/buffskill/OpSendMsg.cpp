#include "StdAfx.h"
#include "OpSendMsg.h"
#include "../Player.h"
#include "..\MoveShape.h"
#include "..\ServerRegion.h"
#include "..\..\..\..\public\SkillRelated.h"
#include "..\script\VariableList.h"


COpSendMsg::COpSendMsg(stModuParam *pParam)
:COperator(pParam)
{
}
COpSendMsg::~COpSendMsg(void)
{
}

//������Ʒ��Ϣ
void COpSendMsg::SendLockGoodsMsg()
{
	if (!m_pParam->pUser->IsDied())
	{
		CMessage msg(MSG_S2C_SKILL_LOCK_GOODS);
		msg.Add(m_pParam->lContainerID);
		msg.Add(m_pParam->lPos);
		msg.SendToPlayer(m_pParam->pUser->GetExID(),false);
	}

}

//���ͼ��ܿ�ʼ��Ϣ
void COpSendMsg::CreateSkillBeginMsg()
{
	//����������Ʒ��Ϣ
	if (m_pParam->lContainerID!=0)
	{
		SendLockGoodsMsg();	
	}
	if (m_pParam->pUser->GetState()==CShape::STATE_PEACE && m_pParam->eAddSubType==eSub_Type)
	{
		m_pParam->pUser->ChangeStateTo(CShape::STATE_FIGHT);
	}
	CMessage msg(MSG_S2C_SKILL_USE);
	msg.Add((CHAR)SKILL_USE_RESULT_BEGIN);
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( m_pParam->pUser->GetDirEx());
	//ȫ��id
	msg.Add(m_pParam->dwGlobleID);
	//id�͵ȼ�
	msg.Add((long)m_pParam->nID);
	msg.Add((BYTE)m_pParam->nLvl );
	//��ǰ����ִ�е��Ľ׶�
	msg.Add((BYTE)(m_pParam->nProceNum+1));
	//�׶κ�ʱ
	msg.Add((long)m_pParam->lUseTime);
	//��ȴʱ��
	msg.Add((long)m_pParam->lCoolDownTime);

	//�������λ��
	if (m_pParam->pMouseShape)
	{
		msg.Add((short)m_pParam->pMouseShape->GetTileX());
		msg.Add((short)m_pParam->pMouseShape->GetTileY());	
	}
	else
	{
		msg.Add((short)m_pParam->nMouseX);
		msg.Add((short)m_pParam->nMouseY);	

	}

	//�������Ŀ����󼯺�
	DWORD dwSize = m_pParam->OperObjs.size();
	msg.Add((BYTE)dwSize);
	itDestObj it = m_pParam->OperObjs.begin();
	for(;it != m_pParam->OperObjs.end();it++)
	{
		msg.Add((*it)->nDestType);
		msg.Add((*it)->DestID);
	}
	if(m_eRange == MsgRange_Single )
		msg.SendToPlayer(m_pParam->pUser -> GetExID(),false);
	else if(m_eRange == MsgRange_Around)
	{
		CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pParam->pUser->GetFather());
		if(pRegion)
		{
			pRegion->FindLimitPlayersAround(m_pParam->pUser,CGlobeSetup::GetSetup()->lSendSkillMsgLimitPlayerNum,m_pParam->SendMsgPlayers,NULL_GUID);
			GetGame()->RemoveTransferCongPlayerID(m_pParam->SendMsgPlayers);
		}
		msg.SendToPlayers(m_pParam->SendMsgPlayers);
	}
#ifdef _DEBUG
	char str[256]="";
	long lTime = timeGetTime();
	_snprintf(str, 256, "���ͼ��ܿ�ʼ��Ϣ:����ID:%6d,ȫ��ID:%6d,ʱ��%d,�׶κ�:%d\n",m_pParam->nID,m_pParam->dwGlobleID,timeGetTime(),m_pParam->nProceNum);
	OutputDebugString(str);
#endif

}
//���ͼ��ܽ�����Ϣ
void COpSendMsg::CreateSkillEndMsg()
{
	CMessage msg(MSG_S2C_SKILL_USE);
	msg.Add((CHAR)SKILL_USE_RESULT_END);
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( m_pParam->pUser->GetDirEx());
	//ȫ��id
	msg.Add(m_pParam->dwGlobleID);
	//id�͵ȼ�
	msg.Add((long)m_pParam->nID);
	msg.Add((BYTE)m_pParam->nLvl );
	//��ǰ����ִ�е��Ľ׶�
	msg.Add((BYTE)(m_pParam->nProceNum+1));

	if(m_eRange == MsgRange_Single)
		msg.SendToPlayer(m_pParam->pUser -> GetExID(),false);
	else if(m_eRange == MsgRange_Around)
		msg.SendToPlayers(m_pParam->SendMsgPlayers);
#ifdef _DEBUG
	char str[256]="";
	_snprintf(str, 256, "���ͼ��ܽ�����Ϣ:����ID:%6d,ȫ��ID:%6d,�׶κ�:%d\n",m_pParam->nID,m_pParam->dwGlobleID, m_pParam->nProceNum);
	OutputDebugString(str);
#endif
}
//���ͼ���ʧ����Ϣ
void COpSendMsg::CreateSKillFailMsg()
{
	CMoveShape* pUser = m_pParam->pUser;

	if(m_strVariaName !="")
	{
		//  [5/4/2009 chenxianj]	
		m_lValue = m_pParam->pVariableList->GetVarValue(m_strVariaName.c_str(),0);		
		//  [5/4/2009 chenxianj]
	}
	//����ʧ����������ʱ��
	if (pUser)
	{
		m_pParam->pUser->SetIntonateTime(timeGetTime(),0);
	}
	if (m_lValue==SKILL_USE_FAILED_INVALID_EQUIP)
	{
		CPlayer* pPlayer= dynamic_cast<CPlayer*>(pUser);
		if (pPlayer)
		{
			CGoods *pWeapon1 = pPlayer->GetEquipmentContainer()->GetGoods(CEquipmentContainer::EC_WEAPON);
			
			if (pWeapon1==NULL)
			{
			}
			else if (pWeapon1->HasAddonProperty(GAP_GOODS_MAXIMUM_DURABILITY)&&
					 pWeapon1->GetAddonPropertyValues(GAP_GOODS_MAXIMUM_DURABILITY,2)==0)
			{
				m_lValue=SKILL_USE_FAILED_WEAPON_DAMAGE;
			}
			CGoods *pWeapon2 = pPlayer->GetEquipmentContainer()->GetGoods(CEquipmentContainer::EC_WEAPON2); 
			if (pWeapon2==NULL)
			{
			}
			else if (pWeapon2->HasAddonProperty(GAP_GOODS_MAXIMUM_DURABILITY)&&
				pWeapon2->GetAddonPropertyValues(GAP_GOODS_MAXIMUM_DURABILITY,2)==0)
			{
				m_lValue=SKILL_USE_FAILED_WEAPON_DAMAGE;
			}
		}
	}
	CMessage msg(MSG_S2C_SKILL_USE);
	msg.Add((CHAR)SKILL_USE_RESULT_FAILED);
	msg.Add( pUser->GetExID());
	//id�͵ȼ�
	msg.Add((long)m_pParam->nID);
	msg.Add((BYTE)m_pParam->nLvl );
	//ȫ��id
	msg.Add(m_pParam->dwGlobleID);
	//��������
	msg.Add((BYTE)m_lValue);
	CMoveShape* pPlayer = pUser;
	if( TYPE_PET == pUser->GetType() )
	{
		pPlayer = pUser->GetHost();
	}

	if(m_eRange == MsgRange_Single)
		msg.SendToPlayer(pPlayer->GetExID(), false);
	else if(m_eRange == MsgRange_Around)
		msg.SendToPlayers(m_pParam->SendMsgPlayers);
#ifdef _DEBUG
	char str[256]="";
	_snprintf(str, 256, "���ͼ���ʧ����Ϣ:����ID:%6d,ȫ��ID:%6d,�׶κ�:%d\n",m_pParam->nID,m_pParam->dwGlobleID, m_pParam->nProceNum);
	OutputDebugString(str);
#endif

}

//����buff��ʼ��Ϣ
void COpSendMsg::CreateBuffBeginMsg()
{
	//����ʣ��ʱ��
	long lRemainTime = 0;
	stModuParam::itTimer it = m_pParam->Timers.begin();
	if(it != m_pParam->Timers.end())
	{
		stTimerParam *pTimerParam = (*it);
		lRemainTime += pTimerParam->lFuture+pTimerParam->lInter*pTimerParam->nCount;
	}

	CMessage msg(MSG_S2C_SKILL_ADD_EX_STATE);

	//ʹ����type��ID
	msg.Add(m_pParam->pUser->GetType());
	msg.Add(m_pParam->pUser->GetExID());

	//buff�ĵȼ���ID
	msg.Add((DWORD)m_pParam->nID);
	msg.Add((DWORD)m_pParam->nLvl);
	msg.Add(lRemainTime);
	msg.Add(m_bUpdate);

	if(m_eRange == MsgRange_Single)
		msg.SendToPlayer(m_pParam->pUser -> GetExID(),false);
	else if(m_eRange == MsgRange_Around)
		msg.SendToAround(m_pParam->pUser);
}



//����buff������Ϣ
void COpSendMsg::CreateBuffEndMsg()
{
	CMessage msg( MSG_S2C_SKILL_END_EX_STATE );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add((DWORD)m_pParam->nID);
	msg.Add((DWORD)m_pParam->nLvl);
	if(m_eRange == MsgRange_Single)
		msg.SendToPlayer(m_pParam->pUser -> GetExID(),false);
	else if(m_eRange == MsgRange_Around)
		msg.SendToAround(m_pParam->pUser);
}

//ִ��
void COpSendMsg::Execute(CMoveShape* pShape)
{
	if(m_eRange == MsgRange_Single && m_pParam->pUser->GetType() != TYPE_PLAYER && m_pParam->pUser->GetType() != TYPE_PET)
		return;
	if(m_pCreateMsg)
		(this->*m_pCreateMsg)();
}


COpSendDamageMsg::COpSendDamageMsg(stModuParam *pParam)
:COperator(pParam)
{
}

COpSendDamageMsg::~COpSendDamageMsg(void)
{
}

//��ͨ��Ѫ��Ϣ
void COpSendDamageMsg::CreateSelfDamageMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_DAMAGE );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	int nSize = m_pParam->SelfDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->SelfDamages.begin();
	for(;it!=m_pParam->SelfDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).first);
	}
	msg.Add( m_pParam->pUser ->GetHP());
	msg.SendToNearestPlayer(m_pParam->pUser);

}
//��ͨ��Ѫ��Ϣ
void COpSendDamageMsg::CreateDestDamageMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_BREAK );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).first);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_lBlastAttack);
	msg.Add((DWORD)m_pParam->nID);
	msg.Add((char)m_pParam->nLvl);
	msg.SendToNearestPlayer(pShape,true);
}
//����������Ϣ
void COpSendDamageMsg::CreateDestDodgeMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_FULLMISS );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	msg.Add( (char)m_lBlastAttack);
	msg.SendToNearestPlayer(pShape);
}

//��������Ϣ
void COpSendDamageMsg::CreateDestBlockMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_BLOCK );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).first);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_lBlastAttack);
	msg.SendToNearestPlayer(pShape);
}

//�м���Ϣ
void COpSendDamageMsg::CreateDestParryMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_PARRY );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	msg.Add( (char)m_lBlastAttack);
	msg.SendToNearestPlayer(pShape);
}

//�ֿ���Ϣ
void COpSendDamageMsg::CreateDestResistMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_RESIST );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	msg.Add( (char)m_lBlastAttack);
	msg.SendToNearestPlayer(pShape);
}

//������Ϣ
void COpSendDamageMsg::CreateDestSuckMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_SUCK );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).first);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_lBlastAttack);
	msg.SendToNearestPlayer(pShape);
}

//������Ϣ
void COpSendDamageMsg::CreateDestReboundMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_REBOUND );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).first);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_lBlastAttack);
	msg.SendToNearestPlayer(pShape);
}

//ִ��
void COpSendDamageMsg::Execute(CMoveShape* pShape)
{
	if(m_strBlastVariaName !="")
	{
		//  [5/4/2009 chenxianj]		
		m_lBlastAttack = m_pParam->pVariableList->GetVarValue(m_strBlastVariaName.c_str(),0);
		//  [5/4/2009 chenxianj]		
	}
	if(m_pCreateMsg)
		m_pCreateMsg(pShape);
}
