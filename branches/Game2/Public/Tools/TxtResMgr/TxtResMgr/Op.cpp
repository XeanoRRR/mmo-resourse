#include "stdafx.h"
#include "Op.h"
#include "../../../Common/TextResourceSetup.h"
#include <map>
#include <string>
#include <sstream>
using namespace std;

extern map<string, string> g_Prefixes;//type name -- prefix
extern map<string, string> g_Texts;//text id -- text

static const size_t tmpStrSize = 1023;
static char tmpStr[tmpStrSize+1];//1 for the '\0'
static const size_t idSize = 8;

static HWND saveBtn;

static HWND prefixList;
static HWND prefixEdit;

static HWND textList;
static HWND textEdit;

static HWND searchEdit;

//�����ַ���
string GetPrefix(const string& s)
{
    return string(s, 0, s.find_first_of('-'));
}
string GetDesc(const string& s)
{
    return string(s, s.find_first_of('-')+1, s.size());
}
string GetId(const string& s)
{
    return string(s, 0, s.find_first_of('-'));
}
string GetText(const string& s)
{
    return string(s, s.find_first_of('-')+1, s.size());
}

WNDPROC oldPrefixListProc;
LRESULT CALLBACK PrefixListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DELETE:
            size_t idx = SendMessage(hWnd, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                SendMessage(hWnd, LB_GETTEXT, idx, (LPARAM)tmpStr);
                SendMessage(hWnd, LB_DELETESTRING, idx, 0);
                DelPrefix(GetPrefix(tmpStr));
            }
        }
        break;
    default:
        break;
    }
    return CallWindowProc(oldPrefixListProc, hWnd, message, wParam, lParam);
}


WNDPROC oldTextListProc;
LRESULT CALLBACK TextListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DELETE:
            size_t idx = SendMessage(hWnd, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                SendMessage(hWnd, LB_GETTEXT, idx, (LPARAM)tmpStr);
                SendMessage(hWnd, LB_DELETESTRING, idx, 0);
                DelText(GetId(tmpStr));
            }
        }
        break;
    default:
        break;
    }
    return CallWindowProc(oldTextListProc, hWnd, message, wParam, lParam);
}


WNDPROC oldPrefixEditProc;
LRESULT CALLBACK PrefixEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            size_t len = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
            if (len == 0)
            {
                break;
            }
            if (len > tmpStrSize)
            {
                MessageBox(hWnd, "�ַ���̫��", "warning", MB_OK);
                break;
            }

            SendMessage(hWnd, WM_GETTEXT, tmpStrSize+1, (LPARAM)tmpStr);
            string str(tmpStr);
            size_t idx = str.find_first_of("-");

            if (idx == string::npos)
            {
                MessageBox(hWnd, "ǰ׺�������á�-������", "warning", MB_OK);
                break;
            }

            string prefix(str, 0, idx);
            string desc(str, idx+1, str.size());

            if (desc.empty())
            {
                MessageBox(hWnd, "��������������", "warning", MB_OK);
                break;
            }

            //�������ӵ�prefix�Ѿ����ڣ���鿴��ǰѡ�е�prefix��
            //��ͬ���޸ģ���ͬ����ʾ����
            if (IsPrefixExist(prefix))
            {
                idx = SendMessage(prefixList, LB_GETCURSEL, 0, 0);
                if (idx == LB_ERR)
                {
                    MessageBox(hWnd, "��ǰ׺�Ѿ�����", "warning", MB_OK);
                    break;
                }
                SendMessage(prefixList, LB_GETTEXT, idx, (LPARAM)tmpStr);
                if (prefix != GetPrefix(tmpStr))
                {
                    MessageBox(hWnd, "��ǰ׺�Ѿ�����", "warning", MB_OK);
                    break;
                }
                SendMessage(prefixList, LB_DELETESTRING, idx, 0);
            }

            idx = SendMessage(prefixList, LB_ADDSTRING, 0, (LPARAM)(prefix+"-"+desc).c_str());
            if (idx == LB_ERR)
            {
                MessageBox(hWnd, "���ʧ��", "warning", MB_OK);
                break;
            }
            if (idx == LB_ERRSPACE)
            {
                MessageBox(hWnd, "ϵͳ�ռ䲻�㣬���ʧ��", "warning", MB_OK);
                break;
            }
            SendMessage(prefixList, LB_SETCURSEL, idx, 0);
            AddPrefix(prefix, desc);
        }
        break;
    }
    return CallWindowProc(oldPrefixEditProc, hWnd, message, wParam, lParam);
}


