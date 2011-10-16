#include "StdAfx.h"
#include "DBPlayer.h"
#include "DBentityManager.h"
#include "DBGood.h"
#include "player.h"
#include "DBLoginPlayer.h"
#include "Linkman/LinkmanSystem.h"
#include "DBMail.h"
#include "DBMailGroup.h"
#include "DBLinkManGroup.h"
#include "DBGoodsGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBPlayer& CDBPlayer::operator=(CDBPlayer& right)
{
	CBaseDBEntity::operator=(right);
	if(this != &right)
	{
		m_GUID = right.m_GUID;
		(*p_GoodsGroup) = *(right.p_GoodsGroup);
		(*p_LinkmanGroup) = *(right.p_LinkmanGroup);
		(*p_MailGroup) = *(right.p_MailGroup);
	}
	return *this;
}

CDBPlayer::CDBPlayer()
: CBaseDBEntity()
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
	// ��ʼ���ֶ�����
	InitProperty();
	p_GoodsGroup = new CDBGoodsGroup(NULL_GUID);
	p_GoodsGroup->SetOwnerGUID(m_GUID);
	p_LinkmanGroup = new CDBLinkmanGroup(NULL_GUID);
	p_LinkmanGroup->SetOwnerGUID(m_GUID);
	p_MailGroup = new CDBMailGroup(NULL_GUID);
	p_MailGroup->SetOwnerGUID(m_GUID);
}

CDBPlayer::CDBPlayer(const CGUID& guid)
: CBaseDBEntity(guid)
{
	// ��ʼ���ֶ�����
	InitProperty();
	p_GoodsGroup = new CDBGoodsGroup(NULL_GUID);
	p_GoodsGroup->SetOwnerGUID(m_GUID);
	p_LinkmanGroup = new CDBLinkmanGroup(NULL_GUID);
	p_LinkmanGroup->SetOwnerGUID(m_GUID);
	p_MailGroup = new CDBMailGroup(NULL_GUID);
	p_MailGroup->SetOwnerGUID(m_GUID);
}
CDBPlayer::~CDBPlayer()
{
	SAFE_DELETE(p_GoodsGroup);
	SAFE_DELETE(p_LinkmanGroup);
	SAFE_DELETE(p_MailGroup);
}

// virtual interfaces
void CDBPlayer::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
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

void CDBPlayer::Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	m_mapDirtyAttrData.clear();

	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // �����������
	
	char varName[1024];
	for(int i=0; i<(int)attrNum; i++)
	{
		//--�¸�ʽ
		//�������ַ������ȣ�long��
		//�������ַ���
		//�������ͣ�long��
		//�������ݳ��ȣ�long��
		//��������
		long varNameLen = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ַ������ȣ�long��
		_GetStringFromByteArray(msgBuf, msgBufPos, varName, bufSize);//�������ַ���
		m_mapDirtyAttrData.insert(string(varName));
		long bufType = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ͣ�long��
		long varBufLen = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ݳ��ȣ�long��

		CEntityProperty* ep = NULL;
		CDataEntityManager::EntityPropertyMapItr itr = GetDataEntityManager().GetEntityPropertyMap().find(varName);
		if(itr != GetDataEntityManager().GetEntityPropertyMap().end())
			ep = itr->second;
		else
		{
			CDataEntityManager* pdeManager = NULL;
			std::map<string, CDataEntityManager*>::iterator itr = GetGame()->GetDBEntityManager()->GetObjAttrDef().find(string("player"));
			if(itr != GetGame()->GetDBEntityManager()->GetObjAttrDef().end())
				pdeManager = itr->second;

			if(pdeManager)
			{
				CEntityProperty* templateEP = pdeManager->GetEntityProperty(varName);
				if(templateEP)
					ep = GetDataEntityManager().CreateEntityProperty(varName, templateEP->GetDBTableName(), templateEP->GetDataType());
				else
					AddLogText("����������[%s]��������ָ��Ϊ��!", varName);
			}
		}

		assert(ep != NULL);

		switch((DATA_OBJECT_TYPE)bufType)
		{
		case DATA_OBJECT_TIME:
			{
				// 6��longֵ : �� �� �� ʱ �� ��
				BYTE timeValue[6*sizeof(DWORD)];
				memset(timeValue, 0, 6*sizeof(DWORD));
				_GetBufferFromByteArray(msgBuf, msgBufPos, timeValue, 6*sizeof(DWORD), bufSize);
				ep->SetBufAttr(0, timeValue, 6*sizeof(DWORD));
			}
			break;
		case DATA_OBJECT_STRING:
			{
				char varValue[1024];
				_GetStringFromByteArray(msgBuf, msgBufPos, varValue, bufSize);//��������
				ep->SetStringAttr(0, string((char*)varValue));
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
				long value = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//��������
				ep->SetLongAttr(0, (long)value);
			}
			break;
		case DATA_OBJECT_GUID:
			{
				CGUID tGUID;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize);//��������
				ep->SetGuidAttr(0, tGUID);
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				BYTE* buf = new BYTE[varBufLen];
				_GetBufferFromByteArray(msgBuf, msgBufPos, buf, varBufLen, bufSize);//��������
				ep->SetBufAttr(0, buf, varBufLen);
				SAFE_DELETE_ARRAY(buf);
			}
			break;
		}
	}
}

