#pragma  once
#include "SkillAI.h"
class CFixSummonAI : public CSkillAI
{
public:
	CFixSummonAI();
	CFixSummonAI(CSkillXml *pSkill);
	virtual ~CFixSummonAI();

	virtual void AI();
	virtual bool Display();
	virtual bool StepBeginAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepRunAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepEndAI();

	DWORD	GetRemainedTime();
protected:
	CSkillXml *m_pSkill;

	long	m_lHostType;		// Ŀ���������
	long	m_lHostID;			// Ŀ����
	DWORD	m_dwLifeTime;		// ����ʱ��
	DWORD	m_dwTimeStamp;		// ����ʱ����

	vector<long> m_vecSummonEffect;		// �ٻ���Ч�б�
	vector<long> m_vecSummonSound;		// �ٻ���Ч
private:
};