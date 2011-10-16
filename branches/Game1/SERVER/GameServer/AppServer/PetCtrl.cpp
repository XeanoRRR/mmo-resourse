#include "StdAfx.h"
#include "PetCtrl.h"
#include "Pet.h"
#include "Player.h"
#include "Monster.h"
#include "ServerRegion.h"
#include "../../Public/ConfigParse.h"
#include "../../GameServer/GameServer/GameManager.h"
#include "../../NETS/NetServer/Message.h"


// ע����Ϣ��Ӧ����
BEGIN_MSG_MAP(CPetCtrl)
	//ON_REGISTER_MEM_FUNC(GMSG_C2G_CONNECTED, &CPetCtrl::CheckLogic)
	//ON_REGISTER_MEM_FUNC(GMSG_C2C_TRANSMIT, &CPetCtrl::TransmitMsg)
END_MSG_MAP()

CPetCtrl::CPetCtrl(void)
{
}

CPetCtrl::~CPetCtrl(void)
{
}

// ���ý���
bool CPetCtrl::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	setReadDB.GetBufferFromByteArray(&m_ConfPet, sizeof(m_ConfPet));
	long lNum = setReadDB.GetLongFromByteArray();
	tagPetBase tPetConf;
	for( long l=0; l<lNum; ++l )
	{
		setReadDB.GetBufferFromByteArray(&tPetConf, sizeof(tPetConf));
		m_ConfPets[tPetConf.lIndex] = tPetConf;
	}

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

	char szLog[512];
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
			_snprintf(szLog, 512, "�쳣: ���%sû��GUIDΪ%s�ĳ���.",
				pPlayer->GetName(),
				szGUID);
			PutoutLog("Pet", LT_ERROR, szLog);
			continue;
		}
		setWriteDB.AddToByteArray(char(1));
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
	CGUID guID;
	char cFlag = 0;
	CPet* pPet = NULL;
	for( long i=0; i<lNum; ++i )
	{
		cFlag = setReadDB.GetCharFromByteArray();
		if( 0 == cFlag )
			continue;

		pPet = pPlayer->CreatePet(NULL_GUID);

		// ����������ɹ�, ��Ϊ�����ӳ���.
		// ����, ֱ��ɾ���������.
		if( pPet->DecodeFromDataBlock(setReadDB) )
		{
			pPlayer->AddPet(pPet);
		}
		else // ����ʧ��(����������ֹ)
		{
			MP_DELETE(pPet);
		}
	}

	return true;
}

// �������
void CPetCtrl::ActivatePet(CPet* pPet)
{
	CPlayer* pPlayer = pPet->m_pHost;
	if( NULL == pPlayer )
	{
		PutoutLog("Pet", LT_ERROR, "����ʧ��, ������.");
		return;
	}

	m_Pets[pPet->GetExID()] = pPet;
	pPet->InitialByHost(IT_LOGIN_GS);
	InformClient(pPlayer, pPet, true);
	pPet->OnActive();
}

// ����ɾ������
void CPetCtrl::OnDeletePet(void)
{
	if( m_DetectPets.empty() ) return;

	char szLog[512];
	char szGUID[64];
	static long lCount = 0;
	vector<CPet*>::iterator itr = m_DetectPets.begin();
	CPet* pPet = NULL;
	while( !m_DetectPets.empty() )
	{
		PutoutLog("PetCD", LT_DEFAULT, "Del pet.");
		if( (pPet = *itr) != NULL )
		{
			pPet->GetExID().tostring(szGUID);
			_snprintf(szLog, 512, "Pet(%s: %s) will be delete.",
				pPet->GetName(),
				szGUID);
			PutoutLog("PetCD", LT_DEFAULT, szLog);
			DELETE_SAFE(pPet);
		}
		m_DetectPets.erase(itr++);
	}
}

CPet* CPetCtrl::GetPet(const CGUID& rID)
{
	CPet* pPet = NULL;
	C_ITR_PET itr = m_Pets.find(rID);
	if( itr != m_Pets.end() )
	{
		pPet = itr->second;
	}
	return pPet;
}

// ɾ��(δ)�������
void CPetCtrl::DelPet(CPet* pPet, long lOpType)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pPet->GetHost());
	assert(pPlayer != NULL);

	if( pPet->IsActive() )
	{
		pPet->OnHouse(lOpType);
		if(lOpType != PET_OT_BEEN_KILLED)
		{
			InformClient(pPlayer, pPet, false);
		}
	}
	else
	{
		pPet->DelFromRegion(lOpType);
		InformClient(pPlayer, pPet, false);
	}

	m_Pets.erase(pPet->GetExID());

	if( lOpType & PET_OT_DELETE )
	{
		pPlayer->DelPet(pPet->GetExID());
		m_DetectPets.push_back(pPet);
		pPet->SetHost(NULL);

		if( pPet->NeedBeSaved() )
		{
			InformWS(pPlayer);
		}
	}
}

// �������
bool CPetCtrl::RelivePet(CPet* pPet, long lInitType)
{
	// ����������
	CPlayer* pPlayer = (CPlayer*)(pPet->GetHost());
	long lIndexID = pPet->GetIndexID();
	C_ITR_PET_LIST itrPet = m_ConfPets.find(lIndexID);
	if( m_ConfPets.end() == itrPet ||
		NULL == pPlayer )
	{
		PutoutLog("PET", LT_ERROR,
			"����ΪNULL || ���ʧ��, δ�ҵ���Ӧ������Ϣ, �����������: %ld.",
			lIndexID);
		return false;
	}

	return pPet->InitialDiffAttrs(lInitType);
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

	if( bAdd )
	{
		CMessage MsgHost(MSG_S2C_RGN_ADDSHAPE);
		MsgHost.Add(pPet->GetType());
		MsgHost.Add(pPet->GetExID());
		MsgHost.Add(pPet->m_BaseAttrs.lPetType);
		MsgHost.Add(char(1));		// ���˱�־λ
		DBWriteSet setWriteDBHost;
		MsgHost.GetDBWriteSet(setWriteDBHost);
		pPet->CodeToDataBlock_ForClient(setWriteDBHost);
		MsgHost.Add(char(0));		// �Ƿ������Ч
		MsgHost.SendToPlayer(pPlayer->GetExID());
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
CPet* CPetCtrl::CreatePet(CPlayer* pPlayer, long lType, long lIndexID)
{
	if( NULL == pPlayer )
	{
		PutoutLog("Pet", LT_ERROR, "Player == NULL.");
		return NULL;
	}

	// �������ͬ���ͼ������
	CPet* pOldPet = pPlayer->GetActivePet(lType);
	if( pOldPet != NULL && pOldPet->IsActive() )
	{
		char szGUID[64];
		pOldPet->GetExID().tostring(szGUID);
		PutoutLog("Pet", LT_ERROR,
			"լ�����%s�Ѿ�����ӵ�еĳ���<%ld:%ld:%s>.",
			pPlayer->GetName(),
			lType,
			lIndexID,
			szGUID);
		DelPet(pOldPet, PET_OT_HOUSE);
	}

	// ��������
	// 1) ��������
	// 2) ���������ͳ��ﲻ���б���
	long lInitType = IT_RELIVE;
	CPet* pPet = pPlayer->GetPet(lIndexID);
	if( pPet == NULL )
	{
		CGUID guID;
		CGUID::CreateGUID(guID);
		pPet = pPlayer->CreatePet(guID);
		pPet->SetIndexID(lIndexID);
		pPet->SetPetType(lType);
		lInitType = IT_CREATE;
	}

	if( NULL == pPet )
	{
		PutoutLog("Pet", LT_ERROR,
			"���%s��������<%ld>ʧ��.",
			pPlayer->GetName(),
			lIndexID);
		return NULL;
	}
	else
	{
		pPet->m_DiffAttrs.lActive = 1;
	}

	// ���ó���
	if( RelivePet(pPet, lInitType) )
	{
		// �������
		ActivatePet(pPet);
		if( pPet->NeedBeSaved() )
		{
			InformWS(pPlayer);
		}

		return pPet;
	}

	return NULL;
}

// ɾ������(ΪHOUSE)
bool CPetCtrl::DelPet(const CGUID& rID)
{
	// �������ͬ���ͼ������
	CPet* pPet = GetPet(rID);
	if( pPet != NULL )
	{
		DelPet(pPet, PET_OT_HOUSE);
	}

	return true;
}

// �������
bool CPetCtrl::RelivePet(CPlayer* pPlayer, long lType, long lIndexID)
{
	CPet* pPet = CreatePet(pPlayer, lType, lIndexID);
	if (pPet != NULL)
	{
		pPet->SetHP(pPet->GetMaxHP() >> 1);
		pPet->SetMP(pPet->GetMaxMP() >> 1);
		return true;
	}

	return false;
}

// ��ӳ��＼��
bool CPetCtrl::AddPetSkill(CPlayer* pPlayer,
						   long lType,
						   long lID,
						   long lLevel,
						   long lPos)
{
	if( NULL == pPlayer )
	{
		PutoutLog("Pet", LT_ERROR, "Player == NULL.");
		return false;
	}

	char szLog[512];
	// ���������Ƿ���ڱ��������
	CPet* pPet = pPlayer->GetActivePet(lType);
	if( NULL == pPet )
	{
		sprintf_s(szLog, sizeof(szLog),
			"���%s����������Ϊ%ld�ĳ���.",
			pPlayer->GetName(), lType);
		PutoutLog("PET", LT_ERROR, szLog);
		return false;
	}

	if( lType == PET_TYPE_ELF ||			// ����׷���츳�������Զ�׷��, �˴�ֻҪΪ����Ŀ��
		0 == lID ||							// Ĭ��Ϊ׷�����ü���
		0 == lLevel ||
		-1 == lPos )
	{
		pPet->AppendGeniusSkill();
		return true;
	}
	return pPet->ModifySkill(lID, lLevel, lPos);
}