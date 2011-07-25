#pragma once              //��  //��
#ifndef __MSASSERTX_H__   //��  //��
#define __MSASSERTX_H__   //��  //��
//����������������������������  //��
//�ļ����ƣ�MsAssertx.h         //��
//�����������������Զ���        //��
//ƽ̨��أ�Windows             //��
//����������������������������������

//����������������������������������������������������������
//������־�ı���󳤶�                                  //��
#define ASSERTX_STR_MAX_LEN 1256                        //��
                                                        //��
enum                                                    //��
{                                                       //��
    COMMAND_NORMAL_MSGBOX = 0,                          //��
    COMMAND_IGNORE_MSGBOX,                              //��
    COMMAND_NORMAL_LOG,                                 //��
    COMMAND_IGNORE_LOG,                                 //��
                                                        //��
};                                                      //��
														//��
enum E_ASSERT_LEVEL                                     //��
{                                                       //��
	ASSERT_LEVEL_NORMAL = 0,							//��
	ASSERT_LEVEL_CATASTROPHIC,                          //��
	ASSERT_LEVEL_SHOW_ONLY,                             //��
	ASSERT_LEVEL_IGNORE,								//��
};                                                      //��
#define AL_NORMAL		ASSERT_LEVEL_NORMAL				//��
#define AL_CATAST		ASSERT_LEVEL_CATASTROPHIC		//��
#define AL_SHOW_ONLY	ASSERT_LEVEL_SHOW_ONLY			//��
#define AL_IGNORE		ASSERT_LEVEL_IGNORE				//��
// 0:�洢��־ 1:������־                                //��
extern INT      g_Command_Assert_Log;                   //��
                                                        //��
// 0:ʹ��MessageBox��ʾ 1:����MessageBox��ʾ            //��
extern INT      g_Command_MessageBox;                   //��
                                                        //��
// ȫ����־�ļ����                                     //��
extern HANDLE   g_hAssertLogFile;                       //��
                                                        //��
// ȫ���Ƿ��Ѿ���ʼ�����Ծ��                           //��
extern BOOL     g_bAssertInit;                          //��
                                                        //��
//����������������������������������������������������������

BOOL InitAssertx(TCHAR* szAssertFileName);
BOOL ReleaseAssertx(VOID* /*Parameter*/);

//��������������������������������������������������������������
//���ԶԻ���T                                           	//��
inline VOID MESSAGEBOX(										//��
	CONST E_ASSERT_LEVEL eLevel,							//��
	CONST TCHAR* msg										//��
	)														//��
{                                                       	//��
	if (COMMAND_NORMAL_MSGBOX == g_Command_MessageBox)  	//��
	{                                                   	//��
		switch (eLevel)										//��
		{													//��
		// ��׼����											//��
		case ASSERT_LEVEL_NORMAL:							//��
			{												//��
#ifdef _DEBUG                                           	//��
				INT nRet = ::MessageBox(                	//��
					NULL,                               	//��
					msg,                                	//��
					_T("�� �� ���ж�,")                 	//��
					_T("�� �� ������.")                 	//��
					_T("��ȡ��������,"),                	//��
					MB_YESNOCANCEL);                    	//��
#else                                                   	//��
				INT nRet = ::MessageBox(                	//��
					NULL,                               	//��
					msg,                                	//��
					_T("��ȷ��������.")                 	//��
					_T("��ȡ��������,"),                	//��
					MB_OKCANCEL);                       	//��
#endif                                                  	//��
				switch (nRet)                           	//��
				{                                       	//��
				case IDOK:                              	//��
				case IDNO:                              	//��
					{                                   	//��
						;                               	//��
					}break;                             	//��
				case IDYES:                             	//��
					{                                   	//��
						_CrtDbgBreak();                 	//��
					}break;                             	//��
				default:                                	//��
				case IDCANCEL:                          	//��
					{										//��
						static TCHAR MsgBuf[KB_SIZE] = {0}; //��
						::FormatMessage(                    //��
							FORMAT_MESSAGE_FROM_SYSTEM,     //��
							NULL,                           //��
							::GetLastError(),               //��
							MAKELANGID(                     //��
							LANG_NEUTRAL,                   //��
							SUBLANG_CUSTOM_DEFAULT          //��
							),                              //��
							MsgBuf,                         //��
							KB_SIZE,                        //��
							NULL);                          //��
						SIZE_T sMsgLen = _tcslen(MsgBuf);   //��
						if (sMsgLen)                        //��
						{                                   //��
#ifdef UNICODE												//��
							MsgBuf[sMsgLen-1] = _T('\0');   //��
							MsgBuf[sMsgLen-2] = _T('\0');   //��
#else														//��
							MsgBuf[sMsgLen-1] = _T('\0');   //��
#endif														//��
						}                                   //��
						::MessageBox(						//��
							NULL,							//��
							MsgBuf,							//��
							_T("��������"),					//��
							MB_OK);							//��
						exit(INVALID_NID);					//��
					}break;                                 //��
				}											//��
			}break;											//��
		// ������											//��
		case ASSERT_LEVEL_CATASTROPHIC:						//��
			{												//��
				INT nRet = ::MessageBox(                    //��
					NULL,                                   //��
					msg,                                    //��
					_T("�����Զ���")                        //��
#ifdef _DEBUG												//��
					_T("���򼴽�����.")                     //��
					_T("�Ƿ��жϲ鿴"),                     //��
					MB_YESNO);								//��
#else														//��
					_T("���򼴽�����."),                    //��
					MB_OK);									//��
#endif														//��
				switch (nRet)                               //��
				{                                           //��
				case IDOK:                                  //��
				case IDNO:                                  //��
					{                                       //��
						;                                   //��
					}break;                                 //��
				case IDYES:                                 //��
					{                                       //��
						_CrtDbgBreak();                     //��
					}break;									//��
				}											//��
				static TCHAR MsgBuf[KB_SIZE] = {0};			//��
				::FormatMessage(							//��
					FORMAT_MESSAGE_FROM_SYSTEM,				//��
					NULL,									//��
					::GetLastError(),						//��
					MAKELANGID(								//��
					LANG_NEUTRAL,							//��
					SUBLANG_CUSTOM_DEFAULT					//��
					),										//��
					MsgBuf,									//��
					KB_SIZE,								//��
					NULL);									//��
				SIZE_T sMsgLen = _tcslen(MsgBuf);			//��
				if (sMsgLen)								//��
				{											//��
#ifdef UNICODE												//��
					MsgBuf[sMsgLen-1] = _T('\0');			//��
					MsgBuf[sMsgLen-2] = _T('\0');			//��
#else														//��
					MsgBuf[sMsgLen-1] = _T('\0');			//��
#endif														//��
				}											//��
				::MessageBox(								//��
					NULL,									//��
					MsgBuf,									//��
					_T("��������"),							//��
					MB_OK);									//��
				exit(INVALID_NID);							//��
		}break;												//��
		// ����ʾ											//��
		case ASSERT_LEVEL_SHOW_ONLY:						//��
			{												//��
				;											//��
				INT nRet = ::MessageBox(                    //��
					NULL,                                   //��
					msg,                                    //��
#ifdef _DEBUG												//��
					_T("��ʾ����")							//��
					_T("�Ƿ��жϲ鿴")						//��
					_T("��ȡ������������"),                 //��
					MB_YESNOCANCEL);						//��
#else														//��
					_T("��ʾ����")							//��
					_T("��ȡ������������"),                 //��
					MB_OKCANCEL);							//��
#endif														//��
				switch (nRet)                               //��
				{                                           //��
				case IDOK:                                  //��
				case IDNO:                                  //��
					{                                       //��
						;                                   //��
					}break;                                 //��
				case IDYES:                                 //��
					{                                       //��
						_CrtDbgBreak();                     //��
					}break;									//��
				case IDCANCEL:								//��
					{										//��
						g_Command_MessageBox =				//��
							COMMAND_IGNORE_MSGBOX;			//��
					}break;									//��
				}											//��
			}break;											//��
		// ������ʾ											//��
		case ASSERT_LEVEL_IGNORE:							//��
			{												//��
				;											//��
			}break;											//��
	}														//��
}                                                   		//��
}                                                       	//��
//���ԶԻ���A                                           	//��
inline VOID MESSAGEBOXA(CONST ACHAR* msg)               	//��
{                                                       	//��
	if (COMMAND_NORMAL_MSGBOX == g_Command_MessageBox)  	//��
	{                                                   	//��
#ifdef _DEBUG                                           	//��
		INT nRet = ::MessageBoxA(                       	//��
			NULL,                                       	//��
			msg,                                        	//��
			"�� �� ���ж�,"                             	//��
			"�� �� ������."                             	//��
			"��ȡ��������,",                            	//��
			MB_YESNOCANCEL);                            	//��
#else                                                   	//��
		INT nRet = ::MessageBoxA(                       	//��
			NULL,                                       	//��
			msg,                                        	//��
			"��ȷ��������."                             	//��
			"��ȡ��������,",                            	//��
			MB_OKCANCEL);                               	//��
#endif                                                  	//��
		switch (nRet)                                   	//��
		{                                               	//��
		case IDOK:                                      	//��
		case IDNO:                                      	//��
			{                                           	//��
				;                                       	//��
			}break;                                     	//��
		case IDYES:                                     	//��
			{                                           	//��
				_CrtDbgBreak();                         	//��
			}break;                                     	//��
		default:                                        	//��
		case IDCANCEL:                                  	//��
			{                                           	//��
				static ACHAR MsgBuf[KB_SIZE] = {0};     	//��
				::FormatMessageA(                       	//��
					FORMAT_MESSAGE_FROM_SYSTEM,         	//��
					NULL,                               	//��
					::GetLastError(),                   	//��
					MAKELANGID(                         	//��
					LANG_NEUTRAL,                       	//��
					SUBLANG_CUSTOM_DEFAULT              	//��
					),                                  	//��
					MsgBuf,                             	//��
					KB_SIZE,                            	//��
					NULL);                              	//��
				SIZE_T sMsgLen = strlen(MsgBuf);        	//��
				if (sMsgLen)                            	//��
				{                                       	//��
					MsgBuf[sMsgLen-1] = '\0';           	//��
				}                                       	//��
				if (IDYES == ::MessageBoxA(             	//��
					NULL,                               	//��
					MsgBuf,                             	//��
					"��������",                       	//��
					MB_YESNO))                          	//��
				{                                       	//��
					exit(INVALID_NID);                  	//��
				}                                       	//��
			}break;                                     	//��
		}                                               	//��
	}                                                   	//��
}                                                       	//��
//���ԶԻ���W                                           	//��
inline VOID MESSAGEBOXW(CONST WCHAR* msg)               	//��
{                                                       	//��
	if (COMMAND_NORMAL_MSGBOX == g_Command_MessageBox)  	//��
	{                                                   	//��
#ifdef _DEBUG                                           	//��
		INT nRet = ::MessageBoxW(                       	//��
			NULL,                                       	//��
			msg,                                        	//��
			L"�� �� ���ж�,"                            	//��
			L"�� �� ������."                            	//��
			L"��ȡ��������,",                           	//��
			MB_YESNOCANCEL);                            	//��
#else                                                   	//��
		INT nRet = ::MessageBoxW(                       	//��
			NULL,                                       	//��
			msg,                                        	//��
			L"��ȷ��������."                            	//��
			L"��ȡ��������,",                           	//��
			MB_OKCANCEL);                               	//��
#endif                                                  	//��
		switch (nRet)                                   	//��
		{                                               	//��
		case IDOK:                                      	//��
		case IDNO:                                      	//��
			{                                           	//��
				;                                       	//��
			}break;                                     	//��
		case IDYES:                                     	//��
			{                                           	//��
				_CrtDbgBreak();                         	//��
			}break;                                     	//��
		default:                                        	//��
		case IDCANCEL:                                  	//��
			{                                           	//��
				static WCHAR MsgBuf[KB_SIZE] = {0};     	//��
				::FormatMessageW(                       	//��
					FORMAT_MESSAGE_FROM_SYSTEM,         	//��
					NULL,                               	//��
					::GetLastError(),                   	//��
					MAKELANGID(                         	//��
					LANG_NEUTRAL,                       	//��
					SUBLANG_CUSTOM_DEFAULT              	//��
					),                                  	//��
					MsgBuf,                             	//��
					KB_SIZE,                            	//��
					NULL);                              	//��
				SIZE_T sMsgLen = wcslen(MsgBuf);        	//��
				if (sMsgLen)                            	//��
				{                                       	//��
					MsgBuf[sMsgLen-2] = L'\0';          	//��
					MsgBuf[sMsgLen-1] = L'\0';          	//��
				}                                       	//��
				if (IDYES == ::MessageBoxW(             	//��
					NULL,                               	//��
					MsgBuf,                             	//��
					L"��������",                      	//��
					MB_YESNO))                          	//��
				{                                       	//��
					exit(INVALID_NID);                  	//��
				}                                       	//��
			}break;                                     	//��
		}                                               	//��
	}                                                   	//��
}                                                       	//��
//��������������������������������������������������������������

//��������������������������������������������������������������������������������������
inline VOID LOG_(CONST TCHAR* szTemp)                                               //��
{                                                                                   //��
    // ��̬������ʾ��                                                               //��
    static MsLock lock;                                                             //��
                                                                                    //��
    // ��־������                                                                   //��
    TCHAR szLog_t[ASSERTX_STR_MAX_LEN] = {0};                                       //��
                                                                                    //��
    // �ɹ�д����־���ֽ���                                                         //��
    DWORD dwLogSize = 0;                                                            //��
                                                                                    //��
    // �������Ƿ��Ѿ���ʼ��                                                       //��
    if (!g_bAssertInit)                                                             //��
    {                                                                               //��
        MessageBox(NULL, _T("����δ��ʼ��,��������"), _T("����"), MB_OK);           //��
        g_bAssertInit = TRUE;                                                       //��
        g_Command_Assert_Log = COMMAND_IGNORE_LOG;                                  //��
    }                                                                               //��
                                                                                    //��
    if(COMMAND_NORMAL_LOG == g_Command_Assert_Log)                                  //��
    {                                                                               //��
        BOOL bRet = ::CreateDirectory(_T("Log"), NULL);                             //��
        if (!bRet)                                                                  //��
        {                                                                           //��
            DWORD dwErrorCode = ::GetLastError();                                   //��
            if (ERROR_ALREADY_EXISTS != dwErrorCode)                                //��
            {                                                                       //��
                _stprintf_s(                                                        //��
                    szLog_t,                                                        //��
                    ASSERTX_STR_MAX_LEN,                                            //��
                    _T("������־Ŀ¼����ʧ�ܣ�����\n%d"),                           //��
                    dwErrorCode                                                     //��
                    );                                                              //��
            }                                                                       //��
        }                                                                           //��
        {                                                                           //��
            AUTO_LOCK(lock);                                                        //��
                                                                                    //��
            dwLogSize =                                                             //��
                (DWORD)(_tcsnlen(szTemp, ASSERTX_STR_MAX_LEN)*sizeof(TCHAR));       //��
                                                                                    //��
            if (!::WriteFile(                                                       //��
                g_hAssertLogFile,                                                   //��
                szTemp,                                                             //��
                dwLogSize,                                                          //��
                &dwLogSize,                                                         //��
                NULL                                                                //��
                ))                                                                  //��
            {                                                                       //��
                _stprintf_s(                                                        //��
                    szLog_t,                                                        //��
                    ASSERTX_STR_MAX_LEN,                                            //��
                    _T("������־д��ʧ�ܣ�����\n%d"),                               //��
                    ::GetLastError()                                                //��
                    );                                                              //��
            }                                                                       //��
        }                                                                           //��
    }                                                                               //��
}                                                                                   //��
                                                                                    //��
inline VOID ASSERT_(                                                                //��
	CONST E_ASSERT_LEVEL eLevel,                                                    //��
	CONST BOOL bThrow,                                                              //��
    CONST TCHAR* file,                                                              //��
    CONST DWORD  line,                                                              //��
    CONST TCHAR* func,                                                              //��
    CONST TCHAR* expr,                                                              //��
    CONST TCHAR* str                                                                //��
        )                                                                           //��
{                                                                                   //��
    // �������Ƿ��Ѿ���ʼ��                                                       //��
    if (!g_bAssertInit)                                                             //��
    {                                                                               //��
        MessageBox(NULL, _T("����δ��ʼ��,��������"), _T("����"), MB_OK);           //��
        g_bAssertInit = TRUE;                                                       //��
        g_Command_Assert_Log = COMMAND_IGNORE_LOG;                                  //��
    }                                                                               //��
                                                                                    //��
    if ((COMMAND_IGNORE_LOG == g_Command_Assert_Log) &&                             //��
        (COMMAND_IGNORE_MSGBOX == g_Command_MessageBox))                            //��
    {                                                                               //��
        return;                                                                     //��
    }                                                                               //��
    TCHAR szTemp[ASSERTX_STR_MAX_LEN] = {0};                                        //��
    UPDATA_LOCALTIME;                                                               //��
                                                                                    //��
    _stprintf_s(                                                                    //��
        szTemp,                                                                     //��
        ASSERTX_STR_MAX_LEN,                                                        //��
        _T("�ļ�: [%s]\n")                                                          //��
        _T("��  : [%d]\n")                                                          //��
        _T("����: [%s]\n")                                                          //��
        _T("ԭ��: [%s]\n")                                                          //��
        _T("�쳣: [%s]\n")                                                          //��
        _T("ʱ��: [%d��%d��%d��-����%d")                                            //��
        _T("[%dʱ%d��%d��%d����]\n"),                                               //��
        file, line, func, expr, str,                                                //��
        LOCALTIME_YEAR, LOCALTIME_MONTH, LOCALTIME_DAY, LOCALTIME_WEEK,             //��
        LOCALTIME_HOUR, LOCALTIME_MINUTE, LOCALTIME_SECOND, LOCALTIME_MILLISECOND   //��
        );                                                                          //��
                                                                                    //��
    LOG_(szTemp);                                                                   //��
    MESSAGEBOX(eLevel, szTemp);                                                     //��
    if (bThrow)                                                                     //��
    {                                                                               //��
        throw(expr);                                                                //��
    }                                                                               //��
}                                                                                   //��
                                                                                    //��
inline VOID ASSERTA_(                                                               //��
	CONST E_ASSERT_LEVEL eLevel,                                                    //��
	CONST BOOL bThrow,                                                              //��
    CONST ACHAR* file,                                                              //��
    CONST DWORD  line,                                                              //��
    CONST ACHAR* func,                                                              //��
    CONST ACHAR* expr,                                                              //��
    CONST ACHAR* str                                                                //��
    )                                                                               //��
{                                                                                   //��
    // �������Ƿ��Ѿ���ʼ��                                                       //��
    if (!g_bAssertInit)                                                             //��
    {                                                                               //��
        MessageBox(NULL, _T("����δ��ʼ��,��������"), _T("����"), MB_OK);           //��
        g_bAssertInit = TRUE;                                                       //��
        g_Command_Assert_Log = COMMAND_IGNORE_LOG;                                  //��
    }                                                                               //��
                                                                                    //��
    if ((COMMAND_IGNORE_LOG == g_Command_Assert_Log) &&                             //��
        (COMMAND_IGNORE_MSGBOX == g_Command_MessageBox))                            //��
    {                                                                               //��
        return;                                                                     //��
    }                                                                               //��
    ACHAR szTempA[ASSERTX_STR_MAX_LEN] = {0};                                       //��
#ifdef UNICODE                                                                      //��
    TCHAR szTempT[ASSERTX_STR_MAX_LEN] = {0};                                       //��
#endif                                                                              //��
    UPDATA_LOCALTIME;                                                               //��
                                                                                    //��
    _snprintf_s(                                                                    //��
        szTempA,                                                                    //��
        sizeof(szTempA),                                                            //��
        "�ļ�: [%s]\n"                                                              //��
        "��  : [%d]\n"                                                              //��
        "����: [%s]\n"                                                              //��
        "ԭ��: [%s]\n"                                                              //��
        "�쳣: [%s]\n"                                                              //��
        "ʱ��: [%d��%d��%d��-����%d"                                                //��
        "[%dʱ%d��%d��%d����]\n",                                                   //��
        file, line, func, expr, str,                                                //��
        LOCALTIME_YEAR, LOCALTIME_MONTH, LOCALTIME_DAY, LOCALTIME_WEEK,             //��
        LOCALTIME_HOUR, LOCALTIME_MINUTE, LOCALTIME_SECOND, LOCALTIME_MILLISECOND   //��
        );                                                                          //��
                                                                                    //��
#ifdef UNICODE                                                                      //��
    // ���ַ�ת���ɶ��ַ��ļ����ַ���                                               //��
    ACharToWChar(szTempA, szTempT, MAX_PATH);                                       //��
    LOG_(szTempT);                                                                  //��
    MESSAGEBOX(eLevel, szTempT);                                                    //��
#else                                                                               //��
    LOG_(szTempA);                                                                  //��
    MESSAGEBOX(eLevel, szTempA);                                                    //��
#endif                                                                              //��
    if (bThrow)                                                                     //��
    {                                                                               //��
        throw(expr);                                                                //��
    }                                                                               //��
}                                                                                   //��
                                                                                    //��
inline VOID ASSERTEXFORMATT(                                                        //��
	CONST E_ASSERT_LEVEL eLevel,                                                    //��
	CONST BOOL bThrow,                                                              //��
    CONST TCHAR* file,                                                              //��
    CONST DWORD  line,                                                              //��
    CONST TCHAR* func,                                                              //��
    CONST TCHAR* expr,                                                              //��
    CONST TCHAR* str,                                                               //��
    ...                                                                             //��
    )                                                                               //��
{                                                                                   //��
    va_list ap;                                                                     //��
    TCHAR szStrBuff[MAX_PATH] = {0};                                                //��
                                                                                    //��
    va_start(ap, str);                                                              //��
    _vsntprintf_s(szStrBuff, ASSERTX_STR_MAX_LEN, str, ap);                         //��
    va_end(ap);                                                                     //��
                                                                                    //��
    // �������Ƿ��Ѿ���ʼ��                                                       //��
    if (!g_bAssertInit)                                                             //��
    {                                                                               //��
        MessageBox(NULL, _T("����δ��ʼ��,��������"), _T("����"), MB_OK);           //��
        g_bAssertInit = TRUE;                                                       //��
        g_Command_Assert_Log = COMMAND_IGNORE_LOG;                                  //��
    }                                                                               //��
                                                                                    //��
    if ((COMMAND_IGNORE_LOG == g_Command_Assert_Log) &&                             //��
        (COMMAND_IGNORE_MSGBOX == g_Command_MessageBox))                            //��
    {                                                                               //��
        return;                                                                     //��
    }                                                                               //��
    TCHAR szTemp[ASSERTX_STR_MAX_LEN] = {0};                                        //��
    UPDATA_LOCALTIME;                                                               //��
                                                                                    //��
    _stprintf_s(                                                                    //��
        szTemp,                                                                     //��
        ASSERTX_STR_MAX_LEN,                                                        //��
        _T("�ļ�: [%s]\n")                                                          //��
        _T("��  : [%d]\n")                                                          //��
        _T("����: [%s]\n")                                                          //��
        _T("ԭ��: [%s]\n")                                                          //��
        _T("�쳣: \n")                                                              //��
        _T("��������������������������������������������\n")                        //��
        _T("%s\n")                                                                  //��
        _T("��������������������������������������������\n")                        //��
        _T("ʱ��: [%d��%d��%d��-����%d[%dʱ%d��%d��%d����]\n"),                     //��
        file, line, func, expr, szStrBuff,                                          //��
        LOCALTIME_YEAR, LOCALTIME_MONTH, LOCALTIME_DAY, LOCALTIME_WEEK,             //��
        LOCALTIME_HOUR, LOCALTIME_MINUTE, LOCALTIME_SECOND, LOCALTIME_MILLISECOND   //��
        );                                                                          //��
                                                                                    //��
    LOG_(szTemp);                                                                   //��
    MESSAGEBOX(eLevel, szTemp);                                                     //��
    if (bThrow)                                                                     //��
    {                                                                               //��
        throw(expr);                                                                //��
    }                                                                               //��
}                                                                                   //��
                                                                                    //��
inline VOID ASSERTEXFORMATA(                                                        //��
	CONST E_ASSERT_LEVEL eLevel,                                                    //��
    CONST BOOL bThrow,                                                              //��
    CONST ACHAR* file,                                                              //��
    CONST DWORD  line,                                                              //��
    CONST ACHAR* func,                                                              //��
    CONST ACHAR* expr,                                                              //��
    CONST ACHAR* str,                                                               //��
    ...                                                                             //��
    )                                                                               //��
{                                                                                   //��
    va_list ap;                                                                     //��
    ACHAR szStrBuff[MAX_PATH] = {0};                                                //��
                                                                                    //��
    va_start(ap, str);                                                              //��
    _vsnprintf_s(szStrBuff, ASSERTX_STR_MAX_LEN, str, ap);                          //��
    va_end(ap);                                                                     //��
                                                                                    //��
    // �������Ƿ��Ѿ���ʼ��                                                       //��
    if (!g_bAssertInit)                                                             //��
    {                                                                               //��
        MessageBox(NULL, _T("����δ��ʼ��,��������"), _T("����"), MB_OK);           //��
        g_bAssertInit = TRUE;                                                       //��
        g_Command_Assert_Log = COMMAND_IGNORE_LOG;                                  //��
    }                                                                               //��
                                                                                    //��
    if ((COMMAND_IGNORE_LOG == g_Command_Assert_Log) &&                             //��
        (COMMAND_IGNORE_MSGBOX == g_Command_MessageBox))                            //��
    {                                                                               //��
        return;                                                                     //��
    }                                                                               //��
    ACHAR szTempA[ASSERTX_STR_MAX_LEN] = {0};                                       //��
#ifdef UNICODE                                                                      //��
    TCHAR szTempT[ASSERTX_STR_MAX_LEN] = {0};                                       //��
#endif                                                                              //��
    UPDATA_LOCALTIME;                                                               //��
                                                                                    //��
    _snprintf_s(                                                                    //��
        szTempA,                                                                    //��
        sizeof(szTempA),                                                            //��
        "�ļ�: [%s]\n"                                                              //��
        "��  : [%d]\n"                                                              //��
        "����: [%s]\n"                                                              //��
        "ԭ��: [%s]\n"                                                              //��
        "�쳣: \n"                                                                  //��
        "��������������������������������������������\n"                            //��
        "%s\n"                                                                      //��
        "��������������������������������������������\n"                            //��
        "ʱ��: [%d��%d��%d��-����%d[%dʱ%d��%d��%d����]\n",                         //��
        file, line, func, expr, szStrBuff,                                          //��
        LOCALTIME_YEAR, LOCALTIME_MONTH, LOCALTIME_DAY, LOCALTIME_WEEK,             //��
        LOCALTIME_HOUR, LOCALTIME_MINUTE, LOCALTIME_SECOND, LOCALTIME_MILLISECOND   //��
        );                                                                          //��
#ifdef UNICODE                                                                      //��
    // ���ַ�ת���ɶ��ַ��ļ����ַ���                                               //��
    ACharToWChar(szTempA, szTempT, MAX_PATH);                                       //��
    LOG_(szTempT);                                                                  //��
    MESSAGEBOX(eLevel, szTempT);                                                    //��
#else                                                                               //��
    LOG_(szTempA);                                                                  //��
    MESSAGEBOX(szTempA);                                                            //��
#endif                                                                              //��
    if (bThrow)                                                                     //��
    {                                                                               //��
        throw(expr);                                                                //��
    }                                                                               //��
}                                                                                   //��
//��������������������������������������������������������������������������������������

#if defined(USE_MS_ASSERTX_FULL)
    #define Assert(level,expr)          ((VOID)((expr)  ? \
        0                                               :\
        (ASSERT_(level, TRUE, _T(__FILE__), __LINE__, _T(__FUNCTION__), _T(#expr), _T("")))))
    #define AssertEx(level,expr,str)    ((VOID)((expr)  ? \
        0                                               :\
        (ASSERT_(level, FALSE, _T(__FILE__), __LINE__, _T(__FUNCTION__), _T(#expr), str))))
    #define AssertA(level,expr)         ((VOID)((expr)  ? \
        0                                               :\
        (ASSERTA_(level, TRUE, __FILE__, __LINE__, __FUNCTION__, #expr), _T(""))))
    #define AssertExA(level,expr,str)   ((VOID)((expr)  ? \
        0                                               :\
        (ASSERTA_(level, FALSE, __FILE__, __LINE__, __FUNCTION__, #expr, str))))
    #define fAssert(level,expr,str,...) ((VOID)((expr)  ? \
        0                                               :\
        (ASSERTEXFORMATT(   \
		level,				\
        TRUE,               \
        _T(__FILE__),       \
        __LINE__,           \
        _T(__FUNCTION__),   \
        _T(#expr),          \
        str,                \
        __VA_ARGS__))))
    #define fAssertEx(level,expr,str,...) ((VOID)((expr)? \
        0                                               : \
        (ASSERTEXFORMATT(   \
		level,				\
		FALSE,              \
        _T(__FILE__),       \
        __LINE__,           \
        _T(__FUNCTION__),   \
        _T(#expr),          \
        str,                \
        __VA_ARGS__))))
    #define fAssertA(level,expr,str,...)((VOID)((expr)  ? \
        0                                               : \
        (ASSERTEXFORMATA(   \
		level,				\
		TRUE,               \
        __FILE__,           \
        __LINE__,           \
        __FUNCTION__,       \
        #expr,              \
        str,                \
        __VA_ARGS__))))
    #define fAssertExA(level,expr,str,...)((VOID)((expr)? \
        0                                               : \
        (ASSERTEXFORMATA(   \
		level,				\
		FALSE,              \
        __FILE__,           \
        __LINE__,           \
        __FUNCTION__,       \
        #expr,              \
        str,                \
        __VA_ARGS__))))
#elif defined(USE_MS_ASSERTX_IMPART_ONLY)
    #define Assert(level,expr,hwnd)
    #define AssertEx(level,expr,str)
    #define AssertA(level,expr,hwnd)
    #define AssertExA(level,expr,str)
#else
    #define fAssert(level,expr,hwnd)
    #define fAssertEx(level,expr,str)
    #define fAssertA(level,expr,hwnd)
    #define fAssertExA(level,expr,str)
#endif

//����ע�ͽ��ͣ�
//#define ��Ϊ��ʹ������������������κκ���

//1���������:(���ñ������Ķ���,˵��������ڴ�����õ����.
//   ���������Դ˺����ĵ���!)
#define NO_PURPOSE_0001 AssertEx(p, "p,�������");

//2�����󴫲�:(�Ժ�������Ĳ�����������,�����˶���,
//   ���麯�����ô��Ƿ��ڲ��õ��õĵط������˸ú�����������쳣!)
#define NO_PURPOSE_0002 AssertEx(p, "p,���󴫲�");

//3������ʧ��:(˵������ִ�з���ʧ�ܻ����,������ú����ľ������ݻ�����Ƿ���ȷ!)
#define NO_PURPOSE_0003 AssertEx(Result == "����", "pfunʧ��,����ʧ��");

//4���ڴ治��:(˵������ǰ����һ�������ڴ����,������ʧ�ܵĽ��!)
#define NO_PURPOSE_0004 AssertEx(p, "p,�ڴ治��");

//5���������:(˵������Խ��,һ����ƫ���Ƿ񳬹�����޶Ȼ�С����С�޶�!
//   ���ܴ��ڴ�����û��߼�����!)
#define NO_PURPOSE_0005 AssertEx(p>&(p[max]), "p,�������");

//6�������쳣:(˵���˴������Ŀ���в���ϣ������,�ڴ���ʵ����ҲҪ�����,
//   �����ǳ�����!Ӧ�ü�������߼��Ƿ����ʹ����д���!)
#define NO_PURPOSE_0006 AssertEx(AL_NORMAL, FALSE, "uNumber,�����쳣");

//����������������������
#endif//#pragma once  ��
//����������������������
