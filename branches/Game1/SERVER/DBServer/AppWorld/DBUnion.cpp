#include "StdAfx.h"
#include "DBUnion.h"
#include "..\..\..\dbaccess\worlddb\RSUnion.h"
#include "DBEntityManager.h"
#include "../DBServer/game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBUnion::CDBUnion()
: CBaseDBEntity()
{
	//GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBUnion::CDBUnion(const CGUID& id)
: CBaseDBEntity(id)
{

}
CDBUnion::~CDBUnion()
{
//	GetGame()->GetDBEntityManager()->FreeDBUnionID(m_ID);
}

// virtual interfaces
void CDBUnion::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
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
////			g_RsUnion.SaveConfederation(this);
//		}
//		break;
//	case DB_OPER_LOAD:
//		{
//			// ��װ��Ϣ
//			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
//			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
//			// MSG ID:        4byte, ����Ϣ��MSGID
//			// Entity Num:	2byte, ��ʵ�����(X)
//			GetGame()->GetRsConfederation()->LoadConfederation(this);
//			vector<BYTE> pBA;
//			Load(msgid, msgBuf, msgBufPos, pBA);
//		}
//		break;
//	case DB_OPER_UPDATE:
//		//g_RSCards.SaveLargess(this);
//		Update(msgid, msgBuf, msgBufPos);
//		break;
//	}
}
//void CDBUnion::Create(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//
//}
//void CDBUnion::Delete(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//	
//}
//void CDBUnion::Load(long msgid, BYTE* msgBuf, long& msgBufPos, vector<BYTE>& pBA)
//{
//	// initiate
//	//Initiate();
//
//	CDBEntityManager::DBUnionMapItr itr = GetGame()->GetDBEntityManager()->DBUnionMapBegin();
//	for(; itr != GetGame()->GetDBEntityManager()->DBUnionMapEnd(); itr++)
//	{
//		_AddToByteArray(&pBA, m_GUID); // ʵ��ID
//		_AddToByteArray(&pBA, (WORD)5/*CDBCityWar�����������������*/); // ���͵�CDBCityWar�����������������
//
//		// add one Faction data
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_UNION_ATTR_LONG_ID);
//		_AddToByteArray(&pBA, GetExID());
//
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_UNION_ATTR_SZ_NAME);
//		_AddToByteArray(&pBA, (char*)GetName().c_str());
//				
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_UNION_ATTR_LONG_MASTER_ID);
//		_AddToByteArray(&pBA, GetMasterID());
//		
//		_AddToByteArray(&pBA, (WORD)1);
//		_AddToByteArray(&pBA, (long)DB_UNION_ATTR_BUF_PROPERTY);
//		GetPropertyData(pBA);
//			
//		_AddToByteArray(&pBA, (WORD)m_Members.size());
//		_AddToByteArray(&pBA, (long)DB_UNION_ATTR_BUF_MEMBERS);
//		GetMembersData(pBA);	
//	}
//
//}
//
//void CDBUnion::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
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
//		WORD num = _GetWordFromByteArray(msgBuf, msgBufPos); // �������ݸ���
//		DB_UNION_ATTR_TYPE type = (DB_UNION_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);//��������ö��ֵ
//
//		long pos = 0; // ���ݿ��ȡָ��
//		switch(type)
//		{
//		case DB_UNION_ATTR_LONG_ID:
//			{
//				CGUID tGuid;
//				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//				SetExID(tGuid);
//			}
//			break;
//		case DB_UNION_ATTR_SZ_NAME:
//			{
//				char name[128];
//				_GetStringFromByteArray(msgBuf, msgBufPos, name);
//				SetName(string(name));
//			}
//			break;
//		case DB_UNION_ATTR_LONG_MASTER_ID:
//			{
//				CGUID tGuid;
//				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
//				SetMasterID(tGuid);
//			}
//			break;
//
//			break;
//		case DB_UNION_ATTR_BUF_PROPERTY:
//			{
//				tagFacBaseProperty& bp = GetProperty();
//				_GetBufferFromByteArray(msgBuf, msgBufPos, &bp, sizeof(tagFacBaseProperty));
//			}
//			break;
//		case DB_UNION_ATTR_BUF_MEMBERS:
//			{
//				map<CGUID,tagMemInfo>& mi = GetMembers();
//				mi.clear();
//
//				tagMemInfo tmi;
//				for(int i=0; i<num; i++)
//				{
//					_GetBufferFromByteArray(msgBuf, msgBufPos, &tmi, sizeof(tagMemInfo));
//					mi[tmi.MemberGuid] = tmi;
//				}
//			}
//			break;
//		}
//	}
//}
//void CDBUnion::Update(long msgid, BYTE* msgBuf, long& msgBufPos)
//{
//	
//}
//
////�õ���������
//bool CDBUnion::GetPropertyData(vector<BYTE>& ByteArray)
//{
//	_AddToByteArray(&ByteArray,&m_Property,sizeof(tagFacBaseProperty));
//	return true;
//}
////�õ���Ա��Ϣ����
//bool CDBUnion::GetMembersData(vector<BYTE>& ByteArray)
//{
//	map<CGUID,tagMemInfo>::iterator it = m_Members.begin();
//	for(;it!=m_Members.end();it++)
//	{
//		_AddToByteArray(&ByteArray,&(*it).second,sizeof(tagMemInfo) );
//	}
//	return true;
//}