WNDPROC oldTextEditProc;
LRESULT CALLBACK TextEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            size_t len = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
            if (len == 0)
            {
                break;
            }
            if (len > tmpStrSize)
            {
                MessageBox(hWnd, "�ַ���̫��", "warning", MB_OK);
                break;
            }

            SendMessage(hWnd, WM_GETTEXT, tmpStrSize+1, (LPARAM)tmpStr);
            string str(tmpStr);
            size_t idx = str.find_first_of("-");

            if (idx == string::npos)
            {
                MessageBox(hWnd, "ID���ı��á�-������", "warning", MB_OK);
                break;
            }

            string id(str, 0, idx);
            string text(str, idx+1, str.size());

            if (text.empty())
            {
                MessageBox(hWnd, "�������ı�����", "warning", MB_OK);
                break;
            }

            idx = SendMessage(prefixList, LB_GETCURSEL, 0, 0);
            if (idx == LB_ERR)
            {
                MessageBox(hWnd, "��ѡ��һ�����ǰ׺��", "warning", MB_OK);
                break;
            }

            SendMessage(prefixList, LB_GETTEXT, idx, (LPARAM)tmpStr);
            if (id.find(GetPrefix(tmpStr)) != 0)
            {
                MessageBox(hWnd, "ID��ǰ׺�������", "warning", MB_OK);
                break;
            }

            //�������ӵ�ID�Ѿ����ڣ���鿴��ǰѡ�е�ID��
            //��ͬ���޸ģ���ͬ����ʾ����
            if (IsIdExist(id))
            {
                idx = SendMessage(textList, LB_GETCURSEL, 0, 0);
                if (idx == LB_ERR)
                {
                    MessageBox(hWnd, "��ID�Ѿ�����", "warning", MB_OK);
                    break;
                }
                SendMessage(textList, LB_GETTEXT, idx, (LPARAM)tmpStr);
                if (id != GetId(tmpStr))
                {
                    MessageBox(hWnd, "��ID�Ѿ�����", "warning", MB_OK);
                    break;
                }
                SendMessage(textList, LB_DELETESTRING, idx, 0);
            }

            idx = SendMessage(textList, LB_ADDSTRING, 0, (LPARAM)(id+"-"+text).c_str());
            if (idx == LB_ERR)
            {
                MessageBox(hWnd, "���ʧ��", "warning", MB_OK);
                break;
            }
            if (idx == LB_ERRSPACE)
            {
                MessageBox(hWnd, "ϵͳ�ռ䲻�㣬���ʧ��", "warning", MB_OK);
                break;
            }
            SendMessage(textList, LB_SETCURSEL, idx, 0);
            AddText(id, text);
        }
        break;
    }
    return CallWindowProc(oldTextEditProc, hWnd, message, wParam, lParam);
}

WNDPROC oldSearchEditProc;
LRESULT CALLBACK SearchEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            size_t len = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
            if (len == 0)
            {
                break;
            }
            if (len > tmpStrSize)
            {
                MessageBox(hWnd, "�ַ���̫��", "warning", MB_OK);
                break;
            }

            SendMessage(hWnd, WM_GETTEXT, tmpStrSize+1, (LPARAM)tmpStr);
            string sub(tmpStr);
            size_t cnt = SendMessage(textList, LB_GETCOUNT, 0, 0);
            size_t idx = SendMessage(textList, LB_GETCURSEL, 0, 0);
            if (idx == LB_ERR)
                idx = 0;
            else
                idx += 1;//����һ�п�ʼ����
            size_t i=0;
            for (; i < cnt; ++i, ++idx)
            {
                if (idx == cnt)
                    idx = 0;
                SendMessage(textList, LB_GETTEXT, idx, (LPARAM)tmpStr);
                if (strstr(tmpStr, sub.c_str()) != NULL)
                {
                    SendMessage(textList, LB_SETCURSEL, idx, 0);
                    break;
                }
            }
            if (i == cnt)
                MessageBox(hWnd, "û�ҵ����ַ���", "warning", MB_OK);
            break;//case VK_RETURN
        }
        break;//case WM_KEYDOWN
    }
    return CallWindowProc(oldSearchEditProc, hWnd, message, wParam, lParam);
}

