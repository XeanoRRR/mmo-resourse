/*
*	file:		SpriteDef.h
*	Brief:		С����Ķ����ļ�
*	detail:		
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/

#pragma once

//! �ٷ�֮��
const LONG A_Hundred_Percent	= 100;
//! �Զ������Ķ�ʱ��ʱ����
const LONG Auto_Supply_Space	= 500;
//! �Զ��������ֶ���󳤶�
const LONG Max_DB_Item_Size		= 2048;
//! �ȶ�����ͨ�һ����ж�ʱ��ʱ���� 3s,��λ��ms
const LONG Auto_Common_Space     = 3000;
//! �ȶ��һ�ʱ��Ķ�ʱ��ʱ���� 1s,��λ��ms
const LONG Auto_StableTime_Space = 1000;  
//! �ȶ��һ�ʱ������ȶ�״̬������ʱ��10����,��λ��s
const LONG Auto_EnterStableTime  = 600; 
//! �ȶ��һ�ʱ�����ֵ30�죬��λ��s
const DWORD MAX_STABLETIME       = 2592000;    
//! �Զ��������������
const DWORD MAX_REPAIRMONMEY     = 4000000;
//! �ȶ��һ��н��뱣��״̬�������Զ�ԭ�ظ����ʱ�� ,��λ��s
const DWORD Auto_DieReliveTime   = 60;
//! ����װ���;�Ϊ0��سǵ�ʱ��3���ӣ���λ��s
const DWORD Auto_ReturnCity      = 180;

//! �Զ���������
//! ����ö��ֵ����Ϊ�����������޸�ʱС��
enum eAotoSupplyType
{
	//! ��һ�ֲ���HP
	eAST_HP1,
	//! �ڶ��в���HP
	eAST_HP2,
	//! ����MP
	eAST_MP,
	//! ����С����MP
	eAST_SpriteMP,
	//! ������������
	eAST_ArmPower,

	//! ��������������������߽��ã�
	AotoSupplyTypeNum,
};


//! �����һ�����
enum eAutoHookType
{
	//! �Զ���������Ϊ �ȶ����� ����
	eAHT_STABLE,
	//! �Զ���������Ϊ ��ͨ���� ����
	eAHT_COMMON,
	//! �������� ����
	eAHT_FIGHTSKILL,
	//! �������� ����
	eAHT_ASSISTSKILL,
	//! �������� ����
	eAHT_ASSISTGOODS,
	//! �Զ����� ����
	eAHT_REPAIR,

	//! ��������������������߽��ã�
	AutoHookTypeNum,

};


//! �����Զ��������ö���
struct tagAotoSupplyNode
{
	//!			״̬
	BOOL		bActive;
	//!			�����ٷֱȣ��ٷ������ӣ�
	LONG		lSpringScale;
	//!			������Ʒ����
	LONG		lGoodsIndex;

	tagAotoSupplyNode(void){memset(this, 0, sizeof(tagAotoSupplyNode));}
};
struct tagAutoHookBuff
{
	string	strSkillName;					// ��������(ֻ��Ϊ�˷���߻����ã�Ҫ��Ҫ��������)
	string	strOrginName;			        // ��Ʒԭʼ��
	DWORD	dwAutoHookIndex;   				// ���ܻ�����Ʒ ��Ӧ��ID��	
	DWORD	dwBuffIndex;					// buff ��Ӧ��ID��
	DWORD	Level;				        	// �ȼ���ʶ��(��������)

};

//!�����Զ��������
struct tagAutoFightNode 
{
	//!      ״̬
	int     bOpen;
	//!      ����
	LONG     lFightTypeIndex;

	tagAutoFightNode(void) {memset(this, 0, sizeof(tagAutoFightNode));}
};

enum eAutoSetupErrorType
{
	//! �ɹ�
	eASET_NotErr,
	//! �������ͳ�����Χ
	eASET_TypeWithout,
	//! ���õ�ֵ���Ϸ�
	eASET_ValueErr,
	//! û�������ı�
	eASET_NotChange,
	//! ��ͼ��֧��
	eASET_NotMap,
	//! ���ܻ���߿��Է���
	eASET_CanUse,
	//! ���ܻ���߲���ʹ��
	eASET_NoCan,
	//! �ȶ�ʱ�䲻��
	eASET_NoStableTime,
	//! ���Ҳ���
	eASET_NoMoney,
	//! δ֪����
	eASET_UnknowErr,
};


//! ��Ʒ�����÷�Χ
enum eGoodsApplyBound
{
	eGAB_HP		= 0x1,
	eGAB_MP		= 0x2,
	eGAB_SMP	= 0x4,
	eGAB_POWER	= 0x8,
};