#include "StdAfx.h"
#include "DBEnemyFaction.h"
#include "DBentityManager.h"
#include "..\..\..\dbaccess\worlddb\RSEnemyFactions.h"
#include "../DBServer/game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
//CDBEnemyFaction::CDBEnemyFaction()
//: CBaseDBEntity(),
//m_lFactionID1(NULL_GUID),
//m_lFactionID2(NULL_GUID),
//m_dwDisandTime(0)
//{
//	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
//}
//
//CDBEnemyFaction::CDBEnemyFaction(const CGUID& id)
//:	m_lFactionID1(NULL_GUID),
//	m_lFactionID2(NULL_GUID),
//	m_dwDisandTime(0),
//	CBaseDBEntity(id)
//{
//}
//CDBEnemyFaction::~CDBEnemyFaction()
//{
////	GetGame()->GetDBEntityManager()->FreeDBEnemyFactionID(m_ID);
//}
//
//// virtual interfaces
//void CDBEnemyFaction::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//	switch(op)
//	{
//	case DB_OPER_CREATE:
//		Create(msgid, msgBuf, msgBufPos);
//		break;
//	case DB_OPER_DELETE:
//		Delete(msgid, msgBuf, msgBufPos);
//		break;
//	case DB_OPER_SAVE:
//		{
//			// ��װ��Ϣ
//			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
//			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
//			// MSG ID:        4byte, ����Ϣ��MSGID
//			// Entity Num:	2byte, ��ʵ�����(X)
//			Save(msgid, msgBuf, msgBufPos);
//			GetGame()->GetRsEnemyFactions()->SaveEnemyFaction(this);
//		}
//		break;
//	case DB_OPER_LOAD:
//		{
//			// ��װ��Ϣ
//			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
//			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
//			// MSG ID:        4byte, ����Ϣ��MSGID
//			// Entity Num:	2byte, ��ʵ�����(X)
//			GetGame()->GetRsEnemyFactions()->LoadEnemyFaction(this);
//		}
//		break;
//	case DB_OPER_UPDATE:
//		//g_RSCards.SaveLargess(this);
//		Update(msgid, msgBuf, msgBufPos);
//		break;
//	}
//}
//void CDBEnemyFaction::Create(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//	// initiate
//	//	Initiate();
//
//	// MSGID
////	long msgid = _GetLongFromByteArray(msgBuf, msgBufPos);
//
//	// ������Ϣ
//	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �����������
//	for(WORD i = 0; i < attrNum; i++)
//	{
//		WORD attrDataNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �������ݸ���
//		DB_ENEMY_FACTION_ATTR_TYPE type = (DB_ENEMY_FACTION_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);//��������ö��ֵ
//
//		switch(type)
//		{
//		case DB_ENEMY_FACTION_ATTR_LONG_FACTIONID1:
//			{
//				CGUID tGuid;
//				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//				SetFactionID1(tGuid);
//			}
//			break;
//		case DB_ENEMY_FACTION_ATTR_LONG_FACTIONID2:
//			{
//				CGUID tGuid;
//				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//				SetFactionID2(tGuid);
//			}
//			break;
//		case DB_ENEMY_FACTION_ATTR_DWORD_DISANDTIME:
//			{
//				SetDisandTime(_GetLongFromByteArray(msgBuf, msgBufPos));
//			}
//			break;
//		}
//	}
//}
//void CDBEnemyFaction::Delete(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//
//}
//void CDBEnemyFaction::Load(long msgid, BYTE* msgBuf, long& msgBufPos, vector<BYTE>& pBA)
//{
//	// initiate
//	//Initiate();
//	CDBEntityManager::DBEnemyFactionMapItr itr = GetGame()->GetDBEntityManager()->DBEnemyFactionMapBegin();
//	for(; itr != GetGame()->GetDBEntityManager()->DBEnemyFactionMapEnd(); itr++)
//	{
//		_AddToByteArray(&pBA, m_GUID); // ʵ��ID
//		_AddToByteArray(&pBA, (WORD)3/*CDBEnemyFaction�����������������*/); // ���͵�CDBEnemyFaction�����������������
//
//		// add one card data
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_ENEMY_FACTION_ATTR_LONG_FACTIONID1);
//		_AddToByteArray(&pBA, GetFactionID1());
//
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_ENEMY_FACTION_ATTR_LONG_FACTIONID2);
//		_AddToByteArray(&pBA, GetFactionID2());
//
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_ENEMY_FACTION_ATTR_DWORD_DISANDTIME);
//		_AddToByteArray(&pBA, GetDisandTime());
//	}
//}
//
//void CDBEnemyFaction::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//	// initiate
//	//	Initiate();
//
//	// MSGID
////	long msgid = _GetLongFromByteArray(msgBuf, msgBufPos);
//
//	// ������Ϣ
//	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �����������
//	for(WORD i = 0; i < attrNum; i++)
//	{
//		WORD attrDataNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �������ݸ���
//		DB_ENEMY_FACTION_ATTR_TYPE type = (DB_ENEMY_FACTION_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);//��������ö��ֵ
//		//DB_ATTR_DATA_TYPE cppType = (DB_ATTR_DATA_TYPE)pMsg->GetByte(); //�����Ե�C++��������
//
//		switch(type)
//		{
//		case DB_ENEMY_FACTION_ATTR_LONG_FACTIONID1:
//			{
//				CGUID tGuid;
//				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//				SetFactionID1(tGuid);
//			}
//			break;
//		case DB_ENEMY_FACTION_ATTR_LONG_FACTIONID2:
//			{
//				CGUID tGuid;
//				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//				SetFactionID2(tGuid);
//			}
//			break;
//		case DB_ENEMY_FACTION_ATTR_DWORD_DISANDTIME:
//			{
//				SetDisandTime(_GetLongFromByteArray(msgBuf, msgBufPos));
//			}
//			break;
//		}
//	}
//}
//void CDBEnemyFaction::Update(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//}
