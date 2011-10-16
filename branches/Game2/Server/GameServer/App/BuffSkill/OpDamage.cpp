#include "StdAfx.h"
#include "opdamage.h"
#include "../MoveShape.h"
#include "../Player.h"
#include "../Monster.h"
#include "../MonsterTaunt/TauntEvents.h"
#include "../../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../../Public/ServerPublic/Server/Common/VarListUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COpDamage::COpDamage(stModuParam *pParam)
:COperator(pParam)
{
}
COpDamage::COpDamage(void* pParam)
:COperator((stModuParam*)pParam)
{

}
COpDamage::~COpDamage(void)
{
}

//ִ��
void COpDamage::Execute(CMoveShape* pShape)
{
	if(m_pFun)
	{
		if(m_strVariName != "")
		{
				//  [5/4/2009 chenxianj]		
				m_lValue = (long)m_pParam->pVariableList->GetVarValue<double>(m_strVariName.c_str(),0);			
				//  [5/4/2009 chenxianj]			
		}
        if (m_strDamParamname!="")
        {
                // @by:expter ���Ӽ��ܱ���
                m_lValue = pShape->GetSkillValue(m_pParam->nID,m_pParam->nLvl,m_strDamParamname);
        }
		if (m_lValue<0)
		{
			m_lValue = 0;
		}

#ifdef _SKILL_INFO_D_
        Log4c::Trace(SKILL_MODULE, "COpDamage:�����˺����� �˺�����:%d �˺�ֵ%d",m_lType,m_lValue);    
#endif
		(this->*m_pFun)(pShape);
	}
}

void COpDamage::AddDamage(CMoveShape* pShape)
{
	if(m_Obj==Obj_Dest)
		m_pParam->DestDamages[m_lType] += m_lValue;
	else if(m_Obj==Obj_Self)
		m_pParam->SelfDamages[m_lType] += m_lValue;
}

void COpDamage::SubDamge(CMoveShape* pShape)
{
	if(m_Obj==Obj_Dest)
		m_pParam->DestDamages[m_lType] -= m_lValue;
	else if(m_Obj==Obj_Self)
		m_pParam->SelfDamages[m_lType] -= m_lValue;
}

void COpDamage::MultiDamge(CMoveShape* pShape)
{
    if(m_Obj==Obj_Dest)
    {
        if( m_pParam->DestDamages[m_lType] <= 0 )
            m_pParam->DestDamages[m_lType] = 1;
        float  dfadio = m_lValue / lDamMulti;
        m_pParam->DestDamages[m_lType] = (long)(m_pParam->DestDamages[m_lType]*dfadio);
    }
    else if(m_Obj==Obj_Self)
    {
        if( m_pParam->SelfDamages[m_lType] <= 0 )
            m_pParam->SelfDamages[m_lType] = 1;
        float  dfadio = m_lValue / lDamMulti;
        m_pParam->DestDamages[m_lType] = (long)(m_pParam->DestDamages[m_lType]*dfadio);
    }
}

void COpDamage::SetDamge(CMoveShape* pShape)
{
	if(m_Obj==Obj_Dest)
		m_pParam->DestDamages[m_lType] = m_lValue;
	else if(m_Obj==Obj_Self)
		m_pParam->SelfDamages[m_lType] = m_lValue;
}

void COpDamage::ClearDamge(CMoveShape* pShape)
{
	if(m_Obj==Obj_Dest)
		m_pParam->DestDamages.clear();
	else if(m_Obj==Obj_Self)
		m_pParam->SelfDamages.clear();
}

void COpDamage::IncHP(CMoveShape* pShape)
{
	if (m_Obj==Obj_Dest)
	{
		m_pParam->DestIncHP[m_lType] += m_lValue;
	}
	else if (m_Obj==Obj_Self)
	{
		m_pParam->SelfIncHP[m_lType] += m_lValue;
	}
}

