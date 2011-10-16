
/// @file ScriptPlayerFunc.h
/// @brief Script�ڵ���Ҳ�������
///

// class Script {
///
/// �����������
/// @param attrName ������
/// @param attrVal ����ֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
bool SetPlayerAttr( const char *attrName, double attrVal, const CGUID &playerID = NULL_GUID );

///
/// ��ȡ�������ֵ
/// @param attrName ������
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return ����ֵ
///
double GetPlayerAttr( const char *attrName, const CGUID &playerID = NULL_GUID );

///
/// �ı�������ԣ��ӷ�����
/// @param attrName ������
/// @param attrVal ����ֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
bool ChangePlayerAttr( const char *attrName, double attrVal, const CGUID &playerID = NULL_GUID );

///
/// ��ȡ��ҽ�ɫ��
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return �����
///
const char *GetPlayerName( const CGUID &playerID = NULL_GUID );

///
/// ��ȡ���GUID
/// @param name ��ҽ�ɫ��
/// @return ���GUID 
///
CGUID GetPlayerID( const char *name );

///
/// �ı���ҽ�Ǯ
/// @param money ��Ǯֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void ChangePlayerMoney( long money, const CGUID &playerID = NULL_GUID );

///
/// ��ȡ��ҽ�Ǯ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return ��ҽ�Ǯ��
///
long GetPlayerMoney( const CGUID &playerID = NULL_GUID );

///
/// ������ҽ�Ǯ
/// @param money ��Ǯֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void SetPlayerMoney( long money, const CGUID &playerID = NULL_GUID );

///
/// �ı���ҵ�ȯ
/// @param ticket ��ȯֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void ChangePlayerTicket( long ticket, const CGUID &playerID = NULL_GUID );

///
/// ��ȡ��ҵ�ȯ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return ��ҵ�ȯ��
///
long GetPlayerTicket( const CGUID &playerID = NULL_GUID );

///
/// ������ҵ�ȯ
/// @param money ��ȯֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void SetPlayerTicket( long ticket, const CGUID &playerID = NULL_GUID );

///
/// �������øı��ƶ��ٶ�
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void ChangeBaseSpeed( const CGUID &playerID = NULL_GUID );

///
/// �����������
/// @param x X����
/// @param y Y����
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void SetPlayerPos( long x, long y, const CGUID &playerID = NULL_GUID );

///
/// ������ҷ���
/// @param dir ����ֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void SetPlayerDir( long dir, const CGUID &playerID = NULL_GUID );

/// 
/// ȡ�������������
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return ����ID
///
long GetCountry( const CGUID &playerID = NULL_GUID );

///
/// �ı������������
/// @param country ����ID
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void ChangeCountry( long country, const CGUID &playerID = NULL_GUID );

///
/// ��ȡ��ҹ��ҹ���ֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
long GetContribute( const CGUID &playerID = NULL_GUID );

///
/// ������ҹ��ҹ���ֵ
/// @param contribute ����ֵ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void SetContribute( long contribute, const CGUID &playerID = NULL_GUID );

///
/// �������Ƿ��ֵ��
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return true��ʾ��ֵ��
///
bool IsCharged( const CGUID &playerID = NULL_GUID );

///
/// ������ҳ�ֵ��־
/// @param c Ϊ1ʱ��ʾ��ֵ��
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
///
void SetCharged( int c, const CGUID &playerID = NULL_GUID );

///
/// ����ָ��ְҵ��ְҵ�ȼ�
/// @param occu ְҵ��-1��ʾ��ǰְҵ
///
void CheckOccuLvl( long occu );

///
/// ��������
/// @param type 0��ȫ�سǣ�1ԭ�ظ���
/// @param ratio
///
void Relive( long type, long ratio = 50 );

///
/// �ı����ְҵ
/// 
void ChangeOccu( long occu );

///
/// �Զ��ƶ���ָ������
/// @param x X����
/// @param y Y����
///
void AutoMove( float x, float y );

///
/// ����ָ��������ָ����Χ�ڵ���ҵ���һ������
/// @param src Ҫ����������ڳ���GUID
/// @param x1 y1 x2 y2 ԭ������Χ
/// @param type Ŀ�곡������
/// @param dest Ŀ�곡��GUID
///
void MovePlayer( const CGUID &src, long x1, long y1, long x2, long y2,
				long type, const CGUID &dest, long dx1, long dy1, long dx2, long dy2 );

///
/// �л��޵�ģʽ
/// @param mode Ϊ0ʱ�ر��޵У���0�����޵�
///
void GodMode( int mode );

///
/// �ж��Ƿ���GM
/// @param name ��ҽ�ɫ��
/// @return true��ʾ��GM
///
bool IsGM( const char *name );

///
/// ����ָ�����
/// @param name ��ҽ�ɫ��
/// @param flag 0��������0���
///
void DurancePlayer( const char *name, int flag );

///
/// ��ȡ��ǰ�������GameServer ID
///
long GetServer();

///
/// �ж��Ƿ�Ϊ�����飬�������Ƿ�ͬͼ
/// @param playerID [��ѡ]���GUID��Ĭ��ΪԴ����
/// @return �Է�GUID
///
CGUID GetOtherOneInMarriageTeam( const CGUID &playerID = NULL_GUID );

///
/// �õ���鲽��
///
long GetPlayerMarriageStep( const CGUID &playerID = NULL_GUID );

///
/// ���ý�鲽�� 
///
void SetPlayerMarriageStep( long step, const CGUID &playerID = NULL_GUID );

///
/// ����srcΪdest�Ľ�����
///
void SetPlayerSpouse( const CGUID &src, const CGUID &dest = NULL_GUID );

///
/// ɾ����ҽ�����
///
void DelPlayerSpouse( const CGUID &playerID = NULL_GUID );

///
/// ȡ����ҽ�����GUID
///
CGUID GetPlayerSpouse( const CGUID &playerID = NULL_GUID );

///
/// ȡ�ý����������
///
const char *GetPlayerSpouseName( const CGUID &playerID = NULL_GUID );

///
/// �õ�ħ��Ԫ������
///
long GetElementProperty( long etype, long ptype, const CGUID &playerID = NULL_GUID );

///
/// ����ħ��Ԫ������
///
long SetElementProperty( long etype, long ptype, long val, const CGUID &playerID = NULL_GUID );

/// 
/// ���ü����츳id
/// 
void StudyGenius(long id,long lvl,const CGUID& playerID = NULL_GUID);

/// 
///  ɾ���츳
/// 
void RemoveGenius(long id,const CGUID& playerID = NULL_GUID);

// };
