#include "stdafx.h"
#include "RollLantern.h"
#include "../../EngineExtend/CBitmapX.h"

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
* ��������: 2009.01.05
*	2009.01.07 - lpf
*		������FontInfo�ĳ�ʼ����ֵ
*/
CRollLantern::CRollLantern(void)
{
	m_fSpeed	  = 0.1f;				//���ݹ����ٶ�(����/����)
	m_dwNumLoop   = 1;					//ѭ������

	// ����ƻ�������(������ͼƬ��������)
	m_ptPos.x = 100;
	m_ptPos.y = 100;

	m_lContentOffsetHeight = 5;
	m_pBackGroundPic	   = NULL;				//����ͼƬ
	m_bIsFinished		   = true;				//������Ѿ���ʾ���
	m_lContentOffset	   = 0;					//���ݻ���ƫ��
	m_lContentOffsetLeft   = 0;
	m_lContentOffsetRight  = 0;
	m_dwStartTime		   = 0;					//����ƿ�ʼʱ��
	m_dwLoopCount		   = 0;					//����ѭ������
	m_fAlphaPer			   = 0.0f;
	m_dwAlphaTimeLen	   = 5000;				//����ʱ�䳤��
	m_dwAlphaStartTime	   = 0;
	m_eAlphaMode		   = ALPHA_FADE_NONE;	//����Ƴ�����ʧ�Ľ���ģʽ

	m_stFontInfo.iStyleIndex	= 0;
	m_stFontInfo.bBorder		= false;
	m_stFontInfo.bShadow		= true;
	m_stFontInfo.dwShadowOffset = 1;
	m_stFontInfo.fScale			= 1.0f;
	m_stFontInfo.dwColorFont	= 0xffff0000;
	m_stFontInfo.dwColorBorder	= 0xff000000;
	m_stFontInfo.dwColorShadow	= 0x80000000;

	m_pOffscreenLeft  = NULL;
	m_pOffscreenRight = NULL;

	// �ü�������(���������������ϵ,���������ƻ�������)
	SetClip(150, 650);
}



/*
* ����: ��������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
*/
CRollLantern::~CRollLantern(void)
{
	//SAFE_DECREF(m_pBackGroundPic);
	SAFERELEASE(m_pOffscreenLeft);
	SAFERELEASE(m_pOffscreenRight);
}



/*
* ����: �õ����ݹ���ƫ������ֵ
* ժҪ: �����������Ϸ����������ϵ
* ����: -
* ����ֵ: ƫ������ֵ
* ����: lpf
* ��������: 2009.01.05
*	2009.01.09 - lpf
*		Ϊ��������ƫ��ֵ����-1������������Ӧ���ڵ����������ʾ
*/
float CRollLantern::GetContentOffsetLeft()
{
	m_lContentOffsetLeft = m_ptPos.x + (m_lClipLeft - (long)m_sContent.length() * 8) - 1;
	return (float)m_lContentOffsetLeft;
}



/*
* ����: �õ����ݹ���ƫ������ֵ
* ժҪ: �����������Ϸ����������ϵ
* ����: -
* ����ֵ: ƫ������ֵ
* ����: lpf
* ��������: 2009.01.05
*/
float CRollLantern::GetContentOffsetRight()
{
	m_lContentOffsetRight = m_ptPos.x + m_lClipRight;
	return (float)m_lContentOffsetRight;
}



/*
* ����: �趨����ƻ�������(������ͼƬ��������)
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
*/
void CRollLantern::SetPos(POINT & pt)
{
	m_ptPos = pt;

	GetContentOffsetLeft();
	GetContentOffsetRight();
	UpdateClipRect();
}



/*
* ����: �趨�ü�������(���������������ϵ,���������ƻ�������)
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
*/
void CRollLantern::SetClip(long lLeft, long lRight)
{
	m_lClipLeft  = lLeft;
	m_lClipRight = lRight;

	GetContentOffsetLeft();
	GetContentOffsetRight();
	UpdateClipRect();
}



/*
* ����: �趨��ʾ����
* ժҪ: �ú��������һЩ��������
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
*/
void CRollLantern::SetContent(const char * psz)
{
	m_sContent = psz;

	GetContentOffsetLeft();
	GetContentOffsetRight();
}



/*
* ����: �趨����ͼƬ
* ժҪ: -
* ����: pszFilePathName - ����ͼƬ·����
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
*/
bool CRollLantern::SetBGPic(const char * pszFilePathName)
{
	//SAFE_DECREF(m_pBackGroundPic);

	m_pBackGroundPic = CBitmapX::GetBitmapX(pszFilePathName);
	if (m_pBackGroundPic == NULL)
		return false;
	else
		return true;
}



/*
* ����: ��ʼ��ʾ�����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
* �޸���־:
*/
void CRollLantern::Start()
{
	m_bIsFinished	   = false;
	m_dwAlphaStartTime = timeGetTime();
	m_eAlphaMode	   = ALPHA_FADE_IN;
	m_dwLoopCount	   = 0;
	m_lContentOffset   = 0;
}



/*
* ����: �����б��еĶ���
* ժҪ: ��ɽ��������Ķ�����Ҫɾ��
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
* �޸���־:
*/
void CRollLantern::Update()
{
	if (m_bIsFinished)
		return;

	DWORD dwElapseTime  = 0;
	DWORD dwCurrentTime = timeGetTime();

	// ����ͬ�Ľ׶�
	switch (m_eAlphaMode)
	{
	case ALPHA_FADE_IN:
		{
			dwElapseTime = dwCurrentTime - m_dwAlphaStartTime;
			m_fAlphaPer  = (float)dwElapseTime / (float)m_dwAlphaTimeLen;

			if (m_fAlphaPer > 1.0f)
			{
				m_fAlphaPer	  = 1.0f;
				m_eAlphaMode  = ALPHA_FADE_NONE;
				m_dwStartTime = dwCurrentTime;
			}
		}
		break;
	case ALPHA_FADE_NONE:
		{
			dwElapseTime	 = dwCurrentTime - m_dwStartTime;
			m_lContentOffset = (long)(m_fSpeed * (float)dwElapseTime);
			m_lContentOffset *= -1;

			if (m_lContentOffset + m_lContentOffsetRight < m_lContentOffsetLeft)
			{
				m_dwLoopCount++;

				// ����Ƿ�ѭ�����
				if (m_dwLoopCount == m_dwNumLoop)
				{
					m_dwAlphaStartTime = dwCurrentTime;
					m_eAlphaMode	   = ALPHA_FADE_OUT;
					m_lContentOffset   = m_lContentOffsetLeft - m_lContentOffsetRight;
				}else
				{
					m_lContentOffset = 0;
					m_dwStartTime	 = dwCurrentTime;
				}
			}
		}
		break;
	case ALPHA_FADE_OUT:
		{
			dwElapseTime = dwCurrentTime - m_dwAlphaStartTime;
			m_fAlphaPer  = (float)((float)m_dwAlphaTimeLen - (float)dwElapseTime) / (float)m_dwAlphaTimeLen;

			if (m_fAlphaPer < 0.0f)
			{
				m_fAlphaPer		 = 0.0f;
				m_eAlphaMode	 = ALPHA_FADE_NONE;
				m_dwLoopCount	 = 0;
				m_lContentOffset = 0;
				m_bIsFinished	 = true;
			}
		}
		break;
	}
}



