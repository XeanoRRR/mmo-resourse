/// 
/// @file ScriptQuestFunc.h
/// @brief Script�ڵ�������غ���
///

// class Script {

/// @brief ��������ĳ������
bool AddQuest(int questId);

/// @brief ��ȡ��ҵ���������ID
int GetMainQuestId();

/// @brief ������ĳ������
bool CompleteQuest(int questId);

/// @brief ��ҷ���ĳ������
void AbandonQuest(int questId);

/// @brief ��ȡ���ĳ�������״̬
/// @return ����״ֵ̬��0��������δ��ɣ�1������������ɣ�2��������δ���
int GetQuestState(int questId);

/// @brief ��ȡ�����Ч�����������
int GetValidQuestAmount();

/// @brief ����ĳ������Ĳ���
void UpdateQuestStep(int questId, int stepId);

/// @brief ����ĳ������ĳһĿ�������ֵ��
void UpdateQuestParam(int questId, int paramIdx, int param, int regionId, int x, int y);

/// @brief ����ĳ�������ĳ��������ֵ��
void UpdateQuestData(int questId, int paramIdx, int value);

/// @brief ��ȡĳ������Ĳ��������
int GetQuestStepAmount(int questId);

/// @brief ��ȡĳ�������ĳ�������ĳ��������ֵ
int GetParam(int questId, int stepId, int idx);

/// @brief ��ȡĳ�������ĳ�������ĳ�������ĸ������Ե�ֵ
int GetQuestAccParam(int questId, int stepId, int idx);

/// @brief ����ĳ�������ĳ�������ĳ�������ĸ������Ե�ֵ
bool SetQuestAccParam(int questId, int stepId, int idx, int value);

/// @brief ���ĳ�������ĳ�������ĳ�������ĸ������Ե�ֵ
bool AddQuestAccParam(int questId, int stepId, int idx, int value);

/// @brief ��ȡĳ�������ĳ�������ĳ�������ĳ���ID
int GetQuestRegionID(int questId, int stepId, int idx);

/// @brief ����ĳ�������ĳ�������ĳ�������ĳ���ID
bool SetQuestRegionID(int questId, int stepId, int idx, int regionId);

/// @brief ��ȡĳ�������ĳ�������ĳ�������ĳ���X����
int GetQuestRegionX(int questId, int stepId, int idx);

/// @brief ����ĳ�������ĳ�������ĳ�������ĳ���X����
bool SetQuestRegionX(int questId, int stepId, int idx, int x);

/// @brief ��ȡĳ�������ĳ�������ĳ�������ĳ���Y����
int GetQuestRegionY(int questId, int stepId, int idx);

/// @brief ����ĳ�������ĳ�������ĳ�������ĳ���Y����
bool SetQuestRegionY(int questId, int stepId, int idx, int y);

/// @brief ��ȡ��ҵ������б����������
int GetQuestAmount();

/// @brief ��ȡ��ҵ�����������½ڱ��
int GetQuestChapter();

/// @brief ������ҵ�����������½ڱ��
bool SetQuestChapter(int num);

/// @brief �����ҵ������¼
bool AddFinishedQuestRecord(int questId);

/// @brief ɾ����ҵ������¼
bool DelFinishedQuestRecord(int questId);

/// @brief ��ȡ��ҵ�ĳ�������Ƿ����
bool GetQuestFinishedRecord(int questId);

/// @brief ��ȡ��ҵ�ĳ��������ı�����
int GetQuestTextType(int questId);

/// @brief ��ȡ��ҵ�ĳ�����������
const char* GetQuestName(int questId);

/// @brief ��ҵ�ĳ�������Ƿ񱻼�¼
int IsQuestRecorded(int questId);

/// @brief ��ȡ��ҵ�ĳ�����������
int GetQuestType(int questId);

/// @brief ��ȡĳ�������Ƿ���Թ���
int GetQuestIsShare(int questId);

/// @brief �жϼ��������Ƿ�����
bool GetQuestReqSkill(int questId);

/// @brief �ж�������Ա���������
bool GetQuestSexLimit(int questId);

/// @brief �ж��Ƿ�Я�������޶�����
bool GetQuestTotePet(int questId);

/// @brief �ж���ҵ�ĳ�������Ƿ���ʾ
int IsQuestDisplayed(int questId);

/// @brief �ж���ҵ�ĳ�������Ƿ��ܷ�����
int IsQuestRepeated(int questId);

/// @brief �ж���ҵ�ĳ�������Ƿ���������ʧ��
int IsQuestFailOnDead(int questId);

/// @brief �ж���ҵ�ĳ�������Ƿ���ʧ�ܺ������½�
int IsQuestRepeatedOnFail(int questId);

/// @brief ��ȡ��ҵ�ĳ���������С�ȼ�����
int GetQuestMinLvLimit(int questId);

/// @brief ��ȡ��ҵ�ĳ����������ȼ�����
int GetQuestMaxLvLimit(int questId);

/// @brief ��ȡ��ҵ�ĳ�������ְҵ����
int GetQuestOccupationLimit(int questId, int occupationIdx);

/// @brief ��ȡ��ҵ�ĳ��������½���
int GetQuestChapterAmount(int questId);

/// @brief ��ȡ��ҵ�ĳ���������ĳ������ID
int GetNextQuestId(int questId);

/// @brief ��ȡ��ҵ�ĳ�������ĳ��������Ӧ��ǰ������ID
int GetPreQuest(int questId, int idx);

/// @brief ��ȡ��ҵ�ĳ�������ǰ����������
int GetPreQuestAmount(int questId);

/// @brief ��ȡĿ��ģ������
int GetAimAMTType(int questId, int stepId, int aimId);

/// @brief ��ȡĿ������
int GetAimType(int questId, int stepId, int aimId);

/// @brief ��ȡĿ��ID
int GetAimId(int questId, int stepId, int aimId);

/// @brief ��ȡĿ������
int GetAimAmount(int questId, int stepId, int aimId);

/// @brief ��ȡ�ڴ��б�������
int GetAimParameIndex(int questId, int stepId, int aimId);

/// @brief ��ȡ����������Ʒ����������
int GetDropFrom(int questId, int stepId, int aimId);

/// @brief ��ȡ���伸��
int GetDropOdds(int questId, int stepId, int aimId);

/// @brief ��ȡ����Ŀ�������
const char* GetAimName(int questId, int stepId, int aimId);

/// @brief ��ȡ��������
int GetPrepareParam(int questId, int stepId, int aimId, int index);

/// @brief ��ȡ����������
int GetQuestEncouragementSum(int questId);

/// @brief ��ȡindex��Ӧ�Ĳ��轱������
const char* GetStepEncouragementName(int questId, int stepId, int index);

/// @brief ��ȡindex��Ӧ������������
const char* GetQuestEncouragementName(int questId, int index);

/// @brief ��ȡindex��Ӧ�Ĳ��轱������
int GetStepEncouragementType(int questId, int stepId, int index);

/// @brief ��ȡindex��Ӧ������������
int GetQuestEncouragementType(int questId, int index);

/// @brief ��ȡindex��Ӧ�Ĳ��轱��ID
int GetStepEncouragementID(int questId, int stepId, int index);

/// @brief ��ȡindex��Ӧ��������ID
int GetQuestEncouragementID(int questId, int index);

/// @brief ��ȡindex��Ӧ�Ĳ��轱������
int GetStepEncouragementNum(int questId, int stepId, int index);

/// @brief ��ȡindex��Ӧ������������
int GetQuestEncouragementNum(int questId, int index);

/// @brief ��ȡindex��Ӧ�Ĳ��轱����ռ������
int GetStepEncouragementCell(int questId, int index);

/// @brief ��ȡindex��Ӧ����������ռ������
int GetQuestEncouragementCell(int questId);

/// @brief ��ȡ�����ܲ�����
int GetStepSum(int questId);

/// @brief ��ȡ��������Ŀ����
int GetStepAimNum(int questId, int stepId);

/// @brief �������Ƿ�������ϵͳ��
int FindRoleIsInSystem(int questId);

/// @brief ĳ�����ĳһ�����Ƿ�������ϵͳ��
int FindRoleStepIsInSystem(int questId, int stepId);

/// @brief �����ҵ�Npc��״̬
int GetQuestStateId(int questId);

/// @brief �������NPC���ı���ʾid
const char* GetQuestDisplayText(int questId,int state);
// };