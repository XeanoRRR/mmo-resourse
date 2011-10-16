//=============================================================================
/**
 *  file: BuffSkillFun.cpp
 *
 *  Brief:�����˸�״̬��������صĽű�����
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
#include "../Player.h"
#include "../Monster.h"
#include "../PKSys.h"
#include "../serverregion.h"
#include "../../../setup/LogSystem.h"
#include "../BuffSkill/NewSkillBaseProperties.h"
#include "../BuffSkill/NewSkillFactory.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "../Goods/CGoodsFactory.h"

double CScript::AddState(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	int stateID = 0;
	int level	= 0;
	CMoveShape *p_MoveShape = NULL;
	p_MoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	

	stateID = GetIntParam(CmdStr, 0);
	if(stateID != ERROR_CODE && stateID != INVILID_VALUE && p_MoveShape)
	{
		level = GetIntParam(CmdStr, 1);
		if(level != ERROR_CODE && level != INVILID_VALUE)
		{		
			p_MoveShape->AddNewState(dynamic_cast<CMoveShape*>(p_SrcShape),stateID,level,19);		
		}
	}
	return INVILID_VALUE;
}

double CScript::AddNewState(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	int stateID = 0;
	int level	= 0;
	CMoveShape *p_MoveShape = NULL;
	DWORD dwDest = GetIntParam(CmdStr,2);
	if (dwDest!=ERROR_CODE && dwDest!=INVILID_VALUE)
	{
		p_MoveShape = (CMoveShape*)(dwDest);
	} 
	else
	{
		p_MoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	}


	stateID = GetIntParam(CmdStr, 0);
	if(stateID != ERROR_CODE && stateID != INVILID_VALUE && p_MoveShape)
	{
		level = GetIntParam(CmdStr, 1);
		if(level != ERROR_CODE && level != INVILID_VALUE)
		{		
			p_MoveShape->AddNewState(dynamic_cast<CMoveShape*>(p_SrcShape),stateID,level,20);		
		}
	}
	return INVILID_VALUE;
}


double CScript::RemoverNewState(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	int stateID = 0;
	int level	= 0;
	long lCount = 0;
	CMoveShape *p_MoveShape = NULL;
	DWORD dwDest = GetIntParam(CmdStr,2);
	lCount = GetIntParam(CmdStr,3);

	if (lCount==ERROR_CODE || lCount==INVILID_VALUE)
	{
		lCount = 0;
	}

	if (dwDest!=ERROR_CODE && dwDest!=INVILID_VALUE)
	{
		p_MoveShape = (CMoveShape*)(dwDest);
	} 
	else
	{
		p_MoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	}


	stateID = GetIntParam(CmdStr, 0);
	if(stateID != ERROR_CODE && stateID != INVILID_VALUE && p_MoveShape)
	{
		level = GetIntParam(CmdStr, 1);
		if(level != ERROR_CODE && level != INVILID_VALUE)
		{	
			p_MoveShape->DelNewState(stateID,level,lCount);			
		}
	}
	return INVILID_VALUE;
}


double CScript::AddSkill(const char* CmdStr, char* retStr)
{
	int ret = -1;	//��Ҳ�����

	double lSkillID = GetIntParam(CmdStr,0);
	double lSkillLv = GetIntParam(CmdStr,1);
	if (lSkillLv==ERROR_CODE || lSkillLv==INVILID_VALUE)
		lSkillLv = 1;
	if (lSkillID==ERROR_CODE || lSkillID==INVILID_VALUE)
		return ret;

	CPlayer *pPlayer = NULL;	
	pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);

	if (pPlayer)
	{
		CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(lSkillID);

		if (NULL!=pSkillProperty)
		{
			CNewSkillProperties::tagSkill *ptgSkill = pSkillProperty->GetNewSkill(lSkillLv);
			if (ptgSkill)
			{
				pPlayer->AddSkill(lSkillID,lSkillLv);
				pPlayer->OnBeginSkill(lSkillID,lSkillLv);				
			}
		}		
	}
	return ret;
}
double CScript::DelSkill(const char* CmdStr, char* retStr)
{
	int ret = -1;	//��Ҳ�����
	long lSkillID = GetIntParam(CmdStr,0);
	long lSkillLv = GetIntParam(CmdStr,1);
	if (lSkillLv==ERROR_CODE || lSkillLv==INVILID_VALUE)
	{
		lSkillLv = 1;
	}
	CMoveShape *pShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	if (lSkillID!=ERROR_CODE && p_SrcShape!=NULL && lSkillID!=INVILID_VALUE && pShape)
	{
		pShape->DelSkill(lSkillID,lSkillLv);		
		ret = 1;
	}	
	return ret;
}

double CScript::SetSkillLevel(const char* CmdStr, char* retStr)
{
	int ret = -1;

	return ret;
}
double CScript::GetSkillLevel(const char* CmdStr, char* retStr)
{
	int ret = 0;	
	DWORD dwSkill  = GetIntParam(CmdStr,0);	
	if(dwSkill==ERROR_CODE || dwSkill==INVILID_VALUE)
		return ret;
	stModuParam *pSkill = (stModuParam*)(dwSkill);

	if (pSkill!=NULL)
	{
		ret = pSkill->GetLv();	
	}

	return ret;
}

double CScript::CheckStudySkill(const char* CmdStr, char* retStr)
{
	int ret = 0;	//��Ҳ�����

	CPlayer* pPlayer = (CPlayer*)p_SrcShape;	
	if (pPlayer)
	{
		DWORD lSkillID = GetIntParam(CmdStr,0);
		DWORD dwLv=0;
		if (lSkillID!=ERROR_CODE && lSkillID!=INVILID_VALUE)
		{
			if(pPlayer->CheckStudySkill(lSkillID,dwLv))
			{
				ret = 1;
			}
		}
	}
	return ret;
}

double CScript::GetAllStateNum(const char* CmdStr,char* retStr)
{
	double dRet = 0;
	CMoveShape *pMoveShape=NULL;
	char* strGuid = GetStringParam(CmdStr,0);
	if(strGuid != NULL)
	{
		long nType = GetIntParam(CmdStr,1);
		if (nType!=ERROR_CODE && nType!=INVILID_VALUE)
		{
			CGUID shapeID = GetVariableList()->GetGuidValue(strGuid);
			pMoveShape = GetGame() -> FindShape(OBJECT_TYPE(nType), shapeID );
		}		
	}
	else
	{
		pMoveShape = dynamic_cast<CMoveShape*>(this->p_SrcShape);
	}

	if (pMoveShape)
	{		
		dRet = pMoveShape->GetAllStateNum();
	}

	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

double CScript::DelNewState(const char* CmdStr, char* retStr)
{
	CMoveShape *pMoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	DWORD dwID = GetIntParam(CmdStr,0);
	DWORD dwLv = GetIntParam(CmdStr,1);
	DWORD dwCount = GetIntParam(CmdStr,2);
	if (dwCount==ERROR_CODE || dwCount==INVILID_VALUE)
	{
		dwCount = 0;
	}
	if (pMoveShape && dwID!=ERROR_CODE && dwLv!=ERROR_CODE && dwID!=INVILID_VALUE && dwLv!=INVILID_VALUE)
	{
		pMoveShape->DelNewState(dwID,dwLv,dwCount);
	}
	return 0;
}

double CScript::DelDeBuff(const char* CmdStr, char* retStr)
{
	DWORD dwDes = GetIntParam(CmdStr,0);
	if (dwDes!=ERROR_CODE && dwDes!=INVILID_VALUE)
	{
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if(pDes)
		{
			pDes->EndDeBuff();
		}
	}
	return 0;	
}

double CScript::DelAllBuff(const char* CmdStr, char* retStr)
{
	DWORD dwDes = GetIntParam(CmdStr,0);
	if (dwDes!=ERROR_CODE && dwDes!=INVILID_VALUE)
	{
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if(pDes)
		{
			pDes->EndBuffAndDebuff();
		}
	}
	return 0;
}

double CScript::Modulus(long ModulusType,long OccuTypeA,long OccuTypeB, long LevelA, long LevelB)
{
	long loc_lin9 = 0;
	float qchushi80 = 0.0f;
	float qzzxishu80 = 0.0f;
	float hchushi80 = 0.0f;
	float hzzxishu80 = 0.0f;
	float zhengtxishu = 0.0f;

	switch(ModulusType)
	{
	case 1://����
		{
			//����
			qchushi80 = CPlayerList::GetOccuValueRatio("hit", OccuTypeA);
			qzzxishu80 = 1.5f;
			hchushi80 = CPlayerList::GetOccuValueRatio("hit", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;

			loc_lin9 = LevelA;
		}
		break;
	case 2://���� 
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("dodge", OccuTypeB);
			qzzxishu80 = 0.08f;
			hchushi80 = CPlayerList::GetOccuValueRatio("dodge", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 3://�м� Parry
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("parry", OccuTypeB);
			qzzxishu80 = 2.48f;
			hchushi80 = CPlayerList::GetOccuValueRatio("parry", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 4://�񵵹�ʽ
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("block", OccuTypeB);
			qzzxishu80 = 0.012f;
			hchushi80 = CPlayerList::GetOccuValueRatio("block", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 5://ʩ���ٶ� == ���٣�����
		{			
			qchushi80 = CPlayerList::GetOccuValueRatio("matkspeed", OccuTypeA);
			qzzxishu80 = 0.1f;
			hchushi80 = CPlayerList::GetOccuValueRatio("matkspeed", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 6://רע������
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("absorb", OccuTypeB);
			qzzxishu80 = 0.15f;
			hchushi80 = CPlayerList::GetOccuValueRatio("absorb", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 7://���ˣ�����
		{			
			qchushi80 = 99999.0f;
			qzzxishu80 = 0.0f;
			hchushi80 = 8.0f;
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 8://������ϵ��
		{			
			qchushi80 = CPlayerList::GetOccuValueRatio("cri", OccuTypeA);
			qzzxishu80 = 0.04f;
			hchushi80 = CPlayerList::GetOccuValueRatio("cri", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 9://ħ��ϵ��
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("mcri", OccuTypeA);
			qzzxishu80 = 0.02f;
			hchushi80 = CPlayerList::GetOccuValueRatio("mcri", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 10:
		{
			//�ﱩ������ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("cridamage", OccuTypeA);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("cridamage", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 11:
		{
			//ħ��������ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("mcridamage", OccuTypeA);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("mcridamage", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 12:
		{
			//�ﱩ�˼���ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("cridef", OccuTypeB);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("cridef", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 13:
		{
			//ħ���˼���ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("mcridef", OccuTypeB);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("mcridef", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	default:
		return 0.0f;
	}

	if( loc_lin9 < 25 )
	{
		return 10.0f;
	}
	else if( loc_lin9 >= 80 )
	{
		float loc_a = 0.0f;
		float loc_b = 0.0f;
		float loc_c = 0.0f;

		loc_a = 1 + hzzxishu80;
		loc_b = loc_lin9 - 80;
		loc_c = std::pow(loc_a, loc_b);

		return hchushi80 * loc_c;

	}
	return qchushi80 * (1.0f + qzzxishu80 * (loc_lin9 - 25.0f));

}
//! ����һ��ϵ��������ӿ���Ϊ�ű�Ч���Ż����߼��Ķ������ѯ�߻������
//! ����1��ϵ������
//! ����2��ְҵA
//! ����3��ְҵB
//! ����4���ȼ�A
//! ����5���ȼ�B
//! ����ֵ��һ��ϵ����ʧ�ܷ���0��
double CScript::Modulus(const char* CmdStr, char* retStr)
{
	
	//����ϵ������ű�

	//�����������£�
	//����������������
	//��������ϵ�� = 80ǰ��ʼֵ*(1+80ǰ����ϵ��*(����ȼ�-25))   25-79��
	///		  80���ʼֵ*((1+80������ϵ��)^(����ȼ�-80))  80����

	//localvar("loc_lin9",0);
	long loc_lin9 = 0;

	long ModulusType = GetIntParam(CmdStr, 0);
	if (ERROR_CODE == ModulusType)
		return 0.0f;
	long OccuTypeA = GetIntParam(CmdStr, 1);
	if (ERROR_CODE == OccuTypeA)
		return 0.0f;
	long OccuTypeB = GetIntParam(CmdStr, 2);
	if (ERROR_CODE == OccuTypeB)
		return 0.0f;
	long LevelA = GetIntParam(CmdStr, 3);
	if (ERROR_CODE == LevelA)
		return 0.0f;
	long LevelB = GetIntParam(CmdStr, 4);
	if (ERROR_CODE == LevelB)
		return 0.0f;


	float qchushi80 = 0.0f;
	float qzzxishu80 = 0.0f;
	float hchushi80 = 0.0f;
	float hzzxishu80 = 0.0f;
	float zhengtxishu = 0.0f;

	switch(ModulusType)
	{
	case 1://����
		{
			//����
			qchushi80 = CPlayerList::GetOccuValueRatio("hit", OccuTypeA);
			qzzxishu80 = 1.5f;
			hchushi80 = CPlayerList::GetOccuValueRatio("hit", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;

			loc_lin9 = LevelA;
		}
		break;
	case 2://���� 
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("dodge", OccuTypeB);
			qzzxishu80 = 0.08f;
			hchushi80 = CPlayerList::GetOccuValueRatio("dodge", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 3://�м� Parry
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("parry", OccuTypeB);
			qzzxishu80 = 2.48f;
			hchushi80 = CPlayerList::GetOccuValueRatio("parry", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
	    break;
	case 4://�񵵹�ʽ
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("block", OccuTypeB);
			qzzxishu80 = 0.012f;
			hchushi80 = CPlayerList::GetOccuValueRatio("block", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
	    break;
	case 5://ʩ���ٶ� == ���٣�����
		{			
			qchushi80 = CPlayerList::GetOccuValueRatio("matkspeed", OccuTypeA);
			qzzxishu80 = 0.1f;
			hchushi80 = CPlayerList::GetOccuValueRatio("matkspeed", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 6://רע������
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("absorb", OccuTypeB);
			qzzxishu80 = 0.15f;
			hchushi80 = CPlayerList::GetOccuValueRatio("absorb", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 7://���ˣ�����
		{			
			qchushi80 = 99999.0f;
			qzzxishu80 = 0.0f;
			hchushi80 = 8.0f;
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 8://������ϵ��
		{			
			qchushi80 = CPlayerList::GetOccuValueRatio("cri", OccuTypeA);
			qzzxishu80 = 0.04f;
			hchushi80 = CPlayerList::GetOccuValueRatio("cri", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 9://ħ��ϵ��
		{
			qchushi80 = CPlayerList::GetOccuValueRatio("mcri", OccuTypeA);
			qzzxishu80 = 0.02f;
			hchushi80 = CPlayerList::GetOccuValueRatio("mcri", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 10:
		{
			//�ﱩ������ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("cridamage", OccuTypeA);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("cridamage", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
	    break;
	case 11:
		{
			//ħ��������ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("mcridamage", OccuTypeA);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("mcridamage", OccuTypeA);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelA;
		}
		break;
	case 12:
		{
			//�ﱩ�˼���ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("cridef", OccuTypeB);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("cridef", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	case 13:
		{
			//ħ���˼���ϵ��
			qchushi80 = CPlayerList::GetOccuValueRatio("mcridef", OccuTypeB);
			qzzxishu80 = 1.0f;
			hchushi80 = CPlayerList::GetOccuValueRatio("mcridef", OccuTypeB);
			hzzxishu80 = 0.037f;
			zhengtxishu = 1.0f;
			loc_lin9 = LevelB;
		}
		break;
	default:
			return 0.0f;
	}

	if( loc_lin9 < 25 )
	{
		return 10.0f;
	}
	else if( loc_lin9 >= 80 )
	{
		float loc_a = 0.0f;
		float loc_b = 0.0f;
		float loc_c = 0.0f;

		loc_a = 1 + hzzxishu80;
		loc_b = loc_lin9 - 80;
		loc_c = std::pow(loc_a, loc_b);

		return hchushi80 * loc_c;

	}
	return qchushi80 * (1.0f + qzzxishu80 * (loc_lin9 - 25.0f));
}


double CScript::SetMoveShapeAttrEx(const char* CmdStr, char* retStr)
{
	int ptr    = 0;
	double value = 0;
	double ret = INVILID_VALUE;

	ptr = GetIntParam(CmdStr, 0);
	if(ptr != ERROR_CODE && ptr != INVILID_VALUE)
	{
		char* name = GetStringParam(CmdStr, 1);
		if(name && *(long*)name != ERROR_CODE && *(long*)name != INVILID_VALUE)
		{
			value = GetIntParam(CmdStr, 2);
			if(value != ERROR_CODE && value != INVILID_VALUE)
			{
				ret = SetMoveShapeAttrEx(ptr, string(name), value);
			}
		}
		M_FREE(name, sizeof(char)*MAX_VAR_LEN);
	}
	else // ����Ϊ�ַ���
	{
		AddErrorLogText(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_SETMSATTRFIRSTVARISSTRING));
	}

	return ret;
}
double CScript::GetMoveShapeAttrEx(const char* CmdStr, char* retStr)
{

	int ptr     = 0;
	double value = 0;
	double ret = INVILID_VALUE;

	ptr = GetIntParam(CmdStr, 0);
	if(ptr != ERROR_CODE && ptr != INVILID_VALUE)
	{
		char* name = GetStringParam(CmdStr, 1);
		if(name && *(long*)name != ERROR_CODE && *(long*)name != INVILID_VALUE)
		{
			ret = GetMoveShapeAttrEx(ptr, string(name));
		}
		M_FREE(name, sizeof(char)*MAX_VAR_LEN);
	}
	else // ����Ϊ�ַ���
	{
		AddErrorLogText(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_GETMSATTRFIRSTVARISSTRING));
	}
	return ret;
}
double CScript::ChangeMoveShapeAttrEx(const char* CmdStr, char* retStr)
{
	int ptr     = 0;
	double value = 0;
	double ret = INVILID_VALUE;

	ptr = GetIntParam(CmdStr, 0);
	if(ptr != ERROR_CODE && ptr != INVILID_VALUE)
	{
		char* name = GetStringParam(CmdStr, 1);
		if(name && *(long*)name != ERROR_CODE && *(long*)name != INVILID_VALUE)
		{
			value = GetIntParam(CmdStr, 2);
			if(value != ERROR_CODE && value != INVILID_VALUE)
			{
				ret = ChangeMoveShapeAttrEx(ptr, string(name), value);
			}
		}
		M_FREE(name, sizeof(char)*MAX_VAR_LEN);
	}
	else // ����Ϊ�ַ���
	{
		AddErrorLogText(CStringReading::LoadString(IDS_GS_FUNCTION, STR_GS_FUNCTION_CHANGEMSATTRFIRSTVARISSTRING));
	}
	return ret;
}

double CScript::GetPkResult(const char* CmdStr, char* retStr)
{
	DWORD dwSrc = GetIntParam(CmdStr,0);
	DWORD dwDes = GetIntParam(CmdStr,1);
	int	   nAddSub =  GetIntParam(CmdStr,2);
	if (dwDes!=ERROR_CODE && dwSrc!=ERROR_CODE &&  dwDes!=INVILID_VALUE && dwSrc!=INVILID_VALUE)
	{
		CMoveShape *pSrc = (CMoveShape*)(dwSrc);
		CMoveShape *pDes = (CMoveShape*)(dwDes);

		if(pDes && pSrc && GetPKSys()->JudgeAttackRelation(pSrc,pDes,nAddSub))
		{
			return 1;
		}

	}	
	return 0;

}
double CScript::GetDistance(const char* CmdStr, char* retStr)
{
	double dRet=0.0;
	DWORD dwSrc = GetIntParam(CmdStr,0);
	DWORD dwDes = GetIntParam(CmdStr,1);
	if (dwDes!=ERROR_CODE && dwSrc!=ERROR_CODE && dwDes!=INVILID_VALUE && dwSrc!=INVILID_VALUE)
	{
		CMoveShape *pSrc = (CMoveShape*)(dwSrc);
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if(pSrc && pDes)
		{
			dRet = pSrc->RealDistance(pDes);				
		}
	}
	if (dRet<0)
	{
		dRet = 0.0;
	}

	return dRet;
}

double CScript::AddDeBuffToObj(const char* CmdStr, char* retStr)
{
	DWORD dwSrc = GetIntParam(CmdStr,0);
	DWORD dwDes = GetIntParam(CmdStr,1);
	if (dwDes!=ERROR_CODE && dwSrc!=ERROR_CODE && dwDes!=INVILID_VALUE && dwSrc!=INVILID_VALUE)
	{
		CMoveShape *pSrc = (CMoveShape*)(dwSrc);
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if(pSrc && pDes)
		{
			pDes->GetAI()->WhenBeenHurted(pSrc->GetType(),pSrc->GetExID(),0);			
		}
	}
	return 1.0;
}
double CScript::ReLiveSkill(const char* CmdStr, char* retStr)
{
	DWORD dwSkill = GetIntParam(CmdStr,0);
	if (dwSkill!=ERROR_CODE && dwSkill!=INVILID_VALUE)
	{	
		 stModuParam *pBaseModule = (stModuParam*)dwSkill;
		 CPlayer *pPlayer = (CPlayer*)p_DesShape;
		if (pBaseModule)
		{
			long lRecoverRatio = 0;
			lRecoverRatio = pBaseModule->GetScriptParam(6);
			if (lRecoverRatio>0 && lRecoverRatio<=100)
			{
				pPlayer->SetReliveSkill(true);

				CMessage msg(MSG_S2C_PLAYER_SKILL_RELIVE);
				msg.Add(lRecoverRatio);
				msg.SendToPlayer(pPlayer->GetExID());
			}
			
		}
	}
	return 0;
}
double CScript::SetEventDestObj(const char* CmdStr, char* retStr)
{
	DWORD dwSkill = GetIntParam(CmdStr,0);
	if (dwSkill!=ERROR_CODE && dwSkill!=INVILID_VALUE)
	{
		stModuParam *pBaseModule = (stModuParam*)dwSkill;
		if (pBaseModule)
		{
			tagProcVar *ptgProc = pBaseModule->GetEventProc();
			if (ptgProc && ptgProc->pVar)
			{
				CMoveShape* pMoveShape =  (CMoveShape*)(ptgProc->pVar);	
				if (pMoveShape)
				{
					pBaseModule->SetDesObjs(pMoveShape->GetTileX(),pMoveShape->GetTileY(),pMoveShape->GetType(),pMoveShape->GetExID());
					pBaseModule->SetDestObjByEvent(pMoveShape);
					return 1;
				}
			}
		}
	}
	return 0;
}



double CScript::GetBaseModuleLv(const char* CmdStr, char* retStr)
{
	double ret=0;
	DWORD dwBaseModule = GetIntParam(CmdStr,0);

	if (dwBaseModule!=ERROR_CODE && dwBaseModule!=INVILID_VALUE)
	{
		stModuParam *pBaseModule = (stModuParam*)dwBaseModule;
		if (pBaseModule)
		{
			ret = pBaseModule->GetLv();
		}
	}
	return ret;
}


double CScript::GetShieldNO(const char* CmdStr, char* retStr)
{
	double dRet = 0;
	DWORD dwDest = GetIntParam(CmdStr,0);
	if (dwDest!=ERROR_CODE && dwDest!=INVILID_VALUE)
	{
		CMoveShape *pDest = (CMoveShape*)(dwDest);
		if (pDest)
		{
			CGoods *pEquip = pDest->FindGoods(CGoodsBaseProperties::GT_EQUIPMENT,CEquipmentContainer::EC_WEAPON2);
			if (pEquip)
			{
				dRet =  pEquip->GetAmount();
			}
		} 		
	}
	return dRet;	
}
double CScript::GetObjectType(const char* CmdStr, char* retStr)
{
	double dRet = 0;
	DWORD dwDes = GetIntParam(CmdStr,0);
	if (dwDes!=ERROR_CODE && dwDes!=INVILID_VALUE)
	{
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if (pDes)
		{
			dRet = pDes->GetType();
		}		
	}
	return dRet;

}
double CScript::SetTarget(const char* CmdStr, char* retStr)
{
	DWORD dwSrc = GetIntParam(CmdStr,0);
	DWORD dwDes = GetIntParam(CmdStr,1);
	if (dwSrc!=ERROR_CODE && dwDes!=ERROR_CODE && dwSrc!=INVILID_VALUE && dwDes!=INVILID_VALUE)
	{
		CMoveShape *pSrc = (CMoveShape*)(dwSrc);
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if (pDes && pSrc)
		{
			if( pDes->GetAI()==NULL)	return -1;
			pDes->GetAI()->CBaseAI::SetTarget(pSrc->GetType(),pSrc->GetExID());
		}

	}
	return 0;
}

double CScript::SendSkillList(const char* CmdStr, char* retStr)
{
	char* strNpc =  GetStringParam(CmdStr,0);	
	CPlayer *pShape = dynamic_cast<CPlayer*>(p_SrcShape);
	if (pShape && strNpc!=NULL && p_DesShape)
	{
		pShape->SendBackSkill(strNpc,p_DesShape->GetExID());
	}	
	M_FREE( strNpc, sizeof(char)*MAX_VAR_LEN );	
	return 0;
}

double CScript::GetDesEnergyRatio(const char* CmdStr, char* retStr)
{
	double dRet = 0;
	DWORD dwDes = GetIntParam(CmdStr,0);
	if (dwDes!=ERROR_CODE && dwDes!=INVILID_VALUE)
	{
		CMoveShape *pDes = (CMoveShape*)(dwDes);
		if(NULL == pDes)
			return 0;
		long lParam = GetIntParam(CmdStr,1);
		BOOL bMagic = (BOOL)GetIntParam(CmdStr,2);

		if (lParam!=ERROR_CODE && bMagic!=ERROR_CODE && lParam!=INVILID_VALUE && bMagic!=INVILID_VALUE)
		{
			dRet = pDes->GetReinforceByEnum(lParam,bMagic);		
		}
	}

	return dRet;
}

double CScript::GetEnergyRatio(const char* CmdStr, char* retStr)
{
	double dRet = 0;
	CMoveShape *pMoveShape = dynamic_cast<CMoveShape*>(this->p_SrcShape);
	long lParam = GetIntParam(CmdStr,0);
	int nRet = GetIntParam(CmdStr,1);

	if (lParam!=ERROR_CODE && nRet!=ERROR_CODE && lParam!=INVILID_VALUE && nRet!=INVILID_VALUE && pMoveShape)
	{
		BOOL bMagic = (BOOL)nRet;
		dRet =   pMoveShape->GetReinforceByEnum(lParam,bMagic);		
	}
	return dRet;
}
//
double CScript::GetSkillParam(const char* CmdStr,char* retStr)
{
	double dRet = 0;
	DWORD dwSkillParam = GetIntParam(CmdStr,0);
	int	  nPos = GetIntParam(CmdStr,1);
	if (dwSkillParam!=ERROR_CODE && nPos!=ERROR_CODE && dwSkillParam!=INVILID_VALUE && nPos!=INVILID_VALUE)
	{
		stModuParam *pSkill = (stModuParam*)dwSkillParam;	
		if (pSkill)
		{
			dRet = pSkill->GetScriptParam(nPos);
		}
	}
	return dRet;
}

double CScript::GetSkillUseType(const char* CmdStr, char* retStr)
{
	double dRet =0;
	DWORD dwSkillParam = GetIntParam(CmdStr,0);
	if (dwSkillParam==ERROR_CODE || dwSkillParam==INVILID_VALUE)
	{
		return dRet;
	}
	stModuParam *pSkill = (stModuParam*)dwSkillParam;
	if (pSkill)	
		dRet =  pSkill->GetUseType();
	return dRet;
}

double CScript::GetModType(const char* CmdStr, char* retStr)
{
	double dRet = 0;
	DWORD dwSkillParam = GetIntParam(CmdStr,0);
	if (dwSkillParam==ERROR_CODE || dwSkillParam==INVILID_VALUE)
	{
		return dRet;
	}
	stModuParam *pSkill = (stModuParam*)dwSkillParam;
	if (pSkill)	
		dRet =  pSkill->GetModType();
	return dRet;
}


double CScript::GetSkillElement(const char* CmdStr, char* retStr)
{
	double dRet =0;
	DWORD dwSkillParam = GetIntParam(CmdStr,0);
	if (dwSkillParam==ERROR_CODE || dwSkillParam==INVILID_VALUE)
	{
		return dRet;
	}
	stModuParam *pSkill = (stModuParam*)dwSkillParam;
	if (pSkill)	
		dRet =  pSkill->GetElemnet();
	return dRet;
}
//
double CScript::DeductEnergy(const char* CmdStr,char* retStr)
{
	double dRet = 0;
	CMoveShape *pMoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);	

	int nRet = GetIntParam(CmdStr,0);
	if (nRet!=ERROR_CODE && nRet!=INVILID_VALUE && pMoveShape)
	{
		BOOL bMagic=(BOOL)(nRet);
		pMoveShape->ExpendPower(bMagic);
		dRet = 1;	
	}
	return dRet;
}

double CScript::AddStateToShape(const char* CmdStr,char* retStr)
{
	double dRet = 0.0;
	CMoveShape *pMoveShape=NULL;
	DWORD dwID = GetIntParam(CmdStr,0);
	DWORD dwLv = GetIntParam(CmdStr,1);

	char* strGuid = GetStringParam(CmdStr,2);
	long nType = GetIntParam(CmdStr,3);
	if(strGuid!=NULL && nType!=ERROR_CODE && nType!=INVILID_VALUE)
	{
		CGUID shapeID = GetScriptGuidValue((CMoveShape*)p_SrcShape, strGuid);
		pMoveShape = GetGame() -> FindShape(OBJECT_TYPE(nType), shapeID );
	}
	else
	{
		pMoveShape = dynamic_cast<CMoveShape*>(this->p_SrcShape);
	}
	CMoveShape *pUser = dynamic_cast<CMoveShape*>(this->p_SrcShape);
	if (pMoveShape && dwID!=ERROR_CODE && dwID!=INVILID_VALUE && dwLv!=ERROR_CODE && dwLv!=INVILID_VALUE && pUser)
	{
		pMoveShape->AddNewState(pUser,dwID,dwLv,21);
		dRet = 1.0;
	}
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}


double CScript::GetStateParamByID(const char* CmdStr,char* retStr)
{
	double dRet = 0.0;
	CMoveShape *pMoveShape=NULL;
	DWORD dwID = GetIntParam(CmdStr,0);
	char* strParam = GetStringParam(CmdStr,1);

	char* strGuid = GetStringParam(CmdStr,2);
	if(strGuid!=NULL)
	{
		long nType = GetIntParam(CmdStr,3);
		if (nType!=ERROR_CODE && nType!=INVILID_VALUE  && (CMoveShape*)p_SrcShape)
		{
			CGUID shapeID = GetScriptGuidValue((CMoveShape*)p_SrcShape, strGuid);
			pMoveShape = GetGame() -> FindShape(OBJECT_TYPE(nType), shapeID );
		}		
	}
	else
	{
		pMoveShape = dynamic_cast<CMoveShape*>(this->p_SrcShape);
	}

	if (pMoveShape && dwID!=ERROR_CODE && strParam!=NULL)
	{		
		dRet = pMoveShape->GetStateParamByID(dwID,strParam);
	}

	M_FREE( strParam, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

double CScript::GetStateParamByIndex(const char* CmdStr,char* retStr)
{
	double dRet = 0.0;
	CMoveShape *pMoveShape=NULL;
	DWORD dwIndex = GetIntParam(CmdStr,0);
	char* strParam = GetStringParam(CmdStr,1);

	char* strGuid = GetStringParam(CmdStr,2);
	long nType = GetIntParam(CmdStr,3);
	if(strGuid!=NULL && nType!=ERROR_CODE && nType!=INVILID_VALUE ) 
	{

		CGUID shapeID = GetScriptGuidValue((CMoveShape*)p_SrcShape, strGuid);
		pMoveShape = GetGame() -> FindShape(OBJECT_TYPE(nType), shapeID );
	}
	else
	{
		pMoveShape = dynamic_cast<CMoveShape*>(this->p_SrcShape);
	}

	if (pMoveShape && dwIndex!=ERROR_CODE && dwIndex!=INVILID_VALUE && strParam!=NULL)
	{
		dRet =  pMoveShape->GetStateParamByIndex(dwIndex,strParam);
	}

	M_FREE( strParam, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}
double CScript::CheckStateFirer(const char* CmdStr, char* retStr/* =NULL */)
{
	double dRet = 0.0;
	DWORD dwSkillParam = GetIntParam(CmdStr,0);
	if (dwSkillParam!=ERROR_CODE && dwSkillParam!=INVILID_VALUE)
	{
		stModuParam *pState =  (stModuParam*)dwSkillParam;
		if(pState)
		{
			int nType = pState->GetFirerType();
			CGUID FirerID = pState->GetFirerID();
			CMoveShape *pMoveShape = GetGame()->FindShape((OBJECT_TYPE)nType,FirerID);
			if (NULL!=pMoveShape)
			{
				dRet = 1.0;
			}
		}
		
	}	
	return dRet;

}
double CScript::BeedAttackEvent(const char* CmdStr, char* retStr)
{
	DWORD dwDest = GetIntParam(CmdStr,0);
	if (dwDest!=ERROR_CODE && dwDest!=INVILID_VALUE)
	{
		CMoveShape *pDest = (CMoveShape*)dwDest;
		if(pDest)
			pDest->ProduceEvent(ET_BeAttack);
		return 1.0;
	}
	return 0;
}

double CScript::GetSkillLv(const char* CmdStr, char* retStr)
{
	double dRet = 0.0;
	DWORD dwSkillID = GetIntParam(CmdStr,0);
	if (dwSkillID!=ERROR_CODE && dwSkillID!=INVILID_VALUE)
	{
		CPlayer *pUser = dynamic_cast<CPlayer*>(p_SrcShape);
		if(pUser)
		{
#ifdef _RUNSTACKINFO1_
			char pszStatckInfo[1024]="";
			_snprintf(pszStatckInfo,1024,"CScript::GetSkillLv(Name:%s,SkillID:%d)",pUser->GetName(),dwSkillID);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
			dRet = pUser->GetSkillLv(dwSkillID);
		}
	}
	return dRet;

}

double CScript::ReLiveInScript(const char* CmdStr, char* retStr/* =NULL */)
{
	if (p_SrcShape == NULL)
	{
		return 0;
	}
	DWORD dwDest = GetIntParam(CmdStr,0);
	double dbType = GetIntParam(CmdStr,1);
	if (dwDest != ERROR_CODE && dbType != ERROR_CODE && dwDest != INVILID_VALUE && dbType != INVILID_VALUE)
	{
		CMoveShape *pDestShape = (CMoveShape*)(dwDest);
		if (NULL != pDestShape)
		{
			pDestShape->OnRelive(dbType);
			return 1;
		}
	}
	return 0;
}


double CScript::BeginUseSkill(const char* CmdStr,char* retStr)
{
	double dRet = 0;
	DWORD dwID = 0;
	DWORD dwLevel = 1;
	CPlayer *pShape = (CPlayer*)p_SrcShape;
	dwID = GetIntParam(CmdStr,0);
	dwLevel = GetIntParam(CmdStr,1);

	if (dwID!=ERROR_CODE && dwLevel!=ERROR_CODE && dwID!=INVILID_VALUE && dwLevel!=INVILID_VALUE)
	{
		if (pShape)
		{
			list<long> lSkill = CNewSkillFactory::GetInitOccuSkill(pShape->GetOccupation());
			for (list<long>::iterator it=lSkill.begin(); it!=lSkill.end(); ++it)
			{
				dwID = (*it);			
				CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwID);

				if (NULL!=pSkillProperty)
				{
					CNewSkillProperties::tagSkill *ptgSkill = pSkillProperty->GetNewSkill(dwLevel);
					if (ptgSkill)
					{
						pShape->AddSkill(dwID,dwLevel);					
					}
				}
			}
			dRet = 1;
		}
	}

	return dRet;
}
//��ȡĿ�������Ĵ�ɱ��
double CScript::GetKillerInfo(const char* cmdStr, char* retStr)
{
	double dRet = 0.0;
	
	char *strGuid = GetStringParam(cmdStr,0);
	char *strType = GetStringParam(cmdStr,1);
	CMoveShape *pMoveShape = (CMoveShape*)(p_SrcShape);
	if (NULL!=strGuid  && strType!=NULL && pMoveShape)
	{
		CGUID guidKiller = NULL_GUID;
		long lKillerType = 0;
		pMoveShape->GetKillerInfo(guidKiller,lKillerType);
		GetVariableList()->SetGuidValue(strGuid,&(guidKiller));
		GetVariableList()->SetVarValue(strType,0,lKillerType);
		dRet = 1.0;
	}
	
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strType, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

//�ڼ�����Ŀ�������˵�guid������
double CScript::GetBeneficiaryInfo(const char* cmdStr, char* retStr)
{
	double dRet = 0.0;
	DWORD dwObject = GetIntParam(cmdStr,0);
	char *strGuid = GetStringParam(cmdStr,1);
	char *strType = GetStringParam(cmdStr,2);

	if (NULL!=strGuid  && strType!=NULL )
	{
		CMonster  *pMonster =(CMonster*)(dwObject);
		if (pMonster)		
		{
			CPlayer *pBeneficiary = pMonster->GetBeneficiary();
			if (pBeneficiary)
			{
				GetVariableList()->SetGuidValue(strGuid,&(pBeneficiary->GetExID()));
				GetVariableList()->SetVarValue(strType,0,pBeneficiary->GetType());			
				dRet = 1.0;
			}
			else
			{
				if (p_SrcShape)
				{
					GetVariableList()->SetGuidValue(strGuid,&(p_SrcShape->GetExID()));
					GetVariableList()->SetVarValue(strType,0,p_SrcShape->GetType());			
					dRet = 1.0;
				}
				
			}
		}
	}
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strType, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

//���ݲ�������õ���Ӧ��guid
double CScript::GetGuidByParam(const char* cmdStr, char* retStr)
{
	char* strGuid = GetStringParam(cmdStr,0);
	if (strGuid)
	{
		DWORD dwDest = GetIntParam(cmdStr,1);
		if (dwDest!=ERROR_CODE)
		{
			CMoveShape* pDest = (CMoveShape*)(dwDest);
			if (pDest)
			{
				int isFind = INVILID_VALUE;
				if(p_SrcShape)
					isFind = SetScriptGuidValue((CMoveShape*)p_SrcShape, strGuid, pDest->GetExID());
				if(isFind != INVILID_VALUE)
				{
					M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
					return 1;
				}
			}
		}
	}
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

//���ݶ������нű�
double CScript::RunScriptByDest(const char* cmdStr,char* retStr)
{
	DWORD dwDest = GetIntParam(cmdStr,0);
	char* strScript = GetStringParam(cmdStr,1);
	if (dwDest==ERROR_CODE || strScript==NULL)
	{
		M_FREE( strScript, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	CMoveShape *pDest = (CMoveShape*)(dwDest);
	if (pDest)
	{
		stRunScript stRunScr;
		stRunScr.srcShape = pDest;
		stRunScr.desShape = NULL;
		stRunScr.pszFileName = strScript;

		RunScript(&stRunScr,(char*)GetGame()->GetScriptFileData(strScript));
	}
	M_FREE( strScript, sizeof(char)*MAX_VAR_LEN );
	return 1;
}




double CScript::GetGoodsAddProperty(const char* CmdStr, char* retStr)
{
	double ret=0.0;
	if(p_SrcShape)
	{	
		CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
		int nAddonId=(int)GetIntParam(CmdStr,0);	
		int nValId=(int)GetIntParam(CmdStr,1);
		DWORD dwSkill = GetIntParam(CmdStr,2);
		if (nAddonId!=ERROR_CODE && nAddonId!=INVILID_VALUE && nValId!=ERROR_CODE && nValId!=INVILID_VALUE && dwSkill!=ERROR_CODE && dwSkill!=INVILID_VALUE)
		{
			stModuParam *pSkill = (stModuParam*)dwSkill;
			if (pSkill)
			{
				long lGoodsIndex = pSkill->GetGoodsIndex();
				CGoodsBaseProperties *pProperty=CGoodsFactory::QueryGoodsBaseProperties(lGoodsIndex);
				if (pProperty)
				{
					ret = pProperty->GetAddonPropertyValue(static_cast<GOODS_ADDON_PROPERTIES>(nAddonId),nValId);
				}
			}			
		}		
	}
	return ret;
}


double  CScript::GetGoodsPlacePos(const char* CmdStr, char* retStr)
{
	double dRet = 0.0;
	CPlayer*  pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	DWORD dwPlace = 0;
	DWORD dwPos = 0;
	char* strPlace = GetStringParam(CmdStr,0);
	char* strPos = GetStringParam(CmdStr,1);
	DWORD dwSkill = GetIntParam(CmdStr,2);


	if (strPlace!=NULL && strPos!=NULL && pPlayer && dwSkill!=ERROR_CODE && dwSkill!=INVILID_VALUE)
	{
		stModuParam *pSkill = (stModuParam*)dwSkill;
		if (pSkill)
		{
			dwPlace = pSkill->GetContainerID();
			dwPos = pSkill->GetPos();
			SetScriptVarValue(pPlayer,strPlace,0,dwPlace);
			SetScriptVarValue(pPlayer,strPos,0,dwPos);		
			dRet = 1;
		}
	}
	M_FREE( strPlace, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strPos, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}

double CScript::SucceedUseItem(const char* CmdStr, char* retStr)
{
	double dRet = 0.0;
	CPlayer*  pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	DWORD dwSkill = GetIntParam(CmdStr,0);
	if (dwSkill!=ERROR_CODE && dwSkill!=INVILID_VALUE && pPlayer)
	{
		stModuParam *pSkill = (stModuParam*)dwSkill;
		if (pSkill)
		{
			DWORD dwGoodsIndex = pSkill->GetGoodsIndex();
			if(pPlayer->UpdateCooldown(dwGoodsIndex))
				pPlayer->SendUseItemSucceed(dwGoodsIndex);

		}
	}
	return dRet;
}
double CScript::DelGoodsByPlacePos(const char* CmdStr, char* retStr)
{
	double dRet = 0.0;
	CPlayer*  pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	char* strPlace = GetStringParam(CmdStr,0);
	char* strPos = GetStringParam(CmdStr,1);
	if (strPlace!=NULL && strPos!=NULL && pPlayer)
	{
		DWORD dwPlace = 0;
		DWORD dwPos = 0;
		dwPlace = GetScriptVarValue(pPlayer,strPlace,0);
		dwPos = GetScriptVarValue(pPlayer,strPos,0);
		pPlayer->UseItem(dwPlace,dwPos);		
		dRet = 1.0;
	}
	M_FREE( strPlace, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strPos, sizeof(char)*MAX_VAR_LEN );
	return dRet;
}
//��ͻ��˷��ͽ����Ʒ������Ϣ
double CScript::SendUnLockGoodsMsg(const char* CmdStr, char* retStr)
{
	double dRet = 0.0;
	CPlayer*  pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	DWORD dwPlace = GetIntParam(CmdStr,0);
	DWORD dwPos = GetIntParam(CmdStr,1);
	if (pPlayer && dwPlace!=INVILID_VALUE && dwPlace!=ERROR_CODE && dwPos!=INVILID_VALUE && dwPos!=ERROR_CODE )
	{
		CMessage msg(MSG_S2C_SKILL_UNLOCK_GOODS);
		msg.Add(dwPlace);
		msg.Add(dwPos);
		msg.SendToPlayer(pPlayer->GetExID());
		dRet = 1.0;
	}	
	return dRet;
}

//ȡ�ü��ܶ�������ʱ��
double CScript::GetActModify(const char* cmd, char* retStr)
{
	if (p_SrcShape==NULL) return -1;

	CMonster *pMonster= dynamic_cast<CMonster*>(p_SrcShape);
	if (pMonster== NULL) return -1;

	DWORD dwSkillParam = GetIntParam(cmd,0);
	stModuParam *pSkill = (stModuParam*)dwSkillParam;
	if (pSkill==NULL)
	{
		return -1;
	}
	long lSkillID = pSkill->GetID();
	return pMonster->GetActModify(lSkillID);

	return 0;
}
double CScript::OnCostYuanBao(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if (pPlayer== NULL) 
		return 0.0;

	long lNum = GetIntParam(cmd,0);
	if (lNum!=ERROR_CODE && lNum!=INVILID_VALUE)
	{
		pPlayer->OnCostYuanBao(lNum);
	}
	return 1.0;
}

//��ȡ��ǰ����������
double CScript::GetAttackerType(const char* cmd, char* retStr)
{
	double dbType = 0;
	CMoveShape *pMoveShape = dynamic_cast<CMoveShape*>(p_SrcShape);
	if (pMoveShape)
	{
		dbType = pMoveShape->GetAttackerType();
	}
	return dbType;
}

//��ȡ�Ʒ��˺�
double CScript::GetPartDamageOut(const char* cmd, char* retStr)
{
	//ͨ���˺����㹫ʽ
	double dbSkillseven = GetIntParam(cmd, 0);//���ܱ���7
	double dbSkilleight = GetIntParam(cmd, 1);//���ܼ�ֵ8
	double dbBaseDamage = GetIntParam(cmd, 2);//��һ�ű������˺�ֵ
	double dbMType      = GetIntParam(cmd, 3);//��������ϵ��
	double dbPrevent    = GetIntParam(cmd, 4);//δ�Ʒ��˺�
	double dbBModifier  = GetIntParam(cmd, 5);//B����ϵ��
	double dbBUTTUP     = GetIntParam(cmd, 6);//�����˺�����
	double dbALEVELT    = GetIntParam(cmd, 7);
	double dbBLEVELT    = GetIntParam(cmd, 8);
	int    nACTYPE      = GetIntParam(cmd, 9);
	int	   nBCTYPE      = GetIntParam(cmd, 10);
	double dbEquipStrA  = GetIntParam(cmd, 11);

	if (dbSkillseven==ERROR_CODE	|| dbSkillseven==INVILID_VALUE
		|| dbSkilleight==ERROR_CODE	|| dbSkilleight==INVILID_VALUE
		|| dbBaseDamage==ERROR_CODE	|| dbBaseDamage==INVILID_VALUE
		|| dbALEVELT==ERROR_CODE	|| dbALEVELT==INVILID_VALUE
		|| dbBLEVELT==ERROR_CODE	|| dbBLEVELT==INVILID_VALUE
		|| dbMType==ERROR_CODE		|| dbMType==INVILID_VALUE
		|| dbPrevent==ERROR_CODE	|| dbPrevent==INVILID_VALUE
		|| dbBModifier==ERROR_CODE	|| dbBModifier==INVILID_VALUE
		|| dbBUTTUP==ERROR_CODE		|| dbBUTTUP==INVILID_VALUE
		|| nACTYPE==ERROR_CODE		|| nACTYPE==INVILID_VALUE
		|| nBCTYPE==ERROR_CODE		|| nBCTYPE==INVILID_VALUE
		|| dbEquipStrA==ERROR_CODE	|| dbEquipStrA==INVILID_VALUE)
	{
		return 0.0;
	}


	double dbPartDamageOut = 0.0;
	double dbEquipStrLD = 0.0;
	double dbPvPDR = 0.0;

	dbPartDamageOut = dbBaseDamage * dbSkillseven + dbSkilleight;
	dbPartDamageOut = dbPartDamageOut * dbMType;
	//���˹�ʽ��ֵdbPrevent����Ϊ���˱���
	dbPartDamageOut = dbPartDamageOut * dbPrevent;
	dbPartDamageOut = dbPartDamageOut * dbBModifier;

	if( nACTYPE == 400 )
	{
		//����ǿ�ȴ�߼��ֹ�ʽ
		dbEquipStrLD = dbBLEVELT - (dbEquipStrA + 7);
			//�������ȼ���Խ����ǿ�ȶ��� ��7����˼�����ٵȼ���Ҫ��Խ����ǿ��7���ſ�ʼ���ˣ�
			if(dbEquipStrLD > 0)
			{
				dbEquipStrLD = 1 - (dbEquipStrLD * 0.15);
				//ÿ��Խ1��ɼ���0.15���˺�
				if(dbEquipStrLD < 0.2)
				{
					dbEquipStrLD = 0.2;
				}
				//��ͼ���0.2��
				dbPartDamageOut = dbPartDamageOut * dbEquipStrLD;
			}
			dbPartDamageOut = dbPartDamageOut * dbBUTTUP;
			if( nBCTYPE == 400 )
			{
				//PVP�ȼ��������˹�ʽ
				dbPvPDR = dbALEVELT/10 + 4;
				//PVPʱ������ÿ10���˺�����1��
				dbPartDamageOut = dbPartDamageOut/dbPvPDR;			
			}	
	}

	if( dbPartDamageOut < 1 )
	{
		dbPartDamageOut =1;
	}
	return dbPartDamageOut;
}

//��ȡδ�Ʒ��˺�
double CScript::GetPrevent(const char* cmd, char* retStr)
{

	double dbAmianshang = GetIntParam(cmd, 0); //����������
	double dbAchuanci   = GetIntParam(cmd, 1);   //�����ߴ���
	int    nPAoccu      = GetIntParam(cmd, 2);	   //������ְҵ
	int    nPBoccu      = GetIntParam(cmd, 3);	   //������ְҵ
	double dbALEVELT    = GetIntParam(cmd, 4);
	double dbBLEVELT    = GetIntParam(cmd, 5);

	if (dbAmianshang==ERROR_CODE		|| dbAmianshang==INVILID_VALUE
		|| nPAoccu==ERROR_CODE			|| nPAoccu==INVILID_VALUE
		|| nPBoccu==ERROR_CODE			|| nPBoccu==INVILID_VALUE
		|| dbALEVELT==ERROR_CODE		|| dbALEVELT==INVILID_VALUE
		|| dbBLEVELT==ERROR_CODE		|| dbBLEVELT==INVILID_VALUE
		|| dbAchuanci==ERROR_CODE	    || dbAchuanci==INVILID_VALUE)
	{
		return 0.0;
	}

	
	double dbPrevent = 0.0;
	double dbDrTop = 0.75;

	dbPrevent = Modulus(7, nPAoccu, nPBoccu, dbALEVELT, dbBLEVELT);
	//ʵ��������  = ( ����ֵ �C��͸ֵ ) / ����ϵ�� / 100
	dbPrevent = ( dbAmianshang - dbAchuanci ) / dbPrevent / 100;
	if( dbPrevent > 1 )
	{
		dbPrevent = 1;
	}
	if( dbPrevent < 0 )
	{
		dbPrevent = 0;
	}
	//���˱�������
	dbPrevent = 1 - ( dbDrTop * dbPrevent );
	return dbPrevent;

}
//��ȡB����ϵ��
double CScript::GetBModifier(const char* cmd, char* retStr)
{
	double dbShibienum	   = GetIntParam(cmd, 0);
	int    nPAoccu		   = GetIntParam(cmd, 1);
	int    nPBoccu		   = GetIntParam(cmd, 2);
	double dbALEVELT	   = GetIntParam(cmd, 3);
	double dbBLEVELT	   = GetIntParam(cmd, 4);
	double dbCriDamageup   = GetIntParam(cmd, 5);
	double dbCriDamagedown = GetIntParam(cmd, 6);
	double dbTCri		   = GetIntParam(cmd, 7);    
	int	   nAType		   = GetIntParam(cmd, 8);
	int    nBType		   = GetIntParam(cmd, 9);
	char*  strBlastResult  = GetStringParam(cmd, 10);	

	if (dbShibienum==ERROR_CODE			|| dbShibienum==INVILID_VALUE
		|| nPAoccu==ERROR_CODE			|| nPAoccu==INVILID_VALUE
		|| nPBoccu==ERROR_CODE			|| nPBoccu==INVILID_VALUE
		|| dbALEVELT==ERROR_CODE		|| dbALEVELT==INVILID_VALUE
		|| dbBLEVELT==ERROR_CODE		|| dbBLEVELT==INVILID_VALUE
		|| dbCriDamageup==ERROR_CODE	|| dbCriDamageup==INVILID_VALUE
		|| dbCriDamagedown==ERROR_CODE	|| dbCriDamagedown==INVILID_VALUE
		|| dbTCri==ERROR_CODE			|| dbTCri==INVILID_VALUE
		|| nAType==ERROR_CODE			|| nAType==INVILID_VALUE
		|| nBType==ERROR_CODE			|| nBType==INVILID_VALUE
		|| strBlastResult==NULL)
	{
		if(NULL != strBlastResult)
			M_FREE(strBlastResult, sizeof(char)*MAX_VAR_LEN);
		return 0.0;
	}

	double dbBModifier = 0.0;
	double dbGgxzxishu = 0.0;
	double dbDjxiuzhengxs = 0.0;
	double dbCriDamageupxs = 0.0;
	double dbCriDamageupl = 0.0;
	double dbCriDamagedownl = 0.0;
	double dbShijilinshi = 0.0;
    double dbGxzbflv = 0.0;
	int nRNo = 0;
	int nBlastResult = 0;	

	//�ȼ�������ϵ��
	dbDjxiuzhengxs = ( 2 * dbALEVELT ) / ( dbALEVELT + dbBLEVELT );
	//ʶ����8�ﱬ����
	if( dbShibienum == 8 )
	{
		dbGgxzxishu = Modulus(10, nPAoccu, nPBoccu, dbALEVELT, dbBLEVELT);
		//ԭʼ�ﱩ������ϵ��
		dbCriDamageupxs = dbGgxzxishu;
		dbGgxzxishu = Modulus(12, nPAoccu, nPBoccu, dbALEVELT, dbBLEVELT);
		//ԭʼ�ﱩ�˼���ϵ��
		dbCriDamagedown = dbGgxzxishu;
		//ʶ������8
		dbShibienum = 8;
	}
	//ʶ����9ħ������
	if( dbShibienum == 9 )
	{
		dbGgxzxishu = Modulus(11, nPAoccu, nPBoccu, dbALEVELT, dbBLEVELT);
		//ԭʼħ��������ϵ��
		dbCriDamageupxs = dbGgxzxishu;
		dbGgxzxishu = Modulus(13, nPAoccu, nPBoccu, dbALEVELT, dbBLEVELT);
		//ԭʼħ���˼���ϵ��
		dbCriDamagedown = dbGgxzxishu;
		//ʶ������9
		dbShibienum = 9;
	}

	//�����˺�������
	dbCriDamageupl = dbCriDamageup / dbCriDamageupxs / 100;
	if( nAType == 600 )
	{
		dbCriDamageupl = 0;
	}

	//�����˺�������
	dbCriDamagedownl = dbCriDamagedown / dbCriDamagedown / 100;
	if( nBType == 600 )
	{
		dbCriDamagedownl = 0;
	}

	if( dbCriDamageupl > 0.5 )
	{
		dbCriDamageupl = 0.5;
	}
	if( dbCriDamagedownl > 0.5 )
	{
		dbCriDamagedownl = 0.5;
	}

	if( dbALEVELT < 80 )
	{
		//�����˺�������
		dbCriDamageupl = 0;
		//�����˺�������
		dbCriDamagedownl = 0;
	}

	//������������
	dbShijilinshi = dbTCri;	
	dbGgxzxishu = Modulus(dbShibienum, nPAoccu, nPBoccu, dbALEVELT, dbBLEVELT);
	dbGxzbflv = dbTCri / dbGgxzxishu / 100;

	//ʵ���ﱩ�ٷ��� =  ԭʼ�ﱩ�ٷ��� * �ȼ�������ϵ�� / ������������
	dbTCri = dbGxzbflv * dbDjxiuzhengxs;

	//����50%����
	if( dbTCri > 0.5 )
	{
		dbTCri = 0.5;
	}
	//�������������⼼�ܻᵼ�¼���������
	if( dbShijilinshi > 5000 )
	{
		dbTCri = (dbShijilinshi/1000) / 10;
		if( dbTCri >= 1 )
		{
			dbTCri = 1;
		}
	}
	dbTCri = dbTCri * 1000;

	dbBModifier = 1;
	//��ʼ�������˺�
	nRNo = ::random(1000);

	//����ܷ���ֱ���
	if( nRNo <= dbTCri )
	{
		nBlastResult = 1;	
		//�����˺����㣬"�����˺�"��"��������"	
		dbBModifier = 1.6 + dbCriDamageupl - dbCriDamagedownl;	
	}
	SetScriptVarValue((CMoveShape*)p_SrcShape, strBlastResult, 0, nBlastResult);

	M_FREE(strBlastResult, sizeof(char)*MAX_VAR_LEN);

	return dbBModifier;	
}

//��ȡħ��ϵ��
double CScript::GetFunMoHua(const char* cmd, char* retStr)
{
	double dbMohuaC2xishu = GetIntParam(cmd, 0);
	int    nPAoccu		  = GetIntParam(cmd, 1);
	int	   nPBoccu		  = GetIntParam(cmd, 2);
	double dbTAttack	  = GetIntParam(cmd, 3); 
	double dbDefTure	  = GetIntParam(cmd, 4);
	double dbLoc_C_MinAtk = GetIntParam(cmd, 5);
	DWORD  dwFirerPtr	  = GetIntParam(cmd, 6);
	DWORD  dwDestPtr	  = GetIntParam(cmd, 7);

	if (dbMohuaC2xishu==ERROR_CODE  || dbMohuaC2xishu==INVILID_VALUE
		|| nPAoccu==ERROR_CODE		|| nPAoccu==INVILID_VALUE
		|| nPBoccu==ERROR_CODE		|| nPBoccu==INVILID_VALUE
		|| dbTAttack==ERROR_CODE	|| dbTAttack==INVILID_VALUE
		|| dbDefTure==ERROR_CODE	|| dbDefTure==INVILID_VALUE
		|| dbLoc_C_MinAtk==ERROR_CODE || dbLoc_C_MinAtk==INVILID_VALUE
		|| dwFirerPtr==ERROR_CODE	  || dwFirerPtr==INVILID_VALUE
		|| dwDestPtr==ERROR_CODE	  || dwDestPtr==INVILID_VALUE)
	{
		return 0.0;
	}

	CMoveShape *pFirer = ((CMoveShape*)dwFirerPtr);
	CMoveShape *pDest = ((CMoveShape*)dwDestPtr);
	if(pFirer==NULL || pDest==NULL)
	{
		return 0.0;
	}

	int nGong = 0;
	int nFang = 0;
	double dbB1 = 0.0;
	double dbG2 = 0.0;
	double dbXis1 = 0.0;
	double dbXis2 = 0.0;
	double dbf2 = 0.0;
	double dbB2 = 0.0;

	double dbGdi = 0.0;
	double dbGshui = 0.0;
	double dbGhuo = 0.0;
	double dbGfeng = 0.0;
	double dbGguang = 0.0;
	double dbGan = 0.0;

	double dbFdi = 0.0;
	double dbFshui = 0.0;
	double dbFhuo = 0.0;
	double dbFfeng = 0.0;
	double dbFguang = 0.0;
	double dbFan = 0.0;

	double dbGdi_xiang = 0.0;
	double dbGshui_xiang = 0.0;
	double dbGhuo_xiang = 0.0;
	double dbGfeng_xiang = 0.0;
	double dbGguang_xiang = 0.0;
	double dbGan_xiang = 0.0;


	double dbFdi_xiang = 0.0;
	double dbFshui_xiang = 0.0;
	double dbFhuo_xiang = 0.0;
	double dbFfeng_xiang = 0.0;
	double dbFguang_xiang = 0.0;
	double dbFan_xiang = 0.0;

	double dbB   = 0.0;
	double dbC	 = 0.0;
	double dbC1	 = 0.0;
	double dbC3  = 0.0;
	double dbC11 = 0.0;
	double dbC12 = 0.0;
	double dbC13 = 0.0;
	double dbC14 = 0.0;
	double dbC15 = 0.0;
	double dbC16 = 0.0;	

	//����սʿϵ
	if( nPAoccu == 1 || nPAoccu == 4 || nPAoccu == 5 || ( nPAoccu >= 10 && nPAoccu <= 13 ) )
	{
		nGong = 1;
	}
	//��������ϵ
	if( nPAoccu == 3 || nPAoccu == 6 || nPAoccu == 7 || ( nPAoccu >= 14 && nPAoccu <= 17 ) )
	{
		nGong = 2;
	}
	//���߷�ʦϵ
	if( nPAoccu == 2 || nPAoccu == 8 || nPAoccu == 9 || ( nPAoccu >= 18 && nPAoccu <= 21 ) )
	{
		nGong = 3;
	}

	//����սʿϵ
	if( nPBoccu == 1 || nPBoccu == 4 || nPBoccu == 5 || ( nPBoccu >= 10 && nPBoccu <= 13 ) )
	{
		nFang = 1;
	}
	//��������ϵ
	if( nPBoccu == 3 || nPBoccu == 6 || nPBoccu == 7 || ( nPBoccu >= 14 && nPBoccu <= 17 ) )
	{
		nFang = 2;
	}
	//���߷�ʦϵ
	if( nPBoccu == 2 || nPBoccu == 8 || nPBoccu == 9 || ( nPBoccu >= 18 && nPBoccu <= 21 ) )
	{
		nFang = 3;
	}
	//�������ǹ���
	if( nPAoccu == 0 )
	{
		nGong = 1;
	}
	//�������ǹ���
	if( nPBoccu == 0 )
	{
		nFang = 1;
	}

	if( nGong == 0 || nFang == 0 )
	{
		nGong = 1;
		nFang = 1;
	}
	//B1ϵ�����
	if( nGong == 1 && nFang == 1 )
	{
		dbB1 = 1;
	}
	if( nGong == 1 && nFang == 2 )
	{
		dbB1 = 1.5;
	}
	if( nGong == 1 && nFang == 3 )
	{
		dbB1 = 2;
	}
	if( nGong == 2 && nFang == 1 )
	{
		dbB1 = 1.5;
	}
	if( nGong == 2 && nFang == 2 )
	{
		dbB1 = 2;
	}
	if( nGong == 2 && nFang == 3 )
	{
		dbB1 = 2.5;
	}
	if( nGong == 3 && nFang == 1 )
	{
		dbB1 = 1.5;
	}
	if( nGong == 3 && nFang == 2 )
	{
		dbB1 = 1.125;
	}
	if( nGong == 3 && nFang == 3 )
	{
		dbB1 = 1.425;
	}

	if( nGong == 1 )
	{
		dbG2 = 1;
		dbXis1 = 2;
		dbXis2 = 0.1;
		if( nFang == 1 )
		{
			dbf2 = 1;
		}
		if( nFang == 2 )
		{
			dbf2 = 0.8;
		}
		if( nFang == 3 )
		{
			dbf2 = 0.6;
		}
	}
	if( nGong == 2 )
	{
		dbG2 = 1.143;
		dbXis1 = 2;
		dbXis2 = 0.1;
		if( nFang == 1 )
		{
			dbf2 = 1;
		}
		if( nFang == 2 )
		{
			dbf2 = 0.8;
		}
		if( nFang == 3 )
		{
			dbf2 = 0.6;
		}
	}
	if( nGong == 3 )
	{
		dbTAttack = dbLoc_C_MinAtk;
		dbG2 = 1.5;
		dbXis1 = 2;
		dbXis2 = 0.5;
		if( nFang == 1 )
		{
			dbf2 = 1.5;
		}
		if( nFang == 2 )
		{
			dbf2 = 1.65;
		}
		if( nFang == 3 )
		{
			dbf2 = 1.53;
		}
	}

	dbB2 = ( dbTAttack * dbXis1 / dbG2 ) / ( dbDefTure * dbXis1 / dbf2 ) / 1.4;

	dbGdi		= pFirer->GetAttribute("Earth_Attack");
	dbGshui		= pFirer->GetAttribute("Water_Attack");
	dbGhuo		= pFirer->GetAttribute("Fire_Attack");
	dbGfeng		= pFirer->GetAttribute("Wind_Attack");
	dbGguang	= pFirer->GetAttribute("Bright_Attack");
	dbGan		= pFirer->GetAttribute("Dark_Attack");

	dbFdi		= pDest->GetAttribute("Earth_Defend");
	dbFshui		= pDest->GetAttribute("Water_Defend");
	dbFhuo		= pDest->GetAttribute("Fire_Defend");
	dbFfeng		= pDest->GetAttribute("Wind_Defend");
	dbFguang	= pDest->GetAttribute("Bright_Defend");
	dbFan		= pDest->GetAttribute("Dark_Defend");

	if( dbFdi < 180 )
	{
		dbFdi = 180;
	}
	if( dbFshui < 180 )
	{
		dbFshui = 180;
	}
	if( dbFhuo < 180 )
	{
		dbFhuo = 180;
	}
	if( dbFfeng < 180 )
	{
		dbFfeng = 180;
	}
	if( dbFguang < 180 )
	{
		dbFguang = 180;
	}
	if( dbFan < 180 )
	{
		dbFan = 180;
	}

	dbGdi_xiang		 = pFirer->GetAttribute("Earth_AttackXiang");
	dbGshui_xiang	 = pFirer->GetAttribute("Water_AttackXiang");
	dbGhuo_xiang	 = pFirer->GetAttribute("Fire_AttackXiang");
	dbGfeng_xiang	 = pFirer->GetAttribute("Wind_AttackXiang");
	dbGguang_xiang	 = pFirer->GetAttribute("Bright_AttackXiang");
	dbGan_xiang		 = pFirer->GetAttribute("Dark_AttackXiang");

	dbFdi_xiang		 = pDest->GetAttribute("Earth_DefendXiang");
	dbFshui_xiang	 = pDest->GetAttribute("Water_DefendXiang");
	dbFhuo_xiang	 = pDest->GetAttribute("Fire_DefendXiang");
	dbFfeng_xiang	 = pDest->GetAttribute("Wind_DefendXiang");
	dbFguang_xiang	 = pDest->GetAttribute("Bright_DefendXiang");
	dbFan_xiang		 = pDest->GetAttribute("Dark_DefendXiang");

	if( dbGdi_xiang < 0 )
	{
		dbGdi_xiang = 0;
	}
	if( dbGshui_xiang < 0 )
	{
		dbGshui_xiang = 0;
	}
	if( dbGhuo_xiang < 0 )
	{
		dbGhuo_xiang = 0;
	}
	if( dbGfeng_xiang < 0 )
	{
		dbGfeng_xiang = 0;
	}
	if( dbGguang_xiang < 0 )
	{
		dbGguang_xiang = 0;
	}
	if( dbGan_xiang < 0 )
	{
		dbGan_xiang = 0;
	}
	if( dbFdi_xiang < 0 )
	{
		dbFdi_xiang = 0;
	}
	if( dbFshui_xiang < 0 )
	{
		dbFshui_xiang = 0;
	}
	if( dbFhuo_xiang < 0 )
	{
		dbFhuo_xiang = 0;
	}
	if( dbFfeng_xiang < 0 )
	{
		dbFfeng_xiang = 0;
	}
	if( dbFguang_xiang < 0 )
	{
		dbFguang_xiang = 0;
	}
	if( dbFan_xiang < 0 )
	{
		dbFan_xiang = 0;
	}

	dbC11 = dbGdi * 200 * ( dbGdi_xiang * 3 / 100 + 1 ) / ( dbFdi * ( dbFdi_xiang * 3 / 100 + 1 ) );
	dbC12 = dbGshui * 200 * ( dbGshui_xiang * 3 / 100 + 1 ) /  ( dbFshui * ( dbFshui_xiang * 3 / 100 + 1 ) );
	dbC13 = dbGhuo * 200 * ( dbGhuo_xiang * 3 / 100 + 1 ) / ( dbFhuo * ( dbFhuo_xiang * 3 / 100 + 1 ) );
	dbC14 = dbGfeng * 200 * ( dbGfeng_xiang * 3 / 100 + 1 ) / ( dbFfeng * ( dbFfeng_xiang * 3 / 100 + 1 ) );
	dbC15 = dbGguang * 200 * ( dbGguang_xiang * 3 / 100 + 1 ) / ( dbFguang * ( dbFguang_xiang * 3 / 100 + 1 ) );
	dbC16 = dbGan * 200 * ( dbGan_xiang * 3 / 100 + 1 ) / ( dbFan * ( dbFan_xiang * 3 / 100 + 1 ) );

	dbC1 = dbC11 + dbC12 + dbC13 + dbC14 + dbC15 + dbC16;
	dbC3 = 1 + ( pFirer->GetAttribute("Level") - pDest->GetAttribute("Level") ) * 0.02;

	if( dbC3 > 1.3 )
	{
		dbC3 = 1.3;
	}
	if( dbC3 < 0 )
	{
		dbC3 = 0;
	}
	dbB = dbB1 * dbB2;
	dbC = dbC1 * dbMohuaC2xishu * dbC3;
	return dbB * dbC;
}

