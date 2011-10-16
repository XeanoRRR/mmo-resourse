#include "StdAfx.h"
#include "DBRegion.h"
#include "DBentityManager.h"
#include "WorldRegion.h"
#include "GlobalRgnManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBRegion::CDBRegion()
: CBaseDBEntity()
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);

	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
}

CDBRegion::CDBRegion(const CGUID& id)
:CBaseDBEntity(id)
{
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
	m_SendAttrEnumVector.push_back(1);
}
CDBRegion::~CDBRegion()
{

}

void CDBRegion::OpenAttrFlagInVector(DB_REGION_ATTR_TYPE attrType)
{
	if(attrType == DB_REGION_ATTR_BUF_REGIONPARAM)
	{
		m_SendAttrEnumVector[0] = 1;
	}
}
// �ر�׼�����͵�������Vector�е���Ӧλ�õı�־
void CDBRegion::CloseAttrFlagInVector(DB_REGION_ATTR_TYPE attrType)
{
	if(attrType == DB_REGION_ATTR_BUF_REGIONPARAM)
	{
		m_SendAttrEnumVector[0] = 0;
	}
}

// virtual interfaces
void CDBRegion::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	//CBaseDBEntity::ProcessMsg(op, msgid, pMsg);
	switch(op)
	{
	case DB_OPER_SAVE:
		Save(msgid, msgBuf, msgBufPos);
		break;
	case DB_OPER_LOAD:
		Load(msgid, msgBuf, msgBufPos, bufSize);
		break;
	}
}

void CDBRegion::Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{	
	long lRegionId=_GetLongFromByteArray(msgBuf,msgBufPos);
	SetID(lRegionId);
	//ͨ��regionid�ҵ�CWorldRegion
	CWorldRegion* pWorldRegion=GetGame()->GetGlobalRgnManager()->GetWorldRegionByRgnId(lRegionId);
	BYTE bLoadFromDB=_GetByteFromByteArray(msgBuf,msgBufPos);
	if(bLoadFromDB==1)
	{
		//���´����ݿ��ȡ������
		tagRegionParam stParam;
		_GetBufferFromByteArray(msgBuf,msgBufPos,&stParam,sizeof(tagRegionParam));
		tagRegionParam& stDBRegionParam=GetRegionParam();
		stDBRegionParam.OwnedFactionID=stParam.OwnedFactionID;
		stDBRegionParam.OwnedUnionID=stParam.OwnedUnionID;
		stDBRegionParam.lCurrentTaxRate=stParam.lCurrentTaxRate;
		stDBRegionParam.dwTotalTax=stParam.dwTotalTax;
		stDBRegionParam.dwTodayTotalTax=stParam.dwTodayTotalTax;	
		stDBRegionParam.dwBusinessTotalTax=stParam.dwBusinessTotalTax;
		stDBRegionParam.lBusinessTaxRate=stParam.lBusinessTaxRate;
		stDBRegionParam.lYuanbaoTaxRate=stParam.lYuanbaoTaxRate;
		
		if(pWorldRegion)
		{
			::tagRegionParam stRegionParam;
			memcpy(&stRegionParam,&stDBRegionParam,sizeof(tagRegionParam));
			::tagRegionParam& stRegionParam2=pWorldRegion->GetRegionParam();
			stRegionParam.lID=stRegionParam2.lID;
			stRegionParam.lTurnInTaxRate=stRegionParam2.lTurnInTaxRate;
			stRegionParam.lSupRegionID=stRegionParam2.lSupRegionID;
			stRegionParam.lMaxTaxRate=stRegionParam2.lMaxTaxRate;
			pWorldRegion->SetRegionParam(stRegionParam);
		}
	}
	else
	{
		//����ΪĬ������

	}
}

void CDBRegion::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
{
	char szGuid[128];
	GetExID().tostring(szGuid);
	//AddLogText("DB2W[%d] : DB_OPER_SAVE_Region, RegionID[%s]...OK!", msgid, szGuid);
}

void CDBRegion::AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA)
{
	// ����������Ϣǰ���Ѿ�������
	// OPEntityID:    4byte, ����ʵ���IDֵ
	// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
	// MSG ID:        4byte, ����Ϣ��MSGID
	// Entity Num:    2byte, ��ʵ�����
	if(DB_OPER_DELETE == opType) // DELETE����
	{
		return;
	}

	WORD attrNum = 0;
	for(int i = 0; i < (int)m_SendAttrEnumVector.size(); i++)
	{
		if(m_SendAttrEnumVector[i] != 0)
			attrNum++;
	}
	_AddToByteArray(&pBA, (long)m_ID); // ʵ��ID
	_AddToByteArray(&pBA, attrNum); // �����������

	for(int i = 0; i < (int)m_SendAttrEnumVector.size(); i++)
	{
		if(m_SendAttrEnumVector[i] != 0 
			&& ( DB_OPER_SAVE == opType || DB_OPER_UPDATE == opType || DB_OPER_LOAD == opType) )// ��־�򿪲���ΪSAVE/UPDATE/CREATE����
		{
			if(DB_REGION_ATTR_BUF_REGIONPARAM)
			{
				_AddToByteArray(&pBA, (WORD)1);
				_AddToByteArray(&pBA, (long)DB_REGION_ATTR_BUF_REGIONPARAM);
				if(DB_OPER_LOAD != opType)
				{
					_AddToByteArray(&pBA, &GetRegionParam(), sizeof(tagRegionParam));
				}
				else
					_AddToByteArray(&pBA, (BYTE)0);
			}
		}
	}

}


