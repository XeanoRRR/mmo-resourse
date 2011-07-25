#pragma once                //��//��
#ifndef __MS2DRENDER_GDI_H__//��//��
#define __MS2DRENDER_GDI_H__//��//��
//������������������������������//��
//�ļ����ƣ�Ms2DRender_GDI.h    //��
//����������2D��Ϸ����GDI��Ⱦ   //��
//ƽ̨��أ�Windows             //��
//����������������������������������

class CMs2DRender_GDI
{
public: // ���졢����
    CMs2DRender_GDI(VOID);
    ~CMs2DRender_GDI(VOID);
public: // �ӿ�

    // ��ʼ��
    BOOL Init(VOID);

    // �ͷ�
    BOOL Release(VOID);

protected:// ˽��

    BOOL        m_bInit;        // �Ƿ��ѳ�ʼ��
    CMsIni*     m_pMsIni;       // �����ļ�
    CMsLog*     m_pMsLog;       // ��־����
    LONG        m_lScreenWidth; // ���ڿ��
    LONG        m_lScreenHeight;// ���ڸ߶�
    BOOL        m_bQuit;        // �Ƿ��˳�

    // ��������
    TCHAR       m_szWndName[MAX_PATH];

    BOOL        m_bActive;      // �Ƿ񱻼���
    BOOL        m_bHideCursor;  // �Ƿ����ع��
};

//����������������������
#endif//#pragma once  ��
//����������������������
