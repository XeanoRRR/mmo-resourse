//=============================================================================
/**
 *  file: Expressions.h
 *
 *  Brief:ʵ����״̬����������Ҫʹ�õ���һЩ�ű����ʽ
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EXPRESSIONS_H__
#define __EXPRESSIONS_H__

#pragma once
class CScript;
class Script;

class CExpressions
{
public:
	CExpressions(void);
	virtual ~CExpressions(void);
private:
	//���ʽ
	string m_strFileName;
	string m_strContent;
public:
	void InitBaseData(string& strFileName,string& strContend)
	{
		m_strFileName = strFileName;
		m_strContent = strContend;
	}
	//ִ�н��
	int Execute(Script* pScript,CMoveShape* pDestShape);
};

#endif
