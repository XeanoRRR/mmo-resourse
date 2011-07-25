#pragma once

#define STR_TEXT_MAX_COUNT 1024
#define DEFAULT_FONT_TYPE _T("����")

// λͼ�ļ�ͷ
#pragma pack(1)
typedef struct BITMAP_FILE_HEAD
{
    BITMAP_FILE_HEAD(DWORD dwMapWidth, DWORD dwMapHeight, WORD wMapBitCount)
        : m_BM              (0x4D42)
        , m_FileLength      (0x00000036)
        , m_Reserve         (0x00000000)
        , m_BFHLength       (0x00000036)

        , m_MapHLength      (0x00000028)
        , m_MapWidth        (dwMapWidth)
        , m_MapHeight       (dwMapHeight)
        , m_MapDcLevel      (0x0001)
        , m_MapBitCount     (wMapBitCount)
        , m_MapIsCompress   (0x00000000)
        , m_MapLength       (dwMapWidth*dwMapHeight*wMapBitCount/8)
        , m_MapPelsPerMeterX(0x00000000)
        , m_MapPelsPerMeterY(0x00000000)
        , m_ClrUsed         (0x00000000)
        , m_ClrImportant    (0x00000000){}
    WORD    m_BM;               // BM���
    DWORD   m_FileLength;       // �ļ�����
    DWORD   m_Reserve;          // ����
    DWORD   m_BFHLength;        // ͼ��Ϣ��ʼλ��

    DWORD   m_MapHLength;       // ͼ��Ϣͷ����
    DWORD   m_MapWidth;         // ͼ��
    DWORD   m_MapHeight;        // ͼ��
    WORD    m_MapDcLevel;       // λͼ�豸����
    WORD    m_MapBitCount;      // λͼλ��
    DWORD   m_MapIsCompress;    // �Ƿ�ѹ��
    DWORD   m_MapLength;        // λͼ���ݳߴ�
    DWORD   m_MapPelsPerMeterX; // ����ֱ���(����ÿ��)
    DWORD   m_MapPelsPerMeterY; // ����ֱ���(����ÿ��)
    DWORD   m_ClrUsed;          // λͼʵ��ʹ�õ���ɫ���е���ɫ��
    DWORD   m_ClrImportant;     // λͼ��ʾ��������Ҫ����ɫ��
}BFH;
#pragma pack(4)

typedef struct DRAW_TEXT_STRUCT
{
    DRAW_TEXT_STRUCT(VOID)
        : m_hBitmap         (NULL)
        , m_bLucencyBack    (TRUE)
        , m_FontColor       (RGB(255,255,255))
        , m_BackColor       (0)
        , m_nFormat         (DT_LEFT)
        , m_pHge            (NULL)
        , m_BitmapBit       (NULL)
    {
        //ZeroMemoryStruct(m_QuadCache);
        //ZeroMemoryArray(m_szString);
        m_hFont = ::CreateFont(
            10,
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            GB2312_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FF_DONTCARE|DEFAULT_PITCH,
            DEFAULT_FONT_TYPE);
    }
    ~DRAW_TEXT_STRUCT(VOID)
    {
        //if (m_pHge && m_QuadCache.tex)
        //{
        //    m_pHge->Texture_Free(m_QuadCache.tex);
        //    m_QuadCache.tex = NULL;
        //}
        if(m_hBitmap)
        {
            ::DeleteObject(m_hBitmap);
            m_hBitmap = NULL;
        }
        //SAFE_DELETE(m_BitmapBit);
    }

    HBITMAP     m_hBitmap;                      // ����ͼƬ���
    hgeQuad     m_QuadCache;                    // HGEǶ��
    TCHAR       m_szString[STR_TEXT_MAX_COUNT]; // ��������
    BOOL        m_bLucencyBack;                 // �����Ƿ�͸��
    COLORREF    m_FontColor;                    // ������ɫ
    COLORREF    m_BackColor;                    // ������ɫ
    HFONT       m_hFont;                        // ����
    RECT        m_rc;                           // ����ͼƬλ��
    UINT        m_nFormat;                      // ���ָ�ʽ
    HGE*        m_pHge;                         // HGEϵͳָ��
    BYTE*       m_BitmapBit;                    // λͼ����

}DTS;

class CHgeText
{
public:
	CHgeText(VOID);
	~CHgeText(VOID);

protected:
    HDC         m_DesktopDc;
    HDC         m_hDcCompatible;
    DTS*        m_pDts;
    hgeSprite*  m_phgeSprite;

public:

	BOOL CreateTextBox(DTS* pDts);
	BOOL UpDataString(VOID);
	VOID Render(VOID);
};
