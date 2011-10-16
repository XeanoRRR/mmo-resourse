#include "stdafx.h"
#include "Goods.h"
#include "../../GameClient/Game.h"
#include "Effect.h"
#include "../../EngineExtend/DisplayModel.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "Player.h"
#include "../../Public/Common/Public.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGoods::CGoods(void)
{
	SetType(TYPE_GOODS);
	m_dwIndex=0;
	m_lNum=0;
	//m_bInTradePage=false;
	//m_bInPlayerShop = false;
	m_bHaveShadow = false;
	m_dwPurchaseLimit = 0;

	m_pFlashEffect = new CEffect;
	m_pFlashEffect->SetGraphicsID(10);
	m_pFlashEffect->SetDisplayModel();
	m_pFlashEffect->SetDisplayModelGroup();
	
	m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('g','r','n','d');
	m_pAnimInfo->GetActionInfo()->bCurActionLooped = true;

	//m_dwNameColor=0;

	m_bDropEffect = false;
	m_fDropHeight = 0.2f;

	m_wHoleNum = 0;

	// ��ʼ ��������Ϊ 0
	m_lLockCounter = 0;

	m_strManufacturerName = "";

	//m_strActiveInfor = "";
}

CGoods::CGoods(ulong dwIndex)
{
	CGoodsList::tagGoods2* pTagGoods = CGoodsList::GetProperty(dwIndex);
	if (pTagGoods!=NULL)
	{
		GetEffect()->clear();
		m_wHoleNum = 0;
		m_mapHoleState.clear();
		m_InlayHole.clear();

		SetType(TYPE_GOODS);
		SetExID(CGUID::GUID_INVALID);
		m_dwIndex = dwIndex;
		m_lNum = 0;
		//m_bInTradePage=false;
		//m_bInPlayerShop = false;
		m_bHaveShadow = false;
		m_dwPurchaseLimit = 0;
		m_wHoleNum = 0;
		m_lLockCounter = 0;

		m_pFlashEffect = new CEffect;
		m_pFlashEffect->SetGraphicsID(10);
		m_pFlashEffect->SetDisplayModel();
		m_pFlashEffect->SetDisplayModelGroup();

		m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('g','r','n','d');
		m_pAnimInfo->GetActionInfo()->bCurActionLooped = true;

		m_bDropEffect = false;
		m_fDropHeight = 0.2f;
		m_stBaseProperty.strName = pTagGoods->BaseProperty.strName;
		SetName(m_stBaseProperty.strName.c_str());
		m_stBaseProperty.dwValue = pTagGoods->BaseProperty.dwPrice;
		m_stBaseProperty.dwSilverPrice = pTagGoods->BaseProperty.dwPrice;
		m_stBaseProperty.dwType = pTagGoods->BaseProperty.dwType;
		m_GoodsBaseType = static_cast<eGoodsBaseType>(m_stBaseProperty.dwType);

		vector<CGoodsList::tagEffect>::const_iterator EffectIter = pTagGoods->vectorEffect.begin();
		for(;EffectIter!= pTagGoods->vectorEffect.end();EffectIter++)
		{
			m_vectorEffect.push_back(*EffectIter);
		}
		SetGraphicsID(pTagGoods->BaseProperty.dwGroundId);

		m_strManufacturerName = "";

		//m_strActiveInfor = "";
	}
}

CGoods::CGoods(const CGoods &srcGoods)
{
	GetEffect()->clear();
	m_wHoleNum = 0;
	m_mapHoleState.clear();
	m_InlayHole.clear();

	SetType(TYPE_GOODS);
	SetExID(srcGoods.m_guExID);
	m_dwIndex = srcGoods.m_dwIndex;
	m_lNum = srcGoods.m_lNum;
	//m_bInTradePage = srcGoods.m_bInTradePage;
	//m_bInPlayerShop = srcGoods.m_bInPlayerShop;
	m_wHoleNum = srcGoods.m_wHoleNum;
	if (m_wHoleNum>0&&!srcGoods.m_mapHoleState.empty())
	{
		map<long,bool>::const_iterator Iter = srcGoods.m_mapHoleState.begin();
		for (;Iter!=srcGoods.m_mapHoleState.end();Iter++)
		{
			m_mapHoleState[Iter->first] = Iter->second;
		}
	}
	m_bHaveShadow = srcGoods.m_bHaveShadow;
	m_lLockCounter = srcGoods.m_lLockCounter;
	m_dwPurchaseLimit = srcGoods.m_dwPurchaseLimit;

	m_pFlashEffect = new CEffect(*srcGoods.m_pFlashEffect);

	m_pAnimInfo->GetActionInfo()->dwCurAction = MAKEFOURCC('g','r','n','d');
	m_pAnimInfo->GetActionInfo()->bCurActionLooped = true;

	m_bDropEffect = srcGoods.m_bDropEffect;
	m_fDropHeight = srcGoods.m_fDropHeight;
	m_stBaseProperty.strName = CGoodsList::GetProperty(m_dwIndex)->BaseProperty.strName;
	SetName(m_stBaseProperty.strName.c_str());
	m_stBaseProperty.dwBuyPrice = srcGoods.m_stBaseProperty.dwBuyPrice;
	m_stBaseProperty.dwValue = srcGoods.m_stBaseProperty.dwValue;
	m_stBaseProperty.dwSilverPrice = srcGoods.m_stBaseProperty.dwSilverPrice;
	m_stBaseProperty.dwType = CGoodsList::GetProperty(m_dwIndex)->BaseProperty.dwType;
	m_GoodsBaseType = static_cast<eGoodsBaseType>(srcGoods.m_stBaseProperty.dwType);
	if (!srcGoods.m_InlayHole.empty())
	{
		map<long,tagCardProperty>::const_iterator iter = srcGoods.m_InlayHole.begin();
		for (;iter!=srcGoods.m_InlayHole.end();iter++)
		{
			m_InlayHole[iter->first] = iter->second;
		}
	}
	vector<CGoodsList::tagEffect>::const_iterator EffectIter = srcGoods.m_vectorEffect.begin();
	for(;EffectIter!= srcGoods.m_vectorEffect.end();EffectIter++)
	{
		m_vectorEffect.push_back(*EffectIter);
	}

	SetGraphicsID(GetProperty()->dwGroundId);

	m_strManufacturerName = srcGoods.m_strManufacturerName;

	//m_strActiveInfor = srcGoods.m_strActiveInfor;
}

CGoods::~CGoods(void)
{
	SAFE_DELETE(m_pFlashEffect);
}

/// ���������ߵ�����
void CGoods::SetManufacturerName(const char* pStrName)
{
	if (pStrName==NULL)
	{
		m_strManufacturerName = "";
	}else
		m_strManufacturerName = pStrName;
}

void CGoods::AI(void)
{
	//����Ч��
	if (m_bDropEffect)
	{
		ulong dwCurrentTime = timeGetTime();
		ulong dwTimeElapse = dwCurrentTime - m_dwDropTimeStart;
		if (dwTimeElapse > 800)
		{
			m_bDropEffect = false;
		}
		else
		{
			const float fHeightArray[] = { 1.7f,2.2f,3.0f,
				2.2f,1.7f,1.0f,0.6f,0.2f,0.0f,
				0.2f,0.4f,0.3f,0.2f,0.1f,0.0f};//15
			int iPos = 15 * dwTimeElapse / 800;
			if (iPos > 15 - 1)
				iPos = 15 - 1;
			m_fDropHeight = fHeightArray[iPos];
		}
		if (dwTimeElapse % 3)
		{
			float fDirectY = D3DXToRadian(rand() % 360);
			SetNowDir(fDirectY);
		}
	}
	else
	{
		SetNowDir(0.0f);
	}
		
}

// ��ӵ�CByteArray
bool CGoods::AddToByteArray(vector<uchar>* pByteArray, bool bExData)
{
	CGoodsList::tagBaseProperty *pstGood=GetProperty();
	_AddToByteArray(pByteArray, GetIndex());//ѹ����ƷINDEX
	_AddToByteArray(pByteArray,GetExID());
	_AddToByteArray(pByteArray,GetNum());

	_AddToByteArray(pByteArray,(char *)GetName(),(long)strlen(GetName()));

	_AddToByteArray(pByteArray,GetBaseProperty()->dwValue);
	_AddToByteArray(pByteArray,GetBaseProperty()->dwType);
	_AddToByteArray(pByteArray,GetEffectVal(37,2));
	_AddToByteArray(pByteArray,GetPosX());
	_AddToByteArray(pByteArray,GetPosY());

	return true;
}

//�ⲿ���õ�ˢ�����ԵĽӿ�
void CGoods::UpdateProperty(CMessage* pMsg)
{
	long wNum = pMsg->GetLong();						//���θ��µ����Ը���
	long pro_type = 0;									//�������ͣ���Ʒ�������Ե�ö��ֵ��
	long dataPos = 0;									//ȡ�������Եĵ�һ��ֵ���ǵڶ���ֵ
	long data = 0;										//�����ֵ

	for (int i = 0;i<wNum;i++)
	{	
		pro_type = pMsg->GetLong();
		dataPos = pMsg->GetLong();
		data = pMsg->GetLong();
		SetNewEffectVal((ushort)pro_type,(int)dataPos,data);
	}	
}


