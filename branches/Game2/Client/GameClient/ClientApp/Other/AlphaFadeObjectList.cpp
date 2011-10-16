#include "stdafx.h"
#include "Shape.h"
#include "Effect.h"
#include "Player.h"
#include "ClientRegion.h"
#include "AlphaFadeObjectList.h"
#include "../../EngineExtend/TextureProjective.h"
#include "../GameClient/Game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
* ����: ���캯��
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
*/
CAlphaFadeObjectList::CAlphaFadeObjectList(void)
{
}


/*
* ����: ��������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
*/
CAlphaFadeObjectList::~CAlphaFadeObjectList(void)
{
	ReleaseAllObject();
}



/*
* ����: �ͷ����ж���
* ժҪ: �ú�������ע�����,���ͷ���Դ
*		 �ڸ��������������Ҳ�����˸ú���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
* �޸���־:
*/
void CAlphaFadeObjectList::ReleaseAllObject()
{
	vector<CShape *>::iterator it = m_vecObjectList.begin();
	while (it != m_vecObjectList.end())
	{
		SAFE_DELETE(*it);
		++it;
	}

	m_vecObjectList.clear();
}



/*
* ����: �ͷ�ָ����������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
* �޸���־:
*/
void CAlphaFadeObjectList::ReleaseObjectById(DWORD dwId)
{
	if ((int)dwId < 0 || dwId > m_vecObjectList.size())
		return;

	CShape * pShape = m_vecObjectList[dwId];
	SAFE_DELETE(pShape);

	m_vecObjectList.erase(m_vecObjectList.begin() + dwId);
}



/*
* ����: ����һ��������������
* ժҪ: ͨ���ú��������Զ����һ���������ģ��
* ����: pShape	- ����ָ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
* �޸���־:
*	2009.05.07 - lpf
*		������Ч����Ч������,���������Ī���ĵ���,�ʽ�ֹ����Ч����Ч��;
*	2009.05.21 - lpf
*		�����޸���һЩģ�͵ײ����,���ֽ���Ч����Ч���ָ�;
*/
void CAlphaFadeObjectList::AddFadeOutObject(CShape * pShape)
{
	if (pShape->GetDisplayModel() == NULL || pShape->GetAnimInfo() == NULL/* ||
																		  pShape->GetType() == TYPE_EFFECT*/)
																		  return;

	CShape * pTmp = NULL;
	pShape->CopyShapeDisplayInfo(pTmp);
	if (pTmp == NULL)
		return;

	pTmp->SetAlphaFadeInStartTime(timeGetTime());
	pTmp->SetAlphaFadeInTimeLength(pShape->GetAlphaFadeInTimeLength());
	pTmp->SetAlphaFadeMode(ALPHA_FADE_OUT);
	pTmp->SetAlphaValue(1.0f);

	m_vecObjectList.push_back(pTmp);
}

/*
* ����: �����б��еĶ���
* ժҪ: ��ɽ��������Ķ�����Ҫɾ��
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
* �޸���־:
*/
void CAlphaFadeObjectList::Update()
{
	long lElapseTime = 0;

	vector<CShape *>::iterator it = m_vecObjectList.begin();
	while (it != m_vecObjectList.end())
	{
		CShape * pTmp = (*it);
		lElapseTime = timeGetTime() - pTmp->GetAlphaFadeInStartTime();

		if (lElapseTime <= pTmp->GetAlphaFadeInTimeLength())
		{
			pTmp->SetAlphaValue((float)(pTmp->GetAlphaFadeInTimeLength() - lElapseTime) / (float)pTmp->GetAlphaFadeInTimeLength());
			++it;
		}else
		{
			SAFE_DELETE(pTmp);
			it = m_vecObjectList.erase(it);
		}
	}
}