void FillTextList(const string& prefix)
{
    SendMessage(textList, LB_RESETCONTENT, 0, 0);

    map<string, string>::iterator pos = g_Texts.lower_bound(prefix);
    map<string, string>::iterator end = g_Texts.lower_bound(prefix+"~");
    for (; pos != end; ++pos)
    {
        SendMessage(textList, LB_ADDSTRING, 0, (LPARAM)(pos->first+"-"+pos->second).c_str());
    }
}

void FillPrefixList()
{
    for (map<string, string>::iterator it = g_Prefixes.begin();
        it != g_Prefixes.end(); ++it)
    {
        SendMessage(prefixList, LB_ADDSTRING, 0, (LPARAM)(it->first+"-"+it->second).c_str());
    }
}

void CreateSubWindows(HWND hWnd)
{
    prefixList = CreateWindow (TEXT ("listbox"), NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | 
        LBS_NOTIFY | LBS_SORT | LBS_USETABSTOPS,
        0, 0, 300, 600,
        hWnd, (HMENU) ID_PREFIXLIST,
        (HINSTANCE) GetWindowLong (hWnd, GWL_HINSTANCE),
        NULL) ;
    oldPrefixListProc = (WNDPROC)SetWindowLong(prefixList, GWL_WNDPROC, (LONG)PrefixListProc);

    textList = CreateWindow (TEXT ("listbox"), NULL,
        WS_CHILD | WS_VISIBLE | LBS_STANDARD,
        310, 40, 600, 560,
        hWnd, (HMENU) ID_TEXTLIST,
        (HINSTANCE) GetWindowLong (hWnd, GWL_HINSTANCE),
        NULL) ;
    oldTextListProc = (WNDPROC)SetWindowLong(textList, GWL_WNDPROC, (LONG)TextListProc);

    prefixEdit = CreateWindow (TEXT ("edit"), "ǰ׺+��-��+���������磺RW-����",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        0, 600, 300, 30,
        hWnd, (HMENU) ID_PREFIXEDIT,
        (HINSTANCE) GetWindowLong (hWnd, GWL_HINSTANCE),
        NULL);
    oldPrefixEditProc = (WNDPROC)SetWindowLong(prefixEdit, GWL_WNDPROC, (LONG)PrefixEditProc);

    textEdit = CreateWindow (TEXT ("edit"), "id+��-��+�ı������磺RW010300-���Ϻ�",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
        310, 600, 600, 80,
        hWnd, (HMENU) ID_TEXTEDIT,
        (HINSTANCE) GetWindowLong (hWnd, GWL_HINSTANCE),
        NULL);
    oldTextEditProc = (WNDPROC)SetWindowLong(textEdit, GWL_WNDPROC, (LONG)TextEditProc);

    searchEdit = CreateWindow (TEXT ("edit"), "Google",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        310, 0, 600, 30,
        hWnd, (HMENU) ID_SEARCHEDIT,
        (HINSTANCE) GetWindowLong (hWnd, GWL_HINSTANCE),
        NULL);
    oldSearchEditProc = (WNDPROC)SetWindowLong(searchEdit, GWL_WNDPROC, (LONG)SearchEditProc);

    saveBtn = CreateWindow (TEXT ("button"), TEXT ("����"),
        WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
        0, 650, 60, 30,
        hWnd, (HMENU) ID_SAVEBTN,
        (HINSTANCE) GetWindowLong (hWnd, GWL_HINSTANCE),
        NULL);

    LoadTextResourceSetup();
    FillPrefixList();
}

void OnPrefixListSelectChange()
{
    size_t idx = SendMessage(prefixList, LB_GETCURSEL, 0, 0);
    if (idx != LB_ERR)
    {
        SendMessage(prefixList, LB_GETTEXT, idx, (LPARAM)tmpStr);
        SendMessage(prefixEdit, WM_SETTEXT, 0, (LPARAM)tmpStr);
        FillTextList(GetPrefix(tmpStr));
    }
}

void OnTextListSelectChange()
{
    size_t idx = SendMessage(textList, LB_GETCURSEL, 0, 0);
    if (idx != LB_ERR)
    {
        SendMessage(textList, LB_GETTEXT, idx, (LPARAM)tmpStr);
        SendMessage(textEdit, WM_SETTEXT, 0, (LPARAM)tmpStr);
    }
}