#include "stdafx.h"
#include "DBPlayer.h"
#include "DBGoodsGroup.h"
#include "DBentityManager.h"
#include "DBGood.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBGoodsGroup::CDBGoodsGroup()
: CBaseDBEntity()
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBGoodsGroup::CDBGoodsGroup(const CGUID& id)
: CBaseDBEntity(id)
{
	m_mapGoodsGroup.clear();
}
CDBGoodsGroup::~CDBGoodsGroup()
{
	map<CGUID, CDBGoods*>::iterator itr = m_mapGoodsGroup.begin();
	for(; itr != m_mapGoodsGroup.end(); itr++)
	{
		SAFE_DELETE(itr->second);
	}
	m_mapGoodsGroup.clear();
}

// virtual interfaces
void CDBGoodsGroup::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_LOAD:
		{
			Load(msgid, msgBuf, msgBufPos, bufSize);
		}
		break;	
	}
}

void CDBGoodsGroup::AddToByteArray(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, m_OwnerGUID); // ӵ����ID

	if(m_OwnerGUID == NULL_GUID)
	{
		AddLogText("CDBGoodsGroup::AddToByteArray() OnerID is NULL!");
	}

	// ��Ӹð�������Ʒ��place��num��Ϣ
	// �Ƿ�������
	_AddToByteArray(&pBA, (long)GetGoodsGroupMap().size());
	map<CGUID, CDBGoods*>::iterator placeItr = GetGoodsGroupMap().begin();
	for(; placeItr != GetGoodsGroupMap().end(); placeItr++)
	{
		// ��������Ʒ����
		placeItr->second->AddEntityDataToMsg(DB_OPER_LOAD, pBA);
	}
}


void CDBGoodsGroup::Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{ 
	Clear();

	long tmpPos = msgBufPos;

	char szOwnerID[128];
	m_OwnerGUID.tostring(szOwnerID);

	// �Ƿ������ݱ�־λ
	long num = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);
	if(num)
	{
		for(int j=0; j<num; j++)
		{
			// ��������Ʒ����
			CGUID goodsID;
			_GetBufferFromByteArray(msgBuf, msgBufPos, goodsID, bufSize);
			CDBGoods* existGoods = new CDBGoods(goodsID);
			if(existGoods)
				existGoods->Load(0, msgBuf, msgBufPos, bufSize);
			m_mapGoodsGroup[goodsID] = existGoods;
		}
	}
}

void CDBGoodsGroup::Clear(void)
{
	map<CGUID, CDBGoods*>::iterator itr = m_mapGoodsGroup.begin();
	for(; itr != m_mapGoodsGroup.end(); itr++)
	{
		SAFE_DELETE(itr->second);
	}
	m_mapGoodsGroup.clear();
}
