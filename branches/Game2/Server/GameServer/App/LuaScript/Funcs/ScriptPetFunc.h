/// 
/// @file  ScriptPetFunc.h
/// @brief ����ϵͳ�ű�����
/// 

// class Script {

/// ���ݳ�������ID��������
/// @param  type		��������
/// @param  id			��������ID
/// @param  isWild		�Ƿ�Ұ����־
/// @return	��������ɹ����
bool CreatePet(long type, long id, long isWild);

/// ���ݳ���GUIDɾ������
/// @param  guid		����GUID
/// @return	�ɹ����
bool DelPet(const CGUID& guid);

/// ���ó�������
/// @param attrName ������
/// @param attrVal ����ֵ
/// @param guid ����GUID
/// @return	�ɹ����
bool SetPetAttr(const char *attrName, double attrVal, const CGUID& guid);

/// ��ȡ��������ֵ
/// @param attrName ������
/// @param guid ����GUID
/// @return ����ֵ
long GetPetAttr(const char* attrName, const CGUID& guid);

/// �������
/// @param  guid		����GUID
/// @return	�ɹ����
bool ActivePet(const CGUID& guid);

/// Ϊ��ҳ�����Ӽ���
/// @param  guid		����GUID
/// @param  id			����ID
/// @param  level		���ܵȼ�
/// @param	pos			λ��
/// @return	��ӳ��＼�ܳɹ����
bool AddPetSkill(const CGUID& guid, long id, long level, long pos);

/// ��ȡ��ǰ�������GUID
/// @return	�������GUID
const CGUID& GetFocusPetGUID();

/// ��ȡ��ҳ���Ʒ��
/// @param guid			����GUID
/// @return	����Ʒ��
long GetPetGrade(const CGUID& guid);

/// ��ȡ��ҳ������������ǿ������
/// @param guid			����GUID
/// @return	�������������ǿ������
long GetPetDAfterCnt(const CGUID& guid);

/// ���ó�������
/// @param  guid		����GUID
/// @return	�������ʳɹ����
bool ResetPetTalent(const CGUID& guid);

/// ���ó����������ǿ������
/// @param  guid		����GUID
/// @return	���ú�������ǿ�������ɹ����
bool ResetPetDAfterCnt(const CGUID& guid);

/// ��������ǿ��
/// @param  guid		����GUID
/// @param	val			ǿ��ϵ��(����ǿ��Ч��ֵ)
/// @return	��������ǿ���ɹ����
bool UpgradePetDAfterVal(const CGUID& guid, long val);

/// ���ó������Ե�
/// @param  guid		����GUID
/// @return	���ó������Ե�ɹ����
bool ResetPetSavvy(const CGUID& guid);

/// ȡ���������ӵ���̴���Ŀ
/// @return �������ӵ���̴���Ŀ
long GetMaxServantNum(void);

/// �̴Ӿ����ȡ
/// @param  guid		����GUID
/// @param  val			����ֵ
void IncPetExp(const CGUID& guid, long val);

/// �̴���Ϊ�����ȡ
/// @param  guid		����GUID
/// @param  val			��Ϊ����ֵ
void IncPetDExp(const CGUID& guid, long val);

/// ��������
/// @param  guid		����GUID
/// @param  newLev		�����µȼ�
/// @return ���������Ƿ�ɹ�
bool PetLevUp(const CGUID& guid, long newLev);

// };