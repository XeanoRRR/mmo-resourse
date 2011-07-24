/*****************************************************************************************
//	������Core�ӿڷ���
//	Copyright : Kingsoft 2002
//	Author	:   
//	CreateTime:	2002-9-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KCore.h"
#include "GameDataDef.h"
#include "CoreShell.h"
#ifndef _SERVER
#include "CoreDrawGameObj.h"
#include "ImgRef.h"
#endif
#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KItemList.h"
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"
#ifndef _SERVER
#include "KNpcResList.h"
#include "Scene/KScenePlaceC.h"
#include "kskills.h"
#include "GameDataDef.h"
#include "MsgGenreDef.h"
#include "KOption.h"
#include "KSubWorld.h"
#endif
#include "KBuySell.h"

#define	NPC_TRADE_BOX_WIDTH		6
#define	NPC_TRADE_BOX_HEIGHT	10
#define	MAX_TRADE_ITEM_WIDTH	2
#define	MAX_TRADE_ITEM_HEIGHT	4
IClientCallback* l_pDataChangedNotifyFunc = 0;

class KCoreShell : public iCoreShell
{
public:
	int	 GetProtocolSize(BYTE byProtocol);
	int	 Debug(unsigned int uDataId, unsigned int uParam, int nParam);
	int	 OperationRequest(unsigned int uOper, unsigned int uParam, int nParam);
	void ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);
	int	 FindSelectPlayer(int x, int y, void* pReturn);
	int FindSpecialPlayer(char* Name, void* pReturn);
	int ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength);
	int	 GetGameData(unsigned int uDataId, unsigned int uParam, int nParam);
	int  FreeGameData(unsigned int uDataId, unsigned int uParam, int nParam);
	void DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam);
	void DrawGameSpace();
	DWORD GetPing();
	void SendPing();
	int	 SetCallDataChangedNofify(IClientCallback* pNotifyFunc);
	void NetMsgCallbackFunc(void* pMsgData);
	void SetRepresentShell(struct iRepresentShell* pRepresent);
	void SetMusicInterface(void* pMusicInterface);
	void SetRepresentAreaSize(int nWidth, int nHeight);
	int  Breathe();
	void Release();	//�ͷŽӿڶ���
	void SetClient(LPVOID pClient);
	void SendNewDataToServer(void* pData, int nLength);
};

static KCoreShell	g_CoreShell;

CORE_API void g_InitCore();
#ifndef _STANDALONE
extern "C" __declspec(dllexport)
#endif
iCoreShell* CoreGetShell()
{
	g_InitCore();
	return &g_CoreShell;
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϸ�������ݸı��֪ͨ����
//	���أ���δ��ע��֪ͨ��������ֱ�ӷ���0�����򷵻�֪ͨ����ִ�н����
//--------------------------------------------------------------------------
void CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam)
{
	if (l_pDataChangedNotifyFunc)
		l_pDataChangedNotifyFunc->CoreDataChanged(uDataId, uParam, nParam);
}


void KCoreShell::Release()
{
	g_ReleaseCore();
}

//--------------------------------------------------------------------------
//	���ܣ���������ɴ���������Ϣ
//--------------------------------------------------------------------------
void KCoreShell::NetMsgCallbackFunc(void* pMsgData)
{
#ifndef _SERVER
	g_ProtocolProcess.ProcessNetMsg((BYTE *)pMsgData);
#endif
}
//--------------------------------------------------------------------------
//	���ܣ�������Ϸ�������ݸı��֪ͨ����
//	������fnCoreDataChangedCallback pNotifyFunc --> ֪ͨ������ָ�롣
//	���أ�����ֵΪ��0ֵ��ʾע��ɹ��������ʾʧ�ܡ�
//--------------------------------------------------------------------------
int	KCoreShell::SetCallDataChangedNofify(IClientCallback* pNotifyFunc)
{
	l_pDataChangedNotifyFunc = pNotifyFunc;
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ�����Ϸ�����ȡ����
//	������unsigned int uDataId --> ��ʾ��ȡ��Ϸ���ݵ�������������������ֵΪ÷������
//							GAMEDATA_INDEX��ȡֵ֮һ��
//		  unsigned int uParam  --> ����uDataId��ȡֵ�������
//		  int nParam --> ����uDataId��ȡֵ�������
//	���أ�����uDataId��ȡֵ���������
//--------------------------------------------------------------------------
int	KCoreShell::GetGameData(unsigned int uDataId, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uDataId)
	{
	case GDI_PLAYER_IS_MALE:
		nRet = (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID);
		break;
	case GDI_TRADE_ITEM_PRICE:
		if (uParam)
		{
			KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
			KItem*	pItem = NULL;

			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						pItem = &Item[pObj->Obj.uId];
					}
				}
				break;
			case CGOG_NPCSELLITEM:
				{
					int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
					if (nBuyIdx != -1)
					{
						int nIndex = BuySell.GetItemIndex(nBuyIdx, pObj->Obj.uId);
						if (nIndex >= 0)
							pItem = BuySell.GetItem(nIndex);
					}
				}
				break;
			default:
				break;
			}

			if (!pItem)
				break;

			KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
		
			if (pObj->eContainer == UOC_NPC_SHOP)
				pInfo->nPrice = pItem->GetPrice();
			else
				pInfo->nPrice = pItem->GetPrice() / BUY_SELL_SCALE;
			strcpy(pInfo->szItemName, pItem->GetName());
			nRet = (pItem->GetGenre() != item_task);
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
	//��Ϸ��������˵���ı���
	//uParam = (KUiGameObject*) ������Ϸ����Ľṹ���ݵ�ָ��
	//nParam = (char*) ָ��һ����������ָ�룬��ռ䲻����256�ֽڡ�
	case GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO:
#ifndef _SERVER
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
			char* pszDescript = (char *)nParam;
			pszDescript[0] = 0;
			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						int nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId);
						Item[pObj->Obj.uId].GetDesc(pszDescript, true, BUY_SELL_SCALE, nActive);
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript, true, BUY_SELL_SCALE);
				}
				break;			
			case CGOG_NPCSELLITEM:
				{
					int nIdx = -1;
					if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
						break;
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->Obj.uId);

					KItem* pItem = NULL;
					if (nIdx < 0)
						break;
					pItem = BuySell.GetItem(nIdx);

					if (!pItem)
						break;
					pItem->GetDesc(pszDescript, true);
				}
				break;
			}
		}
#endif
		break;
	case GDI_GAME_OBJ_DESC:
#ifndef _SERVER
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
			char* pszDescript = (char *)nParam;
			pszDescript[0] = 0;
			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						int nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId);
						Item[pObj->Obj.uId].GetDesc(pszDescript, false, 1, nActive);
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript);
				}
				break;
			case CGOG_SKILL:
			case CGOG_SKILL_FIGHT:
			case CGOG_SKILL_LIVE:
			case CGOG_SKILL_SHORTCUT:
				{
					int nLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(pObj->Obj.uId);
					_ASSERT(nLevel >= 0);
					if (pObj->Obj.uId >0)
					{
						
						KSkill::GetDesc(
							pObj->Obj.uId,
							nLevel,
							pszDescript,
							Player[CLIENT_PLAYER_INDEX].m_nIndex,
							(pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT)?false:true
							);
					}
					
				}
				break;
			
			case CGOG_PLAYER_FACE:
				break;
			case CGOG_NPCSELLITEM:
				{
					int nIdx = -1;
					if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
						break;
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->Obj.uId);

					KItem* pItem = NULL;
					if (nIdx < 0)
						break;
					pItem = BuySell.GetItem(nIdx);

					if (!pItem)
						break;
					pItem->GetDesc(pszDescript);
				}
				break;
			}
		}
		break;
#endif
	//���ǵ�һЩ���ױ������
	//uParam = (KUiPlayerBaseInfo*)pInfo	
	case GDI_PLAYER_BASE_INFO:
#ifndef _SERVER
		if (uParam)
		{
			KUiPlayerBaseInfo* pInfo = (KUiPlayerBaseInfo*)uParam;
			strcpy(pInfo->Name,Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			//to do:no implements in this version
			pInfo->Agname[0] = 0;
			pInfo->Title[0] = 0;
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_btRankId)
			{
				char szRankId[5];
				itoa(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_btRankId, szRankId, 10);
				g_RankTabSetting.GetString(szRankId, "RANKSTR", "", pInfo->Title, 32);
			}
			pInfo->nCurFaction = Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction;
		}
#endif
		break;

	//���ǵ�һЩ�ױ������
	//uParam = (KUiPlayerRuntimeInfo*)pInfo
	case GDI_PLAYER_RT_INFO:
		if (uParam)
		{
			KUiPlayerRuntimeInfo* pInfo = (KUiPlayerRuntimeInfo*)uParam;
			pInfo->nLifeFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax;			//������ֵ
			pInfo->nLife = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife;					//����
			pInfo->nManaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax;			//������ֵ
			pInfo->nMana = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana;					//����
			pInfo->nStaminaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax;		//������ֵ
			pInfo->nStamina = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina;				//����

			pInfo->nAngryFull = 0;													//ŭ��ֵ
			pInfo->nAngry = 0;														//ŭ
			
			pInfo->nExperienceFull = Player[CLIENT_PLAYER_INDEX].m_dwNextLevelExp;					//������ֵ
			pInfo->nExperience = Player[CLIENT_PLAYER_INDEX].m_dwExp;									//����
			pInfo->nCurLevelExperience = Player[CLIENT_PLAYER_INDEX].m_dwNextLevelExp - PlayerSet.m_cLevelAdd.GetLevelExp(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level - 1);
			
			pInfo->byActionDisable = 0;
			//to do	��pInfo->bActionDisable��������ʵ�ֵ

			pInfo->byAction = PA_NONE;
#ifndef _SERVER
			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
				pInfo->byAction |= PA_RUN;
#endif
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
				pInfo->byAction |= PA_SIT;
			pInfo->wReserved = 0;
		}
		break;

	//���ǵ�һЩ�ױ����������
	//uParam = (KUiPlayerAttribute*)pInfo
	case GDI_PLAYER_RT_ATTRIBUTE:
#ifndef _SERVER
		if (uParam)
		{
			KUiPlayerAttribute* pInfo = (KUiPlayerAttribute*)uParam;
			KNpc*	pNpc = &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex];
			pInfo->nMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);				//����
			pInfo->nBARemainPoint = Player[CLIENT_PLAYER_INDEX].m_nAttributePoint;						//��������ʣ�����
			pInfo->nStrength = Player[CLIENT_PLAYER_INDEX].m_nCurStrength;								//����
			pInfo->nDexterity = Player[CLIENT_PLAYER_INDEX].m_nCurDexterity;								//����
			pInfo->nVitality = Player[CLIENT_PLAYER_INDEX].m_nCurVitality;								//����
			pInfo->nEnergy = Player[CLIENT_PLAYER_INDEX].m_nCurEngergy;									//����

			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nKillMIN, &pInfo->nKillMAX, 0);				//�����Сɱ����
			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nRightKillMin , &pInfo->nRightKillMax, 1);
			pInfo->nAttack = pNpc->m_CurrentAttackRating;				//������
			pInfo->nDefence = pNpc->m_CurrentDefend;					//������
			pInfo->nMoveSpeed = pNpc->m_CurrentRunSpeed;				//�ƶ��ٶ�
			pInfo->nAttackSpeed = pNpc->m_CurrentAttackSpeed;			//�����ٶ�
			//�������
			if (pNpc->m_CurrentPhysicsResistMax >= pNpc->m_CurrentPhysicsResist)
				pInfo->nPhyDef = pNpc->m_CurrentPhysicsResist;
			else
				pInfo->nPhyDef = pNpc->m_CurrentPhysicsResistMax;
			//��������
			if (pNpc->m_CurrentColdResistMax >= pNpc->m_CurrentColdResist)
				pInfo->nCoolDef = pNpc->m_CurrentColdResist;
			else
				pInfo->nCoolDef = pNpc->m_CurrentColdResistMax;
			//�������
			if (pNpc->m_CurrentLightResistMax >= pNpc->m_CurrentLightResist)
				pInfo->nLightDef = pNpc->m_CurrentLightResist;
			else
				pInfo->nLightDef = pNpc->m_CurrentLightResistMax;
			//�������
			if (pNpc->m_CurrentFireResistMax >= pNpc->m_CurrentFireResist)
				pInfo->nFireDef = pNpc->m_CurrentFireResist;
			else
				pInfo->nFireDef = pNpc->m_CurrentFireResistMax;
			//���ط���
			if (pNpc->m_CurrentPoisonResistMax >= pNpc->m_CurrentPoisonResist)
				pInfo->nPoisonDef = pNpc->m_CurrentPoisonResist;
			else
				pInfo->nPoisonDef = pNpc->m_CurrentPoisonResistMax;
			
			pInfo->nLevel = pNpc->m_Level;

			// �������״̬��ʾ ״̬���� not end ��Ӧ����״̬��Ӧ������������ spe
			memset(pInfo->StatusDesc, 0, sizeof(pInfo->StatusDesc));
			switch(pNpc->m_Series)
			{
			case series_water:
				strcpy(pInfo->StatusDesc, "ˮϵ");
				break;
			case series_wood:
				strcpy(pInfo->StatusDesc, "ľϵ");
				break;
			case series_metal:
				strcpy(pInfo->StatusDesc, "��ϵ");
				break;
			case series_fire:
				strcpy(pInfo->StatusDesc, "��ϵ");
				break;
			case series_earth:
				strcpy(pInfo->StatusDesc, "��ϵ");
				break;
			}
		}
		break;

	//���ǵ�����ʹ����Ʒ���书
	//uParam = (KUiPlayerImmedItemSkill*)pInfo
#endif
	case GDI_PLAYER_IMMED_ITEMSKILL:
#ifndef _SERVER
		if (uParam)
		{
			KUiPlayerImmedItemSkill* pInfo = (KUiPlayerImmedItemSkill*)uParam;
			memset(pInfo,0,sizeof(KUiPlayerImmedItemSkill));
			pInfo->IMmediaSkill[0].uGenre	= CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[0].uId		= Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
			pInfo->IMmediaSkill[1].uGenre	= CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[1].uId		= Player[CLIENT_PLAYER_INDEX].GetRightSkill();

			for (int i = 0; i < MAX_IMMEDIACY_ITEM; i++)
			{
				pInfo->ImmediaItem[i].uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_immediacy].FindItem(i, 0);
				if (pInfo->ImmediaItem[i].uId > 0)
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_ITEM;
				}
				else
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_NOTHING;
				}
			}
		}
		break;

	//��������Я����Ǯ
	//nRet = ��������Я����Ǯ
#endif
	case GDI_PLAYER_HOLD_MONEY:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);
		break;
	//��������Я������Ʒ��
	//uParam = (KUiObjAtRegion*) pInfo -> KUiObjAtRegion�ṹ�����ָ�룬KUiObjAtRegion
	//				�ṹ���ڴ洢��Ʒ�����书�����ݼ����������λ����Ϣ��
	//nParam = pInfo�����а���KUiObjAtRegion�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpInfo�����е�ǰ���ٸ�KUiObjAtRegion
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiObjAtRegion�ṹ������
	//			�Ź��洢ȫ��������Я������Ʒ��Ϣ��

	// flying�����һ���Ƿ�������������������ʵ�֣�gg����һ�ޣ�mm���KFCŶ
	case GDI_PLAYER_CAN_RIDE:
		// ʵ�ַ�֧�Ĵ������������
		break;

	case GDI_ITEM_TAKEN_WITH:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproom)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproom)
					nCount++;
			}
			nRet = nCount;
		}
		break;

	//����װ����Ʒ
	//uParam = (KUiObjAtRegion*)pInfo -> ����10��Ԫ�ص�KUiObjAtRegion�ṹ����ָ�룬
	//			KUiObjAtRegion�ṹ���ڴ洢װ�������ݺͷ���λ����Ϣ��
	//			KUiObjAtRegion::Region::h ��ʾ���ڵڼ���װ��
	//			KUiObjAtRegion::Region::v ��ʾ�����ĸ�λ�õ�װ��,��ֵΪ÷������
	//			UI_EQUIPMENT_POSITION��ȡֵ֮һ����ο�UI_EQUIPMENT_POSITION��ע�͡�
	//nParam =	Ҫ��ȡ���ǵڼ���װ����Ϣ
	//Return =  ��ֵ��ʾpInfo�����е�ǰ���ٸ�KUiObjAtRegion�ṹ���������Ч�����ݡ�
	case GDI_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			// TODO����ʱû�����ڶ���װ��
			if (nParam == 1)
				break;

			int PartConvert[itempart_num] = 
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,
			};

			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (int i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
				
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;

	//���ײ�����ص�����
	//uParam = (UI_TRADE_OPER_DATA)eOper ���庬���UI_TRADE_OPER_DATA
	//nParam ����Ӧ���뺬����uParam��ȡֵ״������,��UI_TRADE_OPER_DATA��˵��
	//Return ���庬����uParam��ȡֵ״������,��UI_TRADE_OPER_DATA��˵��
	//���ǵ����������
	case GDI_TRADE_OPER_DATA:
#ifndef _SERVER
		if (uParam == UTOD_IS_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock;
		else if (uParam == UTOD_IS_TRADING)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState;
		else if (uParam == UTOD_IS_OTHER_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock;
		else if (uParam == UTOD_IS_WILLING)
		{
			nRet = (Player[CLIENT_PLAYER_INDEX].m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN);
		}
#endif
		break;

	//uParam = (KUiPlayerLiveSkillBase*) pInfo -> ���ǵ����������
	case GDI_LIVE_SKILL_BASE:
		if (uParam)
		{
			KUiPlayerLiveSkillBase* pInfo = (KUiPlayerLiveSkillBase*)uParam;
			//to do:no implements in this version;
			pInfo->nLiveExperience = 0 ;
			pInfo->nRemainPoint = 0 ;
			pInfo->nLiveExperienceFull = 0 ;
			memset(pInfo,0,sizeof(KUiPlayerLiveSkillBase));
		}
		break;

	//�������յĸ��������
	//uParam = (unsigned int*) pSkills -> ����10��unsigned int���������ڴ洢��������ܵ�id��
	case GDI_LIVE_SKILLS:
		if (uParam)
		{
			//to do:no implements in this version;
			KUiSkillData* pInfo = (KUiSkillData*)uParam;
			memset(pInfo,0,sizeof(KUiSkillData)*10);
		}
		break;

	//ʣ��ս�����ܵ���
	//Return = ʣ��ս�����ܵ���
	case GDI_FIGHT_SKILL_POINT:
		//to do:no implements in this version;
		nRet = Player[CLIENT_PLAYER_INDEX].m_nSkillPoint;
		break;

	//�������յĸ���ս������
	//uParam = (unsigned int*) pSkills -> ����30��unsigned int���������ڴ洢����ս�����ܵ�id��
	case GDI_FIGHT_SKILLS:
#ifndef _SERVER
		if (uParam)
		{
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillSortList(pSkills);
		}
#endif
		break;
	//��ʾ��������б�
	//uParam = (KUiSkillData*) pSkills -> ����65��KUiSkillData���������ڴ洢�����ܵ����ݡ�
	//								KUiSkillData::nLevel��������������ʾ�ڵڼ���
	//Return = ������Ч���ݵ�Skills����Ŀ
	case GDI_LEFT_ENABLE_SKILLS:
#ifndef _SERVER
		{
			KUiSkillData * pSkills = (KUiSkillData*)uParam;
			int nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLeftSkillSortList(pSkills);
			return nCount;

		}
#endif
		break;
	//��ʾ�Ҽ������б�
	//uParam = (KUiSkillData*) pSkills -> ����65��KUiSkillData���������ڴ洢�����ܵ����ݡ�
	//								KUiSkillData::nLevel��������������ʾ�ڵڼ���
	//Return = ������Ч���ݵ�Skills����Ŀ
	case GDI_RIGHT_ENABLE_SKILLS:
#ifndef _SERVER
		{
			KUiSkillData * pSkills = (KUiSkillData*)uParam;
			int nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetRightSkillSortList(pSkills);
			return nCount;
		}
#endif
		break;
	//���ǵ��Դ��书
	//uParam = (unsigned int*) pSkills -> ����5��unsigned int���������ڴ洢�����Դ��书��id��
	case GDI_CUSTOM_SKILLS:
		if (uParam)
		{
			//to do: no implements in this version;
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			memset(pSkills,0,sizeof(KUiSkillData)*5);
		}
		break;

	//�������ڵĶ�����Ϣ
	//uParam = (KUiPlayerTeam*)pTeam -> ������Ϣ
	//nParam = �ӳ�id,�����ֵΪ0��ʾ��ȡ�����Լ����ڵĶ���
	//Return = pTeam�ṹ�Ƿ������Ϣ��
	case GDI_TEAM_INFO:
#ifndef _SERVER
		if (uParam)
		{
			//to do: waiting for... ;
			KUiPlayerTeam* pTeam = (KUiPlayerTeam*)uParam;
			if (nParam == 0)
				nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetInfo(pTeam);
			else
				;
		}
#endif
		break;

	//��ȡ�������ڶ����Ա�б�
	//uParam = (KUiPlayerItem*)pList -> ��Ա��Ϣ�б�
	//nParam = pList�����а���KUiPlayerItem�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpList�����е�ǰ���ٸ�KUiPlayerItem
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiPlayerItem�ṹ������
	//			�Ź��洢ȫ���ĳ�Ա��Ϣ��
	case GDI_TEAM_MEMBER_LIST:
#ifndef _SERVER
		nRet = g_Team[0].GetMemberInfo((KUiPlayerItem *)uParam, nParam);
#endif
		break;

	case GDI_TEAM_REFUSE_INVITE_STATUS:
#ifndef _SERVER
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetAutoRefuseState();
#endif
		break;

	//����루������ڼң������������ҵ��б�
	//uParam = (KUiPlayerItem*)pPlayerList -> KUiPlayerItem�ṹ�����ָ�룬KUiPlayerItem
	//				�ṹ����Ԥ�����������ҵ���Ϣ��
	//nParam = pPlayerList�����а���KUiPlayerItem�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpPlayerList�����е�ǰ���ٸ�Ԫ�ر���
	//			������Ч�����ݣ������ʾ��Ҫ����������ٸ�Ԫ�ص�����Ź��洢ȫ������Ϣ��
	case GDI_TEAM_TOJOIN_LIST:
#ifndef _SERVER
		if (uParam)
		{
			int nCount = 0;
			for(int i = 0; i < MAX_TEAM_APPLY_LIST; i++)
			{
				if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].m_dwNpcID > 0)
					nCount++;				
			}

			if (nParam >= nCount)
			{
				KUiPlayerItem* pPlayerList = (KUiPlayerItem*)uParam;
				int nPos = 0; 
				for(int i = 0;i < MAX_TEAM_APPLY_LIST;i++)
				{
					 if	(Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].m_dwNpcID > 0)
					 {
						 pPlayerList[nPos].uId = Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].m_dwNpcID;						 
						 pPlayerList[nPos].nIndex = 0;
						 strcpy(pPlayerList[nPos].Name,Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].m_szName);
						 nPos++;
					 }
				}
			}	
			nRet = nCount ;
		}
#endif
		break;

	//��ȡ��Χ��ҵ��б�
	//uParam = (KUiPlayerItem*)pList -> ��Ա��Ϣ�б�
	//			KUiPlayerItem::nData = 0
	//nParam = pList�����а���KUiPlayerItem�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpList�����е�ǰ���ٸ�KUiPlayerItem
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiPlayerItem�ṹ������
	//			�Ź��洢ȫ����Ա��Ϣ��
	case GDI_NEARBY_PLAYER_LIST:
#ifndef _SERVER
		nRet = NpcSet.GetAroundNotFriendPlayer((KUiPlayerItem*)uParam, nParam);
#endif
		break;

	//��ȡ��Χ�µ������������ҵ��б�
	//��������ͬGDI_NEARBY_PLAYER_LIST
	case GDI_NEARBY_IDLE_PLAYER_LIST:
#ifndef _SERVER
		nRet = NpcSet.GetAroundPlayerForTeamInvite((KUiPlayerItem*)uParam, nParam);
#endif
		break;

	//����ͳ˧������ص�����
	//uParam = (KUiPlayerLeaderShip*) -> ����ͳ˧������ص����ݽṹָ��
	case GDI_PLAYER_LEADERSHIP:
#ifndef _SERVER
		if (uParam)
		{
			KUiPlayerLeaderShip* pInfo = (KUiPlayerLeaderShip*)uParam ;
			pInfo->nLeaderShipExperience = Player[CLIENT_PLAYER_INDEX].m_dwLeadExp ;		//ͳ˧������ֵ
			//to do: waiting for...;
			pInfo->nLeaderShipExperienceFull = Player[CLIENT_PLAYER_INDEX].m_dwNextLevelExp;	//ͳ˧������ֵ
			pInfo->nLeaderShipLevel = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel ;			//ͳ˧���ȼ�
		}
#endif
		break;
	case GDI_NPC_RES:
#ifndef _SERVER
		if (uParam)
		{
			RoleBaseInfo* pInfo = (RoleBaseInfo *)uParam;
			KNpcRes*	pRes = NULL;
			
			pRes = new KNpcRes;
			if (pRes)
			{
				char szName[32];
				if (pInfo->Sex)
					strcpy(szName, "Ů����");
				else
					strcpy(szName, "������");
				pRes->Init(szName, &g_NpcResList);
				pRes->SetAction(cdo_fightstand);
//				pRes->SetArmor(pInfo->ArmorType);
//				pRes->SetHelm(pInfo->HelmType);
//				pRes->SetWeapon(pInfo->WeaponType);
				pRes->SetRideHorse(FALSE);
			}
			nRet = (int)pRes;
		}
#endif
		break;
	//�����Ʒ��ĳ������λ�õ�����״̬
	//uParam = (KUiGameObject*)pObj����nParam==0ʱ����Ʒ����Ϣ
	//uParam = (KUiObjAtContRegion*)pObj����nParam!=0ʱ����Ʒ����Ϣ
	//nParam = (int)(bool)bJustTry  �Ƿ�ֻ�ǳ��Է���
	//Return = (ITEM_IN_ENVIRO_PROP)eProp ��Ʒ������״̬
	case GDI_ITEM_IN_ENVIRO_PROP:
#ifndef _SERVER
		{
			if (!nParam)
			{
				KUiGameObject *pObj = (KUiGameObject *)uParam;
				if (pObj->uGenre != CGOG_ITEM && pObj->uGenre != CGOG_NPCSELLITEM)
					break;

				KItem* pItem = NULL;

				if (pObj->uGenre == CGOG_ITEM && pObj->uId > 0 && pObj->uId < MAX_ITEM)
				{
					pItem = &Item[pObj->uId];
				}
				else if (pObj->uGenre == CGOG_NPCSELLITEM)
				{
					int nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->uId);
					pItem = BuySell.GetItem(nIdx);
				}

				_ASSERT(pItem);
				if (!pItem || pItem->GetGenre() != item_equip)
					break;

				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pItem))
				{
					nRet = IIEP_NORMAL;
				}
				else
				{
					nRet = IIEP_NOT_USEABLE;
				}
			}
			else
			{
				KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
				if (pObj->Obj.uGenre != CGOG_ITEM || pObj->Obj.uId >= MAX_ITEM)
					break;

				int PartConvert[itempart_num] = 
				{ 
					itempart_head,		itempart_weapon,
					itempart_amulet,	itempart_cuff,
					itempart_body,		itempart_belt,
					itempart_ring1,		itempart_ring2,
					itempart_pendant,	itempart_foot,
					itempart_horse,
				};

				_ASSERT(pObj->eContainer < itempart_num);
				if (pObj->eContainer >= itempart_num || pObj->eContainer < 0)
					break;

				if (Item[pObj->Obj.uId].GetGenre() != item_equip)
					break;

				int nPlace = PartConvert[pObj->eContainer];

				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pObj->Obj.uId, nPlace))
				{
					nRet = IIEP_NORMAL;
				}
				else
				{
					nRet = IIEP_NOT_USEABLE;
				}
			}
		}
#endif
		break;
	//��NPC������
	//uParam = (KUiObjAtContRegion*) pInfo -> KUiObjAtRegion�ṹ�����ָ�룬KUiObjAtRegion
	//				�ṹ���ڴ洢��Ʒ�����ݼ����������λ����Ϣ��
	//				����KUiObjAtContRegion::nContainerֵ��ʾ�ڼ�ҳ����Ʒ
	//nParam = pInfo�����а���KUiObjAtRegion�ṹ����Ŀ
	//Return = �������ֵС�ڵ��ڴ������nParam����ֵ��ʾpInfo�����е�ǰ���ٸ�KUiObjAtRegion
	//			�ṹ���������Ч�����ݣ������ʾ��Ҫ����������ٸ�KUiObjAtRegion�ṹ������
	//			�Ź��洢ȫ����npc�г������׵���Ʒ��Ϣ��
	case GDI_TRADE_NPC_ITEM:
#ifndef _SERVER
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			int nPage = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion *)uParam;

			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			if (!BuySell.m_pShopRoom)
				break;
			BuySell.m_pShopRoom->Clear();
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					// Set pInfo->Obj.uGenre
					pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
					// Set pInfo->Obj.uId
					pInfo->Obj.uId = i;

					POINT	Pos;
					if (BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos))
					{
						// nIndex + 1��֤��Ϊ0
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					else
					{
						nPage++;
						BuySell.m_pShopRoom->Clear();
						// Clear��ɺ��Ȼ�ɹ�������û���ж�
						BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos);
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					pInfo->Region.h = Pos.x;
					pInfo->Region.v = Pos.y;
					pInfo->Region.Width = pItem->GetWidth();
					pInfo->Region.Height = pItem->GetHeight();
					pInfo->nContainer = nPage;
					nCount++;
					pInfo++;
				}
			}			
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
#endif
		break;
	case GDI_CHAT_SEND_CHANNEL_LIST:
#ifndef _SERVER
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.SendSelectChannelList((KUiChatChannel*)uParam, nParam);
#endif
		break;
	case GDI_CHAT_RECEIVE_CHANNEL_LIST:
#ifndef _SERVER
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.SendTakeChannelList((KUiChatChannel*)uParam, nParam);
#endif
		break;
	case GDI_CHAT_CURRENT_SEND_CHANNEL:
#ifndef _SERVER
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.GetCurChannel((KUiChatChannel*)uParam);
#endif
		break;
	case GDI_CHAT_GROUP_INFO:
#ifndef _SERVER
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.IGetTeamInfo(nParam, (KUiChatGroupInfo*)uParam);
#endif
		break;
	//����һ�����ѷ����к��ѵ���Ϣ
	//uParam = (KUiPlayerItem*)pList -> ��Ա��Ϣ�б�
	//			KUiPlayerItem::nData = (CHAT_STATUS)eFriendStatus ���ѵĵ�ǰ״̬
	//nParam = Ҫ��ȡ�б�ĺ��ѷ��������
	//Return = ��ֵ��ʾpList�����е�ǰ���ٸ�KUiPlayerItem�ṹ���������Ч������.
	case GDI_CHAT_FRIENDS_IN_AGROUP:
#ifndef _SERVER
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.IGetTeamFriendInfo(nParam, (KUiPlayerItem*)uParam);
#endif
		break;
	case GDI_ITEM_IN_STORE_BOX:
#ifndef _SERVER
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			// ��һ����Ǯ
			nCount++;
			// ������װ��
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
					nCount++;
			}
			nRet = nCount;
		}
#endif
		break;
	case GDI_SCENE_TIME_INFO:
#ifndef _SERVER
		if (uParam)
		{
			KUiSceneTimeInfo* pInfo = (KUiSceneTimeInfo*)uParam;
			g_ScenePlace.GetSceneNameAndFocus(pInfo->szSceneName, pInfo->nSceneId,
				pInfo->nScenePos0, pInfo->nScenePos1);
			pInfo->nGameSpaceTime = (SubWorld[0].m_dwCurrentTime / 100) % 1440;
		}
#endif
		break;
	}
	return nRet;
}

int KCoreShell::FreeGameData(unsigned int uDataId, unsigned int uParam, int nParam)
{
	int nRet = 0;

	switch(uDataId)
	{
	case GDI_NPC_RES:
#ifndef _SERVER
		delete (KNpcRes *)uParam;
#endif
		break;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ�����Ϸ���Ͳ���
//	������unsigned int uDataId --> Core�ⲿ�ͻ���core�Ĳ����������������
//							��ֵΪ÷������GAMEOPERATION_INDEX��ȡֵ֮һ��
//		  unsigned int uParam  --> ����uOperId��ȡֵ�������
//		  int nParam --> ����uOperId��ȡֵ�������
//	���أ�����ɹ����Ͳ������󣬺������ط�0ֵ�����򷵻�0ֵ��
//--------------------------------------------------------------------------
int	KCoreShell::OperationRequest(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 1;
	switch(uOper)
	{
	//uParam = (const char*)pszFileName
	case GOI_PLAY_SOUND:
#ifndef _SERVER
		if (uParam)
		{
			static KCacheNode* pSndNode = NULL;
			KWavSound* pSound = NULL;
			pSndNode	= (KCacheNode*)g_SoundCache.GetNode((char *)uParam, (KCacheNode * )pSndNode);
			pSound		= (KWavSound*)pSndNode->m_lpData;
			if (pSound)
			{
				if (pSound->IsPlaying())
					break;
				pSound->Play(0, Option.GetSndVolume(), 0);
			}
		}
#endif
		break;
	case GOI_PLAYER_RENASCENCE:
		{
#ifndef _SERVER
			int nReviveType;
			if (nParam)	// bBackTown
			{
				nReviveType = REMOTE_REVIVE_TYPE;
			}
			else
			{
				nReviveType = LOCAL_REVIVE_TYPE;
			}
//			SendClientCmdRevive(nReviveType);
			SendClientCmdRevive();
#endif
		}
		break;
	case GOI_MONEY_INOUT_STORE_BOX:
		{
#ifndef _SERVER
			BOOL	bIn = (BOOL)uParam;
			int		nMoney = nParam;
			int		nSrcRoom, nDesRoom;


			if (bIn)
			{
				nSrcRoom = room_equipment;
				nDesRoom = room_repository;
			}
			else
			{
				nDesRoom = room_equipment;
				nSrcRoom = room_repository;
			}
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeMoney(nSrcRoom, nDesRoom, nMoney);
#endif
		}
		break;
		//�뿪��Ϸ
	case GOI_EXIT_GAME:
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].SaveClientSkillData();
		g_SubWorldSet.Close();
#endif
		//		if (g_pClient)
//			g_pClient->Shutdown();
		break;
	case GOI_TRADE_NPC_BUY:
		{
#ifndef _SERVER
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
					break;

			int nIdx = 0;
			KItem* pItem = NULL;

			nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
			pItem = BuySell.GetItem(nIdx);

			int nWidth, nHeight;
			ItemPos	Pos;

			nWidth = pItem->GetWidth();
			nHeight = pItem->GetHeight();
			if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
			{
				nRet = 0;
				break;
			}
			if (Pos.nPlace != pos_equiproom)
			{
				nRet = 0;

				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}

			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice())
			{
				nRet = 0;
				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}
			SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
#endif
		}
		break;
	case GOI_TRADE_NPC_SELL:
		{
#ifndef _SERVER
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
//			KUiObjAtContRegion* pObject2 = (KUiObjAtContRegion*)nParam;

			if (CGOG_ITEM != pObject1->Obj.uGenre)
				break;
			//����ȥ�Ķ�����Ϊ�գ�������������
			int nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{
				if (Item[nIdx].GetGenre() == item_task)
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return 0;
				}
				SendClientCmdSell(Item[nIdx].GetID());
				return 1;
			}
			else
			{
				return 0;
			}
#endif
		}
		break;
	case GOI_SWITCH_OBJECT:
		//uParam = (KUiObjAtContRegion*)pObject1 -> �������Ʒ����ǰ����Ϣ
		//nParam = (KUiObjAtContRegion*)pObject2 -> ���µ���Ʒ���������Ϣ
		{
#ifndef _SERVER
			ItemPos	P1, P2;
			int PartConvert[itempart_num] = 
			{ 
				itempart_head,		itempart_weapon,
				itempart_amulet,	itempart_cuff,
				itempart_body,		itempart_belt,
				itempart_ring1,		itempart_ring2,
				itempart_pendant,	itempart_foot,
				itempart_horse,
			};
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			KUiObjAtContRegion* pObject2 = (KUiObjAtContRegion*)nParam;
			
			if (!pObject1 && !pObject2)
				break;
			
			if (pObject1)
			{
				switch(pObject1->eContainer)
				{
				case UOC_STORE_BOX:
					P1.nPlace = pos_repositoryroom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_IMMEDIA_ITEM:
					P1.nPlace = pos_immediacy;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_ITEM_TAKE_WITH:
					P1.nPlace = pos_equiproom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_EQUIPTMENT:
					{
						// TODO:��ʱû�еڶ�װ��
						if (pObject1->Region.h == 1)
							break;
						P1.nPlace = pos_equip;
						P1.nX = PartConvert[pObject1->Region.v];
					}
					break;
				case UOC_TO_BE_TRADE:
					P1.nPlace = pos_traderoom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_NPC_SHOP:
					if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
						break;

					int nIdx = 0;
					KItem* pItem = NULL;
					
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
					pItem = BuySell.GetItem(nIdx);
					
					int nWidth, nHeight;
					ItemPos	Pos;
					
					nWidth = pItem->GetWidth();
					nHeight = pItem->GetHeight();
					if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					if (Pos.nPlace != pos_equiproom)
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice())
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					// �������Ķ�����Ϊ�գ�����������
					SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
					break;
				}
			}
			
			if (pObject2)
			{
				switch(pObject2->eContainer)
				{
				case UOC_STORE_BOX:
					P2.nPlace = pos_repositoryroom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_IMMEDIA_ITEM:
					P2.nPlace = pos_immediacy;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_ITEM_TAKE_WITH:
					P2.nPlace = pos_equiproom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EQUIPTMENT:
					{
						// TODO:��ʱû�еڶ�װ��
						if (pObject2->Region.h == 1)
							break;
						P2.nPlace = pos_equip;
						P2.nX = PartConvert[pObject2->Region.v];
					}
					break;
				case UOC_TO_BE_TRADE:
					P2.nPlace = pos_traderoom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_NPC_SHOP:
					break;
				}
			}
			if (!pObject1)
			{
				memcpy(&P1, &P2, sizeof(P1));
			}
			if (!pObject2)
			{
				memcpy(&P2, &P1, sizeof(P1));
			}
			Player[CLIENT_PLAYER_INDEX].MoveItem(P1, P2);
#endif
		}
		break;

	//��ҵ���Ի���
	case GOI_INFORMATION_CONFIRM_NOTIFY:
	{
#ifndef _SERVER
		PLAYER_SELECTUI_COMMAND command;
		command.nSelectIndex = 0;
		Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_TALKDIALOG);
#endif
		break;
	}
	
	//����ѡ���
	//nParma = nAnswerIndex
	case GOI_QUESTION_CHOOSE:
#ifndef _SERVER
		if (g_bUISelLastSelCount == 0 )
			break;
		
		{
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex = nParam;
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_SELECTDIALOG);
		}
#endif
		break;

	//ʹ����Ʒ
	//uParam = (KUiObjAtRegion*)pInfo -> ��Ʒ�������Լ���Ʒԭ���ڷŵ�λ��
	//nParam = ��Ʒʹ��ǰ���õ�λ�ã�ȡֵΪö������UIOBJECT_CONTAINER��
	case GOI_USE_ITEM:
		//to do: waiting for...
#ifndef _SERVER
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			int nPlace = nParam;
			ItemPos	Pos;
			switch(nPlace)
			{
			case UOC_ITEM_TAKE_WITH:
				Pos.nPlace = pos_equiproom;
				break;
			case UOC_IMMEDIA_ITEM:
				Pos.nPlace = pos_immediacy;
				break;
			default:
				Pos.nPlace = -1;
				break;
			}
			Pos.nX = pInfo->Region.h;
			Pos.nY = pInfo->Region.v;
			if (pInfo->Obj.uGenre == CGOG_ITEM && pInfo->Obj.uId > 0 && Pos.nPlace != -1)
				Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pInfo->Obj.uId, Pos);
		}
#endif
		break;

	//����װ��
	//uParam = (KUiObjAtRegion*)pInfo -> װ�������ݺͷ���λ����Ϣ
	//			KUiObjAtRegion::Region::h ��ʾ���ڵڼ���װ��
	//			KUiObjAtRegion::Region::v ��ʾ�����ĸ�λ�õ�װ��,��ֵΪ÷������
	//			UI_EQUIPMENT_POSITION��ȡֵ֮һ����ο�UI_EQUIPMENT_POSITION��ע�͡�
	case GOI_WEAR_EQUIP:
		//to do: waiting for...
#ifndef _SERVER
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
		}
#endif
		break;

	//ʩչ�书/����
	//uParam = (KUiGameObject*)pInfo -> ��������
	case GOI_USE_SKILL:	
#ifndef _SERVER
		if (uParam)
		{
			//to do:wating for...
			KUiGameObject* pInfo = (KUiGameObject*)uParam;
		}
#endif
		break;

	//������������
	//uParam = (KUiGameObject*)pSKill, ������Ϣ
	//nParam = ����λ�ã�0��ʾΪ�Ҽ����ܣ�1��4��ʾΪF1��F4����
	case GOI_SET_IMMDIA_SKILL:
#ifndef _SERVER
		if (uParam)
		{
			KUiGameObject* pSkill = (KUiGameObject*)uParam;
			if (nParam == 0)
				//to do : modify;
			{
				if ( (int)pSkill->uId > 0 )
					Player[CLIENT_PLAYER_INDEX].SetLeftSkill((int)pSkill->uId);
				else if ((int)(pSkill->uId) == -1) //���õ�ǰ������Ϊ����
				{
					int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
					
					//��������
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
					}//Զ������
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
					}//����
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
					}
					
				}
			}
			else if (nParam == 1)
			{
				if ((int)pSkill->uId > 0)
					Player[CLIENT_PLAYER_INDEX].SetRightSkill((int)pSkill->uId);
				else if ((int)(pSkill->uId) == -1)
				{
					int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
					
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
					}
				}
			}
		}
#endif
		break;

	//��ǿһ�ּ��ܣ���һ�μ�һ��
	//uParam = ��������
	//nParam = (uint)����id
	case GOI_TONE_UP_SKILL:
		//to do : waiting for...
		{
#ifndef _SERVER
			Player[CLIENT_PLAYER_INDEX].ApplyAddSkillLevel((int)nParam, 1);
#endif
		}
		break;

	//��ǿһЩ���Ե�ֵ��һ�μ�һ��
	//uParam = ��ʾҪ��ǿ�����ĸ����ԣ�ȡֵΪUI_PLAYER_ATTRIBUTE��÷��ֵ֮һ
	case GOI_TONE_UP_ATTRIBUTE:
#ifndef _SERVER
		switch (uParam)
		{
		case UIPA_STRENGTH:		//����
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(0, 1);
			break;
		case UIPA_DEXTERITY:	//����
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(1, 1);
			break;		
		case UIPA_VITALITY:		//����
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(2, 1);
			break;
		case UIPA_ENERGY:		//����
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(3, 1);
			break;		
		}
#endif
		break;

	//===========������==================
	case GOI_TEAM_COLLECT_NEARBY_LIST:
#ifndef _SERVER
		NpcSet.GetAroundOpenCaptain(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp);
#endif
		break;

	case GOI_TEAM_APPLY:
		if (uParam)
#ifndef _SERVER
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(((KUiTeamItem*)uParam)->Leader.nIndex);
#endif
		break;

	case GOI_TRAM_REFUSE_INVITE:
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].m_cTeam.SetAutoRefuseInvite(nParam);
#endif
		break;

	//�������
	//uParam = (const char*)pTeamName -> ������������
	case GOI_TEAM_NEW:
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].ApplyCreateTeam();//(char*)uParam);
#endif
		break;

	//�����ӳ�
	//uParam = uNewLeaderId -> �¶ӳ���id
	case GOI_TEAM_APPOINT:
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].ApplyTeamChangeCaptain(((KUiPlayerItem*)uParam)->uId);
#endif
		break;

	//������˼������
	case GOI_TEAM_INVITE:
#ifndef _SERVER
		if (uParam)
		{
			Player[CLIENT_PLAYER_INDEX].TeamInviteAdd(((KUiPlayerItem*)uParam)->uId);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_SEND_INVITE, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
#endif
		break;

	//�߳������һ����Ա
	//uParam = uMemberId -> Ҫ�߳��Ķ�Ա��id
	case GOI_TEAM_KICK:
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].TeamKickMember(((KUiPlayerItem*)uParam)->uId);
#endif
		break;

	//��׼���˼������
	//uParam = uPlayerId -> ����׼���˵�id
	case GOI_TEAM_APPROVE:
#ifndef _SERVER
		if (uParam)
		{
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].AcceptTeamMember(((KUiPlayerItem*)uParam)->uId);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(((KUiPlayerItem*)uParam)->uId);
				Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
			}
		}
#endif
		break;

	//�뿪����
#ifndef _SERVER
	case GOI_TEAM_LEAVE:
		Player[CLIENT_PLAYER_INDEX].LeaveTeam();
		break;

	// ���鿪�š��ر�
	case GOI_TEAM_CLOSE:
		if (nParam)
		{
			Player[CLIENT_PLAYER_INDEX].ApplyTeamOpenClose(TRUE);
		}
		else
			Player[CLIENT_PLAYER_INDEX].ApplyTeamOpenClose(FALSE);
		break;

	//���������Ļظ�
	//uParam = (KUiPlayerItem*)pTeamLeader �����������Ķӳ�
	//nParam = (int)(bool)bAccept �Ƿ��������
	case GOI_TEAM_INVITE_RESPONSE:
		if (uParam)
		{
			KUiPlayerItem	*pTeamLeader = (KUiPlayerItem*)uParam;
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(pTeamLeader->nIndex, 1);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(pTeamLeader->nIndex, 0);
			}
		}
		break;

	//��Ӧ/�ܾ���������
	//uParam = (KUiPlayerItem*)pRequestPlayer ������������
	//nParam = (int)(bool)bAccept �Ƿ��������
	case GOI_TRADE_INVITE_RESPONSE:
		if (uParam)
		{
			KTrade::ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
		}
		break;

	//����һ������������Ʒ
	//uParam = (KUiObjAtRegion*) pObject -> ��Ʒ��Ϣ������������ϢΪ�ڽ��׽����е�����
	//nParam = bAdd -> 0ֵ��ʾ���٣�1ֵ��ʾ����
	//Remark : �����Ʒ�ǽ�Ǯ�Ļ�����KUiObjAtRegion::Obj::uId��ʾ�ѽ�Ǯ�����Ϊ���ֵ����nParam�����塣
	case GOI_TRADE_DESIRE_ITEM:
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			if (pInfo->Obj.uGenre != CGOG_MONEY)
				break;
			Player[CLIENT_PLAYER_INDEX].TradeMoveMoney(pInfo->Obj.uId);
		}
		break;

	//���޽�������
	//nParam = bWilling
	case GOI_TRADE_WILLING:
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyClose();
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyOpen((char*)uParam, nParam);
		}
		break;

	//��������
	//nParam = (int)(book)bLock �Ƿ�����
	case GOI_TRADE_LOCK:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock)
			Player[CLIENT_PLAYER_INDEX].TradeApplyLock(0);
		else
			Player[CLIENT_PLAYER_INDEX].TradeApplyLock(1);
		break;

	//����
	case GOI_TRADE:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock != 1 || Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock != 1)
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState == 0)
		{
			Player[CLIENT_PLAYER_INDEX].TradeDecision(1);		// ����ȷ��
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeDecision(2);		// ����ȷ��ȡ��
		}
		break;

	//����ȡ��
	case GOI_TRADE_CANCEL:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			break;
		Player[CLIENT_PLAYER_INDEX].TradeDecision(0);		// ����ȡ��
		break;

	//��ѯ�Ƿ���Զ�ĳ����������Ϸ����
	//uParam = (KUiGameObject*)pObject -> ��Ʒ��Ϣ
	//nParam = ���϶������ĵ�ǰ���꣨�������꣩���������ڵ�16λ���������ڸ�16λ��(���ص�����)
	//Return = �Ƿ���Է���
	case GOI_DROP_ITEM_QUERY:
		//to do : waiting for...
		break;

	case GOI_SEND_MSG:
		if (uParam && nParam)
			Player[CLIENT_PLAYER_INDEX].SendChat((KUiMsgParam*)nParam, (char*)uParam);
		break;
	case GOI_SET_SEND_CHAT_CHANNEL:	//���õ�ǰ������Ϣ��Ƶ��
		if (uParam)
		{
			KUiChatChannel* pChannelInfo = (KUiChatChannel*)uParam;// pChannelInfo Ҫ���õ�Ƶ������Ϣ
			Player[CLIENT_PLAYER_INDEX].m_cChat.SetCurChannel(pChannelInfo->nChannelNo, pChannelInfo->uChannelId, pChannelInfo->nChannelIndex);
		}
		break;
	// ���õ�ǰ������Ϣ��Ƶ��Ϊ��ĳ�˵���
	//uParam = (KUiPlayerItem*) pFriend
	case GOI_SET_SEND_WHISPER_CHANNEL:
		if (uParam)
		{
			if (((KUiPlayerItem*)uParam)->nIndex)
			{
				int	nIdx = NpcSet.SearchName(((KUiPlayerItem*)uParam)->Name);
				if (nIdx > 0)
				{
					Player[CLIENT_PLAYER_INDEX].m_cChat.SetCurChannel(CHAT_CUR_CHANNEL_SCREENSINGLE, Npc[nIdx].m_dwID, 0);
				}
				else
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_NOT_NEAR, ((KUiPlayerItem*)uParam)->Name);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
			}
			else
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_NOT_ONLINE, ((KUiPlayerItem*)uParam)->Name);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			}
		}
		break;

	case GOI_SET_RECEIVE_CHAT_CHANNEL://���ö���/ȡ����ϢƵ��
		if (uParam)
		{
			//uParam = (KUiChatChannel*) pChannelInfo Ҫ���õ�Ƶ������Ϣ
			//nParam = (int)(bool) bEnable �Ƿ���
			KUiChatChannel* pChannelInfo = (KUiChatChannel*)uParam;// pChannelInfo Ҫ���õ�Ƶ������Ϣ
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].m_cChat.AddChannel(pChannelInfo->nChannelNo);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cChat.SubChannel(pChannelInfo->nChannelNo);
			}
		}
		break;

	//�½����������
	//uParam = (const char*) pGroupName
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_GROUP_NEW:
		Player[CLIENT_PLAYER_INDEX].m_cChat.AddTeam(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, (char*)uParam);
		CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
		break;

	//���������������
	//uParam = (const char*) pGroupName
	//nParam = nIndex ������
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_GROUP_RENAME:
		Player[CLIENT_PLAYER_INDEX].m_cChat.RenameTeam(nParam, (char*)uParam, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
		CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
		break;

	//ɾ�����������
	//nParam = nIndex ������
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_GROUP_DELETE:
		Player[CLIENT_PLAYER_INDEX].m_cChat.DeleteTeam(nParam, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
		CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
		break;

	//������������
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	case GOI_CHAT_FRIEND_ADD:
		{
			Player[CLIENT_PLAYER_INDEX].m_cChat.ApplyAddFriend(((KUiPlayerItem*)uParam)->uId);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_APPLY_ADD_FRIEND, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	//ɾ���������
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	//nParam = nGroupIndex �������ڵ��������
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_FRIEND_DELETE:
		{
			Player[CLIENT_PLAYER_INDEX].m_cChat.ApplyDeleteFriend(((KUiPlayerItem*)uParam)->uId, nParam, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_DELETE_FRIEND, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;

	//�����Ƶ�����
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	//nParam = nGroupIndex �������������������
	case GOI_CHAT_FRIEND_MOVE:
		{
			KUiPlayerItem	*pPlayer = (KUiPlayerItem*)uParam;
			int		nSrcTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.GetTeamNo(pPlayer->uId);
			if (nSrcTeamNo == -1)
			{
				// ʧ�ܣ������Ҫ����Ϣ���ڴ�
				break;
			}
			if (Player[CLIENT_PLAYER_INDEX].m_cChat.OneFriendChangeTeam(pPlayer->uId, nSrcTeamNo, nParam))
			{	// �ɹ�
				CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
			}
			else
			{
				// ʧ�ܣ������Ҫ����Ϣ���ڴ�
			}
		}
		break;

	//��������һ��ѶϢ
	//uParam = (KUiChatMessage*)pMessage ��Ϣ����
	//nParam = (KUiPlayerItem*)pFriend   ����ѶϢ�ĺ���
	case GOI_CHAT_WORDS_TO_FRIEND:
		{
			char	szSentence[MAX_SENTENCE_LENGTH];
			KUiChatMessage	*pMsg = (KUiChatMessage*)uParam;
			KUiMsgParam	sMsg;

			sMsg.eGenre = MSG_G_CHAT;
			sMsg.szName[0] = 0;
			memset(sMsg.cChatPrefix, 0, sizeof(sMsg.cChatPrefix));
			sMsg.cChatPrefixLen = sizeof(unsigned int);
			sMsg.cChatPrefix[0] = ((char*)(&pMsg->uColor))[3];
			sMsg.cChatPrefix[1] = ((char*)(&pMsg->uColor))[2];
			sMsg.cChatPrefix[2] = ((char*)(&pMsg->uColor))[1];
			sMsg.cChatPrefix[3] = ((char*)(&pMsg->uColor))[0];

			if (pMsg->nContentLen >= MAX_SENTENCE_LENGTH)
			{
				memcpy(szSentence, pMsg->szContent, MAX_SENTENCE_LENGTH - 1);
				szSentence[MAX_SENTENCE_LENGTH - 1] = 0;
				sMsg.nMsgLength = MAX_SENTENCE_LENGTH - 1;
			}
			else
			{
				memcpy(szSentence, pMsg->szContent, pMsg->nContentLen);
				szSentence[pMsg->nContentLen] = 0;
				sMsg.nMsgLength = pMsg->nContentLen;
			}

			Player[CLIENT_PLAYER_INDEX].m_cChat.QQSendSentence(
				((KUiPlayerItem*)nParam)->uId,
				((KUiPlayerItem*)nParam)->nIndex, &sMsg, szSentence);
		}
		break;

	//�Ա���Ҫ���Լ�Ϊ���ѵĻظ�
	//uParam = (KUiPlayerItem*)pRequestPlayer ������������
	//nParam = (int)(bool)bAccept �Ƿ��������
	case GOI_CHAT_FRIEND_INVITE:
		if (uParam)
		{
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].ChatAddFriend(((KUiPlayerItem*)uParam)->nIndex);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].ChatRefuseFriend(((KUiPlayerItem*)uParam)->nIndex);
			}
		}
		break;

	case GOI_OPTION_SETTING:			//ѡ������
		if (uParam == OPTION_DYNALIGHT)
		{
			g_ScenePlace.EnableDynamicLights(nParam != 0);
			if (g_pRepresent)
				g_pRepresent->SetOption(DYNAMICLIGHT, nParam != 0);
		}
		else if (uParam == OPTION_PERSPECTIVE)
		{
			if (g_pRepresent)
				g_pRepresent->SetOption(PERSPECTIVE, nParam != 0);
		}
		else if (uParam == OPTION_MUSIC_VALUE)
			Option.SetMusicVolume(nParam);
		else if (uParam == OPTION_SOUND_VALUE)
			Option.SetSndVolume(nParam);
		else if (uParam == OPTION_BRIGHTNESS)
			Option.SetGamma(nParam);

		break;
		
	case GOI_PLAYER_ACTION:
		{
			switch(uParam)
			{
			case PA_RUN:
				Player[CLIENT_PLAYER_INDEX].m_RunStatus = !Player[CLIENT_PLAYER_INDEX].m_RunStatus;
				break;
			case PA_SIT:
				if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
				{
					if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit)
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
					else
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
					SendClientCmdSit();
				}
				else
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_CLICK;
					Msg.eType = SMT_PLAYER;
					Msg.byPriority = 1;
					Msg.byParamSize = 0;
					strcpy(Msg.szMessage, "����ʱ�޷�������");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
				}
				break;
			}
		}
		break;
	case GOI_IS_SCENE_MAP_SHOWING:
		g_ScenePlace.SetMapParam(uParam, nParam);
		break;
	case GOI_PAINT_SCENE_MAP:
		if (uParam)
		{
			nRet = g_ScenePlace.PaintMap(((KUiRegion*)uParam)->h,
				((KUiRegion*)uParam)->v, (KUiRegion*)nParam);
		}
		break;
#endif
	default:
		nRet = 0;
		break;
	}

	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ����������豸�����������Ϣ
//--------------------------------------------------------------------------
void KCoreShell::ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam)
{
#ifndef _SERVER
	Player[CLIENT_PLAYER_INDEX].ProcessInputMsg(uMsg, uParam, nParam);
#endif
}

int KCoreShell::FindSelectPlayer(int x, int y, void* pReturn)
{
#ifndef _SERVER
	Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_none | relation_ally | relation_enemy);
	int nT = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
	if (nT > 0 &&
		Npc[nT].m_Kind == kind_player)
	{
		if (pReturn)
		{
			KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
			strncpy(p->Name, Npc[nT].Name, 32);
			p->nIndex = Npc[nT].m_Index;
			p->uId = Npc[nT].m_dwID;
		}
		return true;
	}
#endif
	return false;
}

int KCoreShell::FindSpecialPlayer(char* Name, void* pReturn)
{
	if (Name == NULL || Name[0] == 0)
		return false;
	for (int nT = 0; nT < MAX_NPC; nT++)
	{
		if	(Npc[nT].m_Kind == kind_player &&
			strcmp(Npc[nT].Name, Name) == 0)
		{
			if (pReturn)
			{
				KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
				strncpy(p->Name, Npc[nT].Name, 32);
				p->nIndex = Npc[nT].m_Index;
				p->uId = Npc[nT].m_dwID;
			}
			return true;
		}
	}
	return false;
}

int KCoreShell::ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC)
		{
			int nTalker = p->nIndex;
			if (Npc[nTalker].m_Kind == kind_player &&
				Npc[nTalker].m_dwID == p->uId)
			{
				memset(Npc[nTalker].m_szChatBuffer, 0, sizeof(Npc[nTalker].m_szChatBuffer));
				memcpy(Npc[nTalker].m_szChatBuffer, pMsgBuff, nMsgLength);
				Npc[nTalker].m_nCurChatTime = NPC_SHOW_CHAT_TIME_LENGTH;
				return true;
			}
		}
	}

	return false;
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϸ����
//--------------------------------------------------------------------------
void KCoreShell::DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam)
{
#ifndef _SERVER
	if (g_pRepresent)
		CoreDrawGameObj(uObjGenre, uId, x, y, Width, Height, nParam);
#endif
}

#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#endif
//--------------------------------------------------------------------------
//	���ܣ�������Ϸ����
//--------------------------------------------------------------------------
void KCoreShell::DrawGameSpace()
{
#ifndef _SERVER
	if (g_pRepresent)
	{
		g_ScenePlace.Paint();
		Player[CLIENT_PLAYER_INDEX].DrawSelectInfo();
	}
#endif
}

//--------------------------------------------------------------------------
//	���ܣ����û�ͼ�ӿ�ʵ����ָ��
//--------------------------------------------------------------------------
void KCoreShell::SetRepresentShell(struct iRepresentShell* pRepresent)
{
#ifndef _SERVER
	g_pRepresent = pRepresent;
	g_ScenePlace.RepresentShellReset();
#endif
}

void KCoreShell::SetMusicInterface(void* pMusicInterface)
{
#ifndef _SERVER
	g_pMusic = (KMusic*)pMusicInterface;
#endif
}

//�ճ����core���Ҫ���������򷵻�0�����򷵻ط�0ֵ
int KCoreShell::Breathe()
{
	g_SubWorldSet.MessageLoop();
	g_SubWorldSet.MainLoop();
#ifndef _SERVER
	g_ScenePlace.Breathe();
#endif
	return true;
}

int KCoreShell::GetProtocolSize(BYTE byProtocol)
{
	if (byProtocol <= s2c_clientbegin || byProtocol >= s2c_end)
		return -1;
	return g_nProtocolSize[byProtocol - s2c_clientbegin - 1];
}

#ifdef SWORDONLINE_SHOW_DBUG_INFO
extern int		g_bShowObstacle;
extern bool		g_bShowGameInfo;	//�Ƿ���ʾ��Ϸ����������Ϣ
#endif
int KCoreShell::Debug(unsigned int uDataId, unsigned int uParam, int nParam)
{
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	switch(uDataId)
	{
	case DEBUG_SHOWINFO:
		Player[CLIENT_PLAYER_INDEX].m_DebugMode = !Player[CLIENT_PLAYER_INDEX].m_DebugMode;
		g_bShowGameInfo = !g_bShowGameInfo;
		break;
	case DEBUG_SHOWOBSTACLE:
		g_bShowObstacle = !g_bShowObstacle;
		break;
	}
#endif
	return 0;
}

DWORD KCoreShell::GetPing()
{
#ifndef _SERVER
	return g_SubWorldSet.GetPing();
#else
	return 0;
#endif
}

void KCoreShell::SendPing()
{
#ifndef _SERVER
	SendClientCmdPing();
#endif
}

void KCoreShell::SetRepresentAreaSize(int nWidth, int nHeight)
{
#ifndef _SERVER
	g_ScenePlace.SetRepresentAreaSize(nWidth, nHeight);
#endif
}

void KCoreShell::SetClient(LPVOID pClient)
{
#ifndef _SERVER
	g_SetClient(pClient);
#endif
}

void KCoreShell::SendNewDataToServer(void* pData, int nLength)
{
#ifndef _SERVER
	if (g_pClient)
		g_pClient->SendPackToServer(pData, nLength);
#endif
}
