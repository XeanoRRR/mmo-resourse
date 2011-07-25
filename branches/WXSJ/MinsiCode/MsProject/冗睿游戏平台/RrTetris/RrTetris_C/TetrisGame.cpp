#include "Precompiled.h"

// ȫ����Ϸ����
extern CT_Game* g_pGame = NULL;

CT_Game::CT_Game(HWND hWnd)
:m_pScene           (NULL)						// ����ģ��ָ��
,m_pSquare              (NULL)                  // ����ģ��ָ��
,m_pRender              (NULL)                  // ��Ⱦģ��ָ��
,m_pSound               (NULL)                  // ����ģ��ָ��
,m_GameState            (GAME_STATE_NO_INIT)    // ��Ϸ״̬δ��ʼ����
,m_hWnd                 (hWnd)                  // ��ʼ����Ϸ�����ھ��
,m_uTimerId             (1)                     // ��ʼ����Ϸ��ʱ��IDΪ0
,m_dwGameLevel          (0)                     // ��ʼ����Ϸ�ٶȼ���Ϊ��Ч
,m_dwGameSpeed          (INVALID_UID)           // ��Ϸ�ٶ�
,m_pLuaManager          (NULL)                  // ��ʼ���ű�������ָ��Ϊ��
,m_IsAutoBack           (FALSE)
,m_IsAutoFront          (FALSE)
,m_IsDrawShadow         (TRUE)
,m_uIntegral            (0)
,m_dwGameTollGateLevel  (0)
,m_dwEachLevelIntegral  (0)
,m_bGameNeedReset       (FALSE)
,m_IsWin                (FALSE)
,m_dwBackMusicCount     (0)
,m_dwCurrentMusicIndex  (0)
{
    ;
}

CT_Game::~CT_Game(VOID)
{
    this->ReleaseGame();
}

// ��ʼ����Ϸ
BOOL CT_Game::InitGame(VOID)
{
    // �������ͷ���ֵ
    BOOL bRet = TRUE;
    // ����ű����ͷ���ֵ
    LUA_FUN_RET_VALUE LuaFunRetVal(LUA_RET_INT);

    AssertEx(AL_CATAST, m_GameState == GAME_STATE_NO_INIT, _T("��Ϸ״̬����,���ô���"));

    // ���뱳������
    m_pScene = NEW CT_Scene;
    AssertEx(AL_CATAST, m_pScene, _T("�ڴ治��"));

    // ���뷽�����
    m_pSquare = NEW CT_Square;
    AssertEx(AL_CATAST, m_pSquare, _T("�ڴ治��"));

    // ������Ⱦ����
    m_pRender = NEW CRender(m_hWnd, BACKSCENE_MAX_WIDTH, BACKSCENE_MAX_HEIGHT, m_pScene, m_pSquare);
    if (m_pRender)
    {
        m_lpD3DFrameFont = CMs2DEngine::s_CreateD3D9Font(20, FW_BOLD, 1, FALSE, _T("����"));
        RECT rc = {0, 0, BACKSCENE_MAX_WIDTH, BACKSCENE_MAX_HEIGHT};
        m_pRender->EnableFrame(m_lpD3DFrameFont, TRUE, rc);
    }
    else
    {
        AssertEx(AL_CATAST, m_pRender, _T("�ڴ治��"));
    }

    // ������������
    m_pSound = NEW CMsSound(GAME_SOUND_INDEX_COUNT);
    AssertEx(AL_CATAST, m_pSound, _T("�ڴ治��"));
    bRet = m_pSound->Init(FALSE);

    // ����ű�������
    m_pLuaManager = NEW CMsLuaManager;
    if (m_pLuaManager)
    {
        m_pLuaManager->Init();
        // �򿪽ű������ļ�
        TCHAR szFileNameBuff[MAX_PATH] = {0};
        CreateFullPath(szFileNameBuff, _T(SCRIPT_FILE_NAME));
        m_pLuaManager->LoadLuaFile(szFileNameBuff);

        // ����������Ҫ�ķ���
        m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("GetEachLevelNeedIntegral"), NULL);
        m_dwEachLevelIntegral = (DWORD)LuaFunRetVal.m_lLuaFunRet;
    }
    else
    {
        AssertEx(AL_CATAST, m_pLuaManager, _T("�ڴ治��"));
    }

    // ������Ϸ����
    this->LoadGameSound();

    // ����Ϸ״̬��Ϊֹͣ
    m_GameState = GAME_STATE_STOP;

    // ��ʼ��Ⱦ
    //m__pRender1 = NEW CTestRender(m_hWnd, BACKSCENE_MAX_WIDTH, BACKSCENE_MAX_HEIGHT);
    //m_lpD3DFrameFont = CMs2DEngine::s_CreateD3D9Font(20, FW_BOLD, 1, FALSE, _T("����"));
    //RECT rc = {0, 0, BACKSCENE_MAX_WIDTH, BACKSCENE_MAX_HEIGHT};
    //m__pRender1->EnableFrame(m_lpD3DFrameFont, TRUE, rc);

    ////m__pRender2 = NEW CTestRender(m_hWnd, 460, 331);
    ////m_lpD3DFrameFont = CMs2DEngine::s_CreateD3D9Font(20, FW_BOLD, 1, FALSE, _T("����"));

    //m__pRender1->Run();
    //m__pRender2->Run();

    m_pRender->Run();

    // ���سɹ�
    return TRUE;
}

// �ͷ���Ϸ
BOOL CT_Game::ReleaseGame(VOID)
{
    CMs2DEngine::s_DeleteD3D9();
    CMs2DEngine::s_Release();

    // ֹͣ��Ϸ
    this->EndGame();

    if (m_pRender)
    {
        m_pRender->Stop(TRUE);
    }
    //if (m__pRender1)
    //{
    //    m__pRender1->Stop(TRUE);
    //}
    //SAFE_DELETE(m__pRender1);

    //if (m__pRender2)
    //{
    //    m__pRender2->Stop(TRUE);
    //}
    //SAFE_DELETE(m__pRender2);

    // ��ȫɾ���ű�ģ��
    SAFE_DELETE(m_pLuaManager);

    // ��ȫɾ������ģ��
    SAFE_DELETE(m_pSound);

    // ��ȫɾ����Ⱦģ��
    SAFE_DELETE(m_pRender);

    // ��ȫɾ������ģ��
    SAFE_DELETE(m_pSquare);

    // ��ȫɾ������ģ��
    SAFE_DELETE(m_pScene);

    // ����Ϸ״̬�޸�Ϊδ��ʼ��
    m_GameState = GAME_STATE_NO_INIT;

    // ���سɹ�
    return TRUE;
}

// ��ʼ��Ϸ
BOOL CT_Game::StartGame(VOID)
{
    // �������ͷ���ֵ
    BOOL bRet = TRUE;
    // ����ű����ͷ���ֵ
    LUA_FUN_RET_VALUE LuaFunRetVal(LUA_RET_INT);

    // �����Ϸ�Ƿ��Ѿ���ʼ���ɹ�
    if (m_GameState != GAME_STATE_STOP)
    {
        return TRUE;
    }

    // ������Ϸ�ؿ��ȼ�Ϊ1��
    m_dwGameTollGateLevel = 1;
    // ������Ϸ�ȼ�Ϊ1��
    m_dwGameLevel = 1;

    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("GetTollGateSize"), _T("%d"), m_dwGameTollGateLevel);
    DWORD SceneSize = (DWORD)LuaFunRetVal.m_lLuaFunRet;
    this->Reset(HIWORD(SceneSize), LOWORD(SceneSize));

    // ����һ���·���
    m_pSquare->CreateShape(HIWORD(SceneSize));

    // ������ó���Ϸ��
    m_GameState = GAME_STATE_PLAYING;

    // ��������
    bRet = m_pSound->Play(GAME_SOUND_READYGO, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("��������ʧ��,����ʧ��"));

    //// ������������
    //::InvalidateRect(m_hWnd, NULL, FALSE);

    // ���سɹ�
    return TRUE;
}

// ��ͣ��Ϸ
BOOL CT_Game::PauseGame(VOID)
{
    // �����Ϸ������
    if (m_GameState == GAME_STATE_PLAYING)
    {

        // ����������ͣ
        m_pSound->Pause(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);

        // ������Ϸ��ͣ���
        m_GameState = GAME_STATE_PAUSE;
    }
    else
    {
        AssertEx(AL_CATAST, FALSE, _T("��Ϸ״̬�쳣,�߼�����"));
    }
    return TRUE;
}

// ������Ϸ
BOOL CT_Game::ContinueGame(VOID)
{
    // �����Ϸ��ͣ��
    if (m_GameState == GAME_STATE_PAUSE)
    {
        // ������Ϸ
        m_pSound->Continue(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);

        // ������Ϸ���б��
        m_GameState = GAME_STATE_PLAYING;
    }
    else
    {
        AssertEx(AL_CATAST, FALSE, _T("��Ϸ״̬�쳣,�߼�����"));
    }
    return TRUE;
}

// ��ȡ��Ϸ״̬
GAME_STATE CT_Game::GetGameState(VOID)
{
    // ������Ϸ״̬
    return m_GameState;
}

// ������Ϸ
BOOL CT_Game::EndGame(VOID)
{
    // �����Ϸδ����
    if (m_GameState == GAME_STATE_NO_INIT ||
        m_GameState == GAME_STATE_STOP)
    {
        // ���سɹ�
        return TRUE;
    }
    // ������Ϸ����ʱ�������
    //::InvalidateRect(m_hWnd, NULL, FALSE);
    //m_pDlg->UpDataDlg();

    // ����Ϸ״̬��Ϊֹͣ
    m_GameState = GAME_STATE_STOP;

    // ֹͣ��������
    m_pSound->Stop(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);

    // �رն�ʱ����
    ::KillTimer(m_hWnd, m_uTimerId);
    //m_pDlg->KillTimer(m_uTimerId);

    // ���سɹ�
    return TRUE; 
}

//// ������
//BOOL CT_Game::DrawBack(VOID)
//{
//    // ���Ʊ���
//    m_pRender->DrawBackScene();
//    return m_pBuffImage->EndDraw();
//}

//// ������Ϸ
//BOOL CT_Game::Render(VOID)
//{
//    // �������ͷ���ֵ
//    BOOL bRet = TRUE;
//    USE_PARAM(bRet);
//    //UPDATA_LOCALTIME;
//    //FILETIME FileTime;
//    //INT64* lpn64Time = (INT64*)&FileTime;
//    //::SystemTimeToFileTime(&g_LocalTime, &FileTime);
//    //static INT64 OldTime = *lpn64Time;
//    //static INT64 CurTime = *lpn64Time;
//    //FLOAT Time  = (CurTime  - OldTime)/1000.0f/10000.f;
//    //OldTime = CurTime;
//    //CurTime = *lpn64Time;
//    //FLOAT FPS = 1.0f/Time;
//    //TCHAR szBuff[FWORD_NUMBER_SIZE] = {0};
//    //_stprintf_s(szBuff,FWORD_NUMBER_SIZE,_T("%3.7f"),FPS);
//
//   
//
//    //DWORD dwBackSceneWidth,dwBackSceneHeight;
//    //m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);
//
//    //m_pBuffImage->BeginDraw(FALSE);
//
//    // ���ñ���
//    m_pRender->ResetBackScene();
//    
//    switch (m_GameState)
//    {
//    //��Ϸ��ʼ״̬
//    case GAME_STATE_PLAYING:
//        {
//            // ���Ʊ���
//            bRet = m_pRender->DrawBackScene();
//            AssertEx(AL_NORMAL, bRet, _T("DrawBackScene����ʧ��,����ʧ��"));
//
//            // �����ı�
//            bRet = m_pRender->DrawText();
//            AssertEx(AL_NORMAL, bRet, _T("DrawText����ʧ��,����ʧ��"));
//
//            // ���ƺ�������
//            bRet = m_pRender->DrawNextShape();
//            AssertEx(AL_NORMAL, bRet, _T("DrawNextShape����ʧ��,����ʧ��"));
//
//            // ���㷽����Ӱλ��
//            this->CalculateShadowPos();
//
//            // ���Ʒ���
//            bRet = m_pRender->DrawActivityShape(m_IsDrawShadow);
//            AssertEx(AL_NORMAL, bRet, _T("DrawActivityShape����ʧ��,����ʧ��"));
//        }break;
//    //// ��Ϸ׼����ʼ״̬
//    //case GAME_STATE_START:
//    //    {
//    //        // �����ı�
//    //        m_wTime += Time;
//    //        bRet = m_pRender->DrawText();
//    //        AssertEx(bRet, _T("DrawText����ʧ��,����ʧ��"));
//    //        if(m_wTime < 0.5f)
//    //        {
//    //            // �����ı�
//    //            RECT rc = {280,250,480,350};
//    //            bRet = m_pRender->DrawCenterText(_T("READY!!!"), &rc, RGB(255,255,0));
//    //            AssertEx(bRet, _T("DrawCenterText����ʧ��,����ʧ��"));
//    //        }else
//    //        {
//    //            // �����ı�
//    //            RECT rc = {280,250,480,350};
//    //            bRet = m_pRender->DrawCenterText(_T("GO!!!"), &rc, RGB(255,255,0));
//    //            AssertEx(bRet, _T("DrawCenterText����ʧ��,����ʧ��"));
//    //        }
//
//
//    //        if ( !m_pSound->IsPlaying(GAME_SOUND_READYGO))
//    //        {
//    //            m_GameState = GAME_STATE_PLAYING;
//    //            m_wTime = 0;
//    //            this->PlayBackMusic();
//    //            this->ResetIntegral();
//    //        }
//    //    }break;
//    //��Ϸ��ͣ״̬
//    case GAME_STATE_PAUSE:
//        {
//            // �����ı�
//            bRet = m_pRender->DrawText();
//            AssertEx(AL_NORMAL, bRet, _T("DrawText����ʧ��,����ʧ��"));
//            // �����ı�
//            RECT rc = {280,250,480,350};
//            bRet = m_pRender->DrawCenterText(_T("��Ϸ��ͣ"), &rc, RGB(255,255,0));
//            AssertEx(AL_NORMAL, bRet, _T("DrawCenterText����ʧ��,����ʧ��"));
//           
//        }break;
//    }
//    // �����ı�
//    //RECT rc1 = {100,100,480,350};
//    //bRet = m_pRender->DrawCenterText(szBuff, &rc1, RGB(255,255,0));
//
//    //m_pRender->Render();
//    // ���سɹ�
//    return TRUE;
//}

// ������Ϸ����
BOOL CT_Game::AddGameVolume(VOID)
{
    return FALSE;
}

// ������Ϸ����
BOOL CT_Game::SubGameVolume(VOID)
{
    return FALSE;
}

// ������������
BOOL CT_Game::AddMusicVolume(VOID)
{
    // �����Ϸ״̬�Ƿ�Ϊ��Ϸ��
    if (m_GameState == GAME_STATE_PLAYING)
    {
        // ��������
        m_pSound->AddVolume(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);
    }

    // ���سɹ�
    return TRUE;
}

// ������������
BOOL CT_Game::SubMusicVolume(VOID)
{
    // �����Ϸ״̬�Ƿ�Ϊ��Ϸ��
    if (m_GameState == GAME_STATE_PLAYING)
    {
        // ��������
        m_pSound->SubVolume(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);
    }

    // ���سɹ�
    return TRUE;
}

// ��Ϸ����(��ʱ��������Ϸʱ��Ƭ����)
BOOL CT_Game::GameHeartbeat(VOID)
{
    if (m_GameState != GAME_STATE_PLAYING)
    {
        return TRUE;
    }

    if (m_IsAutoBack)
    {
        static WORD wBackSceneMetafileIndex = 0;
        wBackSceneMetafileIndex++;
        wBackSceneMetafileIndex %= 1;
        m_pScene->SetBackMetafileIndex(wBackSceneMetafileIndex);
    }

    UINT uRadix = R_BYTE_A(1, 100);
    if (1 == uRadix)
    {
        this->PlayBackMusic();
    }

    if (m_IsAutoFront)
    {
        static WORD wFrontSceneMetafileIndex = 0;
        wFrontSceneMetafileIndex++;
        wFrontSceneMetafileIndex %= 9;
        m_pScene->SetFrontMetafileIndex(wFrontSceneMetafileIndex);
    }
    this->MoveShape(VK_DOWN);

    return TRUE;
}

// �����״�Ƿ���Ե�����
BOOL CT_Game::CheckShapeCanHere(NPOS SquarePosX, NPOS SquarePosY, WORD wShapeIndex)
{
    CT_ShapeFile* pShapeFile = m_pSquare->GetShapeFile();
    BYTE* pShape = pShapeFile->GetShapeByIndex(wShapeIndex);
    SHORT VoidCountByLeft = pShapeFile->GetShapeUsedSquareByLeft(wShapeIndex);
    SHORT VoidCountByRight= pShapeFile->GetShapeUsedSquareByRight(wShapeIndex);
    SHORT VoidCountByDown = pShapeFile->GetShapeUsedSquareByDown(wShapeIndex);
    INT nBackSceneWidth,nBackSceneHeight;
    m_pScene->GetBackSceneSize((DWORD&)nBackSceneWidth, (DWORD&)nBackSceneHeight);
    BACK_SCENE_BYTE* pSceneElement = m_pScene->GetBackSceneBuff();

    // ѭ���������еķ���
    for (INT Y = 0; Y < SHAPE_MAX_HEIGHT ; Y++)
    {
        // ѭ������ָ���еĺ��ŷ���
        for (INT X = 0; X < SHAPE_MAX_WIDTH; X++)
        {
            if (((X+SquarePosX+VoidCountByLeft) < 0) ||
                ((Y+SquarePosY-VoidCountByDown) >= nBackSceneHeight) ||
                ((X+SquarePosX-VoidCountByRight) >= nBackSceneWidth))
            {
                return FALSE;
            }
            if ((pSceneElement[(Y+SquarePosY) * nBackSceneWidth + (X+SquarePosX)].m_Square) &&
                pShape[Y * SHAPE_MAX_WIDTH + X])
            {
                return FALSE;
            }
        }
    }

    // ���سɹ�
    return TRUE;
}

// �ƶ�����
BOOL CT_Game::MoveShape(UINT uWay)
{
    if (m_GameState != GAME_STATE_PLAYING)
    {
        return TRUE;
    }

    // ��������ֵ
    BOOL bRet = TRUE;

    bRet = this->IsGameOver();
    if (bRet)
    {
        return TRUE;
    }

    NPOS SquarePosX, SquarePosY;
    m_pSquare->GetShapePos(SquarePosX, SquarePosY);
    WORD wShapeIndex        = m_pSquare->GetCurrentShapeIndex();
    BYTE VoidCountByLeft    = m_pSquare->GetVoidCountByLeft();
    BYTE VoidCountByRight   = m_pSquare->GetVoidCountByRight();
    BYTE VoidCountByDown     = m_pSquare->GetVoidCountByDown();
    //BACK_SCENE_BYTE* pSceneElement = m_pScene->GetBackSceneBuff();
    DWORD   dwBackSceneWidth, dwBackSceneHeight;
    m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);

    switch (uWay)
    {
    case VK_LEFT:
        {
            if (SquarePosX > (-VoidCountByLeft))
            {
                if (this->CheckShapeCanHere(SquarePosX-1, SquarePosY, wShapeIndex))
                {
                    m_pSquare->SetShapePos(SquarePosX-1, SquarePosY);
                }
            }
        }break;
    case VK_RIGHT:
        {
            // �ж������������ƶ����ĸ�λ��
            if ((SquarePosX + (SHAPE_MAX_WIDTH - VoidCountByRight)) < (INT)dwBackSceneWidth)
            {
                if (this->CheckShapeCanHere(SquarePosX+1, SquarePosY, wShapeIndex))
                {
                    m_pSquare->SetShapePos(SquarePosX+1, SquarePosY);
                }
            }
        }break;
    case VK_DOWN:
        {
            // ������黹û��������
            if ((SquarePosY + (SHAPE_MAX_HEIGHT - VoidCountByDown)) < (INT)dwBackSceneHeight)
            {
                if (this->CheckShapeCanHere(SquarePosX, SquarePosY+1, wShapeIndex))
                {
                    m_pSquare->SetShapePos(SquarePosX, SquarePosY+1);
                }
                // ������������ײ
                else
                {
                    // �̶���״������
                    this->FixationShape();
                }
            }
            // ���鵽����
            else if (SquarePosY + (SHAPE_MAX_HEIGHT - VoidCountByDown) == (INT)dwBackSceneHeight)
            {
                // �̶���״������
                this->FixationShape();
            }
        }break;
    case VK_UP:
        {
            m_pSquare->Distortion(dwBackSceneWidth, dwBackSceneHeight);
            bRet = m_pSound->Play(GAME_SOUND_DISTORTION, FALSE);
            AssertEx(AL_SHOW_ONLY, bRet, _T("��������ʧ��,����ʧ��"));
        }break;
    case WM_USER:
    case VK_SPACE:
        {
            for (;;)
            {
                // ������黹û��������
                if ((SquarePosY + (SHAPE_MAX_HEIGHT - VoidCountByDown)) < (INT)dwBackSceneHeight)
                {
                    if (this->CheckShapeCanHere(SquarePosX, SquarePosY+1, wShapeIndex))
                    {
                        SquarePosY += 1;
                    }
                    // ������������ײ
                    else
                    {
                        if (WM_USER == uWay)
                        {
                            m_pSquare->SetShapeShadowPos(SquarePosX, SquarePosY);
                            return TRUE;
                        }
                        else if (VK_SPACE == uWay)
                        {
                            // �̶���״������
                            m_pSquare->SetShapePos(SquarePosX, SquarePosY);
                            this->FixationShape();
                        }
                        break;
                    }
                }
                // ���鵽����
                else if (SquarePosY + (SHAPE_MAX_HEIGHT - VoidCountByDown) == (INT)dwBackSceneHeight)
                {
                    if (WM_USER == uWay)
                    {
                        m_pSquare->SetShapeShadowPos(SquarePosX, SquarePosY);
                        return TRUE;
                    }
                    else if (VK_SPACE == uWay)
                    {
                        // �̶���״������
                        m_pSquare->SetShapePos(SquarePosX, SquarePosY);
                        this->FixationShape();
                    }
                    break;
                }
            }
        }break;
    default:
        {
            // ���سɹ�
            return FALSE;
        }
    }

    // ���»���
    //::InvalidateRect(m_hWnd, NULL, FALSE);

    // ���سɹ�
    return FALSE;
}

