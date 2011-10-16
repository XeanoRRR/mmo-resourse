#pragma once

#include "../../../Public/ServerPublic/db/CnPool.h"

//  ��Ҫ����һ�����ݿ����Ӷ����ָ�룬��ʵ������BounsDB��ֱ��ʵ��
// 
class CBounsAdoBase
{
public:
	CBounsAdoBase(string strConnectionString);
	virtual ~CBounsAdoBase(void);	

	bool OpenCn();
	bool CloseCn();

	bool ExcuteSQL(LPCSTR sql);

	void PrintErr(LPCSTR msg, _com_error &e);
	void PrintErr(LPCSTR msg);

	_ConnectionPtr& GetConnPtr()	{ return m_cnPtr; }

private:
	_ConnectionPtr	m_cnPtr;
	string			m_strConnectionString;
};
