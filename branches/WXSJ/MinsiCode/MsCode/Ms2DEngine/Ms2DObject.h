#pragma once            //��//��
#ifndef __MS2DOBJECT_H__//��//��
#define __MS2DOBJECT_H__//��//��
//��������������������������//��
//�ļ����ƣ�Ms2DObject.h    //��
//����������2D����:         //��
//          ��Ϸ�������    //��
//ƽ̨��أ�Windows         //��
//������������������������������

//������������������MS2DEngine����������������������
                                                //��
// 2D����,������󳤶�                          //��
#define _2DE_MAX_OBJ_NAME_LEN           10      //��
                                                //��
// 2D����,�������ߴ�                          //��
#define _2DE_MAX_OBJ_SIZE               100.0f  //��
                                                //��
// 2D����,��������ƶ��ٶ�                      //��
#define _2DE_MAX_OBJ_MOVE_SPEED         100.0f  //��
                                                //��
                                                //��
//��������������������������������������������������

class CMs2DObject
{
public:
    CMs2DObject(VOID);
    ~CMs2DObject(VOID);

    // ����
    CONST TCHAR*    GetName(VOID);
    VOID            SetName(CONST TCHAR* szName, DWORD dwNameLen);
    DWORD           GetNameLen(VOID);

    // ����
    VOID GetPos(FPOS& fPosX, FPOS& fPosY);
    VOID SetPos(FPOS fPosX, FPOS fPosY);

    // �ߴ�
    VOID GetSize(FSIZE& fWidth, FSIZE& fLength);
    VOID SetSize(FSIZE fWidth, FSIZE fLength);

    // ����(��ת�Ƕ�)
    FLOAT   GetRotary(VOID);
    VOID    SetRotary(FLOAT fRotary);

    // �ٶ�
    FLOAT   GetWalkSpeed(VOID);
    VOID    SetWalkSpeed(FLOAT fWalkSpeed);
    FLOAT   GetRunSpeed(VOID);
    VOID    SetRunSpeed(FLOAT fRunSpeed);

protected:

    // �����������
    TCHAR   m_szName[_2DE_MAX_OBJ_NAME_LEN];// ����
    FPOS    m_fPosX;                        // X����
    FPOS    m_fPosY;                        // Y����
    FSIZE   m_fWidth;                       // ���
    FSIZE   m_fLength;                      // ����
    FLOAT   m_fRotary;                      // ����(��ת�Ƕ�)
    FLOAT   m_fWalkSpeed;                   // ��·�ٶ�
    FLOAT   m_fRunSpeed;                    // �ܲ��ٶ�



    // �߼���������
    DWORD       m_dwInPoolIndex;            // �ڳ����е�����
    FILETIME    m_ftLandTime;               // ��½ʱ��
};


//����������������������
#endif//#pragma once  ��
//����������������������
