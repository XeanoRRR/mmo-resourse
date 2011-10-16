//=============================================================================
/**
 *  file: OrganiFun.cpp
 *
 *  Brief:��������顢��ᡢ���˵���֯ϵͳ��صĽű�����
 *			
 *
 *  Authtor:wangqiao
 *	
 *	Create Datae:2008-11-06
 */
//=============================================================================

#include "StdAfx.h"
#include "mmsystem.h"
#include "script.h"
#include "variablelist.h"
#include "../player.h"
#include "../serverregion.h"
#include "../Country/Country.h"
#include "../Country/CountryHandler.h"
#include "..\OrganizingSystem\GameOrganizingCtrl.h"
#include "..\OrganizingSystem\GSTeam.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "../../../../public/kl_tp_utility.h"



#define FACTION_PLAYER(pPlayer,pFaction)\
	char* szPlayerGuid=GetStringParam(cmd,0);\
	if (szPlayerGuid==NULL)\
		return 0;\
	if (0==strlen(szPlayerGuid))\
	{\
		pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);\
		if (pPlayer==NULL)\
		{\
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );\
			return 0;\
		}\
		pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());\
	}\
	else\
	{\
		pPlayer=GetGame()->FindPlayer(\
			GetScriptGuidValue((CMoveShape*)p_SrcShape,szPlayerGuid));\
		if (pPlayer==NULL)\
		{\
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );\
			return 0;\
		}\
		pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());\
	}\
	if (pFaction==NULL)\
	{\
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );\
		return 0;\
	}\
	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );


double CScript::GetTeamNum(const char* CmdStr, char* retStr)			//��ȡ��������
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return -1;
	const CGUID& TeamID = pPlayer->GetTeamID();
	if ( TeamID != NULL_GUID )
	{
		GSTeam* pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(TeamID);
		if (NULL != pTeam)
		{
			return pTeam->GetMemberNum();
		}
	}

	return 0;
}

double CScript::GetTeamNumInRgn(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return -1;
	const CGUID& TeamID = pPlayer->GetTeamID();
	if ( TeamID != NULL_GUID )
	{
		GSTeam* pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(TeamID);
		if (NULL != pTeam)
		{
			return pTeam->GetMemberNumInRgn(pPlayer->GetRegionGUID());
		}
	}

	return 0;
}

double CScript::GetTeamerName(const char* CmdStr, char* retStr)		//��ȡ�������� (������Ϊ�����Ĳ���ʹ��,ֻ�ܷ��ص�������!!)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return -1;

	int iPos = GetIntParam(CmdStr,0);
	if (iPos==ERROR_CODE) return 0.0f;

	char* strPlayerName = GetStringParam(CmdStr, 1);
	if(strPlayerName == NULL)
		return -1.0f;

	const CGUID& TeamID = pPlayer->GetTeamID();
	if ( TeamID != NULL_GUID )
	{
		GSTeam* pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(TeamID);
		if (NULL != pTeam)
		{

			list<CGUID> MemberList;
			pTeam -> GetMemberList(MemberList);
			list<CGUID>::iterator ite = MemberList.begin();
			for( int i = 0; ite != MemberList.end(); ite ++, i ++ )
			{
				if( i == iPos )
				{
					CPlayer * pPlayer = GetGame()->FindPlayer(*ite);
					if (NULL != pPlayer)
					{
						SetScriptVarValue(pPlayer, strPlayerName, pPlayer->GetName());
						M_FREE( strPlayerName, sizeof(char)*MAX_VAR_LEN );
						return 1;
					}
					break;
				}
			}
		}
	}

	M_FREE( strPlayerName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetTeamGUID(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if (pPlayer== NULL) return 0;

	char* cGuidName= GetStringParam(cmd,0);
	if (cGuidName== NULL)	return -1;

	long re = 0;
	if(NULL_GUID != pPlayer->GetTeamID())
	{
		re = 1;
	}
	SetScriptGuidValue(pPlayer, cGuidName,pPlayer->GetTeamID());

	M_FREE( cGuidName, sizeof(char)*MAX_VAR_LEN );
	return re;
}

double CScript::GetPlayerTeamGUID(const char* cmd, char* retStr)
{
	char* szMyGuid=GetStringParam(cmd,0);
	char* szTeamGuid=GetStringParam(cmd,1);
	if (szMyGuid== NULL || szTeamGuid== NULL )
	{
		M_FREE( szMyGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szTeamGuid, sizeof(char)*MAX_VAR_LEN );
		return -1;
	}

	long  re = 0;
	CGUID playerGUID = GetVariableList()->GetGuidValue(szMyGuid);
	CServerRegion* pRegion=dynamic_cast<CServerRegion*>(p_SrcShape->GetFather());
	if (pRegion!= NULL)
	{
		CPlayer* pPlayer= dynamic_cast<CPlayer*>(pRegion->FindChildObject(TYPE_PLAYER,playerGUID));
		if (pPlayer!= NULL)
		{
			if(NULL_GUID != pPlayer->GetTeamID())
				re = 1;
			GetVariableList()->SetGuidValue(szTeamGuid,&pPlayer->GetTeamID());
		}
	}
	M_FREE( szMyGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szTeamGuid, sizeof(char)*MAX_VAR_LEN );
	return re;
}

double CScript::IsTeamCaptain(const char* CmdStr, char* retStr)		//�ж��Լ��Ƿ��Ƕӳ�	����ֵres 1�Ƕӳ� 0���Ƕӳ� -1û�ж���
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return -1;
	if ( pPlayer->GetTeamID() != NULL_GUID )
	{
		GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
		if( pGSTeam )
		{
			const CGUID& CaptainID = pGSTeam->GetLeader();
			if (CaptainID == ((CPlayer*)this->p_SrcShape)->GetExID())
				return 1;
			else
				return 0;
		}
	}

	return -1;
}

//! ������е�ͼ��ֻ�е���С��ָ�������Ķ���
//! ���أ�1=������ 0=������
double CScript::CheckRgnOnlyTeam(const char* CmdStr, char* retStr)
{
	CMoveShape *pShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	if(NULL == pShape)
		return 0;

	CServerRegion *pRegion = dynamic_cast<CServerRegion*>(pShape->GetFather());
	if(NULL == pRegion)
		return 0;

	long Num = GetIntParam(CmdStr, 0);

	return !pRegion->ConfirmTeamNum(Num);
}

double CScript::GetTeamCaptainPos(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return -1;
	if ( pPlayer->GetTeamID() != NULL_GUID )
	{
		GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
		if( pGSTeam )
		{
			const CGUID& CaptainID = pGSTeam->GetLeader();
			list<CGUID> MemberList;
			pGSTeam -> GetMemberList(MemberList);
			list<CGUID>::iterator ite = MemberList.begin();
			for( int i = 0; ite != MemberList.end(); ite ++, i ++ )
			{
				if( CaptainID == *ite )
				{
					return i;
				}
			}
		}
	}

	return -1;
}



double CScript::SetRegionForTeam(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return 0;


	// assert(false);
	int	 iType		= GetIntParam( CmdStr, 0 );
	eRgnType rgnType = (eRgnType)(long)GetIntParam(CmdStr, 1);
	char* tGuid = GetStringParam(CmdStr,2);
	if(!tGuid) return 0;

	CGUID lRegionID = GetScriptGuidValue((CMoveShape*)p_SrcShape, tGuid);

	double lX			= GetIntParam( CmdStr, 3 );
	double lY			= GetIntParam( CmdStr, 4 );
	double dir		= GetIntParam( CmdStr, 5 );
	double lRange		= GetIntParam( CmdStr, 6 );

	// ����
	// 0�ɹ�  ʧ�ܣ� 1 û�ж���


	//���鴫������ 
	// 0 7x7��   1 ͬ����  2 ͬ������  3 ͬ����
	if( iType < 0 || iType >3 )
		iType = 0;

	if( lX == ERROR_CODE || lY == ERROR_CODE || lX==0 || lY==0)
		lX = lY = -1;

	if( dir == ERROR_CODE )
		dir = p_SrcShape->GetDir();

	if (lRange==ERROR_CODE)
		lRange = 2;


	//��ȡ��������
	GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID( pPlayer-> GetTeamID());

	// û�ж���
	if( !pGSTeam )

	{
		M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
		return 1; // û�ж��飬����ʧ�ܡ�
	}

	// �ж���
	if (iType == 0) {
		//* 7x7�ڴ��� *//
		// �ö����Ƿ���ͬһ������
		if( pGSTeam -> GetCurrentServerTeamatesAmount() != pGSTeam -> GetTeamatesAmount() )
		{
			M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
			return 2; //���Ѳ��Ƕ���һ���������
		}


		// ���Ѷ���һ����������
		// �ö���ÿ�������Ƿ���ͬһ����
		//bool bIsSameRegion = true;
		if( pGSTeam->GetCurrentRegionTeamatesAmount(pPlayer->GetRegionGUID()) != pGSTeam->GetTeamatesAmount() )
		{
			M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
			return 3; // ���Ѳ�����һ��������
		}

		// ��� ���ж��Ѷ���ͬһ����
		// ��ʼ7x7����
		int nStartX=0,nStartY=0;
		CServerRegion* pRegion=dynamic_cast<CServerRegion*>(pPlayer->GetFather());
		nStartX=pPlayer->GetTileX() - 3;
		nStartY=pPlayer->GetTileY() - 3;

		if(pPlayer->GetTileX()+3 >= pRegion->GetWidth())
			nStartX=pRegion->GetWidth() - 7;
		else if(pPlayer->GetTileX()-3 <= 0)
			nStartX=0;

		if(pPlayer->GetTileY()+3 >= pRegion->GetHeight())
			nStartY=pRegion->GetHeight() - 7;
		else if(pPlayer->GetTileY()-3 <= 0)
			nStartY=0;

		// ��������
		// �ж϶����Ƿ� 7��7������
		bool bIs7x7 = true;

		CPlayer *pMember = NULL;
		list<CGUID> memberList;
		pGSTeam->GetMemberList(memberList);

		for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++) 
		{
			{
				pMember = GetGame()->FindPlayer(*it);

				if(pMember) {
					if( pMember->GetTileX() >= nStartX && pMember->GetTileX() <= nStartX+7 &&
						pMember->GetTileY() >= nStartY && pMember->GetTileY() <= nStartY+7 ) {

							bIs7x7 &= true;
					}
					else {
						bIs7x7 = false;
						break;
					}
				}
				else {
					bIs7x7 = false;
					break;
				}
			} 
		} 

		if( !bIs7x7 )
		{
			M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
			return 4; // ���Ѳ�����7x7������
		}

		// ������7x7������
		// ����ÿ�����ѵ�ָ������[����]
		for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++) 
		{
			{
				pMember = GetGame()->FindPlayer(*it);

				if(pMember) 
				{
					// [071127 AHC REGION]
					pMember -> ChangeRegion(rgnType, lRegionID, lX, lY, dir, 0, lRange );
				}
			}
		}
		M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
		return 0; // �ɹ���
	}

	else if(iType == 1) 
	{
		//* ͬ�����ڴ��� *//
		// �ö����Ƿ���ͬһ������
		if( pGSTeam -> GetCurrentServerTeamatesAmount() != pGSTeam -> GetTeamatesAmount() )
		{
			M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
			return 2; //���Ѳ��Ƕ���һ���������
		}


		// ���Ѷ���һ����������
		// �ö���ÿ�������Ƿ���ͬһ����
		//bool bIsSameRegion = true;
		if( pGSTeam->GetCurrentRegionTeamatesAmount(pPlayer->GetRegionGUID()) != pGSTeam->GetTeamatesAmount() )
		{
			M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
			return 3; // ���Ѳ�����һ��������
		}


		CPlayer *pMember = NULL;

		//��һ��������
		// ����ÿ�����ѵ�ָ������[����]
		list<CGUID> memberList;
		pGSTeam->GetMemberList(memberList);

		for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++) 
		{
			{
				pMember = GetGame()->FindPlayer(*it);

				if(pMember) 
				{
					pMember -> ChangeRegion(rgnType, lRegionID, lX, lY, dir, 0, lRange );
				}
			}
		}
		M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;// �ɹ�
	}

	else if(iType == 2) 
	{
		//* ͬ�������ڴ��� *//
		// �ö����Ƿ���ͬһ������
		if( pGSTeam -> GetCurrentServerTeamatesAmount() != pGSTeam -> GetTeamatesAmount() )
		{
			M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
			return 2; //���Ѳ��Ƕ���һ���������
		}

		//��һ����������
		// ����ÿ�����ѵ�ָ������[����]
		CPlayer *pMember = NULL;
		list<CGUID> memberList;
		pGSTeam->GetMemberList(memberList);

		for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++)  
		{
			pMember = GetGame()->FindPlayer(*it);
			if(pMember)
				pMember -> ChangeRegion(rgnType, lRegionID, lX, lY, dir, 0, lRange );
		}
		M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;// �ɹ�
	}

	else if(iType == 3)
	{
		M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}// end iType

	M_FREE( tGuid, sizeof(char)*MAX_VAR_LEN );
	return 1;
}
double CScript::SetTeamRegion(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)return 0;

	//assert(false);
	eRgnType type = (eRgnType)(long)GetIntParam(CmdStr, 0);
	char* szGUID = GetStringParam( CmdStr, 1 );
	CGUID lRegionID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szGUID);
	double lX			= GetIntParam( CmdStr, 2 );
	double lY			= GetIntParam( CmdStr, 3 );
	// ��Ȱ뾶
	double lRadius	= GetIntParam( CmdStr, 4 );
	double dir		= GetIntParam( CmdStr, 5 );
	double lRange		= GetIntParam( CmdStr, 6 );

	if( lRegionID == NULL_GUID )
	{
		M_FREE( szGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if( lX == ERROR_CODE || lY == ERROR_CODE || lX==0 || lY==0)
		lX = lY = -1;

	if( lRadius == ERROR_CODE )
		lRadius = 3;

	if( dir == ERROR_CODE )
		dir = p_SrcShape->GetDir();

	if (lRange==ERROR_CODE)
		lRange = 2;

	//��ȡ��������
	GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID( pPlayer->GetTeamID() );

	if(pGSTeam)
	{
		double lOldRegionID = pPlayer->GetRegionID();
		double lOldTileX = pPlayer->GetTileX();
		double lOldTileY = pPlayer->GetTileY();

		// ����ÿ�����ѵ�ָ������[����]
		CPlayer *pMember = NULL;
		list<CGUID> memberList;
		pGSTeam->GetMemberList(memberList);

		for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++) 
		{
			pMember = GetGame()->FindPlayer(*it);
			if(!pMember) continue;

			// �Լ�ֱ�Ӵ���
			if( pPlayer->GetExID() == pMember->GetExID() ) 
			{
				// [071127 AHC REGION]
				pMember -> ChangeRegion(type, lRegionID, lX, lY, dir, 0, lRange );
				continue;
			}
			// ��ͬһ���� �� ��һ����Χ��, �����޲���
			if( pMember->IsInRegion(lOldRegionID) &&
				pMember->IsInArea(lOldTileX, lOldTileY, lRadius) )  
			{
				pMember -> ChangeRegion(type, lRegionID, lX, lY, dir, 0, lRange );
			}
		}
	}

	M_FREE( szGUID, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
double CScript::IsTeammatesAroundMe(const char* CmdStr, char* retStr)
{
	if(NULL == dynamic_cast<CPlayer*>(p_SrcShape))
		return 0;
	//assert(false);
	double lType   = GetIntParam( CmdStr, 0 );
	double lRadius = GetIntParam( CmdStr, 1 );

	if( lType == ERROR_CODE )
		lType = 0;

	if( lRadius == ERROR_CODE )
		lRadius = 3;

	//��ȡ��������
	GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(( dynamic_cast<CPlayer*>(p_SrcShape))->GetTeamID() );

	if(!pGSTeam) return 0; // û�ж���

	CPlayer *pPlayer = NULL;
	switch( (long)lType )
	{
		// �Ƿ���ĳ����Χ��
	case 0:
		{
			// �ö����Ƿ���ͬһ������

			//�������� ���Ѳ��Ƕ���һ���������
			if( pGSTeam->GetCurrentServerTeamatesAmount() != pGSTeam->GetTeamatesAmount() )
				return 0; 

			list<CGUID> memberList;
			pGSTeam->GetMemberList(memberList);

			for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++) 
			{
				pPlayer = GetGame()->FindPlayer(*it);
				// NULL
				if(!pPlayer) return 0;
				// �����Լ�
				if( p_SrcShape->GetExID() == pPlayer->GetExID() )
					continue;
				if( !pPlayer->IsInRegion(p_SrcShape->GetRegionID()) ) 
					return 0; // ����һ������
				if( !pPlayer->IsInArea( p_SrcShape->GetTileX(), p_SrcShape->GetTileY(), lRadius) ) 
					return 0; // �������� ����һ������ ����һ��������
			}
		}

		break; // break 0

		// �Ƿ���һ��������
	case 1:
		{
			// �ö����Ƿ���ͬһ������
			if( pGSTeam->GetCurrentServerTeamatesAmount() != pGSTeam->GetTeamatesAmount() )
				return 0; 

			list<CGUID> memberList;
			pGSTeam->GetMemberList(memberList);

			for(list<CGUID>::iterator it = memberList.begin(); it != memberList.end(); it++) 
			{
				pPlayer = GetGame()->FindPlayer(*it);
				// NULL
				if(!pPlayer) 
					return 0;
				// �����Լ�
				if( p_SrcShape->GetExID() == pPlayer->GetExID() )
					continue;
				// ����Ƿ���ͬһ����
				if( !pPlayer->IsInRegion(p_SrcShape->GetRegionID()) )
					return 0; // ����һ������
			}
		}
		break; // break 1

		// �Ƿ���һ����������
	case 2:
		// �ö����Ƿ���ͬһ������
		if( pGSTeam->GetCurrentServerTeamatesAmount() != pGSTeam->GetTeamatesAmount() )
			return 0; 
		break;

	default:
		return 0;// ��������
	} // end case

	return 1; // ���ж�Ա���Ͳ��������һ����Χ��
}

//! �жϵ�ǰ������ڶ���ĵȼ����Ƿ�������ķ�Χ��
double CScript::InLvlDifference(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)
		return 0;
	LONG lSpace = GetIntParam(cmd, 0);
	if(ERROR_CODE == lSpace)
		return 0;

	const char *pRet = GetStringParam(cmd, 1);
	if(NULL == pRet)
		return 0;

	if( pPlayer && NULL_GUID != pPlayer -> GetTeamID() )
	{
		GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
		if( pGSTeam )
		{
			CMessage msg(MSG_S2W_TEAM_INLVLDIFFERENCE);
			msg.Add(GetID());
			msg.Add(pGSTeam->GetExID());
			msg.Add(lSpace);
			msg.Add(pRet);
			msg.Send(false);

			SetIsHang(true);
			SetHangFunc(SR_FUNC_INLVLDIFFERENCE);
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
	}

	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

//! �жϵ�ǰ������ڶ���ĵȼ����Ƿ�������ķ�Χ��
double CScript::InLvlSpace(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(NULL == pPlayer)
		return 0;
	LONG lMin = GetIntParam(cmd, 0);
	if(ERROR_CODE == lMin)
		return 0;
	LONG lMax = GetIntParam(cmd, 1);
	if(ERROR_CODE == lMax)
		return 0;

	const char *pRet = GetStringParam(cmd, 2);
	if(NULL == pRet)
		return 0;

	if( pPlayer && NULL_GUID != pPlayer -> GetTeamID() )
	{
		GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
		if( pGSTeam )
		{
			CMessage msg(MSG_S2W_TEAM_INLVLSPACE);
			msg.Add(GetID());
			msg.Add(pGSTeam->GetExID());
			msg.Add(lMin);
			msg.Add(lMax);
			msg.Add(pRet);
			msg.Send(false);

			SetIsHang(true);
			SetHangFunc(SR_FUNC_INLVLSPACE);
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
	}

	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::RunScriptForTeam(const char* CmdStr, char* retStr)
{
	double lRet = -1;
	char *pGuidName = GetStringParam( CmdStr, 0 );
	CGUID guid = NULL_GUID;
	if (NULL != pGuidName)
	{
		guid = GetVariableList()->GetGuidValue(pGuidName);
	}

	char* strScriptName	= GetStringParam( CmdStr,1 );
	if(strScriptName == NULL)
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return lRet;
	}
	double lDis = GetIntParam( CmdStr, 2 );
	if(lDis == ERROR_CODE)
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		M_FREE( strScriptName, sizeof(char)*MAX_VAR_LEN );
		return lRet;
	}

	CPlayer* pPlayer = NULL;
	if( guid == NULL_GUID)
		pPlayer = (dynamic_cast<CPlayer*>(p_SrcShape));
	else
		pPlayer = GetGame()->FindPlayer(guid);

	if( pPlayer && NULL_GUID != pPlayer -> GetTeamID() )
	{
		GSTeam *pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
		if( pGSTeam )
		{
			pGSTeam->RunScript(strScriptName,pPlayer,lDis);
			M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
			M_FREE( strScriptName, sizeof(char)*MAX_VAR_LEN );
			return 1;
		}
	}
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strScriptName, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}

//-----------------------------------------------------------------------------
double CScript::CreateFaction(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	CMessage msg(MSG_S2C_ORGSYS_CreateFaction_Notify);

	do 
	{
		if(NULL == GetCountryHandler()->GetCountry(pPlayer->GetCountry()))
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_Country);
			break;
		}

		if (NULL_GUID != pPlayer->GetFactionID())
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_HadFaction);
			break;
		}

		eFactionOptErrInfo eRe = GetOrganiCtrl()->TestUpgradeFacCondition(pPlayer, 1, eUT_FactionLevel);
		if(eFOEI_NotErr != eRe)
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eRe);
			break;
		}

		//! ������֤����ʵ������ʱ�����
		//! �������֤��������������빤������

		GetOrganiCtrl()->PlayerFacOptBegin(pPlayer->GetExID(), GameOrganizingCtrl::ePFOT_Create);
		msg.Add(PLAYER_OPT_SUCCEED);

	} while(false);

	msg.SendToPlayer(pPlayer->GetExID());

	return 1;
}

double CScript::CreateUnion(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	CMessage msg(MSG_S2C_UNION_Script_Creation);

	do 
	{
		if(NULL == GetCountryHandler()->GetCountry(pPlayer->GetCountry()))
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_Country);
			break;
		}

		eFactionOptErrInfo eRe = GetOrganiCtrl()->TestCreateUnionCondition(pPlayer);
		if(eFOEI_NotErr != eRe)
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eRe);
			break;
		}

		//! ������֤����ʵ������ʱ�����
		//! ��֤���������������������
		GetOrganiCtrl()->PlayerFacOptBegin(pPlayer->GetExID(), GameOrganizingCtrl::ePFOT_CreateUnion);
		msg.Add(PLAYER_OPT_SUCCEED);

	} while(false);

	msg.SendToPlayer(pPlayer->GetExID());

	return 1;
}


double CScript::OpenApplyBoard(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	GetOrganiCtrl()->OpenApplyBoard(pPlayer->GetExID());

	return 1;
}
double CScript::OpenFacIconBoard(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	GetOrganiCtrl()->OpenUpLoadIconBoard(pPlayer->GetExID());

	return 1;
}
double CScript::SetFactionRecruitState(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	LONG lInPut = GetIntParam(cmd, 0);
	if(ERROR_CODE == lInPut)
		return 0.0;

	return GetOrganiCtrl()->SetFactionRecruit(pPlayer, (0 != lInPut));
}

double CScript::GetFactionLevel(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);

	if(pPlayer == NULL) return 0;

	return GetOrganiCtrl()->GetFactionLvl(pPlayer);
}

double CScript::GetFactionIDByPlayerName(const char* CmdStr, char* retStr)
{
	CHAR* szPlayerName	= GetStringParam( CmdStr, 0 );

	static CGUID ret; ret = NULL_GUID;
	if(szPlayerName==NULL)
		return (long)(&ret);	

	CPlayer* pPlayer = NULL;

	//�ҵ�Player
	if( strlen(szPlayerName) == 0 )
		pPlayer = (dynamic_cast<CPlayer*>(p_SrcShape));
	else
		pPlayer = GetGame()->FindPlayer(szPlayerName);
	
	if(pPlayer)
	{
		//���ýű�GUID����
		char* szName=GetStringParam(CmdStr,1);
		SetScriptGuidValue(pPlayer, szName,pPlayer->GetFactionID());
		M_FREE( szName, sizeof(char)*MAX_VAR_LEN );
	}
	M_FREE( szPlayerName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
double CScript::GetUinionIDByPlayerName(const char* CmdStr, char* retStr)
{
	CHAR* szPlayerName	= GetStringParam( CmdStr, 0 );

	static CGUID ret(NULL_GUID);
	if(szPlayerName==NULL)
		return (long)(&ret);	

	CPlayer* pPlayer = NULL;

	//�ҵ�Player
	if( strlen(szPlayerName) == 0 )
		pPlayer = (dynamic_cast<CPlayer*>(p_SrcShape));
	else
		pPlayer = GetGame()->FindPlayer(szPlayerName);

	if(pPlayer)
	{
		ret = pPlayer->GetUnionID();
	}
	M_FREE( szPlayerName, sizeof(char)*MAX_VAR_LEN );
	return (long)(&ret);
}
//! Ϊ������ڹ��������Դ�����ٴ��븺����@PlayerGuid��=""��$Ret=1��ʾִ�гɹ���AddFacRes("@PlayerGuid", #value, $Ret);
double CScript::AddFacRes(const char* CmdStr, char* retStr)
{
	char* pPlayerGuid = GetStringParam(CmdStr, 0);
	if(NULL == pPlayerGuid)
		return 0;

	CPlayer *pPlayer = NULL;
	if(0 == strlen(pPlayerGuid))
		pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	else
		pPlayer = GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, pPlayerGuid));
	
	M_FREE( pPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	if(NULL == pPlayer)
		return 0;

	GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return 0;

	LONG lDeltaRes = GetIntParam(CmdStr, 1);
	if(lDeltaRes == ERROR_CODE)
		return 0;

	// ����ֵ������
	char* szRet = GetStringParam(CmdStr, 2);
	if(szRet == NULL) 
		return 0;

	CMessage msg(MSG_S2W_Faction_ChangeRes_Reques);
	msg.Add(pFaction->GetExID());
	msg.Add(lDeltaRes);
	msg.Add(GetID());
	msg.Add(szRet);
	msg.Send(false);

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDFACRES);

	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return 0;

}
//! Ϊ������ڹ�����Ӿ��飨���ٴ��븺����@PlayerGuid��=""��$Ret=1��ʾִ�гɹ���AddFacExp("@PlayerGuid", #value, $Ret);
double CScript::AddFacExp(const char* CmdStr, char* retStr)
{
	char* pPlayerGuid = GetStringParam(CmdStr, 0);
	if(NULL == pPlayerGuid)
		return 0;

	CPlayer *pPlayer = NULL;
	if(0 == strlen(pPlayerGuid))
		pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	else
		pPlayer = GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, pPlayerGuid));

	M_FREE( pPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	if(NULL == pPlayer)
		return 0;

	GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return 0;

	LONG lDeltaExp = GetIntParam(CmdStr, 1);
	if(lDeltaExp == ERROR_CODE)
		return 0;

	// ����ֵ������
	char* szRet = GetStringParam(CmdStr, 2);
	if(szRet == NULL) 
		return 0;
	CMessage msg(MSG_S2W_Faction_ChangeExp_Reques);
	msg.Add(pFaction->GetExID());
	msg.Add(lDeltaExp);
	msg.Add(GetID());
	msg.Add(szRet);
	msg.Send(false);

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDFACEXP);

	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
//! Ϊ�����ӹ��ṱ�ף����ٴ��븺����@PlayerGuid��=""��$Ret=1��ʾִ�гɹ���AddFacMemberContribute("@PlayerGuid", #value, $Ret);
double CScript::AddFacMemberContribute(const char* CmdStr, char* retStr)
{
	char* pPlayerGuid = GetStringParam(CmdStr, 0);
	if(NULL == pPlayerGuid)
		return 0;

	CPlayer *pPlayer = NULL;
	if(0 == strlen(pPlayerGuid))
		pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	else
		pPlayer = GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, pPlayerGuid));
	
	M_FREE( pPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	if(NULL == pPlayer)
		return 0;

	GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return 0;

	LONG lDeltaContribute = GetIntParam(CmdStr, 1);
	if(lDeltaContribute == ERROR_CODE)
		return 0;

	// ����ֵ������
	char* szRet = GetStringParam(CmdStr, 2);
	if(szRet == NULL) 
		return 0;

	CMessage msg(MSG_S2W_Faction_ChangeMemberContribute_Reques);
	msg.Add(pFaction->GetExID());
	msg.Add(pPlayer->GetExID());
	msg.Add(lDeltaContribute);
	msg.Add(GetID());
	msg.Add(szRet);
	msg.Send(false);
	
	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDFACMEMBERCONTRIBUTE);

	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetMemContribute(const char* cmd,char* retStr)
{
	CPlayer* pPlayer=NULL;
	GameFaction* pFaction=NULL;
	FACTION_PLAYER(pPlayer,pFaction);
	return pFaction->GetMemberContribute(pPlayer->GetExID());
}

double CScript::GetFacRes(const char* cmd,char* retStr)
{
	CPlayer* pPlayer=NULL;
	GameFaction* pFaction=NULL;
	FACTION_PLAYER(pPlayer,pFaction);
	return pFaction->GetRes();
}

