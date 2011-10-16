#pragma once

#pragma once

#include <windows.h>

class CPlayerTracker
{
	BOOL  m_bShocked;
	float m_fShockMaxRatio;
	float m_fShcokrOffset;

	DWORD m_dwShockStartTime;
	DWORD m_dwShockDelayTime;
	DWORD m_dwShockTimeLength;

	//ˮƽ�ӽ�
	int m_iHoriAngle;

	//��ֱ�ӽ�
	int   m_iMinVertAngle;
	int   m_iMaxVertAngle;
	int   m_iVertAngle;

	//�۲����
	float m_fMinDistance;
	float m_fMaxDistance;
	float m_fDistance;

	//
	int m_nScorll;	// -1:����ת 0:���� 1:����ת

	bool	m_bKeyboardAble;		// ������Ч
	bool	m_bMouseWheelAble;		// ��������Ч
	BOOL	m_bHRotataion;

	static float s_fDistance;		// ��¼��ҵ��ӽǾ���

	// ����Ҽ�����,��¼������
	POINT	m_ptOldMouse;
	// ��¼ԭ����
	int m_iOldHoriAngle;
	// ����Ķ���
	int m_iSourceHoriAngle;
	//�Ƿ�ƽ��
	BOOL m_bsmooth;
	BOOL m_brepos;

public:
	CPlayerTracker(void);
	~CPlayerTracker(void);

	void Update(void);
	void Run(void);

	void SetShock(float fMaxRatio,DWORD dwDelayTime,DWORD dwTimeLength);

	BOOL IsShock() const { return m_bShocked;}
	//ADD by ��ǫ
	BOOL IsSmooth()const {return m_bsmooth;}
	void SetSmooth(BOOL bsmooth){m_bsmooth = bsmooth;}
	//BOOL Isrepos()const {return m_brepos;}
	//void Setrepos(BOOL brepos){m_brepos = brepos;}
	//ADD by ��ǫ
	float GetSockOffset() const { return m_fShcokrOffset;}

	void GetPosition(D3DXVECTOR3 &vPosition);

	float GetHoriAngle() { return (float)m_iHoriAngle*(3.1415926f/180.0f); }

	void SetKeyboardAble(bool b)	{m_bKeyboardAble=b;}
	void SetMouseWheelAble(bool b)	{m_bMouseWheelAble=b;}
	void SetHRotation(BOOL bValue)  {m_bHRotataion = bValue;}

	float GetCameraPosModify()	{return (m_fDistance-m_fMinDistance)/(m_fMaxDistance-m_fMinDistance);}
};

