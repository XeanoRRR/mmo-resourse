#pragma once
class CSkillAI;
class CMoveShape;
class CPlayer;
#include "Shape.h"
#include "SkillListXml.h"
#include "SkillAI.h"
#include "SkillDecorator.h"

///���ܵ�װ����
class CSkillXml : public CShape,public SkillComponent
{
private:
	CSkillXml();
public:
	CSkillXml(SkillComponent* com);
	
	virtual ~CSkillXml(void);
protected:
	SkillComponent* m_com;//ָ��װ����
public:
	virtual void AI(void);
	virtual bool Display(void);
	/////////////////////////////
	//SkillComponent�Ľӿ�

	///ִ�м��ܿ�ʼ����
	bool StepBegin(long lID, long lLevel, 
		long sType, const CGUID& sID,
		long lDestX, long lDestY, float fDir,
		vector<long> vType,
		vector<CGUID> vID);
	/// ִ�з��ͼ�����Ϣ����
	bool virtual StepSend(DWORD dwID,DWORD dwLevel,
		CMoveShape *sShape, CMoveShape *dShape,
		long dX, long dY);
	///ִ�м���ʵʩ����
	bool  StepRun(long lID,long lLevel,
		long sType,const CGUID& sID,
		long dX,long dY,float fDir,
		vector<long>vType,vector<CGUID>vID);
	///ִ�м��ܽ�������
	bool  StepEnd(long lID,long lLevel,
		long sType,const CGUID& sID,float fDir);
	///ִ�м��ܻ��ղ���
	bool  StepSummon();
	/// ���ü��ܼ�ֵ
	void SetSkillKey(long lSkillKey){m_lSkillKey = lSkillKey;}
	//////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// ���ߺ���
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// ��������ʱ��
	void virtual IntonateTime(long lTime);
	/// ȡ��ʩ���߱��
	virtual const CGUID& GetSourID(){return m_SourID;}
	/// ȡ�������ٻ���Ч���
	virtual vector<long> GetSummonEffect(){return m_vecSummonEffect;}
	/// ȡ�������ٻ���Ч
	virtual vector<long> GetSummonSound(){return m_vecSummonSound;}
	///ȡ���ٻ�ʢ������ʱ��
	virtual DWORD GetSummonLifeTime() { return m_lSummonLifeTime ;}
	/// ȡ��ʩ��������
	virtual long GetSourType(){return m_lSourType;}
	
	/// ȡ�ü��ܼ�ֵ
	virtual long GetSkillKey(){return m_lSkillKey;}

	/// ȡ��ʩ����shape����
	virtual CMoveShape* GetSourMoveShape(){return m_pMoveShape;}
	
	/// ȡ��Ŀ��shape����
	virtual vector<CMoveShape*> GetDestMoveShape(){return m_vecDestMoveShape;}

	/// ȡ��Ŀ��������
	virtual long GetDestType(){return m_lDestType;}

	/// ȡ��Ŀ����
	virtual const CGUID& GetDestID(){return m_DestID;}

	/// ȡ��Ŀ����λ��x
	virtual long GetDestX(){return m_lDestX;}

	/// ȡ��Ŀ����λ��y
	virtual long GetDestY(){return m_lDestY;}

	/// ȡ�ü��ܱ��
	virtual long GetSkillID(){return m_lSkillID;}	

	/// ȡ�ü��ܵȼ�
	virtual long GetSkillLevel(){return m_lSkillLevel;}

	/// ȡ�÷�����Ч��ʼλ��
	virtual vector<eLOCATOR> GetFlyEffectLoc(){return m_vecFlyEffectPos;}

	/// ȡ�÷�����Ч����ʱ��
	virtual vector<long> GetFlyTime(){return m_vecFlyTime;}

	/// ȡ�÷�����Ч�ӳ�ʱ��
	virtual vector<long> GetFlyDelay(){return m_vecFlyDelay;}

	/// ȡ�����еķ�����Ч���
	virtual vector<long> GetFlyEffect(){return m_vecFlyEffect;}

