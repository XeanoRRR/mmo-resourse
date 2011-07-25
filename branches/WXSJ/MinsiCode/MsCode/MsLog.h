#pragma once          //��  //��
#ifndef __MSLOG_H__   //��  //��
#define __MSLOG_H__   //��  //��
//������������������������  //��
//�ļ����ƣ�MsLog.h         //��
//������������־��ض���    //��
//ƽ̨��أ�Windows         //��
//������������������������������

//������־�ı���󳤶�
#define LOG_STR_MAX_LEN 1256

typedef struct LOG_FILE_INFO
{
    HANDLE  m_hLogFile;
    TCHAR   m_szLogFileName[MAX_PATH];
}LFI;

enum CMSLOG_FLAG
{
    CMSLOG_FLAG_TIME = 0,           // ��ӡ����(��̬�ַ�)
    CMSLOG_FLAG_TIMEW,              // ��ӡ����(���ַ�)
    CMSLOG_FLAG_TIMEA,              // ��ӡ����(���ַ�)
    CMSLOG_FLAG_NOSAVE,             // ������
    CMSLOG_FLAG_SAVE,               // ����
    CMSLOG_FLAG_NEWLINE,            // ����
    CMSLOG_FLAG_TIME_BEGIN,         // ��ʱ��ʼ
    CMSLOG_FLAG_TIME_END,           // ��ʱ����(��̬�ַ�)
    CMSLOG_FLAG_TIME_ENDA,          // ��ʱ����(���ַ�)
    CMSLOG_FLAG_TIME_ENDW,          // ��ʱ����(���ַ�)
    CMSLOG_FLAG_END,                // ������־
    CMSLOG_FLAG_NEWLINE_END,        // ��������
    CMSLOG_FLAG_PAUSE,              // �����������
    CMSLOG_FLAG_PAUSE_REPEAT,       // ��������������ظ���ʾ
    CMSLOG_FLAG_FC_FINTENSITY,      // ����������ɫ����
    CMSLOG_FLAG_FC_UN_FINTENSITY,   // ����������ɫ�Ǹ���
    CMSLOG_FLAG_FC_COLOR_BLACK,     // ����������ɫΪ��
    CMSLOG_FLAG_FC_COLOR_WHITE,     // ����������ɫΪ��
    CMSLOG_FLAG_FC_COLOR_BLUE,      // ����������ɫΪ��
    CMSLOG_FLAG_FC_COLOR_GREEN,     // ����������ɫΪ��
    CMSLOG_FLAG_FC_COLOR_RED,       // ����������ɫΪ��
    CMSLOG_FLAG_FC_COLOR_BLUE_GREEN,// ����������ɫΪ����
    CMSLOG_FLAG_FC_COLOR_BLUE_RED,  // ����������ɫΪ����
    CMSLOG_FLAG_FC_COLOR_GREEN_RED, // ����������ɫΪ�̺�
    CMSLOG_FLAG_FC_COLOR_RV,        // ����������ɫΪ��ɫ
    CMSLOG_FLAG_LOCK,               // ��С��
    CMSLOG_FLAG_UNLOCK,             // ��С��
    CMSLOG_FLAG_LOCK_A,             // �Ӵ���
    CMSLOG_FLAG_UNLOCK_A,           // �����
    //CMSLOG_FLAG_BC_COLOR_BLACK,     // ����������ɫΪ��
    //CMSLOG_FLAG_BC_COLOR_WHITE,     // ����������ɫΪ��
    //CMSLOG_FLAG_BC_COLOR_BLUE,      // ����������ɫΪ��
    //CMSLOG_FLAG_BC_COLOR_GREEN,     // ����������ɫΪ��
    //CMSLOG_FLAG_BC_COLOR_RED,       // ����������ɫΪ��
    //CMSLOG_FLAG_BC_COLOR_BLUE_GREEN,// ����������ɫΪ����
    //CMSLOG_FLAG_BC_COLOR_BLUE_RED,  // ����������ɫΪ����
    //CMSLOG_FLAG_BC_COLOR_GREEN_RED, // ����������ɫΪ�̺�
};

