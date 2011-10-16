#ifndef _DUPLIREGIONSSETUP_H_
#define _DUPLIREGIONSSETUP_H_


class CDupliRegionSetup
{
	struct tagDupliRegion
	{
		long	lRegionID;		//ԭʼID
		long	lDupRegionID;	//�����ĵ�ͼID
	};
public:
	CDupliRegionSetup(void);
	~CDupliRegionSetup(void);

	bool Load();
	bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

	bool IsDupliRegion(long lRegionID,long lDupliRegionID);

	//WorldServerʹ��,��ԭʼ�����͸�������������õ�һ����
	long GetRandomRegion(long lRegionID);
private:

	typedef list<tagDupliRegion>::iterator itDR;
	list<tagDupliRegion>	m_DupliRegions;
};

#endif
