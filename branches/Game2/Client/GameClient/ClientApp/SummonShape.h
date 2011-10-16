#pragma once
#include "MoveShape.h"
#include "../../../Public/Common/SkillRelated.h"
//##�ٻ���û���������ٻ���,���綾��,ħ����
class CSummonShape : public CMoveShape
{
protected:
	//##�������ȣ�0��ʾ����ʧ
	DWORD	m_dwLifeTime;

	//##����ʱ��
	DWORD	m_dwTimeStamp;

	tagSkillID	m_tSkillID;
	long	m_lSkillLevel;
	long	m_lHostType;	// ��������
	long	m_lHostID;		// ����id

public:
	//##����/��������
    CSummonShape();
	virtual ~CSummonShape();

	DWORD	GetRemainedTime();
	DWORD	GetLifeTime()			{return m_dwLifeTime;}
	void	SetLifeTime(DWORD dw)	{m_dwLifeTime=dw;}

	//##�����ٻ����ܵ�����
	virtual tagSkillID	GetSkillID()	{return m_tSkillID;}
	void SetSkillID(tagSkillID tID)		{m_tSkillID=tID;}
	long GetSkillLevel()				{return m_lSkillLevel;}
	void SetSkillLevel(long l)			{m_lSkillLevel=l;}

	void SetHostType(long lType)		{m_lHostType = lType;}
	void SetHostID(long lID)			{m_lHostID = lID;}
	long GetHostType()					{return m_lHostType;}
	long GetHostID()					{return m_lHostID;}
	
	virtual BOOL		DecodeFromByteArray( BYTE* pByte, LONG& lPos, bool bExData ) {return true;}
};
