#pragma once
#include "StdAfx.h"
#include "CCDKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

hash_map<string,CCDKey*> CCDKey::m_mapCDKeyList;
//////////////////////////////////////////////////////////////////////////
CCDKey::CCDKey(const char* strCDKey)
{
	m_strCDKey = strCDKey;
}
CCDKey::~CCDKey()
{
	for(int i=0;i<CONTAINER_NUM;i++)
	{
		hash_map<CGUID,tagCDKeyGoodsInfo,hash_guid_compare>::iterator it = m_mapContainerID[i].begin();
		for(;it != m_mapContainerID[i].end();it++)
		{
			SAFE_DELETE(it->second.pGoods);
		}
		m_mapContainerID->clear();
	}
}
void CCDKey::InitCDKeyList(const char* filename)
{
	std::ifstream in_f;
	if( in_f.fail())
		return;
	in_f.open(filename);

	while(true)
	{
		string strCDKey;
		if( in_f.eof())
			break;
		in_f>>strCDKey;
		if(strCDKey.empty())
			break;
		//
		CCDKey* pCDKey = new CCDKey(strCDKey.c_str());
		m_mapCDKeyList.insert(make_pair(pCDKey->m_strCDKey,pCDKey));
		InitCDKeyData(pCDKey);
	}
}
void CCDKey::InitCDKeyData(CCDKey* pCDKey)
{
	if(pCDKey)
	{
		pCDKey->GetSkill();
		pCDKey->GetPlayerBaseProperty();
		pCDKey->GetAllMoney();
		pCDKey->GetAllContainerData();
	}
}
void CCDKey::Release()
{
	if( !m_mapCDKeyList.empty() )
	{
		hash_map<string,CCDKey*>::iterator it = m_mapCDKeyList.begin();
		for(;it != m_mapCDKeyList.end();)
		{
			CCDKey* pCDKey = (*it).second;
			SAFE_DELETE(pCDKey);
			m_mapCDKeyList.erase(it++);
		}
	}
}

CCDKey* CCDKey::GetCDKey(const char* strCDKey)
{
	hash_map<string,CCDKey*>::iterator it = m_mapCDKeyList.find(strCDKey);
	if( it != m_mapCDKeyList.end())
	{
		CCDKey* pCDKey = (*it).second;
		return pCDKey;
	}
	return FALSE;
}
void CCDKey::AddCDKey(CCDKey* pCDKey)
{
	if( m_mapCDKeyList.find(pCDKey->m_strCDKey) == m_mapCDKeyList.end() )
	{
		FILE* f;
		string filename = "CDKey//CDKeyList.ini";
		if( (f = fopen(filename.c_str(),"a+")) == NULL )
			return;
		fseek(f,0,SEEK_END);
		fwrite(pCDKey->m_strCDKey.c_str(),strlen(pCDKey->m_strCDKey.c_str()),1,f);
		fwrite("\n",1,1,f);
		fclose(f);

		m_mapCDKeyList.insert(make_pair(pCDKey->m_strCDKey,pCDKey));
	}
}
BOOL CCDKey::FindCDKey(const char* strCDKey)
{ 
	hash_map<string,CCDKey*>::iterator it = m_mapCDKeyList.find(strCDKey);
	if( it != m_mapCDKeyList.end() )
	{
		return TRUE;
	}
	return FALSE;
}