	/// ȡ�����з�����Ч����
	virtual vector<long> GetFlyEffectIndex(){return m_vecFlyEffectIndex;}
	
	/// ȡ�÷�����ЧX�᷽��ƫ�ƽǶȶ���
	virtual vector<long> GetFlyEffectTurnX(){return m_vecFlyEffectTurnX;}

	/// ȡ�÷�����ЧY�᷽��ƫ�ƽǶȶ���
	virtual vector<long> GetFlyEffectTurnY(){return m_vecFlyEffectTurnY;}

	/// ȡ�÷�����Ч�Ƿ�ѭ�����ŵı�־
	virtual bool IsFlyEffectLoop()	{return m_bFlyEffectCycle;}


	/// ȡ������ٻ�����X�᷶Χ
	virtual long GetRandomSummonX(){return m_lRandomSummonX;}

	/// ȡ������ٻ�����Y�᷶Χ
	virtual long GetRandomSummonY(){return m_lRandomSummonY;}

	/// ȡ������ٻ������ٻ�������
	virtual long GetRandomSummonNum(){return m_lRandomSummonNum;}

	/// ȡ�ü��ܵ�����ʱ��
	virtual long GetIntonateTime(){return m_lIntonateTime;}

	/// ȡ������������ЧID
	virtual long GetIntonateEffect(){return m_lIntonateEffect;}

	/// ������õķ�����Ч�ĸ���
	virtual long GetFlyEffectNum(){return m_lFlyEffectNum;}

	/// ������õļ�����Ч��λ��
	virtual eLOCATOR GetReleaseEffectPosition()	{return m_eReleaseEffectPosition;}

	/// ���������׶���Ҫѭ������
	virtual WORD GetIntonateMainAct()	{return m_wIntonateMainAct;}

	/// ��������ٻ������ٻ�����ӳٲ���ʱ��
	virtual long GetRandomSummonDelayTime()	{return m_lRandomSummonDelayTime;}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// ���ܺ���
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	
	/// �ж�Ŀ�����
	virtual BOOL SkillObject(long l);
	
	/// �ж�ǿ��Ŀ�����
	virtual BOOL SkillForceObject(long l);
	
	/// �ж��Ƿ���ԶԷ�pvp��ɫ��������
	virtual BOOL FirstAtt(long l);
	
	/// �ж�Ҫʩ�ż��������buff
	virtual BOOL SkillStarBuff(long l);
	
	/// �ж�ʩ�ż���ʱ��ұ��봦�ں�����Ϸ״̬
	virtual BOOL SkillStarState(long l);
	
	/// ����Ҫʩ�ż�������װ����id
	virtual void SkillStarEquipId(long l);
	
	/// ����Ҫʩ�ż�������װ������ںδ�
	virtual void SkillStarEquipPlace(long l);
	
	///	�ж�Ҫʩ�ż��ܱ����װ������
	virtual BOOL SkillStarEquipUse();

	/// ����ʩ�ż����������id
	virtual void SkillItemId(long l);

	/// ����ʩ�ż������������Ҫ������
	virtual void SkillItemNum(long l);

	/// ����ʩ�ż��������������ںδ�
	virtual void SkillItemPlace(long l);

	/// �ж�ʩ�ż��������������
	virtual BOOL SkillUseItem();

	/// �ж�ʩ�ż�������hp
	virtual BOOL SkillHp(long l);

	/// �ж�ʩ�ż�������mp
	virtual BOOL SkillMp(long l);

	/// �ж�ʩ�ż�����������
	virtual BOOL SkillEnergy(long l);
	
	/// �ж�������Сʩ�ž���
	virtual BOOL IntonateLeastSpace(long l);

	/// �ж��������ʩ�ž���
	virtual BOOL IntonateMostSpace(long l);

	/// �ж�ʩ����Сʩ�ž���
	virtual BOOL FireLeastSpace(long l);

	/// �ж�ʩ�����ʩ�ž���
	virtual BOOL FireMostSpace(long l);
protected:

	/// ���������׶���Ҫѭ������
	void IntonateMainAct(WORD l);
	
	/// ������������ID
	void IntonateAct(long l);

	/// ��������������ЧID
	void IntonateEffect(long l);

	/// ����������Ч����
	void IntonateEffectIndex(long l);

	/// ������Ч�Ƿ����������
	void IntonateEffectByEquip(long l);

	/// ����������Ч��λ��
	void IntonateEffectPosition(long l);

	/// ����������Ч�Ƿ�ѭ��
	void IntonateEffectCycle(long l);
	
	/// ��ʼ����
	void IntonatePlay();

	/// ����������Чid
	void IntonateSound(long l);

	/// ������Ч�Ƿ����������
	void IntonateSoundByEquip(long l);

	/// ����������Ч�Ƿ�ѭ��
	void IntonateSoundCycle(long l);

	/// ��ʼ����������Ч
	void IntonateSoundPlay();

	/// �����Ƿ���Ա����
	void IntonateBreak(long l);

public:

	/// ִ�м���ʵʩ����
	//virtual bool StepRun(long lID, long lLevel, long sType,const CGUID& sID,long dX, long dY,  float fDir, vector<long> vType, vector<CGUID> vID);

	/// ����ʩ�Ŷ���ID
	virtual void ReleaseAct(long l);

	/// ����ʩ�ź�������ID
	virtual void ReleaseActEx(long l);

	/// ����ʩ�Ŷ�����Чid
	virtual void ReleaseEffect(long l);

	/// ���ö�����Ч����
	virtual void ReleaseEffectIndex(long l);

	/// ʩ����Ч�Ƿ����������
	virtual void ReleaseEffectByEquip(long l);

	/// ����ʩ����ЧĿ��
	virtual void ReleaseEffectDest(long l);

	/// ����ʩ����Ч��λ��
	virtual void ReleaseEffectPosition(long l);

	/// ����ʩ����Ч�Ƿ�ѭ��
	virtual void ReleaseEffectCycle(long l);

	/// ��ʼʩ�Ŷ���
	virtual void ReleasePlay();

	/// ����ʩ����Чid
	virtual void ReleaseSound(long l);

	/// ʩ����Ч�Ƿ����������
	virtual void ReleaseSoundByEquip(long l);

	/// ����ʩ����Ч�Ƿ�ѭ��
	virtual void ReleaseSoundCycle(long l);

	/// ����ʩ����ЧĿ��
	virtual void ReleaseSoundDest(long l);

	/// ����ʩ����Ч
	virtual void ReleaseSoundPlay();
	
	/// ��Ƶ��(�ٷֱ�)
	virtual void ShockRatio(long l);

	/// �𶯼��ʱ
	virtual void ShockInterval(long l);

	/// ��ʱ�䳤��
	virtual void ShockTimeLength(long l);

	/// ��ʼ��
	virtual void ShockPlay();

	/// ���÷�����Чid
	virtual void FlyEffect(long l);

	/// ���÷�����Ч����
	virtual void FlyEffectIndex(long l);

	/// ������Ч�Ƿ����������
	virtual void FlyEffectByEquip(long l);

	/// ���÷�����Ч�Ӻδ�����
	virtual void FlyEffectPosition(long l);

	/// ���÷�����Ч�Ƿ�ѭ������
	virtual void FlyEffectCycle(long l);

	/// ���÷�����ЧX�᷽��ƫ�ƽǶ�
	virtual void FlyEffectTurnX(long l);

	/// ���÷�����ЧY�᷽��ƫ�ƽǶ�
	virtual void FlyEffectTurnY(long l);

	/// ��ʼ���ж���
	virtual void FlyPlay();

	/// ���÷�����Чid
	virtual void FlySound(long l);

	/// ������Ч�Ƿ����������
	virtual void FlySoundByEquip(long l);

	/// ���÷�����Ч�Ӻδ��ͷ�
	virtual void FlySoundPosition(long l);

	/// ���÷�����Ч�Ƿ�ѭ��
	virtual void FlySoundCycle(long l);

	/// ��ʼ���ŷ�����Ч
	virtual void FlySoundPlay();

	/// ���÷����ٶ�(����һ���������õ�ʱ��)
	virtual void FlyTime(long l);

	/// ���÷����ӳ�ʱ��
	virtual void FlyDelay(long l);

	/// ������Ч�ĸ���
	virtual void FlyEffectNum(long l);

	/// �����ٻ�������Ч
	virtual void SummonEffect(long l);

	/// �����ٻ���Ч
	virtual void SummonSound(long l);

	/// �����ٻ���������
	virtual void SummonLifeTime(long l);

	/// ��������ٻ�����X�᷶Χ
	virtual void RandomSummonX(long l);

	/// ��������ٻ�����Y�᷶Χ
	virtual void RandomSummonY(long l);

	/// ��������ٻ������ٻ�������
	virtual void RandomSummonNum(long l);

	/// ��������ٻ������ٻ�����ӳٲ���ʱ��
	virtual void RandomSummonDelayTime(long l);

	/// �����Ƿ���ʾ�Ӷ������Ĺ�ӰЧ��
	virtual void WeaponEffect(long l);

	/// �ı�ָ��������
	virtual void ChangeProperty(long lvalue, const string& strValue);

	/// ���õ�ǰ�����Ĺ涨ʱ��
	virtual void SetActTime(DWORD dwTime);

	/// ���
	virtual queue<WORD>* GetRunStepActExQueue()	{return &m_queueRunStepActEx;}

	/// ���úͻ���ͷŽ׶ε����һ����̶����Ƿ���ѭ������
	virtual void SetActExIsCycle(BOOL bIsCycle)	{m_bIsLastActExCycle = bIsCycle;}

	virtual BOOL IsActExCycle()					{return m_bIsLastActExCycle;}

protected:

	/// ִ�м��ܽ�������
	//bool StepEnd(long lID, long lLevel, long sType, const CGUID& sID, float fDir);

	/// �趨ɾ����Ч
	void DelLocEffect(long l);

	/// �趨ɾ����Ч����
	void DelLocEffectIndex(long l);

	/// ��ʼɾ����Ч
	void StartDelEffect(long l);

//=================07.12.20����===============
// �����׶ε���Ч����ؽṹ
public:
	/// ����
	enum eSTEP
	{
		STEP_START = 0,
		STEP_RUN,
		STEP_END,
		STEP_NUM,
	};
	/// ĳһȷ����Ч�������ṹ������ͨ�����е�һ��ֵ��ȷ��һ����Ч
	struct tagEffectIndex
	{
		long lEffectIndex;			/// ���õ���Ч����
		CGUID guidEffect;			/// ��Ч��GUID
		tagEffectIndex()
		{
			lEffectIndex = -1;
			guidEffect = CGUID::GUID_INVALID;
		}
	};
	/// ��Ч��ض���Ľṹ���ö������Ϊ������shape�ȣ�ͨ�����ͺ�GUID�����֣�
	struct tagObjectEffect
	{
		long lType;			/// ��������
		CGUID guid;			/// �����GUID
		list<tagEffectIndex> lsEffectIndex; /// ��Ч�б�
		tagObjectEffect()
		{
			lType = 0;
			guid = CGUID::GUID_INVALID;
		}
	};
public:
	/// ȡ�õ�ǰ�����У��ض��׶ε�ָ���������Ч�б�
	virtual list<tagEffectIndex> *GetObjectEffectList(long lStep, long lType, CGUID guid);
	/// ȡ�õ�ǰ�����У�ָ���׶ε�������Ч�����б�
	virtual vector<tagObjectEffect> *GetEffectIndex(long lStep);

	/// ɾ����ǰ�����У��ض��׶ε�ָ���������Ч�б�
	virtual bool DelObjectEffect(long lStep, long lTypeObj, CGUID guidObj);

	/// ɾ����ǰ���ܣ���ָ���׶ε�ָ�������������Ч
	virtual bool DelAllEffectListByObject(long lStep, long lTypeObj, CGUID guidObj);

	/// ɾ����ǰ���ܣ���ָ���׶ε����ж����������Ч
	virtual bool DelAllEffectListByStep(long lStep);

	/// ��õ�ǰ�������ڵĽ׶�
	virtual long GetCurStep()	{return m_lStep;}

	/// ����ѭ����Ч�ӳ�ɾ��ʱ��
	virtual void SetCycleEffectDelayTime(long l)	{m_lCycleEffectDelayTime = l;}
///============================================

private:

	/// ִ�����м��ܲ���
	BOOL ExecuteStep(int iWordKey, long lValue, const string& strValue);

	/// �ж���Щְҵ����ʹ�õ�ǰ����
	BOOL SkillOccType(CSkillListXml::tagSkill *pSkill, CMoveShape *sShape);

	CMoveShape *m_pLauncher;		/// ���ܷ�����Ϣ�׶ε�ʩ����
	CShape *m_pDestShape;		/// ���ܷ�����Ϣ�׶ε�Ŀ����
	CMoveShape *m_pMoveShape;	/// �������͵�ʩ���ߣ������ҡ�������
	vector<CMoveShape*> m_vecDestMoveShape;/// �������͵�Ŀ�꣨�����ҡ�������

	vector<tagObjectEffect> m_vecEffectIndex[STEP_NUM];		/// �������Ч�����б��ֳ������׶Σ�

	long m_lStep;				/// �ü��ܵ�ǰ�����Ľ׶Σ�0����ʼ��1�����У�2��������

	long m_lSourType;			/// ʩ��������
	CGUID m_SourID;				/// ʩ���߱��
	long m_lDestType;			/// Ŀ��������
	CGUID m_DestID;				/// Ŀ���߱��
	long m_lDestX;				/// Ŀ��xλ��
	long m_lDestY;				/// Ŀ��yλ��
	long m_lSkillKey;			/// ���ܼ�ֵ
	long m_lSkillID;			/// ���ܱ��
	long m_lSkillLevel;			/// ���ܵȼ�

	CSkillAI *m_pSkillAI;			/// ����AI��
	float m_fDir;				/// ʩ������Եķ���

	vector<long> m_vecStarEquipId;		/// ����װ��id�б�
	vector<long> m_vecStarEquipPlace;	/// ����װ�����õ�λ���б�
	
	long m_lItemId;				/// ������Ʒid
	long m_lItemNum;			/// ������Ʒ��
	long m_lItemPlace;			/// ������Ʒ���õ�λ��

	//====================================================================================///

	long m_lIntonateTime;		/// ����ʱ�䣨07.12.11������ӣ���Ӧ����Ϊ72��
	WORD m_wIntonateMainAct;	/// �����׶���Ҫ����ID

	long m_lIntonateEffect;		/// ������Ч
	long m_lIntonateEffectIndex;/// ������Ч����
	bool m_bIntonateEffectByEquip;	/// ������Ч�Ƿ����������
	bool m_bIntonateEffectCycle;/// �����Ƿ�ѭ��
	eLOCATOR m_eIntonateEffectPosition;	/// �����󶨵�λ��

	long m_lIntonateSound;		/// ��������
	bool m_bIntonateSoundByEquip; /// ������Ч�Ƿ����������
	bool m_bIntonateSoundCycle;	/// ���������Ƿ�ѭ��
	int m_IntonateSoundChannel; ///�ʳ�����channel

	///====================================================================================///

	queue<WORD>	m_queueRunStepActEx;	/// �ͷŽ׶εĺ��������б�
	BOOL		m_bIsLastActExCycle;	/// ���һ���ͷŽ׶κ�̶����Ƿ�ѭ��

	long m_lReleaseEffect;		/// ʩ����Ч
	long m_lReleaseEffectIndex;	/// ʩ����Ч����
	bool m_bReleaseEffectByEquip;	/// ʩ����Ч�Ƿ����������
	long m_lReleaseEffectDest;	/// ʩ����ЧĿ��
	eLOCATOR m_eReleaseEffectPosition;	/// ʩ����Ч��λ��
	bool m_bReleaseEffectCycle;	/// ʩ����Ч�Ƿ�ѭ��

	long m_lReleaseSound;		/// ʩ����Ч����
	bool m_bReleaseSoundByEquip;	/// ʩ����Ч�Ƿ����������
	bool m_bReleaseSoundCycle;	/// ʩ����Ч�����Ƿ�ѭ��
	long m_lReleaseSoundDest;	/// ʩ����ЧĿ��

	float m_fShockRatio;		/// ��Ƶ��
	long m_lShockInterval;		/// �𶯼��
	long m_lShockTimeLength;	/// ��ʱ�䳤��

	long m_lFlyEffectNum;		/// ������Ч�ĸ���
	long m_lFlyEffect;			/// ������Ч
	long m_lFlyEffectIndex;		/// ������Ч����
	bool m_bFlyEffectByEquip;	/// ������Ч�Ƿ����������
	eLOCATOR m_eFlyEffectPosition;	/// ������Ч�Ӻδ�����
	bool m_bFlyEffectCycle;		/// ������Ч�Ƿ�ѭ��
	long m_lFlyEffectTurnX;				/// ������ЧX�᷽��ƫ�ƽǶ�
	long m_lFlyEffectTurnY;				/// ������ЧY�᷽��ƫ�ƽǶ�
	vector<long> m_vecFlyEffect;		/// ������Ч����
	vector<long> m_vecFlyEffectIndex;	/// ������Ч��������
	vector<eLOCATOR> m_vecFlyEffectPos;	/// ������Ч���������
	vector<long> m_vecFlyTime;			/// ����ʱ��
	vector<long> m_vecFlyDelay;			/// �����ӳ�ʱ��
	vector<long> m_vecFlyEffectTurnX;	/// ������ЧX�᷽��ƫ�ƽǶȶ���
	vector<long> m_vecFlyEffectTurnY;	/// ������ЧY�᷽��ƫ�ƽǶȶ���
	vector<long> m_vecSummonEffect;		/// �ٻ�������Ч
	vector<long> m_vecSummonSound;		/// �ٻ���Ч
	long m_lSummonLifeTime;				/// �ٻ���������
	long m_lRandomSummonX;				/// ����ٻ�����X�᷶Χ
	long m_lRandomSummonY;				/// ����ٻ�����Y�᷶Χ
	long m_lRandomSummonNum;			/// ����ٻ������ٻ�����
	long m_lRandomSummonDelayTime;		/// �漴�ٻ������ٻ�����ӳٲ���ʱ��

	long m_lFlySound;			/// ��������
	bool m_bFlySoundByEquip;	/// ������Ч�Ƿ����������
	bool m_bFlySoundCycle;		/// ���������Ƿ�ѭ��
	eLOCATOR m_eFlySoundPosition;	/// ������Ч�Ӻδ�����
	
	long m_lDelLocEffect;		/// ɾ����Ч
	long m_lDelLocEffectIndex;	/// ɾ����Ч����

	long m_lCycleEffectDelayTime;	/// ѭ����Ч���ӳ�ɾ��ʱ��
    //===========================================================================//
private:
	vector<int> m_VecFlySoundChannel;        ///������Ч��Channel
	CPlayer *ConvertFromPetOrPlayer(CMoveShape *pTransform);		///��������ת�� ָ��
	CPlayer *ConvertFromPetOrPlayer(CShape *pTransform);		///��������ת�� ָ��
public:
	virtual void SetFlySoundChannel(int nChannel);
	virtual void StopFlySound();
};

//�µļ���װ��ʵ����
class ConcreteDecSkillXml: public CSkillXml
{
public:
	///���캯��
	ConcreteDecSkillXml(SkillComponent* com);
	///��������
	~ConcreteDecSkillXml();
};


