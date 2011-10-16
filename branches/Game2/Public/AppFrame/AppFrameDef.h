/**
* @file AppFrameDef.h
* 
* ʹ�ø�ģ���д����������������, ���� Win32App �����Ķ��Ƚ�С. ��Ҫ����������
* ���� (�ɲο�s/StringMan):
* 
* * ����ʹ�ö��߳�, ���޸� Windows ��Ϣѭ��;
* * �����ṩ�� debug ģ��;
* * ��������ʽʹ�� WS_DLGFRAME|WS_SYSMENU|WS_MINIMIZEBOX;
* * �Զ���ؼ���Ϣ���� (��Ҫ�� WndProc �����):
*       if (AppFrame::MsgProc((HWND)lParam, wmId, wmEvent))
*           return 0;
* * ���� WM_CREATE ��Ϣ (��Ҫ�� WndProc �����) ʱ, �����Զ���������Դ (�������߼����߳�):
*       InitConfig(hWnd);
* * ���� WM_QUIT ��Ϣ (��Ҫ����Ϣѭ�������) ʱ, �ͷ��Զ���������Դ (���˳��߼����߳�):
*       FiniConfig();
* * ������־����Ĵ�������������Ϊ"log", ���ڼ��״������������Ƽ�����Ϊ"info";
*
* ��Ƶĳ����ǽ�model, view �� contrl �ֿ�, �����������ݿ�ֱ��Ӧ���� Win32API,
* �����ýṹ�еĲ�������(�細�ڲ���)��Ϊ����. �����Ŀ�����Ҫ��Ϊ����˵������.
* 
* �����б�����, ��־��Ϣ�ȽϷ���, ���ض�����UI����ʱ������Ϣ�������¼���. ��
* release �汾���ض�����UI���ֵĲ˵� '��ͼ' ������.
* @author Fox (xiangyunfeng@svn.com)
* @date Mar.15, 2010
*/

#pragma once

//#include "Log.h"
#include "TextRes.h"
#include "../DefType.h"

namespace AppFrame
{
    const char* const LIST = "list";
    const char* const INFO = "info";
    const char* const LOG = "log";
    // �˴���ģ���а����ĺ�����ǰ������:
    BOOL                InitApp(HINSTANCE, int, unsigned (__stdcall *)(void*));
    /*ATOM				MyRegisterClass(HINSTANCE hInstance);
    BOOL				InitInstance(HINSTANCE, int);
    LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
    LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);*/

    /**
    * �˵�ID(16-bit)���ԭ��:
    * IDM = MY_MENUS | PARENT_INDEX << 5 | ITEM_INDEX
    * �����Զ���˵���ʼIDΪ MY_MENU, PARENT_INDEX (zero-based),
    * ITEM_INDEX (zero-based). Ϊʵ�ֿ��̴߳���, �Բ�ͬλ��ʶ�˵���
    * �˵���, ���������� 31 �������˵� (id0), ÿ�������˵����ӵ
    * �� 32 ���˵��� (id1).
    *
    * ����:
    * id0 == 1 �������˵���Ӧ'��ͼ'�˵�, ���뱣��.
    */

    /// ���������ļ����ɲ˵� ID
    /// @param id0 �����˵� ID
    /// @param id1 �˵��� ID
#define GenMenuId(id0, id1) ((((id0) << 5)) | (id1))

    /// ��ʶ�˵���С ID
#define MY_MENU 0x0020

    /// ���ݵ�ѡ�˵���ȷ�������˵� ID
    /// ע��: �Զ��������˵� ID �� 2 ��ʼ, �˴���ȡ ID �� 0 ��ʼ,
    /// �Ա�֤�� menuOps �±��Ӧ
#define GetPopMenuId(id) (((id) >> 5) - 2)

    /// ���ݵ�ѡ�˵�ȷ�ϲ˵��� ID
    /// ע��: �� ID ��Ӧ�Զ���˵�ö�� (ÿ��ö��ֵ��ռ 1 λ)
#define GetMenuItemId(id) (0x0001 << ((id) & 0x001F))

    /**
    * ȷ��UI��ʽ��'��ͼ'�˵���������ʱ����
    */
    struct Menu
    {
        /// �˵����
        HMENU handle;

        /// �˵������ļ�����
        string file;
    };

    /**
    * ���ڲ���û��top-left����, ��ȥ����ʱ���鷳, ֻ���ݴ������ǰһ����λ�ò���.
    * ���������ڲ���һ��Ϊ: ����, ����, ��.
    */
    struct Layouts
    {
        struct Window
        {
            string name;
            HWND handle;
            bool callback;
            bool (*func)(HWND, int);
            int x;
            int y;
            int width;
            int height;
        };

        /// ���ڲ��������ļ�����
        string file;

        /// ���ڼ���, first: ��������, second: ���ھ��
        map<string, Window> windows;
    };

    typedef map<string, Layouts::Window>::iterator WndItr;

    /**
    * �����߳̿���, Ŀǰֻ֧��һ���߳� (��Ӧ�ó���������߼����߳�)
    */
    struct Thread
    {
        HANDLE handle;                      ///< �߳̾��
        HANDLE event;                       ///< �߳��¼�
        unsigned id;                        ///< �߼��߳�ID
        bool exit;                          ///< �߳��˳�
    };

    struct Configure
    {
        Thread thread;                      ///< ��Ӧ�ó���������߼��߳�
        HWND wnd;                           ///< Ӧ�ó��򴰿ھ��
        Menu menu;                          ///< Ӧ�ó���˵�
        HFONT font;                         ///< �Ӵ�������(����ʹ�õȿ�����)
        string log;                         ///< Ӧ�ó�����־�ļ�
        int frameWidth;
        int frameHeight;
        Layouts layouts;                    ///< Ӧ�ó��򴰿ڲ���
    };

    /**
    * ��ʼ��������Դ��ע���߼����̺߳���
    *  @param wnd Ӧ�ó��������ھ��
    *  @param func ԭ��: unsigned __stdcall MainThreadFunc(void* args);
    */
    bool InitConfig(HWND wnd);

    /**
    * �߼����߳��˳�, �ͷ�������Դ
    */ 
    void FiniConfig();

    /**
    * ��������������Դ (����, ��־, �˵���)
    * @param sync �Ƿ�ͬ������
    */ 
    static bool LoadAll(bool sync = true);

    /**
    * ���ز˵� (���ڲ˵���������˵�ID�Ĺ���, �ݲ�֧���ȼ���)
    */
    static bool LoadMenus(bool sync = true);

    /// ����/���ò˵�
    /// @params id �����˵�ID, Ϊ -1 ��������в˵�
    /// @return ��Ӧ�˵���ǰ״̬: true -- ����; false -- ����
    bool ToggleMenus(int id = -1);

    /**
    * ���ش��ڲ���
    */
    static bool LoadLayout(bool sync = true);

    /**
    * ���ô��ڱ���
    * @param check ���ͬ��Ӧ���Ƿ��Ѵ���, ������, ���˳�
    */
    bool SetTitle(const string& title, bool check = true);
    const char* GetTitle(char* title);

    /**
    * ������־�ļ�
    */
   // static bool SetLog(LogLevel level);

    /// ��������
    static bool SetFont(const string& name = "");

    /// ����ͼ��
    static bool SetIcon(const string& bi, const string& si);

    /**
    * WM_COMMAND��Ϣ����, ���˵���Ӧ�����ڿؼ���Ӧ.
    * ͨ�� RegisterMenuProc ע��˵��Զ��崦���� [event == 0];
    * ͨ�� RegisterWndProc ע�ᴰ���Զ��崦���� [id == 0].
    */
    bool MsgProc(HWND handle, int id, int event);

    /// ��ȡ�������ƵĴ��ھ��, Ĭ�ϻ�nameΪ���򷵻������ھ��
    HWND GetWnd(const string& name = "");

    /**
    * ��ȡ��������Ĵ��ڶ���
    */
    Layouts::Window* GetWnd(HWND handle);

    bool IsCallback(HWND handle);

    /**
    * ע�ᴰ���Զ��崦����.
    * @param func ������MsgProc
    */
    void RegisterWndProc(const string& name, bool (*func)(HWND, int));

    //void ShowLogs(const string& log);
    void ResetList(const string& name);
    void AddToList(const string& name, const string& item);
    void DelFromList(const string& name, const string& item);
    void SetEdit(const string& name, const string& text);

    //Log& GetLog();

    void Exit();
    bool IsExit();

    /**
    * ��ȡ�����ڵĳߴ�
    */
    const int GetFrameWidth();
    const int GetFrameHeight();

    /// �����̺߳�������
    void ExitThread();

    /**
    * Ϊ���ܹ�����������λ��, �Ժ����ʽ��װErrorInfo.
    */
#define ERRFMT  "%s@%s(%d) : %s"
#define ERRARGS __FUNCTION__, __FILE__, __LINE__

// #define AddErrLog(ID_FMT, ...) do {                                     \
//     const int maxLength = 4096;                                         \
//     char info[maxLength];                                               \
//     sprintf(info, AppFrame::GetText(ID_FMT),##__VA_ARGS__);             \
//     AppFrame::AddLog(AppFrame::Error, ERRFMT, ERRARGS, info); } while (0)
// 
//     void AddWarnLog(const char* id, ...);
//     void AddStdLog(const char* id, ...);
//     void AddInfoLog(const char* id, ...);
//     void AddDebugLog(const char* id, ...);
//     void AddLog(LogLevel type, const char* fmt, ...);

    /**
    * ����MessageBox
    */
    int NotifyMessageBox(const char* msg,
        const char* title = "INFO",
        uint flag = MB_OK);

    /// ���ݵ�ѡ�˵���ȷ�ϲ˵�����
    void SetMenuOp(uint id);

    /// �жϲ˵����Ƿ���Ҫ���� (�����)
    bool MenuClicked(uint id);

    /// ��ȡ��ǰ���в˵���������
    void GetMenuOps(uint* ops);
}; // namespace AppFrame