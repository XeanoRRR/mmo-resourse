#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\public\GUID.h"

#define NAME_STR_NUM 256
#define VALUE_STR_NUM 5120

class CPlayer;
class CRegion;
class CMonster;

// �����ṹ
struct stVariable : public BaseMemObj
{
    stVariable();
	~stVariable(void);

	long Array;		//���� (=0) ��ͨ���� (>1) �����ͱ��� (<0) �ַ�����,���ַ���
	union{
			double* Value;
			char* strValue;
	};
};


/*-------------------------------------------*/
// �ṩ��hashʹ��
class str_compare
{
public:
	inline bool operator( )( const string& Key1, const string& Key2 ) const
	{
		if(Key1 < Key2)
			return true;
		return false;
	}

};

class hash_str_compare:public hash_compare<string,str_compare>
{
private:
	str_compare	comp;
public:
	//����ɢ��ֵ
	size_t operator( )( const string& Key ) const
	{
		unsigned long __h = 0;
		for (size_t i = 0 ; i < Key.size() ; i ++)
			__h = 5*__h + Key[i];
		return size_t(__h);

	}
	//less operator
	bool operator( )( const string& _Key1,const string& _Key2 ) const
	{
		return comp(_Key1,_Key2);
	}
};

// �����б���
class CVariableList : public BaseMemObj 
{
public:
	typedef hash_map<string,stVariable*, hash_str_compare> VarList;

    typedef VarList::iterator varlistitr;
    varlistitr VarListBegin(void) { return m_pVarList.begin(); }
    varlistitr VarListEnd(void)   { return m_pVarList.end();   }
    varlistitr VarListNext(void)   { return m_VarListItr++;   }
    long GetVarNum()		{ return m_pVarList.size(); }	// ��ȡ��������

   
	typedef hash_map<string,CGUID*, hash_str_compare> GuidList;

private:
    VarList m_pVarList; // �����б�
    varlistitr m_VarListItr;

    GuidList m_pGuidList; // GUID�б�

public:
	enum eError
	{
		VAR_NOT_FOUND=-99999999,	// �ñ���û���ҵ�
	};

	void LoadVarList(char *file, char* index, const char* pData=NULL);	//��������б�

	void AddVar(const char* name,const char* value); //����һ���ַ�������
	void AddVar(const char* name,double value); //����һ�����ͱ���

    // [070528 AHC]
    void AddGuid(const char* name, const CGUID& guid); // ����һ��GUID����

	void AddVar(const char* name,long bound,double value); //����һ�������������
	bool AddVar(const char* name); //���ӱ���(����nameǰ׺�Զ��ж�����)

	bool RemoveVar(const char* name);//ɾ������
	bool isExist(const char* name);	//�������Ƿ����

    // [070528 AHC]
    bool RemoveGuid(const char* name);//ɾ��Guid����
    bool isGuidExist(char* name);	//���Guid�����Ƿ����

    // [070528 AHC]
    long GetGuidNum()		{ return m_pGuidList.size(); }	// ��ȡGuid��������

	int GetArrayNum(const char *, CPlayer*, CRegion*, CMonster* pNpc=NULL, CGUID& guUsedItemID=CGUID::GUID_INVALID, DWORD dwDiedMonsterIndex=0);	// ��ȡ����������±ꡡ( >0 ���飬=0 �� )
	void GetArrayName(const char *, char *);		// ��ȡ�������������
	
    double GetVarValue(const char *, int);				// ��ȡ������ֵ
	const char* GetVarValue(const char *);			// ��ȡ������ֵ(�ַ���)

    const CGUID &GetGuidValue(const char *);				// ��ȡGUID��ֵ
    int SetGuidValue(const char *, const CGUID*);				// ����GUID��ֵ

	int SetVarValue(const char *, int=0, double=0);	// ���ñ�����ֵ
	int SetVarValue(const char *name,const char* value);	// ���ñ�����ֵ(�ַ���)

	GuidList* GetGuidList(void) { return &m_pGuidList; }
public:
	CVariableList();
	virtual ~CVariableList();
	void Release();

	bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�vector<BYTE>
	bool DecordFromByteArray(BYTE* pByte, long& pos, bool isPlayerVar=true, long bufSize=-1);	// ����

	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	bool DecodeFromDataBlock(DBReadSet& setReadDB,bool isPlayerVar=true);

	void LoadVarData(char *, char*);	//���������ϸ����
	void SaveVarData(char *, char*);	//д�������ϸ����

	VarList* GetVarList()	{ return &m_pVarList;}	// ���ر����б��ָ��
	void SetVarList(const VarList* VarList);
	void UpdateVarList(const VarList* VarList);	//�����б��еı���

	void ReleaseGuidList(void);
	void SetGuidList(const GuidList* guidList);
};
