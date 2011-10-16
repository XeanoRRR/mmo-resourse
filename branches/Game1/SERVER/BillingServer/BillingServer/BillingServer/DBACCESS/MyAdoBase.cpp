//********************************************************************************
//	created:	2004/30/12 14:21:01
//	file name: 	e:\Program\test\MyAdoBase.cpp
//	file path:	e:\Program\test
//	file base:	MyAdoBase
//	file ext:	cpp
//	author:		
//
//	purpose:	��̬ADO�ֻ࣬Ҫ�Ǽ�ADO������
//********************************************************************************
#include "stdafx.h"
#include ".\myadobase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef _DEBUG
int CMyAdoBase::rsCounter = 0;
int CMyAdoBase::cnCounter = 0;
#endif // _DEBUG

/* ���徲̬��Ա���� */
//_ConnectionPtr CMyAdoBase::m_Connection = NULL;
//_ConnectionPtr CMyAdoBase::m_cnOpenPlayerBase = NULL;
//_ConnectionPtr CMyAdoBase::m_cnLoadPlayerData = NULL;

string CMyAdoBase::m_strConnectionString;

string CMyAdoBase::m_strProvider;
string CMyAdoBase::m_strDataSource;
string CMyAdoBase::m_strInitialCatalog;
string CMyAdoBase::m_strUserID;
string CMyAdoBase::m_strPassword;
string CMyAdoBase::m_strConnectTimeout;		// default 0
string CMyAdoBase::m_strIntegratedSecurity; // default SSPI

/* ���徲̬��Ա���� */
//com��ʼ�������Ӷ���ĳ�ʼ��
bool CMyAdoBase::Initialize(string strProvider, string strDataSource, string strInitialCatalog,
							string strUserID, string strPassword, string strConnectTime, 
							string strIntegratedSecurity)
{
	m_strProvider			= strProvider;
	m_strDataSource			= strDataSource;
	m_strInitialCatalog		= strInitialCatalog;
	m_strUserID				= strUserID;
	m_strPassword			= strPassword;
	m_strConnectTimeout 	= strConnectTime;
	m_strIntegratedSecurity	= strIntegratedSecurity;

	// ����Connection String
	m_strConnectionString.clear();
	//	m_strConnectionString = "Driver={SQL Server};Server=192.168.0.90,1433;Database=Bugs_GameDB;UID=sa;PWD=123";



	m_strConnectionString = "Provider=" + m_strProvider +
		"; Server=" + m_strDataSource + 
		"; Database=" + m_strInitialCatalog + 
		"; UID=" + m_strUserID + 
		"; PWD=" + m_strPassword + 
		"; OLE DB Services=-1; Driver={SQL Server}";

	CoInitialize(NULL);

//	CreateCn(m_Connection);
//	CreateCn(m_cnOpenPlayerBase);
//	CreateCn(m_cnLoadPlayerData);


//	OpenCn(m_Connection);
//	OpenCn(m_cnOpenPlayerBase);
//	OpenCn(m_cnLoadPlayerData);

	return true;
}

//�ͷ����ݿ�����
bool CMyAdoBase::Uninitalize()
{
//	CloseCn(m_Connection);
//	CloseCn(m_cnOpenPlayerBase);
//	CloseCn(m_cnLoadPlayerData);

//	ReleaseCn(m_Connection);
//	ReleaseCn(m_cnOpenPlayerBase);
//	ReleaseCn(m_cnLoadPlayerData);

	CoUninitialize();
	return true;
}

//--------------------------------------------------------------------------------
//  Function:   void CMyAdoBase::PrintErr(LPCSTR msg, _com_error &e)
//  Parameters: 
//      LPCSTR msg				��Ϣ
//		_com_error &e			�������
//  Return:     
//
//  Purpose:    
//  [2004/12/30]    
//--------------------------------------------------------------------------------
void CMyAdoBase::PrintErr(LPCSTR msg, _com_error &e)
{
	extern void AddLogText(char*, ...);
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

	AddLogText(buf);
}

