#include "stdafx.h"
#include "StringOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static map<string, ConfigParse*> s_files;
static ConfigParse* s_curFile = NULL;

const string g_StrSep = " - ";

#define ID_MAX_LEN 20
#define TEXT_MAX_LEN 4000
#define STRING_MAX_LEN (ID_MAX_LEN + TEXT_MAX_LEN + 3)

char g_StringTemp[STRING_MAX_LEN];

LRESULT CALLBACK FilesListBoxProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldFilesListBoxProc = NULL;

LRESULT CALLBACK StringsListBoxProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldStringsListBoxProc = NULL;

LRESULT CALLBACK SearchIdButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldSearchIdButtonProc = NULL;

LRESULT CALLBACK SearchTextButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldSearchTextButtonProc = NULL;

LRESULT CALLBACK AddStringButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldAddStringButtonProc = NULL;

LRESULT CALLBACK UpdateStringButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldUpdateStringButtonProc = NULL;

LRESULT CALLBACK SaveButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldSaveButtonProc = NULL;

string GetIdFromString(const string& str, const string& sep);
string GetTextFromString(const string& str, const string& sep);

void RefreshString(HWND hwnd, int idx);

enum MyUserID
{
    // �����˵�1: 0x8000 | 0x0100
    // (����)LOAD
    LOAD_RES_DIR = 0x8100,
    LOAD_CALLBACK_FUNC,
    // �����˵�N: 0x8000 | 0x0N00
    // ������Ӧ�ó��������λ����Ӻ���ӦMenuItem
};

bool MyMenuProc(int id)
{
    bool sync = false;
    // Ӧ�ó�����׼����, ��ʵ��ͬ����д.
#define APP_READY 0
    if (APP_READY) sync = true;

    bool ret = false;
    switch (id)
    {
    case LOAD_RES_DIR:
        ret = LoadResDir(sync);
        break;
    case LOAD_CALLBACK_FUNC:
#ifdef _DEBUG
        ret = LoadCallbackFunc(sync);
#endif
        break;
    default:
        break;
    }
    return ret;
}

bool ProcessFile(const char* dir, const char* name, bool sync)
{
    string file = string(dir) + '\\' + string(name);
    map<string, ConfigParse*>::iterator itr = s_files.find(name);
    if (itr != s_files.end())
    {
        if (IDOK == AppFrame::NotifyMessageBox("�ļ������б���, �Ƿ��ض�?",
            "����",
            MB_OKCANCEL))
        {
            delete itr->second;
        }
        else
        {
            return false;
        }
    }

    AppFrame::AddToList("files", name);
    ConfigParse* conf = new ConfigParse(file.c_str(), sync);
    if (!conf || !conf->IsLoaded())
    {
        delete conf;
        AddErrLog("���ļ�(%s)ʧ��, ErrorID: %d",
            file,
            GetLastError());
        return false;
    }

    s_files[name] = conf;
    return true;
}


#define XML_VERSION "1.0"
#define XML_ENCODING "gb2312"
#define XML_STANDALONE "no"

bool SaveFile(const char* filename)
{
    TiXmlDocument doc(filename);
    TiXmlElement tableElem("StringTable");
    TiXmlElement stringElem("String");

    TiXmlDeclaration declaration(XML_VERSION, XML_ENCODING, XML_STANDALONE);
    doc.InsertEndChild(declaration);

    HWND stringsListbox = AppFrame::GetWnd("strings");
    int count = SendMessage(stringsListbox, LB_GETCOUNT, 0, 0);
    string item, id, text;
    for (int idx = 0; idx < count; ++idx)
    {
        if( LB_ERR == SendMessage(stringsListbox, LB_GETTEXT, idx, (LPARAM)g_StringTemp))
            continue;

        item = g_StringTemp;
        id = GetIdFromString(item, g_StrSep);
        text = GetTextFromString(item, g_StrSep);

        stringElem.SetAttribute("id", id.c_str());
        stringElem.SetAttribute("text", text.c_str());
        tableElem.InsertEndChild(stringElem);
    }

    doc.InsertEndChild(tableElem);
    return doc.SaveFile();
}

