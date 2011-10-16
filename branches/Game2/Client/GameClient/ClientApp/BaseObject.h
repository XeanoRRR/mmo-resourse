#pragma once
using namespace std;
#include "../../../Public/Module/GUID.h"
#include "../../../Public/DataStream/DataBlockSetWriteRead.h"
class CMessage;
class CBaseObject;
typedef list<CBaseObject*>	listBaseObject;
typedef list<CBaseObject*>::iterator itBaseObject;

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
	bool		m_bIsHide;		// �Ƿ����ظ�����Ϣ
	long		m_lType;		// ����
	//long		m_lID;			// ��Ϸ��Ψһ���
	CGUID		m_guExID;
	long		m_lGraphicsID;	// ͼƬID
	string		m_strName;		// ���� < 20

public:
	bool GetIsHideInfo()			{ return m_bIsHide; }
	void SetIsHideInfo(bool l)		{ m_bIsHide = l; }
	long GetType()					{ return m_lType; }
	void SetType(long l)			{ m_lType = l; }
	//long GetID()					{ return m_lID; }
	//void SetID(long l)				{ m_lID = l; }
	const CGUID& GetExID() const	{ return m_guExID;}
	void SetExID(const CGUID& guid){m_guExID = guid;};
	long GetGraphicsID()			{ return m_lGraphicsID; }
	void SetGraphicsID(long l)		{ m_lGraphicsID=l; }
	const char* GetName()			{ return (LPCSTR)m_strName.c_str(); }
	void SetName(const char* str)	{ m_strName = str; }

	// ���ԭʼ��(Monster��ɼ�����Ҫ��дGetOrigName�ӿڷ���ԭʼ��)
	virtual const char* GetOrigName()			{ return GetName();}

	virtual void GetCurGraphicTypeID(long &lType,long &lID){lType = m_lType;lID=m_lGraphicsID;}

/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////
public:
	bool	m_bIncludeChild;	// �Ƿ�洢�ӽڵ�����

	virtual BOOL Load();		// ��ȡ
	virtual BOOL Save();		// �洢

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);


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

	CBaseObject* IsThisObject(long typeID, const CGUID& id)			// �Ƿ��Ǳ�object
	{
		return (m_lType==typeID && m_guExID==id)?this:NULL;
	}

	virtual CBaseObject* RecursiveFindObject(long typeID,CGUID& id);		// �ݹ����
	virtual CBaseObject* RecursiveFindObject(long typeID,char* strName);// �����ֲ���
	
	virtual CBaseObject* FindChildGoods(const CGUID& guExID = CGUID::GUID_INVALID);// ������goods
	virtual CBaseObject* FindChildObject(long typeID,const CGUID& id);	// ������object!
	virtual CBaseObject* FindChildObject(const CGUID& id);	// ������object!
	virtual bool FindChildObject(CBaseObject* p);				// ������object!


public:
	virtual void DeleteChildObject(long typeID,const CGUID& ID);		// ɾ����OBJECT
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
};
