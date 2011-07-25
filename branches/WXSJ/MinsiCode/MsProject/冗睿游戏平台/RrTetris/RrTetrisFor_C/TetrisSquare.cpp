#include "Precompiled.h"

// ����Ĺ��캯��
CT_Square::CT_Square(VOID)
: m_SquarePosX              (0)     // ��ʼ����״�ڳ����еĺ�����Ϊ0
, m_SquarePosY              (0)     // ��ʼ����״�ڳ����е�������Ϊ0
, m_nMetafilePosX           (0)
, m_nMetafilePosY           (0)
, m_wCurrentShapeIndex      (0)
, m_wNext_ShapeIndex        (0)
, m_wNext2_ShapeIndex       (0)
, m_wNext3_ShapeIndex       (0)
, m_pCurrentShape           (NULL)
, m_pNext_Shape             (NULL)
, m_pNext2_Shape            (NULL)
, m_pNext3_Shape            (NULL)
, m_wCurrentMetafileIndex   (0)
, m_wNext_MetafileIndex     (0)
, m_wNext2_MetafileIndex    (0)
, m_wNext3_MetafileIndex    (0)
, m_pBitmapMetafile         (NULL)
, m_VoidCountByLeft         (0)
, m_VoidCountByRight        (0)
, m_VoidCountByUp           (0)
, m_VoidCountByDown         (0)
, m_dwShapeCount            (0)
, m_SquareShadowPosX        (0)
, m_SquareShadowPosY        (0)
, m_pShapeFile              (NULL)
{
    m_pShapeFile = NEW CT_ShapeFile(DEFAULT_SHAPE_FILE_NAME);
    AssertEx(AL_CATAST, m_pShapeFile, _T("�ڴ治��"));
    m_dwShapeCount = m_pShapeFile->GetShapeCount();
}


// �������������
CT_Square::~CT_Square(VOID)
{
    SAFE_DELETE(m_pShapeFile);
}

