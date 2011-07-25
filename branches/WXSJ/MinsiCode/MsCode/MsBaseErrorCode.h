#pragma once                    //��//��
#ifndef __MSBASEERRORCODE_H__   //��//��
#define __MSBASEERRORCODE_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�MsBaseErrorCode.h       //��
//��������������������            //��
//ƽ̨��أ�WINDOWS                 //��
//��������������������������������������

enum E_MS_ERROR
{
    // ����ʧ��,δ֪����
    MS_ERROR_UNKNOW             = 0xFFFFFFFF,

    // �����ɹ�,û�д���
    MS_ERROR_SUCCEED            = 0x00000000,

    MS_ERROR_EXCEPTION,                 // ����ʧ��,�����쳣
    MS_ERROR_ACCESS,                    // ����ʧ��,Ȩ�޴���
    MS_ERROR_SUB_CALL_ACCESS,           // ����ʧ��,����Ȩ�޴���

    MS_ERROR_BUFFER_NOT_ENOUGH_FREE,    // ����ʧ��,��������û���㹻�Ŀ���ռ�
    MS_ERROR_BUFFER_NOT_ENOUGH_DATA,    // ����ʧ��,��������û���㹻�����ݿ���

    MS_ERROR_INVALID_PARAMETER,         // ����ʧ��,������Ч
    MS_ERROR_NO_INIT,                   // ����ʧ��,����û�н��г�ʼ��

    MS_ERROR_MAX_COUNT                  // ���������
};

extern LPTCHAR  szMsErrorCodeName[MS_ERROR_MAX_COUNT+2];
extern LPTCHAR  szMsErrorCodeInfo[MS_ERROR_MAX_COUNT+2];
TCHAR* GetErrorCodeName(E_MS_ERROR eMsError);
TCHAR* GetErrorCodeInfo(E_MS_ERROR eMsError);

//����������������������
#endif//#pragma once//��
//����������������������
