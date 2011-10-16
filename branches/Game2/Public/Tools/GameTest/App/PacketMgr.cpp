/**
============================================================
*  Copyright (c) 2010
*  All rights reserved
*  �ļ����ƣ�CMainFrame.cpp
*  ���ܣ��̶ȵ��ĵ����
*  ��ǰ�汾��1.1.0
*  ���ߣ�������
*  ������ڣ�2010��8��23�� 
*  ȡ���汾��1.0.0
*  ԭ���ߣ�������
*  ������ڣ�2010��8��23��
*
============================================================
*/



#include "stdafx.h"
#include "PacketMgr.h"
#include "Goods.h"
#include "GoodsMgr.h"
#include "Player.h"
#include "DepotMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPacketMgr::CPacketMgr(CGoodsMgr* pFather)
{
	m_pFather = pFather;

	m_CoteBussinesPack = new tagCote;
	for (int i = 0;i<COTE_NUM;i++)
	{
		m_Cote[i] = new tagCote;
		m_Cote[i]->pGoods = NULL;
	}
}

CPacketMgr::~CPacketMgr()
{
	SAFE_DELETE(m_CoteBussinesPack);
	for (int i = 0;i<COTE_NUM;i++)
	{
		if (m_Cote[i]->pGoods != NULL)
			SAFE_DELETE(m_Cote[i]->pGoods);
		SAFE_DELETE(m_Cote[i]);
	}
}

void CPacketMgr::Release()
{
	//��ձ���CGoodsָ��
	for(list<tagGoods>::iterator it = m_listGoods.begin(); it !=m_listGoods.end();it++)
	{	
		SAFE_DELETE(it->pGoods);
	}

	//���������Ʒ��������
	m_listGoods.clear(); 

	//��յ��߱���
	for (int i = 0;i<COTE_NUM;i++)  //����ÿ������
	{
		if (m_Cote[i]!=NULL)
		{
			if( m_Cote[i]->pGoods != NULL )
			{
				if( !m_Cote[i]->m_listCoteGoods.empty())
				{
					list<tagGoods>::iterator it = m_Cote[i]->m_listCoteGoods.begin();
					for (;it!=m_Cote[i]->m_listCoteGoods.end();it++)
					{
						SAFE_DELETE( it->pGoods );		//���ÿ�����������Ʒ����Ϣ
					}
					m_Cote[i]->m_listCoteGoods.clear();	
				}
				SAFE_DELETE(m_Cote[i]->pGoods);
			}
		//	SAFE_DELETE(m_Cote[i]);
		}
	}

	/*if( m_CoteBussinesPack != NULL )
	{
	if(m_CoteBussinesPack->pGoods!=NULL)
	{
	if (!m_CoteBussinesPack->m_listCoteGoods.empty())
	{
	list<tagGoods>::iterator it = m_CoteBussinesPack->m_listCoteGoods.begin();
	for (;it!=m_CoteBussinesPack->m_listCoteGoods.end();it++)
	{
	SAFE_DELETE(it->pGoods);
	}
	m_CoteBussinesPack->m_listCoteGoods.clear();
	}
	SAFE_DELETE(m_CoteBussinesPack->pGoods);
	}
	//
	SAFE_DELETE(m_CoteBussinesPack);
	}*/
}

tagGoods* CPacketMgr:: GetGoodsByOrgName(const char* strOrgName)
{
	//ͨ��ԭʼ���õ�һ��Goods
	//CGoods* pGoods = NULL;
	//ԭʼ����
	/*list<tagGoods>::iterator it = m_listGoods.begin();
	for( ; it != m_listGoods.end(); it++)
	{
	if( !strcmp(it->pGoods->GetOrginName(), strOrgName) )
	{
	return &(*it);
	}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
	//���ӱ����в���
	list<tagGoods>* pGoodList = GetSubGoodslist(i);
	if (pGoodList)
	{
	list<tagGoods>::iterator GoodsIter=pGoodList->begin();
	for(;GoodsIter!=pGoodList->end();GoodsIter++)
	{
	if( !strcmp(GoodsIter->pGoods->GetOrginName(), strOrgName) )
	return &(*GoodsIter);
	}
	}
	}
	return NULL;*/


	//ԭʼ����
	list<tagGoods>::iterator it = m_listGoods.begin();
	for( ; it != m_listGoods.end(); it++)
	{
		if( !strcmp(it->pGoods->GetOrginName(), strOrgName) )
		{
			return &(*it);
		}
	}
	//��ѯ�ӱ���
	/*for (int i = 0;i<COTE_NUM;i++)
	{	
	//���ӱ����в���
	list<tagGoods>* pGoodList = GetSubGoodslist(i);
	if (pGoodList)
	{
	list<tagGoods>::iterator GoodsIter=pGoodList->begin();
	for(;GoodsIter!=pGoodList->end();GoodsIter++)
	{
	if( !strcmp(GoodsIter->pGoods->GetOrginName(), strOrgName) )
	return &(*GoodsIter);
	}
	}
	}*/
	return NULL;
}

