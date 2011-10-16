#pragma once

class CRegionSetup
{
public:
	struct tagRegionSetup
	{
		long lID;					// ��ͼID
		long lCanEnterLevel;		// �ܹ�����ĵȼ�
		char pszEnterScript[MAX_PATH];//���볡��ִ�нű�
		char pszExitScript[MAX_PATH];//�˳�����ִ�нű�
	};

private:
	static map<long,tagRegionSetup> s_mapRegionSetup;

public:
	
	CRegionSetup();
	~CRegionSetup();	 
	static bool LoadRegionSetup(const char* filename);
	static tagRegionSetup*	GetProperty(long lIndex)	{return (s_mapRegionSetup.find(lIndex)!=s_mapRegionSetup.end())?(&s_mapRegionSetup[lIndex]):NULL;}

	//���볡���ĵȼ�����
	static long GetEnterRegionLvl(long lRegionID);
	//���볡��ִ�еĽű�
	static const char* GetEnterRegionScript(long lRegionID);
	//�˳�����ִ�еĽű�
	static const char* GetExitRegionScript(long lRegionID);

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};