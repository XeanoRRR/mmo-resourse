#pragma once

enum eGMLevel
{
	GM_GOD = 200,		//����
	GM_ADMIN = 100,		//��߼�����Ա,����ִ��ϵͳ������
	GM_ARCH = 90,		//����������ִ�й��������GM
	GM_WIZARD = 50,		//��ͨGM,ֻ��ִ����ʱ�Ը���(�������)��GM
	GM_PLAYER = 0		//Ĭ�ϵȼ�(��GM)
};				//GM�ȼ�


//GM��Ϣ
struct tagGMInfo
{
	string		strName;
	eGMLevel	lLevel;
	
	tagGMInfo()
	{
		lLevel=GM_PLAYER;
	}
	tagGMInfo(const char* name, eGMLevel level)
	{
		strName = name;
		lLevel = level;
	}
};	//GM��Ϣ(����,�ȼ�)


//GM�б�
class CGMList
{
protected:
	static map<string, tagGMInfo>		m_mapGMInfo;	// map<name, tagGMInfo>

public:
	static bool Load(char* filename);	// ����

	static tagGMInfo* GetInfoByName(const char* name);	//��GM���ֶ�ȡ����Ϣ
	static inline int count()		{return (int)m_mapGMInfo.size();}

	static void Clear();

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

	static map<string, tagGMInfo>* GetGMInfoMap()	{return &m_mapGMInfo;}

	CGMList(void);
	~CGMList(void);

	static string s_strGodPassport;		// ����У��
};
