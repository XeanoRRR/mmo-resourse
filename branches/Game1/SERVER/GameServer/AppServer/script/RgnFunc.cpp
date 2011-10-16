#include "StdAfx.h"
#include "mmsystem.h"
#include "script.h"
#include "variablelist.h"

#include "../player.h"
#include "../goods/cgoods.h"
#include "../serverregion.h"
#include "../monster.h"
#include "../../../setup/GlobeSetup.h"
#include "../../../setup/LogSystem.h"
#include "../Country/Country.h"
#include "../Country/CountryHandler.h"
#include "..\OrganizingSystem\GameOrganizingCtrl.h"
#include "..\OrganizingSystem\GSTeam.h"
#include "../RgnManager.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"


double CScript::GetRgnAttr(const char* cmd, char* retStr)
{
	long lRet = 0;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pRegion);
	if(!pRegion)
		return lRet;
	char *strAttrName=GetStringParam(cmd,0);
	if (strAttrName)
	{
		lRet = pRegion->GetAttribute(strAttrName);
		M_FREE( strAttrName, sizeof(char)*MAX_VAR_LEN );
	}
	return lRet;
}
double CScript::SetRgnAttr(const char* cmd, char* retStr)
{
	long lRet = 0;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pRegion);
	if(!pRegion)
		return lRet;
	char *strAttrName=GetStringParam(cmd,0);
	if (strAttrName)
	{
		double lNum=GetIntParam(cmd,1);
		if(lNum != ERROR_CODE)
		{
			 pRegion->SetAttribute(strAttrName,lNum);
		}
		M_FREE( strAttrName, sizeof(char)*MAX_VAR_LEN );
	}
	return lRet;
}
double CScript::GetCellType(const char* cmd, char* retStr)
{
	long lRet = 0;
	if(!m_pRegion)
		return lRet;
	long lTileX = GetIntParam(cmd,0);
	long lTileY = GetIntParam(cmd,1);
	if(lTileX == ERROR_CODE || lTileY== ERROR_CODE)
		return lRet;
	lRet = m_pRegion->GetSecurity(lTileX,lTileY);
	return lRet;
}

double CScript::RegionIsExit(const char* CmdStr, char* retStr)
{
	long lRet = 0;
	char* szGuid=GetStringParam(CmdStr,0);
	if (szGuid==NULL)
	{
		return 0;
	}

	const CGUID& RegionID = GetScriptGuidValue(dynamic_cast<CMoveShape*>(p_SrcShape),szGuid);
	if(RegionID == NULL_GUID)
	{
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CRegion* pRegion=NULL;
	pRegion = (CRegion*)GetGame()->FindRegion(RegionID);			
	if (pRegion)
		lRet = 1;
	else
	{
		CMessage msg(MSG_S2W_GM_QUERY_REGIONISEXIT);
		if(p_SrcShape)
			msg.Add(p_SrcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(RegionID);
		msg.Add(GetID());
		msg.Send();
		/*�ű���ִͣ��,�ȴ����ؽ��*/
		SetIsHang(true);
		SetHangFunc(SR_FUNC_REGIONISEXIT);
	}
	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}

double CScript::GetRegionID(const char* CmdStr, char* retStr)
{
	char* pRegionName=GetStringParam(CmdStr,0);
	if (pRegionName==NULL)
		return 0;

	char* szGuid=GetStringParam(CmdStr,1);
	if (szGuid==NULL)
	{
		M_FREE( pRegionName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CRegion* pRegion=NULL;
	pRegion = (CRegion*)GetGame()->FindRegion(pRegionName);			
	if (pRegion)
		SetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid, pRegion->GetExID());
	else
	{
		CMessage msg(MSG_S2W_GM_QUERY_FINDREGION);
		if(p_SrcShape)
			msg.Add(p_SrcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(pRegionName);
		msg.Add(GetID());
		msg.Send();
		/*�ű���ִͣ��,�ȴ����ؽ��*/
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETREGIONID);
	}
	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pRegionName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetMapInfo(const char* CmdStr, char* retStr)
{
	CRegion* pRegion = GetRegion();
	if( pRegion )
	{
		double lX	= GetIntParam( CmdStr, 0 );
		double lY	= GetIntParam( CmdStr, 1 );
		if( lX == ERROR_CODE || lY == ERROR_CODE ) return -1;
		CRegion::tagCell* pCell = pRegion -> GetCell( lX, lY );
		if( pCell )
		{
			/**## ����ǹ�ս�����ͷ���2���������ȼ���� */
			if( pCell -> lWar == 1 ) return 2;
			/**## ����ǰ�ȫ��������3 */
			else if( pRegion -> GetSecurity(lX,lY)== CRegion::SECURTIY_SAFE  ) return 3;
			/**## ����Ǿ�����������1 */
			else if( pRegion -> GetSecurity(lX,lY) == CRegion::SECURTIY_FIGHT ) return 1;
			/**## ����������ϵģ���Ĭ��Ϊ��ͨPK��������0 */
			else return 0;
		} 
	} 
	/**## �������ͷ���-1 */
	return -1;
}

double CScript::GetCountryOwningRegion(const char* CmdStr, char* retStr)
{
	double lRet = 0;
	return lRet;
}


double CScript::SetBlock(const char* CmdStr, char* retStr)
{
	char* szID = GetStringParam(CmdStr,0);
	if(!szID) return 0;

	CGUID lRegionID(szID);

	double x = GetIntParam(CmdStr,1);
	double y = GetIntParam(CmdStr,2);
	double block = GetIntParam(CmdStr,3);

	if (lRegionID==NULL_GUID)
	{
		M_FREE( szID, sizeof(char)*MAX_VAR_LEN );
		return -1;
	}

	if (block<CRegion::BLOCK_NO || block > CRegion::BLOCK_SHAPE)
	{
		M_FREE( szID, sizeof(char)*MAX_VAR_LEN );
		return -1;
	}

	CServerRegion* pServerRegion = GetGame()->FindRegion(lRegionID);
	if (pServerRegion)
	{
		pServerRegion->SetBlock(x,y, block);
	}

	M_FREE( szID, sizeof(char)*MAX_VAR_LEN );
	return 1;
}
double CScript::GetRegionRandomPos(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	char* szID = GetStringParam(CmdStr,0);

	CGUID lRegionID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szID);

	if (lRegionID == NULL_GUID)
	{
		lRegionID = p_SrcShape->GetRegionGUID();
	}

	CServerRegion* pRegion = GetGame()->FindRegion(lRegionID);
	if (pRegion)
	{
		long x = 0, y = 0;
		pRegion->GetRandomPos(x, y);

		((CPlayer*)p_SrcShape)->GetVariableList()->SetVarValue("$m_Temp", 0, x);
		((CPlayer*)p_SrcShape)->GetVariableList()->SetVarValue("$m_Temp", 1, y);
		M_FREE( szID, sizeof(char)*MAX_VAR_LEN );
		return 1;
	}

	M_FREE( szID, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetRandomPosInRange(const char* CmdStr, char* retStr)
{
	double rgnID = GetIntParam( CmdStr, 0 );
	
	CServerRegion* pRegion = NULL;
	pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, rgnID);
	if(pRegion == NULL)
		pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_PERSONAL, rgnID);
	if(pRegion == NULL)
		pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_PERSONAL_HOUSE, rgnID);
	if(pRegion == NULL)
		pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_TEAM, rgnID);
	
	if(!pRegion)
	{
		pRegion = static_cast<CServerRegion*>( this->p_SrcShape->GetFather() );
	}

	if( pRegion == NULL )
	{
		return -1;
	}

	int x, y, w, h;
	long lPosX = 0, lPosY = 0;

	x = GetIntParam( CmdStr, 1 );
	y = GetIntParam( CmdStr, 2 );
	w = GetIntParam( CmdStr, 3 );
	h = GetIntParam( CmdStr, 4 );

	if( x == ERROR_CODE || y == ERROR_CODE || w == ERROR_CODE || h == ERROR_CODE )
	{
		return -1;
	}

	pRegion->GetRandomPosInRange( lPosX, lPosY, x, y, w, h );

	char *strPosX = GetStringParam( CmdStr, 5 );
	char *strPosY = GetStringParam( CmdStr, 6 );
	int ret = CVariableList::VAR_NOT_FOUND;
	ret = SetScriptVarValue( static_cast<CMoveShape*>( p_SrcShape ), strPosX, 0, lPosX );
	ret = SetScriptVarValue( static_cast<CMoveShape*>( p_SrcShape ), strPosY, 0, lPosY ) == CVariableList::VAR_NOT_FOUND ? 
		CVariableList::VAR_NOT_FOUND : ret;

	M_FREE( strPosX, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strPosY, sizeof(char)*MAX_VAR_LEN );

	return ret == CVariableList::VAR_NOT_FOUND;
}

double CScript::GetLineDir( const char *cmd, char *retStr )
{
	double sx = GetIntParam( cmd, 0 );
	double sy = GetIntParam( cmd, 1 );
	double dx = GetIntParam( cmd, 2 );
	double dy = GetIntParam( cmd, 3 );
	
	if( sx == ERROR_CODE || sy == ERROR_CODE || dx == ERROR_CODE || dy == ERROR_CODE )
	{
		return -1;
	}

	long dir = ::GetLineDir( sx, sy, dx, dy );

	return dir;
}

double CScript::GetNextPosByDir( const char *cmd, char *retStr )
{
	double dir = GetIntParam( cmd, 0 );
	double posX = GetIntParam( cmd, 1 );
	double posY = GetIntParam( cmd, 2 );
	double step = GetIntParam( cmd, 3 );

	// pos table
	int post[8][2] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, -1,-1};
	int destX = posX + post[(int)dir][0] * step;
	int destY = posY + post[(int)dir][1] * step;

	char *retPosX = GetStringParam( cmd, 4 );
	char *retPosY = GetStringParam( cmd, 5 );
	SetScriptVarValue( static_cast<CMoveShape*>( p_SrcShape ), retPosX, 0, destX );
	SetScriptVarValue( static_cast<CMoveShape*>( p_SrcShape ), retPosY, 0, destY );

	M_FREE( retPosX, sizeof(char)*MAX_VAR_LEN );
	M_FREE( retPosY, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::GetRegionPlayerNum(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	char* strRgnID = GetStringParam(CmdStr,0);
	if(!strRgnID) return -1.0f;

	char* szRetValue = GetStringParam(CmdStr, 1);
	if(!szRetValue) 
	{
		M_FREE( strRgnID, sizeof(char)*MAX_VAR_LEN );
		return -1.0f;
	}

	CGUID rgnGuid;
	if(strRgnID[0] == '\0') // ��ǰ����
	{
		if(GetSrcShape())
			rgnGuid = GetSrcShape()->GetRegionGUID();
	}
	else
		rgnGuid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), strRgnID);
	if(rgnGuid == NULL_GUID)
	{
		M_FREE( strRgnID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRetValue, sizeof(char)*MAX_VAR_LEN );
		return -1.0f;
	}

	CServerRegion* pRegion = NULL;
	pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnGuid);
	if(pRegion)
	{
		SetScriptVarValue((CMoveShape*)GetSrcShape(), szRetValue, 0, pRegion->GetPlayerAmout());
		M_FREE( strRgnID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRetValue, sizeof(char)*MAX_VAR_LEN );
		return 1.0f;
	}
	else // ��Ҫ���������ѯ
	{
		//�ȴ�����, �յ���Ӧ��Ϣ�����нű�
		SetIsHang(true);
		SetHangFunc(SR_FUNC_VIEWGSPLAYERNUM);
		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_VIEWGSPLAYERNUM);
		msg.Add(rgnGuid);
		msg.Add(p_SrcShape->GetExID()); // �鿴��GUID
		msg.Add(GetID()); // script id
		msg.Add(szRetValue);
		msg.Send();
	}
	M_FREE( strRgnID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetValue, sizeof(char)*MAX_VAR_LEN );
	return -1.0f;
}

double CScript::RunScriptForAllPlayerInRgn(const char* CmdStr, char* retStr)
{
	if( m_pRegion != 0 )
	{
		char* strPath = GetStringParam(CmdStr,0);
		static_cast<CServerRegion*>( m_pRegion )->RunScriptAllPlayer(strPath);
		M_FREE( strPath, sizeof(char)*MAX_VAR_LEN );
	}
	return 0;
}

//���ض���ͼ����һ�������������ظø����ģɣģ�����򸱱������Ѵﵽ�����򴴽�ʧ�ܱ�����0��
double CScript::CreatePRgn(const char* CmdStr, char* retStr)
{
	//�ȴ�����, �յ���Ӧ��Ϣ�����нű�
	SetIsHang(true);
	SetHangFunc(SR_FUNC_CREATEPRGN);

	char* szPGUID = GetStringParam(CmdStr, 0);
	if(szPGUID == NULL) return 0;

	CGUID pGuid = NULL_GUID;
	pGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPGUID);
	CPlayer* player = GetGame()->FindPlayer(pGuid);
	if(player == NULL)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// �첽��������RgnID
	char* szRetRgnGuid = GetStringParam(CmdStr, 1);

	if(szRetRgnGuid == NULL)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}


	double templateID = GetIntParam(CmdStr,2);

	//wq:2008-7-9,��Ҫ�޸�
	player->SetCreateDupRgnType(RGN_PERSONAL);
	player->SetlCreateDupRgnTID(templateID);
	player->SetlCreateDupRgnScriptID(this->GetID());

	// ����ֵ�����������첽�����Ľ��
	char* szRetVarName = GetStringParam(CmdStr, 3);
	if(szRetVarName == NULL) 
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long lRetFlag = GameManager::GetInstance()->GetRgnManager()->CreateRgnMsg(this, templateID, RGN_PERSONAL, player->GetExID(), szRetVarName, szRetRgnGuid); // ��WS ������Ϣ
	if(lRetFlag < 0)
		SetIsHang(false);

	M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetVarName, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
double CScript::CreatePHRgn(const char* CmdStr, char* retStr)
{
	char* szPGUID = GetStringParam(CmdStr, 0);
	if(szPGUID == NULL) return 0;

	CGUID pGuid = NULL_GUID;
	pGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPGUID);
	CPlayer* player = GetGame()->FindPlayer(pGuid);
	if(player == NULL)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// ������з��ݸ���
	if(player->GetPersonalHouseRgnGUID() != NULL_GUID)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	//�ȴ�����, �յ���Ӧ��Ϣ�����нű�
	SetIsHang(true);
	SetHangFunc(SR_FUNC_CREATEPHRGN);

	// �첽��������RgnID
	char* szRetRgnGuid = GetStringParam(CmdStr, 1);
	if(szRetRgnGuid == NULL) 
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	double templateID = GetIntParam(CmdStr, 2);

	player->SetCreateDupRgnType(RGN_PERSONAL_HOUSE);
	player->SetlCreateDupRgnTID(templateID);
	player->SetlCreateDupRgnScriptID(this->GetID());	

	// ����ֵ�����������첽�����Ľ��
	char* szRetVarName = GetStringParam(CmdStr, 3);
	if(szRetVarName == NULL)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long lRetFlag = GameManager::GetInstance()->GetRgnManager()->CreateRgnMsg(this, templateID, RGN_PERSONAL_HOUSE, player->GetExID(), szRetVarName, szRetRgnGuid); // ��WS ������Ϣ
	if(lRetFlag < 0)
		SetIsHang(false);

	M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetVarName, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}

double CScript::CreateTRgn(const char* CmdStr, char* retStr)
{
	//�ȴ�����, �յ���Ӧ��Ϣ�����нű�
	SetIsHang(true);
	SetHangFunc(SR_FUNC_CREATETRGN);

	char* szPGUID = GetStringParam(CmdStr, 0);
	if(szPGUID == NULL) return 0;

	CGUID pGuid = NULL_GUID;
	pGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPGUID);
	CPlayer* player = GetGame()->FindPlayer(pGuid);
	if(player == NULL)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// �첽��������RgnID
	char* szRetRgnGuid = GetStringParam(CmdStr, 1);
	if(szRetRgnGuid == NULL) 
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	double templateID = GetIntParam(CmdStr, 2);

	
	//wq:2008-7-9,��Ҫ�޸�
	// ������ҷ����Ŷ�,map�Զ���long��ʱ��ֵ����,С����ǰ��		
	player->SetCreateDupRgnType(RGN_TEAM);
	player->SetlCreateDupRgnTID(templateID);
	player->SetlCreateDupRgnScriptID(this->GetID());	
	
	// ����ֵ�����������첽�����Ľ��
	char* szRetVarName = GetStringParam(CmdStr, 3);
	if(szRetVarName == NULL)
	{
		M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long lRetFlag = GameManager::GetInstance()->GetRgnManager()->CreateRgnMsg(this, templateID, RGN_TEAM, player->GetExID(), szRetVarName, szRetRgnGuid); // ��WS ������Ϣ
	if(lRetFlag < 0)
		SetIsHang(false);

	M_FREE( szPGUID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetVarName, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
// ȡ�õ�ǰ�ű���Ҹ��˸���GUID	GetCurPlayerPDupRgnID(@Ret);
double CScript::GetCurPlayerPDupRgnID	(const char* CmdStr, char* retStr)
{
	int ret = 0;
	char* szRetRgnGuid = GetStringParam(CmdStr, 0);
	if(!szRetRgnGuid) return 0;

	CPlayer* player = (CPlayer*)p_SrcShape;

	if(player)
	{
		const CGUID& sGuid = player->GetPersonalRgnGUID();
		ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetRgnGuid, sGuid);
	}

	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return (double)ret;
}
//ȡ�õ�ǰ�ű���Ҹ��˷��ݸ���GUID	GetCurPlayerPHDupRgnID(@Ret);
double CScript::GetCurPlayerPHDupRgnID	(const char* CmdStr, char* retStr)
{
	int ret = 0;
	char* szRetRgnGuid = GetStringParam(CmdStr, 0);
	if(!szRetRgnGuid) return 0;

	CPlayer* player = (CPlayer*)p_SrcShape;

	if(player)
	{
		const CGUID& sGuid = player->GetPersonalHouseRgnGUID();
		ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetRgnGuid, sGuid);
	}

	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return (double)ret;
}


double CScript::GetPlayerPDupRgnID(const char* CmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(CmdStr, 0);
	if(!szPlayerGuid) return 0;

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);
	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	int ret = 0;
	char* szRetRgnGuid = GetStringParam(CmdStr, 1);
	if(szRetRgnGuid == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	
	if(player)
	{
		const CGUID& sGuid = player->GetPersonalRgnGUID();
		ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetRgnGuid, sGuid);
	}
	else
	{
		CMessage msg(MSG_S2W_RGN_VIEWPLAYERPID); // �鿴��ҵĸ��˸���
		msg.Add(sPGuid);
		if(szRetRgnGuid)
			msg.Add(szRetRgnGuid);
		else
			msg.Add("");
		msg.Add(GetID());
		msg.Send();
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETPLAYERPDUPRGNID);
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return (double)ret;
}
double CScript::GetPlayerPHDupRgnID(const char* CmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(CmdStr, 0);
	if(!szPlayerGuid) return 0;

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);
	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	int ret = 0;
	char* szRetRgnGuid = GetStringParam(CmdStr, 1);
	if(szRetRgnGuid == NULL)
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if(player)
	{
		const CGUID& sGuid = player->GetPersonalHouseRgnGUID();
		int ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetRgnGuid, sGuid);
	}
	else
	{
		CMessage msg(MSG_S2W_RGN_VIEWPLAYERPHID); // �鿴��ҵĸ��˷��ݸ���
		msg.Add(sPGuid);
		if(szRetRgnGuid)
			msg.Add(szRetRgnGuid);
		else
			msg.Add("");
		msg.Add(GetID());
		msg.Send();
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETPLAYERPHDUPRGNID);
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return (double)ret;
}
double CScript::GetTeamDupRgnID(const char* CmdStr, char* retStr)	
{
	// ����player
	char* szPlayerGuid = GetStringParam(CmdStr, 0);
	if(!szPlayerGuid) return 0;

	LONG lRgnID = GetIntParam(CmdStr, 1);

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);

	char* szRetRgnGuid = GetStringParam(CmdStr, 2);
	if(szRetRgnGuid == NULL)
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	int ret = 0;
	if(sPGuid != NULL_GUID)
	{
		const CGUID& sGuid = GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGUID(sPGuid, lRgnID);
		int ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetRgnGuid, sGuid);
	}
	else
	{
		CMessage msg(MSG_S2W_RGN_VIEWPLAYERTID); // �鿴��ҵ���Ӹ���
		msg.Add(sPGuid);
		msg.Add(lRgnID);
		if(szRetRgnGuid)
			msg.Add(szRetRgnGuid);
		else
			msg.Add("");
		msg.Add(GetID());
		msg.Send();
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETTEAMDUPRGNID);
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return (double)ret;
}
double CScript::GetNormalDupRgnID(const char* CmdStr, char* retStr)
{
	long rgnTID = (long)GetIntParam(CmdStr,0);
	if( rgnTID == ERROR_CODE )
	{
		return 0;
	}

	// ����Rgn
	char* szGuid = GetStringParam(CmdStr, 1);
	if(szGuid == NULL) return 0;

	CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, rgnTID);
	if(pRgn)
	{
		int ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid, pRgn->GetExID());
	}
	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}

double CScript::ChangeDupRgn(const char* cmdStr, char* retStr)
{
	CPlayer* player = (CPlayer*)p_SrcShape;
	if(player == NULL) return 0;

	// ����RgnID
	char* szRgnGuid = GetStringParam(cmdStr, 0);
	if(szRgnGuid == NULL) return 0;

	// rgn type
	long rType = GetIntParam(cmdStr, 1);
	if(rType == ERROR_CODE) 
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// Rgn GUID
	CGUID RgnGuid;
	RgnGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);
	long ret = 0;
	if(player)
	{
		long x = GetIntParam(cmdStr, 2);
		if(x == ERROR_CODE) x = -1;
		long y = GetIntParam(cmdStr, 3);
		if(y == ERROR_CODE) y = -1;

		ret = player->ChangeRegion((eRgnType)rType, RgnGuid, x, y, 0);
	}

	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
// �л�����byģ��id
double CScript::ChangeRgnByTemplateID (const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer) return 0;

	double templateID = GetIntParam(CmdStr,0);

	if( templateID == ERROR_CODE )
	{
		return 0;
	}

	//����ServerRegion  GUID
	CGUID rgnGUID = NULL_GUID;
	CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, templateID);
	if(pRgn)
		rgnGUID = pRgn->GetExID();
	
	double x = GetIntParam(CmdStr,1);
	double y = GetIntParam(CmdStr,2);
	double dir = GetIntParam(CmdStr,3);
	double lRange = GetIntParam(CmdStr, 4);
	double lCarriageDistance = GetIntParam(CmdStr,5);

	if( x == ERROR_CODE || y == ERROR_CODE )
	{
		x = y = -1;
		dir = pPlayer->GetDir();
	}
	if( dir == ERROR_CODE )
	{
		dir = pPlayer->GetDir();
	}
	if (lRange==ERROR_CODE)
	{
		lRange = 2;
	}
	if (lCarriageDistance==ERROR_CODE)
	{
		lCarriageDistance = 0;
	}
	pPlayer->ChangeRegion(RGN_NORMAL, rgnGUID, x, y, dir, templateID, 0,lRange, lCarriageDistance);
	return 0;
}
// ��ӳ�������
double CScript::AddRgnVar(const char* cmdStr, char* retStr)
{
	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	// ����player
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(!szRgnGuid) return 0;

	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);

	if(sPGuid == NULL_GUID)
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	CMessage msg(MSG_S2W_RGN_VAR_ADD);
	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Add(GetID());
	msg.Add(rgnType); // leixing

	// ����ָ��
	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(sPGuid);

	// ��Ҫ�첽����
	if(NULL == rgn)
	{
		SetIsHang(true);
		SetHangFunc(SR_FUNC_ADDRGNVAR);
	}

	// RgnID
	msg.Add(sPGuid);

	// ȡ��һ�������� �������ַ���
	char* firstVarName = NULL;
	firstVarName = GetStringParam(cmdStr, 2);
	if(firstVarName)
	{
		if( (firstVarName[0] != '#' && firstVarName[0] != '@' && firstVarName[0] != '$')  )
		{
			TestLog(CStringReading::LoadString(IDS_GS_RGNFUNC, STR_GS_RGNFUNC_ADDRGNVARERR));
			M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		msg.Add(firstVarName);

		// ��ȡ�ַ���ֵ
		if(firstVarName[0] == '#') // �ַ���
		{
			char* strValue = NULL;
			strValue = GetStringParam(cmdStr, 3);

			if(m_pVariableList && strValue)
			{	
				if(rgn != NULL)
				{
					rgn->GetVariableList()->AddVar(firstVarName, strValue);
					long size = (long)strlen(strValue);	
					msg.Add(size);
					msg.AddEx((void*)strValue, size);
				}
			}
			M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
		}
		else if(firstVarName[0] == '$') //����
		{
			double value = GetIntParam(cmdStr, 3);

			if(m_pVariableList && (value != ERROR_CODE) )
			{
				if(rgn != NULL)
					rgn->GetVariableList()->AddVar(firstVarName, value);
				{
					msg.Add((long)sizeof(double));
					msg.AddEx((void*)&value, sizeof(double));
				}
			}
		}

		else if(firstVarName[0] == '@') // ����GUID
		{
			char* strValue = NULL;
			strValue = GetStringParam(cmdStr, 3);

			if(strValue)
			{
				CGUID tGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, strValue);
				if(rgn != NULL)
					rgn->GetVariableList()->AddGuid(firstVarName, tGUID);
				msg.Add(tGUID);
				M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
			}
		}

		M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
	}

	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Send();
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
// ���ó�������
double CScript::SetRgnVar(const char* cmdStr, char* retStr)
{
	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	// ���� rgn
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	// rgn GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);

	if(sPGuid == NULL_GUID) 
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	CMessage msg(MSG_S2W_RGN_VAR_SET);
	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Add(GetID());
	msg.Add(rgnType); // leixing

	// ����ָ��
	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(sPGuid);

	// ��Ҫ�첽����
	if(NULL == rgn)
	{
		SetIsHang(true);
		SetHangFunc(SR_FUNC_SETRGNVAR);
	}

	msg.Add(sPGuid);
	
	// ȡ��һ�������� �������ַ���
	char* firstVarName = NULL;
	firstVarName = GetStringParam(cmdStr, 2);

	if(firstVarName)
	{
		if( (firstVarName[0] != '#' && firstVarName[0] != '@' && firstVarName[0] != '$')  )
		{
			TestLog(CStringReading::LoadString(IDS_GS_RGNFUNC, STR_GS_RGNFUNC_SETRGNVARERR));
			M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		msg.Add(firstVarName);

		// ��ȡ�ַ���ֵ
		if(firstVarName[0] == '#') // �ַ���
		{
			char* strValue = NULL;
			strValue = GetStringParam(cmdStr, 3);

			if(m_pVariableList && strValue)
			{
				if(rgn != NULL)
					rgn->GetVariableList()->SetVarValue(firstVarName, strValue);
				long size = (long)strlen(strValue);
				msg.Add(size);
				msg.AddEx((void*)strValue, size);				
			}
			M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
		}
		else if(firstVarName[0] == '$') //����
		{
			double value = GetIntParam(cmdStr, 3);

			if(m_pVariableList && (value != ERROR_CODE) )
			{
				if(rgn != NULL)
					rgn->GetVariableList()->SetVarValue(firstVarName, 0, value);
				msg.Add((long)sizeof(double));
				msg.AddEx((void*)&value, sizeof(double));
			}
		}
		else if(firstVarName[0] == '@') // ����GUID
		{
			char* strValue = NULL;
			strValue = GetStringParam(cmdStr, 3);

			if(strValue)
			{
				CGUID tGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, strValue);
				if(rgn != NULL)
					rgn->GetVariableList()->SetGuidValue(firstVarName, &tGUID);
				msg.Add(tGUID);
				M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			}
		}

		M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
	}

	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Send();
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
// ɾ����������
double CScript::DelRgnVar(const char* cmdStr, char* retStr)
{
	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	// ����player
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(!szRgnGuid) return 0;

	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);
	
	if(sPGuid == NULL_GUID)
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	CMessage msg(MSG_S2W_RGN_VAR_DEL);
	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Add(GetID());
	msg.Add(rgnType); // leixing

	// ����ָ��
	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(sPGuid);

	// ��Ҫ�첽����
	if(NULL == rgn)
	{
		SetIsHang(true);
		SetHangFunc(SR_FUNC_DELRGNVAR);
	}

	msg.Add(sPGuid);

	// ȡ��һ�������� �������ַ���
	char* firstVarName = NULL;
	firstVarName = GetStringParam(cmdStr, 2);

	if(firstVarName)
	{
		if( (firstVarName[0] != '#' && firstVarName[0] != '@' && firstVarName[0] != '$')  )
		{
			TestLog(CStringReading::LoadString(IDS_GS_RGNFUNC, STR_GS_RGNFUNC_DELRGNVARERR));
			M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		// ��ȡ�ַ���ֵ
		if(firstVarName[0] == '#') // �ַ���
		{
			if(m_pVariableList)
			{
				if(rgn != NULL)
					rgn->GetVariableList()->RemoveVar(firstVarName);
				msg.Add(firstVarName);
			}
		}
		else if(firstVarName[0] == '$') //����
		{
			if(m_pVariableList)
			{
				if(rgn != NULL)
					rgn->GetVariableList()->RemoveVar(firstVarName);
				msg.Add(firstVarName);	
			}
		}
		else if(firstVarName[0] == '@') // ����GUID
		{
			if(rgn != NULL)
				rgn->GetVariableList()->RemoveGuid(firstVarName);
			msg.Add(firstVarName);
		}

		M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
	}

	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Send();
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
// ȡ��ָ����������
double CScript::GetRgnVar(const char* cmdStr, char* retStr)
{
	double ret = 0;

	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	// ����rgn
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(!szRgnGuid) return 0;

	// rgn GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);

	if(sPGuid == NULL_GUID) 
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	CMessage msg(MSG_S2W_RGN_VAR_GET);
	msg.Add((BYTE)GetGame()->GetIDIndex());
	msg.Add(GetID()); // script id
	msg.Add(rgnType); // leixing

	// ����ָ��
	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(sPGuid);
	if(NULL == rgn)
	{
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETRGNVAR);
	}
	msg.Add(sPGuid);

	// ȡ��һ�������� �������ַ���
	char* firstVarName = NULL;
	firstVarName = GetStringParam(cmdStr, 2);

	if(firstVarName)
	{
		if( (firstVarName[0] != '#' && firstVarName[0] != '@' && firstVarName[0] != '$')  )
		{
			TestLog(CStringReading::LoadString(IDS_GS_RGNFUNC, STR_GS_RGNFUNC_GETRGNVARERR));
			M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		// ��ȡ�ַ���ֵ
		if(firstVarName[0] == '#') // �ַ���
		{
			// ����ֵ������
			char* strValue = GetStringParam(cmdStr, 3);

			if(m_pVariableList)
			{
				if(rgn != NULL)
				{
					if(strValue)
					{
						char* strRet = (char*)rgn->GetVariableList()->GetVarValue(firstVarName);
						SetScriptVarValue((CMoveShape*)p_SrcShape, strValue, strRet);
					}
				}
				else
				{
					if(strValue)
						msg.Add(firstVarName);
					else
						msg.Add("");
					msg.Add(strValue); // ��ӷ���ֵ�ַ���������
				}
			}
			M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
		}
		else if(firstVarName[0] == '$') //����
		{
			char* strValue = GetStringParam(cmdStr, 3);

			if(m_pVariableList && (strValue != NULL) )
			{
				if(rgn != NULL)
				{
					if(strValue)
					{
						ret = rgn->GetVariableList()->GetVarValue(firstVarName, 0);
						SetScriptVarValue((CMoveShape*)p_SrcShape, strValue, 0, ret);
					}
				}
				else
				{
					if(strValue)
						msg.Add(firstVarName);
					else
						msg.Add("");
					msg.Add(strValue); // ��ӷ���ֵ�ַ���������
				}
			}
			M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
		}

		else if(firstVarName[0] == '@') // ����GUID
		{
			char* strValue = GetStringParam(cmdStr, 3);

			if(rgn != NULL)
			{
				CGUID oGUID;
				if(strValue)
				{
					oGUID = rgn->GetVariableList()->GetGuidValue(firstVarName);
					SetScriptGuidValue((CMoveShape*)p_SrcShape, strValue, oGUID);
				}
			}
			else
			{
				if(strValue)
					msg.Add(firstVarName);
				else
					msg.Add("");
				msg.Add(strValue); // ��ӷ���ֵ�ַ���������
			}

			M_FREE( strValue, sizeof(char)*MAX_VAR_LEN );
		}
		M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
	}
	if(!rgn)
	{
		msg.Send();
	}
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
//ɾ��ָ�������ɣĵĸ�����
double CScript::DeleteRgn(const char* cmdStr, char* retStr)
{
	// GSID
//	long GSID = (long)GetIntParam(cmdStr, 0);
	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);
	

	// GUID������
	char* guid = GetStringParam(cmdStr, 1);
	if(guid == NULL || guid[0] != '@') 
	{
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnGUID;
	// ����GUID
	if(p_SrcShape)
		rgnGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);

	char* szRetVarName = GetStringParam(cmdStr, 2);
	if(!szRetVarName) 
	{
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	double dRet = 0.0f;

	//wq:2008-7-9,��Ҫ�޸�
	dRet = GameManager::GetInstance()->GetRgnManager()->DeleteRgnMsg(this->GetID(), (eRgnType)rgnType, NULL_GUID, rgnGUID, szRetVarName, guid);
	// ����script���ȴ�WS�Ļ�Ӧ��Ȼ��ִ��ɾ������
	SetIsHang(true);
	SetHangFunc(SC_FUNC_DELETERGN);
	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetVarName, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

//����ָ�������ɣĵĸ�������ͼ�ɣģ���ø����ɣĵĸ����������򷵻�0��
double CScript::GetTemplateIDByGUID	(const char* cmdStr, char* retStr)
{
	double ret = -1.0f;

	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);
	if(rgnType == ERROR_CODE) return 0;

	// GUID������
	char* guid = GetStringParam(cmdStr, 1);
	if(guid == NULL) return 0;

	// ����ֵ������
	char* szRet = GetStringParam(cmdStr, 2);
	if(szRet == NULL) 
	{
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnGUID;
	// ����GUID
	if(p_SrcShape)
		rgnGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);
	
	CServerRegion* findRgn = NULL;
	switch((eRgnType)rgnType)
	{
	case RGN_PERSONAL:
		{
			CRgnManager::RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().find(rgnGUID);
			if(itr != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end())
			{
				findRgn = itr->second;
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			CRgnManager::RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().find(rgnGUID);
			if(itr != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
			{
				findRgn = itr->second;
			}
		}
		break;
	case RGN_TEAM:
		{
			CRgnManager::RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().find(rgnGUID);
			if(itr != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end())
			{
				findRgn = itr->second;
			}
		}
		break;
	case RGN_NORMAL:
		{
			CRgnManager::RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().find(rgnGUID);
			if(itr != GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end())
			{
				findRgn = itr->second;
			}
		}
		break;
	}

	//wq:2008-7-9,��Ҫ�޸�
	if(findRgn != NULL) // ����Rgn
	{
		SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, 0, (double)findRgn->GetID());
	}
	else
	{
		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_GETTEMPLATEID);
		if(p_SrcShape)
			msg.Add(p_SrcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add((long)rgnType);
		msg.Add(rgnGUID);
		msg.Add(szRet);
		msg.Add(GetID());
		msg.Send();
		SetIsHang(true);
		SetHangFunc(SC_FUNC_GETMPLATEIDBYGUID);
	}
	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//���ص�ǰ���ڳ����ģɣģ�����ó����������򷵻�0��
double CScript::GetCurRgnGUID(const char* cmdStr, char* retStr)
{
	double ret = 0.0f;
	char* guid = NULL;

	if((CPlayer*)p_SrcShape)
	{
		// GUID������
		guid = GetStringParam(cmdStr, 0);
		if(guid == NULL) return 0;

		// ����GUID
		int sRet = SetScriptGuidValue((CMoveShape*)p_SrcShape, guid, m_guRegionID );
		if(sRet != CVariableList::VAR_NOT_FOUND)
			ret = 1.0f;
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//����ָ�������ɣĵĸ����е�ǰ������������ԭGetRegionPlayerNum��
//����ָ�������ɣĵĸ��������͡�
double CScript::GetDupTypeByGUID(const char* cmdStr, char* retStr)
{
	double ret = -1.0f;

	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(NULL == guid) return 0;

	// ����ֵ������
	char* szRet = GetStringParam(cmdStr, 1);
	if(NULL == szRet) 
	{
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnGUID;
	// ����GUID
	if(p_SrcShape)
		rgnGUID=GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);

	CRgnManager::RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().find(rgnGUID);
	if(itr != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end())
	{
		SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, 0, (double)RGN_PERSONAL);
		ret = 0.0f;
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
		return ret;
	}

	if(itr == GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end())
	{
		itr = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().find(rgnGUID);
		if(itr != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
		{
			SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, 0, (double)RGN_PERSONAL_HOUSE);
			ret = 0.0f;
			M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
			return ret;
		}
	}

	if(itr == GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
	{
		itr = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().find(rgnGUID);
		if(itr != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end())
		{
			SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, 0, (double)RGN_TEAM);
			ret = 0.0f;
			M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
			return ret;
		}
	}

	if(itr == GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end())
	{
		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_GETDUPTYPE);
		if(p_SrcShape)
			msg.Add(p_SrcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(rgnGUID);
		msg.Add(szRet);
		msg.Add(GetID());
		msg.Send();
		SetIsHang(true);
		SetHangFunc(RGN_FUNC_GETDUPTYPEBYGUID);
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//����ָ�������ɣĵĸ����������ߣɣġ�
double CScript::GetDupRgnOnwerGUID	(const char* cmdStr, char* retStr)
{
	double ret = 0.0f;

	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(guid == NULL) return 0;

	char* ownerGuid = GetStringParam(cmdStr, 1);
	if(ownerGuid == NULL)
	{
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnGUID;
	// ����GUID
	rgnGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);
	CServerRegion* sRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnGUID);
	if(sRgn)
		SetScriptGuidValue((CMoveShape*)p_SrcShape, ownerGuid, sRgn->GetOwnerGUID());
	else
	{
		// ������Ϣ��WS
		CMessage msg(MSG_S2W_RGN_GETDUPRGNONWER);
		if(p_SrcShape)
			msg.Add(p_SrcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(rgnGUID);
		msg.Add(ownerGuid);
		msg.Add(GetID());
		msg.Send();
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETDUPRGNONWERGUID);
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( ownerGuid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//����ָ�������ɣĵĸ������������ޡ�
double CScript::GetDupRgnPlayerMaxNum(const char* cmdStr, char* retStr)
{
	double ret = 0.0f;

	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(guid == NULL) return 0;

	if(guid)
	{
		// rgn GUID
		CGUID rgnGUID;
		// ����GUID
		if(p_SrcShape)
			rgnGUID=GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);
		
		CServerRegion* sRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnGUID);
		if(sRgn)
		{
			switch((eRgnType)sRgn->GetRgnType())
			{
			case RGN_PERSONAL:
				ret = CGlobeSetup::GetSetup()->lPDupRgnMaxPlayerNum;
				break;
			case RGN_PERSONAL_HOUSE:
				ret = CGlobeSetup::GetSetup()->lPHDupRgnMaxPlayerNum;
				break;
			case RGN_TEAM:
				ret = CGlobeSetup::GetSetup()->lTDupRgnMaxPlayerNum;
				break;
			}
		}
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//ȡ�ø���GUID
double CScript::GetPlayerGUID(const char* cmdStr, char* retStr)
{
	double ret = 0;

	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(guid)
	{
		int isFind = INVILID_VALUE;
		CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
		if(pMoveShape)
			isFind = SetScriptGuidValue(pMoveShape, guid, pMoveShape->GetExID());
		if(isFind != INVILID_VALUE)
			ret = 1.0f;
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

//ȡ��Ŀ���GUID
double CScript::GetDestGUID(const char* cmdStr, char* retStr)
{
	double ret = 0;
	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(guid)
	{
		int isFind = INVILID_VALUE;
		CMoveShape* pSrcMoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
		CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(p_DesShape);
		if(pSrcMoveShape && pMoveShape)
			isFind = SetScriptGuidValue(pSrcMoveShape, guid, pMoveShape->GetExID());
		if(isFind != INVILID_VALUE)
			ret = 1.0f;
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}


//����ָ����ɫ�ɣ���ҽ�ɫ�ĸ��˷��ݸ����ɣġ�����Ҫ����Ҳ�����ʱҲ�ɲ�ѯ��
double CScript::GetPHRgnGUIDByOnwerName	(const char* cmdStr, char* retStr)
{
	double ret = 0;

	// ��ɫName
	char* szPlayerName = GetStringParam(cmdStr, 0);
	if(szPlayerName == NULL) return 0;

	// ����GUID
	CPlayer* player = GetGame()->FindPlayer(szPlayerName);

	// ����GUID
	char* szRetGUID = GetStringParam(cmdStr, 1);
	if(szRetGUID == NULL)
	{
		M_FREE( szPlayerName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnGUID;

	if(player)
	{	
		rgnGUID = player->GetPersonalHouseRgnGUID();
		int tret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetGUID, rgnGUID);
		ret = 1.0f;
	}
	else // ����û�и��������
	{
		//wq:2008-7-9,��Ҫ�޸�
		//�ȴ�����, ֱ���յ����ݿⷵ����Ϣ��ȡ���ù����־
		SetIsHang(true);
		SetHangFunc(SC_FUNC_GETPHRGNGUIDBYONWERNAME);
		// ������Ϣ��WS,WS���û�����������ݿ���
		CMessage msg(MSG_S2W_SERVER_PLAYER_PERSONALHOUSEGUID);
		msg.Add(GetID());// Script ID
		msg.Add(szPlayerName); // PlayerGUID
		msg.Add(szRetGUID); // ���ص�PH GUID ������
		msg.SendToSocket(GetGame()->GetDBSSocket());
	}

	M_FREE( szPlayerName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetGUID, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//������ص�ǰ�����е�һ����Ϊ�赲�����꣬��������ֵ���浽ָ����2����ұ����С���ԭGetRegionRandomPos��
//���͵�ָ��������ָ�����ꡣ��ԭChangeRegion��
//�����д�������ĳ�Աȫ�����͵�ָ��������ָ�����ꡣ��ԭSetTeamRegion��
//��ָ��������ָ�����괴��ָ��ԭʼ�����������ű��������ԭCreateMonster��
//ɾ��ָ������������Ϊָ��ԭʼ�������
double CScript::DeleteMonster		(const char* cmdStr, char* retStr)
{
	return DelMonster(cmdStr);
}

//��ָ��������ָ�����괴��ָ���ɣĵĵ�ͼ������ݶ������ܹ���󿪷ţ�
double CScript::CreateDupRgnEntity	(const char* cmdStr, char* retStr)
{
	return 0;
}
//�Ը���Ϊִ����ִ��һ���ű�������ʹ�ø�����û����ҽű�Ҳ����ִ�У�
double CScript::RunDupRgnScript(const char* cmdStr, char* retStr)
{
	double ret = 0;
	//return call(cmdStr);
	char* szScript = GetStringParam(cmdStr, 0);
	if(!szScript) return 0;

	stRunScript stRunScr;
	stRunScr.srcShape = NULL;
	stRunScr.desShape = NULL;
	stRunScr.pszFileName = szScript;

	RunScript(&stRunScr, const_cast<CHAR*>( GetGame() -> GetScriptFileData(szScript)));
	M_FREE( szScript, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
//����Ŀǰ��Ϸ�д��ڵĸ���������
double CScript::GetDupRgnNum(const char* cmdStr, char* retStr)
{
	//�ȴ�����, �յ���Ӧ��Ϣ�����нű�
	SetIsHang(true);
	SetHangFunc(SR_FUNC_GETDUPRGNNUM);

	double ret = 0;
	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	// ���ص�WS�����и�������
	char* szRetNumVar = GetStringParam(cmdStr, 1);
	if(szRetNumVar == NULL) return -1.0f;

	CMessage msg(MSG_S2W_RGN_VIEWRGNNUM);
	msg.Add(rgnType);
	msg.Add(p_SrcShape->GetExID()); // �鿴��GUID
	if(szRetNumVar)
		msg.Add(szRetNumVar);
	else
		msg.Add("");
	msg.Add(GetID()); // script id
	msg.Send();
	
	M_FREE( szRetNumVar, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//����Ŀǰ��Ϸ�д��ڵĸ����������ޡ�
double CScript::GetDupRgnMaxNum(const char* cmdStr, char* retStr)
{
	double ret = 0;
	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	switch((eRgnType)rgnType)
	{
	case RGN_NORMAL:
		ret = -1.0f;
		break;
	case RGN_PERSONAL:
		ret = CGlobeSetup::GetSetup()->lPDupRgnMaxNum;
		break;
	case RGN_PERSONAL_HOUSE:
		ret = CGlobeSetup::GetSetup()->lPHDupRgnMaxNum;
		break;
	case RGN_TEAM:
		ret = CGlobeSetup::GetSetup()->lTDupRgnMaxNum;
		break;
	}
	return ret;
}
//ȡ��ָ�������ɣĵĸ����Ĺ���Ȩ���ţ�1��0����
double CScript::GetDupRgnOperFlag(const char* cmdStr, char* retStr)
{
	double ret = 0;

	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(guid == NULL) return 0;

	// rgn GUID
	CGUID rgnGUID;
	// ����GUID
	if(p_SrcShape)
		rgnGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);
	int sRet = INVILID_VALUE;
	CRgnManager::RgnByIDMapItr itr = GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().find(rgnGUID);
	if(itr != GameManager::GetInstance()->GetRgnManager()->GetPersonalDupRgnMap().end())
	{
		sRet = itr->second->GetOperFlag();
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return sRet;
	}

	itr = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().find(rgnGUID);
	if(itr != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
	{
		sRet = itr->second->GetOperFlag();
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return sRet;
	}

	itr = GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().find(rgnGUID);
	if(itr != GameManager::GetInstance()->GetRgnManager()->GetTeamDupRgnMap().end())
	{
		sRet = itr->second->GetOperFlag();
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return sRet;
	}


	itr = GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().find(rgnGUID);
	if(itr != GameManager::GetInstance()->GetRgnManager()->GetNormalRgnMap().end())
	{
		sRet = itr->second->GetOperFlag();
		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return sRet;
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

// ȡ�õ�ǰ�ű������ߵ�GUID
double CScript::GetCallerGUID(const char* cmd, char* retStr)
{
	char* szRetGUID;
	CShape* pLeader = p_SrcShape;
	if(pLeader == NULL) return 0;

	szRetGUID = GetStringParam(cmd, 0);
	if(szRetGUID)
	{
		SetScriptGuidValue((CMoveShape*)p_SrcShape, szRetGUID, pLeader->GetExID());
	}
	M_FREE( szRetGUID, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}
//����ִ���ߵ�ǰ���ڶ����е�ָ������˳��ŵĽ�ɫ�ɣġ�
double CScript::GetTeamMaterIDByPos(const char* cmdStr, char* retStr)
{
	double ret = 0;
	CPlayer* player = (CPlayer*)p_SrcShape;
	if(player == NULL) return 0;

	long pos = GetIntParam(cmdStr, 0);
	if(pos == ERROR_CODE)
		return 0;

	char* szMateGUID = GetStringParam(cmdStr, 1);
	if(szMateGUID == NULL) return 0;

	CGUID teamGUID = player->GetTeamID();
	GSTeam* team = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(teamGUID);
	if(team) // ��Ӵ���
	{
		list<CGUID> teamMemberList;
		team->GetMemberList(teamMemberList);
		list<CGUID>::iterator teamItr = teamMemberList.begin();
		long num = 0;
		while(teamItr != teamMemberList.end())
		{
			if(pos == num)
				break;
			
			num++;
			teamItr++;
		}

		if(teamItr != teamMemberList.end())
		{
			ret = SetScriptGuidValue(player, szMateGUID, *teamItr);
		}
	}
	
	M_FREE( szMateGUID, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
//�޸�ָ�������ɣĵ����ø�������ʾ��������һ����������޸ġ���������ͬ��ҽ�ɫ��������
double CScript::EditDupRgnName(const char* cmdStr, char* retStr)
{
	double ret = 0;
	// GUID������
	char* guid = GetStringParam(cmdStr, 0);
	if(guid == NULL) return 0;

	// rgn GUID
	CGUID rgnGUID;
	// ����GUID
	if(p_SrcShape)
		rgnGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, guid);
	int sRet = INVILID_VALUE;
	
	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnGUID);
	if(rgn)
	{
		char* RgnName = GetStringParam(cmdStr, 1);
		if(RgnName)
		{
			rgn->SetName(RgnName);
			// ������Ϣ���ͻ���
			CMessage msg(MSG_S2C_RGN_CHANGENAME);
			msg.Add((long)rgn->GetRgnType());
			msg.Add(rgn->GetExID());
			msg.Add(rgn->GetID());
			msg.Add(rgn->GetResourceID());
			msg.Add((char*)rgn->GetName());
			if(p_SrcShape)
				msg.SendToPlayer(p_SrcShape->GetExID());
			sRet = 1.0f;
		}

		M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
		return sRet;
	}

	M_FREE( guid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

// �������˷��ݸ�����Ʒ(Monster)
double CScript::CreatePHGoods(const char* cmdStr, char* retStr)//(const CGUID& ownerID, const char* name, long nCounts, long posX, long posY,
//	long width, long height, long dir, const char* strScript, const char* strTalk);
{
	double ret = 0;
	char* szOwnerID = GetStringParam(cmdStr, 0); // GUID������
	if(!szOwnerID) return 0;

	CGUID ownerID = NULL_GUID;
	ownerID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szOwnerID);
	char* name = GetStringParam(cmdStr, 1);
	if(name == NULL) 
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long nCounts = (long)GetIntParam(cmdStr, 2);
	long posX = (long)GetIntParam(cmdStr, 3);
	long posY = (long)GetIntParam(cmdStr, 4);
	long width = (long)GetIntParam(cmdStr, 5);
	long height = (long)GetIntParam(cmdStr, 6);
	long dir = (long)GetIntParam(cmdStr, 7);
	char* strScript = GetStringParam(cmdStr, 8);
	if(strScript == NULL)
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	char* strTalk = GetStringParam(cmdStr, 9);
	if(strTalk == NULL)
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		M_FREE( strScript, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CPlayer* player = GetGame()->FindPlayer(ownerID);
	if(player == NULL)
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		M_FREE( strScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( strTalk, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CGUID phRgnID = player->GetPersonalHouseRgnGUID();
	CRgnManager::RgnByIDMapItr rgnItr = GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().find(phRgnID);
	if(rgnItr != GameManager::GetInstance()->GetRgnManager()->GetPersonalHouseDupRgnMap().end())
	{
		CServerRegion* pRegion	= rgnItr->second;

		if( pRegion ) // ���ط�����
		{
			GameManager::GetInstance()->GetRgnManager()->CreatePHGoods(phRgnID, name, nCounts, posX, posY,
				width, height, dir, strScript, strTalk);
		}
	}
	else
	{
		// ������Ϣ��WS����WS������GS�ϴ���Goods
		//##������Ϣ��World Server�����ͺ󲻹�
		CMessage msg( MSG_S2W_SERVER_CREATEPHGOODS );
		msg.Add( (BYTE)0 );
		CGUID guid;
		CGUID::CreateGUID(guid);	
		msg.Add(guid);
		msg.Add( ownerID );
		msg.Add( name );
		msg.Add( (long)nCounts );
		msg.Add( (long)width );
		msg.Add( (long)height );			
		msg.Add( (long)posX );
		msg.Add( (long)posY );
		msg.Add ( (long)-1 );
		if( strScript )
		{
			msg.Add( BYTE(1) );
			msg.Add( strScript );
		}
		else 
		{
			msg.Add( BYTE(0) );
		}					
		msg.Send();
	}


	M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strScript, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strTalk, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

// ɾ�����˷��ݸ�����Ʒ(Monster)
double CScript::DeletePHGoods(const char* cmdStr, char* retStr)//(const CGUID& ownerID, const CGUID& goodsID);
{
	double ret = 0;
	char* szOwnerID = GetStringParam(cmdStr, 0); // GUID������
	if(!szOwnerID) return 0;

	CGUID ownerID = NULL_GUID;
	ownerID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szOwnerID);
	char* szgoodsID = GetStringParam(cmdStr, 1);
	if(!szgoodsID) 
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if(szgoodsID[0] == '@') // ��GUID
	{
		CGUID goodsID = NULL_GUID;
		if(CScript::GetGeneralVariableList())
			goodsID = CScript::GetGeneralVariableList()->GetGuidValue(szgoodsID);
		if(goodsID == NULL_GUID && p_SrcShape)
			((CPlayer*)p_SrcShape)->GetVariableList()->GetGuidValue(szgoodsID);
		if(goodsID == NULL_GUID)
			GetVariableList()->GetGuidValue(szgoodsID);

		GameManager::GetInstance()->GetRgnManager()->DeletePHGoods(ownerID, goodsID);
	}
	else // ������
	{
		GameManager::GetInstance()->GetRgnManager()->DeletePHGoods(ownerID, szgoodsID);
	}

	M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szgoodsID, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

// ȡ�ñ���GSID
double CScript::GetLocalGSID(const char* cmdStr, char* retStr)
{
	return (double)GetGame()->GetIDIndex();
}

// ͨ��������ֲ���GUID
double CScript::GetGUIDByPlayerName  (const char* cmdStr, char* retStr)
{
	double ret = 0;
	char* szgoodsID = NULL;
	char* szOwnerID = GetStringParam(cmdStr, 0); // player����
	if(!szOwnerID) return 0;

	CPlayer* player = GetGame()->FindPlayer(szOwnerID);
	if(player)//�ڵ�ǰ��������
	{
		szgoodsID = GetStringParam(cmdStr, 1);
		if(!szgoodsID) 
		{
			M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		const CGUID& goodsID = player->GetExID();
		int err = SetScriptGuidValue((CMoveShape*)p_SrcShape, szgoodsID, goodsID);
		// GUID ��ַָ��ֵ
		ret = (double)(long)&player->GetExID();
	}
	else
	{
		//���ڵ�ǰ��������,��WorldServer�ύ��ѯ
		CMessage msg(MSG_S2W_GM_QUERY_PLAYER);
		msg.Add(p_SrcShape->GetExID());
		if(szOwnerID)
			msg.Add(szOwnerID);// player����
		else
			msg.Add("");
		msg.Add(GetID()); // script id
		if(szgoodsID)
			msg.Add(szgoodsID); // GUID ��������
		else
			msg.Add("");
		msg.Send();

		/*�ű���ִͣ��,�ȴ����ؽ��*/
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETGUIDBYPLAYERNAME);			
	}

	M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szgoodsID, sizeof(char)*MAX_VAR_LEN );

	return ret;
}
// ͨ��������ֲ�����P GUID
double CScript::GetPGUIDByPlayerName  (const char* cmdStr, char* retStr)
{
	double ret = 0;

	char* szOwnerID = GetStringParam(cmdStr, 0); // player����
	if(szOwnerID == NULL) return 0;
	char* szgoodsID = GetStringParam(cmdStr, 1);
	if(szgoodsID == NULL)
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CPlayer* player = GetGame()->FindPlayer(szOwnerID);
	if(player)//�ڵ�ǰ��������
	{
		int err = SetScriptGuidValue((CMoveShape*)p_SrcShape, szgoodsID, player->GetPersonalRgnGUID());
		ret = 1.0f;
	}
	else
	{
		//���ڵ�ǰ��������,��WorldServer�ύ��ѯ
		CMessage msg(MSG_S2W_GM_QUERY_PLAYER_P);
		msg.Add(szOwnerID);// player����
		msg.Add(GetID()); // script id
		msg.Add(szgoodsID); // GUID ��������
		msg.Send();

		/*�ű���ִͣ��,�ȴ����ؽ��*/
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETPGUIDBYPLAYERNAME);
		ret = 2.0f;				
	}

	M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szgoodsID, sizeof(char)*MAX_VAR_LEN );

	return ret;
}
// ͨ��������ֲ�����PH GUID
double CScript::GetPHGUIDByPlayerName  (const char* cmdStr, char* retStr)
{
	double ret = 0;

	char* szOwnerID = GetStringParam(cmdStr, 0); // player����
	if(szOwnerID == NULL) return 0;
	char* szgoodsID = GetStringParam(cmdStr, 1);
	if(szgoodsID == NULL)
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CPlayer* player = GetGame()->FindPlayer(szOwnerID);
	if(player)//�ڵ�ǰ��������
	{
		int err = SetScriptGuidValue((CMoveShape*)p_SrcShape, szgoodsID, player->GetPersonalHouseRgnGUID());
	}
	else
	{
		//���ڵ�ǰ��������,��WorldServer�ύ��ѯ
		CMessage msg(MSG_S2W_GM_QUERY_PLAYER_PH);
		msg.Add(szOwnerID);// player����
		msg.Add(GetID()); // script id
		msg.Add(szgoodsID); // GUID ��������
		msg.Send();

		/*�ű���ִͣ��,�ȴ����ؽ��*/
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETPHGUIDBYPLAYERNAME);
		//            m_nRunningFunction=CommandId;					
	}

	M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szgoodsID, sizeof(char)*MAX_VAR_LEN );

	return ret;
}
// ͨ��������ֲ�����T GUID
double CScript::GetTGUIDByPlayerName  (const char* cmdStr, char* retStr)
{
	double ret = 0;

	char* szOwnerID = GetStringParam(cmdStr, 0); // player����
	if(szOwnerID == NULL) return 0;
	char* szgoodsID = GetStringParam(cmdStr, 1);
	LONG lTemplateID = GetIntParam(cmdStr, 2);
	if(szgoodsID == NULL)
	{
		M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CGUID ownerID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szOwnerID);

	if(ownerID != NULL_GUID)//�ڵ�ǰ��������
	{
		int err = SetScriptGuidValue((CMoveShape*)p_SrcShape, szgoodsID, 
			GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGUID(ownerID, lTemplateID));
		ret = 1.0f;
	}
	else
	{
		//���ڵ�ǰ��������,��WorldServer�ύ��ѯ
		CMessage msg(MSG_S2W_GM_QUERY_PLAYER_T);
		msg.Add(szOwnerID);// player����
		msg.Add(GetID()); // script id
		msg.Add(szgoodsID); // GUID ��������
		msg.Send();

		/*�ű���ִͣ��,�ȴ����ؽ��*/
		SetIsHang(true);
		SetHangFunc(SR_FUNC_GETTGUIDBYPLAYERNAME);			
	}

	M_FREE( szOwnerID, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szgoodsID, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

double CScript::ChangeDupRgnByRgnID(const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);
	
	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	// ����Rgn
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(szRgnGuid == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	long rType = GetIntParam(cmdStr, 2);
	if(rType == ERROR_CODE)
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// player GUID
	CGUID rgnPGuid;
	rgnPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);

	long ret = 0;
	if(player)
	{
		long x = GetIntParam(cmdStr, 3);
		long y = GetIntParam(cmdStr, 4);
		ret = player->ChangeRegion((eRgnType)rType, rgnPGuid, x, y, 0);
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
// ���õ�ǰ�ű���Ҹ��˸���GUID       SetPlayerPDupRgnID("@OwnerID","@RgnID");
double CScript::SetPlayerPDupRgnID (const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);
	
	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	// ����Rgn
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(szRgnGuid == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnPGuid;
	rgnPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);
	
	long ret = 0;
	if(player)
	{
		player->SetPersonalRgnGUID(rgnPGuid);
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
// ���õ�ǰ�ű���Ҹ��˷��ݸ���GUID		SetPlayerPHDupRgnID("@OwnerID","@RgnID");	
double CScript::SetPlayerPHDupRgnID (const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);
	
	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	// ����Rgn
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(szRgnGuid == NULL)
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnPGuid;
	rgnPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);

	long ret = 0;
	if(player)
	{
		player->SetPersonalHouseRgnGUID(rgnPGuid);
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
// ���õ�ǰ�ű����鸱��GUID		AddTeamRgnID("@OwnerID",int,"@RgnID");
double CScript::AddTeamRgnID (const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	LONG templateRgnID = GetIntParam(cmdStr, 1);
	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);
	
	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	// ����Rgn
	char* szRgnGuid = GetStringParam(cmdStr, 2);
	if(szRgnGuid == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// rgn GUID
	CGUID rgnPGuid;
	rgnPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);
	
	long ret = 0;
	if(NULL != player)
	{
		GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(NULL == pTeam)
		{
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
			
		CPlayer *pMaster = GetGame()->FindPlayer(pTeam->GetLeader());
		if (NULL != pMaster)
		{
			GameManager::GetInstance()->GetRgnManager()->AddTeamRgnGUID(sPGuid, templateRgnID, rgnPGuid, FALSE);
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_AddRgnID);
			msg.Add(pTeam->GetLeader());
			msg.Add(templateRgnID);
			msg.Add(rgnPGuid);
			msg.Send(false);
		}
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

//! Ϊ��ǰ�ű�����ɾ������GUID			DelTeamRgnID("@OwnerID", $templateID);
double CScript::DelTeamRgnID (const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	LONG templateRgnID = GetIntParam(cmdStr, 1);
	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);

	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	if(NULL != player)
	{
		GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(NULL == pTeam)
		{
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		CPlayer *pMaster = GetGame()->FindPlayer(pTeam->GetLeader());
		if (NULL != pMaster)
		{
			GameManager::GetInstance()->GetRgnManager()->DelTeamRgnGUID(sPGuid, templateRgnID);
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_DelRgnID);
			msg.Add(pTeam->GetLeader());
			msg.Add(templateRgnID);
			msg.Send(false);
		}
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

//! Ϊ��ǰ�ű�����������и���GUID		ClearTeamRgnID("@OwnerID");
double CScript::ClearTeamRgnID (const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* player = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);

	// ����Player
	player = GetGame()->FindPlayer(sPGuid);

	if(NULL != player)
	{
		GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(player->GetTeamID());
		if(NULL == pTeam)
		{
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		CPlayer *pMaster = GetGame()->FindPlayer(pTeam->GetLeader());
		if (NULL != pMaster)
		{
			GameManager::GetInstance()->GetRgnManager()->ClearTeamRgnGUID(sPGuid);
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_ClearRgnID);
			msg.Add(pTeam->GetLeader());
			msg.Send(false);
		}
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::SwapTeamRgnID(const char* cmdStr, char* retStr)
{
	return 0;
}

//! �õ�������ڶ���Ķ��鸱����			GetTeamRgnIDNum("@OwnerID", $Ret);
double CScript::GetTeamRgnIDNum(const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* pPlayer = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);

	pPlayer = GetGame()->FindPlayer(sPGuid);

	// ����ֵ������
	char* szRet = GetStringParam(cmdStr, 1);
	if(szRet == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if(NULL != pPlayer)
	{
		GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(pPlayer->GetTeamID());
		if(NULL == pTeam)
		{
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		CPlayer *pMaster = GetGame()->FindPlayer(pTeam->GetLeader());
		if (NULL != pMaster)
		{
			long rgnNum = 0;
			map<LONG, CGUID>* rgnIdMap = GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGuidMap(pMaster->GetExID());
			if(rgnIdMap)
				rgnNum = rgnIdMap->size();
			SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, 0, (LONG)rgnNum);
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_GetRgnNum);
			msg.Add(pTeam->GetLeader());
			msg.Add(GetID());
			msg.Add(szRet);

			SetIsHang(true);
			SetHangFunc(SR_FUNC_GETTEAMRGNIDNUM);
		}
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
//! �õ�������ڶ���ĵ�N�����鸱��ID		GetTeamRgnIDByIndex("@OwnerID", $N, "@RgnGuid");
double CScript::GetTeamRgnIDByIndex(const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* pPlayer = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);

	pPlayer = GetGame()->FindPlayer(sPGuid);
	LONG lIndex = GetIntParam(cmdStr, 1);

	// ����ֵ������
	char* szRet = GetStringParam(cmdStr, 2);
	if(szRet == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if(NULL != pPlayer)
	{
		GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(pPlayer->GetTeamID());
		if(NULL == pTeam)
		{
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		CPlayer *pMaster = GetGame()->FindPlayer(pTeam->GetLeader());
		if (NULL != pMaster)
		{
			char szGuid[128];
			GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGuidByIndex(pMaster->GetExID(), lIndex).tostring(szGuid);
			SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, szGuid);
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_GetRgnGuid);
			msg.Add(pTeam->GetLeader());
			msg.Add(GetID());
			msg.Add(lIndex);
			msg.Add(szRet);

			SetIsHang(true);
			SetHangFunc(SR_FUNC_GETTEAMRGNIDBYINDEX);
		}
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
//! �õ�������ڶ���ĵ�N�����鸱������ͼID	GetTeamRgnTemplatIDByIndex("@OwnerID", $N, $Ret);
double CScript::GetTeamRgnTemplatIDByIndex(const char* cmdStr, char* retStr)
{
	// ����player
	char* szPlayerGuid = GetStringParam(cmdStr, 0);
	if(szPlayerGuid == NULL) return 0;

	CPlayer* pPlayer = NULL;
	// player GUID
	CGUID sPGuid;
	sPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szPlayerGuid);

	pPlayer = GetGame()->FindPlayer(sPGuid);
	LONG lIndex = GetIntParam(cmdStr, 1);

	// ����ֵ������
	char* szRet = GetStringParam(cmdStr, 2);
	if(szRet == NULL) 
	{
		M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if(NULL != pPlayer)
	{
		GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(pPlayer->GetTeamID());
		if(NULL == pTeam)
		{
			M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		CPlayer *pMaster = GetGame()->FindPlayer(pTeam->GetLeader());
		if (NULL != pMaster)
		{
			SetScriptVarValue((CMoveShape*)p_SrcShape, szRet, 0, 
				GameManager::GetInstance()->GetRgnManager()->GetTeamRgnTemplateIdByIndex(pMaster->GetExID(), lIndex));
		}
		else
		{
			CMessage msg(MSG_S2W_TEAM_GetRgnTemplateID);
			msg.Add(pTeam->GetLeader());
			msg.Add(GetID());
			msg.Add(lIndex);
			msg.Add(szRet);

			SetIsHang(true);
			SetHangFunc(SR_FUNC_GETTEAMRGNTEMPLATIDBYINDEX);
		}
	}

	M_FREE( szPlayerGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRet, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

// �鿴������������
double CScript::ViewRgnVar(const char* cmdStr, char* retStr)
{
	if(p_SrcShape == NULL)
		return -1.0f;

	// rgn type
	long rgnType = (long)GetIntParam(cmdStr, 0);

	// ����Rgn
	char* szRgnGuid = GetStringParam(cmdStr, 1);
	if(szRgnGuid == NULL) return 0;

	// rgn GUID
	CGUID rgnPGuid;
	rgnPGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);
	
	int cmdLen = strlen(cmdStr);

	// ȡ��һ�������� �������ַ���
	char* firstVarName = NULL;
	firstVarName = GetStringParam(cmdStr, 2);
	if(szRgnGuid == NULL)
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return -1.0f;
	}

	// ����ֵ��������
	char* retVarName = GetStringParam(cmdStr, 3);
	if(retVarName == NULL)
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
		return -1.0f;
	}

	if(firstVarName)
	{
		if( (firstVarName[0] != '#' && firstVarName[0] != '@' && firstVarName[0] != '$')  )
		{
			TestLog(CStringReading::LoadString(IDS_GS_RGNFUNC, 4));
			M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
			M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
			M_FREE( retVarName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}

		double firstVarLen = strlen(firstVarName);
		char MainVarName[NAME_STR_NUM];
		char InnVarName[NAME_STR_NUM];

		char* MainVarNamePtr = MainVarName;
		char* InnVarNamePtr  = InnVarName;
		
		//--begin ����Ƕ��'['']'ʹ��
		long pos = 0;
		CheckParamStringForArray(firstVarName, firstVarLen, &MainVarNamePtr, NAME_STR_NUM, &InnVarNamePtr, NAME_STR_NUM, pos);
		//--end ����Ƕ��'['']'ʹ��

		CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID((eRgnType)rgnType, rgnPGuid);

		// ����GSID
		if(rgn)
		{
			// ��ȡ�ַ���ֵ
			if(MainVarName[0] == '#') // �ַ���
			{
				char* strValue = NULL;

				if(m_pVariableList)
				{
					if(InnVarName[0] == '\0') // �����������
					{
						if(rgn)
						{
							strValue = (char*)rgn->GetVariableList()->GetVarValue(MainVarName);
							SetScriptVarValue((CMoveShape*)p_SrcShape, retVarName, strValue);
						}
					}
					else
					{
						int lArray = 0;
						
						if(InnVarName[0] == '$') // �Ǳ���
						{
							if(rgn)
								lArray = rgn->GetVariableList()->GetVarValue(InnVarName, 0);
						}
						else if(InnVarName[0] != '$' && InnVarName[0] != '#') // �ǳ���
							lArray = atoi(InnVarName);
					
						// ȥ��'[' ']'
						char name[128];
						_snprintf(name, 128, "%s%d", MainVarName, lArray);

						if(rgn)
						{
							strValue = (char*)rgn->GetVariableList()->GetVarValue(name);
							SetScriptVarValue((CMoveShape*)p_SrcShape, retVarName, strValue);
						}
					}
				}
			}
			else if(firstVarName[0] == '$') //����
			{
				double value = 0;

				if(m_pVariableList && (value != ERROR_CODE) )
				{
					if(InnVarName[0] == '\0') // �����������
					{
						if(rgn)
						{
							value = rgn->GetVariableList()->GetVarValue(MainVarName, 0);
							SetScriptVarValue((CMoveShape*)p_SrcShape, retVarName, 0, value);
						}
					}
					else
					{
						int lArray = 0;
						if(InnVarName[0] == '$') // �Ǳ���
						{
							if(rgn)
								lArray = rgn->GetVariableList()->GetVarValue(InnVarName, 0);
						}
						else if(InnVarName[0] != '$' && InnVarName[0] != '#') // �ǳ���
							lArray = atoi(InnVarName);

						// ȥ��'[' ']'
						if(rgn)
						{
							value = rgn->GetVariableList()->GetVarValue(MainVarName, lArray);
							SetScriptVarValue((CMoveShape*)p_SrcShape, retVarName, lArray, value);
						}
					}
				}

			}
			else if(firstVarName[0] == '@') // ����GUID
			{
				CGUID tGUID;
				if(InnVarName[0] == '\0') // �����������
				{
					if(rgn)
						tGUID = rgn->GetVariableList()->GetGuidValue(MainVarName);
				}
				else // ���������
				{
					int lArray = 0;
					if(InnVarName[0] == '$') // �±�����ͨ����
					{
						lArray = GetScriptVarValue((CMoveShape*)p_SrcShape, InnVarName, 0);
					}
					else if(InnVarName[0] != '$' && InnVarName[0] != '#') // �±��ǳ���
					{
						lArray = atoi(InnVarName);
					}

					// ȥ��'[' ']'
					for(int i=0; i < lArray; i++)
					{
						char tNum[NAME_STR_NUM];
						char name[NAME_STR_NUM];
						strcpy_s(name, NAME_STR_NUM, MainVarName);
						itoa(i, tNum, 10);
						strcat_s(name, NAME_STR_NUM, tNum);
						if(rgn)
						{
							tGUID = rgn->GetVariableList()->GetGuidValue(name);
							SetScriptGuidValue((CMoveShape*)p_SrcShape, retVarName, tGUID);
						}
					}
				}	
			}
		}
		else // ����MSG_S2W_RGN_VIEWRGNVAR��Ϣ��WS
		{
			//wq:2008-7-9,��Ҫ�޸�
			CMessage msg(MSG_S2W_RGN_VIEWRGNVAR);
			msg.Add((long)rgnType);
			msg.Add(rgnPGuid);
			msg.Add(firstVarName);
			msg.Add(retVarName);
			msg.Add(p_SrcShape->GetExID());
			msg.Add(GetID()); // hunging script ID
			msg.Send();
			SetIsHang(true);
			SetHangFunc(SC_FUNC_VIEWRGNVAR);
		}
	}

	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( firstVarName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( retVarName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
// ͨ��GUID���ø�player��monster�Ľű�
double CScript::CallScriptByGUID(const char* cmdStr, char* retStr)
{
	if(NULL == p_SrcShape)	return 0;

	// ����shape GUID string
	char* szshapeGuid = GetStringParam(cmdStr, 0);
	if(szshapeGuid == NULL) return 0;

	// shape GUID
	CGUID shapePGuid;
	shapePGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szshapeGuid);

	CMoveShape* shape = GetGame()->FindShape(TYPE_PLAYER, shapePGuid);
	
	if(NULL == shape)
	{
		CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pRegion);
		if(pRegion)
			shape = dynamic_cast<CMoveShape*>(pRegion->FindChildObject(TYPE_MONSTER, shapePGuid));
	}		

	char* szScript = GetStringParam(cmdStr, 1);
	if(NULL == szScript)
	{
		M_FREE( szshapeGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	if(shape) // ���ص��ýű�
	{
		stRunScript stRunScr;
		stRunScr.pRegion = (CRegion*)(p_SrcShape->GetFather());
		stRunScr.srcShape = shape;
		stRunScr.desShape = NULL;
		stRunScr.pszFileName = szScript;
		RunScript(&stRunScr,(char*)GetGame()->GetScriptFileData(szScript));
	}
	else // ������ýű�
	{
		CMessage msg(MSG_S2W_QUEST_PlayerRunScript);
		msg.Add(shapePGuid); // call guid
		msg.Add(szScript);	 // called script name
		msg.SendToSocket(GetGame()->GetWSSocket());
	}

	M_FREE( szshapeGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szScript, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}

// �鿴ָ��pos�ĸ�����ϸ����
double CScript::ViewDupRgnData(const char* cmdStr, char* retStr)
{
	long rgnType = (long)GetIntParam(cmdStr, 0);
	long pos = (long)GetIntParam(cmdStr, 1);

	long dataType = (long)GetIntParam(cmdStr, 2);
	char* retVarName = GetStringParam(cmdStr, 3);
	if(retVarName == NULL)
		return -1.0f;

	char* szGuid = GetStringParam(cmdStr, 4);
	if(szGuid == NULL)
	{
		M_FREE( retVarName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CGUID guid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid);
	CMessage msg(MSG_S2W_RGN_VIEWRGNDATA);
	msg.Add(guid);
	msg.Add(rgnType);
	msg.Add(pos);
	if(p_SrcShape)
		msg.Add(p_SrcShape->GetExID());//viewer id
	else
		msg.Add(NULL_GUID);
	msg.Add(dataType);
	msg.Add(retVarName);
	msg.Add(GetID());
	msg.Send();
	
	SetIsHang(true);
	SetHangFunc(SR_FUNC_VIEWDUPRGNDATA);
	
	M_FREE( retVarName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}

// @������ֵ�ӿ�,Param1:@Src   Param2:@Des
double CScript::EvaluateGUID(const char* cmdStr, char* retStr)
{
	// ����szSrcGuid
	char* szSrcRgnGuid = GetStringParam(cmdStr, 0);
	if(szSrcRgnGuid == NULL) return 0;

	CGUID rgnSrcGUID;
	// src GUID
	rgnSrcGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szSrcRgnGuid);
	// ����szDesGuid
	char* szDesGuid = GetStringParam(cmdStr, 1);
	if(szDesGuid == NULL) 
	{
		M_FREE( szSrcRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// des GUID
	int ret = SetScriptGuidValue((CMoveShape*)p_SrcShape, szDesGuid, rgnSrcGUID);
	M_FREE( szSrcRgnGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szDesGuid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
// �鿴Rgn�Ƿ����
double CScript::ViewRgnValidByGUID(const char* cmd, char* retStr)
{
	// ����szRgnGuid
	char* szRgnGuid = GetStringParam(cmd, 0);
	if(szRgnGuid == NULL) return 0;

	CGUID rgnGUID;
	// src GUID
	rgnGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szRgnGuid);

	// ����ֵ������
	char* szRetName = GetStringParam(cmd, 1);
	if(szRetName == NULL)
	{
		M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnGUID);
	if(rgn)
	{
		// Ϊ����ֵ��ֵ
		int ret = SetScriptVarValue((CMoveShape*)p_SrcShape, szRetName, 0, 1.0f);
	}
	else
	{
		CMessage msg(MSG_S2W_RGN_VIEWRGNAVLID);
		msg.Add(rgnGUID); // rgn guid
		if(p_SrcShape)
			msg.Add(p_SrcShape->GetExID());
		else
			msg.Add(NULL_GUID);
		msg.Add(szRetName); // ret name
		msg.Add(GetID()); //script id
		msg.Send();

		SetIsHang(true);
		SetHangFunc(SR_FUNC_VIEWRGNVALIDBYGUID);
	}

	M_FREE( szRetName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
//����ָ�������ɣĵĸ����Ĺ���Ȩ���ţ�1��0����
double CScript::SetDupRgnOperFlag(const char* cmd, char* retStr)
{
	double ret = 0;
	// ����szSrcGuid
	char* szSrcRgnGuid = GetStringParam(cmd, 0);
	if(szSrcRgnGuid == NULL) return 0;
	
	// flag
	double flag = GetIntParam(cmd, 1);
	if(flag == ERROR_CODE)
	{
		M_FREE( szSrcRgnGuid, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	CGUID rgnSrcGUID;
	// src GUID
	rgnSrcGUID = GetScriptGuidValue((CMoveShape*)p_SrcShape, szSrcRgnGuid);
	
	CServerRegion* rgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnSrcGUID);
	if(rgn)
	{
		bool opFlag = false;
		if(abs(flag-0)<0.00001f) opFlag = false;
		else if(abs(flag-1)<0.00001f) opFlag = true;
		rgn->SetOperFlag(opFlag);
	}

	M_FREE( szSrcRgnGuid, sizeof(char)*MAX_VAR_LEN );
	return ret;
}
// ˢ����ҵĸ�����¼�б�
double CScript::ReFreshPlayerDupRgnList(const char* cmd, char* retStr)
{
	char* szGuid = GetStringParam(cmd, 0);
	if(szGuid)
	{
		SetIsHang(true);
		SetHangFunc(SR_FUNC_REFRESHPLAYERDUPRGNLIST);

		CMessage msg(MSG_S2W_RGN_QUESTPLAYERRGNLIST);
		msg.Add(GetID());
		
		CGUID playerGuid;
		if(strcmp(szGuid, "") != 0)
			playerGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, szGuid);
		else
			playerGuid = p_SrcShape->GetExID();

		msg.Add(playerGuid);

		CPlayer* player = GetGame()->FindPlayer(playerGuid);
		if(player)
		{
			msg.Add((BYTE)1); // ��GS��ѯ
			map<LONG, CGUID>* rgnIdMap = GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGuidMap(playerGuid);
			if(rgnIdMap)
			{
				msg.Add((long)rgnIdMap->size());
				map<LONG, CGUID>::iterator itr = rgnIdMap->begin();
				for(; itr != rgnIdMap->end(); itr++)
				{
					msg.Add((long)itr->first);
					msg.Add(itr->second);
				}
			}
			else
				msg.Add((long)0);

			
		}
		else
		{
			msg.Add((BYTE)0); // �Ǳ�GS��ѯ
		}
		msg.Send();
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
		return 1;
	}
	return 0;
}
// �����ǰGS�����и����͸���
double CScript::ClearCurGSDupRgn(const char* cmd, char* retStr)
{
	long rgnType = GetIntParam(cmd, 0);
	if(rgnType == ERROR_CODE)
		return 0;

	CShape* player = p_SrcShape;
	if(player == NULL) return 0;

	CRgnManager* rgnManager = GameManager::GetInstance()->GetRgnManager();
	if(rgnManager)
	{
		CServerRegion* rgn = NULL;
		switch(rgnType)
		{
		case RGN_PERSONAL:
			{
				CRgnManager::RgnByIDMap::iterator rgnItr = rgnManager->GetPersonalDupRgnMap().begin();
				for(; rgnItr != rgnManager->GetPersonalDupRgnMap().end(); rgnItr++)
				{
					rgn = rgnItr->second;
					// ����ø�����������򷵻�
					if( rgn && (rgn->GetPlayerAmout() != 0) )
						continue;
					
					CMessage msg(MSG_S2W_RGN_DELETE);
					msg.Add(rgn->GetOwnerGUID());   // owner id
					msg.Add(rgn->GetExID());		// Region��GUID
					msg.Add((long)rgnType); // ģ��Rgn��Type
					msg.Add((long)-1);/*����Ľű�ID*/
					// �첽��������ֵ������
					msg.Add((char*)"");
					// �첽��������ֵ������
					msg.Add((char*)"");
					msg.Send();
				}
			}
			break;
		case RGN_PERSONAL_HOUSE:
			{
				CRgnManager::RgnByIDMap::iterator rgnItr = rgnManager->GetPersonalHouseDupRgnMap().begin();
				for(; rgnItr != rgnManager->GetPersonalHouseDupRgnMap().end(); rgnItr++)
				{
					rgn = rgnItr->second;
					// ����ø�����������򷵻�
					if( rgn && (rgn->GetPlayerAmout() != 0) )
						continue;

					CMessage msg(MSG_S2W_RGN_DELETE);
					msg.Add(rgn->GetOwnerGUID());   // owner id
					msg.Add(rgn->GetExID());		// Region��GUID
					msg.Add((long)rgnType); // ģ��Rgn��Type
					msg.Add((long)-1);/*����Ľű�ID*/
					// �첽��������ֵ������
					msg.Add((char*)"");
					// �첽��������ֵ������
					msg.Add((char*)"");
					msg.Send();
				}
			}
			break;
		case RGN_TEAM:
			{
				CRgnManager::RgnByIDMap::iterator rgnItr = rgnManager->GetTeamDupRgnMap().begin();
				for(; rgnItr != rgnManager->GetTeamDupRgnMap().end(); rgnItr++)
				{
					rgn = rgnItr->second;
					// ����ø�����������򷵻�
					if( rgn && (rgn->GetPlayerAmout() != 0) )
						continue;

					CMessage msg(MSG_S2W_RGN_DELETE);
					msg.Add(rgn->GetOwnerGUID());   // owner id
					msg.Add(rgn->GetExID());		// Region��GUID
					msg.Add((long)rgnType); // ģ��Rgn��Type
					msg.Add((long)-1);/*����Ľű�ID*/
					// �첽��������ֵ������
					msg.Add((char*)"");
					// �첽��������ֵ������
					msg.Add((char*)"");
					msg.Send();
				}
			}
			break;
		}

	}
	return 1.0f;
}
// �ı䵱ǰ������ĳ���ݵ�״̬
double CScript::SetRgnStrongPointState (const char* cmd, char* retStr)
{
	if(!m_pRegion) return -1.0f;


	char* szStrongPointName = GetStringParam(cmd, 0);
	if(szStrongPointName)
	{
		double llState = GetIntParam(cmd, 1);
		if(llState != ERROR_CODE)
		{
			CServerRegion* pRgn = dynamic_cast<CServerRegion*>(m_pRegion);
			if(pRgn)
			{
				pRgn->SetStrongPointState(szStrongPointName, llState);
				// ������Ϣ֪ͨ�ó����������
				CMessage msg(MSG_S2C_RGN_STRONGPOINT_STATE_CHANGE);
				msg.Add((long)pRgn->GetID()); // region id
				msg.Add(pRgn->GetExID()); // region guid
				msg.Add(szStrongPointName); // strongpoint name
				msg.Add((long)llState);
				msg.SendToRegion(pRgn);
			}
		}
		M_FREE(szStrongPointName, sizeof(char)*MAX_VAR_LEN);
	}
	return -1.0f;
}
// ȡ�õ�ǰ������ĳ���ݵ�״̬
double CScript::GetRgnStrongPointState(const char* cmd, char* retStr)
{
	if(!m_pRegion) return -1.0f;

	long retFlag = -1;
	char* szStrongPointName = GetStringParam(cmd, 0);
	if(szStrongPointName)
	{
		CServerRegion* pRgn = dynamic_cast<CServerRegion*>(m_pRegion);
		if(pRgn)
		{
			retFlag = pRgn->GetStrongPointState(szStrongPointName);
		}
		M_FREE(szStrongPointName, sizeof(char)*MAX_VAR_LEN);
	}
	return retFlag;
}
// ȡ��ָ����ͼָ����Ӫ��ȷ����
double CScript::GetRegionCampPlayerNum(const char* cmd, char* retStr)
{
	if(!m_pRegion) return -1.0f;

	char* szRgnGuidName = GetStringParam(cmd, 0);
	if(szRgnGuidName)
	{
		long lCampType =  (long)GetIntParam(cmd, 1);
		if(lCampType != eCampUnion && lCampType != eCampEmpire) // ��Ӫֵδ�ڶ��巶Χ��
		{
			char* szRetName = GetStringParam(cmd, 2);
			if(szRetName)
			{
				SetScriptVarValue((CMoveShape*)GetSrcShape(), szRetName, 0, -1.0f);
				M_FREE(szRetName, sizeof(char)*MAX_VAR_LEN);
			}
			M_FREE(szRgnGuidName, sizeof(char)*MAX_VAR_LEN);
			return -1.0f;
		}

		char* szRetName = GetStringParam(cmd, 2);
		if(!szRetName)
		{
			M_FREE(szRgnGuidName, sizeof(char)*MAX_VAR_LEN);
			return -1.0f;
		}

		CGUID rgnGUID;
		CServerRegion* pRgn = NULL;
		if(szRgnGuidName[0] == '\0') // �ǿ��ַ�������ʾ��ǰ����
		{
			pRgn = dynamic_cast<CServerRegion*>(m_pRegion);
		}
		else
		{
			rgnGUID = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szRgnGuidName);
			pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(rgnGUID);
		}

		if(pRgn) // �Ǳ��س���
		{
			long lCampId = pRgn->GetCampPlayerNum((eCampType)lCampType);
			SetScriptVarValue((CMoveShape*)GetSrcShape(), szRetName, 0, lCampId);
		}
		else // ����������������
		{
			// ����ýű�
			SetIsHang(true);
			SetHangFunc(SR_FUNC_GETRGNCAMPPLAYERNUM);

			// ���Ͳ�ѯ��Ϣ
			CMessage msg(MSG_S2W_RGN_CAMP_PLAYER_NUM_QUEST);
			msg.Add((long)GetGame()->GetIDIndex()); // gsid
			msg.Add(rgnGUID);// ����GUID
			msg.Add((long)lCampType);// ָ����Ӫ����
			msg.Add(GetID()); // ����ű�guid
			msg.Add(szRetName); //����ű����ؽű�������
			msg.Send();
		}


		M_FREE(szRetName, sizeof(char)*MAX_VAR_LEN);
		M_FREE(szRgnGuidName, sizeof(char)*MAX_VAR_LEN);
	}
	return 1.0f;
}