bool ShowFile(const char* filename)
{
    AppFrame::ResetList("strings");
    HWND stringsWnd = AppFrame::GetWnd("strings");

    TiXmlDocument doc;
    if (!doc.LoadFile(filename))
    {
        return false;
    }

    for (TiXmlElement* elem = doc.FirstChildElement()->FirstChildElement();
        elem != NULL; elem = elem->NextSiblingElement())
    {
        const char* id = elem->Attribute("id");
        if (NULL == id)
        {
            continue;
        }

        const char* text = elem->Attribute("text");
        if (NULL == text)
        {
            continue;
        }

        string item;
        item = item + id + g_StrSep + text;
        SendMessage(stringsWnd, LB_ADDSTRING, (WPARAM)0, (LPARAM)item.c_str());
    }
    return true;
}

bool MyFilesProc(HWND handle, int index)
{
    static char text[MAX_PATH] = "";
    ::SendMessage(handle, LB_GETTEXT, index, (LPARAM)text);
    if (!ShowFile(text)) return false;
    AppFrame::SetEdit("id", "");
    AppFrame::SetEdit("text", "");
    return true;
}

bool MyStringsProc(HWND handle, int index)
{
    static char text[MAX_PATH] = "";
    s_curFile->SetCurNode(index);
    AppFrame::SetEdit("id", s_curFile->GetAttr("id"));
    AppFrame::SetEdit("text", s_curFile->GetAttr("text"));
    return true;
}

bool MyEditProc(HWND handle, int event)
{
    static char id[4096] = "";
    AppFrame::AddInfoLog("ID Event: %X", event);
    switch (event)
    {
    case EN_CHANGE:
        ::SendMessage(handle, WM_GETTEXT, MAX_PATH, (LPARAM)id);
        if (s_curFile && strcmp(s_curFile->GetAttr("id"), id))
        {
            s_curFile->SetAttr("id", id);
            UpdateStringItem();
        }
        break;
    default:
        break;
    }
    return true;
}

bool MyIdEditProc(HWND handle, int event)
{
    static char id[4096] = "";
    switch (event)
    {
    case EN_CHANGE:
        ::SendMessage(handle, WM_GETTEXT, MAX_PATH, (LPARAM)id);
        if (strcmp(s_curFile->GetAttr("id"), id))
        {
            s_curFile->SetAttr("id", id);
            UpdateStringItem();
        }
        break;
    default:
        break;
    }
    return true;
}

bool MyTextEditProc(HWND handle, int event)
{
    static char text[4096] = "";
    switch (event)
    {
    case EN_CHANGE:
        ::SendMessage(handle, WM_GETTEXT, 4096, (LPARAM)text);
        if (strcmp(s_curFile->GetAttr("text"), text))
        {
            s_curFile->SetAttr("text", text);
            UpdateStringItem();
        }
        break;
    default:
        break;
    }
    return true;
}

