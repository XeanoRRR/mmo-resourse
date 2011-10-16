#ifndef _INC_MYADOBASE
#define _INC_MYADOBASE
#pragma once

#import "msado15.dll" no_namespace rename("EOF","adoEOF")
#include <string>
using namespace std;

class CMyAdoBase
{
public:
	// ��ʼ#
	static bool Initialize(string strProvider, 
		string strDataSource, 
		string strInitialCatalog,
		string strUserID, 
		string strPassword, 
		string strConnectTime = string("0"), 
		string strIntegratedSecurity = string("SSPI"));

	// �ͷ�#
	static bool Uninitalize();

	/*
	* Exception
	*/

	// ���Ժ��� �����쳣���׳�#
	static void TESTHR (bool _b) { if(!_b) _com_issue_error (E_FAIL);} 
	static void TESTHR (HRESULT _hr){ if(FAILED(_hr)) _com_issue_error (E_FAIL);}
	static void TESTHR (BOOL _b) {if(!_b) _com_issue_error(E_FAIL);}

public:

	/*
	* Connection object
	*/
//	static _ConnectionPtr m_Connection;
//	static _ConnectionPtr m_cnOpenPlayerBase;
//	static _ConnectionPtr m_cnLoadPlayerData;

	// ����һ�����Ӷ���
	static bool CreateCn(_ConnectionPtr &cn);
	// �ͷ�����
	static void ReleaseCn(_ConnectionPtr &cn);
	// ������
	static bool OpenCn(_ConnectionPtr &cn);
	// �ر�����
	static bool CloseCn(_ConnectionPtr &cn);

	static bool ExecuteCn(LPCSTR sql, _ConnectionPtr cn);

	/* Construction */
	CMyAdoBase();

	/* Destruction */
	virtual ~CMyAdoBase(void);

	/*
	 * Recordset object
	 */

	// �򿪼�¼����
	static bool OpenRs(LPCSTR sql, _RecordsetPtr &rs, _ConnectionPtr cn,
		CursorTypeEnum ct = adOpenForwardOnly,
		LockTypeEnum lt = adLockReadOnly,
		LONG op = adCmdText);
	// ������¼����#
	static bool CreateRs(IN _RecordsetPtr &rs);
	// �ͷż�¼����#
	static bool ReleaseRs(IN _RecordsetPtr &rs);
	// �رռ�¼����
	static bool CloseRs(_RecordsetPtr &rs);
	

	//��������������
	static char* FixSingleQuotes(char *sstr, char *dstr);


	// ����һ��GUID. Format: "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}"  40λ�ַ�
	static char *GenerateGuidString(OUT char* GuidString);

	static char *GetTimeString(OUT char *szTime);

	static bool HasSingleQuotes(const char *str);
protected:

	// �����ݿ����Ӷ���
	//static _ConnectionPtr m_cn;

	// Connection String
	static string m_strConnectionString;


	// ������ӶϿ����������ӡ�
	//static bool ReConnect(IN _ConnectionPtr cn = NULL);

	/*
	 * Print error
	 */

	// ��������#
	static void PrintErr(LPCSTR msg, _com_error &e);
	static void PrintErr(LPCSTR msg);
private:

	/*
	 * Connetion string members
	 */

	static string m_strProvider;
	static string m_strDataSource;
	static string m_strInitialCatalog;
	static string m_strUserID;
	static string m_strPassword;
	static string m_strConnectTimeout;
	static string m_strIntegratedSecurity; // SSPI

#ifdef _DEBUG
	static int rsCounter;
	static int cnCounter;
#endif // _DEBUG
	

};

#endif // _INC_MYADOBASE