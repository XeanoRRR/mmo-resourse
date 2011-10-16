//======================================================================================
/**
*	@file		RegionSetup.h
*	@data		
*	@author		
*/
//======================================================================================
#pragma once


/**
*	@class CRegionSetup
*
*	@brief ��������������
*/

class CRegionSetup
{
public:
	/// ��������ṹ
	struct tagRegionSetup
	{
		long lID;						// ��ͼID
		long lCanEnterLevel;			// �ܹ�����ĵȼ�
		char pszEnterScript[MAX_PATH];	//���볡��ִ�нű�
		char pszExitScript[MAX_PATH];	//�˳�����ִ�нű�
	};

private:
	/// ������������
	static map<long,tagRegionSetup> s_mapRegionSetup;

public:
	/// ����
	CRegionSetup();

	/// ����
	~CRegionSetup();

	/// ���ļ����س�����������
	static bool LoadRegionSetup(const char* filename);

	/// ȡ�ó�����������
	static tagRegionSetup*	GetProperty(long lIndex)	{return (s_mapRegionSetup.find(lIndex)!=s_mapRegionSetup.end())?(&s_mapRegionSetup[lIndex]):NULL;}

	/// ȡ�ý��볡���ĵȼ�����
	static long GetEnterRegionLvl(long lRegionID);

	/// ȡ����볡��ִ�еĽű�
	static const char* GetEnterRegionScript(long lRegionID);

	/// ȡ���˳�����ִ�еĽű�
	static const char* GetExitRegionScript(long lRegionID);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);	
};