//======================================================================================
/**
*	@file		RegionPlugSetup.h
*	@data		
*	@author		
*/
//======================================================================================
#pragma once


/**
*	@class CRegionPlugSetup
*
*	@brief 
*/
class CRegionPlugSetup
{
public:
	/// ����
	CRegionPlugSetup(void){}

	/// ����
	~CRegionPlugSetup(void){}

	/// region plug�ṹ
	struct tagRegionPlugSetup
	{
		long lID;				// ��ͼID
		bool bType;				// �������� 0:�ع�������,1:ָ����ͼ����
		long lRegionID;			// ���͵�ͼID, ������Ϊ0
		long lX1;
		long lY1;
		long lX2;
		long lY2;
	};

private:
	/// region plug����
	static	map<long, tagRegionPlugSetup>	m_mapRegionPlugSetup;

public:
	/// ȡ��region plug ����
	static map<long, tagRegionPlugSetup>& GetRegionPlugSetupMap(void);

	/// ���ļ�����region plug����
	static bool LoadRegionPlugSetup(const char* szPath);

	/// ����
	static void Release();

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);	
};
