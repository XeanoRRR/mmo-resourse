#pragma once
#ifndef _CContributeSetup_h_
#define _CContributeSetup_h_
class CContributeSetup
{
public:
	struct tagContributeItem
	{
		DWORD	dwLoValue;		// ��Ʒ����
		DWORD	dwHiValue;		// ��Ʒ����
		DWORD	dwNum;			// ��Ʒ����
		string	strName;		// ����ԭʼ��
	};


public:
	CContributeSetup(void);
	virtual ~CContributeSetup(void);


//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:

	static long	lCombatLevel1;
	static long	lCombatLevel2;

	static long	lContribute1;
	static long	lContribute2;

	static long	lContributeLevel;
	static long	lContributeMultiple;

	static long	lContributeBaseValue;
	static long	lContributeMax;

	static long	lCountryCityContributeModifierLose;
	static long	lCountryCityContributeModifierGain;
	static long	lCountryCityContributeItemModifier;


	static vector<tagContributeItem>	m_vContributeItems;	// ��Ʒ�б�

public:
	static BOOL LoadContributeSetup(const char* filename);			// ���ļ���ȡ�б�

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};

#endif