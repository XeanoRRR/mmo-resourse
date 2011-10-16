/**
* @filename:ShopCitySys.cpp
* @date: 2010/7/19
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: �̳ǵ��߼���ʵ��
*/
#include "stdafx.h"
#include "ShopCitySys.h"
#include "../Player.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../Other/GlobeSetup.h"
#include "../../Game.h"
#include "../../GameControl.h"
#include "../../../Engine/Utility.h"
#include "../../../ui/Windows/ShopCityDef.h"

extern void FireUIEvent(const char* id, const char* e);
/////////////////////////////////////////////////
// zhaohang  2010/7/20 
// shopCityGoodsData
////////////////////////////////////////////////
SCGData::SCGData()
{
	Clear();
}

SCGData::~SCGData()
{
	Clear();
}

void SCGData::Clear()
{
	ClearAllGoodsList();
	//�����¼
	ClearBuyLogList();
	//������Ʒ
	ClearTop10List();
	//��Ʒ�Ƽ�
	ClearNewestList();
	//����
	ClearGuideList();
	// ɸѡ
	ClearSelectList();
	//��������б�
	ClearSelfLatestBuyList();
}
void SCGData::ClearSelfLatestBuyList()
{
	m_SelfLatestBuyTop10MapA.clear();
}
//ɸѡ
void SCGData::ClearSelectList()
{
	MapFLDTA::iterator iter = m_FilterMapA.begin();
	for ( ; iter != m_FilterMapA.end() ; ++iter)
	{
		MapUFLDTPA& useldt = (*iter).second;
		MapUFLDTPA::iterator uiter = useldt.begin();
		for( ; uiter != useldt.end() ; ++uiter)
		{
			MapStrFilDTPA& strdt = (*uiter).second;
			MapStrFilDTPA::iterator strIter = strdt.begin();
			for ( ; strIter != strdt.end() ; ++strIter)
			{
				(*strIter).second.clear();
			}
			strdt.clear();
		}
		useldt.clear();
	}
	m_FilterMapA.clear();
}
void SCGData::ClearGuideList()
{
	MapGuideDataA::iterator iter = m_GuideMapA.begin();
	for ( ; iter != m_GuideMapA.end() ; ++iter)
	{
		(*iter).second.clear();
	}
	m_GuideMapA.clear();
}
void SCGData::ClearNewestList()
{
	MapNewestA::iterator iter = m_newestMapA.begin();
	for(; iter != m_newestMapA.end() ; ++iter)
	{
		(*iter).second.clear();
	}
	m_newestMapA.clear();
}
void SCGData::ClearTop10List()
{
	MapTop10A::iterator iter = m_Top10MapA.begin();
	for ( ; iter != m_Top10MapA.end() ; ++iter)
	{
		(*iter).second.clear();
	}
	m_Top10MapA.clear();
}
void SCGData::ClearBuyLogList()
{
	m_buyRecordVec.clear();
}
void SCGData::ClearAllGoodsList()
{
	//������Ʒ��Ϣ
	for(MapSCGD::iterator iter = m_goodsDTMap.begin() ; iter != m_goodsDTMap.end() ; ++iter )
	{
		(*iter).second.clear();
	}
	m_goodsDTMap.clear();
}

