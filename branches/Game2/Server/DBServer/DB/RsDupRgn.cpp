#include "StdAfx.h"
#include "RsDupRgn.h"
#include "..\..\server\dbserver\AppWorld\DBDupRgn.h"
#include "..\..\server\dbserver\AppWorld\DBentityManager.h"
#include "../../server/DBServer/AppWorld/DBAccessThread.h"
#include "..\..\server\DBServer/dbserver/game.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "..\..\server\DBServer\AppWorld\DBRgnGoods.h"
#include "..\..\server\DBServer\AppWorld\DBRgnGoodsGroup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRsDupRgn::CRsDupRgn(void)
{
}

CRsDupRgn::~CRsDupRgn(void)
{
}
//��Buffer
bool CRsDupRgn::SaveBufferField(CBaseDBEntity *pGoods, const char* szFiledName, _RecordsetPtr &rs)
{
	if(pGoods == NULL)	return false;
	if(szFiledName == NULL) return false;

	VARIANT varBLOB;
	SAFEARRAY* psa;

	CEntityProperty* ep = pGoods->GetDataEntityManager().GetEntityProperty(string(szFiledName));
	if(NULL == ep) return false;

	BYTE* buffer = NULL;
	long dataSize = ep->GetBufSize(0);
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
			rs->GetFields()->GetItem(szFiledName)->AppendChunk(varBLOB);
		}
		catch(_com_error e)
		{
			PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN, STR_DBS_DUPRGN_SAVERGNOBJFAIL), e);
			SAFE_DELETE_ARRAY(buffer);
			return false;
		}
		SafeArrayDestroy(psa);
	}
	SAFE_DELETE_ARRAY(buffer);
	return true;
}
//��Buffer
bool CRsDupRgn::LoadBufferField(CBaseDBEntity *pGoods, const char* szFiledName, _RecordsetPtr &rs)
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
		FieldPtr itemPtr = fldPtr->GetItem(szFiledName);
		lSize = itemPtr->ActualSize;

		if(lSize > 0)
		{
			_variant_t varBLOB;
			varBLOB = itemPtr->GetChunk(lSize);

			BYTE *pBuf=NULL;
			SafeArrayAccessData(varBLOB.parray,(void**)&pBuf);
			SafeArrayUnaccessData(varBLOB.parray);

			// ���SKILL
			CEntityProperty* ep = pGoods->GetDataEntityManager().GetEntityProperty(string(szFiledName));
			if(ep)
				ep->SetBufAttr(0, pBuf, lSize);
		}
	}
	catch(_com_error e)
	{
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_LOADRGNOBJERR), e);
		return false;
	}
	return true;
}

//װ�����еĳ�ս����
bool CRsDupRgn::LoadAll(_ConnectionPtr& cn)
{
	if(cn == NULL)
	{
		AddLogText(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_CNPTRNULL));
		return false;
	}

	_RecordsetPtr rs;
	try
	{
		GetGame()->GetDBEntityManager()->ClearDupRgnMap();

		TESTHR(CreateRs(rs));
		TESTHR(OpenRs("SELECT * FROM CSL_DUPRGN_PROPERTY", rs, cn));
		while(!rs->GetadoEOF())
		{
			// GUID
			CGUID guid((_bstr_t)rs->GetCollect("guid"));
			CDBDupRgn* dupRgn = new CDBDupRgn(guid);

			char szGUID[128];
			dupRgn->GetExID().tostring(szGUID);

			CDataEntityManager::EntityPropertyMapItr epItr = dupRgn->GetDataEntityManager().GetEntityPropertyMap().begin();
			for(; epItr != dupRgn->GetDataEntityManager().GetEntityPropertyMap().end(); epItr++)
			{
				if(epItr->second->GetDBTableName() == "CSL_DUPRGN_PROPERTY")
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
							LoadBufferField((CBaseDBEntity*)dupRgn, epItr->first.c_str(), rs);
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
						}
						break;
					}
				}
			}

			char outStrx[128];			
			// ��ȡ��������Ʒ
			if(LoadDupRgnObj(dupRgn->GetExID(),dupRgn->GetGoodsGroupPtr(), cn) == false)
			{
				sprintf(outStrx, CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_LOADRGNOBJERR), szGUID);
				PrintErr(outStrx);
				return false;
			}
			
			GetGame()->GetDBEntityManager()->GetDBDupRgnMap()[guid] = dupRgn;

			sprintf(outStrx, CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_LOADRGNOK), szGUID);
			AddLogText(outStrx);

			// ������¼
			rs->MoveNext();
		}

		ReleaseRs(rs);

		return true;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_LOADRGNERR), e);
		return true;
	}

	return true;
}
//�������еĳ�ս��
bool CRsDupRgn::SaveAll(void)
{
	_ConnectionPtr cn;
	_RecordsetPtr rs;
	char sql[500] = "";
	ReleaseRs(rs);
	return true;
}
//! ��ȡ���˷�����Ϣ
void CRsDupRgn::LoadDupRgn(CDBDupRgn* dupRgn, _ConnectionPtr& cn)
{
	_RecordsetPtr rs;
	char sql[500] = "";
	if(cn == NULL)
	{
		AddLogText(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_CNPTRNULL));
		return;
	}

	try
	{
		TESTHR(CreateRs(rs));

		char szGUID[128];
		dupRgn->GetExID().tostring(szGUID);

	//	TESTHR(OpenRs("CSL_DUPRGN_PROPERTY", rs, cn, adOpenForwardOnly, adLockOptimistic, adCmdTable));
		sprintf(sql, "SELECT * FROM CSL_DUPRGN_PROPERTY WHERE guid='%s'", szGUID);
	//	// �Ȳ��Ҹø��˷��ݵĳ�Ա
		TESTHR(OpenRs(sql, rs, cn));
		CDataEntityManager::EntityPropertyMapItr epItr = dupRgn->GetDataEntityManager().GetEntityPropertyMap().begin();
		for(; epItr != dupRgn->GetDataEntityManager().GetEntityPropertyMap().end(); epItr++)
		{
			if(epItr->second->GetDBTableName() == "CSL_DUPRGN_PROPERTY")
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
						LoadBufferField((CBaseDBEntity*)dupRgn, epItr->first.c_str(), rs);
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
					}
					break;
				}
			}
		}
		ReleaseRs(rs);
		// ��ȡ��������Ʒ
		LoadDupRgnObj(dupRgn->GetExID(), dupRgn->GetGoodsGroupPtr(), cn);
		return;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_LOADONERGNFAIL), e);
		return;
	}

	return;
}
//! ������˷�����Ϣ
void CRsDupRgn::SaveDupRgn(CDBDupRgn* dupRgn, _ConnectionPtr& cn)
{	
	if(cn == NULL)
	{
		AddLogText(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_FINDDUPRGNPROPERTYERR));
		return;
	}

	_RecordsetPtr rs;
	char sql[1024] = "";
	try
	{
		TESTHR(CreateRs(rs));

		char szGUID[128];
		dupRgn->GetExID().tostring(szGUID);
		
		sprintf(sql, "select * from dbo.CSL_DUPRGN_PROPERTY where guid='%s'", szGUID);
		TESTHR(OpenRs(sql, rs, cn, adOpenDynamic, adLockOptimistic));
	
		bool isNewCreated = false;
		if(rs->GetadoEOF()) // û�м�¼,����һ��
		{
			rs->AddNew();
			isNewCreated = true;
		}

		set<string>::iterator epItr = dupRgn->GetDirtyAttrDataMap().begin();
		for(; epItr != dupRgn->GetDirtyAttrDataMap().end(); epItr++)
		{
			CEntityProperty* ep = dupRgn->GetDataEntityManager().GetEntityProperty(*epItr);
			if(ep)
			{
				if(ep->GetDBTableName() == "CSL_DUPRGN_PROPERTY")
				{
					switch(ep->GetDataType())
					{
					case DATA_OBJECT_TIME:
						{
							char szTimeValue[2048];
							DWORD timeValue[6] = {0};
							ep->GetBufAttr(0, (void*)timeValue, 6*sizeof(DWORD));
							if(szTimeValue[0] != 0 && szTimeValue[1] != 0 && szTimeValue[2] != 0
								&& szTimeValue[3] != 0 && szTimeValue[4] != 0 && szTimeValue[5] != 0)
							{
								sprintf(szTimeValue, "%d-%d-%d %d:%d:%d", timeValue[0], timeValue[1], timeValue[2],
									timeValue[3], timeValue[4], timeValue[5]);
								rs->PutCollect((*epItr).c_str(), szTimeValue);
							}
						}
						break;
					case  DATA_OBJECT_STRING:
						{
							const char* pStr = ep->GetStringAttr(0);
							if(pStr)
								rs->PutCollect((*epItr).c_str(), pStr);
						}
						break;
					case DATA_OBJECT_BUFFER:
						{
							SaveBufferField((CBaseDBEntity*)dupRgn, (*epItr).c_str(), rs);
						}
						break;
					case DATA_OBJECT_GUID:
						{
							CGUID tGUID;
							ep->GetGuidAttr(0, tGUID);
							char szGUID[128];
							tGUID.tostring(szGUID);
							if( (ep->GetDbFlag() == 0) ) // 0:������,1:����
								rs->PutCollect((*epItr).c_str(), szGUID);
							else if(isNewCreated) // �����������Ǹմ�������
								rs->PutCollect((*epItr).c_str(), szGUID);
						}
						break;
					case DATA_OBJECT_BOOL:
					case DATA_OBJECT_CHAR:
					case DATA_OBJECT_BYTE:
						{
							rs->PutCollect((*epItr).c_str(), (BYTE)ep->GetLongAttr(0));
						}
						break;
					case DATA_OBJECT_SHORT:
					case DATA_OBJECT_WORD:
					case DATA_OBJECT_USHORT:
						{
							rs->PutCollect((*epItr).c_str(), (WORD)ep->GetLongAttr(0));
						}
						break;
					case DATA_OBJECT_FLOAT:
					case DATA_OBJECT_LONG:
					case DATA_OBJECT_ULONG:
					case DATA_OBJECT_DWORD:
						{
							rs->PutCollect((*epItr).c_str(), (DWORD)ep->GetLongAttr(0));
						}
						break;
					}
				}
			}
		}
		rs->Update();
		ReleaseRs(rs);
		return;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_SAVERGNOBJFAIL), e);
		return;
	}

	return;
}

//! ɾ�����˷�����Ϣ
void CRsDupRgn::DeleteDupRgn(CDBDupRgn* dupRgn)
{
	_RecordsetPtr rs;
	char sql[1024] = "";
	try
	{
		char szGUID[128];
		dupRgn->GetExID().tostring(szGUID);

		sprintf(sql, "DELETE FROM CSL_DUPRGN_PROPERTY WHERE guid='%s'", szGUID);
		// ��ɾ���ø��˷��ݵĳ�Ա
		if(!ExecuteCn(sql, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr()))
		{
			PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_DELDUPRGNPROPERTYERR));
			return;
		}

		// ɾ����������Ʒ
		DeleteDupRgnObj(dupRgn->GetExID());
		return;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_DELRGNOBJFAIL), e);
		return;
	}

	return;
}
//! ��ȡ���˷����ڵ���Ʒ��Ϣ
bool    CRsDupRgn::LoadDupRgnObj(const CGUID& rgnGUID, CDBRgnGoodsGroup* goodsGroup, _ConnectionPtr& cn)
{
	bool ret = false;

	if(cn == NULL)
	{
		AddLogText(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_CNPTRNULL));
		return ret;
	}

	_RecordsetPtr rs;
	char sql[1024] = "";
	try
	{
		TESTHR(CreateRs(rs));

		char szGUID[128];
		rgnGUID.tostring(szGUID);

		// �Ȳ��Ҹø��˷��ݵĳ�Ա
		sprintf(sql, "SELECT * FROM CSL_DUPRGN_GOODS WHERE DupRgnGUID='%s'", szGUID);
		TESTHR(OpenRs(sql, rs, cn));

		while(!rs->GetadoEOF())
		{
			// �������Բ���
			_variant_t vGoodsID = rs->GetCollect("guid");
			CGUID guid = CGUID(((char*)(_bstr_t)vGoodsID));

			CDBRgnGoods* pGoods = new CDBRgnGoods(guid);
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
						LoadBufferField((CBaseDBEntity*)pGoods, epItr->first.c_str(), rs);
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
					}
					break;
				}
			}

			goodsGroup->GetGoodsGroupMap()[pGoods->GetExID()] = pGoods;
			// ������¼
			rs->MoveNext();
		}

		ReleaseRs(rs);
		
		return true;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_LOADRGNGOODSFAIL), e);
		return false;
	}

	return true;
}
//! ������˷����ڵ���Ʒ��Ϣ
bool    CRsDupRgn::SaveDupRgnObj(const CGUID& ownerID, map<CGUID,CDBRgnGoods*>& goodsGroup, _ConnectionPtr& cn)
{

	if(cn == NULL)
	{
		AddLogText(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_CNPTRNULL));
		return false;
	}

	_RecordsetPtr rs;
	// ����Ʒ���������2048�ֽ�
	char *sql = new char[1024 * (goodsGroup.size()+2)];
	
	string iniName   = "phrgnobj";
	string tableName = "CSL_DUPRGN_GOODS";
	try
	{
		TESTHR(CreateRs(rs));

		char szGUID[128];
		ownerID.tostring(szGUID);

		sprintf(sql, "DELETE FROM CSL_DUPRGN_GOODS WHERE DupRgnGUID='%s'", szGUID);
		// ��ɾ���ø��˷��ݵĳ�Ա
		if(!ExecuteCn(sql, cn))
		{
			PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_DELRGNOBJFAIL));
			return false;
		}

		// ���sql���
		memset(sql, 0, 1024 * (goodsGroup.size()+2));

		// �������ͷ
		strcpy(sql, "INSERT INTO CSL_DUPRGN_GOODS(");

		CDataEntityManager* pObjAttrDef = NULL;
		std::map<string, CDataEntityManager*>::iterator itr = GetGame()->GetDBEntityManager()->GetObjAttrDef().find(iniName);
		if(itr != GetGame()->GetDBEntityManager()->GetObjAttrDef().end())
			pObjAttrDef = itr->second;

		if(!pObjAttrDef) return false;

		//! ���������
		CDataEntityManager::EntityPropertyMapItr attrItr = pObjAttrDef->GetEntityPropertyMap().begin();
		for (; attrItr != pObjAttrDef->GetEntityPropertyMap().end(); attrItr++)
		{
			if(attrItr->second->GetDBTableName() == tableName)
			{
				CWrapDataObject* wdo = attrItr->second->GetDataObject(0);
				if(wdo)
				{
					// ��������ֶ���
					strcat(sql, attrItr->second->GetEPName().c_str());
					strcat(sql, ",");
				}
			}
		}

		long sqlLen = strlen(sql);
		sql[sqlLen-1] = '\0';
		strcat(sql, ") select ");

		map<CGUID,CDBRgnGoods*>::iterator goodsItr = goodsGroup.begin();
		for(; goodsItr != goodsGroup.end(); goodsItr++)
		{	
			//! ���������
			attrItr = pObjAttrDef->GetEntityPropertyMap().begin();
			for (; attrItr != pObjAttrDef->GetEntityPropertyMap().end(); attrItr++)
			{
				if(attrItr->second)
				{
					if(attrItr->second->GetDBTableName() == tableName)
					{
						// ��������ֶ�ֵ
						CEntityProperty* ep = (goodsItr->second)->GetDataEntityManager().GetEntityProperty(attrItr->second->GetEPName());
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
										//rs->PutCollect((*epItr).c_str(), szTimeValue);
										strcat(sql, "'");
										strcat(sql, szTimeValue);
										strcat(sql, "',");
									}
									break;
								case  DATA_OBJECT_STRING:
									{
										//rs->PutCollect((*epItr).c_str(), );
										strcat(sql, "'");
										const char* pStr = ep->GetStringAttr(0);
										if(pStr)
											strcat(sql, pStr);
										else
											strcat(sql, "");
										strcat(sql, "',");
									}
									break;
								case DATA_OBJECT_BUFFER:
									{
										//SaveBufferField((*goodsItr), (*epItr).c_str(), rs);
									}
									break;
								case DATA_OBJECT_GUID:
									{
										CGUID tGUID;
										ep->GetGuidAttr(0, tGUID);
										char szGuid[128];
										tGUID.tostring(szGuid);
										//rs->PutCollect((*epItr).c_str(), szGuid);
										strcat(sql, "'");
										strcat(sql, szGuid);
										strcat(sql, "',");
									}
									break;
								case DATA_OBJECT_BOOL:
								case DATA_OBJECT_CHAR:
								case DATA_OBJECT_BYTE:
									{
										//rs->PutCollect((*epItr).c_str(), (BYTE)ep->GetLongAttr(0));
										char value[32];
										memset(value, 0, sizeof(value));
										itoa((BYTE)ep->GetLongAttr(0), value, 10);
										strcat(sql, value);
										strcat(sql, ",");
									}
									break;
								case DATA_OBJECT_SHORT:
								case DATA_OBJECT_WORD:
								case DATA_OBJECT_USHORT:
									{
										//rs->PutCollect((*epItr).c_str(), (WORD)ep->GetLongAttr(0));
										char value[32];
										memset(value, 0, sizeof(value));
										itoa((WORD)ep->GetLongAttr(0), value, 10);
										strcat(sql, value);
										strcat(sql, ",");
									}
									break;
								case DATA_OBJECT_FLOAT:
								case DATA_OBJECT_LONG:
								case DATA_OBJECT_ULONG:
								case DATA_OBJECT_DWORD:
									{
										//rs->PutCollect((*epItr).c_str(), (DWORD)ep->GetLongAttr(0));
										char value[32];
										memset(value, 0, sizeof(value));
										itoa((DWORD)ep->GetLongAttr(0), value, 10);
										strcat(sql, value);
										strcat(sql, ",");
									}
									break;
								}
							}
						}
					}
				}
			}

			sqlLen = strlen(sql);
			sql[sqlLen-1] = '\0';

			strcat(sql, " union all select ");
		}
		sqlLen = strlen(sql);
		sql[sqlLen-17] = '\0';

		TESTHR(ExecuteCn(sql, cn));
		SAFE_DELETE_ARRAY(sql);
		return true;
	}
	catch (_com_error &e)
	{
		SAFE_DELETE_ARRAY(sql);
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_SAVERGNGOODSFAIL), e);
		return false;
	}
	SAFE_DELETE_ARRAY(sql);
	return false;
}

//! ɾ�����˷����ڵ���Ʒ��Ϣ
void    CRsDupRgn::DeleteDupRgnObj(const CGUID& guid)
{
	_RecordsetPtr rs;
	char sql[500] = "";
	try
	{
		TESTHR(CreateRs(rs));

		char szGUID[128];
		guid.tostring(szGUID);

		sprintf(sql, "DELETE FROM CSL_DUPRGN_GOODS WHERE DupRgnGUID='%s'", szGUID);
		// ��ɾ���ø��˷��ݵĳ�Ա
		TESTHR(OpenRs(sql, rs, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr()));

		ReleaseRs(rs);
		return;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_DELRGNOBJFAIL), e);
		return;
	}

	return;
}
// GS��ѯָ����ҵĸ��˷��ݸ���GUID
void	CRsDupRgn::QuestPHGuidByPlayerID(const char* playerName, CGUID& retPHGUID)
{
	_RecordsetPtr rs, rs1;
	char sql[500] = "";
	try
	{
		TESTHR(CreateRs(rs));
		TESTHR(CreateRs(rs1));

		// �Ȳ���Player GUID
		sprintf(sql, "SELECT * FROM baseproperty WHERE Name='%s'", playerName);
		TESTHR(OpenRs(sql, rs, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr()));

		char* szGUID = NULL;
		if(!rs->GetadoEOF())
		{
			szGUID = (_bstr_t)rs->GetCollect("guid");
		}
		ReleaseRs(rs);
		sprintf(sql, "SELECT * FROM CSL_DUPRGN_PROPERTY WHERE OwnerGUID='%s'", szGUID);
		TESTHR(OpenRs(sql, rs, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr()));

		while(!rs->GetadoEOF())
		{
			// GUID
			retPHGUID = CGUID((_bstr_t)rs->GetCollect("guid"));
		}
		ReleaseRs(rs1);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_QUESTRGNGUIDOK));
		return;
	}
	catch (_com_error &e)
	{
		ReleaseRs(rs);
		ReleaseRs(rs1);
		PrintErr(CStringReading::LoadString(IDS_DBS_RSDUPRGN,STR_DBS_DUPRGN_QUESTRGNGUIDFAIL), e);
		return;
	}

	return;
}