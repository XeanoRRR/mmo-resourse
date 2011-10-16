//********************************************************************************
// created: 2004/10/12 14:36:41
// file name: d:\Project\DBAccess\Worlddb\DBGoods.cpp
// file path: d:\Project\DBAccess\Worlddb
// file base: DBGoods
// file ext: cpp
// author: Bugs
//
// purpose: �洢�����Ʒ�����ݿ�
//********************************************************************************
#include "StdAfx.h"
#include ".\rsgoods.h"

#include "..\..\server\dbserver\appworld\dbplayer.h"
#include "..\..\server\dbserver\appworld\dbgood.h"
#include "..\..\server\dbserver\appworld\dbentitymanager.h"
#include "..\..\server\dbserver\dbserver\game.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "../public/DepotDef.h"
#include "../public/entityproperty.h"
#include "..\..\server\dbserver\appworld\dbgoodsgroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRSGoods::CRSGoods(void)
{
}
CRSGoods::~CRSGoods(void)
{
}
//��Buffer
bool CRSGoods::SaveBufferField(CDBGoods *pGoods, const string& szFiledName, _RecordsetPtr &rs)
{
	if(pGoods == NULL)	return false;

	VARIANT varBLOB;
	SAFEARRAY* psa;

	CEntityProperty* ep = pGoods->GetDataEntityManager().GetEntityProperty(szFiledName);
	if(NULL == ep) return false;

	long dataSize = ep->GetBufSize(0);
	BYTE* buffer = NULL;
	if(dataSize)
	{
		buffer = new BYTE[dataSize];
		ep->GetBufAttr(0, (void*)buffer, dataSize);

		long lSize=(long)dataSize;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound=0;
		rgsabound[0].cElements=lSize;
		psa=SafeArrayCreate(VT_UI1,1,rgsabound);

		for(long i=0;i<lSize;i++)
		{
			SafeArrayPutElement(psa,&i,&buffer[i]);
		}
		varBLOB.vt=VT_ARRAY|VT_UI1;
		varBLOB.parray=psa;

		try
		{
			rs->GetFields()->GetItem(szFiledName.c_str())->AppendChunk(varBLOB);
		}
		catch(_com_error e)
		{
			PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_SAVEGOODSERR), e);
			SAFE_DELETE_ARRAY(buffer);
			return false;
		}

		SafeArrayDestroy(psa);
	}
	SAFE_DELETE_ARRAY(buffer);
	return true;
}
//��Buffer
bool CRSGoods::LoadBufferField(CDBGoods *pGoods, const string& szFiledName, _RecordsetPtr &rs)
{
	if(pGoods == NULL)	return false;

	try
	{
		if(rs->GetadoEOF())
		{
			ReleaseRs(rs);
			return false;
		}

		long lSize=0;
		FieldsPtr fldPtr = rs->GetFields();
		FieldPtr itemPtr = fldPtr->GetItem(szFiledName.c_str());
		lSize = itemPtr->ActualSize;

		if(lSize > 0)
		{
			_variant_t varBLOB;
			varBLOB = itemPtr->GetChunk(lSize);

			BYTE *pBuf=NULL;
			SafeArrayAccessData(varBLOB.parray,(void**)&pBuf);
			SafeArrayUnaccessData(varBLOB.parray);

			// ���SKILL
			CEntityProperty* ep = pGoods->GetDataEntityManager().GetEntityProperty(szFiledName);
			if(ep)
				ep->SetBufAttr(0, pBuf, lSize);
		}
	}
	catch(_com_error e)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_LOADERR), e);
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------
// Function: SaveGoods
// Parameters:
// CDBPlayer *pPlayer �����Ϣ
// CDBGoods *pGoods ��Ʒ��Ϣ
// BYTE lPlace ��Ʒ��ŵص㣨0���ϣ�1���ϣ�2������3�ֿ⣩
// BYTE lPos λ��
// Return:
// true �ɹ�
// false ʧ��
//
// Purpose: �洢�����Ʒ����CRsPlayer���SaveGoods���á��洢ǰ�Ƚ��ɵ���Ʒ
// �����ݿ�ɾ�����ڰѷ������е�д�����ݿ⡣
// ɾ����Ʒ��¼ʱ���Ѿ�����Ӧ����Ʒ�������ԣ��͸�������ֵ�����˼�
// ��ɾ����
// [2004/12/10]
//--------------------------------------------------------------------------------
bool CRSGoods::SaveGood(const CGUID& ownerID, CDBGoodsGroup* goodsGroup, _ConnectionPtr& cn)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_CNPTRNULL));
		return false;
	}
	if(!goodsGroup) return false;
	if(goodsGroup->GetGoodsGroupMap().size() == 0) return true;

	char szPlayerID[128];
	ownerID.tostring(szPlayerID);
	
	string iniName   = "goods";
	string tableName = "equipment";
	CDataEntityManager* pObjAttrDef = NULL;
	std::map<string, CDataEntityManager*>::iterator itr = GetGame()->GetDBEntityManager()->GetObjAttrDef().find(iniName);
	if(itr != GetGame()->GetDBEntityManager()->GetObjAttrDef().end())
		pObjAttrDef = itr->second;
	if(!pObjAttrDef) return false;
	_RecordsetPtr rs;
	try
	{
		// ����Ʒ���������2048�ֽ�
		char sql[1024];

		// ����RS
		TESTHR(CreateRs(rs));
		// ���±�
		sprintf(sql, "select * from equipment where playerid = '%s'", szPlayerID);
		TESTHR(OpenRs(sql, rs, cn, adOpenForwardOnly, adLockOptimistic));

		map<CGUID, CDBGoods*>::iterator goodsItr = goodsGroup->GetGoodsGroupMap().begin();
		for(; goodsItr != goodsGroup->GetGoodsGroupMap().end(); goodsItr++)
		{	
			// ���һ���¼�¼
			rs->AddNew();

			CGUID id;
			// ��Ʒ���ݿ���
			CGUID::CreateGUID(id);
			char szid[128];
			id.tostring(szid);
			rs->PutCollect("id", szid);

			//! 
			CDataEntityManager::EntityPropertyMapItr attrItr = pObjAttrDef->GetEntityPropertyMap().begin();
			for (; attrItr != pObjAttrDef->GetEntityPropertyMap().end(); attrItr++)
			{
				if(attrItr->second)
				{
					if(attrItr->second->GetDBTableName() == tableName)
					{
						// ��������ֶ�ֵ
						CEntityProperty* ep = goodsItr->second->GetDataEntityManager().GetEntityProperty(attrItr->second->GetEPName());
						if(ep)
						{
							if(ep->GetDBTableName() == tableName)
							{
								switch(ep->GetDataType())
								{
								case DATA_OBJECT_TIME:
									{
										char szTimeValue[2048];
										DWORD timeValue[6] = {0};
										ep->GetBufAttr(0, (void*)&timeValue[0], sizeof(DWORD)*6);
										sprintf(szTimeValue, "%d-%d-%d %d:%d:%d", timeValue[0], timeValue[1], timeValue[2],
											timeValue[3], timeValue[4], timeValue[5]);
										rs->PutCollect(ep->GetEPName().c_str(), szTimeValue);
									}
									break;
								case  DATA_OBJECT_STRING:
									{
										rs->PutCollect(ep->GetEPName().c_str(), ep->GetStringAttr(0));
									}
									break;
								case DATA_OBJECT_BUFFER:
									{
										SaveBufferField(goodsItr->second, ep->GetEPName(), rs);
									}
									break;
								case DATA_OBJECT_GUID:
									{
										CGUID tGUID;
										ep->GetGuidAttr(0, tGUID);
										char szGuid[128];
										tGUID.tostring(szGuid);
										rs->PutCollect(ep->GetEPName().c_str(), szGuid);
									}
									break;
								case DATA_OBJECT_BOOL:
								case DATA_OBJECT_CHAR:
								case DATA_OBJECT_BYTE:
									{
										rs->PutCollect(ep->GetEPName().c_str(), (BYTE)ep->GetLongAttr(0));
									}
									break;
								case DATA_OBJECT_SHORT:
								case DATA_OBJECT_WORD:
								case DATA_OBJECT_USHORT:
									{
										rs->PutCollect(ep->GetEPName().c_str(), (WORD)ep->GetLongAttr(0));
									}
									break;
								case DATA_OBJECT_FLOAT:
								case DATA_OBJECT_LONG:
								case DATA_OBJECT_ULONG:
								case DATA_OBJECT_DWORD:
									{
										rs->PutCollect(ep->GetEPName().c_str(), (DWORD)ep->GetLongAttr(0));
									}
									break;
								}
							}
						}
					}
				}
			}
		}
		rs->Update();
		ReleaseRs(rs);
		return true;
	}
	catch(_com_error &e)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_SAVEGOODSERR), e);
		return false;
	}
	return true;
}
bool CRSGoods::DeleteContainerGoods(CDBGoodsGroup* goodsGroup, const CGUID& ownerID, _ConnectionPtr& cn)
{
/*	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_CNPTRNULL));
		return false;
	}
	if(NULL == goodsGroup) return false;

	long goodsPlace = INVILID_VALUE;
	char sql[5120];
	char szPlayerID[128];
	ownerID.tostring(szPlayerID);

	// sql ���ͷ
	sprintf(sql, "DELETE FROM equipment WHERE PlayerID='%s' AND (", szPlayerID);
	// sql �����
	map<long, set<CGUID>>::iterator placeItr = goodsGroup->GetGoodsGroupMap().begin();
	for (; placeItr != goodsGroup->GetDirtyGoodsMap().end(); placeItr++)
	{
		char placeStr[128];
		itoa(placeItr->first, placeStr, 10);
		switch(placeItr->first) // �����ӱ�����λ����
		{
		case PLACE_PACKET:
		case PLACE_EQUIP:		
		case PLACE_WALLET:
		case PLACE_SILVERWALLET:
		case PLACE_GEM:
		case eDCT_Gold:
		case eDCT_Gem:
		case eDCT_Primary:
		case eDCT_Primary2:
		case eDCT_Secondary:
		case eDCT_Secondary1:
		case eDCT_Secondary2:
		case eDCT_Secondary3:
		case eDCT_Secondary4:
			{
				strcat(sql, "place=");
				strcat(sql, placeStr);
			}
			break;
		case PLACE_PACK:
			continue;
		case PLACE_PACK1:
		case PLACE_PACK2:
		case PLACE_PACK3:
		case PLACE_PACK4:
		case PLACE_PACK5:
			{
				// �����ɾ�����ӱ�������䲿��
				strcat(sql, "(place=10000 AND position=");
				strcat(sql, placeStr);
				strcat(sql, ") OR ");

				// �����ɾ�����ӱ����������䲿��
				strcat(sql, "place=");
				strcat(sql, placeStr);
				strcat(sql, " OR ");

				long strLen = strlen(sql);
				sql[strLen-4] = '\0';
			}
			break;
		}
		strcat(sql, " OR ");
	}
	long strLen = strlen(sql);
	sql[strLen-4] = '\0';
	strcat(sql, ")");

	// ��ɾ������ҵ�����ͬ������Ʒ
	if( !ExecuteCn(sql, cn) )
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_DELEQUIPMENTERR));
		return false;
	}*/
	return true;
}
bool CRSGoods::SaveGoodsFiled(const CGUID& ownerID, CDBGoodsGroup* goodsGroup, _ConnectionPtr& cn)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_CNPTRNULL));
		return false;
	}

	long nTransactionCounter = 0;
	char szPlayerID[128];
	ownerID.tostring(szPlayerID);

	try
	{	
		//TESTHR( DeleteContainerGoods(goodsGroup, ownerID, cn) );
		TESTHR(DeleteGoods(ownerID, cn));
		TESTHR( SaveGood(ownerID, goodsGroup, cn) );
	}
	catch(_com_error &e)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_SAVEGOODSERR), e);	
		return false;
	}
	return true;
}