// ����
bool CGoods::DecordFromByteArray(uchar* pByte, long& pos, bool bExData)
{
	GetEffect()->clear();
	m_wHoleNum = 0;
	m_mapHoleState.clear();
	m_InlayHole.clear();
	/// ��Ʒ�ļ򻯰����
	if (!bExData)
	{
		SetIndex(_GetDwordFromByteArray(pByte,pos));
		SetNum(_GetLongFromByteArray(pByte,pos));
		SetPosX(_GetFloatFromByteArray(pByte, pos));
		SetPosY(_GetFloatFromByteArray(pByte, pos));
		CGoodsList::tagGoods2* pstGoods = CGoodsList::GetProperty(GetIndex());
		if(pstGoods)
		{
			m_stBaseProperty.strName = pstGoods->BaseProperty.strName;
			m_stBaseProperty.dwValue = pstGoods->BaseProperty.dwPrice;					//	��Ҽ۸�
			m_stBaseProperty.dwSilverPrice = pstGoods->BaseProperty.dwSilverPrice;		// ���Ҽ۸�
			m_stBaseProperty.dwBuyPrice = 0;											//	��Ʒ����۸�
			m_stBaseProperty.dwType = pstGoods->BaseProperty.dwType;
			// �ȸ������������������Ʒ�ĸ��������б�
			vector<CGoodsList::tagEffect>::iterator EffectIter = pstGoods->vectorEffect.begin();
			for(;EffectIter!=pstGoods->vectorEffect.end();EffectIter++)
			{
				CGoodsList::tagEffect GoodsEffect;
				GoodsEffect.wType = EffectIter->wType;
				GoodsEffect.dwValue1 = EffectIter->dwValue1;
				GoodsEffect.dwValue2 = EffectIter->dwValue2;
				GoodsEffect.bEnable = false;
				GoodsEffect.bHide = false;
				GetEffect()->push_back(GoodsEffect);
			}
			SetNewEffectVal(GAP_WEAPON_LEVEL,1,_GetLongFromByteArray(pByte, pos));
			SetGraphicsID(GetProperty()->dwGroundId);
			m_stBaseProperty.dwType = GetProperty()->dwType;
			m_GoodsBaseType = static_cast<eGoodsBaseType>(m_stBaseProperty.dwType);
			SetName(m_stBaseProperty.strName.c_str());
			return true;
		}
		else	// �ͻ���goods.dat��û�������Ʒ���������ȥ
		{
			char str[256];
			sprintf(str, AppFrame::GetText("Package_8"), m_stBaseProperty.strName.c_str());
			MessageBox(g_hWnd, str, "warning", MB_OK);
			return false;
		}
	}
	/// ��Ʒ���������ݽ���
	else
	{
		SetIndex(_GetDwordFromByteArray(pByte,pos));
		_GetBufferFromByteArray(pByte, pos,m_guExID);
		SetNum(_GetLongFromByteArray(pByte,pos));

		///�������ߵ�����
		char str[512];
		_GetStringFromByteArray(pByte, pos, str);		
		m_strManufacturerName = str;


		GetBaseProperty()->dwValue = _GetDwordFromByteArray(pByte,pos);		//	��Ҽ۸�
		GetBaseProperty()->dwSilverPrice = _GetDwordFromByteArray(pByte,pos);		// ���Ҽ۸�

		GetBaseProperty()->dwBuyPrice = _GetDwordFromByteArray(pByte,pos);	//	��Ʒ����۸�
		GetBaseProperty()->dwType = _GetDwordFromByteArray(pByte,pos);
		m_GoodsBaseType=static_cast<eGoodsBaseType>(GetBaseProperty()->dwType);

		CGoodsList::tagGoods2* pstGoods = CGoodsList::GetProperty(GetIndex());
		if(pstGoods)
		{
			// �ȸ������������������Ʒ�ĸ��������б�
			vector<CGoodsList::tagEffect>::iterator EffectIter = pstGoods->vectorEffect.begin();
			for(;EffectIter!=pstGoods->vectorEffect.end();EffectIter++)
			{
				CGoodsList::tagEffect GoodsEffect;
				GoodsEffect.wType = EffectIter->wType;
				GoodsEffect.dwValue1 = EffectIter->dwValue1;
				GoodsEffect.dwValue2 = EffectIter->dwValue2;
				GoodsEffect.bEnable = false;
				GoodsEffect.bHide = false;
				GetEffect()->push_back(GoodsEffect);
			}

			// ���ݷ������������Ĳ��ָ��������������������ⲿ�����ݣ��������������������ӵ�һЩ���ԣ�
			long lEffectSize = _GetLongFromByteArray(pByte,pos);
			for (int i = 0;i<lEffectSize;i++)
			{
				ushort wEffectType = _GetWordFromByteArray(pByte,pos);
				long lValue1 = _GetLongFromByteArray(pByte,pos);
				long lValue2 = _GetLongFromByteArray(pByte,pos);
				SetNewEffectVal(wEffectType,1,lValue1);
				SetNewEffectVal(wEffectType,2,lValue2);
			}

			SetPosX(_GetFloatFromByteArray(pByte, pos));
			SetPosY(_GetFloatFromByteArray(pByte, pos));

			SetGraphicsID(GetProperty()->dwGroundId);
			m_stBaseProperty.strName = pstGoods->BaseProperty.strName;
			SetName(m_stBaseProperty.strName.c_str());

			long lBaseHoleNum = GetEffectVal(GAP_HOLE_NUM,2);
			if (lBaseHoleNum!=0)
			{
				DWORD dwCardNum = _GetDwordFromByteArray(pByte,pos);
				//m_wHoleNum = _GetDwordFromByteArray(pByte,pos);
				m_wHoleNum = (ushort)lBaseHoleNum;
				for (long i = 0;i<lBaseHoleNum;i++)
				{
					m_mapHoleState.insert(pair<long,bool>(i,true));
				}
				//if (m_wHoleNum>0)
				//{
				//	for (ushort i = 0;i<m_wHoleNum;i++)
				//	{
				//		m_mapHoleState[i] = true;
				//	}
				//}
				DWORD dwGoodsIndex = 0;
				CGoods* pCardGoods = NULL;
				for (DWORD i = 0;i<dwCardNum;i++)
				{
					dwGoodsIndex = _GetDwordFromByteArray(pByte,pos);
					if (dwGoodsIndex!=0)
					{
						pCardGoods = new CGoods();
						pCardGoods->DecordFromByteArray(pByte,pos);
						tagCardProperty temp;
						temp.dwCardIndex = pCardGoods->GetIndex();
						temp.CardGUID = pCardGoods->GetExID();
						temp.dwCardType = pCardGoods->GetEffectVal(GAP_CARD_CATEGORY,1);
						temp.bMaxNum = pCardGoods->GetEffectVal(GAP_CARD_ENCHASE_NUM_LIMIT,1);
						temp.strCardName = pCardGoods->GetName();
						vector<CGoodsList::tagEffect>::iterator Iter = pCardGoods->GetEffect()->begin();
						for(;Iter!=pCardGoods->GetEffect()->end();Iter++)
						{
							temp.CardEffect.push_back(*Iter);
						}
						m_InlayHole.insert(pair<long,tagCardProperty>(i,temp));
						SAFE_DELETE(pCardGoods);
					}
				}
			}
			return true;
		}
		else	// �ͻ���goods.dat��û�������Ʒ���������ȥ
		{
			ulong dwDur=_GetDwordFromByteArray(pByte,pos);
			long lEffectSize=_GetLongFromByteArray(pByte,pos);
			SetEffectVal(GAP_GOODS_MAX_DURABILITY,2,dwDur);
			SetPosX(_GetFloatFromByteArray(pByte, pos));
			SetPosY(_GetFloatFromByteArray(pByte, pos));
			char str[256];
            sprintf(str, AppFrame::GetText("Package_8"), m_stBaseProperty.strName.c_str());
			MessageBox(g_hWnd, str, "warning", MB_OK);
			return false;
		}
	}
	return false;
}


bool CGoods::DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData)
{	
	GetEffect()->clear();
	m_wHoleNum = 0;
	m_mapHoleState.clear();
	m_InlayHole.clear();
	/// ��Ʒ�ļ򻯰����
	if (!bExData)
	{
		SetIndex(setReadDB.GetDwordFromByteArray());
		SetNum(setReadDB.GetLongFromByteArray());
		SetPosX(setReadDB.GetFloatFromByteArray());
		SetPosY(setReadDB.GetFloatFromByteArray());
		
		CGoodsList::tagGoods2* pstGoods = CGoodsList::GetProperty(GetIndex());
		if(pstGoods)
		{
			m_stBaseProperty.strName = pstGoods->BaseProperty.strName;
			// �ȸ������������������Ʒ�ĸ��������б�
			vector<CGoodsList::tagEffect>::iterator EffectIter = pstGoods->vectorEffect.begin();
			for(;EffectIter!=pstGoods->vectorEffect.end();EffectIter++)
			{
				CGoodsList::tagEffect GoodsEffect;
				GoodsEffect.wType = EffectIter->wType;
				GoodsEffect.dwValue1 = EffectIter->dwValue1;
				GoodsEffect.dwValue2 = EffectIter->dwValue2;
				GoodsEffect.bEnable = false;
				GoodsEffect.bHide = false;
				GetEffect()->push_back(GoodsEffect);
			}
			SetNewEffectVal(GAP_WEAPON_LEVEL,1,setReadDB.GetLongFromByteArray());
			SetGraphicsID(GetProperty()->dwGroundId);
			m_stBaseProperty.dwType = GetProperty()->dwType;
			m_GoodsBaseType = static_cast<eGoodsBaseType>(m_stBaseProperty.dwType);
			SetName(m_stBaseProperty.strName.c_str());
			return true;
		}
		else	// �ͻ���goods.dat��û�������Ʒ���������ȥ
		{
			char str[256];
			sprintf(str, AppFrame::GetText("Package_8"), m_stBaseProperty.strName.c_str());
			MessageBox(g_hWnd, str, "worning", MB_OK);
			return false;
		}
	}
	/// ��Ʒ���������ݽ���
	else
	{
		SetIndex(setReadDB.GetDwordFromByteArray());
		setReadDB.GetBufferFromByteArray(m_guExID);
		SetNum(setReadDB.GetLongFromByteArray());

		/// �����ߵ�����
		char str[512];
		setReadDB.GetStringFromByteArray(str,512);
		m_strManufacturerName = str;

		GetBaseProperty()->dwValue = setReadDB.GetDwordFromByteArray();		//	��Ҽ۸�
		GetBaseProperty()->dwSilverPrice = setReadDB.GetDwordFromByteArray();		// ���Ҽ۸�

		GetBaseProperty()->dwBuyPrice = setReadDB.GetDwordFromByteArray();	//	��Ʒ����۸�
		GetBaseProperty()->dwType = setReadDB.GetDwordFromByteArray();
		m_GoodsBaseType=static_cast<eGoodsBaseType>(GetBaseProperty()->dwType);

		CGoodsList::tagGoods2* pstGoods = CGoodsList::GetProperty(GetIndex());
		if(pstGoods)
		{
			// �ȸ������������������Ʒ�ĸ��������б�
			vector<CGoodsList::tagEffect>::iterator EffectIter = pstGoods->vectorEffect.begin();
			for(;EffectIter!=pstGoods->vectorEffect.end();EffectIter++)
			{
				CGoodsList::tagEffect GoodsEffect;
				GoodsEffect.wType = EffectIter->wType;
				GoodsEffect.dwValue1 = EffectIter->dwValue1;
				GoodsEffect.dwValue2 = EffectIter->dwValue2;
				GoodsEffect.bEnable = false;
				GoodsEffect.bHide = false;
				GetEffect()->push_back(GoodsEffect);
			}

			// ���ݷ������������Ĳ��ָ��������������������ⲿ�����ݣ��������������������ӵ�һЩ���ԣ�
			long lEffectSize = setReadDB.GetLongFromByteArray();
			for (int i = 0;i<lEffectSize;i++)
			{
				ushort wEffectType = setReadDB.GetWordFromByteArray();
				long lValue1 = setReadDB.GetLongFromByteArray();
				long lValue2 = setReadDB.GetLongFromByteArray();
				SetNewEffectVal(wEffectType,1,lValue1);
				SetNewEffectVal(wEffectType,2,lValue2);
			}

			SetPosX(setReadDB.GetFloatFromByteArray());
			SetPosY(setReadDB.GetFloatFromByteArray());

			SetGraphicsID(GetProperty()->dwGroundId);
			m_stBaseProperty.strName = pstGoods->BaseProperty.strName;
			SetName(m_stBaseProperty.strName.c_str());

			long lBaseHoleNum = GetEffectVal(GAP_HOLE_NUM,2);
			if (lBaseHoleNum!=0)
			{
				ulong dwCardNum = setReadDB.GetDwordFromByteArray();
				//m_wHoleNum = setReadDB.GetDwordFromByteArray();
				m_wHoleNum = (ushort)lBaseHoleNum;
				for (long i = 0;i<lBaseHoleNum;i++)
				{
					m_mapHoleState.insert(pair<long,bool>(i,true));
				}
				//if (m_wHoleNum>0)
				//{
				//	for (ushort i = 0;i<m_wHoleNum;i++)
				//	{
				//		m_mapHoleState[i] = true;
				//	}
				//}
				DWORD dwGoodsIndex = 0;
				CGoods* pCardGoods = NULL;
				for (DWORD i = 0;i<dwCardNum;i++)
				{
					dwGoodsIndex = setReadDB.GetDwordFromByteArray();
					if (dwGoodsIndex!=0)
					{
						pCardGoods = new CGoods();
						pCardGoods->DecodeFromDataBlock(setReadDB,bExData);
						tagCardProperty temp;
						temp.dwCardIndex = pCardGoods->GetIndex();
						temp.CardGUID = pCardGoods->GetExID();
						temp.dwCardType = pCardGoods->GetEffectVal(GAP_CARD_CATEGORY,1);
						temp.bMaxNum = pCardGoods->GetEffectVal(GAP_CARD_ENCHASE_NUM_LIMIT,1);
						temp.strCardName = pCardGoods->GetName();
						vector<CGoodsList::tagEffect>::iterator Iter = pCardGoods->GetEffect()->begin();
						for(;Iter!=pCardGoods->GetEffect()->end();Iter++)
						{
							temp.CardEffect.push_back(*Iter);
						}
						m_InlayHole.insert(pair<long,tagCardProperty>(i,temp));
						SAFE_DELETE(pCardGoods);
					}
				}
			}
			return true;
		}
		else	// �ͻ���goods.dat��û�������Ʒ���������ȥ
		{
			ulong dwDur=setReadDB.GetDwordFromByteArray();
			long lEffectSize=setReadDB.GetLongFromByteArray();
			SetEffectVal(GAP_GOODS_MAX_DURABILITY,2,dwDur);
			SetPosX(setReadDB.GetFloatFromByteArray());
			SetPosY(setReadDB.GetFloatFromByteArray());
			char str[256];
			sprintf(str, AppFrame::GetText("Package_8"), m_stBaseProperty.strName.c_str());
			MessageBox(g_hWnd, str, "worning", MB_OK);
			return false;
		}
	}
	return true;
}

