/// 
/// @file ScriptShapeFunc.h
/// @brief Script�ڵ�Shape��غ���
/// @details �����漰���������͵ģ�TYPE_PLAYER��ʾ������ͣ�TYPE_MONSTER��ʾ�������͡�
///

// class Script {
///
/// ����Shape(����/���)����
///
/// @param type Shape����: TYPE_PLAYER, TYPE_MONSTER
/// @param id Shape��GUID
/// @param attrName Ҫ���õ�������
/// @param attrVal �µ�����ֵ�����֣�
///
void SetAttr( long type, const CGUID &id, const char *attrName, double attrVal );

///
/// ��ȡShape(����/���)������
/// @param type Shape���ͣ�TYPE_PLAYER, TYPE_MONSTER
/// @param id Shape��GUID
/// @param attrName Ҫ��ȡ��������
/// @return ���صõ�������ֵ�����֣�
///
double GetAttr( long type, const CGUID &id, const char *attrName );

///
/// �ı�Shape���ԣ�ִ�����Լӷ�����
/// @see SetAttr
///
void ChangeAttr( long type, const CGUID &id, const char *attrName, double attrVal );

///
/// ����ָ���ı���
/// @param motion ����ID
/// @param around Ϊ1ʱ�㲥����Χ���
///
void PlayMotion( long motion, long around );

///
/// ��ָ�����������нű�����ָ�������Ϊ�µĽű�ʵ����Դ����
/// @param type ��������
/// @param id ����GUID
/// @param script �ű��ļ���
///
void ShapeRunScript( long type, const CGUID &id, const char *script );

///
/// ��ȡ��Χ�������
/// @param range ��Χ
/// @param diedFlag [��ѡ]ֵΪ0�򲻻�ȡ�������
/// @param hideFlag [��ѡ]ֵΪ0�򲻻�ȡ�������
/// @return �������
///
long GetAroundPlayerNum( long range, long diedFlag = 0, long hideFlag = 0 );

///
/// ��ȡ��Χ��������
/// @param range ��Χ
/// @return ���ع�������
///
long GetAroundMonsterNum( long range );

///
/// ��ȡ��Χ��Դ������ͬ���͵Ĺ��Դ���������Ϊ����
/// @param range ��Χ
/// @return ���ع�������
///
long GetAroundSameMonsterNum( long range );

/// 
/// Ϊָ����������ɾ����ʱ��
/// @param type ��������
/// @param id ����GUID
/// @param delTime ����ʱ���ɾ��
///
void SetDelTime( long type, const CGUID &id, long delTime );

///
/// �ı��ƶ��ٶ�
/// @param type ��������
/// @param id ����GUID
/// @param speed �ٶ�ֵ
///
void ChangeMoveSpeed( long type, const CGUID &id, short speed );

///
/// �����ƶ��ٶ�
/// @param type ��������
/// @param id ����GUID
/// @param speed �ٶ�ֵ
///
void SetMoveSpeed( long type, const CGUID &id, short speed );

///
/// ��ȡ��ǰ�ٶ�
/// @param type ��������
/// @param id ����GUID
/// @return �ٶ�ֵ
///
float GetCurrentSpeed( long type, const CGUID &id );

/// 
/// �ж�ָ�������Ƿ���Ч
/// @param type ��������
/// @param id ����GUID
/// @return ����true��ʾ������Ч��������Ч
///
bool IsShapeValid( long type, const CGUID &id );

// };