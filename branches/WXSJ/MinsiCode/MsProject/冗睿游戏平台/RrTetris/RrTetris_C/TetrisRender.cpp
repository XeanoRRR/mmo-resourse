#include "Precompiled.h"

CRender::CRender(
    HWND hWnd,
    DWORD dwScreenWidth,
    DWORD dwScreenHeight,
    CT_Scene* pScene,
    CT_Square* pSquare)
    : CMs2DRender_DX9(hWnd, dwScreenWidth, dwScreenHeight)
    , m_pTextureBack        (NULL)
	, m_pTextureMetafile_0	(NULL)
	, m_pTextureMetafile_1	(NULL)
	, m_pTextureMetafile_2	(NULL)
	, m_pTextureMetafile_3	(NULL)
    , m_pScene              (pScene)    // ��ʼ������ָ��
    , m_pSquare             (pSquare)   // ��ʼ������ָ��
    , m_lpD3DIntegralFont   (NULL)
    , m_lpD3DLevelFont      (NULL)
{
    // ��鴫��ĳ�����ָ���Ƿ�Ϸ�
    AssertEx(AL_CATAST, m_pScene, _T("m_pSceneΪ��,�������"));

    // ��鴫��ķ�����ָ���Ƿ�Ϸ�
    AssertEx(AL_CATAST, m_pSquare, _T("m_pSceneΪ��,�������"));

    m_pTextureMetafile_0 = NEW CMs2DTexture(g_pCMsWinMain->GetHInstance(), IDB_BITMAP_0);
	AssertEx(AL_CATAST, m_pTextureMetafile_0, _T("�ڴ治��"));
	m_pTextureMetafile_1 = NEW CMs2DTexture(g_pCMsWinMain->GetHInstance(), IDB_BITMAP_1);
	AssertEx(AL_CATAST, m_pTextureMetafile_1, _T("�ڴ治��"));
	m_pTextureMetafile_2 = NEW CMs2DTexture(g_pCMsWinMain->GetHInstance(), IDB_BITMAP_2);
	AssertEx(AL_CATAST, m_pTextureMetafile_2, _T("�ڴ治��"));
	m_pTextureMetafile_3 = NEW CMs2DTexture(g_pCMsWinMain->GetHInstance(), IDB_BITMAP_3);
	AssertEx(AL_CATAST, m_pTextureMetafile_3, _T("�ڴ治��"));

    //{
    //    RECT rc = {0, 0, MAX_METAFILE_WIDTH, MAX_METAFILE_HEIGHT};
    //    m_pTextureMetafile_0->SetRect(rc, rc);
    //    m_pTextureMetafile_0->UpdataVB();
    //    m_pTextureMetafile_0->UpdataRenderState(m_lpDirectDevice9);

    //    m_pTextureMetafile_1->SetRect(rc, rc);
    //    m_pTextureMetafile_1->UpdataVB();
    //    m_pTextureMetafile_1->UpdataRenderState(m_lpDirectDevice9);

    //    m_pTextureMetafile_2->SetRect(rc, rc);
    //    m_pTextureMetafile_2->UpdataVB();
    //    m_pTextureMetafile_2->UpdataRenderState(m_lpDirectDevice9);

    //    m_pTextureMetafile_3->SetRect(rc, rc);
    //    m_pTextureMetafile_3->UpdataVB();
    //    m_pTextureMetafile_3->UpdataRenderState(m_lpDirectDevice9);
    //}
    // ����ͼԪͼƬ
    TCHAR szBackImagePath[MAX_PATH] = {0};
	CreateFullPath(szBackImagePath, _T("%s\\RrTetris\\BackBitmap.bmp"));
	
    m_pTextureBack = NEW CMs2DTexture(szBackImagePath);
    AssertEx(AL_CATAST, m_pTextureBack, _T("�ڴ治��"));
    RECT rc = {0, 0, BACKSCENE_MAX_WIDTH/2, BACKSCENE_MAX_HEIGHT/2};
    m_pTextureBack->SetRect(rc, rc);
    m_pTextureBack->UpdataVB();
    m_pTextureBack->UpdataRenderState(m_lpDirectDevice9);

    // ������������
    m_lpD3DIntegralFont = CMs2DEngine::s_CreateD3D9Font(30, FW_BLACK, 1, FALSE, _T("��Բ"));

    // �����ȼ�����
    m_lpD3DLevelFont = CMs2DEngine::s_CreateD3D9Font(12, FW_BLACK, 1, FALSE, _T("��Բ"));

    FOR_x_TO(i, 5)
    {
        FOR_x_TO(j, 5)
        {
            if (i == j)
            {
                m_ShadowColorBate.m[i][j] = 0.5f;
            }
            else
            {
                m_ShadowColorBate.m[i][j] = 0.0f;
            }
        }
    }

    FOR_x_TO(i, 5)
    {
        FOR_x_TO(j, 5)
        {
            if (i == j)
            {
                m_ShadowGrayness.m[i][j] = 1.0f;
            }
            else
            {
                m_ShadowGrayness.m[i][j] = 0.0f;
            }
        }
    }
}

CRender::~CRender(VOID)
{
    SAFE_DELETE(m_pTextureBack);
	SAFE_DELETE(m_pTextureMetafile_0);
	SAFE_DELETE(m_pTextureMetafile_1);
	SAFE_DELETE(m_pTextureMetafile_2);
	SAFE_DELETE(m_pTextureMetafile_3);
}

// ����
VOID CRender::Draw(VOID)
{
    GAME_STATE gs = g_pGame->GetGameState();
    this->ResetBackScene();
    if (gs == GAME_STATE_PLAYING)
    {
        this->DrawBackScene();
		//this->DrawActivityShape(FALSE);
		//this->DrawNextShape();
		this->DrawText();
    }
    
}

// ��Ⱦ����
BOOL CRender::DrawBackScene(VOID)
{
	// ��ȡ����Ԫ�������׵�ַ
	BACK_SCENE_BYTE* pBS_BYTE = m_pScene->GetBackSceneBuff();
	if (pBS_BYTE)
	{
		DWORD dwBackSceneWidth, dwBackSceneHeight;
		m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);

		//RECT rc;
		//rc.top = DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT);
		//rc.left = DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH);
		//rc.right = DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + (dwBackSceneWidth * MAX_METAFILE_WIDTH);
		//rc.bottom = DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + (dwBackSceneHeight * MAX_METAFILE_HEIGHT);

		//m_pBuffDc->DrawEdge(&rc,BDR_RAISEDINNER,BF_TOP|BF_BOTTOM|BF_LEFT|BF_RIGHT);
		// ѭ���������еķ���
		for (UPOS Y = 0; Y < dwBackSceneHeight ; Y++)
		{
			// ѭ������ָ���е����к��ŷ���
			for (UPOS X = 0; X < dwBackSceneWidth; X++)
			{
				UPOS uMetafilePosX, uMetafilePosY;
				CMs2DTexture* pTexture = this->AccountMetafileInfo(pBS_BYTE[Y * dwBackSceneWidth + X].m_MetafileIndex, uMetafilePosX, uMetafilePosY);

				RECT DesRc = 
				{
					DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + (X * MAX_METAFILE_WIDTH),
					DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + (Y * MAX_METAFILE_HEIGHT),
					DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + (X * MAX_METAFILE_WIDTH) + MAX_METAFILE_WIDTH,
					DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + (Y * MAX_METAFILE_HEIGHT) + MAX_METAFILE_HEIGHT,
				};

				RECT SrcRc = 
				{
					uMetafilePosX,
					uMetafilePosY,
					uMetafilePosX + MAX_METAFILE_WIDTH,
					uMetafilePosY + MAX_METAFILE_HEIGHT,
				};
				if (pTexture)
				{
					pTexture->SetRect(DesRc, SrcRc);
                    pTexture->UpdataVB();
                    pTexture->RenderTexture(m_lpDirectDevice9);
				}
			}
		}
	}
	else
	{
		AssertEx(AL_CATAST, pBS_BYTE, _T("��ȡ����Ԫ�������׵�ַʧ��,����ʧ��"));
		return FALSE;
	}
	// ���سɹ�
	return TRUE;
}

// ��Ⱦǰ��
BOOL CRender::DrawActivityShape(BOOL /*IsDrawShadow*/)
{
    // ��ȡ����Ԫ���׵�ַ
    LPBYTE pBYTE = m_pSquare->GetCurrentShapeBuff();
    if (!pBYTE)
    {
        return TRUE;
    }

    DWORD dwBackSceneWidth, dwBackSceneHeight;
    m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);

    NPOS ShapePosX, ShapePosY;
    m_pSquare->GetShapePos(ShapePosX, ShapePosY);

    NPOS ShapeShadowPosX, ShapeShadowPosY;
    m_pSquare->GetShapeShadowPos(ShapeShadowPosX, ShapeShadowPosY);

    // ѭ���������еķ���
    for (UPOS BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT ; BYTE_Y++)
    {
        // ѭ������ָ���е����к��ŷ���
        for (UPOS BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
        {
            DWORD dwCurrentMetafileIndex = m_pSquare->GetCurrentMetafileIndex();
			UPOS uMetafilePosX, uMetafilePosY;
			CMs2DTexture* pTexture = this->AccountMetafileInfo(dwCurrentMetafileIndex, uMetafilePosX, uMetafilePosY);
            // ����ָ��������ͼԪ��ͼƬ�еĺ�����,������
            if (pBYTE[BYTE_Y * SHAPE_MAX_WIDTH + BYTE_X])
            {
                RECT DesRc = 
                {
                    DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + ((BYTE_X + ShapePosX) * MAX_METAFILE_WIDTH),
                    DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + ((BYTE_Y + ShapePosY) * MAX_METAFILE_HEIGHT),
                    DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + ((BYTE_X + ShapePosX) * MAX_METAFILE_WIDTH) + MAX_METAFILE_WIDTH,
                    DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + ((BYTE_Y + ShapePosY) * MAX_METAFILE_HEIGHT) + MAX_METAFILE_HEIGHT,
                };
                RECT SrcRc = 
                {
                    uMetafilePosX,
                    uMetafilePosY,
                    uMetafilePosX + MAX_METAFILE_WIDTH,
                    uMetafilePosY + MAX_METAFILE_HEIGHT,
                };
				if (pTexture)
				{
					pTexture->SetRect(DesRc, SrcRc);
                    pTexture->UpdataVB();
                    pTexture->RenderTexture(m_lpDirectDevice9);
				}

                //// ���������Ӱ
                //if (IsDrawShadow && !((ShapeShadowPosX == ShapePosX) && (ShapeShadowPosY == ShapePosY)) )
                //{
                //    RECT ShadowDesRc = 
                //    {
                //        DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + ((BYTE_X + ShapeShadowPosX) * MAX_METAFILE_WIDTH),
                //        DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + ((BYTE_Y + ShapeShadowPosY) * MAX_METAFILE_HEIGHT),
                //        DRAW_BACK_SCENE_POS_X + ((BACKSCENE_MAX_SQUARE_COUNT_X - dwBackSceneWidth)/2*MAX_METAFILE_WIDTH) + ((BYTE_X + ShapeShadowPosX) * MAX_METAFILE_WIDTH) + MAX_METAFILE_WIDTH,
                //        DRAW_BACK_SCENE_POS_Y + ((BACKSCENE_MAX_SQUARE_COUNT_Y - dwBackSceneHeight)/2*MAX_METAFILE_HEIGHT) + ((BYTE_Y + ShapeShadowPosY) * MAX_METAFILE_HEIGHT) + MAX_METAFILE_HEIGHT,
                //    };
                //    pTexture->SetRect(ShadowDesRc, SrcRc);
                //    pTexture->RenderTexture(m_lpDirectDevice9);
                //}
            }
        }
    }
    // ���سɹ�
    return TRUE;
}

// ���л����ı���Ϣ
BOOL CRender::DrawCenterText(TCHAR* szText, RECT* rcIntegral,  COLORREF cColor)
{
    DWORD dwSize = (DWORD)_tcslen(szText);
    USE_PARAM(dwSize, rcIntegral, cColor);
    //g_pGame->GetBuffImage()->PrintText(m_hIntegralFont, rcIntegral, szText, cColor, dwSize, DT_CENTER);
    return TRUE;
}

// �����ı���Ϣ
BOOL CRender::DrawText(VOID)
{
    TCHAR szTextBuff[MAX_PATH] = {0};

    RECT rcIntegral = {395, 48, 586, 104};
    // ������Ϸ����
    DWORD dwIntegral = g_pGame->GetIntegral();
    _stprintf_s(szTextBuff, MAX_PATH, _T("����:%d"), dwIntegral);
    INT nSize = (INT)_tcslen(szTextBuff);
	this->DrawFont(m_lpD3DIntegralFont, szTextBuff, nSize, &rcIntegral, DT_LEFT | DT_SINGLELINE | DT_VCENTER, ARGB(255, 255, 0, 0));

    // ������Ϸ�ٶȵȼ�
    DWORD dwSpeedLevel = g_pGame->GetSpeedLevel();
    RECT rcSpeedLevel = {395, 113, 481, 129};
    _stprintf_s(szTextBuff, MAX_PATH, _T("�ٶȼ���:%d"), dwSpeedLevel);
	nSize = (INT)_tcslen(szTextBuff);
	this->DrawFont(m_lpD3DLevelFont, szTextBuff, nSize, &rcSpeedLevel, DT_LEFT | DT_SINGLELINE | DT_VCENTER, ARGB(255, 255, 0, 0));

    // ������Ϸ�ؿ��ȼ�
    DWORD dwGameLevel = g_pGame->GetGameLevel();
    RECT rcGameLevel = {496, 113, 585, 129};
    _stprintf_s(szTextBuff, MAX_PATH, _T("�ؿ�����:%d"), dwGameLevel);
	nSize = (INT)_tcslen(szTextBuff);
	this->DrawFont(m_lpD3DLevelFont, szTextBuff, nSize, &rcGameLevel, DT_LEFT | DT_SINGLELINE | DT_VCENTER, ARGB(255, 255, 0, 0));
    return TRUE;
}
// ������һ������
BOOL CRender::DrawNextShape(VOID)
{
    // ��ȡ����Ԫ���׵�ַ
    LPBYTE pNext_BYTE = m_pSquare->GetNext_ShapeBuff();
    LPBYTE pNext2_BYTE = m_pSquare->GetNext2_ShapeBuff();
    LPBYTE pNext3_BYTE = m_pSquare->GetNext3_ShapeBuff();
    if (!pNext_BYTE || !pNext2_BYTE || !pNext3_BYTE)
    {
        return TRUE;
    }

    DWORD dwNext_MetafileIndex = m_pSquare->GetNext_MetafileIndex();
    // ������һ������
    for (UPOS BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT ; BYTE_Y++)
    {
        // ѭ������ָ���е����к��ŷ���
        for (UPOS BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
        {
            // ����ָ��������ͼԪ��ͼƬ�еĺ�����,������
            if (pNext_BYTE[BYTE_Y * SHAPE_MAX_WIDTH + BYTE_X])
            {
				UPOS uMetafilePosX, uMetafilePosY;
				CMs2DTexture* pTexture = this->AccountMetafileInfo(dwNext_MetafileIndex, uMetafilePosX, uMetafilePosY);

				RECT DesRc = 
				{
					309 + (BYTE_X * MAX_METAFILE_WIDTH),
					56 + (BYTE_Y * MAX_METAFILE_HEIGHT),
					309 + (BYTE_X * MAX_METAFILE_WIDTH) + MAX_METAFILE_WIDTH,
					56 + (BYTE_Y * MAX_METAFILE_HEIGHT) + MAX_METAFILE_HEIGHT,
				};
				RECT SrcRc = 
				{
					uMetafilePosX,
					uMetafilePosY,
					uMetafilePosX + MAX_METAFILE_WIDTH,
					uMetafilePosY + MAX_METAFILE_HEIGHT,
				};
				if (pTexture)
				{
					pTexture->SetRect(DesRc, SrcRc);
                    pTexture->UpdataVB();
                    pTexture->RenderTexture(m_lpDirectDevice9);
				}
            }
        }
    }

    DWORD dwNext2_MetafileIndex = m_pSquare->GetNext2_MetafileIndex();
    // ��������һ������
    for (UPOS BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT ; BYTE_Y++)
    {
        // ѭ������ָ���е����к��ŷ���
        for (UPOS BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
        {
            // ����ָ��������ͼԪ��ͼƬ�еĺ�����,������
            if (pNext2_BYTE[BYTE_Y * SHAPE_MAX_WIDTH + BYTE_X])
            {
				UPOS uMetafilePosX, uMetafilePosY;
				CMs2DTexture* pTexture = this->AccountMetafileInfo(dwNext2_MetafileIndex, uMetafilePosX, uMetafilePosY);

				RECT DesRc = 
				{
					182 + (BYTE_X * MAX_METAFILE_WIDTH),
					56 + (BYTE_Y * MAX_METAFILE_HEIGHT),
					182 + (BYTE_X * MAX_METAFILE_WIDTH) + MAX_METAFILE_WIDTH,
					56 + (BYTE_Y * MAX_METAFILE_HEIGHT) + MAX_METAFILE_HEIGHT,
				};
				RECT SrcRc = 
				{
					uMetafilePosX,
					uMetafilePosY,
					uMetafilePosX + MAX_METAFILE_WIDTH,
					uMetafilePosY + MAX_METAFILE_HEIGHT,
				};
				if (pTexture)
				{
					pTexture->SetRect(DesRc, SrcRc);
                    pTexture->UpdataVB();
                    pTexture->RenderTexture(m_lpDirectDevice9);
				}
            }
        }
    }

    DWORD dwNext3_MetafileIndex = m_pSquare->GetNext3_MetafileIndex();
    // ����������һ������
    for (UPOS BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT ; BYTE_Y++)
    {
        // ѭ������ָ���е����к��ŷ���
        for (UPOS BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
        {
            // ����ָ��������ͼԪ��ͼƬ�еĺ�����,������
            if (pNext3_BYTE[BYTE_Y * SHAPE_MAX_WIDTH + BYTE_X])
            {

				UPOS uMetafilePosX, uMetafilePosY;
				CMs2DTexture* pTexture = this->AccountMetafileInfo(dwNext3_MetafileIndex, uMetafilePosX, uMetafilePosY);

				RECT DesRc = 
				{
					56 + (BYTE_X * MAX_METAFILE_WIDTH),
					56 + (BYTE_Y * MAX_METAFILE_HEIGHT),
					56 + (BYTE_X * MAX_METAFILE_WIDTH) + MAX_METAFILE_WIDTH,
					56 + (BYTE_Y * MAX_METAFILE_HEIGHT) + MAX_METAFILE_HEIGHT,
				};
				RECT SrcRc = 
				{
					uMetafilePosX,
					uMetafilePosY,
					uMetafilePosX + MAX_METAFILE_WIDTH,
					uMetafilePosY + MAX_METAFILE_HEIGHT,
				};
				if (pTexture)
				{
					pTexture->SetRect(DesRc, SrcRc);
                    pTexture->UpdataVB();
                    pTexture->RenderTexture(m_lpDirectDevice9);
				}
            }
        }
    }
    // ���سɹ�
    return TRUE;
}

// ���ñ���
VOID CRender::ResetBackScene(VOID)
{
    return m_pTextureBack->RenderTexture(m_lpDirectDevice9);
}

// ������Ҫʹ�õ�ͼԪ��Ϣ
CMs2DTexture* CRender::AccountMetafileInfo(DWORD dwCurrentMetafileIndex, UPOS& uMetafilePosX, UPOS& uMetafilePosY)
{
	uMetafilePosX = 0;
	uMetafilePosY = 0;
	CMs2DTexture* pTexture = NULL;
	switch (dwCurrentMetafileIndex)
	{
	case 0:
		{
			pTexture = m_pTextureMetafile_0;
		}break;
	case 1:
		{
			pTexture = m_pTextureMetafile_0;
			uMetafilePosX = MAX_METAFILE_WIDTH;
		}break;
	case 2:
		{
			pTexture = m_pTextureMetafile_1;
		}break;
	case 3:
		{
			pTexture = m_pTextureMetafile_1;
			uMetafilePosX = MAX_METAFILE_WIDTH;
		}break;
	case 4:
		{
			pTexture = m_pTextureMetafile_1;
			uMetafilePosY = MAX_METAFILE_WIDTH;
		}break;
	case 5:
		{
			pTexture = m_pTextureMetafile_1;
			uMetafilePosX = MAX_METAFILE_WIDTH;
			uMetafilePosY = MAX_METAFILE_WIDTH;
		}break;
	case 6:
		{
			pTexture = m_pTextureMetafile_2;
		}break;
	case 7:
		{
			pTexture = m_pTextureMetafile_2;
			uMetafilePosX = MAX_METAFILE_WIDTH;
		}break;
	case 8:
		{
			pTexture = m_pTextureMetafile_2;
			uMetafilePosY = MAX_METAFILE_WIDTH;
		}break;
	case 9:
		{
			pTexture = m_pTextureMetafile_2;
			uMetafilePosX = MAX_METAFILE_WIDTH;
			uMetafilePosY = MAX_METAFILE_WIDTH;
		}break;
	case 10:
		{
			pTexture = m_pTextureMetafile_3;
		}break;
	case 11:
		{
			pTexture = m_pTextureMetafile_3;
			uMetafilePosX = MAX_METAFILE_WIDTH;
		}break;
	}
	return pTexture;
}