ulong CGoods::GetGoodsBaseType()
{
	CGoodsList::tagGoods2* pstGoods=CGoodsList::GetProperty(m_dwIndex);
	if(!pstGoods)
		return -1;
	return pstGoods->BaseProperty.dwType;
}

//�ж��ܷ�ʰȡ�ɵ�����Ʒ
bool CGoods::isGoodsCanAccumulate(CGoods *pOnGroundGoods)
{
	if (pOnGroundGoods==NULL)	return false;
	CPlayer* pPlayer = GetGame()->GetMainPlayer();
	if (pPlayer!=NULL)
	{
		int iExtendID = -1;
		int iPos = -1;
		if (pPlayer->FindOverLapGoodsByIndex(pOnGroundGoods->GetIndex(),pOnGroundGoods->GetNum(),iExtendID,iPos))
		{
			return true;
		}
	}
	//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(CStringReading::LoadText(200,9));
	return false;
}
bool CGoods::IsLocked()
{
	return m_lLockCounter > 0 ? true : false;
}

void CGoods::Lock()
{
	m_lLockCounter = 1;
	if ( m_lLockCounter> 0 )
	{
		SetHaveShadowState(true);
	}
}

void CGoods::UnLock()
{ 
	m_lLockCounter = 0;
	if (m_lLockCounter==0)
	{
		SetHaveShadowState(false);
	}
}

long CGoods::GetEffectVal(ushort wType,int nWhich)
{
	if(!GetEffect()) return 0;

	if(GetEffect()->empty())return 0;
	vector<CGoodsList::tagEffect>::iterator iter=GetEffect()->begin();
	for(;iter!=GetEffect()->end();iter++)
	{
		if(iter->wType==wType)
		{
			if(nWhich==1)
				return iter->dwValue1;
			else if(nWhich==2)
				return iter->dwValue2;
		}
	}
	
	return 0;
}

void CGoods::SetEffectVal(ushort wType,int nWhich,long lVal)
{
	vector<CGoodsList::tagEffect>::iterator iter=GetEffect()->begin();
	for(;iter!=GetEffect()->end();iter++)
	{
		if(iter->wType==wType)
		{
			if(nWhich==1)
				iter->dwValue1=lVal;
			else
				iter->dwValue2=lVal;
			return;
		}
	}
}

//����Ʒ����µĸ�������
void CGoods::SetNewEffectVal(ushort wType,int nWhich,long lVal)
{
	vector<CGoodsList::tagEffect>::iterator iter=GetEffect()->begin();
	for(;iter!=GetEffect()->end();iter++)
	{
		if(iter->wType==wType)
		{
			if(nWhich==1)
				iter->dwValue1=lVal;
			else
				iter->dwValue2=lVal;
			return;
		}
	}
	CGoodsList::tagEffect temp;
	temp.wType = wType;
	if (nWhich==1)
	{
		temp.dwValue1 = lVal;
	}else
		temp.dwValue2 = lVal;
	m_vectorEffect.push_back(temp);
}

bool CGoods::CanRepair()
{	
	if(GetBaseProperty()->dwType == GBT_EQUIPMENT
		/*GetBaseProperty()->dwType>=GBT_E_HEAD && 
		GetBaseProperty()->dwType<=GBT_E_WEAPON && 
		GetBaseProperty()->dwType!=GBT_E_WING*/ )
	{
		return true;
	}
	return false;
}

bool CGoods::CanUpgrade()
{
	//if(GetBaseProperty()->dwType==GBT_E_HEAD || 
	//	GetBaseProperty()->dwType==GBT_E_BODY || 
	//	GetBaseProperty()->dwType==GT_GLOVE || 
	//	GetBaseProperty()->dwType==GT_WEAPON || 
	//	GetBaseProperty()->dwType==GT_BOOT ||
	//	GetBaseProperty()->dwType==GT_RING ||
	//	GetBaseProperty()->dwType==GT_NECKLACE)
	//{
	//	return true;
	//}
	return false;
}



/*
*���ܣ� �ж��Ƿ�ӵ�д� ����
*ϸ�������� 
*������ wType ���Ե�ö��ֵ
*����ֵ�� bool ���򷵻� true
*�޸�ʱ�䣺2008-1-2
*/
bool CGoods::HasEffect( ushort wType )
{
	if( GetEffect() )
	{
		if( ! GetEffect()->empty() )
		{
			vector< CGoodsList::tagEffect >::iterator	iter;
			for( iter = GetEffect()->begin(); iter != GetEffect()->end(); iter++ )
			{
				if( iter->wType == wType )
				{
					return true;
				}
			}
		}
	}

	return false;
}

ulong CGoods::RepairPrice()
{
	ulong dwRepairPrice = 0;
	if( CanRepair() )
	{
		LONG lCurrentDurability	= GetEffectVal( 37, 2 );
		LONG lMaxDurability		= GetEffectVal( 37, 1 );
		FLOAT fFactor			= GetGame()->GetSetup()->fRepairFactor;
		FLOAT fRate				= ( lMaxDurability - lCurrentDurability ) / static_cast<FLOAT>( lMaxDurability );
		if( fRate < 0.0f )
		{
			fRate = 0.0f;
		}
		dwRepairPrice = static_cast<ulong>( GetPrice() * fRate * fFactor );
	}
	return dwRepairPrice;
}

bool CGoods::Display(void)
{
	if( !CMoveShape::Display() )
		return false;
	DispalayFlashEffect();
	return true;
}

// ��ʼ��������Ч
void CGoods::IniFlashEffect()
{
	m_pFlashEffect->SetPosXY(GetPosX(), GetPosY());
	float fH = 0.0f;
	if(GetGame()->GetRegion())
		fH = ((CClientRegion*)GetGame()->GetRegion())->GetCellHeight(GetPosX(),GetPosY());
	m_pFlashEffect->SetHeight(fH);
	m_pFlashEffect->SetLoop(true);

	m_pFlashEffect->SetDirEx(0.0f);
	m_pFlashEffect->SetNowDir(0.0f);
}

//�õ����������
void CGoods::DispalayFlashEffect()
{
	m_pFlashEffect->Display();
}

ulong CGoods::GetUpdatePrice()
{
	ulong dwResult = 0;
	if( CanUpgrade() )
	{
		// ÿ���������ã���Ʒ�����۸����1����38��4��lg��Ʒ�����۸�/15��pow����Ʒ��ǰ�ȼ���
		//LONG lCurrentLevel	= GetEffectVal( 48, 1 );
		LONG lUpgradePrice	= GetEffectVal( 72, 1 );
		return lUpgradePrice;
		//FLOAT fFactor		= 1 + ( 38 - 4 * log10( static_cast<FLOAT>(lUpgradePrice) ) ) / 15;
		//fFactor				= pow( fFactor, lCurrentLevel );
		//dwResult			= static_cast<ulong>( lUpgradePrice * fFactor );
	}
	return dwResult;
}

ulong CGoods::GetPrice()
{
	return m_stBaseProperty.dwValue>m_stBaseProperty.dwSilverPrice?m_stBaseProperty.dwValue:m_stBaseProperty.dwSilverPrice;
}

ulong CGoods::GetPriceType()
{
	return GetBaseProperty()->dwPriceType;
}

ulong CGoods::GetSalePrice()
{
	ulong dwVendPrice = 0;
	//if( GetPrice() )
	//{
	//	//##���Ȼ�ȡȫ������
	//	FLOAT fVendPrice = static_cast<FLOAT>( GetPrice() );

	//	//CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( 
	//	//	pGoods -> GetBasePropertiesIndex() );
	//	//if( pProperties )
	//	{
	//		if( pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_EQUIPMENT ) 
	//		{
	//			//##��ȡ�۾ɱ�ֵ��
	//			FLOAT fDepreciableRate = CGlobeSetup::GetSetup() -> fBasePriceRate;

	//			//##��ȡ��ǰ/����;�
	//			ulong dwCurrentDurability	= pGoods -> GetAddonPropertyValues( 
	//				CGoodsBaseProperties::GAP_GOODS_MAXIMUM_DURABILITY, 2 );
	//			ulong dwMaxDurability		= pGoods -> GetAddonPropertyValues( 
	//				CGoodsBaseProperties::GAP_GOODS_MAXIMUM_DURABILITY, 1 );

	//			if( dwMaxDurability == 0 )
	//			{
	//				//##����޷���ȡ������;�,��ô�ͱ�������0����
	//				fVendPrice = pGoods -> GetPrice() * fDepreciableRate;
	//			}
	//			else 
	//			{
	//				//##�����۾ɺ�ļ۸�
	//				fVendPrice = pGoods -> GetPrice() * fDepreciableRate + 
	//					pGoods -> GetPrice() * ( 1.0f - fDepreciableRate ) * ( dwCurrentDurability / dwMaxDurability );
	//			}
	//		}
	//	}

	//	//##��ȡ�ۼ���
	//	FLOAT fTradeInPriceRate = CGlobeSetup::GetSetup() -> fTradeInRate;

	//	dwVendPrice = static_cast<ulong>( fVendPrice * fTradeInPriceRate );
	//}
	return dwVendPrice;
}

