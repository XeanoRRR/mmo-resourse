#include "StdAfx.h"
#include ".\confederation.h"
#include "OrganizingCtrl.h"
#include "..\dbaccess\worlddb\rsconfederation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long CConfederation::MAX_MemberNum = 6;		//����Ա����

CConfederation::CConfederation(long lID,long lMastterID,string& strName /*=  string("")*/)
:m_lID(lID)
,m_lMastterID(lMastterID)
,m_strName(strName)
{
	Initial();
}

CConfederation::~CConfederation(void)
{
}

//��ʼ��
bool CConfederation::Initial()
{
	m_ApplyPerson = 0;

	tagMemInfo MemInfo;
	MemInfo.lID = m_lMastterID;
	//������Ĭ��ְ�����1
	MemInfo.lJobLvl = MIN_JobLvl;
	//��������ƺ�
	strcpy(MemInfo.strTitle,"����");

	MemInfo.listPV[PV_Disband] = PST_Permit;
	MemInfo.listPV[PV_Exit] = PST_No;
	MemInfo.listPV[PV_ConMem] = PST_Permit;
	MemInfo.listPV[PV_FireOut] = PST_Permit;
	MemInfo.listPV[PV_DubJobLvl] = PST_No;
	MemInfo.listPV[PV_Pronounce] = PST_No;
	MemInfo.listPV[PV_LeaveWord] = PST_No;
	MemInfo.listPV[PV_EditLeaveWord] = PST_No;
	MemInfo.listPV[PV_ObtainTax] = PST_No;
	MemInfo.listPV[PV_EndueROR] = PST_No;
	
	m_Members[m_lMastterID] = MemInfo;
	return true;
}

//���𱣴�����
bool CConfederation::Save()
{
	GetGame()->GetRsConfederation()->SaveConfederation(this);
	return true;
}
//����װ������
bool CConfederation::Load()
{
	return true;
}

//���ó�Ա��Ϣ
bool CConfederation::SetMembers(BYTE* pByte,long lSize)
{
	tagMemInfo* pMemInfo;
	long lCount = lSize/sizeof(tagMemInfo);
	assert(lCount>=1);
	for(long i=0; i < lCount;i++)
	{
		pMemInfo = (tagMemInfo*)pByte;
		m_Members[pMemInfo->lID] = *pMemInfo;
		pByte+=sizeof(tagMemInfo);
	}
	return true;
}

//�õ���Ա��Ϣ����
bool CConfederation::GetMembersData(vector<BYTE>& ByteArray)
{
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(;it!=m_Members.end();it++)
	{
		_AddToByteArray(&ByteArray,&(*it).second,sizeof(tagMemInfo) );
	}
	return true;
}

// ��ӵ�CByteArray
bool CConfederation::AddToByteArray()
{
	return true;
}

//����ͬ�˵�����
bool CConfederation::SetName(string& strName)
{
	m_strName = strName;
	return true;
}

//�жϸøð��Ƿ�ӵ������
//����ֵ:0��ʾû�У����򷵻ظ����ǵ�ID
long CConfederation::IsOwnedCity(long lCityID)
{
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing((*it).first);
		if(pOrgFaction && pOrgFaction->IsOwnedCity(lCityID) > 0)
			return pOrgFaction->IsOwnedCity(lCityID);
	};
	return 0;
}

//���øð�������ID
void CConfederation::AddOwnedCity(long lCityID)
{
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing((*it).first);
		if(pOrgFaction)
		{
			//������������ѸóǸ����˰�
			if((*it).first == m_lMastterID)	pOrgFaction->AddOwnedCity(lCityID);
			//���������иóǣ���ɾ��������
			else pOrgFaction->DelOwnedCity(lCityID);
		}
	};
}

//ɾ��һ��ӵ�е�����ID
bool CConfederation::DelOwnedCity(long lCityID)
{
	//�������������ڵİ�
	COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing(m_lMastterID);
	if(pOrgFaction)
	{
		pOrgFaction->DelOwnedCity(lCityID);
		return true;
	}
	return false;
}
//�Եж԰���б�������
//�õ�������֯�б�
list<long> CConfederation::GetFriendList()
{
	list<long> TemptList;
	TemptList.clear();
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		TemptList.push_back((*it).first);
	}
	return TemptList;
}
//�õ��ж���֯�б�
list<long> CConfederation::GetEnemyList()
{
	//�õ�ͬ������һ�����ɵĵж��б�Ϳ��ԣ�����ȡ�����İ���
	COrganizing* pOrga = GetOrganiCtrl()->GetFactionOrganizing(m_lMastterID);
	if(pOrga)
	{
		return pOrga->GetEnemyList();
	}
	else
	{
		list<long> TemptList;
		TemptList.clear();
		return TemptList;
	}
}

//���һ���ж���֯�б�
void CConfederation::AddEnemyOrganizing(list<long>& TemptList)
{
	//��ÿ����Ա��֯���
	//����ѭ�����Ϊ6*6=36��
	COrganizing* pOrga = NULL;
	list<long>::iterator itList = TemptList.begin();
	for(; itList != TemptList.begin(); itList++)
	{
		map<long,tagMemInfo>::iterator itMem = m_Members.begin();
		for(;itMem != m_Members.end(); itMem++)
		{
			if(pOrga = GetOrganiCtrl()->GetFactionOrganizing((*itMem).first))
			{
				pOrga->AddEnemyOrganizing((*itList));
			}
		}
	}
}
//ɾ��һ���ж���֯�б�
void CConfederation::DelEnemyOrganizing(list<long>& TemptList)
{
	//����ÿ����Ա��֯ɾ��
	//����ѭ�����Ϊ6*6=36��
	COrganizing* pOrga = NULL;
	list<long>::iterator itList = TemptList.begin();
	for(; itList != TemptList.begin(); itList++)
	{
		map<long,tagMemInfo>::iterator itMem = m_Members.begin();
		for(;itMem != m_Members.end(); itMem++)
		{
			if(pOrga = GetOrganiCtrl()->GetFactionOrganizing((*itMem).first))
			{
				pOrga->DelEnemyOrganizing((*itList));
			}
		}
	}
}
//�Ƿ��еж԰�����
bool CConfederation::IsHaveEnymyFaction()
{
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing((*it).first);
		if(pOrgFaction &&	pOrgFaction->IsHaveEnymyFaction() ) return true;
	}
	return false;
}
//���һ���ж԰��,��ÿ����Ա����ӵж԰��
void CConfederation::AddEnemyOrganizing(long lFactionID)
{
	//����ð���Ǹ�ͬ�˵ĳ�Ա���˳�
	if(IsMember(lFactionID) > 0)	return;

	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing((*it).first);
		if(pOrgFaction)	pOrgFaction->AddEnemyOrganizing(lFactionID);
	}
}
//ɾ��һ���ж԰��
//ɾ��ÿ������Ա�ĵж԰��
void CConfederation::DelEnemyOrganizing(long lFactionID)
{
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing((*it).first);
		if(pOrgFaction)	pOrgFaction->DelEnemyOrganizing(lFactionID);
	}
}


//����ж԰���б�
//���ÿ������Ա�ĵж԰���б�
void CConfederation::ClearEnemyFation()
{
	map<long,tagMemInfo>::iterator it = m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		COrganizing* pOrgFaction = GetOrganiCtrl()->GetFactionOrganizing((*it).first);
		if(pOrgFaction)	pOrgFaction->ClearEnemyFation();
	}
}

//�ж�ĳID�Ƿ���ʹ�ô�Ȩ��
//������ҵ�ID
bool CConfederation::IsUsingPV(long lID,ePurview ePV)
{
	//Ȩ�޲�����
	if(ePV < PV_Disband || ePV > PV_EndueROR)	return false;

	long lFactionID = GetOrganiCtrl()->IsFactionMaster(lID);
	//����Ҳ��ǰ���,��û��Ȩ�޲�����
	if(lFactionID == 0)	return false;
	//���������û�д˰���,�˳�
	if( m_Members.find(lFactionID) == m_Members.end() )	return false;

	//�ð��Ƿ��д�Ȩ�޲��Ҹ�Ȩ���Ƿ��
	if( m_Members[lFactionID].listPV[ePV] != PST_Permit )	return false;
	//ӵ�д�Ȩ��
	return true;
}
//��ĳID����һ��Ȩ��
//������ID
void CConfederation::SetMemPV(long lID,ePurview ePV)
{
	//Ȩ�޲�����
	if(ePV < PV_Disband || ePV > PV_EndueROR)	return;
	//�ҵ��˳�Ա����Ϣ
	if(m_Members.find(lID) == m_Members.end()) return;
	//ӵ�д�Ȩ���˳�
	if( m_Members[lID].listPV[ePV] != PST_No)	return;
	//����һ��Ȩ�޸��ó�Ա,Ĭ��Ϊ�򿪴�Ȩ��
	m_Members[lID].listPV[ePV] = PST_Permit;
}

//ȡ��ĳ����֯��Ȩ��
//�������ID
void CConfederation::AbolishMemPV(long lID,ePurview ePV)
{
	//Ȩ�޲�����
	if(ePV < PV_Disband || ePV > PV_EndueROR)	return;
	//�ҵ��˳�Ա����Ϣ
	if(m_Members.find(lID) == m_Members.end()) return;
	//�Ѿ�û�д�Ȩ���˳�
	if(m_Members[lID].listPV[ePV] == PST_No)	return;
	//ȡ����Ȩ��
	m_Members[lID].listPV[ePV] = PST_No;
	return ;
}
//���ĳIDִ��ĳ�������ĺϷ���
//�������ID
bool CConfederation::CheckOperValidate(long lExecuteID,long lBeExecutedID,ePurview ePV)
{
	//�Լ����ܶ��Լ����в���
	if(lExecuteID == lBeExecutedID )	return false;
	//�������߲���������
	if(lBeExecutedID == m_lMastterID)	return false;
	//�õ����ڵİﲢ�������ǰ���
	long lExeFactionID		= GetOrganiCtrl()->IsFactionMaster(lExecuteID);
	long lBeExedFactionID	= GetOrganiCtrl()->IsFactionMaster(lBeExecutedID);
	if(lExeFactionID == 0 || lBeExedFactionID == 0)	return false;

	//���Ǹ��˵İ����������˲���
	if( IsMember(lExeFactionID) == 0 || IsMember(lBeExedFactionID) == 0) return false;
	//������û�д˲���Ȩ�޻������˶��д˲���Ȩ������ִ�д˲���
	bool lResult = IsUsingPV(lExeFactionID,ePV);
	if( lResult == false || lResult == IsUsingPV(lBeExedFactionID,ePV) ) return false;
	
	return true;
}
//���ĳIDִ��ĳ�������ĺϷ���
//�������ID
bool CConfederation::CheckOperValidate(long lExecuteID,ePurview ePV)
{
	//�õ����ڵİﲢ�������ǰ���
	long lFactionID		= GetOrganiCtrl()->IsFactionMaster(lExecuteID);
	if(lFactionID == 0)	return false;
	//���Ǳ����Ա����ִ�д˲���
	if(IsMember(lFactionID)==0)	return false;
	//û�д˲���Ȩ������ִ�д˲���
	if(IsUsingPV(lFactionID,ePV)== false)	return false;
	
	return true;
}
//�ӱ�����ɾ��һ����Ա
//������ID
bool CConfederation::DelMember(long lID)
{
	m_Members.erase(lID);
	//ɾ���ð��ĵж���֯,�����ϼ���֯Ϊ0
	COrganizing* pFaction = GetOrganiCtrl()->GetFactionOrganizing(lID);
	if(pFaction)
	{
		pFaction->ClearEnemyFation();
		pFaction->SetSuperiorOrganizing(0);
	}
	return true;
}
//�Ƴ�һ�������б��еĳ�Ա
long CConfederation::RemoveApplyMember(long lID)
{
	return 0;
}
//�������ɰ��ɳ�Ա����
bool CConfederation::ApplyForJoin(long lPlayerID,long lLvl,long lOrgMemID /*= 0*/)
{
	//�����ǰ�ǹ���ʱ�䣬����������ɼ���ͬ��
	if(GetOrganiCtrl()->IsAggressCityTime() == true)	return false;

	//��ǰ���������,�˳�
	if(m_ApplyPerson > 0)	return false;
	//���������ڵİ��,�����ǰ���
	long lFactionID = GetOrganiCtrl()->IsFactionMaster(lPlayerID);
	if(lFactionID == 0)	return false;
	//��鱻�����player�Ƿ���н����³�Ա��Ȩ��
	if(CheckOperValidate(lOrgMemID,PV_ConMem) == false)	return false;
	//�ð���������ɰ���
	long lConfederationID = GetOrganiCtrl()->IsFreeFaction(lFactionID);
	if(lConfederationID > 0)	return false;

	//����Ҫ����İ���ID
	m_ApplyPerson = lFactionID;

	//�����첽�ص���
	class PlayerApplyForJoinConfeder:public IAsyncCaller,public IAsyncCallback
	{
	public:
		PlayerApplyForJoinConfeder(long lID)
			:m_lID(lID)
		{
		}
		//�ͷ��Լ�
		void Release(){	delete this; }

		void DoAsyncCall(long lID,long lPassWord,va_list va)
		{
			long lAppPlayerID = va_arg(va,long);
			long lFactinoMemID	 = va_arg(va,long);
			//������Ϣ��lFactinoMemID����,�ȴ��첽���ص���			
		}

		void OnAsyncCallback(tagAsyncResult& Result)
		{
			if(Result.Result == AR_OK)
			{
				long lExecuteID = va_arg(Result.Arguments,long);
				long lBeExecutedID	 = va_arg(Result.Arguments,long);
				tagTime Time = va_arg(Result.Arguments,tagTime);

				COrganizing* pConfederation = GetOrganiCtrl()->GetConfederationOrganizing(m_lID);
				if(pConfederation)
				{
					pConfederation->DoJoin(lExecuteID,lBeExecutedID,Time);
				}
			}
		}
	private:
		//����һ�����˵�ID,�Ա�ص�ʱ����
		long  m_lID;
	};

	//����һ��Session
	CNetSession* pSession = GetNetSessionManager()->CreateSession(lOrgMemID);
	//�����������첽���ýӿ�
	PlayerApplyForJoinConfeder* pInstance = new PlayerApplyForJoinConfeder(m_lID);
	pSession->SetCallbackHandle(pInstance);
	//���賬ʱʱ��,����ʼ�첽����
	pSession->Beging(1000,pInstance,lPlayerID,lOrgMemID);

	return true;
}
//����Ҫ�������˵���֯
//����PlayerID
bool CConfederation::DoJoin(long lExecuteID,long lBeExecutedID,tagTime& Time)
{
	//���������ڵİ��,�����ǰ���
	long lFactionID = GetOrganiCtrl()->IsFactionMaster(lBeExecutedID);
	if(lFactionID == 0)	return false;
	//����������İ����Ƿ�Ϸ�
	if(m_ApplyPerson != lFactionID)	return false;
	//���
	m_ApplyPerson = 0;
	//�ð���������ɰ���
	long lConfederationID = GetOrganiCtrl()->IsFreeFaction(lFactionID);
	if(lConfederationID > 0)	return false;
	//�������ĺϷ���
	if(CheckOperValidate(lExecuteID,PV_ConMem) == false)	return false;

	//���ӵж԰��
	COrganizing* pFaction = GetOrganiCtrl()->GetFactionOrganizing(lFactionID);
	if(pFaction)
	{
		AddEnemyOrganizing(pFaction->GetEnemyList());
		pFaction->AddEnemyOrganizing(GetEnemyList());
	}

	//�Ѹó�Ա������
	tagMemInfo MemInfo;
	MemInfo.lID=lFactionID;
	//�������ְ��
	MemInfo.lJobLvl = MAX_JobLvl;
	//�����ԱĬ�ϳƺ�
	strcpy(MemInfo.strTitle, "��Ա");

	//�����Ա���˳�����Ȩ��
	MemInfo.listPV[PV_Disband] = PST_No;
	MemInfo.listPV[PV_Exit] = PST_Permit;
	MemInfo.listPV[PV_ConMem] = PST_No;
	MemInfo.listPV[PV_FireOut] = PST_No;
	MemInfo.listPV[PV_DubJobLvl] = PST_No;
	MemInfo.listPV[PV_Pronounce] = PST_No;
	MemInfo.listPV[PV_LeaveWord] = PST_No;
	MemInfo.listPV[PV_EditLeaveWord] = PST_No;
	MemInfo.listPV[PV_ObtainTax] = PST_No;
	MemInfo.listPV[PV_EndueROR] = PST_No;


	m_Members[lFactionID] = MemInfo;

	//���øð����ϼ���֯
	if(pFaction)	pFaction->SetSuperiorOrganizing(m_lID);
	return true;
}

//���������Ա�б�
bool CConfederation::ClearApplyList(long lPlayerID)
{
	//�������ĺϷ���
	if(CheckOperValidate(lPlayerID,PV_ConMem) == false)	return false;
	//���������Ա
	m_ApplyPerson = 0;
	return true;
}

//������Ա����
bool CConfederation::Invite(long lInvitedID, long lBeInvitedID)
{
	return true;
}
//�˳�
bool CConfederation::Exit(long lPlayerID)
{
	//�����ǰ�ǹ���ʱ�䣬���ܽ���ͬ��
	if(GetOrganiCtrl()->IsAggressCityTime() == true)	return false;
	//�������ĺϷ���
	if(CheckOperValidate(lPlayerID,PV_Exit) == false)	return false;
	//�õ���������ڵİ���
	long lFactionID = GetOrganiCtrl()->IsFactionMaster(lPlayerID);
	if(lFactionID == 0)	return false;
	
	//ɾ���ð��
	DelMember(lFactionID);
	return true;
}
//��ɢ
//����PlayerID
bool CConfederation::Disband(long lID)
{
	//�������ĺϷ���
	if(CheckOperValidate(lID,PV_Disband) == false)	return false;

	//�����Ա
	map<long,tagMemInfo>::iterator it=m_Members.begin();
	for(; it != m_Members.end(); it++)
	{
		DelMember((*it).first);
	}
	//��0
	m_ApplyPerson = 0;
	return true;
}

//����
bool CConfederation::FireOut(long lFireID,long lBeFiredID)
{	
	//�����ǰ�ǹ���ʱ�䣬���ܿ�����Ա 
	if(GetOrganiCtrl()->IsAggressCityTime() == true)	return false;
	//�������ĺϷ���
	if(CheckOperValidate(lFireID,lBeFiredID,PV_FireOut) == false)	return false;
	//�õ��������ߵİ��
	long lFactionID = GetOrganiCtrl()->IsFactionMaster(lBeFiredID);
	if(lFactionID == 0)	return false;
	//ɾ���ð���
	DelMember(lFactionID);
	return true;
}

//������֮λ
bool CConfederation::Demise(long lMasterID,long lID)
{
	//�����ǰ�ǹ���ʱ�䣬���ܽ�����֮λ
	if(GetOrganiCtrl()->IsAggressCityTime() == true)	return false;
	//�Լ����ܶ��Լ����в���
	if(lMasterID == lID )	return false;
	//�õ����ڵİﲢ�������ǰ���
	long lMasterFactionID		= GetOrganiCtrl()->IsFactionMaster(lMasterID);
	long lFactionID				= GetOrganiCtrl()->IsFactionMaster(lID);
	if(lMasterFactionID == 0 || lFactionID == 0)	return false;

	//���Ǹ��˵�����������������˲���
	if( IsMaster(lMasterFactionID) == 0 || IsMember(lFactionID) == 0) return false;

	//�ı�����
	m_lMastterID = lFactionID;
	//�ı����˳�Ա��Ϣ
	m_Members[lFactionID] = m_Members[lMasterFactionID];
	m_Members[lFactionID].lID = lFactionID;

	//�ı�ԭ�����ĳƺź�Ȩ��
	m_Members[lMasterFactionID].lJobLvl = 99;
	m_Members[lMasterFactionID].lID = lMasterFactionID;
	strcpy(m_Members[lMasterFactionID].strTitle,"��Ա");
;
	//�����Ա���˳�����Ȩ��
	m_Members[lMasterFactionID].listPV[PV_Disband] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_Exit] = PST_Permit;
	m_Members[lMasterFactionID].listPV[PV_ConMem] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_FireOut] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_DubJobLvl] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_Pronounce] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_LeaveWord] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_EditLeaveWord] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_ObtainTax] = PST_No;
	m_Members[lMasterFactionID].listPV[PV_EndueROR] = PST_No;
	return true;
}

//����˰��
bool CConfederation::OperatorTax(long lPlayerID)
{
	//ֻ�������İ��ɲ�ӵ�в���˰���Ȩ��
	COrganizing* pOrga = GetOrganiCtrl()->GetFactionOrganizing(m_lMastterID);
	if(pOrga == NULL)	return false;

    return pOrga->OperatorTax(lPlayerID);
}
//����ƺ�
bool CConfederation::DubAndSetJobLvl(long lDubID,long lBeDubedID,string& strTitle,long lLvl)
{	
	return true;
}
//ȡ����Աĳ��Ȩ��
bool CConfederation::AbolishRightToMember(long lEndueID,long lBeEnduedID,ePurview ePV)
{
	return true;
}
//�����ԱȨ��
bool CConfederation::EndueRightToMember(long lEndueID,long lBeEnduedID,ePurview ePV)
{
	return true;
}

//����
bool CConfederation::Pronounce(long lPlayerID,string& strPro,tagTime& Time)
{	
	return true;
}
//����
bool CConfederation::LeaveWord(long lPlayerID,string& strWords,tagTime& Time)
{
	return true;
}
