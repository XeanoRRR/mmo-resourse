#pragma once

/// GM�ȼ�
enum eGMLevel
{
	GM_GOD = 200,		//����
	GM_ADMIN = 100,		//��߼�����Ա,����ִ��ϵͳ������
	GM_ARCH = 90,		//����������ִ�й��������GM
	GM_WIZARD = 50,		//��ͨGM,ֻ��ִ����ʱ�Ը���(�������)��GM
	GM_PLAYER = 0		//Ĭ�ϵȼ�(��GM)
};

/// GM��Ϣ(����,�ȼ�)
struct tagGMInfo
{
	string		strName;
	eGMLevel	lLevel;
	
	tagGMInfo()
	{
		lLevel = GM_PLAYER;
	}
	tagGMInfo(const char* name, eGMLevel level)
	{
		strName = name;
		lLevel = level;
	}
};


/**
*	@class GMList
*
*	@brief GM�б���
*/
class GMList
{
public:
	/// ����
	GMList(void);

	/// ����
	~GMList(void);

private:
	/// GM�б� map<name, tagGMInfo>
	static map<string, tagGMInfo> m_mapGMInfo;

public:
	/// ���ļ�����GM�б�
	static bool Load(char* filename);

	/// ��GM���ֶ�ȡ����Ϣ
	static tagGMInfo* GetInfoByName(const char* name);

	/// ����
	static void Clear();

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ȡ��GM�б�
	static map<string, tagGMInfo>* GetGMInfoMap()	{return &m_mapGMInfo;}

	/// ����У��
	static string s_strGodPassport;
};