/*
* ����: ��Ⱦ�б��еĶ���
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
* �޸���־:
*	2009.01.07 - lpf
*		����FontInfo�������������ݵ���ʾ
*	2009.01.09 - lpf
*		�ھ��ο���ʱ,���ǵ���ʧ�ܵ����.�Դ����豸����ʱ�˴����ܿ���ʧ�ܵ����
*/
void CRollLantern::Draw()
{
	if (m_bIsFinished)
		return;

	LPDIRECT3DSURFACE9 pRenderTarget = NULL;
	LPDIRECT3DDEVICE9  pD3DDevice	 = render::Interface::GetInstance()->GetDevice();

	DWORD dwAlpha = ((DWORD)(m_fAlphaPer * 255.0f)) << 24;
	DWORD dwColor = 0xffffffff;

	// ���Ʊ���ͼƬ
	if (m_pBackGroundPic != NULL)
		m_pBackGroundPic->Draw(m_ptPos.x, m_ptPos.y, (dwAlpha | 0x00ffffff));

	//render::Interface::GetInstance()->EndScene();
	pD3DDevice->GetRenderTarget(0, &pRenderTarget);

	if (pD3DDevice->StretchRect(pRenderTarget, &m_rcSourceLeft, m_pOffscreenLeft, NULL, D3DTEXF_NONE) != D3D_OK)
		CreateOffscreenLeft();
	if (pD3DDevice->StretchRect(pRenderTarget, &m_rcSourceRight, m_pOffscreenRight, NULL, D3DTEXF_NONE) != D3D_OK)
		CreateOffscreenRight();

	// �����ַ�������
	POINT lDrawPos = { m_lContentOffset + m_lContentOffsetRight, m_ptPos.y + m_lContentOffsetHeight + 1 };
	render::Interface::GetInstance()->GetLayer2D()->DrawText(lDrawPos.x, lDrawPos.y, m_sContent.c_str(), &m_stFontInfo);

	if (pD3DDevice->StretchRect(m_pOffscreenLeft, NULL, pRenderTarget, &m_rcSourceLeft, D3DTEXF_NONE) != D3D_OK)
		CreateOffscreenLeft();
	if (pD3DDevice->StretchRect(m_pOffscreenRight, NULL, pRenderTarget, &m_rcSourceRight, D3DTEXF_NONE) != D3D_OK)
		CreateOffscreenRight();

	SAFERELEASE(pRenderTarget);
}



/*
* ����: ���²ü�����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.05
* �޸���־:
*	2009.01.09 - lpf
*		�����˹������Ĳü�����,����Ӧ18���ص�����(���������������ʾ)
*/
void CRollLantern::UpdateClipRect()
{
	m_rcSourceLeft.left   = 0;
	m_rcSourceLeft.top	  = m_ptPos.y + m_lContentOffsetHeight;
	m_rcSourceLeft.right  = m_ptPos.x + m_lClipLeft;
	m_rcSourceLeft.bottom = m_rcSourceLeft.top + 18;

	m_rcSourceRight.left   = m_ptPos.x + m_lClipRight;
	m_rcSourceRight.top	   = m_rcSourceLeft.top;
	m_rcSourceRight.right  = render::Interface::GetInstance()->GetSetting()->iWndWidth;
	m_rcSourceRight.bottom = m_rcSourceLeft.bottom;

	CreateOffscreenLeft();
	CreateOffscreenRight();
}



/*
* ����: ��������������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.09
* �޸���־:
*/
bool CRollLantern::CreateOffscreenLeft()
{
	SAFERELEASE(m_pOffscreenLeft);

	if (render::Interface::GetInstance()->GetDevice()->CreateRenderTarget(m_rcSourceLeft.right - m_rcSourceLeft.left,
		18,
		render::Interface::GetInstance()->GetSetting()->d3dfmtBackBuffer,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pOffscreenLeft,
		NULL) != D3D_OK)
		return false;
	else
		return true;
}



/*
* ����: �����ҷ���������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.01.09
* �޸���־:
*/
bool CRollLantern::CreateOffscreenRight()
{
	SAFERELEASE(m_pOffscreenRight);

	if (render::Interface::GetInstance()->GetDevice()->CreateRenderTarget(m_rcSourceRight.right - m_rcSourceRight.left,
		18,
		render::Interface::GetInstance()->GetSetting()->d3dfmtBackBuffer,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pOffscreenRight,
		NULL) != D3D_OK)
		return false;
	else
		return true;
}