#include "Precompiled.h"

CT_ShapeFile::CT_ShapeFile(TCHAR* strShapeFileName)
:m_hShapeFile(NULL)
,m_pData(NULL)
,m_dwFileSize(0)
{
    // ��ʼ����״�ļ�ͷ
    ZeroMemory(&m_ShapeFileHead, sizeof(m_ShapeFileHead));

    TCHAR szShapeFile[MAX_PATH] = {0};
    CreateFullPath(szShapeFile, strShapeFileName);

    // ����״�ļ�
    m_hShapeFile = ::CreateFile(
        szShapeFile,       // ���ļ���·����
        FILE_READ_DATA,         // ���ļ���Ȩ��Ϊ��
        FILE_SHARE_READ,        // �Թ�����ķ�ʽ���ļ�
        NULL,                   // ��ȫ�ṹ��Ϊ��
        OPEN_EXISTING,          // �Ա��ִ򿪵ķ�ʽ��
        NULL,                   // �ļ�����Ϊ��
        NULL
        );

    // ������״�ļ��Ƿ�ɹ�
    AssertEx(AL_CATAST, m_hShapeFile != INVALID_HANDLE_VALUE, _T("����״�ļ�ʧ��,����ʧ��"));

    // ��ȡ��״�ļ��ĳߴ�
    m_dwFileSize = ::GetFileSize(m_hShapeFile, NULL);

    // �����״�ļ��ߴ��Ƿ�Ϸ�
    AssertEx(AL_CATAST, m_dwFileSize, _T("��״�ļ��ߴ��쳣,����ʧ��"));

    // ������״�ļ�ͷ����
    DWORD dwShapeFileSize = sizeof(m_ShapeFileHead);

    // ��ȡ��״�ļ����ݵ�������
    BOOL bRet = ::ReadFile(
        m_hShapeFile,           // ��ȡ�ļ��ľ��
        &m_ShapeFileHead,       // ��״�ļ����ݻ�����
        dwShapeFileSize,          // ��ȡ��״�ļ�ͷ�ĳߴ�
        &dwShapeFileSize,         // ��ȡ��״�ļ�ͷ�ĳߴ�У��
        NULL                    // ��ȡ���Խṹ���
        );
    AssertEx(AL_CATAST, bRet, _T("��ȡ��״�ļ�ͷʧ��,����ʧ��"));

    // ����ȡ��״�ļ������Ƿ�ɹ�
    AssertEx(AL_CATAST, dwShapeFileSize == sizeof(m_ShapeFileHead), _T("��ȡ��״�ļ�ͷʧ��,����ʧ��"));

    // ���򿪵���״�ļ��Ƿ�Ϸ�
    AssertEx(AL_CATAST, SHAPE_FILE_FILG == m_ShapeFileHead.m_FileFlag, _T("��״�ļ��쳣,�ļ��쳣"));

    // ���򿪵���״�ļ��汾�Ƿ�ƥ��
    AssertEx(AL_CATAST, SHAPE_FILE_VERSION == m_ShapeFileHead.m_Version, _T("��״�ļ��汾�쳣,�ļ��쳣"));

    // ��ȡ��ȥ��״�ļ�ͷʣ�����ݵ��ܳ���
    m_dwFileSize -= sizeof(m_ShapeFileHead);

    // �����ļ����ݻ������ռ�
    m_pData = NEW BYTE[m_dwFileSize];

    // ��黺�����ռ��Ƿ�����ɹ�
    AssertEx(AL_CATAST, m_pData, _T("m_pDataΪNULL,�ڴ治��"));

    // ��ȡ��״�ļ����ݵ�������
    bRet = ::ReadFile(
        m_hShapeFile,   // ��ȡ�ļ��ľ��
        m_pData,        // ��״�ļ����ݻ�����
        m_dwFileSize,    // ��ȡ���ݵĳߴ�
        &m_dwFileSize,   // ��ȡ���ݵĳߴ�У��
        NULL            // ��ȡ���Խṹ���
        );
    AssertEx(AL_CATAST, bRet, _T("��ȡ��״�ļ�ͷʧ��,����ʧ��"));

    // ����ȡ��״�ļ������Ƿ�ɹ�
    AssertEx(
		AL_CATAST,
        m_dwFileSize == (SHAPE_FILE_ONE_SHAPE_SIZE * m_ShapeFileHead.m_ShapeCount),
        _T("��ȡ��״�ļ�����ʧ��,����ʧ��")
        );
}

