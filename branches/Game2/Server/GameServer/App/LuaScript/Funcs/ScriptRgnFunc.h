/// 
/// @file ScriptRgnFunc.h
/// @brief Script�ڳ�����غ���
///

 //class Script {

///
/// \brief ��ȡ��ǰ����������ֵ
///
int GetRgnAttr(const char* attrName);

///
/// \brief ���õ�ǰ����������ֵ
///
void SetRgnAttr(const char* attrName, int attrValue);

///
/// \brief ��ȡ��ǰ����ָ���������������
/// \return 0=��ͨ����1=��������2=��ȫ����3=��ս��
///
int GetCellType(int x, int y);

///
/// \brief ��ȡ��ǰ����ָ���������������
/// \return 0=��ͨ����1=��������2=��ս����3=��ȫ����-1=����
///
int GetMapInfo(int x, int y);

///
/// \brief ���õ�ͼ�赲
/// \param block 0=���赲��1=�ɷ��У�2=���ɷ��У�3=��
///
void SetBlock(const CGUID& regionGuid, int x, int y, int block);

///
/// \brief ��ȡĳ�����������赲�������
///
bool GetRegionRandomPos(const CGUID& regionGuid=NULL_GUID, long* x=NULL, long* y=NULL);

///
/// \brief ��ȡĳ������ĳ�������ڵ����赲�������
///
bool GetRandomPosInRange(long regionId, int x, int y, int width, int height, long* randX=NULL, long* randY=NULL);

///
/// \brief ��ȡĳ���㵽��һ����ķ���
///
long GetLineDir(int srcX, int srcY, int destX, int destY);

///
/// \brief ����ĳ��������꣬���򣬲�������ȡ��һ���������
///
void GetNextPosByDir(int dir, int x, int y, int step, int* destX=NULL, int* destY=NULL);

///
/// \brief ������������Ҵ���ĳһ�ű�
///
void RunScriptForAllPlayerInRgn(const char* scriptFilePath);

///
/// \brief ��ȡĳ����ͨ������GUID
/// \param regionId ������ģ���ID
///
CGUID GetNormalDupRgnID(long regionId);

///
/// \brief �л���ͼ
///
bool ChangeRgn(long regionId, long x=-1, long y=-1, long dir=0, long range=0, long carriageDistance=0);

///
/// \brief �л�������ͼ
///
bool ChangeDupRgn(const CGUID &regionId, long regionType, long x, long y);

///
/// \brief ��ȡ��ǰ������GUID
///
CGUID GetCurRgnGUID();

///
/// \brief ��ȡĳ����������������
///
int GetDupRgnPlayerMaxNum(const CGUID& regionGuid);

///
/// \brief �Ը���Ϊִ����ִ��һ���ű�
///
bool RunDupRgnScript(const char* scriptFilename);

///
/// \brief ��ȡ��ǰ�ĸ���������������
///
int GetDupRgnMaxNum(int regionType);

///
/// \brief ����ָ�������Ĺ���Ȩ����
///
void SetDupRgnOperFlag(const CGUID& regionGuid, long operationFlag);

///
/// \brief ��ȡָ�������Ĺ���Ȩ����
///
bool GetDupRgnOperFlag(const CGUID& regionGuid);

///
/// \brief ��ȡִ���ߵ�ǰ���ڶ����е�ָ������˳��ŵĽ�ɫ��GUID
///
CGUID GetTeamMaterIDByPos(int pos);

///
/// \brief �޸�ָ��������������ʾ��
///
void EditDupRgnName(const CGUID& regionGuid, const char* regionName);

///
/// \brief ��������
///
void CreatePHGoods(const CGUID& ownerGuid, const char* name, int counts, 
						   int x, int y, int width, int height, int dir, 
						   const char* script, const char* talk);

///
/// \brief ɾ�����˷��ݸ�������
///
void DeletePHGoods(const CGUID& ownerGuid, const CGUID& goodGuid);

///
/// \brief ��ȡ��ǰ�ű���Ҹ��˸�����GUID
///
CGUID GetCurPlayerPDupRgnID();

///
/// \brief ��ȡ��ǰ�ű���Ҹ��˷��ݸ�����GUID
///
CGUID GetCurPlayerPHDupRgnID();

///
/// \brief ���õ�ǰ�ű���Ҹ��˸�����GUID
///
void SetPlayerPDupRgnID(const CGUID& ownerGuid, const CGUID& regionGuid);

///
/// \brief ���õ�ǰ�ű���Ҹ��˷��ݸ�����GUID
///
void SetPlayerPHDupRgnID (const CGUID& ownerGuid, const CGUID& regionGuid);

///
/// \brief ��ȡ����GS��ID
///
int GetLocalGSID();

///
/// \brief ������л���ָ��������ͼ
///
bool ChangeDupRgnByRgnID(const CGUID& playerGuid, int regionType, const CGUID& regionGuid, int x, int y);

///
/// \brief ��ȡ��ǰ�ű������ߵ�GUID
///
CGUID GetCallerGUID();

///
/// \brief ����ָ����һ�����ָ���ű�
///
void CallScriptByGUID(const CGUID& shapeGuid, const char* scriptFilename);

///
/// \brief �����ǰGS������ָ�����͵ĸ���
///
void ClearCurGSDupRgn(int regionType);

///
/// \brief ���õ�ǰ�ű��Ķ��鸱����GUID
///
void AddTeamRgnID(const CGUID& playerGuid, int tempRegionId, const CGUID& regionGuid);

///
/// \brief Ϊ��ǰ�ű������������GUID
///
void DelTeamRgnID(const CGUID& playerGuid, int tempRegionId);

///
/// \brief Ϊ��ǰ�ű�����������и���GUID
///
void ClearTeamRgnID(const CGUID& playerGuid);

///
/// \brief ���þݵ�״̬
///
void SetRgnStrongPointState(const char* footholdName, int footholdState);

///
/// \brief ��ȡ�ݵ�״̬
///
int GetRgnStrongPointState(const char* footholdName);


 //};