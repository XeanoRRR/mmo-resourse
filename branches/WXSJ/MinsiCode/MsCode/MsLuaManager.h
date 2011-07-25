#pragma once              //��  //��
#ifndef __LUAMANAGER_H__  //��  //��
#define __LUAMANAGER_H__  //��  //��
//����������������������������  //��
//�ļ����ƣ�LuaManager.h        //��
//����������Lua�ű�������       //��
//ƽ̨��أ�Windows             //��
//����������������������������������

#include "MsLuaLTCFun.h"

// LUA���ش���
#define LUA_ERROR               0xFFFFFFFF

// LUA�ű����������ߴ�
#define MAX_LUA_FUN_NAME_LEN    255

// LUA�ļ��ı���֪ͨ��������С
#define MAX_LUA_FILE_CHANGE_NOTIFY_BUFF_LEN 1024

enum RETURN_VALUE_TYPE
{
    LUA_RET_VOID = 0,
    LUA_RET_INT,
    LUA_RET_DOUBLE,
    LUA_RET_BOOL,
    LUA_RET_STRING,
};

typedef struct LUA_FUN_RET_VALUE
{
    LUA_FUN_RET_VALUE(RETURN_VALUE_TYPE type):m_eType(type){}
    union
    {
        SIZE_T OUT  m_lLuaFunRet;
        DOUBLE OUT  m_dLuaFunRet;
        BOOL   OUT  m_bLuaFunRet;
        VOID*  OUT  m_lpVoid;
        struct STR
        {
            size_t       OUT  m_Size;
            CONST ACHAR* OUT  m_str;
        }m_String;
    };
    RETURN_VALUE_TYPE   m_eType;
}LFRV;


class CMsLuaManager:
    public CMsFindFileManager
{
public:
    CMsLuaManager();
    virtual ~CMsLuaManager(VOID);

    // ����CMsFindFileManager�е�֪ͨ��Ϣ����
public:
    // �����������������ļ�ʱ�����õ�֪ͨ����
    virtual BOOL    ProcessFile(CONST TCHAR* strFileName);

    // ����������Ŀ¼ʱ�����õ�֪ͨ����
    virtual BOOL    ProcessPath(CONST TCHAR* strCurrentPath);

    // lua�ű�������������Ա����
public:
    // ��ʼ��
    BOOL Init(VOID);

    // �����ű�����
    VOID EnableDebug(BOOL bEnable, lua_Hook lpLuaHookCallBackFun, DWORD dwDebugFlag, DWORD dwRunCount = 0);

    BOOL LoadLuaFile(CONST TCHAR* strLuaFileName);

    BOOL RunLuaFun(
        LUA_FUN_RET_VALUE* pLuaRet,
        DWORD dwParamCount,
        TCHAR* strLuaFunName,
        TCHAR* strLuaParameter,
        ...
        );

    BOOL RunLuaFun_Va(
        LUA_FUN_RET_VALUE* pLuaRet,
        DWORD dwParamCount,
        TCHAR* strLuaFunName,
        TCHAR* strLuaParameter,
        va_list vaList
        );

    // ����LUA�ļ���Ŀ¼
    BOOL    SetLuaFileRootPath(TCHAR* strLuaFilePath = NULL);

    // ��ʼ���LUA�ļ��Ƿ��иı�
    BOOL    BeginCheckLuaFileIsChange(VOID);

    // ֹͣ���LUA�ļ��Ƿ��иı�
    BOOL    StopCheckLuaFileIsChange(VOID);

    // ��⵽LUA�ļ��Ѿ��޸�
    BOOL    OnLuaFileIsChange(TCHAR* strLuaFileName);

    // ע��C++������LUA
    BOOL    RegisterCFunToLua(luaL_Reg& NewRegLTC);

    // Ĭ��ע��ϵͳȫ�������е�LTC������ʹ��Ĭ�ϲ���
    BOOL    RegisterCFunToLua(luaL_Reg* pRegLTC, UINT uRegLTCCount);

    // ���ü��LUA�ļ��޸��߳�Ϊ��Ԫ����
    friend DWORD WINAPI CheckLuaFileIsChangeThreadProc(LPVOID lpParameter);
protected:
    // Lua������ָ��
    lua_State* m_pExplainOrgan;

    // ����LUA������������
    //ACHAR m_strLuaFunName[MAX_LUA_FUN_NAME_LEN];

    // LUA�ļ��ı���֪ͨ������
    //ACHAR m_ChangeNotify[MAX_LUA_FILE_CHANGE_NOTIFY_BUFF_LEN];  

    // LUA�ļ���Ŀ¼
    TCHAR m_szTLuaFilePath[MAX_PATH];

    // LUA�ļ���Ŀ¼�ļ����
    HANDLE m_hFilePath;

    // LUA�ļ����֪ͨ������
    FILE_NOTIFY_INFORMATION* m_pLuaChangeNotify;
    ACHAR m_pLuaChangeNotifyBuff[(sizeof(FILE_NOTIFY_INFORMATION) + MAX_PATH)];

    // LUA�ļ�����߳�ID
    DWORD m_nThreadId;

    // LUA�ļ�����߳̾��
    HANDLE m_hThread;

    // ���LUA�ļ��Ƿ�����
    BOOL m_IsChecking;

    // �ı��LUA�ļ�·��
    ACHAR m_strLuaFileName[MAX_PATH];
    ACHAR m_szALuaFilePath[MAX_PATH];
    ACHAR m_strLuaParameter[MAX_PATH];

    lua_Hook m_lpLuaHookCallBackFun;

private:
    CMsLuaManager& operator = (CMsLuaManager&){};
};

//typedef void* (__stdcall *func_call)();
//int __fastcall api_call(lua_State *L);
//int __fastcall open_dll(lua_State *L);
//int __fastcall get_procaddress(lua_State *L);

//����������������������
#endif//#pragma once//��
//����������������������
