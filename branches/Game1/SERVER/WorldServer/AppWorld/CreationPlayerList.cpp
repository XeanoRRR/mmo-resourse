#include "StdAfx.h"
#include ".\creationplayerlist.h"



//CCreationPlayerList::CCreationPlayerList(void)
//{
//}
//
//CCreationPlayerList::~CCreationPlayerList(void)
//{
//}
CCreationPlayerList *CCreationPlayerList::instance = NULL;

CCreationPlayerList* CCreationPlayerList::GetInstance()
{
	if(!instance)
	{
		instance = new CCreationPlayerList();
	}
	return instance;
}

void CCreationPlayerList::Release()
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
void CCreationPlayerList::AppendPlayer(CPlayer *pPlayer, BYTE nLocation)
{
	// player==NULL OR location==0 THEN exit
	assert(pPlayer);
	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		// ����б��д��ڸ����
		if( pPlayer->GetID() == (*it).pPlayer->GetID() ) return;
	}
	tagCreationPlayer CreationPlayer = {pPlayer, nLocation};
	GetPlayerList()->push_back(CreationPlayer);
}
void CCreationPlayerList::DeletePlayer(CPlayer *pPlayer)
{
	if (!pPlayer) return;

	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if (pPlayer->GetID() == (*it).pPlayer->GetID())
		{
			CPlayer* pTemp = (*it).pPlayer;
			SAFE_DELETE(pTemp);
			GetPlayerList()->erase(it);
			DelPlayerList(pPlayer->GetName());
		}
	}
}
/*
 * ����б��и�Name�Ƿ�ʹ����
 */
bool CCreationPlayerList::IsNameExist(LPCSTR szName)
{
	// Name ���� NULL
	assert(szName);

	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		// �ҵ���ʹ�ù�������
		if( !strcmpi((*it).pPlayer->GetName(), szName) ) 
		{
			return true;
		}
	}
	return false;
}

/*
// 0x 0 1 2 3 ... 7
//    1 2 3 reserve
// 1��2��3��ʾ�ɷ��ɫ������λ�ӣ�����һ���ֽڣ�8bit����1���Ѿ�ʹ�ã�0û��ʹ��
// res�ӵ͵��ߵ�0��1bit��ʾλ��1��1��ʾλ��2��2��ʾλ��3��3-7����
// ���磺 0x00       0x01        0x02         0x05        0x06
//     0000 0000  0000 0001   0000 0010    0000 0101   0000 0111
//     ȫ��λ     λ��1����   λ��2����    λ��1��3��  ȫ��
 */
BYTE CCreationPlayerList::GetLocationStateInCdkey(LPCSTR szCdkey)
{
	assert(szCdkey);
	
	// res | (1/2/4)  001 010 100
	BYTE res = 0x00;

	// ��ȡһ��cdkey�����ԵĴ�������ɫ
	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( !strcmpi((*it).pPlayer->GetAccount(), szCdkey) )
		{
			//if( (*it).nLocation >0 || (*it).nLocation<4 )
			switch((*it).nLocation) 
			{
			case 1:
				res |= 1;
				break;
			case 2:
				res |= 2;
				break;
			case 3:
				res |= 4;
				break;
			default:
				res &= 0x07; // 0000 0111
			}
		}
	}
	return res;
}

/*
 * ��ȡͬһ�ʻ��½�ɫ������
 */
BYTE CCreationPlayerList::GetPlayerCountInCdkey(LPCSTR szCdkey)
{
	// һ���ʻ������Ҹ�����0��3
	assert(szCdkey);

	UINT8 res = 0;
	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( !stricmp((*it).pPlayer->GetAccount(), szCdkey) )
		{
			res++;
		}
	}
	return res;
}

/*
 * ����һ���ʻ������Խ�ɫ���飨size:0-3)
 */
vector<CCreationPlayerList::tagCreationPlayer> CCreationPlayerList::GetPlayerVectorByCdkey(LPCSTR szCdkey)
{
	vector<tagCreationPlayer> vCreationPlayer;
	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( !stricmp((*it).pPlayer->GetAccount(), szCdkey) )
		{
			vCreationPlayer.push_back(*it);
		}
	}
	return vCreationPlayer;
}

/*
* ͨ��PlayerID��PlayerList����һ��Player
*/
CPlayer* CCreationPlayerList::GetPlayerByID(UINT nID)
{
	CPlayer* pPlayer = NULL;
	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( (*it).pPlayer->GetID() == nID )
		{
			pPlayer = (*it).pPlayer;
			break;
		}
	}
	return pPlayer;
}

/*
* ͨ��PlayerID����һ��Player
*/
CPlayer* CCreationPlayerList::ClonePlayer(UINT nID)
{
	CPlayer* pClonePlayer = NULL;
	for(list<tagCreationPlayer>::iterator it=GetPlayerList()->begin(); it!=GetPlayerList()->end(); it++)
	{
		if( (*it).pPlayer->GetID() == nID )
		{
			pClonePlayer = new CPlayer;
			long lPos=0;
			CPlayer* pPlayer = (*it).pPlayer;
			vector<BYTE> vBuf;
			pPlayer->AddToByteArray(&vBuf);
			pClonePlayer->DecordFromByteArray(&vBuf[0],lPos);
			break;
		}
	}
    return pClonePlayer;
}