#define L_TIME      CMSLOG_FLAG_TIME
#define L_TIMEW     CMSLOG_FLAG_TIMEW
#define L_TIMEA     CMSLOG_FLAG_TIMEA
#define L_NOSAVE    CMSLOG_FLAG_NOSAVE
#define L_SAVE      CMSLOG_FLAG_SAVE
#define L_NEWLINE   CMSLOG_FLAG_NEWLINE
#define L_TB        CMSLOG_FLAG_TIME_BEGIN
#define L_TE        CMSLOG_FLAG_TIME_END
#define L_TEA       CMSLOG_FLAG_TIME_ENDA
#define L_TEW       CMSLOG_FLAG_TIME_ENDW
#define L_END       CMSLOG_FLAG_END
#define L_NEND      CMSLOG_FLAG_NEWLINE_END
#define L_PAUSE     CMSLOG_FLAG_PAUSE
#define L_RPAUSE    CMSLOG_FLAG_PAUSE_REPEAT
#define L_FCF       CMSLOG_FLAG_FC_FINTENSITY
#define L_FCNF      CMSLOG_FLAG_FC_UN_FINTENSITY
#define L_FCK       CMSLOG_FLAG_FC_COLOR_BLACK
#define L_FCW       CMSLOG_FLAG_FC_COLOR_WHITE
#define L_FCB       CMSLOG_FLAG_FC_COLOR_BLUE
#define L_FCG       CMSLOG_FLAG_FC_COLOR_GREEN
#define L_FCR       CMSLOG_FLAG_FC_COLOR_RED
#define L_FCBG      CMSLOG_FLAG_FC_COLOR_BLUE_GREEN
#define L_FCBR      CMSLOG_FLAG_FC_COLOR_BLUE_RED
#define L_FCGR      CMSLOG_FLAG_FC_COLOR_GREEN_RED 
#define L_FCRV      CMSLOG_FLAG_FC_COLOR_RV
#define L_LOCK      CMSLOG_FLAG_LOCK
#define L_UNLOCK    CMSLOG_FLAG_UNLOCK
#define L_LOCK_A    CMSLOG_FLAG_LOCK_A
#define L_UNLOCK_A  CMSLOG_FLAG_UNLOCK_A
//#define L_BCK       CMSLOG_FLAG_BC_COLOR_BLACK
//#define L_BCW       CMSLOG_FLAG_BC_COLOR_WHITE
//#define L_BCB       CMSLOG_FLAG_BC_COLOR_BLUE
//#define L_BCG       CMSLOG_FLAG_BC_COLOR_GREEN
//#define L_BCR       CMSLOG_FLAG_BC_COLOR_RED
//#define L_BCBG      CMSLOG_FLAG_BC_COLOR_BLUE_GREEN
//#define L_BCBR      CMSLOG_FLAG_BC_COLOR_BLUE_RED
//#define L_BCGR      CMSLOG_FLAG_BC_COLOR_GREEN_RED 

class CMsLog
{
public:
    // ����
    CMsLog(LOG_FILE_INFO* pLogFileInfo, DWORD dwLogFileCount, BOOL bLock);
    CMsLog(TCHAR* szLogFileName, BOOL bLock);
    ~CMsLog(VOID);

    CMsLog& operator << (WCHAR Inof);       // �ַ�
    CMsLog& operator << (ACHAR Inof);

    CMsLog& operator << (WCHAR* strInof);   // �ַ���
    CMsLog& operator << (ACHAR* strInof);

    CMsLog& operator << (INT nInof);        // ����
    CMsLog& operator << (DWORD dwInof);

    CMsLog& operator << (INT64 n64Inof);    // ����
    CMsLog& operator << (UINT64 u64Inof);

    CMsLog& operator << (FLOAT fInof);      // ����
    CMsLog& operator << (DOUBLE dInof);

    CMsLog& operator << (CMSLOG_FLAG LogFlag);  // ����

    // ��־ʱ��,ָ������
    BOOL Time(DWORD dwLogIndex, BOOL bSaveLog);
    BOOL TimeW(DWORD dwLogIndex, BOOL bSaveLog);
    BOOL TimeA(DWORD dwLogIndex, BOOL bSaveLog);

    // ��־ʱ��,Ĭ������
    BOOL Time(VOID);
    BOOL TimeW(VOID);
    BOOL TimeA(VOID);

    // ��¼��־,ָ������
    BOOL Log(DWORD dwLogIndex, BOOL bSaveLog, TCHAR* strInof);
    BOOL LogW(DWORD dwLogIndex, BOOL bSaveLog, WCHAR* strInof);
    BOOL LogA(DWORD dwLogIndex, BOOL bSaveLog, ACHAR* strInof);

    // ��¼��־,Ĭ������
    BOOL Log(TCHAR* strInof);
    BOOL LogW(WCHAR* strInof);
    BOOL LogA(ACHAR* strInof);

    // ��¼��ʽ����־,ָ������
    BOOL Logf(DWORD dwLogIndex, BOOL bSaveLog, TCHAR* strInof, ...);
    BOOL LogfW(DWORD dwLogIndex, BOOL bSaveLog, WCHAR* strInof, ...);
    BOOL LogfA(DWORD dwLogIndex, BOOL bSaveLog, ACHAR* strInof, ...);
    BOOL Logf(DWORD dwLogIndex, BOOL bSaveLog, TCHAR* strInof, va_list ap);
    BOOL LogfW(DWORD dwLogIndex, BOOL bSaveLog, WCHAR* strInof, va_list ap);
    BOOL LogfA(DWORD dwLogIndex, BOOL bSaveLog, ACHAR* strInof, va_list ap);

    // ��¼��ʽ����־,Ĭ������
    BOOL Logf(TCHAR* strInof, ...);
    BOOL LogfW(WCHAR* strInof, ...);
    BOOL LogfA(ACHAR* strInof, ...);
    BOOL Logf(TCHAR* strInof, va_list ap);
    BOOL LogfW(WCHAR* strInof, va_list ap);
    BOOL LogfA(ACHAR* strInof, va_list ap);

    // ����Ĭ�ϴ洢��־������
    DWORD SetFileIndex(DWORD dwFileIndex)
    {
        DWORD dwTemp = m_dwFileIndex;
        m_dwFileIndex = dwFileIndex;
        return dwTemp;
    }
    DWORD GetFileIndex(VOID){return m_dwFileIndex;}

    // �����Ƿ񱾵ر�����־
    VOID  SetSaveLog(BOOL bSave){m_bSaveLog = bSave;}
    BOOL  GetSaveLog(VOID){return m_bSaveLog;}

    // �����Ƿ�������־
    VOID  SetEnableLog(BOOL bEnable){m_bEnable = bEnable;}
    BOOL  GetEnableLog(VOID){return m_bEnable;}

    // �����������
    TCHAR* SwitchError(
        DWORD dwErrorCode,
        BOOL bShow = FALSE
        );

    // ��ѯ������
    TCHAR* ShowLastError(BOOL bShow = FALSE);

    // �����������
    VOID Pause(BOOL bRepeatShow = FALSE);
    VOID UnPause(VOID);
    VOID RePause(VOID);

    // ����ǰ������ɫ
    E_FOREGOUND_TEXT_COLOR SetFontColor(
        E_FOREGOUND_TEXT_COLOR eColor,
        OUT IN BOOL& bfIntensity
        );

    // ��ʼ�����ͷ�
    BOOL Init(VOID);
    BOOL Release(VOID);

private:

    // ������־������
    inline BOOL SendLogW(DWORD dwLogIndex, WCHAR* szLog);
    inline BOOL SendLogA(DWORD dwLogIndex, ACHAR* szLog);
    inline BOOL SendLog(DWORD dwLogIndex, TCHAR* szLog);

    // ��ʾ��־
    inline BOOL ShowLogW(WCHAR* szLog);
    inline BOOL ShowLogA(ACHAR* szLog);
    inline BOOL ShowLog(TCHAR* szLog);

    // д�뱾����־
    inline BOOL WriteLogA(DWORD dwLogIndex, ACHAR* szLog);
    inline BOOL WriteLogW(DWORD dwLogIndex, WCHAR* szLog);
    inline BOOL WriteLog(DWORD dwLogIndex, TCHAR* szLog);

    // ������־������
    inline BOOL SendLogWPrivate(DWORD /*dwLogIndex*/, WCHAR* /*szLog*/);
    inline BOOL SendLogAPrivate(DWORD /*dwLogIndex*/, ACHAR* /*szLog*/);

    // ��ʾ��־
    inline BOOL ShowLogWPrivate(WCHAR* szLog);
    inline BOOL ShowLogAPrivate(ACHAR* szLog);

    // д�뱾����־
    inline BOOL WriteLogWPrivate(DWORD dwLogIndex, WCHAR* szLog);
    inline BOOL WriteLogAPrivate(DWORD dwLogIndex, ACHAR* szLog);

    static CMsConsole*  s_pCMsConsole;      // ��־����̨����
    static DWORD        s_dwLogCount;       // ��־ʵ������
    //static MsLock       s_Loglock;          // ������
    static MsLock       s_LogLock;          // ��־����
    MsLock              m_Loglock;          // ��־С��

    // ��Ա
    LOG_FILE_INFO*  m_pLogFileInfo;         // ��־����
    TCHAR           m_szLog[LOG_STR_MAX_LEN];
    DWORD           m_dwLogFileCount;       // ��־����
    DWORD           m_dwFileIndex;          // Ĭ����־����
    BOOL            m_bSaveLog;             // �Ƿ񱣴���־
    BOOL            m_QuondamValueSaveLog;  // ֮ǰ��־�Ƿ񱣴�
    BOOL            m_bEnable;              // �Ƿ�������־
    UINT64          m_u64TimeBegin;         // ��־��ʱ��ʼʱ��
    BOOL            m_bPause;               // �Ƿ�����ͣ�ȴ�״̬
    BOOL            m_bfIntensity;          // ����̨ǰ���Ƿ����
    BOOL            m_bLock;                // �Ƿ�ʹ����־��
};

//����������������������
#endif//#pragma once//��
//����������������������
