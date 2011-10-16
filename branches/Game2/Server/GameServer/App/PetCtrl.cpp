#include "StdAfx.h"
#include "PetCtrl.h"
#include "Pet.h"
#include "Player.h"
#include "Monster.h"
#include "ServerRegion.h"
#include "../Server/GameManager.h"
#include "../Net/Message.h"

CPetCtrl::CPetCtrl(void)
{
}

CPetCtrl::~CPetCtrl(void)
{
}

void CPetCtrl::GetGrowCoefs(long lIndex, GROW_LIST& rList)
{
	ITR_GROW_CONF itr = m_GrowConfPets.find(lIndex);
	if( itr != m_GrowConfPets.end() )
	{
		rList = itr->second;
	}
}

// ���ý���
bool CPetCtrl::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	// ����ϵͳͳһ����
	setReadDB.GetBufferFromByteArray(&m_ConfPet, sizeof(m_ConfPet));

	// �����б�����
	long lNum = setReadDB.GetLongFromByteArray();
	tagPetBase tPetConf;
	for( long l=0; l<lNum; ++l )
	{
		setReadDB.GetBufferFromByteArray(&tPetConf, sizeof(tPetConf));
		m_ConfPets[tPetConf.lIndex] = tPetConf;
	}

	// ����ɳ�
	lNum = setReadDB.GetLongFromByteArray();
	long lIndex = 0;
	long lSecs = 0;
	tagPetOrig tPetOrig;
	GROW_LIST PetOrigs;
	for( long l=0; l<lNum; ++l )
	{
		lIndex = setReadDB.GetLongFromByteArray();
		PetOrigs.clear();
		lSecs = setReadDB.GetLongFromByteArray(); 
		for( long m=0; m<lSecs; ++m )
		{
			setReadDB.GetBufferFromByteArray(&tPetOrig, sizeof(tPetOrig));
			PetOrigs.push_back(tPetOrig);
		}
		m_GrowConfPets[lIndex] = PetOrigs;
	}

	// �̴�ǿ������
	GetInst(PetSetup).DecodeFromDataBlock(setReadDB);

	return true;
}

// GS==>Client����
bool CPetCtrl::CodeToDataBlockForClient(DBWriteSet& setWriteDB,
										CPlayer* pPlayer)
{
	C_PETS crPets = pPlayer->GetPets();
	long lNum = (long)crPets.size();
	setWriteDB.AddToByteArray(lNum);
	if( 0 == lNum ) return true;

	CPet* pPet = NULL;
	C_ITR_PET itrPet = crPets.begin();
	for( ; itrPet != crPets.end(); ++itrPet )
	{
		pPet = itrPet->second;
		if( pPet != NULL )
		{
			if( pPet->m_BaseAttrs.lPetType != PET_TYPE_NULL )
			{
				setWriteDB.AddToByteArray(pPet->m_BaseAttrs.lPetType);
				setWriteDB.AddToByteArray(pPet->GetExID());
				setWriteDB.AddToByteArray(pPet->m_BaseAttrs.lPicID);
				setWriteDB.AddToByteArray(pPet->GetName());

				// ������ϸ���Ա���
				pPet->CodeAttrToClient(setWriteDB);
			}
			else
			{
				setWriteDB.AddToByteArray(long(0));
			}
		}
		else
		{
			setWriteDB.AddToByteArray(long(0));
		}
	}

	return true;
}

// GS==>WS����
bool CPetCtrl::CodeToDataBlock(DBWriteSet& setWriteDB,
							   CPlayer* pPlayer)
{
	CONDITION_CHECK(m_ConfPet.nFuncSwitch);
	char szGUID[128];
	CPet* pPet = NULL;
	C_PETS rPets = pPlayer->GetPets();
	C_ITR_PET itrPet = rPets.begin();
	long lNum = (long)rPets.size();
	setWriteDB.AddToByteArray(lNum);
	for( ; itrPet != rPets.end(); ++itrPet )
	{
		if( NULL == (pPet = itrPet->second) )
		{
			setWriteDB.AddToByteArray(char(0));
			itrPet->first.tostring(szGUID);
            Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_4",pPlayer->GetName(),szGUID));
			continue;
		}
		setWriteDB.AddToByteArray(char(1));
		setWriteDB.AddToByteArray(itrPet->first);
		pPet->CodeToDataBlock(setWriteDB);
	}
	return true;
}

