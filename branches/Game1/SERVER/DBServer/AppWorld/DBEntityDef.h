#ifndef DB_ENTITY_DEF_H
#define DB_ENTITY_DEF_H
#include "../public/BaseDef.h"
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


// ���̶߳�ȡ����������Ϣ
struct LoadObj
{
	CGUID				objGUID;
	long				objID;
	long				msgID;
	long				objNum;
	DB_OBJ_TYPE			objType;
	char				szCDKEY[64];
	DB_OPERATION_TYPE	opType;
	DWORD				dwIP;
	long				lQueryTime;

	LoadObj();
	virtual ~LoadObj();
	LoadObj& operator=(const LoadObj& right);
};

// ���߳�д�봦��������Ϣ
struct SaveObj
{
	CGUID		objGUID;
	long		objID;
	long		msgID;
	long		objNum;// �������Ϊ�������˶��پͱ�ʾɾ�����ٸ�����
	DB_OBJ_TYPE objType;
	std::vector<long> objAttrEnum; // Ҫ���������ö��ֵ
	DB_OPERATION_TYPE	opType;
	DB_OPERATION_RESULT_TYPE	opResultType;
	DWORD		dwIP; // �Ʒ���
	SaveObj();
	virtual ~SaveObj();
	SaveObj& operator=(const SaveObj& right);

};

#endif//DB_ENTITY_DEF_H