//EntityGroup.h/////////////////////////////////////////////////////////////////////
//������:�ö��������Լ�����������,�������ӽڵ����
//Author:������
//Create Time:2008.11.03
#ifndef EntityGroup_h
#define EntityGroup_h

#include "BaseEntity.h"
#include <vector>
#include <string>
#include <list>
#include <map>
#include "../public/dbDefine.h"
#include "baseentity.h"


typedef map<CGUID, CBaseEntity*> EntityGroupMap;
typedef EntityGroupMap::iterator EntityGroupMapItr;

class CEntityGroup: public CBaseEntity
{
public:
	CEntityGroup(const std::string& flag);
	CEntityGroup(const std::string& flag, const CGUID& guid);
	virtual ~CEntityGroup();
private:
	CEntityGroup();

public:
	// virtual interfaces
	virtual void ProcessMsg(bool isLoadOperFlag, const CGUID& SessionID, long retFlag, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	virtual void GetFromByteArray(bool isLoadOperFlag, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	void AddToByteArray(std::vector<BYTE>& pBA);

	public: 		
		void					AddChild		(CBaseEntity* entity);
		virtual void			RemoveChild		(const CGUID& guid);
		virtual CBaseEntity*	GetChild		(const CGUID& guid);

		// ��ʼ����������
		virtual void InitProperty();

		const CGUID& QueryChildGuidByComFlag(const string& comFlag)
		{
			map<string, CGUID>::iterator itr = m_mapGuidByComFlag.find(comFlag);
			if(itr != m_mapGuidByComFlag.end())
				return itr->second;
			return NULL_GUID;
		}

		virtual CEntityGroup& operator=(CEntityGroup& right);
		map<CGUID, CBaseEntity*>& GetEntityGroupMap(void) { return m_mapEntityGroup; }

		// ��������������ָ���ڵ�
		CBaseEntity* FindEntityBytravelTree(const CGUID& desGuid);

		// ���ĳ��Ҷ�Ӽ�Ҷ�ӵ�Ҷ��
		void ReleaseLeaf(const CGUID& guid);

		// ���ĳ��Ҷ�ӵ�Ҷ��,������Ҷ��
		void ClearLeafChilds(const CGUID& guid);

		void ReleaseChilds();

		std::map<string, CGUID>& GetGuidByComFlagMap(void) { return m_mapGuidByComFlag; }

		long	GetLeafComType(void) { return m_lLeafComType; }
		const string&    GetLeafComFlag(void) { return m_lLeafComFlag; }
		void	SetLeafComType(long type) { m_lLeafComType = type; }
		void    SetLeafComFlag(const string& flag) { m_lLeafComFlag = flag; }
		// �丸�ڵ�����������ֵ����ݿ������
		const string&    GetDetailComPositeFlagName(void) { return m_strDetailComPositeFlagName; }
		// �丸�ڵ�����������ֵ����ݿ������
		void			SetDetailComPositeFlagName(const string& str) {  m_strDetailComPositeFlagName = str; }

		long    GetDetailComPositeFlagType(void) { return m_lDetailComPositeFlagType; }
		void	SetDetailComPositeFlagType(long type) { m_lDetailComPositeFlagType = type; }

		BYTE	GetHasDetailLeavesFlag(void) { return m_bHasDetailLeavesFlag; }
		void	SetHasDetailLeavesFlag(BYTE flag) { m_bHasDetailLeavesFlag = flag; }


		void QueryDataRelease()
		{
			vector<CTempEntityProperty*>::iterator epItr = m_pQueryDataMgr.begin();
			for(; epItr != m_pQueryDataMgr.end(); epItr++)
			{
				delete *epItr;
			}
			m_pQueryDataMgr.clear();
		}

		vector<CTempEntityProperty*>& GetQueryDataVec(void) { return m_pQueryDataMgr; }
		void RetQueryDataRelease()
		{
			vector<CTempEntityProperty*>::iterator epItr = m_pRetQueryDataMgr.begin();
			for(; epItr != m_pRetQueryDataMgr.end(); epItr++)
			{
				delete *epItr;
			}
			m_pRetQueryDataMgr.clear();
		}
		vector<CTempEntityProperty*>& GetRetQueryDataVec(void) { return m_pRetQueryDataMgr; }

		bool GetFindByNameFlag(void)				{ return m_bIsFindByName; }
		void SetFindByNameFlag(bool flag)			{ m_bIsFindByName = flag; }

		const char* GetName(void) { return &m_szName[0]; }
		void SetName(const char* pName);

protected:
	map<CGUID, CBaseEntity*> m_mapEntityGroup;
	std::map<string, CGUID> m_mapGuidByComFlag;
	long	m_lLeafComType;
	string    m_lLeafComFlag;
	// �丸�ڵ�����������ֵ����ݿ������
	std::string    m_strDetailComPositeFlagName;
	long	m_lDetailComPositeFlagType;
	BYTE	m_bHasDetailLeavesFlag;
	bool m_bIsFindByName; //0:no,1:yes
	char m_szName[64];
	// ��ѯ�ֶ�����
	vector<CTempEntityProperty*> m_pQueryDataMgr;
	vector<CTempEntityProperty*> m_pRetQueryDataMgr;
};


#endif//DBGoodsGroup_h