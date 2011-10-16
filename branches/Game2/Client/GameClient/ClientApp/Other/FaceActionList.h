/* CFaceActionList.h
 * ����: ���鶯���б�������
 * ժҪ: �������ֱ�ӵ����Һ͹���ı����б�,ͨ����ʼ��ʱ�������ļ���������,�������ṩ�ĽӿڶԱ��鶯�����в���
 * ����: lpf
 * ��������: 2008-06-16
 */
class CMoveShape;
// ���鶯��������Ϣ�ṹ��
struct tagFaceAction
{
	DWORD  dwID;			//ID
	string sCallStr;		//�����ַ���(��������,����ǹ�����̬�б���ó�ԱֵΪ��)
	string sNote;			//ע��

	// �������
	int	   iActionIndex;	//�����ļ�����
	string sActionFileName;	//�����ļ���

	bool bPlayInWater;		//�Ƿ������ˮ�²���

	// �������
	DWORD  dwFaceCount;		//�������
	DWORD  dwFaceStartIndex;//���鿪ʼ���
	DWORD  dwFaceEndIndex;	//����������
	DWORD  dwFaceInterval;	//������֡�ٶ�(����)
	DWORD  dwFaceFileHandle;//�����ļ����
	string sFaceFileName;	//�����ļ���

	string sShowTextTarget;		//��ʾ�ı�(��Ŀ��)
	string sShowTextNoTarget;	//��ʾ�ı�(û��Ŀ��)

	tagFaceAction() : dwID(0), sCallStr(""), sNote(""), iActionIndex(0), sActionFileName(""), bPlayInWater(false), dwFaceCount(0), dwFaceStartIndex(0), dwFaceEndIndex(0), dwFaceInterval(0), dwFaceFileHandle(0), sFaceFileName(""), sShowTextTarget(""), sShowTextNoTarget("") {}
};

// �ֽ⶯���ṹ��
struct tagActionDec
{
	string sAction;				//�������
	int	   iLoopCount;			//����ѭ������
};

// �����б�������Ϣ�ṹ��
struct tagActionList
{
	DWORD dwIdEffect;			//��ЧID
	DWORD dwStartTime;			//��Ч��ʼ���ŵ�ʱ��
	bool  bIsLoop;				//��Ч�Ƿ�ѭ������
	DWORD eLocator;				//��Ч�󶨵�

	vector<tagActionDec> m_vecActionDecList;			//�ֽ⶯���б�
};



// ���鶯���б���
class CFaceActionList
{
	vector<tagFaceAction> m_vecPlayerFaceAction;				//�����̬������
	vector<tagActionList> m_vecPlayerActionList;				//�����̬�����б�
	vector<tagFaceAction> m_vecMonsterFaceAction;				//������̬������

public:
	CFaceActionList(void);
	~CFaceActionList(void);

	// ���ݷ��ʺ���
	DWORD			GetPlayerFaceActionID(const char * pszStr);				//���ݴ�����ַ����õ������̬ID
	tagFaceAction * GetPlayerFaceAction(DWORD dwID);						//���ݴ����ID�õ������̬����
	tagFaceAction * GetMonsterFaceAction(DWORD dwID);						//���ݴ����ID�õ�������̬����
	tagActionList * GetActionList(DWORD dwIndex);							//���ݴ���Ķ����ļ������õ������б�

	// �����ӿ�
	bool LoadFaceAction(const char * pszFileDir);											//������̬�����ļ�
	bool LoadActionList(const string & sFilePathName);										//������Ҷ�������
	void Release();																			//�ͷ���̬�б�
	void SetupFaceAction(DWORD dwFaceActionID, CMoveShape * pSourceObj, long lSourceObjType, CMoveShape * pTargetObj, long lTargetObjType);		//�趨��̬����
	bool IsCanPlay(DWORD dwFaceActionID, CMoveShape * pSourceObj, long lSourceObjType);		//����Ƿ���Բ���ָ������̬����
};