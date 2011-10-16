#include "stdafx.h"
#include "../DBServer/game.h"
#include "DBMailGoodsGroup.h"
#include "DBentityManager.h"
#include "DBLoadThread.h"
#include "DBMail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBMailGoodsGroup::CDBMailGoodsGroup()
: CBaseDBEntity()
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBMailGoodsGroup::CDBMailGoodsGroup(const CGUID& id)
: CBaseDBEntity(id)
{
}
CDBMailGoodsGroup::~CDBMailGoodsGroup()
{
	set<CDBMailGoods*>::iterator itr = m_setGoodsGroup.begin();
	for(; itr != m_setGoodsGroup.end(); itr++)
	{
		SAFE_DELETE(*itr);
	}
	m_setGoodsGroup.clear();
}

// virtual interfaces
void CDBMailGoodsGroup::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_SAVE:
		{
			SaveThreadData* saveData = new SaveThreadData(GetExID(), DB_MAILGOODSGROUP);
			// ��Ҫ��¼һ��ʱ���,�ڴ����߳���
			Save(msgid, msgBuf, msgBufPos, bufSize);
			saveData->saveObj->objGUID = GetExID();
			saveData->saveObj->objType = DB_MAILGOODSGROUP;
			saveData->saveObj->objNum  = 1;
			*(CDBMailGoodsGroup*)(saveData->dbObj) = *this;
			GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
		}
		break;
	case DB_OPER_LOAD:
		{
		}
		break;
	}
}

void CDBMailGoodsGroup::AddToByteArray(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_OwnerGUID); // ӵ����ID
	_AddToByteArray(&pBA, (WORD)m_setGoodsGroup.size()); // ��Ʒ����

	set<CDBMailGoods*>::iterator itr = m_setGoodsGroup.begin();
	for(; itr != m_setGoodsGroup.end(); itr++)
	{
		(*itr)->AddEntityDataToMsg(DB_OPER_LOAD, pBA);
	}
}


void CDBMailGoodsGroup::Save(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{ 
	m_mapDirtyAttrData.clear();
	set<CDBMailGoods*>::iterator itr = m_setGoodsGroup.begin();
	for(; itr != m_setGoodsGroup.end(); itr++)
		SAFE_DELETE(*itr);
	m_setGoodsGroup.clear();

	_GetBufferFromByteArray(msgBuf, msgBufPos, m_OwnerGUID, bufSize); // ӵ����ID

	WORD goodsNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // ��Ʒ����

	for(int j=0; j<goodsNum; j++)
	{
		// ������Ϣ
		CGUID goodsID;
		_GetBufferFromByteArray(msgBuf, msgBufPos, goodsID, bufSize);
		CDBMailGoods* existGoods = new CDBMailGoods(goodsID);
		if(existGoods)
		{
			existGoods->Save(0, msgBuf, msgBufPos, bufSize);
			m_setGoodsGroup.insert(existGoods);
		}
	}
}

