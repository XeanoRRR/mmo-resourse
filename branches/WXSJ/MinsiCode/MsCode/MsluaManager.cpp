#include "Precompiled.h"
#include "MsLuaManager.h"

// LUA�ű�������ȫ��ָ��
CMsLuaManager* g_pCMsLuaManager = NULL;

CMsLuaManager::CMsLuaManager()
: m_hFilePath               (NULL)
, m_pExplainOrgan           (NULL)
, m_pLuaChangeNotify        (NULL)
, m_nThreadId               (0)
, m_hThread                 (NULL)
, m_IsChecking              (FALSE)
, m_lpLuaHookCallBackFun    (NULL)
{
    _ENTER_FUN_C;

    // ��ʼ��LUA�ļ���Ŀ¼
    ZeroMemoryArray(m_szTLuaFilePath);
    ZeroMemoryArray(m_strLuaFileName);
    ZeroMemoryArray(m_szALuaFilePath);
    ZeroMemoryArray(m_strLuaParameter);

    // ��ָ֪ͨ��ָ�򻺳���
    m_pLuaChangeNotify = (FILE_NOTIFY_INFORMATION*)m_pLuaChangeNotifyBuff;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}
CMsLuaManager::~CMsLuaManager(VOID)
{
    _ENTER_FUN_C;

    // �������ͷ���ֵ
    BOOL bRet = TRUE;

    // ֹͣ���LUA�ļ��仯�߳�
    bRet = this->StopCheckLuaFileIsChange();
    AssertEx(AL_NORMAL, bRet,_T("StopCheckLuaFileIsChangeʧ��,����ʧ��"));

    // ���Lua
    if (m_pExplainOrgan)
    {
        lua_close(m_pExplainOrgan);
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
}

// �����������������ļ�ʱ�����õ�֪ͨ����
BOOL CMsLuaManager::ProcessFile(CONST TCHAR* strFileName)
{
    _ENTER_FUN_C;

    // �����ҵ���LUA�ļ�
    LoadLuaFile(strFileName);

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

// ����������Ŀ¼ʱ�����õ�֪ͨ����
BOOL CMsLuaManager::ProcessPath(CONST TCHAR* /*strCurrentPath*/)
{
    return TRUE;
}

BOOL CMsLuaManager::Init(VOID)
{
    _ENTER_FUN_C;

    BOOL bRet = TRUE;

    // ��ʼ��Lua
    m_pExplainOrgan = lua_open();

    // ����Lua������
    luaL_openlibs(m_pExplainOrgan);

    // ���ò����ļ�����
    bRet = this->SetFindFileType(_T("lua"));
    AssertEx(AL_NORMAL, bRet, _T("���ò���lua�ļ�����ʧ��,"));

    // ע��ϵͳĬ��LTC����
    bRet = this->RegisterCFunToLua(NULL, 0);
    AssertEx(AL_NORMAL, bRet, _T("ע��ϵͳĬ��LTC����ʧ��,"));

    //if (bNeedWinApi)
    //{
    //    luaL_Reg reg;
    //    reg.func = open_dll;
    //    reg.name = "lua_open_dll";
    //    this->RegisterCFunToLua(reg);

    //    reg.func = get_procaddress;
    //    reg.name = "lua_get_procaddress";
    //    this->RegisterCFunToLua(reg);

    //    reg.func = CreateRect;
    //    reg.name = "lua_CreateRect";
    //    this->RegisterCFunToLua(reg);
    //}

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

// �����ű�����
VOID CMsLuaManager::EnableDebug(BOOL bEnable, lua_Hook lpLuaHookCallBackFun, DWORD dwDebugFlag, DWORD dwRunCount)
{
    if (bEnable)
    {
        lua_sethook(m_pExplainOrgan, lpLuaHookCallBackFun, dwDebugFlag, dwRunCount);
    }
    else
    {
        lua_sethook(m_pExplainOrgan, lpLuaHookCallBackFun, 0, 0);
    }
}

BOOL CMsLuaManager::LoadLuaFile(CONST TCHAR* strLuaFileName)
{
    _ENTER_FUN_C;

    TCharToAChar(strLuaFileName, m_strLuaFileName, MAX_PATH);

    INT nRet = TRUE;

    LUA_FUN_RET_VALUE pLuaRet(LUA_RET_STRING);

    if (strLuaFileName)
    {
        // ���нű�
        nRet = luaL_loadfile(m_pExplainOrgan, m_strLuaFileName);
        switch (nRet)
        {
        case LUA_OK:
            {
                luaL_dofile(m_pExplainOrgan, m_strLuaFileName);
            }break;
        case LUA_ERRSYNTAX:
            {
                // ��ȡ������Ϣ
                pLuaRet.m_String.m_str = 
                    lua_tolstring(m_pExplainOrgan, -1, &pLuaRet.m_String.m_Size);

                MESSAGEBOXA(pLuaRet.m_String.m_str);
                return FALSE;
            }break;
        case LUA_NOEXIST:
            {
                fAssertExA(AL_NORMAL, FALSE,"�ű��ļ�������\n%s", m_strLuaFileName);
            }break;
        default:
            {
                fAssertExA(AL_NORMAL, FALSE,"�ű��ļ�����ʧ��,δ֪����\n%s", m_strLuaFileName);
            }break;
        }
    }
    else
    {
        AssertEx(AL_NORMAL, m_strLuaFileName,_T("strLuaFileName����Ϊ��"));
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

INT CMsLuaManager::RunLuaFun(
    LUA_FUN_RET_VALUE* pLuaRet,
    DWORD dwParamCount,
    TCHAR* strLuaFunName,
    TCHAR* strLuaParameter,
    ...
    )
{
    // ��������б�
    va_list vaList;
    // ���ݹ̶�����Ѱ�Ҳ�����ַ
    va_start(vaList, strLuaParameter);
    return this->RunLuaFun_Va(pLuaRet, dwParamCount, strLuaFunName, strLuaParameter, vaList);
}

BOOL CMsLuaManager::RunLuaFun_Va(
    LUA_FUN_RET_VALUE* pLuaRet,
    DWORD dwParamCount,
    TCHAR* strLuaFunName,
    TCHAR* strLuaParameter,
    va_list vaList
    )
{
    _ENTER_FUN_C;

    // LUA�ű������������
    INT nLuaParameterCount = 0;

    if (strLuaFunName)
    {
        TCharToAChar(strLuaFunName, m_strLuaFileName, MAX_PATH);

        // ѹ��LUA��������
        lua_getglobal(m_pExplainOrgan, LUA_ERROR_DISPOSE_FUN_NAME);

        // ��Ҫ���õĺ���ѹ���ջ
        lua_getglobal(m_pExplainOrgan, m_strLuaFileName);
    }

    if (strLuaParameter)
    {
        TCharToAChar(strLuaParameter, m_strLuaParameter, MAX_PATH);
        ACHAR* lpParameter = m_strLuaParameter;

        // ���ݹ̶��������Һ�������
        //strFindNextParameter = strLuaLoadFunBuff;

        while(*lpParameter)
        {
            if('%' == *lpParameter)
            {
                lpParameter++;
                switch(*lpParameter)
                {
                case 's':
                    {
                        // ����%sȷ����һ������Ϊ�ַ�����
                        // �������Դ�ѹ���ջ
                        lua_pushstring(m_pExplainOrgan, va_arg(vaList, ACHAR*));

                        // �����������++
                        nLuaParameterCount++;
                    }break;
                case 'd':
                    {
                        // ����%sȷ����һ������Ϊ����
                        // �������Դ�ѹ���ջ
                        lua_pushinteger(m_pExplainOrgan, va_arg(vaList, INT));

                        // �����������++
                        nLuaParameterCount++;
                    }break;
                case 'f':
                    {
                        // ����%sȷ����һ������Ϊ������
                        // �������Դ�ѹ���ջ
                        lua_pushnumber(m_pExplainOrgan, va_arg(vaList, DOUBLE));

                        // �����������++
                        nLuaParameterCount++;
                    }break;
                case 'c':
                    {
                        // ����%sȷ����һ������Ϊ�ַ���
                        // �������Դ�ѹ���ջ
                        lua_pushinteger(m_pExplainOrgan, va_arg(vaList, ACHAR));

                        // �����������++
                        nLuaParameterCount++;
                    }break;
                case 'b':
                    {
                        // ����%sȷ����һ������Ϊ������
                        // �������Դ�ѹ���ջ
                        lua_pushboolean(m_pExplainOrgan, va_arg(vaList, BOOL));

                        // �����������++
                        nLuaParameterCount++;
                    }break;
                default:
                    {
                        AssertEx(AL_NORMAL, FALSE, _T("�޷�ʶ��Ĵ�������"));
                    }break;
                }
            }
            else
            {
                lpParameter++;
                continue;
            }
        }
    }
    // ������������б�
    va_end(vaList);

    // ���ú�����������������뷵��ֵ����
    INT nLuaCallRet = lua_pcall(
        m_pExplainOrgan,
        nLuaParameterCount,
        1,
        nLuaParameterCount + 1
        );

    switch(nLuaCallRet)
    {
    case LUA_OK:
        break;
    case LUA_YIELD:
        {
            AssertEx(AL_NORMAL, FALSE, _T("LUA_YIELD,"));
        }break;
    case LUA_ERRRUN:
        {
            AssertEx(AL_NORMAL, FALSE, _T("����ʱ����,"));
        }break;
    case LUA_ERRSYNTAX:
        {
            AssertEx(AL_NORMAL, FALSE, _T("�����LUA�﷨,"));
        }break;
    case LUA_ERRMEM:
        {
            AssertEx(AL_NORMAL, FALSE, _T("�ڴ�������,"));
        }break;
    case LUA_ERRERR:
        {
            fAssertExA(AL_NORMAL, FALSE, "�����д�������ʱ�����Ĵ���\n%s", lua_tostring(m_pExplainOrgan, -1));
        }break;
    default:
        {
            AssertEx(AL_NORMAL, FALSE, _T("δ֪LUA����,"));
        }break;
    }

    FOR_x_TO(i, dwParamCount)
    {
        switch(pLuaRet[i].m_eType)
        {
        case LUA_RET_INT:
            {
                // ��ȡ����ֵ
                pLuaRet[i].m_lLuaFunRet = lua_tointeger(m_pExplainOrgan, -1);
                lua_pop(m_pExplainOrgan, 1);
            }break;
        case LUA_RET_DOUBLE:
            {
                // ��ȡ����ֵ
                pLuaRet[i].m_dLuaFunRet = lua_tonumber(m_pExplainOrgan, -1);
                lua_pop(m_pExplainOrgan, 1);
            }break;
        case LUA_RET_BOOL:
            {
                // ��ȡ����ֵ
                pLuaRet[i].m_bLuaFunRet = lua_toboolean(m_pExplainOrgan, -1);
                lua_pop(m_pExplainOrgan, 1);
            }break;
        case LUA_RET_STRING:
            {
                // ��ȡ����ֵ
                pLuaRet[i].m_String.m_str = 
                    lua_tolstring(m_pExplainOrgan, -1, &pLuaRet->m_String.m_Size);
                lua_pop(m_pExplainOrgan, 1);
            }break;
        case LUA_RET_VOID:
        default:
            {
                // ��ȡ����ֵ
                pLuaRet[i].m_lpVoid = lua_touserdata(m_pExplainOrgan, -1);
                lua_pop(m_pExplainOrgan, 1);
            }break;
        }
    }
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

BOOL CMsLuaManager::SetLuaFileRootPath(TCHAR* strLuaFilePath)
{
    _ENTER_FUN_C;

    if (NULL == strLuaFilePath)
    {
        // ���ò����ļ�Ŀ¼
        SetRootPath();

        // ����Ŀ¼�����ļ�
        BrowsePath();

        // ����LUA�ļ��仯���
        BeginCheckLuaFileIsChange();

        // ��⴫������Ƿ�Ϊ��
        AssertEx(AL_NORMAL, m_szCurrentDirectory, _T("m_szCurrentDirectoryΪ��,���󴫲�"));

        // ��ȡ��·������
        SIZE_T sFilePathLen = _tcslen(m_szCurrentDirectory);

        // �����·�������Ƿ�Խ��
        AssertEx(AL_NORMAL, sFilePathLen < MAX_PATH, _T("��������,Խ��"));

        // ������·������Ա����
        _tcsncpy_s(
            m_szTLuaFilePath,
            MAX_PATH,
            m_szCurrentDirectory,
            sFilePathLen
            );
    }
    else
    {
        // ���ò����ļ�Ŀ¼
        this->SetRootPath(strLuaFilePath);

        // ����Ŀ¼�����ļ�
        this->BrowsePath();

        // ����LUA�ļ��仯���
        this->BeginCheckLuaFileIsChange();

        // ��⴫������Ƿ�Ϊ��
        AssertEx(AL_NORMAL, strLuaFilePath, _T("strLuaFilePathΪ��,���󴫲�"));

        // ��ȡ��·������
        SIZE_T sFilePathLen = _tcslen(strLuaFilePath);
        AssertEx(AL_NORMAL, sFilePathLen < MAX_PATH, _T("nFilePathLenԽ��,���󴫲�"));

        // ������·������Ա����
        _tcsncpy_s(m_szTLuaFilePath, MAX_PATH, strLuaFilePath, sFilePathLen);
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

// ��ʼ���LUA�ļ��Ƿ��иı�
BOOL CMsLuaManager::BeginCheckLuaFileIsChange(VOID)
{
    _ENTER_FUN_C;

    // ����LUA�ű��ļ�����߳�
    m_hThread = ::CreateThread(
        NULL,
        0,
        CheckLuaFileIsChangeThreadProc,
        (LPVOID)this,
        0,
        &m_nThreadId
        );

    AssertEx(AL_NORMAL, m_hThread, _T("����LUA�ű��ļ����Threadʧ��,����ʧ��"));

    // ������еı����λ
    m_IsChecking = TRUE;

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

// ֹͣ���LUA�ļ��Ƿ��иı�
BOOL CMsLuaManager::StopCheckLuaFileIsChange(VOID)
{
    _ENTER_FUN_C;

    // �������ͷ���ֵ
    BOOL bRet = TRUE;

    // ������еı�����
    m_IsChecking = FALSE;

    // ����߳��Ƿ��Ѿ�ֹͣ
    if(m_hThread && m_nThreadId)
    {
        // �ر��߳�
        bRet = ::CloseHandle(m_hThread);
        AssertEx(
            AL_NORMAL,
            bRet,
            _T("�ر�CheckLuaFileIsChangeThreadProc�߳�ʧ��,����ʧ��")
            );

        // ���߳�ID,�߳̾���ÿ�
        m_nThreadId = 0;
        m_hThread   = NULL;
    }
    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

// ע��C++������LUA
BOOL CMsLuaManager::RegisterCFunToLua(luaL_Reg& NewRegLTC)
{
    _ENTER_FUN_C;

    // ����C++����ע��
    lua_pushcfunction(m_pExplainOrgan, NewRegLTC.func);
    lua_setglobal(m_pExplainOrgan, NewRegLTC.name);

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

BOOL CMsLuaManager::RegisterCFunToLua(luaL_Reg* pRegLTC, UINT uRegLTCCount)
{
    _ENTER_FUN_C;

    BOOL bRet = TRUE;
    // ��ϵͳĬ��ע��LTC����
    if ((0 == uRegLTCCount) || (pRegLTC == NULL))
    {
        // ��ȡϵͳĬ��ע��LTC����ָ�������
        pRegLTC         = g_RegLTCArray;
        uRegLTCCount    = sizeof(g_RegLTCArray) / sizeof(luaL_Reg);
    }
    // �Զ���ע��LTC����
    else
    {
        // ��ȡ�Զ���ע��LTC����ָ�������
        pRegLTC         = pRegLTC;
        uRegLTCCount    = uRegLTCCount;
    }

    // ѭ����LTC����ע���������ע��
    while((pRegLTC->func != NULL) && (pRegLTC->name != NULL))
    {
        // ����Ƿ�ע��ʧ��
        bRet = this->RegisterCFunToLua(*pRegLTC);
        AssertEx(AL_NORMAL, bRet, _T("CTL����ע��ʧ��,����ʧ��"));
        pRegLTC++;
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}


BOOL CMsLuaManager::OnLuaFileIsChange(TCHAR* strLuaFileName)
{
    _ENTER_FUN_C;

    // ���¼���ָ���ı��LUA�ļ�
    TCHAR szLuaFilePath[MAX_PATH] = {0};

    if (strLuaFileName)
    {
        {
            _tcscpy_s(szLuaFilePath, MAX_PATH, m_szTLuaFilePath);

            INT nPathLen = (INT)_tcslen(szLuaFilePath);
            // ������һλ����'\\'
            if (szLuaFilePath[nPathLen - 1] != _T('\\') && 
                szLuaFilePath[nPathLen - 1] != _T('/'))
            {
                // ���һλ����Ϊ'\\'
                szLuaFilePath[nPathLen] = _T('\\');

                // ���һλ׷��һ��'\0'
                szLuaFilePath[nPathLen + 1] = _T('\0');
            }
        }

        {
            _tcscat_s(szLuaFilePath, MAX_PATH, strLuaFileName);
        }
        return this->LoadLuaFile(szLuaFilePath);
    }

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return FALSE;
}

DWORD WINAPI CheckLuaFileIsChangeThreadProc(LPVOID lpParameter)
{
    _ENTER_FUN_C;

    // �������ͷ���ֵ
    BOOL bRet = TRUE;

    // �򿪼��Ŀ¼,�õ�Ŀ¼�ľ��
    ((CMsLuaManager*)lpParameter)->m_hFilePath = CreateFile(
        ((CMsLuaManager*)lpParameter)->m_szTLuaFilePath,
        GENERIC_READ|GENERIC_WRITE,
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
        ); 

    // ���Ŀ¼�Ƿ�򿪳ɹ�
    AssertEx(
        AL_NORMAL,
        ((CMsLuaManager*)lpParameter)->m_hFilePath,
        _T("LUA�ű�Ŀ¼��ʧ��,")
        );

    DWORD dwBytesReturned = 0;        
    while(((CMsLuaManager*)lpParameter)->m_IsChecking)
    {
        if(::ReadDirectoryChangesW(

            // ��ص��ļ�·��
            ((CMsLuaManager*)lpParameter)->m_hFilePath, 

            // ���֪ͨ�ṹ��
            ((CMsLuaManager*)lpParameter)->m_pLuaChangeNotify,

            // ֪ͨ�ṹ���С
            sizeof(((CMsLuaManager*)lpParameter)->m_pLuaChangeNotifyBuff),

            // �Ƿ�����Ŀ¼
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME|   // ����ļ�����
            FILE_NOTIFY_CHANGE_DIR_NAME|
            FILE_NOTIFY_CHANGE_ATTRIBUTES|
            FILE_NOTIFY_CHANGE_SIZE|        // ����ļ���С�ı�
            FILE_NOTIFY_CHANGE_LAST_WRITE|  // ����ļ����д��ı�
            FILE_NOTIFY_CHANGE_LAST_ACCESS|
            FILE_NOTIFY_CHANGE_CREATION|    // ����ļ�����
            FILE_NOTIFY_CHANGE_SECURITY,
            &dwBytesReturned,
            NULL,
            NULL))
        {
            switch(((CMsLuaManager*)lpParameter)->m_pLuaChangeNotify->Action)
            {
            case FILE_ACTION_ADDED:
                {
                    //MESSAGEBOX(AL_SHOW_ONLY, _T("��������ļ�"));
                }break;
            case FILE_ACTION_REMOVED:
                {
                    //MESSAGEBOX(AL_SHOW_ONLY, _T("ɾ�����ļ�"));
                }break;
            case FILE_ACTION_MODIFIED:
                {
                    // ��ȡ�ļ�·���ַ�������
                    DWORD dwFileNameLength = (((CMsLuaManager*)lpParameter)->
                        m_pLuaChangeNotify->
                        FileNameLength)/(DWORD)sizeof(TCHAR);

                    // ����ļ�·���ַ�������
                    AssertEx(
                        AL_NORMAL,
                        (dwFileNameLength < MAX_PATH), _T("�ļ�·������Խ��,"));

                    // ���ݷ��ص��ļ�·�����Ƚض��ַ���
                    ((CMsLuaManager*)lpParameter)->
                        m_pLuaChangeNotify->FileName[dwFileNameLength] = '\0';

                    // ���ַ�ת���ɶ��ַ�
                    WCharToAChar(
                        ((CMsLuaManager*)lpParameter)->
                        m_pLuaChangeNotify->FileName,
                        ((CMsLuaManager*)lpParameter)->m_strLuaFileName,
                        MAX_PATH
                        );

                    ((CMsLuaManager*)lpParameter)->OnLuaFileIsChange(
                        ((CMsLuaManager*)lpParameter)->
                        m_pLuaChangeNotify->FileName
                        );

                }break;
            case FILE_ACTION_RENAMED_NEW_NAME:
                {
                    MESSAGEBOX(AL_SHOW_ONLY, _T("���������ļ�"));
                }break;
            default:
                {
                    MESSAGEBOX(AL_SHOW_ONLY, _T("δ֪�ļ��仯"));
                }break;
            }        
        }
    }

    // �رյ�ǰ�߳�
    bRet = ::CloseHandle(((CMsLuaManager*)lpParameter)->m_hThread);
    AssertEx(AL_NORMAL, bRet, _T("�ر�CheckLuaFileIsChangeThreadProc�߳�ʧ��,����ʧ��"));

    // ���߳�ID,�߳̾���ÿ�
    ((CMsLuaManager*)lpParameter)->m_nThreadId = 0;
    ((CMsLuaManager*)lpParameter)->m_hThread   = NULL;

    // ���سɹ�
    return 0;

    _LEAVE_FUN_DISPOSE_CONTINUE_C;
    return LUA_ERROR;
}

//int __fastcall api_call(lua_State *L)
//{
//    int n = lua_gettop(L);
//    func_call fc = (func_call)lua_touserdata(L, lua_upvalueindex(2));
//    DWORD arg;
//    for (int type, i = n; i > 0; i--)
//    {
//        type = lua_type(L, i);
//        switch(type)
//        {
//        case LUA_TNIL:
//            {
//                arg = 0;
//            }break;
//        case LUA_TNUMBER:
//            {
//                arg = (DWORD)lua_tointeger(L,i);
//            }break;
//        case LUA_TBOOLEAN:
//            {
//                arg = (DWORD)lua_toboolean(L,i);
//            }break;
//        case LUA_TSTRING:
//            {
//                arg = (DWORD)lua_tostring(L,i);
//            }break;
//        case LUA_TLIGHTUSERDATA:
//            {
//                arg = (DWORD)lua_touserdata(L,i);
//            }break;
//        default:
//            {
//                lua_pushstring(L,"unknown argument type");
//                lua_error(L);
//            }break;
//        }
//        __asm
//        {
//            push arg;
//        }
//    }
//    int* pRet;
//    __asm
//    {
//        call fc;
//        mov pRet,eax;
//    }
//    lua_pushinteger(L, (int)pRet);
//
//    return 1;
//}

//int __fastcall open_dll(lua_State *L)
//{
//    CONST ACHAR* Name = lua_tostring(L,1);
//
//#ifdef UNICODE
//    TCHAR strFileName[MAX_PATH] = {0};
//    ::MultiByteToWideChar(
//        CP_ACP, 
//        0, 
//        Name,
//        strlen(Name) + 1,
//        strFileName,
//        MAX_PATH);
//#else
//    CONST ACHAR* strFileName = Name;
//#endif
//    HMODULE hm = ::LoadLibrary(strFileName);
//    lua_pushlightuserdata(L, hm);
//    return 1;
//}
//
//int __fastcall get_procaddress(lua_State *L)
//{
//    HMODULE hm          = (HMODULE)lua_touserdata(L, 1);
//    const char *name    = lua_tostring(L, 2);
//    void *func          = ::GetProcAddress(hm, name);
//    lua_pushvalue(L, 3);
//    lua_pushlightuserdata(L, func);
//    lua_pushcclosure(L, api_call, 2);
//    return 1;
//}