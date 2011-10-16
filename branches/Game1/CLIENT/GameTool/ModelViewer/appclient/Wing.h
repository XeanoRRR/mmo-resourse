#pragma once
#include "specialequip.h"

// ����
enum eACTION
{
	ACT_NO = -1,			// û���κζ���
	ACT_STAND,				// վ(������ֹ)
	ACT_IDLE,				// վ��idle
	ACT_WALK,				// ��
	ACT_RUN,				// ��
	ACT_ATTACK,				// ����
	ACT_BEATTACK,			// ������
	ACT_DIED,				// ����(��������)
	ACT_INTONATE,			// ����
	ACT_BLOCK,				// ��
	ACT_PARRY,				// �м�
	ACT_MISS,				// ����

	ACT_OPEN,				// ���Ŵ�

	// ѡ�˽��涯��
	ACT_SEL_X0,				// ����վ��
	ACT_SEL_X1,				// �ν�
	ACT_SEL_X2,				// �ӽ�x
	ACT_SEL_X3,				// �ֽ�վ��x
	ACT_SEL_X4,				// �ս�x
	ACT_SEL_X5,				// �ָ�վ��

	ACT_PERFORM = 50,		// ʩ��
	ACT_MAX_PERFORM = 70,	// ���ʩ������

	ACT_MAX = 100			// �����ٸ�����
};



class CWing : public CSpecialEquip
{
public:
	enum WING_STATUS 
	{
		WING_IDLE  = ACT_DIED,
		WING_STAND = ACT_STAND,
		WING_WALK  = ACT_WALK,
		WING_RUN   = ACT_RUN,
		//WING_HURT = CShape::ACT_BEATTACK,
	};

	CWing();
	~CWing();
	void ChangeStatus(DWORD eStaus);
};