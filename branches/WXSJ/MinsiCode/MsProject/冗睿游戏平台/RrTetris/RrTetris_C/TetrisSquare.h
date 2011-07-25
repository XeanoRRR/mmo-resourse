#pragma once                //��//��
#ifndef __TETRISSQUARE_H__  //��//��
#define __TETRISSQUARE_H__  //��//��
//������������������������������//��
//�ļ����ƣ�TetrisSquare.h      //��
//��������������˹������״����  //��
//ƽ̨��أ�Windows             //��
//����������������������������������

// Ĭ�ϵ���״�ļ���
#define DEFAULT_SHAPE_FILE_NAME _T("%s/RrTetris/Shape.sap")

// ����ͼԪ����
#define MAX_METAFILE_COUNT      10

// ͼԪ�����
#define MAX_METAFILE_WIDTH      16
#define MAX_METAFILE_HEIGHT     16

// һ�������м��ֱ���״̬
#define SHAPE_DISTORTION_STATE  4

// ������״
class CT_Square
{
public:
    CT_Square(VOID);
    ~CT_Square(VOID);

    BOOL InitMetafile(WORD wMetafileIndex);     // ��ʼ��ͼԪ���ݳ�Ա
    BOOL InitShape(WORD wShapeIndex);           // ��ʼ����״���ݳ�Ա

    VOID GetShapePos(NPOS& uX, NPOS& uY);       // ��ȡ��״�ڳ����е�����
    VOID SetShapePos(NPOS X, NPOS Y);           // ������״�ڳ����е�����

    VOID GetShapeShadowPos(NPOS& uX, NPOS& uY); // ��ȡ��״��Ӱ�ڳ����е�����X
    VOID SetShapeShadowPos(NPOS X, NPOS Y);     // ������״��Ӱ�ڳ����е�����

    BOOL CreateShape(DWORD dwBackSceneWidth);   // ��������
    VOID Clear(VOID);                           // �����״

    CT_ShapeFile* GetShapeFile(VOID);           // ��ȡ��״�ļ�

    // ��״����
    BOOL Distortion(
        DWORD dwBackSceneWidth,
        DWORD dwBackSceneHeight,
        BOOL bOrder = TRUE
        );

    BYTE GetVoidCountByLeft(VOID)       {return m_VoidCountByLeft;}
    BYTE GetVoidCountByRight(VOID)      {return m_VoidCountByRight;}
    BYTE GetVoidCountByUp(VOID)         {return m_VoidCountByUp;}
    BYTE GetVoidCountByDown(VOID)       {return m_VoidCountByDown;}

    // ��ȡ��״����
    WORD GetCurrentShapeIndex(VOID)     {return m_wCurrentShapeIndex;}
    WORD GetNext_ShapeIndex(VOID)       {return m_wNext_ShapeIndex;}
    WORD GetNext2_ShapeIndex(VOID)      {return m_wNext2_ShapeIndex;}
    WORD GetNext3_ShapeIndex(VOID)      {return m_wNext3_ShapeIndex;}

    // ��ȡ��״ͼԪ����
    WORD GetCurrentMetafileIndex(VOID)  {return m_wCurrentMetafileIndex;}
    INT GetNext_MetafileIndex(VOID)     {return m_wNext_MetafileIndex;}
    INT GetNext2_MetafileIndex(VOID)    {return m_wNext2_MetafileIndex;}
    INT GetNext3_MetafileIndex(VOID)    {return m_wNext3_MetafileIndex;}

    // ��ȡ��״����ָ��
    LPBYTE GetCurrentShapeBuff(VOID)    {return m_pCurrentShape;}
    LPBYTE GetNext_ShapeBuff(VOID)      {return m_pNext_Shape;}
    LPBYTE GetNext2_ShapeBuff(VOID)     {return m_pNext2_Shape;}
    LPBYTE GetNext3_ShapeBuff(VOID)     {return m_pNext3_Shape;}

private:

    CT_ShapeFile*       m_pShapeFile;       // ��״�ļ�
    CMsGdiPlusImage*    m_pBitmapMetafile;  // ��״��ʹ�õ�ͼԪͼƬ

    WORD    m_wCurrentMetafileIndex;    // ��ǰͼԪ��ͼƬ�е�����λ��
    WORD    m_wNext_MetafileIndex;      // ��һ��ͼԪ��ͼƬ�е�����λ��
    WORD    m_wNext2_MetafileIndex;     // ����һ��ͼԪ��ͼƬ�е�����λ��
    WORD    m_wNext3_MetafileIndex;     // ������һ��ͼԪ��ͼƬ�е�����λ��

    LPBYTE  m_pCurrentShape;            // ��ǰ������״����ָ��
    LPBYTE  m_pNext_Shape;              // ��һ��������״����ָ��
    LPBYTE  m_pNext2_Shape;             // ����һ��������״����ָ��
    LPBYTE  m_pNext3_Shape;             // ������һ��������״����ָ��

    WORD    m_wCurrentShapeIndex;       // �������״����
    WORD    m_wNext_ShapeIndex;         // ��һ���������״����
    WORD    m_wNext2_ShapeIndex;        // ����һ���������״����
    WORD    m_wNext3_ShapeIndex;        // ������һ���������״����

    UPOS    m_nMetafilePosX;            // ͼԪ��ͼƬ�еĺ�����
    UPOS    m_nMetafilePosY;            // ͼԪ��ͼƬ�е�������
    NPOS    m_SquarePosX;               // ��״�ڳ����еĺ�����
    NPOS    m_SquarePosY;               // ��״�ڳ����е�������

    BYTE    m_VoidCountByLeft;          // ��ǰ��״����߿հ׸���
    BYTE    m_VoidCountByRight;         // ��ǰ��״���ұ߿հ׸���
    BYTE    m_VoidCountByUp;            // ��ǰ��״���ϱ߿հ׸���
    BYTE    m_VoidCountByDown;          // ��ǰ��״���±߿հ׸���

    DWORD   m_dwShapeCount;             // ��״�ܸ���

    NPOS    m_SquareShadowPosX;         // ��״��Ӱ�ڳ����еĺ�����
    NPOS    m_SquareShadowPosY;         // ��״��Ӱ�ڳ����е�������
};



//����������������������
#endif//#pragma once//��
//����������������������
