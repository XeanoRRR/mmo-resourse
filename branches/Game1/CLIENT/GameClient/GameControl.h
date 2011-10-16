#pragma once
#include "../AppClient/Other/SkillListXml.h"
#include "../AppClient/Skills/SkillXml.h"
#include "../AppClient/Other/StateListXml.h"
#include "../AppClient/Effect.h"
#include "../AppClient/CollectionGoods.h"

#include "../AppClient/AutoAttackHook.h"



//#ifndef  MOVE_IN_TILE
//#define MOVE_IN_TILE
//#endif

class CFindPath;
enum	MoveDir
{
	MOVE_NONE			= 0x0000,
	MOVE_FORWARD		= 0x0100,
	MOVE_LEFT			= 0x0010,	
	MOVE_RIGHT			= 0x0001,
	MOVE_BACK			= 0x1000,
	MOVE_LEFT_FORWARD	= 0x0110,
	MOVE_RIGHT_FORWARD  = 0x0101,
	MOVE_LEFT_BACK      = 0x1010,
	MOVE_RIGHT_BACK     = 0x1001,
};
// ��Ϸ������
class CGameControl
{
	// ��Χ�����ͷ�ʱ����Ϣ����������ͷţ�
	struct tagAreaSkill 
	{
		DWORD dwSkillID;
		float fPosX;
		float fPosY;
		tagAreaSkill()
		{
			dwSkillID = 0;
			fPosX = 0.f;
			fPosY = 0.f;
		}
	};
	// ��ǰ��Ҫ�ͷŵķ�Χ����
	tagAreaSkill m_stCurAreaSkill;
	CClientRegion*		pRegion;
	CPlayer*			pMainPlayer;
public:
	// ʹ�ü��ܵķ���ֵ
	enum eUseSkillResult
	{
		USR_NONE_THIS_SKILL = 0,		// �޴˼���
		USR_SUCCESS,					// �ɹ�
		USR_COOLTIMING,					// ������ȴ��
	};

	CGameControl(void);
	virtual ~CGameControl(void);

	void	Run();
	void	ReNew();

	//--------------------�����жϺ���--------------------------
	bool	IsEveryThingNotToDo();					// �Ƿ������κβ���
	bool	IsCanControlCondition();				// �Ƿ���Բ�����״̬
	bool	IsCanMoveCondition();					// �Ƿ�������ߵ�״̬�������˲��ɲ���״̬
	bool	IsCanPickupGoodsCondition();			// �Ƿ���Լ�����Ʒ��״̬
	bool	IsCanUseGoodsCondition(DWORD dwGoodsIndex = 0);	// �Ƿ����ʹ����Ʒ��״̬
	bool	IsCanUsePhysicsSkillCondition();		// �Ƿ����ʹ�������ܵ�״̬
	bool	IsCanUseMagicSkillCondition();			// �Ƿ����ʹ��ħ�����ܵ�״̬
	//----------------------------------------------------------

	//---------------------�������������׶�---------------------
	long	OnMouseEvent();					// ��괦��
	long	OnKeyboardEvent();				// ���̴���
	long	OnAnotherEvent();				// ���������Զ�����ȣ�
	//----------------------------------------------------------

	//---------------------��괦����-------------------------
	long	OnLeftButtonDown();				// ���������´���
	
	long	OnLeftButtonUp();				// �������ɿ�����
	long	OnLeftButtonHolding();			// ��������ס����
	long	OnLeftDoubleButtonDown();		// ������˫��
	long	OnLeftClickShape();				// �������SHAPE�ϵ��´���

	long	OnRightButtonDown();			// ����Ҽ����´���
	long	OnRightButtonUp();				// ����Ҽ��ɿ�����
	long	OnRightButtonHolding();			// ����Ҽ���ס����
	long	OnRightDoubleButtonDown();		// ����Ҽ�˫��
	long	OnRightClickShape();			// ����ҽ���SHAPE�ϵ��´���
	//----------------------------------------------------------

	//---------------------���̴�����-------------------------
	long	ProcessHotKey();				// �����ݼ�����
	//----------------------------------------------------------
	//----------------------------------------------------------

	//-------------------����������غ���-----------------------

	// �����������ϼ����ɷ�����Ʒ���߼��ܵ�С��ʹ�õ���Ӧ
	bool TriggerHotGrid(int gridIndex);
public:
	bool	IsControlAble()					{return m_bControlAble;}				// �Ƿ��ܱ�����
	void	SetControlAble(bool b)			{m_bControlAble=b; if(b==false) SetLastControlTimer(GetCurTickCount());}

	bool	IsMoveAble()					{return m_bMoveAble;}					// �Ƿ��ܱ������ƶ�
	void	SetMoveAble(bool b)				{m_bMoveAble=b;}

	void	SetLastControlTimer(long l)		{m_lLastControlTimer=l;}				// �������һ���ܱ����Ƶ�ʱ��
	long	GetLastControlTimer()			{return m_lLastControlTimer;}			// ������һ���ܱ����Ƶ�ʱ��

	long	GetSendedMoveMsgCount()			{return m_lSendedMoveMsgCount;}			// ����ƶ���Ϣ������
	void	SetSendedMoveMsgCount(long l)	{m_lSendedMoveMsgCount=l;}				// �����ƶ���Ϣ������

	long	GetSendedUseSkillMsgCount()		{return m_lSendedUseSkillMsgCount;}
	void	SetSendedUseSkillMsgCount(long l){m_lSendedUseSkillMsgCount=l;}

	CShape* GetMouseShape()					{return m_pMouseShape;}					// �����굱ǰָ����ͣ���Ķ���
	CShape* GetSelectedShape()				{return m_pSelectedShape;}				// �����굱ǰѡ��Ķ���
	CShape* GetCurAttractShape()			{return m_pCurAttractShape;}			// �����굱ǰ���ڹ����Ķ���
	CShape* GetMouseShapeGoods()			{return m_pMouseShapeGoods;}			// �����굱ǰָ����ͣ������Ʒ

	void	SetMouseShape(CShape* pShape)		{m_pMouseShape=pShape;}				// ������굱ǰָ����ͣ���Ķ���
	void	SetCurAttractShape(CShape* pShape)	{ m_pCurAttractShape=pShape;}
	void	SetMouseShapeGoods(CShape* pShape)	{m_pMouseShapeGoods=pShape;}		// ������굱ǰָ����ͣ������Ʒ
	void	SetSelectedShape(CShape* pShape,bool bIsCToS = true);					// ������굱ǰѡ��Ķ���(Ĭ����Ҫ�����������ѡ����Ϣ)

	long	PickupGoods(CGoods* pGoods);						// ����Ʒ
	void	CancelSelectedTarget();								// ȡ��ѡ�е�Ŀ��

	void	SetIntonateBreak(bool bBreak)	{m_bIntonateBreak = bBreak;}
	bool	GetIntonateBreak();
	//������Ϣ�ı��ƶ�ģʽ
	void	ChangeMoveMode();
	bool	GetChangingMoveMode()	{return m_bChangingMoveMode;}
	void	SetChangingMoveMode(bool b){m_bChangingMoveMode=b;}

	// ���úͻ���Ƿ�����װ״̬
	void	SetIsInTryOnState(bool bState)	{m_bIsInTryOnState = bState;}
	bool	GetIsInTryOnState()				{return m_bIsInTryOnState;}

	// ��ձ���Tabѡ�ж����б�
	void	ClearSelectedList()				{m_lsSelectedShape.clear();}

	// ���úͻ���Ƿ���������ѡ��
	void	SetIsTabTeamMate(bool bLock);
	bool	GetIsTabTeamMate()				{return m_bIsTabTeamMate;}

	void SetLocalIP(DWORD dwIP)	{m_dwLocalIP=dwIP;}
	//��ʱpingServer����
	void PingServer();


