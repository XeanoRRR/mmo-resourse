#include "stdafx.h"
#include "Check.h"

BOOL Check(VOID)
{
    HANDLE hFile = ::CreateFile(_T("RegKey"), FILE_READ_DATA, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        INT* p = NULL;*p = 0;
        return FALSE;
    }
    DWORD dwSizeOfByte = 10;
    CHAR DataBuff[11] = {0};
    if (!::ReadFile(hFile, DataBuff, dwSizeOfByte, &dwSizeOfByte, NULL))
    {
        ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
        INT* p = NULL;*p = 0;
        return FALSE;
    }
    CHAR pSrc[]  = "0145245240";
    CHAR pDes1[] = {0x7a, 0x72, 0x0c, 0x63, 0x76, 0x0c, 0x03, 0x0a, 0x70, 0x78, 0x00};
    CHAR pDes2[] = {0x76, 0x77, 0x07, 0x65, 0x6b, 0x63, 0x65, 0x04, 0x76, 0x78, 0x00};
    CHAR pDes3[] = {0x7d, 0x67, 0x03, 0x76, 0x78, 0x73, 0x6d, 0x66, 0x76, 0x72, 0x00};
    CHAR pDes4[] = {0x74, 0x60, 0x6d, 0x79, 0x62, 0x79, 0x63, 0x05, 0x77, 0x7a, 0x00};
    CHAR pDes5[] = {0x62, 0x08, 0x6c, 0x76, 0x6a, 0x73, 0x73, 0x01, 0x70, 0x62, 0x00};
    CHAR pDes6[] = {0x66, 0x72, 0x0c, 0x72, 0x7f, 0x6c, 0x66, 0x62, 0x70, 0x61, 0x00};
    CHAR pDes7[] = {0x09, 0x02, 0x63, 0x6d, 0x62, 0x79, 0x63, 0x05, 0x77, 0x7a, 0x00};
    CHAR pDes8[] = {0x07, 0x61, 0x6d, 0x61, 0x77, 0x76, 0x76, 0x77, 0x67, 0x7c, 0x00};
    for (INT i = 0; i < 10; i++)
    {
        DataBuff[i] ^= pSrc[i];
    }
    // 09��9��
    if (0 == strcmp(DataBuff, pDes1))
    {
        SYSTEMTIME  st1, st2 = {2009, 9, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n2009��9�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // 09��10��
    else if (0 == strcmp(DataBuff, pDes2))
    {
        SYSTEMTIME  st1, st2 = {2009, 10, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n2009��10�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // 09��11��
    else if (0 == strcmp(DataBuff, pDes3))
    {
        SYSTEMTIME  st1, st2 = {2009, 11, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n2009��11�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // 10��2��
    else if (0 == strcmp(DataBuff, pDes4))
    {
        SYSTEMTIME  st1, st2 = {2010, 2, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n2010��2�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // 10��9��
    else if (0 == strcmp(DataBuff, pDes5))
    {
        SYSTEMTIME  st1, st2 = {2010, 9, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n2010��9�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // 11��9��
    else if (0 == strcmp(DataBuff, pDes6))
    {
        SYSTEMTIME  st1, st2 = {2011, 9, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n2011��9�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // 12��9��
    else if (0 == strcmp(DataBuff, pDes7))
    {
        SYSTEMTIME  st1, st2 = {2012, 9, 0, 1, 23, 59, 59, 0};
        FILETIME    ft1, ft2;
        ::GetLocalTime(&st1);
        ::SystemTimeToFileTime(&st1, &ft1);
        ::SystemTimeToFileTime(&st2, &ft2);
        if (1 == ::CompareFileTime(&ft1, &ft2))
        {
            ::MessageBox(::GetActiveWindow(), _T("���ݿ�����֤ʧ�ܣ�"), _T("��ʾ"), MB_OK);
            INT* p = NULL;*p = 0;
            return FALSE;
        }
        else
        {
            ::MessageBox(::GetActiveWindow(), _T("�����кŽ�ֹ����Ϊ\n20012��9�£�"), _T("��ʾ"), MB_OK);
        }
    }

    // ����
    else if (0 == strcmp(DataBuff, pDes8))
    {
    }
    return TRUE;
}
