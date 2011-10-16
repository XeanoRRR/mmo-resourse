


/*
*	file:		OrganBuffSys.h
*	Brief:		��֯ϵͳ��buffϵͳ
*	detail:		������֯ϵͳ�����buff����
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-11-03
*
*/



#pragma once




class OrganBuffSys
{
public:
	//! ��֯����
	enum eOrganType
	{
		eOT_Faction,
		eOT_Union,
	};

public:
	OrganBuffSys(VOID);
	OrganBuffSys(DWORD dwOrganType);
	~OrganBuffSys(VOID);

public:
	//!					��ȡ����
	static	BOOL		LoadSetup(const char *pFilename);

	//!					����
	BOOL				AddToByteArray(vector<BYTE>& ByteArray);
	//!					����
	BOOL				Decode(BYTE* pData, LONG &lPos);


public:
	//!					���һ�����õ�buff
	BOOL				AddBuff(DWORD dwBuff);
	//!					ɾ��һ�����õ�buff
	BOOL				DelBuff(DWORD dwBuff);

	//!					����һ��buff
	BOOL				StartBuff(DWORD dwBuff);
	//!					�ر�һ��buff
	BOOL				CloseBuff(DWORD dwBuff);


private:
	//!		buff����
	struct	tagOrgBuffSetup
	{
		//!		buff���
		DWORD	_dwBuffID;
		//!		Ĭ�ϵĿ���״̬
		BOOL	_bOpenState;
		//!		�����ȼ�
		DWORD	_dwOpenLvl;
	};

	//!										��������
	static	map<DWORD, tagOrgBuffSetup>		m_mapFactionSetup;
	//!										��������
	static	map<DWORD, tagOrgBuffSetup>		m_mapUnionSetup;
};