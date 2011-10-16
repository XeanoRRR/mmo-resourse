#include "stdafx.h"
#include "../DBServer/game.h"
#include "DBLinkmanGroup.h"
#include "DBentityManager.h"
#include "DBlinkman.h"
#include "DBLoadThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBLinkmanGroup::CDBLinkmanGroup()
: CBaseDBEntity()
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBLinkmanGroup::CDBLinkmanGroup(const CGUID& id)
: CBaseDBEntity(id)
{
}
CDBLinkmanGroup::~CDBLinkmanGroup()
{
	set<CDBLinkMan*>::iterator itr = m_setLinkmanGroup.begin();
	for(; itr != m_setLinkmanGroup.end(); itr++)
	{
		SAFE_DELETE(*itr);
	}
	m_setLinkmanGroup.clear();
}

// virtual interfaces
void CDBLinkmanGroup::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_SAVE:
		{
			// ��װ��Ϣ
			SaveThreadData* saveData = new SaveThreadData(GetExID(), DB_LINKMANGROUP);
			// ��Ҫ��¼һ��ʱ���,�ڴ����߳���
			Save(msgid, msgBuf, msgBufPos, bufSize);
			saveData->saveObj->objGUID = GetExID();
			saveData->saveObj->objType = DB_LINKMANGROUP;
			saveData->saveObj->objNum  = 1;
			*(CDBLinkmanGroup*)(saveData->dbObj) = *this;
			GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
		}
		break;
	case DB_OPER_LOAD:
		{
			if(b_IsFirstCreated) // �Ǹմ����Ķ���,�����ݿ��ȡ
			{
				LoadThreadData* loadData = new LoadThreadData(GetExID(), DB_LINKMANGROUP);
				loadData->loadObj->objGUID = GetExID();
				loadData->loadObj->objType = DB_LINKMANGROUP;
				loadData->loadObj->objNum  = 1;
				*(CDBLinkmanGroup*)(loadData->dbObj) = *this;
				GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
			}
			else //���Ǹմ����Ķ���,ֱ�ӷ����ڴ�����
			{
				// ��װ��Ϣ
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				// opType		  4byte, ������������
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg.Add((long)DB_LINKMANGROUP);
				msg.Add((BYTE)0);
				msg.Add((BYTE)DB_OPER_LOAD);
				msg.Add(msgid);
				msg.Add((WORD)1);
				vector<BYTE> pBA;
				AddToByteArray(pBA);
				msg.Add((long)pBA.size());
				msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
		}
		break;
	}
}

void CDBLinkmanGroup::AddToByteArray(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, GetOwnerGUID()); // ownerID
	_AddToByteArray(&pBA, (WORD)m_setLinkmanGroup.size()); // ��Ʒ����

	set<CDBLinkMan*>::iterator itr = m_setLinkmanGroup.begin();
	for(; itr != m_setLinkmanGroup.end(); itr++)
	{
		(*itr)->AddEntityDataToMsg(DB_OPER_LOAD, pBA);
	}
}


void CDBLinkmanGroup::Save(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	m_mapDirtyAttrData.clear();
	set<CDBLinkMan*>::iterator itr = m_setLinkmanGroup.begin();
	for(; itr != m_setLinkmanGroup.end(); itr++)
		SAFE_DELETE(*itr);
	m_setLinkmanGroup.clear();

//	_GetBufferFromByteArray(msgBuf, msgBufPos, m_OwnerGUID, bufSize); // ӵ����ID

	WORD goodsNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // ��Ʒ����

	for(int j=0; j<goodsNum; j++)
	{
		// ������Ϣ
		CGUID goodsID;
		_GetBufferFromByteArray(msgBuf, msgBufPos, goodsID, bufSize);
		CDBLinkMan* existGoods = GetGame()->GetDBEntityManager()->FindDBLinkman(goodsID);
		if(NULL == existGoods)
		{
			existGoods = new CDBLinkMan(goodsID);
		}
		if(existGoods)
		{
			existGoods->Save(0, msgBuf, msgBufPos, bufSize);
			m_setLinkmanGroup.insert(existGoods);
		}
	}

	

}

