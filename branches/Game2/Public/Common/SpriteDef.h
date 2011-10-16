/*
*	file:		SpriteDef.h
*	Brief:		С����Ķ����ļ�
*	detail:		
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/

#pragma once

//! �ٷ�֮��
const long A_Hundred_Percent	= 100;
//! �Զ������Ķ�ʱ��ʱ����
const long Auto_Supply_Space	= 500;
//! �Զ��������ֶ���󳤶�
const long Max_DB_Item_Size		= 2048;

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

//! �����Զ��������ö���
struct tagAotoSupplyNode
{
	//!			״̬
	bool		bActive;
	//!			�����ٷֱȣ��ٷ������ӣ�
	long		lSpringScale;
	//!			������Ʒ����
	long		lGoodsIndex;

	tagAotoSupplyNode(void){memset(this, 0, sizeof(tagAotoSupplyNode));}
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