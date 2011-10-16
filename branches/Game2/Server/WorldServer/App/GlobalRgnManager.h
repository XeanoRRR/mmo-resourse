//GlobalRgnManager.h/////////////////////////////////////////////////////////////////////
//������:�ö���������еĳ�������ʵ��
//Author:������
//Create Time:2008.11.03

#ifndef GLOBAL_RGN_MANAGER
#define GLOBAL_RGN_MANAGER

class CWorldRegion;

class CGlobalRgnManager
{
public:

	//ʹ��Region GUID����
	typedef std::map<CGUID/*RgnID*/, CWorldRegion*> MapRgn;
	typedef MapRgn::iterator MapRgnItr;

	CGlobalRgnManager();
	~CGlobalRgnManager();


	//! ��ȡIni�����ļ�����,����������ģ�����map
	bool LoadRgnSetupIni(const char* strFilePath);
	// ��ȡ�����������ȷ����������ļ�
	bool LoadDupRgnServerIni(const char* filePath);
	// ��ȡRegionPlugSetup����
	BOOL LoadRegionPlugSetupIni(const char* szPath);
	//! ����ָ��GS��Rgn
	bool CreateGSRgn(long templateRgnID, eRgnType rgnType, const CGUID& ownerID, const CGUID& scriptID/*����Ľű�ID*/, long pMsgGSID, const char* szVarName, const char* szRgnIDName, bool isDupRgnDataFlag);
	//! ɾ��ָ��GS��Rgn
	bool DeleteGSRgn(eRgnType rgnType, const CGUID& ownerID, const CGUID& rgnID, const CGUID& scriptID/*�ű�ID*/, long pMsgGSID, const char* szVarName, const char* szRgnIDName);
	//! ����Region
	CWorldRegion* FindRgnByGUID(const CGUID& guid);
	//! ��װ���߳�������
	bool AddLinedRgnInfoToByteArray(vector<BYTE>& pBA);
	//! ���㵱ǰGS�����͸����Ƿ�ﵽ����,������ЧGS��gsid
	long GetCurGsDupRgnNumIsMax(eRgnType rgnType);

	// ��Ҹ������ݲ����ӿ�
	//!					���һ����Ҷ��鸱��GUID
	void				AddTeamRgnGUID(const CGUID& ownerGuid, LONG lTemplateRgnID, const CGUID& rgnGuid, BOOL bCover);
	//!					ɾ��һ����Ҷ��鸱��GUID
	void				DelTeamRgnGUID(const CGUID& ownerGuid, LONG lTemplateRgnID);
	void				DelTeamRgnGUID(const CGUID& ownerGuid, const CGUID& rgnGuid);
	//!					���һ����ҵĶ��鸱��
	void				ClearTeamRgnGUID(const CGUID& ownerGuid);
	//!					�õ�һ�����鸱����GUID
	const	CGUID&		GetTeamRgnGUID(const CGUID& ownerGuid, LONG lTemplateRgnID);
	//!					ͨ��˳��õ�һ����ҵĶ���ĸ���GUID
	const	CGUID&		GetTeamRgnGuidByIndex(const CGUID& ownerGuid, LONG lIndex);
	//!					ͨ��˳��õ�һ����ҵĶ���ĸ���ģ��ID
	LONG				GetTeamRgnTemplateIdByIndex(const CGUID& ownerGuid, LONG lIndex);
	//!					�õ���ҵĶ��鸱��GUID�б�
	map<LONG, CGUID>*	GetTeamRgnGuidMap(const CGUID& ownerGuid);//{return m_mapTeamRgnGUID;}
	//!					��ҵĶ��鸱��GUID����
	VOID				RgnGuidCode(const CGUID& ownerGuid, vector<BYTE> &vData);
	//!					��ҵĶ��鸱��GUID����
	VOID				RgnGuidDecode(const CGUID& ownerGuid, BYTE *pData, LONG &lPos);
	//!					���뽻����ҵĶ��鸱������������ͬģ��ĸ�����
	VOID				RgnGuidSwapFromArray(const CGUID& ownerGuid, BYTE *pData, LONG &lPos);

	//!					��ҵĶ��鸱��GUID����
	VOID				RgnGuidCode(const CGUID& ownerGuid, DBWriteSet& setWriteDB);
	//!					��ҵĶ��鸱��GUID����
	VOID				RgnGuidDecode(const CGUID& ownerGuid, DBReadSet& setReadDB);
	//!					���뽻����ҵĶ��鸱������������ͬģ��ĸ�����
	VOID				RgnGuidSwapFromArray(const CGUID& ownerGuid, DBReadSet& setReadDB);


	//!					�õ���ҵĸ��˸���GUID�б�
	map<CGUID, CGUID>&	GetPersonalRgnGuidMap(const CGUID& ownerGuid);
	void				AddPersonalRgnGUID(const CGUID& ownerGuid, const CGUID& rgnGuid);
	//!					ɾ��һ����Ҹ��˸���GUID
	void				DelPersonalRgnGUID(const CGUID& ownerGuid);
	//!					�õ�һ�����˸�����GUID
	const	CGUID&		GetPersonalRgnGUID(const CGUID& ownerGuid);
public:
	// ����������Ϣ
	struct tagRegion
	{
		CWorldRegion	*pRegion;			// ����ָ��
		DWORD			dwGameServerIndex;	// ����Gameserver���
		eRgnType		RegionType;			// ��������
	};

public:
	// ���ݵ�ͼID�õ�tagRegionָ��
	tagRegion* GetTemplateNormalRegion(long lID/*ģ��ID*/)				{return (m_mapRegionList.find(lID)!=m_mapRegionList.end())?&m_mapRegionList[lID]:NULL;}
	//���ݵ�ͼ���ֵõ�tagRegionָ��
	tagRegion* GetTemplateNormalRegion(const char* strName);
	// ���ݵ�ͼID�õ�tagRegionָ��
	tagRegion* GetTemplatePersonalRegion(long lID/*ģ��ID*/)		{return (m_mapPersonalRgnList.find(lID)!=m_mapPersonalRgnList.end())?&m_mapPersonalRgnList[lID]:NULL;}
	// ���ݵ�ͼID�õ�tagRegionָ��
	tagRegion* GetTemplatePersonalHouseRegion(long lID/*ģ��ID*/)	{return (m_mapPersonalHouseRgnList.find(lID)!=m_mapPersonalHouseRgnList.end())?&m_mapPersonalHouseRgnList[lID]:NULL;}
	// ���ݵ�ͼID�õ�tagRegionָ��
	tagRegion* GetTemplateTeamRegion(long lID/*ģ��ID*/)		{return (m_mapTeamRgnList.find(lID)!=m_mapTeamRgnList.end())?&m_mapTeamRgnList[lID]:NULL;}

	// ȡ��Normal Rgnģ�峡���б�
	map<long, tagRegion>* GetTemplateRegionListMap()	{return &m_mapRegionList;}
	// ȡ��Personal Rgn�����б�
	map<long, tagRegion>* GetTemplatePersonalRgnListMap()	{return &m_mapPersonalRgnList;}
	// ȡ��Personal House Rgn�����б�
	map<long, tagRegion>* GetTemplatePersonalHouseRgnListMap()	{return &m_mapPersonalHouseRgnList;}
	// ȡ��Team Rgn�����б�
	map<long, tagRegion>* GetTemplateTeamRgnListMap()	{return &m_mapTeamRgnList;}

	MapRgn& GetRgnMap() { return m_mapRgn; };
	
	//WorldServerʹ��,��ԭʼ�����͸�������������õ�һ����
	long GetRandomRegion(long RegionID);

	CWorldRegion* GetWorldRegionByRgnId(long lId);

	//! ����GSID by RgnGUID
	long FindGSIDByRgnID(const CGUID& ownerID);

	// ���˷��ݸ����������ȷ�����
	vector<long>&	GetFirstPersonalHouseServer(void) { return m_vecFirstPersonalHouseServer; }
	// ���˸����������ȷ�����
	vector<long>&	GetFirstPersonalServer(void) { return m_vecFirstPersonalServer; }
	// ��Ӹ����������ȷ�����
	vector<long>&	GetFirstTeamServer(void) { return m_vecFirstTeamServer; }

	//! ����Rgn,��GS����RGN_CREATE��Ϣ
	CWorldRegion* CreateRgn(bool isDupRgnDataFlag,
		long scrGSID								/*���ʹ��������GSID*/,
		long templateRgnID							/*ģ��Region��ID*/,
		eRgnType rgnType, long GSID, 
		const CGUID& ownerID=NULL_GUID				/*NORMAL������IDΪ��*/,
		const CGUID& scriptID=NULL_GUID,
		const CGUID& dbRgnGUID=NULL_GUID, 
		const char* szRetVarName=NULL,
		const char* szRetRgnIDName=NULL);			/*�첽����ֵ������*/

	//! ɾ��Rgn,��GS����RGN_DELETE��Ϣ
	void DeleteRgn(eRgnType rgnType,
		const CGUID& ownerID,
		const CGUID& rgnID,
		const CGUID& scriptID=NULL_GUID,
		const char* szRetVarName=NULL,
		const char* szRetRgnIDName=NULL);			/*�첽����ֵ������*/


	// �������ݿ�ɾ��DBDupRgn��Ϣ
	void SendDelPHDupRgnMsgToDB(const CGUID& rgnGUID);

	// ���߳�������
	map<long/*����ID*/, vector<long/*templateID*/>>& GetLinedRgnIdByLinedIdMap(void) { return m_mapLinedRgnIdByLinedId; }
private:
	// �����б�<id, tagRegion>, ����������ļ���ȡ�ĳ������ݶ���
	map<long, tagRegion>			m_mapRegionList;
	map<long, tagRegion>			m_mapPersonalRgnList;
	map<long, tagRegion>			m_mapTeamRgnList;
	map<long, tagRegion>			m_mapPersonalHouseRgnList;

	// ���߳�������
	map<long/*����ID*/, vector<long/*templateID*/>> m_mapLinedRgnIdByLinedId;
private:

	map<CGUID/*ownerID*/, map<LONG/*templateID*/, CGUID/*rgnGuid*/>> m_mapTeamRgnGUID;
	map<CGUID/*ownerID*/, CGUID/*rgnGuid*/> m_mapPersonalRgnGUID;

	MapRgn m_mapRgn;

	// ���˷��ݸ����������ȷ�����
	vector<long>	m_vecFirstPersonalHouseServer;
	// ���˸����������ȷ�����
	vector<long>	m_vecFirstPersonalServer;
	// ��Ӹ����������ȷ�����
	vector<long>	m_vecFirstTeamServer;
};

#endif//GLOBAL_RGN_MANAGER