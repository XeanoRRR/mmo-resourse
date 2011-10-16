#include "StdAfx.h"
#include ".\loginplayerlist.h"

//CLoginPlayerList::CLoginPlayerList(void)
//{
//}
//
//CLoginPlayerList::~CLoginPlayerList(void)
//{
//}

CLoginPlayerList *CLoginPlayerList::instance = NULL;

CLoginPlayerList* CLoginPlayerList::GetInstance()
{
	if(!instance)
	{
		instance = new CLoginPlayerList();
	}
	return instance;
}

void CLoginPlayerList::Release()
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}
/*
* ׷��һ��Player��PlayerList
*/
void CLoginPlayerList::AppendPlayer(CPlayer *pPlayer)
{
	if(!pPlayer) return;

	for(list<CPlayer*>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		// ����б��д��ڸ���ң������µ��滻���б��еġ�
		if( pPlayer->GetID() == (*it)->GetID() )
		{
			CPlayer *pTemp = *it;
			SAFE_DELETE(pTemp);
			*it = pPlayer;
		}
	}

	//add to player list in back
	GetPlayerList()->push_back(pPlayer);

	// pronounce server managerment this palyer has been added
	AddPlayerList(pPlayer->GetName());
}

/*
* ��PlayerList�Ƴ�һ��player	
*/
void CLoginPlayerList::RemovePlayer(CPlayer *pPlayer)
{
	if (!pPlayer) return;
	//remove the player from playerlist
	GetPlayerList()->remove(pPlayer);

	DelPlayerList(pPlayer->GetName());
}

/*
* ��PlayerListɾ��һ��Player
*/
void CLoginPlayerList::DeletePlayer(CPlayer *pPlayer)
{
	if (!pPlayer) return;

	for(list<CPlayer*>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if (pPlayer->GetID() == (*it)->GetID())
		{
			CPlayer* pTemp = *it;
			SAFE_DELETE(pTemp);
			GetPlayerList()->erase(it);
			DelPlayerList(pPlayer->GetName());
		}
	}
}

/*
* ͨ��PlayerID��PlayerList����һ��Player
*/
CPlayer* CLoginPlayerList::GetPlayerByID(UINT nID)
{
	CPlayer* pPlayer = NULL;
	for(list<CPlayer*>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( (*it)->GetID() == nID )
		{
			pPlayer = *it;
			break;
		}
	}
	return pPlayer;
}

/*
* ͨ��PlayerName��PlayerList����һ��PlayerID
*/
UINT CLoginPlayerList::GetPlayerIDByName(LPCSTR szName)
{
	long nID = 0;

	for(list<CPlayer*>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		// ��PlayerList�ҵ�������ͬ��
		if( !strcmpi((*it)->GetName(), szName) )
		{
			nID = (*it)->GetID();
			break;
		}
	}
	return nID;
}

/*
* ͨ��PlayerID����һ��Player
*/
CPlayer* CLoginPlayerList::ClonePlayer(UINT nID)
{
	CPlayer* pClonePlayer = NULL;
	for(list<CPlayer*>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( (*it)->GetID() == nID )
		{
			pClonePlayer = new CPlayer;
			long lPos=0;
			CPlayer* pPlayer = *it;
			vector<BYTE> vBuf;
			pPlayer->AddToByteArray(&vBuf);
			pClonePlayer->DecordFromByteArray(&vBuf[0],lPos);
			break;
		}
	}
	return pClonePlayer;
}