/**
 *  brief ����MessageBox�����Ͷ���
 *  date  2010/06/12
 */
#pragma  once 

/// ѡ����
enum   eMsgResult
{
    S_INValid = -1,
    S_Ok,
    S_Mk, 
    S_Cancel,
    S_Esc,
};

// ��Ϣ��ʾ��ķ��
enum eMsgBoxType
{
    Msg_Ok,        ///> ֻ��һ��ȷ����ť
    Msg_YesNo,     ///> �ǣ���
    Msg_GoOn,      ///> �������˳�
    Msg_Agree,     ///> ͬ�⣬��ͬ��
    Msg_Ok_Yes_No, ///> ȷ�����ܾ���ȡ��
};