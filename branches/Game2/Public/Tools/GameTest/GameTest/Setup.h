//=============================================================================
/**
 *  file: Setup.h
 *
 *  Brief:������
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-7-30
 */
//=============================================================================
#ifndef __SETUP_H__
#define __SETUP_H__
#pragma once

class CSetup
{
public:
	CSetup(void);
	virtual ~CSetup(void);

private:
	static CSetup* instance;
public:
	static CSetup* getInstance();
	static void Release();
public:
	//������IP�Ͷ˿�
	string	m_strServerIP;
	int		m_nPort;

	string m_strIniScript;
public:
	void Load(const char* file=NULL);
};

#endif
