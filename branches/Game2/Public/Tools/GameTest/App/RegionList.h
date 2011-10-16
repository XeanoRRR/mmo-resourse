//=============================================================================
/**
*  file: CRegionList.h
*
*  Brief:��ͼ���á���Ϊxml���õ�ͼ��Ϣ
*
*  Authtor:����
*	
*  Datae:2008-2-1
*/
//=============================================================================
#pragma once

// ״̬����
class CRegionList
{
public:
	// ��ͼ����
	enum MAP_TYPE
	{
		REGIONMAP_TYPE = 0,			// ����
		AREAMAP_TYPE,				// ����
		WORLDMAP_TYPE				// ����
	};
	// �����ڵ�����ģʽ
	enum SEARCH_MODE
	{
		UNKNOWN_MODE = -1,			// ��Ч
		REGION_REGION,				// ��һ��������ĳ������ң�Դ������Ŀ�곡��������ID��ͬ��ѡ�����ַ�ʽ��
		AREA_AREA,					// ��һ�����ҵ���һ�����ҵĲ��ң�Դ������Ŀ�곡��������ID��ͬ���Ҷ���Ϊ0��ѡ�����ַ�ʽ��
		AREA_WREGION,				// ��һ�����ҵ����糡���Ĳ��ң�Դ����������ID��Ϊ0��Ŀ�곡������IDΪ0��ѡ�����ַ�ʽ��
		WREGION_AREA,				// ���糡����ĳ�����ҵĲ��ң�Դ����������IDΪ0��Ŀ�곡������ID��Ϊ0��ѡ�����ַ�ʽ��
		WREGION_WREGION,			// ���糡�������糡���Ĳ��ң�Դ������Ŀ�곡��������ID��Ϊ0��ѡ�����ַ�ʽ��
	};
	// �����
	struct tagPos
	{
		long x;
		long y;
		tagPos()
		{
			x = 0;
			y = 0;
		}
	};
	// �����еĵص���Ϣ
	struct tagAddressDetails
	{
		tagPos	stPos;
		long AddRegionId;				// �õص��Ӧ�ĳ���ID������õص��ʾ���ǳ����л���Ļ�����Ч��
		string strAddName;				// �ڵ�ǰ��ͼ����ʾ�ĵص�����
		tagAddressDetails()
		{
			AddRegionId = 0;
			strAddName = "";
		}
	};
	// �����ṹ
	struct tagRegion
	{
		long	lID;									// id
		string	strName;								// ����
		RECT	rect;									// �ڴ��ͼ�ϵ�λ�÷�Χ
		tagPos	stPos;									// �ڸ��������λ��
		long	lParentRegionID;						// ������ID
		long	lParentAreaID;							// ��������ID
		long	lParentWorldID;							// �������ͼID
		long	lIconID;								// ��ǰ����� ��ЧͼƬ ID
		vector<tagAddressDetails> vecCurAddrDetails;	// ����ô��ͼ�µ���ϸ�ص���Ϣ
		vector<RECT> vec_stRect;						// �ó����µĽ�������
		tagRegion()
		{
			lID = 0;
			lParentRegionID = 0;
			lParentAreaID = 0;
			lParentWorldID = 0;
			strName = "";
		}
	};
	// ����ṹ
	struct tagArea
	{
		long	lID;						// id
		string	strName;					// ����
		RECT	rect;						// �ڴ��ͼ�ϵ�λ�÷�Χ
		long	lParentWorldID;				// �������ͼID
		long	lBorderID;					// �ù��ı߾�ID
		map<long, tagRegion> m_mpRegions;	// ����������µ���ϸ������Ϣ
	};
	// �����ͼ�ṹ
	struct tagWorldMap
	{
		long	lID;						// �����ͼ�ı��
		string	strName;					// �����ͼ����������
		vector<tagArea>		vecArea;		// ����ͼ�µ������б�
		map<long, tagRegion> m_mpRegions;	// ����ͼ�µ�ֱ�������б�
	};
	// ����·����Ϣ����Ҫ���������糡��Ѱ·�ĸ��������ڵ����Ϣ��
	struct tagRegionPathInfo 
	{
		long lRegionID;						// �����ڵ�
		tagPos stPos;						// Ҫ����ó���������Ҫ���ڱ������¶�Ӧ�����꣨һ��ָ������ڳ������ó������л��㣩
		long lIndex;						// �õ��ڳ���·���б��е�����
		tagRegionPathInfo()
		{
			lRegionID = 0;
			lIndex = 0;
		}
	};

	//=======================================================
private:
	static long m_lSouAreaID;				// ��ʼ����
	// ����ģʽ
	static long m_lSearchMode;				// ����ģʽ
	// ���ҵĳ���·���б�
	static list<tagRegionPathInfo>	m_lsRegionPathList;
	// �����ͼ�б�
	static list<tagWorldMap>	m_lsWorldMap;
	// ��ʼ��������Ϣ
	static BOOL InitWorldInfo(tagWorldMap *stWorldMap,TiXmlElement* pElem);
	// ��ʼ�����ͼ�µ�������Ϣ
	static BOOL InitAreaInfo(tagArea *stArea,TiXmlElement* pElem);
	// ��ʼ��������Ϣ(��2��3�������ֱ��ʾ�ó�����һ������ͺ�ID)
	static BOOL InitRegionInfo(tagRegion *stRegion,MAP_TYPE lUpType,void *pUp, TiXmlElement* pElem);
	// ��ʼ�������µĽ���������Ϣ
	static BOOL InitRegionRectInfo(tagRegion *stRegion);
	// ��ӳ���·��(���һ��������ʾ����ģʽ)
	static BOOL AddRegionPath(list<tagRegionPathInfo> & lsRegionPath,tagRegionPathInfo stSourInfo,tagRegion * pTarget);

public:
	CRegionList(void);
	virtual ~CRegionList(void);

	// ������ͼ�����ļ�
	static BOOL LoadMap();
	// ��ò��ҵĳ���·���б�
	static list<tagRegionPathInfo> & GetRegionPathList()	{return m_lsRegionPathList;}
	// ���ָ����ŵ������ͼ
	static tagWorldMap * GetWorldMap(long lWorldMapID);
	// ���ָ����ŵ�����
	static tagArea * GetArea(long lAreaID);
	// ���ָ����ŵĳ���
	static tagRegion* GetRegion(long lRegionID);
	// ����ָ��������Ŀ�곡���ĳ���·��
	static BOOL FindRegionPath(long lSourID,long lTargetID);
	//=======================================================
};



/******************************************************************************
*  file: CMailBoxList.h
*
*  Brief:�����е���Ͳ���ã���ʾ�˸��������е���Ͳλ��
*
*  Authtor:����
*	
*  Datae:2009-3-7
******************************************************************************/

class CMailBoxPosList
{
public:
	// �����
	struct tagPos
	{
		long x;
		long y;
		tagPos()
		{
			x = 0;
			y = 0;
		}
	};
	// �����е�����ṹ
	struct tagMailBoxPos
	{
		long					lResourceID;				// ������ԴID
		string					strName;					// ��������
		vector<tagPos>			vecPos;						// ���������б�
	};

	CMailBoxPosList(void);
	virtual ~CMailBoxPosList(void);

	// ������Ͳ���������ļ�
	static BOOL LoadMailBoxPos();
	// �ͷ���Ͳ���������ļ�
	static void ReleaseMailBoxPos();
	// ���ָ�������У���ָ�������������������
	// ����1��������ԴID������2������3��ָ��������
	static void GetMailBoxPos(long lResourceID,long sx,long sy,long& dx,long& dy);

private:
	// ���г���������
	static map<long , tagMailBoxPos>			m_mapMailBoxPos;
	// ��ʼ��������ͼ�µ���������
	static BOOL InitMailPos(tagMailBoxPos& stMailBoxPos,TiXmlElement* pElem);
};