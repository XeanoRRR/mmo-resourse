/** 
* @file   Pet.h
* @author Nikai
* @brief  �ͻ��˳���ϵͳ.
*
* @date   2008-11      
*/

#pragma once
#include "../../../GameClient/ClientApp/MoveShape.h"
#include "../../../Public/Common/BasePetDef.h"

/// ���＼�ܽṹ
struct	tagPetSkillInfo:public tagSkill	
{
    long	lStartCoolTime;				///< ��ǰ��ȴʱ��
    long	lRestorTimeLength;			///< ��������ȴʱ�䳤��
    long	lMinAtkDistance;			///< ��С��������
    long	lMaxAtkDistance;			///< ��󹥻�����
    long	lCostMp;					///< ��Ҫ���Ѷ���MPֵ
    long	lSkillStartTime;			///< ���ܿ�ʼʱ��
    long	lSkillActTime;				///< ���ܶ���ʱ��
    bool	bIsUsingByHotKey;			///< ��ǰ�Ƿ��ɿ�ݼ�ʹ��
    bool	bIsSkillUsingEnd;			///< ����ʹ���Ƿ����
    bool	bSkillIsChanged;			///< �����ܱ��ı��־ �����û���ȡ���Զ��ͷţ��������õ������� ����Ϊ���ı䣩
    tagPetSkillInfo(void)
        :lStartCoolTime(0)
        ,bIsUsingByHotKey(false)
        ,bIsSkillUsingEnd(true)
        ,bSkillIsChanged(false)
        ,lSkillStartTime(-1)
        ,lSkillActTime(-1)
    {}
};

class CFloatMove;
class CPet:public CMoveShape
{
    //�����ս��״̬����˯����������ս�����
public:
    PET_STATE GetPetState() const {return (PET_STATE)m_tagPetDiff->lStateType;}
    void SetPetState(PET_STATE s);

public:
    const static float FAR_FROM_TARGET_DISTANCE;		//��Ŀ������������

    /// ��ս��ģʽ�µķ���ֵ
    enum eFightModeRet
    {
        FMR_NO_SKILL				= 0,				///< û�����ü���
        FMR_SKILL_CD_NOT_ARRIVAL	= 1,				///< ������ȴʱ��δ��
        FMR_SKILL_NOT_ENOUGH_MP		= 2,				///< ��������MP����
        FMR_SKILL_ACTION_NOT_FINISH = 3,				///< ���ܶ���û���
        FMR_SKILL_IS_NOT_AUTO		= 4,				///< ���ܲ����Զ��ͷ�
        FMR_SKILL_LEVEL_FAIL		= 5,				///< ��ǰ�ȼ�ĳ������ʧ��
        FMR_Trace_Near				= 10,				///< ����׷�� ̫��
        FMR_Trace_Far				= 11,				///< ����׷�� ̫Զ
        FMR_Attack					= 12,				///< ����
        FMR_No_TARGET				= 13,				///< û�й���Ŀ�� ���߹���Ŀ������
        FMR_SKILL_NOT_FINISH		= 14,				///< ����û����
        FMR_Other,										///< �������
    };

private:
    /// ����ͣ������λ��
    struct tagStandInfo			
    {
        bool			bOccupy;
        CShape::eDIR	eDirPos;
        PET_TYPE		ePetType;
        tagStandInfo():bOccupy(false),eDirPos(CShape::DIR_DOWN){}
    };

protected:
    static vector<tagStandInfo>	vecPetStandDirPos;

    PET_TYPE m_PetType;		///< ��������
    CGUID m_MasterGUID;		///< ���˵�GUID
    CPlayer* m_Host;		///< ����
    bool m_bIsActived;		///< �Ƿ񱻼���

    tagPetDiff* m_tagPetDiff;				///< ���������������
    tagPetTemp* m_tagPetDetail;				///< �����߼�����

    map<long ,tagPetSkillInfo> m_PetSkillMap;	//���＼�ܱ�	lpos ���� ���ܲ�λ�á� ��0-3���츳���ܣ�4-11����ͨ����

    long						m_lDistanceFromTarget;			//��Ŀ��֮��ľ���

    float						m_lStartDestX;			//  ��ʼ����ĳ�ʼxλ��
    float						m_lStartDestY;			//  ��ʼ����ĳ�ʼyλ��

    long						m_lLastPosX;		//��һ������X
    long						m_lLastPosY;		//��һ������Y
    long						m_lLastDestX;		//��һ��Ŀ�ĵص�X
    long						m_lLastDestY;		//��һ��Ŀ�ĵص�Y

    float						m_lStayModePosX;	//ͣ��ģʽ�¼�¼��X
    float						m_lStayModePosY;	//ͣ��ģʽ�¼�¼��Y

    CGUID						m_curAtkID_Not_Find;		//���ﵱǰ����Ŀ���GUID ���ͻ����ҵ���Ӧshape��ʱ�������
    CMoveShape				*	m_pCurAttackShape;			//��ǰ������Ŀ��
    CMoveShape				*	m_pPassiveAtkTarget;		//��������ģʽ�µ�Ŀ��

    bool						m_bPassiveForceToFight;				//�����ڱ���ģʽǿ�ƹ�����־������ҿ���״̬

    bool						m_bPetIsRetaliateUponNextShape;		//�����Ƿ��Զ�������һ��Ŀ���־  �������ﴦ�ڷ���������ģʽ�£����˻�����յ��˺����� ��

    long						m_lCurUseSkillPos;					//��ǰʹ�ü��ܵ�λ��
    long						m_lLastUseSkillPos;					//��һ��ʹ�ü��ܵ�λ��

    DWORD						m_dwSkillPublicBeginCoolTime;		//��ǰ����������ȴʱ��

    DWORD						m_MoveMsgNum;				//�ͻ��˷����ƶ�����Ϣ����
    DWORD						m_dwStopMoveMsgNum;			//�ͻ��˷���ֹͣ�ƶ���Ϣ����
    DWORD						m_dwSkillMsgSendNum;		//����ʹ�ü��ܷ�����Ϣ����
    DWORD						m_dwSkillSuccMsgRecvNum;	//���＼��ʹ�óɹ��յ���Ϣ
    DWORD						m_dwSkillFailMsgRecvNum;	//����ʹ��ʧ����Ϣ
    DWORD						dwFightBackMsgNum;			//�յ�������������Ϣ���յ��˺� ���� ���Ϊ����ģʽ��
    DWORD						dwForceMove;				//�յ�ǿ���ƶ�
    long						m_lFightAImode;				//���ﵱǰս��AI����

    bool						m_bPetSkillBechanged;		//���＼�ܱ��ı��־�������ܱ�����һ�����ı䱻��Ϊ�Ѿ��ı䣩
    bool						m_bIsInBornState;			//�������״̬��
public:
	CPet();
	virtual ~CPet();
	void	RendShadow(bool bsimple = false);
	long	AutoMoveAI_A(float fDestX,float fDestY);		//�Զ�Ѱ·��Ŀ��(A*)
	

	long	FollowObjectAI(CShape *pTarget);			//����Ŀ�� ��A*
	long	FollowObjectAI(long lDx , long lDy);		//����Ŀ�� ��A*

private:
    CFloatMove  *m_FloatMove;
    //���Զ��ƶ�AMM_TO_DESTDIRģʽ�£���¼�����ƶ����ĸ��ӡ�
    //������ָ����������ƶ����ĸ��ӣ�������Զ�ֹͣ
    typedef map<POINT,long,point_compare>::iterator itMovedCell;
    map<POINT,long,point_compare>	m_MovedCell;
    char    strMoveInfo[256];   //������Ϣ
    bool	AddMovedCell(long lTileX,long lTileY);		                    //����ƶ����ĸ���
	void	MoveToCell(float fDestX, float fDestY);		//�ƶ���ָ������  bFlag �Ƿ���Ҫ����Ϣ
	long	GetNextMoveDestCell(float fX,float fY,long lMaxDistance,float &fDestX,float &fDestY);//�õ�ĳ���㷽������һ���ƶ�Ŀ���
    long	GetNextMoveDestCell(float fRadianDir,float fMaxDistance,float &fDestX,float &fDestY);//�õ�һ�������ϵ���һ���ƶ�Ŀ���
public:
    void AI(void);
    virtual	bool Display();
    virtual void UpdateProperty(CMessage *pMsg);

    //����GUID
    CGUID	GetMasterGUID()				{return m_MasterGUID;}
    //�������ָ��
    CPlayer *GetMaster();
    /// ��������
    void SetHost(CPlayer* pPlayer);

    /// ȡ�ó�������
    const char* GetPetName(void) const { return m_tagPetDiff->szName; }
    /// ���ó�������
    void SetPetName(const char* name) { strcpy_s(m_tagPetDiff->szName, 32, name); }

    //����
    void	SetPetIsActive(bool b)	{m_bIsActived = b;}
    const	bool	IsActive()				{return	m_bIsActived;}

    //��Ϊģʽ
    void SetPetBehavior(PET_MOVE_STYLE  pms);
    PET_MOVE_STYLE GetPetBehavior()			{return (PET_MOVE_STYLE)m_tagPetDetail->lMoveType;}

    //����ģʽ
    void SetPetFightMode(PET_FIGHT_STYLE pfs);
    PET_FIGHT_STYLE	GetPetFightMode()		{return (PET_FIGHT_STYLE)m_tagPetDetail->lFightType;}

    // ����
    void SetPetRace(long lRace) { m_tagPetDetail->lRace = lRace; }
    long GetPetRace(void) const {return m_tagPetDetail->lRace;}

    // ����
    void SetHp(DWORD dwHp)			{m_tagPetDiff->lHp = dwHp;}				
    DWORD GetHp() const				{return	m_tagPetDiff->lHp;}

    //�������ֵ
    void SetMaxHp(DWORD dwMaxHp)		{m_tagPetDetail->lMaxHp= dwMaxHp;}
    DWORD GetMaxHp() const {return m_tagPetDetail->lMaxHp;}

    // ħ��
    void SetMp(DWORD dwMp)			{m_tagPetDiff->lMp = dwMp;}				
    DWORD GetMp() const				{return m_tagPetDiff->lMp;}

    //���ħ��ֵ
    void SetMaxMp(DWORD dwMaxMp)		{m_tagPetDetail->lMaxMp = dwMaxMp;}
    DWORD GetMaxMp() const			{return	m_tagPetDetail->lMaxMp;}

    // ����ֵ
    void SetEnergy(DWORD dwEnergy) { m_tagPetDiff->lEnergy = dwEnergy; }
    DWORD GetEnergy(void) const { return m_tagPetDiff->lEnergy; }

    // �������ֵ
    void SetMaxEnergy(DWORD dwMaxEnergy) { m_tagPetDetail->lMaxEnergy = dwMaxEnergy; }
    DWORD GetMaxEnergy(void) const { return m_tagPetDetail->lMaxEnergy; }

    //�ҳ϶�
    DWORD	GetPetLoyalty() const {return	m_tagPetDiff->lLoyalty;}
    void	SetPetLoyalty(DWORD loyalty)		{m_tagPetDiff->lLoyalty = loyalty;}

    // ����
    long GetPetLife(void) const { return m_tagPetDiff->lLife; }
    void SetPetLife(long lLife) { m_tagPetDiff->lLife = lLife; }

    //����ȼ�
    const	DWORD	GetPetLevel() const {return m_tagPetDiff->lLevel;}
    void	SetPetLevel(DWORD dwLv)				{m_tagPetDiff->lLevel = dwLv;}

