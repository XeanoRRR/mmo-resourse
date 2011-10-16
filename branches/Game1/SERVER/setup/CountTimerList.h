#pragma once
#include <map>
#include <string>

struct tagCountTimerParam
{
	tagCountTimerParam(void)
		: m_dwType(-1)
		, m_strPicPath("")
		, m_strBackPicPath("")
		, m_strTextPath("")
		, m_dwTextTime(0)
		, m_dwTextViewTime(0)
		, m_strText("")
		, m_strTimeoutText("")
		, m_strScriptPath("")
	{

	}
	~tagCountTimerParam(void)
	{

	}

	void operator=(const tagCountTimerParam& r)
	{
		m_dwType = r.m_dwType;
		m_strPicPath = r.m_strPicPath;
		m_strBackPicPath = r.m_strBackPicPath;
		m_strTextPath = r.m_strTextPath;
		m_dwTextTime = r.m_dwTextTime;
		m_dwTextViewTime = r.m_dwTextViewTime;
		m_strText = r.m_strText;
		m_strTimeoutText = r.m_strTimeoutText;
		m_strScriptPath = r.m_strScriptPath;
	}

	DWORD m_dwType;				// ����
	std::string m_strPicPath; // �ͻ������ͼ��·��
	std::string m_strBackPicPath; // �ͻ����������ͼ��·��
	std::string m_strTextPath; // �ͻ������ֿ�ͼ��·��
	DWORD m_dwTextTime; // �ı��򵯳�ʱ���� ��λ����
	DWORD m_dwTextViewTime; // �ı�����ʾʱ��ʱ�� ��λ����
	std::string m_strText; // �ͻ��˼�ʱ���ı�������
	std::string m_strTimeoutText; // �ͻ��˼�ʱ�������ı�������
	std::string m_strScriptPath; // �ͻ��˵�����нű�·��
};

class CCountTimerList
{
public:
	CCountTimerList(void);
	~CCountTimerList(void);

	static bool LoadSetup(char* pFileName);

	static bool Serialize(std::vector<BYTE>& pCountBA);
	static bool Unserialize(BYTE* countTimerBuf, long& pos);

	static const tagCountTimerParam& GetCountTimerParam(long id);

	static DWORD GetMaxTimerNum(void);
private:
	static std::map<long, tagCountTimerParam>	s_mCountTimer;
	static DWORD m_dwMaxTimerNum;
};

static tagCountTimerParam NULL_TIMERPARAM;