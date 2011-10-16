#include "stdafx.h"
#include "AppFrameDef.h"
#include "DateTime.h"

#include "../Package/ConfigParse.h"
#include "../Log/WinAppender.h"

#include <commctrl.h>
#include <stdlib.h>

#pragma warning(push)
#pragma warning(disable : 4311)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInst;
extern unsigned (__stdcall *mainThreadFunc)(void*);
extern void initializeWin32LogFactory();
namespace AppFrame
{
    /**
    * �˵�������
    */
    
    static uint volatile menuOps[MY_MENU];

    static Configure conf;
   // static Log* logger;

    const int MaxLogContentLen = 65536; 

    enum UserID
    {
        // (��ͼ)VIEW
        LOAD_ALL = MY_MENU,
        LOAD_LAYOUT,
        SET_FONT,
        LOAD_CALLBACK_FUNC,
    };

    bool InitConfig(HWND wnd)
    {
        initializeWin32LogFactory();
        Log4c::Init("log4cplus.properties");
        Log4c::Register(ROOT_MODULE,"");//all.log
        Log4c::Register(NET_MODULE,"Net");//net.log
        Log4c::Register(NET_MODULE,"Pet");//pet.log
        Log4c::Register(NET_MODULE,"Skill");//skill.log
        Log4c::Register(MAIL_MODULE,"Mail");//mail.log


        // ����LogĿ¼������־�ļ�
        ::CreateDirectory("Log", NULL);

        static int flag = false;
        conf.wnd = wnd;
        if (flag)
        {
            NotifyMessageBox("INFO_INITCONFIG_ONCE");
            return false;
        }

        flag = true;
        conf.menu.handle = ::CreateMenu();
        ::SetMenu(conf.wnd, conf.menu.handle);
        if (!LoadAll(false))
        {
            NotifyMessageBox("INFO_LOADCONFIG_FAILED");
            return false;
        }

        if (!mainThreadFunc)
        {
            Log4c::Warn(ROOT_MODULE,"�̺߳���ΪNULL");
            return false;
        }

        conf.thread.exit = false;
        conf.thread.handle = (HANDLE)_beginthreadex(NULL,
            0,
            mainThreadFunc,
            NULL,
            0,
            &conf.thread.id);
        conf.thread.event = CreateEvent(NULL, 0, 0, 0);
        if (conf.thread.handle == NULL)
        {
            Log4c::Warn(ROOT_MODULE,"�����߳�ʧ��, ErrorID:%d",errno); 
            return false;
        }

        srand((unsigned)time(NULL));
        random(100);

        return true;
    }

    void FiniConfig()
    {
       // if (logger)
       // {
            //logger->RegisteListener(NULL);
            //*logger << Insane << "== �߳��˳�׼�� ==" << Endl;
        Log4c::Trace(ROOT_MODULE,"== �߳��˳�׼�� ==");
        conf.thread.exit = true;                      // �߳��˳�
        ::WaitForSingleObject(conf.thread.event, INFINITE);
        ::CloseHandle(conf.thread.handle);

        //*logger << Insane << "== �߳��˳��ɹ� ==" << Endl;
        //*logger << "================== ϵͳ�ر� ==================" << Endl;

         Log4c::Trace(ROOT_MODULE,"== �߳��˳��ɹ� ==");
         Log4c::Trace(ROOT_MODULE,"================== ϵͳ�ر� ==================");
        ::DestroyMenu(conf.menu.handle);
      //  }
    }


