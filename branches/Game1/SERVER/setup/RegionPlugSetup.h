#ifndef REGION_PLUG_SETUP_H
#define REGION_PLUG_SETUP_H

class CRegionPlugSetup
{
public:
	CRegionPlugSetup(void){}
	virtual ~CRegionPlugSetup(void){}

	struct tagRegionPlugSetup
	{
		long lID; // ��ͼID
		bool bType; // �������� 0:�ع�������,1:ָ����ͼ����
		long lRegionID; // ���͵�ͼID, ������Ϊ0
		long lX1;
		long lY1;
		long lX2;
		long lY2;
	};

	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
private:
	static	map<long, tagRegionPlugSetup>	m_mapRegionPlugSetup;		// region plug array

public:
	static map<long, tagRegionPlugSetup>& GetRegionPlugSetupMap(void);
	static BOOL LoadRegionPlugSetup(const char* szPath);			// ��Ŀ¼��ȡ
	static void Release();

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};


#endif//REGION_PLUG_SETUP_H