// �̶�����
BOOL CT_Game::FixationShape(VOID)
{
    // ��������ֵ
    BOOL bRet = TRUE;

    if (m_GameState != GAME_STATE_PLAYING)
    {
        return TRUE;
    }

    NPOS SquarePosX, SquarePosY;
    m_pSquare->GetShapePos(SquarePosX, SquarePosY);
    WORD wShapeIndex        = m_pSquare->GetCurrentShapeIndex();
    SHORT VoidCountByLeft   = m_pSquare->GetVoidCountByLeft();
    SHORT VoidCountByRight  = m_pSquare->GetVoidCountByRight();
    SHORT VoidCountByDown   = m_pSquare->GetVoidCountByDown();
    BYTE* pShape           = m_pSquare->GetShapeFile()->GetShapeByIndex(wShapeIndex);
    WORD wMetafileIndex     = m_pSquare->GetCurrentMetafileIndex(); 
    BACK_SCENE_BYTE* pSceneElement = m_pScene->GetBackSceneBuff();
    INT nBackSceneWidth,nBackSceneHeight;
    m_pScene->GetBackSceneSize((DWORD&)nBackSceneWidth, (DWORD&)nBackSceneHeight);

    m_pSound->Play(GAME_SOUND_FIXATIONSOUND_0, FALSE);

    // ѭ���������еķ���
    for (INT Y = 0; Y < SHAPE_MAX_HEIGHT ; Y++)
    {
        // ѭ������ָ���еĺ��ŷ���
        for (INT X = 0; X < SHAPE_MAX_WIDTH; X++)
        {
            if (((X+SquarePosX+VoidCountByLeft) < 0) ||
                ((Y+SquarePosY-VoidCountByDown) >= nBackSceneHeight) ||
                ((X+SquarePosX-VoidCountByRight) >= nBackSceneWidth))
            {
                AssertEx(AL_CATAST, FALSE, _T("�����뷽�������쳣,�����߼�"));
            }
            if ((pSceneElement[(Y+SquarePosY) * nBackSceneWidth + (X+SquarePosX)].m_Square) &&
                pShape[Y * SHAPE_MAX_WIDTH + X])
            {
                bRet = this->IsGameOver();
                AssertEx(AL_CATAST, bRet, _T("��Ϸ����û�н���,�����߼�"));
                return TRUE;
            }
            else
            {
                // ������״������
                if (pShape[Y * SHAPE_MAX_WIDTH + X])
                {
                    pSceneElement[(Y+SquarePosY) * nBackSceneWidth + (X+SquarePosX)].m_Square = pShape[Y * SHAPE_MAX_WIDTH + X];
                    pSceneElement[(Y+SquarePosY) * nBackSceneWidth + (X+SquarePosX)].m_MetafileIndex = wMetafileIndex;
                }
            }
        }
    }

    // �����ǰ��״
    m_pSquare->Clear();

    this->AddIntegral(m_dwGameLevel);

    // ��������
    this->RemoveFullLine();

    if (m_bGameNeedReset)
    {
        SIZE_T SceneSize = 0;
        if (m_IsWin)
        {
            LUA_FUN_RET_VALUE LuaFunRetVal(LUA_RET_INT);
            m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("GetTollGateSize"), _T("%d"), 11);
            SceneSize = LuaFunRetVal.m_lLuaFunRet;
        }
        else
        {
            LUA_FUN_RET_VALUE LuaFunRetVal(LUA_RET_INT);
            m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("GetTollGateSize"), _T("%d"), m_dwGameTollGateLevel);
            SceneSize = LuaFunRetVal.m_lLuaFunRet;
        }
        this->Reset(HIWORD(SceneSize), LOWORD(SceneSize));
        m_bGameNeedReset = FALSE;
    }

    // �����µ���״
    m_pScene->GetBackSceneSize((DWORD&)nBackSceneWidth, (DWORD&)nBackSceneHeight);
    m_pSquare->CreateShape((DWORD)nBackSceneWidth);
    this->SetTimer(m_dwGameSpeed);
    this->IsGameOver();

    // ���سɹ�
    return TRUE;
}

#pragma warning(push)
#pragma warning(disable : 6385)
// �������еķ���
BOOL CT_Game::RemoveFullLine(VOID)
{
    DWORD X,Y;
    DWORD dwBackSceneWidth, dwBackSceneHeight;
    m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);
    BACK_SCENE_BYTE* pSceneElement = m_pScene->GetBackSceneBuff();

    UINT uClearCount = 0;
    // ѭ���������еķ���
    for (Y = 0; Y <  dwBackSceneHeight; Y++)
    {
        // ѭ������ָ���е����к��ŷ���
        for (X = 0; X < dwBackSceneWidth; X++)
        {
            // ������ŷ������пո�
            if (pSceneElement[Y * dwBackSceneWidth + X].m_Square == NO_SQUARE)
            {
                // ������ǰ����,����������һ����
                break;
            }
        }
        // ��������Ѿ��ɹ����������һ��,˵����ǰ��������,��������
        if (X == dwBackSceneWidth)
        {
            uClearCount++;
            m_pSound->Play(GAME_SOUND_BLASTSOUND_1 + (uClearCount - 1), FALSE);
            m_pSound->SetVolume(GAME_SOUND_BLASTSOUND_1 + (uClearCount - 1), uClearCount * GAME_NORMAL_VOLUME);
            this->RemoveEffect(&pSceneElement[Y * dwBackSceneWidth], dwBackSceneWidth);
            // ������ͷ��ʼ��������֮������ݿ������ڶ���
            memcpy(&pSceneElement[dwBackSceneWidth], pSceneElement, Y*dwBackSceneWidth*sizeof(BACK_SCENE_BYTE));
            // ����һ�г�ʼ��Ϊ��
            ZeroMemory(pSceneElement, dwBackSceneWidth * sizeof(BACK_SCENE_BYTE));
            for (DWORD i = 0; i < dwBackSceneWidth; i++)
            {
                pSceneElement[i].m_MetafileIndex = m_pScene->GetBackMetafileIndex();
            }
            this->AddIntegral(dwBackSceneWidth);
        }
    }

    switch (uClearCount)
    {
    case 0x01:
        {
            this->AddIntegral(dwBackSceneWidth);
        }break;
    case 0x02:
        {
            this->AddIntegral(dwBackSceneWidth*3);
            m_pSound->Play(GAME_SOUND_HURRAY01, FALSE);
            m_pSound->SetVolume(GAME_SOUND_HURRAY01, VOLUME_MAX_VALUE);
        }break;
    case 0x03:
        {
            this->AddIntegral(dwBackSceneWidth*5);
            m_pSound->Play(GAME_SOUND_HURRAY02, FALSE);
            m_pSound->SetVolume(GAME_SOUND_HURRAY02, VOLUME_MAX_VALUE);
        }break;
    case 0x04:
        {
            this->AddIntegral(dwBackSceneWidth*7);
            m_pSound->Play(GAME_SOUND_HURRAY03, FALSE);
            m_pSound->SetVolume(GAME_SOUND_HURRAY03, VOLUME_MAX_VALUE);
        }break;
    }
    return TRUE;
}
#pragma warning(pop)

// �������еķ���
BOOL CT_Game::RemoveEffect(BACK_SCENE_BYTE* pSceneElement, DWORD Count)
{
    DWORD dwRemoveSpeed;
    if (Count > 11)
    {
        dwRemoveSpeed = 3;
    }
    else if (Count > 5)
    {
        dwRemoveSpeed = 2;
    }
    else
    {
        dwRemoveSpeed = 1;
    }
    for (DWORD i = 0; i < Count; i++)
    {
        pSceneElement[i].m_MetafileIndex = 0x01;
        if (i % dwRemoveSpeed == 0 || i == Count)
        {
			::SleepEx(5, TRUE);
        }
    }

    for (DWORD i = 0; i < Count; i++)
    {
        if (0 == i)
        {
            pSceneElement[i].m_MetafileIndex = 0x0A;
        }
        else if (1 == i)
        {
            pSceneElement[i - 1].m_MetafileIndex = 0x02;
            pSceneElement[i].m_MetafileIndex = 0x0A;
        }
        else
        {
            pSceneElement[i-2].m_MetafileIndex = 0x01;
            pSceneElement[i-1].m_MetafileIndex = 0x02;
            pSceneElement[i].m_MetafileIndex = 0x0A;
        }
        ::SleepEx(20,TRUE);
    }

    return TRUE;
}

// ������Ӱλ��
VOID CT_Game::CalculateShadowPos(VOID)
{
    NPOS SquarePosX, SquarePosY;
    m_pSquare->GetShapePos(SquarePosX, SquarePosY);
    WORD wShapeIndex = m_pSquare->GetCurrentShapeIndex();
    CT_ShapeFile* pShapeFile = m_pSquare->GetShapeFile();
    BYTE VoidCountByDown = pShapeFile->GetShapeUsedSquareByDown(wShapeIndex);
    DWORD dwBackSceneWidth,dwBackSceneHeight;
    m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);

    while (m_IsDrawShadow)
    {
        // ������黹û��������
        if (((SquarePosY + (SHAPE_MAX_HEIGHT - VoidCountByDown)) < (INT)dwBackSceneHeight) &&
            (this->CheckShapeCanHere(SquarePosX, SquarePosY+1, wShapeIndex)))
        {
            if (this->CheckShapeCanHere(SquarePosX, SquarePosY+1, wShapeIndex))
            {
                SquarePosY += 1;
            }
        }
        else
        {
            m_pSquare->SetShapeShadowPos(SquarePosX, SquarePosY);
            return;
        }
    }
}

// �����Զ�����
VOID CT_Game::SetAutoBack(BOOL bOpen)
{
    m_IsAutoBack = bOpen;
    return;
}

// �Ƿ��Զ�����
BOOL CT_Game::IsAutoBack(VOID)
{
    return m_IsAutoBack;
}

// ����Ƿ���Ϸ����(��������Ϸ����������)
BOOL CT_Game::IsGameOver(VOID)
{
    // ��������ֵ
    BOOL bRet = TRUE;

    // ��鷽���Ƿ���Է��뵱ǰλ��
    NPOS SquarePosX, SquarePosY;
    m_pSquare->GetShapePos(SquarePosX, SquarePosY);
    bRet = this->CheckShapeCanHere(SquarePosX, SquarePosY, m_pSquare->GetCurrentShapeIndex());

    // ���������
    if (!bRet)
    {
        // �ͷ���Ϸ
        this->EndGame();
        m_pSound->Play(GAME_SOUND_DEAD, FALSE);
        if (m_IsWin)
        {
            // ��Ϸ����
            return TRUE;
        }
        if (IDYES == ::MessageBox(m_hWnd, _T("�Ƿ����¿�ʼ��"),_T("��Ϸ����"), MB_YESNO))
        {
            // ��ʼ����Ϸ
            this->StartGame();
        }
        else
        {
            // ������Ϸ����
        //::InvalidateRect(m_hWnd, NULL, FALSE);
        }
        // ��Ϸ����
        return TRUE;
    }
    else
    {
        // ��Ϸ����
        return FALSE;
    }
}

// ������Ϸ�������
UINT CT_Game::SetTimer(UINT uTime)
{
    // ��¼��ǰ��ʱ����
    UINT uOldTime = m_dwGameSpeed;

    // ��ȡ��ʱ����
    m_dwGameSpeed = uTime;

    // �����µ�ʱ����
    ::KillTimer(m_hWnd, m_uTimerId);
    ::SetTimer(m_hWnd, m_uTimerId, m_dwGameSpeed, NULL);

    // ������ǰ��ʱ����
    return uOldTime;
}

// ������Ϸ
BOOL CT_Game::Reset(DWORD dwBackSceneWidth, DWORD dwBackSceneHeight)
{
    this->GetHeartbeatTime();
    this->SetTimer(m_dwGameSpeed);

    m_pScene->Reset(0, dwBackSceneWidth, dwBackSceneHeight);
    m_pRender->ResetBackScene();
    //::InvalidateRect(m_hWnd, NULL, FALSE);

    return TRUE;
}

//// ��ȡ������ָ��
//CVSTServerDlg* CT_Game::GetDlgPoint(VOID)
//{
//    _ENTER_FUN;
//
//    return m_pDlg;
//
//    _LEAVE_FUN_KEEP_NO_DISPOSE;
//    return FALSE;
//}
//
//

// ��ȡ��Ϸ�����Ӧ�������
INT CT_Game::GetHeartbeatTime(VOID)
{
    // ����ű����ͷ���ֵ
    LUA_FUN_RET_VALUE LuaFunRetVal(LUA_RET_INT);
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("GetHeartbeatTime"), _T("%d"), m_dwGameLevel);
    return m_dwGameSpeed = (DWORD)LuaFunRetVal.m_lLuaFunRet;
}

// ��ȡ��Ϸ����
DWORD CT_Game::GetIntegral(VOID)
{
    return m_uIntegral;
}

// ������Ϸ����
DWORD CT_Game::AddIntegral(UINT uMete)
{
    static UINT uLastIntegral = 0;

    if ((uMete + m_uIntegral) < m_uIntegral)
    {
        AssertEx(AL_CATAST, FALSE, _T("�������"));
    }
    else
    {
        m_uIntegral += uMete;
    }

    DWORD dwBackSceneWidth,dwBackSceneHeight;
    m_pScene->GetBackSceneSize(dwBackSceneWidth, dwBackSceneHeight);

    uLastIntegral -= (uLastIntegral % m_dwEachLevelIntegral);
    UINT uTemp = m_uIntegral - (m_uIntegral % m_dwEachLevelIntegral);
    if (uTemp > uLastIntegral && !m_IsWin)
    {
        this->LevelUp();
        uLastIntegral = m_uIntegral;
        return FALSE;
    }
    uLastIntegral = m_uIntegral;
    return TRUE;
}

// ������Ϸ����
DWORD CT_Game::SubIntegral(UINT uMete)
{
    if (uMete > m_uIntegral)
    {
        m_uIntegral = 0;
    }
    else
    {
        m_uIntegral -= uMete;
    }

    return TRUE;
}

// ������Ϸ����
VOID CT_Game::ResetIntegral(VOID)
{
    m_uIntegral = 0;
}

// ��Ϸ����
BOOL CT_Game::LevelUp(VOID)
{
    if (m_dwGameTollGateLevel < 10)
    {
        m_dwGameTollGateLevel++;
        this->SetReset();
        m_pSound->Play(GAME_SOUND_LEVELUP, FALSE);
    }
    else if (m_dwGameTollGateLevel == 10)
    {
        if (m_dwGameLevel == 10)
        {
            this->GameWin();
        }
        else
        {
            m_dwGameLevel++;
            this->GetHeartbeatTime();
            this->SetTimer(m_dwGameSpeed);
            m_pSound->Play(GAME_SOUND_LEVELCOMPLETE, FALSE);
            m_dwGameTollGateLevel = 1;
            this->SetReset();
        }
    }

    this->PlayBackMusic(TRUE);
    return TRUE;
}

// ������Ϸ����һ����ʱ������
VOID CT_Game::SetReset(VOID)
{
    m_bGameNeedReset = TRUE;
}

// ��Ϸʤ��
VOID CT_Game::GameWin(VOID)
{
    m_IsWin = TRUE;
    m_IsAutoFront = TRUE;
    this->SetReset();
}

// ���ű�������
BOOL CT_Game::PlayBackMusic(BOOL IsRePlay)
{
    // ��������ֵ
    BOOL bRet = TRUE;

    bRet = m_pSound->IsPlaying(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);
    if (bRet)
    {
        if (IsRePlay)
        {
            m_pSound->Stop(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex);
        }
        else
        {
            return TRUE;
        }
    }
    DOUBLE fRadix = RADIXN;
    DWORD nRadix = R_BYTE(fRadix, 1, m_dwBackMusicCount);
    if (m_dwCurrentMusicIndex == nRadix)
    {
        nRadix = R_BYTE(fRadix, 1, (m_dwBackMusicCount - 1));
        if (m_dwCurrentMusicIndex == nRadix)
        {
            nRadix++;
        }
    }
    m_dwCurrentMusicIndex = nRadix;
    bRet = m_pSound->Play(GAME_SOUND_INDEX_BACK_MUSIC00 + m_dwCurrentMusicIndex, FALSE, TRUE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("��������ʧ��,����ʧ��"));
    
    return bRet;
}

// ������Ϸ����
BOOL CT_Game::LoadGameSound(VOID)
{
    // ��������ֵ
    BOOL bRet = TRUE;
    // ����ű����ͷ���ֵ
    LUA_FUN_RET_VALUE LuaFunRetVal(LUA_RET_INT);

    // ��ȡ�������ָ���
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetBackMusicCount"), _T("%d"), 0);
    m_dwBackMusicCount = (DWORD)LuaFunRetVal.m_lLuaFunRet;

    LuaFunRetVal.m_eType = LUA_RET_STRING;
    FOR_x_TO(i, m_dwBackMusicCount)
    {
        // ��ȡ���������ļ�·����
        m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetBackMusic"), _T("%d"), (i));
        AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("���ر�������������Ϣʧ��,����ʧ��"));
        bRet = m_pSound->Open(GAME_SOUND_INDEX_BACK_MUSIC00 + i, LuaFunRetVal.m_String.m_str, FALSE);
        AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));
    }

    // ��ȡ��ʼ��Ϸ����
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetReadyGoSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_READYGO, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ�̶���������
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetFixationSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_FIXATIONSOUND_0, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ��ת��������
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetDistortioSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_DISTORTION, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ������������
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetBlastSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_BLASTSOUND_1, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_BLASTSOUND_2, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_BLASTSOUND_3, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_BLASTSOUND_4, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ��Ϸʧ������
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetDeadSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_DEAD, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ��Ϸ�Ȳ�����
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_Gethurray01Sound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_HURRAY01, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_Gethurray02Sound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_HURRAY02, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_Gethurray03Sound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_HURRAY03, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ��Ϸ��������
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetLevelUpSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_LEVELUP, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    // ��ȡ��Ϸ��������
    m_pLuaManager->RunLuaFun(&LuaFunRetVal, 1, _T("Sound_GetLevelCompleteSound"), NULL);
    AssertEx(AL_SHOW_ONLY, LuaFunRetVal.m_String.m_str, _T("������Ϸ��Ч������Ϣʧ��,����ʧ��"));
    bRet = m_pSound->Open(GAME_SOUND_LEVELCOMPLETE, LuaFunRetVal.m_String.m_str, FALSE);
    AssertEx(AL_SHOW_ONLY, bRet, _T("�������ļ�ʧ��,����ʧ��"));

    return TRUE;
}

// ��ȡ��Ϸ�ٶȵȼ�
DWORD CT_Game::GetSpeedLevel(VOID)
{
    return m_dwGameLevel;
}

// ��ȡ��Ϸ�ؿ�
DWORD CT_Game::GetGameLevel(VOID)
{
    return m_dwGameTollGateLevel;
}