bool CRSGoods::DeleteGoods(const CGUID& dwPlayerID, _ConnectionPtr& cn)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_CNPTRNULL));
		return false;
	}

	char sql[256];

	// ɾ����ɫ��Ʒ
	char szPlayerID[128];
	dwPlayerID.tostring(szPlayerID);
	sprintf(sql,"DELETE FROM equipment WHERE PlayerID='%s'", szPlayerID);
	if(!ExecuteCn(sql, cn))
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_CLEARALLFAIL));
		return false;
	}

	return true;
}


//--------------------------------------------------------------------------------
// Function: LoadGoodsFiled
// Parameters: CDBPlayer* pPlayer
// CDBGoods* pGoods
// Return: bool
// true �ɹ�
// false ʧ��
// Purpose: �����ݿ��е�player_goods���ж������ݵ�CDBPlayer���е�m_cHand,
// m_cEquipment��m_cPackage.
// [2004/12/14]
//--------------------------------------------------------------------------------

CDBGoods* CRSGoods::FindGoodsByPos(CDBPlayer* pPlayer,long place, long pos)
{
	CDBGoods* pGoods = NULL;
	//map<CGUID, CDBGoods*>::iterator itrEquip = pPlayer->GetmapEquipGoods().begin();
	//for(; itrEquip != pPlayer->GetmapEquipGoods().end(); itrEquip++)
	//{
	//	if(itrEquip->second->GetPos() == pos && itrEquip->second->GetDPlace() == place)
	//		pGoods = itrEquip->second;
	//}
	return pGoods;
}

bool CRSGoods::LoadGoods(CDBPlayer* pPlayer, CDBGoodsGroup* goodsGroup, _ConnectionPtr& cn)
{
	if(goodsGroup == NULL) return false;

	// �����ʱ����
	goodsGroup->Clear();

	_RecordsetPtr rs;
	string sID;
	char sql[512];

	// �������Բ���
	_variant_t vGoodsID;
	_variant_t vPlayerID;
	CGUID guid;
	CGUID pguid;

	CDBGoods *pGoods = NULL;
	bool IsLocalConnection = false;

	char szID[128];
	pPlayer->GetExID().tostring(szID);
	if(NULL == cn)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_CNPTRNULL));
		return false;
	}
	try
	{
		TESTHR(CreateRs(rs));

		//select a.*, b.type,b.modifierValue1, b.modifierValue2 from player_goods as a left join extend_properties as b on a.id=b.id where playerid=10002 order by playerid

		/* �����ɫ������Ʒ */

		//sprintf(sql, "select a.*, b.type,b.modifierValue1, b.modifierValue2 from player_goods as a left join extend_properties as b on a.id=b.id where playerid=%d order by playerid", pPlayer->GetID());
		sprintf(sql,"select * from equipment where playerid='%s' order by place", szID);
		TESTHR(OpenRs(sql, rs, cn));

		long place = 0;
		while(!rs->GetadoEOF())
		{	
			// �������Բ���
			vGoodsID = rs->GetCollect("goodsID");
			guid = CGUID(((char*)(_bstr_t)vGoodsID));
			vPlayerID = rs->GetCollect("PlayerID");
			pguid = CGUID(((char*)(_bstr_t)vPlayerID));
			
			pGoods = new CDBGoods(guid);
			
			CDataEntityManager::EntityPropertyMapItr epItr = pGoods->GetDataEntityManager().GetEntityPropertyMap().begin();
			for(; epItr != pGoods->GetDataEntityManager().GetEntityPropertyMap().end(); epItr++)
			{
				switch(epItr->second->GetDataType())
				{
				case DATA_OBJECT_TIME:
					{
						_variant_t var = rs->GetCollect(epItr->first.c_str());
						long timeBuf[6] = {0};
						if( var.vt != VT_NULL ) 
						{
							SYSTEMTIME st;
							VariantTimeToSystemTime(var.date, &st);

							timeBuf[0] = st.wYear;
							timeBuf[1] = st.wMonth;
							timeBuf[2] = st.wDay;
							timeBuf[3] = st.wHour;
							timeBuf[4] = st.wMinute;
							timeBuf[5] = st.wSecond;
							epItr->second->SetBufAttr(0, (void*)&timeBuf[0], sizeof(DWORD)*6);
						}
					}
					break;
				case  DATA_OBJECT_STRING:
					{
						string strValue = (_bstr_t)rs->GetCollect(epItr->first.c_str());
						epItr->second->SetStringAttr(0, strValue.c_str());
					}
					break;
				case DATA_OBJECT_BUFFER:
					{
						LoadBufferField(pGoods, epItr->first, rs);
					}
					break;
				case DATA_OBJECT_GUID:
					{
						CGUID tGUID(_bstr_t(rs->GetCollect(epItr->first.c_str())));
						epItr->second->SetGuidAttr(0, tGUID);
					}
					break;
				case DATA_OBJECT_FLOAT:
				case DATA_OBJECT_LONG:
				case DATA_OBJECT_SHORT:
				case DATA_OBJECT_ULONG:
				case DATA_OBJECT_USHORT:
				case DATA_OBJECT_BOOL:
				case DATA_OBJECT_CHAR:
				case DATA_OBJECT_BYTE:
				case DATA_OBJECT_WORD:
				case DATA_OBJECT_DWORD:
					{
						long value = rs->GetCollect(epItr->first.c_str());
						epItr->second->SetLongAttr(0, value);
						if(epItr->first == "place")
							place = value;
					}
					break;
				}
			}

			// ���±�־
			goodsGroup->GetGoodsGroupMap()[guid] = pGoods;
			rs->MoveNext();
		}
		ReleaseRs(rs);
		return true;
	}	
	catch(_com_error &e)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_LOADERR), e);	
		ReleaseRs(rs);
		return false;
	}

}

bool CRSGoods::UpdatePlayerGoldCoins(CDBGoods *pGoods, const CGUID& ownerID, _ConnectionPtr& cn)
{
	_RecordsetPtr rs;
	try
	{
		// ����Ʒ���������2048�ֽ�
		char sql[1024];

		// ����RS
		TESTHR(CreateRs(rs));
		// ���±�
		char szPlayerID[128];
		ownerID.tostring(szPlayerID);
		sprintf(sql, "select * from equipment where playerid = '%s'", szPlayerID);
		TESTHR(OpenRs(sql, rs, cn, adOpenForwardOnly, adLockOptimistic));
		if(rs->GetadoEOF())
		{
			rs->AddNew();
			CGUID id;
			// ��Ʒ���ݿ���
			CGUID::CreateGUID(id);
			char szid[128];
			id.tostring(szid);
			rs->PutCollect("id", szid);
		}

		string tableName = "equipment";
		CDataEntityManager::EntityPropertyMapItr attrItr = pGoods->GetDataEntityManager().GetEntityPropertyMap().begin();
		for (; attrItr != pGoods->GetDataEntityManager().GetEntityPropertyMap().end(); attrItr++)
		{
			if(attrItr->second)
			{
				if(attrItr->second->GetDBTableName() == tableName)
				{
					// ��������ֶ�ֵ
					CEntityProperty* ep = attrItr->second;
					if(ep)
					{
						if(ep->GetDBTableName() == tableName)
						{
							switch(ep->GetDataType())
							{
							case DATA_OBJECT_TIME:
								{
									char szTimeValue[2048];
									DWORD timeValue[6] = {0};
									ep->GetBufAttr(0, (void*)&timeValue[0], sizeof(DWORD)*6);
									sprintf(szTimeValue, "%d-%d-%d %d:%d:%d", timeValue[0], timeValue[1], timeValue[2],
										timeValue[3], timeValue[4], timeValue[5]);
									rs->PutCollect(ep->GetEPName().c_str(), szTimeValue);
								}
								break;
							case  DATA_OBJECT_STRING:
								{
									rs->PutCollect(ep->GetEPName().c_str(), ep->GetStringAttr(0));
								}
								break;
							case DATA_OBJECT_BUFFER:
								{
									SaveBufferField(pGoods, ep->GetEPName(), rs);
								}
								break;
							case DATA_OBJECT_GUID:
								{
									CGUID tGUID;
									ep->GetGuidAttr(0, tGUID);
									char szGuid[128];
									tGUID.tostring(szGuid);
									rs->PutCollect(ep->GetEPName().c_str(), szGuid);
								}
								break;
							case DATA_OBJECT_BOOL:
							case DATA_OBJECT_CHAR:
							case DATA_OBJECT_BYTE:
								{
									rs->PutCollect(ep->GetEPName().c_str(), (BYTE)ep->GetLongAttr(0));
								}
								break;
							case DATA_OBJECT_SHORT:
							case DATA_OBJECT_WORD:
							case DATA_OBJECT_USHORT:
								{
									rs->PutCollect(ep->GetEPName().c_str(), (WORD)ep->GetLongAttr(0));
								}
								break;
							case DATA_OBJECT_FLOAT:
							case DATA_OBJECT_LONG:
							case DATA_OBJECT_ULONG:
							case DATA_OBJECT_DWORD:
								{
									rs->PutCollect(ep->GetEPName().c_str(), (DWORD)ep->GetLongAttr(0));
								}
								break;
							}
						}
					}
				}
			}
		}
		rs->Update();
		ReleaseRs(rs);
		return true;
	}
	catch(_com_error &e)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSGOODS,STR_DBS_GOODS_SAVEGOODSERR), e);
		return false;
	}
	return true;
}