void COpDamage::MultiInc(CMoveShape* pShape)
{
    if (m_Obj==Obj_Dest)
    {
        if( m_pParam->DestIncHP[m_lType] <= 0 )
            m_pParam->DestIncHP[m_lType] = 1;
        float  dfadio = m_lValue / lDamMulti;
        m_pParam->DestDamages[m_lType] = (long)(m_pParam->DestDamages[m_lType]*dfadio);
    }
    else if (m_Obj==Obj_Self)
    {
        if( m_pParam->DestIncHP[m_lType] <= 0 )
            m_pParam->DestIncHP[m_lType] = 1;
        float  dfadio = m_lValue / lDamMulti;
        m_pParam->DestDamages[m_lType] = (long)(m_pParam->DestDamages[m_lType]*dfadio);
    }
}


////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
COpDoDamage::COpDoDamage(stModuParam *pParam)
:COperator(pParam)
{
}
COpDoDamage::COpDoDamage(void* pParam)
:COperator((stModuParam*)pParam)
{

}
COpDoDamage::~COpDoDamage(void)
{
}

//����Ŀ������
void COpDoDamage::DoDestDamage(CMoveShape* pShape)
{
	long lRet = 0;
	if(NULL==pShape || pShape->GetState()==CShape::STATE_DIED ||
		m_pParam->DestDamages.size() == 0 || m_pParam->pFirer==NULL)
	{
		m_pParam->DestDamages.clear();
		return;
	}
    
    //�����˺���Ϣ
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it != m_pParam->DestDamages.end();it++)
	{
		if((*it).first == DAMAGE_TYPE_HP && (m_lDamageReslut==DAMAGE_RESULT_Damage 
			|| m_nBlastAttack==1 
			|| m_lDamageReslut==DAMAGE_RESULT_FightBlock
			|| m_lDamageReslut==DAMAGE_RESULT_DamageEx
			|| m_lDamageReslut==DAMAGE_RESULT_DotaDamage))
		{
			lRet = pShape->OnBeenAttacked(m_pParam->pFirer,(*it).second,m_lDamageReslut);
			if (lRet)
			{
				//����
				break;
			}
		}
        else if((*it).first == DAMAGE_TYPE_TAUNT )
        {
            if( pShape->GetType() == TYPE_MONSTER )
            {
                CMonster *target = (CMonster*) pShape;
                TauntEvents events( target->GetTauntHandler() );
                events.Attacked( m_pParam->pFirer->GetExID(), m_pParam->pFirer->GetType(), (*it).second );
            }
        }
		else if((*it).first == DAMAGE_TYPE_MP)
		{			
		}
	}
	//����ֵ����
	//DoEnergy(pShape);
	if (m_nBlastAttack )
	{
		pShape->ProduceEvent(ET_BeenBlast);
		m_pParam->pUser->ProduceEvent(ET_Blast);
	}
	if (m_lDamageReslut==DAMAGE_RESULT_DamageEx)
	{
		pShape->ProduceEvent(ET_BeenDamageEx);
		m_pParam->pUser->ProduceEvent(ET_DamageEx);
	}
	else if (m_lDamageReslut==DAMAGE_RESULT_DotaDamage)
	{
		pShape->ProduceEvent(ET_BeSAttack);
		m_pParam->pUser->ProduceEvent(ET_SAttack);
	}

	hash_set<CGUID,hash_guid_compare> hsPlayers;

	if (pShape->GetType()==TYPE_PLAYER || pShape->GetType()==TYPE_PET)
	{
		CPlayer *pPlayer = (CPlayer*)pShape;
		if (pShape->GetType()==TYPE_PET)
		{
			pPlayer = (CPlayer*)(pShape->GetHost());
		}
		pPlayer->GetPlayerIDAndTeammateID(hsPlayers);
	}

	if (m_pParam->pUser->GetType()==TYPE_PLAYER || m_pParam->pUser->GetType()==TYPE_PET)
	{
		CPlayer *pPlayer = (CPlayer*)(m_pParam->pUser);		

		if (m_pParam->pUser->GetType()==TYPE_PET)
		{
			pPlayer = (CPlayer*)(m_pParam->pUser->GetHost());
		}
		
		pPlayer->GetPlayerIDAndTeammateID(hsPlayers);
	}
	if (m_pParam->pFirer->GetType()==TYPE_PLAYER || m_pParam->pFirer->GetType()==TYPE_PET)
	{
		CPlayer *pPlayer = (CPlayer*)(m_pParam->pFirer);		

		if (m_pParam->pFirer->GetType()==TYPE_PET)
		{
			pPlayer = (CPlayer*)(m_pParam->pFirer->GetHost());
		}

		pPlayer->GetPlayerIDAndTeammateID(hsPlayers);
	}

	if(m_lDamageReslut == DAMAGE_RESULT_Damage || m_lDamageReslut==DAMAGE_RESULT_DamageEx
											   || m_lDamageReslut==DAMAGE_RESULT_DotaDamage)
	{
		SendDestDamageMsg(pShape,hsPlayers);
	}
	else if(m_lDamageReslut == DAMAGE_RESULT_Dodge)
	{
		//���
		pShape->ProduceEvent(ET_Dodge);
		m_pParam->pUser->ProduceEvent(ET_BeenDodged);
		SendDestDodgeMsg(pShape,hsPlayers);			
	}
	else if(m_lDamageReslut == DAMAGE_RESULT_FightBlock)
	{
		//��
		pShape->ProduceEvent(ET_Block);
		m_pParam->pUser->ProduceEvent(ET_BeenBlocked);
		SendDestBlockMsg(pShape,hsPlayers);
	}
	else if(m_lDamageReslut == DAMAGE_RESULT_Parry)
	{
		//�м�
		pShape->ProduceEvent(ET_Parry);
		m_pParam->pUser->ProduceEvent(ET_BeenParried);
		SendDestParryMsg(pShape,hsPlayers);
	}
	else if(m_lDamageReslut == DAMAGE_RESULT_Resist)
	{
		//�ֿ�
		pShape->ProduceEvent(ET_Resist);
		m_pParam->pUser->ProduceEvent(ET_BeenResisted);
		SendDestResistMsg(pShape,hsPlayers);
	}
	else if(m_lDamageReslut == DAMAGE_RESULT_Suck)
		SendDestSuckMsg(pShape,hsPlayers);
	else if(m_lDamageReslut == DAMAGE_RESULT_Rebound)
		SendDestReboundMsg(pShape,hsPlayers);
	else if (m_lDamageReslut== DAMAGE_RESULT_Lose)
		SendSelfLoseMsg(hsPlayers);

	if (lRet == 1)
	{
		//��������	
		if (pShape->IsDied())
		{
			SendDiedMsg(pShape);
		}		
		pShape->OnBeenKilled(m_pParam->pFirer);	
		
	}
	if(m_pParam->DestDamages.size()>0)
		m_pParam->DestDamages.clear();
}