void CPacketMgr::DecodeFromDataBlock(DBReadSet& dbRead,bool bExData)
{
	bool bVal = false;

	//	������
	long lNum = dbRead.GetLongFromByteArray();
	if(lNum>0)
	{
		for(int i=0;i<lNum;++i)
		{
			tagGoods stGoods;
			stGoods.bChange = dbRead.GetByteFromByteArray() != 0;
			stGoods.lNum = dbRead.GetWordFromByteArray();
			stGoods.lPos = dbRead.GetByteFromByteArray();
			stGoods.lType = PEI_PACKET;
			stGoods.pGoods = new CGoods;
			bVal=stGoods.pGoods->DecordFromDataBlock(dbRead,bExData);

			if(bVal)
			{
				m_listGoods.push_back(stGoods);
				m_setPacketEmptyPos.insert(stGoods.lPos);
				stGoods.pGoods = NULL;
			}
			else
			{
				SAFE_DELETE(stGoods.pGoods);
			}
		}
	}

	//	������λ
	BYTE num = dbRead.GetByteFromByteArray();
	for(int i=0;i<num;i++)
	{
		CGoods* pGoods = new CGoods;
		bVal = pGoods->DecordFromDataBlock(dbRead,bExData);
		BYTE bIndex = dbRead.GetByteFromByteArray();
		if(bVal&&m_Cote[bIndex]!=NULL)
		{
			m_Cote[bIndex]->pGoods = pGoods;
			pGoods = NULL;
		}
		else
			SAFE_DELETE(pGoods);
	}

	//ÿ���ӱ����ڵ���Ʒ
	for (int i=0;i<COTE_NUM;++i)
	{
		lNum = dbRead.GetLongFromByteArray();
		if(lNum>0)
		{
			for(int j=0;j<lNum;j++)
			{
				tagGoods stGoods;
				stGoods.bChange = dbRead.GetByteFromByteArray() == 0? false:true;
				stGoods.lNum = dbRead.GetWordFromByteArray();
				stGoods.lPos = dbRead.GetByteFromByteArray();
				stGoods.lType = PEI_PACK1 + i;
				stGoods.pGoods = new CGoods;
				bVal = stGoods.pGoods->DecordFromDataBlock(dbRead,bExData);

				if(bVal)
				{
					m_Cote[i]->m_listCoteGoods.push_back(stGoods);
					m_Cote[i]->m_setCoteHaveGoods.insert(stGoods.lPos);
					stGoods.pGoods = NULL;
				}
				else
				{
					SAFE_DELETE(stGoods.pGoods);
				}
			}
		}
	}
}


void CPacketMgr::DecodeBusinessFromDataBlock(DBReadSet& dbRead,bool bExData)
{
	bool bVal = false;

	BYTE bIsHaveBusinessPack = dbRead.GetByteFromByteArray();
	if( bIsHaveBusinessPack >0 )
	{
		CGoods* pBusinessPackGoods = new CGoods;
		bVal = pBusinessPackGoods->DecordFromDataBlock(dbRead,bExData);
		if( bVal && m_CoteBussinesPack != NULL )
		{
			m_CoteBussinesPack->pGoods = pBusinessPackGoods;							//pGoods�а����˵��߱����Ĵ�С������
			pBusinessPackGoods = NULL;		
			//##��ʼ�����ʱ������ÿ����Ʒ����Ϣ
			long lNum=dbRead.GetLongFromByteArray();	// ��Ʒ����
			if (lNum>0)
			{
				for(int j=0;j<lNum;j++)
				{
					tagGoods stGood;
					stGood.bChange=dbRead.GetByteFromByteArray() != 0;
					stGood.lNum = dbRead.GetWordFromByteArray();
					stGood.lPos = dbRead.GetByteFromByteArray();
					stGood.lType = PEI_BUSSINESPACK;
					stGood.pGoods = new CGoods;
					bVal = stGood.pGoods->DecordFromDataBlock(dbRead,bExData);
					if(bVal)
					{
						m_CoteBussinesPack->m_listCoteGoods.push_back(stGood);
						stGood.pGoods = NULL;
					}
					else
					{
						SAFE_DELETE(stGood.pGoods);
					}
				}
			}
		}
		else
		{
			SAFE_DELETE(pBusinessPackGoods);
		}
	}
}

CGoods* CPacketMgr::GetGoodsByGUID(const CGUID& guID,int *pPos,long* lExtendID)
{
	CGoods* pGoods = NULL;

	list<tagGoods>::iterator it = m_listGoods.begin();
	for(;it != m_listGoods.end(); ++it )
	{
		pGoods = it->pGoods;
		if(pGoods && pGoods->GetExID() == guID)
		{
			if(pPos)
				pPos[0] = it->lPos;
			if(lExtendID)
				(*lExtendID) = PEI_PACKET;
			return pGoods;
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ�����λ�ϲ���
		pGoods = GetSubPGoods(i);
		if (pGoods&&pGoods->GetExID()==guID)
		{
			if(pPos)pPos[0]=i;
			if(lExtendID) 
				(*lExtendID) = PEI_SUBPACK;
			return pGoods;
		}
		//���ӱ����в���
		list<tagGoods>* pGoodList = GetSubGoodslist(i);
		if (pGoodList!=NULL)
		{
			list<tagGoods>::iterator it =pGoodList->begin();
			for(;it !=pGoodList->end();it++)
			{
				pGoods = it->pGoods;
				if(pGoods && pGoods->GetExID() == guID)
				{
					if(pPos)pPos[0]= it->lPos;
					if(lExtendID)
						(*lExtendID) = (PEI_PACK1 + i);
					return pGoods;
				}
			}
		}
	}

	if( m_CoteBussinesPack != NULL && m_CoteBussinesPack->pGoods != NULL )
	{
		if (m_CoteBussinesPack->pGoods->GetExID()==guID)
		{
			return m_CoteBussinesPack->pGoods;
		}
		list<tagGoods>* pGoodList = &m_CoteBussinesPack->m_listCoteGoods;
		if (pGoodList)
		{
			list<tagGoods>::iterator GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				pGoods = GoodsIter->pGoods;
				if(pGoods && pGoods->GetExID() == guID)
				{
					if(pPos!=NULL) pPos[0]=GoodsIter->lPos;
					if (lExtendID!=NULL) (*lExtendID) = PEI_BUSSINESPACK;
					return pGoods;
				}
			}
		}
	}

	return NULL;
}

CGoods* CPacketMgr::FindGoodsByOrgName(const char* name)
{
	//�ڱ����в���
	list<tagGoods>* pGoodList = GetGoodsList();
	if ( pGoodList != NULL )
	{
		list<tagGoods>::iterator GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			if( !strcmp( GoodsIter->pGoods->GetOrginName(),name) )
				return /*(CGoods*)*/GoodsIter->pGoods;
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ�����λ�ϲ���
		CGoods* pGoods = GetSubPGoods(i);
		if ( !strcmp( pGoods->GetOrginName(), name) )
		{
			return pGoods;
		}
		//���ӱ����в���
		pGoodList = GetSubGoodslist(i);
		if (pGoodList!=NULL)
		{
			list<tagGoods>::iterator GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				CGoods* pGoods = GoodsIter->pGoods;
				if(pGoods && !strcmp( pGoods->GetOrginName(), name))
				{
					return pGoods;
				}
			}
		}
	}
	return NULL;
}


