#include "Precompiled.h"

CT_Scene::CT_Scene(VOID)
: m_wBackSceneMetafileIndex (0)     // ��ʼ������ʹ�õ�ͼԪ����Ϊ0
, m_pSceneBYTE              (NULL)  // ��ʼ������Ԫ������ָ��Ϊ��
, m_dwBackSceneWidth        (0)     // ��ʼ���������Ϊ0
, m_dwBackSceneHeight       (0)     // ��ʼ�������߶�Ϊ0
{
    ;
}

CT_Scene::~CT_Scene(VOID)
{
    this->Release();
}


// ��ʼ��������
BOOL CT_Scene::Init(
    WORD wBackSceneMetafileIndex,
    DWORD dwBackSceneWidth,
    DWORD dwBackSceneHeight
    )
{
    _ENTER_FUN_C;

    // ��鴫���ͼԪ�����Ƿ�Ϸ�,��ȡ�����ͼԪ����
    AssertEx(AL_CATAST, wBackSceneMetafileIndex < MAX_METAFILE_COUNT, _T("�����ͼԪ�����Ƿ�,�������"));
    m_wBackSceneMetafileIndex = wBackSceneMetafileIndex;

    // ��鴫��ĳ�������Ƿ�Ϸ�,��ȡ����ĳ������
    AssertEx(AL_CATAST, dwBackSceneWidth <= BACKSCENE_MAX_SQUARE_COUNT_X, _T("����ĳ�������쳣,�������"));
    m_dwBackSceneWidth = dwBackSceneWidth;

    // ��鴫��ĳ�������Ƿ�Ϸ�,��ȡ����ĳ����߶�
    AssertEx(AL_CATAST, dwBackSceneHeight <= BACKSCENE_MAX_SQUARE_COUNT_Y, _T("����ĳ����߶��쳣,�������"));
    m_dwBackSceneHeight = dwBackSceneHeight;

    // ���볡��Ԫ������ռ�
    m_pSceneBYTE = NEW BACK_SCENE_BYTE[m_dwBackSceneWidth * m_dwBackSceneHeight];
    AssertEx(AL_CATAST, m_pSceneBYTE, _T("m_pSceneBYTEָ��Ϊ��,�ڴ治��"));

    // ��ʼ������Ԫ������Ϊ��
    ZeroMemory(m_pSceneBYTE, m_dwBackSceneWidth * m_dwBackSceneHeight * sizeof(BACK_SCENE_BYTE));

    // ���س�ʼ���ɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}


// �ͷų���
VOID CT_Scene::Release(VOID)
{
    // �ͷų�����������ռ�
    SAFE_DELETE_ARRAY(m_pSceneBYTE);

    m_dwBackSceneWidth = 0;
    m_dwBackSceneHeight = 0;
}

// ���ó���
BOOL CT_Scene::Reset(
    WORD wBackSceneMetafileIndex,
    DWORD dwBackSceneWidth,
    DWORD dwBackSceneHeight
    )
{
    this->Release();
    return this->Init(wBackSceneMetafileIndex, dwBackSceneWidth, dwBackSceneHeight);
}

// ��ȡ����Ԫ�������׵�ַ
BACK_SCENE_BYTE* CT_Scene::GetBackSceneBuff(VOID)
{
    // ����Ԫ�������׵�ַ
    return m_pSceneBYTE;
}

// ��ȡ����Ԫ�������׵�ַ
VOID CT_Scene::GetBackSceneSize(DWORD& dwBackSceneWidth, DWORD& dwBackSceneHeight)
{
    // ���س�����������߶�
    dwBackSceneWidth = m_dwBackSceneWidth;
    dwBackSceneHeight = m_dwBackSceneHeight;
}

// ���ó���ͼԪ����
BOOL CT_Scene::SetBackMetafileIndex(WORD wBackSceneMetafileIndex)
{
    _ENTER_FUN_C;

    // ��鴫���ͼԪ�����Ƿ�Ϸ�,��ȡ�����ͼԪ����
    AssertEx(AL_CATAST, wBackSceneMetafileIndex < MAX_METAFILE_COUNT, _T("�����ͼԪ�����Ƿ�,�������"));

    FOR_x_TO(i, (m_dwBackSceneWidth * m_dwBackSceneHeight))
    {
        if (m_pSceneBYTE[i].m_MetafileIndex == m_wBackSceneMetafileIndex)
        {
            m_pSceneBYTE[i].m_MetafileIndex = wBackSceneMetafileIndex;
        }
    }
    m_wBackSceneMetafileIndex = wBackSceneMetafileIndex;

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return NULL;
}

// ����ǰ��ͼԪ����
BOOL CT_Scene::SetFrontMetafileIndex(WORD wFrontSceneMetafileIndex)
{
    _ENTER_FUN_C;

    // ��鴫���ͼԪ�����Ƿ�Ϸ�,��ȡ�����ͼԪ����
    AssertEx(AL_CATAST, wFrontSceneMetafileIndex < MAX_METAFILE_COUNT, _T("�����ͼԪ�����Ƿ�,�������"));

    FOR_x_TO(i, (m_dwBackSceneWidth * m_dwBackSceneHeight))
    {
        if (m_pSceneBYTE[i].m_MetafileIndex != m_wBackSceneMetafileIndex)
        {
            m_pSceneBYTE[i].m_MetafileIndex = wFrontSceneMetafileIndex;
        }
    }
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return NULL;
}

// ���ó���ͼԪ����
WORD CT_Scene::GetBackMetafileIndex(VOID)
{
    return m_wBackSceneMetafileIndex;
}