CT_ShapeFile::~CT_ShapeFile(VOID)
{
    // �ͷ����뵽�Ŀռ�
    SAFE_DELETE_ARRAY(m_pData);

    if (m_hShapeFile)
    {
        ::CloseHandle(m_hShapeFile);
        m_hShapeFile = NULL;
    }
}

// ʹ��������ȡ��״
LPBYTE CT_ShapeFile::GetShapeByIndex(WORD wIndex)
{
    // ��鴫��������Ƿ�Ϸ�
    AssertEx(AL_CATAST, wIndex < m_ShapeFileHead.m_ShapeCount, _T("����Խ��,�������"));

    // �����״�ļ��Ƿ��ѱ���
    AssertEx(AL_CATAST, m_hShapeFile, _T("��״�ļ���ʧ��,���ô���"));

    // �����״�ļ������Ƿ��ѱ���ȡ
    AssertEx(AL_CATAST, m_pData, _T("��״�ļ������쳣,���ô���"));

    // ��������ָ������״������׵�ַ
    return (LPBYTE)(m_pData + (wIndex * SHAPE_FILE_ONE_SHAPE_SIZE));
}


// ��ȡ��״����
DWORD CT_ShapeFile::GetShapeCount(VOID)
{
    return m_ShapeFileHead.m_ShapeCount;
}

// ��ȡ��һ���з����λ�ô���
BYTE CT_ShapeFile::GetShapeUsedSquareByLeft(WORD wIndex)
{
    LPBYTE pBYTE = this->GetShapeByIndex(wIndex);
    for (BYTE BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
    {
        for (BYTE BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT; BYTE_Y++)
        {
            if (HAVE_SQUARE == (pBYTE[BYTE_Y * SHAPE_MAX_WIDTH + BYTE_X]))
            {
                AssertEx(AL_CATAST, (BYTE_X <= (BYTE)(INVALID_NID)), _T("�����쳣,�����߼�"));
                return BYTE_X;
            }
        }
    }
    return INVALID_BID;
}

// ��ȡ��һ���з����λ�ô���
BYTE CT_ShapeFile::GetShapeUsedSquareByRight(WORD wIndex)
{
    LPBYTE pBYTE = this->GetShapeByIndex(wIndex);
    for (BYTE BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
    {
        for (BYTE BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT; BYTE_Y++)
        {
            if (HAVE_SQUARE == (pBYTE[BYTE_Y * SHAPE_MAX_WIDTH + ((SHAPE_MAX_WIDTH-1) - BYTE_X)]))
            {
                AssertEx(AL_CATAST, (BYTE_X <= (BYTE)(INVALID_NID)), _T("�����쳣,�����߼�"));
                return BYTE_X;
            }
        }
    }
    return INVALID_BID;
}

// ��ȡ��һ���з����λ�ô���
BYTE CT_ShapeFile::GetShapeUsedSquareByUp(WORD wIndex)
{
    LPBYTE pBYTE = this->GetShapeByIndex(wIndex);
    for (BYTE BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT ; BYTE_Y++)
    {
        for (BYTE BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
        {
            if (HAVE_SQUARE == (pBYTE[BYTE_Y * SHAPE_MAX_WIDTH + BYTE_X]))
            {
                AssertEx(AL_CATAST, (BYTE_Y <= (BYTE)(INVALID_NID)), _T("�����쳣,�����߼�"));
                return BYTE_Y;
            }
        }
    }
    return INVALID_BID;
}

// ��ȡ��һ���з����λ�ô���
BYTE CT_ShapeFile::GetShapeUsedSquareByDown(WORD wIndex)
{
    LPBYTE pBYTE = this->GetShapeByIndex(wIndex);
    for (BYTE BYTE_Y = 0; BYTE_Y < SHAPE_MAX_HEIGHT ; BYTE_Y++)
    {
        for (BYTE BYTE_X = 0; BYTE_X < SHAPE_MAX_WIDTH; BYTE_X++)
        {
            if (HAVE_SQUARE == (pBYTE[((SHAPE_MAX_HEIGHT-1) - BYTE_Y) * SHAPE_MAX_WIDTH + BYTE_X]))
            {
                AssertEx(AL_CATAST, (BYTE_Y <= (BYTE)(INVALID_NID)), _T("�����쳣,�����߼�"));
                return BYTE_Y;
            }
        }
    }
    return INVALID_BID;
}