// ��ʼ��ͼԪ���ݳ�Ա
BOOL CT_Square::InitMetafile(WORD wMetafileIndex)
{
    _ENTER_FUN_C;

    // ��鴫�������ֵ�Ƿ���ȷ
    AssertEx(AL_CATAST, wMetafileIndex <= MAX_METAFILE_COUNT, _T("�����������Χ����,�������"));

    // ����ͼԪ����ֵ
    m_wCurrentMetafileIndex = wMetafileIndex;

    // ����ָ��������ͼԪ��ͼƬ�е�����,������
    m_nMetafilePosX = m_wCurrentMetafileIndex * MAX_METAFILE_WIDTH;
    m_nMetafilePosY = 0;

    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��ʼ����״���ݳ�Ա
BOOL CT_Square::InitShape(WORD wShapeIndex)
{
    _ENTER_FUN_C;

    m_wCurrentShapeIndex = wShapeIndex;

    // ������״������ȡ��״
    m_pCurrentShape = m_pShapeFile->GetShapeByIndex(wShapeIndex);
    AssertEx(AL_CATAST, m_pCurrentShape, _T("��ȡ��״ʧ��,����ʧ��"));

    // ��ȡ��״�ܱ߿հ׸�����Ϣ
    m_VoidCountByLeft   = m_pShapeFile->GetShapeUsedSquareByLeft(m_wCurrentShapeIndex);
    AssertEx(AL_CATAST, m_VoidCountByLeft!=INVALID_BID, _T("��ȡ��ǰ��״ʹ���˼�����ȵķ���ʧ��,����ʧ��"));

    m_VoidCountByRight  = m_pShapeFile->GetShapeUsedSquareByRight(m_wCurrentShapeIndex);
    AssertEx(AL_CATAST, m_VoidCountByRight!=INVALID_BID, _T("��ȡ��ǰ��״ʹ���˼�����ȵķ���ʧ��,����ʧ��"));

    m_VoidCountByUp     = m_pShapeFile->GetShapeUsedSquareByUp(m_wCurrentShapeIndex);
    AssertEx(AL_CATAST, m_VoidCountByDown!=INVALID_BID, _T("��ȡ��ǰ��״ʹ���˼�����ȵķ���ʧ��,����ʧ��"));

    m_VoidCountByDown   = m_pShapeFile->GetShapeUsedSquareByDown(m_wCurrentShapeIndex);
    AssertEx(AL_CATAST, m_VoidCountByDown!=INVALID_BID, _T("��ȡ��ǰ��״ʹ���˼�����ȵķ���ʧ��,����ʧ��"));

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��״����
BOOL CT_Square::Distortion(DWORD dwBackSceneWidth, DWORD dwBackSceneHeight, BOOL bOrder)
{
    _ENTER_FUN_C;

    WORD    wNextShapeIndex = m_wCurrentShapeIndex;
    NPOS    SquarePosX = m_SquarePosX;
    NPOS    SquarePosY = m_SquarePosY;

    // �����˳����ת
    if (bOrder)
    {
        // ����ǵ�ǰ��״�����һ��
        if (((m_wCurrentShapeIndex + 1) % SHAPE_DISTORTION_STATE) == 0)
        {
            // ���ص���һ����״
            wNextShapeIndex -= SHAPE_DISTORTION_STATE - 1;
        }
        // �������һ��
        else
        {
            // ++����һ����״
            wNextShapeIndex++;
        }
    }
    // ����ǵ�����ת
    else
    {
        // ����ǵ�ǰ��״�е�һ��
        if (((m_wCurrentShapeIndex + 1) % SHAPE_DISTORTION_STATE) == 1)
        {
            // ���ص����һ����״
            wNextShapeIndex += SHAPE_DISTORTION_STATE - 1;
        }
        // ���ǵ�һ��
        else
        {
            // --����һ����״
            wNextShapeIndex--;
        }
    }

    // ��ȡ��ǰ��״ʹ���˼�����ȵķ���
    BYTE NextShape_FirstHaveIndexByLeft = m_pShapeFile->GetShapeUsedSquareByLeft(wNextShapeIndex);
    BYTE NextShape_FirstHaveIndexByRight= m_pShapeFile->GetShapeUsedSquareByRight(wNextShapeIndex);
    BYTE NextShape_FirstHaveIndexByUp   = m_pShapeFile->GetShapeUsedSquareByUp(wNextShapeIndex);
    BYTE NextShape_FirstHaveIndexByDown = m_pShapeFile->GetShapeUsedSquareByDown(wNextShapeIndex);

    // �������״λ������ǽ��Ҫ����
    if (SquarePosX + (SHAPE_MAX_WIDTH - NextShape_FirstHaveIndexByRight) >= (INT)dwBackSceneWidth)
    {
        //if (m_VoidCountByRight > NextShape_FirstHaveIndexByRight)
        {
            SquarePosX -= ((SquarePosX + (SHAPE_MAX_WIDTH - NextShape_FirstHaveIndexByRight)) - dwBackSceneWidth);
        }
    }

    // �������״λ������ǽ��Ҫ����
    if (SquarePosX + NextShape_FirstHaveIndexByLeft < 0)
    {
        //if (m_VoidCountByLeft > NextShape_FirstHaveIndexByLeft)
        {
            SquarePosX = (-NextShape_FirstHaveIndexByLeft);
        }
    }

    // �������״λ������ǽ��Ҫ����
    else if (SquarePosY + NextShape_FirstHaveIndexByUp < 0)
    {
        SquarePosY = (-NextShape_FirstHaveIndexByUp);
    }

    // �������״λ������ǽ��Ҫ����
    if (SquarePosY + NextShape_FirstHaveIndexByUp < 0)
    {
        SquarePosY = (-NextShape_FirstHaveIndexByUp);
    }
    // �������״λ������ǽ��Ҫ����
    else if (SquarePosY + NextShape_FirstHaveIndexByDown > (INT)dwBackSceneHeight)
    {
        return TRUE;
    }

    if (g_pGame->CheckShapeCanHere(SquarePosX, SquarePosY, wNextShapeIndex))
    {
        // ��ȡ�µ���״�������µ�ʹ�ÿ��
        m_VoidCountByLeft   = NextShape_FirstHaveIndexByLeft;
        m_VoidCountByRight  = NextShape_FirstHaveIndexByRight;
        m_VoidCountByUp     = NextShape_FirstHaveIndexByUp;
        m_VoidCountByDown   = NextShape_FirstHaveIndexByDown;
        m_wCurrentShapeIndex= wNextShapeIndex;
        m_SquarePosX        = SquarePosX;
        m_SquarePosY        = SquarePosY;

        // ��ȡ����״
        this->InitShape(m_wCurrentShapeIndex);
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;

}

// ��ȡ��״�ڳ����е�����
VOID CT_Square::GetShapePos(NPOS& nX, NPOS& nY)
{
    // ������״�ڳ����е�����
    nX = m_SquarePosX;
    nY = m_SquarePosY;
}

// ��ȡ��״��Ӱ�ڳ����е�����
VOID CT_Square::GetShapeShadowPos(NPOS& uX, NPOS& uY)
{
    // ������״��Ӱ�ڳ����е�����
    uX = m_SquareShadowPosX;
    uY = m_SquareShadowPosY;
}

// ��ȡ��״�ڳ����е�����
VOID CT_Square::SetShapePos(NPOS X, NPOS Y)
{
    // ��ȡ��״�ڳ����е�����
    m_SquarePosX = X;
    m_SquarePosY = Y;
}

// ��ȡ��״��Ӱ�ڳ����е�����
VOID CT_Square::SetShapeShadowPos(NPOS X, NPOS Y)
{
    // ��ȡ��״�ڳ����е�����
    m_SquareShadowPosX = X;
    m_SquareShadowPosY = Y;
}

// ��ȡ��״�ļ�
CT_ShapeFile* CT_Square::GetShapeFile(VOID)
{
    return m_pShapeFile;
}

// ��������
BOOL CT_Square::CreateShape(DWORD dwBackSceneWidth)
{
    // ����õ�������ɫ
    DOUBLE fRadix;
    m_wCurrentMetafileIndex = m_wNext_MetafileIndex ? m_wNext_MetafileIndex : ((fRadix = RADIXN) , (R_BYTE(fRadix, 3, 11)));
    m_wNext_MetafileIndex = m_wNext2_MetafileIndex ? m_wNext2_MetafileIndex : ((fRadix = RADIXN) , (R_BYTE(fRadix, 3, 11)));
    m_wNext2_MetafileIndex = m_wNext3_MetafileIndex ? m_wNext3_MetafileIndex : ((fRadix = RADIXN) , (R_BYTE(fRadix, 3, 11)));
    fRadix = RADIXN;
    m_wNext3_MetafileIndex = R_BYTE(fRadix, 3, 11);

    m_wCurrentShapeIndex = m_wNext_ShapeIndex ? m_wNext_ShapeIndex : ((fRadix = RADIXN) , (R_WORD(fRadix, 0, m_dwShapeCount)));
    m_wNext_ShapeIndex = m_wNext2_ShapeIndex ? m_wNext2_ShapeIndex : ((fRadix = RADIXN) , (R_WORD(fRadix, 0, m_dwShapeCount)));
    m_wNext2_ShapeIndex = m_wNext3_ShapeIndex ? m_wNext3_ShapeIndex : ((fRadix = RADIXN) , (R_WORD(fRadix, 0, m_dwShapeCount)));
    fRadix = RADIXN;
    m_wNext3_ShapeIndex = R_WORD(fRadix, 0, m_dwShapeCount);
    this->InitShape(m_wCurrentShapeIndex);

    // ������״������ȡ��״
    m_pNext_Shape = m_pShapeFile->GetShapeByIndex(m_wNext_ShapeIndex);
    m_pNext2_Shape = m_pShapeFile->GetShapeByIndex(m_wNext2_ShapeIndex);
    m_pNext3_Shape = m_pShapeFile->GetShapeByIndex(m_wNext3_ShapeIndex);

    // ���õ�ǰ��״��λ�����������м�
    this->SetShapePos((dwBackSceneWidth / 2) - (SHAPE_MAX_WIDTH / 2), (-m_VoidCountByUp));

    // ���سɹ�
    return TRUE;
}

// �����״
VOID CT_Square::Clear(VOID)
{
    this->InitMetafile(0);
    m_pCurrentShape = NULL;
    this->SetShapePos(INVALID_UID, INVALID_UID);
}