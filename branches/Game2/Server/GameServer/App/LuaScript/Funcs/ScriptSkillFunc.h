/// 
/// @file ScriptSkillFunc.h
/// @brief Script�ڵļ��ܺ���
/// @details �󲿷ּ��ܺ�����ֻ�����ڼ����˺��ű���
///

#define DEF_SHAPE (ulong)(-1)

// class Script {
///
/// ���״̬
/// @param ID ״̬ID
/// @param level ״̬�ȼ�
/// @param shape [��ѡ]Ҫ��ӵ���shape��Ĭ��ΪԴ����
///
bool AddState( long ID, long level, ulong shape = DEF_SHAPE );

///
/// �Ƴ�״̬
/// @param ID ״̬ID
/// @param level ״̬�ȼ�
/// @param count [��ѡ]״̬����
/// @param shape [��ѡ]ָ���Ƴ�����Ĭ��ΪԴ����
///
void RemoveState( long ID, long level, long count = 0, ulong shape = DEF_SHAPE );

///
/// ��Դ������һ������
/// @param ID ����ID
/// @param level ���ܵȼ�
/// @return �ɹ�����true
///
bool AddSkill( long ID, long level );

/// 
/// �Ƴ�Դ������ϵ�ĳ������
/// @param ID ����ID
/// @param level ���ܵȼ�
///
void DelSkill( long ID, long level );

///
/// ��ȡָ�����ܵĵȼ�
/// @param ����
/// @return ���ܵȼ�
///
long GetSkillLevel( ulong skillPtr );

///
/// �ж�ѧϰ��������
/// @param ID ����ID
/// @return true��ʾ��ѧϰ
///
bool CheckStudySkill( long ID );

///
/// ��ȡ״̬����
/// @param shapeID ����GUID
/// @param shapeType ��������
/// @return ָ�������ϵ�״̬����
///
long GetAllStateNum( const CGUID &shapeID = NULL_GUID, long shapeType = 0 );

///
/// ɾ������DEBUFF
/// @param shapePtr �����ʶ
///
void DelDeBuff( ulong shapePtr );

/// 
/// ɾ������DEBUFF��BUFF
/// @param shapePtr �����ʶ
///
void DelAllBuff( ulong shapePtr );

///
/// ���ݶ����ʶ����������
/// @param shapePtr �����ʶ
/// @param attrName ������
/// @param attrVal ����ֵ
///
void SetMoveShapeAttrEx( ulong shapePtr, const char *attrName, double attrVal );

/// 
/// ���ݶ����ʶ��ȡ������
/// @param shapePtr �����ʶ
/// @param attrName ������
/// @return ����ֵ
///
double GetMoveShapeAttrEx( ulong shapePtr, const char *attrName );

///
/// ���ݶ����ʶ�ı�������
/// @param shapePtr �����ʶ
/// @param attrName ������
/// @param attrVal ����ֵ
///
void ChangeMoveShapeAttrEx( ulong shapePtr, const char *attrName, double attrVal );

///
/// ��ָ���������DEBUFF��Ĵ���
/// @param srcPtr Դ�����ʶ
/// @param destPtr Ŀ������ʶ
///
void AddDeBuffToObj( ulong srcPtr, ulong destPtr );

///
/// ���ܸ���,��ȡ��
/// @param skillPtr ���ܱ�ʶ
///
void ReliveSkill( ulong skillPtr );

///
/// ��ȡPK�жϽ��
///
long GetPkResult( ulong srcPtr, ulong destPtr, long addSub );

///
/// ���ݶ����ʶ��ȡ������
/// @param shapePtr �����ʶ
/// @return ��������
///
long GetObjectType( ulong shapePtr );

/// 
/// ���ݶ����ʶ��ȡ�˴˼�ľ���
/// @param srcPtr Դ�����ʶ
/// @param destPtr Ŀ������ʶ
/// @return ������֮��ľ���
///
double GetDistance( ulong srcPtr, ulong destPtr );

///
/// ���ͼ����б�
///
void SendSkillList( const char *str );

///
/// ��ȡ�����ӳ�ϵ��
/// @return �����ӳ�ϵ��
///
float GetEnergyRatio( long param, uchar magic );

///
/// �۳�����ֵ
///
void DeductEnergy( uchar magic );

/// 
/// ����Դ����ΪĿ�����Ĺ���Ŀ��
/// @param srcPtr Դ����
/// @param destPtr Ŀ�����
///
void SetTarget( ulong srcPtr, ulong destPtr );

///
/// �ж�Ŀ��װ������
///
long GetShieldNum( ulong destPtr );

///
/// ��ȡ���ܵĽű�����
///
long GetSkillParam( ulong skillPtr, long pos );

///
/// ��ȡ���ܵ�Ԫ������
/// @param skillPtr ���ܱ�ʶ
/// @return ���ܵ�Ԫ������
///
long GetSkillElement( ulong skillPtr );

///
/// ��ȡ���ܵ����
/// @param skillPtr ���ܱ�ʶ
/// @return �������
///
long GetSkillUseType( ulong skillPtr );

long GetSkillModType( ulong skillPtr );

///
/// ͨ��ID���״̬��ز���
///
long GetStateParamByID( long ID, const char *param, 
					   const CGUID &shapeID = NULL_GUID, long shapeType = 0 );

///
/// ͨ�������õ�״̬��ز���
///
long GetStateParamByIndex( long ID, const char *param, 
					   const CGUID &shapeID = NULL_GUID, long shapeType = 0 );

///
/// ���ʩ����
///
bool CheckStateFirer( ulong skillPtr );

///
/// ͨ������ID��ȡ���ܵȼ�
///
long GetSkillLv( long ID );

///
/// �ڽű���ʹ�õĸ���ӿ�
///
void ReliveInScript( ulong shapePtr, long type );

///
/// ���ݲ�������õ���Ӧ��guid
/// @param shapePtr �����ʶ
///
CGUID GetGUIDByParam( ulong shapePtr );

///
/// ��ָ���������нű�
/// @param shapePtr �����ʶ
/// @param scrip �����ļ���
///
void RunScriptByDest( ulong shapePtr, const char *script );

///
/// ��������
///
bool BeginUseSkill( long ID, long level );

///
/// �����¼������������ö���
///
void SetEventDestObj( ulong skillPtr );

///
/// ���ݼ��ܻ�״ָ̬��õ���Ӧ�ĵȼ�
///
int GetBaseModuleLv( ulong skillPtr );

///
/// ��ȡ��Ʒ��������
///
long GetGoodsAddonProperty( ulong skillPtr, int addonID, int pos );

///
/// ��ȡ������ص���Ʒ��Ϣ
/// @param skillPtr ���ܱ�ʶ
/// @return ��Ʒ��������
/// @return ��Ʒ���������ڵ�λ��
///
void GetGoodsPlacePos( ulong skillPtr, long *place = 0, long *pos = 0 );

///
/// ʹ��ָ��λ�õ���Ʒ
///
void UseGoodsByPlacePos( long place, long pos );

///
/// ��Ʒʹ�óɹ�
///
void SuccedUseItem( ulong skillPtr );

///
/// ȡ�ö�������ʱ��
///
long GetActModify( ulong skillPtr );

///
/// ��ͻ��˷��ͽ����Ʒ������Ϣ
///
void SendUnlockGoodsMsg( long place, long pos );

///
/// ��ȡ�������������Ϣ
/// @param shapePtr �����ʶ
/// @return ������GUID
/// @return ����������
///
CGUID GetBeneficiaryInfo( ulong shapePtr, long *type = 0 );

///
/// ��ȡ��ɱ����Ϣ
/// @return ��ɱ��GUID
/// @return ��ɱ������
///
CGUID GetKillerInfo( long *type = 0 );

/// 
/// ��ȡ��������Ϣ
/// @return ������GUID
/// @return ����������
///
CGUID GetAttackerInfo( long *type = 0 );

/// 
/// ��ȡ���ܻ�������ֵ
/// 
long  GetSkillValue(long skillid,long lvl,std::string name);

/// 
/// ��ȡ���ܻ�������(�ű�����)
/// @param skillPtr ��̬����stModuParam
/// @param name     ��õ�ǰ������ֵ
/// 
long  GetSkillAttriValue(ulong skillPtr,std::string name);

/// 
/// ɾ��ָ�����͵�״̬����Buf
/// @param SkillType  ��������
/// @param Count      ��������
/// 
void  ClearSkillAttackType(long  SkillType,long Count);

// };