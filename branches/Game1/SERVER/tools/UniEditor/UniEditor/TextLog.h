#pragma once
#include <fstream>

using namespace std;

class CTextLog
{
public:
	CTextLog(void);
public:
	~CTextLog(void);

	const inline SYSTEMTIME GetStartTime() {return m_sysTime;}

// ������־
public:
	static bool AddLog(const CString &str);
	static bool WriteLog(const CString &str);
protected:
	static ofstream	m_ofs;
	static SYSTEMTIME	m_sysTime;
};