void CDBPlayer::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
{
	char szGuid[128];
	GetExID().tostring(szGuid);
	//AddLogText("DB2W[%d] : DB_OPER_SAVE_PLAYER, PlayerID[%s]...OK!", msgid, szGuid);
}
// ȫ��������װ��Ϣ
void CDBPlayer::AddAllEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, (WORD)GetDataEntityManager().GetEntityPropertyMap().size()); // �����������

	//--�¸�ʽ
	//�������ַ������ȣ�long��
	//�������ַ���
	//�������ͣ�long��
	//�������ݳ��ȣ�long��
	//��������
	CDataEntityManager::EntityPropertyMapItr itr = GetDataEntityManager().GetEntityPropertyMap().begin();
	for(; itr != GetDataEntityManager().GetEntityPropertyMap().end(); itr++)
	{
		_AddToByteArray(&pBA, (long)itr->first.size());//�������ַ������ȣ�long��
		_AddToByteArray(&pBA, itr->first.c_str());//�������ַ���
		_AddToByteArray(&pBA, (long)itr->second->GetDataType());//�������ͣ�long��
		_AddToByteArray(&pBA, (long)itr->second->GetBufSize(0));//�������ݳ��ȣ�long��
		switch(itr->second->GetDataType())
		{
		case DATA_OBJECT_TIME:
			{
				// 6��longֵ : �� �� �� ʱ �� ��
				BYTE timeValue[6*sizeof(DWORD)];
				memset(timeValue, 0, 6*sizeof(DWORD));
				itr->second->GetBufAttr(0, (void*)timeValue, 6*sizeof(DWORD));
				_AddToByteArray(&pBA, timeValue, 6*sizeof(DWORD));
			}
			break;
		case DATA_OBJECT_STRING:
			{
				_AddToByteArray(&pBA, itr->second->GetStringAttr(0));//��������
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
				_AddToByteArray(&pBA, (long)itr->second->GetLongAttr(0));//��������
			}
			break;
		case DATA_OBJECT_GUID:
			{
				CGUID tGUID;
				itr->second->GetGuidAttr(0, tGUID);
				_AddToByteArray(&pBA, tGUID);//��������
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				long bufSize = itr->second->GetBufSize(0);
				if(bufSize)
				{
					BYTE* buf = new BYTE[bufSize];
					itr->second->GetBufAttr(0, (void*)buf, bufSize);
					_AddToByteArray(&pBA, buf, bufSize);//��������
					SAFE_DELETE_ARRAY(buf);
				}
			}
			break;
		}

	}
}
void CDBPlayer::AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA)
{
	// ����������Ϣǰ���Ѿ�������
	// OPEntityID:    4byte, ����ʵ���IDֵ
	// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
	// MSG ID:        4byte, ����Ϣ��MSGID
	// Entity Num:    2byte, ��ʵ�����
	WORD attrNum = m_mapDirtyAttrData.size();
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, (WORD)attrNum); // �����������

	//--�¸�ʽ
	//�������ַ������ȣ�long��
	//�������ַ���
	//�������ͣ�long��
	//�������ݳ��ȣ�long��
	//��������
	set<string>::iterator itr = m_mapDirtyAttrData.begin();
	for(; itr != m_mapDirtyAttrData.end(); itr++)
	{
		CEntityProperty* ep = GetDataEntityManager().GetEntityProperty(*itr);
		if(ep)
		{
			_AddToByteArray(&pBA, (long)(*itr).size());//�������ַ������ȣ�long��
			_AddToByteArray(&pBA, (*itr).c_str());//�������ַ���
			_AddToByteArray(&pBA, (long)ep->GetDataType());//�������ͣ�long��
			_AddToByteArray(&pBA, (long)ep->GetBufSize(0));//�������ݳ��ȣ�long��
			switch(ep->GetDataType())
			{
			case DATA_OBJECT_TIME:
				{
					// 6��longֵ : �� �� �� ʱ �� ��
					BYTE timeValue[6*sizeof(DWORD)];
					memset(timeValue, 0, 6*sizeof(DWORD));
					ep->GetBufAttr(0, (void*)timeValue, 6*sizeof(DWORD));
					_AddToByteArray(&pBA, timeValue, 6*sizeof(DWORD));
				}
				break;
			case DATA_OBJECT_STRING:
				{
					_AddToByteArray(&pBA, ep->GetStringAttr(0));//��������
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
					_AddToByteArray(&pBA, (long)ep->GetLongAttr(0));//��������
				}
				break;
			case DATA_OBJECT_GUID:
				{
					CGUID tGUID;
					ep->GetGuidAttr(0, tGUID);
					_AddToByteArray(&pBA, tGUID);//��������
				}
				break;
			case DATA_OBJECT_BUFFER:
				{
					long bufSize = ep->GetBufSize(0);
					if(bufSize)
					{
						BYTE* buf = new BYTE[bufSize];
						ep->GetBufAttr(0, (void*)buf, bufSize);
						_AddToByteArray(&pBA, buf, bufSize);//��������
						SAFE_DELETE_ARRAY(buf);
					}
				}
				break;
			}
		}
	}
	m_mapDirtyAttrData.clear();
}

