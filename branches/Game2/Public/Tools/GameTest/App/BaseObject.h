#pragma once
#include "../Utility/timerdef.h"
#define D3DX_PI    ((FLOAT)  3.141592654f)

class CBaseObject;
typedef list<CBaseObject*>	listBaseObject;
typedef list<CBaseObject*>::iterator itBaseObject;

typedef LRESULT (CALLBACK* P_FUNCTION_DO)(LPARAM lparam1, LPARAM lparam2);

// CBaseObject command target
// ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
class CBaseObject:public CGameEvent
{
public:
	CBaseObject();
	virtual ~CBaseObject();

	// ��������
protected:
	long		m_lType;		// ����
	CGUID		m_guExID;		// ��Ϸ��Ψһ���
	//	long		m_lGraphicsID;	// ͼƬID
	char		m_strName[32];		// ���� < 20

	//�¼ӵı�����Ա
	bool		m_bIsHide;

	//�������Խӿ�
public:
	long GetType() const 			{ return m_lType; }
	void SetType(long l)			{ m_lType = l; }

	const CGUID& GetExID()			{ return m_guExID;}
	void SetExID(const CGUID& guid){m_guExID = guid;};

	//	long GetGraphicsID() const		{ return m_lGraphicsID; }
	//	void SetGraphicsID(long l)		{ m_lGraphicsID=l; }

	const char* GetName()			{ return /*(LPCSTR)*/m_strName; }	//.c_str()����Ϊconst char* ����ǿת
	void SetName(const char* str)	{ strcpy(m_strName,str); }

	virtual const char* GetOrigName()			{ return GetName();}			// ���ԭʼ��(Monster��ɼ�����Ҫ��дGetOrigName�ӿڷ���ԭʼ��)

	//	virtual void GetCurGraphicTypeID(long& lType, long& lID)	{ lType = m_lType; lID = m_lGraphicsID; }

	// ��ȡ�ӿ�
public:
	bool	m_bIncludeChild;			//�Ƿ�洢�ӽڵ�����
	virtual BOOL Load();		// ��ȡ
	virtual BOOL Save();		// �洢

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool DecordFromDataBlock(DBReadSet& db_r,bool bEx = true);

	/////////////////////////////////////////////////////////////////////
	// �ӽڵ��б�������������
	/////////////////////////////////////////////////////////////////////
protected:
	CBaseObject* m_pFather;							// ���ڵ㣨ֻ��һ����
	list<CBaseObject*>	m_listObject;				// �ӽڵ�
public:
	list<CBaseObject*>* GetObjectList()		{return &m_listObject;}
	CBaseObject* GetFather()  {return m_pFather;}				// ��ȡ���ڵ�
	void SetFather(CBaseObject* p)	{ m_pFather = p; }			// ���ø��ڵ�

	CBaseObject* IsThisObject(long typeID, const CGUID& gID)			// �Ƿ��Ǳ�object
	{
		return (m_lType==typeID && m_guExID==gID)?this:NULL;
	}

	virtual CBaseObject* RecursiveFindObject(long typeID,CGUID& gID);	// �ݹ����
	virtual CBaseObject* RecursiveFindObject(long typeID,char* strName);// �����ֲ���

	virtual CBaseObject* FindChildGoods(const CGUID& guExID = CGUID::GUID_INVALID);// ������goods
	virtual CBaseObject* FindChildObject(long typeID,const CGUID& gId);
	virtual CBaseObject* FindChildObject(const CGUID & guid);
	virtual bool FindChildObject(CBaseObject* p);				// ������object!


public:	
	virtual void DeleteChildObject(long typeID,const CGUID& gID);		// ɾ����OBJECT
	virtual void DeleteChildObject(CBaseObject* p);						// ɾ����OBJECT
	virtual void DeleteAllChildObject(CBaseObject* pExpect=NULL);		// ɾ��������OBJECT(pExpect = ����)
	virtual void RemoveObject(CBaseObject* pObject);					// �Ƴ�OBJECT
	virtual void AddObject(CBaseObject* pObject);						// ���OBJECT

	// ��Ϣ��Ӧ
	virtual void OnMessage(CMessage*)	{ return; }
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var){return 0;};
	//����ʱ����ȡ���ĵ���
	virtual void OnTimerCancel(DWORD timerid,const void* var){};
	//����ʵ�����Ϣ����
	//virtual long Proc(DWORD type,const void* var1,const void* var2){return 0;};
    virtual long Proc(eEventType type,const tagProcVar *pVar) { return 0; }
	// �ص�һ�����������в��ҵ��Ķ�ӦTYPE����������
	virtual void DgFindObjectsByTypes( long lType, P_FUNCTION_DO p_function_do, LPARAM lparam);
};

//
// Kevin Lynx, ugly codes, to access UI.
//
Timer *QueryTimer();