//��ȡָ����ŵ��ӱ���ָ��λ�õ���Ʒ
tagGoods* CPacketMgr::GetSubGoods(WORD ExtendID,WORD lPos)
{
	if (ExtendID>=0&&ExtendID<=4)
	{
		list<tagGoods>* pLGoods = GetSubGoodslist(ExtendID);
		if (pLGoods==NULL)
		{
			return NULL;
		}
		list<tagGoods>::iterator it  = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==lPos)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

list<tagGoods>* CPacketMgr::GetSubGoodslist(WORD index) 
{
	if (index<0||index>=COTE_NUM)
	{
		return	NULL;
	}
	return &(m_Cote[index]->m_listCoteGoods);
}

//��ȡԭʼ����ָ��λ�õ���Ʒ
tagGoods* CPacketMgr::GetGoods(WORD lPos)
{
	if (lPos>=0&&lPos<=20)
	{
		list<tagGoods>* pLGoods = GetGoodsList();
		if (pLGoods==NULL)
		{
			return NULL;
		}
		list<tagGoods>::iterator it = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==lPos)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

//��ȡָ����ŵ��ӱ������ݻ�
WORD CPacketMgr::GetPackageDimen(WORD index)	
{
	if( index < 0 || index >= COTE_NUM || m_Cote[index] == NULL || m_Cote[index]->pGoods == NULL)
	{ return 0; }
	else if(m_Cote[index]->pGoods != NULL)
	{ return (WORD)m_Cote[index]->pGoods->GetEffectVal(CGoods::GAP_PACK_CAPACITY,1); }				//��ȡ���߱����ĸ�����

	return 0;

}

//�ж��Ƿ����еı�����������
bool CPacketMgr::IsAllPackageFull()
{
	for (int i = 0;i<PACK_NUM;i++)
	{
		long num = GetGoodsList(i)->size();
		long dimension = 0;
		if (i == 0)
			dimension = HOST_GRIDITEM;
		else if (GetSubPGoods(i-1))
		{
			dimension = GetPackageDimen(i-1);
		}
		if (num == dimension)
		{
			return false;
		}
	}
	return true;
}

//��ȡδ����������ţ���ԭʼ������ʼ�ƣ�
WORD CPacketMgr::GetNotFullPackage()
{
	for (int i= 0;i<PACK_NUM;i++)
	{
		long num = GetGoodsList(i)->size();
		long dimension = 0;
		if (i == 0)
			dimension = HOST_GRIDITEM;
		else if (GetSubPGoods(i-1))
		{
			dimension = GetPackageDimen(i-1);
		}
		if (num != dimension)
		{
			return	i;
		}
	}
	return -1;
}

//��ȡĳ����������Ʒ����
list<tagGoods>* CPacketMgr::GetContainerGoodsList(int iIndex)
{
	if( iIndex == PEI_PACKET )
	{
		return &m_listGoods;
	}
	else if( iIndex == PEI_PACK1 || iIndex == PEI_PACK2 || iIndex == PEI_PACK3 || iIndex == PEI_PACK4 || iIndex == PEI_PACK5 )
	{
		return GetSubGoodslist(iIndex - PEI_PACK1);
	}
	/*else*/
	return NULL;
}

//��ȡĳ��ָ�������пո���
DWORD CPacketMgr::GetContainerEmptyPos(int iContainerID)
{
	if( iContainerID == PEI_PACKET )
	{
		for( int i=1;i<=HOST_GRIDITEM;i++)
		{
			if ( m_setPacketEmptyPos.find(i) == m_setPacketEmptyPos.end() )
			{
				return (abs(HOST_GRIDITEM-i));
			}
		}
	}
	if( iContainerID == PEI_PACK1 || iContainerID == PEI_PACK2 || iContainerID == PEI_PACK3 ||
		iContainerID == PEI_PACK4 || iContainerID == PEI_PACK5 || iContainerID == PEI_SUBPACK)
	{
		if( iContainerID == PEI_SUBPACK)
		{
			for( int i=0;i<COTE_NUM;i++)
			{
				if( m_Cote[i] != NULL && m_Cote[i]->pGoods == NULL)
				{
					return (i + PEI_PACK1);
				}
			}
		}
		else
		{
			for( int i=0;i<PACK_KB108_CELL_NUM;i++)
			{
				if( m_Cote[iContainerID - PEI_PACK1] != NULL && m_Cote[iContainerID-PEI_PACK1]->pGoods != NULL  == FALSE)
					return i;
			}
		}
	}
	CDepotMgr CDepotObj;
	DWORD dwNum;
	dwNum=CDepotObj.GetEmptyPos(iContainerID);
	//��λ
	return dwNum;
	//return m_pDepotMgr->GetEmptyPos(iContainerID);
}


//��ȡĳ����������Ʒ��������ԭʼ������
list<tagGoods>* CPacketMgr::GetGoodsList(WORD index)
{
	if (index<0||index>=PACK_NUM)
	{
		return NULL;
	}
	else if(index==0)
	{
		return &m_listGoods;
	}

	return GetSubGoodslist(index-1);
}

//��ȡ����ָ����ŵ��ӱ���
tagCote* CPacketMgr::GetCotePackage(WORD index) 
{
	if (index<0||index>=COTE_NUM)
	{
		return	NULL;
	}
	return m_Cote[index];
}										

CGoods* CPacketMgr::GetSubPGoods(WORD index)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//	�ӱ���
//////////////////////////////////////////////////////////////////////////
void CPacketMgr::SetSubPGoods(WORD index,CGoods* pGoods)
{
	if (index>=0&&index<5&&m_Cote[index]!=NULL)
	{
		m_Cote[index]->pGoods = pGoods;
	}
}

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
tagGoods* CPacketMgr::GetBusinessGoods(WORD wPos)
{
	if (wPos>=0&&wPos<=GRIDITEM&&m_CoteBussinesPack!=NULL)
	{
		list<tagGoods>* pLGoods = &m_CoteBussinesPack->m_listCoteGoods;
		if (pLGoods==NULL)
		{
			return NULL;
		}
		list<tagGoods>::iterator it = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==wPos)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

void CPacketMgr::SetBussinesPack(CGoods*pPackGoods)
{
	if (m_CoteBussinesPack!=NULL&&pPackGoods!=NULL)
	{
		ClearBussinesPackGoods();
		m_CoteBussinesPack->pGoods = pPackGoods;
	}
}

///�������е����ʱ��������ʱ�����CGoods���󣬱����������Ʒ����û�б仯��
bool CPacketMgr::ClearBussinesPackGoods()
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		m_CoteBussinesPack->pGoods = NULL;
		return true;
	}
	return false;
}

void CPacketMgr::ClearAllGoods(long lSocketID)
{
	if( m_pFather == NULL )	return;
	CPlayer* pPlayer = m_pFather->GetOwnerPlayer();

	char strScript[64];

	if( !m_listGoods.empty())
	{
		list<tagGoods>::iterator it =  m_listGoods.begin();
		for( ;it != m_listGoods.end(); ++it)
		{
			CGoods* pGoods = it->pGoods;

			sprintf(strScript,"DelGoods(\"%s\",%d);",pGoods->GetOrginName(),it->lNum);

			CMessage *pMsg = (CMessage *)CBaseMessage::AllocMsg();
			pMsg->Reset();
			pMsg->Init(MSG_C2S_OTHER_TALK);
			pMsg->Add((BYTE)0);
			pMsg->Add((long)RUN_SCRIPT);
			pMsg->Add((long)TYPE_PLAYER);
			pMsg->Add(pPlayer->GetExID());
			pMsg->Add((long)0);
			pMsg->Add(strScript);
			pMsg->SendBySocket(lSocketID);
			CBaseMessage::FreeMsg(pMsg);
		}
	}

	//�ӱ���
	for (int i=0;i<COTE_NUM;i++)
	{
		if( m_Cote[i] != NULL )
		{
			list<tagGoods>::iterator it = m_Cote[i]->m_listCoteGoods.begin();
			for(; it!= m_Cote[i]->m_listCoteGoods.end();++it)
			{
				CGoods* pGoods = it->pGoods;
				sprintf(strScript,"DelGoods(\"%s\",%d);",pGoods->GetOrginName(),it->lNum);
				CMessage *pMsg = (CMessage *)CBaseMessage::AllocMsg();
				pMsg->Reset();
				pMsg->Init(MSG_C2S_OTHER_TALK);
				pMsg->Add((BYTE)0);
				pMsg->Add((long)RUN_SCRIPT);
				pMsg->Add((long)TYPE_PLAYER);
				pMsg->Add(pPlayer->GetExID());
				pMsg->Add((long)0);
				pMsg->Add(strScript);
				pMsg->SendBySocket(lSocketID);
				CBaseMessage::FreeMsg(pMsg);
			}
		}
	}
}

