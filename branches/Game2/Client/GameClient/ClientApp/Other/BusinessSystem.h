#pragma once

class CCreditSystem;
class CBusinessSystem 
{
public:
	// �����ȼ��ṹ
	struct tagCreditLevel
	{
		long lMinNum;		//��������
		long lMaxNum;		//��������
		char LevelName[64];	//�ȼ�����
	};

	//ó����
	struct tagBusinessZone
	{
		string						strZoneName;			//ó��������
		WORD						wZoneID;				//ó����ID
		WORD						wOccupyCountryID;		//����ռ���ID
		map<WORD/*�����ҵ�ID*/,DWORD/*����ֵ*/>		map_CountryCredit;		//ÿ�������ڴ�ó����������
		vector<WORD>				vec_PointID;			//��Ŵ�ó�����¸�ó�׵��ID
	};

	//ó�׵�
	struct tagBusinessPoint
	{
		string			strPointName;	   					//(ó�׵������)
		WORD			wPointID;		   					//(ó�׵��ID)
		WORD			wSuperiorZoneID; 					//(������ó������ID)
		WORD			wBusinessAdscriptionID;  			//(��ҵ�����Ĺ���ID)
		map<WORD/*�����ҵ�ID*/,DWORD/*Ͷ���ܶ�*/>	map_CountryInvest;		//(ÿ�������ڴ�ó�׵��Ͷ���ܶ�)
	};

	//ĳ����Ʒ��ǰ��NPC�ļ۸���Ϣ
	struct  tagTradeGoodsInfor
	{
		DWORD dwGoodsIndex;									//��Ʒ��ID
		DWORD dwPrice;										//��ǰ�����۸�
		int PriceScale;										// ��۱� PriceScale%
	};

	//һ��Ͷ���е���Ϣ
	struct tagInvestInfor 
	{
		CGUID NpcGUID;										//�����NPC��GUID
		DWORD dwPointID;									//�˴ν���Ͷ�ʵ�ó�׵��ID
		DWORD dwValue;										//�˴�Ͷ�ʵ�����
		DWORD dwMaxValue;									//�˴�Ͷ�ʵ�����
		tagInvestInfor()
		{
			NpcGUID = CGUID::GUID_INVALID;
			dwPointID = 0;
			dwValue = 0;
			dwMaxValue = 0;
		}
	};

	//��������
	struct tagAreaCredit
	{
		int id;
		DWORD dwAreaId;
		char chName[100];
		tagAreaCredit()
		{
			id=0;
			dwAreaId=0;
		}
	};

	CBusinessSystem();
	~CBusinessSystem() {}

	static map<WORD,CBusinessSystem::tagBusinessZone>* GetAllZonesInfor() {return &m_mapBusinessZones;}
	static map<WORD,CBusinessSystem::tagBusinessPoint>* GetAllPointInfor(){return &m_mapBusinessPoint;}
	static tagBusinessZone* GetZoneInfor(WORD wZoneID);
	static tagBusinessPoint* GetPointInfor(WORD wPointID);
	static bool CreateBusinessSystem();
	//static bool InitBusinessSystem();
	static void Release();
	static bool ResetSystem();
	static void UpdateZoneData(WORD wZoneID);
	static void UpdatePointData(WORD wPointID);
	static map<DWORD,tagTradeGoodsInfor>* GetTradeGoodsInforList() { return &m_mapTradeGoods; }
	static tagInvestInfor* GetInvestInfor()		{ return &m_tagInvest; }
	
	static bool InitCreditLevel(const char* filename);								//��ȡ�����ļ��е������ȼ�����Ϣ
	static char* GetCreditLevelName(long lCurCreditValue);							//���ݴ��������ֵ��ȡ�����ȼ�����
	static long GetMaxCreditValue(long lCurCreditValue);							//���ݴ��������ֵ��ȡ��ǰ�ȼ����������ֵ
	static long GetCurCreditLevel(long lCurCreditValue);							//���ݴ��������ֵ��ȡ��ǰ�ȼ�
private:
	static map<WORD/*��ó������ID*/,tagBusinessZone>  m_mapBusinessZones;			//��ó��������
	static map<WORD/*��ó�׵��ID*/,tagBusinessPoint> m_mapBusinessPoint;			//��ó�׵�����
	static CBusinessSystem* m_pBusinessSystem;										//Ψһ��BusinessSystem�����ָ��
	static map<DWORD,tagTradeGoodsInfor> m_mapTradeGoods;							//��ǰ������Ʒ��ĳ��NPC�ļ۸��
	static tagInvestInfor m_tagInvest;												//�����ĳ��ó�׵��һ��Ͷ����Ϣ
	
	static vector<tagCreditLevel> m_vecCreditLevel;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	static size_t  GetAreaNumSize();                                                 //������������������ 
	static size_t  GetOrganNumSize();                                                 //�����������֯������ 
	static bool InitCreditSystem();
	static long GetOrgeCreditMaxValue(DWORD dwOrganID,DWORD dwValue);                   //���ݴ������֯ID �Ͷ�Ӧ�������ȼ����ص�ǰ�ȼ���������ֵ����
	static const char* CreditOrganName(DWORD dwOrganID,DWORD dwValue);                //��ȡָ����֯��ָ������ֵ ����Ӧ������
	static long CreditOrganLevel(DWORD dwOrganID,DWORD dwValue);                       //���ݴ������֯id������ֵ�õ���Ӧ�������ȼ�
	static long MaxAreaValue(DWORD dwValue);                                          //��ȡָ���ȼ���������������
	static long AreaLevel(DWORD dwValue);                                          //��ȡָ��ֵ��ȡ�����ĵȼ�
	static const char* AreaCreditName(DWORD dwValue);                                 //��������ֵ��ȡ��ǰ��������ʾ��
	static const char* GetAreaName(int id);                                        //���ݴ��������id���Ҹ��������ʾ��
	static void ReleaseCreditSystem();                                        //�ͷ�����ϵͳ��Դ
	static long GetMaxCreditByOrgan(long Id);                                  //����һ����֯���������ֵ
	static long MaxAreaCredit();                                               //��ȡ�������������ֵ
	static long GetMaxCreditLevelByOrgan(long id);                                    //���һ����֯����������ȼ�
	static long MaxAreaCreditLevel();                                          //����������������ȼ�
	static const char* GetOrganName(DWORD dwOrganID);							//������֯������ID��ȡ������������
	static const char* GetDirtyName(DWORD dwOrganID);							//������֯������ID��ȡ������������
private:
	static map<int,tagAreaCredit> m_mapCreditArea;                                           //��¼������ID��Ӧ�ĵ�ͼid
	static long m_lAreaNumSize;                                                      //�����ļ���������������������������б�
	static CCreditSystem  *m_pCreditSystem;                                            //�������� 
};	