void COpDoDamage::DoDestIncHp(CMoveShape* pShape)
{
	if (m_pParam->DestIncHP.size()==0)
		return;
	if(NULL==pShape || pShape->GetState()==CShape::STATE_DIED ||
		m_pParam->pFirer==NULL)
		return;
	hash_set<CGUID,hash_guid_compare> hsPlayers;

	if (pShape->GetType()==TYPE_PLAYER)
	{
		CPlayer *pPlayer = (CPlayer*)pShape;
		pPlayer->GetPlayerIDAndTeammateID(hsPlayers);
	}

	if (m_pParam->pUser->GetType()==TYPE_PLAYER)
	{
		CPlayer *pPlayer = (CPlayer*)(m_pParam->pUser);
		pPlayer->GetPlayerIDAndTeammateID(hsPlayers);
	}


	stModuParam::itDamage it = m_pParam->DestIncHP.begin();
	for(;it != m_pParam->DestIncHP.end();it++)
	{
		if((*it).first == DAMAGE_TYPE_HP )
		{
			if (m_pParam->eType==ModuType_Skill_Active && m_pParam->eAddSubType==eAdd_Type)
			{
				//���漼���趨
				pShape->OnBeenAddAppendSkill(m_pParam->pUser);
				if (pShape->GetState()==1)
				{
					m_pParam->pUser->ChangeStateTo((CShape::eSTATE)pShape->GetState());
				}
			}

            if(pShape->OnIncreaseHp((*it).second, m_pParam->pFirer))
				SendDestIncHpMsg(pShape,hsPlayers);
			break;
		}
	}	
	m_pParam->DestIncHP.clear();
}