/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// �����״δ��̳�ʱ������
////////////////////////////////////////////////
void SCGData::DecodeFromDataBlock(CMessage* pMsg)
{
	//�Ƿ����״ε�¼
	BYTE bFlag =  pMsg->GetByte();
	if(bFlag)//��
	{
		/////////////////////////////////////////////////
		// zhaohang  2010/7/20 
		// //����������Ʒ��Ϣ
		////////////////////////////////////////////////
		Clear();//�����������
		CGoodsList::tagGoods2* pstGoods2;
		eSCType type;
		BYTE ShopTypeNum = pMsg->GetByte();//�̳�����
		for(BYTE shopcityType = 0 ; shopcityType < ShopTypeNum ; ++shopcityType)//�����̳����ͣ������Ӧ�̵�����
		{
			type = eSCType(shopcityType);//�̳�����
			//��ϸ��Ʒ�б�
			VecGD GoodsDTVec;
			long size = pMsg->GetLong();//	//��Ŀ
			char str[128]="";//��ʱ��
			for(long sz = 0 ; sz < size ; ++sz)//����������Ʒ��Ϣ
			{
				tagGD goodsDT;
				goodsDT.type = pMsg->GetDWord();
				goodsDT.index = pMsg->GetDWord();
				goodsDT.price = pMsg->GetDWord();
				goodsDT.num = pMsg->GetDWord();
				goodsDT.discount = pMsg->GetByte();
				goodsDT.timelimit = pMsg->GetStr(str,128);
				goodsDT.numlimit = pMsg->GetLong();
				goodsDT.buylimit = pMsg->GetLong();
				//�Ƿ�����Ʒ�б���
				pstGoods2 = CGoodsList::GetProperty(goodsDT.index);
				if(pstGoods2)
					GoodsDTVec.push_back(goodsDT);
			}
			m_goodsDTMap[type] = GoodsDTVec;
			/////////////////////////////////////////////////
			// zhaohang  2010/7/20 
			// ������Ʒ
			////////////////////////////////////////////////
			long top10Num = pMsg->GetLong();//������Ʒ��Ŀ
			VecGDPTA Top10vecDTA;
			for(long l = 0 ; l < top10Num ; ++l)
			{
				tagGDPTA dtpartA;
				dtpartA.type = pMsg->GetDWord();
				dtpartA.index = pMsg->GetDWord();
				//�Ƿ�����Ʒ�б���
				pstGoods2 = CGoodsList::GetProperty(dtpartA.index);
				if(pstGoods2)
					Top10vecDTA.push_back(dtpartA);
			}
			m_Top10MapA[type] = Top10vecDTA;
			/////////////////////////////////////////////////
			// zhaohang  2010/7/20 
			// ��Ʒ�Ƽ�
			////////////////////////////////////////////////

			VecGDPTA newestVecDTA;
			long newestNum = pMsg->GetLong();//��Ʒ�Ƽ���Ŀ
			for(long l = 0 ; l < newestNum ; ++l)
			{
				tagGDPTA dtPartA;
				dtPartA.type = pMsg->GetDWord();
				dtPartA.index = pMsg->GetDWord();
				pstGoods2 = CGoodsList::GetProperty(dtPartA.index);
				if(pstGoods2)
					newestVecDTA.push_back(dtPartA);
			}
			m_newestMapA[type] = newestVecDTA;
			/////////////////////////////////////////////////
			// zhaohang  2010/7/20 
			// ����
			////////////////////////////////////////////////
			MapStrGGDTPA mapDTA;
			long guideType = pMsg->GetLong();//��������,�뵼�������������
			for(long l = 0 ; l < guideType ; ++l)
			{
				VecGDPTA vecDtA;
				char str[256]="";
				pMsg->GetStr(str,128);//��������
				long num = pMsg->GetLong();//��ȡ����ĳ���µ���Ʒ��Ŀ
				for(long n = 0 ; n < num ; ++n)
				{
					tagGDPTA dtA;
					dtA.type = pMsg->GetDWord();
					dtA.index = pMsg->GetDWord();
					pstGoods2 = CGoodsList::GetProperty(dtA.index);
					if(pstGoods2)
						vecDtA.push_back(dtA);
				}
				mapDTA[str] = vecDtA;//
			}
			m_GuideMapA[type] = mapDTA;
			/////////////////////////////////////////////////
			// zhaohang  2010/7/20 
			// ɸѡ
			////////////////////////////////////////////////
			//��Ʒ��ɸѡ������
			long typeNum = pMsg->GetLong();
			MapUFLDTPA mapusldtpa;
			for(long l = 1 ; l <= typeNum ; ++l)//���ݷ��������ô�1��ʼ��0Ҫ����������Ʒ
			{
				//��Ӧɸѡ�����¿�ɸѡ����
				long lFilterNumOfType = pMsg->GetLong();
				char str[128] = "";
				MapStrFilDTPA mapstrsldtpa;
				VecGDPTA vecDTA;
				for(long num = 0 ; num < lFilterNumOfType ; ++num)
				{
					pMsg->GetStr(str,128);//��������
					vecDTA.clear();
					//��Ӧ��ɸѡ���µ���Ʒ��Ŀ
					long itmNum = pMsg->GetLong();
					for(long item = 0 ; item < itmNum ; ++item)
					{
						tagGDPTA dtpartA;
						dtpartA.type = pMsg->GetDWord();
						dtpartA.index = pMsg->GetDWord();
						pstGoods2 = CGoodsList::GetProperty(dtpartA.index);
						if(pstGoods2)
							vecDTA.push_back(dtpartA);
					}
					mapstrsldtpa[str] = vecDTA;
				}
				eSType sType =  eSType(l);//�̵�����
				mapusldtpa[sType] = mapstrsldtpa;
			}
			m_FilterMapA[type] = mapusldtpa;
		}//for(BYTE shopcityType = 0 ; shopcityType < ShopTypeNum ; ++shopcityType)

		/////////////////////////////////////////////////
		// zhaohang  2010/7/21 
		// �����������ǰʮ��¼,ֻ����ƷID������
		////////////////////////////////////////////////
		//�̳�������Ŀ
		BYTE btypeNum = pMsg->GetByte();
		VecGDPTA vecDTASelf;
		for(BYTE b = 0; b < btypeNum ; ++b)
		{
			vecDTASelf.clear();
			BYTE shopCitytype = pMsg->GetByte();//�̳�����
			//�����¼��Ŀ
			long recordNum = pMsg->GetLong();
			for(long l = 0 ; l < recordNum ; ++l)
			{
				tagGDPTA dtpta;
				dtpta.type = pMsg->GetDWord();
				dtpta.index = pMsg->GetDWord();
				pstGoods2 = CGoodsList::GetProperty(dtpta.index);
				if(pstGoods2)
					vecDTASelf.push_back(dtpta);
			}
			eSCType type = (eSCType)b;
			m_SelfLatestBuyTop10MapA[type] = vecDTASelf;
		}
	}//if(bFlag)
}

