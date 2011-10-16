#ifndef DB_ENTITY_DEF_H
#define DB_ENTITY_DEF_H
#include "../public/dbdefine.h"

// ����һ������ʾ��ȡȫ��������Entity
#define MAX_ENTITY_ID_NUM 99999999

// �������ȫ��������ʵ���־
const long ALL_ENTITIES_FLAG = -99999999;

// [070622 AHC]
//������������:	�����Ե�C++��������
enum DB_ATTR_DATA_TYPE
{
	DB_ATTR_DATA_BYTE = 0,	// 1 BYTE
	DB_ATTR_DATA_WORD,		// 2 BYTE
	DB_ATTR_DATA_DWORD,		// 4 BYTE
	DB_ATTR_DATA_FLOAT,		// 4 BYTE
	DB_ATTR_DATA_LONG,		// 4 BYTE
	DB_ATTR_DATA_SHORT,		// 2 BYTE
	DB_ATTR_DATA_CHAR,		// 1 BYTE
	DB_ATTR_DATA_BOOL,		// 1 BYTE
	DB_ATTR_DATA_BUF,		// N BYTE(���ض�ֵBYTE)

	DB_ATTR_DATA_BYTEARRAY, // 1 BYTE * M����
	DB_ATTR_DATA_WORDARRAY,	// 2 BYTE * M����
	DB_ATTR_DATA_DWORDARRAY,// 4 BYTE * M����
	DB_ATTR_DATA_FLOATARRAY,// 4 BYTE * M����
	DB_ATTR_DATA_LONGARRAY,	// 4 BYTE * M����
	DB_ATTR_DATA_SHORTARRAY,// 2 BYTE * M����
	DB_ATTR_DATA_CHARARRAY,	// 1 BYTE * M����
	DB_ATTR_DATA_BOOLARRAY,	// 1 BYTE * M����
	DB_ATTR_DATA_BUFARRAY	// N BYTE(���ض�ֵBYTE) * M����
};


enum PLAYER_EXTEND_ID
{
	PEI_PACKET					= 1,	//��ͨ����
	PEI_EQUIPMENT,						//װ����
	//PEI_HAND,							//�������
	PEI_WALLET,							//��Ǯ
	PEI_SILVERWALLET,					//����Ǯ��
	PEI_GEM,							//! ��ʯ
	PEI_YUANBAO,						//Ԫ��
	PEI_JIFEN,							//����
	PEI_EQUIPMENT_UPGRADING,			//װ������
	//PEI_BANK,							//����
	//PEI_DEPOT,							//�ֿ�
	PEI_ENHANCEMENT,					//
	PEI_PACK =10000,						//�ӱ�������
	PEI_PACK1=10001,					//�ӱ�����λ1
	PEI_PACK2=10002,					//�ӱ�����λ2
	PEI_PACK3=10003,					//�ӱ�����λ3
	PEI_PACK4=10004,					//�ӱ�����λ4
	PEI_PACK5=10005						//�ӱ�����λ5
};

#endif//DB_ENTITY_DEF_H