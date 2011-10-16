

/*
*	file:		DepotDef.h
*	Brief:		���ڲֿ�ĵ�һЩC/S�˹��õĶ���
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-01-22
*/

#pragma once

//!			�ֿ������������
#define		DEPOT_PASSWORD_SIZE				PASSWORD_SIZE
#define		DEPOT_PASSWORD_ARRAY_LEN		(DEPOT_PASSWORD_SIZE + 1)

//!			���ֿ��ݻ�
#define		PRIMARY_SIZE			42U
//!			���ֿ�2�ݻ�
#define		PRIMARY2_SIZE			20U
//!			�����ֿ⣨��ҡ���ʯ������
#define		AMOUNT_DEPOT_NUM		2U
//!			��չ�ֿ���������ǰ���ӱ�������������ݻ�Ϊ5��������Ķ��峬�������ݻ����Ǳ�ҲӦ�޸ģ�
#define		DEPOT_SUBPACK_NUM		4U

//!		�ֿ�����
struct	tagDepotSetup
{
	//!		�ӱ����۸�
	long	arr_lSubpackPrice[DEPOT_SUBPACK_NUM];
	//!		һ��ʱ����������������������
	long	lMaxPwdErrCount;
	//!		����ʱ�䣬Ҳ���������Ա����������������(����)
	long	lFrostTime;
};

//! �ֿ���������
enum eDepotContainerType
{
	//! ����һ���ֿ��������ͱ�������ϵ������������ֵ��PEI_PACK5=10005���������ʼֵ�������ظ�
	DEPOT_CONTAINER_TYPE_BEGIN_VALUE = 0xFFFF00,

	//! ��Ҳֿ�
	eDCT_Gold,
	//! ���Ҳֿ�
	eDCT_Silver,

	//! ���ֿ�
	eDCT_Primary,
	//! ���ֿ�2
	//eDCT_Primary2,

	//! ��չ��������
	eDCT_Secondary,
	//! ��չ�ֿ�1
	eDCT_Secondary1,
	//! ��չ�ֿ�2
	eDCT_Secondary2,
	//! ��չ�ֿ�3
	eDCT_Secondary3,
	//! ��չ�ֿ�4
	eDCT_Secondary4,
	//! ��չ�ֿ�5
	//eDCT_Secondary5,


	//! ����ֵ�����ڷ�Χ�жϣ�
	DEPOT_CONTAINER_TYPE_END_VALUE,
};

#define IS_DEPOT_VOLCONTAINER(type) ((type) < DEPOT_CONTAINER_TYPE_END_VALUE && (type) > DEPOT_CONTAINER_TYPE_BEGIN_VALUE && (type) != eDCT_Gold && (type) != eDCT_Silver)
#define IS_DEPOT_CONTAINER(type) ((type) < DEPOT_CONTAINER_TYPE_END_VALUE && (type) > DEPOT_CONTAINER_TYPE_BEGIN_VALUE)


//! ��Ҳֿ����ݵ�����ͷ
struct tagPlayerDepotDataHead
{
	//! �Ƿ�������
	long lHasPassword;
	//! �ⶳʱ��
	long lThawDate;

	//! ���õģ��ѹ���ģ��Ӳ�λ
	long lUsableSubbackNum;

	//! ��λ�۸��
	long arr_lSubbackPrice[DEPOT_SUBPACK_NUM];


	tagPlayerDepotDataHead(void){memset(this, 0, sizeof(tagPlayerDepotDataHead));}

};

//! �ֿ����Ʒ������Ϣ��
struct tagDepotGoodsMoveMsg
{
	//! Դ��������
	ULONG lSrcContainerType;
	//! Դ����λ��
	ULONG lSrcPos;
	//! Դ��������
	ULONG lDimContainerType;
	//! Դ����λ��
	ULONG lDimPos;
	//! ��������
	ULONG lOptAmount;

	tagDepotGoodsMoveMsg(void){memset(this, 0xff, sizeof(tagDepotGoodsMoveMsg));}
};

//! ���������Ϣ�����Чֵ
const ULONG MOVE_MSG_USELESS_VALUE = 0xFFFFFFFFU;

//! ��Ϣ����
enum eDepotMsgCode
{
	eDMC_Failed			= 0,		//! ʧ��
	eDMC_Succeed		= 1,		//! �ɹ�
};