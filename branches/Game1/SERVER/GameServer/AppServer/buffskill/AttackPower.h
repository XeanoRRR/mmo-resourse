#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_ATTACKPOWER_INCLUDED
#define _INC_ATTACKPOWER_INCLUDED
#include "..\..\..\..\Public\SkillRelated.h"

enum AttackType
{
	ATTACK_TYPE_UNKNOW = 0,		//##δ֪���͹�����������
	ATTACK_TYPE_PHYSICAL,		//##������
	ATTACK_TYPE_MAGERY,			//##ħ������
	ATTACK_TYPE_ELEMENT,		//##Ԫ�ع���
	ATTACK_TYPE_SOUL,			//##��������
	ATTACK_TYPE_POISON,			//##������
};

enum eDamageResult
{
	DAMAGE_RESULT_Damage,//����
	DAMAGE_RESULT_Dodge,//����
	DAMAGE_RESULT_FightBlock,//��
	DAMAGE_RESULT_Parry,//�м�
	DAMAGE_RESULT_Resist,//�ֿ�
	DAMAGE_RESULT_Suck,//����
	DAMAGE_RESULT_Rebound,//����	
	DAMAGE_RESULT_Lose,//��ʧ
	DAMAGE_RESULT_IncHp,//��Ѫ
	DAMAGE_RESULT_DamageEx,//��������
	DAMAGE_RESULT_DotaDamage,//DOTA�˺�
};

//##����ĳ�ֹ����ԶԷ���ɵ�ĳ���˺�
struct tagAttackPower : public BaseMemObj
{
	AttackType	atAttackType;					//##��������
	LONG		lHPDamage;						//##��HP���˺�
	LONG		lMPDamage;						//##��MP���˺�
	LONG		lEnergyDamage;					//##���������˺�
	DWORD		dwOtherEffect;					//##�����ĸ���Ч��������Σ����˵ȵȡ�
	DWORD		dwEffectPresentDegree;			//##���ֵĳ̶�
	DWORD		dwEffectPresistTime;			//##������ʱ��

	tagAttackPower				();
};

//##�����ߵ���ϸ�������Լ��������˺�ǿ�ȡ�
struct tagAttackInformation
{
	tagSkillID				tSkillID;				//##�������ܱ��
	BYTE					bySkillLevel;			//##�������ܵȼ�
	LONG					lAttackerType;			//##����������
	CGUID					AttackerID;				//##����������
	bool					bCritical;				//##�����Ƿ���ȫ��һ��
	bool					bBlastAttack;			//##�����Ƿ��Ǳ���
	LONG					lTargetDodgeModifier;	//##Ŀ��Ķ������
	LONG					lHitModifier;			//##������������ֵ
	FLOAT					fDamageFactor;			//##�����˺�ϵ��[0.0-n]
	LONG					lDamageModifier;		//##�����˺�����ֵ
	LONG					bFullMiss;				//##�Ƿ���ȫ����(1), ��(2);

	//##�������ڹ���˲���PVP����״̬��ֻ��CPlayer��Ч
	bool					bAllowKillPlayer;		//##�ͷ����빥����������
	bool					bAllowKillTeammate;		//##�ͷ����빥������
	bool					bAllowKillGuildMember;	//##�ͷ����빥�������Ա
	bool					bAllowKillCriminal;		//##�ͷ����빥���ﷸ
	bool					bAllowKillCountry;		//##�ͷ������������
	CGUID					AttackerTeamID;		//##�ͷ��ߵ�Team���
	CGUID					AttackerGuildID;		//##�ͷ��ߵİ����
	CGUID					AttackerUnionID;		//##�ͷ��ߵ�ͬ�˱��

	vector<tagAttackPower*> vDamage;				//##�˺��б�

	VOID					Clear();

	tagAttackInformation();
	~tagAttackInformation();
};


//##����ĳ����Ϊ�Գ��ܷ���ɵ�ĳ��Ӱ�졣

#endif