void CCDKey::CheckGoods(CPlayer* pPlayer)
{
	//////////////////////////////////////////////////////////////////////////
	/*
	��Ϊ���������Ǿ�����ȷ��
	�����ñ��� Goods ȥ���������
	������ص�ÿ�� Goods ��������������ҵ�,���ұ���Goods������������ϵ�Goods������ͬ �������ȷ
	*/
	char strOutInfo[128];
	//��������Goods����
	for( int i=0;i<CONTAINER_NUM;i++)
	{
		eContainerID eID = (eContainerID)i; 	
		switch(eID)
		{
		case EQUIP_ITEM:
			{
				//������غ��������������ͬ
				if( m_mapContainerID[EQUIP_ITEM].size() != pPlayer->GetEquipAmount())
				{
					sprintf(strOutInfo,"(%s)װ���������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
					continue;
					//return;
				}
				//������ͬ
				else
				{
					hashGoodsIter itEquipInfo = m_mapContainerID[EQUIP_ITEM].begin();
					//��ÿ������������Goodsȥ�ҽ��������
					for(;itEquipInfo != m_mapContainerID[eID].end();itEquipInfo++)
					{
						tagCDKeyGoodsInfo equipInfo = itEquipInfo->second;
						CGoods* pGoods = equipInfo.pGoods;								//����Goods
						CGoods* pEquip = pPlayer->GetEquip(equipInfo.dwPos);			//�������

						eCompareGoodsError err = CompareGoodsData(pEquip,pGoods);
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)װ���������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
							//return;
						}
					}
				}
			}
			break;
		case MAIN_PACK:
			{
				if( m_mapContainerID[MAIN_PACK].size() != pPlayer->GetGoodsList(0)->size())
				{
					sprintf(strOutInfo,"(%s)ԭʼ�����������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
					continue;
				//	return;
				}
				else{
					hashGoodsIter itCDKeyGoods = m_mapContainerID[MAIN_PACK].begin();
					for(;itCDKeyGoods != m_mapContainerID[MAIN_PACK].end();itCDKeyGoods++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = itCDKeyGoods->second;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;

						CPlayer::tagGoods* ptagGoods = pPlayer->GetGoods(cdkeyGoodsInfo.dwPos);
						if(ptagGoods && cdkeyGoods )
						{
							CGoods* playerGoods = ptagGoods->pGoods;
							if( cdkeyGoodsInfo.wNum == ptagGoods->lNum )
							{
								eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods);
								if( err == ONE_INVALID || err == RESULT_ERROR )
								{
									sprintf(strOutInfo,"(%s)ԭʼ�����������",m_strCDKey.c_str());
									PutPlayerDataInfo(strOutInfo);
								//	return;
								}
							}
						}
					}
				}
			}
			break;
		case MAIN_PACK_ITEM:
			{
				if( m_mapContainerID[MAIN_PACK_ITEM].size() != pPlayer->GetCoteGoodsAmount())
				{
					sprintf(strOutInfo,"(%s)����������������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
				//	return;
				}
				else{
					hashGoodsIter itCDKey = m_mapContainerID[MAIN_PACK_ITEM].begin();
					for(;itCDKey != m_mapContainerID[MAIN_PACK_ITEM].end();itCDKey++)
					{
						tagCDKeyGoodsInfo cdkeyGInfo = itCDKey->second;
						CPlayer::tagCote* pCote = pPlayer->GetCoteGoods(cdkeyGInfo.dwPos);
						if(pCote==NULL&& cdkeyGInfo.pGoods != NULL)
						{
							sprintf(strOutInfo,"(%s)����������������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
					//		return;
						}

						CGoods* playerGoods = pCote->pGoods;
						CGoods* cdkeyGoods = cdkeyGInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)����������������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
					//		return;
						}
					}
				}
			}
			break;
		case MAIN_PACK_ITEM_PACK_1:
			{
				if( m_mapContainerID[MAIN_PACK_ITEM_PACK_1].size() != pPlayer->GetSubGoodslist(0)->size() )
				{
					sprintf(strOutInfo,"(%s)�ӱ���1�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
				//	return;
				}
				else
				{
					hashGoodsIter it = m_mapContainerID[MAIN_PACK_ITEM_PACK_1].begin();
					for(;it != m_mapContainerID[MAIN_PACK_ITEM_PACK_1].end();it++)
					{
						tagCDKeyGoodsInfo cdkGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetSubGoods(0,cdkGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ӱ���1�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
					//		return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ӱ���1�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
					//		return;
						}
					}
				}
			}
			break;
		case MAIN_PACK_ITEM_PACK_2:
			{
				if( m_mapContainerID[MAIN_PACK_ITEM_PACK_2].size() != pPlayer->GetSubGoodslist(1)->size() )
				{
					sprintf(strOutInfo,"(%s)�ӱ���2�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
				//	return;
				}
				else
				{
					hashGoodsIter it = m_mapContainerID[MAIN_PACK_ITEM_PACK_2].begin();
					for(;it != m_mapContainerID[MAIN_PACK_ITEM_PACK_2].end();it++)
					{
						tagCDKeyGoodsInfo cdkGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetSubGoods(1,cdkGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ӱ���2�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
					//		return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ӱ���2�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
					//		return;
						}
					}
				}
			}
			break;
		case MAIN_PACK_ITEM_PACK_3:
			{
				if( m_mapContainerID[MAIN_PACK_ITEM_PACK_3].size() != pPlayer->GetSubGoodslist(2)->size() )
				{
					sprintf(strOutInfo,"(%s)�ӱ���3�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
			//		return;
				}
				else
				{
					hashGoodsIter it = m_mapContainerID[MAIN_PACK_ITEM_PACK_3].begin();
					for(;it != m_mapContainerID[MAIN_PACK_ITEM_PACK_3].end();it++)
					{
						tagCDKeyGoodsInfo cdkGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetSubGoods(2,cdkGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ӱ���3�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
				//			return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ӱ���3�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
				//			return;
						}
					}
				}
			}
			break;
		case MAIN_PACK_ITEM_PACK_4:
			{
				if( m_mapContainerID[MAIN_PACK_ITEM_PACK_4].size() != pPlayer->GetSubGoodslist(3)->size() )
				{
					sprintf(strOutInfo,"(%s)�ӱ���4�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
			//		return;
				}
				else
				{
					hashGoodsIter it = m_mapContainerID[MAIN_PACK_ITEM_PACK_4].begin();
					for(;it != m_mapContainerID[MAIN_PACK_ITEM_PACK_4].end();it++)
					{
						tagCDKeyGoodsInfo cdkGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetSubGoods(3,cdkGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ӱ���4�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
				//			return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ӱ���4�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
				//			return;
						}
					}
				}
			}
			break;
		case MAIN_PACK_ITEM_PACK_5:
			{
				if( m_mapContainerID[MAIN_PACK_ITEM_PACK_5].size() != pPlayer->GetSubGoodslist(4)->size() )
				{
					sprintf(strOutInfo,"(%s)�ӱ���4�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
			//		return;
				}
				else
				{
					hashGoodsIter it = m_mapContainerID[MAIN_PACK_ITEM_PACK_5].begin();
					for(;it != m_mapContainerID[MAIN_PACK_ITEM_PACK_5].end();it++)
					{
						tagCDKeyGoodsInfo cdkGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetSubGoods(4,cdkGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ӱ���5�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
		//					return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ӱ���5�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
			//				return;
						}
					}
				}
			}
			break;
		case DEPOT_MAIN_PACK:
			{
				if(m_mapContainerID[DEPOT_MAIN_PACK].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_MAIN_PACK)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
		//			return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_MAIN_PACK].begin();
					for(;it != m_mapContainerID[DEPOT_MAIN_PACK].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_MAIN_PACK,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
			//				return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
			//				return;
						}
					}
				}
			}
			break;
		case DEPOT_Def_SubPACK:
			{
				if(m_mapContainerID[DEPOT_Def_SubPACK].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_DEFAULT_PACK)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)Ĭ�ϲֿ��ӱ����������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
		//			return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_Def_SubPACK].begin();
					for(;it != m_mapContainerID[DEPOT_Def_SubPACK].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_DEFAULT_PACK,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
		//					return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
		//					return;
						}
					}
				}
			}
			break;
		case DEPOT_ITEM:
			{
				if(m_mapContainerID[DEPOT_ITEM].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_ITEM_PACK)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)�ֿ������������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
		//			return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_ITEM].begin();
					for(;it != m_mapContainerID[DEPOT_ITEM].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_ITEM_PACK,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
			//				return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)ס�ֿ�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
			//				return;
						}
					}
				}
			}
			break;
		case DEPOT_ITEM_PACK_1:
			{
				if(m_mapContainerID[DEPOT_ITEM_PACK_1].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_EXTEND_PACK1)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)�ֿ��ӱ���1�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
		//			return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_ITEM_PACK_1].begin();
					for(;it != m_mapContainerID[DEPOT_ITEM_PACK_1].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_EXTEND_PACK1,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���1�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
			//				return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���1�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
			//				return;
						}
					}
				}
			}
			break;
		case DEPOT_ITEM_PACK_2:
			{
				if(m_mapContainerID[DEPOT_ITEM_PACK_2].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_EXTEND_PACK2)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)�ֿ��ӱ���2�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
			//		return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_ITEM_PACK_2].begin();
					for(;it != m_mapContainerID[DEPOT_ITEM_PACK_2].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_EXTEND_PACK2,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���2�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
				//			return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���2�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
				//			return;
						}
					}
				}
			}
			break;
		case DEPOT_ITEM_PACK_3:
			{
				if(m_mapContainerID[DEPOT_ITEM_PACK_3].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_EXTEND_PACK3)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)�ֿ��ӱ���3�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
			//		return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_ITEM_PACK_3].begin();
					for(;it != m_mapContainerID[DEPOT_ITEM_PACK_3].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_EXTEND_PACK3,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���3�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
			//				return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���3�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
			//				return;
						}
					}
				}
			}
			break;
		case DEPOT_ITEM_PACK_4:
			{
				if(m_mapContainerID[DEPOT_ITEM_PACK_4].size() != pPlayer->GetDepotInfo(CPlayer::DEPOT_EXTEND_PACK4)->listGoods.size())
				{
					sprintf(strOutInfo,"(%s)�ֿ��ӱ���4�������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
		//			return;
				}
				else{
					hashGoodsIter it = m_mapContainerID[DEPOT_ITEM_PACK_4].begin();
					for(;it != m_mapContainerID[DEPOT_ITEM_PACK_4].end();it++)
					{
						tagCDKeyGoodsInfo cdkeyGoodsInfo = it->second;
						CPlayer::tagGoods* ptagGoods = pPlayer->GetDepotGoodsByPos(CPlayer::DEPOT_EXTEND_PACK4,cdkeyGoodsInfo.dwPos);
						if(ptagGoods==NULL&&cdkeyGoodsInfo.pGoods!=NULL)
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���4�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
							continue;
			//				return;
						}
						CGoods* playerGoods = ptagGoods->pGoods;
						CGoods* cdkeyGoods = cdkeyGoodsInfo.pGoods;
						eCompareGoodsError err = CompareGoodsData(playerGoods,cdkeyGoods) ;
						if( err == ONE_INVALID || err == RESULT_ERROR )
						{
							sprintf(strOutInfo,"(%s)�ֿ��ӱ���4�������",m_strCDKey.c_str());
							PutPlayerDataInfo(strOutInfo);
			//				return;
						}
					}
				}
			}
			break;
		}
	}
}

void CCDKey::CheckPlayerData(CPlayer* pPlayer)
{
	BOOL bError = TRUE;

	char * strName = (char*)pPlayer->GetName();
	char strOutInfo[256];
	//����
	CPlayer::tagSkill Skill;
	list<CPlayer::tagSkill> skillList = pPlayer->GetSkillList();
	if(skillList.size() == m_mapSkillEx.size())
	{
		hash_map<DWORD,CPlayer::tagSkill>::iterator it = m_mapSkillEx.begin();
		for(;it != m_mapSkillEx.end();it++){
			CPlayer::tagSkill* pSkill = pPlayer->GetSkill(it->second.dwID);
			if(pSkill)
			{
				Skill = it->second;
				//
				if(pSkill->bIsPublicCoolEffect == Skill.bIsPublicCoolEffect &&
					pSkill->dwCoolDownGrayStartTime == Skill.dwCoolDownGrayStartTime &&
					pSkill->dwCoolDownStartTime == Skill.dwCoolDownStartTime &&
					pSkill->dwID == Skill.dwID &&
					pSkill->dwIntonateStartTime == Skill.dwIntonateStartTime &&
					pSkill->dwIntonateTime == Skill.dwIntonateTime &&
					pSkill->dwRestoreTime == Skill.dwRestoreTime &&
					pSkill->dwUseingStartTime == Skill.dwUseingStartTime && 
					pSkill->dwUseRestoreTime == Skill.dwUseRestoreTime &&
					pSkill->wAffectRangeMax == Skill.wAffectRangeMax &&
					pSkill->wAffectRangeMin == Skill.wAffectRangeMin &&
					pSkill->wCostMP == Skill.wCostMP &&
					pSkill->wLevel == pSkill->wLevel )
					continue;
				else
				{
					sprintf(strOutInfo,"(%s)���ܽ������",m_strCDKey.c_str());
					PutPlayerDataInfo(strOutInfo);
					return;
				}
			}
		}
	}
	//��������
	tagPPro player_pro = pPlayer->GetPlayerProperty();
	if( player_pro.byArtisanLevel == m_PlayerBaseProperty.byArtisanLevel &&
		player_pro.byAssOccu == m_PlayerBaseProperty.byAssOccu &&
		player_pro.byBusinessLevel == m_PlayerBaseProperty.byBusinessLevel &&
		player_pro.byClass == m_PlayerBaseProperty.byClass &&
		player_pro.byCountry == m_PlayerBaseProperty.byCountry &&
		player_pro.byFacePic == m_PlayerBaseProperty.byFacePic &&
		player_pro.byHairPic == m_PlayerBaseProperty.byHairPic &&
		player_pro.byOccu == m_PlayerBaseProperty.byOccu &&
		player_pro.byPKOnOff == m_PlayerBaseProperty.byPKOnOff &&
		player_pro.byRankOfMercenary == m_PlayerBaseProperty.byRankOfMercenary &&
		player_pro.byRankOfNobility == m_PlayerBaseProperty.byRankOfNobility &&
		player_pro.bySex == m_PlayerBaseProperty.bySex &&
		player_pro.dwArtisanCredit == m_PlayerBaseProperty.dwArtisanCredit &&
		player_pro.dwArtisanExp == m_PlayerBaseProperty.dwArtisanExp &&
		player_pro.dwBusinessExp == m_PlayerBaseProperty.dwBusinessExp &&
		player_pro.dwCountryContribute == m_PlayerBaseProperty.dwCountryContribute &&
		player_pro.dwExp == m_PlayerBaseProperty.dwExp &&
		player_pro.dwHp == m_PlayerBaseProperty.dwHp &&
	//	player_pro.dwLastExitGameTime == m_PlayerBaseProperty.dwLastExitGameTime &&
		player_pro.dwMercenaryCredit == m_PlayerBaseProperty.dwMercenaryCredit &&
		player_pro.dwOccuPoint == m_PlayerBaseProperty.dwOccuPoint &&
		player_pro.dwPkCount == m_PlayerBaseProperty.dwPkCount &&
		player_pro.dwPkValue == m_PlayerBaseProperty.dwPkValue &&
		player_pro.dwPresentExp == m_PlayerBaseProperty.dwPresentExp &&
		player_pro.dwPVPCount == m_PlayerBaseProperty.dwPVPCount &&
		player_pro.dwPVPValue == m_PlayerBaseProperty.dwPVPValue &&
		player_pro.dwRankOfNobCredit == m_PlayerBaseProperty.dwRankOfNobCredit &&
		player_pro.dwSkillPoint == m_PlayerBaseProperty.dwSkillPoint &&
		player_pro.dwSpouseID == m_PlayerBaseProperty.dwSpouseID &&
		player_pro.dwSpouseParam == m_PlayerBaseProperty.dwSpouseParam &&
		player_pro.dwUpgradeExp == m_PlayerBaseProperty.dwUpgradeExp &&
		player_pro.wEnergy == m_PlayerBaseProperty.wEnergy &&
		player_pro.wLevel == m_PlayerBaseProperty.wLevel &&
		player_pro.wMp == m_PlayerBaseProperty.wMp &&
		player_pro.wOccuLevel == m_PlayerBaseProperty.wOccuLevel &&
		player_pro.wStamina == m_PlayerBaseProperty.wStamina )
		;
	else
	{
		sprintf(strOutInfo,"(%s)�������Խ������",m_strCDKey.c_str());
	//	PutPlayerDataInfo(strOutInfo);
	//	return;
	}

	//
	CheckGoods(pPlayer);

}

