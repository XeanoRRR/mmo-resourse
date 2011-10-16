// Fox@20081230----------------------------------------------
// File:    PetFun.h
// Brief:   ������ؽű�.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Dec.30, 2008
// Fox@20081230----------------------------------------------

#include "StdAfx.h"
#include "Script.h"
#include "../player.h"
#include "../Pet.h"


// ���ݳ�������ID��������
double CScript::CreatePet(const char* pCmd, char* pRetStr)
{
	long lType = GetIntParam(pCmd, 0);
	long lID = GetIntParam(pCmd, 1);
	GetInst(CPetCtrl).CreatePet((CPlayer*)p_SrcShape, lType, lID);

	return 0;
}

// ���ݳ���GUIDɾ������
double CScript::DelPet(const char* pCmd, char* pRetStr)
{
	char* szGUID = GetStringParam(pCmd, 0);
	if( szGUID != NULL )
	{
		const CGUID& rID = GetScriptGuidValue(NULL, szGUID);
		M_FREE( szGUID, sizeof(char)*MAX_VAR_LEN );
		GetInst(CPetCtrl).DelPet(rID);
	}

	return 0;
}

// ���ݳ������ʹ�������
double CScript::RelivePet(const char* pCmd, char* pRetStr)
{
	long lType = GetIntParam(pCmd, 0);
	long lID = GetIntParam(pCmd, 1);
	GetInst(CPetCtrl).RelivePet((CPlayer*)p_SrcShape, lType, lID);

	return 0;
}

// Ϊ��ҵ�ǰ������Ӽ���
double CScript::AddPetSkill(const char* pCmd, char* pRetStr)
{
	long lType = GetIntParam(pCmd, 0);
	long lID = GetIntParam(pCmd, 1);
	long lLevel = GetIntParam(pCmd, 2);
	long lPos = GetIntParam(pCmd, 3);
	GetInst(CPetCtrl).AddPetSkill((CPlayer*)p_SrcShape, lType, lID, lLevel, lPos);

	return 0;
}

// ��ȡ��ǰ�������GUID
double CScript::GetFocusPet(const char* pCmd, char* pRetStr)
{
	char* szGUID = GetStringParam(pCmd, 0);
	if( szGUID != NULL )
	{
		int isFind = INVILID_VALUE;
		if( p_SrcShape != NULL)
		{
			SetScriptGuidValue((CMoveShape*)p_SrcShape,
				szGUID, ((CPlayer*)p_SrcShape)->GetFocusPet());
		}
		M_FREE( szGUID, sizeof(char)*MAX_VAR_LEN );
	}

	return 0;
}

// ��ȡĳ���ͳ���GUID
double CScript::GetPet(const char* pCmd, char* pRetStr)
{
	long lType = GetIntParam(pCmd, 0);
	long lID = GetIntParam(pCmd, 1);
	char* szGUID = GetStringParam(pCmd, 2);
	if( szGUID != NULL )
	{
		int isFind = INVILID_VALUE;
		if( p_SrcShape != NULL)
		{
			CGUID ID;
			CPet* pPet = ((CPlayer*)p_SrcShape)->GetActivePet(lType);
			if (pPet != NULL && pPet->GetDiffPro().lIndex == lID)
			{
				ID = pPet->GetExID();
			}
			else
			{
				ID = NULL_GUID;
			}
			SetScriptGuidValue((CMoveShape*)p_SrcShape,
				szGUID, ID);
		}

		M_FREE( szGUID, sizeof(char)*MAX_VAR_LEN );
	}

	return 0;
}