/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// ���빺����־��Ϣ
////////////////////////////////////////////////
void SCGData::DecodeBuyLogDataBlock(CMessage* pMsg)
{
	long recordNum = pMsg->GetLong();
	ClearBuyLogList();
	for(long l = 0 ; l < recordNum ; ++l)
	{
		char str[128] = "";
		tagBRec buydt;
		pMsg->GetStr(str,128);
		buydt.buytime = str;
		buydt.priceType = pMsg->GetDWord();
		buydt.pricenum = pMsg->GetDWord();
		buydt.buynum = pMsg->GetDWord();
		buydt.goodsIdx = pMsg->GetDWord();
		m_buyRecordVec.push_back(buydt);
	}
}
/////////////////////////////////////////////////
// zhaohang  2010/8/2 
// ������Ʒ�������̳����ͻ�ȡ�������Ʒ��������
////////////////////////////////////////////////
SCGData::tagGD* SCGData::GetGoodsData(uint index,eSCType shopcitytype)
{
	VecGD& vecGD = m_goodsDTMap[shopcitytype];
	for(size_t i = 0 ; i < vecGD.size() ; ++i)
	{
		if(vecGD[i].index == index)
			return &vecGD[i];
	}
	return NULL;
}
/////////////////////////////////////////////////
// zhaohang  2010/8/2 
// ���ݸ������̳ǣ��̵�͵����ı���������Ʒ��ϸ����
////////////////////////////////////////////////
void SCGData::GetGuideInGoodsData(eSCType sctype,string guidstr,VecGD& vecGD)
{
	MapGuideDataA& guiddta = GetGuideList();
	MapStrGGDTPA&  strgdta = guiddta[sctype];
	VecGDPTA& vecgd = strgdta[guidstr];
	//m_goodsDTMap[sctype];
	for(size_t i = 0 ; i < vecgd.size() ; ++i)
	{
		tagGD* gd = GetGoodsData((eSType)vecgd[i].type,vecgd[i].index,sctype);
		if(gd)
			vecGD.push_back(*gd);
	}
}
/////////////////////////////////////////////////
// zhaohang  2010/8/2 
// ���ݸ�������Ʒ�̳ǣ��̵꣬��ɸѡ�ı���������Ʒ��ϸ����
////////////////////////////////////////////////
void SCGData::GetFilterInGoodsData(eSCType sctype,eSType stype,string filterstr,VecGD& vecGD)
{
	MapFLDTA& fldt = GetFilterList();
	MapUFLDTPA& ufldta = fldt[sctype];
	MapStrFilDTPA& ref = ufldta[stype];
	VecGDPTA& vecgd = ref[filterstr];
	for(size_t i = 0 ; i < vecgd.size() ; ++i)
	{
		tagGD* gd = GetGoodsData(stype,vecgd[i].index,sctype);
		if(gd)
			vecGD.push_back(*gd);
		//for (VecGD::iterator iter = m_goodsDTMap[sctype].begin() ; iter != m_goodsDTMap[sctype].end() ; ++iter)
		//{
		//	if(vecgd[i].type == (*iter).type && vecgd[i].index == (*iter).index)
		//		vecGD.push_back((*iter));
		//}
	}
}
/////////////////////////////////////////////////
// zhaohang  2010/7/30 
// ���ݸ�������Ʒ���ֽṹ��������Ʒ��ϸ����
void SCGData::GetTop10InGoodsData( eSCType compType,VecGD& vecGD)
{
	MapTop10A& ref = GetTop10List();
	VecGDPTA& vecgd = ref[compType];
	for(size_t i = 0 ; i < vecgd.size() ; ++i)
	{
		tagGD* gd = GetGoodsData((eSType)vecgd[i].type,vecgd[i].index,compType);
		if(gd)
			vecGD.push_back(*gd);
		//for(VecGD::iterator iter = m_goodsDTMap[compType].begin() ; iter != m_goodsDTMap[compType].end() ;  ++iter)
		//{
		//	if(vecgd[i].type == (*iter).type && vecgd[i].index == (*iter).index)
		//		vecGD.push_back(*iter);
		//}
	}
}
/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// ������Ʒ���ͣ�ID���̵����ͻ�ȡһ����Ʒ���ݽṹ
////////////////////////////////////////////////
SCGData::tagGD* SCGData::GetGoodsData(eSType goodsType,uint index,eSCType shoptype)
{
	//����Ƿ�Ϸ�
	if(goodsType >=TABTYPE_HOT && goodsType < TABTYPE_NUM && shoptype >= ST_XINGZUAN && shoptype < ST_NUM)
	{
		VecGD* vecGDT = &m_goodsDTMap[shoptype];
		VecGD::iterator iter = vecGDT->begin();
		for(; iter != vecGDT->end() ; ++iter)
		{
			if(goodsType == (*iter).type && index == (*iter).index)
				return &(*iter);
		}
	}
	return NULL;
}


