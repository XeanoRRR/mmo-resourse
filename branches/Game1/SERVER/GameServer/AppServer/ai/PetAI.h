#pragma once
#include "BaseAI.h"

class CPetAI : public CBaseAI
{
public:
	CPetAI();
	~CPetAI();

	virtual BOOL MoveTo(float fDestX, float fDestY);	// ��һ��ָ�������ƶ�
	virtual DWORD BornSpring(void);						// ������״̬�ȴ���
	virtual BOOL OnActive(void);						// �״̬AI����
	virtual BOOL OnDeadState(void);
	virtual BOOL OnReturnPeace(void);					// �ָ�����ƽ״̬
	virtual BOOL OnSkillEnd(long lAddSub);				// ���ܽ�������
	virtual void SetTarget(long lType,
		const CGUID& TargetGuid);						// ����Ŀ��
	virtual void Attack(long lSkillID,
		long lX = 0,
		long lY = 0,
		CMoveShape* pTarget = NULL);
	virtual BOOL WhenBeenHurted(LONG lType,
		const CGUID& guid,
		DWORD a);										// ����������ʱ��
	virtual BOOL WhenBeenKilled(LONG lType,
		const CGUID& guid);								// ���ﱻɱ��
	void AddRetPeaceAct(void);							// ע�᷵�غ�ƽ�¼�
	void LogoutRetPeaceAct(void);						// ע�����غ�ƽ�¼�
	void SetHost(CMoveShape* pHost){ m_pHost = pHost; }	// ���ó�������

protected:
	bool SearchEnemy(void);								// Ѱ��
	bool MissTarget(void);								// ��ʧĿ��
	virtual	void ChangeStateTo(INT lState);

protected:
	BOOL		m_bFightUseSkill;
	long		m_lCount;
	long		m_lReturnPeaceTime;
	AI_EVENT	m_aeReturnPeace;
	AI_EVENT	m_aeRetPeace;
	CMoveShape*	m_pHost;								// ��������
};