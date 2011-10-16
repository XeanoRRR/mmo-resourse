//.�ͻ��˳���ϵͳ//
//	AddBy: Nikai	2008.11
#pragma once
#include "..\MoveShape.h"
#include "..\Player.h"
#include "..\App\petsystem\BasePetDef.h"

class CPet : public CMoveShape
{
public:
	//��ս��ģʽ�µķ���ֵ
	enum eFightModeRet
	{
		FMR_NO_SKILL				= 0,	//û�����ü���
		FMR_SKILL_CD_NOT_ARRIVAL	= 1,	//������ȴʱ��δ��
		FMR_SKILL_NOT_ENOUGH_MP		= 2,	//��������MP����
		FMR_SKILL_ACTION_NOT_FINISH = 3,	//���ܶ���û���
		FMR_SKILL_IS_NOT_AUTO		= 4,	//���ܲ����Զ��ͷ�
		FMR_SKILL_LEVEL_FAIL		= 5,	//��ǰ�ȼ�ĳ������ʧ��
		FMR_Trace_Near				= 10,				//����׷�� ̫��
		FMR_Trace_Far				= 11,				//����׷�� ̫Զ
		FMR_Attack					= 12,				//����
		FMR_No_TARGET				= 13,				//û�й���Ŀ�� ���߹���Ŀ������
		FMR_SKILL_NOT_FINISH		= 14,	//����û����
		FMR_Other		,					//�������
	};
public:
	CPet();
	virtual ~CPet();

protected:
	//	��Ӧ���׽���
	long m_lSocketID;
	//	����
	CPlayer* m_pMaster;
	//	����
	CClientRegion* m_pRegion;

	CGUID m_MasterGuid;		//	����guid
	long m_lPetType;		//	��������
	bool m_bActive;			//	�Ƿ񼤻�

	CGUID m_guidCurAtk;
	CMoveShape* m_pCurAtkShape;
	CMoveShape* m_pPassiveAtkShape;

	//	����
	long m_lCurUseSkillPos;
	long m_lLastUseSkillPos;

	//	��������
	tagPetDiff	* m_tagPetAttr;
	//	�����߼�
	tagPetDetail	* m_tagPetLogic;
	long m_lCurFightMode;

	map<long,tagPetSkill>	m_mapSkillList;

	//	�ƶ�
	float m_lStartAimX;		//	��ʼ����ĳ�ʼλ��
	float m_lStartAimY;
	long m_lLastPosX;		//	�ϴ�����
	long m_lLastPosY;


public:
	long GetPetSocketID()			{ return m_lSocketID; }
	void SetPetSocketID(long lSocketID) { m_lSocketID = lSocketID; }

	const CGUID & GetMasterGuid()	{ return m_MasterGuid; }
	void SetMasterGuid(const CGUID& guid) { m_MasterGuid = guid; }

	CClientRegion* GetPetRegion()		{ return m_pRegion; }
	void SetPetRegion(CClientRegion* pRegion)	{ m_pRegion = pRegion; }

	CPlayer* GetMaster();
	void SetMaster(CPlayer* pMaster)	{ m_pMaster = pMaster; }

	void SetActive(bool b)		{ m_bActive = b; }
	bool IsActive()				{ return m_bActive; }
	long GetPetType()			{ return m_lPetType; }

	virtual bool DecodePetFromDataBlock(DBReadSet& setReadDB, bool bExData = true,CPlayer* pMaster = NULL,CClientRegion* pRegion = NULL);	//��������
	void DecordChangePropertyFromDataBlock(DBReadSet& setReadDB);		//���³�������

	virtual void UpdateProperty(CMessage *pMsg);
	void SetPetAttibute(WORD wAttrType,long lData);
	//	���볡��
	void EnterRegion(bool bSelf = false );

	CMoveShape* GetCurAtkShape()	{ return m_pCurAtkShape; }
	CMoveShape* GetPassiveAtkShape(){ return m_pPassiveAtkShape; }
	const CGUID& GetAtkGuid()		{ return m_guidCurAtk; }

	void SetCurAtkShape(CMoveShape* pShape)  { m_pCurAtkShape = pShape; }
	void SetCurAtkShape(const CGUID& guid)	{ m_guidCurAtk = guid; }
	void SetPassiveAtkShape(CMoveShape* pShape) { m_pPassiveAtkShape = pShape; }


	//	--	�ƶ�
	//	�Զ�Ѱ·��Ŀ�꣨A*��
	long AutoMoveAI(long lPosX,long lPosY);
	//	�ƶ���ָ������
	void MoveToCell(float fAimX,float fAimY);
	//	�����ƶ�
	long ContinueMoveTo(long lAimX,long lAimY);
	void UpdateAimPos( CMoveShape* pAimShape );
	//	����Ŀ��
	long FollowObjectAI( CShape* pAimObject );
	long FollowObjectAI( long lAimX,long lAimY );

	virtual void AI();
	//	ս��AI
	long SkillAI();
	long PetFightAI();
	void AutoRetaliateAI();

	void UsePetSkill(long);

	void AddSkill(tagPetSkill& skillinfo);
};