    bool LoadAll(bool sync)
    {
        ConfigParse file("Settings/_All.xml", sync);
        if (!file.IsLoaded())
            return false;

        file.SetFirstChild();
        string title = file.GetAttr("title");
        ::SetWindowText(conf.wnd, title.c_str());
        int width = 0, height = 0;
        file.GetAttr("width", &width);
        file.GetAttr("height", &height);

        conf.frameHeight = height;
        conf.frameWidth = width;

        string font = file.GetAttr("font");
        int x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        int y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;
        ::SetWindowPos(conf.wnd, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
        if (!SetFont(font))
            return false;

           file.SetNextSibling();
//         conf.log = file.GetAttr(LOG);
//         int level = 0;
//         file.GetAttr("level", &level);
//         if (!SetLog((LogLevel)level))
//             return false;

        file.SetNextSibling();
        conf.layouts.file = file.GetAttr("layout");
        if (!LoadLayout(sync))
            return false;

        file.SetNextSibling();
        conf.menu.file = file.GetAttr("menu");
        if (!LoadMenus(sync))
            return false;

        file.SetNextSibling();
        string big_icon = file.GetAttr("bigicon");
        string small_icon = file.GetAttr("smallicon");
        if (!SetIcon(big_icon, small_icon))
            return false;

        Log4c::Trace(ROOT_MODULE,"== UI��Դ���سɹ� ==");
        return true;
    }

    bool LoadMenus(bool sync)
    {
        // ��ȡ�˵���Դ����
        Menu& menu = conf.menu;
        ConfigParse file(menu.file.c_str(), sync);

        if (!file.IsLoaded())
        {
            Log4c::Warn(ROOT_MODULE,"���ز˵�ʧ��, ErrorID:%d ",::GetLastError());
            return false;
        }

        // ���¼���֮ǰ��ɾ�����в˵�
        int count = ::GetMenuItemCount(menu.handle);
        for (int i = count - 1; i >= 0; --i)
        {
            ::DeleteMenu(menu.handle, i, MF_BYPOSITION);
        }

        HMENU menuParent = NULL;
        HMENU menuItem = NULL;
        int level = 0;
        uint style = MF_STRING;
        uint id = 0;
        uint id0 = -1;
        uint id1 = -1;
        uint idm = 0;
        string info;
        file.SetFirstChild();
        do
        {
            file.GetAttr("level", &level);
            info = file.GetAttr(INFO);
            replace(info.begin(), info.end(), '_', '&');
            if (level == 0)
            {
                id = ++id0;
                assert(id < MY_MENU && "�Ӳ˵���������");
                id1 = -1;
                style = MF_POPUP | MF_BYPOSITION | MF_GRAYED;
                menuItem = ::CreatePopupMenu();
                menuParent = menu.handle;
                idm = (uint)menuItem;
            }
            else if (level == 1)
            {
                id = ++id1;
                assert(id < MY_MENU && "�˵�����������");
                style = MF_STRING;
                menuParent = menuItem;
                idm = GenMenuId(id0 + 1, id1);
            }
            else     // Wrong item.
            {
                assert(0 && "�˵����ô���, level ֵ (0 or 1) �Ƿ�");
                break;
            }

            BOOL res = ::InsertMenu(menuParent,
                id,
                style,
                idm,
                info.c_str());
        } while (file.SetNextSibling());

        ::DrawMenuBar(conf.wnd);
        Log4c::Trace(ROOT_MODULE,"(Insan) ���ز˵���Դ�ɹ�");
        return true;
    }

    bool ToggleMenus(int id)
    {
#ifndef _DEBUG
        if (id == 0)
            return false;
#endif

        if (id == -1)
        {
            int count = ::GetMenuItemCount(conf.menu.handle);
            for (int i = count - 1; i >= 0; --i)
            {
                ToggleMenus(i);
            }
            return true;
        }

        uint grayed = ~(::GetMenuState(conf.menu.handle, id, MF_BYPOSITION)) & MF_GRAYED;
        uint style = MF_CHANGE | MF_BYPOSITION | grayed;
        ::EnableMenuItem(conf.menu.handle, id, style);
        ::DrawMenuBar(GetWnd());
        return true;
    }

    bool LoadLayout(bool sync)
    {
        Layouts& layouts = conf.layouts;
        WndItr itr = layouts.windows.begin();
        for ( ; itr != layouts.windows.end(); ++itr)
        {
            ::DestroyWindow(itr->second.handle);
        }
        layouts.windows.clear();
        ConfigParse file(layouts.file.c_str(), sync);

        if (!file.IsLoaded())
        {
            Log4c::Warn(ROOT_MODULE,"���ش��ڲ���ʧ��, ErrorID:",::GetLastError());
            return false;
        }
        string name = "", type = "", text = "";
        int style = 0, x = 0, y = 0, width = 0, height = 0;
        file.SetFirstChild();
        do
        {
            Layouts::Window wnd;
            wnd.func = NULL;
            wnd.name = file.GetAttr("name");
            file.GetAttr("callback", (int*)&wnd.callback);
            type = file.GetAttr("type");
            string s = file.GetAttr("style");
            style = strtol(s.c_str(), NULL, 0);
            file.GetAttr("x", &x);
            file.GetAttr("y", &y);
            file.GetAttr("width", &width);
            file.GetAttr("height", &height);
            wnd.x = x;
            wnd.y = y;
            wnd.height = height;
            wnd.width = width;
            text = file.GetAttr("text");
            // �б��
            if (type == "LISTBOX") 
            {
                style |= WS_VSCROLL|LBS_WANTKEYBOARDINPUT;
            }
            // ���б༭��
            else if (type == "EDIT" && style & ES_MULTILINE)
            {
                style |= WS_VSCROLL;
            }
            style |= WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP;
            size_t off = 0;
            // ���ÿ�ݼ�
            if (!text.empty())
            {
                off = text.find('_');
                if (off == -1)
                {
                    off = 0;
                }
                else
                {
                    text[off] = '&';
                    off += 1;
                }
            }
            wnd.handle = ::CreateWindow(type.c_str(),
                text.c_str(),
                style,
                x,
                y,
                width,
                height,
                conf.wnd,
                NULL,
                g_hInst,
                NULL);
            ::SendMessage(wnd.handle, WM_SETFONT, (WPARAM)conf.font, TRUE);
            if (off)
            {
                static int id = 0;
                int ret = RegisterHotKey(wnd.handle, ++id, MOD_ALT, text[off]);
                cout << ret;
            }
            layouts.windows[wnd.name] = wnd;
        } while (file.SetNextSibling());

        Log4c::Trace(ROOT_MODULE,"(Insan) ���ش��ڲ��ֳɹ�");
        return true;
    }

    bool SetTitle(const string& title, bool check)
    {
        HWND hWnd = ::FindWindow(NULL, title.c_str());
        if (hWnd != NULL)
        {
            AppFrame::NotifyMessageBox("��Ӧ�ó���ֻ����һ��ʵ������");
            
            return false;
        }

        ::SetWindowText(GetWnd(), title.c_str());
        return true;
    }

    const char* GetTitle(char* title)
    {
        ::GetWindowText(GetWnd(), title, 128);
        return title;
    }

 /*   bool SetLog(LogLevel level)
    {
        OStringStream ss;
        TimeValue t;
        GetCurTime(t);
        ss  <<"log\\"<< setw(4) << t.year << '_'
            << setw(2) << setfill('0') << t.mon << '_'
            << setw(2) << setfill('0') << t.mday << '_'
            << conf.log << ".log";

        logger = &GetLogger().AddLog(ss.str(), level);
        if (!logger) return false;

        logger->RegisteListener(ShowLogs);
        *logger << "== ��ʼ����������Դ ==" << Endl;
        *logger << Insane << "������־�ļ��ɹ�" << Endl;
        return true;
    }*/

    bool SetFont(const string& name)
    {
        CHOOSEFONT cf;
        LOGFONT lf;
        memset(&lf, 0, sizeof(lf));
        if (!name.empty())
        {
            lf.lfHeight = -13;
            lf.lfWeight = 400;
            lf.lfCharSet = GB2312_CHARSET;
            strcpy(lf.lfFaceName, name.c_str());
            conf.font = ::CreateFontIndirect(&lf);
        }
        else
        {
            // Initialize members of the CHOOSEFONT structure. 
            cf.lStructSize = sizeof(CHOOSEFONT);
            cf.hwndOwner = (HWND)NULL;
            cf.hDC = (HDC)NULL;
            cf.lpLogFont = &lf;
            cf.iPointSize = 0;
            cf.Flags = CF_SCREENFONTS;
            cf.rgbColors = RGB(0, 0, 0);
            cf.lCustData = 0L;
            cf.lpfnHook = (LPCFHOOKPROC)NULL;
            cf.lpTemplateName = (LPSTR)NULL;
            cf.hInstance = (HINSTANCE) NULL;
            cf.lpszStyle = (LPSTR)NULL;
            cf.nFontType = SCREEN_FONTTYPE;
            cf.nSizeMin = 0;
            cf.nSizeMax = 0;
            ::ChooseFont(&cf); 
            conf.font = ::CreateFontIndirect(cf.lpLogFont);
        }

        map<string, Layouts::Window>& windows = conf.layouts.windows;
        WndItr itr = windows.begin();
        for (; itr != windows.end(); ++itr)
        {
            ::SendMessage(itr->second.handle,
                WM_SETFONT,
                (WPARAM)conf.font, TRUE);
        }
        return true;
    }

    bool SetIcon(const string& bi, const string& si)
    {
        HICON b = (HICON)::LoadImage(NULL,
            bi.c_str(),
            IMAGE_ICON,
            0,
            0,
            LR_LOADFROMFILE);
        HICON s = (HICON)::LoadImage(NULL,
            si.c_str(),
            IMAGE_ICON,
            0,
            0,
            LR_LOADFROMFILE);
        ::SendMessage(conf.wnd, WM_SETICON, ICON_BIG, (LPARAM)b);
        ::SendMessage(conf.wnd, WM_SETICON, ICON_SMALL, (LPARAM)s);
        return true;
    }

    typedef bool (*WndFunc)(HWND, int);
    WndFunc GetWndFunc(HWND handle)
    {
        map<string, Layouts::Window>& windows = conf.layouts.windows;

        for (WndItr itr = windows.begin();
            itr != windows.end(); ++itr)
        {
            if (handle == itr->second.handle)
            {
                return itr->second.func;
            }
        }

        return NULL;
    }

    bool MsgProc(HWND handle, int id, int event)
    {
        if (!IsCallback(handle) && id < MY_MENU)
            return false;

        bool ret = false;
        if (event == 0)                // ����˵���button
        {
            bool sync = false;
            // Ӧ�ó�����׼����, ��ʵ��ͬ����д.
#define APP_READY 0
            if (APP_READY) sync = true;

            switch (id)
            {
            case 0:      // ����button
                {
                    WndFunc func = GetWndFunc(handle);
                    if (func)
                    {
                        ret = (*func)(handle, event);
                    }
                    else
                    {
                        ::SendMessage(handle, WM_COMMAND, LOAD_CALLBACK_FUNC, NULL);
                        //*logger << Insane << "�����Ӵ��ڻص�����, ������..." << Endl;
                        Log4c::Trace(ROOT_MODULE,"�����Ӵ��ڻص�����, ������..." );
                    }
                }
                break;

            case LOAD_ALL:
                ret = LoadAll(sync);
                ToggleMenus();
                break;

            case LOAD_LAYOUT:
                ret = LoadLayout(sync);
                break;

            case SET_FONT:
                ret = SetFont();
                break;

            default: // �Զ���˵�����
                SetMenuOp(id);
                break;
            }
        }

        else                                          // �����Ӵ����¼�
        {
            switch (event)
            {
            case EN_CHANGE:
                {
                    WndFunc func = GetWndFunc(handle);
                    if (func)
                    {
                        ret = (*func)(handle, event);
                    }
                }
                break;

            case LBN_SELCHANGE:
                {
                    WndFunc func = GetWndFunc(handle);
                    int index = (int)::SendMessage(handle, LB_GETCURSEL, 0, 0);
                    if (func)
                    {
                        ret = (*func)(handle, index);
                    }
                    else
                    {
                        NotifyMessageBox("�����¼��ػص�����");
                    }
                }
                break;

            default:
                break;
            }
        }

        return ret;
    }

    HWND GetWnd(const string& name)
    {
        if (name.empty())
            return conf.wnd;

        WndItr itr = conf.layouts.windows.find(name);
        if (itr == conf.layouts.windows.end())
            return NULL;

        return itr->second.handle;
    }

    Layouts::Window* GetWnd(HWND handle)
    {
        map<string, Layouts::Window>& windows = conf.layouts.windows;
        WndItr itr = windows.begin();
        for (; itr != windows.end(); ++itr)
        {
            if (handle == itr->second.handle)
            {
                return &itr->second;
            }
        }

        return NULL;
    }

    bool IsCallback(HWND handle)
    {
        Layouts::Window* window = GetWnd(handle);
        if (!window)
            return false;
        return window->callback;
    }

    void RegisterWndProc(const string& name, bool (*func)(HWND, int))
    {
        WndItr itr = conf.layouts.windows.find(name);
        if (itr == conf.layouts.windows.end())
        {
            Log4c::Warn(ROOT_MODULE,"δ�ҵ�����(%s)",name);
            return;
        }
        itr->second.func = func;
        Log4c::Trace(ROOT_MODULE,"ע�ᴰ��(%s)���¼���Ӧ�����ɹ�",name);
    }

//     Log& GetLog()
//     {
//         return *logger;
//     }

    const int GetFrameWidth() {
        return conf.frameWidth;
    }

    const int GetFrameHeight() {
        return conf.frameHeight;
    }

    void Exit()
    {
        conf.thread.exit = true;
    }

    bool IsExit()
    {
        return conf.thread.exit;
    }

    void ExitThread()
    {
        SetEvent(conf.thread.event);
        ::PostMessage(GetWnd(), WM_DESTROY, 0, 0);
        _endthreadex(0);
    }

    //void ShowLogs(const string& log)
    //{
    //    HWND wnd = GetWnd(LOG);
    //    static DWORD last = timeGetTime();
    //    if (timeGetTime() - last > 300000 ||         // ����ʱ����
    //        (::GetWindowTextLength(wnd) + sizeof("\r\n")+ log.size()) >= MaxLogContentLen)     // ���ݳ���64K
    //    {
    //        last = timeGetTime();
    //        ::SetWindowText(wnd, "");
    //    }
    //    static char strText[MaxLogContentLen];
    //    ::GetWindowText(wnd,strText,MaxLogContentLen);
    //    strcat(strText,"\r\n");
    //    strcat(strText,log.c_str());
    //    ::SetWindowText(wnd,strText);

    //  int nLine = ::SendMessage(wnd, EM_GETLINECOUNT, 0, 0);
  		//::SendMessage(wnd, EM_LINESCROLL, 0, nLine);

    //}

    void ResetList(const string& name)
    {
        HWND wnd = GetWnd(name);
        ::SendMessage(wnd, LB_RESETCONTENT, 0, 0);
    }

    void AddToList(const string& name, const string& item)
    {
        HWND wnd = GetWnd(name);
        ::SendMessage(wnd, LB_INSERTSTRING, -1, (LPARAM)item.c_str());
    }

    void DelFromList(const string& name, const string& item)
    {
        HWND wnd = GetWnd(name);
        uint pos = (uint)::SendMessage(wnd, LB_FINDSTRING, 0, (LPARAM)item.c_str());
        if (pos != LB_ERR)
        {
            ::SendMessage(wnd, LB_DELETESTRING, pos, 0);
        }
    }

    void SetEdit(const string& name, const string& text)
    {
        HWND wnd = GetWnd(name);
        ::SendMessage(wnd, WM_SETTEXT, FALSE, (LPARAM)text.c_str());
    }

//     void AddWarnLog(const char* id, ...)
//     {
//         static const int maxLength = 4096;            // log��Ϣ��󳤶�
//         static char info[maxLength];
// 
//         va_list va;
//         va_start(va, id);
//         vsprintf(info, GetText(id), va);
//         va_end(va);
// 
//         *logger << Warning << info << Endl;
//     }
// 
//     void AddStdLog(const char* id, ...)
//     {
//         if (!id || strlen(id) <= 0) return;
// 
//         static const int maxLength = 4096;            // log��Ϣ��󳤶�
//         static char info[maxLength];
// 
//         va_list va;
//         va_start(va, id);
//         vsprintf(info, GetText(id), va);
//         va_end(va);
// 
//         *logger << Standard << info << Endl;
//     }
// 
//     void AddInfoLog(const char* id, ...)
//     {
//         static const int maxLength = 4096;            // log��Ϣ��󳤶�
//         static char info[maxLength];
// 
//         va_list va;
//         va_start(va, id);
//         vsprintf(info, GetText(id), va);
//         va_end(va);
// 
//         *logger << Info << info << Endl;
//     }
// 
//     void AddDebugLog(const char* id, ...)
//     {
//         static const int maxLength = 4096;            // log��Ϣ��󳤶�
//         static char info[maxLength];
// 
//         va_list va;
//         va_start(va, id);
//         vsprintf(info, GetText(id), va);
//         va_end(va);
// 
//         *logger << Insane << info << Endl;
//     }
// 
//     void AddLog(LogLevel type, const char* fmt, ...)
//     {
//         static const int maxLength = 4096;            // log��Ϣ��󳤶�
//         static char info[maxLength];
// 
//         va_list va;
//         va_start(va, fmt);
//         vsprintf(info, fmt, va);
//         va_end(va);
// 
//         if (logger)
//             *logger << type << info << Endl;
//         else
//             ::PutDebugString(info);
//     }

    int NotifyMessageBox(const char* msg, const char* title, uint flag)
    {
       /* if (logger)
        {
            *logger << Warning << msg << Endl;
        }*/

        return ::MessageBox(conf.wnd, msg, title, flag);
    }

    void SetMenuOp(uint id)
    {
        menuOps[GetPopMenuId(id)] |= GetMenuItemId(id);
    }

    bool MenuClicked(uint id)
    {
        bool res = false;
        if (menuOps[GetPopMenuId(id)] & GetMenuItemId(id))
        {
            menuOps[GetPopMenuId(id)] ^= GetMenuItemId(id);
            res = true;
        }
        return res;
    }

    void GetMenuOps(uint* ops)
    {
    }
}

#pragma warning(pop)