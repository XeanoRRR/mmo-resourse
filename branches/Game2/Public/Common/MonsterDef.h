//����͹�����ص�ö�ٺͽṹ

#ifndef MONSTER_DEF_H
#define MONSTER_DEF_H

#include "BaseDef.h"
#include "StateDef.h"

#define MONSTER_DIS_FLOAT
#ifdef MONSTER_DIS_FLOAT
#define DisType float
#define DisTypeID 3 /* for InitNameValueMap */
#define MemP pfValue
#else
#define DisType short
#define DisTypeID 5
#define MemP psValue
#endif


typedef struct tagMonsterProperty
{   
    eEleType	eEType;					//Ԫ�����
	eNpcType	eNType;					//Npc��� (NNPC,Monster,Guard)
	eClass		eClass_;				//����

    uchar		byBlockType;			//�赲����
    uchar		byCountry;				//����ID
	uchar		byCatchable;			//�Ƿ���Բ���
    uchar		byIsFlyToPeace;			//ת��Ϊ��ƽ״̬ʱ���Ƿ�ɻ�ȥ
	uchar		byIsResumeAll;			//ת��Ϊ��ƽ״̬��ʱ���Ƿ���ȫ�ָ�

	ushort		wTitleColor;			//��ν��ɫ
	ushort		wTitlePicID;			//��νͼ��id
    ushort		wLevel;				    //�ȼ�
    ushort		wCountryContriValue;	//���ҹ���ֵ
	ushort		wSPContriValue;			//SP����ֵ
    ushort      wSkillProportion;       //���ܸ���,һ��С�ڵ���100�����ڵ���0�����͡�
    DisType		ReturnRange;			//��λ����
	DisType		PeaceMoveRange;		//��ƽ�ƶ���Χ
    ushort		wMinStopTimes;			//��Сͣ������
	ushort		wMaxStopTimes;			//���ͣ������
	ushort		wMinRunTimes;			//�������߲���
	ushort		wMaxRunTimes;			//������߲���
	ushort		wCanTalk;				//�Ƿ��ܶԻ�
    ushort		wRace;					//��Ⱥ(AIʹ��)
	ushort		wMoveRandomValue;		//�����ƶ��ĸ���(����)
	ushort		wStopTime;				//ͣ����ʱ��(��һ�������м�ͣ��֡��)
	ushort		wAIHangupType;			//AI����ʽ
	ushort		wFigure;				//����(0:1x1 1:3*3 2:5*5)*
	ushort		wBossLevel;				//boss�ȼ�

	float       fHurtProportion;        //���˱���         
    float       fEscapePoint;           //���ܵ��ߣ����ܵ㣩
    float		fMoveSpeed;				//�ƶ��ٶ�
	float		fFightMoveSpeed;		//ս���ƶ��ٶ�
	float       fTurningSpeed;			//ת���ٶ�

    long		lRefeashIndex;			//��������

	ulong       dwGuardTime;            //����ʱ��
	ulong       dwSkillSpace;           //���ܼ��
	ulong		dwChaseTime;			//׷��ʱ�䣬��׷����Χ���û�ܵ�����ת��Ϊ��ƽ��ʱ��
	ulong       dwAIReferrence[MONSTER_AI_REFERRENCE_NUM];      //AI�ο�
    ulong		dwHp;					//��ǰ����ֵ
	ulong       dwAIType;               //AI����
    ulong		dwIndex;				//�����������
	ulong		dwPicID;				//ͼ��ID
	ulong		dwPicLevel;				//ͼ�εȼ�
	ulong		dwNameColor;			//������ɫ���,��Ӧ���ͻ�������
	ulong		dwHpBarColor;			//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
	ulong		dwSoundID;				//����ID
	ulong		dwExp;					//����ֵ*
	ulong		dwDExp;					//��Ϊ����ֵ
	ulong		dwOccuPoint;			//ְҵ����
	ulong		dwMaxCatchFastness;		//������󲶻���
	ulong		dwServantID;			//�����Ӧ�̴�ID

}tagMPro;


// �����ս������
typedef struct tagMonsterFightProperty
{	
	short		wHpRecoverSpeed;		//�����ָ��ٶ�
	short		wMinAtk;				//��С������		    
	short		wMaxAtk;				//��󹥻���
	short		wMinMAtk;				//��С����������
	short		wMaxMAtk;				//�����������
	short		wHit;					//��������
	short		wDef;					//������
	short		wMDef;					//ħ��������
	short		wDodge;					//��������
	short		wBlock;					//��
	short		wParry;					//�м�
	short		wImmunity;				//����
	short		wPierce;				//��͸

	DisType		FarAtkDis;				//��Զ��������*
	DisType		NeaAtkDis;				//�����������*
	DisType		GuardRange;			//���䷶Χ*
	DisType		ChaseRange;			//׷����Χ*

	short		wWillDef;				//��־����
	short		wConstiDef;				//���ʿ���
	short		wCri;					//��������
	short		wCriDamg;				//�������˺�	
	short		wCriDef;				//����������
	short		wMCri;					//ħ��������
	short		wMCriDamg;				//ħ�������˺�
	short		wMCriDef;				//ħ����������

	short		wDeBuffDef[DET_Max];	//Debuff����
	short		wElemDef[ET_Max];		//����Ԫ�ؿ���
	short		wClassDam[C_Max];		//�Ը��������˺�
	short		wElemDam[ET_Max];		//�Ը���Ԫ���˺�

    long		dwMaxHp;				//��������

}tagMFPro;
#endif