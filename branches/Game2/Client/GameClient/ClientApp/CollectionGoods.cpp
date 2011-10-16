////////////////////////////////////////////////////////////////////////////
//CollectionGoods.cpp///////////////////////////////////////////////////////////////////
//�ɼ��࣬�������������ͷ���
//Author:Ф��
//Create Time:2008.2.3
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\CollectionGoods.h"
#include ".\Other\DeputyOccuSystemClient.h"
#include "../../EngineExtend\DisplayModel.h"
#include "../../EngineExtend\GameModelManager.h"
#include "../../../Public/Common/Public.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CGameModelManager;

CCollectionGoods::CCollectionGoods()
{
	m_lCreatorType = 0;			// ���������ߵ�����
	m_CreatorID = CGUID::GUID_INVALID;
	//m_dwCollectionGoodsID = 0;
	//m_bCanBeCollect = false;
	//m_strCollectionGoodsShowName = "";		//�ɼ������ʾ����
	//m_strCollectionGoodsOriName = "";		//�ɼ����ԭʼ����
	//m_eOccuID = DOccu_Num;					//��ְҵid
	//m_dwPicID = 0;							//ͼ��id
	//m_dwRadarID = 0;						//С��ͼ��־(�״�id)
	//m_dwMixTimes = 0;						//��С�ɼ�����
	//m_dwMaxTimes = 0;						//���ɼ�����
	//m_dwCollectLevel = 0;					//�ɼ��ȼ�
	//m_dwRoleID = 0;							//��Ҫ����ID
	//m_dwRoleStep = 0;						//��Ҫ������
}

//���òɼ����״̬
void CCollectionGoods::SetAction(WORD l)
{

	if( m_wAction != l )
	{
		m_wAction = l;
		OnActionChange();
	}
}



/*
 * ����: ��ʼ����ʾģ��
 * ժҪ: -
 * ����: bAsyn - �Ƿ�ͬ����ʾ
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.10.27 - lpf
 *		�ɼ�������趨��ʾģ��Ϊͬ����ʾ(������һ�ν���Ϸ,���ڲɼ��������趨�ȼ�,��������޷���ʾģ������);
 */
void CCollectionGoods::SetDisplayModelGroup(bool bAsyn)
{
	if (this->GetDisplayModel()!=NULL)
	{
		this->GetDisplayModel()->ShowGroup(MAKEFOURCC('0','0','0','0'),MAKEFOURCC('m','e','s','h'),0xffffffff,false);
	}
}

////���õȼ�
//void CCollectionGoods::SetModelLevel()
//{
//	if (this->GetDisplayModel()!=NULL)
//	{
//		DWORD dwGroupName;
//		DWORD dwPartName;
//		DWORD dwLevelNum;
//		DWORD dwLevel;
//		CDisplayModel::CDisplayGroup* pGroup;
//		CDisplayModel::itDisplayGroup it = this->GetDisplayModel()->GetGroups().begin();
//		for (;it != this->GetDisplayModel()->GetGroups().end();it++ )
//		{
//			dwGroupName = (*it).first;
//			pGroup = (*it).second;
//			dwPartName = pGroup->GetPartName();
//			GameModel* pgamemodel =  CGameModelManager::GetInstance()->GetGameModel(this->GetDisplayModel()->GetGameModelID());
//			if(pgamemodel!=NULL)
//			{
//				std::map<DWORD,GameModel::Group*>::iterator GropIter = pgamemodel->GetGroupArray()->find(dwGroupName);
//				GameModel::Group* pgroup = GropIter->second;
//				if (pgroup!=NULL)
//				{
//					std::map<DWORD,GameModel::Group::Part*>::iterator PartIter = pgroup->GetPartArray()->find(dwPartName);
//					GameModel::Group::Part* pPart = PartIter->second;
//					if (pPart!=NULL)
//					{
//						dwLevelNum = pPart->GetLevelCount();
//						dwLevel = random((int)dwLevelNum);
//						this->GetDisplayModel()->ShowGroup(dwGroupName,dwPartName,dwLevel);
//					}
//				}
//			}
//		}
//	}
//}

//�ɼ������ʾ
bool CCollectionGoods::Display(void)
{
	CMoveShape::Display();
	return true;
}


//�л��ɼ����״̬
void CCollectionGoods::OnActionChange()
{
	static char str[32];

	AnimInfo::tagActionInfo *pActionInfo = m_pAnimInfo->GetActionInfo();
	//---------------------------------------------
	pActionInfo->dwCurActionStartTime = GetCurTickCount();
	pActionInfo->dwCurActionLoopCount = 0;

	if (m_wAction == CShape::ACT_STAND || m_wAction == CShape::ACT_INTONATE
		|| m_wAction == CShape::ACT_WALK || m_wAction == CShape::ACT_RUN 
		|| m_wAction == CShape::ACT_MEGRIM || m_wAction == CShape::ACT_ALREADYDIED )
	{
		pActionInfo->bCurActionLooped = TRUE;
	}
	else
	{
		pActionInfo->bCurActionLooped = FALSE;
	}

	// �����վ������
	if (m_wAction!=ACT_STAND)
	{
		m_lIdleCount = 0;
	}

	static DWORD dwSTND = MAKEFOURCC('S','T','N','D');
	static DWORD dwDIED = MAKEFOURCC('D','I','E','D');
	//static DWORD dwOPEN = MAKEFOURCC('O','P','E','N');
	static DWORD dwHURT = MAKEFOURCC('H','U','R','T');
	static DWORD dwDIE1 = MAKEFOURCC('D','I','E','1');

	switch (m_wAction)
	{
	case CShape::ACT_STAND:
		pActionInfo->dwCurAction = dwSTND;
		break;

	case CShape::ACT_DIED:
		pActionInfo->dwCurAction= dwDIED;
		break;

	// ���壺�Ѿ�����
	case ACT_ALREADYDIED:
			pActionInfo->dwCurAction = dwDIE1;
		break;

	case CShape::ACT_BEATTACK:
		pActionInfo->dwCurAction= dwHURT;
		break;

	}
}

// ����
bool CCollectionGoods::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CMoveShape::DecordFromByteArray(pByte, pos, bExData);
	// ���񱣻���Ϣ(���ͺ�GUID)
	m_lCreatorType = _GetLongFromByteArray(pByte,pos);
	_GetBufferFromByteArray(pByte,pos,m_CreatorID);
	return true;
}

bool CCollectionGoods::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);

	// ���񱣻���Ϣ(���ͺ�GUID)
	m_lCreatorType = setReadDB.GetLongFromByteArray();
	setReadDB.GetBufferFromByteArray(m_CreatorID);
	m_dwCollectID		= setReadDB.GetDwordFromByteArray();
	if (m_dwCollectID!=0)
	{
		const CCollectionLayout* CollectLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
		if (CollectLayout!=NULL)
		{
			m_strCollectionName = CollectLayout->GetCollectionName();
		}
	}
	m_bIsBeenCollect        = (setReadDB.GetByteFromByteArray() == 0)?false : true;
	m_bCanBeenCollect		= (setReadDB.GetByteFromByteArray() == 0)?false : true;
	return true;
}

//�ɼ����AI
void CCollectionGoods::AI()
{
	CShape::AI();

	switch( GetAction() )
	{
	case ACT_STAND:
	case ACT_IDLE:
		if( AI_Stand() == RET_AI_BREAK ) return;
		break;
	case ACT_BEATTACK:
		if( AI_BeAttack() == RET_AI_BREAK ) return;
		break;
	case ACT_DIED:
		if( AI_Died() == RET_AI_BREAK ) return;
		break;
	}
}

//��ȡ��Ʒ����ʾ����
string CCollectionGoods::GetColletctionGoodsShowName()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetCollectionName();	//�ɼ������ʾ����
	}
	return "";
}

//��ȡ��Ʒ��ԭʼ��
string CCollectionGoods::GetCollectionGoodsOriName()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetOrginName();	//�ɼ����ԭʼ����
	}
	return "";
}

//��ȡ��ְҵ��ID
DWORD  CCollectionGoods::GetOccuID()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetOccuID();		//��ְҵid
	}
	return 0;
}

//��ȡ�ɼ�����ʾģ�͵�ID
DWORD  CCollectionGoods::GetPicID()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetPicID();		//ģ��id
	}
	return 0;
}

//��ȡ�״��ͼ��ID
DWORD  CCollectionGoods::GetRadarID()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetRadarID();		//С��ͼ��־(�״�id)
	}
	return 0;
}

//��ȡ��С�ɱ��ɼ��Ĵ���
DWORD  CCollectionGoods::GetMixTimes()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{	
		return pCollectionLayout->GetMinTimes();	//��С�ɼ�����
	}
	return 0;
}

//��ȡ���ɱ��ɼ��Ĵ���
DWORD  CCollectionGoods::GetMaxTimes()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetMaxTime();	//���ɼ�����
	}
	return 0;
}

//��ȡ�ɼ���Ҫ��Ĳɼ��ȼ�
DWORD  CCollectionGoods::GetCollectLevel()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetCollectLevel();	//�ɼ��ȼ�
	}
	return 0;
}

//��ȡ����ID
int  CCollectionGoods::GetRoleID()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetRoleID();   //��Ҫ����ID
	}
	return 0;
}

//��ȡ����ID�б�
const vector<tagQuestGoods>* CCollectionGoods::GetRoleInfo()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return &(pCollectionLayout->GetRoleInfo());
	}
	return NULL;
}

//��ȡ�ɼ���ı��ɼ��������б�
const list<tgConditionGoods>* CCollectionGoods::GetConditionList()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return &(pCollectionLayout->GetConditionList());
	}
	return NULL;
}

//��ȡ����׶�
DWORD  CCollectionGoods::GetRoleStep()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetRoleStep();//��Ҫ������
	}
	return 0;
}

///��ȡ�ɼ��˲ɼ���ʱʹ�õĶ���ID
DWORD  CCollectionGoods::GetActionID()
{
	const CCollectionLayout* pCollectionLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	if (pCollectionLayout!=NULL)
	{
		return pCollectionLayout->GetActionID();
	}
	return 0;
}