void CCDKey::AddPlayerData(CPlayer* pPlayer)
{
	//��������
	m_PlayerBaseProperty = pPlayer->GetPlayerProperty();
	//ս������
//	m_PlayerFightProperty = pPlayer->GetPlayerFightProperty();
	//ս������

	//Skill
	m_mapSkillEx.clear();
	list<CPlayer::tagSkill> skilllist = pPlayer->GetSkillList();
	if(!skilllist.empty())
	{
		list<CPlayer::tagSkill>::iterator it = skilllist.begin();
		for(;it!=skilllist.end();++it)
		{
			m_mapSkillEx.insert(make_pair((*it).dwID,(*it)));
		}
	}

	//�������ҵϵͳ�е�����
	m_mapTradeCredit.clear();
	map<DWORD,DWORD> mapTradeCredit = pPlayer->GetPlayerTradeCredit();
	map<DWORD,DWORD>::iterator it = mapTradeCredit.begin();
	for(;it != mapTradeCredit.end();++it)
	{
		m_mapTradeCredit[it->first] = it->second;
	}
	//װ��
	if( !m_mapContainerID[EQUIP_ITEM].empty())
	{
		hashGoodsIter it = m_mapContainerID[EQUIP_ITEM].begin();
		for(;it != m_mapContainerID[EQUIP_ITEM].end();it++)
		{
			SAFE_DELETE(it->second.pGoods);
		}
		m_mapContainerID[EQUIP_ITEM].clear();
	}
	for(DWORD i =0;i<CPlayer::EQUIP_SUM;i++)
	{
		CGoods* pGoods = pGoods = pPlayer->GetEquip(i);
		if( pGoods != NULL )
		{
			tagCDKeyGoodsInfo goodsInfo;
			goodsInfo.containerID = EQUIP_ITEM;
			goodsInfo.dwPos = i;
			goodsInfo.wNum = (WORD)1;
			goodsInfo.pGoods = new CGoods(*pGoods);
			//
			m_mapContainerID[EQUIP_ITEM][pGoods->GetExID()] = goodsInfo;
		}
	}
	//ԭʼ����
	if( !m_mapContainerID[MAIN_PACK].empty())
	{
		hashGoodsIter it = m_mapContainerID[MAIN_PACK].begin();
		for(;it != m_mapContainerID[MAIN_PACK].end();it++)
		{
			SAFE_DELETE(it->second.pGoods);
		}
		m_mapContainerID[MAIN_PACK].clear();
	}
	list<CPlayer::tagGoods>* goodsList = pPlayer->GetMainPacket();
	list<CPlayer::tagGoods>::iterator iter = goodsList->begin();
	for( ;iter != goodsList->end();iter++)
	{
		tagCDKeyGoodsInfo goodsInfo;
		goodsInfo.containerID = MAIN_PACK;
		goodsInfo.dwPos = iter->lPos;
		goodsInfo.wNum = iter->lNum;
		goodsInfo.pGoods = new CGoods(*(iter->pGoods));
		m_mapContainerID[MAIN_PACK][iter->pGoods->GetExID()] = goodsInfo;
	}
	//��������Ƿ��б���/�Լ������ڵ���Ʒ����
	if(!m_mapContainerID[MAIN_PACK_ITEM].empty())
	{
		hashGoodsIter itPackOfItem = m_mapContainerID[MAIN_PACK_ITEM].begin();
		for( int i = 1; itPackOfItem != m_mapContainerID[MAIN_PACK_ITEM].end();itPackOfItem++,i++)
		{
			CGoods* pPackItemGoods = itPackOfItem->second.pGoods;
			if( pPackItemGoods)
			{
				hashGoodsIter itGoodsOfPack = m_mapContainerID[MAIN_PACK_ITEM + i].begin();
				for(; itGoodsOfPack != m_mapContainerID[MAIN_PACK_ITEM + i].end();itGoodsOfPack++)
				{
					CGoods* pGoods = itGoodsOfPack->second.pGoods;
					if( pGoods)
						SAFE_DELETE(pGoods);
				}
				m_mapContainerID[MAIN_PACK_ITEM + i].clear();
			}
			SAFE_DELETE(pPackItemGoods);
		}
		m_mapContainerID[MAIN_PACK_ITEM].clear();
	}
	for(int i=0;i<COTE_NUM;i++)
	{
		CPlayer::tagCote* pCote = pPlayer->GetCoteGoods(i);
		if( pCote!=NULL && pCote->pGoods != NULL)
		{
			tagCDKeyGoodsInfo itemGoods;
			itemGoods.containerID = MAIN_PACK_ITEM;
			itemGoods.dwPos = i;
			itemGoods.wNum = 1;
			itemGoods.pGoods = new CGoods(*(pCote->pGoods));
			m_mapContainerID[MAIN_PACK_ITEM][pCote->pGoods->GetExID()] = itemGoods;
			//
			list<CPlayer::tagGoods>::iterator it = pCote->m_listCoteGoods.begin();
			for(;it != pCote->m_listCoteGoods.end();it++)
			{
				tagCDKeyGoodsInfo goodsInfo;
				goodsInfo.containerID = (eContainerID)(MAIN_PACK_ITEM_PACK_1 + i);
				goodsInfo.dwPos = it->lPos;
				goodsInfo.wNum = it->lNum;
				goodsInfo.pGoods = new CGoods(*(it->pGoods));
				m_mapContainerID[MAIN_PACK_ITEM_PACK_1+i][it->pGoods->GetExID()] = goodsInfo;
			}
		}
	}
	//���/����
	m_MoneyAmount = pPlayer->GetPlayerMoneyNum();
	m_SilverAmount = pPlayer->GetPlayerSilverMoneyNum();
	m_BaoShiAmount = pPlayer->GetPlayerBaoShiNum();
	m_JiFenAmount = pPlayer->GetPlayerJiFenNum();
	
	m_MoneyGuid = pPlayer->GetPlayerMoneyGuid();
	m_SilverGuid = pPlayer->GetPlayerSilverGuid();
	m_BaoShiGuid = pPlayer->GetPlayerBaoShiGuid();
	m_JiFenGuid = pPlayer->GetPlayerJiFenGuid();

	AddDepotData(pPlayer);
}
void CCDKey::AddDepotData(CPlayer* pPlayer)
{
	/*
			�ֿⲿ������
	*/
	for(int i=0;i<CPlayer::DEPOT_PACK_NUM;i++)
	{
		hashGoodsIter itDepot = m_mapContainerID[DEPOT_MAIN_PACK+i].begin();
		for(;itDepot != m_mapContainerID[DEPOT_MAIN_PACK+i].end();itDepot++)
		{
			CGoods* pGoods = itDepot->second.pGoods;
			if(pGoods)
				SAFE_DELETE(pGoods);
		}
		m_mapContainerID[DEPOT_MAIN_PACK+i].clear();
	}
	for(int i=0;i<CPlayer::DEPOT_PACK_NUM;i++)
	{
		CPlayer::tagDepotPackData* depotGoods = pPlayer->GetDepotInfo((CPlayer::eDEPOT_PACK_ID)i);
		list<CPlayer::tagGoods>::iterator it = depotGoods->listGoods.begin();
		for(;it != depotGoods->listGoods.end();++it)
		{
			tagCDKeyGoodsInfo goodsInfo;
			goodsInfo.containerID = (eContainerID)(DEPOT_MAIN_PACK + i);
			goodsInfo.dwPos = it->lPos;
			goodsInfo.wNum = it->lNum;
			goodsInfo.pGoods = new CGoods(*(it->pGoods));
			m_mapContainerID[goodsInfo.containerID][goodsInfo.pGoods->GetExID()] = goodsInfo;
		}
	}
	//
	m_DepotMoneyAmount = 0;
	m_DepotSilverAmount = 0;
	m_DepotMoneyGuid = CGUID::GUID_INVALID;
	m_DepotSilverGuid = CGUID::GUID_INVALID;

	m_DepotMoneyAmount = pPlayer->GetDepotMoneyAmount();
	m_DepotSilverAmount = pPlayer->GetDepotSilverMoneyAmount();
	m_DepotMoneyGuid = pPlayer->GetDepotMoneyGuid();
	m_DepotSilverGuid = pPlayer->GetDepotSilverMoneyGuid();

	//д�뱾�ش���
	SaveAllContainerData();
	SaveAllMoney();
	SaveSkill();
	SavePlayerBaseProperty();
}

eCompareGoodsError CCDKey::CompareGoodsData(CGoods* pPlayerGoods,CGoods* pCDKeyGoods)
{
	//�����һ��������
	if( ( pPlayerGoods && pCDKeyGoods == NULL )	||
		( pCDKeyGoods && pPlayerGoods == NULL) )
	{
		return ONE_INVALID;
	}
	//��������ڴ���
	else if( pPlayerGoods == NULL && pCDKeyGoods == NULL )
		return TWO_INVALID;
	//���������
	else
	{
		//��������
		if( pPlayerGoods->GetIndex() == pCDKeyGoods->GetIndex() &&
			pPlayerGoods->GetExID()	 == pCDKeyGoods->GetExID()  &&
			pPlayerGoods->GetNum()	== pCDKeyGoods->GetNum() &&
			pPlayerGoods->GetBaseProperty()->dwValue == pCDKeyGoods->GetBaseProperty()->dwValue &&
			pPlayerGoods->GetBaseProperty()->dwSilverPrice == pCDKeyGoods->GetBaseProperty()->dwSilverPrice &&
			pPlayerGoods->GetBaseProperty()->dwBuyPrice == pCDKeyGoods->GetBaseProperty()->dwBuyPrice &&
			pPlayerGoods->GetBaseProperty()->dwType == pCDKeyGoods->GetBaseProperty()->dwType )
		{
			return RESULT_NO_ERROR;
		}
		return RESULT_ERROR;
	}
}

/*
	д�����ļ�
*/
void CCDKey::SaveSkill()
{			
	std::ofstream out_f;
	string strPath = "PlayerData//";
	string strCDKeyFolder = strPath + m_strCDKey;
	//��CDKeyΪ�������ļ���
	CreateDirectory(strCDKeyFolder.c_str(),NULL);
	//�ļ���
	string strFileName = strCDKeyFolder + "//Skill.ini";
	out_f.open(strFileName.c_str(),ios::out);
	//д����
	out_f.write("#���	",			 sizeof("���"));
	out_f.write("�ȼ�	",			sizeof("�ȼ�"));
	out_f.write("��ȴʱ��	", sizeof("��ȴʱ��"));
	out_f.write("��ʼ������Чʱ��	", sizeof("��ʼ������Чʱ��"));
	out_f.write("ͼ����ʱ��	", sizeof("ͼ����ʱ��"));
	out_f.write("������ʼʱ��	", sizeof("������ʼʱ��"));
	out_f.write("����ʱ��	", sizeof("����ʱ��"));
	out_f.write("��С����	", sizeof("��С����"));
	out_f.write("������	", sizeof("������"));
	out_f.write("����mp	",	sizeof("����mp"));
	out_f.write("�Ƿ���ȴ��	",	sizeof("�Ƿ���ȴ��"));
	out_f.write("���������ʱ��	", sizeof("���������ʱ��"));
	out_f.write("��ʼʹ��ʱ��	", sizeof("��ʼʹ��ʱ��"));
	out_f.write("#\n	",	sizeof("#\n"));	

	char strOut[128];
	hash_map<DWORD,CPlayer::tagSkill>::iterator it = m_mapSkillEx.begin();
	for(;it != m_mapSkillEx.end();it++)
	{
		sprintf(strOut,"%d",it->second.dwID);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.wLevel);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwRestoreTime);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwCoolDownStartTime);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwCoolDownGrayStartTime);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwIntonateStartTime);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwIntonateTime);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.wAffectRangeMin);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.wAffectRangeMax);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.wCostMP);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.bIsPublicCoolEffect);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwUseRestoreTime);
		WriteOutInfo(out_f,strOut);

		sprintf(strOut,"%d",it->second.dwUseingStartTime);
		WriteOutInfo(out_f,strOut);
		
		//����
		out_f.put('\n');
	}
	out_f.close();
}
void CCDKey::SavePlayerBaseProperty()
{
	std::ofstream out_f;
	string strPath = "PlayerData//";
	string strCDKeyFolder = strPath + m_strCDKey;
	//��CDKeyΪ�������ļ���
	CreateDirectory(strCDKeyFolder.c_str(),NULL);
	//�ļ���
	string strFileName = strCDKeyFolder + "//PlayerBaseProperty.ini";
	out_f.open(strFileName.c_str(),ios::out);
	//д����
	out_f.write("ְҵ	",	 sizeof("ְҵ"));
	out_f.write("��ְҵ	",		sizeof("��ְҵ"));
	out_f.write("�Ա�	", sizeof("�Ա�"));
	out_f.write("��������	", sizeof("��������"));
	out_f.write("ͷ��ͼƬID	", sizeof("ͷ��ͼƬID"));
	out_f.write("�沿ͼƬID 	", sizeof("�沿ͼƬID "));
	out_f.write("��ǰ����ֵ	", sizeof("��ǰ����ֵ"));
	out_f.write("��ǰħ��ֵ	", sizeof("��ǰħ��ֵ"));
	out_f.write("��ǰ����ֵ	", sizeof("��ǰ����ֵ"));
	out_f.write("��ǰ����	",	 sizeof("��ǰ����"));
	out_f.write("�ȼ�	",		sizeof("�ȼ�"));
	out_f.write("��ǰ����ֵ	", sizeof("��ǰ����ֵ"));
	out_f.write("���;���	", sizeof("���;���"));
	out_f.write("��ǰְҵ����	", sizeof("��ǰְҵ����"));
	out_f.write("�������辭��ֵ 	", sizeof("�������辭��ֵ "));
	out_f.write("��ҵ�ְҵ�ȼ�	", sizeof("��ҵ�ְҵ�ȼ�"));
	out_f.write("��λ�ȼ�	", sizeof("��λ�ȼ�"));
	out_f.write("��λ������ֵ	", sizeof("��λ������ֵ"));
	out_f.write("Ӷ���ȼ�	",	 sizeof("Ӷ���ȼ�"));
	out_f.write("Ӷ��������ֵ	",		sizeof("Ӷ��������ֵ"));
	out_f.write("����	", sizeof("����"));
	out_f.write("���ҹ���	", sizeof("���ҹ���"));
	out_f.write("��żID	", sizeof("��żID"));
	out_f.write("����ż�Ĺ�ϵֵ	", sizeof("����ż�Ĺ�ϵֵ"));
	out_f.write("��ҵ�ȼ�	", sizeof("��ҵ�ȼ�"));
	out_f.write("��ҵ����	", sizeof("��ҵ����"));
	out_f.write("��������	", sizeof("��������"));
	out_f.write("�����ȼ�	",	 sizeof("�����ȼ�"));
	out_f.write("��������	",		sizeof("��������"));
	out_f.write("PKɱ����������	", sizeof("PKɱ����������"));
	out_f.write("PKֵ	", sizeof("PKֵ"));
	out_f.write("PVPɱ���������	", sizeof("PVPɱ���������"));
	out_f.write("PVPֵ 	", sizeof("PVPֵ "));
	out_f.write("PK����	", sizeof("PK����	"));
	out_f.write("��ǰӵ�м��ܵ���	", sizeof("��ǰӵ�м��ܵ���"));
	out_f.write("�ϴ��˳���Ϸʱ��	", sizeof("�ϴ��˳���Ϸʱ��"));
		out_f.write("#\n	",	sizeof("#\n"));	

	char strOut[128];
	sprintf(strOut,"%d",m_PlayerBaseProperty.byOccu);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byAssOccu);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.bySex);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byClass);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byHairPic);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byFacePic);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwHp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.wMp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.wEnergy);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.wStamina);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.wLevel);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwExp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwPresentExp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwOccuPoint);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwUpgradeExp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.wOccuLevel);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byRankOfNobility);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwRankOfNobCredit);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byRankOfMercenary);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwMercenaryCredit);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byCountry);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwCountryContribute);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwSpouseID);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwSpouseParam);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byBusinessLevel);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwBusinessExp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwArtisanCredit);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byArtisanLevel);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwArtisanExp);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwPkCount);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwPkValue);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwPVPCount);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwPVPValue);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.byPKOnOff);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwSkillPoint);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_PlayerBaseProperty.dwLastExitGameTime);
	WriteOutInfo(out_f,strOut);

	//����
	out_f.close();
}
void CCDKey::SaveAllMoney()
{
	std::ofstream out_f;
	string strPath = "PlayerData//";
	string strCDKeyFolder = strPath + m_strCDKey;
	//��CDKeyΪ�������ļ���
	CreateDirectory(strCDKeyFolder.c_str(),NULL);
	//�ļ���
	string strFileName = strCDKeyFolder + "//AllMoney.ini";
	out_f.open(strFileName.c_str(),ios::out);
	//д����
	out_f.write("#GUID	",		sizeof("#GUID"));
	out_f.write("����	",	sizeof("����"));
	out_f.write("����λ��	",	sizeof("����λ��"));
	out_f.write("����	",		sizeof("����"));
		out_f.write("#\n	",	sizeof("#\n"));	

	char strOut[128];
	//���
	m_MoneyGuid.tostring(strOut);
	WriteOutInfo(out_f,strOut);

	WriteOutInfo(out_f,"���");

	sprintf(strOut,"%d",PACKET);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_MoneyAmount);
	WriteOutInfo(out_f,strOut);

	out_f.put('\n');
	//����
	m_SilverGuid.tostring(strOut);
	WriteOutInfo(out_f,strOut);

	WriteOutInfo(out_f,"����");

	sprintf(strOut,"%d",PACKET);
	WriteOutInfo(out_f,"0");

	sprintf(strOut,"%d",m_SilverAmount);
	WriteOutInfo(out_f,strOut);

	out_f.put('\n');
