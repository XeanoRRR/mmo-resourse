#include "stdafx.h"
#include "../WorldServer/game.h"
#include "DBMailGroup.h"
#include "DBentityManager.h"
#include "DBMail.h"
#include "DBPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBMailGroup::CDBMailGroup()
: CBaseDBEntity()
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBMailGroup::CDBMailGroup(const CGUID& id)
: CBaseDBEntity(id)
{
}
CDBMailGroup::~CDBMailGroup()
{
	set<CDBMail*>::iterator itr = m_setMailGroup.begin();
	for(; itr != m_setMailGroup.end(); itr++)
	{
		SAFE_DELETE(*itr);
	}
	m_setMailGroup.clear();
}

// virtual interfaces
void CDBMailGroup::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_LOAD:
		{
			// ��Ҫ��¼һ��ʱ���,�ڴ����߳���
			Load(msgid, msgBuf, msgBufPos, bufSize);
		}
		break;
	case DB_OPER_DELETE:
		{
			_GetBufferFromByteArray(msgBuf, msgBufPos, m_OwnerGUID, bufSize); // ӵ����ID

			WORD goodsNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // ��Ʒ����

		}
		break;
	}
}

void CDBMailGroup::AddToByteArray(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, m_OwnerGUID); // ӵ����ID
	_AddToByteArray(&pBA, (WORD)m_setMailGroup.size()); // ��Ʒ����

	set<CDBMail*>::iterator itr = m_setMailGroup.begin();
	for(; itr != m_setMailGroup.end(); itr++)
	{
		(*itr)->AddToByteArray(pBA);
	}
}

void CDBMailGroup::Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{ 
	m_mapDirtyAttrData.clear();
	set<CDBMail*>::iterator itr = m_setMailGroup.begin();
	for(; itr != m_setMailGroup.end(); itr++)
		SAFE_DELETE(*itr);
	m_setMailGroup.clear();

//	_GetBufferFromByteArray(msgBuf, msgBufPos, m_OwnerGUID, bufSize); // ӵ����ID

	WORD goodsNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // ��Ʒ����

	for(int j=0; j<goodsNum; j++)
	{
		// ������Ϣ
		CGUID goodsID;
		_GetBufferFromByteArray(msgBuf, msgBufPos, goodsID, bufSize);
		CDBMail* existGoods = new CDBMail(goodsID);
		if(existGoods)
		{
			existGoods->Load(0, msgBuf, msgBufPos, bufSize);
			m_setMailGroup.insert(existGoods);
		}
	}
}

void CDBMailGroup::Delete(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	_GetBufferFromByteArray(msgBuf, msgBufPos, m_OwnerGUID, bufSize); // ӵ����ID

	WORD goodsNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // ��Ʒ����
	list<CGUID> guidList;

	for(int i=0; i<goodsNum; i++)
	{
		CGUID mailGuid;
		_GetBufferFromByteArray(msgBuf, msgBufPos, mailGuid, bufSize);
		guidList.push_back(mailGuid);
	}
	GetGame()->GetMailManager()->DeleteMail(m_OwnerGUID,guidList);
	// ��������
}
void CDBMailGroup::Clear(void)
{
	set<CDBMail*>::iterator itr = GetMailGroupSet().begin();
	for(; itr != GetMailGroupSet().end(); itr++)
	{
		SAFE_DELETE((*itr));
	}
	GetMailGroupSet().clear();
}

