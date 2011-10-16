#ifndef _DUPLIREGIONSSETUP_H_
#define _DUPLIREGIONSSETUP_H_


class CDupliRegionSetup
{
	struct tagDupliRegion
	{
		long	lRegionID;		//ԭʼID
		long	lDupRegionID;	//�����ĵ�ͼID
	}; 
    
    typedef list<tagDupliRegion>::iterator itDR;

public:

	CDupliRegionSetup(void);
	~CDupliRegionSetup(void);

	bool Load();
    
    /// ��ӵ�CByteArray
	bool AddToByteArray(vector<BYTE>* pByteArray);		
    
    /// ����
	bool DecordFromByteArray(BYTE* pByte, long& pos);	

	bool IsDupliRegion(long lRegionID,long lDupliRegionID);

	//WorldServerʹ��,��ԭʼ�����͸�������������õ�һ����
	long GetRandomRegion(long lRegionID);

private:

    list<tagDupliRegion>	m_DupliRegions;
};

#endif