double CScript::GetFacExp(const char* cmd,char* retStr)
{
	CPlayer* pPlayer=NULL;
	GameFaction* pFaction=NULL;
	FACTION_PLAYER(pPlayer,pFaction);
	return pFaction->GetExp();
}


//! ��ѯ����Ƿ��ǻ᳤
double CScript::IsFacMaster(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = (dynamic_cast<CPlayer*>(p_SrcShape));
	if(NULL == pPlayer)
		return 0;

	GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return 0;

	if(pFaction->GetMaster() == pPlayer->GetExID())
		return 1;

	return 0;
}

double CScript::AddFacTickets(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	char* cBaseName=GetStringParam(cmd,1);
	if (cBaseName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long tickets=GetIntParam(cmd,2);
	char* pRet=GetStringParam(cmd,3);
	if (pRet==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( cBaseName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	CMessage msg(MSG_S2W_Faction_ChangeTickets);
	msg.Add(pFaction->GetExID());
	msg.Add(GetID());
	msg.Add(pRet);
	msg.Add(cBaseName);
	msg.Add(tickets);
	msg.Send(false);

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDFACTICKETS);

	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( cBaseName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::AddFacBaseBattle(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	char* cBaseName=GetStringParam(cmd,1);
	if (cBaseName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long lBaseBattle=GetIntParam(cmd,2);
	char* pRet=GetStringParam(cmd,3);
	if (pRet==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( cBaseName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CMessage msg(MSG_S2W_Faction_ChangeBaseBattle);
	msg.Add(pFaction->GetExID());
	msg.Add(GetID());
	msg.Add(pRet);
	msg.Add(cBaseName);
	msg.Add(lBaseBattle);
	msg.Send(false);

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDBASEBATTLE);

	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( cBaseName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::ChangeBaseInfo(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;

	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	char* pBaseName=GetStringParam(cmd,1);
	if (pBaseName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	char* pRet=GetStringParam(cmd,2);
	if (pRet==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pBaseName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CMessage msg(MSG_S2W_Faction_ChangeBase);
	msg.Add(pFaction->GetExID());
	msg.Add(GetID());
	msg.Add(pRet);
	msg.Add(pBaseName);
	msg.Send(false);

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDBASE);

	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pBaseName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::AddBattleVal(const char* cmd,char* retstr)
{
	char* szGuid=GetStringParam(cmd,0);
	if (szGuid==NULL)
		return 0;
	long lValue=GetIntParam(cmd,1);
	//Ϊ0��ʾ�޸Ĺ���ս��ֵ��Ϊ1�޸����ս��ֵ
	long lFlags=GetIntParam(cmd,2);
	char* pRet=GetStringParam(cmd,3);
	if (pRet==NULL)
	{
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	GameFaction* pFaction=NULL;
	CPlayer* pPlayer=NULL;
	CMessage msg(MSG_S2W_Faction_ChangeBattle_Reques);
	//szGuid��ʾ��ԱGUID,����Ϊ��
	if (lFlags>0)
	{
		if (stricmp("",szGuid)==0)
		{
			pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		else
		{
			pPlayer=GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid));
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		msg.Add(pFaction->GetExID());
		msg.Add(GetID());
		msg.Add(pRet);
		msg.Add(lValue);
		msg.Add(pPlayer->GetExID());
	}
	//szGuid��ʾ����GUID,����Ϊ��
	else
	{
		if (stricmp("",szGuid)==0)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szGuid));
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		msg.Add(pFaction->GetExID());
		msg.Add(GetID());
		msg.Add(pRet);
		msg.Add(lValue);
		msg.Add(NULL_GUID);
	}
	msg.Send();
	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDBATTLE);

	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::AddArmyInvest(const char* cmd,char* retstr)
{
	char* szGuid=GetStringParam(cmd,0);
	if (szGuid==NULL)
		return 0;
	long lValue=GetIntParam(cmd,1);
	//Ϊ0��ʾ�޸Ĺ������Ͷ�ʣ�Ϊ1�޸ĳ�ԱͶ��ֵ
	long lFlags=GetIntParam(cmd,2);
	char* pRet=GetStringParam(cmd,3);
	if (pRet==NULL)
	{
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	GameFaction* pFaction=NULL;
	CPlayer* pPlayer=NULL;
	CMessage msg(MSG_S2W_Faction_ChangeArmyInvest_Reques);
	//szGuid��ʾ��ԱGUID,����Ϊ��
	if (lFlags>0)
	{
		if (stricmp("",szGuid)==0)
		{
			pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		else
		{
			pPlayer=GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid));
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		msg.Add(pFaction->GetExID());
		msg.Add(GetID());
		msg.Add(pRet);
		msg.Add(lValue);
		msg.Add(pPlayer->GetExID());
	}
	//szGuid��ʾ����GUID,����Ϊ��
	else
	{
		if (stricmp("",szGuid)==0)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szGuid));
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		msg.Add(pFaction->GetExID());
		msg.Add(GetID());
		msg.Add(pRet);
		msg.Add(lValue);
		msg.Add(NULL_GUID);
	}
	msg.Send();
	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDARMYINVEST);
	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetFacTickets(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	char* pBaseName=GetStringParam(cmd,1);
	if (pBaseName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	LONG lTickets=pFaction->GetTickets(pBaseName);
	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pBaseName, sizeof(char)*MAX_VAR_LEN );
	return lTickets;
}

double CScript::GetBattleVal(const char* cmd,char* retstr)
{
	GameFaction *pFaction=NULL;
	CPlayer *pPlayer=NULL;
	char* szGuid=GetStringParam(cmd,0);
	if (szGuid==NULL)
		return 0;
	LONG lFlags=GetIntParam(cmd,1);
	//��ȡ��Ա��ս��ֵ
	if (lFlags>0)
	{
		if (stricmp("",szGuid)==0)
		{
			pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		else
		{
			pPlayer=GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid));
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return pFaction->GetBattle(pPlayer->GetExID());
	}
	//szGuid��ʾ����GUID,����Ϊ��
	else
	{
		if (stricmp("",szGuid)==0)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szGuid));
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return pFaction->GetBattle();
	}		
}

double CScript::GetArmyInvest(const char* cmd,char* retstr)
{
	GameFaction *pFaction=NULL;
	CPlayer *pPlayer=NULL;
	char* szGuid=GetStringParam(cmd,0);
	if (szGuid==NULL)
		return 0;
	LONG lFlags=GetIntParam(cmd,1);
	//��ȡ��Ա��ս��ֵ
	if (lFlags>0)
	{
		if (stricmp("",szGuid)==0)
		{
			pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		else
		{
			pPlayer=GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid));
			if(NULL == pPlayer)
			{
				M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		}
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return pFaction->GetArmyInvest(pPlayer->GetExID());
	}
	//szGuid��ʾ����GUID,����Ϊ��
	else
	{
		if (stricmp("",szGuid)==0)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szGuid));
		if (pFaction==NULL)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return pFaction->GetArmyInvest();
	}
}

double CScript::GetJoinBattle(const char* cmd,char* retstr)
{
	GameFaction *pFaction=NULL;
	CPlayer *pPlayer=NULL;
	char* szGuid=GetStringParam(cmd,0);
	if (szGuid==NULL)
		return 0;
	if (strlen(szGuid)==0)
	{
		pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		if(NULL == pPlayer)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	}
	else
	{
		pPlayer=GetGame()->FindPlayer(GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid));
		if(NULL == pPlayer)
		{
			M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	}
	if (pFaction==NULL)
	{
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	if (pFaction->GetJoin(pPlayer->GetExID()))
	{
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 1;
	}
	return 0;
}

double CScript::AddTech(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	LONG lTechNo=GetIntParam(cmd,1);
	LONG lLv=GetIntParam(cmd,2);
	char* pRet=GetStringParam(cmd,3);
	if (pRet==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	CMessage msg(MSG_S2W_Faction_ChangeTechLevel);
	msg.Add(pFaction->GetExID());
	msg.Add(GetID());
	msg.Add(pRet);
	msg.Add(lTechNo);
	msg.Add(lLv);
	msg.Send();

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDTECHLV);
	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetTechLv(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	LONG lTechNo=GetIntParam(cmd,1);
	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	return pFaction->GetTechLv(lTechNo);
}

double CScript::GetFactionName(const char* cmd,char* retstr)
{
	CPlayer* pPlayer = (dynamic_cast<CPlayer*>(p_SrcShape));
	if(NULL == pPlayer)
		return 0;
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	const CGUID& guid=GetVariableList()->GetGuidValue(szFacGuid);
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(guid);
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	char* szName=GetStringParam(cmd,1);
	if (szName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	SetScriptVarValue(pPlayer, szName, pFaction->GetName());
	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::ChangeEnemy(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	
	char* szFacName=GetStringParam(cmd,1);
	if (szFacName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	//���ô˽ű�����������磺10-21��lTime=1����1��10-22
	long lTime=GetIntParam(cmd,2);
	//���1��ɾ��0
	long lFlag=GetIntParam(cmd,3);
	long lBattleType=GetIntParam(cmd,4);
	if (lBattleType>=eMaxWar)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szFacName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	char* pRet=GetStringParam(cmd,5);
	if (pRet==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szFacName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	CMessage msg(MSG_S2W_Faction_ChangeEnemy);
	msg.Add(pFaction->GetExID());
	msg.Add(GetID());
	msg.Add(pRet);
	msg.Add(szFacName);
	msg.Add(lTime);
	msg.Add(lBattleType);
	msg.Add(lFlag);
	msg.Send();

	SetIsHang(true);
	SetHangFunc(SR_FUNC_ADDENEMY);

	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szFacName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetFacBaseBattle(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	char* cBaseName=GetStringParam(cmd,1);
	if (cBaseName==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	LONG lBaseBattle=pFaction->GetBaseBattle(cBaseName);
	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( cBaseName, sizeof(char)*MAX_VAR_LEN );
	return lBaseBattle;
}

double CScript::SendBaseInfo(const char* cmd,char* retstr)
{
	char* szFacGuid=GetStringParam(cmd,0);
	if (szFacGuid==NULL)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(GetScriptGuidValue((CMoveShape*)p_SrcShape,szFacGuid));
	if (pFaction==NULL)
	{
		M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	pFaction->SendBaseInfo();
	M_FREE( szFacGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetFacLevel(const char* cmd,char* retstr)
{
	CPlayer* pPlayer=NULL;
	GameFaction* pFaction=NULL;
	FACTION_PLAYER(pPlayer,pFaction);
	long lLevelType=GetIntParam(cmd,1);
	return pFaction->GetLevel((eUpgradeType)lLevelType);
}

double CScript::FacDisplay(const char* cmd,char* retstr)
{
	long lValue1=GetIntParam(cmd,0);
	long lValue2=GetIntParam(cmd,1);
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if (NULL==pPlayer)
		return 0;
	GameFaction* pFaction=GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if (pFaction==NULL)
		return 0;
	pFaction->FacDisplay(pPlayer->GetExID(),lValue1,lValue2);
	return 0;
}


//! ս���ŶӵĽ�����˳�
//! ����1����0=�μ� 0=�˳�
//! ����ֵ��ֵ��ʾ��Ϣת���Ƿ�ɹ�
double CScript::SentaiQueue(const char* cmd, char* retStr)
{
	long lValue1 = GetIntParam(cmd, 0);
	if(ERROR_CODE != lValue1)
	{
		CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
		if (NULL != pPlayer)
		{
			CMessage msg(MSG_S2W_SENTAI_REQUEST_QUEUE);
			msg.Add(pPlayer->GetExID());
			msg.Add(lValue1);
			msg.Send();

			return 1.0f;
		}
	}
	return 0.0f;
}

//! �����ҵ�ս�ӻ���
//! ����1��Ŀ����ҵ�GUID��Ϊ""ʱĬ��Ϊ��ǰ���
//! ����ֵ��0x09fffff9��ʾû��ȡ�û���
double CScript::GetPlayerSentaiPoints(const char* cmd, char* retStr)
{
	CPlayer* pAimPlayer = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
			pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		else
		{
			CGUID guid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			pAimPlayer = GetGame()->FindPlayer(guid);
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL == pAimPlayer)
		return ERROR_CODE;

	return pAimPlayer->GetSentaiPoints();
}


//! �޸���ҵ�ս�ӻ���
//! ����1��Ŀ����ҵ�GUID��Ϊ""ʱĬ��Ϊ��ǰ���
//! ����ֵ��������
double CScript::ChangePlayerSentaiPoints(const char* cmd, char* retStr)
{
	const CGUID *pPlayerGuid = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	long ChangeValue = GetIntParam(cmd, 1);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
		{
			CPlayer *pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL != pAimPlayer)
				pPlayerGuid = &(pAimPlayer->GetExID());
		}
		else
		{
			const CGUID &PlayerGuid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			if(NULL_GUID != PlayerGuid)
				pPlayerGuid = &PlayerGuid;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL != pPlayerGuid)
	{
		CMessage msg(MSG_S2W_SENTAI_REQUEST_CHANGE_POINT);
		msg.Add(*pPlayerGuid);
		msg.Add(ChangeValue);
		msg.Send();
	}

	return 0;
}

//! ��ȡ�����ս�ӻ���
//! ����1��Ŀ������GUID��Ϊ""ʱĬ��Ϊ��ǰ��Ҷ���
//! ����ֵ��0x09fffff9��ʾû��ȡ�û���
double CScript::GetTeamSentaiPoints(const char* cmd, char* retStr)
{
	const CGUID *pTeamGuid = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
		{
			CPlayer *pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL != pAimPlayer && NULL_GUID != pAimPlayer->GetTeamID())
				pTeamGuid = &(pAimPlayer->GetTeamID());
		}
		else
		{
			const CGUID &TeamGuid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			if(NULL_GUID != TeamGuid)
				pTeamGuid = &TeamGuid;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL != pTeamGuid)
	{
		GSTeam* pTeam = ((GSTeam*)(GetOrganiCtrl()->GetGameTeamByID(*pTeamGuid)));
		if(NULL != pTeam)
			return pTeam->GetSentaiPoints();
	}
	
	return ERROR_CODE;
}

#ifdef _DEBUG_SENTAI_
long GetEndID(void)
{
	static long IdSeed = 0;
	return IdSeed++;
}
#endif

//! ��������
//! ����1�������ĳ���ID
//! ����ֵ��������
double CScript::EndMatch(const char* cmd, char* retStr)
{
	long RegionID = GetIntParam(cmd, 0);
	if(ERROR_CODE != RegionID)
	{
#ifdef _DEBUG_SENTAI_
		long EndID = GetEndID();
		char DebugInfo[128] = {0};
		_snprintf(DebugInfo, 128, "MSG_S2W_SENTAI_END_MATCH gsid:%d flag:%d", GetGame()->GetIDIndex(), EndID);
		PutStringToFile("SentaiS2W", DebugInfo);
#endif

		CMessage msg(MSG_S2W_SENTAI_END_MATCH);
#ifdef _DEBUG_SENTAI_
		msg.Add(EndID);
#endif
		msg.Add(RegionID);
		msg.Send();
	}

	return 0.0f;
}

//! ����һ������ı���
//! ����1�����ID��Ϊ""��Ĭ��Ϊ��ǰ���
//! ����2�������ĳ���ID
//! ����ֵ��������
double CScript::EndTeamMatch(const char* cmd, char* retStr)
{
	const CGUID *pPlayerGuid = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
		{
			CPlayer *pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL != pAimPlayer)
				pPlayerGuid = &(pAimPlayer->GetExID());
		}
		else
		{
			const CGUID &PlayerGuid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			if(NULL_GUID != PlayerGuid)
				pPlayerGuid = &PlayerGuid;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL != pPlayerGuid)
	{
		// ����ֵ������
		long RegionID = GetIntParam(cmd, 1);
		if(ERROR_CODE != RegionID) 
		{
#ifdef _DEBUG_SENTAI_
			long EndID = GetEndID();
			char DebugInfo[128] = {0};
			_snprintf(DebugInfo, 128, "MSG_S2W_SENTAI_END_MATCH gsid:%d flag:%d", GetGame()->GetIDIndex(), EndID);
			PutStringToFile("SentaiS2W", DebugInfo);
#endif

			CMessage msg(MSG_S2W_SENTAI_END_TEAM_MATCH);
#ifdef _DEBUG_SENTAI_
			msg.Add(EndID);
#endif
			msg.Add(RegionID);
			msg.Add(*pPlayerGuid);
			msg.Send();
		}
	}

	return 0.0f;
}

//! ��ѯ����Ƿ���ս��ϵͳ��
//! ����1��Ŀ����ҵ�GUID��Ϊ""ʱĬ��Ϊ��ǰ���
//! ����2�����淵�ؽ���ı�����
//! ����ֵ��1=ִ�гɹ����������ķ������������� 0=ִ��ʧ��
double CScript::IsSentaiSys(const char* cmd, char* retStr)
{
	const CGUID *pPlayerGuid = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
		{
			CPlayer *pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
			if(NULL != pAimPlayer)
				pPlayerGuid = &(pAimPlayer->GetExID());
		}
		else
		{
			const CGUID &PlayerGuid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			if(NULL_GUID != PlayerGuid)
				pPlayerGuid = &PlayerGuid;
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL != pPlayerGuid)
	{
		// ����ֵ������
		char* szRet = GetStringParam(cmd, 1);
		if(szRet == NULL) 
			return 0;


		//CMessage msg(MSG_S2W_Faction_ChangeMemberContribute_Reques);
		CMessage msg(MSG_S2W_SENTAI_REQUEST_PLAYER_STATE);
		msg.Add(*pPlayerGuid);
		msg.Add(GetID());
		msg.Add(szRet);
		msg.Send(false);

		SetIsHang(true);
		SetHangFunc(SR_FUNC_ISSENTAISYS);

		M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
		return 1;
	}

	return 0;
}