    // ������еȼ�
    long GetPetDLev(void) { return m_tagPetDiff->ServantAttr.lDLevel; }
    void SetPetDLev(long dLev) { m_tagPetDiff->ServantAttr.lDLevel = dLev; }

    //����
    const	DWORD	GetPetDLevel() const {return m_tagPetDiff->ServantAttr.lDLevel;}
    void	SetPetDLevel(DWORD dwLv)				{m_tagPetDiff->ServantAttr.lDLevel = dwLv;}

    // Я���ȼ�
    const	DWORD	GetTakeLev() const {return m_tagPetDetail->lTakeLev;}
    void	SetTakeLev(DWORD dwTakeLv)				{m_tagPetDetail->lTakeLev = dwTakeLv;}

    //��������
    const	PET_TYPE	GetPetType() const {return m_PetType;}
	void SetPetType(PET_TYPE type) { m_PetType = type; } 

    //��һ�ȼ�������ֵ
    const	DWORD	GetPetNextLvExp() const {return m_tagPetDetail->lMaxExp;}
    void	SetPetNextLvExp(DWORD NextExp)		{m_tagPetDetail->lMaxExp = NextExp;}

    // ��һ�ȼ���Ϊ����ֵ
    long GetPetNextLvDExp() const { return m_tagPetDetail->lMaxDExp; }
    void SetPetNextLvDExp(long NextDExp) { m_tagPetDetail->lMaxDExp = NextDExp; }

    //����ֵ
    const	DWORD	GetPetExp() const {return	m_tagPetDiff->lExp;}
    void	SetPetExp(DWORD dwExp)				{m_tagPetDiff->lExp = dwExp;}

    // ��Ϊֵ
    long GetPetDExp(void) const { return m_tagPetDiff->ServantAttr.lDExp; }
    void SetPetDExp(long dExp) { m_tagPetDiff->ServantAttr.lDExp = dExp; }

    // �����˺�
    DWORD GetPetPhysiAtk(void) const {return m_tagPetDetail->lPhysicsAtk;}
    void SetPetPhysiAtk(DWORD dwAtk)			{m_tagPetDetail->lPhysicsAtk = dwAtk;}

    //ħ��������
    const	DWORD	GetPetMagicATK() const {return	m_tagPetDetail->lMAtk;}
    void	SetPetMagicATK(DWORD dwAtk)			{m_tagPetDetail->lMAtk = dwAtk;}

    //�������
    const	DWORD	GetPetPhysiDef() const {return	m_tagPetDetail->lPhysicsDef;}
    void	SetPetPhysiDef(DWORD dwDef)			{m_tagPetDetail->lPhysicsDef = dwDef;}

    //ħ������
    const	DWORD	GetPetMagicDef() const {return	m_tagPetDetail->lMDef;}
    void	SetPetMagicDef(DWORD dwDef)			{m_tagPetDetail->lMDef = dwDef;}

    //������
    const	DWORD	GetPetHitRate() const {return	m_tagPetDetail->lHit;}
    void	SetHitRate(DWORD dwRate)			{m_tagPetDetail->lHit = dwRate;}

    //�����ٶ�
    const	DWORD	GetPetIntonateSpeed() const {return	m_tagPetDetail->lMAtkSpeed;}
    void	SetPetIntonateSpeed(DWORD dwSpeed)	{m_tagPetDetail->lMAtkSpeed = dwSpeed;}

    //����
    const	DWORD	GetPetMiss() const {return m_tagPetDetail->lDodge;}
    void	SetPetMiss(DWORD	dwMiss)			{m_tagPetDetail->lDodge = dwMiss;}

    //ˮϵԪ�ؿ���
    DWORD	GetPetElemDefWater() const {return m_tagPetDetail->lElemDef0;}

    //��ϵԪ�ؿ���
    DWORD	GetPetElemDefDust() const {return m_tagPetDetail->lElemDef1;}

    //ľϵԪ�ؿ���	
    DWORD	GetPetElemDefWood() const {return	m_tagPetDetail->lElemDef2;}

    //��ϵԪ�ؿ���
    DWORD	GetPetElemDefGold() const {return m_tagPetDetail->lElemDef3;}	

    //��ϵԪ�ؿ���
    DWORD	GetPetElemDefFire() const {return	m_tagPetDetail->lElemDef4;}

    //����
    DWORD	GetPetStrenth() const {return m_tagPetDetail->lStrenth;}

    // ��
    DWORD GetPetDexterity() const { return m_tagPetDetail->lDexterity; }

    // ����
    DWORD GetPetCon() const { return m_tagPetDetail->lCon; }

    // ��־
    DWORD GetPetIntellect() const { return m_tagPetDetail->lIntellect; }

    // ����
    DWORD GetPetSpiritualism() const { return m_tagPetDetail->lSpiritualism; }

    // �����ӵ�
    DWORD GetPetStrenthPoint() const { return m_tagPetDiff->ServantAttr.savvyVal.lStrenth; }
    // ���ӵ�
    DWORD GetPetDexterityPoint() const { return m_tagPetDiff->ServantAttr.savvyVal.lDexterity; }
    // ���Ǽӵ�
    DWORD GetPetConPoint() const { return m_tagPetDiff->ServantAttr.savvyVal.lCon; }
    // ��־�ӵ�
    DWORD GetPetIntellectPoint() const { return m_tagPetDiff->ServantAttr.savvyVal.lIntellect; }
    // ���Լӵ�
    DWORD GetPetSpiritualismPoint() const { return m_tagPetDiff->ServantAttr.savvyVal.lSpiritualism; }

    // ��������
    DWORD GetPetStrenthTalent() const { return m_tagPetDiff->ServantAttr.talentVal.lStrenth; }
    // ������
    DWORD GetPetDexterityTalent() const { return m_tagPetDiff->ServantAttr.talentVal.lDexterity; }
    // ��������
    DWORD GetPetConTalent() const { return m_tagPetDiff->ServantAttr.talentVal.lCon; }
    // ��־����
    DWORD GetPetIntellectTalent() const { return m_tagPetDiff->ServantAttr.talentVal.lIntellect; }
    // ��������
    DWORD GetPetSpiritualismTalent() const { return m_tagPetDiff->ServantAttr.talentVal.lSpiritualism; }

    ////���ܵ�
    // const	DWORD	GetPetSkillPoint()		{return m_tagPetDiff->lSP;}
    // void	SetPetSkillPoint(DWORD	dwSp)		{m_tagPetDiff->lSP = dwSp;}

    const CFindPath* GetPetFindPath() {return &m_FindPath;}

    long			ContinueMoveTo(float fDestX, float fDestY);	

    /// ���뼤���������
    virtual bool	DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData = true);
	/// �����������
	bool DecodeAttrFromDataBlock(DBReadSet& setReadDB);

    void			DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB);		//���³�������

    void			SetPetAttibute(WORD wAttrType,long lData);		//���ó���ĳ������

    bool			IsMainPlayerPet();			//�Ƿ����Լ��ĳ���

    void			EnterRegion(bool isSelf = false);				//���볡��

    void			PetStopMove(bool Flag = false);

    DWORD			GetMoveMsg()		{return	m_MoveMsgNum;}
    DWORD			GetStopMoveMsg()	{return	m_dwStopMoveMsgNum;}	

    void	AddSkill(tagPetSkillInfo &skill);	//��Ӽ���
    tagPetSkillInfo * GetPetSkillInfoByPos(long lPos);		//���ݼ��ܲۻ�ȡ����	��0-3���츳���ܣ�4-11����ͨ����
    tagPetSkillInfo * GetPetSkillInfoByID(long lSkillID);

    long			GetPetSkillCount()	{return (long)m_PetSkillMap.size();}		//�õ�������				

    eFightModeRet	SkillAI();	//��ѯ�����б� ���ص������ͷŻ����б�
    void	UsePetSkill(long lSkillPos);	//ʹ�ü���

    void	SetCurAttackShape(CMoveShape *pTarget);		//���õ�ǰ������Ŀ��
    void	CPet::SetCurAttackShape(CGUID & guid);

    CMoveShape*	GetCurAttackShape(){return m_pCurAttackShape;}		//��ȡ��ǰ������Ŀ��
    void	SetSpeed(float fSpeed);		//���ó����ƶ��ٶ�

    ///////------------AI ���-----------------////////////
    eFightModeRet	PetFightAI();			//���﹥��Ŀ�����AI
    void	AutoRetaliateUponAI();	//���򻹻� 

    void	SetPetCurUseSkillPos(long lPos)	{m_lCurUseSkillPos = lPos;}				//���õ�ǰʹ�õļ���
    long	GetPetCurUseSkillPos()			{return m_lCurUseSkillPos;}
    void	UpdateTargetPos(long lPosX,long lPosY);							//���¸���Ŀ����Ϣ
    void	UpdateTargetPos(CMoveShape *pTarget);

    void	SetForceToAttack()	{m_bPassiveForceToFight = true;}		//����ǿ�ƹ���

    void	SetIsRetaliateUponNextShape(bool b)		{m_bPetIsRetaliateUponNextShape  = b;}	//�Ƿ��Զ�����

    bool	CheckAttackTargetIsValidate(CMoveShape * pAtkShape);		//��鹥��Ŀ���Ƿ�Ϸ�

    bool	IsPetMoveWithCurAction();						// �жϵ�ǰ�����Ƿ�����ƶ�

    bool	IsPetActWithCurAction();						// �жϵ�ǰ�����Ƿ���Թ���

    long	GetPetSkillSendMsgCount()			{return m_dwSkillMsgSendNum;}

    long	GetPetSkillSuccRecvMsgCount()		{return m_dwSkillSuccMsgRecvNum;}

    long	GetPetSkillFailRecvMsgCount()		{return m_dwSkillFailMsgRecvNum;}
    void	SkillSuccReceived()					{++m_dwSkillSuccMsgRecvNum;}

    void	SkillFailReceived()					{++m_dwSkillFailMsgRecvNum;}

    void	PetContAttRecved()					{++dwFightBackMsgNum;}

    long	GetPetCountAttMsgCount()			{return dwFightBackMsgNum;}

    bool	IsSkillBeBlcoked();					//�жϼ����Ƿ��赲�ˡ�

    long	GetPetFightAI_Mode()				{return	m_lFightAImode;}		//�õ���ǰ����ս��AI����

    void	ClearFindPath()						{m_FindPath.GetPathList()->clear();}		//���A*·����

    void	ProcessPetHotKey();					//��������ݼ�

    void	LetMeAttack();						//�ó���ȥ����

    void	SetPetSkillChanged(bool b)			{m_bPetSkillBechanged = b;}

    bool	IsPetSkillChanged()					{return m_bPetSkillBechanged;}

    bool	IsInBornState()						{return m_bIsInBornState;}

    void	SetInBornState(bool b)				{m_bIsInBornState = b;}

    bool	CheckBornIsComplete();				//��������Ƿ����

    void	ChangeToNextSkill();				//����ʹ�óɹ��л�����һ������

    bool	IsPreviousSkillActionFinish();		//��һ�������Ƿ����

    CPet::eFightModeRet	PetSkillIsCanRelease(tagPetSkillInfo *pSkillInfo);	//�жϳ��＼���Ƿ�ﵽ�ͷ�����

    CShape::eDIR	GetRandomPlaceBesideMaster();	//��ȡ������߿�ͣ����λ��

    static void	ConfusePetStandInfo();			// ����ͣ��վ��λ��

    //sends messages
    void AskforChangeName(const char* newName);
    void AskforChangeState(PET_STATE newState) const;
    void AskforDelete() const;
    void AskforLvUp() const;
    void AskforDLvUp() const;

	void OnHostDied(void);					///< ��������
	void OnDied(void);						///< ��������
};