/// 
/// @file ScriptTauntFunc.h
/// @brief ��������غ���
///
// class Script {
///
/// ȡ�ó���б���ָ�������ĳ��ֵ
///
/// @param index ��������0��ʼ
/// @return ���ֵ��-1��ʾδ�ҵ�
///
int GetTauntValue( int index );

///
/// ����ָ������ĳ��ֵ
///
/// @param id ����GUID
/// @param value ���ֵ
///
void SetTauntValue( const CGUID &id, int value );

///
/// ȡ�ó���б���ָ��GUID����ĳ��ֵ
///
/// @param id ����GUID
/// @return ���ֵ��-1��ʾδ�ҵ�
///
int GetTauntValue( const CGUID &id );

///
/// �ڳ���б������ѡ��ָ�����͵Ķ���
///
/// @param type ��������
/// @return ����GUID���б�Ϊ�շ���NULL_GUID
///
CGUID GetRandObject( long type );

///
/// �ó���б���ָ�����͵Ķ���ִ��һ���ű�
///
/// @param type ��������
/// @param file �ű���
/// 
void ObjectRunScript( long type, const char *file );

///
/// ǿ���趨Ϊ����б�ĵ�һλ
///
/// @param id ����ID
///
bool ForceToFirst( const CGUID &id );

///
/// ���س���б���
///
int GetTauntSize();

///
/// ʩ���߶�ĳ�����ͷ����漼�ܣ��ڼ����˺��ű���ʹ�á�
///
/// @see GetGUIDByParam
/// @param firer ʩ����GUID
/// @param owner ���BUFF��GUID
/// @param tvalue ���ӣ���ã��ĳ��ֵ
///
void TauntEvents( const CGUID &firer, long ftype, const CGUID &owner, long otype, int tvalue );

// }