//// ���װ����Ʒmap
//void CDBPlayer::ClearContainerMap(PLAYER_EXTEND_ID containerID)
//{
//	map<long, set<CGUID>>::iterator placeItr = p_GoodsGroup->GetDirtyGoodsMap().find((long)containerID);
//	if(placeItr != p_GoodsGroup->GetDirtyGoodsMap().end())
//	{
//		set<CGUID>::iterator goodsItr = placeItr->second.begin();
//		while( goodsItr != placeItr->second.end() )
//		{
//			if(!p_GoodsGroup->DelGoods(*goodsItr, (long)containerID))
//			{
//				char szPlayerID[128];
//				char szGoodsID[128];
//				GetExID().tostring(szPlayerID);
//				(*goodsItr).tostring(szGoodsID);
//				AddLogText("CDBPlayer::ClearEquipmentMap del goods Err!Player[%s], goods[%s], place[%d]!", szPlayerID, szGoodsID, placeItr->first);
//			}
//			goodsItr++;
//		}
//		p_GoodsGroup->GetDirtyGoodsMap().erase(placeItr);
//	}
//}

// ����ӱ�������
//void CDBPlayer::ClearSubpackObjMap(long subPos)
//{
	//map<long, set<CGUID>>::iterator placeItr = p_GoodsGroup->GetDirtyGoodsMap().find((long)PEI_PACK);
	//if(placeItr != p_GoodsGroup->GetDirtyGoodsMap().end())
	//{
	//	set<CGUID>::iterator goodsItr = placeItr->second.begin();
	//	while( goodsItr != placeItr->second.end() )
	//	{
	//		CDBGoods* packGoods = p_GoodsGroup->FindGoods(*goodsItr);
	//		if(packGoods)
	//		{
	//			long pos = packGoods->GetLongAttr(string("position"), 0);
	//			if(pos == PEI_PACK1+subPos)
	//			{
	//				if(!p_GoodsGroup->DelGoods(*goodsItr, (long)PEI_PACK))
	//				{
	//					char szPlayerID[128];
	//					char szGoodsID[128];
	//					GetExID().tostring(szPlayerID);
	//					(*goodsItr).tostring(szGoodsID);
	//					AddLogText("CDBPlayer::ClearSubpackObjMap del goods Err!Player[%s], goods[%s], pos[%d]!", szPlayerID, szGoodsID, pos);
	//				}
	//				placeItr->second.erase((*goodsItr));
	//				break;
	//			}
	//		}
	//		goodsItr++;
	//	}
	//	if(placeItr->second.size() == 0) // �����
	//	{
	//		p_GoodsGroup->GetDirtyGoodsMap().erase(placeItr);
	//	}
	//}
//}
// ��ʼ����������
void CDBPlayer::InitProperty()
{
	CDataEntityManager* pdeManager = NULL;
	std::map<string, CDataEntityManager*>::iterator itr = GetGame()->GetDBEntityManager()->GetObjAttrDef().find(string("player"));
	if(itr != GetGame()->GetDBEntityManager()->GetObjAttrDef().end())
		pdeManager = itr->second;

	if(pdeManager)
	{
		CDataEntityManager::EntityPropertyMapItr attrItr = pdeManager->GetEntityPropertyMap().begin();

		for(; attrItr != pdeManager->GetEntityPropertyMap().end(); attrItr++)
		{
			CEntityProperty* pe = m_DataEntityManager.CreateEntityProperty(attrItr->first, attrItr->second->GetDBTableName(), attrItr->second->GetDataType());
			if(pe)
			{
				if(pe->GetDataType() == DATA_OBJECT_TIME)
				{
					long timeBuf[6] = {0};
					timeBuf[0] = 1990;
					timeBuf[1] = 1;
					timeBuf[2] = 1;
					pe->SetBufAttr(0, (void*)&timeBuf[0], sizeof(DWORD)*6);
				}
			}
		}
	}
	else
	{
		AddLogText("CDBPlayer::InitProperty() Err,δ�ҵ����������Ϣ!");
	}
}

CDBGoodsGroup* CDBPlayer::GetGoodsGroupPtr(void)
{
	return p_GoodsGroup;
}
CDBLinkmanGroup* CDBPlayer::GetLinkmanGroupPtr(void)
{
	return p_LinkmanGroup;
}
CDBMailGroup* CDBPlayer::GetMailGroupPtr(void)
{
	return p_MailGroup;
}
//DBMail////////////////////////////////////////////////////////////////////////
void CDBPlayer::AddOneDBMail(CDBMail* mail)
{
	if(mail)
	{
		p_MailGroup->GetMailGroupSet().insert(mail);
	}
}
// ɾ��һ��DBDupRgn
void CDBPlayer::DelOneDBMail(CDBMail* mail)
{
	if(mail)
	{
		set<CDBMail*>::iterator itr = p_MailGroup->GetMailGroupSet().find(mail);
		if(itr != p_MailGroup->GetMailGroupSet().end())
		{
			delete *itr;
			p_MailGroup->GetMailGroupSet().erase(mail);
		}
	}
}

// �������
void CDBPlayer::ClearDBMailMap(void)
{
	set<CDBMail*>::iterator itr = p_MailGroup->GetMailGroupSet().begin();
	for(; itr != p_MailGroup->GetMailGroupSet().end(); itr++)
	{
		SAFE_DELETE((*itr));
	}
	p_MailGroup->GetMailGroupSet().clear();
}