void COpDoDamage::DoSelfIncHp(CMoveShape* pShape)
{
	if (m_pParam->SelfIncHP.size()==0)
		return;
	hash_set<CGUID,hash_guid_compare> hsPlayers;

	
	if (m_pParam->pUser->GetType()==TYPE_PLAYER)
	{
		hsPlayers.insert(m_pParam->pUser->GetExID());
	}

	stModuParam::itDamage it = m_pParam->DestIncHP.begin();
	for(;it != m_pParam->DestIncHP.end();it++)
	{
		if((*it).first == DAMAGE_TYPE_HP )
		{
			if(pShape->OnIncreaseHp((*it).second, m_pParam->pFirer))
				SendDestIncHpMsg(pShape,hsPlayers);
			break;
		}
	}	
	m_pParam->DestIncHP.clear();

}

void COpDoDamage::DoSelfDamage(CMoveShape* pShape)
{
	if(m_pParam->SelfDamages.size() == 0)	return;

	stModuParam::itDamage it = m_pParam->SelfDamages.begin();	

	for(;it != m_pParam->SelfDamages.end();it++)
	{
		if((*it).first == DAMAGE_TYPE_HP )
		{
			m_pParam->pUser->OnBeenAttacked(pShape,(*it).second);
		}
		else if((*it).first == DAMAGE_TYPE_MP)
		{			
		}
	}
	SendSelfDamageMsg(m_pParam->pUser);
	m_pParam->SelfDamages.clear();
}

//��Ч��Ϣ
//void COpDoDamage::SendEffectMsg(CMoveShape* pShape)
//{
//	CMessage msg( MSG_S2C_SHAPE_ATK_DAMAGE );
//	msg.Add( pShape -> GetType() );
//	msg.Add( pShape -> GetExID() );
//	msg.Add(m_pParam->nID)
//	msg.Add(m_pParam->nLvl);
//	msg.Add(m_lDamageReslut);
//	msg.SendToAround(pShape);
//}

//��ͨ��Ѫ��Ϣ
void COpDoDamage::SendSelfDamageMsg(CMoveShape* pShape)
{
	hash_set<CGUID,hash_guid_compare> SendMsgPlayers;;
	if (pShape->GetType()==TYPE_PLAYER)
	{

	}
	CMessage msg( MSG_S2C_SHAPE_ATK_DAMAGE );
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );
	int nSize = (int)m_pParam->SelfDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->SelfDamages.begin();
	for(;it!=m_pParam->SelfDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).second);
	}
	msg.Add( m_pParam->pUser ->GetHP());	
	msg.SendToNearestPlayer(pShape);
}
//��ͨ��Ѫ��Ϣ
void COpDoDamage::SendDestDamageMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_BREAK );
	msg.Add( m_pParam->pFirer-> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = (int)m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).second);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_nBlastAttack);
	msg.Add((DWORD)m_pParam->nID);
	msg.Add((char)m_pParam->nLvl);	
	msg.SendToPlayers(hsPlayers,true);
	//msg.SendToNearestPlayer(pShape,true);
}

//��Ѫ
void COpDoDamage::SendDestIncHpMsg(CMoveShape *pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{

	CMessage msg(MSG_S2C_SHAPE_ATK_ADDBLOOD);	
	msg.Add( m_pParam->pFirer-> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );

	stModuParam::itDamage it = m_pParam->DestIncHP.begin();
	for(;it!=m_pParam->DestIncHP.end();it++)
	{		
		msg.Add((*it).second);
		break;
	}
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);


}
//����������Ϣ
void COpDoDamage::SendDestDodgeMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_FULLMISS );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	msg.Add( (char)m_nBlastAttack);	
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);
}