void CGoods::SetDropEffect()
{
	m_bDropEffect = true;
	m_dwDropTimeStart = timeGetTime();
}

//���ظ߶�
float CGoods::GetHeight()
{
	if (m_bDropEffect)
	{
		return m_fDropHeight + m_fHeight;
	}
	return m_fHeight;
}

////////////��Ʒ����Ƕ�׽ӿ�////////////////

//����Ʒ���
bool CGoods::Stiletto()
{
	if (m_wHoleNum>=0&&m_wHoleNum<GetEffectVal(GAP_HOLE_NUM,2))
	{
		m_mapHoleState[m_wHoleNum] = true;
		m_wHoleNum++;
		this->SetEffectVal(GAP_REFINE,1,0);
		return true;
	}
	return false;
}

//�����Ƕ��
bool CGoods::PlugUpHole(ushort index,ushort wHoleNum)
{
	if (index<0||index>=GetEffectVal(GAP_HOLE_NUM,2)) 
		return false;
	m_wHoleNum = wHoleNum;
	if (m_wHoleNum>=0)
	{
		map<long,tagCardProperty>::iterator it = m_InlayHole.find(index);
		if (it!=m_InlayHole.end())
		{
			m_InlayHole.erase(it);
		}
		map<long,bool>::iterator HoleIter = m_mapHoleState.find(index);
		if (HoleIter!=m_mapHoleState.end()&&HoleIter->second)
		{
			HoleIter->second=false;
		}
		return true;
	}else
	{
		m_wHoleNum = 0;
	}
	return false;	
}

//��Ƕ��Ƭ
bool CGoods::EnchaseCard(ushort index,tagCardProperty card)
{
	if (index>=m_InlayHole.size()&&index<m_wHoleNum)
	{
		m_InlayHole.insert(pair<long,tagCardProperty>(index,card));
		return true;
	}
	return false;
}

//�����Ƭ
bool CGoods::RemoveCard(ushort index)
{
	map<long,tagCardProperty>::iterator it = m_InlayHole.find(index);
	if (it!=m_InlayHole.end())
	{
		m_InlayHole.erase(it);
		return true;
	}
	return false;
}

//������Ʒ�ĵ�ǰ����
void CGoods::SetHoleNum(ushort wNum)
{
	if (wNum>=0&&wNum<=GetEffectVal(GAP_HOLE_NUM,2))
	{
		m_wHoleNum = wNum;
	}
}

//��ȡ��Ʒָ��λ�õ���Ƕ�׵�״̬�������أ�
bool CGoods::GetHoleState(ushort HoleIndex) 
{
	if (HoleIndex>=0&&HoleIndex<GetEffectVal(GAP_HOLE_NUM,2))
	{
		return m_mapHoleState[HoleIndex];
	}
	return false;
}

//������Ʒָ��λ�õ���Ƕ�׵�״̬�������أ�
void CGoods::SetHoleState(ushort HoleIndex,bool bState)
{
	if (HoleIndex>=0&&HoleIndex<GetEffectVal(GAP_HOLE_NUM,2))
	{
		m_mapHoleState[HoleIndex] = bState;
	
	}
}

