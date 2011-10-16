////////////////////////////////////////////////////////////////////////////
//CityGate.h//////////////////////////////////////////////////////////////
//������
//Author:Joe
//Create Time:2004.721
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "build.h"
#include "ServerRegion.h"
#include "..\setup\GateList.h"


//����
class CGate:
	public CBuild
{
public:
	CGate(void);
	~CGate(void);
	virtual BOOL InitProperty(CGateList::tagGate* p);
	virtual BOOL InitSetProperty(CServerRegion::tagGateProperty* p);
public:
	virtual void SetGate(WORD l);
	virtual void SetOpen(string str)	{m_strOpen=str;}
	virtual void SetClose(string str)	{m_strClose=str;}
	virtual void SetOpenSound(string str)	{m_strOpenSound=str;}
	virtual void SetCloseSound(string str)	{m_strCloseSound=str;}
	virtual string &GetOpen()				{return m_strOpen;}
	virtual string &GetClose()				{return m_strClose;}
	virtual string &GetOpenSound()			{return m_strOpenSound;}
	virtual string &GetCloseSound()			{return m_strCloseSound;}
	
	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	// ����
private:


	string	m_strOpen;		//�򿪽ű�
	string	m_strClose;		//�رսű�
	string	m_strOpenSound;	//������Ч
	string  m_strCloseSound;//������Ч

public:
	virtual void OnDied();															// ������ʱ����
	virtual bool IsAttackAble(CMoveShape* pAttackShape);								// �ɷ񹥻�		
};