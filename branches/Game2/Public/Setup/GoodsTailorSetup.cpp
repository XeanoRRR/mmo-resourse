#include "StdAfx.h"
#include "GoodsTailorSetup.h"
#include "GoodsSetup.h"

//! OneMakeTable
//!-------------------------------------------------------------------------------------------------
/// ����
OneMakeTable::OneMakeTable(void)
:m_lID(0)
{
	memset(m_szName, 0, MAKE_TABLE_NAME_SIZE);
	memset(m_szNarrate, 0, MAKE_TABLE_NARRATE_SIZE);
}

/// ����
void OneMakeTable::Code(DBWriteSet& setWriteSet) const
{
	setWriteSet.AddToByteArray(m_lID);
	setWriteSet.AddToByteArray((void*)m_szName, MAKE_TABLE_NAME_SIZE);
	setWriteSet.AddToByteArray((void*)m_szNarrate, MAKE_TABLE_NARRATE_SIZE);

	setWriteSet.AddToByteArray((long)m_vExpend.size());
	for (size_t i = 0; i < m_vExpend.size(); ++i)
	{
		setWriteSet.AddToByteArray((void*)&m_vExpend[i], sizeof(tagExpend));
	}

	setWriteSet.AddToByteArray((long)m_vProduction.size());
	for (size_t i = 0; i < m_vProduction.size(); ++i)
	{
		setWriteSet.AddToByteArray((void*)&m_vProduction[i], sizeof(tagProduction));
	}
}

/// ����
void OneMakeTable::Decode(DBReadSet& setReadDB)
{
	m_lID = setReadDB.GetLongFromByteArray();
	setReadDB.GetBufferFromByteArray(m_szName, MAKE_TABLE_NAME_SIZE);
	setReadDB.GetBufferFromByteArray(m_szNarrate, MAKE_TABLE_NARRATE_SIZE);

	m_vExpend.clear();
	long lNum = setReadDB.GetLongFromByteArray();
	for (long i = 0; i < lNum; ++i)
	{
		tagExpend Expend;
		setReadDB.GetBufferFromByteArray(&Expend, sizeof(tagExpend));
		m_vExpend.push_back(Expend);
	}

	m_vProduction.clear();
	lNum = setReadDB.GetLongFromByteArray();
	for (long i = 0; i < lNum; ++i)
	{
		tagProduction Production;
		setReadDB.GetBufferFromByteArray(&Production, sizeof(tagProduction));
		m_vProduction.push_back(Production);
	}
}

/// ���û�����Ϣ
bool OneMakeTable::SetBaseInfo(long lID, const char *pName, const char *pNarrate)
{
	if(NULL == pName || NULL == pNarrate)
		return false;
	m_lID = lID;

	size_t lNameSize = strlen(pName);
	lNameSize = (lNameSize < MAKE_TABLE_NAME_SIZE) ? lNameSize : MAKE_TABLE_NAME_SIZE - 1;
	memcpy(m_szName, pName, lNameSize);

	size_t lNarrateSize = strlen(pNarrate);
	lNarrateSize = (lNarrateSize < MAKE_TABLE_NARRATE_SIZE) ? lNarrateSize : MAKE_TABLE_NARRATE_SIZE - 1;
	memcpy(m_szNarrate, pNarrate, lNarrateSize);

	return true;
}

/// ���һ���ķ���
bool OneMakeTable::AddExpend(tagExpend &Expend)
{
	for (size_t i = 0; i < m_vExpend.size(); ++i)
	{
		if(Expend == m_vExpend[i])
			return false;
	}
	m_vExpend.push_back(Expend);
	return true;
}

/// ���һ��������
bool OneMakeTable::AddProduction(tagProduction& Production)
{
	for (size_t i = 0; i < m_vProduction.size(); ++i)
	{
		if(Production == m_vProduction[i])
			return false;
	}
	m_vProduction.push_back(Production);
	return true;
}


//! GoodsTailorSetup
//!-------------------------------------------------------------------------------------------------
map<long, OneMakeTable*> GoodsTailorSetup::m_mapOneMakeTable;

/// ����
GoodsTailorSetup::GoodsTailorSetup(void)
{

}

/// ����
GoodsTailorSetup::~GoodsTailorSetup(void)
{
	Release();
}

/// ����
void GoodsTailorSetup::Code(DBWriteSet& setWriteSet)
{
	//! ����
	setWriteSet.AddToByteArray((long)m_mapOneMakeTable.size());

	//! ����
	map<long, OneMakeTable*>::iterator ite = m_mapOneMakeTable.begin();
	for (; ite != m_mapOneMakeTable.end(); ++ite)
	{
		ite->second->Code(setWriteSet);
	}
}

/// ����
void GoodsTailorSetup::Decode(DBReadSet& setReadDB)
{
	Release();

	long lNum = setReadDB.GetLongFromByteArray();

	for (long i = 0; i < lNum; ++i)
	{
		OneMakeTable* pOneMakeTable = new OneMakeTable();
		if(NULL != pOneMakeTable)
		{
			pOneMakeTable->Decode(setReadDB);
			if(!AddOneMakeTable(pOneMakeTable))
			{
				SAFE_DELETE(pOneMakeTable);
			}
		}
	}
}

/// ���������䷽
void GoodsTailorSetup::Release(void)
{
	map<long, OneMakeTable*>::iterator ite = m_mapOneMakeTable.begin();
	for (; ite != m_mapOneMakeTable.end(); ++ite)
	{
		SAFE_DELETE(ite->second);
	}
	m_mapOneMakeTable.clear();
}

/// ���һ���䷽
bool GoodsTailorSetup::AddOneMakeTable(OneMakeTable* pOneMakeTable)
{
	if(NULL == pOneMakeTable)
		return false;

	if(m_mapOneMakeTable.end() != m_mapOneMakeTable.find(pOneMakeTable->GetID()))
	{
		SAFE_DELETE(pOneMakeTable);
		return false;
	}

	m_mapOneMakeTable[pOneMakeTable->GetID()] = pOneMakeTable;
	return true;
}

/// �ҳ�һ���䷽
const OneMakeTable* GoodsTailorSetup::FindOneMakeTable(long lID)
{
	map<long, OneMakeTable*>::iterator ite = m_mapOneMakeTable.find(lID);
	if(m_mapOneMakeTable.end() == ite)
		return NULL;

	return ite->second;
}

/// ���طǸ�ְҵ�����䷽����
bool GoodsTailorSetup::LoadGoodsTailorSetup(const char *filename)
{
	TiXmlNode* pNode = NULL;

	CRFile* prfile = rfOpen(filename);
	if(NULL == prfile)
	{
		return false;
	}

	TiXmlDocument m_Tdoc(filename);
	//���뼼�������ļ�
	if(!m_Tdoc.LoadData(prfile->GetData(), prfile->GetDatalen()))
	{
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);

	pNode=m_Tdoc.FirstChild("GoodsTailor");
	if (pNode == NULL) 
		return false;

	TiXmlElement* pGoodsTailor = pNode->ToElement();//��ȡnode ��ָ��
	if (pGoodsTailor == NULL) 
		return false;

	//��ȡ����
	Release();

	TiXmlElement* pStuffList = pGoodsTailor->FirstChildElement("StuffList");
	for (; pStuffList!=NULL; pStuffList = pStuffList->NextSiblingElement("StuffList"))
	{
		OneMakeTable* pOneMakeTable = new OneMakeTable();
		if(NULL != pOneMakeTable)
		{
			long lID = atoi(pStuffList->Attribute("ID"));
			const char* pName = pStuffList->Attribute("Name");
			const char* pNarrate = pStuffList->Attribute("Narrate");

			if(pOneMakeTable->SetBaseInfo(lID, pName, pNarrate))
			{
				TiXmlElement* pStuff = pStuffList->FirstChildElement("Stuff");
				for (; pStuff!=NULL; pStuff = pStuff->NextSiblingElement("Stuff"))
				{
					tagExpend Expend;
					const char* pGoodsName = pStuff->Attribute("GoodsName");

					Expend.lGoodsIndex = GoodsSetup::QueryGoodsIDByOriginalName(pGoodsName);
					Expend.lGoodsNum = atoi(pStuff->Attribute("GoodsNum"));
					if(0 == Expend.lGoodsIndex)
					{
						Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_COMPOSE_40", pGoodsName));
						continue;
					}
					if(0 >= Expend.lGoodsNum)
					{
						Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_COMPOSE_41", pGoodsName));
						continue;
					}

					pOneMakeTable->AddExpend(Expend);
				}

				TiXmlElement* pProduction = pStuffList->FirstChildElement("Production");
				for (; pProduction!=NULL; pProduction = pProduction->NextSiblingElement("Production"))
				{
					tagProduction Production;
					const char* pGoodsName = pProduction->Attribute("GoodsName");

					Production.lGoodsIndex = GoodsSetup::QueryGoodsIDByOriginalName(pGoodsName);
					Production.lGoodsNum = atoi(pProduction->Attribute("GoodsNum"));
					Production.lOutOdds = atoi(pProduction->Attribute("Odds"));
					if(0 == Production.lGoodsIndex)
					{
						Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_COMPOSE_42", pGoodsName));
						continue;
					}
					if(0 >= Production.lGoodsNum)
					{
						Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_COMPOSE_41", pGoodsName));
						continue;
					}
					if(0 >= Production.lOutOdds)
					{
						Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_COMPOSE_43", pGoodsName));
						continue;
					}

					pOneMakeTable->AddProduction(Production);
				}
			}

			if(!AddOneMakeTable(pOneMakeTable))
			{
				Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_COMPOSE_44", pOneMakeTable->GetName(), pOneMakeTable->GetID()));
			}
		}
	}

	return true;
}