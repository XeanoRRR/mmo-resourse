#pragma once
#include "moveshape.h"
#include "Monster.h"
//typedef  CMonster CNpc;
/*
class CNpc :
	public CMoveShape
{
public:
	CNpc(void);
	virtual ~CNpc(void);

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

//////////////////////////////////////////////////////////////////////////
//	ս��
//////////////////////////////////////////////////////////////////////////
public:
	// ս����ʽ�����õ��麯��
	virtual DWORD	GetExp()						{return 0;}		//	����ֵ
	virtual DWORD	GetExpDead()					{return 0;}		//	ɱ���õ��ľ���ֵ
	virtual DWORD	GetHP()							{return 0;}
	virtual void	SetHP(DWORD l)					{}
	virtual DWORD	GetMaxHP()						{return 0;}

	virtual WORD	GetMinAtk()						{return 0;}
	virtual WORD	GetMaxAtk()						{return 0;}
	virtual WORD	GetHit()						{return 0;}
	virtual WORD	GetDef()						{return 0;}
	virtual WORD	GetDodge()						{return 0;}
	virtual WORD	GetAtcSpeed()					{return 0;}
	virtual WORD	GetElementResistant()			{return 0;}
	virtual WORD	GetSoulResistant()				{return 0;}
	virtual WORD	GetHpRecoverSpeed()				{return 0;}
	virtual WORD	GetMpRecoverSpeed()				{return 0;}

	virtual bool IsAttackAble(CMoveShape* pAttackShape){return false;}	// �ɷ񹥻�
	virtual long GetAckRangeMin()					{return 0;}
	virtual long GetAckRangeMax()					{return 0;}
	virtual long GetGuardRange()					{return 0;}
	virtual long GetChaseRange()					{return 0;}

	virtual WORD LossHP(long lLossHP, CMoveShape* pAttacker=NULL) {return 0;};	// ��Ѫ

//////////////////////////////////////////////////////////////////////////
//	�ű�
//////////////////////////////////////////////////////////////////////////
public:
	const char* GetScriptFile()			{return m_strScriptFile.c_str();}
	void SetScriptFile(const char* str)	{m_strScriptFile=str;}
	void Talk( const char* strContent );

	virtual VOID			EnterCombatState()		{}
	virtual VOID			EnterPeaceState()		{}
	virtual VOID			UpdateCurrentState()	{}
	virtual BOOL			IsDied()				{ return FALSE; }

private:
	string m_strScriptFile;		// �ű��ļ���


//////////////////////////////////////////////////////////////////////////
//	npc�б�
//////////////////////////////////////////////////////////////////////////
public:
	void SetShowList(bool b)		{m_bShowList=b;}
	bool IsShowList()				{return m_bShowList;}

private:
	bool m_bShowList;
};
*/
