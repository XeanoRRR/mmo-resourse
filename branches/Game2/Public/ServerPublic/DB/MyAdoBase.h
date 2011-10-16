#ifndef _INC_MYADOBASE
#define _INC_MYADOBASE
#pragma once

#import "msado15.dll" no_namespace rename("EOF","adoEOF")

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

	static bool Initialize(void);
	// �ͷ�#
	static bool Uninitalize();

	/*
	* Exception
	*/

	// ���Ժ��� �����쳣���׳�#
	static void TESTHR (bool _b) { if(!_b) _com_issue_error (E_FAIL);} 
	static void TESTHR (HRESULT _hr){ if(FAILED(_hr)) _com_issue_error (E_FAIL);}

public:

	/*
	* Connection object
	*/

	// ����һ�����Ӷ���
	static long CreateCn(_ConnectionPtr &cn);
	// �ͷ�����
	static long ReleaseCn(_ConnectionPtr &cn);
	// ������
	static long OpenCn(_ConnectionPtr &cn);
	static long OpenCn(_ConnectionPtr &cn, string strSQL);
	// �ر�����
	static long CloseCn(_ConnectionPtr &cn);

	static long ExecuteCn(LPCSTR sql, _ConnectionPtr& cn);

	/* Construction */
	CMyAdoBase();

	/* Destruction */
	virtual ~CMyAdoBase(void);

	/*
	 * Recordset object
	 */

	// �򿪼�¼����
	static long OpenRs(LPCSTR sql, _RecordsetPtr &rs, _ConnectionPtr& cn,
		CursorTypeEnum ct = adOpenStatic,
		LockTypeEnum lt = adLockReadOnly,
		long op = adCmdText);
	// ������¼����#
	static long CreateRs(IN _RecordsetPtr &rs);
	// �ͷż�¼����#
	static long ReleaseRs(IN _RecordsetPtr &rs);
	// �رռ�¼����
	static long CloseRs(_RecordsetPtr &rs);
	

	//��������������
	static char* FixSingleQuotes(char *sstr, char *dstr);


	// ����һ��GUID. Format: "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}"  40λ�ַ�
	static char *GenerateGuidString(OUT char* GuidString);

	static char *GetTimeString(OUT char *szTime);

	static bool HasSingleQuotes(const char *str);

	// ��������#
	static void PrintErr(LPCSTR msg, _com_error &e);
	static void PrintErr(LPCSTR msg);
protected:

	// �����ݿ����Ӷ���
	//static _ConnectionPtr m_cn;

	// Connection String
	static string m_strConnectionString;


	// ������ӶϿ����������ӡ�
	//static bool ReConnect(IN _ConnectionPtr& cn = NULL);

	/*
	 * Print error
	 */
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