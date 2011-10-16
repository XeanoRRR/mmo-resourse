#ifndef RGN_MANAGER_H
#define RGN_MANAGER_H

#include "..\public\BaseDef.h"
#include <map>
#include "../public/GUID.h"
#include "WorldRegion.h"
#include "WorldVillageRegion.h"
#include "WorldWarRegion.h"
//#include "ProxyServerRegion.h"
#include "../public/msgtype.h"
#include "DBEntityDef.h"

// Ԥ����Rgn�ڴ��
#define MAX_FREE_RGN_NUM 16

class CMessage;

class CRgnManager
{
public:

	CRgnManager();
	~CRgnManager();

	//ʹ��Region GUID����
	typedef std::map<CGUID/*RgnID*/, CWorldRegion*> RgnByGUIDMap;
	typedef RgnByGUIDMap::iterator RgnByIDMapItr;

	typedef std::map<CGUID, CGUID> OwnerID2RgnGUIDMap;
	typedef OwnerID2RgnGUIDMap::iterator OwnerID2RgnIDMapItr;

	//! ����Rgn,��GS����RGN_CREATE��Ϣ
	CWorldRegion* CreateRgn(long scrGSID/*���ʹ��������GSID*/, long templateRgnID/*ģ��Region��ID*/, eRgnType rgnType, long GSID, const CGUID& ownerID=NULL_GUID/*NORMAL������IDΪ��*/, long scriptID=0, const CGUID& dbRgnGUID=NULL_GUID, const char* szRetVarName=NULL, const char* szRetRgnIDName=NULL/*�첽����ֵ������*/);
	//! ɾ��Rgn,��GS����RGN_DELETE��Ϣ
	void DeleteRgn(eRgnType rgnType, const CGUID& ownerID, const CGUID& rgnID, long GSID, long scriptID=0, const char* szRetVarName=NULL, const char* szRetRgnIDName=NULL/*�첽����ֵ������*/);

	//! ����Region
	CWorldRegion* FindRgnByGUID(eRgnType rgnType, const CGUID& rgnID=NULL_GUID);
	
	//! ��ӱ���, ��WS����VAR_ADD��Ϣ
	bool AddVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, const char* strValue);
	bool AddVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, long value);
	bool AddVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, long value, long arraySize/*�����С*/);
	bool AddVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, const CGUID& value);
	//! ɾ������, ��WS����VAR_DEL��Ϣ
	bool DelVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName);
	//! ���ñ���, ��WS����VAR_SET��Ϣ
	bool SetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, const char* strValue);
	bool SetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, long value);
	bool SetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, long value, long arrayPos/*����λ��*/);
	bool SetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, const CGUID& value);
	//! ȡ�ñ���, ��WS����VAR_GET��Ϣ
	bool GetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, const char* strValue);
	bool GetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, long& value);
	bool GetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, long& value, long arrayPos/*����λ��*/);
	bool GetVar(eRgnType rgnType, const CGUID& rgnID, const char* strVarName, CGUID& value);

	RgnByGUIDMap& GetPersonalDupRgn(void) { return m_mapPersonalDupRgn; }
	RgnByGUIDMap& GetTeamDupRgn(void) { return m_mapTeamDupRgn; }
	RgnByGUIDMap& GetPersonalHouseDupRgn(void) { return m_mapPersonalHouseDupRgn; }
	RgnByGUIDMap& GetNormalRgn(void) { return m_mapNormalRgn; }

	// �������ݿ�ɾ��DBDupRgn��Ϣ
	void SendDelPHDupRgnMsgToDB(const CGUID& rgnGUID);

private:
	RgnByGUIDMap m_mapPersonalDupRgn;
	RgnByGUIDMap m_mapTeamDupRgn;
	RgnByGUIDMap m_mapPersonalHouseDupRgn;
	RgnByGUIDMap m_mapNormalRgn;

	OwnerID2RgnGUIDMap m_mapPersonalID2RgnID;
	OwnerID2RgnGUIDMap m_mapPersonalHouseID2RgnID;
	OwnerID2RgnGUIDMap m_mapTeamID2RgnID;
};



#endif//RGN_MANAGER_H