#include "StdAfx.h"
#include "DBRegion.h"
#include "..\..\..\dbaccess\worlddb\RSRegion.h"
#include "DBEntityManager.h"
#include "../DBServer/game.h"
#include "ThreadDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDBRegion::CDBRegion()
: CBaseDBEntity()
{
	CGUID::CreateGUID(m_GUID);
	m_bLoadedFromDB=false;
}

CDBRegion::CDBRegion(const CGUID& id)
:CBaseDBEntity(id)
{
	m_bLoadedFromDB=false;
}

CDBRegion::~CDBRegion()
{
}

// virtual interfaces
void CDBRegion::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
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
			
			SaveThreadData* saveData = new SaveThreadData(GetExID(), DB_REGION);
			// ��Ҫ��¼һ��ʱ���,�ڴ����߳���
			
			SaveForThread(msgid, msgBuf, msgBufPos, saveData->saveObj);
			saveData->saveObj->objGUID = GetExID();
			saveData->saveObj->objType = DB_REGION;
			saveData->saveObj->objNum  = 1;
			*(CDBRegion*)(saveData->dbObj) = *this;
			GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
		}
		break;
	case DB_OPER_LOAD:
		{
			// ��װ��Ϣ
			// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
			// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
			// MSG ID:        4byte, ����Ϣ��MSGID
			// Entity Num:	2byte, ��ʵ�����(X)
			if(b_IsFirstCreated) // �Ǹմ����Ķ���,�����ݿ��ȡ
			{
				LoadThreadData* loadData = new LoadThreadData(GetExID(), DB_REGION);
				long lRegionId=_GetLongFromByteArray(msgBuf,msgBufPos);
				SetID(lRegionId);
				loadData->loadObj->objGUID = GetExID();
				loadData->loadObj->objType = DB_REGION;
				loadData->loadObj->objNum  = 1;				
				*(CDBRegion*)(loadData->dbObj) = *this;
				GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
			}
			else //���Ǹմ����Ķ���,ֱ�ӷ����ڴ�����
			{
				// ��װ��Ϣ				
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
				// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
				// MSG ID:        4byte, ����Ϣ��MSGID
				// Entity Num:	2byte, ��ʵ�����(X)
				msg.Add((long)DB_REGION);
				msg.Add((BYTE)0);
				msg.Add((BYTE)DB_OPER_LOAD);
				msg.Add(msgid);
				msg.Add((WORD)1);
				vector<BYTE> pBA;
				_AddToByteArray(&pBA,m_GUID);
				_AddToByteArray(&pBA,(BYTE*)(&GetRegionParam()),sizeof(tagRegionParam));
				msg.Add((long)pBA.size());
				msg.AddEx(&pBA[0], pBA.size());				
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
		}
		break;
	}
}

void CDBRegion::SaveForThread(long msgid, BYTE* msgBuf, long& msgBufPos, SaveObj* saveData)
{
	tagRegionParam stParam;
	_GetBufferFromByteArray(msgBuf,msgBufPos,&stParam,sizeof(tagRegionParam));
	SetRegionParam(stParam);
}