//////////////////////////////////////////////////////////////////////////
	m_DepotMoneyGuid.tostring(strOut);
	WriteOutInfo(out_f,strOut);

	WriteOutInfo(out_f,"���");

	sprintf(strOut,"%d",DEPOT);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_DepotMoneyAmount);
	WriteOutInfo(out_f,strOut);

	out_f.put('\n');
	//
	m_DepotSilverGuid.tostring(strOut);
	WriteOutInfo(out_f,strOut);

	WriteOutInfo(out_f,"����");

	sprintf(strOut,"%d",DEPOT);
	WriteOutInfo(out_f,strOut);

	sprintf(strOut,"%d",m_DepotSilverAmount);
	WriteOutInfo(out_f,strOut);

	//����
	out_f.close();
}
void CCDKey::SaveAllContainerData()
{
	std::ofstream out_f;
	string strPath = "PlayerData//";
	string strCDKeyFolder = strPath + m_strCDKey;
	//��CDKeyΪ�������ļ���
	CreateDirectory(strCDKeyFolder.c_str(),NULL);
	//�ļ���
	string strFileName = strCDKeyFolder + "//ContainerGoodsInfo.ini";
	//���ļ�
	out_f.open(strFileName.c_str(),ios::out);
	//д����
	out_f.write("GUID	",		sizeof("#GUID"));
	out_f.write("��������ID	",	sizeof("��������ID"));
	out_f.write("����������	",	sizeof("����������"));
	out_f.write("�ص�����	",	sizeof("�ص�����"));
	out_f.write("��Ʒ����	",	sizeof("��Ʒ����"));
	out_f.write("��Ʒ����	",	sizeof("��Ʒ����"));
	out_f.write("��Ҽ۸�	",	sizeof("��Ҽ۸�"));
	out_f.write("���Ҽ۸�	",	sizeof("���Ҽ۸�"));
	out_f.write("����۸�	",	sizeof("����۸�"));
	out_f.write("��Ʒ����	",	sizeof("��Ʒ����"));
	out_f.write("#\n	",	sizeof("#\n"));	
	//����ÿ������,д������
	for(int i=0;i<CONTAINER_NUM;i++)
	{
		hashGoodsIter itGoodsInfo = m_mapContainerID[(eContainerID)i].begin();
		for(;itGoodsInfo != m_mapContainerID[(eContainerID)i].end();itGoodsInfo++)
		{
			tagCDKeyGoodsInfo goodsInfo = itGoodsInfo->second;
			CGoods* pGoods = goodsInfo.pGoods;
			//
			if(pGoods)
			{
				char strOut[128];
				//GUID
				CGUID guid = pGoods->GetExID();
				guid.tostring(strOut);
				WriteOutInfo(out_f,strOut);
				//����ID
				sprintf(strOut,"%d",goodsInfo.containerID);
				WriteOutInfo(out_f,strOut);
				//����������
				sprintf(strOut,"%d",goodsInfo.dwPos);
				WriteOutInfo(out_f,strOut);
				//�ص�����
				sprintf(strOut,"%d",goodsInfo.wNum);
				WriteOutInfo(out_f,strOut);
				//��Ʒ����
				sprintf(strOut,"%d",pGoods->GetIndex());
				WriteOutInfo(out_f,strOut);
				//��Ʒ����
				sprintf(strOut,"%d",pGoods->GetNum());
				WriteOutInfo(out_f,strOut);
				//��Ҽ۸�
				sprintf(strOut,"%d",pGoods->GetBaseProperty()->dwValue);
				WriteOutInfo(out_f,strOut);
				//���Ҽ۸�
				sprintf(strOut,"%d",pGoods->GetBaseProperty()->dwSilverPrice);
				WriteOutInfo(out_f,strOut);
				//����۸�
				sprintf(strOut,"%d",pGoods->GetBaseProperty()->dwBuyPrice);
				WriteOutInfo(out_f,strOut);
				//��Ʒ����
				sprintf(strOut,"%d",pGoods->GetBaseProperty()->dwType);
				WriteOutInfo(out_f,strOut);
				//����
				out_f.put('\n');
			}
		}
	}
	out_f.close();
}
void CCDKey::WriteOutInfo(std::ofstream & out_f,const char* strOutInfo)
{
	out_f.write(strOutInfo,strlen(strOutInfo));
	out_f.put('	');
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	�������ļ�
*/
void CCDKey::GetAllContainerData()
{
	std::ifstream in_f;
	const string strPath = "PlayerData//";
	string strFileName = strPath + m_strCDKey + "//ContainerGoodsInfo.ini";
	in_f.open(strFileName.c_str());
	string strFlag;
	while(true)
	{
		in_f>>strFlag;
		if( strFlag == "#")
			break;
	}
	//
	while( true)
	{
		if( in_f.eof() )	break;
		string str[10];
		in_f>>str[0]	>>str[1]	>> str[2]	>>str[3]	>>str[4]
		>>str[5]	>>str[6]	>>str[7]	>> str[8]	>>str[9];
		if(str->empty())
			return;

		tagCDKeyGoodsInfo goodsInfo;
		goodsInfo.containerID = (eContainerID)atoi(str[1].c_str());
		goodsInfo.dwPos = atoi(str[2].c_str());
		goodsInfo.wNum = atoi(str[3].c_str());
		CGoods* pGoods = goodsInfo.pGoods = new CGoods;
		CGUID goodsGuid(str[0].c_str());
		pGoods->SetExID(goodsGuid);
		pGoods->SetIndex(atoi(str[4].c_str()));
		pGoods->SetNum(atoi(str[5].c_str()));
		pGoods->GetBaseProperty()->dwValue = atoi(str[6].c_str());
		pGoods->GetBaseProperty()->dwSilverPrice = atoi(str[7].c_str());
		pGoods->GetBaseProperty()->dwBuyPrice = atoi(str[8].c_str());
		pGoods->GetBaseProperty()->dwType = atoi(str[9].c_str());

		m_mapContainerID[goodsInfo.containerID][goodsGuid] = goodsInfo;
	}
	in_f.close();
}

void CCDKey::GetAllMoney()
{
	std::ifstream in_f;
	const string strPath = "PlayerData//";
	string strFileName = strPath + m_strCDKey + "//AllMoney.ini";
	in_f.open(strFileName.c_str());
	string strFlag;
	while(true)
	{
		in_f>>strFlag;
		if( strFlag == "#")
			break;
	}
	while( true)
	{
		if( in_f.eof() )	break;
		string str[4];
		in_f>>str[0]	>>str[1]	>> str[2]	>>str[3];
		//>>str[4]	>>str[5]	>>str[6]	>>str[7]	>> str[8]	>>str[9];
		if(str->empty())
			return;

		if( atoi(str[2].c_str()) == DEPOT )
		{
			if( atoi(str[1].c_str()) == GOLD)
			{
				m_DepotMoneyAmount = atoi(str[3].c_str());
				m_DepotMoneyGuid = CGUID(str[0].c_str());
			}
			else if( atoi(str[1].c_str()) == SILVER)
			{
				m_DepotSilverAmount = atoi(str[3].c_str());
				m_DepotSilverGuid = CGUID(str[0].c_str());
			}
		}
		else if( atoi(str[2].c_str()) == PACKET )
		{
			if(atoi(str[1].c_str()) == GOLD)
			{
				m_MoneyAmount = atoi(str[3].c_str());
				m_MoneyGuid = CGUID(str[0].c_str());
			}
			else if(atoi(str[1].c_str()) == SILVER )
			{
				m_SilverAmount = atoi(str[3].c_str());
				m_SilverGuid = CGUID(str[0].c_str());
			}
		}
	}
	in_f.close();
}

void CCDKey::GetPlayerBaseProperty()
{
	std::ifstream in_f;
	const string strPath = "PlayerData//";
	string strFileName = strPath + m_strCDKey + "//PlayerBaseProperty.ini";
	in_f.open(strFileName.c_str());
	string strFlag;
	while(true)
	{
		in_f>>strFlag;
		if( strFlag == "#")
			break;
	}

	string str[36];
	in_f>>str[0]	>>str[1]	>> str[2]	>>str[3]	>>str[4]
	>>str[5]	>>str[6]	>>str[7]	>> str[8]	>>str[9]
	>>str[10]	>>str[11]	>> str[12]	>>str[13]	>>str[14]
	>>str[15]	>>str[16]	>>str[17]	>> str[18]	>>str[19]
	>>str[20]	>>str[21]	>> str[22]	>>str[23]	>>str[24]
	>>str[25]	>>str[26]	>>str[27]	>> str[28]	>>str[29]
	>>str[30]	>>str[31]	>> str[32]	>>str[33]	>>str[34]
	>>str[35];//	>>str[36]	>>str[37]	>> str[38]	>>str[39];
	if(str->empty())
		return;

	m_PlayerBaseProperty.byOccu	= (eOccupation)atoi(str[0].c_str());					//ְҵ
	m_PlayerBaseProperty.byAssOccu	= (eDeputyOccu)atoi(str[1].c_str());				//��ְҵ
	m_PlayerBaseProperty.bySex	= atoi(str[2].c_str());					//�Ա�(0:�� 1:Ů) 
	m_PlayerBaseProperty.byClass	= atoi(str[3].c_str());				//��������
	m_PlayerBaseProperty.byHairPic	= atoi(str[4].c_str());				//ͷ��ͼƬID 
	m_PlayerBaseProperty.byFacePic	= atoi(str[5].c_str());				//�沿ͼƬID 
	m_PlayerBaseProperty.dwHp	= atoi(str[6].c_str());					//��ǰ����ֵ 
	m_PlayerBaseProperty.wMp	= atoi(str[7].c_str());					//��ǰħ��ֵ 
	m_PlayerBaseProperty.wEnergy	= atoi(str[8].c_str());				//��ǰ����ֵ 
	m_PlayerBaseProperty.wStamina	= atoi(str[9].c_str());				//��ǰ����	
	m_PlayerBaseProperty.wLevel	= atoi(str[10].c_str());					//�ȼ� 
	m_PlayerBaseProperty.dwExp	= atoi(str[11].c_str());					//��ǰ����ֵ
	m_PlayerBaseProperty.dwPresentExp	= atoi(str[12].c_str());			//���;���
	m_PlayerBaseProperty.dwOccuPoint	= atoi(str[13].c_str());			//��ǰְҵ����
	m_PlayerBaseProperty.dwUpgradeExp	= atoi(str[14].c_str());			//�������辭��ֵ
	m_PlayerBaseProperty.wOccuLevel	= atoi(str[15].c_str());				//��ҵ�ְҵ�ȼ�
	m_PlayerBaseProperty.byRankOfNobility	= atoi(str[16].c_str());		//��λ�ȼ�
	m_PlayerBaseProperty.dwRankOfNobCredit	= atoi(str[17].c_str());		//��λ������ֵ
	m_PlayerBaseProperty.byRankOfMercenary	= atoi(str[18].c_str());		//Ӷ���ȼ�
	m_PlayerBaseProperty.dwMercenaryCredit	= atoi(str[19].c_str());		//Ӷ��������ֵ
	m_PlayerBaseProperty.byCountry	= atoi(str[20].c_str());				//����ID
	m_PlayerBaseProperty.dwCountryContribute	= atoi(str[21].c_str());	//�Թ��ҵĹ���ֵs
	m_PlayerBaseProperty.dwSpouseID	= atoi(str[22].c_str());				//��ż���ID 
	m_PlayerBaseProperty.dwSpouseParam	= atoi(str[23].c_str());			//����ż�Ĺ�ϵֵ
	m_PlayerBaseProperty.byBusinessLevel	= atoi(str[24].c_str());		//��ҵ�ȼ�
	m_PlayerBaseProperty.dwBusinessExp	= atoi(str[25].c_str());			//��ҵ����
	m_PlayerBaseProperty.dwArtisanCredit	= atoi(str[26].c_str());		//��������
	m_PlayerBaseProperty.byArtisanLevel	= atoi(str[27].c_str());			//�����ȼ�
	m_PlayerBaseProperty.dwArtisanExp	= atoi(str[28].c_str());			//��������
	m_PlayerBaseProperty.dwPkCount	= atoi(str[29].c_str());				//���PKɱ����������
	m_PlayerBaseProperty.dwPkValue	= atoi(str[30].c_str());				//���PKֵ(����ֵ)
	m_PlayerBaseProperty.dwPVPCount	= atoi(str[31].c_str());				//���PVPɱ���������
	m_PlayerBaseProperty.dwPVPValue	= atoi(str[32].c_str());				//���PVPֵ(����ֵ)
	m_PlayerBaseProperty.byPKOnOff	= atoi(str[33].c_str());				//PK����(b1:ȫ�屣��,b2:��ӱ���,b4:���ᱣ��,b8:��������,b16:��������,b32:��Ӫ����,b64.��������)
	m_PlayerBaseProperty.dwSkillPoint	= atoi(str[34].c_str());			//��ǰӵ�еļ��ܵ���(SP)
	m_PlayerBaseProperty.dwLastExitGameTime	= atoi(str[35].c_str());		//��һ���˳���Ϸ��ʱ��

	in_f.close();
}

void CCDKey::GetSkill()
{
	std::ifstream in_f;
	const string strPath = "PlayerData//";
	string strFileName = strPath + m_strCDKey + "//Skill.ini";
	in_f.open(strFileName.c_str());
	string strFlag;
	while(true)
	{
		in_f>>strFlag;
		if( strFlag == "#")
			break;
	}

	while( true)
	{
		if( in_f.eof() )	break;
		string str[13];
		in_f>>str[0]	>>str[1]	>> str[2]	>>str[3]	>>str[4]
		>>str[5]	>>str[6]	>>str[7]	>> str[8]	>>str[9]
		>>str[10]	>>str[11]	>> str[12]	>>str[12];
		if(str->empty())
			return;

		CPlayer::tagSkill skill;
		skill.dwID = atoi(str[0].c_str());									//	���
		skill.wLevel = atoi(str[1].c_str());								//	�ȼ�
		skill.dwRestoreTime = atoi(str[2].c_str());						//	��ȴʱ�� (����)
		skill.dwCoolDownStartTime = atoi(str[3].c_str());				//	��ʼ������ȴ��Чʱ�� timeGetTime (Ϊ0��ʾ����ʾCoolDown��Ч)
		skill.dwCoolDownGrayStartTime = atoi(str[4].c_str());				//	ͼ����ʱ���ʱ�䣬�������3���ɫ����ô�Զ��л�����
		skill.dwIntonateStartTime = atoi(str[5].c_str());					//	����������ʼʱ��
		skill.dwIntonateTime = atoi(str[6].c_str());						//	���ܵ�����ʱ��
		skill.wAffectRangeMin = atoi(str[7].c_str());							//	��С�ͷž���
		skill.wAffectRangeMax = atoi(str[8].c_str());					//	����ͷž���
		skill.wCostMP = atoi(str[9].c_str());								//	���ĵ�MP
		skill.bIsPublicCoolEffect = atoi(str[10].c_str()) == 0?false:true;				//	�Ƿ��ڹ�����ȴʱ����
		skill.dwUseRestoreTime = atoi(str[11].c_str());							//	ʹ�õĹ�����,�ڿ�����ϲ���ʹ��Ч���ĳ���ʱ��
		skill.dwUseingStartTime = atoi(str[12].c_str());								//	��ʼʹ�øü��ܵ�ʱ��

		m_mapSkillEx[skill.dwID] = skill;
	}
	in_f.close();
}

