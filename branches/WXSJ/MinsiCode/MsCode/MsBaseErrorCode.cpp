#include "Precompiled.h"

extern LPTCHAR szMsErrorCodeName[MS_ERROR_MAX_COUNT+2] =
{
    _T("δ֪����"),     _T("�����ɹ�"),     _T("�����쳣"),     _T("Ȩ�޴���"),
    _T("���в���"),   _T("���ݲ���"),       _T("δ��ʼ��"),
    //_T("?"),            _T("?"),            _T("?"),            _T("?"),


    _T("ERROR_END")
};

extern LPTCHAR szMsErrorCodeInfo[MS_ERROR_MAX_COUNT+2] =
{
    _T("δ����Ĵ�������"),
    _T("�����ɹ�,û�м�¼�κδ���"),
    _T("������ϵͳ���ʳ�ͻ,���������Ĵ����鴥���쳣"),
    _T("һ��Ϊ����ϵͳȨ�޴���"),
    _T("��������û���㹻�Ŀ���ռ�"),
    _T("��������û���㹻�����ݿ���"),
    _T("���û�ʹ�õ��Ķ���δ���г�ʼ��"),
    //_T("?"),            _T("?"),            _T("?"),            _T("?"),


    _T("ERROR_END")
};

TCHAR* GetErrorCodeName(E_MS_ERROR eMsError)
{
    return szMsErrorCodeName[eMsError+1];
}

TCHAR* GetErrorCodeInfo(E_MS_ERROR eMsError)
{
    return szMsErrorCodeInfo[eMsError+1];
}
