#include "Precompiled.h"

CGameTree::CGameTree(CMsGdiBuffImage* lpCMsGdiBuffImage)
: m_lpCGameTypeRoot     (NULL)
, m_dwGameTypeNodeCount (0)
, m_lpCMsGdiBuffImage   (lpCMsGdiBuffImage)
, m_nLastBottom         (0)
, m_lpLastNode          (NULL)
, m_pCGameTreeCaption   (NULL)
, m_pCGameTreeScrollbar (NULL)
, m_bScrollFocus        (FALSE)
{
    m_pCGameTreeCaption = NEW CGameTreeCaption(m_lpCMsGdiBuffImage);
    AssertEx(AL_CATAST, m_pCGameTreeCaption, _T("�ڴ治��"));
    m_pCGameTreeCaption->SetFont(RR_GAME_CAPTION_TEXT_SIZE, FW_HEAVY, FALSE, FALSE, _T("����"));

    DWORD dwScrollbarX = 0;
    dwScrollbarX = g_pCGameStation->GetConfig()->dwDeskWidth;
    dwScrollbarX += g_pCGameStation->GetConfig()->dwRoomWidth;
    dwScrollbarX += g_pCGameStation->GetConfig()->dwDistrictWidth;
    dwScrollbarX += g_pCGameStation->GetConfig()->dwGameWidth;
    dwScrollbarX += g_pCGameStation->GetConfig()->dwGameTypeWidth;

    DWORD dwScrollbarY = 0;
    //dwScrollbarY = g_pCGameStation->GetConfig()->dwDeskHight;
    //dwScrollbarY += g_pCGameStation->GetConfig()->dwRoomHight;
    //dwScrollbarY += g_pCGameStation->GetConfig()->dwDistrictHight;
    //dwScrollbarY += g_pCGameStation->GetConfig()->dwGameHight;
    //dwScrollbarY += g_pCGameStation->GetConfig()->dwGameTypeHight;

    m_pCGameTreeScrollbar = NEW CGameTreeScrollbar(m_lpCMsGdiBuffImage, dwScrollbarX, dwScrollbarY, SCROLL_BAR_WIDTH, SCROLL_BAR_HEIGHT, LOGNITUDINAL);
    AssertEx(AL_CATAST, m_pCGameTreeScrollbar, _T("�ڴ治��"));

    m_CaptionRc.left = 0;
    m_CaptionRc.right = 500;
    m_CaptionRc.top = 0;
    m_CaptionRc.bottom = 200;
}

CGameTree::~CGameTree(VOID)
{
    this->Destroy();
}

// ���ٽڵ�
VOID CGameTree::Destroy(VOID)
{
    AUTO_LOCK(m_Lock);
    lpCGameTypeNode lpGameTypeNode = NULL;
    FOR_x_TO(i, m_dwGameTypeNodeCount)
    {
        lpGameTypeNode = m_lpCGameTypeRoot;
        m_lpCGameTypeRoot = (lpCGameTypeNode)m_lpCGameTypeRoot->GetNextNode();
        lpGameTypeNode->CutMe();
        lpGameTypeNode->Destroy();
        SAFE_DELETE(lpGameTypeNode);
    }
    m_dwGameTypeNodeCount = 0;
    SAFE_DELETE(m_pCGameTreeCaption);
    SAFE_DELETE(m_pCGameTreeScrollbar);
}

// ���ýڵ�
VOID CGameTree::Reset(VOID)
{
    AUTO_LOCK(m_Lock);
    lpCGameTypeNode lpGameTypeNode = NULL;
    FOR_x_TO(i, m_dwGameTypeNodeCount)
    {
        lpGameTypeNode = m_lpCGameTypeRoot;
        m_lpCGameTypeRoot = (lpCGameTypeNode)m_lpCGameTypeRoot->GetNextNode();
        lpGameTypeNode->CutMe();
        lpGameTypeNode->Destroy();
        SAFE_DELETE(lpGameTypeNode);
    }
    m_dwGameTypeNodeCount = 0;
}

// ���ƽڵ�
VOID CGameTree::Render(VOID)
{
    AUTO_LOCK(m_Lock);
    if (m_lpCGameTypeRoot)
    {
        // ���ƽڵ�
        m_lpCGameTypeRoot->Render(m_nLastBottom);

        // ���ƹ�����
        m_pCGameTreeScrollbar->DrawScrollbar();

        // ���Ʊ���
        m_pCGameTreeCaption->PrintText(&m_CaptionRc, _T("�����Ϸƽ̨"), (INT)_tcslen(_T("�����Ϸƽ̨")), RGB(255,0,0), DT_CENTER);
    }
}

// ������Ϸ����
CGameTypeNode* CGameTree::AddGameTypeNode(VOID)
{
    lpCGameTypeNode lpNode = NULL;

    // ����ǵ�һ���ڵ㡢�����Լ�Ϊͷβ�ڵ�
    if (0 == m_dwGameTypeNodeCount)
    {
        m_lpCGameTypeRoot = NEW CGameTypeNode(m_lpCMsGdiBuffImage, HEAD_NODE, TAIL_NODE);
        AssertEx(AL_CATAST, m_lpCGameTypeRoot, _T("�ڴ治��"));
        m_dwGameTypeNodeCount++;
        m_lpLastNode = m_lpCGameTypeRoot;
        lpNode = m_lpCGameTypeRoot;
    }

    // ���֮ǰ�нڵ㡢����֮ǰ�ڵ�ĺ���
    else
    {
        m_lpLastNode = NEW CGameTypeNode(m_lpCMsGdiBuffImage, m_lpLastNode, TAIL_NODE);
        AssertEx(AL_CATAST, m_lpLastNode, _T("�ڴ治��"));
        m_dwGameTypeNodeCount++;
        lpNode = m_lpLastNode;
    }
    DWORD dwHight = g_pCGameStation->GetConfig()->dwGameTypeHeight + g_pCGameStation->GetConfig()->m_dwBetween;
    m_pCGameTreeScrollbar->AddMaxMoveScrollY(dwHight);
    return lpNode;
}

// ����Ƿ񴩹�������
BOOL CGameTree::IsOnScrollbar(POINT& Pos)
{
    USE_PARAM(Pos);
    return TRUE;
}

// ��¼���λ��
VOID CGameTree::SaveMousePos(POINT& Pos)
{
    m_OldMousePos = Pos;
}

// ��¼���λ��
VOID CGameTree::OperationPosOffset(POINT& Pos)
{
    Pos.x -= m_OldMousePos.x;
    Pos.y -= m_OldMousePos.y;
}

// �ƶ�������
VOID CGameTree::MoveScroll(POINT& Pos)
{
    if (m_pCGameTreeScrollbar)
    {
        m_pCGameTreeScrollbar->MoveScrollX(Pos.x);
        m_pCGameTreeScrollbar->MoveScrollY(Pos.y);
    }
}

// ���ù���������
VOID CGameTree::GetScrollFocus(VOID)
{
    m_bScrollFocus = TRUE;
}

// �ͷŹ���������
VOID CGameTree::ReleaseScrollFocus(VOID)
{
    m_bScrollFocus = FALSE;
}

// �Ƿ����������
BOOL CGameTree::IsScrollFocus(VOID)
{
    return m_bScrollFocus;
}