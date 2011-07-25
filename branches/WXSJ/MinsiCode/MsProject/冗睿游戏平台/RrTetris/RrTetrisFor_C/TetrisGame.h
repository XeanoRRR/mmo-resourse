#pragma once

// �궨�屳�������ļ���
#define SCRIPT_FILE_NAME    "%s/RrTetris/Script/Config.lua"

// ����˹���鱳�����(��Ϸ��ƾ���,��Ҫ���׸Ķ�)
#define BACKSCENE_MAX_WIDTH     800

// ����˹���鱳���߶�(��Ϸ��ƾ���,��Ҫ���׸Ķ�)
#define BACKSCENE_MAX_HEIGHT    600

// ��Ϸ��������
enum GAME_SOUND_INDEX
{
    GAME_SOUND_INDEX_BACK_MUSIC00 = 0,
    GAME_SOUND_INDEX_BACK_MUSIC01,
    GAME_SOUND_INDEX_BACK_MUSIC02,
    GAME_SOUND_INDEX_BACK_MUSIC03,
    GAME_SOUND_INDEX_BACK_MUSIC04,
    GAME_SOUND_INDEX_BACK_MUSIC05,
    GAME_SOUND_INDEX_BACK_MUSIC06,
    GAME_SOUND_READYGO,
    GAME_SOUND_DISTORTION,
    GAME_SOUND_DEAD,
    GAME_SOUND_FIXATIONSOUND_0,
    GAME_SOUND_BLASTSOUND_1,
    GAME_SOUND_BLASTSOUND_2,
    GAME_SOUND_BLASTSOUND_3,
    GAME_SOUND_BLASTSOUND_4,
    GAME_SOUND_HURRAY01,
    GAME_SOUND_HURRAY02,
    GAME_SOUND_HURRAY03,
    GAME_SOUND_LEVELUP,
    GAME_SOUND_LEVELCOMPLETE,
    GAME_SOUND_INDEX_COUNT,
};

enum GAME_STATE
{
    GAME_STATE_NO_INIT = 0, // ��Ϸδ��ʼ��״̬
    GAME_STATE_STOP,        // ��Ϸֹͣ״̬
    GAME_STATE_PAUSE,       // ��Ϸ��ͣ״̬
    GAME_STATE_PLAYING,     // ��Ϸ������״̬
};

class CT_Game
{
public:
    CT_Game(HWND hWnd);
    virtual ~CT_Game(VOID);

    // ��ʼ����Ϸ
    BOOL    InitGame(VOID);
    BOOL    ReleaseGame(VOID);      // �ͷ���Ϸ
    BOOL    StartGame(VOID);        // ��ʼ��Ϸ
    BOOL    PauseGame(VOID);        // ��ͣ��Ϸ
    BOOL    ContinueGame(VOID);     // ������Ϸ
    BOOL    EndGame(VOID);          // ������Ϸ
    BOOL    AddGameVolume(VOID);    // ������Ϸ����
    BOOL    SubGameVolume(VOID);    // ������Ϸ����
    BOOL    AddMusicVolume(VOID);   // ������������
    BOOL    SubMusicVolume(VOID);   // ������������
    BOOL    GameHeartbeat(VOID);    // ��Ϸ����(��ʱ��������Ϸʱ��Ƭ����)
    BOOL    MoveShape(UINT uWay);   // �ƶ�����
    BOOL    FixationShape(VOID);    // �̶�����
    BOOL    RemoveFullLine(VOID);   // �������еķ���
    VOID    SetAutoBack(BOOL bOpen);// �����Զ�����
    BOOL    IsAutoBack(VOID);       // �Ƿ��Զ�����
    BOOL    IsGameOver(VOID);       // ����Ƿ���Ϸ����(�������һ��������Ϸ����������)
    UINT    SetTimer(UINT uTime);   // ������Ϸ�������
    HWND    GetMainWnd(VOID);       // ��ȡ�����ھ��
    INT     GetHeartbeatTime(VOID); // ��ȡ��Ϸ�����Ӧ�������
    DWORD   GetIntegral(VOID);      // ��ȡ��Ϸ����
    DWORD   AddIntegral(UINT uMete);// ������Ϸ����
    DWORD   SubIntegral(UINT uMete);// ������Ϸ����
    VOID    ResetIntegral(VOID);    // ������Ϸ����
    DWORD   GetSpeedLevel(VOID);    // ��ȡ��Ϸ�ٶȵȼ�
    DWORD   GetGameLevel(VOID);     // ��ȡ��Ϸ�ؿ�
    BOOL    LevelUp(VOID);          // ��Ϸ����
    VOID    GameWin(VOID);          // ��Ϸʤ��
    BOOL    LoadGameSound(VOID);    // ������Ϸ����
    VOID    SetReset(VOID);         // ������Ϸ����һ����ʱ������

    // ��ȡ��Ϸ״̬
    GAME_STATE GetGameState(VOID);

    // ������Ϸ
    BOOL Render(VOID); 

    // ���ű�������
    BOOL PlayBackMusic(
        BOOL IsRePlay =FALSE
        );

    // ��鷽���Ƿ���Է��ڴ˴�
    BOOL CheckShapeCanHere(
        NPOS SquarePosX,
        NPOS SquarePosY,
        WORD wShapeIndex
        );

    // �������з������Ч����
    BOOL RemoveEffect(
        BACK_SCENE_BYTE* pSceneElement,
        DWORD dwCount
        );

    // ������Ӱλ��
    VOID CalculateShadowPos(VOID);

    // ������Ϸ
    BOOL Reset(
        SIZE_T BackSceneWidth,
        SIZE_T BackSceneHeight
        );

// ˽�����ݳ�Ա
private:

    ID3DXFont*          m_lpD3DFrameFont;       // D3D֡���������
    BOOL                m_IsInit;               // ��Ϸ���Ƿ��ʼ�����
    HWND                m_hWnd;                 // ��Ϸ�����ھ��
    CT_Scene*           m_pScene;               // ����ģ��
    CT_Square*          m_pSquare;              // �������ָ��
    CRender*            m_pRender;              // ��Ⱦ����ָ��
    CMsSound*           m_pSound;               // ��������ָ��
    GAME_STATE          m_GameState;            // ��Ϸ״̬
    UINT                m_uTimerId;             // ��Ϸ��ʱ��ID
    DWORD               m_dwGameLevel;          // ��Ϸ�ȼ�
    DWORD               m_dwGameTollGateLevel;  // ��Ϸ�ؿ��ȼ�
    DWORD               m_dwGameSpeed;          // ��Ϸ�ٶ�
    CMsLuaManager*      m_pLuaManager;          // �ű�������ָ��
    BOOL                m_IsAutoBack;           // �Ƿ��Զ��л�����
    BOOL                m_IsAutoFront;          // �Ƿ��Զ��л�ǰ��
    BOOL                m_IsDrawShadow;         // �Ƿ������Ӱ
    UINT                m_uIntegral;            // ��Ϸ����
    BOOL                m_bGameNeedReset;       // ��Ϸ��Ҫ����
    BOOL                m_IsWin;                // ��Ϸ�Ƿ��Ѿ�ʤ��
    DWORD               m_dwBackMusicCount;     // �������ָ���
    DWORD               m_dwEachLevelIntegral;  // ����ÿ���������
    DWORD               m_dwCurrentMusicIndex;  // ��ǰ������������
};

// ȫ����Ϸ����
extern CT_Game* g_pGame;