/**
*File:  DBAdoBase.cpp
*Brief: ������:�ö���ά��һ�����ݿ�Cn����
*Author:������
*Date:  2008.11.03
*/
#include "stdafx.h"
#include "DBAdoBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DBAdoBase::DBAdoBase(string strConnectionString)
{
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	m_strConnectionString.clear();
	m_strConnectionString = strConnectionString;
}

DBAdoBase::~DBAdoBase(void)
{
    ::CoUninitialize();
}
   
/// ������
bool DBAdoBase::OpenCn(void)
{
	if(!SUCCEEDED(m_cnPtr.CreateInstance(__uuidof(Connection))))
		return false;

	try
	{
		m_cnPtr->ConnectionTimeout = 5;
		if(SUCCEEDED(m_cnPtr->Open(m_strConnectionString.c_str(), "", "", adConnectUnspecified)))
		{
			return true;
		}
	}
	catch(_com_error &e)
	{
		m_cnPtr.Release();
		m_cnPtr = NULL;
		PrintErr("conn open error", e);
	}
	return false;
}


bool DBAdoBase::CloseCn(void)
{
	try
	{
		if(m_cnPtr)
		{
			if(m_cnPtr->GetState() == adStateOpen)
			{
				m_cnPtr->Close();
			}
		}
	}
	catch(_com_error &e)
	{
		PrintErr("LogAdoBase Close cn Failed", e);
		return false;
	}

	if(m_cnPtr) m_cnPtr.Release();
	return true;
}

long DBAdoBase::CreateRs(_RecordsetPtr &rs)
{
    return rs.CreateInstance(__uuidof(Recordset));
}

/// �򿪽����
long DBAdoBase::OpenRs(LPCSTR sql, _RecordsetPtr &rs, 
                       CursorTypeEnum cursorType /* = adOpenStatic */, 
                       LockTypeEnum lockType /* = adLockReadOnly */, 
                       long lOptions /* = adCmdText */)
{
    return rs->Open(_bstr_t(sql), m_cnPtr.GetInterfacePtr(),
        cursorType,
        lockType,
        lOptions);
}

long DBAdoBase::CloseRs(_RecordsetPtr &rs)
{
    return rs->Close();
}

long DBAdoBase::ReleaseRs(_RecordsetPtr &rs)
{
    long lRet = CloseRs(rs);
    if(lRet == S_OK)
    {
        if(rs)
        {
            rs.Release();
        }
    }
    return lRet;
}

/// ִ�н����(������ִ�С��ͷ�)����Ҫ���ؽ����,��Ҫ���øýӿ�
bool DBAdoBase::ExecuteRs(LPCSTR sql, _RecordsetPtr &rs)
{
    CreateRs(rs);
    OpenRs(sql, rs);
    ReleaseRs(rs);
    return true;
}

/// ִ��SQL���
bool DBAdoBase::ExecuteCn(LPCSTR sql)
{
	try
	{	
		m_cnPtr->Execute(sql, NULL, adCmdText);
		return true;
	}
	catch(...)
	{
		try
		{	
			CloseCn();
			if (OpenCn())
			{
				m_cnPtr->Execute(sql, NULL, adCmdText);
				return true;
			}
		}
		catch(_com_error &e)
		{
            // ���õĳ����ӷ�ʽ�����ﲻ����CloseCn()!!!!
            // ����ExecuteCn�����쳣���ٵ���ExecuteRs��OpenRs
            // �ض�����!!!
			PrintErr("conn execute error", e);
		}

		return false;
	}
}

void DBAdoBase::PrintErr(LPCSTR msg, _com_error &e)
{
	char buf[1024];

	if (e.Error() == 0x80004005)
	{
		wsprintf( buf, "%s (һ�����������)", msg);
	}
	else
	{
		wsprintf( buf, "%s\r\n \
					   Code = %08lx\r\n \
					   Cnode meaning = %d\r\n \
					   Source = %s\r\n \
					   Description = %s\r\n",
					   msg ,e.Error(), e.ErrorMessage(), (LPCSTR)((_bstr_t)e.Source()), (LPCSTR)((_bstr_t)e.Description()));
	}

	Log4c::Warn(ROOT_MODULE,"%s",buf);
}

void DBAdoBase::PrintErr(LPCSTR msg)
{
	Log4c::Warn(ROOT_MODULE,"Connetcion Executing : %s",msg);
}