bool LoadResDir(bool sync)
{
    static char files[4096] = "";
    char dir[MAX_PATH] = "";
    ::GetCurrentDirectory(MAX_PATH, dir);
    uint offset = AppFrame::FileOpenDlg(files);
    ::SetCurrentDirectory(dir);
    AppFrame::LoadTextRes(false);
    if (!offset)
    {
        AppFrame::AddWarnLog("�޷�����Դ�ļ�");
        return false;
    }

    ///���ѡȡ����ļ��������ַ����ĸ�ʽΪ��·�� + NULL + �ļ���1 + NULL + �ļ���2 + NULL + ... + �ļ���n + NULL
    ///���ѡȡһ���ļ��������ַ����ĸ�ʽΪ��·�� + "\\" + �ļ��� + NULL
    ///Ϊ��ʹ�������������һ�£����������ѡȡһ���ļ����ɵ��ַ�����"\\"��Ϊ"NULL"
    ///[gr]
    files[offset-1] = NULL;

    const char* file = files + offset;

    while (*file)
    {
        AppFrame::AddStdLog("%s\\%s", files, file);

        //ProcessFile(files, file, sync);

        //string filename = files;
        //filename = filename + "\\" + file;

        //if (GetStrMgr()->IsFileExist(filename))
        //{
        //    if (IDOK == AppFrame::NotifyMessageBox("�ļ������б���, �Ƿ��ض�?", "����", MB_OKCANCEL))
        //    {
        //        GetStrMgr()->DelFile(filename);
        //    }
        //    else
        //    {
        //        file += strlen(file) + 1;
        //        continue;
        //    }
        //}

        //if (GetStrMgr()->ReadFile(filename.c_str()))
        //{
        //    AppFrame::AddToList("files", filename);
        //}

        string filename = files;
        filename = filename + "\\" + file;
        AppFrame::AddToList("files", filename);
        file += strlen(file) + 1;
    }

    AppFrame::AddStdLog("�ɹ�����Դ�ļ�");
    return true;
}

bool LoadCallbackFunc(bool sync)
{
    AppFrame::RegisterMenuProc(MyMenuProc);
    HWND filesWnd = AppFrame::GetWnd("files");
    oldFilesListBoxProc = (WNDPROC)SetWindowLong(filesWnd, GWL_WNDPROC, (LONG)FilesListBoxProc);

    HWND stringWnd = AppFrame::GetWnd("strings");
    oldStringsListBoxProc = (WNDPROC)SetWindowLong(stringWnd, GWL_WNDPROC, (LONG)StringsListBoxProc);

    HWND searchIdButton = AppFrame::GetWnd("searchId");
    oldSearchIdButtonProc = (WNDPROC)SetWindowLong(searchIdButton, GWL_WNDPROC, (LONG)SearchIdButtonProc);

    HWND searchTextButton = AppFrame::GetWnd("searchText");
    oldSearchTextButtonProc = (WNDPROC)SetWindowLong(searchTextButton, GWL_WNDPROC, (LONG)SearchTextButtonProc);

    HWND addStringButton = AppFrame::GetWnd("addString");
    oldAddStringButtonProc = (WNDPROC)SetWindowLong(addStringButton, GWL_WNDPROC, (LONG)AddStringButtonProc);

    HWND updateStringButton = AppFrame::GetWnd("updateString");
    oldUpdateStringButtonProc = (WNDPROC)SetWindowLong(updateStringButton, GWL_WNDPROC, (LONG)UpdateStringButtonProc);

    HWND saveButton = AppFrame::GetWnd("save");
    oldSaveButtonProc = (WNDPROC)SetWindowLong(saveButton, GWL_WNDPROC, (LONG)SaveButtonProc);

    AppFrame::AddStdLog("���ػص������ɹ�");
    return true;
}

void UpdateStringItem()
{
    HWND handle = AppFrame::GetWnd("strings");
    int index = (int)::SendMessage(handle, LB_GETCURSEL, 0, 0);
    s_curFile->SetCurNode(index);
    int id = 0;
    s_curFile->GetAttr("id", &id);
    string text = s_curFile->GetAttr("text");
    static char item[4096] = "";
    sprintf(item, "%d: %s", id, text.c_str());
    ::SendMessage(handle, LB_DELETESTRING, index, 0);
    ::SendMessage(handle, LB_INSERTSTRING, index, (LPARAM)item);
    ::SendMessage(handle, LB_SETCURSEL, index, 0);
}

bool ListFiles()
{
    bool ret = false;

    return ret;
}

