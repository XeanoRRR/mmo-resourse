
#include "StdAfx.h"
#include "PlayerDepot.h"
#include "Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PlayerDepot::PlayerDepot(void)
:
m_pPlayer(NULL),
m_bIsFree(false),
m_lInputPwdErrCount(0),
m_lFirstErrTime(0),
m_lThawDate(0),
m_uSessionRadius(0),
m_lSessionPosX(0),
m_lSessionPosY(0),
m_GoldDepot(GoodsSetup::GetGoldCoinIndex()),
m_GemDepot(GoodsSetup::GetSilverIndex())
{
	m_Primary.SetContainerVolume(PRIMARY_SIZE);
	//m_Primary2.SetContainerVolume(PRIMARY2_SIZE);
	memset(m_szPassword, 0, DEPOT_PASSWORD_ARRAY_LEN + 1);
	memset(m_arrHlodSubpack, 0, DEPOT_SUBPACK_NUM * sizeof(bool));
}

PlayerDepot::~PlayerDepot(void)
{
	Release();
}




///////////////////////////////////////////////////////////////////////////////////////////////
//! ����
void PlayerDepot::AddToByteArray_ForServer(vector<BYTE> *pByteArray)
{
	//! ����
	_AddToByteArray(pByteArray, m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			lFlag |= (1 << i);
	}
	_AddToByteArray(pByteArray, lFlag);
	//! ����ʱ��
	_AddToByteArray(pByteArray, m_lThawDate);

	//! ��Ʒ����
	m_Primary.Serialize( pByteArray, true);
	//m_Primary2.Serialize( pByteArray, true);
	m_Secondary.Serialize( pByteArray, true);
	m_GoldDepot.Serialize(pByteArray,true);
	m_GemDepot.Serialize(pByteArray,true);
}

//! ����
void PlayerDepot::GetFormByteArray_FromServer(BYTE *pByteArray, long &lPos)
{
	//! ����
	_GetBufferFromByteArray(pByteArray, lPos, m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = _GetLongFromByteArray(pByteArray, lPos);
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = (0 != (lFlag & (1 << i)));
	}
	//! ����ʱ��
	m_lThawDate = _GetLongFromByteArray(pByteArray, lPos);

#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7831);
#endif
	//! ��Ʒ����
	Clear();
	m_Primary.Unserialize(pByteArray, lPos);
//#ifdef __MEMORY_LEAK_CHECK__
//		CBaseObject::SetConstructFlag(7832);
//#endif
//	m_Primary2.Unserialize(pByteArray, lPos);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7833);
#endif
	m_Secondary.Unserialize(pByteArray, lPos);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7834);
#endif
	m_GoldDepot.Unserialize(pByteArray, lPos);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7835);
#endif
	m_GemDepot.Unserialize(pByteArray, lPos);
}

//! �������ݽ���
void PlayerDepot::GetSetupFormByteArray(BYTE *pByteArray, long &lPos)
{
	//! ����
	_GetBufferFromByteArray(pByteArray, lPos, m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = _GetLongFromByteArray(pByteArray, lPos);
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = (0 != (lFlag & (1 << i)));
	}
	//! ����ʱ��
	m_lThawDate = _GetLongFromByteArray(pByteArray, lPos);
}

//!		����
void PlayerDepot::CodeToDataBlock_ForServer(DBWriteSet& setWriteDB)
{
	//! ����
	setWriteDB.AddToByteArray(m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if(m_arrHlodSubpack[i])
			lFlag |= (1 << i);
	}
	setWriteDB.AddToByteArray(lFlag);
	//! ����ʱ��
	setWriteDB.AddToByteArray(m_lThawDate);

	//! ��Ʒ����
	m_Primary.Serialize( setWriteDB, true);
	m_Secondary.Serialize( setWriteDB, true);
	m_GoldDepot.Serialize(setWriteDB,true);
	m_GemDepot.Serialize(setWriteDB,true);
}

//!		����
void PlayerDepot::DecodeFormDataBlock_FromServer(DBReadSet& setReadDB)
{
	//! ����
	setReadDB.GetBufferFromByteArray( m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag = setReadDB.GetLongFromByteArray();
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = (0 != (lFlag & (1 << i)));
	}
	//! ����ʱ��
	m_lThawDate = setReadDB.GetLongFromByteArray( );

#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7831);
#endif
	//! ��Ʒ����
	Clear();
	m_Primary.Unserialize(setReadDB);
//#ifdef __MEMORY_LEAK_CHECK__
//		CBaseObject::SetConstructFlag(7832);
//#endif
//	m_Primary2.Unserialize(pByteArray, lPos);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7833);
#endif
	m_Secondary.Unserialize(setReadDB);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7834);
#endif
	m_GoldDepot.Unserialize(setReadDB);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(7835);
#endif
	m_GemDepot.Unserialize(setReadDB);
}
//!		�������ݽ���
void PlayerDepot::DecodeSetupFromDataBlock(DBReadSet& setReadDB)
{
	//! ����
	setReadDB.GetBufferFromByteArray(m_szPassword, DEPOT_PASSWORD_ARRAY_LEN);
	//! ��չ��λ��ʶ
	long lFlag =setReadDB.GetLongFromByteArray();
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = (0 != (lFlag & (1 << i)));
	}
	//! ����ʱ��
	m_lThawDate = setReadDB.GetLongFromByteArray( );
}

//! ���ö���
void PlayerDepot::Clear(void)
{
	m_Primary.Release();
	m_Primary.SetContainerVolume(PRIMARY_SIZE);
	//m_Primary2.Release();
	m_Secondary.ReleaseAllGoods();
	m_GoldDepot.Release();
	m_GemDepot.Release();
}

//! �ͷŶ���
void PlayerDepot::Release(void)
{
	m_pPlayer = NULL;
	Init();

	m_Primary.Release();
	//m_Primary2.Release();
	m_Secondary.Release();
	m_GoldDepot.Release();
	m_GemDepot.Release();
}

//! ��ʼ������
void PlayerDepot::Init(void)
{
	m_pPlayer			= NULL;
	m_bIsFree			= false;
	m_lInputPwdErrCount = 0;
	m_lFirstErrTime		= 0;
	m_lThawDate			= 0;
	m_uSessionRadius	= 0;
	m_lSessionPosX		= 0;
	m_lSessionPosY		= 0;

	m_Primary.SetContainerVolume(PRIMARY_SIZE);
	//m_Primary2.SetContainerVolume(PRIMARY2_SIZE);
	memset(m_szPassword, 0, DEPOT_PASSWORD_ARRAY_LEN + 1);
	memset(m_arrHlodSubpack, 0, DEPOT_SUBPACK_NUM * sizeof(bool));
}
//! �󶨽�ɫ
void PlayerDepot::BindPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
	if (NULL != pPlayer)
	{
	}
}

//! �õ������б�
void PlayerDepot::GetGoodsContainerList(GoodsContainerList &ContainerList)
{
	ContainerList.clear();
	pair<CGoodsContainer*, long> tmpNode;

	tmpNode.first = &m_Primary;
	tmpNode.second = eDCT_Primary;
	ContainerList.push_back(tmpNode);

	//tmpNode.first = &m_Primary2;
	//tmpNode.second = eDCT_Primary2;
	//ContainerList.push_back(tmpNode);

	tmpNode.first = FindContainer(eDCT_Secondary1);
	if (NULL != tmpNode.first)
	{
		tmpNode.second = eDCT_Secondary1;
		ContainerList.push_back(tmpNode);
	}

	tmpNode.first = FindContainer(eDCT_Secondary2);
	if (NULL != tmpNode.first)
	{
		tmpNode.second = eDCT_Secondary2;
		ContainerList.push_back(tmpNode);
	}

	tmpNode.first = FindContainer(eDCT_Secondary3);
	if (NULL != tmpNode.first)
	{
		tmpNode.second = eDCT_Secondary3;
		ContainerList.push_back(tmpNode);
	}

	tmpNode.first = FindContainer(eDCT_Secondary4);
	if (NULL != tmpNode.first)
	{
		tmpNode.second = eDCT_Secondary4;
		ContainerList.push_back(tmpNode);
	}

	tmpNode.first = &m_Secondary;
	tmpNode.second = eDCT_Secondary;
	ContainerList.push_back(tmpNode);

	tmpNode.first = &m_GoldDepot;
	tmpNode.second = eDCT_Gold;
	ContainerList.push_back(tmpNode);

	tmpNode.first = &m_GemDepot;
	tmpNode.second = eDCT_Silver;
	ContainerList.push_back(tmpNode);
}


//! �õ�����
const char* PlayerDepot::GetPassword(void)
{
	return m_szPassword;
}

//! ��������
void PlayerDepot::SetPassword(const char* pPassword)
{
	if(NULL == pPassword) return;
	long lPwsSize = (long)strlen(pPassword);
	lPwsSize = (lPwsSize < DEPOT_PASSWORD_ARRAY_LEN) ? lPwsSize : DEPOT_PASSWORD_ARRAY_LEN;
	memset(m_szPassword, 0, DEPOT_PASSWORD_ARRAY_LEN);
	memmove(m_szPassword, pPassword, lPwsSize);
}

//! �õ��ӱ��������ʶ
long PlayerDepot::GetSubpackFlag(void)
{
	long lFlag = 0;
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		if (m_arrHlodSubpack[i])
		{
			lFlag |= (1 << i);
		}
	}
	return lFlag;
}

//! �����ӱ����������
void PlayerDepot::SetSubpackFlag(long lFlag)
{
	for (long i = 0; i < DEPOT_SUBPACK_NUM; ++i)
	{
		m_arrHlodSubpack[i] = ((lFlag & (1 << i)) != 0);
	}
}

//! �õ��ⶳʱ��
long PlayerDepot::GetThawdate(void)
{
	return m_lThawDate;
}
//! �õ��ⶳʱ��
void PlayerDepot::SetThawdate(long lThawdate)
{
	m_lThawDate = lThawdate;
}














////////////////////////////////////////

tagDepotSetup PlayerDepot::c_DepotSetup;

bool PlayerDepot::LoadSetup(string filename)
{
	//! ----------------------------���ڵ� �������ڵĴ���Ӧ����1~10----------------------------
	TiXmlNode *pNode = NULL; 		

	CRFile *prfile = rfOpen(filename.c_str());
	if(NULL == prfile)
	{
		//���������Ϣ
		return false;
	}

	TiXmlDocument m_Tdoc(filename.c_str());
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);

	pNode=m_Tdoc.FirstChild("PlayerDepot");
	if (pNode==NULL)
	{
		return false;
	}

	TiXmlElement* pDepot = pNode->ToElement();//��ȡnode ��ָ��
	if (pDepot==NULL) 
	{
		return false;
	}
	//! ----------------------------��ȡ����λ�۸�----------------------------
	bool bRe = true;
	TiXmlElement* pSubDepotPrice = pDepot->FirstChildElement("SubDepotPrice");
	if (NULL == pSubDepotPrice)
	{
		return false;
	}

	const char *pTmpString = NULL;

	pTmpString = pSubDepotPrice->Attribute("Pos1_Price");
	if (NULL != pTmpString)
	{
		c_DepotSetup.arr_lSubpackPrice[0] = atoi(pTmpString);
	}
	else
	{
		bRe = false;
	}

	pTmpString = pSubDepotPrice->Attribute("Pos2_Price");
	if (NULL != pTmpString)
	{
		c_DepotSetup.arr_lSubpackPrice[1] = atoi(pTmpString);
	}
	else
	{
		bRe = false;
	}

	pTmpString = pSubDepotPrice->Attribute("Pos3_Price");
	if (NULL != pTmpString)
	{
		c_DepotSetup.arr_lSubpackPrice[2] = atoi(pTmpString);
	}
	else
	{
		bRe = false;
	}

	pTmpString = pSubDepotPrice->Attribute("Pos4_Price");
	if (NULL != pTmpString)
	{
		c_DepotSetup.arr_lSubpackPrice[3] = atoi(pTmpString);
	}
	else
	{
		bRe = false;
	}

	//! ----------------------------��������----------------------------
	TiXmlElement* pFrostSetup = pDepot->FirstChildElement("FrostSetup");
	if (NULL == pFrostSetup)
	{
		return false;
	}

	pTmpString = pFrostSetup->Attribute("MaxErrCount");
	if (NULL != pTmpString)
	{
		c_DepotSetup.lMaxPwdErrCount = atoi(pTmpString);
	}
	else
	{
		bRe = false;
	}

	pTmpString = pFrostSetup->Attribute("FrostTime");
	if (NULL != pTmpString)
	{
		c_DepotSetup.lFrostTime = atoi(pTmpString) * 60;
	}
	else
	{
		bRe = false;
	}

	//! ----------------------------�������----------------------------
	return true;
}

const	tagDepotSetup* PlayerDepot::GetDepotSetup(void)
{
	return &c_DepotSetup;
}











////////////////////////////////////////
//! �ҵ���������
CGoodsContainer* PlayerDepot::FindContainer(long eContainerId)
{
	switch(eContainerId)
	{
	case eDCT_Primary:
		return &m_Primary;
	//case eDCT_Primary2:
	//	return &m_Primary2;
	case eDCT_Secondary:
		return &m_Secondary;
	case eDCT_Secondary1:
		return m_Secondary.GetSubpack(0)->pSubpackContainer;
		break;
	case eDCT_Secondary2:
		return m_Secondary.GetSubpack(1)->pSubpackContainer;
		break;
	case eDCT_Secondary3:
		return m_Secondary.GetSubpack(2)->pSubpackContainer;
		break;
	case eDCT_Secondary4:
		return m_Secondary.GetSubpack(3)->pSubpackContainer;
		break;
	case eDCT_Gold:
		return &m_GoldDepot;
	case eDCT_Silver:
		return &m_GemDepot;
	}

	return NULL;
}