#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\public\GUID.h"

class CPlayer;
class CRegion;

// �����ṹ
struct stVariable
{
	char *Name;		//����
	long Array;		//���� (=0) ��ͨ���� (>1) �����ͱ��� (<0) �ַ�����,���ַ���
	union{
			double* Value;
			char* strValue;
	};
	stVariable(void)
	{
		memset(this, 0, sizeof(stVariable));
	}
};

// �����б���
class CVariableList :public BaseMemObj 
{
	vector<stVariable*> m_pVarList;		// �����б�
public:
	typedef std::map<std::string, CGUID*> GuidList;
	
	enum eError
	{
		VAR_NOT_FOUND=-99999999,	// �ñ���û���ҵ�
	};

	CVariableList& operator=(CVariableList& right)
	{
		if(this == &right)
		{
			Release();
			GuidList::iterator guidItr = right.m_pGuidList.begin(); // GUID�б�
			for(; guidItr != right.m_pGuidList.end(); guidItr++)
			{
				AddGuid(guidItr->first.c_str(), *guidItr->second);
			}

			vector<stVariable*>::iterator varItr = m_pVarList.begin();
			for(; varItr != m_pVarList.end(); varItr++)
			{
				stVariable* pVar = *varItr;
				if(pVar)
				{
					if(pVar->Array == 0)
						AddVar(pVar->Name, *pVar->Value);
					if(pVar->Array > 0)
					{
						AddVar(pVar->Name, pVar->Array, 0);
						for(int i=0; i<pVar->Array; i++)
							SetVarValue(pVar->Name, i, pVar->Value[i]);
					}
					else if(pVar->Array < 0)
						AddVar(pVar->Name, pVar->strValue);
				}
			}
		}
		return *this;
	};

	void IniVarNum(long lNum);		// ��ʼ����������

	void LoadVarList(char *file, char* index, const char* pData=NULL);	//��������б�

	void ReLoadVarList(char *file, char* index, const char* pData=NULL);	//���¶�������б�
	
	void AddVar(const char* name, const char* value); //����һ���ַ�������
	void AddVar(const char* name,double value); //����һ�����ͱ���
	void AddVar(const char* name,long bound,double value); //����һ�������������
	bool AddVar(const char* name); //���ӱ���(����nameǰ׺�Զ��ж�����)
	// [071116 AHC]
	void AddGuid(const char* name, const CGUID& guid); // ����һ��GUID����

	bool RemoveVar(const char* name);//ɾ������
	bool isExist(const char* name);	//�������Ƿ����

	// [071116 AHC]
	bool RemoveGuid(const char* name);//ɾ��Guid����
	bool isGuidExist(char* name);	//���Guid�����Ƿ����

	long GetVarNum()		{ return m_pVarList.size(); }	// ��ȡ��������
	void SetVarNum(long l)	{ m_pVarList.resize(l); }		// ���ñ�������

	int GetArrayNum(char *, CPlayer*, CRegion*);	// ��ȡ����������±ꡡ( >0 ���飬=0 �� )
	void GetArrayName(char *, char *);				// ��ȡ�������������
	double GetVarValue(const char *, int);					// ��ȡ������ֵ
	const char* GetVarValue(const char *);				// ��ȡ������ֵ(�ַ���)
	int SetVarValue(const char *, int=0, double=0);			// ���ñ�����ֵ
	int SetVarValue(const char *name, const char* value);		// ���ñ�����ֵ(�ַ���)
	
	CGUID GetGuidValue(const char *);				// ��ȡGUID��ֵ
	int SetGuidValue(const char *, const CGUID*);				// ����GUID��ֵ

	char* GetVarName(int n)			{ return m_pVarList[n]->Name; }		// ��ȡ����������
	int GetVarArray(int n)			{ return m_pVarList[n]->Array; }		// ��ȡ�������±�ֵ
	double GetVarValue(int n, int m=0)	{ return m_pVarList[n]->Value[m]; }	// ��ȡ������ֵ

	GuidList&	GetGuidList(void) { return m_pGuidList; }
public:
	CVariableList();
	virtual ~CVariableList();
	void Release();

	bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�vector<BYTE>
	bool DecordFromByteArray(BYTE* pByte, long& pos, long bufSize=-1);	// ����
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	void LoadVarData(char *, char*);	//���������ϸ����

	vector<stVariable*>& GetVarList()	{ return m_pVarList;}	// ���ر����б��ָ��

	GuidList m_pGuidList; // GUID�б�
};
