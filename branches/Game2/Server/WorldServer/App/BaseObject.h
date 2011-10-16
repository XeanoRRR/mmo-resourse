#pragma once

class CMessage;
class CBaseObject;
typedef list< CBaseObject* > listBaseObject;
typedef list< CBaseObject* >::iterator itBaseObject;

typedef LRESULT (CALLBACK* P_FUNCTION_DO)(LPARAM lparam1, LPARAM lparam2);

// CBaseObject command target
// ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
class CBaseObject 
{
public:
	CBaseObject();
	virtual ~CBaseObject();

/////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////
protected:
	long		m_lType;		// ����
	//long		m_lID;			// ��Ϸ��Ψһ���
	CGUID		m_guExID;
	long		m_lGraphicsID;	// ͼƬID
	string		m_strName;		// ���� < 20

public:
	long GetType()					{ return m_lType; }
	void SetType(long l)			{ m_lType = l; }
	/*long GetID()					{ return m_lID; }
	void SetID(long l)				{ m_lID = l; }*/
	virtual const CGUID& GetExID()	{ return m_guExID;}
	virtual void SetExID(const CGUID& guid){m_guExID = guid;};
	long GetGraphicsID()			{ return m_lGraphicsID; }
	void SetGraphicsID(long l)		{ m_lGraphicsID=l; }
	const char* GetName()			{ return (LPCSTR)m_strName.c_str(); }
	void SetName(const char* str)	{ if(NULL == str) return; m_strName = str; }

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
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);		// ����


	// ���书��
public:
	BYTE			GetCanHide(void) { return m_byCanHide; }// �ܷ�����
	void			SetCanHide(BYTE flag) { m_byCanHide = flag; }
	BYTE			GetHideFlag(void) { return m_byHideFlag; }// ��ǰ����״̬
	void			SetHideFlag(BYTE flag) { m_byHideFlag = flag; }
private:
	BYTE			m_byCanHide; // �ܷ�����
	BYTE			m_byHideFlag; // ��ǰ����״̬

/////////////////////////////////////////////////////////////////////
// �ӽڵ��б�������������
/////////////////////////////////////////////////////////////////////
protected:
	CBaseObject* m_pFather;					// ���ڵ㣨ֻ��һ����
	list<CBaseObject*> m_listObject;		// �ӽڵ�

public:
	list<CBaseObject*>* GetObjectList()		{return &m_listObject;}
	CBaseObject* GetFather() {return m_pFather;}				// ��ȡ���ڵ�
	void SetFather(CBaseObject* p)	{ m_pFather = p; }			// ���ø��ڵ�

	CBaseObject* IsThisObject(long typeID, const CGUID& guid)			// �Ƿ��Ǳ�object
	{
		return (m_lType==typeID && guid==m_guExID)?this:NULL;
	}

	virtual CBaseObject* RecursiveFindObject(long typeID,const CGUID& guid);		// �ݹ����
	virtual CBaseObject* RecursiveFindObject(long typeID,char* strName);// �����ֲ���
	virtual CBaseObject* FindChildObject(long typeID,const CGUID& guExID = CGUID::GUID_INVALID);// ������object!
	virtual bool FindChildObject(CBaseObject* p);				// ������object!

	//virtual CBaseObject* CreateChildObject(long typeID,const CGUID& guid,const char* name=NULL, long lGraphicsID=0 );
	static CBaseObject* CreateObject(long typeID, const CGUID& guid);		// ����OBJECT

public:
	virtual void DeleteChildObject(long typeID, CGUID& guExID = CGUID::GUID_INVALID);		// ɾ����OBJECT
	virtual void DeleteChildObject(CBaseObject* p);				// ɾ����OBJECT
	virtual void DeleteAllChildObject(CBaseObject* pExpect=NULL);	// ɾ��������OBJECT(pExpect = ����)
	virtual void RemoveObject(CBaseObject* pObject);			// �Ƴ�OBJECT
	virtual void AddObject(CBaseObject* pObject);				// ���OBJECT

/////////////////////////////////////////////////////////////////////
// AI
/////////////////////////////////////////////////////////////////////
	virtual void AI();		// AI����

/////////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ
/////////////////////////////////////////////////////////////////////
	virtual void OnMessage(CMessage*)	{ return; }

/////////////////////////////////////////////////////////////////////
// �㲥��Ϣ
/////////////////////////////////////////////////////////////////////
	virtual void BoardCast(long lCommand, long lParam);		// �㲥

	// �ص�һ�����������в��ҵ��Ķ�ӦTYPE����������
	virtual void DgFindObjectsByTypes( long lType, P_FUNCTION_DO p_function_do, LPARAM lparam);


//����й¶������,author:wangqiao,date:2008.10.11
//֧�ֵ��߳�
#ifdef __MEMORY_LEAK_CHECK__
private:
	static long	m_nConstructFlag;	//������Ƕ���ĳ�ι���ı�ʾ
	long	m_lSelfFlag;
	typedef map<long,long>	mapLL;
	typedef map<long,long>::iterator itLL;
	typedef map<long,map<long,long> >	mapLLL;
	typedef map<long,map<long,long> >::iterator itLLL;
	
	static mapLLL	m_mapConstructObjects;
public:
	void SetSelfConsflag(long l)	{m_lSelfFlag = l;}
	long GetSelfConsFlag()			{return m_lSelfFlag;}
	static void ClearAllConstrucRef()	{ m_mapConstructObjects.clear(); }
	static void SetConstructFlag(long nFlag)	{m_nConstructFlag=nFlag;}
	static long GetConstructFlag()				{return m_nConstructFlag;}
	static void AddConstructRef(long lType);
	static void RefConstructRef(long lType,long lConFlag);
	static void OutputConstructRef(long lType);
#endif
};