// WS==>GS����
bool CPetCtrl::DecodeFromDataBlock(DBReadSet& setReadDB,
								   CPlayer* pPlayer)
{
	CONDITION_CHECK(m_ConfPet.nFuncSwitch);

	pPlayer->ReleaseAllPets();	// ��ճ����б�
	long lNum = setReadDB.GetLongFromByteArray();
	if( 0 == lNum ) return true;
	char cFlag = 0;
	CPet* pPet = NULL;
	for( long i=0; i<lNum; ++i )
	{
		cFlag = setReadDB.GetCharFromByteArray();
		if( 0 == cFlag )
			continue;

		CGUID guid;
		setReadDB.GetBufferFromByteArray(guid);
		pPet = pPlayer->CreatePet(guid);

		// ����������ɹ�, ��Ϊ�����ӳ���.
		// ����, ֱ��ɾ���������.
		if( pPet->DecodeFromDataBlock(setReadDB) )
		{
			pPlayer->AddPet(pPet);
			switch(pPet->m_DiffAttrs.lStateType)
			{
			case PET_STATE_FIGHT:
				pPlayer->AddFightPet(guid);
				break;
			case PET_STATE_ACTIVE:
				pPlayer->SetActivePetGUID(guid);
				break;
			}
		}
		else // ����ʧ��(����������ֹ)
		{
			OBJ_RELEASE(CPet, pPet);
		}
	}

	return true;
}

// ��ӳ���(���ó�����ʱ����)
void CPetCtrl::OnCreatePet(CPet* pPet)
{
	CPlayer* pPlayer = pPet->m_pHost;
	if( NULL == pPlayer )
	{
        //Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PET_5"));
		return;
	}

	pPet->OnCreate();

	// ������ӳ�����Ϣ
	CMessage msg(MSG_S2C_PET_CREATE_PET);
	msg.Add(pPet->GetPetType());
	msg.Add(pPet->GetExID());
	msg.Add(pPet->GetGraphicsID());

	DBWriteSet setWriteDBHost;
	msg.GetDBWriteSet(setWriteDBHost);
	pPet->CodeAttrToClient(setWriteDBHost);
	msg.SendToPlayer(pPlayer->GetExID());
}

// ����GUID��ȡ����
CPet* CPetCtrl::GetPet(CPlayer* player, const CGUID& guid)
{
	return player->GetPet(guid);
}

// ���ó���
bool CPetCtrl::ReSetPet(CPet* pPet, long lInitType, bool IsWild)
{
	// ����������
	CPlayer* pPlayer = (CPlayer*)(pPet->GetHost());
	long lIndexID = pPet->GetIndexID();
	C_ITR_PET_LIST itrPet = m_ConfPets.find(lIndexID);
	if( m_ConfPets.end() == itrPet || NULL == pPlayer )
	{
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_3"), lIndexID);//�õ�һ��������Ϊ����[%d], ִ�з���!
		return false;
	}

	return pPet->InitialDiffAttrs(lInitType,IsWild);
}

// ������Ϣ��WS, �˴������־λ, ������̵㷽�������.
void CPetCtrl::InformWS(CPlayer* pPlayer)
{
	GetGame()->UpdateSavePlayer(pPlayer, SAVE_DETAIL_PETS);
}

// ������Ϣ������
void CPetCtrl::InformClient(CPlayer* pPlayer, CPet* pPet, bool bAdd)
{
	assert(pPlayer != NULL);
	assert(pPet != NULL);

	if(bAdd)
	{
		CMessage msg(MSG_S2C_RGN_ADDSHAPE);
		msg.Add(pPet->GetType());
		msg.Add(pPet->GetExID());
		msg.Add(pPet->m_BaseAttrs.lPetType);
		msg.Add(char(1));		// ���˱�־λ
		DBWriteSet setWriteDBHost;
		msg.GetDBWriteSet(setWriteDBHost);
		pPet->CodeToDataBlock_ForClient(setWriteDBHost);
		msg.Add(char(0));		// �Ƿ������Ч
		msg.SendToPlayer(pPlayer->GetExID());
	}
	else
	{
		CMessage MsgAround(MSG_S2C_RGN_DELSHAPE);
		MsgAround.Add(pPet->GetType());
		MsgAround.Add(pPet->GetExID());
		MsgAround.Add((char)0);		// �Ƿ������Ч
		MsgAround.SendToPlayer(pPlayer->GetExID());
	}
}

// ֪ͨ�ͻ���AI�¼�
void CPetCtrl::InformClientAI(CPet* pPet, long lOpType)
{
	CPlayer* pPlayer = (CPlayer*)pPet->GetHost();

	// ����������
	CMessage msg(MSG_S2C_PET_AI);
	msg.Add(pPet->GetExID());
	msg.Add(lOpType);
	switch( lOpType )
	{
	case PET_OT_AI_SET_TARGET:
		{
			const CMoveShape* pTarget = pPet->GetTarget();
			if( pTarget != NULL )
			{
				msg.Add(pTarget->GetType());
				msg.Add(pTarget->GetExID());
			}
			else
			{
				msg.Add((long)0);
				msg.Add(NULL_GUID);
			}
		}
		break;
	}

	msg.SendToPlayer(pPlayer->GetExID());
}

// ��������
bool CPetCtrl::CreatePet(CPlayer* pPlayer, long lType, long lIndexID, bool IsWild)
{
	if(NULL == pPlayer)
	{
		//PutoutLog("Pet", LT_ERROR, AppFrame::GetText("GS_PET_1"));
		return false;
	}

	CGUID guid;
	CGUID::CreateGUID(guid);
	CPet* pPet = pPlayer->CreatePet(guid);
	if (!pPet)
	{
		//PutoutLog("Pet", LT_ERROR, AppFrame::GetText("GS_PET_2"), pPlayer->GetName(), lIndexID);
		return false;
	}
	pPet->SetIndexID(lIndexID);
	pPet->SetPetType(lType);
	long lInitType = IT_CREATE;
	// �»�ȡ�ĳ����״̬����Ϊ����״̬
	pPet->SetStateType(PET_STATE_AWAIT);

	// ���ó���
	if(ReSetPet(pPet, lInitType, IsWild))
	{
		// ��ӳ���
		OnCreatePet(pPet);
		InformWS(pPlayer);
		return true;
	}
	return false;
}

// ɾ������
bool CPetCtrl::DelPet(CPlayer* player, const CGUID& guid)
{
	CPet* pet = player->GetPet(guid);
	if(pet != NULL)
	{
		player->DelPet(guid);
		return true;
	}

	return false;
}

// �������
//bool CPetCtrl::RelivePet(CPlayer* pPlayer, const CGUID& guid)
//{
//	CPet* pPet = pPlayer->GetPet(guid);
//	if (pPet != NULL)
//	{
//		long lInitType = IT_RELIVE;
//		if (!RelivePet(pPet, lInitType))
//			return false;
//		RelivePet(pPet, lInitType);
//		pPet->SetHP(pPet->GetMaxHP() >> 1);
//		pPet->SetMP(pPet->GetMaxMP() >> 1);
//		return true;
//	}
//
//	return false;
//}

/// ��ӳ��＼��(Ŀǰֻ�ṩ������Ƕ��ʽ��ӳ��＼��)
bool CPetCtrl::AddPetSkill(CPlayer* player,
						   const CGUID& guid,
						   long id,
						   long level,
						   long pos)
{
	if( NULL == player )
	{
		Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__, "Player == NULL.");
		return false;
	}
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		/*sprintf_s(szLog, sizeof(szLog), AppFrame::GetText("GS_PET_4"),
			player->GetName(), guid.tostring());
		PutoutLog("PET", LT_ERROR, szLog);*/
		return false;
	}
	
	// ����������Ͳ����̴ӣ����ܽ��м�����Ƕ
	if(!pet->IsServant())
	{
		/*sprintf_s(szLog, sizeof(szLog),AppFrame::GetText("GS_PET_6"),
			player->GetName(), pet->GetPetType());
		PutoutLog("PET", LT_ERROR, szLog);*/
		return false;
	}
	return pet->ModifySkill(id, level, pos);
}

/// ��������
bool CPetCtrl::ResetPetTalent(CPlayer* pPlayer, const CGUID& guid)
{
	if( NULL == pPlayer )
	{
		return false;
	}
	CPet* pet = pPlayer->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		return false;
	}
	return pet->OnReSetTalent();
}

/// ��������ǿ��
bool CPetCtrl::UpgradePetDAfterVal(CPlayer* pPlayer, const CGUID& guid, long val)
{
	if( NULL == pPlayer )
	{
		return false;
	}
	CPet* pet = pPlayer->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		return false;
	}
	return pet->OnDAfterValUpGrade(val);
}

/// ���ú�������ǿ������
bool CPetCtrl::ResetPetDAfterCnt(CPlayer* pPlayer, const CGUID& guid)
{
	if( NULL == pPlayer )
	{
		return false;
	}
	CPet* pet = pPlayer->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		return false;
	}
	return pet->OnResetDAfterCnt();
}

/// ���ó������Ե�
bool CPetCtrl::ResetPetSavvy(CPlayer* pPlayer, const CGUID& guid)
{
	if( NULL == pPlayer )
	{
		return false;
	}
	CPet* pet = pPlayer->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		return false;
	}
	return pet->OnResetSavvy();
}

/// ȡ���̴�Ʒ��
long CPetCtrl::GetPetGrade(CPlayer* pPlayer, const CGUID& guid)
{
	if( NULL == pPlayer )
	{
		return false;
	}
	CPet* pet = pPlayer->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		return false;
	}
	return pet->GetGrade();
}

/// ȡ���̴Ӻ�����ǿ������
long CPetCtrl::GetPetDAfterCnt(CPlayer* pPlayer, const CGUID& guid)
{
	if( NULL == pPlayer )
	{
		return false;
	}
	CPet* pet = pPlayer->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet )
	{
		return false;
	}
	return pet->GetDAfterCnt();
}

/// ȡ���������ӵ���̴���Ŀ
long CPetCtrl::GetMaxServantNum(void)
{
	return m_ConfPet.maxTakeNum;
}

void CPetCtrl::GetMaterialList(CPlayer* player, const CGUID& guid,
							   long type, vector<tagMaterial>& goods)
{
	if (NULL==player) return;
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet ) return;
	
	MATERIAL_LIST* pMaterialList = GetInst(PetSetup).GetMaterialList();
	ITR_MATERIAL_LIST iterM = pMaterialList->find(type);
	if (iterM!=pMaterialList->end())
	{
		tagMaterialList& materialList = iterM->second;
		vector<tagMaterialGoods>::iterator iterGoods 
			= materialList.goodsList.begin();
		for (; iterGoods != materialList.goodsList.end(); ++iterGoods)
		{
			long keyNum = iterGoods->keyNum;
			if (keyNum!=0)
			{
				// keyֵ������ѡ��Ʒ
				string key = iterGoods->key;
				long index = pet->GetAttr(key.c_str());
				goods.push_back(iterGoods->goodstable[index]);
			}
			else
			{
				goods.push_back(iterGoods->goodstable[0]);
			}
		}
	}
}

void CPetCtrl::IncPetExp(CPlayer* player, const CGUID& guid, long val)
{
	if (NULL==player) return;
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet ) return;

	pet->IncExp(val);
}


void CPetCtrl::IncPetDExp(CPlayer* player, const CGUID& guid, long val)
{
	if (NULL==player) return;
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet ) return;

	pet->IncDExp(val);
}

bool CPetCtrl::PetLevUp(CPlayer* player, const CGUID& guid, long newLev)
{
	if (NULL==player) return false;
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet ) return false;

	if (pet->GetLevel()==newLev)
		return false;

	pet->SetLevel(newLev);
	return true;
}

bool CPetCtrl::SetPetAttr(CPlayer* player, const char *attrName, double attrVal, const CGUID& guid)
{
	if (NULL==player) return false;
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet ) return false;

	if(pet->GetAttr(attrName)==attrVal)
		return false;

	pet->SetAttr(attrName, attrVal);
	pet->UpdateAttribute();
	return true;
}

long CPetCtrl::GetPetAttr(CPlayer* player, const char* attrName, const CGUID& guid)
{
	if (NULL==player) return false;
	CPet* pet = player->GetPet(guid);
	// ��Ҳ����ڸó���
	if( NULL == pet ) return false;

	pet->GetAttr(attrName);
	return true;
}

bool CPetCtrl::ActivePet(CPlayer* player, const CGUID& guid)
{
	if (NULL==player) return false;
	CPet* pet = player->GetPet(guid);
	if( NULL == pet ) return false;

	CGUID activeGuid = player->GetActivePetGUID();
	if (player->GetActivePetGUID()!=NULL_GUID)
	{
		/*sprintf_s(szLog, sizeof(szLog),AppFrame::GetText("GS_PET_7"),
			player->GetName(), pet->GetPetName());
		PutoutLog("PET", LT_ERROR, szLog);*/
		return false;
	}
	if (!player->GetFightPet(guid))
	{
		/*sprintf_s(szLog, sizeof(szLog),AppFrame::GetText("GS_PET_8"),
			player->GetName(), pet->GetPetName());
		PutoutLog("PET", LT_ERROR, szLog);*/
		return false;
	}
	player->SetActivePetGUID(guid);
	ActivePet(pet);
	return true;
}

bool CPetCtrl::UnActivePet(CPlayer* player, const CGUID& guid)
{
	if (NULL==player) return false;
	CPet* pet = player->GetPet(guid);
	if( NULL == pet ) return false;

	pet->OnHouse(PET_OT_HOUSE);
	// ֪ͨ�ͻ����ڳ�����ɾ������
	//InformClient(player,pet,false);
	return true;
}

// �������
void CPetCtrl::ActivePet(CPet* pet)
{
	pet->OnActive();
	InformClient((CPlayer*)pet->GetHost(),pet,true);
}