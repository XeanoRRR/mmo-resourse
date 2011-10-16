#include "StdAfx.h"
#include "Script.h"
#include "../player.h"
#include "../AntiCheat/CPlayerManager.h"

#define CHECK_SHAPE( ret ) \
	if( this->p_SrcShape == NULL || \
	dynamic_cast<CPlayer*>( this->p_SrcShape ) == NULL ) \
	{ \
		return ret; \
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������:     ��Ӹı���һ�Ծ�ȵĽű�����
/// �����ű�ԭ�ͣ�$retActivity = ChangeActivity( "@PlayerGUID", $changeActivity );
/// ��������:     @PlayerGUID : ���GUID
///               $changeActivity : �ı�Ļ�Ծ��ֵ
/// ��������ֵ:   �ı��Ļ�Ծ��ֵ
///////////////////////////////////////////////////////////////////////////////////////////////////////
double CScript::ChangeActivity(const char *cmd, char *retStr /* = NULL */)
{
	CHECK_SHAPE( 0 );

	long lRet = 0;
	char *pszGUID = GetStringParam(cmd, 0);
	if(!pszGUID) 
		return lRet;


	long nAnctivity = GetIntParam(cmd, 1);
	if(ERROR_CODE == nAnctivity)
	{
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		return lRet;
	}

	if(strlen(pszGUID) > 0)
	{
		const CGUID& rID = GetScriptGuidValue(dynamic_cast<CMoveShape*>(p_SrcShape), pszGUID);
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		if(NULL_GUID == rID)
		{
			return lRet;
		}
		return GetInst(AntiCheat::CPlayerManager).PlayerChangeActivity(rID, nAnctivity);
	}
	/// ����GUIDΪ�յ����
	else if( p_SrcShape != NULL )
	{
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		return GetInst(AntiCheat::CPlayerManager).PlayerChangeActivity(dynamic_cast<CPlayer*>(p_SrcShape)->GetExID(), nAnctivity);
	}

	return lRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������:     ��������߲�������ڶ�����ܵ�¼�Ľű�����
/// �����ű�ԭ�ͣ�ACKickPlayer( ["@playerGUID"] );
/// ��������:     @PlayerGUID : ���GUID, ��ѡ
/// ��������ֵ:   ��
///////////////////////////////////////////////////////////////////////////////////////////////////////
double CScript::ACKickPlayer(const char *cmd, char *retStr /* = NULL */)
{
	CHECK_SHAPE( 0 );

	char *pszGUID = GetStringParam(cmd, 0);
	if(!pszGUID)
		return 0;
	if(strlen(pszGUID) > 0)
	{
		const CGUID& rID = GetScriptGuidValue(dynamic_cast<CMoveShape*>(p_SrcShape), pszGUID);
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		if(NULL_GUID == rID)
		{
			return 0;
		}
		GetInst(AntiCheat::CPlayerManager).KickOnlinePlayer(rID);
	}
	/// ����GUIDΪ�յ����
	else if( p_SrcShape != NULL )
	{
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		GetInst(AntiCheat::CPlayerManager).KickOnlinePlayer(dynamic_cast<CPlayer*>(p_SrcShape)->GetExID());
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������:     ֹͣ��Ҽ��Ľű�����
/// �����ű�ԭ�ͣ�StopCheckAC( ["@playerGUID"] );
/// ��������:     @PlayerGUID : ���GUID, ��ѡ
/// ��������ֵ:   ��
///////////////////////////////////////////////////////////////////////////////////////////////////////
double CScript::StopCheckAC(const char *cmd, char *retStr /* = NULL */)
{
	CHECK_SHAPE( 0 );

	char *pszGUID = GetStringParam(cmd, 0);
	if(!pszGUID)
		return 0;
	if(strlen(pszGUID) > 0)
	{
		const CGUID& rID = GetScriptGuidValue(dynamic_cast<CMoveShape*>(p_SrcShape), pszGUID);
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		if(NULL_GUID == rID)
		{
			return 0;
		}
		GetInst(AntiCheat::CPlayerManager).StopCheck(rID);
	}
	/// ����GUIDΪ�յ����
	else if( p_SrcShape != NULL )
	{
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		GetInst(AntiCheat::CPlayerManager).StopCheck(dynamic_cast<CPlayer*>(p_SrcShape)->GetExID());
	}

	M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
	return 0;
}

double CScript::GetACAttr( const char *cmd, char *retStr )
{
	CHECK_SHAPE( 0 );

	char *strName = GetStringParam( cmd, 0 );
	char *strAttrName = GetStringParam( cmd, 1 );
	if( strName != NULL && strAttrName != NULL )
	{
		CPlayer *player = GetGame()->FindPlayer( strName );
		if( player == NULL )
		{
			player = static_cast<CPlayer*>( this->p_SrcShape );
		}

		if( player == NULL )
		{
			M_FREE(strName, sizeof(char)*MAX_VAR_LEN);
			M_FREE(strAttrName, sizeof(char)*MAX_VAR_LEN);
			return 0;
		}

		long ret = GetInst( AntiCheat::CPlayerManager ).ScriptQueryProperty( player->GetExID(), strAttrName);
		M_FREE(strName, sizeof(char)*MAX_VAR_LEN);
		M_FREE(strAttrName, sizeof(char)*MAX_VAR_LEN);

		return ret;
	}
	M_FREE(strName, sizeof(char)*MAX_VAR_LEN);
	M_FREE(strAttrName, sizeof(char)*MAX_VAR_LEN);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/// �������� :����������϶�ʱ��
/// �����ű�ԭ��: StarCheackAC( ["@playerGUID"] );
/// ��������:     @PlayerGUID : ���GUID, ��ѡ
/// ��������ֵ:   ��
//////////////////////////////////////////////////////////////////////////
double CScript::StartCheackAC(const char *cmd , char *retStr /* = NULL */)
{
	CHECK_SHAPE( 0 );

	char *pszGUID = GetStringParam(cmd, 0);
	if(!pszGUID)
		return 0;
	if ( strlen(pszGUID) >0 )
	{
		const CGUID& rID = GetScriptGuidValue( dynamic_cast<CMoveShape*>(p_SrcShape), pszGUID );
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		if ( NULL_GUID == rID)
		{
			return 0;
		}
		GetInst( AntiCheat::CPlayerManager ).StartCheck(rID);
	}
	else if ( p_SrcShape != NULL)
	{
		M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
		GetInst( AntiCheat::CPlayerManager ).StartCheck( dynamic_cast<CPlayer*>(p_SrcShape)->GetExID());
	}

	M_FREE(pszGUID, sizeof(char)*MAX_VAR_LEN);
	return 0;
}