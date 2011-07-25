#pragma once                    //��//��
#ifndef __MS2DRENDER_DX10_H__   //��//��
#define __MS2DRENDER_DX10_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�Ms2DRender_DX10.h       //��
//����������2D��Ϸ����DX10��Ⱦ      //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

class CMs2DRender_DX10
{
public: // ���졢����
    CMs2DRender_DX10(VOID);
    ~CMs2DRender_DX10(VOID);
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
