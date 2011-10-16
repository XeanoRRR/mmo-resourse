#pragma once
class CMoveShape;
#include "SkillAI.h"

class CNormalLongDistanceAttackAI : public CSkillAI
{
public:	
	enum FLY_MODE
	{
		FLY_TRACK = 0,		//���ٷ���
		FLY_LOCK_LINE = 1	//������
	};
	CNormalLongDistanceAttackAI();
	CNormalLongDistanceAttackAI(CSkillXml* pSkill);
	CNormalLongDistanceAttackAI(CSkillXml* pSkill, FLY_MODE mode);
	virtual ~CNormalLongDistanceAttackAI();

	virtual void AI();
	virtual bool Display();
	virtual bool StepBeginAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepRunAI(vector<long> vecType, vector<CGUID> vecID);
	virtual bool StepEndAI();

	struct tagObjectInfo
	{
		long lType;
		CGUID guid;
	};

protected:
	CSkillXml *m_pSkill;
	CMoveShape *m_pMoveshape;
	FLY_MODE	m_emFlyMode;
	void Release();

	struct tagEffect
	{
		tagEffect()
		{
			bgone = false;
			barrived = false;
			bloadinregion = false;
			fstartx = 0.0f;
			fstarty = 0.0f;
			fstarth = 0.0f;
			llasttime = 0;
			ldelaytime = 0;
			lstarttime = 0;
			lspeed = 0;
			dwlocator = 0;
			dweffectID = 0;
			dweffectindex = 0;	
			D3DXMatrixIdentity(&matrot);
		}
		tagObjectInfo objinfo;
		float fstartx;
		float fstarty;
		float fstarth;
		long  llasttime;
		long  ldelaytime;
		long  lspeed;
		long  lstarttime;
		bool  bgone;
		bool  barrived;
		bool  bloadinregion;
		DWORD dwlocator;
		DWORD dweffectID;
		DWORD dweffectindex;
		CGUID guidmyguid;
		D3DXMATRIX matrot;
	};
	vector<tagEffect> m_veceffect;
	/*vector<tagObjectInfo> m_vecObjInfo;	// ����Ŀ���б�

	vector<float> m_vecFStartX;			// ÿ�ַ�����Ч��ʼλ��x�����б�
	vector<float> m_vecFStartY;			// ÿ�ַ�����Ч��ʼλ��y�����б�
	vector<float> m_vecFStartHeight;		// ÿ�ַ�����Ч��ʼλ��height�����б�

	vector<float> m_vecFEndX;				// ÿ�ַ�����Ч����λ��x�����б�
	vector<float> m_vecFEndY;				// ÿ�ַ�����Ч����λ��y�����б�
	vector<float> m_vecFEndHeight;			// ÿ�ַ�����Ч����λ��height�����б�

	vector<DWORD> m_vecDwFlyingTime;		// ÿ�ַ�����Ч����ʱ��

	vector<DWORD> m_vecDwStartTime;			// ���п�ʼʱ��
	vector<DWORD> m_vecLastTime;			// ����������ʱ��

	bool m_bIsRun;							// ��ʼ���еı�־*/
};