void CMyAdoBase::PrintErr(LPCSTR msg)
{
	extern void AddLogText(char*, ...);
	AddLogText("Connetcion Executing : %s",msg);
}

//--------------------------------------------------------------------------------
//  Function:   bool CMyAdoBase::CreateRs(IN _RecordsetPtr rs)
//  Parameters: 
//      IN _RecordsetPtr rs 
//  Return:     
//		bool
//  Purpose:	����һ��_RecordSetPtrָ�뵽rs    
//  [2004/12/30]    
//--------------------------------------------------------------------------------
bool CMyAdoBase::CreateRs(IN _RecordsetPtr &rs)
{
	if(SUCCEEDED(rs.CreateInstance(__uuidof(Recordset)))) return true;
	return false;
}

//--------------------------------------------------------------------------------
//  Function:   bool CMyAdoBase::ReleaseRs(IN _RecordsetPtr rs)
//  Parameters: 
//      IN _RecordsetPtr rs
//  Return:     
//		bool
//  Purpose:    
//  [2004/12/30]    �ͷ�ָ��rsָ��
//--------------------------------------------------------------------------------
bool CMyAdoBase::ReleaseRs(IN _RecordsetPtr &rs)
{
	try
	{
		if(rs)
		{
			//if(rs->State == adStateOpen) rs->Close();
			CloseRs(rs);
			if(rs) rs.Release();
		}
		return true;
	}
	catch(_com_error &e)
	{
		CMyAdoBase::PrintErr("CMyAdoBase Release rs Failed", e);
		return false;
	}
}



/* ���幹�캯�� */
CMyAdoBase::CMyAdoBase()
{
}

/* ������������ */
CMyAdoBase::~CMyAdoBase(void)
{
}


//--------------------------------------------------------------------------------
//  Function:   bool CMyAdoBase::OpenCn()
//  Parameters: 
//      
//  Return:     
//		bool
//  Purpose:    �����ӣ����ã�
//  [2004/12/30]    
//--------------------------------------------------------------------------------
bool CMyAdoBase::OpenCn(_ConnectionPtr &cn)
{
	try
	{
		if(SUCCEEDED(cn->Open(m_strConnectionString.c_str(), "", "", adConnectUnspecified)))
		{
			return true;
		}
	}
	catch(_com_error &e)
	{
		PrintErr("conn opne error", e);
	}
	return false;
}

//--------------------------------------------------------------------------------
//  Function:   bool CMyAdoBase::CloseCn(_ConnectionPtr &cn)
//  Parameters: 
//      _ConnectionPtr &cn
//  Return:     
//		bool
//  Purpose:    
//  [2004/12/30]    
//--------------------------------------------------------------------------------
bool CMyAdoBase::CloseCn(_ConnectionPtr &cn)
{
	try
	{
		if(cn)
		{
			if(cn->GetState() == adStateOpen)
			{
				cn->Close();
			}
		}
		return true;
	}
	catch(_com_error &e)
	{
		PrintErr("CMyAdoBase Close cn Failed", e);
		return false;
	}
}

//--------------------------------------------------------------------------------
//  Function:   
//  Parameters: 
//      
//  Return:     
//
//  Purpose:    
//  [2005/1/13]    
//--------------------------------------------------------------------------------
bool CMyAdoBase::ExecuteCn(LPCSTR sql, _ConnectionPtr cn)
{
	//if(!ReConnect(cn)) return false;
	try
	{
		cn->Execute(sql, NULL, adCmdText);
		return true;
	}
	catch(_com_error &e)
	{
		//		char err[512];
		//		sprintf(err, "CMyadoBase Execute Failed\r\n%s\r\n", sql);
		//		PrintErr(err, e);
		PrintErr("conn execute error", e);
		PutStringToFile("sql",sql);
		return false;
	}
}

//--------------------------------------------------------------------------------
//  Function:   
//  Parameters: 
//      
//  Return:     
//
//  Purpose:    
//  [2005/1/13]    
//--------------------------------------------------------------------------------
//bool CMyAdoBase::ReConnect(IN _ConnectionPtr cn)
//{
//	try
//	{
//		if(cn->GetState() == adStateClosed)
//		{
//			if(!OpenCn(cn)) return false;
//		}
//		return true;
//	}
//	catch(_com_error &e)
//	{
//		PrintErr("CMyAdoBase ReConnection Failed", e);
//		return false;
//	}
//}


//--------------------------------------------------------------------------------
//  Function:   bool CMyAdoBase::OpenRs()
//  Parameters: 
//      
//  Return:     
//		bool
//  Purpose:    �����ӣ����ã�
//  [2004/12/30]    
//--------------------------------------------------------------------------------
bool CMyAdoBase::OpenRs(LPCSTR sql, _RecordsetPtr &rs, _ConnectionPtr cn, 
						CursorTypeEnum ct, LockTypeEnum lt, LONG op)
{
	//if(!ReConnect(cn)) return false;
	try
	{

		if(SUCCEEDED(rs->Open(_bstr_t(sql), cn.GetInterfacePtr(), ct, lt, op))) 
		{
			return true;
		}
	}
	catch(_com_error &e)
	{
		//		char err[512];
		//		sprintf(err, "CMyadoBase Open Rs Failed\r\n%s\r\n", sql);
		//		PrintErr(err, e);
		PrintErr("rs open error", e);
		PutStringToFile("sql",sql);
		return false;
	}
	return false;
}


//--------------------------------------------------------------------------------
//  Function:   bool CMyAdoBase::CloseCn(_ConnectionPtr &cn)
//  Parameters: 
//      _ConnectionPtr &cn
//  Return:     
//		bool
//  Purpose:    
//  [2004/12/30]    
//--------------------------------------------------------------------------------
bool CMyAdoBase::CloseRs(_RecordsetPtr &rs)
{
	try
	{

		if(rs->State == adStateOpen) 
		{
			rs->Close();
		}
		return true;
	}
	catch(_com_error &e)
	{
		CMyAdoBase::PrintErr("CMyAdoBase Close cn Failed", e);
		return false;
	}
}


bool CMyAdoBase::CreateCn(_ConnectionPtr &cn)
{
	if(SUCCEEDED(cn.CreateInstance(__uuidof(Connection)))) return true;
	return false;
}


void CMyAdoBase::ReleaseCn(_ConnectionPtr &cn)
{
	CloseCn(cn);
	if(cn) cn.Release();
}


//��������������
char* CMyAdoBase::FixSingleQuotes(char *sstr,char *dstr)
{
	char *tmp = dstr;
	while( *sstr )
	{
		if(*sstr == '\'')
		{
			*tmp = '\'';
			tmp++;
		}
		*tmp = *sstr;
		tmp++;
		sstr++;
	}
	*tmp = '\0';
	return dstr;
}

bool CMyAdoBase::HasSingleQuotes(const char *str)
{
	while( *str )
	{
		if( *str == '\'' )
		{
			return true;
		}
		str++;
	}
	return false;
}

//--------------------------------------------------------------------------------
// Function: GenerateGuidString(OUT char* GuidString);
// Parameters: ���ڲ��� GuidString
//
// Return:
// true
// false
// Purpose: GoodsPropertyID string = 40 (included '\0')
// Guid format:"{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}"
// [2004/12/21]
//--------------------------------------------------------------------------------
char* CMyAdoBase::GenerateGuidString(OUT char* GuidString)
{
	//if(FAILED(::CoInitialize(NULL))) return false;
	GUID guid;
	if(FAILED(CoCreateGuid(&guid))) return false;
	sprintf(GuidString,"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3, guid.Data4[0],guid.Data4[1],
		guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]);
	//::CoUninitialize();
	return GuidString;
}

char* CMyAdoBase::GetTimeString(OUT char *szTime)
{		
	SYSTEMTIME Time;
	GetLocalTime(&Time);
	sprintf(szTime,"%d-%d-%d %d:%d:%d",Time.wYear,Time.wMonth,Time.wDay,Time.wHour,Time.wMinute,Time.wSecond);
	return szTime;
}