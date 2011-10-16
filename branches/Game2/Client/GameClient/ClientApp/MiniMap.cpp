#include "stdafx.h"

#include "minimap.h"
#include "../../EngineExtend/GameGraphics2D.h"
#include "../../EngineExtend/CBitmapX.h"
#include "../../Engine\utility.h"
#include "Other/RegionList.h"
#include "Other/PicList.h"
#include "Other/CountryList.h"
#include "../GameClient/Game.h"
#include "ClientRegion.h"
#include "../../EngineExtend/GameMap.h"
#include "Player.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int angle[] = {135,90,45,0,315,270,225,180};
static const int angleNoRotation[] = {90,45,0,315,270,225,180,135};

CMiniMap::CMiniMap(void)
{
	m_pMiniMap = NULL;
	m_iMapMode = 1;

	m_pRenderTexture128 = NULL;
	m_pDepthStencilSurface128 = NULL;
}

CMiniMap::~CMiniMap(void)
{
	Destroy();
}

//---------------------------------------------------------------------------------
// Function name   : CMiniMap::Create
// Description     : 
// Return type     : void 
// Argument        : DWORD dwFrameWidth		: С��ͼ���ڿ��
// Argument        : DWORD dwFrameHeight	: С��ͼ���ڸ߶�
// Argument        : char *filename			: icon�ļ��б�
//---------------------------------------------------------------------------------
void CMiniMap::Create(DWORD dwFrameWidth, DWORD dwFrameHeight, CONST TCHAR *filename)
{
	m_fScaleFactor = 1;
	m_dwColor = 0xffffffff;

	m_dwFrameWidth = dwFrameWidth;
	m_dwFrameHeight = dwFrameHeight;

	ZeroMemory(m_pShapeInfo, sizeof(_tagShapeInfo) * MAX_NUM);
	m_dwNumOfShape = 0;
	m_vecPartner.clear();
	m_vecRedName.clear();
	m_vecPirates.clear();
	m_vecOther.clear();
	m_vecNPC.clear();
	m_vecUnLeagPlayer.clear();
	m_vecLeagPlayer.clear();

	// ����ͼ��
	utility::File file;
	file.Open(filename);

	utility::Text script;
	script.Create(&file);

	utility::Text::Node *pNode = script.GetRootNode();
	utility::Text::Variable *pVar;

	pVar = pNode->GetVar("PlayerIcon");				// ���ͼ��
	m_PlayerIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("MonsterIcon");			// ����ͼ��
	m_MonsterIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("LeagueNpcIcon");			// ����NPCͼ��
	m_LeagueNpcIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("UnLeagueNpcIcon");		// ������NPCͼ��
	m_UnLeagueNpcIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("PartnerIcon");			// ����ͼ��
	m_PartnerIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("LeaguePlayerIcon");		// �����������ͼ��
	m_LeaguePlayerIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("UnLeaguePlayerIcon");		// �������������ͼ��
	m_UnLeaguePlayerIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("RedIcon");				// ����ͼ��
	m_RedIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("PiratesIcon");			// ����ͼ��
	m_PiratesIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("TaskIcon");				// ���ͼ��
	m_TaskIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("PetIcon");				// ����ͼ��
	m_PetIcon = CBitmapX::GetBitmapX(pVar->GetStringValue());

	pVar = pNode->GetVar("AlphaMap");
	m_pAlphaMap = CBitmapX::GetBitmapX(pVar->GetStringValue());

	script.Destroy();
	file.Close();

	HRESULT				hResult	   = NULL;
	render::Interface * pInterface = render::Interface::GetInstance();
	hResult = pInterface->GetDevice()->CreateTexture(128,
		128,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pRenderTexture128,
		NULL);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: CreateTexture(...) failed!.\n");
		return;
	}

	hResult = pInterface->GetDevice()->CreateDepthStencilSurface(128,
		128, 
		pInterface->GetInfo()->d3dpp.AutoDepthStencilFormat, 
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDepthStencilSurface128,
		NULL);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: CreateDepthStencilSurface(...) failed!.\n");
		return;
	}
}

void CMiniMap::Destroy()
{
	SAFE_DECREF(m_pMiniMap);
	SAFE_DECREF(m_PlayerIcon);
	SAFE_DECREF(m_MonsterIcon);
	SAFE_DECREF(m_LeagueNpcIcon);
	SAFE_DECREF(m_UnLeagueNpcIcon);
	SAFE_DECREF(m_PartnerIcon);
	SAFE_DECREF(m_LeaguePlayerIcon);
	SAFE_DECREF(m_UnLeaguePlayerIcon);
	SAFE_DECREF(m_RedIcon);
	SAFE_DECREF(m_PiratesIcon);
	SAFE_DECREF(m_TaskIcon);
	SAFE_DECREF(m_PetIcon);
	SAFE_DECREF(m_pAlphaMap);

	DestroyDirect3DResource();
}



/*
* ����: ����D3D�����Դ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.03.02
*/
void CMiniMap::DestroyDirect3DResource()
{
	SAFERELEASE(m_pRenderTexture128);
	SAFERELEASE(m_pDepthStencilSurface128);
}



/*
* ����: ����D3D�����Դ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.03.02
*/
void CMiniMap::ResetDirect3DResource()
{
	HRESULT				hResult	   = NULL;
	render::Interface * pInterface = render::Interface::GetInstance();

	hResult = pInterface->GetDevice()->CreateTexture(128,
		128,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pRenderTexture128,
		NULL);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: CreateTexture(...) failed!.\n");
		return;
	}

	hResult = pInterface->GetDevice()->CreateDepthStencilSurface(128,
		128, 
		pInterface->GetInfo()->d3dpp.AutoDepthStencilFormat, 
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDepthStencilSurface128,
		NULL);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: CreateDepthStencilSurface(...) failed!.\n");
		return;
	}
}



//---------------------------------------------------------------------------------
// Function name   : CMiniMap::LoadMiniMap
// Description     : 
// Return type     : BOOL 
// Argument        : const char *filename    : С��ͼ�ļ���
// Argument        : DWORD width             : ������ͼ���
// Argument        : DWORD height            : ������ͼ�߶�
//---------------------------------------------------------------------------------
BOOL CMiniMap::LoadMiniMap(const char *filename, DWORD width, DWORD height)
{
	if (!utility::File::FileExist(filename))
		return FALSE;
	m_pMiniMap = CBitmapX::GetBitmapX(filename);
	if (!m_pMiniMap)
		return FALSE;

	m_dwMiniMapWidth = m_pMiniMap->GetWidth();
	m_dwMiniMapHeight= m_pMiniMap->GetHeight();

	m_dwMapWidth = width;
	m_dwMapHeight = height;

	m_nDestX = -1;
	m_nDestY = -1;

	// С��ͼͼƬ��ά����Ҫ�볡����ͬ
	if(m_dwMiniMapWidth != m_dwMapWidth || m_dwMiniMapHeight != m_dwMapHeight)
	{
		//if (g_bDebug)
		//{
		//          //MessageBox(NULL, "ERROR: С��ͼ�볡����ƥ��", filename, MB_OK);
		//}
		m_iMapMode = m_dwMiniMapWidth / m_dwMapWidth;
		return FALSE;
	}
	m_iMapMode = 1;
	return TRUE;
}

//---------------------------------------------------------------------------------
// Function name   : CMiniMap::DrawMiniMap
// Description     : 
// Return type     : void 
// Argument        : DWORD x                      : С��ͼ����x ����
// Argument        : DWORD y                      : С��ͼ����y ����
// Argument        : _tagShapeInfo *pShapeInfo    : �����Ϣ
//---------------------------------------------------------------------------------
void CMiniMap::DrawMiniMap(DWORD x, DWORD y)
{
	if (!m_pMiniMap)
		return;

	m_vecPartnerForUI.clear();

	if (IsRotationAble())
	{
		if (!RenderMiniMapRotation(x, y))
		{
			OutputError("CMiniMap: RenderMiniMapRotation(...) failed!.\n");
			return;
		}
	}else
	{
		if (!RenderMiniMapNoRotation(x, y))
		{
			OutputError("CMiniMap: RenderMiniMapNoRotation(...) failed!.\n");
			return;
		}
	}
}


//---------------------------------------------------------------------------------
// Function name   : CMiniMap::DrawMiniMap
// Description     : 
// Return type     : void 
// Argument        : DWORD x                      : С��ͼ����x ����
// Argument        : DWORD y                      : С��ͼ����y ����
// Argument        : _tagShapeInfo *pShapeInfo    : �����Ϣ
//---------------------------------------------------------------------------------
//void CMiniMap::DrawMiniMapOnWorldMap(DWORD x, DWORD y,CBitmapX *pBitmap,int index)
//{
//	if (!m_pMiniMap)
//		return ;
//
//	if (IsRotationAble())
//	{
//		//RenderMiniMapNoRotationOnWorldMap(x,y,pBitmap,index);
//		RenderMiniMapRotationOnWorldMap(x,y,pBitmap,index);
//	}
//	else
//	{
//		RenderMiniMapNoRotationOnWorldMap(x,y,pBitmap,index);
//	}
//}



/*
* ����: ����Alpha�ɰ�
* ժҪ: ���ڻ��������ͼ��Alphaͨ���ڵ�Ч��
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.26
*/
void CMiniMap::DrawAlphaMap()
{
	//DWORD					  dwFogEnable;
	render::Interface		* pInterface = render::Interface::GetInstance();
	LPDIRECT3DDEVICE9		  pD3DDevice = pInterface->GetDevice();
	LPDIRECT3DVERTEXBUFFER9	  lp2DVB	 = pInterface->GetLayer2D()->GetVertexBuffer();		//���㻺�����ĵĽӿ�ָ��


	pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// FILL Solid Mode
	pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// Setup sampler Texture Filter
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	// Setup Alpha
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);

	// Setup Color OP
	pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// Setup Alpha OP
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// ����������Ϣ
	tagD3DFVF_2D * pVertices;	
	lp2DVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	pVertices[0].pos.x = pVertices[2].pos.x = 0.0f;
	pVertices[1].pos.x = pVertices[3].pos.x = 128.0f;

	pVertices[0].pos.y = pVertices[1].pos.y = 0.0f;
	pVertices[2].pos.y = pVertices[3].pos.y = 128.0f;

	pVertices[0].pos.z = 
		pVertices[0].rhw = 
		pVertices[1].pos.z = 
		pVertices[1].rhw = 
		pVertices[2].pos.z = 
		pVertices[2].rhw = 
		pVertices[3].pos.z = 
		pVertices[3].rhw = 1.0f;

	pVertices[0].color =
		pVertices[1].color =
		pVertices[2].color =
		pVertices[3].color = -1;

	pVertices[0].uv = D3DXVECTOR2(0.0f,0.0f);	
	pVertices[1].uv = D3DXVECTOR2(1.0f,0.0f);	
	pVertices[2].uv = D3DXVECTOR2(0.0f,1.0f);	
	pVertices[3].uv = D3DXVECTOR2(1.0f,1.0f);

	lp2DVB->Unlock();

	pInterface->SetTexture(0, m_pAlphaMap->GetFirstTextureHandle());

	//bind stream source
	pD3DDevice->SetStreamSource(0, lp2DVB, 0, sizeof(tagD3DFVF_2D));
	pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	if (FAILED(pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2)))
	{
		MessageBox(NULL,"DPС��ͼ�ɰ�ʧ��","����",MB_OK);
	}

	pD3DDevice->SetTexture(0, NULL);
	if (GetGame()->GetRegion()->GetGameMap())
	{
		pD3DDevice->SetRenderState(D3DRS_FOGENABLE, GetGame()->GetRegion()->GetGameMap()->GetFogEnable());
	}
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}



/*
* ����: �������յ������ͼ����
* ժҪ: ���յ������ͼ����������������б����Ƴ���
* ����: dwX - ����λ��X����
*		 dwY - ����λ��Y����
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.26
*/
void CMiniMap::DrawFinalMiniMap(DWORD dwX, DWORD dwY)
{
	//DWORD					  dwFogEnable;
	render::Interface		* pInterface = render::Interface::GetInstance();
	LPDIRECT3DDEVICE9		  pD3DDevice = pInterface->GetDevice();
	LPDIRECT3DVERTEXBUFFER9	  lp2DVB	 = pInterface->GetLayer2D()->GetVertexBuffer();		//���㻺�����ĵĽӿ�ָ��

	pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// FILL Solid Mode
	pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// Setup sampler Texture Filter
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	// Setup Alpha
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Setup Color OP
	pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	//Setup Alpha OP
	pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// ����������Ϣ
	tagD3DFVF_2D * pVertices;	
	lp2DVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	pVertices[0].pos.x = pVertices[2].pos.x = (float)dwX;
	pVertices[1].pos.x = pVertices[3].pos.x = (float)(dwX + 128);

	pVertices[0].pos.y = pVertices[1].pos.y = (float)dwY;
	pVertices[2].pos.y = pVertices[3].pos.y = (float)(dwY + 128);

	pVertices[0].pos.z = 
		pVertices[0].rhw = 
		pVertices[1].pos.z = 
		pVertices[1].rhw = 
		pVertices[2].pos.z = 
		pVertices[2].rhw = 
		pVertices[3].pos.z = 
		pVertices[3].rhw = 1.0f;

	pVertices[0].color =
		pVertices[1].color =
		pVertices[2].color =
		pVertices[3].color = -1;

	pVertices[0].uv = D3DXVECTOR2(0.0f,0.0f);	
	pVertices[1].uv = D3DXVECTOR2(1.0f,0.0f);	
	pVertices[2].uv = D3DXVECTOR2(0.0f,1.0f);	
	pVertices[3].uv = D3DXVECTOR2(1.0f,1.0f);

	lp2DVB->Unlock();

	pD3DDevice->SetTexture(0, m_pRenderTexture128);

	//bind stream source
	pD3DDevice->SetStreamSource(0, lp2DVB,0, sizeof(tagD3DFVF_2D));
	pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	if (FAILED(pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2)))
	{
		MessageBox(NULL,"DPС��ͼʧ��","����",MB_OK);
	}
	pD3DDevice->SetTexture(0, NULL);
	if (GetGame()->GetRegion()->GetGameMap())
	{
		pD3DDevice->SetRenderState(D3DRS_FOGENABLE, GetGame()->GetRegion()->GetGameMap()->GetFogEnable());
	}
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}



// �Ƿ�֧����ת��ʾ
bool CMiniMap::IsRotationAble()
{
	if (!m_pMiniMap)
		return false;

	render::Interface *pInterface = render::Interface::GetInstance();
	if ((pInterface->GetInfo()->d3dCaps.DevCaps2 & D3DDEVCAPS2_CAN_STRETCHRECT_FROM_TEXTURES))
	{
		return true;
	}
	return false;
}

//void CMiniMap::RenderMiniMapRotationOnWorldMap(DWORD x,DWORD y,CBitmapX *pBitmap,int index)
//{
//	int width=int(m_dwMapWidth/1.414f), height=int(m_dwMapHeight/1.414f);
//	x-=width/2;
//	y-=height/2;
//	render::Interface *pInterface = render::Interface::GetInstance();
//
//	//����RenderTarget-------------------------------------------------
//	pInterface->GetDevice()->EndScene();
//
//	IDirect3DSurface9 *pRenderTarget, *pDepthStencilSurface;
//	HRESULT hResult;
//	bool bToDraw = true; //һ���д����Ȳ���ʧ�ܾ�Ϊfalse,������֮�����Ⱦ��(�Ͷ��Կ����׳���)
//
//	IDirect3DSurface9* 	pOldRenderTarget;
//	IDirect3DSurface9* 	pOldZStencil;
//
//	hResult = pInterface->GetDevice()->GetRenderTarget(0, &pOldRenderTarget);
//	if ( FAILED(hResult))
//	{
//		bToDraw = false;
//		OutputError("CMiniMap: GetRenderTarget(...) failed!.\n");
//		ASSERT(0);
//	}
//
//	hResult = pInterface->GetDevice()->GetDepthStencilSurface(&pOldZStencil);
//	if ( FAILED(hResult))
//	{
//		bToDraw = false;
//		OutputError("CMiniMap: GetDepthStencilSurface(...) failed!.\n");
//		ASSERT(0);
//	}
//	D3DFORMAT dwRenderTargetFormat = pInterface->GetInfo()->d3dpp.BackBufferFormat;
//
//	hResult = pInterface->GetDevice()->CreateRenderTarget( 512,
//														   512,
//														   dwRenderTargetFormat, 
//														   D3DMULTISAMPLE_NONE, 
//														   0, 
//														   TRUE, 
//														   &pRenderTarget, 
//														   NULL);
//	if ( FAILED(hResult))
//	{
//		bToDraw = false;
//		OutputError("CMiniMap: CreateRenderTarget(...) failed!.\n");
//		ASSERT(0);
//	}
//
//	D3DFORMAT dwDepthStencilSurface = pInterface->GetInfo()->d3dpp.AutoDepthStencilFormat;
//	hResult = pInterface->GetDevice()->CreateDepthStencilSurface( 512,
//																  512,
//																  dwDepthStencilSurface, 
//																  D3DMULTISAMPLE_NONE,
//																  0,
//																  TRUE,
//																  &pDepthStencilSurface,
//																  NULL);
//
//	if ( FAILED(hResult))
//	{
//		bToDraw = false;
//		OutputError("CMiniMap: CreateDepthStencilSurface(...) failed!.\n");
//		ASSERT(0);
//	}
//
//	hResult = pInterface->GetDevice()->SetRenderTarget(0, pRenderTarget);
//	if ( FAILED(hResult))
//	{
//		bToDraw = false;
//		OutputError("CMiniMap: SetRenderTarget(...) failed!.\n");
//		ASSERT(0);
//	}
//
//	hResult = pInterface->GetDevice()->SetDepthStencilSurface(pDepthStencilSurface);
//	if ( FAILED(hResult))
//	{
//		bToDraw = false;
//		OutputError("CMiniMap: SetDepthStencilSurface(...) failed!.\n");
//		ASSERT(0);
//	}
//	
//
//
//	if (bToDraw)
//	{
//		//��Ⱦ
//		pInterface->GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
//			D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
//
//		GameGraphics2D *pGameGraphics2D = GameGraphics2D::GetInstance();
//
//		m_pMiniMap->DrawRotation2(0,height/2,0,0,D3DXToRadian(-45), .5f);
//
//		RECT rcMinMap = { 0,0,width,height};
//
//		//��ʾ������Ϣ
//
//		//��ת45��
//		static D3DXMATRIX mRot;
//		D3DXMatrixRotationZ(&mRot,D3DXToRadian(-45));
//		static D3DXVECTOR2 v2;
//		int mx , my;
//		_tagShapeInfo *shapeInfo;
//		for(DWORD i = 0; i < m_dwNumOfShape; i++)
//		{
//			// ��������
//			shapeInfo = &m_pShapeInfo[i];
//			int iX=shapeInfo->x,iY=shapeInfo->y;
//			v2.x=(float)iX;
//			v2.y=(float)iY;
//			D3DXVec2TransformCoord(&v2,&v2,&mRot);
//			mx=(int)v2.x/2;
//			my=int(v2.y/2+height/2);
//
//			switch(shapeInfo->type)
//			{
//			case MT_PARTNER:		// ����
//				{
//					if (InTheRect(&rcMinMap,mx,my))
//					{
//						// �ڷ�Χ�ھ�ֱ����ʾ
//						m_PartnerIcon->DrawRotation(mx,my,0);
//					}
//				}
//				break;
//			case MT_LEAGPLAYER:		// �������
//				{
//					if (InTheRect(&rcMinMap,mx,my))
//					{
//						// �ڷ�Χ�ھ�ֱ����ʾ
//						m_LeaguePlayerIcon->DrawRotation(mx,my,0);
//					}
//				}
//				break;
//			case MT_UNLEAGPLAYER:		// ���������
//				{
//					if (InTheRect(&rcMinMap,mx,my))
//					{
//						m_UnLeaguePlayerIcon->DrawRotation(mx,my,0);
//					}
//				}
//				break;
//			case MT_TASK:
//				{
//					if (InTheRect(&rcMinMap,mx,my))
//					{
//						m_TaskIcon->DrawRotation(mx,my,0);
//					}
//				}
//				break;
//			}
//		}
//
//		// �����������
//		int iPlayerX=m_iMainPlayerX,iPlayerY=m_iMainPlayerY;
//		v2.x=(float)iPlayerX;
//		v2.y=(float)iPlayerY;
//		D3DXVec2TransformCoord(&v2,&v2,&mRot);
//		iPlayerX=int(v2.x/2);
//		iPlayerY=int(v2.y/2+height/2);
//
//		// ��ʾ���λ��	
//		if(m_iMainPlayerDir>=0 && m_iMainPlayerDir<=7)
//			m_PlayerIcon->DrawRotation(iPlayerX,iPlayerY,-D3DXToRadian(angle[m_iMainPlayerDir]));
//
//		// ��ʾ�⻷
//		if(pBitmap&&index>=0)
//		{
//			static RECT rect;
//			static int width=pBitmap->GetWidth()/4,height=pBitmap->GetHeight()/4;
//			rect.right=width;
//			rect.bottom=height;
//			rect.left=(16-index)%4*width;
//			rect.top=(16-index)/4*height;
//			//pBitmap->Draw(width/2-64,height/2-64,rect);
//			pBitmap->Draw(iPlayerX-width/2,iPlayerY-height/2,rect);
//		}
//
//		//�ָ�RenderTarget------------------------------------------------------
//		hResult = pInterface->GetDevice()->SetRenderTarget(0, pOldRenderTarget);
//		if ( FAILED(hResult))
//		{
//			OutputError("CMiniMap: SetRenderTarget(Old) failed!.\n");
//			ASSERT(0);
//		}
//
//		hResult = pInterface->GetDevice()->SetDepthStencilSurface(pOldZStencil);
//		if ( FAILED(hResult))
//		{
//			OutputError("CMiniMap: SetDepthStencilSurface(Old) failed!.\n");
//			ASSERT(0);
//		}
//
//		pInterface->GetDevice()->BeginScene();
//
//		//��RenderTarget���Ƶ���������
//		D3DSURFACE_DESC dsds;
//		hResult = pRenderTarget->GetDesc(&dsds);
//		if ( FAILED(hResult))
//		{
//			ASSERT(0);
//		}                             	
//
//		D3DSURFACE_DESC dsds1;
//		hResult = pOldRenderTarget->GetDesc(&dsds1);
//
//		//rc & screen
//		RECT rcDest = {x,y,x + width,y+height};
//
//		if (!(rcDest.left >= (int)dsds1.Width || rcDest.top >= (int)dsds1.Height || rcDest.right < 0 || rcDest.bottom < 0)) 
//		{
//			if (rcDest.left < 0) rcDest.left = 0;
//			if (rcDest.top  < 0) rcDest.top  = 0;
//			if (rcDest.right >=  (int)dsds1.Width) rcDest.right = (int)dsds1.Width;
//			if (rcDest.bottom >= (int)dsds1.Height) rcDest.bottom = (int)dsds1.Height;
//
//			int iClipX = rcDest.left - x;
//			int iClipY = rcDest.top - y;
//			int iClipWidth  = rcDest.right  - rcDest.left;
//			int iClipHeight = rcDest.bottom - rcDest.top;
//
//			RECT rcSrc = { iClipX,iClipY,iClipX + iClipWidth,iClipY + iClipHeight};
//
//			hResult = pInterface->GetDevice()->StretchRect(pRenderTarget,&rcSrc,pOldRenderTarget,&rcDest,D3DTEXF_POINT);
//
//			if ( FAILED(hResult))
//			{
//				OutputError("CMiniMap: StretchRect() failed!.\n");
//				ASSERT(0);
//			}
//		}
//	}
//
//	//�ͷű���
//	SAFERELEASE(pOldRenderTarget);
//	SAFERELEASE(pOldZStencil);
//
//	SAFERELEASE(pRenderTarget);
//	SAFERELEASE(pDepthStencilSurface);
//	
//}


//void CMiniMap::RenderMiniMapNoRotationOnWorldMap(DWORD x,DWORD y,CBitmapX *pBitmap,int index)
//{
//	GameGraphics2D *pGameGraphics2D = GameGraphics2D::GetInstance();
//
//	m_dwFrameWidth = m_dwMapWidth / 2;
//	m_dwFrameHeight = m_dwMapHeight / 2;
//	x-=m_dwFrameWidth/2;
//	y-=m_dwFrameHeight/2;
//
//	// С��ͼ�볡����ͼ�ı���
//	FLOAT fScaleX, fScaleY;
//	fScaleX = m_dwMiniMapWidth / (FLOAT)m_dwMapWidth;
//	fScaleY = m_dwMiniMapHeight / (FLOAT)m_dwMapHeight;
//
//	// ������Ҫ��ʾ��С��ͼ��Χ
//	DWORD halfWidth, halfHeight;
//	halfWidth = (DWORD)(m_dwFrameWidth * 0.5f * m_fScaleFactor);    // ������������
//	halfHeight = (DWORD)(m_dwFrameHeight * 0.5f * m_fScaleFactor);
//
//	DWORD offsetX, offsetY;
//	RECT rc;
//	rc.left = (DWORD)(m_iMainPlayerX * fScaleX) - halfWidth;
//	if(rc.left < 0)
//		rc.left = 0;
//
//	rc.right = rc.left + (DWORD)(m_dwFrameWidth * m_fScaleFactor);
//	if((DWORD)rc.right > m_dwMiniMapWidth)
//	{
//		offsetX = rc.right - m_dwMiniMapWidth;
//		rc.right -= offsetX;
//		rc.left -= offsetX;
//	}
//
//	rc.top = (DWORD)(m_iMainPlayerY * fScaleY) - halfHeight;
//	if(rc.top < 0)
//		rc.top = 0;
//
//	rc.bottom = rc.top + (DWORD)(m_dwFrameHeight * m_fScaleFactor);
//	if((DWORD)rc.bottom > m_dwMiniMapHeight)
//	{
//		offsetY = rc.bottom - m_dwMiniMapHeight;
//		rc.bottom -= offsetY;
//		rc.top -= offsetY;
//	}
//
//	rc.right = rc.right - rc.left;
//	rc.bottom = rc.bottom - rc.top;
//	m_pMiniMap->Draw(x, y, (FLOAT)m_dwFrameWidth, (FLOAT)m_dwFrameHeight, rc, m_dwColor);
//
//	// ��ʾ���λ�á������λ��ת����С��ͼ�����ת��
//	FLOAT fTemp = 1.0f / m_fScaleFactor;
//	// ��ʾ����shape
//	rc.right += rc.left;
//	rc.bottom += rc.top;
//
//	_tagShapeInfo *shapeInfo;
//	for(DWORD i = 0; i < m_dwNumOfShape; i++)
//	{
//		shapeInfo = &m_pShapeInfo[i];
//		switch(shapeInfo->type)
//		{
//		case MT_PARTNER:		// ����
//			{	
//				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
//				{	// �ڷ�Χ�ھ�ֱ����ʾ
//					m_PartnerIcon->DrawRotation((DWORD)(x + (shapeInfo->x * fScaleX - rc.left) * fTemp), 
//						(DWORD)(y + (shapeInfo->y * fScaleY - rc.top) * fTemp),0); 
//				}
//			}
//			break;
//		case MT_LEAGPLAYER:		// �������
//			{
//				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
//				{	
//					m_LeaguePlayerIcon->DrawRotation((DWORD)(x + (shapeInfo->x * fScaleX - rc.left) * fTemp), 
//						(DWORD)(y + (shapeInfo->y * fScaleY - rc.top) * fTemp),0); 
//				}
//			}
//			break;
//		case MT_UNLEAGPLAYER:	// ���������
//			{
//				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
//				{
//					m_UnLeaguePlayerIcon->DrawRotation((DWORD)(x + (shapeInfo->x * fScaleX - rc.left) * fTemp), 
//						(DWORD)(y + (shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
//				}
//			}
//			break;
//
//		case MT_TASK:
//			{
//				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
//				{
//					m_TaskIcon->DrawRotation((DWORD)(x + (shapeInfo->x * fScaleX - rc.left) * fTemp), 
//						(DWORD)(y + (shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
//				}
//			}
//			break;
//		}
//	}
//
//	// ��ʾ���λ�á������λ��ת����С��ͼ�����ת��
//	if(m_iMainPlayerDir>=0 && m_iMainPlayerDir<=7)
//	m_PlayerIcon->DrawRotation((DWORD)(x + (m_iMainPlayerX * fScaleX - rc.left) * fTemp), 
//		(DWORD)(y + (m_iMainPlayerY * fScaleY - rc.top) * fTemp), -D3DXToRadian(angleNoRotation[m_iMainPlayerDir]));
//
//	// ��ʾ�⻷
//	if(pBitmap&&index>=0)
//	{
//		static RECT rect;
//		static int width=pBitmap->GetWidth()/4,height=pBitmap->GetHeight()/4;
//		rect.right=width;
//		rect.bottom=height;
//		rect.left=(16-index)%4*width;
//		rect.top=(16-index)/4*height;
//		//pBitmap->Draw(width/2-64,height/2-64,rect);
//		pBitmap->Draw((DWORD)(x + (m_iMainPlayerX * fScaleX - rc.left) * fTemp)-width/2,(DWORD)(y + (m_iMainPlayerY * fScaleY - rc.top) * fTemp)-height/2,rect);
//	}
//
//	m_dwFrameWidth=m_dwFrameHeight=128;
//}



/*
* ����: ������תЧ���������ͼ
* ժҪ: -
* ����: x - ����λ��X����
*		 y - ����λ��Y����
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.01.26 - lpf
*		�����˸������õ�Alpha�ɰ�ͼ�������ڵ�Ч���������ͼ����
*	2008.10.17 - lpf
*		�����˾ֲ����������ʼֵ;
*	2009.03.02 - lpf
*		�Ż��������ͼ�Ļ���,������Ⱦ�����н����������ȱ���Ĵ���;
*		����������ִ�н������ֵ;
*/
bool CMiniMap::RenderMiniMapRotation(DWORD x, DWORD y)
{
	HRESULT			   hResult = NULL;
	LPDIRECT3DSURFACE9 pRenderTarget = NULL;
	LPDIRECT3DSURFACE9 pOldRenderTarget = NULL;
	LPDIRECT3DSURFACE9 pOldZStencil = NULL;

	GameGraphics2D	  * pGameGraphics2D = GameGraphics2D::GetInstance();
	render::Interface * pInterface		= render::Interface::GetInstance();
	if (pInterface->GetDeviceLost())
	{
		return false;
	}
	//pInterface->GetDevice()->EndScene();
	//pInterface->GetDevice()->BeginScene();

	// ����RenderTarget
	hResult = pInterface->GetDevice()->GetRenderTarget(0, &pOldRenderTarget);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: GetRenderTarget(...) failed!.\n");
		return false;
	}

	hResult = pInterface->GetDevice()->GetDepthStencilSurface(&pOldZStencil);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: GetDepthStencilSurface(...) failed!.\n");
		return false;
	}

	m_pRenderTexture128->GetSurfaceLevel(0, &pRenderTarget);

	// �趨�µ���ȾĿ��
	hResult = pInterface->GetDevice()->SetRenderTarget(0, pRenderTarget);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetRenderTarget(...) failed!.\n");
		return false;
	}
	hResult = pInterface->GetDevice()->SetDepthStencilSurface(m_pDepthStencilSurface128);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetDepthStencilSurface(...) failed!.\n");
		return false;
	}

	// ��Ⱦ
	pInterface->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	m_iMainPlayerX *= m_iMapMode;
	m_iMainPlayerY *= m_iMapMode;
	m_pMiniMap->DrawRotation2(64, 64, -m_iMainPlayerX, -m_iMainPlayerY, D3DXToRadian(-45), m_fScaleFactor);

	// ��ʾ������Ϣ
	RECT rcMinMap = { 0, 0, 128, 128 };

	// ��ת45��
	static D3DXMATRIX  mRot;
	static D3DXVECTOR2 v2;

	float				mx, my;
	_tagShapeInfo * shapeInfo;
	D3DXMatrixRotationZ(&mRot, D3DXToRadian(-45.0f));

	// ��ν������ͼ��
	_tagShapeInfo stShapeInfo;
	vector<_tagShapeInfo> vecQuestInfo;

	// ����
	for (DWORD i = 0; i<m_vecPartner.size(); ++i)
	{
		shapeInfo = &m_vecPartner[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + (float)shapeInfo->x;
		v2.y = -m_iMainPlayerY + (float)shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		int iDir = 0;
		// ����
		if(shapeInfo->type == MT_PARTNER)
		{
			// �ڷ�Χ�ھ�ֱ����ʾ
			if(InTheCircinal(64,64,RADIUS_NUM,mx,my,iDir))
			{
				m_PartnerIcon->DrawRotation(mx,my,0);
			}
			else
			{
				m_PlayerIcon->DrawRotation(mx,my,-D3DXToRadian(angle[iDir]));
			}
			AddToPartnerList(mx,my,shapeInfo->strName.c_str());
		}
		// ����
		else if(shapeInfo->type == MT_PET)
		{
			if (InTheRect(&rcMinMap,(int)mx,(int)my))
			{
				m_PetIcon->DrawRotation(mx,my,0);
			}
		}
	}
	// ����
	for (DWORD i = 0; i<m_vecRedName.size(); ++i)
	{
		shapeInfo = &m_vecRedName[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + (float)shapeInfo->x;
		v2.y = -m_iMainPlayerY + (float)shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		if (InTheRect(&rcMinMap,(int)mx,(int)my))
		{
			m_RedIcon->DrawRotation(mx,my,0);
		}
	}
	// ����
	for (DWORD i = 0; i<m_vecPirates.size(); ++i)
	{
		shapeInfo = &m_vecPirates[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + (float)shapeInfo->x;
		v2.y = -m_iMainPlayerY + (float)shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		if (InTheRect(&rcMinMap,(int)mx,(int)my))
		{
			m_PiratesIcon->DrawRotation(mx,my,0);
		}
	}
	// ����
	for (DWORD i = 0; i<m_vecOther.size(); ++i)
	{
		shapeInfo = &m_vecOther[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + (float)shapeInfo->x;
		v2.y = -m_iMainPlayerY + (float)shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		if (InTheRect(&rcMinMap,(int)mx,(int)my))
		{
			// ��Ч���
			if(shapeInfo->type == MT_TASK)
			{
				m_TaskIcon->DrawRotation(mx,my,0);
			}
			// ����ǲɼ���
			else if (shapeInfo->type >= MT_COLLECTION && shapeInfo->type < MT_QUEST)
			{
				CPlayer* pPlayer = GetGame()->GetMainPlayer();
				// ���ɼ���ı�ŷ������ﵱǰ�ĸ�ְҵID��ʱ����ʾ����ɼ���
				if (pPlayer!=NULL&&pPlayer->GetAssOccu()==(shapeInfo->type-MT_COLLECTION))
				{
					if(m_fScaleFactor >= 1.f)
						GetGame()->GetPicList()->Display(CPicList::PT_MINIMAP_ICON, shapeInfo->type, (int)(mx-9),(int)(my-9));
					else
						GetGame()->GetPicList()->Display(CPicList::PT_MINIMAP_ICON, shapeInfo->type, (int)(mx-4),(int)(my-4),16,16);
				}
			}
			// ��ν������
			else
			{
				stShapeInfo.type = shapeInfo->type;
				stShapeInfo.x = mx;
				stShapeInfo.y = my;
				vecQuestInfo.push_back(stShapeInfo);
			}
		}
	}
	// NPC
	for (DWORD i = 0; i<m_vecNPC.size(); ++i)
	{
		shapeInfo = &m_vecNPC[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + (float)shapeInfo->x;
		v2.y = -m_iMainPlayerY + (float)shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		if (InTheRect(&rcMinMap,(int)mx,(int)my))
		{
			// ����NPC
			if(shapeInfo->type == MT_LEAGNPC)
			{
				m_LeagueNpcIcon->DrawRotation(mx,my,0);
			}
			// ������NPC
			else if(shapeInfo->type == MT_UNLEAGNPC)
			{
				m_UnLeagueNpcIcon->DrawRotation(mx,my,0);
			}
			// ����
			else
				m_MonsterIcon->DrawRotation(mx,my,0);
		}
	}
	// ��������ͨ���
	for (DWORD i = 0; i<m_vecUnLeagPlayer.size(); ++i)
	{
		shapeInfo = &m_vecUnLeagPlayer[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + shapeInfo->x;
		v2.y = -m_iMainPlayerY + shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		// �ڷ�Χ�ڲ���ʾ
		if (InTheRect(&rcMinMap,(int)mx,(int)my))
		{
			m_UnLeagueNpcIcon->DrawRotation(mx,my,0);
		}
	}
	// ������ͨ���
	for (DWORD i = 0; i<m_vecLeagPlayer.size(); ++i)
	{
		shapeInfo = &m_vecLeagPlayer[i];
		shapeInfo->x *= m_iMapMode;
		shapeInfo->y *= m_iMapMode;
		v2.x = -m_iMainPlayerX + shapeInfo->x;
		v2.y = -m_iMainPlayerY + shapeInfo->y;
		D3DXVec2TransformCoord(&v2,&v2,&mRot);
		mx = (v2.x * m_fScaleFactor + 64.0f);
		my = (v2.y * m_fScaleFactor + 64.0f);
		// �ڷ�Χ�ڲ���ʾ
		if (InTheRect(&rcMinMap,(int)mx,(int)my))
		{
			m_LeagueNpcIcon->DrawRotation(mx,my,0);
		}
	}
	// ��ʾ���λ��
	if(m_iMainPlayerDir>=0 && m_iMainPlayerDir<=7)
		m_PlayerIcon->DrawRotation(64,64,-D3DXToRadian(angle[m_iMainPlayerDir]));

	// ��ʾ�ص�������Ϣ(��ת)
	CRegionList::tagRegion *pRegion = CRegionList::GetRegion(GetGame()->GetRegion()->GetResourceID());
	if (pRegion)
	{
		vector<CRegionList::tagAddressDetails>::iterator it = pRegion->vecCurAddrDetails.begin();
		for(;it!=pRegion->vecCurAddrDetails.end();it++)
		{
			string strPlaceName = it->strAddName;	
			//�ж��Ƿ���ת
			if(IsRotationAble())
			{
				static D3DXMATRIX mRot;
				D3DXMatrixRotationZ(&mRot,D3DXToRadian(-45));
				static D3DXVECTOR2 v2;
				//��ȡ�����ļ�������λ��
				float x = (float)it->stPos.x * m_iMapMode;
				float y = (float)it->stPos.y * m_iMapMode;
				//�������Լ���ɫ�ı���
				v2.x = -m_iMainPlayerX + x;
				v2.y = -m_iMainPlayerY + y;
				//����任���õ���ת��45�ȵ������꣩
				D3DXVec2TransformCoord(&v2,&v2,&mRot);
				//����
				x = (float)(v2.x * m_fScaleFactor + 64.0f);
				y = (float)(v2.y * m_fScaleFactor + 64.0f);
				//У����ʾλ�ã�������ʾ��
				x -= GetGame()->GetFontWidth(it->strAddName.c_str())/2;
				y -= (GetGame()->GetFontHeight(-1));
				//����ܰ���ȫ����ʾ�� ��ʾ����
				GetGame()->OutPutText(int(x-1),(int)y,strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
				GetGame()->OutPutText((int)x,int(y+1),strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
				GetGame()->OutPutText(int(x+1),(int)y,strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
				GetGame()->OutPutText((int)x,int(y-1),strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
				GetGame()->OutPutText((int)x,(int)y,strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,255,255,0));
			}
		}
	}

	// ���������νͼ��
	for (BYTE i = 0; i < vecQuestInfo.size(); ++i)
	{
		stShapeInfo = vecQuestInfo[i];
		if(m_fScaleFactor >= 1.f)
		{
			GetGame()->GetPicList()->Display(CPicList::PT_MINIMAP_ICON, stShapeInfo.type, (long)stShapeInfo.x-9,(long)stShapeInfo.y-9);
		}
		else
		{
			GetGame()->GetPicList()->Display(CPicList::PT_MINIMAP_ICON, stShapeInfo.type, (long)stShapeInfo.x-4,(long)stShapeInfo.y-4,16,16);
		}
	}
	//----------------------------------------

	DrawAlphaMap();

	//�ָ�RenderTarget------------------------------------------------------
	hResult = pInterface->GetDevice()->SetRenderTarget(0, pOldRenderTarget);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetRenderTarget(Old) failed!.\n");
		return false;
	}

	hResult = pInterface->GetDevice()->SetDepthStencilSurface(pOldZStencil);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetDepthStencilSurface(Old) failed!.\n");
		return false;
	}

	//pInterface->GetDevice()->BeginScene();

	DrawFinalMiniMap(x, y);

	//�ͷű���
	SAFERELEASE(pOldRenderTarget);
	SAFERELEASE(pOldZStencil);
	SAFERELEASE(pRenderTarget);

	return true;
}



/*
* ����: ����û����תЧ���������ͼ
* ժҪ: -
* ����: x - ����λ��X����
*		 y - ����λ��Y����
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.01.26 - lpf
*		�����˸������õ�Alpha�ɰ�ͼ�������ڵ�Ч���������ͼ����
*	2008.10.17 - lpf
*		�����˾ֲ����������ʼֵ;
*/
bool CMiniMap::RenderMiniMapNoRotation(DWORD x, DWORD y)
{	
	HRESULT			   hResult = NULL;
	LPDIRECT3DSURFACE9 pRenderTarget = NULL;
	LPDIRECT3DSURFACE9 pOldRenderTarget = NULL;
	LPDIRECT3DSURFACE9 pOldZStencil = NULL;

	GameGraphics2D	  * pGameGraphics2D = GameGraphics2D::GetInstance();
	render::Interface * pInterface		= render::Interface::GetInstance();
	if (pInterface->GetDeviceLost())
	{
		return false;
	}
	//pInterface->EndScene();

	// ����RenderTarget
	hResult = pInterface->GetDevice()->GetRenderTarget(0, &pOldRenderTarget);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: GetRenderTarget(...) failed!.\n");
		return false;
	}

	hResult = pInterface->GetDevice()->GetDepthStencilSurface(&pOldZStencil);
	if (FAILED(hResult))
	{
		OutputError("CMiniMap: GetDepthStencilSurface(...) failed!.\n");
		return false;
	}

	m_pRenderTexture128->GetSurfaceLevel(0, &pRenderTarget);

	//�趨�µ���ȾĿ��
	hResult = pInterface->GetDevice()->SetRenderTarget(0, pRenderTarget);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetRenderTarget(...) failed!.\n");
		return false ;
	}

	hResult = pInterface->GetDevice()->SetDepthStencilSurface(m_pDepthStencilSurface128);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetDepthStencilSurface(...) failed!.\n");
		return false ;
	}

	// ��Ⱦ
	pInterface->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	// С��ͼ�볡����ͼ�ı���
	FLOAT fScaleX, fScaleY;
	fScaleX = (FLOAT)m_dwMiniMapWidth / m_dwMapWidth;
	fScaleY = (FLOAT)m_dwMiniMapHeight / m_dwMapHeight;

	// ������Ҫ��ʾ��С��ͼ��Χ
	DWORD halfWidth, halfHeight;
	halfWidth  = (DWORD)(m_dwFrameWidth * 0.5f / m_fScaleFactor);    // ������������
	halfHeight = (DWORD)(m_dwFrameHeight * 0.5f / m_fScaleFactor);

	//	DWORD offsetX, offsetY;
	RECT rc;
	rc.left = (DWORD)(m_iMainPlayerX * fScaleX) - halfWidth;
	//if(rc.left < 0)
	//	rc.left = 0;

	rc.right = rc.left + (DWORD)(m_dwFrameWidth / m_fScaleFactor);
	//if((DWORD)rc.right > m_dwMiniMapWidth)
	//{
	//	offsetX = rc.right - m_dwMiniMapWidth;
	//	rc.right -= offsetX;
	//	rc.left -= offsetX;
	//}

	rc.top = (DWORD)(m_iMainPlayerY * fScaleY) - halfHeight;
	//if(rc.top < 0)
	//	rc.top = 0;

	rc.bottom = rc.top + (DWORD)(m_dwFrameHeight / m_fScaleFactor);
	//if((DWORD)rc.bottom > m_dwMiniMapHeight)
	//{
	//	offsetY = rc.bottom - m_dwMiniMapHeight;
	//	rc.bottom -= offsetY;
	//	rc.top -= offsetY;
	//}

	rc.right  = rc.right - rc.left;
	rc.bottom = rc.bottom - rc.top;
	m_pMiniMap->Draw(0, 0, (FLOAT)m_dwFrameWidth, (FLOAT)m_dwFrameHeight, rc, m_dwColor);
	//pGameGraphics2D->DrawImageSubRect(0, 0, &rc, m_pMiniMap->GetFirstTextureHandle());

	// ��ʾ���λ�á������λ��ת����С��ͼ�����ת��
	FLOAT fTemp = 1.0f * m_fScaleFactor;

	// ��ʾ����shape
	rc.right  += rc.left;
	rc.bottom += rc.top;

	_tagShapeInfo * shapeInfo;
	for(DWORD i = 0; i < m_dwNumOfShape; i++)
	{
		shapeInfo = &m_pShapeInfo[i];
		switch(shapeInfo->type)
		{
		case MT_PARTNER:		// ����
			{	
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{	// �ڷ�Χ�ھ�ֱ����ʾ
					m_PartnerIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp),0); 
					//shapeInfo->angle);
				}
		case MT_MONSTER:		// ����
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (int)(shapeInfo->x * fScaleX), (int)(shapeInfo->y * fScaleY)))
				{
					m_MonsterIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;
		case MT_LEAGNPC:		// ����NPC
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (int)(shapeInfo->x * fScaleX), (int)(shapeInfo->y * fScaleY)))
				{
					m_LeagueNpcIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;

		case MT_UNLEAGNPC:		// ������NPC
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (int)(shapeInfo->x * fScaleX), (int)(shapeInfo->y * fScaleY)))
				{
					m_UnLeagueNpcIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;
		case MT_LEAGPLAYER:		// �������
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{
					m_LeaguePlayerIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;
		case MT_UNLEAGPLAYER:	// ���������
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{
					m_UnLeaguePlayerIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;
		case MT_RED:	// ����
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{
					m_RedIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;
		case MT_PIRATES:	// ����
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{
					m_PiratesIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;

		case MT_PET:	// ����
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{
					m_PetIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;

		case MT_TASK:	// ���
			{
				// �ڷ�Χ�ڲ���ʾ
				if(InTheRect(&rc, (DWORD)(shapeInfo->x * fScaleX), (DWORD)(shapeInfo->y * fScaleY)))
				{
					m_TaskIcon->DrawRotation(((shapeInfo->x * fScaleX - rc.left) * fTemp), 
						((shapeInfo->y * fScaleY - rc.top) * fTemp), 0);
					//shapeInfo->angle);
				}
			}
			break;
			}
		}
	}
	// ��ʾ�ص�������Ϣ(����ת)
	//CRegionList::tagRegion *pRegion = CRegionList::GetRegion(GetGame()->GetRegion()->GetID());
	//if (pRegion)
	//{
	//	vector<CRegionList::tagAddressDetails>::iterator it = pRegion->vecCurAddrDetails.begin();
	//	for(;it!=pRegion->vecCurAddrDetails.end();it++)
	//	{
	//		string strPlaceName = it->strAddName;	
	//		float mx = x + (it->x * fScaleX - rc.left) * fTemp;
	//		float my = y + (it->y * fScaleY - rc.top) * fTemp;
	//		mx -= GetGame()->GetFontWidth(-1)*(int)it->strAddName.size()/2;
	//		my -= (GetGame()->GetFontHeight());
	//		if(InTheRect(&rc, (DWORD)(it->x * fScaleX), (DWORD)(it->y * fScaleY)))
	//		{
	//			GetGame()->TextOut(int(mx-1),(int)my,strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
	//			GetGame()->TextOut((int)mx,int(my+1),strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
	//			GetGame()->TextOut(int(mx+1),(int)my,strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
	//			GetGame()->TextOut((int)mx,int(my-1),strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,0,0,0));
	//			GetGame()->TextOut((int)mx,(int)my,strPlaceName.c_str(),(int)strPlaceName.size(),D3DCOLOR_ARGB(255,255,255,0));
	//		}
	//	}
	//}
	// ��ʾ���λ�á������λ��ת����С��ͼ��.
	if(m_iMainPlayerDir>=0 && m_iMainPlayerDir<=7)
	{
		m_PlayerIcon->DrawRotation(((m_iMainPlayerX * fScaleX - rc.left) * fTemp), 
			((m_iMainPlayerY * fScaleY - rc.top) * fTemp), -D3DXToRadian(angleNoRotation[m_iMainPlayerDir]));
	}


	DrawAlphaMap();

	//�ָ�RenderTarget------------------------------------------------------
	hResult = pInterface->GetDevice()->SetRenderTarget(0, pOldRenderTarget);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetRenderTarget(Old) failed!.\n");
		return false;
	}

	hResult = pInterface->GetDevice()->SetDepthStencilSurface(pOldZStencil);
	if ( FAILED(hResult))
	{
		OutputError("CMiniMap: SetDepthStencilSurface(Old) failed!.\n");
		return false;
	}

	//	pInterface->GetDevice()->BeginScene();

	DrawFinalMiniMap(x, y);

	//�ͷű���
	SAFERELEASE(pOldRenderTarget);
	SAFERELEASE(pOldZStencil);
	SAFERELEASE(pRenderTarget);

	return true;
}



/*
* ����: ����һ��Shape��Ϣ����ʾ������ȥ
* ժҪ: -
* ����: x - ����λ��X����
*		 y - ����λ��Y����
*		 dwType - Shape����
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.01.26 - lpf
*		ȡ�����趨Shape����ʱ��ʱ�����m_iMapMode�Ĳ���,����������ᱻ�Ƶ���ͬ�Ļ��ƺ�����ȥ���в�ͬ�Ĵ���
*/
void CMiniMap::AddToShapeList(float x, float y, DWORD dwType,const char*szName)
{
	if (m_dwNumOfShape < MAX_NUM)
	{
		m_pShapeInfo[m_dwNumOfShape].x	  = x;
		m_pShapeInfo[m_dwNumOfShape].y	  = y;
		m_pShapeInfo[m_dwNumOfShape].type = dwType;
		if(szName && dwType == MT_PARTNER)
			m_pShapeInfo[m_dwNumOfShape].strName = szName;
		++m_dwNumOfShape;
	}
	_tagShapeInfo stShapeInfo;
	stShapeInfo.x = x;
	stShapeInfo.y = y;
	stShapeInfo.type = dwType;
	// ����
	if(dwType == MT_TASK || dwType >= MT_OTHER)
		m_vecOther.push_back(stShapeInfo);
	switch(dwType)
	{
		// ����
	case MT_PARTNER:
	case MT_PET:
		stShapeInfo.strName = szName;
		m_vecPartner.push_back(stShapeInfo);
		break;
		// �������
	case MT_RED:
		m_vecRedName.push_back(stShapeInfo);
		break;
		// �������
	case MT_PIRATES:
		m_vecPirates.push_back(stShapeInfo);
		break;
		// NPC(���ˣ������ˣ�����)
	case MT_LEAGNPC:
	case MT_UNLEAGNPC:
	case MT_MONSTER:
		m_vecNPC.push_back(stShapeInfo);
		break;
		// ��������ͨ���
	case MT_UNLEAGPLAYER:
		m_vecUnLeagPlayer.push_back(stShapeInfo);
		break;
		// ������ͨ���
	case MT_LEAGPLAYER:
		m_vecLeagPlayer.push_back(stShapeInfo);
		break;
	}
}

//����һ��������Ϣ����ʾ�б���ȥ
void CMiniMap::AddToPartnerList(float x, float y,const char*szName)
{
	if(szName == NULL)
		return;
	_tagShapeInfo stInfo;
	stInfo.x = x;
	stInfo.y = y;
	stInfo.type = MT_PARTNER;
	stInfo.strName = szName;
	m_vecPartnerForUI.push_back(stInfo);
}

// ���ݴ���Ķ��������ö�����Ϣ�������С��ͼ�ϵĽ������꣩
_tagShapeInfo* CMiniMap::GetPartnerFromPos(DWORD x, DWORD y)
{
	for(size_t i = 0; i<m_vecPartnerForUI.size();++i)
	{
		if(m_vecPartnerForUI[i].x+2 >= x && m_vecPartnerForUI[i].x-2 <= x &&
			m_vecPartnerForUI[i].y+2 >= y && m_vecPartnerForUI[i].y-2 <= y)
			return &m_vecPartnerForUI[i];
	}
	return NULL;
}


void CMiniMap::CleanShapeList()
{
	ZeroMemory(m_pShapeInfo, sizeof(_tagShapeInfo) * m_dwNumOfShape);
	m_dwNumOfShape = 0;
	m_vecPartner.clear();
	m_vecRedName.clear();
	m_vecPirates.clear();
	m_vecOther.clear();
	m_vecNPC.clear();
	m_vecUnLeagPlayer.clear();
	m_vecLeagPlayer.clear();
}
DWORD CMiniMap::GetShapeListCount()
{
	return m_dwNumOfShape;
}

void CMiniMap::ScaleMiniMap(FLOAT fScaleFactor)
{
	m_fScaleFactor = fScaleFactor;
	if(m_fScaleFactor <= 0)
		m_fScaleFactor = 1.0f;
}

BOOL CMiniMap::InTheRect(RECT *rc, int x, int y)
{
	if(x >= rc->left && x <= rc->right && y >= rc->top && y <= rc->bottom)
		return TRUE;
	else
		return FALSE;
}

// (x,y) �Ƿ���Բ�С�centerX��centerYΪ���ĵ㡣�뾶Ϊ64
BOOL CMiniMap::InTheCircinal(float centerX, float centerY, float nr, float &x, float &y, int &iDir)
{	
	float iDes = (centerX-x)*(centerX-x) + (centerY-y)*(centerY-y);
	if(iDes <= nr*nr)
	{
		return TRUE;
	}
	// ������Բ�Ľ�������ͽǶ�
	float a = 0.0f,b = 0.0f,r = nr-10;
	a = ((x-centerX)*r) / sqrt(iDes);
	b = sqrt((r*r - a*a));

	x = a+nr;
	y = (y>64)?nr+b:nr-b;
	// (0-7��Ӧangle[]������)
	if(x >= 32 && x <= centerY + 32)
	{
		iDir = (y < centerY)?1:5;
	}
	else if(x > centerX+32)
	{
		iDir = 3;
		if(y < centerY-32)
			iDir = 2;
		else if(y > centerY+32)
			iDir = 4;
	}
	else
	{
		iDir = 7;
		if(y < centerY-32)
			iDir = 0;
		else if(y > centerY+32)
			iDir = 6;
	}
	return FALSE;
}

void CMiniMap::SetMainPlayer(int x,int y,int dir)
{
	m_iMainPlayerDir = dir;
	m_iMainPlayerX   = x;//*m_iMapMode;
	m_iMainPlayerY	 = y;//*m_iMapMode;
}

//-----------------------08.3.12liuke----------------------
// ����������ڳ������������
/***********************************************************************/
/*  zhaohang 2010-11-16 fix λ�ò����޸�Ϊfloat��*/
/***********************************************************************/
void CMiniMap::RenderOnWorldMap(CBitmapX *pMapBitmap,CBitmapX *pBitmap,int index,bool bShowPlayer,float x, float y)
{
	if(pMapBitmap)
	{
		int width=int(pMapBitmap->GetWidth()*1.414f), height=int(pMapBitmap->GetHeight()*1.414f);
		RECT rcMinMap = { 0,0,width,height};
		long posx = SCREEN_WIDTH/2 - (long)(pMapBitmap->GetWidth() / 1.414)+2;
		long posy = SCREEN_HEIGHT/2+4;
		//��ת45��
		static D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot,D3DXToRadian(-45));
		static D3DXVECTOR2 v2;
		float mx , my;
		_tagShapeInfo *shapeInfo;
		// ������
		for (DWORD i = 0; i<m_vecPartner.size(); ++i)
		{
			shapeInfo = &m_vecPartner[i];
			float iX=shapeInfo->x,iY=shapeInfo->y;
			v2.x=iX;
			v2.y=iY;
			D3DXVec2TransformCoord(&v2,&v2,&mRot);
			mx=(v2.x + posx);
			my=(v2.y + posy);
			int iDir = 0;
			// ����
			m_PartnerIcon->DrawRotation(mx,my,0);
			// ����
			m_PetIcon->DrawRotation(mx,my,0);
		}
		// ����
		for (DWORD i = 0; i<m_vecRedName.size(); ++i)
		{
			shapeInfo = &m_vecRedName[i];
			float iX=shapeInfo->x,iY=shapeInfo->y;
			v2.x=iX;
			v2.y=iY;
			D3DXVec2TransformCoord(&v2,&v2,&mRot);
			mx=(v2.x + posx);
			my=(v2.y + posy);
			if (InTheRect(&rcMinMap,(int)mx,(int)my))
			{
				m_RedIcon->DrawRotation(mx,my,0);
			}
		}
		// ����
		for (DWORD i = 0; i<m_vecPirates.size(); ++i)
		{
			shapeInfo = &m_vecPirates[i];
			float iX=shapeInfo->x,iY=shapeInfo->y;
			v2.x=iX;
			v2.y=iY;
			D3DXVec2TransformCoord(&v2,&v2,&mRot);
			mx=(v2.x + posx);
			my=(v2.y + posy);
			if (InTheRect(&rcMinMap,(int)mx,(int)my))
			{
				m_PiratesIcon->DrawRotation(mx,my,0);
			}
		}
		//for(DWORD i = 0; i < m_dwNumOfShape; i++)
		//{
		//	// ��������
		//	shapeInfo = &m_pShapeInfo[i];
		//	int iX=shapeInfo->x,iY=shapeInfo->y;
		//	v2.x=(float)iX;
		//	v2.y=(float)iY;
		//	D3DXVec2TransformCoord(&v2,&v2,&mRot);
		//	mx=(int)(v2.x + posx);
		//	my=int(v2.y + posy);

		//	switch(shapeInfo->type)
		//	{
		//	case MT_PARTNER:		// ����
		//		{
		//			//if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				// �ڷ�Χ�ھ�ֱ����ʾ
		//				m_PartnerIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	case MT_LEAGPLAYER:		// �������
		//		{
		//			if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				// �ڷ�Χ�ھ�ֱ����ʾ
		//				m_LeaguePlayerIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	case MT_UNLEAGPLAYER:		// ���������
		//		{
		//			if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				// �ڷ�Χ�ھ�ֱ����ʾ
		//				m_UnLeaguePlayerIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	case MT_RED:				// �������
		//		{
		//			if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				// �ڷ�Χ�ھ�ֱ����ʾ
		//				m_RedIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	case MT_PIRATES:			// �������
		//		{
		//			if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				// �ڷ�Χ�ھ�ֱ����ʾ
		//				m_PiratesIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	case MT_PET:			// ����
		//		{
		//			if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				m_PetIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	case MT_TASK:			// ���
		//		{
		//			if (InTheRect(&rcMinMap,mx,my))
		//			{
		//				m_TaskIcon->DrawRotation(mx,my,0);
		//			}
		//		}
		//		break;
		//	}
		//}
		// �����������
		float iEffectX=GetGame()->GetMainPlayer()->GetPosX(),iEffectY=GetGame()->GetMainPlayer()->GetPosY();
		if(bShowPlayer)
		{
			v2.x=iEffectX;
			v2.y=iEffectY;
			D3DXVec2TransformCoord(&v2,&v2,&mRot);
			iEffectX=(v2.x + posx);
			iEffectY=(v2.y + posy);
			// ��ʾ���λ��	
			m_PlayerIcon->DrawRotation(iEffectX,iEffectY,-D3DXToRadian(angle[GetGame()->GetMainPlayer()->GetDir()]));
		}
		else
		{
			iEffectX = x;
			iEffectY = y;
		}
		// ��ʾ�⻷
		if(pBitmap&&index>=0)
		{
			static RECT rect;
			static int width=pBitmap->GetWidth()/4,height=pBitmap->GetHeight()/4;
			rect.right=width;
			rect.bottom=height;
			rect.left=(16-index)%4*width;
			rect.top=(16-index)/4*height;
			pBitmap->Draw((int)iEffectX-width/2,(int)iEffectY-height/2,rect);
		}
		// ��ʾĿ����ʾ
		if(bShowPlayer && m_nDestX != -1 && m_nDestY != -1)
		{
			v2.x=(float)m_nDestX;
			v2.y=(float)m_nDestY;
			D3DXVec2TransformCoord(&v2,&v2,&mRot);
			iEffectX=(v2.x + posx);
			iEffectY=(v2.y + posy);
			m_TaskIcon->DrawRotation(iEffectX,iEffectY,0);
		}
	}
}


// �����ڴ��ͼ����Ҫ��ʵ��ʾ��Ŀ���
void CMiniMap::SetDestPos(int x,int y)
{
	m_nDestX = x;
	m_nDestY = y;
}
//---------------------------------------------------------