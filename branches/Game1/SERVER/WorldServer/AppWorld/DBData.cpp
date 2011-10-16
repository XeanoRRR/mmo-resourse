#include "StdAfx.h"
#include "DBData.h"

CDBData *CDBData::instance = NULL;


CDBData::CDBData()
{
	for(int i=0;i<16;i++)
	{
		m_stDBData.stGoodsID[i].bUsed=false;
		m_stDBData.stGoodsID[i].dwGoodsID=CGoodsID::GOODSID_ZONE_SIZE*i;
		m_stDBData.stGoodsID[i].dwGameServerIndex=0;
	}
}

CDBData::~CDBData(void)
{
}

CDBData* CDBData::GetInstance()
{
	if(!instance)
	{
		instance = new CDBData();
	}
	return instance;
}
void CDBData::Release()
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}

// ������Ҫ���̵����ݵ�SaveData
void CDBData::GenerateDBData()
{
	EnterCriticalSection(&g_CriticalSectionSaveThread);


	ClearDBCreationPlayer();
	// ����Creation players
	for(list<CCreationPlayerList::tagCreationPlayer>::iterator it=CCreationPlayerList::GetInstance()->GetPlayerList()->begin();
		it!=CCreationPlayerList::GetInstance()->GetPlayerList()->end(); it++)
	{
		CPlayer *pPlayer = (*it).pPlayer;
		BYTE nLocation = (*it).nLocation;

		if(pPlayer && nLocation)
		{
			vector<BYTE> vBuf;
			long lPos=0;

			pPlayer->AddToByteArray(&vBuf);
			CPlayer* pClonePlayer=new CPlayer;
			pClonePlayer->DecordFromByteArray(&vBuf[0],lPos);

			//CCreationPlayerList::tagCreationPlayer CreationPlayer;
			//CreationPlayer.pPlayer = pClonePlayer;
			//CreationPlayer.nLocation = nLocation; 

			// append this to DB Creation Player List
			AppendDBCreationPlayer(pClonePlayer, nLocation);
		}
	}

	ClearDBDeletionPlayer();
	// ����Deletion players
	for(list<CDeletionPlayerList::tagDeletionPlayer>::iterator it=CDeletionPlayerList::GetInstance()->GetPlayerList()->begin();
		it!=CDeletionPlayerList::GetInstance()->GetPlayerList()->end(); it++)
	{
//		DWORD nPlayerID = (*it).nPlayerID;
//		time_t tDelDate = (*it).tDelDate;
		AppendDBDeletionPlayer((*it).nPlayerID, (*it).tDelDate);
	}

	ClearDBOnlinePlayer();
	// ����online players
	for (list<CPlayer*>::iterator it=COnlinePlayerList::GetInstance()->GetPlayerList()->begin();
		it!=COnlinePlayerList::GetInstance()->GetPlayerList()->end(); it++)
	{
		CPlayer* pPlayer=*it;
		if (pPlayer)
		{
			vector<BYTE> byteVec;
			long lPos=0;

			pPlayer->AddToByteArray(&byteVec);
			CPlayer* pClonePlayer=new CPlayer;
			pClonePlayer->DecordFromByteArray(&byteVec[0],lPos);

			AppendDBOnlinePlayer(pClonePlayer);
		}
	}

	ClearDBOfflinePlayer();
	// ����offline players
	for (list<CPlayer*>::iterator it=COfflinePlayerList::GetInstance()->GetPlayerList()->begin();
		it!=COfflinePlayerList::GetInstance()->GetPlayerList()->end(); it++)
	{
		CPlayer* pPlayer=*it;
		if (pPlayer)
		{
			vector<BYTE> byteVec;
			long lPos=0;

			pPlayer->AddToByteArray(&byteVec);
			CPlayer* pClonePlayer=new CPlayer;
			pClonePlayer->DecordFromByteArray(&byteVec[0],lPos);

			AppendDBOfflinePlayer(pClonePlayer);
		}
	}

	// ����playerid
	m_stDBData.nPlayerID = CPlayerID::GetInstance()->GetPlayerID();

	// ����GoodsID
	for(int i=0; i<16; i++)
		m_stDBData.stGoodsID[i] = CGoodsID::GetInstance()->m_stGoodsID[i];

	// ����ȫ�ֽű�����
	//vector<BYTE> byteArray;
	//long pos = 0;

	//GetGeneralVariableList()->AddToByteArray(&byteArray);
	//SAFE_DELETE(m_stSaveData.pGeneralVariableList);
	//m_stSaveData.pGeneralVariableList = new CVariableList;
	//m_stSaveData.pGeneralVariableList->DecordFromByteArray(&byteArray[0], pos);

	LeaveCriticalSection(&g_CriticalSectionSaveThread);
}


// ����ID�Ӵ���������б��еõ�һ�����(����һ��)
CPlayer* CDBData::CloneDBPlayer(UINT nID)
{
	CPlayer* pClonePlayer = NULL;
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);

	// �ȴ���������б����û���ڴ�����������
	if(	!( pClonePlayer  = ClonePlayerFromOnline(nID) ) )
		pClonePlayer = ClonePlayerFromOffline(nID);

	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
	return pClonePlayer;
}

CPlayer* CDBData::ClonePlayerFromOnline(UINT nID)
{
	CPlayer* pClonePlayer = NULL;

	for(list<CPlayer*>::iterator it=m_stDBData.liDBOnlinePlayer.begin();
		it!=m_stDBData.liDBOnlinePlayer.end(); it++)
	{
		if( (*it)->GetID() == nID )
		{
			CPlayer* pPlayer=*it;
			vector<BYTE> vBuf;
			pPlayer->AddToByteArray(&vBuf);
			pClonePlayer=new CPlayer;
			long lPos=0;
			pClonePlayer->DecordFromByteArray(&vBuf[0],lPos);
			break;
		}
	}

	return pClonePlayer;
}

CPlayer* CDBData::ClonePlayerFromOffline(UINT nID)
{
	CPlayer* pClonePlayer = NULL;

	for(list<CPlayer*>::iterator it=m_stDBData.liDBOfflinePlayer.begin();
		it!=m_stDBData.liDBOfflinePlayer.end(); it++)
	{
		if( (*it)->GetID() == nID )
		{
			CPlayer* pPlayer=*it;
			vector<BYTE> vBuf;
			pPlayer->AddToByteArray(&vBuf);
			pClonePlayer=new CPlayer;
			long lPos=0;
			pClonePlayer->DecordFromByteArray(&vBuf[0],lPos);
			break;
		}
	}
	return pClonePlayer;
}

// ��մ����̵�����б�
void CDBData::ClearDBCreationPlayer()
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);
	for(list<CCreationPlayerList::tagCreationPlayer>::iterator it=m_stDBData.liDBCreationPlayer.begin();
		it!=m_stDBData.liDBCreationPlayer.end(); it++)
	{
		CPlayer *pPlayer = (*it).pPlayer;
		SAFE_DELETE(pPlayer);
		m_stDBData.liDBCreationPlayer.erase(it);
	}
	m_stDBData.liDBCreationPlayer.clear();
	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);

}
// ��մ����̵�����б�
void CDBData::ClearDBDeletionPlayer()
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);
//	for(list<CDeletionPlayerList::tagDeletionPlayer>::iterator it=m_stDBData.liDBDeletionPlayer.begin();
//		it!=m_stDBData.liDBDeletionPlayer.end(); it++)
//	{
//		m_stDBData.liDBDeletionPlayer.erase(it);
//	}
	m_stDBData.liDBDeletionPlayer.clear();
	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}
// ��մ����̵�����б�
void CDBData::ClearDBOnlinePlayer()
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);
	for(list<CPlayer*>::iterator it=m_stDBData.liDBOnlinePlayer.begin();
		it!=m_stDBData.liDBOnlinePlayer.end(); it++)
	{
		CPlayer *pPlayer = *it;
		SAFE_DELETE(pPlayer);
		m_stDBData.liDBOnlinePlayer.erase(it);
	}
	m_stDBData.liDBOnlinePlayer.clear();
	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}
// ��մ����̵�����б�
void CDBData::ClearDBOfflinePlayer()
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);
	for(list<CPlayer*>::iterator it=m_stDBData.liDBOfflinePlayer.begin();
		it!=m_stDBData.liDBOfflinePlayer.end(); it++)
	{
		CPlayer *pPlayer = *it;
		SAFE_DELETE(pPlayer);
		m_stDBData.liDBOfflinePlayer.erase(it);
	}
	m_stDBData.liDBOfflinePlayer.clear();
	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}

/*
 * 
 */
void CDBData::AppendDBCreationPlayer(CPlayer* pPlayer, BYTE nLocation)
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);

	CCreationPlayerList::tagCreationPlayer CreationPlayer;
	CreationPlayer.pPlayer = pPlayer;
	CreationPlayer.nLocation=nLocation;
	m_stDBData.liDBCreationPlayer.push_back(CreationPlayer);

	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}

void CDBData::AppendDBDeletionPlayer(DWORD nPlayerID, time_t tDelDate)
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);

	CDeletionPlayerList::tagDeletionPlayer DeletionPlayer = {nPlayerID, tDelDate};
	m_stDBData.liDBDeletionPlayer.push_back(DeletionPlayer);

	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}

// ���һ����ҵ������̵�����б�
void CDBData::AppendDBOnlinePlayer(CPlayer* pPlayer)
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);

	for(list<CPlayer*>::iterator it=m_stDBData.liDBOnlinePlayer.begin();
		it!=m_stDBData.liDBOnlinePlayer.end(); it++)
	{
			if( pPlayer->GetID()== (*it)->GetID() )
			{
				CPlayer* pOldPlayer=*it;
				SAFE_DELETE(pOldPlayer);
				*it=pPlayer;
			}
	}

	m_stDBData.liDBOnlinePlayer.push_back(pPlayer);

	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}

void CDBData::AppendDBOfflinePlayer(CPlayer* pPlayer)
{
	EnterCriticalSection(&g_CriticalSectionSavePlayerList);

	pPlayer->SetSaveDepotFlag(true);
	bool bFoundEqualId=false;

	for(list<CPlayer*>::iterator it=m_stDBData.liDBOfflinePlayer.begin();
		it!=m_stDBData.liDBOfflinePlayer.end(); it++)
	{
		if( pPlayer->GetID()== (*it)->GetID() )
		{
			CPlayer* pOldPlayer=*it;
			SAFE_DELETE(pOldPlayer);
			*it=pPlayer;
		}
	}

	m_stDBData.liDBOfflinePlayer.push_back(pPlayer);

	LeaveCriticalSection(&g_CriticalSectionSavePlayerList);
}