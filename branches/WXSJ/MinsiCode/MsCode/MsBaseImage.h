#pragma once                //��//��
#ifndef __MSBASEIMAGE_H__   //��//��
#define __MSBASEIMAGE_H__   //��//��
//������������������������������//��
//�ļ����ƣ�MsBaseImage.h       //��
//����������ͼƬ����            //��
//ƽ̨��أ�WINDOWS             //��
//����������������������������������

class CMsBaseImage
{
public:
    // ���졢����
    CMsBaseImage(VOID);
    virtual ~CMsBaseImage(VOID);

    // ��ͼƬ
    virtual BOOL Open(TCHAR* szImageFileName) = 0;

    // ����ͼƬ
    virtual BOOL Save(TCHAR* szImageFileName) = 0;

    // �ر�ͼƬ
    virtual VOID Close(VOID) = 0;

    // ����ͼƬ
    virtual BOOL Draw(HDC hDc, INT x, INT y, RECT DesRc) = 0;
private:
};



//����������������������
#endif//#pragma once//��
//����������������������
