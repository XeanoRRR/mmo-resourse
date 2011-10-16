/// 
/// @file ScriptGoodsFunc.h
/// @brief Script�ڵ���Ʒ��������
///

// class Script {

///
/// \brief ��ӵ���
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ���ߵ�����
/// \return �ɹ����
///
bool AddGoods(const char* goodsOriginalName, int goodsAmount);

/// \brief ���װ��
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ���ߵ�����
/// \param armamentLevel ��ȵȼ�
/// \param evilLevel ħ���ȼ�
/// \param playerName �������
/// \return �ɹ����
bool AddArmament(const char* goodsOriginalName, int goodsAmount, int armamentLevel, int evilLevel, const char* playerName=NULL);

/// \brief ɾ������
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ���ߵ�����
/// \return �ɹ�ɾ���ĵ�������
int DelGoods(const char *goodsOriginalName, int goodsAmount);

/// \brief �����ߵ�����
/// \param x ������X����
/// \param y ������Y����
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ��������
/// \return �ɹ����
bool AddGoodsToRgn(double x, double y, const char* goodsOriginalName, int goodsAmount);

/// \brief �󶨵���
/// \param containerID ��������
/// \param containerPos �����ڱ����е�λ��
/// \param bindType 0���󶨣�1�����
/// \return �ɹ����
bool BindGoods(int containerID, int containerPos, int bindType);

/// \brief �������
/// \param containerID ��������
/// \param containerPos �����ڱ����е�λ��
/// \param bindType 0���󶨣�1�����
/// \return �ɹ����
bool FrostGoods(int containerID, int containerPos, int bindType);

/// \brief ���ԭʼ������װ�������ӱ�����ĳ�����ߵ�����
/// \param goodsOriginalName ����ԭʼ��
/// \return ��������
int CheckGoods(const char* goodsOriginalName);

/// \brief ������е�ĳ�����ߵ�����
/// \param goodsOriginalName ����ԭʼ��
/// \return ��������
int CheckGoodsInBank(const char* goodsOriginalName);

/// \brief ����ܷ�����һ��������ĳ������
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ��������
/// \return �ɹ����
bool CheckSpace(const char* goodsOriginalName, int goodsAmount);

/// \brief ��ȡԭʼ�����ĵ�������
/// \return ��������
int GetGoodsNum();

/// \brief ��ȡʣ���������
/// \return ʣ���������
int GetGoodsFree();

/// \brief ��ȡĳװ��λ�õ�װ��
/// \param pos װ��λ��
/// \param playerGuid ���GUID
/// \return ���߻�������ID
int GetEquipIDByPos(int pos, const CGUID& playerGuid=NULL_GUID);

/// \brief ��ȡĳװ��λ�õĵ���ԭʼ��
/// \param pos װ��λ��
/// \param playerGuid ���GUID
/// \return equipment ����ԭʼ��
const char* GetEquipNameByPos(int pos, const CGUID& playerGuid);

/// \brief ��ȡ���߸�������
/// \param containerId ��������
/// \param containerPos �����ڱ����е�λ��
/// \param addonPropertyIdx ���߸�������ID
/// \param valueId ֵ1����ֵ2
/// \param playerName �������
/// \return ���߸�������
int GetPlayerGoodsAddonProperty(int containerId, int pos, int addonPropertyIdx, int valueId, const char* playerName=NULL);

/// \brief sets ���߸�������
/// \param containerId ��������
/// \param containerPos �����ڱ����е�λ��
/// \param addonPropertyIdx ���߸�������ID
/// \param valueId ֵ1����ֵ2
/// \param��ֵ ���߸������Ե�ֵ
/// \param playerName �������
/// \return �ɹ����
bool SetPlayerGoodsAddonProperty(int containerId, int containerPos, int addonPropertyIdx, int valueId, int value, const char* playerName=NULL);

/// \brief ɾ����ҵ���
/// \param playerName �������
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ��������
/// \return ����ɾ���ĵ�������
int DelPlayerGoods(const char* playerName, const char* goodsOriginalName, int goodsAmount);

/// \brief ɾ�������ű�ʹ�õĵ���
/// \param goodsAmount ��������
/// \return ����ɾ���ĵ�������
int DelUsedGoods(int goodsAmount);

/// \brief ��ȡ�����ű�ʹ�õĵ��ߵ�����
/// \return ��������
int CheckUsedGoods();

/// \brief ��ȡ�����ű�ʹ�õĵ��ߵĸ������Ե�ֵ
/// \param addonPropertyId ��������ID
/// \param valueId ֵ1����ֵ2
/// \return ���߸������Ե�ֵ1
int GetUsedGoodsPropertiy(int addonPropertyId, int valueId);

/// \brief ����ʹ�õĵ��߸������Ե�ֵ
/// \param addonPropertyId ���߸�������ID
/// \param valueId ֵ1����ֵ2
/// \param��ֵ ���߸������Ե�ֵ
/// \return �ɹ����
bool SetUsedGoodsPropertiy(int addonPropertyId, int valueId, int value);

/// \brief ��������
/// \param npcName NPC����
void BuyGoods(const char* npcName);

/// \brief ��ȡ���߸������Ե�ֵ
/// \param goodsId ����ID
/// \param propertyId ���߻�������ID
/// \param valueId ֵ1����ֵ2
/// \return ���߸������Ե�ֵ
int GetGoodsAddonProperty(int goodsId, int propertyId, int valueId);

/// \brief ������ҵ�װ������ȫ��
void FullRefine();

/// \brief ��ȡ����ǿ��
/// \param srcPoint ���ָ��
/// \return ����ǿ�ȵȼ���ֵ1
int GetWeaponStr(ulong srcPoint);

/// \brief ��ȡ������ʾ��
/// \param goodsOriginalName ����ԭʼ��
/// \return ������ʾ��
const char* GetGoodsDisplayName(const char* goodsOriginalName);

/// \brief ��һس�
/// \return �ɹ����
bool BackToCity();

/// \brief ��ʼһ��ͨ�õ�����������
/// \param onFinishScript ��ɲ����Ľű�
/// \param onPutScript ��Ӧ��Ʒ����Ľű�
/// \param titleId ��������ID
/// \param buttonId ������ťID
/// \param txt ��ʾ�ı�
/// \param radius �رվ���
/// \param maxPushNum ������������Ʒ��
/// \return �ɹ����
bool BeginContainerGoodsOpt(const char* onFinishScript, 
							const char* onPutScript, 
							ulong titleId, 
							ulong buttonId, 
							const char* txt, 
							long radius,
							ulong maxPushNum);

/// \brief ��ȡ�����д������ĵ���
/// \param goodsIdx �ڼ���������λ��1~4��
/// \return ����GUID
CGUID GetOptGoodsInContainer(ulong goodsIdx);

/// \brief ����������ĵ��ߵ�������
/// \param goodsGuid ����GUID
/// \param goodsIdx �ڼ���������λ��1~4��
/// \return �ɹ����
bool PutOptGoodsInContainer(const CGUID& goodsGuid, ulong goodsIdx);

/// \brief ��������еĴ���������
/// \param goodsIdx �ڼ���������λ��1~4��
/// \return �ɹ����
bool RemoveOptGoodsInContainer(ulong goodsIdx);

/// \brief ˢ�������ı�
/// \param txt �����ı�
/// \return �ɹ����
bool RefreshContainerTxt(const char* txt);

/// \brief ˢ��������ʾ�Ľ�Ǯ����
/// \param ������ʾ�Ľ�Ǯ����
/// \return �ɹ����
bool RefreshContainerGold(ulong goldAmount);

/// \brief �ر�һ��ͨ�õ�����������
void EndContainerGoodsOpt();

/// \brief ��ȡ����ԭʼ��
/// \param goodsGuid ����GUID
/// \return ����ԭʼ��
const char* GetGoodsOriginalName(const CGUID& goodsGuid);

/// \brief ��ȡ���߸������Ե�ֵ
/// \param goodsGuid ����GUID
/// \param addonPropertyId ���ߵĸ�������ID
/// \param valueId ֵ1����ֵ2
/// \return ���߸������Ե�ֵ
long GetGoodsAddonPropertyValues(const CGUID& goodsGuid, long addonPropertyId, long valueId);

/// \brief set ���߸������Ե�ֵ
/// \param goodsGuid ����GUID
/// \param addonPropertyId ���ߵĸ�������ID
/// \param valueId ֵ1����ֵ2
/// \param newValue ���߸������Ե�ֵ
/// \return �ɹ����
bool SetGoodsAddonPropertyValues(const CGUID& goodsGuid, long addonPropertyId, long valueId, long newValue);

/// \brief ���õ��ߵ��������ֵ
/// \param goodsGuid ����GUID
/// \return �ɹ����
bool ResetGoodsStochastic(const CGUID& goodsGuid);

/// \brief ɾ������
/// \param goodsGuid ����GUID
/// \return �ɹ����
bool DelGoodsByGuid(const CGUID& goodsGuid);

/// \brief �������ߣ����ص�һ�����ߵ�GUID
/// \param goodsOriginalName ����ԭʼ��
/// \param goodsAmount ���ߵ�����
/// \return ��һ�����ߵ�GUID
CGUID AddGoodsReturnGuid(const char* goodsOriginalName, uint goodsAmount);

/// \brief �Ƿ��Ƕ���ĵ���
/// \param goodsGuid ����GUID
/// \return 0��ʧ�ܣ�1�����ᣬ2��δ����
int IsFrostGoods(const CGUID& goodsGuid);

/// \brief ��ȡ����ʹ�õĵ��ߵ�ԭʼ��
/// \return ����ʹ�õĵ��ߵ�ԭʼ��
const char* GetUsedGoodsOriginalName();

/// \brief ��ȡ����ʹ�õĵ��ߵ�GUID
/// \return ����ʹ�õĵ��ߵ�GUID
CGUID GetUsedGoodsGuid();

/// \brief ���õ��߸��������ı�
/// \param goodsGuid ����GUID
/// \param txt ���߸��������ı�
/// \return �ɹ����
bool SetAddonDesc(const CGUID& goodsGuid, const char* txt);

/// \brief ��ȡ���߸��������ı�
/// \param goodsGuid ����GUID
/// \return ���߸��������ı�
const char* GetAddonDesc(const CGUID& goodsGuid);

/// \brief ��ȡ��ǰ������װ����������
/// \detail ������װ��/жװ��Ʒ�ű���
/// \param pidx ����������λ
/// \param vids ֵ��λ
/// \return δ�ҵ�����0
long GetUsedEquipProperty( int pidx, int vidx );
// };