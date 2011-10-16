#pragma once
#include "SkillAI.h"

class CRushAttackAI : public CSkillAI
{
public:
	CRushAttackAI();
	CRushAttackAI(CSkillXml* pSkill);
	virtual ~CRushAttackAI();

	virtual void AI();
	virtual bool Display();
	virtual bool StepBeginAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepRunAI(vector<long> vecType, vector<CGUID> vecID);	// ִ�о��幦��(����false��ʾɾ��)
	virtual bool StepEndAI();	// ִ�н�������(����false��ʾɾ��)

protected:
	struct tagTrail
	{
		DWORD dwStartTime;
		DWORD dwLifeTime;
		DWORD dwCurColor;
		DWORD dwDefaultColor;
		float x, y, fHeight;

		tagTrail *pNext;
	};
	tagTrail *m_pTrail;

	CSkillXml *m_pSkill;
	float m_fStartX;
	float m_fStartY;
	float m_fEndX;
	float m_fEndY;

	DWORD m_dwFlyingTime;	// ����ʱ��
	DWORD m_dwStartTime;	// ��ʼʱ��
};