//��ʧ��Ϣ
void COpDoDamage::SendSelfLoseMsg(hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg(MSG_S2C_SHAPE_ATK_LOSE);
	msg.Add( m_pParam->pUser -> GetType() );
	msg.Add( m_pParam->pUser -> GetExID() );		
	//msg.SendToNearestPlayer(m_pParam->pUser);
	msg.SendToPlayers(hsPlayers,true);
}

//��������Ϣ
void COpDoDamage::SendDestBlockMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_BLOCK );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = (int)m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).second);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_nBlastAttack);	
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);
}

//�м���Ϣ
void COpDoDamage::SendDestParryMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_PARRY );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	msg.Add( (char)m_nBlastAttack);	
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);
}

//�ֿ���Ϣ
void COpDoDamage::SendDestResistMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_RESIST );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	msg.Add( (char)m_nBlastAttack);	
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);
}

//������Ϣ
void COpDoDamage::SendDestSuckMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_SUCK );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = (int)m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).second);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_nBlastAttack);
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);
}

//������Ϣ
void COpDoDamage::SendDestReboundMsg(CMoveShape* pShape,hash_set<CGUID,hash_guid_compare> &hsPlayers)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_REBOUND );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );
	int nSize = (int)m_pParam->DestDamages.size();
	msg.Add((char)nSize);
	stModuParam::itDamage it = m_pParam->DestDamages.begin();
	for(;it!=m_pParam->DestDamages.end();it++)
	{
		msg.Add((char)(*it).first);
		msg.Add((*it).second);
	}
	msg.Add( pShape ->GetHP());
	msg.Add( (char)m_nBlastAttack);
	//msg.SendToNearestPlayer(pShape);
	msg.SendToPlayers(hsPlayers,true);
}


//������Ϣ
void COpDoDamage::SendDiedMsg(CMoveShape* pShape)
{
	CMessage msg( MSG_S2C_SHAPE_ATK_DIED );
	msg.Add( m_pParam->pFirer -> GetType() );
	msg.Add( m_pParam->pFirer -> GetExID() );
	msg.Add( pShape -> GetType() );
	msg.Add( pShape -> GetExID() );

	msg.SendToAround(pShape);
}

//ִ��
void COpDoDamage::Execute(CMoveShape* pShape) 
{
	//  [5/4/2009 chenxianj]
	if(m_strDamaResVariName != "")
		m_lDamageReslut = (long)m_pParam->pVariableList->GetVarValue<double>(m_strDamaResVariName.c_str(),0);
	if(m_strBlasAttVariName != "")
		m_nBlastAttack = (long)m_pParam->pVariableList->GetVarValue<double>(m_strBlasAttVariName.c_str(),0);
	//  [5/4/2009 chenxianj]

	// by:expter ��ȡ��������
    if (m_strDamParamname!="")
    {
        m_lDamageReslut = pShape->GetSkillValue(m_pParam->nID,m_pParam->nLvl,m_strDamParamname);
    }
    if (m_strBlastParamname!="")
    {
        m_nBlastAttack = pShape->GetSkillValue(m_pParam->nID,m_pParam->nLvl,m_strBlastParamname);
    }

	DoDestDamage(pShape);
	DoSelfDamage(pShape);
	DoDestIncHp (pShape);
	DoSelfIncHp (pShape);

    
#ifdef _SKILL_DAMGENIUS_
    /// ��֤ѧϰĳ���츳�Ƿ���Ҫ����ĳ��״̬
    if( !pShape && m_pParam->SendGeniusList.find(pShape->GetExID()) != m_pParam->SendGeniusList.end())
        return ;
    m_pParam->pFirer->AddGeniusStates( m_pParam->nID , pShape , 0 );
    m_pParam->SendGeniusList.insert( pShape->GetExID() );
#endif

#ifdef _SKILL_INFO_D_  
    Log4c::Trace(SKILL_MODULE,"COpDoDamage:ʹ�ü��ܶ��츳״̬��֤\n");    
#endif
    
}