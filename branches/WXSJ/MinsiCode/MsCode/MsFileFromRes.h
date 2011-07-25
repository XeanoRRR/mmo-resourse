#pragma once              //��            //��
#ifndef __FILEFROMRES_H__ //��            //��
#define __FILEFROMRES_H__ //��            //��
//����������������������������            //��
//�ļ����ƣ�FileFromRes.h                 //��
//�����������ļ�����Դ���ͷ���ز�����װ  //��
//ƽ̨��أ�WINDOWS                       //��
//��������������������������������������������

class CMsFileFromRes
{
public:
    CMsFileFromRes(HINSTANCE hInstance);
    virtual ~CMsFileFromRes(VOID);

    // ���нӿ�
public:

    // �����ļ�����Դ
    BOOL    CreateFileFromRes(INT ID, TCHAR* strResType, TCHAR* szFilePath);

    // ������Դ
    LPVOID  LoadFileFromRes(INT ID, TCHAR* strResType, DWORD* lpdwFileSize);

    // ��������
protected:

    // ��ǰӦ�ó���ʵ�����
    HINSTANCE   m_hInstance;

    // ��Դ�����ַ���
    TCHAR       m_strResType[MAX_PATH];

    // ��Դ�ļ����
    HANDLE      m_hResFile;
};



//����������������������
#endif//#pragma once//��
//����������������������
