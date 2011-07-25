#pragma once                    //��      //��
#ifndef __FINDFILEMANAGER_H__   //��      //��
#define __FINDFILEMANAGER_H__   //��      //��
//����������������������������������      //��
//�ļ����ƣ�FindFileManager.h             //��
//����������Ŀ¼�����ļ�������ط�װ      //��
//ƽ̨��أ�WINDOWS                       //��
//��������������������������������������������

class CMsFindFileManager
{
public:
    CMsFindFileManager(VOID);
    virtual ~CMsFindFileManager(VOID);

    // ����������Ŀ¼
    BOOL            SetRootPath(CONST TCHAR* strPath = NULL);

    // ���������ļ�����
    BOOL            SetFindFileType(CONST TCHAR* strFileType);

    // ����Ŀ¼����ָ�����͵��ļ�
    BOOL            BrowsePath(VOID);

protected:

    // ��������ļ������Ƿ�ƥ��
    BOOL            CheckFileType(CONST TCHAR* strPath);

    // ·��Ĭ��׷��ָ���ַ�
    BOOL            AppPath(TCHAR* strPath, TCHAR cChr = _T('\\'));

    // ��������ÿ�ҵ�һ���ļ�,�͵���ProcessFile,�����ļ�����Ϊ�������ݹ�ȥ
    // �û����Ը�д�ú���,�����Լ��Ĵ������
    virtual BOOL    ProcessFile(CONST TCHAR*) = 0;

    // ��������ÿ����һ��Ŀ¼,�͵���ProcessPath,
    // �������ڴ����Ŀ¼����Ϊ�������ݹ�ȥ,
    // �û����Ը�д�ú���,�����Լ��Ĵ������
    virtual BOOL    ProcessPath(CONST TCHAR*) = 0;

protected:
    // ��ŵ�ǰ��ִ���ļ�·��
    TCHAR           m_szCurrentDirectory[MAX_PATH];

    // ----------------------------------------
    // ������Ŀ¼
    TCHAR           m_szRootPath[MAX_PATH];
    // ������Ŀ¼����
    INT             m_nRootPathLen;
    // ----------------------------------------


    // ----------------------------------------
    // �����ļ�����
    TCHAR           m_szFileType[MAX_PATH];
    // �����ļ����ͳ���
    INT             m_nFileTypeLen;
    // ----------------------------------------


    // ----------------------------------------
    // ��Ҫ���ҵľ���·�������͵��ַ���
    TCHAR           m_szRealFindPathName[MAX_PATH];

    // ��Ҫ���ҵľ���·�������͵��ַ���
    INT             m_nRealPathLen;
    // ----------------------------------------

    // �����ļ�������ؽṹ��
    WIN32_FIND_DATA m_FindFileData;
};

//����������������������
#endif//#pragma once//��
//����������������������
