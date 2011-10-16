#pragma once


#include "LogDef.h"

class CBaseMessage;

/////////////////////////////////////////////////////////////////////////////////
//!���س�ʼ����Ϣ
struct  LogClientInfo
{
	long		lClientNo;
	
	string		LogFileName;		//! ������־��¼�ļ�����������ʽ��
	string		TxtLogFileName;		//!	������־��¼�ļ����ı���ʽ��
	string		ErrLogFileName;		//! �����¼�ļ�
};



/////////////////////////////////////////////////////////////////////////////////
class LogClient
{
public:
	LogClient(void);
	~LogClient(void);

public:
	//!						
	bool						Init(CBaseMessage *pMsg, LogClientInfo &logClientInfo);

	//!							���¿�������
	bool						UpdateCondition(CBaseMessage *pMsg);

	//!							�����Ƿ���LOGSERVERȷ��
	void						PassAffirm(bool bAffirm);

	void						SetSoket(long lLogSocket);
	long						GetSoket(void);
	
	void						Release(void);

public:
	//!							������־�������õ����������������
	//!							����ֻ����Long��float*��char*��CGUID*��������
	//!							ʱ��ͷ����������Ĭ�ϵ������
	bool						SendLogToServer(long LogType, ...);
	//!							����SQL�����־
	bool						SendStrSqlLogToServer(const char *pSql);

	//!							������־�ı���ģʽ
	bool						SetSaveMode(long lLogType, eSendControl eSC);

	//!							�ڱ��ؼ�¼�ı���ʽ��־
	bool						LogTxtToHere(const char *pTxt);

private:
	//!							�ڱ��ؼ�¼��������ʽ��־
	bool						LogToHere(long LogType, ...);

	//!							�������л�ñ�ṹ
	void						ByteArrayToTable(uchar *pData, long pos);

	//!							����־����ѹ��uchar����
	bool						GetLogByteArray(va_list& args, LogTable &tableInfo, vector<uchar> &vData);
	//!							������Ĳ���ֵ������ѹ��uchar����
	//!							����args������va_start֮���
	//!							����lType����ΪeDataTypeCode
	bool						ParamToByteArray(va_list& args, const LogItem& logItem, vector<uchar> &vData);
	//!							ͬ�ϣ���Ҫ���и����������
	//!							RE: -1��ִ��ʧ�ܣ� 0�����������ʧ�ܣ�1�����ͨ��
	long						ParamToByteArray_AndTestCondition(va_list& args, const LogItem& logItem, vector<uchar> &vData);

	/*
	//!							����SQL���
	//!							����args������va_start֮���
	bool						GetSQL(va_list& args, LogTable &tableInfo, string &strValue);
	//!							������������ʹӲ����л��һ��DB��ʽ��ֵ���ַ������Զ���ӵ����ţ�
	//!							����args������va_start֮���
	//!							����lType����ΪeDataTypeCode
	bool						GetValueByType_WithDB(va_list& args, const LogItem& logItem, string &strValue);
	//!							������������ʹӲ����л��һ��DB��ʽ��ֵ���ַ�������ִ�й����н���������⣨�Զ���ӵ����ţ�
	bool						GetValueByType_AndTestCondition_WithDB(va_list& args, const LogItem& logItem, string &strValue);
	*/

	//!							����ɸѡ����
	long						TestCondition(const ItemCondition &condition, void *pCurrValue);

	//!							�ļ�д���߳�
	static	unsigned __stdcall	FileWirteThread(void* pThis);


public:
	//!								�����߳�
	HANDLE							hConnectThread;
	bool							bConnectThreadExit;

private:
	
	//------------------------------------------------------------------------
	//!	֧�ֶ��߳�����ļ����ݵķ�װ�ṹ
	struct LogFile_ForThread
	{
		CRITICAL_SECTION	_critical_section;	//! �ļ�����д��ͬ������

		string				_str_buf;			//! д�뻺��
		bool				_buf_is_null;		//! ����Ϊ�ձ��

		string				_file_name;			//! �ļ�����
		ofstream			_ofstream;			//! �ļ�д����

		//!					ͬ���Ľ��ı�����д�뻺��
		void				WirteToBuf(string &strTxt);

		//!					����������д���ļ������ڵ��̣߳�
		long				BufToFile(void);

	private:
		//!					���ļ���
		bool				OpenStream(void);
	};
	
	//!									д���ļ��¼�
	HANDLE								m_hEvent;
	//!									д���߳�
	HANDLE								m_hWirteThread;
	//!
	long								m_lWirteThreadExit;

	//!									����Logserver�Ŀͻ���
	long								m_LogSocket;

	//!									���ͻ��˱��
	long								m_lClientNo;

	//!									�����¼�ļ�
	LogFile_ForThread					m_ErrFile;
	//!									������־��¼��DB��ʽ��
	LogFile_ForThread					m_LogFile;
	//!									������־��¼�����ĸ�ʽ��
	LogFile_ForThread					m_TxtLogFile;	

	//!									������ñ��
	bool								m_thisUsable;
	//!									����LogServerȷ��
	bool								m_BeAffirmed;

private:

	//!									���Ʊ�Ƕ�Ӧ���ݱ�ṹ
	TableMap_byType						m_TableInfo;

	vector<ItemCondition>				m_vItemCondition;

};