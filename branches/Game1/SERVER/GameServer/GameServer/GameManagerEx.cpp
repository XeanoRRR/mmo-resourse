//=============================================================================
/**
 *  file: GameManager.cpp
 *
 *  Brief:��Ϸʵ��Ĺ����࣬�����¼�����Ϣ
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-5-17
 */
//=============================================================================
#include "StdAfx.h"
#include "Game.h"
#include "GameManager.h"
#include "appserver/BaseObject.h"
#include "appserver\Player.h"
#include "appserver/monster.h"
#include "appserver/ServerRegion.h"
#include "appserver/goods/cgoods.h"
#include "appserver/collection.h"
#include "../setup/GlobeSetup.h"
#include "..\nets\netserver\message.h"
#include "..\..\GameServer\GameServer\GameManager.h"
#include "..\AppServer\RgnManager.h"
#include "..\AppServer\citygate.h"

//����һ��CMonster*
CBaseObject* GameManager::CreateObject(long typeID,const CGUID& guid, const char *pInfo)
{
	int *nNum = 0;
	CBaseObject* pObject = NULL;

	switch(typeID)
	{
	case TYPE_GOODS:		// ��Ʒ
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_GoodsList.end() == m_GoodsList.find(guid));
			pObject = MP_NEW CGoods;
			m_GoodsList[guid] = pObject;
		}
		break;

	case TYPE_COLLECTION:
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_CollectionList.end() == m_CollectionList.find(guid));

			pObject = MP_NEW CCollection;
			m_CollectionList[guid] = pObject;
		}
		break;

	case TYPE_MONSTER:		// ����
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_MonsterList.end() == m_MonsterList.find(guid));

			pObject = MP_NEW CMonster;
			m_MonsterList[guid] = pObject;
		}
		break;

	case TYPE_PLAYER:		// ���
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_PlayerList.end() == m_PlayerList.find(guid));

			pObject = MP_NEW  CPlayer;
			m_PlayerList[guid] = pObject;
		}
		break;

	case TYPE_REGION:		// ��ͼ
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_RegionList.end() == m_RegionList.find(guid));

			throw;
			//pObject = MP_NEW CRegion;
			//m_RegionList[guid] = pObject;
		}
		break;

	case TYPE_BUILD:
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_BuildList.end() == m_BuildList.find(guid));

			pObject = MP_NEW  CBuild;
			m_BuildList[guid] = pObject;
		}
		break;

	case TYPE_CITYGATE:
		{
			//! �������ʧ�ܣ�˵��Ϊͬһ��GUID�Ѿ�������һ���ڴ棬�ٴη��������ڴ�й¶
			assert(m_GateList.end() == m_GateList.find(guid));

			pObject = MP_NEW CGate;
			m_GateList[guid] = pObject;
		}
		break;

	default:
		return NULL;
	}

	pObject->SetExID(guid);
	pObject->SetType(typeID);

	return pObject;
}

void GameManager::FreeObject(CBaseObject** ppObj)
{
	if (NULL == ppObj)
	{
		return;
	}
	if (NULL == *ppObj)
	{
		return;
	}
	CBaseObject *pObj = *ppObj;
	
	const CGUID&	guid	= pObj->GetExID();

	switch(pObj->GetType())
	{
	case TYPE_GOODS:		// ��Ʒ
		{
			m_GoodsList.erase(guid);
			MP_DELETE(pObj);
		}
		break;

	case TYPE_COLLECTION:
		{
			m_CollectionList.erase(guid);
			MP_DELETE(pObj);
		}
		break;

	case TYPE_MONSTER:		// ����
		{
			m_MonsterList.erase(guid);
			MP_DELETE(pObj);
		}
		break;

	case TYPE_PLAYER:		// ���
		{
			m_PlayerList.erase(guid);
			MP_DELETE(pObj);
		}
		break;

	case TYPE_REGION:		// ��ͼ
		{
			m_RegionList.erase(guid);
			throw;
			//MP_DELETE(pObj);
		}
		break;

	case TYPE_BUILD:
		{
			m_BuildList.erase(guid);
			MP_DELETE(pObj);
		}
		break;

	case TYPE_CITYGATE:
		{
			m_GateList.erase(guid);
			MP_DELETE(pObj);
		}
		break;

	default:
		assert(false);
	}

	*ppObj = NULL;
}