bool ReleaseFiles()
{
    map<string, ConfigParse*>::iterator itr = s_files.begin();
    for (; itr != s_files.end(); ++itr)
    {
        if (itr->second->Save())
        {
            AppFrame::AddStdLog("�����ļ� %s �ɹ�", itr->first.c_str());
        }
        delete itr->second;
    }

    AppFrame::AddStdLog("��Դ�ļ��ͷ����");
    return true;
}

bool Init()
{
    // ע�� WM_COMMAND �Զ��崦����
    LoadCallbackFunc();
    static char filter []= "�ַ�����Դ (*.xml)\0*.xml\0�����ļ� (*.*)\0*.*\0\0";
    AppFrame::FileOpInit(filter);
    return true;
}


bool Fini()
{
    ReleaseFiles();
    return true;
}

unsigned __stdcall MainThreadFunc(void* arg)
{
    using namespace AppFrame;
#ifndef _DEBUG
    __try
    {
#endif
        bool ret = Init();
        while (ret)
        {
            // �����߳�
            if (IsExit() /*|| !GetGame().MainLoop()*/
                )
            {
                GetLog() << Insane << "== �߳��˳� ==" << AppFrame::Endl;
                break;
            }
            Sleep(1);
        }

        Fini();

#ifndef _DEBUG
    }
    __except(_Sword3::CrashFilter(GetExceptionInformation(),
        GetExceptionCode()))
    {
        NotifyMessageBox("������ִ�����鿴���µĴ��󱨸��ļ�");
        AddLogText(AppFrame::Error, "������ִ�����鿴���µĴ��󱨸��ļ�");

        Fini();
        Sleep(5000);
        ExitThread();
        return 0;
    }
#endif

    ExitThread();
    return 0;
}

LRESULT CALLBACK FilesListBoxProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_DELETE:
                {
                    //finds current string
                    HWND stringsListBox = AppFrame::GetWnd("files");
                    int idx = SendMessage(stringsListBox, LB_GETCURSEL, 0, 0);
                    if (LB_ERR == idx)
                    {
                        break;
                    }

                    //delete the string
                    SendMessage(stringsListBox, LB_DELETESTRING, idx, 0);

                    //sets current string
                    int count = SendMessage(stringsListBox, LB_GETCOUNT, 0, 0);
                    if (idx == count)
                    {
                        idx -= 1;
                    }
                    SendMessage(stringsListBox, LB_SETCURSEL, idx, 0);
                    break;
                }
            }
            // not break, go on WM_LBUTTONUP
        }
    case WM_LBUTTONUP:
        {
            int idx = SendMessage(hwnd, LB_GETCURSEL, 0, 0);
            if (LB_ERR == idx)
            {
                AppFrame::ResetList("strings");
                break;
            }

            SendMessage(hwnd, LB_GETTEXT, idx, (LPARAM)g_StringTemp);

            if (!ShowFile(g_StringTemp))
            {
                break;
            }

            AppFrame::SetEdit("id", "");
            AppFrame::SetEdit("text", "");
            break;
        }
    }
    return CallWindowProc(oldStringsListBoxProc, hwnd, message, wParam, lParam);
}

void RefreshString(HWND hwnd, int idx)
{
    const char* id = "";
    const char* text = "";

    if (LB_ERR == idx)
    {
        idx = SendMessage(hwnd, LB_GETCURSEL, 0, 0);
    }
    else
    {
        static char content[4096];
        SendMessage(hwnd, LB_GETTEXT, idx, (LPARAM)content);
        char* sep = strstr(content, g_StrSep.c_str());
        if (sep)
        {
            sep[0] = '\0';
            id = content;
            text = sep + g_StrSep.length();
        }
    }

    AppFrame::SetEdit("id", id);
    AppFrame::SetEdit("text", text);
}

LRESULT CALLBACK StringsListBoxProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_DELETE:
                {
                    //finds current string
                    int idx = SendMessage(hwnd, LB_GETCURSEL, 0, 0);
                    if (LB_ERR == idx)
                    {
                        break;
                    }

                    //delete the string
                    SendMessage(hwnd, LB_DELETESTRING, idx, 0);
                    int count = SendMessage(hwnd, LB_GETCOUNT, 0, 0);
                    if (count == 0)
                    {
                        idx = LB_ERR;
                    }
                    else if (count == idx)
                    {
                        idx -= 1;
                    }
                    SendMessage(hwnd, LB_SETCURSEL, idx, 0);
                    RefreshString(hwnd, idx);
                }
                break;
            case VK_DOWN:
                {
                    int idx = SendMessage(hwnd, LB_GETCURSEL, 0, 0) + 1;
                    int count = SendMessage(hwnd, LB_GETCOUNT, 0, 0);
                    if (idx >= count) idx = count - 1;
                    RefreshString(hwnd, idx);
                }
                break;
            case VK_UP:
                {
                    int idx = SendMessage(hwnd, LB_GETCURSEL, 0, 0);
                    idx = (idx == LB_ERR || idx == 0) ? 0 : idx - 1;
                    RefreshString(hwnd, idx);
                }
                break;
            case VK_HOME:
                {
                    RefreshString(hwnd, 0);
                }
                break;
            case VK_END:
                {
                    int count = SendMessage(hwnd, LB_GETCOUNT, 0, 0);
                    RefreshString(hwnd, count - 1);
                }
                break;
            }
            break;
        }

    case WM_LBUTTONUP:
        {
            int idx = SendMessage(hwnd, LB_GETCURSEL, 0, 0);
            RefreshString(hwnd, idx);
        }
        break;
    }
    return CallWindowProc(oldStringsListBoxProc, hwnd, message, wParam, lParam);
}

string GetIdFromString(const string& str, const string& sep)
{
    int idx = str.find(sep);
    if (string::npos != idx)
    {
        return string(str, 0, idx);
    }
    else
    {
        return string();
    }
}

string GetTextFromString(const string& str, const string& sep)
{
    int idx = str.find(sep);
    if (string::npos != idx)
    {
        return string(str, idx+sep.length(), str.length());
    }
    else
    {
        return string();
    }
}

LRESULT CALLBACK SearchIdButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HOTKEY:
    case WM_LBUTTONUP:
        {
            //gets text from search edit control
            HWND searchEdit = AppFrame::GetWnd("search");
            SendMessage(searchEdit, WM_GETTEXT, ID_MAX_LEN, (LPARAM)g_StringTemp);

            //selects an item contains the text as prefix
            HWND stringsListBox = AppFrame::GetWnd("strings");
            SendMessage(stringsListBox, LB_SETCURSEL, -1, 0);
            SendMessage(stringsListBox, LB_SELECTSTRING, -1, (LPARAM)g_StringTemp);
            break;
        }
    }
    return CallWindowProc(oldSearchIdButtonProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK SearchTextButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HOTKEY:
    case WM_LBUTTONUP:
        {
            //gets text from search edit control
            HWND searchEdit = AppFrame::GetWnd("search");
            SendMessage(searchEdit, WM_GETTEXT, TEXT_MAX_LEN, (LPARAM)g_StringTemp);
            if (0 == strlen(g_StringTemp))
            {
                break;
            }
            string subStr = g_StringTemp;

            HWND stringsListBox = AppFrame::GetWnd("strings");
            int count = SendMessage(stringsListBox, LB_GETCOUNT, 0, 0);
            int start = SendMessage(stringsListBox, LB_GETCURSEL, 0, 0);
            SendMessage(stringsListBox, LB_SETCURSEL, -1, 0);
            int idx;
            string item;
            for (int i = 0; i < count; ++i)
            {
                idx = (start + i + 1) % count;//the search starts from the next item
                SendMessage(stringsListBox, LB_GETTEXT, idx, (LPARAM)g_StringTemp);
                item = GetTextFromString(g_StringTemp, g_StrSep);
                if (string::npos != item.find(subStr))
                {
                    SendMessage(stringsListBox, LB_SETCURSEL, idx, 0);
                    break;
                }
            }
            break;
        }
    }
    return CallWindowProc(oldSearchTextButtonProc, hwnd, message, wParam, lParam);
}


LRESULT CALLBACK AddStringButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HOTKEY:
    case WM_LBUTTONUP:
        {
            //gets id
            HWND idEdit = AppFrame::GetWnd("id");
            SendMessage(idEdit, WM_GETTEXT, ID_MAX_LEN, (LPARAM)g_StringTemp);
            if (0 == strlen(g_StringTemp))
            {
                MessageBox(hwnd, "Please input the id!", "warning", MB_OK);
                break;
            }
            string item = g_StringTemp;

            //the id cannot exists in the listbox
            item += g_StrSep;
            HWND stringsListBox = AppFrame::GetWnd("strings");
            if (LB_ERR != SendMessage(stringsListBox, LB_SELECTSTRING, -1, (LPARAM)item.c_str()))
            {
                MessageBox(hwnd, "This id exists already!", "warning", MB_OK);
                break;
            }

            //get text
            HWND textEdit = AppFrame::GetWnd("text");
            SendMessage(textEdit, WM_GETTEXT, TEXT_MAX_LEN, (LPARAM)g_StringTemp);
            if (0 == strlen(g_StringTemp))
            {
                MessageBox(hwnd, "Please input the text!", "warning", MB_OK);
                break;
            }
            item += g_StringTemp;

            //puts the item into the listbox
            int idx = SendMessage(stringsListBox, LB_ADDSTRING, 0, (LPARAM)item.c_str());
            SendMessage(stringsListBox, LB_SETCURSEL, idx, 0);
            break;
        }
    }
    return CallWindowProc(oldAddStringButtonProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK UpdateStringButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HOTKEY:
    case WM_LBUTTONUP:
        {
            //gets id
            HWND idEdit = AppFrame::GetWnd("id");
            SendMessage(idEdit, WM_GETTEXT, ID_MAX_LEN, (LPARAM)g_StringTemp);
            if (0 == strlen(g_StringTemp))
            {
                MessageBox(hwnd, "Please input the id!", "warning", MB_OK);
                break;
            }
            string item = g_StringTemp;

            //the id must exists in the listbox
            item += g_StrSep;
            HWND stringsListBox = AppFrame::GetWnd("strings");
            int idx = SendMessage(stringsListBox, LB_SELECTSTRING, -1, (LPARAM)item.c_str());
            if (LB_ERR == idx)
            {
                MessageBox(hwnd, "This id does not exists!", "warning", MB_OK);
                break;
            }

            //get text
            HWND textEdit = AppFrame::GetWnd("text");
            SendMessage(textEdit, WM_GETTEXT, TEXT_MAX_LEN, (LPARAM)g_StringTemp);
            if (0 == strlen(g_StringTemp))
            {
                MessageBox(hwnd, "Please input the text!", "warning", MB_OK);
                break;
            }
            item += g_StringTemp;

            //update the item in the listbox
            SendMessage(stringsListBox, LB_DELETESTRING, idx, 0);
            idx = SendMessage(stringsListBox, LB_ADDSTRING, 0, (LPARAM)item.c_str());
            SendMessage(stringsListBox, LB_SETCURSEL, idx, 0);
            break;
        }
    }
    return CallWindowProc(oldUpdateStringButtonProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK SaveButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HOTKEY:
    case WM_LBUTTONUP:
        {
            HWND filesListbox = AppFrame::GetWnd("files");
            int idx = SendMessage(filesListbox, LB_GETCURSEL, 0, 0);
            if (LB_ERR == idx)
                break;

            SendMessage(filesListbox, LB_GETTEXT, idx, (LPARAM)g_StringTemp);
            SaveFile(g_StringTemp);
        }
        break;
    }
    return CallWindowProc(oldSaveButtonProc, hwnd, message, wParam, lParam);
}
