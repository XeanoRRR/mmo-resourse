#pragma once                    //��//��
#ifndef __GAMETREESCROLLBAR_H__ //��//��
#define __GAMETREESCROLLBAR_H__ //��//��
//����������������������������������//��
//�ļ����ƣ�GameTreeScrollbar.h     //��
//������������Ϸ�б������          //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ���������
#define SCROLL_BAR_WIDTH    20
// �������߶�
#define SCROLL_BAR_HEIGHT   50

enum E_DIRECTION
{
    TRANSVERSE      = 0x0001,   // �����
    LOGNITUDINAL    = 0x0002,   // �����
};

// ��Ϸ�б������
class CGameTreeScrollbar
    : public CMsGdiPlusImage
{
public:
    CGameTreeScrollbar(CMsGdiBuffImage* lpCMsGdiBuffImage, INT nBasePosX, INT nBasePosY, DWORD dwWidth, DWORD dwHeight, DWORD eDirection);
    ~CGameTreeScrollbar(VOID);

    // ����Ƿ񴩹�������
    BOOL IsOnScrollbar(POINT& Pos);

    // �ƶ�������
    VOID MoveScrollX(INT nOffsetX);
    VOID MoveScrollY(INT nOffsetY);

    // ���������ֵ
    VOID AddMaxMoveScrollX(INT nMaxOffset);
    VOID SubMaxMoveScrollX(INT nMaxOffset);
    VOID AddMaxMoveScrollY(INT nMaxOffset);
    VOID SubMaxMoveScrollY(INT nMaxOffset);

    // ���ƹ�����
    BOOL DrawScrollbar(VOID);
private:
    DWORD   m_eDirection;   // ����������
    INT     m_nBasePosX;    // ������λ��X
    INT     m_nBasePosY;    // ������λ��Y
    INT     m_nOffsetX;     // ������ƫ��X
    INT     m_nOffsetY;     // ������ƫ��Y
    INT     m_nMaxOffsetX;  // ���������ƫ��X
    INT     m_nMaxOffsetY;  // ���������ƫ��Y
    DWORD   m_dwWidth;      // ���������
    DWORD   m_dwHeight;     // �������߶�
};

//����������������������
#endif//#pragma once  ��
//����������������������
