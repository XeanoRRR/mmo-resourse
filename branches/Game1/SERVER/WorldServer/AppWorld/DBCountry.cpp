#include "StdAfx.h"
#include "DBCountry.h"
#include "DBentityManager.h"
#include "Country\Country.h"
#include "Country\CountryHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBCountry::CDBCountry()
: CBaseDBEntity(), m_lPower(0),m_lTechLevel(0),m_lKing(0)
{
	CGUID::CreateGUID(m_GUID);
	for(int pos=(int)DB_COUNTRY_ATTR_LONG_ID;  pos <= (int)DB_COUNTRY_ATTR_LONG_KING; pos++)
	{
		m_SendAttrEnumVector.push_back(1);
	}
}

CDBCountry::CDBCountry(const CGUID& id)
:CBaseDBEntity(id), m_lPower(0),m_lTechLevel(0),m_lKing(0)
{		
	m_ID = 0;
	for(int pos=(int)DB_COUNTRY_ATTR_LONG_ID;  pos <= (int)DB_COUNTRY_ATTR_LONG_KING; pos++)
	{
		m_SendAttrEnumVector.push_back(1);
	}
}
CDBCountry::~CDBCountry()
{

}

// Other interfaces
// ��׼�����͵�������Vector�е���Ӧλ�õı�־
void CDBCountry::OpenAttrFlagInVector(DB_COUNTRY_ATTR_TYPE attrType)
{
	if(attrType >= DB_COUNTRY_ATTR_LONG_ID && attrType <= DB_COUNTRY_ATTR_LONG_KING)
	{
		long pos = DB_COUNTRY_ATTR_LONG_KING - attrType;
		m_SendAttrEnumVector.push_back(1);
	}
}
// �ر�׼�����͵�������Vector�е���Ӧλ�õı�־
void CDBCountry::CloseAttrFlagInVector(DB_COUNTRY_ATTR_TYPE attrType)
{
	if(attrType >= DB_COUNTRY_ATTR_LONG_ID && attrType <= DB_COUNTRY_ATTR_LONG_KING)
	{
		long pos = DB_COUNTRY_ATTR_LONG_KING - attrType;
		m_SendAttrEnumVector[pos] = 0;
	}
}

// virtual interfaces
void CDBCountry::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	//CBaseDBEntity::ProcessMsg(op, msgid, pMsg);
	switch(op)
	{
	case DB_OPER_SAVE:
		Save(msgid, msgBuf, msgBufPos);
		break;
	case DB_OPER_LOAD:
		Load(msgid, msgBuf, msgBufPos);
		break;
	}
}

void CDBCountry::Load(long msgid, BYTE* msgBuf, long& msgBufPos)
{
	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �����������
	
	for(int i=0; i<(int)attrNum; i++)
	{
		// add one card data
		WORD num = _GetWordFromByteArray(msgBuf, msgBufPos);
		DB_COUNTRY_ATTR_TYPE attrType = (DB_COUNTRY_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);
		switch(attrType)
		{
		case DB_COUNTRY_ATTR_LONG_ID:
			{
				SetID(_GetByteFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_COUNTRY_ATTR_LONG_POWER:
			{
				SetPower(_GetLongFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_COUNTRY_ATTR_LONG_TECH_LEVEL:
			{
				SetTechLevel(_GetLongFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_COUNTRY_ATTR_LONG_KING:
			{
				CGUID tGuid;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
				SetKing(tGuid);
			}
			break;	
		}
	}

	CCountry* country = new CCountry;
	GetGame()->GetDBEntityManager()->CDBCountryToCCountry(this, country);
	GetCountryHandler()->Append(country);
}

void CDBCountry::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
{
	char szGuid[128];
	GetExID().tostring(szGuid);
	////AddLogText("DB2W[%d] : DB_OPER_LOAD_COUNTRY, CountryID[%s]...OK!", msgid, szGuid);
}

void CDBCountry::AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA)
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
	_AddToByteArray(&pBA, m_ID); // ʵ��ID
	_AddToByteArray(&pBA, attrNum); // �����������

	for(int i = 0; i < (int)m_SendAttrEnumVector.size(); i++)
	{
		if(m_SendAttrEnumVector[i] != 0 
			&& ( DB_OPER_SAVE == opType || DB_OPER_UPDATE == opType || DB_OPER_LOAD == opType) )// ��־�򿪲���ΪSAVE/UPDATE/CREATE����
		{
			switch(DB_COUNTRY_ATTR_LONG_ID+i)
			{
			case DB_COUNTRY_ATTR_LONG_ID:
				{
					_AddToByteArray(&pBA, (WORD)1);
					_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_ID);
					if(DB_OPER_LOAD != opType)
						_AddToByteArray(&pBA, GetExID());
					else
						_AddToByteArray(&pBA, (BYTE)0);
				}
				break;
			case DB_COUNTRY_ATTR_LONG_POWER:
				{
					_AddToByteArray(&pBA, (WORD)1);
					_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_POWER);
					if(DB_OPER_LOAD != opType)
					{
						_AddToByteArray(&pBA, GetPower());
					}
					else
						_AddToByteArray(&pBA, (BYTE)0);
				}
				break;
			case DB_COUNTRY_ATTR_LONG_TECH_LEVEL:
				{
					_AddToByteArray(&pBA, (WORD)1);
					_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_TECH_LEVEL);
					if(DB_OPER_LOAD != opType)
					{
						_AddToByteArray(&pBA, GetTechLevel());
					}
					else
					{
						_AddToByteArray(&pBA, (BYTE)0);
					}
				}
				break;
			case DB_COUNTRY_ATTR_LONG_KING:
				{
					_AddToByteArray(&pBA, (WORD)1);
					_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_KING);
					if(DB_OPER_LOAD != opType)
					{
						_AddToByteArray(&pBA, GetKing());
					}
					else
					{
						_AddToByteArray(&pBA, (BYTE)0);
					}
				}
				break;
				
			}
		}
	}

}