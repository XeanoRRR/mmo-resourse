#pragma  once
#include "SkillAI.h"
class CRandomSummonAI : public CSkillAI
{
public:
	CRandomSummonAI();
	CRandomSummonAI(CSkillXml *pSkill);
	virtual ~CRandomSummonAI();

	virtual void AI();
	virtual bool Display();
	virtual bool StepBeginAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepRunAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepEndAI();

	// ȡ�ÿͻ��˵Ĳ���ʱ��
	DWORD	GetRemainedTime();
protected:
	CSkillXml *m_pSkill;
	
	long  m_lHostType;		// Ŀ���������
	long  m_lHostID;		// Ŀ����
	DWORD m_dwLifeTime;		// ����ʱ��
	DWORD m_dwTimeStamp;		// ����ʱ����
	DWORD m_dwFrequency;
	DWORD m_dwTotalTime;		// ������ʱ��
	long  m_lPreSkillTime;		// ÿ���ٻ��ﲥ�ŵļ��ʱ��
	long  m_lDeleTime;			// �ٻ��ﲥ�ŵ��ӳ�ʱ��
	vector<long> m_vecSummonEffect; // �ٻ���Ч�б�
	vector<long> m_vecSummonSound;	// �ٻ���Ч
	// ������ṹ
	struct tagCell
	{
		long lX;
		long lY;
	};

	vector<tagCell>	m_vecShotPoint;		// ������б�
	long			m_lCurShotPoint;	// ��ǰ�ķ����
};