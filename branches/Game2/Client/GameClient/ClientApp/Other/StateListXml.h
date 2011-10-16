#pragma once
class CStateListXml
{
public:
	CStateListXml(void);
	virtual ~CStateListXml(void);
public:
	/*	2008.11.17 - �����޸�
	 *		�޸�״̬����Ч����ʽ��ÿ���׶�һ����Ч�б�
	 */
	struct tagStateEffect 
	{
		DWORD	dwEffectID;				// ״̬����Ч���
		DWORD	dwEffectIndex;			// ״̬����Ч�����ţ�Ĭ��Ϊ0��
		DWORD	dwEffectPosition;		// ״̬��Ч�İ�λ��
		DWORD	dwEffectCycle;			// ״̬��Ч�Ƿ�ѭ��
		bool	bIsOrderPlay;			// ״̬��Ч�Ƿ�˳�򲥷�
		tagStateEffect()
		{
			dwEffectID = 0;
			dwEffectIndex = 0;
			dwEffectPosition = 0;
			dwEffectCycle = 0;
			bIsOrderPlay = false;
		}
	};
	struct tagStateLevel
	{
		DWORD	dwAfettleLevel;				// ״̬�ĵȼ�
		string	strAfettleDesc;				// ״̬������
		DWORD	dwAfettleContinueTime;		// ״̬����ʱ��
		DWORD	AfettleDeathIsClear;		// ����ʱ�Ƿ����״̬
		bool	bColorFade;					// �Ƿ���ɫ����
		//---------��ʼ�׶���Ч-----------
		vector<tagStateEffect> vecBeginEffect;	// ��Ч�б�
		//---------�м�׶���Ч-----------
		vector<tagStateEffect> vecMidEffect;	// ��Ч�б�
		//---------�����׶���Ч-----------
		vector<tagStateEffect> vecEndEffect;	// ��Ч�б�
		DWORD	dwInterval;					// ��ɫ�仯����ڣ����룩
		vector<DWORD> vecBuffColor;			// ��ɫ���ݶ���
		//long	lIndex;						// ��ʾ״̬��������Ψһ��
		tagStateLevel()
		{
			dwAfettleLevel = 0;
			strAfettleDesc = "";
			dwAfettleContinueTime = 0;
			AfettleDeathIsClear = 0;
			dwInterval = 0;
		}
	};
	struct tagStateList
	{
		long	lAfettleID;					// ״̬���
		string	strAfettleName;				// ״̬����
		long	lAfettleICON;				// ״̬ͼ��
		DWORD	AfettleStateOnly;			// ״̬�Ƿ�Ψһ
		DWORD	AfettleIsBuff;				// �Ƿ�Ϊ����״̬
		BYTE	bStopMove;					// �Ƿ���״̬������ֹͣ�ƶ�
		BYTE	wAction;					// ��Ӧ�Ķ���
		vector<tagStateLevel> vecStateLevel;	// �ȼ�����
		tagStateList()
		{
			lAfettleID = 0;
			strAfettleName = "";
			lAfettleICON = 0;
			AfettleStateOnly = 0;
			AfettleIsBuff = 0;
			bStopMove = 0;
			wAction = 0;
		}
	};
private:
	static map<long,tagStateList>	m_mapStateList;		// ���е�״̬�б�
public:
	static BOOL Load(const char* filename);				// ��ȡ�����ļ�
	static map<long, tagStateList>* GetStateList(){return &m_mapStateList;}	// ȡ��״̬����
	static tagStateList* GetState(long lID);			// ����״̬���ȡ��״̬
	static void Release();								// �ͷŶ���
};