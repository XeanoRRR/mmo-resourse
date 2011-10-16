#pragma once
#ifndef _FKMS_
#define _FKMS_

#include "tinyxml.h"

/// ���ֿ������ȼ��Ĺ���ö������
typedef enum _FKMS
{
	FKMS_UNKNOW = 0,

	/// �ƶ�����ö��ֵ
	FKMS_MOVEKEY = 100,

	FKMS_MOVEKEY_RUNORWALK,	// �߻���
	FKMS_MOVEKEY_FORWARD = 122,		// ��ǰ
	FKMS_MOVEKEY_GOBACK,		// ���
	FKMS_MOVEKEY_GOLEFT,		// ����
	FKMS_MOVEKEY_GORIGHT,     // ����

	FKMS_MOVEKEY_UP,			// ����ͷ����ǰ��
	FKMS_MOVEKEY_DOWN,			// ����ͷ�����
	FKMS_MOVEKEY_LEFT,			// ����ͷ������
	FKMS_MOVEKEY_RIGHT,		// ����ͷ�����ң�

	/// ���湦��ö��ֵ
	FKMS_INTERFACEKEY = 200,

	FKMS_INTERFACEKEY_ESC,			// ȡ���򿪽�����߿���ϵͳ�˵�����
	FKMS_INTERFACEKEY_HIDEINTERFACE,	// ���ز�������
	FKMS_INTERFACEKEY_TALKFACE,		// ����˽�Ľ���
	FKMS_INTERFACEKEY_QUST = 224,		// �����������
	FKMS_INTERFACEKEY_SHOWGOODSNAME,	// ��ʾ���������Ʒ������ʾ
	FKMS_INTERFACEKEY_FASTREPLY,		// ���ٻظ�
	FKMS_INTERFACEKEY_AUTOSUPPLY,		// ����ҩƷ�Զ���������
	FKMS_INTERFACEKEY_SKILL,			// �������ܽ���
	FKMS_INTERFACEKEY_PRESTIGE,		// ����������������
	FKMS_INTERFACEKEY_PACKAGE,		// ��������
	FKMS_INTERFACEKEY_FRIEND,			// �������ѽ���
	FKMS_INTERFACEKEY_PRODUCE,		// ����רҵ��������
	FKMS_INTERFACEKEY_UNDERWORLD,		// ����������
	FKMS_INTERFACEKEY_PK,				// ����PK���ý���
	FKMS_INTERFACEKEY_HELP,			// ������������
	FKMS_INTERFACEKEY_INCREASESHOP,	// ������ֳ�̵�
	FKMS_INTERFACEKEY_NPCLIST,		// ����NPC�б�
	FKMS_INTERFACEKEY_PLAYERPROPERTY,// �����������Խ���
	FKMS_INTERFACEKEY_SERCHFRIEND,	// ����������������
	FKMS_INTERFACEKEY_PACKAGE2,		// ������������
	FKMS_INTERFACEKEY_BIGMAP,			// �������ͼ

	/// ��ݼ�ö��ֵ
	FKMS_HOTKEY = 300,

	FKMS_HOTKEY_301,	
	FKMS_HOTKEY_302,
	FKMS_HOTKEY_303,
	FKMS_HOTKEY_304,
	FKMS_HOTKEY_305,
	FKMS_HOTKEY_306,
	FKMS_HOTKEY_307,
	FKMS_HOTKEY_308,
	FKMS_HOTKEY_309,
	FKMS_HOTKEY_310,
	FKMS_HOTKEY_311,
	FKMS_HOTKEY_312,
	FKMS_HOTKEY_313,
	FKMS_HOTKEY_314,
	FKMS_HOTKEY_315,
	FKMS_HOTKEY_316,
	FKMS_HOTKEY_317,
	FKMS_HOTKEY_318,
	FKMS_HOTKEY_319,
	FKMS_HOTKEY_320,
	FKMS_HOTKEY_321,
	FKMS_HOTKEY_322,
	FKMS_HOTKEY_323,
	FKMS_HOTKEY_324,

	/// ���������ȼ�ö��ֵ
	FKMS_OTHERHOTKEY = 400,

	FKMS_OTHERHOTKEY_LOCKATTACK,		// ����Ŀ�������ͨ����
	FKMS_OTHERHOTKEY_AUTOMOVE,		// �Զ���������ƶ�
	FKMS_OTHERHOTKEY_FITTING,			// �Դ�����
	FKMS_OTHERHOTKEY_AUTOPRINTSCREEN,// �Զ���ͼ
	FKMS_OTHERHOTKEY_CHAT,			// ���������
	FKMS_OTHERHOTKEY_CHANGETARGET = 426,	// �л�����Ŀ��
	FKMS_OTHERHOTKEY_PICKGOODS,		// ʰȡ��Ʒ
	FKMS_OTHERHOTKEY_AFFIRMTARGET,	// ��ʾ��ǰѡ�е�Ŀ�����
	FKMS_OTHERHOTKEY_FIRESPELLSELF,	// �뼼�ܿ�ݼ����ʹ�ö��Լ�ʩ�ŷ���
	//FKMS_OTHERHOTKEY_CLOSEDISTANCE,	// �����ӽǾ���
	//FKMS_OTHERHOTKEY_FARDISTANCE,		// ��Զ�ӽǾ���

}FKMS;

/// �洢��ֵ�Ľṹ
typedef struct _SETKEY
{
	int mkey;			// ����
	int askey1;		// ��һ������
	int askey2;		// �ڶ�������
	_SETKEY():mkey(0),askey1(0),askey2(0){}

	bool operator==(_SETKEY& keys)
	{
		return (mkey==keys.mkey&&askey1==keys.askey1&&askey2==keys.askey2);
	}

	bool operator!=(_SETKEY& keys) { return !operator==(keys); }

	bool operator==(const _SETKEY& keys) const
	{
		return (mkey==keys.mkey&&askey1==keys.askey1&&askey2==keys.askey2);
	}

	bool operator!=(const _SETKEY& keys) const { return !operator==(keys); }
}SETKEY,*PSETKEY;

/// �洢���ȼ��Ĺ���
typedef struct _MAPKEY
{
	FKMS funcIndex;	// ��������
	SETKEY keyArray;	// ���ܶ�Ӧ���ȼ��飨�������������ȼ���ɣ�Ҳ����������һ������������
	BOOL modify;		// �˹��ܵ��ȼ��Ƿ�ɱ��޸�
	_MAPKEY():funcIndex(FKMS_UNKNOW),modify(0){}
}MAPKEY,*PMAPKEY;

typedef map<FKMS,MAPKEY> MAPFKEY;
typedef map<FKMS,MAPKEY>::iterator MAPFKEYITER;

/// �洢ÿ�������ȼ������ȫ����Ϣ���ͻ���ר�ã�
typedef struct _INFOFUNC
{
	string strFuncDesc;	// �����˹��ܵ�������Ϣ
	MAPKEY	key;			// ��Ӧ�Ĺ���
}INFOFUNC,*PINFOFUNC;

typedef map<FKMS,INFOFUNC> MAPFKEYINFO;
typedef map<FKMS,INFOFUNC>::iterator MAPFKEYINFOITER;
typedef map<FKMS,INFOFUNC>::const_iterator MAPFKEYINFOCITER;

/// �洢���๦�ܵ���Ϣ���ͻ���ר�ã�
typedef struct _INFOTYPE 
{
	WORD  typeId;				// ������ID
	string strTypeDesc;		// �����������������
	MAPFKEYINFO FuncKeyInfo;  // ��Ÿ��ֹ��ܼ����Ӧ���ȼ�������
}INFOTYPE,*PINFOTYPE;

typedef map<WORD,INFOTYPE> MAPINFOTYPE;
typedef map<WORD,INFOTYPE>::iterator MAPINFOTYPEITER;
typedef map<WORD,INFOTYPE>::const_iterator MAPINFOTYPECITER;

class CFuncKeyMapSystem
{

#ifndef USE_IN_SERVER
	
static MAPINFOTYPE m_DefultFuncKeyInfo;	// Ĭ�ϵ�������Ϣ��ͨ���������ļ������ã�

public:

static const MAPINFOTYPE& GetDefultFuncKey() { return m_DefultFuncKeyInfo; }
static bool LoadDefaultFuncMapKey(string strFileName);
static bool LoadFuncKeyConfig(TiXmlElement *pElem,INFOTYPE& typeinfo);

//��ȡ����
static int	GetIntAttribute(TiXmlElement *pElem,string strName);

// �ͷ�����
static void Release();

// ���ݲ���dwFuncIndexָ���Ĺ�������ֵ���˹��ܵ��ȼ��Ƿ񱻴���,bContinuousָ���˹����Ƿ������������ס�����ŵ����(��������ָ����������ʹ��)
static bool CheckFuncKey(MAPFKEY& mapkey,FKMS FuncIndex,SHORT appointkey,bool bContinuous);

// ���ݲ���dwFuncIndexָ���Ĺ�������ֵ���˹��ܵ��ȼ��Ƿ񱻴���,bContinuousָ���˹����Ƿ������������ס�����ŵ�����������ڰ���ǰ����ɨ����ȡ����ֵ��ʹ�ã�
static bool CheckFuncKey(MAPFKEY& mapkey,FKMS FuncIndex,bool bContinuous = false);

static string& GetHotKeyName(string&,const SETKEY&);					// ���ȼ��������ֵת���ɿɼ����ַ���

static string& ConvertVirKeyToString(string&,const int virKey);	// ��һ�������ֵת��Ϊ����Ӧ�Ŀ��ӻ����֣���������ϰ�ߵİ������ƣ�

#endif

//bool DefaultToCurrent();			// ��Ĭ�ϵ�������Ϣ���뵱ǰʹ�õ�m_FuncKey��

// �������Ϳͻ����ô˳�Ա���浱ǰ���ֹ��ܶ�Ӧ���ȼ�
//static MAPFKEY m_FuncKey;

// ��ȡ��ǰ����ʹ�õĹ����ȼ�ӳ���
//static MAPFKEY& GetCurMapKey() { return m_FuncKey; }

public:

// ����ֵ
static bool InsertKeyToCurMap(MAPFKEY&,FKMS,MAPKEY);

// ���ĳ�����ܵ�ǰ�Ƿ������ȼ�ֵ
static bool CheckFunHasAvailableKey(MAPFKEY&,FKMS);

// ɾ��ֵ
static bool DelKeyToCurMap(MAPFKEY&,FKMS);

// �޸�ĳ�����ܵ��ȼ�ֵ
static bool AlterKeyofCurMap(MAPFKEY&,FKMS,MAPKEY);

};

#endif