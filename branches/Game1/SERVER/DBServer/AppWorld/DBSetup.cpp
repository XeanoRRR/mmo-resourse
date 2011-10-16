#include "StdAfx.h"
#include "DBSetup.h"
#include "..\..\..\dbaccess\worlddb\RSSetup.h"
#include "DBEntityManager.h"
#include "../DBServer/game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBSetup::CDBSetup()
: CBaseDBEntity(),
m_lLeaveWordID(0),m_lPlayerID(0)
{	
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBSetup::CDBSetup(const CGUID& id)
: m_lLeaveWordID(0),m_lPlayerID(0)
{
	m_GUID = id;
}
CDBSetup::~CDBSetup()
{
}

// virtual interfaces
void CDBSetup::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_SAVE:
		{
			// ��װ��Ϣ
			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
			// MSG ID:        4byte, ����Ϣ��MSGID
			// Entity Num:	2byte, ��ʵ�����(X)
			Save(msgid, msgBuf, msgBufPos);
//			g_RsSetup.SavePlayerID();
//			g_RsSetup.SaveLeaveWorldID();
		}
		break;
	case DB_OPER_LOAD:
		{
			// ��װ��Ϣ
			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
			// MSG ID:        4byte, ����Ϣ��MSGID
			// Entity Num:	2byte, ��ʵ�����(X)
			GetGame()->GetRsSetup()->LoadLeaveWorldID();
			GetGame()->GetRsSetup()->LoadPlayerID();
			vector<BYTE> pBA;
			Load(msgid, msgBuf, msgBufPos, pBA);
		}
		break;
	}
}
void CDBSetup::Load(long msgid, BYTE* msgBuf, long& msgBufPos, vector<BYTE>& pBA)
{
	// initiate
	//Initiate();

	CDBEntityManager::DBSetupMapItr itr = GetGame()->GetDBEntityManager()->GetDBSetupMap().begin();
	if(itr->second)
	{
		_AddToByteArray(&pBA, GetID()); // ENTITY ID
		_AddToByteArray(&pBA, (WORD)2/*CDRegion�����������������*/); // ���͵�CDRegion�����������������

		// add one card data
		_AddToByteArray(&pBA, (WORD)1);
		_AddToByteArray(&pBA, (long)DB_GEN_SETUP_ATTR_LONG_LEAVE_WORD_ID);
		_AddToByteArray(&pBA, GetLeaveWordID());

		_AddToByteArray(&pBA, (WORD)1);
		_AddToByteArray(&pBA, (long)DB_GEN_SETUP_ATTR_LONG_PLAYER_ID);
		_AddToByteArray(&pBA, GetPlayerID());
	}
}

void CDBSetup::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
{
	// initiate
	//	Initiate();

	// ������Ϣ
	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �����������
	for(WORD i = 0; i < attrNum; i++)
	{
		WORD attrDataNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �������ݸ���
		DB_SETUP_ATTR_TYPE type = (DB_SETUP_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);//��������ö��ֵ

		switch(type)
		{
		case DB_GEN_SETUP_ATTR_LONG_LEAVE_WORD_ID:
			{
				SetLeaveWordID(_GetLongFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_GEN_SETUP_ATTR_LONG_PLAYER_ID:
			{
				CGUID tGuid;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
				SetPlayerID(tGuid);
			}
			break;
		}
	}
}
