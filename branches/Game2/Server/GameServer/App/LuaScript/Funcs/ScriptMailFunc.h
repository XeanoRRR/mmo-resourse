/// 
/// @file ScriptMailFunc.h
/// @brief Script�ڵ��ʼ���������
///

// class Script {
///
/// ��ȡ�ʼ�
/// @param playerID ���GUID
/// @return true��ʾ�ɹ�����
///
bool ReceiveMail( const CGUID &playerID );

///
/// ����ϵͳ�ʼ���Դ���
/// @param mailID �ʼ�ID
/// @param writer ����������
/// @return true�ɹ�
///
bool SendSysMail( ulong mailID, const char *writer );

///
/// ��ȡϵͳ�ʼ�
///
void ReceiveSysMail();

// }
