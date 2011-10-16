#ifndef DB_DEFINE_H
#define DB_DEFINE_H

// ��������
enum COMPOSITE_TYPE
{
	COM_COMPONENT = 0,
	COM_LEAF,
	COM_COMPOSITE,
	//COM_DETAILCOMPOSITE, // ���丸�ڵ��������,�������ļ�������
};

// Ԥ����Rgn�ڴ��
#define MAX_FREE_RGN_NUM 1024
// Ԥ����Proxy Rgn�ڴ��
#define MAX_FREE_PROXY_RGN_NUM 256
// ����һ������ʾ��ȡȫ��������Entity
#define MAX_ENTITY_ID_NUM 99999999

// �������ȫ��������ʵ���־
const long ALL_ENTITIES_FLAG = -99999999;
const long ERROR_FLAG		= -99999999;
const long NULL_FLAG		= -99999999;



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



//���ݿ��ϵ����
enum DB_RELATE_TYPE
{
	DB_RELATE_NULL = 0,
	DB_RELATE_AND,
	DB_RELATE_OR,
	DB_RELATE_GREAT,
	DB_RELATE_EQUAL,
	DB_RELATE_LESS,
	DB_RELATE_GREATANDEQUAL,
	DB_RELATE_LESSANDEQUAL,
	DB_RELATE_LBLANKAND, // ������+AND : ) AND
	DB_RELATE_RBLANKAND, // ������+AND : AND (
	DB_RELATE_LBLANK, // ������ : (
	DB_RELATE_RBLANK, // ������ : )
	DB_RELATE_RBLANKOR, // ������ : ) OR
	DB_RELATE_LBLANKOR, // ������+OR : OR (
};

enum DATA_OBJECT_TYPE
{
	DATA_OBJECT_UNKNOWN = 0,
	DATA_OBJECT_FLOAT,
	DATA_OBJECT_LONG,
	DATA_OBJECT_SHORT,
	DATA_OBJECT_ULONG,
	DATA_OBJECT_USHORT,
	DATA_OBJECT_BOOL,
	DATA_OBJECT_CHAR,
	DATA_OBJECT_BYTE,
	DATA_OBJECT_WORD,
	DATA_OBJECT_DWORD,
	DATA_OBJECT_STRING,
	DATA_OBJECT_BUFFER,
	DATA_OBJECT_GUID,
	DATA_OBJECT_STRUCT,
	DATA_OBJECT_TIME,
	DATA_OBJECT_BIGINT,
};

struct tagQueryInfo 
{
	tagQueryInfo() : relateType(DB_RELATE_NULL),objType(DATA_OBJECT_UNKNOWN)
	{
		memset(szAttrName, 0, sizeof(szAttrName));
	}
	~tagQueryInfo()
	{
	}

	tagQueryInfo& operator=(const tagQueryInfo& right)
	{
		if(this != &right)
		{
			memcpy(szAttrName, right.szAttrName, sizeof(szAttrName));
			relateType = right.relateType;
			objType = right.objType;
		}
		return *this;
	}
	char szAttrName[32];
	DB_RELATE_TYPE relateType;
	DATA_OBJECT_TYPE objType;
};

//���ݿ��������
enum DB_OPERATION_TYPE
{
	DB_OPER_INIT = 0,
	DB_OPER_INSERT,
	DB_OPER_DELETE,
	DB_OPER_SAVE,
	DB_OPER_LOAD,
	DB_OPER_REGISTER,	// ��DBS��ע���ʵ��Ļ������
	DB_OPER_UNREGISTER, // ��DBS��ע����ʵ��Ļ������
	DB_OPER_DELETE_INSERT,// DBS����ɾ���ٲ�������
	DB_OPER_PROCEDURE,		// ��׼�洢����
	DB_OPER_CREATEROLE_PROCEDURE, // ��������ʱ�Ĵ洢����
	DB_OPER_INSERTMAIL_PROCEDURE, // ����ʼ�ʱ�Ĵ洢����
	DB_OPER_CHANGENAME_PROCEDURE, // ��Ҹ����Ĵ洢����
	DB_OPER_ONLY_MEMSAVE,		// ֻ�浽���󻺴��У�������DB
	DB_OPER_DEL_LEAVES,		// ɾ����Ҷ�ӽڵ�
	DB_OPER_SAVE_LEAVES,		// ������Ҷ�ӽڵ�
	DB_OPER_LOAD_LEAVES,		// ��ȡ��Ҷ�ӽڵ�
	DB_OPER_SAVE_RETURN,       // ���沢����
	DB_OPER_LOAD_NOCACHE,      // ��ȡ������
};

enum PLAYER_EXTEND_ID
{
	PEI_PACKET					= 1,	//��ͨ����
	PEI_EQUIPMENT,						//װ����	
	PEI_WALLET,							//��Ǯ
	PEI_SILVERWALLET,					//����Ǯ��
	PEI_GEM,							//! ��ʯ
	PEI_YUANBAO,						//Ԫ��
	PEI_JIFEN,							//����
	PEI_MEDAL_CONTAINER,
	PEI_PACK =10000,					//�ӱ�������
	PEI_PACK1=10001,					//�ӱ�����λ1
	PEI_PACK2=10002,					//�ӱ�����λ2
	PEI_PACK3=10003,					//�ӱ�����λ3
	PEI_PACK4=10004,					//�ӱ�����λ4
	PEI_PACK5=10005,					//�ӱ�����λ5

	PEI_DCT_Gold = 0xFFFF01,		//! ��Ҳֿ�
	PEI_DCT_Silver,					//! ���Ҳֿ⣨��ǰ�Ǳ�ʯ�ֿ⣬ö��ֵ���䣩
	PEI_DCT_Primary,				//! ���ֿ�
	//PEI_DCT_Primary2,				//! ���ֿ�2
	PEI_DCT_Secondary,				//! ��չ��������
	PEI_DCT_Secondary1,				//! ��չ�ֿ�1
	PEI_DCT_Secondary2,				//! ��չ�ֿ�2
	PEI_DCT_Secondary3,				//! ��չ�ֿ�3
	PEI_DCT_Secondary4,				//! ��չ�ֿ�4

	PEI_BUSINESSPACKFIELD = 20000,   //�������̱�������λID
	PEI_BUSINESSPACK = 20001,		//���̱���ID
};

enum SESSION_TYPE
{
	ST_NORMAL_SESSION,
	ST_WS_SESSION,
};

enum DbOperThreadType
{
	eDbOperPlayerType = 0,
	eDbOperOtherType  = 1,
	eDbOperNormallyType = 2,
	eDbOperMailThread = 3,
};

enum eThreadType
{
	eThreadInit = -1,
	eThreadLoadType = 0,
	eThreadSaveType = 1,
};

#endif//DB_DEFINE_H