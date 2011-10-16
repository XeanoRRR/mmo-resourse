/// 
/// @file ScriptClientFunc.h
/// @brief ��ͻ��˽����ĺ���
/// @details ��������ɫ��صĲ�������������ARGB����õ�
///

// class Script {
///
/// ����������Ϣ��Դ����
/// @param str ������Ϣ
/// @param fgColor ������ɫ
/// @param bkColor ������ɫ
/// @param playerID [��ѡ]ָ�����͸��ĸ���ң�Ĭ��ΪԴ����
/// 
void AddInfo( const char *str, ulong fgColor = 0xFFFFFFFF, ulong bkColor = 0, const CGUID &playerID = NULL_GUID );

///
/// ����������Ϣ��һ�������ϵ��������
/// @param str ������Ϣ
/// @see AddInfo
///
void PostRegionInfo( const char *str, ulong fgColor = 0xFFFFFFFF, ulong bkColor = 0 );

///
/// ����������Ϣ��ȫ�����
/// @see AddInfo
///
void PostWorldInfo( const char *str, ulong fgColor = 0xFFFFFFFF, ulong bkColor = 0 );

///
/// ����������Ϣ��ָ�����ҵ����
/// @see AddInfo
///
void PostCountryInfo( const char *str, long country, ulong fgColor = 0xFFFFFFFF, ulong bkColor = 0 );

///
/// ������Ч����
/// @param index ����ID
///
void PlayEffectByIndex( long index );

///
/// ��Դ���˵��
/// @param str ˵�������ַ���
///
void PlayerTalk( const char *str );

///
/// @see PlayerTalk
///
void PlayerTalkByIndex( long textType, long textID );

///
/// �ñ�����˸
///
void PackGlint();

///
/// ��ָ����ҷ���������Ϣ
/// @param playerID ���GUID
/// @see AddInfo
///
void PostPlayerInfo( const CGUID &playerID, const char *str, ulong fgColor = 0xFFFFFFFF, ulong bkColor = 0 );

///
/// @see PostPlayerInfo
///
void PostPlayerInfoByIndex( const CGUID &playerID, long textType, long textID,
						   ulong fgColor = 0xFFFFFFFF, ulong bkColor = 0 );

///
/// ��ʾ�ַ������ͻ�����Ļ
/// @param posX ��ĻX����
/// @param posY ��ĻY����
/// @param str �����ַ���
/// @param isShow ��0��ʾ
///
void ShowString( long posX, long posY, const char *str, long isShow );

///
/// �ı�����
/// @param ID ����ID
///
void ChangeWeather( long ID );

///
/// ������Ч
/// @param effID ����ID
/// @param bindPos 
/// @param inRgn 0/1
/// @param x
/// @param y
///
void PlayEffect( long effID, long bindPos, uchar inRgn = 0, long x = 0, long y = 0 );

///
/// ���Ŷ���
/// @param action ����ID
///
void PlayAction( ushort action );

///
/// ��������
/// @param file �����ļ�
/// @param toAround 0ֻ���Ÿ��Լ���1���Ÿ���Χ
///
void PlaySound( const char *file, uchar toAround );

///
/// @see PlaySound
///
void Play3DSound( const char *file, long loop, long x, long y, long z );

///
/// ���ű�������
/// @param file �����ļ�
/// @param times ѭ������
///
void PlayBkMusic( const char *file, long times );

///
/// ֹͣ��������
///
void StopBkMusic();

///
/// �����Ʒ�������
/// @param goodsName ��Ʒԭʼ��
/// @param barIndex �������λ���
///
int AddGoodsToShortcutBar( const char *goodsName, long barIndex );

///
/// ��Ӽ��ܵ������
/// @param skillID ����ID
/// @param barIndex �������λ���
///
int AddSkillToShortcutBar( long skillID, long barIndex );

///
/// ת�������
///
void TransformCamera( long mode, long param1, long param2, long param3 );

///
/// �����ͻ��˽���
/// @param mode 1�򿪣�2�رգ�3�ر�����
/// @param interfaceName ��������
///
void OperInterface( long mode, const char *interfaceName );

///
/// ������ʾ
/// @param script �ű��ļ���
/// 
void HelpHint( const char *script, long delTime );

///
/// ���������ʾ
///
void InterfaceHint( const char *script, long delTime, const char *interfaceName,
				   long x, long y, long w, long h );

///
/// ������̬����
/// @param actionID ����ID
/// @param srcID �Է���GUID
///
int SendExpAction( long actionID, long srcType, const CGUID &srcID, long destType = 0, const CGUID &destID = NULL_GUID );
// };