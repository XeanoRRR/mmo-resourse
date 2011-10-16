#pragma once
#include "../../GMTPublic/def/InfoDef.h"


class InfoManage
{
public:
	InfoManage(VOID);
	~InfoManage(VOID);

public:
	//!
	static	InfoManage&		GetInstance(VOID);

public:
	//!						��ʼ������
	BOOL					Init(VOID);
	//!						�ͷŶ���
	VOID					Release(VOID);


private:
	//!						�õ�һ�����͵ĸ�����
	DWORD					GetParentType(DWORD dwInfoType);

private:
	//!						������Ӧ��
	map<DWORD, DWORD>		m_mapHypotaxisTable;

private:
	static	InfoManage		c_InfoManage;
};