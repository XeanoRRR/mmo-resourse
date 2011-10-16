/// 
/// @file ScriptMonsterFunc.h
/// @brief Script�ڵĲ�����������
///

// class Script {
///
/// ��Ӷ�ʱ���ù���AI����
///
/// @param millSecs AI������ʱ�䣨���룩
/// @return 1��ʾ�ɹ���0��ʾʧ��
///
int ContinueAI( ulong millSecs );

///
/// ���������Ϊ���ƶ���ָ������
///
/// @param x, y Ŀ������
///
void MoveToCell( long x, long y );

///
/// ���������Ϊ������ƶ�һ��ʱ��
///
/// @param time �ƶ���ʱ�䣨���룩
///
void RandomMove();

///
/// ���������Ϊ����ָ�������ƶ�ָ������
///
/// @param direct ����
/// @param step ����
///
void MoveByStep( long direct, long step );

///
/// ���������Ϊ������ָ��Ŀ��һ��ʱ��
///
/// @param targetID Ŀ��GUID
/// @param targetType Ŀ������
/// @param time ָ��ʱ��
///
void FollowTarget( const CGUID &targetID, long targetType, long time );

///
/// ȡ�õ�ǰ����Ŀ��
/// targetID, targetType = GetTarget()
///
/// @return targetID Ŀ��GUID
/// @return targetType Ŀ������
///
CGUID GetTarget( long *targetType = 0 );

//}