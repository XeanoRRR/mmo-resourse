#include "Precompiled.h"

HOOKAPI ha_MessageBoxA;
HOOKAPI ha_MessageBoxW;

INT WINAPI MyMessageBoxA(
    __in_opt HWND hWnd,
    __in_opt LPCSTR lpText,
    __in_opt LPCSTR lpCaption,
    __in UINT uType
    )
{
    INT(WINAPI*pRealMessageBoxA)(HWND,LPCSTR,LPCSTR,UINT);
    pRealMessageBoxA = (INT(WINAPI*)(HWND,LPCSTR,LPCSTR,UINT))ha_MessageBoxA.m_pOldProc;
    return pRealMessageBoxA(hWnd, "���������MessageBox", "����", MB_OK);
}

INT WINAPI MyMessageBoxW(
     __in_opt HWND hWnd,
    __in_opt LPCWSTR lpText,
    __in_opt LPCWSTR lpCaption,
    __in UINT uType
    )
{
    INT(WINAPI*pRealMessageBoxW)(HWND,LPCWSTR,LPCWSTR,UINT);
    pRealMessageBoxW = (INT(WINAPI*)(HWND,LPCWSTR,LPCWSTR,UINT))ha_MessageBoxW.m_pOldProc;
    return pRealMessageBoxW(hWnd, _T("���������MessageBox"), _T("����"), MB_OK);
}

INT __fastcall MainCall(INT,LPTCHAR*)
{
    _ENTER_FUN_C;

    MSLOG.SetSaveLog(FALSE);
    MSLOG.SetEnableLog(FALSE);

    HMODULE hModule = ::GetModuleHandle(NULL);

    ha_MessageBoxA.m_strFunc    = _T("MessageBoxA");
    ha_MessageBoxA.m_pNewProc   = (ULONG_PTR)MyMessageBoxA;
    CMsWinMain::HookAPIByName(hModule, _T("User32.dll"), &ha_MessageBoxA);

    ha_MessageBoxW.m_strFunc    = _T("MessageBoxW");
    ha_MessageBoxW.m_pNewProc   = (ULONG_PTR)MyMessageBoxW;
    CMsWinMain::HookAPIByName(hModule, _T("User32.dll"), &ha_MessageBoxW);




























    ::MessageBoxA(NULL, "���ԶԻ���", "��ʾ", MB_OK);
    ::MessageBoxW(NULL, _T("���ԶԻ���"), _T("��ʾ"), MB_OK);

    _tsystem(_T("pause"));
    return 0;
    _LEAVE_FUN_DISPOSE_END_C;
    return INVALID_NID;
}

















/*
(2)   ��ActiveKey.h�ļ��м���HOOKAPI�ṹ���˽ṹ�����洢������API�������ơ�ԭAPI������ַ�����������ַ��   
    
  ������typedef   struct   tag_HOOKAPI     
  ������{     
  ������LPCSTR   szFunc;//��HOOK��API�������ơ�   
  ������PROC   pNewProc;//���������ַ��   
  ������PROC   pOldProc;//ԭAPI������ַ��   
  ������}HOOKAPI,   *LPHOOKAPI;       
    
  ����(3)   ��ActiveKey.cpp�ļ������ȼ���һ�����������ڶ�λ��������������ݶ��е�IAT��ַ���������£�   
    
  ������extern   "C"   __declspec(dllexport)PIMAGE_IMPORT_DESCRIPTOR     
  ������LocationIAT(HMODULE   hModule,   LPCSTR   szImportMod)     
  ������//���У�hModuleΪ����ģ������szImportModΪ��������ơ�   
  ������{     
  ������//����Ƿ�ΪDOS�������Ƿ���NULL����DOS����û��IAT��   
  ������PIMAGE_DOS_HEADER   pDOSHeader   =   (PIMAGE_DOS_HEADER)   hModule;     
  ������if(pDOSHeader->e_magic   !=   IMAGE_DOS_SIGNATURE)   return   NULL;     
  ��������//����Ƿ�ΪNT��־�����򷵻�NULL��   
  ��������PIMAGE_NT_HEADERS   pNTHeader   =   (PIMAGE_NT_HEADERS)((DWORD)pDOSHeader+   (DWORD)(pDOSHeader->e_lfanew));     
  ��������if(pNTHeader->Signature   !=   IMAGE_NT_SIGNATURE)   return   NULL;     
  ��������//û��IAT���򷵻�NULL��   
  ��������if(pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress   ==   0)   return   NULL;     
  ��������//��λ��һ��IATλ�á�     
  ��������PIMAGE_IMPORT_DESCRIPTOR   pImportDesc   =   (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDOSHeader   +   (DWORD)(pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));     
  ��������//�������������ѭ��������е�IAT����ƥ���򷵻ظ�IAT��ַ����������һ��IAT��   
  ��������while   (pImportDesc->Name)     
  ��������{     
  ����������//��ȡ��IAT��������������ơ�   
  ������PSTR   szCurrMod   =   (PSTR)((DWORD)pDOSHeader   +   (DWORD)(pImportDesc->Name));     
  ������if   (stricmp(szCurrMod,   szImportMod)   ==   0)   break;     
  ������pImportDesc++;     
  ��������}     
  ��������if(pImportDesc->Name   ==   NULL)   return   NULL;     
  ������return   pImportDesc;     
  ������}     
    
  �����ټ���һ��������������λ������API������IAT��޸�������Ϊ���������ַ���������£�   
    
  ������extern   "C"   __declspec(dllexport)     
  ������HookAPIByName(   HMODULE   hModule,   LPCSTR   szImportMod,   LPHOOKAPI   pHookApi)     
  ������//���У�hModuleΪ����ģ������szImportModΪ��������ƣ�pHookAPIΪHOOKAPI�ṹָ�롣   
  ������{     
  ��������//��λszImportMod��������������ݶ��е�IAT��ַ��   
  ��������PIMAGE_IMPORT_DESCRIPTOR   pImportDesc   =   LocationIAT(hModule,   szImportMod);     
  ����if   (pImportDesc   ==   NULL)   return   FALSE;     
  ��������//��һ��Thunk��ַ��   
  ��������PIMAGE_THUNK_DATA   pOrigThunk   =   (PIMAGE_THUNK_DATA)((DWORD)hModule   +   (DWORD)(pImportDesc->OriginalFirstThunk));     
  ����   //��һ��IAT���Thunk��ַ��   
  ��������PIMAGE_THUNK_DATA   pRealThunk   =   (PIMAGE_THUNK_DATA)((DWORD)hModule   +   (DWORD)(pImportDesc->FirstThunk));     
  ��������//ѭ�����ұ���API������IAT���ʹ�����������ַ�޸���ֵ��   
  ������while(pOrigThunk->u1.Function)     
  {     
  ��//����Thunk�Ƿ�ΪIAT�   
  if((pOrigThunk->u1.Ordinal   &   IMAGE_ORDINAL_FLAG)   !=   IMAGE_ORDINAL_FLAG)     
  {   
  ��   //��ȡ��IAT���������ĺ������ơ�   
  ��PIMAGE_IMPORT_BY_NAME   pByName   =(PIMAGE_IMPORT_BY_NAME)((DWORD)hModule+(DWORD)(pOrigThunk->u1.AddressOfData));     
  ��if(pByName->Name[0]   ==   '\0')   return   FALSE;     
  ����//����Ƿ�Ϊ���غ�����   
  if(strcmpi(pHookApi->szFunc,   (char*)pByName->Name)   ==   0)     
  ��   {     
  ��������������MEMORY_BASIC_INFORMATION   mbi_thunk;   
  ��������������//��ѯ�޸�ҳ����Ϣ��   
  ��������������VirtualQuery(pRealThunk,   &mbi_thunk,   sizeof(MEMORY_BASIC_INFORMATION));     
  //�ı��޸�ҳ��������ΪPAGE_READWRITE��   
  ��������������VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize,   PAGE_READWRITE,   &mbi_thunk.Protect);     
  //����ԭ����API������ַ��   
  ������   ����if(pHookApi->pOldProc   ==   NULL)     
  pHookApi->pOldProc   =   (PROC)pRealThunk->u1.Function;     
  ��   //�޸�API����IAT������Ϊ���������ַ��   
  pRealThunk->u1.Function   =   (PDWORD)pHookApi->pNewProc;     
  //�ָ��޸�ҳ�������ԡ�   
  DWORD   dwOldProtect;     
  ��������������VirtualProtect(mbi_thunk.BaseAddress,   mbi_thunk.RegionSize,   mbi_thunk.Protect,   &dwOldProtect);     
  ����������   }     
  }     
  ��   pOrigThunk++;     
  ��   pRealThunk++;     
  }     
  ����SetLastError(ERROR_SUCCESS);   //���ô���ΪERROR_SUCCESS����ʾ�ɹ���   
  ����return   TRUE;     
  ������}       
    
  ����(4)   ���������������ʵ����ֻ��MessageBoxA��recv����API���е��ء��������£�   
    
  ������static   int   WINAPI   MessageBoxA1   (HWND   hWnd   ,   LPCTSTR   lpText,   LPCTSTR   lpCaption,   UINT   uType)   
  ������{   
  HANDLE   hFile;   
    
  if((hFile   =CreateFile("c:\\MessageBoxA1.log",   GENERIC_WRITE,   0,   NULL,   OPEN_ALWAYS,   FILE_ATTRIBUTE_NORMAL,   NULL))   <0)   
  {   
  WriteLog("open   file   %s   failed",   fname);   
  return   -1;   
  }   
  _llseek((HFILE)hFile,   0,   SEEK_END);   
  char   temp[2048];   
  wsprintf(temp,   "\r\n(LPCTSTR   lpText%s,   LPCTSTR   lpCaption%s)   ",     lpText,lpCaption);   
  DWORD   dw;   
  WriteFile(hFile,   temp,   strlen(temp),   &dw,   NULL);   
    
  _lclose((HFILE)hFile);   
  ��������//���˵�ԭMessageBoxA�����ĺͱ������ݣ�ֻ��ʾ�������ݡ�   
  return   MessageBox(hWnd,   "Hook   API   OK!",   "Hook   API",   uType);     
  ������}     
  ������static   int   WINAPI   recv1(SOCKET   s,   char   FAR   *buf,   int   len,   int   flags   )   
  ������{   
  ������HANDLE   hFile;   
    
  if((hFile   =CreateFile("c:\\recv1.log",   GENERIC_WRITE,   0,   NULL,   OPEN_ALWAYS,   FILE_ATTRIBUTE_NORMAL,   NULL))   <0)   
  {   
  WriteLog("open   file   %s   failed",   fname);   
  return   -1;   
  }   
  _llseek((HFILE)hFile,   0,   SEEK_END);   
  char   temp[2048];   
  wsprintf(temp,   "\r\n(len=%d)   ",     len);   
  DWORD   dw;   
  WriteFile(hFile,   temp,   strlen(temp),   &dw,   NULL);   
    
    
  for(int   i   =0;   i<len;   i++)   
  {   
    
          wsprintf(temp,   "(%02x)",   buf[i]&0x00FF);   
  WriteFile(hFile,   temp,   strlen(temp),   &dw,   NULL);   
  }   
  _lclose((HFILE)hFile);   
    
    
    
    
  ������return   recv(s,buf,len,flags);   
  ������}       
    
  (5)   ��KeyboardProc�����м��뼤���API���룬��if(   wParam   ==   0X79   )����к����������else   if��䣺   
    
  ������......   
  ������//������F11��ʱ����������API�������ܡ�   
  ������else   if(   wParam   ==   0x7A   )   
  ������{     
  ��������HOOKAPI   api[2];   
  api[0].szFunc   ="MessageBoxA";//���ñ����غ��������ơ�   
  api[0].pNewProc   =   (PROC)MessageBoxA1;//������������ĵ�ַ��   
  api[1].szFunc   ="recv";//���ñ����غ��������ơ�   
  api[1].pNewProc   =   (PROC)recv1;   //������������ĵ�ַ��   
  //���õ���User32.dll���е�MessageBoxA������   
  HookAPIByName(GetModuleHandle(NULL),"User32.dll",&api[0]);   
  //���õ���Wsock32.dll���е�recv������   
  HookAPIByName(GetModuleHandle(NULL),"Ws2_32.dll",&api[1]);   
  ������}   
  ������......       
  ����recv1()��д�˸��ػ����ݲ����浽c:\recv1.log�Ĵ��룬�����к�û�нػ�Ŀ���������ݡ�   
  ��ͬ���Ĵ���ȴ��MessageBoxA1()�������������ҷ���Ŀ����򱻹�ס��һ����MessageBoxA�ͱ��ػ񣬲���¼��c:\recv1.log   
    
  ���Ϻָ�㣬Ϊ�νػ񲻵�recv()��   

*/