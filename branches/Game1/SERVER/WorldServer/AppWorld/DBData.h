/*\
 *	created:	2005-1-2 9:54:47
 *	filename: 	d:\Project\Server\WorldServer\AppWorld\DBData.h
 *	file path:	d:\Project\Server\WorldServer\AppWorld
 *	file base:	DBData
 *	file ext:	h
 *	author:		Bugs
 *	
 *	purpose:	singleton class
\*/

#pragma once

#ifndef _INC_CDBDATA_H
#define _INC_CDBDATA_H

#include "CreationPlayerList.h"
#include "DeletionPlayerList.h"
#include "OnlinePlayerList.h"
#include "OfflinePlayerList.h"
#include "GoodsID.h"
#include "VarList.h"
#include "PlayerID.h"

class CDBData
{
private:
	CDBData();
	virtual ~CDBData();

public:
	static CDBData* GetInstance();
	static void Release();

private:
	static CDBData *instance;

public:
	struct tagDBData
	{
		// ����������б�
		list<CCreationPlayerList::tagCreationPlayer>	liDBCreationPlayer;
		// ��ɾ������б�
		list<CDeletionPlayerList::tagDeletionPlayer>	liDBDeletionPlayer;

		// ���ߴ���������б�
		list<CPlayer*>	liDBOnlinePlayer;
		// ���ߴ���������б�
		list<CPlayer*>	liDBOfflinePlayer;

		// ��ɫid
		UINT			nPlayerID;

		CGoodsID::tagGoodsID		stGoodsID[16];		// ��Ʒ��ʼID

		CVariableList	*pVariableList;		// ȫ�ֽű�����
	};

public:
	// ������Ҫ���̵����ݵ�SaveData
	/*static*/ void GenerateDBData();

	/*static*/ CPlayer* CloneDBPlayer(UINT nID);				// ����ID�Ӵ���������б��еõ�һ�����(����һ��)

private:
	/*static*/ void AppendDBCreationPlayer(CPlayer* pPlayer, BYTE nLocation);
	/*static*/ void AppendDBDeletionPlayer(DWORD nPlayerID, time_t tDelDate);			// ���һ����ҵ������̵�����б�
	/*static*/ void AppendDBOnlinePlayer(CPlayer* pPlayer);			// ���һ����ҵ������̵�����б�
	/*static*/ void AppendDBOfflinePlayer(CPlayer* pPlayer);			// ���һ����ҵ������̵�����б�

	/*static*/ void ClearDBCreationPlayer();							// ��մ����̵�����б�
	/*static*/ void ClearDBDeletionPlayer();							// ��մ����̵�����б�
	/*static*/ void ClearDBOnlinePlayer();							// ��մ����̵�����б�
	/*static*/ void ClearDBOfflinePlayer();							// ��մ����̵�����б�

	/*static*/ CPlayer* ClonePlayerFromOnline(UINT nID);
	/*static*/ CPlayer* ClonePlayerFromOffline(UINT nID);

private:
	/*static*/ tagDBData m_stDBData;				// ��������(ÿ�����̵�����һ�Σ����̽������ͷ�)
};


#endif // _INC_CDBDATA_H