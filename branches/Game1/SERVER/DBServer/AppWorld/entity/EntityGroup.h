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



class CEntityGroup: public CBaseEntity
{
public:
	CEntityGroup(const std::string& flag);
	CEntityGroup(const std::string& flag, const CGUID& guid);
	virtual ~CEntityGroup();
private:
	CEntityGroup();

public:

	void GetFromByteArray(BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void AddToByteArray(std::vector<BYTE>& pBA, bool isLoadOperFlag);

	public:		
		void					AddChild	(CBaseEntity* entity);
		virtual void			RemoveChild	(const CGUID& guid);
		virtual CBaseEntity*	GetChild	(const CGUID& guid);

		// ��ʼ����������
		virtual void			InitProperty();

		virtual CEntityGroup& operator=(CEntityGroup& right);
		std::map<CGUID, CBaseEntity*>& GetEntityGroupMap(void) { return m_mapEntityGroup; }

		// ��������������ָ���ڵ�
		CBaseEntity*	FindEntityBytravelTree	(const CGUID& desGuid);
		// ���ĳ��Ҷ�Ӽ�Ҷ�ӵ�Ҷ��
		void			ReleaseLeaf				(const CGUID& guid);
		// ���ĳ��Ҷ�ӵ�Ҷ��,������Ҷ��
		void			ClearLeafChilds			(const CGUID& guid);
		void			ReleaseChilds();
		// ת��ĳҶ�ӵ�ָ��
		CBaseEntity*	ReleaseChildPtr			(const CGUID& guid);

		std::map<string, CGUID>& GetGuidByComFlagMap	(void)					{ return m_mapGuidByComFlag; }
		long				GetLeafComType				(void)					{ return m_lLeafComType; }
		const string&		GetLeafComFlag				(void)					{ return m_lLeafComFlag; }
		void				SetLeafComType				(long type)				{ m_lLeafComType = type; }
		void				SetLeafComFlag				(const string& flag)	{ m_lLeafComFlag = flag; }
		// �丸�ڵ�����������ֵ����ݿ������
		const string&		GetDetailComPositeFlagName	(void)					{ return m_strDetailComPositeFlagName; }
		// �丸�ڵ�����������ֵ����ݿ������
		void				SetDetailComPositeFlagName	(const string& str)		{  m_strDetailComPositeFlagName = str; }
		long				GetDetailComPositeFlagType	(void)					{ return m_lDetailComPositeFlagType; }
		void				SetDetailComPositeFlagType	(long type)				{ m_lDetailComPositeFlagType = type; }
		BYTE				GetHasDetailLeavesFlag		(void)					{ return m_bHasDetailLeavesFlag; }
		void				SetHasDetailLeavesFlag		(BYTE flag)				{ m_bHasDetailLeavesFlag = flag; }

		CGUID				GetSessionID(void) { return m_SessionID; }
		void				SetSessionID(const CGUID& guid) { m_SessionID = guid; }

		// �ȴ���ɾ����Ҷ�ӽڵ�GUID
		const CGUID&		GetDelLeafGuid(void)				{ return m_DeleteLeafGuid; }
		void				SetDelLeafGuid(const CGUID& guid)	{ m_DeleteLeafGuid = guid; }
		const string&		GetDelLeafFlag(void)				{ return m_strDelLeafComFlag; }
		void				SetDelLeafFlag(const string& str)	{ m_strDelLeafComFlag = str; }

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
	bool m_bIsFindByName; //0:no,1:yes
	char m_szName[64];
	std::map<CGUID, CBaseEntity*> m_mapEntityGroup;
	std::map<string, CGUID> m_mapGuidByComFlag;
	long		m_lLeafComType;
	string		m_lLeafComFlag;
	// �丸�ڵ�����������ֵ����ݿ������
	std::string    m_strDetailComPositeFlagName;
	long	m_lDetailComPositeFlagType;
	BYTE	m_bHasDetailLeavesFlag;

	// ��ѯ�ֶ�����
	vector<CTempEntityProperty*> m_pQueryDataMgr;
	vector<CTempEntityProperty*> m_pRetQueryDataMgr;

	string		m_strProcedureName;

	CGUID		m_SessionID;
	// �ȴ���ɾ����Ҷ�ӽڵ�GUID
	CGUID		m_DeleteLeafGuid;
	// �ȴ���ɾ����Ҷ�ӽڵ�ComFlag
	string		m_strDelLeafComFlag;
};


#endif//DBGoodsGroup_h