	// ���Ƶ�ǰ����Ҽ���ȡ�������Ƿ���Ч
	static bool m_bRButtonDownValid;
private:
	// �ͷ���������ͬ���Ŀ���,����һ����Ϣ����ȥ���Ƿ��յ��˻�Ӧ
	// true:�������Ѿ�����,���߳�ʱ3��,�����ж�
	// false:�ϸ��������ڵȴ��������ķ�����Ϣ,���ʱ�����޷�������ز���
	bool	m_bControlAble;
	// �ͻ����ж��Ƿ���Բ����ƶ��ı��,����������ڽ�����,��ô���޷�����
	bool	m_bMoveAble;
	// �Ѿ����ͳ�ȥ��������Ϣ����,��δ�յ����ص�ǰ���������Է���3����ȥ,Ҳ����3���ӳ�
	long	m_lSendedMoveMsgCount;
	// �Ѿ����ɳ�ȥ��δ�յ��ظ���ʹ�ü�����Ϣ������
	long	m_lSendedUseSkillMsgCount;
	// ��һ�β���������������Ϣ��ʱ��
	long	m_lLastControlTimer;
	// ��������ס�ĳ���ʱ��
	DWORD	m_dwLeftHoldingTime;
	// ����Ҽ�����ס�ĳ���ʱ��
	DWORD	m_dwRightHoldingTime;
	bool	m_bIntonateBreak;				// ���������Ƿ���Ա��ж�
	//�Ƿ��ڸı��ƶ�ģ����
	bool	m_bChangingMoveMode;

	// �Ƿ�����װ״̬
	bool	m_bIsInTryOnState;

	// ѡ��Ŀ��Ŀ�ʼʱ��㣬���ڼ�¼�ӳٷ���ѡ����Ϣ
	DWORD	m_dwSelectedShapeStartTime;



	CShape* m_pMouseShapeGoods;				// ���ָ�����Ʒ
	CShape* m_pMouseShape;					// ���ָ��Ķ���
	CShape* m_pSelectedShape;				// ��ǰѡ�еĶ���
	CShape* m_pCurAttractShape;				// ��ǰ���ڱ������Ķ���
	CShape* m_pPressRBtnShape;				// ��ǰ�����Ҽ�ָ��Ķ���
	long	m_lSelectedType;				// ѡ�ж�������ͣ�����������͵ģ�
	CGUID	m_guidSelected;					// ѡ�ж����GUID������������͵ģ�
	list<CShape *> m_lsSelectedShape;		// ����Tabѡ����Ķ����б�
	bool	m_bIsTabTeamMate;				// �Ƿ���������ѡ��
	//----------------------------------------------------------

	//---------------------�ƶ���غ���-------------------------

	//Ping�������Ĳ�������
	//�ϴ�Pingʱ��
	DWORD m_dwLastPingTime;
	//����IP
	DWORD m_dwLocalIP;
public:
	long	MoveDir(CMoveShape* pShape, long dir,short nDis=2);	// ��ָ�������ƶ�һ��
	long	MoveToCell(float fDestX,float fDestY);//��ĳ�����ƶ�����
	long	MoveToCellByHotKey(float fDestX,float fDestY);
	long	StopMove();//ֹͣ�ƶ�
	CFindPath* GetFindPath();									// Ѱ·

	long	FollowObjectAI();										// ��������ƶ�
	void	SetFollowingObj(long lType,const CGUID& guid);				// ����Ҫ�����obj
	bool	IsAutoMoving()		{return m_AutoMoveMode != AMM_NO;}			// �Ƿ����Զ��ƶ�״̬

	//MainPlayer�ĵ�ǰ����Ϊģʽ
	typedef enum eMainPlayerActionMode
	{
		MPAM_No = 0,//���κ���Ϊ(վ��)
		MPAM_AutoMove,//�Զ��ƶ�
		MPAM_AutoAttack,//�Զ�����
		MPAM_AutoPickGoods,//�Զ�ʰȡ��Ʒ
		//MPAM_AutoCollect,	//�Զ��ɼ�
	}eMPAM;

	//�Զ��ƶ�ģʽ
	typedef enum eAutoMoveMode
	{
		AMM_NO=0,				//���Զ��ƶ�
		AMM_A_SEARCH=0x01,	//A*Ѱ·
		AMM_TO_DESTDIR,	//��һ��Ŀ��㷽���ƶ�
		AMM_FOLLOW_MOUSE,//�������
		AMM_FOLLOW_OBJECT,//����һ��Ŀ�����
		AMM_STOP=0x1000,//����
	}eAMM;

	// ����ģʽ
	typedef enum eAutoAttactMode
	{
		AAM_NO=0,			// �޹���ģʽ
		AAM_FIND_ATTACTER,	// ׷��Ŀ��Ĺ���ģʽ
		AAM_ATTACT,			// ����״̬ģʽ
		AAM_STOP=0x1000,			// ֹͣ
	}eAAM;

private:
	//�ڹ���ģʽ�µķ���ֵ
	enum eAAMRet
	{
		AAMR_Stop=-1,//ֹͣ��ģʽ
		AAMR_Trace=0,//����׷��
		AAMR_Attack=1,//����
		AAMR_Other,//�������
	};
	
	long	m_lFollowingObjType;	//������Ŀ���type
	CGUID	m_FollowingObjID;		//  ��������ID
	float	m_lStartDestX;			//  ��ʼ����ĳ�ʼxλ��
	float	m_lStartDestY;			//  ��ʼ����ĳ�ʼyλ��
	long	m_lFollowMaxDistance;	//������������
	CGUID	m_PickGoodsID;			// �Զ�ʰȡ��Ʒ��ID

	//CGUID	m_CollectGoodsID;		// �Զ��ɼ���Ŀ��GUID

	float	m_fDestX;				// Ŀ�ĵ�x
	float	m_fDestY;				// Ŀ�ĵ�y
	//���Զ��ƶ�AMM_TO_DESTDIRģʽ�£���¼�����ƶ����ĸ��ӡ�
	//������ָ����������ƶ����ĸ��ӣ�������Զ�ֹͣ
	typedef map<POINT,long,point_compare>::iterator itMovedCell;
	map<POINT,long,point_compare>	m_MovedCell;

	eMPAM m_MainPlayerActionMode;	// ����Ϊģʽ
	long m_AutoMoveMode;			// �Զ��ƶ�ģʽ
	bool m_bResetAutoMoveMode;		// ������ģʽ�µ�����,�����ƶ�ģʽ
	long m_AutoAttactMode;			// �Զ�����ģʽ
	

	//��Ҫ���ڵ�ǰ��һ������Ϊģʽ,������Ҫ��һ����ͬ����Ϊģʽ
	//�ȵ�û������Ϊģʽ��ʱ��,ִ�л������Ϊģʽ
	eMPAM m_NextMainPlayerActionMode;	// ��������Ϊģʽ

	//������������������͵ĸ���λ��
	long m_lUpdateX;
	long m_lUpdateY;
	//----------------------------------------------------------

	//---------------------������غ���-------------------------
public:
	
	//---------------------�Զ�������-------------------------
	bool	IsMoveWithCurAction();						// �жϵ�ǰ�����Ƿ�����ƶ�
	bool	IsActWithCurAction();						// �жϵ�ǰ�����Ƿ���Թ���
	bool	AddMovedCell(long lTileX,long lTileY);		// ����ƶ����ĸ���
	void	FollowObject(long lType,const CGUID& guid,long lMaxDistance);	// ����һ��Ŀ�����
	void	DoFollowResult();//�������Ľ��
	void	ContinueMoveToOtherRegion();				// �糡����ļ���Ѱ·
	void	StartAttack(CShape* pShape);				// ��ʼ���������ù���ģʽ
	void	AutoMoveStop();								// ��ֹ�Զ��ƶ�
	void	AutoAttactStop();							// ��ֹ�Զ�����
	long	AutoMoveAI();								// �Զ��ƶ���AI
	long	AutoMoveAIEx();								// ��һ������Զ��ƶ�
	void	AutoPickGood(const CGUID& guGoodID);		// �Զ�ʰȡ��Ʒ
	void	AutoPickGoodStop();							// ��ֹ�Զ�ʰȡ��Ʒ
	void	AutoPickGoodEx();							// �Զ�ʰȡ�����Ȧ����Ʒ����Ҫ�Զ��ƶ�����Ʒ��ʰȡ��
#ifdef MOVE_IN_TILE
	void	AutoMoveTo(float fDestX, float fDestY);		// ��ʼ�Զ���Ŀ�������
	void	AutoMoveToEx(float fDestX, float fDestY);	// ��ʼ�Զ�����,ֱ����Ŀ���ķ����ƶ�
	long	ContinueMoveTo(float fDestX, float fDestY);	// �����м䱻�жϺ����Ѱ·
#else
	void	AutoMoveTo(long lDestX, long lDestY);		// ��ʼ�Զ���Ŀ�������
	void	AutoMoveToEx(long lDestX, long lDestY);		// ��ʼ�Զ�����,ֱ����Ŀ���ķ����ƶ�
	long	ContinueMoveTo(long lDestX, long lDestY);	// �����м䱻�жϺ����Ѱ·
#endif

	//�õ�ĳ���㷽������һ���ƶ�Ŀ���
	long	GetNextMoveDestCell(float fX,float fY,long lMaxDistance,float &fDestX,float &fDestY);
	//�õ�һ�������ϵ���һ���ƶ�Ŀ���
	long	GetNextMoveDestCell(float fRadianDir,float fMaxDistance,float &fDestX,float &fDestY);
	long	FollowMouseMove();							//	�Զ���������ƶ�
	
	long	OnMainPlayerMove();				// �����ƶ�����Ŀ�����Զ�����
	eAAMRet	OnMainPlayerAttack();			// ���ǹ������Զ�����
	long	OnNextPlayerActionMode();		// ִ�л������Ϊģʽ
	eAAMRet	AutoLockAttackAI();				// ������ǰ���ڱ����������AI
	eAAMRet	AutoAttackMoveAI();				// ׷����ǰ���ڱ����������AI
	long	OnAutoPickupGoods();


	void	DoCollect(CMoveShape* pMoveShape);

	//�õ���ǰ��ҵ�����Ϊģʽ
	eMPAM GetMPActionMode()	{return m_MainPlayerActionMode;}
	void SetMPActionMode(eMPAM eActionMode) {m_MainPlayerActionMode = eActionMode;}

	eMPAM GettMPNexActionMode()	{return m_NextMainPlayerActionMode;}
	void SetMPNexActionMode(eMPAM eActionMode) {m_NextMainPlayerActionMode = eActionMode;}

	long GetAutoMoveMode()	{return m_AutoMoveMode;}
	void SetAutoMoveMode(eAMM eMoveMode) {m_AutoMoveMode = eMoveMode;}

	void SetResetAutoMoveMode(bool b) {m_bResetAutoMoveMode  = b;} 
	bool IsResetAutoMoveMode();

	long GetAutoAttackMode()	{return m_AutoAttactMode;}
	void SetAutoAttackMode(eAAM eAttackMode){ m_AutoAttactMode = eAttackMode;}

	bool	StartSkill(DWORD dwIndex, CShape* pAttackShape);		// ��ָ��Ŀ�귢����
	bool	StartSkill(DWORD dwIndex, long lType, const CGUID& ID);	// ��ָ��Ŀ�귢����
	bool	UseOccuSkill(DWORD dwIndex,CShape* pTagShape);			// ʹ�ø�ְҵ����

	void	SetBufferSkillID(DWORD dw);
	void	SetLastPressHotKey(int n)	{m_nLastPressHotKey=n;}			// �������һ��ʹ�õĿ�ݼ�
	void	SetAutoSkillID(DWORD dw);									// �����Զ��ͷż��ܵ�ID

	DWORD	GetBufferSkillID()			{return m_dwBufferSkillID;}		// ��û��弼��ID
	DWORD	GetCurrentSkillID()			{return m_dwCurSkillID;}        // ��õ�ǰ����ID
	int		GetLastPressHotKey()		{return m_nLastPressHotKey;}	// ������һ��ʹ�õĿ�ݼ�
	DWORD	GetAutoSkillID()			{return m_dwAutoSkillID;}		// ����Զ��ͷż��ܵ�ID
	void SetSkillGoods(long lExtendID,long lPos)	{m_lSkillGoodsExtendID = lExtendID;m_lSkillGoodsPos = lPos;}
	void SetUpdateXY(long lX,long lY)	{m_lUpdateX=lX;m_lUpdateY=lY;}
	void GetUpdateXY(long &lX,long &lY);

	void SendTouchNpcMes(CGUID NpcGUID);
	eAAMRet AddSkillByGoods(DWORD m_dwCurSkillID);							// ���һ������Ʒ�����ļ���
	
	void CGameControl::ProcessMoveKey();								//�������ƶ�
private:
	long	m_lSkillGoodsExtendID;				// �������ܵ���Ʒ��������ID
	long	m_lSkillGoodsPos;					// �������ܵ���Ʒ�������е�λ��
	DWORD	m_dwBufferSkillID;					// ���ƶ�״̬ʱ���¼��ܿ�ݼ��Ỻ��������ȵ�����һ�����ӵ�ʱ��ʹ��
	DWORD	m_dwAutoSkillID;					// ��ǰ�Զ��ͷŵļ��ܱ��
	DWORD	m_dwCurSkillID;						// ��ǰ����ʹ�õļ���
	int		m_nLastPressHotKey;					// ��һ�ΰ��µĿ�ݼ�(ר���ṩ�� ���ǻ�â������� ����������ʹ��)
	//----------------------------------------------------------	

// ==============================�糡���ƶ�������08.5.6��=================================
private:
	long m_lTargetRegionID;					//Ŀ�곡��ID
	long m_lSPosX;							// ԭʼ��ʼ��
	long m_lSPosY;
	long m_lTPosX;							// ���յ�Ŀ���
	long m_lTPosY;

public:
	/*
	* ����: ����ָ�������µ�ָ��λ�õ�Ŀ�곡����ָ��λ�õ�·����
	* ժҪ: �÷������Ȼ��ҳ�����·���б�Ȼ�����ÿ�������ڵ��ʾ�����꣬������Ѱ·��
	��һ��������Ѱ·��ʱ����Ҫ���ݸó��������õ�·��������ҡ�
	* ����: ����ΪԴ����ID�����꣨x��y����Ŀ�곡��ID�����꣨x��y��
	* ����ֵ: ���ҳɹ�Ϊtrue,ʧ��Ϊfalse
	* ����: ����
	* ��������: 2008.5.6
	*/	
	bool FindPathToOtherRegion(long lSourRegion, long lSourx, long lSoury, long lTargRegion, long lTargx, long lTargy);

	// ���Ŀ���ĳ���ID
	long GetTargetRegionID()	{return m_lTargetRegionID;}

	DWORD	m_dwMoveMsgCounter;	//����ƶ����͵���Ϣ��

	float	m_fParameterOfDisFromCamara;	//����������˵ľ���

	//�����ƶ����
public:
	void HotKeyToMove();           //��������ķ���Ϊ���������
	bool GetHotKeyDir(float &dir);         // ��õ�ǰ���򰴼�������ﳯ��
	int  GetProKey(bool bCurKeyPress);              //��ð����Ļ�����Ƚ���������ں���Ĳ������жϰ�����Ӧ

private:
	bool m_bHotKeyMove;                      // �ȼ��ƶ���ʶ���������β��ֲ���
	bool m_bCurKeyPress;                     // 
	int  m_HotDirKey[4]; 

	//==============================�ڹҹ��ܣ���־ 10-12-27��=================================
public:
	//�����ڹҹһ�״̬Ϊֹͣ���߿�����initTypeΪֹͣ���ͣ�0ΪĬ�ϣ�1Ϊ���л�����
	void SetAttackHookStart(bool itStart,int initType=0);
	bool GetAttackHooKStart(){ return m_bStartAttack; };
	CAutoAttackHook* GetAutoAttackHook(){return m_pAAHook;};
	bool GetbAutoHookProtected(){ return m_bAutoHookProtected; };
	void SetbAutoHookProtected(bool Protected){ m_bAutoHookProtected=Protected;};	

private:
	CAutoAttackHook* m_pAAHook;              //�ڹҵĽӿ�
	bool m_bStartAttack;                     //���һ��ı�־
	bool m_bAutoHookProtected;               //�ȶ��һ�����״̬


};
