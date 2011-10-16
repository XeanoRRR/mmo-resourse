#include "StdAfx.h"
#include "EffLine.h"
#include "BuffSkillDef.h"
#include "CondiGroup.h"
#include "..\MoveShape.h"
#include "..\ServerRegion.h"


CEffLine::CEffLine(stModuParam *pParam,CCondiGroup* pCondiGroup)
:CEffectObjects(pParam,pCondiGroup)
{
}

CEffLine::~CEffLine(void)
{
}


//�õ����õ����ö���
unsigned int CEffLine::GetEffectObjs(list<CMoveShape*>& listObjs)
{
	listObjs.clear();
	int nNum = 0;
	list<POINT> listPos;
	CServerRegion* pServerRegion = (CServerRegion*)(m_pParam->pUser->GetFather());
	if(NULL == pServerRegion)	return 0;


	CBaseObject *pObj = pServerRegion->FindChildObject(m_pParam->nMouseShapeType,m_pParam->MouseShapeID);
	m_pParam->pMouseShape = dynamic_cast<CMoveShape*>(pObj);

	if(m_ePos == ePos_Self)
		return 0;
	//Ŀ��
	else if(m_ePos == ePos_Dest && m_pParam->pMouseShape)
	{
		GetLinePointToDest(listPos,
			m_pParam->pUser->GetTileX(),m_pParam->pUser->GetTileY(),
			m_pParam->pMouseShape->GetTileX(),m_pParam->pMouseShape->GetTileY(),m_nDistance);
	}
	//Ŀ�������λ��
	else if(m_ePos == ePos_Coord)
	{
		GetLinePointToDest(listPos,
			m_pParam->pUser->GetTileX(),m_pParam->pUser->GetTileY(),
			m_pParam->nMouseX,m_pParam->nMouseY,m_nDistance);
	}	
	//���λ��
	else if(m_ePos == ePos_Mouse)
	{		
		long lDir = m_pParam->pUser->GetDestDir(m_pParam->pUser->GetTileX(),
							m_pParam->pUser->GetTileY(),
							m_pParam->nMouseX,m_pParam->nMouseY);
		//�õ���Ŀ��֮��ĸ���
		//�õ���Ŀ��֮��ĸ���
		GetLinePointByDir(listPos,m_pParam->pUser->GetTileX(),
									m_pParam->pUser->GetTileY(),lDir,m_nDistance);
	}

	//�����moveshape
	list<POINT>::iterator it = listPos.begin();
	for(;it != listPos.end();it++)
	{
		POINT& pos = (*it);		
		CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pServerRegion->GetShape(pos.x,pos.y));
		if(pMoveShape &&(NULL==m_pCondiGroup || m_pCondiGroup->Result(pMoveShape,m_pParam)))
		{
			nNum++;
			listObjs.push_back(pMoveShape);
		}

	}
	return nNum;
}