////////////////////////////////////////////////
// zhaohang  2010/7/29 
// ���ض�Ӧ�̳��̵������µ���Ʒ����
////////////////////////////////////////////////
void SCGData::GetVecGoodsData(eSCType scType,eSType sType,VecGD& vecGD)
{
	VecGD::iterator iter = m_goodsDTMap[scType].begin();
	for( ; iter != m_goodsDTMap[scType].end() ; ++iter)
	{
		tagGD& gd = (*iter);
		if(gd.type == sType)
			vecGD.push_back(gd);
	}
}

/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// �������ݸ���
////////////////////////////////////////////////
void SCGData::Update(CMessage* pMsg)
{
	BYTE flag = pMsg->GetByte();			//���±�ʶ��0:�޹�1:������
	BYTE shopType = pMsg->GetByte();		//�̵�����
	uint type = pMsg->GetDWord();			//��Ʒ����
	uint index = pMsg->GetDWord();			//���ID
	uint num = pMsg->GetDWord();			//ʣ��ɹ�����Ŀ
	tagGD* gdt = GetGoodsData((eSType)type,index,(eSCType)shopType);
	if(!gdt)
	{
		if(flag == 0)
			gdt->buylimit;//�޹�
		else
			gdt->numlimit;//����
	}
}

/////////////////////////////////////////////////
// zhaohang  2010/7/20 
// ShopCityMsgMgr
////////////////////////////////////////////////
int ShopCityMsgMgr::arrPGInfo[3][4] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};
uint ShopCityMsgMgr::stateUpdateUIbyType = 0;

ShopCityMsgMgr::ShopCityMsgMgr()
{
	dt = new SCGData();
}

ShopCityMsgMgr::~ShopCityMsgMgr()
{
	SAFEDELETE(dt);
}
void ShopCityMsgMgr::SwitchPage()
{
	if(IsPageOpen)
		Send_PageClose();
	else
		Send_PageOpen();
}
/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// ��Ϣ���Ͳ���
////////////////////////////////////////////////
void ShopCityMsgMgr::Send_PageOpen()
{
	//�ж�����Ƿ�����������������
	if(!IsPlayerDied())
	{
		CMessage msg(MSG_C2S_INCREMENTSHOP_OPEN);
		msg.Send();
	}
}
void ShopCityMsgMgr::Send_PageClose()
{
	FireUIEvent(SHOPCITY_PAGE_NAME,EVENT_CLOSE);
}

void ShopCityMsgMgr::Send_GetBuyLog()
{
	if(!IsPlayerDied())
	{
		CMessage msg(MSG_C2S_INCREMENTSHOP_GETLOG_REQUEST);
		msg.Send();
	}
}
void ShopCityMsgMgr::Send_GetAffiche()
{
	if(!IsPlayerDied())
	{
		CMessage msg(MSG_C2S_INCREMENTSHOP_AFFICHE_REQUEST);
		msg.Send();
	}
}

void ShopCityMsgMgr::Send_CloseShop(void)
{
	if(!IsPlayerDied())
	{
		CMessage msg(MSG_C2S_INCREMENTSHOP_CLOSE);
		msg.Send();
		IsPageOpen = false;
	}
}

void ShopCityMsgMgr::Send_BuyQuery(uint goodsID /* = 0 */,uint buyNum /* = 0 */,uint buyMode /* = 0 */)
{
	if(!IsPlayerDied())
	{
		CMessage msg(MSG_C2S_INCREMENTSHOP_BUY);
		msg.Add((DWORD)goodsID);
		msg.Add((DWORD)buyNum);
		msg.Add((DWORD)buyMode);
		msg.Send();
	}
}
/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// ��Ϣ���ղ���
////////////////////////////////////////////////
void ShopCityMsgMgr::Receive_AfficheResponse(CMessage* pMsg)
{
	long length = pMsg->GetLong();
	char* str = new char[length];
	pMsg->GetEx((void*)str,length);
	Affiche = str;
	SAFEDELETE(str);
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_AFFICHERESPONSE);
}
void ShopCityMsgMgr::Receive_BuyResult(CMessage* pMsg)
{
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_BUYRESULT);
}

void ShopCityMsgMgr::Receive_LogResponse(CMessage* pMsg)
{
	if(dt)
		dt->DecodeBuyLogDataBlock(pMsg);
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_LOGRESPONSE);
}
void ShopCityMsgMgr::Receive_PageOpen(CMessage *pMsg)
{
	if(GetGame()->GetMainPlayer() && GetGame()->GetGameControl())
	{
		GetGame()->GetGameControl()->AutoAttactStop();//ֹͣ�Զ�����
		GetGame()->GetGameControl()->AutoMoveStop();//ֹͣ�Զ��ƶ�
		GetGame()->GetGameControl()->StopMove();//ֹͣ�ƶ�
	}
	BYTE bFlag = pMsg->GetByte();//�����������Ƿ�򿪳ɹ�
	if(bFlag == 1)
	{
		IsPageOpen = true;
		if(dt)//��Ϊ��,�������ݽ���
			dt->DecodeFromDataBlock(pMsg);
		//�������֪ͨ�򿪽���
		FireUIEvent(SHOPCITY_PAGE_NAME,EVENT_OPEN);
		SetPageState(true);
	}
	else
	{
		FireUIEvent(SHOPCITY_PAGE_NAME,EVENT_OPEN);
		SetPageState(true);
	}
}
void ShopCityMsgMgr::Receive_PageClose(CMessage* pMsg)
{
	FireUIEvent(SHOPCITY_PAGE_NAME,EVENT_CLOSE);
	SetPageState(false);
}

void ShopCityMsgMgr::Receive_Updata(CMessage* pMsg)
{
	if(dt)
		dt->Update(pMsg);
	FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATA);
}

/////////////////////////////////////////////////
// zhaohang  2010/7/21 
// �����ӿ�
////////////////////////////////////////////////
bool ShopCityMsgMgr::IsPlayerDied()
{
	CPlayer* player = GetGame()->GetMainPlayer();
	if(player && !player->IsDied())
	{
		return false;
	}
	else
	{
		DoMsgBoxWhenDied();
		return true;
	}
}

void ShopCityMsgMgr::DoMsgBoxWhenDied()
{
    const char * pszStr = pszStr = AppFrame::GetText("TsShop_13");
	/***********************************************************************/
	/* zhaohang fix 2010-9-3
	/* �޸���Ϣ����ʽ�����÷�װ��MsgEventManager,�Լ��µ��ַ���Դ���ط�ʽAppframe
	/***********************************************************************/
	GetInst(MsgEventManager).PushEvent(Msg_Ok,pszStr);
}

const char* ShopCityMsgMgr::GetAffiche()
{
	return Affiche.c_str();
}

uint ShopCityMsgMgr::GetPlayerDianQuan()
{
	CPlayer* player = GetGame()->GetMainPlayer();
	if(player)
		return player->GetJiFen();
	else
		return 0;
}

uint ShopCityMsgMgr::GetPlayerWeiMian()
{
	CPlayer* player = GetGame()->GetMainPlayer();
	if(player)
		return player->GetGoodsAmount(CGlobeSetup::GetSetup()->strWeiMianName);
	else
		return 0;
}

uint ShopCityMsgMgr::GetPlayerXinZuan()
{
	CPlayer* player = GetGame()->GetMainPlayer();
	if(player)
		return player->GetYuanBao();
	else
		return 0;
}

void ShopCityMsgMgr::SetPageState(bool IsOpen)
{
	IsPageOpen = IsOpen;
}

int ShopCityMsgMgr::GetPageInfo(int ShopCityIdx,int ShopIndex)
{
	return arrPGInfo[ShopCityIdx][ShopIndex];
}

void ShopCityMsgMgr::SetPageInfo(int ShopCityIdx,int ShopIndex,int InPage)
{
	arrPGInfo[ShopCityIdx][ShopIndex] = InPage;
}
