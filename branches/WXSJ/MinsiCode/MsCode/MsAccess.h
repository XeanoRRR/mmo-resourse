#pragma once            //��//��
#ifndef __MSACCESS_H__  //��//��
#define __MSACCESS_H__  //��//��
//��������������������������//��
//�ļ����ƣ�MsAccess.h      //��
//����������MFC_DAO_DB      //��
//ƽ̨��أ�WINDOWS         //��
//������������������������������

// SQL��ѯ����ַ���������󳤶�
#define MAX_RESULT_STRING_LEN   4096

class CMsAccess
{
public:
    // ���졢����
    CMsAccess(VOID);
    virtual ~CMsAccess(VOID);

    // �����ݿ��ļ�
    BOOL Open(TCHAR* szDBName);

    BOOL Close(VOID);                   // �ر����ݿ��ļ�
    VOID ExecuteSQL(TCHAR* szSQLText);  // ִ��SQL���
    LONG GetResultCount(VOID);          // ��ȡ�����¼����
    SHORT GetFieldCount(VOID);          // ��ȡ����ֶθ���
    
    VOID MoveFirst(VOID);   // ��һ��
    VOID MoveLast(VOID);    // ���һ��
    VOID MovePrev(VOID);    // ��һ��
    VOID MoveNext(VOID);    // ��һ��

    COleVariant* GetResultValue(TCHAR* szFieldName);// ��ȡ�����¼,�����ֶ���
    COleVariant* GetResultValue(INT nIndex);        // ��ȡ�����¼,�����ֶ�����
private:
    CDaoDatabase*   m_pCDaoDatabase;    // DAO���ݿ����ָ��
    CDaoRecordset*  m_pCDaoRecordset;   // DAO���ݿ���ָ��
    CDaoException*  m_pCDaoException;   // DAO���ݿ��쳣ָ��
    COleVariant     m_COleVariant;      // DAOִ�н��
    TCHAR           m_ResultBuff[MAX_RESULT_STRING_LEN];
    CDaoTableDefInfo    m_CDaoTableDefInfo; // DAO���ݿ����Ϣ
};  

//����������������������
#endif//#pragma once//��
//����������������������
