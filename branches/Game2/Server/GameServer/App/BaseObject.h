#pragma once

class CMessage;
class CBaseObject;

const LONG MOSTER_ROW_LENTH= 1024;
typedef hash_set<CGUID,hash_guid_compare>	hashsetGUID;
typedef hash_set<CGUID,hash_guid_compare>::iterator hsGUIDIt;

const long MIN_VAL[7] = { 0,		0,		0,			(long)0.0f,				-2147483647,	-32767, 0};
const long MAX_VAL[7] = { 256,	65536,	0x7FFFFFFF, (long)99999999.999f,	0x7FFFFFFF,		32767,	1};

// CBaseObject command target
// ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
class CBaseObject: public CGameEvent
{
public:
	CBaseObject();
	virtual ~CBaseObject();

public:
	/*static INT64	GetHashValue	( LONG, LONG );
	static LONG		CalculateType	( INT64 );
	static LONG		CalculateID		( INT64 );*/

/////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////
protected:
	long		m_lType;		// ����
	//long		m_lID;			// ��Ϸ��Ψһ���
	CGUID		m_guid;		// ��Ϸ��Ψһ���
	long		m_lGraphicsID;	// ͼƬID
	string		m_strName;		// ���� < 20

	struct tagPty
	{
		BYTE lType;		// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool 7:CGUID
		union
		{
			BYTE	*plValue;
			WORD	*pwValue;
			DWORD	*pdwValue;
			long	*pValue;
			float	*pfValue;
			short	*psValue;
			bool	*pbValue;
			//CGUID	*pGUIDValue;
		};

		long  lCount;
		long  maxValue; // �Զ������ֵ
		long  minValue; // �Զ�����Сֵ

		DWORD attrEnumValue; // ������ö��ֵ
        
        void SetType(BYTE type)
        {
            lType = type;
            minValue = MIN_VAL[type];
            maxValue = MAX_VAL[type];
        }
	};

public:
	long GetType() const								{ return m_lType;					};
	void SetType(long l)								{ if(0 == m_lType) m_lType = l;		};
	//long GetID()										{ return m_lID;						};
	//void SetID(long l)								{ m_lID = l;						};
	const CGUID&	GetExID() const						{return m_guid;						};
	void			SetExID(const CGUID& guid)			{m_guid = guid;						};
	long GetGraphicsID() const							{ return m_lGraphicsID;				};
	void SetGraphicsID(long l)							{ m_lGraphicsID=l;					};
    const char* GetName() const                         { return (LPCSTR)m_strName.c_str(); };
	void SetName(const char* str)						{ m_strName = str;					};

/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////
public:
	bool	m_bIncludeChild;	// �Ƿ�洢�ӽڵ�����

	virtual BOOL Load();		// ��ȡ
	virtual BOOL Save();		// �洢

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	// ����

	bool CodeToDataBlock_ForClient(DBWriteSet& setWriteDB,bool bExData = true/*�Ƿ��͸��Լ�*/);		// ��ӵ�CByteArray

	// ���书��
public:
	BYTE			GetCanHide(void) { return m_byCanHide; }// �ܷ�����
	void			SetCanHide(BYTE flag) { m_byCanHide = flag; }
	BYTE			GetHideFlag(void) { return m_byHideFlag; }// ��ǰ����״̬
	void			SetHideFlag(BYTE flag);// { m_byHideFlag = flag; }
private:
	BYTE			m_byCanHide; // �ܷ�����	0�������� 1������
	BYTE			m_byHideFlag; // ��ǰ����״̬ 0:������ 1������
/////////////////////////////////////////////////////////////////////
// �ӽڵ��б�������������
/////////////////////////////////////////////////////////////////////
protected:
	CBaseObject* m_pFather;					// ���ڵ㣨ֻ��һ����
	//list<CBaseObject*> m_listObject;		// �ӽڵ�

public:
	//list<CBaseObject*>* GetObjectList()		{return &m_listObject;}
	CBaseObject* GetFather()const {return m_pFather;}				// ��ȡ���ڵ�
	void SetFather(CBaseObject* p)	{ m_pFather = p; }			// ���ø��ڵ�

	//CBaseObject* IsThisObject(long typeID, long lID)			// �Ƿ��Ǳ�object
	//{
	//	return (m_lType==typeID && m_lID==lID)?this:NULL;
	//}

	//virtual CBaseObject* RecursiveFindObject(long typeID,long lID);		// �ݹ����
	//virtual CBaseObject* RecursiveFindObject(long typeID,char* strName);// �����ֲ���
	virtual CBaseObject* FindChildObject(long typeID,const CGUID& guid);// ������object!
	virtual bool FindChildObject(CBaseObject* p);				// ������object!

	//���������CreateObjct+AddObject��ʵ��
	//virtual CBaseObject* CreateChildObject(long typeID,const CGUID& guid,const char* name=NULL, long lGraphicsID=0 );
	//static CBaseObject* CreateObject(long typeID, const CGUID& guid);		// ����OBJECT

public:
	virtual void DeleteChildObject(long typeID,long lID,CGUID& guExID = CGUID::GUID_INVALID);		// ɾ����OBJECT
	virtual void DeleteChildObject(CBaseObject* p);				// ɾ����OBJECT
	virtual void DeleteAllChildObject(CBaseObject* pExpect=NULL);	// ɾ��������OBJECT(pExpect = ����)

	virtual void RemoveObject(CBaseObject* pObject);			// �Ƴ�OBJECT
	virtual void RemoveObject( LONG lType, LONG lID );
	virtual void AddObject(CBaseObject* pObject);				// ���OBJECT
private:
	//��ע����¼��ṹ
	typedef map<long,tagEvenRegVar>::iterator itEventVar;
	typedef map<long,tagEvenRegVar>	EventVar;

	//��ע��������¼�
	typedef map<eEventType,EventVar>::iterator itEvent;
	typedef map<eEventType,EventVar> RegEvents;
	RegEvents m_Events;

	long m_lEventIDCount;


	struct tagDelEventNode
	{
		eEventType type;
		long lEventID;
	};

	typedef list<tagDelEventNode>::iterator itDelEvent;
	list<tagDelEventNode> m_DelEventNodes;
public:

	//������Ϣ��Ӧ
	virtual void OnMessage(CMessage*)	{ return; }
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var) {return 0;}
	//����ʱ����ȡ���ĵ���
	void OnTimerCancel(DWORD timerid,const void* var);
	//����ʵ�����Ϣ����
	virtual long Proc(eEventType type,const tagProcVar *pVar){return 0;}

	long GetEventIDCount()	{m_lEventIDCount++; return m_lEventIDCount;}
	//ע���¼�
	virtual void RegisterEvent(eEventType nType,tagEvenRegVar &RegVar);
	//ע���¼�
	virtual void UnRegisterEvent(eEventType nType,long lEventID);
	//�����¼�
	virtual void ProduceEvent(eEventType nType,long lvar1=0,long lVar2=0,long lVar3=0,long lVar4=0, void* pVar3=NULL);
	//ɾ��ע���¼��ڵ�
	virtual	void ClearEventNode();
	void ClearAllEventNode();
	//
/////////////////////////////////////////////////////////////////////
// �㲥��Ϣ
/////////////////////////////////////////////////////////////////////
	//virtual void BoardCast(long lCommand, long lParam);		// �㲥

	// �ص�һ�����������в��ҵ��Ķ�ӦTYPE����������
	//virtual void DgFindObjectsByTypes( long lType, P_FUNCTION_DO p_function_do, LPARAM lparam);
};
