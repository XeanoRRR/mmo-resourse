/// 
/// @file ScriptGlobalFunc.h
/// @brief Script�ڵ�ȫ��ʹ�ú���(GM)
///

// class Script {
///
/// ��ָ�������µ��������
/// @param regionID ����GUID
///
void KickRegionPlayer( const CGUID &regionID );

///
/// ��ȫ���������
///
void KickAllPlayer();

///
/// ��ָ�����
/// @param name �������
///
void KickPlayer( const char *name );

///
/// ��ʾ����GM
///
int ListOnlineGM();

///
/// ��ʾ�������
///
void ListSelencePlayer();

///
/// ���ָ�����
/// @param name �������
/// @param time ���ʱ��
///
void BanPlayer( const char *name, long time );

///
/// ��ȡ�ַ�����Դ
/// @param type ��Դ����
/// @param id ��ԴID
/// @return �ַ���
///
const char *GetTextRes( const char* id );

///
/// ���ͳ�����������
/// @param rgnType ��������
/// @param num 
/// @param msg
///
void RollRegionInfo( long rgnType, long num, const char *msg, long fileNum, long rgnID = -1 );

///
/// ���ͷ���������
///
void RollWorldInfo( long num, const char *str, long fileNum );

///
/// ����һ����ҵ�ָ������
///
/// @param rgnType Ŀ�곡������
/// @param centerPlayer �����������
/// @param x Ŀ��X����
/// @param y Ŀ��Y����
///
void ChangePlayerRegionEx( long rgnType, const char *centerPlayer, const CGUID &rgnID, long x, long y );
// };