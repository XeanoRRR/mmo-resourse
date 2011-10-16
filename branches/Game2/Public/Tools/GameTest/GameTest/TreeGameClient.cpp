//=============================================================================
/**
*  file: TreeGameClient.cpp
*
*  Brief:���ؼ���������ʾ��Ϸ�����е�GameClient�����Ӷ���
*
*  Authtor:wangqiao
*	
*	Date:2007-7-25
*/
//=============================================================================


#include "stdafx.h"
#include "GameTest.h"
#include "TreeGameClient.h"
#include "WndMsgDef.h"
#include ".\treegameclient.h"

#include "..\App\Player.h"
#include "..\App\ClientRegion.h"
#include "..\App\GameClient.h"

#include "SkillInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTreeGameClient, CTreeCtrl)
CTreeGameClient::CTreeGameClient()
{
	m_pSelNode = new tagNodeInfo;
	memset(m_pSelNode,0,sizeof(tagNodeInfo));
}

CTreeGameClient::~CTreeGameClient()
{
	SAFE_DELETE(m_pSelNode);

	list<tagClientInfo*>::iterator pClient = m_pGameClientNode.begin();
	for (;pClient != m_pGameClientNode.end();)
	{
		tagNodeInfo *pGoods = (*pClient)->m_GoodsInfo;
		tagNodeInfo *pMonster = (*pClient)->m_MonsterInfo;
		tagNodeInfo *pMainPlayer = (*pClient)->m_MainPlayerInfo;
		tagNodeInfo *pRegion = (*pClient)->m_RegionInfo;
		tagNodeInfo *pSkill = (*pClient)->m_SkillInfo;

		SAFE_DELETE(pGoods);
		SAFE_DELETE(pMonster);
		SAFE_DELETE(pMainPlayer);
		SAFE_DELETE(pRegion);
		SAFE_DELETE(pSkill);

		tagClientInfo* pC = (*pClient);
		++pClient;
		SAFE_DELETE(pC);
	}
	m_pGameClientNode.clear();
}


BEGIN_MESSAGE_MAP(CTreeGameClient, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	//	ON_WM_RBUTTONDOWN()
	//ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CTreeGameClient ��Ϣ�������


BOOL CTreeGameClient::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CTreeCtrl::OnWndMsg(message, wParam, lParam, pResult);
}


//�������ݽڵ�
HTREEITEM CTreeGameClient::InsertDataItem(HTREEITEM hParentItem,tagClientInfo* pNodeInfo)
{
	if(NULL==pNodeInfo)	return NULL;

	HTREEITEM hResult = InsertItem(pNodeInfo->strShowName,hParentItem);
	SetItemData(hResult,(DWORD)pNodeInfo);
	return hResult;
}


//����GameClient
void CTreeGameClient::InsertClientItem(CGameClient* pClient)
{
	if(NULL==pClient)	return;
	// new һ���µĿͻ���
	tagClientInfo * pNewClient = new tagClientInfo;
	pNewClient->lClientID = pClient->GetSocketID();
	pNewClient->lType	= 0;
	sprintf(pNewClient->strShowName,"�ͻ��ˣ�%s" , pClient->GetCDKey());
	//������״�ڵ�
	HTREEITEM hRootItem = GetRootItem();
	HTREEITEM hClientItem = InsertClientDataItem(hRootItem,pNewClient);
	HTREEITEM hMainPlayer = InsertItem("�����",hClientItem);
	HTREEITEM hRegionItem = InsertItem("����",hClientItem);
	//region
	pNewClient->m_RegionInfo->lClientID = pNewClient->lClientID;
	pNewClient->m_RegionInfo->lType = TYPE_REGION;
	if ( pClient->GetClientRegion() )
		sprintf(pNewClient->m_RegionInfo->strShowName,pClient->GetClientRegion()->GetName());
	else
		sprintf(pNewClient->m_RegionInfo->strShowName ,"��");
	InsertRegionDataItem(hRegionItem,pNewClient);
	//monster
	pNewClient->m_MonsterInfo->lClientID = pNewClient->lClientID;
	pNewClient->m_MonsterInfo->lType = TYPE_MONSTER;
	sprintf(pNewClient->m_MonsterInfo->strShowName , "����");
	InsertMonsterDataItem(hRegionItem,pNewClient);
	//goods
	pNewClient->m_GoodsInfo->lClientID = pNewClient->lClientID;
	pNewClient->m_GoodsInfo->lType = TYPE_GOODS;
	sprintf(pNewClient->m_GoodsInfo->strShowName , "��Ʒ");
	InsertGoodsDataItem(hRegionItem,pNewClient);
	//mainplayer
	pNewClient->m_MainPlayerInfo->lClientID = pNewClient->lClientID;
	pNewClient->m_MainPlayerInfo->lType = TYPE_PLAYER;
	sprintf(pNewClient->m_MainPlayerInfo->strShowName , pClient->GetMainPlayer()->GetName());
	InsertMainPlayerDataItem(hMainPlayer,pNewClient);
	//skill
	pNewClient->m_SkillInfo->lClientID = pNewClient->lClientID;
	pNewClient->m_SkillInfo->lType = TYPE_SKILL;
	sprintf(pNewClient->m_SkillInfo->strShowName , "����");
	InsertSkillDataItem(hMainPlayer,pNewClient);
	//
	m_pGameClientNode.push_back(pNewClient);
}
void CTreeGameClient::InsertShapeItem(CGameClient* pClient,CShape* pShape)
{
	HTREEITEM hTRegion = FindClientRegionItem(pClient->GetSocketID(),pClient->GetClientRegion()->GetResourceID());

	//tagTreeNodeInfo* pGameClientNodeInfo = new tagTreeNodeInfo;
	//pGameClientNodeInfo->lType = (OBJECT_TYPE)pShape->GetType();
	////	pClientTreeNodeInfo->lID= pShape->GetRegionID();
	////	pClientTreeNodeInfo->lClientID = pClient->GetSocketID();
	//sprintf(pGameClientNodeInfo->strShowName,"������%s",pShape->GetName());

	//InsertDataItem(hTRegion,pGameClientNodeInfo);
}

HTREEITEM CTreeGameClient::FindClientItem(long lClientID)
{
	HTREEITEM hRootItem = GetRootItem();
	return FindChildItem(hRootItem,0,lClientID);
}

HTREEITEM CTreeGameClient::FindClientRegionItem(long lClientID,long lRegionID)
{	
	HTREEITEM hTClient = FindClientItem(lClientID);
	return FindChildItem(hTClient,TYPE_REGION,lRegionID);
}
HTREEITEM CTreeGameClient::FindShapeItem(long lClientID,long lRegionID,long lType,long lShapeID)
{
	HTREEITEM hTRegion = FindClientRegionItem(lClientID,lRegionID);
	return FindChildItem(hTRegion,lType,lShapeID);
}
HTREEITEM CTreeGameClient::FindChildItem(HTREEITEM hParent,long lType,long lClientID)
{
	HTREEITEM hTChild=NULL;
	if (ItemHasChildren(hParent))
	{
		hTChild = GetNextItem(hParent,TVGN_CHILD);
		while (hTChild != NULL)
		{
			tagClientInfo* pNodeInfo = (tagClientInfo*)GetItemData(hTChild);
			if(pNodeInfo->lType == lType && pNodeInfo->lClientID == lClientID)
				break;
			hTChild = GetNextItem(hTChild, TVGN_NEXT);
		}
	}
	return hTChild;
}


//ɾ���ڵ㲢ɾ���û�����
BOOL CTreeGameClient::DelAllChildItem(HTREEITEM hParentItem)
{
	if(NULL==hParentItem)	return FALSE;
	// ɾ������hParentItem���ӽڵ�
	if (ItemHasChildren(hParentItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetChildItem(hParentItem);
		while (hChildItem != NULL)
		{
			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
			DelAllChildItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
	DeleteItem(hParentItem);
	return TRUE;
}

//ɾ���ڵ�
BOOL CTreeGameClient::DelClientItem(long lClientID)
{
	HTREEITEM hClientItem = FindClientItem(lClientID);
	DelAllChildItem(hClientItem);			//ɾ����״�ڵ�
	DelGameClient(lClientID);				//ɾ��list��
	return TRUE;
}


//ɾ��list����gameclient
void CTreeGameClient::DelGameClient(long lClient)
{
	list<tagClientInfo*>::iterator it = m_pGameClientNode.begin();
	for (; it != m_pGameClientNode.end();++it)
	{
		if ( (*it)->lClientID == lClient )
		{
			tagNodeInfo* p = (*it)->m_GoodsInfo;
			delete p;
			p = (*it)->m_MainPlayerInfo;
			delete p;
			p = (*it)->m_MonsterInfo;
			delete p;
			p = (*it)->m_RegionInfo;
			delete p;
			p = (*it)->m_SkillInfo;
			delete p;
			tagClientInfo* pp = (*it);
			delete pp;
			m_pGameClientNode.erase(it);
			return;
		}
	}
}
BOOL CTreeGameClient::DelShapeItem(long lClientID,long lRegionID,long lShapeType,long lShapeID)
{
	HTREEITEM hShapeItem = FindShapeItem(lClientID,lRegionID,lShapeType,lShapeID);
	DelAllChildItem(hShapeItem);
	return TRUE;
}
void CTreeGameClient::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	HTREEITEM hSelectItem = GetSelectedItem();
	tagNodeInfo* pNodeInfo = (tagNodeInfo*)GetItemData(hSelectItem);
	if(pNodeInfo)
	{
		memcpy(m_pSelNode,pNodeInfo,sizeof(tagNodeInfo));
		::PostMessage(this->GetParent()->m_hWnd,GAME_CLIENT_TREE_SELCHANGED,NULL,NULL);
	}
	else
	{
		memset(m_pSelNode,0,sizeof(tagNodeInfo));
	} 
}


//======================================================================================================================================
//client �ڵ�
HTREEITEM CTreeGameClient::InsertClientDataItem(HTREEITEM hParent,tagClientInfo *pNode)
{
	if ( pNode != NULL)
	{
		HTREEITEM	hRet = InsertItem(pNode->strShowName,hParent);
		SetItemData(hRet,(DWORD_PTR)pNode);
		return hRet;
	}
	return NULL;
}
//region
HTREEITEM CTreeGameClient::InsertRegionDataItem(HTREEITEM hP,tagClientInfo* pNode)
{
	if ( pNode != NULL)
	{
		HTREEITEM hRet = InsertItem(pNode->m_RegionInfo->strShowName,hP);
		SetItemData(hRet,(DWORD_PTR)pNode->m_RegionInfo);
		return hRet;
	}
	return NULL;
}
//goods
HTREEITEM CTreeGameClient::InsertGoodsDataItem(HTREEITEM h,tagClientInfo *node)
{
	if ( node != NULL)
	{
		HTREEITEM ret = InsertItem(node->m_GoodsInfo->strShowName,h);
		SetItemData(ret,(DWORD_PTR)node->m_GoodsInfo);
		return ret;
	}
	return NULL;
}
//monster
HTREEITEM CTreeGameClient::InsertMonsterDataItem(HTREEITEM h,tagClientInfo *node)
{
	if ( node != NULL)
	{
		HTREEITEM ret = InsertItem(node->m_MonsterInfo->strShowName,h);
		SetItemData(ret,(DWORD_PTR)node->m_MonsterInfo);
		return ret;
	}
	return NULL;
}
//mainplayer
HTREEITEM CTreeGameClient::InsertMainPlayerDataItem(HTREEITEM h, tagClientInfo* node)
{
	if ( node != NULL)
	{
		HTREEITEM ret = InsertItem( node->m_MainPlayerInfo->strShowName, h );
		SetItemData(ret,(DWORD_PTR)node->m_MainPlayerInfo);
		return ret;
	}
	return NULL;
}
//skill
HTREEITEM CTreeGameClient::InsertSkillDataItem(HTREEITEM h, tagClientInfo *node)
{
	if ( node != NULL)
	{
		HTREEITEM ret = InsertItem(node->m_SkillInfo->strShowName,h);
		SetItemData(ret,(DWORD_PTR)node->m_SkillInfo);
		return ret;
	}
	return NULL;
}