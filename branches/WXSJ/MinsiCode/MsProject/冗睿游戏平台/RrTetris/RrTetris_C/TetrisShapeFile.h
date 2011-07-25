#pragma once                    //��//��
#ifndef __TETRISSHAPEFILE_H__   //��//��
#define __TETRISSHAPEFILE_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�TetrisShapeFile.h       //��
//��������������˹������״�ļ�      //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ��״�ļ�ͷ��Ǻ궨��
#define SHAPE_FILE_FILG     0x45504153 // SAPE

// ��״�ļ�ͷ�汾��
#define SHAPE_FILE_VERSION  0x00000001

// ��״�ļ���ÿ����״��ռ�ֽ���
#define SHAPE_FILE_ONE_SHAPE_SIZE   16

// �궨�巽����
#define SHAPE_MAX_WIDTH     4
#define SHAPE_MAX_HEIGHT    4

// �궨����ڷ���
#define HAVE_SQUARE TRUE

// �궨�岻���ڷ���
#define NO_SQUARE   FALSE

class CT_ShapeFile
{
public:
    typedef struct SHAPE_FILE_HEAD 
    {
        DWORD   m_FileFlag;     // ��״�ļ�ͷ�ı��
        DWORD   m_Version;      // ��״�ļ�ͷ�İ汾��
        DWORD   m_ShapeCount;   // ��״�ļ�ͷ����״����
        DWORD   m_Hold;         // ��״�ļ�ͷ�ı���(4�ֽ�)
    }SFH;

    CT_ShapeFile(TCHAR* strShapeFileName);
    ~CT_ShapeFile(VOID);

    DWORD   GetShapeCount(VOID);                        // ��ȡ��״����
    LPBYTE  GetShapeByIndex(WORD wIndex);               // ��ȡ��״����
    BYTE    GetShapeUsedSquareByLeft(WORD wIndex);      // ��ȡ��һ���з����λ�ô���
    BYTE    GetShapeUsedSquareByRight(WORD wIndex);     // ��ȡ��һ���з����λ�ô���
    BYTE    GetShapeUsedSquareByUp(WORD wIndex);        // ��ȡ��һ���з����λ�ô���
    BYTE    GetShapeUsedSquareByDown(WORD wIndex);      // ��ȡ��һ���з����λ�ô���
private:

    LPBYTE  m_pData;        // ��״�ļ����ݻ�����
    DWORD   m_dwFileSize;   // ��״�ļ��ĳߴ�
    HANDLE  m_hShapeFile;   // ��״�ļ����
    SFH     m_ShapeFileHead;// ��״�ļ�ͷ
};


//����������������������
#endif//#pragma once//��
//����������������������
