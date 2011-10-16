#pragma once
#include "..\myadobase.h"
#include "..\..\server\dbserver\appworld\DBFaction.h"

class CRsFaction :
	public CMyAdoBase
{
public:
	CRsFaction(void);
	~CRsFaction(void);

public:
	//!					��ȡ���а��
	BOOL				LoadAllFaction					(map<CGUID, CDBFaction*> &mapFaction, _ConnectionPtr& cn);
	//!					����һ�����
	BOOL				SaveOneFaction					(CDBFaction* pDBFaction, _ConnectionPtr& cn);
	//!					ɾ��һ�����
	BOOL				DelOneFaction					(const CGUID &FactionGuid, _ConnectionPtr& cn);
	//!					���£������½���һ�����ĳ�Ա
	BOOL				SaveOneFaction_Member			(CDBFactionMemberGroup* memberGroup, _ConnectionPtr& cn);
	//!					���£������½���һ������������
	BOOL				SaveOneFaction_Apply			(CDBFactionApplyGroup* applyGroup, _ConnectionPtr& cn);
private:
	//!					ȡ�����а�������Ϣ
	BOOL				LoadAllFaction_BaseInfo			(map<CGUID, CDBFaction*> &mapFaction, _ConnectionPtr& cn);
	//! 				ȡ�����а���Ա
	BOOL				LoadAllFaction_Member			(map<CGUID, CDBFaction*> &mapFaction, _ConnectionPtr& cn);
	//! 				ȡ�����а���Ա������
	BOOL				LoadAllFaction_Apply			(map<CGUID, CDBFaction*> &mapFaction, _ConnectionPtr& cn);
	//! 				ȡ�����а������
	BOOL				LoadAllFaction_LeaveWord		(map<CGUID, CDBFaction*> &mapFaction, _ConnectionPtr& cn);


	//!					���£������½���һ����������Ϣ
	BOOL				SaveOneFaction_BaseInfo			(CDBFaction* pDBFaction, _ConnectionPtr& cn);
	//!					���£������½���һ����������
	BOOL				SaveOneFaction_LeaveWord		(CDBFaction* pDBFaction, _ConnectionPtr& cn);


	//!					ɾ��һ����������Ϣ
	BOOL				DelOneFaction_BaseInfo			(const CGUID &FactionGuid, _ConnectionPtr& cn);
	//!					ɾ��һ��������г�Ա
	BOOL				DelOneFaction_Member			(const CGUID &FactionGuid, _ConnectionPtr& cn);
	//!					ɾ��һ���������������
	BOOL				DelOneFaction_Apply				(const CGUID &FactionGuid, _ConnectionPtr& cn);
	//!					ɾ��һ���������
	BOOL				DelOneFaction_LeaveWord			(const CGUID &FactionGuid, _ConnectionPtr& cn);

	//��Buffer
	bool SaveBufferField(CBaseDBEntity *pBaseEntity, const char* szFiledName, _RecordsetPtr &pRs);
	//��Buffer
	bool LoadBufferField(CBaseDBEntity *pBaseEntity, const char* szFiledName, _RecordsetPtr &pRs);
};
