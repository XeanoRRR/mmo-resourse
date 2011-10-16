//DBAdoBase.h/////////////////////////////////////////////////////////////////////
//������:�ö���ά��һ�����ݿ�Cn����
//Author:������
//Create Time:2008.11.03
#ifndef DB_ADOBASE_H
#define DB_ADOBASE_H

#pragma warning (disable: 4146)
#import "../../dbaccess/msado15.dll" no_namespace rename("EOF","adoEOF")

#include "../public/GUID.h"
#include <string>



//! ---------------------------------------��־���ݿ�����ķ�װ---------------------------------------
//!	ֻ��ִ���޷��ص�SQL���
//! ������ʱ�Զ�����DB������ʱ�ͷ�����
//! ������ʧЧ��ÿִ��SQL���ʱ�������³�������
//!---------------------------------------------------------------------------------------------------
class DBAdoBase
{
public:
	DBAdoBase(	std::string strConnectionString);

	virtual ~DBAdoBase(void);

public:
	//! ִ��SQL���
	bool ExecuteCn(LPCSTR sql);
	// ȡ�����Ӷ���
	_ConnectionPtr& GetConnPtr(void) { return m_cnPtr; }
	
	//! ������
	bool OpenCn(void);
	//! �ر�����
	bool CloseCn(void);

	//! �����ݿ����Ӷ���
	_ConnectionPtr m_cnPtr;

	//! �����ַ�������
	std::string m_strConnectionString;

	// ��������#
	void PrintErr(LPCSTR msg, _com_error &e);
	void PrintErr(LPCSTR msg);
};

#endif