/*
* ����: ��Ⱦ�б��еĶ���
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
* �޸���־:
*	2008.10.21 - lpf
*		����������Ⱦ��Чʱ,�������������,�ɼ�����,UV�����ȵȵĴ���;
*	2008.10.23 - lpf
*		�����˶�������ʾ�Ĵ���;
*	2008.10.31 - lpf
*		�����˴�����Ч��ʱ,��ͶӰ����Ľ���;
*	2008.12.12 - lpf
*		ȡ���˽���ʱ����Ӱ��Ⱦ;
*	2008.12.30 - lpf
*		�����˶Խ���ģ���б���,��ʾģ���Ƿ��ȡ��ϵ��ж�,�����ɴ˴����ĵ�������;
*	2009.04.24 - lpf
*		�����ʾʱ,�������ָ�������dynamic_cast��ʽ,�������ת��;
*/
void CAlphaFadeObjectList::Render()
{
	CClientRegion * pRegion = GetGame()->GetRegion();
	if (pRegion == NULL)
		return;

	for (size_t st = 0; st < m_vecObjectList.size(); ++st)
	{
		CShape * pTmp = m_vecObjectList[st];

		if (pTmp->GetDisplayModel() == NULL || pTmp->GetDisplayModel()->GetLoadState() != Load_Did)
			continue;
		pTmp->GetDisplayModel()->SetDirLightEnable(GetGame()->GetSetup()->lLight>0);
		pTmp->GetAnimInfo()->SetViewMatrix(pRegion->GetCamera()->GetViewMatrix());
		pTmp->GetAnimInfo()->SetProjectedMatrix(pRegion->GetCamera()->GetProjectionMatrix());

		switch (pTmp->GetType())
		{
		case TYPE_PLAYER:
			{
				CPlayer * pPlayer = (CPlayer *)pTmp;
				if (pPlayer->IsRide())
					pPlayer->GetHorse()->Display(dynamic_cast<CMoveShape *>(pTmp), false);

				pTmp->GetDisplayModel()->ProcessAnimJoint(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessLocator(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->MultiplyModelAlpha(pTmp->GetAlphaValue());
				pTmp->GetDisplayModel()->RenderModelTransparent();

				pPlayer->GetLeftHandWeapon()->Display(dynamic_cast<CShape *>(pTmp));
				pPlayer->GetRightHandWeapon()->Display(dynamic_cast<CShape *>(pTmp));
				pPlayer->GetWing()->Display(dynamic_cast<CShape *>(pTmp));

				//RenderShadow(pTmp);			//��Ⱦ��Ӱ
			}
			break;
		case TYPE_PET:
		case TYPE_MONSTER:
		case TYPE_COLLECTION:
			{
				pTmp->GetDisplayModel()->ProcessAnimJoint(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessLocator(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->MultiplyModelAlpha(pTmp->GetAlphaValue());
				pTmp->GetDisplayModel()->RenderModelTransparent();
				//RenderShadow(pTmp);			//��Ⱦ��Ӱ
			}
			break;
		case TYPE_EFFECT:
			{
				float fX = pTmp->GetPosX();
				float fY = pTmp->GetPosY();
				float fH = pTmp->GetHeight();
				float fNowDir = pTmp->GetNowDir();
				CEffect * pEffect = dynamic_cast<CEffect *>(pTmp);
				if (pEffect && pEffect->GetHostType() !=0 && pEffect->GetHostID() != CGUID::GUID_INVALID)
				{
					CShape * pShape = dynamic_cast<CShape *>(pRegion->FindChildObject(pEffect->GetHostType(), pEffect->GetHostID()));
					if (pShape)
					{
						pShape->GetLocatorPos(pEffect->GetHostLocator(), fX, fY, fH);
						fNowDir = pShape->GetNowDir();
					}
				}

				pTmp->GetAnimInfo()->SetupAnimInfo(fX, fY, fH, fNowDir, pRegion->GetCamera());
				pTmp->GetAnimInfo()->SetTextureProjectiveAlpha(pTmp->GetAlphaValue());
				pTmp->GetDisplayModel()->ProcessAnimJoint(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessLocator(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessVisibility(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessAnimUV(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessAnimTexture(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->ProcessTextureProjective(pTmp->GetAnimInfo());
				pTmp->GetDisplayModel()->MultiplyModelAlpha(pTmp->GetAlphaValue());
				pTmp->GetDisplayModel()->RenderModelTransparent();
			}
			break;
		}
	}
}



/*
* ����: ��Ⱦָ��ģ����Ӱ
* ժҪ: -
* ����: pShape	- ����ָ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.10.13
* �޸���־:
*	2008.10.20 - lpf
*		�Եõ��İ�ָ���Ƿ�Ϊ��,�������ж�,��ֹ�л�����ʱ����
*/
void CAlphaFadeObjectList::RenderShadow(CShape * pShape)
{
	D3DXVECTOR3 pos;

	pos.x = pShape->GetAnimInfo()->GetWorldMatrix()->_41;
	pos.y = pShape->GetHeight();
	pos.z = pShape->GetAnimInfo()->GetWorldMatrix()->_43;
	render::BoundingBox * pboundbox = pShape->GetDisplayModel()->GetSizeBox();
	if (pboundbox == NULL) return;
	float wid  = (max(pboundbox->GetWidth(),pboundbox->GetLength()));
	float hei  = pboundbox->GetHeight() * 3;
	float size = max(wid,hei) + 0.5f;

	if (size < 2.0f)
		size = 2.0f;

	if (pShape->GetDisplayModel()->GetRendShadow())
	{
		if (GetGame()->GetSetup()->lShadow == 2)
			CTextureProjective::SetSimpleShadow();
		else if (GetGame()->GetSetup()->lShadow == 0)
			CTextureProjective::SetSimpleShadow(true);

		CTextureProjective::BeginRendShadow(pos, (int)size);
		CTextureProjective::AddRendShadow(pShape->GetDisplayModel(),pShape->GetAnimInfo());
		CTextureProjective::EndRendShadow();
	}
}