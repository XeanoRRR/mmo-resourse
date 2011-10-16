//////////////////////////////////////////////////////////////////////////
//PKsys.cpp
//Fun:PKϵͳ�����ֹ�����ɱ�ˡ���������
//Author:Joe
//Create Time:2005.6.24
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\pksys.h"
#include "Player.h"
#include "Monster.h"
#include "ServerRegion.h"
#include "../setup/GlobeSetup.h"
#include "../setup/LogSystem.h"
#include "appserver\country\countryHandler.h"
#include "appserver\country\country.h"

//class CCountry;
CPKSys* CPKSys::pInstance = NULL;
CPKSys::CPKSys(void)
{
}

CPKSys::~CPKSys(void)
{
}

CPKSys* CPKSys::getInstance()
{
	if(pInstance == NULL)
	{
		pInstance = MP_NEW CPKSys;
	}
	return pInstance;
}

void CPKSys::Release()
{
	if(pInstance)
	{
		MP_DELETE(pInstance);
	}
}

void CPKSys::SetPKValue(CMoveShape *pAttacker,CMoveShape *pBeenAttacker,int nAddSub)
{
	//��������������ʹ�ü��漼�ܣ�����������ң�ÿ�ι���������20��PKֵ����˷�����õ�PKֵ����Ϊ20����PKֵ�Ѿ��ﵽ�򳬹�20���򲻻�����˷��������ӡ�	

	if (pAttacker==NULL || pBeenAttacker==NULL)
	{
		return;
	}
	if (pAttacker->GetType()==TYPE_MONSTER)
	{
		return;		
	}
	if (pBeenAttacker->GetType()==TYPE_MONSTER)
	{
		if (((CMonster*)pBeenAttacker)->GetNpcType()!=NT_Guard)
		{
			return;
		}	
	}

	CMoveShape* pAtker = pAttacker;
	CMoveShape* pDefer = pBeenAttacker;
	if( TYPE_PET == pAtker->GetType() )
	{
		pAtker = pAtker->GetHost();
	}

	if( TYPE_PET == pDefer->GetType() )
	{
		pDefer = pDefer->GetHost();
	}

	if( NULL == pAtker || NULL == pDefer )
	{
		PutoutLog("Pet", LT_ERROR, "...");
		return;
	}

	// [8/24/2009 chenxianj]
	//////////////////////////////////////////////////////////////////////////
	//�������򲻽���pkֵ�趨
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( pAttacker->GetFather() );
	if( !pRegion ) 
	{
		return;
	}

	CRegion::eSecurity eSecurityType = pRegion->GetSecurity(pAttacker->GetTileX(),pAttacker->GetTileY());

	if (eSecurityType==CRegion::SECURTIY_FIGHT)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	long lNum = pAtker->GetPkValue() + CGlobeSetup::GetSetup()->dwPkCountPerAttack;

	//��������ӪPKֵΪ0�Ľ�ɫ��������ɫ��/������������ӪPKֵ����0С�ڵ���20�Ľ�ɫ��������ɫ��
	if (nAddSub==2				&&
		IsSameCamp(pAtker,pDefer)&&
		(pDefer->GetPkValue()>=0 && pDefer->GetPkValue()<=20 )&&
		(pAtker->GetPkValue()<20))
	{
		if (lNum>20)
		{
			lNum = 20;
		}
		pAtker->SetPkValue(lNum);		
	}
	//����������������ͷ����漼�ܣ�ÿ��ʹ�����漼�ܽ�����20��PKֵ����˷�����õ�PKֵ����Ϊ20����PKֵ�Ѿ��ﵽ�򳬹�20����PKֵ��������˷�������
	else if (nAddSub==1 && (pDefer->GetPkValue()>0) && (pAtker->GetPkValue()<20) && pAtker!=pDefer)
	{
		if (lNum>20)
		{
			lNum = 20;
		}		
		pAtker->SetPkValue(lNum);
	}
	else if (nAddSub==2 && !IsSameCamp(pAtker,pDefer) && pDefer->GetPVPValue()==0 && pDefer->GetPkValue()<=20 && pAtker->GetPVPValue()<20)
	{
		if(pRegion->GetStrongPointFlag()==0) // δ�����ݵ�ս��־
		{
			//�����Ǳ���Ӫ����ֵΪ0��PKֵС�ڵ���20�Ľ�ɫ
			long lNum = pAtker->GetPVPValue() + CGlobeSetup::GetSetup()->dwPkCountPerAttack;
			if(lNum>CGlobeSetup::GetSetup()->dwPkCountPerAttack)
			{
				lNum = CGlobeSetup::GetSetup()->dwPkCountPerAttack;
			}
			pAtker->SetAttribute("PVPValue",lNum);
			pAtker->UpdateAttribute();
		}
	}
	else if (nAddSub==1  && pDefer->GetPVPValue()>0 && pAtker->GetPVPValue()<20 && pAtker!=pDefer)
	{
		if(pRegion->GetStrongPointFlag()==0) // δ�����ݵ�ս��־
		{
			//�����ж���ֵ�Ľ�ɫʹ�����漼��&& !IsSameCamp(pAtker,pDefer)
			long lNum = pAtker->GetPVPValue() + CGlobeSetup::GetSetup()->dwPkCountPerAttack;
			if (lNum>20)
			{
				lNum = 20;
			}
			pAtker->SetAttribute("PVPValue",lNum);	
			pAtker->UpdateAttribute();
		}
	}



}
bool CPKSys::IsSameCamp(CMoveShape *pAttacker,CMoveShape *pBeenAttacker)
{
	string str="Country";
	CCountry *pAttackerCountry = GetCountryHandler()->GetCountry(pAttacker->GetAttribute(str));
	CCountry *pBeenAttackerCountry = GetCountryHandler()->GetCountry(pBeenAttacker->GetAttribute(str));
	if(pAttackerCountry && pBeenAttackerCountry && pAttackerCountry->GetCampID(pAttacker->GetAttribute(str)) == pAttackerCountry->GetCampID(pBeenAttacker->GetAttribute(str)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//����ͬ��ID�õ������б�
void CPKSys::GetCountryList(long lCountyID,list<BYTE> &lCountryIDList)
{
	CCountry *pCountry = GetCountryHandler()->GetCountry(lCountyID);
	if (pCountry)
	{
		pCountry->GetCountryList(pCountry->GetCampID(lCountyID),lCountryIDList);
	}
	
}
//1��ʾ����
//2��ʾ����
BOOL CPKSys::JudgeAttackRelation(CMoveShape *pAttacker,CMoveShape *pBeenAttacker,int nAddSub)
{
	if (pAttacker==NULL || pBeenAttacker==NULL)
	{
		return FALSE;
	}
	if (pBeenAttacker->IsGod())
	{
		return FALSE;
	}

	if (!IsAttackAble(pAttacker,pBeenAttacker))
	{
		return FALSE;
	}
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( pAttacker->GetFather() );
	if( !pRegion ) 
		return FALSE;
	//PVP
	if (pAttacker->GetType()==TYPE_PLAYER && pBeenAttacker->GetType()==TYPE_PLAYER && nAddSub==2)
	{		
		CPlayer* pPlayerAttacker	 = dynamic_cast<CPlayer*>( pAttacker );
		CPlayer* pPlayerBeenAttacker = dynamic_cast<CPlayer*>( pBeenAttacker );

		if (pPlayerAttacker==NULL || pPlayerBeenAttacker==NULL)
		{
			return FALSE;
		}

		// �����nopk��������Ҳ���PK��
		if(eRPT_DisallowPk == pRegion->IsNoPk())
			return FALSE;
		else if(eRPT_DisallowTeamPk == pRegion->IsNoPk())
		{
			if(NULL_GUID != pPlayerAttacker->GetTeamID() && pPlayerAttacker->GetTeamID() == pPlayerBeenAttacker->GetTeamID())
				return FALSE;
		}

		CRegion::eSecurity eBSecurityType =
			pRegion->GetSecurity(pPlayerBeenAttacker->GetTileX(),pPlayerBeenAttacker->GetTileY());
		CRegion::eSecurity eSecurityType = pRegion->GetSecurity(pPlayerAttacker->GetTileX(),pPlayerAttacker->GetTileY());
		if (eSecurityType == CRegion::SECURTIY_SAFE || eBSecurityType == CRegion::SECURTIY_SAFE)
		{
			return FALSE;
		}
		//���
		if (pPlayerAttacker->IsPk_Team())
		{
			if (pPlayerAttacker->GetTeamID()!=NULL_GUID && pPlayerAttacker->GetTeamID()==pPlayerBeenAttacker->GetTeamID())
			{
				return FALSE;
			}
		}
		//���
		if (pPlayerAttacker->IsPk_Union())
		{
			if (pPlayerAttacker->GetFactionID()!=NULL_GUID && pPlayerAttacker->GetFactionID()==pPlayerBeenAttacker->GetFactionID())
			{
				return FALSE;
			}
		}
		//����
		if (pPlayerAttacker->IsPk_Country())
		{
			//  [9/7/2009 chenxianj]
			//ֻ���������������
			if (pPlayerAttacker->GetCountry()==pPlayerBeenAttacker->GetCountry() && pPlayerBeenAttacker->GetPkValue()==0)
			{
				return FALSE;
			}
		}
		//��Ӫ
		if (pPlayerAttacker->IsPk_Camp())
		{
			//  [9/7/2009 chenxianj]
			//ֻ��������Ӫ�������
			if (IsSameCamp(pPlayerAttacker,pPlayerBeenAttacker) && pPlayerBeenAttacker->GetPkValue()==0)
			{
				return FALSE;
			}
		}
		//ȫ��
		if (pPlayerAttacker->IsPk_Normal())
		{
			if(!IsSameCamp(pPlayerAttacker,pPlayerBeenAttacker) && pPlayerBeenAttacker->GetPkValue()==0 && pPlayerBeenAttacker->GetPVPValue()==0)
			{
				return FALSE;
			}			
		}
		//����
		if (pPlayerAttacker->IsPk_Badman())
		{
			if (pPlayerBeenAttacker->GetPkValue()>0)
			{
				return FALSE;
			}
		}
		//����
		if ( pPlayerAttacker->IsPk_Pirate())
		{
			if (pPlayerBeenAttacker->GetPVPValue()>0)
			{
				return FALSE;
			}
		}		
	}
	//PVM
	if (pAttacker->GetType()==TYPE_PLAYER && pBeenAttacker->GetType()==TYPE_MONSTER)
	{
		CPlayer* pPlayerAttacker	 = dynamic_cast<CPlayer*>( pAttacker );
		CMonster* pPlayerBeenAttacker = dynamic_cast<CMonster*>( pBeenAttacker );

		if (pPlayerAttacker==NULL || pPlayerBeenAttacker==NULL)
		{
			return FALSE;
		}

		CRegion::eSecurity SecurityType =
			pRegion->GetSecurity(pPlayerBeenAttacker->GetTileX(),pPlayerBeenAttacker->GetTileY());

		if (SecurityType == CRegion::SECURTIY_SAFE && nAddSub==1)
		{
			//�˶Թ��ڰ�ȫ������ʹ�����漼��
			return FALSE;
		}	
		if (((CMonster*)pBeenAttacker)->GetNpcType()==NT_Guard)
		{
			//����
			if (pPlayerAttacker->IsPk_Country())
			{
				if (pAttacker->GetCountry()==pBeenAttacker->GetCountry())
				{
					return FALSE;
				}
			}
			//��Ӫ
			if (pPlayerAttacker->IsPk_Camp())
			{
				if (IsSameCamp(pPlayerAttacker,pPlayerBeenAttacker))
				{
					return FALSE;
				}
			}
			//ȫ��
			if (pPlayerAttacker->IsPk_Normal())
			{
				return FALSE;
			}
		}	
	}
	//MVP
	if (pAttacker->GetType()==TYPE_MONSTER && pBeenAttacker->GetType()==TYPE_PLAYER)
	{
		CMonster* pPlayerAttacker	 = dynamic_cast<CMonster*>( pAttacker );
		CPlayer* pPlayerBeenAttacker = dynamic_cast<CPlayer*>( pBeenAttacker );
		if (pPlayerAttacker==NULL || pPlayerBeenAttacker==NULL)
		{
			return FALSE;
		}
		CRegion::eSecurity SecurityType =
			pRegion->GetSecurity(pPlayerBeenAttacker->GetTileX(),pPlayerBeenAttacker->GetTileY());
		if (SecurityType == CRegion::SECURTIY_SAFE && nAddSub==1)
		{
			//�ֶ����ڰ�ȫ������ʹ�����漼��
			return FALSE;
		}	

	}
	return TRUE;
}
//�ж�һ��MoveShape�Ƿ���Թ�������һ��MoveShape
bool	CPKSys::IsAttackAble(CMoveShape* pAttacker,CMoveShape* pBeAttacker)
{	

	if(pAttacker == NULL && pAttacker->IsAttackAble() == false)	
		return false;
	if(pBeAttacker == NULL && pBeAttacker->IsBeAttackAble() == false)	
		return false;

	if (pAttacker->IsDied() || pBeAttacker->IsDied())
	{
		return false;
	}

	bool bRet = true;
	if(pAttacker->GetType() == TYPE_PLAYER && pBeAttacker->GetType() == TYPE_PLAYER)
	{
	}
	else if(pAttacker->GetType() == TYPE_MONSTER && pBeAttacker->GetType() == TYPE_MONSTER)
	{
	}
	else if(pAttacker->GetType() == TYPE_PLAYER && pBeAttacker->GetType() == TYPE_MONSTER)
	{
	}
	else if(pAttacker->GetType() == TYPE_MONSTER && pBeAttacker->GetType() == TYPE_PLAYER)
	{
	}
	else if(pAttacker->GetType() == TYPE_MONSTER && pBeAttacker->GetType() == TYPE_CITYGATE)
	{
	}
	else if(pAttacker->GetType() == TYPE_PLAYER && pBeAttacker->GetType() == TYPE_CITYGATE)
	{
	}
	else if(pAttacker->GetType() == TYPE_PLAYER && pBeAttacker->GetType() == TYPE_PET &&
		pAttacker != pBeAttacker->GetHost() )
	{
	}
	else if(pAttacker->GetType() == TYPE_PET && pBeAttacker->GetType() == TYPE_PLAYER &&
		pBeAttacker != pAttacker->GetHost() )
	{
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

//�ж��Ƿ��ǵжԹ�ϵ
bool CPKSys::IsEnemy(CMoveShape* pShape1,CMoveShape* pShape2)
{
	return true;
}

//���ֹ���
void CPKSys::OnFirstAttack(CPlayer* pAttackPlayer,CPlayer* pBeAttackedPlayer,
						   CServerRegion* pRegion,CRegion::eSecurity SecurityType)
{
	if(pAttackPlayer == NULL || pBeAttackedPlayer == NULL || pRegion == NULL)
		return;

	//�����е������,����������ߴ��ڻ��������״̬,�����߶����᷸��
	if(pBeAttackedPlayer->IsBadman() == true)
		return;

	switch(SecurityType)
	{
		//��ս����
	case CRegion::SECURITY_CITYWAR:
		break;
		//��ȫ��
	case CRegion::SECURTIY_SAFE:
		break;
		//������
	case CRegion::SECURTIY_FIGHT:
		break;
		//��ͨ��
	case CRegion::SECURITY_FREE:
		{

			//������ڳ�ս״̬
			if(IsCityWarState(pAttackPlayer,pBeAttackedPlayer) == true)
			{
				//������
			}
			//������ڰ�ս״̬
			else if(ISFactionWarState(pAttackPlayer,pBeAttackedPlayer) == true)
			{
				//������
			}			
			else
			{
				// �������������᷸�ֻ�й����Լ������˲ŷ����ˡ�
				if( pAttackPlayer->GetCountry() == pBeAttackedPlayer->GetCountry() ) 
				{
					//�����߽��뷸��
					pAttackPlayer->EnterCriminalState();
				}				
			}

		}
		break;
	}
}


//���ּ���
void CPKSys::OnFirstSkill(CPlayer* pAttackPlayer,CPlayer* pBeAttackedPlayer,
						  CServerRegion* pRegion,CRegion::eSecurity SecurityType)
{
	if(pAttackPlayer == NULL || pBeAttackedPlayer == NULL || pRegion == NULL)
		return;

	//�����е������,����������ߴ��ڻ��������״̬,�����߶����᷸��
	if(pBeAttackedPlayer->IsBadman() == true)
		return;

	switch(SecurityType)
	{
		//��ս����
	case CRegion::SECURITY_CITYWAR:
		break;
		//��ȫ��
	case CRegion::SECURTIY_SAFE:
		break;
		//������
	case CRegion::SECURTIY_FIGHT:
		break;
		//��ͨ��
	case CRegion::SECURITY_FREE:
		{
			//������ڳ�ս״̬
			if(IsCityWarState(pAttackPlayer,pBeAttackedPlayer) == true)
			{
				//������
			}
			//������ڰ�ս״̬
			else if(ISFactionWarState(pAttackPlayer,pBeAttackedPlayer) == true)
			{
				//������
			}		
			else
			{
				//�����߽��뷸��
				if( pAttackPlayer->GetCountry() == pBeAttackedPlayer->GetCountry() ) 
				{
					pAttackPlayer->EnterCriminalState();
				}
			}
		}
		break;
	}
}
//ɱ������
void CPKSys::OnKill(CPlayer* pKillPlyaer,CPlayer* pBeKilledPlayer,CServerRegion* pRegion)
{
	if(pKillPlyaer == NULL || pBeKilledPlayer == NULL || pRegion == NULL)
		return;

	//�����е������,�����ɱ�ߴ��ڻ��������״̬,ɱ�߶���������Pk��
	if(pBeKilledPlayer->IsBadman() == true)
		return;

	CRegion::eSecurity SecurityType =
		pRegion->GetSecurity(pBeKilledPlayer->GetTileX(),pBeKilledPlayer->GetTileY());

	switch(SecurityType)
	{
		//��ս����
	case CRegion::SECURITY_CITYWAR:
		break;
		//��ȫ��
	case CRegion::SECURTIY_SAFE:
		break;
		//������
	case CRegion::SECURTIY_FIGHT:
		break;
		//��ͨ��
	case CRegion::SECURITY_FREE:
		{
			//������ڳ�ս״̬
			if(IsCityWarState(pKillPlyaer,pBeKilledPlayer) == true)
			{
				//������PK��
			}
			//������ڰ�ս״̬
			else if(ISFactionWarState(pKillPlyaer,pBeKilledPlayer) == true)
			{
				//������PK��
			}			
			else
			{	
				//����PK��
				ReportMurderer(pKillPlyaer,pBeKilledPlayer);				
			}
		}
		break;
	}
}

//��������
void CPKSys::OnDeath(CPlayer* pPlayer,CServerRegion* pRegion)
{
	if(pPlayer == NULL || pRegion == NULL)
		return;
}

//�õ��������ľ���
long CPKSys::GetDiedLostExp(CPlayer* pBeKilledPlayer,CServerRegion* pRegion,
							CRegion::eSecurity SecurityType)
{
	long lLostExp = 0;
	switch(SecurityType)
	{
		//��ս����
	case CRegion::SECURITY_CITYWAR:
		{
			lLostExp = CGlobeSetup::GetSetup()->fLossExp_War;
		}
		break;
		//��ȫ��
	case CRegion::SECURTIY_SAFE:
		break;
		//������
	case CRegion::SECURTIY_FIGHT:
		{
			lLostExp = CGlobeSetup::GetSetup()->fLossExp_Game;
		}
		break;
		//��ͨ��
	case CRegion::SECURITY_FREE:
		{
			lLostExp = CGlobeSetup::GetSetup()->fLossExp_Normal;
		}
		break;
	}
	return lLostExp;
}
//�õ������ı���
bool CPKSys::GetDiedLostGoods(CPlayer* pBeKilledPlayer,long lMurdererType,const CGUID& MurdererID,CServerRegion* pRegion,
							  CRegion::eSecurity SecurityType,long lNameColor,tagDiedLost& DiedLost)
{
	bool bFlag = false;
	long lArea = 0;
	memset(&DiedLost,0,sizeof(tagDiedLost));
	switch(SecurityType)
	{
		//��ս����
	case CRegion::SECURITY_CITYWAR:
		{
			//����ս���������Ʒ
			bFlag = true;
			lArea = 1;
		}
		break;

		//��ȫ��
	case CRegion::SECURTIY_SAFE:	
		break;

		//������
	case CRegion::SECURTIY_FIGHT:
		break;
		//��ͨ��
	case CRegion::SECURITY_FREE:
		{
			//��ս״̬��һ��״̬����������°���ͨ���������Ʒ��
			lArea = 0;
			bFlag = true;
			if(lMurdererType == TYPE_PLAYER)
			{
				CPlayer* pKillPlyaer = GetGame()->FindPlayer(MurdererID);
				//������ڰ�ս״̬,�����ͻ�������
				if(pKillPlyaer)
				{
					if( ISFactionWarState(pKillPlyaer,pBeKilledPlayer) == true )
					{
						if(lNameColor == 0 || lNameColor == 1)
						{
							bFlag = false;
						}
					}					
				}
				else
				{
					bFlag = false;
				}
			}
		}
		break;
	}
	if(bFlag)
	{
		DiedLost.fDropEquip_Head=CGlobeSetup::GetSetup()->fDropEquip_Head[lArea][lNameColor];//(ͷ���������)
		DiedLost.fDropEquip_Necklace=CGlobeSetup::GetSetup()->fDropEquip_Necklace[lArea][lNameColor];//(�����������)	
		DiedLost.fDropEquip_Wing=CGlobeSetup::GetSetup()->fDropEquip_Wing[lArea][lNameColor];//(������)	
		DiedLost.fDropEquip_Body=CGlobeSetup::GetSetup()->fDropEquip_Body[lArea][lNameColor];//(���׵������)	
		DiedLost.fDropEquip_Back=CGlobeSetup::GetSetup()->fDropEquip_Back[lArea][lNameColor];//(�����������)	
		DiedLost.fDropEquip_Glove=CGlobeSetup::GetSetup()->fDropEquip_Glove[lArea][lNameColor];//(���׵������)	
		DiedLost.fDropEquip_Boot=CGlobeSetup::GetSetup()->fDropEquip_Boot[lArea][lNameColor];//(Ь�ӵ������)	
		DiedLost.fDropEquip_Headgear=CGlobeSetup::GetSetup()->fDropEquip_Headgear[lArea][lNameColor];//(ͷ�ε������)
		DiedLost.fDropEquip_Frock=CGlobeSetup::GetSetup()->fDropEquip_Frock[lArea][lNameColor];//(���׵������)	
		DiedLost.fDropEquip_LRing=CGlobeSetup::GetSetup()->fDropEquip_LRing[lArea][lNameColor];//(���ָ�������)
		DiedLost.fDropEquip_RRing=CGlobeSetup::GetSetup()->fDropEquip_RRing[lArea][lNameColor];//(�ҽ�ָ�������)
		DiedLost.fDropEquip_Medal1=CGlobeSetup::GetSetup()->fDropEquip_Medal1[lArea][lNameColor];//(ѫ��1�������)
		DiedLost.fDropEquip_Medal2=CGlobeSetup::GetSetup()->fDropEquip_Medal2[lArea][lNameColor];//(ѫ��2�������)
		DiedLost.fDropEquip_Medal3=CGlobeSetup::GetSetup()->fDropEquip_Medal3[lArea][lNameColor];//(ѫ��3�������)
		DiedLost.fDropEquip_Fairy=CGlobeSetup::GetSetup()->fDropEquip_Fairy[lArea][lNameColor];//(С����������)
		DiedLost.fDropEquip_Weapon=CGlobeSetup::GetSetup()->fDropEquip_Weapon[lArea][lNameColor];//(�����������)
		DiedLost.fDropEquip_Weapon2=CGlobeSetup::GetSetup()->fDropEquip_Weapon2[lArea][lNameColor];//(���������������)	
		DiedLost.fDropEquip_Cloak=CGlobeSetup::GetSetup()->fDropEquip_Cloak[lArea][lNameColor];//(����������)	
		DiedLost.fDropEquip_Horse=CGlobeSetup::GetSetup()->fDropEquip_Horse[lArea][lNameColor];//(����������)
		DiedLost.fDropEquip_Bag=CGlobeSetup::GetSetup()->fDropEquip_Bag[lArea][lNameColor];//(������Ʒ�������)
		DiedLost.fDropEquip_Money=CGlobeSetup::GetSetup()->fDropEquip_Money[lArea][lNameColor];//(��Ǯ�������)	
		DiedLost.fDropEquip_MoneyPercent=CGlobeSetup::GetSetup()->fDropEquip_MoneyPercent[lArea][lNameColor];//(����Ǯ��������)
	}
	return bFlag;
}


//�ж���������Ƿ���ڳ�ս״̬
bool CPKSys::IsCityWarState(CPlayer* pPlayerA,CPlayer* pPlayerB)
{
	if(pPlayerA == NULL || pPlayerB == NULL)
		return false;

	//���а��
	if(NULL_GUID != pPlayerA->GetFactionID() && NULL_GUID != pPlayerB->GetFactionID())
	{
		
	}
	return false;
}
//�ж���������Ƿ���ڵж԰�ս״̬
bool CPKSys::ISFactionWarState(CPlayer* pPlayerA,CPlayer* pPlayerB)
{
	if(pPlayerA == NULL || pPlayerB == NULL)
		return false;

	//���а��
	if(NULL_GUID != pPlayerA->GetFactionID() && NULL_GUID != pPlayerB->GetFactionID() )
	{	
	}
	return false;
}
// �Ƿ��ڹ�ս״̬
bool CPKSys::IsCountryWarState(CPlayer* pPlayerA,CPlayer* pPlayerB)
{
	if(pPlayerA == NULL || pPlayerB == NULL)
		return false;

	return false;
}
//##��������
VOID CPKSys::ReportMurderer(CPlayer* pMurderer,CPlayer* pBMurderer)
{
	long lNum=0;
	if( pMurderer && pBMurderer )
	{
		if( IsSameCamp(pMurderer,pBMurderer)) 
		{
			if (pBMurderer->GetPkValue()==0)
			{
				lNum = pMurderer -> GetPkValue() + CGlobeSetup::GetSetup()->dwPkCountPerKill;
				if (lNum > 65535)
					lNum = 65535;
				pMurderer->SetPkValue(lNum);
			} 
			else if(pBMurderer->GetPkValue()>0 && pBMurderer->GetPkValue()<=20 && pMurderer->GetPkValue()<20)
			{
				lNum = pMurderer -> GetPkValue() + CGlobeSetup::GetSetup()->dwPkCountPerAttack;
				if (lNum>20)
				{
					lNum = 20;
				}
				pMurderer->SetPkValue(lNum);
			}			
		}
		else
		{
			if (pBMurderer->GetPkValue()<=20 && pBMurderer->GetPVPValue()==0)
			{
				CServerRegion* pRgn = dynamic_cast<CServerRegion*>(pBMurderer->GetFather());
				if(pRgn && pRgn->GetStrongPointFlag()==0) // δ�����ݵ�ս��־
				{
					lNum = pMurderer->GetPVPValue() + CGlobeSetup::GetSetup()->dwPkCountPerKill;
					pMurderer->SetAttribute("PVPValue",lNum);
					pMurderer->UpdateAttribute();
				}
			}
		}		
		pMurderer -> OnUpdateMurdererSign();
	}
}
