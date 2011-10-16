//����͹�����ص�ö�ٺͽṹ

#ifndef MONSTER_DEF_H
#define MONSTER_DEF_H

#include "BaseDef.h"
#include "StateDef.h"


typedef struct tagMonsterProperty
{
	WORD		wTitleColor;			//��ν��ɫ
	WORD		wTitlePicID;			//��νͼ��id
	DWORD		dwIndex;				//�����������
	DWORD		dwPicID;				//ͼ��ID
	DWORD		dwPicLevel;				//ͼ�εȼ�
	BYTE		byBlockType;			//�赲����
	DWORD		dwNameColor;			//������ɫ���,��Ӧ���ͻ�������
	DWORD		dwHpBarColor;			//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
	DWORD		dwSoundID;				//����ID
	WORD		wLevel;				    //�ȼ�
	DWORD		dwExp;					//����ֵ*
	DWORD		dwOccuPoint;			//ְҵ����
	BYTE		byCountry;				//����ID
	BYTE		byTamable;				//�Ƿ����ѱ��
	DWORD		dwMaxTameCount;			//�����ѱ������
	WORD		wCountryContriValue;	//���ҹ���ֵ
	WORD		wSPContriValue;			//SP����ֵ
	eEleType	eEType;					//Ԫ�����
	eNpcType	eNType;					//Npc��� (NNPC,Monster,Guard)
	WORD		wBossLevel;				//boss�ȼ�
	eClass		eClass_;				//����
	WORD		wRace;					//��Ⱥ(AIʹ��)
	WORD		wMoveRandomValue;		//�����ƶ��ĸ���(����)
	WORD		wStopTime;				//ͣ����ʱ��(��һ�������м�ͣ��֡��)
	WORD		wAIHangupType;			//AI����ʽ
	WORD		wFigure;				//����(0:1x1 1:3*3 2:5*5)*
	DWORD		dwHp;					//��ǰ����ֵ
	DWORD       dwAIType;               //AI����

	float		fMoveSpeed;				//�ƶ��ٶ�
	float		fFightMoveSpeed;		//ս���ƶ��ٶ�
	float       fTurningSpeed;			//ת���ٶ�
	WORD		wMinStopTimes;			//��Сͣ������
	WORD		wMaxStopTimes;			//���ͣ������
	WORD		wMinRunTimes;			//�������߲���
	WORD		wMaxRunTimes;			//������߲���
	WORD		wCanTalk;				//�Ƿ��ܶԻ�
	LONG		lRefeashIndex;			//��������
	//
	float       fHurtProportion;        //���˱���
	DWORD       dwGuardTime;            //����ʱ��
	float       fEscapePoint;           //���ܵ��ߣ����ܵ㣩
    WORD        wSkillProportion;       //���ܸ���,һ��С�ڵ���100�����ڵ���0�����͡�
	DWORD       dwSkillSpace;           //���ܼ��
	BYTE		byIsFlyToPeace;			//ת��Ϊ��ƽ״̬ʱ���Ƿ�ɻ�ȥ
	BYTE		byIsResumeAll;			//ת��Ϊ��ƽ״̬��ʱ���Ƿ���ȫ�ָ�
	WORD		wReturnRange;			//��λ����
	WORD		wPeaceMoveRange;		//��ƽ�ƶ���Χ
	DWORD		dwChaseTime;			//׷��ʱ�䣬��׷����Χ���û�ܵ�����ת��Ϊ��ƽ��ʱ��
	DWORD       dwAIReferrence[MONSTER_AI_REFERRENCE_NUM];      //AI�ο�
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
	short		wDodge;					//��������
	short		wBlock;					//��
	short		wParry;					//�м�	
	short		wPierce;				//��͸
	long		lImmunity;				//����
	long		lDef;					//������
	long		lMDef;					//ħ��������
	long		dwMaxHp;				//��������

	short		wFarAtkDis;				//��Զ��������*
	short		wNeaAtkDis;				//�����������*
	short		wGuardRange;			//���䷶Χ*
	short		wChaseRange;			//׷����Χ*

	long		lWillDef;				//��־����
	long		lConstiDef;				//���ʿ���	
	long		lCriDef;				//����������	
	long		lMCriDef;				//ħ����������
	short		wCri;					//��������
	short		wCriDamg;				//�������˺�	
	short		wMCri;					//ħ��������
	short		wMCriDamg;				//ħ�������˺�

	long		lDeBuffDef[DET_Max];	//Debuff����
	long		lElemDef[ET_Max];		//����Ԫ�ؿ���
	short		wClassDam[C_Max];		//�Ը��������˺�
	short		wElemDam[ET_Max];		//�Ը���Ԫ���˺�
}tagMFPro;
#endif