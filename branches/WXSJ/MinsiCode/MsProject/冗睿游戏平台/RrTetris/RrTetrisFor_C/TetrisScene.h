#pragma once                    //��//��
#ifndef __TETRISBACKSCENE_H__   //��//��
#define __TETRISBACKSCENE_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�TetrisBackScene.h       //��
//��������������˹���鳡������      //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ����˹���鳡�����ŷ���������(��Ϸ��ƾ���,��Ҫ���׸Ķ�)
#define BACKSCENE_MAX_SQUARE_COUNT_X    25

// ����˹���鳡�����ŷ���������(��Ϸ��ƾ���,��Ҫ���׸Ķ�)
#define BACKSCENE_MAX_SQUARE_COUNT_Y    21

// ������Ԫ�ؽṹ��
typedef struct BACK_SCENE_BYTE 
{
    BYTE m_Square;          // ��������
    WORD m_MetafileIndex;   // ����ʹ�õ�ͼԪ��������
    BYTE m_Reserve;         // �ڴ���䱣��
}BSB;

class CT_Scene
{
public: // ���졢����
    CT_Scene(VOID);
    ~CT_Scene(VOID);

public: // �ӿ�

    // ��ʼ��������
    BOOL Init(
        WORD wBackSceneMetafileIndex,
        DWORD dwBackSceneWidth,
        DWORD dwBackSceneHeight
        );

    // ��ȡ����Ԫ�������׵�ַ
    VOID GetBackSceneSize(
        DWORD& dwBackSceneWidth,
        DWORD& dwBackSceneHeight
        );

    // ���ó���
    BOOL Reset(
        WORD wBackSceneMetafileIndex,
        DWORD dwBackSceneWidth,
        DWORD dwBackSceneHeight
        );

    VOID    Release(VOID);                                          // �ͷų���
    WORD    GetBackMetafileIndex(VOID);                             // ���ó���ͼԪ����
    BOOL    SetBackMetafileIndex(WORD wBackSceneMetafileIndex);     // ���ó���ͼԪ����
    BOOL    SetFrontMetafileIndex(WORD wFrontSceneMetafileIndex);   // ����ǰ��ͼԪ����
    BSB*    GetBackSceneBuff(VOID);                                 // ��ȡ����Ԫ�������׵�ַ

private:// ˽��
    WORD                m_wBackSceneMetafileIndex;  // ������ͼԪ����
    BSB*                m_pSceneBYTE;               // ����Ԫ������ָ��
    DWORD               m_dwBackSceneWidth;         // �����Ŀ��
    DWORD               m_dwBackSceneHeight;        // �����ĸ߶�
};


//����������������������
#endif//#pragma once//��
//����������������������
