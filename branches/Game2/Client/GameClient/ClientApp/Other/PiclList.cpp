#include "StdAfx.h"
#include ".\modellist.h"
#include "..\..\gameExtend\GameModel.h"
#include "..\BaseObject.h"
#include "..\shape.h"
#include "..\player.h"
#include "..\npc.h"
#include "..\monster.h"
#include "..\goods.h"
#include "..\clientregion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CModelList::CModelList(void)
{
	Create();
}

CModelList::~CModelList(void)
{
	ReleaseAll();
	Destroy();
}

CModelList::tagModel* CModelList::Load(long lType, long lGraphicsID, bool bFind)
{
	// Ҫ��������е�ͼ����Դ
	if( bFind == true )
	{
		tagModel* pModel = GetModel( lType, lGraphicsID );
		if( pModel )	// ���ҵ�ͼ��
		{
			pModel->lUseCount++;	// ʹ������1
			return pModel;
		}
	}

	// ���ļ���ȡͼ������
	tagModel* pModel = new tagModel;
	pModel->lType = lType;
	pModel->lGraphicsID = lGraphicsID;
	pModel->lUseCount = 1;

	char strbuf[MAX_PATH];
	switch(lType)
	{
	case TYPE_PLAYER:
		sprintf(strbuf,	"model/player/%03d/config.txt", lGraphicsID);
		break;

	case TYPE_MONSTER:
		sprintf(strbuf,	"model/monster/%03d/config.txt", lGraphicsID);
		break;

	case TYPE_NPC:
		sprintf(strbuf,	"model/npc/%03d/config.txt", lGraphicsID);
		break;

	case TYPE_GOODS:
		sprintf(strbuf,	"model/goods/%03d/config.txt", lGraphicsID);
		break;

	default:
		SAFE_DELETE( pModel );
		return NULL;
	}

	pModel->pModel = new GameModel;
	pModel->pModel->Create(strbuf);

	m_listModel.push_back(pModel); 

	return pModel;
}

BOOL CModelList::Release(long lType, long lGraphicsID)
{
	tagModel* pModel = NULL;

	// �����Ƿ��и���Դ
	list<tagModel*>::iterator it;
	for(it = m_listModel.begin(); it != m_listModel.end(); it++)
	{
		pModel = (*it);

		// ��Դ�б����������Դ
		if( pModel->lType == lType && pModel->lGraphicsID == lGraphicsID )
		{
			pModel->lUseCount--;
			if( pModel->lUseCount == 0 )
			{
				pModel->pModel->Destroy();
				SAFE_DELETE( pModel->pModel );
				SAFE_DELETE( pModel );
				m_listModel.erase( it );
			}
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CModelList::ReleaseAll(void)
{
	tagModel* pModel = NULL;

	// �����Ƿ��и���Դ
	list<tagModel*>::iterator it;
	for( it = m_listModel.begin(); it != m_listModel.end(); it++ )
	{
		pModel = (*it);
		pModel->pModel->Destroy();
		SAFE_DELETE( pModel->pModel );
		SAFE_DELETE( pModel );
	}
	m_listModel.clear();

	return TRUE;
}

BOOL CModelList::Display(CShape *pShape, float x, float y, int Alpha)
{
	BOOL ret = FALSE;
	long lType = pShape->GetType();
	long lGraphicsID = pShape->GetGraphicsID();
	long lDir = pShape->GetDir();
	long lActionTimer = pShape->GetActionTimer();
	long lState = pShape->GetState();
	long lAct = pShape->GetAction();

	tagModel* pModel = GetModel( lType, lGraphicsID );
	if( pModel == NULL )
	{
		// ��ͼ, ���ò����Ƿ������ͼ����Դ, ��Ϊ��������
		pModel = Load(lType, lGraphicsID, false);
	}

	if( pModel && pModel->pModel )
	{
		switch(lType)
		{
		// ���
		case TYPE_PLAYER:
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pShape);
				if (pPlayer==NULL)
					return ret;

				char str[8];
				sprintf(str, "%04d", pPlayer->GetFacePic());
				pModel->pModel->SelPart("head",str);		// ��(����)

				pModel->pModel->SelPart("helm","0001");		// ͷ��
				pModel->pModel->SelPart("body","0001");		// ����
				pModel->pModel->SelPart("rhwp","0001");		// ����
				pModel->pModel->SelPart("glov","0001");		// ����
				pModel->pModel->SelPart("boot","0001");		// ѥ��			
				pModel->pModel->SelPart("back","0001");		// ��

				static char *strState[] = {"P0", "H0", "F"};
				static char *strAction[] = {"S0", "I0", "W0", "R0", "A0", "H0", "D0", "S"};
				lState +=2;
				if (lState>=CShape::STATE_FIGHT)
				{
					if (lAct>=CShape::ACT_PERFORM)
					{
						sprintf(str, "F%dS%d", lState-CShape::STATE_FIGHT, lAct-CShape::ACT_PERFORM);
					}
					else
					{
						sprintf(str, "F%d%s", lState-CShape::STATE_FIGHT, strAction[lAct]);
					}
				}
				else
				{
					if (lAct>=CShape::ACT_PERFORM)
					{
						sprintf(str, "%sS%d", strState[lState], lAct-CShape::ACT_PERFORM);
					}
					else
					{
						sprintf(str, "%s%s", strState[lState], strAction[lAct]);
					}
				}

				pModel->pModel->SetAction(str);
			}
			break;

		// ���NPC����Ʒ
		case TYPE_MONSTER:
		case TYPE_NPC:
		case TYPE_GOODS:
			pModel->pModel->SelPart("body", "0000");
			switch (lAct)
			{
			case CShape::ACT_STAND:
				pModel->pModel->SetAction("stand");
				break;

			case CShape::ACT_WALK:
				pModel->pModel->SetAction("move");
				break;

			case CShape::ACT_DIED:
				pModel->pModel->SetAction("dead");
				break;

			case CShape::ACT_ATTACK:
				pModel->pModel->SetAction("attack");
				break;

			case CShape::ACT_BEATTACK:
				pModel->pModel->SetAction("hurt");
				break;
			}
			break;
		}

		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matWorld,y,0,x);
		float fDirect =  (float)((lDir * 45 - 90) % 360);
		D3DXMATRIX matRot;
		D3DXMatrixRotationY(&matRot,D3DXToRadian(fDirect));
		D3DXMatrixMultiply(&matWorld,&matRot,&matWorld);

		long newtime = timeGetTime();
		if (pModel->pModel->ProcessVertices(&matWorld, lActionTimer,newtime,1.0f))
		{
			if(lAct!=CShape::ACT_DIED)
				pShape->SetActionTimer(newtime);
			ret = TRUE;
		}

		pModel->pModel->Render();

		//��Ⱦ
		//������ʾ�����ȡ���������ȾӰ��

		D3DXMATRIX matShadow;

		render::Camera *pCamera = GetGame()->GetRegion()->GetCamera();

		D3DXVECTOR3 *pCameraPos = pCamera->GetPosition();

		D3DXVECTOR4 vLight = D3DXVECTOR4(y+ 1,4,x + 1,1.0f);

		D3DXPLANE plane;
		D3DXPlaneFromPoints(&plane,&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(1,0,0),&D3DXVECTOR3(0,0,1));

		D3DXMatrixShadow(&matShadow,&vLight,&plane);
		for(DWORD dwGroup = 0 ; dwGroup < pModel->pModel->GetPartGroupCount(); dwGroup++)
		{
			GameModel::PartGroup *pPartGroup = pModel->pModel->GetPartGroup(dwGroup);

			GameModel::PartGroup::Part * pPart = pPartGroup->GetSelPart();
			if (pPart)
			{
				model::Buffer *pBuffer = pPart->GetBuffer();				

				D3DXVECTOR3 *pPosBuffer = pBuffer->GetPosBuffer();
				for(DWORD dwPos = 0; dwPos < pBuffer->GetPosCount(); dwPos++)
				{
					D3DXVec3TransformCoord(&pPosBuffer[dwPos],&pPosBuffer[dwPos],&matShadow);
					pPosBuffer[dwPos].y += 0.01f;

				}	
				pBuffer->Render(m_dwShadowShaderGroupHandle ,0,0);
			}
		}

	}

	return ret;
}

// ��ȡ
CModelList::tagModel* CModelList::GetModel(long lType, long lGraphicsID)
{
	tagModel* pModel = NULL;

	// �����Ƿ��и���Դ
	list<tagModel*>::iterator it;
	for(it = m_listModel.begin(); it != m_listModel.end(); it++)
	{
		pModel = (*it);

		// ��Դ�б����������Դ
		if( pModel->lType == lType && pModel->lGraphicsID == lGraphicsID )
		{
			return pModel;
		}
	}

	return NULL;
}

void CModelList::Create()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Layer3D *pLayer3D = pInterface->GetLayer3D();

	m_dwShadowShaderGroupHandle = pLayer3D->CreateShaderGroupFromFile("model\\effect\\shadow");



}
void CModelList::Destroy()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Layer3D *pLayer3D = pInterface->GetLayer3D();
	pLayer3D->DestroyShaderGroup(m_dwShadowShaderGroupHandle);

}