/// ��ȡ��Ʒ��ʱ��
const string CGoods::GetGoodsLimitTime()
{
	//string strResult = "";
	//��ʧʱ��
	long val1 = GetEffectVal(GAP_TIME_LIMIT,1);
	long val2 = GetEffectVal(GAP_TIME_LIMIT,2);		
	long val3 = 0;
	long val4 = 0;
	long val5 = 0;
	char scResult[64] = "";
	char strTime[64]="";char strHour[32] = "";char strSecond[32] = "";
	/// ֵ1Ϊ1ʱ��ʾ��Ʒ�ܵ���Чʱ��
	if(val1==1)
	{		
		val3 = val2/1440;  val4 = val2%1440;	/// ������
		if (val3!=0)
		{
			sprintf(strTime,AppFrame::GetText("Goods_447"),val3);
		}
		val3 = val4/60; val5 = val4%60;			/// ��Сʱ����
		if (val3!=0)
		{
			sprintf(strHour,AppFrame::GetText("Goods_448"),val3);
			strcat(strTime,strHour);
		}
		if (val5!=0)
		{
			sprintf(strSecond,AppFrame::GetText("Goods_449"),val5);
			strcat(strTime,strSecond);
		}
		sprintf(scResult,AppFrame::GetText("Goods_445"),strTime);
		//strResult = scResult;
	}
	/// ֵ1Ϊ2ʱ��ʾ��Ʒ����ʧ��ʣ����ʱ��
	else if (val1==2)
	{
		//SYSTEMTIME	stTime;
		//char		szTime[128] = {0};
		//GetLocalTime(&stTime);
		//sprintf(szTime,"%d-%d-%d %d:%d:%d" ,
		//	stTime.wYear,
		//	stTime.wMonth,
		//	stTime.wDay,
		//	stTime.wHour,
		//	stTime.wMinute,
		//	stTime.wSecond);

		//time_t curr_time = time(NULL);
		//tm *pTm = localtime(&curr_time);

		// ��ǰʱ��
		long lCurrTime = (long)time( NULL );//+CGuiExPage::m_lC2SDeltaTime;
		val2 -= lCurrTime;
		if (val2>=60)
		{
			val3 = val2/(24 * 60 * 60);  val4 = val2%(24 * 60 * 60);	/// ������
			if (val3!=0)
			{
				sprintf(strTime,AppFrame::GetText("Goods_447"),val3);
			}
			val3 = val4/3600; val5 = val4%3600;		/// ��Сʱ
			if (val3!=0)
			{
				sprintf(strHour,AppFrame::GetText("Goods_448"),val3);
				strcat(strTime,strHour);
			}
			val3 = val5/60;							/// ���
			if (val3!=0)
			{
				sprintf(strSecond,AppFrame::GetText("Goods_449"),val3);
				strcat(strTime,strSecond);
			}
			sprintf(scResult,AppFrame::GetText("Goods_446"),strTime);
			//strResult = scResult;
		}
		/// ���ʱ��С��1�����˾���ʾ������Ʒ�������ڡ�
		else if (val2>0&&val2<60)
		{
			sprintf(scResult,AppFrame::GetText("Goods_460"));
		}
		/// ���ʱ��Ϊ0����Ϊ��������ʾ������Ʒ�ѹ��ڡ�
		else 
		{
			sprintf(scResult,AppFrame::GetText("Goods_461"));
		}

	}
	return string(scResult);
}


// ���ݴ������Ʒ��ö�Ӧ��Ʒ��ͼƬ�Լ���ɫ
void CGoods::GetGoodsQuality(string & str, long & color)
{
	str = "";
	color = 0xffffffff;
	switch( GetEffectVal( GAP_QUALITY, 1 ) )
	{
	case 2:		// ��ɫƷ��
		{
			str = "datafiles/imagesets/Public/icon_00A2FF.tga";
			color = 0xff00A2FF;
		}
		break;
	case 3:		// ��ɫƷ��
		{
			str = "datafiles/imagesets/Public/icon_00FF19.tga";
			color = 0xff00FF19;
		}
		break;
	case 4:		// ��ɫƷ��
		{
			str = "datafiles/imagesets/Public/icon_8400FF.tga";
			color = 0xff8400FF;
		}
		break;
	case 5:		// ��õ��ɫ
		{
			str = "datafiles/imagesets/Public/icon_FF00F0.tga";
			color = 0xffff00f0;
		}
		break;
	case 6:		//����
		{
			str = "datafiles/imagesets/Public/icon_dab26c.tga";
			color = 0xffdab26c;
		}
		break;
	case 7:		//��ɫ
		{
			str = "datafiles/imagesets/Public/icon_FF7200.tga";
			color = 0xffff7200;
		}
		break;
	case 8:		//��ɫ
		{
			str = "datafiles/imagesets/public/icon_FF0000.tga";
			color = 0xffff0000;
		}
		break;
	case 9:		//�������̵���Ʒר�ñ߿�
		{
			str = "datafiles/imagesets/Public/icon_Tradables.tga";
			color = 0xffff0000;
		}
		break;
	}
}

////�Կ�Ƭ�׽�������
//void CGoods::CardHoleOrder()
//{
//	tagCardProperty** temp = NULL;
//	int count = 0;
//	ushort size = (ushort)m_InlayHole.size();
//	temp = new tagCardProperty*[size];
//	map<ushort,tagCardProperty>::iterator it = m_InlayHole.begin();
//	for (int i = 0;it!=m_InlayHole.end();i++)
//	{
//		if (it->second.dwCardIndex==-1)
//		{			
//			count++;
//			temp[i] = NULL;
//		}else 
//		{
//			temp[i] = new tagCardProperty(it->second);
//		}
//		it++;
//	}
//	m_InlayHole.clear();
//	ushort HoleNum = 0;
//	for (ushort k = 0;k<size;k++)
//	{
//		HoleNum = m_InlayHole.size();
//		if (temp[k]!=NULL)
//		{
//			m_InlayHole[HoleNum] = (*temp[k]);
//			SAFE_DELETE(temp[k]);
//		}else
//		{
//			tagCardProperty NULL_CARD;
//			m_InlayHole[HoleNum] = NULL_CARD;
//		}	
//	}
//	SAFE_DELETE_ARRAY(temp);
//}

/// ��ȡ��Ʒװ��λ����λ
eEquipType CGoods::GetEquipType(void)
{
	//eGoodsBaseType eType = GetGoodsBaseType();
	//if (eType==GBT_EQUIPMENT || eType==GBT_FASHIONABLE_DRESS)
	//{
	return (eEquipType)GetEffectVal(GAP_EQUIP_CATEGORY, 1);
	//}
}

/// ��ȡ��Ʒ��������
eTrumpType CGoods::GetTrumpType(void)
{
	//eGoodsBaseType eType = GetGoodsBaseType();
	//if (eType==GBT_TRUMP)
	//{
	return (eTrumpType)GetEffectVal(GAP_TRUMP_CATEGORY, 1);
	//}
}