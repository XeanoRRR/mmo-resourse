#pragma once            //��//��
#ifndef __MSSTRING_H__  //��//��
#define __MSSTRING_H__  //��//��
//��������������������������//��
//�ļ����ƣ�MsString.h      //��
//�����������ַ���������װ  //��
//ƽ̨��أ�Windows         //��
//������������������������������

class CMsString
{
public:

    // �ַ�����
    enum E_CHAR_TYPE
    {
        CY_A_CHAR,  // ���ַ�
        CY_W_CHAR,  // ���ַ�
    };

    // ����
    CMsString(DWORD dwMaxBuffLen, BOOL bChangeCharacterSet = FALSE);  

    // ���ַ�����
    CMsString(LPCHAR szText, DWORD dwMaxBuffLen, BOOL bChangeCharacterSet = FALSE);           

    // ���ַ�����
    CMsString(LPWCHAR szText, DWORD dwMaxBuffLen, BOOL bChangeCharacterSet = FALSE);          

    // ����
    ~CMsString(VOID);

    // ��ȡ���ַ���
    operator LPCHAR();

    // ��ȡ���ַ���
    operator LPWCHAR();

    // ��ȡ�ַ�������(�ַ�����)
    size_t GetTextLen(VOID);   

private:
    LPCHAR      m_strTextA;             // ���ַ���
    LPWCHAR     m_strTextW;             // ���ַ���
    DWORD       m_dwMaxBuffLen;         // �ַ����������ߴ�
    BOOL        m_bChangeCharacterSet;  // �Ƿ�����ת���ַ���
    size_t      m_stTextLen;            // �ַ�������
};

//����������������������
#endif//#pragma once//��
//����������������������
