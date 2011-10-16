#ifndef RGN_MANAGER_H
#define RGN_MANAGER_H

//#include "ProxyServerRegion.h"
#include "ServerRegion.h"

// ���峡����ʱ��
typedef CTimerQueue<CServerRegion*> RegionTimer;

// �����ŶӶ�ʱ��
typedef CTimerQueue<CPlayer*> QueueTimer;

// ������Ҵ������˸�����ʱ��
typedef CTimerQueue<CPlayer*> CreatePRgnTimer;
// ������Ҵ�����Ӹ�����ʱ��
typedef CTimerQueue<CPlayer*> CreateTRgnTimer;
// ���������Ӹ������˶�ʱ��
typedef CTimerQueue<CPlayer*> RgnKickPlayerTimer;

class CMessage;
class CPlayer;
class CScript;

class CRgnManager
{
public:
	// ���˷��ݸ�����Ʒ��
	struct PHGoods
	{
		// ��ƷGUID
		CGUID guid;
		// ��Ʒ����
		char name[64];
		// ����
		int dir;
		// X����
		int width;
		// Y����
		int height;
		// ����
		float posX;
		float posY;

		void operator=(const PHGoods& right)
		{
			if(&right != this)
			{
				// ��ƷGUID
				guid = right.guid;
				// ��Ʒ����
				memcpy(name, right.name, sizeof(right.name));
				// ����
				dir = right.dir;
				// X����
				width = right.width;
				// Y����
				height = right.height;
				// ����
				posX = right.posX;
				posY = right.posY;
			}
		}
		bool operator==(const PHGoods& right)
		{
			if(this != &right)
			{
				if(
				guid == right.guid &&
				strcmp(name, right.name) == 0 &&
				dir == right.dir &&
				width == right.width &&
				height == right.height &&
				posX == right.posX &&
				posY == right.posY)
				return true;
				else
					return false;
			}
			return true;
		}
	};

	struct tagDupRgnData
	{
	public:
		tagDupRgnData(void);
		~tagDupRgnData(void);
		long lBufSize;
		const BYTE* pData;
	};

	CRgnManager();
	~CRgnManager();

	typedef std::map<CGUID/*RgnID*/, CServerRegion*> RgnByIDMap;
	typedef RgnByIDMap::iterator RgnByIDMapItr;

	// ���˷��ݸ�����Ʒ��������
	typedef std::vector<PHGoods> vecPHGoods;
	typedef vecPHGoods::iterator vecPHGoodsItr;

	typedef std::map<CGUID/*PHRgnID*/, vecPHGoods> PHGoodsMap;
	typedef PHGoodsMap::iterator PHGoodsMapItr;
	
	//! ����ʱ,�ȹ��ܵĽӿ�
	void Run(void);
	//! ����Rgn,��WS����RGN_CREATE��Ϣ
	long CreateRgnMsg(Script* pScript/*����Ľű�ID*/, long templateRgnID/*ģ��Region��ID*/, eRgnType rgnType, const CGUID& ownerID=NULL_GUID/*NORMAL������IDΪ��*/, const char* szRetVarName=NULL/*�첽����ֵ������*/, const char* szRetRgnIDName=NULL/*�첽����ֵ������*/);
	//! ɾ��Rgn,��WS����RGN_DELETE��Ϣ
	long DeleteRgnMsg(const CGUID& runScriptID/*����Ľű�ID*/, eRgnType rgnType, const CGUID& ownerID, const CGUID& rgnID, const char* szRetVarName=NULL/*�첽����ֵ������*/, const char* szRetRgnIDName=NULL/*�첽����ֵ������*/);

	//! ����Rgn,��WS����RGN_CREATE��Ϣ
	long OnCreateRgnMsg(CMessage* pMsg);
	//! ɾ��Rgn,��WS����RGN_DELETE��Ϣ
	long OnDeleteRgnMsg(CMessage* pMsg);

	//! ����Rgn
	CServerRegion* FindRgnByRgnGUID(eRgnType rgnType, const CGUID& rgnID);
	//! ��������region
	CServerRegion* FindRgnByRgnGUID(const CGUID& rgnID);
	//! ��������region
	CServerRegion* FindRgnByTemplateID(eRgnType rgnType, long templateID);
	
	// �������˷��ݸ�����Ʒ(Monster)
	void CreatePHGoods(const CGUID& rgnID, const char* name, long nCounts, long posX, long posY,
						long width, long height, long dir, const char* strScript, const char* strTalk);
	// ɾ�����˷��ݸ�����Ʒ(Monster)
	void DeletePHGoods(const CGUID& rgnID, const CGUID& goodsID);
	// ɾ�����˷��ݸ�����Ʒ(Monster)
	void DeletePHGoods(const CGUID& rgnID, const char* goodsName);

	RgnByIDMap& GetPersonalDupRgnMap() { return m_mapPersonalDupRgn; };
	RgnByIDMap& GetTeamDupRgnMap() { return m_mapTeamDupRgn; };
	RgnByIDMap& GetPersonalHouseDupRgnMap() { return m_mapPersonalHouseDupRgn; };
	RgnByIDMap& GetNormalRgnMap() { return m_mapNormalRgn; };

	CServerRegion* AddNormalRgn(const CGUID& rgnID, long templateRgnID);
	void DelNormalRgn(const CGUID& rgnID, long templateRgnID);

	CServerRegion* AddCreatedDupRgn(const CGUID& rgnID, eRgnType type);
	void DelCreatedDupRgn(const CGUID& rgnID, eRgnType type);

	// ȡ�ø��������ĵ�ǰֵ
	long GetCurNormalRgnNum(void) { return m_CurNormalRgnNum; }
	long GetCurPersonalDupRgnNum(void) { return m_CurPersonalDupRgnNum; }
	long GetCurPersonalHouseDupRgnNum(void) { return m_CurPersonalHouseDupRgnNum; }
	long GetCurTeamDupRgnNum(void) { return m_CurTeamDupRgnNum; }

	// ���ø��������ĵ�ǰֵ
	void SetCurNormalRgnNum(long value) { m_CurNormalRgnNum = value; }
	void SetCurPersonalDupRgnNum(long value) { m_CurPersonalDupRgnNum = value; }
	void SetCurPersonalHouseDupRgnNum(long value) { m_CurPersonalHouseDupRgnNum = value; }
	void SetCurTeamDupRgnNum(long value) { m_CurTeamDupRgnNum = value; }


	// ȡ�ø÷���������ʵ�Ŷ���
	long GetQueueMaxNum(void) { return m_QueueMaxNum; }
	void SetQueueMaxNum(long value) { m_QueueMaxNum = value; }

	// ȡ�ø÷������ĵ�ǰ�Ŷ���
	long GetCurQueueNum(void) { return m_CurQueueNum; }
	void SetCurQueueNum(long value) { m_CurQueueNum = value; }

	RegionTimer* GetRegionTimer(void) { return &m_RegionTimer; }

	QueueTimer* GetQueueTimer(void) { return &m_QueueTimer; }

	CreatePRgnTimer* GetCreatePRgnTimer(void) { return &m_CreatePRgnTimer; }
	CreateTRgnTimer* GetCreateTRgnTimer(void) { return &m_CreateTRgnTimer; }
	RgnKickPlayerTimer* GetRgnKickPlayerTimer(void) { return &m_RgnKickPlayerTimer; }
	// ��ǰ�����Ŷӵ���� 
	std::map<long/*��ʼʱ��*/, CPlayer*>& GetPlayerQueue(void) { return m_PlayerQueue; }

	// ���˷��ݸ�����Ʒ����
	PHGoodsMap& GetPHGoodsMap(void) { return m_PHGoodsMap; }

	//! ������߳�������
	bool DecodeLinedRgnInfoFromByteArray(BYTE* buf, long& pos, long bufSize=-1);

	bool AddLinedRgnInfoToByteArray(long linedId, DBWriteSet& setWriteDB);

	// ���߳�������
	map<long/*����ID*/, vector<long/*templateID*/>>& GetLinedRgnIdByLinedIdMap(void) { return m_mapLinedRgnIdByLinedId; }

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
	void				RgnGuidCode(const CGUID& ownerGuid, vector<BYTE> &vData);
	//!					��ҵĶ��鸱��GUID����
	void				RgnGuidDecode(const CGUID& ownerGuid, BYTE *pData, LONG &lPos);
	//!					���뽻����ҵĶ��鸱������������ͬģ��ĸ�����
	void				RgnGuidSwapFromArray(const CGUID& ownerGuid, BYTE *pData, LONG &lPos);

	//!					��ҵĶ��鸱��GUID����
	void				RgnGuidCode(const CGUID& ownerGuid, DBWriteSet& setWriteDB);
	//!					��ҵĶ��鸱��GUID����
	void				RgnGuidDecode(const CGUID& ownerGuid, DBReadSet& setReadDB);
	//!					���뽻����ҵĶ��鸱������������ͬģ��ĸ�����
	void				RgnGuidSwapFromArray(const CGUID& ownerGuid, DBReadSet& setReadDB);
	//!					���͸������ݵ�WS
	void				SendTeamRgnToWs(const CGUID& ownerGuid);


	//!					�õ���ҵĸ��˸���GUID�б�
	map<CGUID, CGUID>&	GetPersonalRgnGuidMap(const CGUID& ownerGuid);
	void				AddPersonalRgnGUID(const CGUID& ownerGuid, const CGUID& rgnGuid);
	//!					ɾ��һ����Ҹ��˸���GUID
	void				DelPersonalRgnGUID(const CGUID& ownerGuid);
	//!					�õ�һ�����˸�����GUID
	const	CGUID&		GetPersonalRgnGUID(const CGUID& ownerGuid);

	//!					���һ���������ݿ�
	bool				AddDupRgnData(long lTemplateID, const BYTE* pData, long lBufSize);
	//!					�ж��Ƿ��иø������ݿ�
	long				FindDupRgnData(long lTemplateID);
	//!					�õ��ø������ݿ�
	const BYTE*			GetDupRgnData(long lTemplateID);
	//!					������и������ݿ�
	void				DelAllDupRgnData(void);
	//!					���ݿ����
	long				GetDupRgnDataNum(void) { return (long)m_mapDupRgnData.size(); }

private:

	map<CGUID/*ownerID*/, map<LONG/*templateID*/, CGUID/*rgnGuid*/>> m_mapTeamRgnGUID;
	map<CGUID/*ownerID*/, CGUID/*rgnGuid*/> m_mapPersonalRgnGUID;

	//! ���и�������ÿ��Playerͬʱֻ����1��,�ʲ���OwnerID����ӦCServerRegion*
	RgnByIDMap m_mapPersonalDupRgn;
	RgnByIDMap m_mapTeamDupRgn;
	RgnByIDMap m_mapPersonalHouseDupRgn;
	RgnByIDMap m_mapNormalRgn;

	long	m_CurNormalRgnNum;
	long	m_CurPersonalDupRgnNum;
	long	m_CurPersonalHouseDupRgnNum;
	long	m_CurTeamDupRgnNum;

	// ���������Ŷ���ʼ��
	long    m_QueueMaxNum;
	// ���������Ŷ������
	long    m_CurQueueNum;

	// ������ʱ��
	RegionTimer m_RegionTimer;
	// ���������ŶӶ�ʱ��
	QueueTimer  m_QueueTimer;

	// ������Ҵ������˸�����ʱ��
	CreatePRgnTimer m_CreatePRgnTimer;
	// ������Ҵ�����Ӹ�����ʱ��
	CreateTRgnTimer m_CreateTRgnTimer;
	// �������˶�ʱ��
	RgnKickPlayerTimer m_RgnKickPlayerTimer;

	// ��ǰ�����Ŷӵ���� 
	std::map<long/*��ʼʱ��*/, CPlayer*> m_PlayerQueue;

	// ���˷��ݸ�����Ʒ����
	PHGoodsMap m_PHGoodsMap;

	// ���߳�������
	map<long/*����ID*/, vector<long/*templateID*/>> m_mapLinedRgnIdByLinedId;

	map<long/*TemplateID*/, tagDupRgnData*> m_mapDupRgnData;
};

extern CRgnManager g_RgnManager;

#endif//RGN_MANAGER_H