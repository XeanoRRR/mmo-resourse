/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CMonsterInfo.h
*  ���ܣ�������ҹ�����ʾ�б�
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ� 
*
============================================================
*/

#pragma once
#include "afxwin.h"
#include "../App/Monster.h"
#include "../App/Region.h"
#include "afxcmn.h"

class CMonsterInfo : public CDialog
{
	DECLARE_DYNAMIC(CMonsterInfo)

public:
	CMonsterInfo(CWnd* pParent = NULL);
	virtual ~CMonsterInfo();

	enum { IDD = IDD_PROPPAGE_MONSTER_INFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:

	void SetDisplayMonster(CRegion* pRegion);
	virtual BOOL OnInitDialog();
	CListCtrl m_listMonster;
	CListCtrl m_listNpc;

private:

	struct staMsInfo{
		CString name;
		long lType;
		CGUID gID;
	} ;
	list<staMsInfo*>	m_pMonster;

public:

	struct staClientMsList{
		long lListID;
		staMsInfo MsInfo;
	};

};
