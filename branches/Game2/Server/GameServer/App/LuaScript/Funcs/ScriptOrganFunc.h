/// 
/// @file ScriptOrganFunc.h
/// @brief Script�ڵĹ����������
///

// class Script {

///
/// \brief ��ȡ���������
///
long GetTeamNum();

///
/// \brief ��ȡ������һ����ͼ�е�����
///
long GetTeamNumInRgn();

///
/// \brief ��ȡ��������
/// \param pos �����ڶ����е�λ��
///
const char* GetTeamerName(int pos);

///
/// \brief ��ȡ�����GUID
///
CGUID GetTeamGUID();

///
/// \brief ͨ����ҵ�GUID��ȡ�����GUID
///
CGUID GetPlayerTeamGUID(const CGUID& playerGuid);

///
/// \brief �ж��Լ��Ƿ��Ƕӳ�
///
bool IsTeamCaptain();

///
/// \brief ��ȡ�ӳ��ڶ����е�λ��
///
long GetTeamCaptainPos();

///
/// \brief ���Ͷ��飨�����������Ӵ��ͣ����򶼲����ͣ�
///
long SetRegionForTeam(int type, int rgnType, const CGUID& regionGuid, long x=-1, long y=-1, long dir=-1, long range=-1);

///
/// \brief ���Ͷ��飨�ڷ�Χ�ڵĴ��ͣ���Χ��û�в�����
///
void SetTeamRegion(int rgnType, const CGUID& regionGuid, long x=-1, long y=-1, long radius=3, long dir=-1, long range=2);

///
/// \brief �ж������Ƿ����Լ���Χ
/// \param type 0��radius������Ч��1��ͬ������2��ͬ������
/// \param radius �뾶��С
///
bool IsTeammatesAroundMe(long type=0, long radius=3);

///
/// \brief Ϊһ���������Ҵ����ű�
/// \param distance �������ƣ�Ϊ0��ʾ�޾�������
///
bool RunScriptForTeam(const char* scriptName, long distance, const CGUID& playerGuid=NULL_GUID);

///
/// \brief �������
///
bool CreateFaction();

///
/// \brief ��������������
///
void OpenApplyBoard();

///
/// \brief �򿪰��ͼ���ϴ����
///
void OpenFacIconBoard();

///
/// \brief ��ȡ���ȼ�
///
long GetFactionLevel();

///
/// \brief ���ð����ļ״̬
/// \param inPut 0���أ���0����
///
bool SetFactionRecruitState(long inPut);

///
/// \brief ͨ����ҵ����ֻ�ȡ����GUID
///
CGUID GetFactionIDByPlayerName(const char* playerName=NULL);

///
/// \brief ͨ����ҵ����ֻ�ȡ���˵�GUID
///
CGUID GetUinionIDByPlayerName(const char* playerName=NULL);

///
/// \brief ��ǰ����Ƿ��ǰ��᳤
///
bool IsFacMaster();

///
/// \brief ��ȡĳ���ݵ����Ʊ������
///
long GetFacTickets(const CGUID& factionGuid, const char* footholdName);

///
/// \brief ��ȡ�������Ա��ս��ֵ
/// \param flag 1����ҵ�GUID��2������GUID
///
long GetBattleVal(int flag, const CGUID& guid);

///
/// \brief ��ȡ�������Ա��Ͷ��ֵ
/// \param flag 1����ҵ�GUID��2������GUID
///
long GetArmyInvest(int flag, const CGUID& guid);

///
/// \brief �ж�ĳ������Ա�Ƿ��в���Ȩ
///
bool GetJoinBattle(const CGUID& playerGuid);

///
/// \brief ��ȡĳ���Ƽ��ĵȼ�
///
long GetTechLv(const CGUID& factionGuid, long techNo);

///
/// \brief ��ȡ��������
///
const char* GetFactionName(const CGUID& factionGuid);

///
/// \brief ��ȡĳ���ݵ��ս��ֵ
///
long GetFacBaseBattle(const CGUID& factionGuid, const char* footholdName);

///
/// \brief �����Ѹ��µľݵ���Ϣ���ݵ��ͻ���
///
void SendBaseInfo(const CGUID& factionGuid);

///
/// \brief ��ȡ����ĳ����Ŀ�ĵȼ�
/// \param type 0�����ȼ���1�������ȼ���2��רҵ�ȼ���3��ս���ȼ���4����ҵ�ȼ���5������ȼ�
///
long GetFacLevel(ulong type, const CGUID& playerGuid=NULL_GUID);

///
/// \brief ��ȡĳ������Ա�Ĺ���ֵ
///
long GetMemContribute(const CGUID& playerGuid=NULL_GUID);

///
/// \brief ����������
///
void OpenCreateFactionUi();


// };