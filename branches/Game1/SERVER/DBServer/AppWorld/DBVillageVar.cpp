#include "StdAfx.h"
#include "DBCityWar.h"
#include "..\..\..\dbaccess\worlddb\RSVillageWar.h"
#include "DBentityManager.h"
#include "..\DBServer\game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
//CDBVillageWar::CDBVillageWar()
//: CBaseDBEntity(),
//m_lRegionID(0)
//{
//	m_listDecWarFactions.clear();
//	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
//}
//
//CDBVillageWar::CDBVillageWar(const CGUID& id)
//: CBaseDBEntity(id),
//m_lRegionID(0)
//{
//	m_listDecWarFactions.clear();
//}
//CDBVillageWar::~CDBVillageWar()
//{
//	//GetGame()->GetDBEntityManager()->FreeDBVillageWarID(m_ID);
//}
//
//// virtual interfaces
//void CDBVillageWar::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos)
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
//			GetGame()->GetRsVillageWar()->Save(this);
//		}
//		break;
//	case DB_OPER_LOAD:
//		{
//			// ��װ��Ϣ
//			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
//			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
//			// MSG ID:        4byte, ����Ϣ��MSGID
//			// Entity Num:	2byte, ��ʵ�����(X)
//			GetGame()->GetRsVillageWar()->Load(this);
//			vector<BYTE> pBA;
//			Load(msgid, msgBuf, msgBufPos, pBA);
//		}
//		break;
//	case DB_OPER_UPDATE:
//		Update(msgid, msgBuf, msgBufPos);
//		break;
//	}
//}
//void CDBVillageWar::Create(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//	
//}
//void CDBVillageWar::Delete(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//
//}
//void CDBVillageWar::Load(long msgid, BYTE* msgBuf, long& msgBufPos, vector<BYTE>& pBA)
//{
//	// initiate
//	//Initiate();
//
//	CDBEntityManager::DBVillageWarMapItr itr = GetGame()->GetDBEntityManager()->DBVillageWarMapBegin();
//	for(; itr != GetGame()->GetDBEntityManager()->DBVillageWarMapBegin(); itr++)
//	{
//		_AddToByteArray(&pBA, m_GUID); // ʵ��ID
//		_AddToByteArray(&pBA, (WORD)3/*CDBVillageWar�����������������*/); // ���͵�CDBVillageWar�����������������
//
//		// add one card data
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_VILLAGE_WAR_ATTR_LONG_VILREGIONID);
//		_AddToByteArray(&pBA, GetRegionID());
//
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_VILLAGE_WAR_ATTR_BUF_WARSTARTTIME);
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().Year());
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().Month());
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().tmTime.tm_wday);
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().Day());
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().Hour());
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().Minute());
//		_AddToByteArray(&pBA, (WORD)GetWarStartTime().Second());
//
//		_AddToByteArray(&pBA, (WORD)GetDecWarFactions().size());
//		_AddToByteArray(&pBA, (long)DB_VILLAGE_WAR_ATTR_BUF_DECWARFACTION);
//		list<CGUID>::iterator itr = GetDecWarFactions().begin();
//		for(; itr != GetDecWarFactions().end(); itr++)
//		{
//			_AddToByteArray(&pBA, *itr);
//		}
//	}
//}
//
//void CDBVillageWar::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
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
//		DB_VILLAGE_WAR_ATTR_TYPE type = (DB_VILLAGE_WAR_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);//��������ö��ֵ
//		//DB_ATTR_DATA_TYPE cppType = (DB_ATTR_DATA_TYPE)pMsg->GetByte(); //�����Ե�C++��������
//		switch(type)
//		{
//		case DB_VILLAGE_WAR_ATTR_LONG_VILREGIONID:
//			{
//				SetRegionID(_GetLongFromByteArray(msgBuf, msgBufPos));
//			}
//			break;
//		case DB_VILLAGE_WAR_ATTR_BUF_WARSTARTTIME:
//			{
//				tagTime& time = GetWarStartTime();
//				time.tmTime.tm_year = _GetWordFromByteArray(msgBuf, msgBufPos);
//				time.tmTime.tm_mon	= _GetWordFromByteArray(msgBuf, msgBufPos);
//				time.tmTime.tm_wday = _GetWordFromByteArray(msgBuf, msgBufPos);
//				time.tmTime.tm_mday = _GetWordFromByteArray(msgBuf, msgBufPos);
//				time.tmTime.tm_hour = _GetWordFromByteArray(msgBuf, msgBufPos);
//				time.tmTime.tm_min	= _GetWordFromByteArray(msgBuf, msgBufPos);
//				time.tmTime.tm_sec	= _GetWordFromByteArray(msgBuf, msgBufPos);
//			}
//			break;
//		case DB_VILLAGE_WAR_ATTR_BUF_DECWARFACTION:
//			{
//				list<CGUID>& df = GetDecWarFactions();
//				for(int j=0; j<attrDataNum; j++)
//				{
//					CGUID tGuid;
//					_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//					df.push_back(tGuid);
//				}
//			}
//			break;
//		}
//	}
//}
//void CDBVillageWar::Update(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//
//}
