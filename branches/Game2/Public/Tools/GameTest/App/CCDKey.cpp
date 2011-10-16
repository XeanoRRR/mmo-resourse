#pragma once
#include "StdAfx.h"
#include "CCDKey.h"
#include "Goods.h"
#include "SkillMgr.h"
#include "EquipMgr.h"
#include "GoodsMgr.h"

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
	m_lLevel = 0;
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
	ClearAllSaveMailData();
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
	in_f.close();
}
void CCDKey::InitCDKeyData(CCDKey* pCDKey)
{
	if(pCDKey)
	{
		std::ifstream in_f;
		char filename[128];
		sprintf(filename,"PlayerData//%s//GoodsAmountByIndex.ini",pCDKey->m_strCDKey.c_str());
		in_f.open(filename);
		if( !in_f.is_open())
			return;

		string strFlag;
		in_f>>strFlag>>strFlag>>strFlag;
		//
		while( true)
		{
			if( in_f.eof() )
				break;
			DWORD dwIndex,dwAmount;
			in_f>>dwIndex
				>>dwAmount;

			pCDKey->m_mapGoodsAmountByIndex[dwIndex] = dwAmount;
		}
		in_f.close();

		std::ifstream in_f2;
		sprintf(filename,"PlayerData//%s//Level.txt",pCDKey->m_strCDKey.c_str());
		in_f2.open(filename);
		while (1)
		{
			if( !in_f2.is_open() )
				break;

			if( in_f2.eof() )
				break;
			DWORD l;
			in_f2>>l;
			pCDKey->SetLevel(l);
			break;
		}
		in_f2.close();
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
	return NULL;
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

void CCDKey::CheckPlayerData(CPlayer* pPlayer,eSaveDataType eType)
{
	switch(eType)
	{
	case eSAVE_GOODS:
		CheckGoods(pPlayer);
		break;
	case eSAVE_MAIL:
		CheckMailData(pPlayer);
		break;
	case eSAVE_LINKMAN:
		CheckSaveLinkManPanleGroup(pPlayer);
		break;
	case eSAVE_FACTION:
		CheckFaction(pPlayer);
		break;
	case eSAVE_LEVEL:
		CheckLevel(pPlayer);
		break;
	case eSAVE_EQUIP:
		CheckEquip(pPlayer);
		//CheckGoods(pPlayer);
		break;
	default:
		break;
	}
}
void CCDKey::AddPlayerData(CPlayer* pPlayer,eSaveDataType eType)
{
	pPlayer->GetExID().tostring(szPlayerGuid);

	switch(eType)
	{
	case eSAVE_MAIL:
		SavaMailData(pPlayer);
		break;
	case eSAVE_GOODS:
		SaveGoodsData(pPlayer);
		break;
	case eSAVE_QUEST:
		SaveQuest(pPlayer);
		break;
	case eSAVE_SKILL:
		SaveSkill(pPlayer);
		break;
	case eSAVE_LINKMAN:
		AddSaveLinkManPanleGroupByTxt(pPlayer);
		break;
	case eSAVE_FACTION:
		AddSaveFactionByTxt(pPlayer);
		break;
	case eSAVE_LEVEL:
		SaveLevel(pPlayer);
		break;
	case eSAVE_EQUIP:
		SaveEquip(pPlayer);
		SaveGoodsData(pPlayer);
		break;
	default:
		break;
	}
}

void CCDKey::SaveEquip(CPlayer* pPlyaer)
{
	std::ofstream out_f;
	string strPath = "PlayerData//";
	string strCDKeyFolder = strPath + m_strCDKey;
	//��CDKeyΪ�������ļ���
	CreateDirectory(strCDKeyFolder.c_str(),NULL);
	//�ļ���
	string strFileName = strCDKeyFolder + "//EquipInfo.ini";
	//���ļ�
	out_f.open(strFileName.c_str(),ios::out);

	for(int i=0;i<EQUIP_SUM;i++)
	{
		CGoods* pGoods = pPlyaer->GetGoodsMgr()->GetEquipMgr()->GetEquip(i);
		if( pGoods != NULL )
		{
			char strInput[128];
			//equip item pos
			sprintf(strInput,"%d",i);
			WriteOutInfo(out_f,strInput);
			//index
			sprintf(strInput,"%d",pGoods->GetIndex());
			WriteOutInfo(out_f,strInput);
			//����������
			pGoods->GetExID().tostring(strInput);
			WriteOutInfo(out_f,strInput);
			//����
			out_f.put('\n');
			//����ÿ������,д������

			tagCDKeyEquipInfo * pInfo = new tagCDKeyEquipInfo;
			pInfo->dwGoodsIndex = pGoods->GetIndex();
			pInfo->dwItemPos = i;
			strcpy(pInfo->szGuid,strInput);

			m_mapCDKeyEquipInfo[i] = pInfo;
		}
	}
}

void CCDKey::CheckEquip(CPlayer* pPlayer)
{
	char szGuid[256];
	pPlayer->GetExID().tostring(szGuid);
	if( strcmp(szGuid,szPlayerGuid) )
	{
		char szInfo[512];
		//		OutputContainerInfo(m_strCDKey.c_str());
		//		OutputContainerInfo(pPlayer->GetName());
		sprintf(szInfo,"OldGuid:%s",szPlayerGuid);
		//		OutputContainerInfo(szInfo);

		sprintf(szInfo,"NewGuid:%s",szGuid);
		// 		OutputContainerInfo(szInfo);

		//		OutputContainerInfo("ɾ��");
		return ;
	}
	DWORD dwEquipCounts = 0;
	//	�������ÿ��װ����λ
	for(int i=0;i<EQUIP_SUM;i++)
	{
		CGoods* pGoods = pPlayer->GetGoodsMgr()->GetEquipMgr()->GetEquip(i);
		if( pGoods != NULL )
		{
			dwEquipCounts++;
		}
	}
	if( m_mapCDKeyEquipInfo.size() != dwEquipCounts )
	{
		// 		OutputContainerInfo(m_strCDKey.c_str());
		//		OutputContainerInfo(pPlayer->GetName());
		//		OutputContainerInfo(szPlayerGuid);
		//		OutputContainerInfo("��ʧ");
		//	MessageBox(NULL,"Equip Lost!","Msg",MB_OK);
	}
	return;
	// 	{
	// 		{
	// 			//	�ڴ��������в���
	// 			map<DWORD,tagCDKeyEquipInfo*>::iterator it = m_mapCDKeyEquipInfo.find(i);
	// 			if( it != m_mapCDKeyEquipInfo.end() )
	// 			{
	// 				if( (*it).second->dwGoodsIndex != pGoods->GetIndex() )
	// 				{
	// 					OutputContainerInfo(m_strCDKey.c_str());
	// 					char szInfo[256];
	// 					sprintf(szInfo,"װ����λ��:%d",i);
	// 					OutputContainerInfo(szInfo);
	// 
	// 					sprintf(szInfo,"����Index:%d",(*it).second->dwGoodsIndex );
	// 					OutputContainerInfo(szInfo);
	// 
	// 					sprintf(szInfo,"�ڴ�Index:%d",pGoods->GetIndex());
	// 					OutputContainerInfo(szInfo);
	// 				}
	// 
	// 				char szGUid1[128];
	// 				char szGudi2[128];
	// 				pGoods->GetExID().tostring(szGUid1);
	// 				strcpy(szGudi2,(*it).second->szGuid);
	// 
	// 				if( strcmp(szGudi2,szGUid1))
	// 				{
	// 					OutputContainerInfo(m_strCDKey.c_str());
	// 					char szInfo[256];
	// 					sprintf(szInfo,"װ����λ��:%d",i);
	// 					OutputContainerInfo(szInfo);
	// 
	// 					sprintf(szInfo,"����Guid:%s",szGudi2);
	// 					OutputContainerInfo(szInfo);
	// 
	// 					sprintf(szInfo,"�ڴ�Guid:%s",szGUid1);
	// 					OutputContainerInfo(szInfo);
	// 				}
	// 			}
	// 		}
	// 	}
}

void CCDKey::SaveLevel(CPlayer * pPlayer)
{
// 	m_lLevel = pPlayer->GetLevel();
// 	std::ofstream out_f;
// 	string strPath = "PlayerData//";
// 	string strCDKeyFolder = strPath + m_strCDKey;
// 	//��CDKeyΪ�������ļ���
// 	CreateDirectory(strCDKeyFolder.c_str(),NULL);
// 	//�ļ���
// 	string strFileName = strCDKeyFolder + "//Level.txt";
// 	//���ļ�
// 	out_f.open(strFileName.c_str(),ios::out);
// 
// 	char strInput[128];
// 	sprintf(strInput,"%d",m_lLevel);
// 	WriteOutInfo(out_f,strInput);
// 	out_f.close();
}

void CCDKey::CheckLevel(CPlayer * pPlayer)
{
// 	if( m_lLevel != pPlayer->GetLevel() )
// 	{
// 		char strOutInfo[128];
// 		sprintf(strOutInfo,"%s,ԭ��%d <<--->> ��:%d",m_strCDKey.c_str(),m_lLevel,pPlayer->GetLevel());
// 		//		PutPlayerDataInfo(strOutInfo);
// 	}
}


void CCDKey::SaveGoodsData(CPlayer* pPlayer)
{
	//���
	m_mapGoodsAmountByIndex.clear();
	//װ��vld
	// 	for(int i=0;i<CPlayer::EQUIP_SUM;i++)
	// 	{
	// 		CGoods* pEquip = pPlayer->GetEquip(i);
	// 		if(pEquip!= NULL)
	// 		{
	// 			AddAmountByGoods(pEquip);
	// 		}
	// 	}
	//ԭʼ����
// 	list<tagGoods>* goodsList = pPlayer->GetMainPacket();
// 	list<tagGoods>::iterator iter = goodsList->begin();
// 	for(;iter != goodsList->end();iter++)
// 	{
// 		CGoods* pGoods = iter->pGoods;
// 		if(pGoods!=NULL)
// 			SaveGoodsCounts(pGoods);
// 	}
// 	//�ӱ���
// 	for(int i=0;i<COTE_NUM;i++)
// 	{
// 		tagCote* pCote = pPlayer->GetCoteGoods(i);
// 		if( pCote!=NULL && pCote->pGoods != NULL)
// 		{
// 			SaveGoodsCounts(pCote->pGoods);
// 			//
// 			list<tagGoods>::iterator it = pCote->m_listCoteGoods.begin();
// 			for(;it != pCote->m_listCoteGoods.end();it++)
// 			{
// 				SaveGoodsCounts(it->pGoods);
// 			}
// 		}
// 	}
// 
// 	std::ofstream out_f;
// 	string strPath = "PlayerData//";
// 	string strCDKeyFolder = strPath + m_strCDKey;
// 	//��CDKeyΪ�������ļ���
// 	CreateDirectory(strCDKeyFolder.c_str(),NULL);
// 	//�ļ���
// 	string strFileName = strCDKeyFolder + "//GoodsAmountByIndex.ini";
// 	//���ļ�
// 	out_f.open(strFileName.c_str(),ios::out);
// 	//д����
// 	out_f.write("Index	",		sizeof("#Index"));
// 	out_f.write("����	",		sizeof("����"));
// 	out_f.write("#",		sizeof("#"));
// 	//����
// 	out_f.put('\n');	
// 	//����ÿ������,д������
// 	map<DWORD,DWORD>::iterator Iter = m_mapGoodsAmountByIndex.begin();
// 	for(;Iter!=m_mapGoodsAmountByIndex.end();Iter++)
// 	{
// 		char strInput[128];
// 		//index
// 		sprintf(strInput,"%d",Iter->first);
// 		WriteOutInfo(out_f,strInput);
// 		//����������
// 		sprintf(strInput,"%d",Iter->second);
// 		WriteOutInfo(out_f,strInput);
// 		//����
// 		out_f.put('\n');
// 	}
// 	out_f.close();
}

void CCDKey::CheckGoods(CPlayer* pPlayer)
{
	//����һ����ʱmap���player����index��Ʒ��������Ϣ
	map<DWORD,DWORD> mapGoodsTemp;
	// 	//װ��
	// 	for(int i=0;i<CPlayer::EQUIP_SUM;i++)
	// 	{
	// 		CGoods* pEquip = pPlayer->GetEquip(i);
	// 		if(pEquip!= NULL)
	// 		{
	// 			DWORD dwGoodsIndex = pEquip->GetIndex();
	// 			DWORD dwGoodsNum = pEquip->GetNum();
	// 
	// 			//����map���Ƿ��и�index����Ʒ
	// 			map<DWORD,DWORD>::iterator it = mapGoodsTemp.find(dwGoodsIndex);
	// 
	// 			//�ҵ�,���������ϵݼ�
	// 			if( it!=mapGoodsTemp.end())
	// 			{
	// 
	// 				DWORD dwNum = it->second;
	// 				mapGoodsTemp[dwGoodsIndex] = dwGoodsNum + dwNum;
	// 			}
	// 			//û���ҵ�,���¼���
	// 			else
	// 			{
	// 				mapGoodsTemp[dwGoodsIndex] = dwGoodsNum;
	// 			}
	// 		}
	// 	}
	//ԭʼ����
// 	list<tagGoods>* goodsList = pPlayer->GetMainPacket();
// 	list<tagGoods>::iterator iter = goodsList->begin();
// 	for(;iter != goodsList->end();iter++)
// 	{
// 		CGoods* pGoods = iter->pGoods;
// 		if(pGoods!=NULL)
// 		{
// 			DWORD dwGoodsIndex = pGoods->GetIndex();
// 			DWORD dwGoodsNum = pGoods->GetNum();
// 
// 			//����map���Ƿ��и�index����Ʒ
// 			map<DWORD,DWORD>::iterator it = mapGoodsTemp.find(dwGoodsIndex);
// 
// 			//�ҵ�,���������ϵݼ�
// 			if( it!=mapGoodsTemp.end())
// 			{
// 
// 				DWORD dwNum = it->second;
// 				mapGoodsTemp[dwGoodsIndex] = dwGoodsNum + dwNum;
// 			}
// 			//û���ҵ�,���¼���
// 			else
// 			{
// 				mapGoodsTemp[dwGoodsIndex] = dwGoodsNum;
// 			}
// 		}
// 	}
// 	//�ӱ���
// 	for(int i=0;i<COTE_NUM;i++)
// 	{
// 		tagCote* pCote = pPlayer->GetCoteGoods(i);
// 		if( pCote!=NULL && pCote->pGoods != NULL)
// 		{
// 			DWORD dwGoodsIndex = pCote->pGoods->GetIndex();
// 			DWORD dwGoodsNum = pCote->pGoods->GetNum();
// 
// 			//����map���Ƿ��и�index����Ʒ
// 			map<DWORD,DWORD>::iterator itCote = mapGoodsTemp.find(dwGoodsIndex);
// 
// 			//�ҵ�,���������ϵݼ�
// 			if( itCote!=mapGoodsTemp.end())
// 			{
// 
// 				DWORD dwNum = itCote->second;
// 				mapGoodsTemp[dwGoodsIndex] = dwGoodsNum + dwNum;
// 			}
// 			//û���ҵ�,���¼���
// 			else
// 			{
// 				mapGoodsTemp[dwGoodsIndex] = dwGoodsNum;
// 			}
// 
// 			//////////////////////////////////////////////////////////////////////////
// 			list<tagGoods>::iterator itGoods = pCote->m_listCoteGoods.begin();
// 			for(;itGoods != pCote->m_listCoteGoods.end();itGoods++)
// 			{
// 				DWORD dwGoodsIndex = itGoods->pGoods->GetIndex();
// 				DWORD dwGoodsNum = itGoods->pGoods->GetNum();
// 
// 				//����map���Ƿ��и�index����Ʒ
// 				map<DWORD,DWORD>::iterator it = mapGoodsTemp.find(dwGoodsIndex);
// 
// 				//�ҵ�,���������ϵݼ�
// 				if( it!=mapGoodsTemp.end())
// 				{
// 
// 					DWORD dwNum = it->second;
// 					mapGoodsTemp[dwGoodsIndex] = dwGoodsNum + dwNum;
// 				}
// 				//û���ҵ�,���¼���
// 				else
// 				{
// 					mapGoodsTemp[dwGoodsIndex] = dwGoodsNum;
// 				}
// 			}
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////
	//��ʼ�Ƚ�
	char strOutInfo[128];
	static const char* szGoodsCheck = "GoodsCheckRet.log";
	//���2��map���С��һ��,˵�����ݶ�ʧ
	if(mapGoodsTemp.size() != m_mapGoodsAmountByIndex.size())
	{
		sprintf(strOutInfo,"(%s)��Ʒ����������һ��: Start",m_strCDKey.c_str());
		PutStringToFile(szGoodsCheck,strOutInfo);
		//		PutPlayerDataInfo(strOutInfo);
		for(map<DWORD,DWORD>::iterator it = mapGoodsTemp.begin();it != mapGoodsTemp.end();it++)
		{
			DWORD dwIndex = it->first;
			sprintf(strOutInfo,"	<����> ��Ʒ����:%d",dwIndex);
			PutStringToFile(szGoodsCheck,strOutInfo);
		}

		for(map<DWORD,DWORD>::iterator it = m_mapGoodsAmountByIndex.begin(); it!= m_mapGoodsAmountByIndex.end();it++)
		{
			DWORD dwIndex = it->first;
			sprintf(strOutInfo,"	<����> ��Ʒ����:%d",dwIndex);
			PutStringToFile(szGoodsCheck,strOutInfo);
		}
		PutStringToFile(szGoodsCheck,strOutInfo);
	}

	//��ϸ�Ƚϵ�����Ʒ���������
	map<DWORD,DWORD>::iterator indexIter = m_mapGoodsAmountByIndex.begin();
	for(;indexIter != m_mapGoodsAmountByIndex.end();indexIter++)
	{
		DWORD dwIndex = indexIter->first;
		DWORD dwNum = indexIter->second;

		//���̵���Ʒ��������Ͽ����ҵ�
		if(mapGoodsTemp.find(dwIndex)!=mapGoodsTemp.end())
		{
			//��������,˵�������б仯
			if( mapGoodsTemp[dwIndex] < dwNum )
			{
				sprintf(strOutInfo,"(%s)��Ʒ����<%d>��������,ԭ��(%d)��;����(%d)��",m_strCDKey.c_str(),dwIndex,dwNum,mapGoodsTemp[dwIndex]);
				PutStringToFile(szGoodsCheck,strOutInfo);
			}
		}
		else
			//���û�ҵ�,˵����Ʒ��ʧ
		{
			sprintf(strOutInfo,"(%s)��Ʒ����<%d>��������϶�ʧ",m_strCDKey.c_str(),dwIndex);
			PutStringToFile(szGoodsCheck,strOutInfo);
		}
	}
}
void CCDKey::SaveGoodsCounts(CGoods* pGoods)
{
	if(pGoods!=NULL)
	{
		DWORD dwGoodsIndex = pGoods->GetIndex();
		DWORD dwGoodsNum = pGoods->GetNum();

		//����map���Ƿ��и�index����Ʒ
		map<DWORD,DWORD>::iterator it = m_mapGoodsAmountByIndex.find(dwGoodsIndex);

		//�ҵ�,���������ϵݼ�
		if( it!=m_mapGoodsAmountByIndex.end())
		{

			DWORD dwNum = it->second;
			m_mapGoodsAmountByIndex[dwGoodsIndex] = dwGoodsNum + dwNum;
		}
		//û���ҵ�,���¼���
		else
		{
			m_mapGoodsAmountByIndex[dwGoodsIndex] = dwGoodsNum;
		}
	}
}

void CCDKey::SaveQuest(CPlayer* pPlayer)
{
	m_mapQuestInfo.clear();
// 	list<tagRoleRecord*>* pList = pPlayer->GetQuestList();
// 	if(pList)
// 	{
// 		if( !pList->empty() )
// 		{
// 			list<tagRoleRecord*>::iterator it = pList->begin();
// 			for(;it != pList->end();it++)
// 			{
// 				m_mapQuestInfo[(*it)->m_lRoleID] = true;
// 			}
// 		}
// 	}
}
void CCDKey::CheckQuest(CPlayer* pPlayer)
{
//	char strOutInfo[256];
// 	list<tagRoleRecord*>* pList = pPlayer->GetQuestList();
// 	if(pList)
// 	{
// 		if( pList->empty() )
// 		{
// 			sprintf(strOutInfo,"(%s)����ʧ",m_strCDKey.c_str());
// 			//			PutPlayerDataInfo(strOutInfo);
// 		}
// 		else
// 		{
// 			list<tagRoleRecord*>::iterator it = pList->begin();
// 			for(;it != pList->end();it++)
// 			{
// 				if( !(m_mapQuestInfo.find((*it)->m_lRoleID) != m_mapQuestInfo.end()) )
// 				{
// 					sprintf(strOutInfo,"(%s)����ʧ",m_strCDKey.c_str());
// 					//					PutPlayerDataInfo(strOutInfo);
// 				}
// 			}
// 		}
// 	}
}
void CCDKey::SaveSkill(CPlayer* pPlayer)
{
	return;
	std::ofstream out_f;
	string strPath = "PlayerData//";
	string strCDKeyFolder = strPath + m_strCDKey;
	//��CDKeyΪ�������ļ���
	CreateDirectory(strCDKeyFolder.c_str(),NULL);
	//�ļ���
	string strFileName = strCDKeyFolder + "//SkillInfo.ini";
	//���ļ�
	out_f.open(strFileName.c_str(),ios::out);
	//д����
	out_f.write("Index	",		sizeof("#Index"));
	out_f.write("#",		sizeof("#"));
	//����
	out_f.put('\n');
	char strInput[128];

	m_mapSkillInfo.clear();
	
	list<tagPlayerSkill> pList = pPlayer->GetSkillMgr()->GetSkillList();
	list<tagPlayerSkill>::iterator it = pList.begin();
	for(;it!=pList.end();it++)
	{
		m_mapSkillInfo[it->dwID] = true;
		sprintf(strInput,"%d",it->dwID);
		WriteOutInfo(out_f,strInput);
		out_f.put('\n');
	}
	out_f.close();
}

void CCDKey::CheckSkill(CPlayer* pPlayer)
{
	char strOutInfo[256];
	list<tagPlayerSkill> pList = pPlayer->GetSkillMgr()->GetSkillList();;
	if( pList.empty())
	{
		sprintf(strOutInfo,"(%s)����Ϊ��",m_strCDKey.c_str());
		//		PutPlayerDataInfo(strOutInfo);
	}
	else
	{
		list<tagPlayerSkill>::iterator it = pList.begin();
		for(;it != pList.end();it++)
		{
			if( !(m_mapSkillInfo.find(it->dwID) != m_mapSkillInfo.end()) )
			{
				sprintf(strOutInfo,"(%s):��ʧ����(%d)",m_strCDKey.c_str(),it->dwID);
				//				PutPlayerDataInfo(strOutInfo);
			}
		}
	}
}

void CCDKey::WriteOutInfo(std::ofstream & out_f,const char* strOutInfo)
{
	out_f.write(strOutInfo,strlen(strOutInfo));
	out_f.put('	');
}

//-----------------------------------------------------------------------------------------
// ��������ϵ��ʼ��������ݱ���ͺ˶�
// by MartySa 2009.1.5
// �������岿��
//-----------------------------------------------------------------------------------------

//load from .txt
void CCDKey::AddSaveMailDataByTxt(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// }
	// 
	// 
	// 
	// 	CStdioFile file;
	// 
	// 	//���ļ�
	// 
	// 	//��������
	// 	std::string strcdkey	 = m_strCDKey.c_str();
	// 	std::string strchar		 = "//";
	// 	std::string strfrist	 = "PlayerData//";
	// 	std::string strend       = "mail.txt";
	// 	std::string strtempname  = strfrist+strcdkey+strchar+strcdkey+strend;
	// 	string cstrBuff;		//����buff
	// 	string  tempBuff;		
	// //////////////////////////////////////////////////////////////////////////
	// 	std::ifstream in_f;
	// 	if( in_f.fail())
	// 		return;
	// 	in_f.open(strtempname);
	// 
	// 	while(true)
	// 	{
	// 		string strCDKey;
	// 		if( in_f.eof())
	// 			break;
	// 		in_f>>strCDKey;
	// 		if(strCDKey.empty())
	// 			break;
	// 
	// /////////////////////////////////////////////////////////////////////////
	// 
	// 	if (file.Open(strtempname.c_str(), CFile::modeRead))
	// 	{
	// 		while (file.ReadString(cstrBuff.c_str()))				//����
	// 		{	
	// 			//�ı��е��ʼ����� 
	// 			tempBuff   = GetCStringToString(cstrBuff, ":");
	// 			long lSize = atoi(tempBuff.c_str());
	// 
	// 			for (long i = 0; i!= lSize; ++i)
	// 			{	
	// 				stMailData maildata;
	// 				
	// 				//��ȡ�ʼ�����
	// 				file.ReadString(cstrBuff);
	// 
	// 				tempBuff             = GetCStringToString(cstrBuff, ":");
	// 				strcpy(maildata.strMailName,tempBuff.c_str());				//д����ʱtagmail�ṹ
	// 				
	// 				//��ȡ�ʼ�Guid
	// 				file.ReadString(cstrBuff);
	// 				tempBuff             = GetCStringToString(cstrBuff, ":");	
	// 				CGUID guid(tempBuff.c_str());					//����һ����ʱguid	
	// 				maildata.gMailID     = guid;
	// 
	// 				//��ȡ�ʼ�����
	// 				file.ReadString(cstrBuff);
	// 
	// 				tempBuff             = GetCStringToString(cstrBuff, ":");
	// 				strcpy(maildata.strMailDesc,tempBuff.c_str());				//д����ʱtagmail�ṹ
	// 				
	// 				//��ȡ�ʼ��յ���ʱ��
	// 				file.ReadString(cstrBuff);
	// 
	// 				tempBuff             = GetCStringToString(cstrBuff, ":");
	// 				long num = atoi(tempBuff.c_str());
	// 				maildata.dwTimeStart = num;						//д����ʱtagmail�ṹ
	// 				
	// 				//��ȡ�ʼ�����
	// 				file.ReadString(cstrBuff);
	// 
	// 				tempBuff            = GetCStringToString(cstrBuff, ":");
	// 				num = atoi(tempBuff.c_str());
	// 				maildata.eMailType	= (eMAILTYPE)num;			//д����ʱtagmail�ṹ
	// 				
	// 				//��ȡ������Ǯ
	// 				file.ReadString(cstrBuff);
	// 
	// 				tempBuff            = GetCStringToString(cstrBuff, ":");
	// 				num		 = atoi(tempBuff.c_str());
	// 				maildata.dwGoldNum	= num;						//д����ʱtagmail�ṹ
	// 				
	// 				//��ȡ������Ʒ����
	// 				file.ReadString(cstrBuff);
	// 
	// 				tempBuff            = GetCStringToString(cstrBuff, ":");
	// 				num		 = atoi(tempBuff.c_str());
	// 				
	// 				for (long j = 0; j!=num; ++j)
	// 				{	
	// 					CGoods *goods  = new CGoods;							//��Lua�µ����ͷţ�����ʱ������
	// 					//��ȡ������Ʒ��
	// 					file.ReadString(cstrBuff);
	// 					tempBuff       = GetCStringToString(cstrBuff, ":");		
	// 					goods->SetName(tempBuff.c_str());					//д����ʱgoods�ṹ
	// 
	// 					//��ȡ��ƷGUID
	// 					file.ReadString(cstrBuff);
	// 					tempBuff       = GetCStringToString(cstrBuff, ":");	
	// 					CGUID guid(tempBuff.c_str());
	// 					goods->SetExID(guid);								//д����ʱgoods�ṹ
	// 					
	// 					//��ȡ������Ʒ����
	// 					file.ReadString(cstrBuff);
	// 					tempBuff	   = GetCStringToString(cstrBuff, ":");	
	// 					long goodsnum = atoi(tempBuff.c_str());
	// 					goods->SetNum(goodsnum);							//д����ʱgoods�ṹ
	// 					
	// 					//��ȡ������Ʒ�±�
	// 					file.ReadString(cstrBuff);
	// 					tempBuff	   = GetCStringToString(cstrBuff, ":");	
	// 					goodsnum      = atoi(tempBuff.c_str());
	// 					goods->SetIndex(goodsnum);							//д����ʱgoods�ṹ
	// 
	// 					maildata.GoodsList.push_back(goods);
	// 				}
	// 				
	// 				m_vecSaveMailData.push_back(maildata);
	// 			}
	// 		}
	// 
	// 		file.Close();
	// 	}
	// 	else
	// 	{	
	// 		//�����ǰĿ¼��û�б����ļ�����
	// 		SavaMailData(pPlayer);
	// 	}
	// 	
}

//save to .txt
void CCDKey::SavaMailData(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// 	}
	// 
	// 		CStdioFile file;
	// 
	// 	std::string strchar  = "//";
	// 	std::string strcdkey = m_strCDKey.c_str();
	// 	std::string strfrist = "PlayerData//";
	// 	std::string strend   = "mail.txt";
	// 	std::string strtemp  = strfrist+strcdkey+strchar+strcdkey+strend;
	// 		//�����ļ���
	// 		std::string strPath = "PlayerData//";
	// 	std::string strCDKeyFolder = strPath+strcdkey;
	// 		//�����ļ���
	// 		CreateDirectory(strCDKeyFolder.c_str(), NULL);
	// 		//д���ļ�
	// 	if (file.Open(strtemp.c_str(), CFile::modeCreate | CFile::modeWrite))
	// 		{
	// 			vector<stMailData> maildata = pPlayer->GetMailData();
	// 			long lMailSize              = (long)maildata.size();
	// 			//д���ʼ���С
	// 			string buff;
	// 			buff.Format("%d", lMailSize);
	// 		buff = GetWriteCString(2, "�ʼ�����:", buff);
	// 			file.WriteString(buff);
	// 		file.WriteString("\n");
	// 			if (lMailSize > 0)
	// 			{
	// 				for (int i=0; i!=lMailSize; ++i)
	// 				{	
	// 					//д���ʼ���
	// 				buff = GetWriteCString(2, "[[[[[ �ʼ����� ]]]]]:", maildata[i].strMailName);
	// 					file.WriteString(buff);
	// 				file.WriteString("\n");
	// 
	// 				//д���ʼ�guid
	// 				CGUID guid    = maildata[i].gMailID;
	// 				char *pchGuid = new char[125]; 
	// 				guid.tostring(pchGuid);
	// 				buff          = GetWriteCString(2, "�ʼ�Guid:", pchGuid);
	// 				delete []pchGuid;
	// 				pchGuid       = NULL;
	// 				file.WriteString(buff);
	// 				file.WriteString("\n");
	// 					
	// 					//д���ʼ�����
	// 				buff = GetWriteCString(2, "�ʼ�����:", maildata[i].strMailDesc);
	// 					file.WriteString(buff);
	// 				file.WriteString("\n");
	// 					
	// 					//д���ʼ��յ�ʱ��
	// 					buff.Format("%d", maildata[i].dwTimeStart); 
	// 				buff = GetWriteCString(2, "�յ�ʱ��:", buff);
	// 					file.WriteString(buff);
	// 				file.WriteString("\n");
	// 					
	// 					//д���ʼ�����
	// 					buff.Format("%d", maildata[i].eMailType); 
	// 				buff = GetWriteCString(2, "�ʼ�����:", buff);
	// 					file.WriteString(buff);
	// 				file.WriteString("\n");
	// 					
	// 					//д���ʼ�������Ǯ
	// 					buff.Format("%d", maildata[i].dwGoldNum); 
	// 				buff = GetWriteCString(2, "������Ǯ:", buff);
	// 					file.WriteString(buff);
	// 				file.WriteString("\n");
	// 					
	// 					//д�븽����Ʒ����
	// 					long goodsnum = maildata[i].GoodsList.size();
	// 					buff.Format("%d", goodsnum);
	// 				buff = GetWriteCString(2, "������Ʒ:", buff);
	// 					file.WriteString(buff);
	// 				file.WriteString("\n");
	// 
	// 					if (goodsnum > 0)
	// 					{					
	// 						list<CGoods*>::iterator it = maildata[i].GoodsList.begin();
	// 						for ( ; it!= maildata[i].GoodsList.end(); ++it)
	// 						{	
	// 							//д����Ʒ����
	// 						buff = GetWriteCString(2, " ��Ʒ��:", (*it)->GetName());
	// 							file.WriteString(buff);
	// 						file.WriteString("\n");
	// 
	// 						//д����ƷGUID
	// 						CGUID guid  = (*it)->GetExID();
	// 						char *pchGuid = new char[125]; 
	// 						guid.tostring(pchGuid);
	// 						buff = GetWriteCString(2, " Guid:", pchGuid);
	// 						delete []pchGuid;
	// 						pchGuid = NULL;
	// 						file.WriteString(buff);
	// 						file.WriteString("\n");
	// 
	// 							//д����Ʒ����
	// 							buff.Format("%d", (*it)->GetNum()); 
	// 						buff = GetWriteCString(2, " ��Ʒ��:", buff);
	// 							file.WriteString(buff);
	// 						file.WriteString("\n");
	// 
	// 							//д����Ʒ�±�
	// 							buff.Format("%d", (*it)->GetIndex()); 
	// 						buff = GetWriteCString(2, " ��Ʒ��:", buff);
	// 							file.WriteString(buff);
	// 						file.WriteString("\n");
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 
	// 		file.Close();
}
// //check maildata
void CCDKey::CheckMailData(CPlayer *pPlayer)
{	
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// 	}
	// 
	// 	vector<stMailData> maildata = pPlayer->GetMailData();		//���浱ǰ����������ʼ��б�
	// 	if ( m_vecSaveMailData.empty() )
	// 	{	
	// 		//���û��ȡ�ɹ��Ͷ�ȡ
	// 		AddSaveMailDataByTxt(pPlayer);
	// 
	// 		return ;
	// 	}
	// 	else if ( maildata.empty() )
	// 	{
	// 		return ;
	// 	}
	// 	//��⿪ʼ
	// 	//����ʼ�����
	// 	long iSaveMailSize   = m_vecSaveMailData.size();				//�ı��б����ʼ����ݴ�С
	// 	long iPlayerMailSize = maildata.size();							//��ǰ������ϵ��ʼ����ݴ�С
	// 	string strPlayerName = pPlayer->GetName();
	// 	string strWirte;
	// 	string cstrInt;
	// 	if ( iSaveMailSize < iPlayerMailSize )
	// 	{	
	// 		strWirte = GetWirteString(2, strPlayerName.c_str(), "�ʼ�������һ��");
	// 		PutCheckMailErr(strWirte.c_str());
	// 
	// 		return ;
	// 	}
	// 	else if (iSaveMailSize == iPlayerMailSize) 
	// 	{
	// 		//����ʼ�������
	// 		for (int i=0; i!=iSaveMailSize; ++i)
	// 		{	
	// 			//����ʼ�guid
	// 			if (m_vecSaveMailData[i].gMailID != maildata[i].gMailID)
	// 			{	
	// 				cstrInt.Format("%d", i+1);
	// 				strWirte = cstrInt.GetBuffer();
	// 				cstrInt.ReleaseBuffer();
	// 				strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte.c_str());
	// 				PutCheckMailErr(strWirte.c_str());
	// 
	// 				strWirte = GetWirteString(2, strPlayerName.c_str(), "�ʼ�guid��һ��");
	// 				PutCheckMailErr(strWirte.c_str());
	// 				return ;
	// 			}
	// 			////����ʼ�����
	// 			//if (m_vecSaveMailData[i].eMailType != maildata[i].eMailType)
	// 			//{	
	// 			//	cstrInt.Format("%d", i+1);
	// 			//	strWirte = cstrInt.GetBuffer();
	// 			//	cstrInt.ReleaseBuffer();
	// 			//	strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte);
	// 			//	PutCheckMailErr(strWirte.c_str());
	// 			//	
	// 			//	strWirte = GetWirteString(2, strPlayerName, "�ʼ����Ͳ�һ��");
	// 			//	PutCheckMailErr(strWirte.c_str());
	// 			//	return ;
	// 			//}
	// 			//����ʼ��ڽ�Ǯ
	// 			if ( m_vecSaveMailData[i].dwGoldNum < maildata[i].dwGoldNum )
	// 			{
	// 				cstrInt.Format("%d", i+1);
	// 				strWirte = cstrInt.GetBuffer();
	// 				cstrInt.ReleaseBuffer();
	// 				strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte.c_str());
	// 				PutCheckMailErr(strWirte.c_str());	
	// 
	// 				strWirte = GetWirteString(2, strPlayerName.c_str(), "�ʼ���Ǯ��һ��");
	// 				PutCheckMailErr(strWirte.c_str());
	// 				return ;
	// 			}
	// 			//����ʼ�����Ʒ
	// 			long savemail_goodssize = (m_vecSaveMailData[i].GoodsList).size();
	// 			long mail_goodssize     = (maildata[i].GoodsList).size();
	// 			if ( savemail_goodssize < mail_goodssize )
	// 			{
	// 				cstrInt.Format("%d", i+1);
	// 				strWirte = cstrInt.GetBuffer();
	// 				cstrInt.ReleaseBuffer();
	// 				strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte.c_str());
	// 				PutCheckMailErr(strWirte.c_str());
	// 
	// 				strWirte = GetWirteString(2, strPlayerName.c_str(), "�ʼ�����Ʒ������һ��");
	// 				PutCheckMailErr(strWirte.c_str());
	// 				return ;
	// 			}
	// 			else if(savemail_goodssize == mail_goodssize)
	// 			{	
	// 				//��ƷGuid
	// 				CGUID guidGoods_1  = CGUID::GUID_INVALID;
	// 				CGUID guidGoods_2  = CGUID::GUID_INVALID;
	// 				//��Ʒ����
	// 				long lGoodsSize_1  = 0;
	// 				long lGoodsSize_2  = 0;
	// 				//��Ʒ�±�
	// 				long lGoodsIndex_1 = 0;
	// 				long lGoodsIndex_2 = 0;
	// 
	// 				list<CGoods*>::iterator ito_MailData     = maildata[i].GoodsList.begin();
	// 				list<CGoods*>::iterator ito_SaveMailData = m_vecSaveMailData[i].GoodsList.begin();
	// 
	// 				//���㵱ǰ�ڼ��ĸ�goods
	// 				int goodsnum = 0;	
	// 
	// 				//˫vec:itor���Ӽ���
	// 				for ( ; ito_MailData != maildata[i].GoodsList.end(), ito_SaveMailData != m_vecSaveMailData[i].GoodsList.end(); 
	// 					++ito_MailData, ++ito_SaveMailData)
	// 				{	
	// 					++goodsnum;										//�Ե���
	// 
	// 					guidGoods_1   = (*ito_MailData)->GetExID();
	// 					lGoodsSize_1  = (*ito_MailData)->GetNum();
	// 					lGoodsIndex_1 = (*ito_MailData)->GetIndex();
	// 
	// 					guidGoods_2   = (*ito_SaveMailData)->GetExID();
	// 					lGoodsSize_2  = (*ito_SaveMailData)->GetNum();
	// 					lGoodsIndex_2 = (*ito_SaveMailData)->GetIndex();
	// 
	// 					//�����Ʒ����
	// 					if (lGoodsSize_1 < lGoodsSize_2)
	// 					{	
	// 
	// 						cstrInt.Format("%d", i+1);
	// 						strWirte = cstrInt.GetBuffer();
	// 						cstrInt.ReleaseBuffer();
	// 						strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte.c_str());
	// 						PutCheckMailErr(strWirte.c_str());
	// 
	// 						cstrInt.Format("%d", goodsnum);
	// 						strWirte = cstrInt.GetBuffer();
	// 						cstrInt.ReleaseBuffer();
	// 						strWirte = GetWirteString(2, "��ǰ����goodsΪ:", strWirte.c_str());
	// 						PutCheckMailErr(strWirte.c_str());
	// 
	// 						strWirte = GetWirteString(2, strPlayerName.c_str(), "�ʼ��ڵ��������������һ��");
	// 						PutCheckMailErr(strWirte.c_str());
	// 						return ;
	// }
	// 
	// 					//�����Ʒindex
	// 					//if (lGoodsIndex_1 < lGoodsIndex_2)
	// 					//{	
	// 					//	cstrInt.Format("%d", i+1);
	// 					//	strWirte = cstrInt.GetBuffer();
	// 					//	cstrInt.ReleaseBuffer();
	// 					//	strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte);
	// 					//	PutCheckMailErr(strWirte.c_str());
	// 
	// 					//	cstrInt.Format("%d", goodsnum);
	// 					//	strWirte = cstrInt.GetBuffer();
	// 					//	cstrInt.ReleaseBuffer();
	// 					//	strWirte = GetWirteString(2, "��ǰ����goodsΪ:", strWirte);
	// 					//	PutCheckMailErr(strWirte.c_str());
	// 					//	
	// 					//	strWirte = GetWirteString(2, strPlayerName, "�ʼ��ڵ�����index��һ��");
	// 					//	PutCheckMailErr(strWirte.c_str());
	// 					//	return ;
	// 					//}
	// 
	// 					//�����Ʒguid
	// 
	// 					else if (lGoodsSize_1 == lGoodsSize_2 )
	// 					{	
	// 						if (guidGoods_1 != guidGoods_2)
	// 						{
	// 							cstrInt.Format("%d", i+1);
	// 							strWirte = cstrInt.GetBuffer();
	// 							cstrInt.ReleaseBuffer();
	// 							strWirte = GetWirteString(2, "��ǰ�ʼ�����Ϊ:", strWirte.c_str());
	// 							PutCheckMailErr(strWirte.c_str());
	// 
	// 							cstrInt.Format("%d", goodsnum);
	// 							strWirte = cstrInt.GetBuffer();
	// 							cstrInt.ReleaseBuffer();
	// 							strWirte = GetWirteString(2, "��ǰ����goodsΪ:", strWirte.c_str());
	// 							PutCheckMailErr(strWirte.c_str());
	// 
	// 							strWirte = GetWirteString(2, strPlayerName.c_str(), "�ʼ��ڵ�����guid��һ��");
	// 							PutCheckMailErr(strWirte.c_str());
	// 							return ;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
}


//clear svaemaildata
void CCDKey::ClearAllSaveMailData()
{	
	if (!m_vecSaveMailData.empty())
	{	
		vector<stMailData>::iterator it = m_vecSaveMailData.begin();

		for ( ; it!= m_vecSaveMailData.end(); ++it)
		{
			if (!(*it).GoodsList.empty())
			{
				list<CGoods*>::iterator itor = (*it).GoodsList.begin();
				for ( ; itor!=(*it).GoodsList.end(); ++itor)
				{
					SAFE_DELETE(*itor);
				}
				(*it).GoodsList.clear();
			}
		}
		m_vecSaveMailData.clear();
	}
}

//debug for check mail err 
// by MartySa 2009.1.7
void CCDKey::PutCheckMailErr(const char *msg)
{
	static int num=0;
	char t[128];

	FILE *fp;
	if( (fp=fopen("checkmail_Erro.txt","a+")) == NULL )
		return;

	num++;
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(t, "\n%d(%02d-%02d %02d:%02d:%02d):", num, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	fseek(fp, 0, SEEK_END);
	fwrite(t, lstrlen(t), 1, fp);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);

	//������� 20M  ɾ��debug�ļ�
	if( GetFileLength("checkmail_Erro.txt")/(1024*1024) > 20 )
	{
		DeleteFileA("checkmail_Erro.txt");
	}
}

string CCDKey::GetWriteCString(int cstrSize, ... )
{	
	string cstrBuff, cstrDest;
	char *pStr = NULL;
	va_list vaStr = NULL;
	va_start(vaStr, cstrSize);

	for (int i=0; i!= cstrSize; ++i)
	{
		pStr	 = va_arg(vaStr, char*);
		cstrBuff = pStr;
		cstrDest += cstrBuff;
	}

	return cstrDest;
}

string CCDKey::GetWirteString(int strSize, ...)
{

	char *pStr = NULL;
	va_list vaStr = NULL;
	va_start(vaStr, strSize);

	string strBuff, strDest;

	for (int i=0; i!= strSize; ++i)
	{
		pStr	= va_arg(vaStr, char*);
		strBuff = pStr;
		strDest += strBuff;
	}

	return strDest;
}

//-----------------------------------------------------------------------------------------
// ��������ϵĺ��ѽ������ݱ���ͺ˶�
// by MartySa 2009.1.12
//-----------------------------------------------------------------------------------------

//	save linkman 
void CCDKey::SaveLinkManPanleGroup(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// 	}
	// 
	// 	CStdioFile file;
	// 
	// 	std::string strchar  = "//";
	// 	std::string strcdkey = m_strCDKey.c_str();
	// 	std::string strfrist = "PlayerData//";
	// 	std::string strend   = "linkman.txt";
	// 	std::string strtemp  = strfrist+strcdkey+strchar+strcdkey+strend;
	// 	//�����ļ���
	// 	std::string strPath = "PlayerData//";
	// 	std::string strCDKeyFolder = strPath+strcdkey;
	// 	//�����ļ���
	// 	CreateDirectory(strCDKeyFolder.c_str(), NULL);
	// 	//д���ļ�
	// 	if (file.Open(strtemp.c_str(), CFile::modeCreate | CFile::modeWrite))
	// 	{	
	// 		//д�����������defaultΪ CPlayer::MAXNUM_PANELS
	// 		string buff;
	// 		buff.Format("%d", (double)CPlayer::MAXNUM_PANELS);
	// 		buff = GetWriteCString(2, "��Һ��ѷ�������Ϊ :", buff);
	// 		file.WriteString(buff);
	// 		file.WriteString("\n");
	// 		for (int i=0; i!=CPlayer::MAXNUM_PANELS; ++i)
	// 		{
	// 			//ѭ���ֱ��ȡ��ͬ����
	// 			CPlayer::SPanel m_playerLinkManPanle = pPlayer->GetSPanle(i);
	// 			//д�뵱ǰ��������
	// 			buff.Format("%d", m_playerLinkManPanle.groupid);
	// 			buff = GetWriteCString(2, " ��ǰ����Ϊ :", buff);
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 
	// 			//д������
	// 			long iMemberSize = (m_playerLinkManPanle.m_PanleMemberData).size();
	// 
	// 			buff.Format("%d", iMemberSize);
	// 			buff = GetWriteCString(2, " ������ϵ����Ϊ :", buff);
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 			//�����������ϵ��
	// 			if ( iMemberSize > 0)
	// 			{
	// 				//�����ϵ��д��
	// 				int j = 0;		//��������
	// 				list<CPlayer::tagLinkman>::iterator it = m_playerLinkManPanle.m_PanleMemberData.begin();
	// 				for ( ; it!=m_playerLinkManPanle.m_PanleMemberData.end(); ++it)
	// 				{
	// 					//д����ϵ������
	// 					j++;
	// 					buff.Format("%d", j);
	// 					buff = GetWriteCString(2, " ��ϵ��", buff);
	// 					buff = GetWriteCString(3, buff, "����Ϊ :", (*it).szName);
	// 					file.WriteString(buff);
	// 					file.WriteString("\n");
	// 
	// 					//д����ϵ��GUID
	// 					CGUID guid    = (*it).guid;
	// 					char *pchGuid = new char[125]; 
	// 					guid.tostring(pchGuid);
	// 					buff          = GetWriteCString(2, "       Guid :", pchGuid);
	// 					delete []pchGuid;
	// 					pchGuid       = NULL;
	// 					file.WriteString(buff);
	// 					file.WriteString("\n");
	// 				}
	// 			}
	// 		}
	// 	}
	// 	file.Close();
}

//load savelinkmanspanel by txt.
void CCDKey::AddSaveLinkManPanleGroupByTxt(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// 	}
	// 	//��ձ�������ݽṹ
	// 	//ClearLinkManAllSavePanleGroup();
	// 
	// 	CStdioFile file;
	// 
	// 	//���ļ�
	// 
	// 	//��������
	// 	std::string strcdkey	 = m_strCDKey.c_str();
	// 	std::string strchar		 = "//";
	// 	std::string strfrist	 = "PlayerData//";
	// 	std::string strend       = "linkman.txt";
	// 	std::string strtempname  = strfrist+strcdkey+strchar+strcdkey+strend;
	// 	string cstrBuff;		//����buff
	// 	string  tempBuff;		
	// 
	// 	if (file.Open(strtempname.c_str(), CFile::modeRead))
	// 	{	
	// 		while (file.ReadString(cstrBuff))				//����
	// 		{	
	// 			//��ȡ��������
	// 			tempBuff   = GetCStringToString(cstrBuff, ":");
	// 			long lPanelSize = atoi(tempBuff.c_str());
	// 			//������ȡд������
	// 			for (int i=0; i!= CPlayer::MAXNUM_PANELS; ++i)
	// 			{
	// 				//��ȡ��������
	// 				file.ReadString(cstrBuff);
	// 				tempBuff				           = GetCStringToString(cstrBuff, ":");
	// 				long lgroupid					   = atoi(tempBuff.c_str());
	// 				m_SaveLinkManPanelGroup[i].groupid = lgroupid;						//д�뱣������
	// 
	// 				//��ȡ������ϵ������
	// 				file.ReadString(cstrBuff);
	// 				tempBuff				           = GetCStringToString(cstrBuff, ":");
	// 				long lLinkmanSize				   = atoi(tempBuff.c_str());
	// 				m_SaveLinkManPanelGroup[i].number  = lLinkmanSize;					//д�뱣������
	// 
	// 				if (lLinkmanSize > 0)
	// 				{						
	// 					for (int j=0; j!=lLinkmanSize; ++j)
	// 					{	
	// 						CPlayer::tagLinkman taglinkman;
	// 						//��ȡ������ϵ������
	// 						file.ReadString(cstrBuff);
	// 						tempBuff				   = GetCStringToString(cstrBuff, ":");
	// 						sprintf(taglinkman.szName, "%s", tempBuff.c_str());
	// 						strncpy(taglinkman.szName, tempBuff.c_str(), 125);
	// 
	// 						//��ȡ������ϵ�˵�guid
	// 						file.ReadString(cstrBuff);
	// 						tempBuff				   = GetCStringToString(cstrBuff, ":");
	// 						CGUID guid(tempBuff.c_str());
	// 						taglinkman.guid            = guid;
	// 						m_SaveLinkManPanelGroup->m_PanleMemberData.push_back(taglinkman);
	// 					}
	// 				}
	// 			}
	// 		}
	// 		file.Close();
	// 	}
	// 	else
	// 	{	
	// 		SaveLinkManPanleGroup(pPlayer);
	// 	}

}

//check linkman
void CCDKey::CheckSaveLinkManPanleGroup(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// 	}
	// 
	// 	//
	// 	//��⿪ʼ
	// 	string strPlayerName = pPlayer->GetName();
	// 	string strWirte;
	// 	string cstrInt;
	// 
	// 	for (int i=0; i!=CPlayer::MAXNUM_PANELS; ++i)
	// 	{
	// 		CPlayer::SPanel m_playerLinkManPanle = pPlayer->GetSPanle(i);
	// 
	// 		//ÿ����������
	// 		long lSaveMemberSize   = (m_SaveLinkManPanelGroup[i].m_PanleMemberData).size();
	// 		long lPlayerMemberSize = (m_playerLinkManPanle.m_PanleMemberData).size();
	// 		if ( (lSaveMemberSize < lPlayerMemberSize) && lSaveMemberSize > 0)
	// 		{	
	// 			cstrInt.Format("%d", m_playerLinkManPanle.groupid);
	// 			strWirte = cstrInt.GetBuffer();
	// 			cstrInt.ReleaseBuffer();
	// 
	// 			strWirte = GetWirteString(4, strPlayerName.c_str(), "��", strWirte.c_str(), "�������������һ��");
	// 			PutCheckMailErr(strWirte.c_str());
	// 			return ;
	// 		}
	// 		else if ( lSaveMemberSize == lPlayerMemberSize)
	// 		{
	// 			//�����ϵ�˵�guid
	// 			int membersize = 0;		//��������
	// 			list<CPlayer::tagLinkman>::iterator save_itor   = m_SaveLinkManPanelGroup[i].m_PanleMemberData.begin();
	// 			list<CPlayer::tagLinkman>::iterator player_itor = m_playerLinkManPanle.m_PanleMemberData.begin();
	// 			for ( ; save_itor != m_SaveLinkManPanelGroup[i].m_PanleMemberData.end(), 
	// 				player_itor != m_playerLinkManPanle.m_PanleMemberData.end(); ++save_itor, ++player_itor)
	// 			{	
	// 				membersize++;	
	// 				if ( (*save_itor).guid != (*player_itor).guid)
	// 				{
	// 					cstrInt.Format("%d", m_playerLinkManPanle.groupid);
	// 					strWirte = cstrInt.GetBuffer();
	// 					cstrInt.ReleaseBuffer();
	// 
	// 					string temp_cstrszie;
	// 					string  temp_strsize;
	// 					temp_cstrszie.Format("%d", membersize);
	// 					temp_strsize = cstrInt.GetBuffer();
	// 					temp_cstrszie.ReleaseBuffer();
	// 
	// 					strWirte = GetWirteString(6, strPlayerName.c_str(), "��", strWirte.c_str(), "�ķ����", temp_strsize.c_str(), "����ϵ��Guid��һ��");
	// 					PutCheckMailErr(strWirte.c_str());
	// 					break;
	// 					return ;
	// 				}
	// 			}
	// 		}
	// 	}
}

void CCDKey::ClearLinkManAllSavePanleGroup(CPlayer *pPlayer)
{
	for (int i=0; i!=MAXNUM_PANELS; ++i)
	{
		if ( !m_SaveLinkManPanelGroup[i].m_PanleMemberData.empty())
		{	
			list<tagLinkman>::iterator it = m_SaveLinkManPanelGroup[i].m_PanleMemberData.begin();

			for ( ; it!=m_SaveLinkManPanelGroup[i].m_PanleMemberData.end(); ++it)
			{
				(*it).guid = CGUID::GUID_INVALID;
			}
			m_SaveLinkManPanelGroup[i].m_PanleMemberData.clear();
		}
	}

	//�����ұ����������
//	pPlayer->ClearLinkmanData();
}
//put debug for linkman_err
void CCDKey::PutCheckLinkManErr(const char *msg)
{
	static int num=0;
	char t[128];

	FILE *fp;
	if( (fp=fopen("check_linkman_Erro.txt","a+")) == NULL )
		return;

	num++;
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(t, "\n%d(%02d-%02d %02d:%02d:%02d):", num, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	fseek(fp, 0, SEEK_END);
	fwrite(t, lstrlen(t), 1, fp);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);

	//������� 20M  ɾ��debug�ļ�
	if( GetFileLength("check_linkman_Erro.txt")/(1024*1024) > 20 )
	{
		DeleteFileA("check_linkman_Erro.txt");
	}
}

//-----------------------------------------------------------------------------------------
// ��������ϵĺ��ѽ������ݱ���ͺ˶�
// by MartySa 2009.1.13
//-----------------------------------------------------------------------------------------

// save faction .txt
void CCDKey::SaveFaction(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// {
	// 		return ;
	// 	}
	// 
	// 	CStdioFile file;
	// 
	// 	std::string strchar  = "//";
	// 	std::string strcdkey = m_strCDKey.c_str();
	// 	std::string strfrist = "PlayerData//";
	// 	std::string strend   = "faction.txt";
	// 	std::string strtemp  = strfrist+strcdkey+strchar+strcdkey+strend;
	// 	//�����ļ���
	// 	std::string strPath  = "PlayerData//";
	// 	std::string strCDKeyFolder = strPath+strcdkey;
	// 	//�����ļ���
	// 	CreateDirectory(strCDKeyFolder.c_str(), NULL);
	// 	//д���ļ�
	// 	if (file.Open(strtemp.c_str(), CFile::modeCreate | CFile::modeWrite))
	// 	{	
	// 		string buff;
	// 		string  temp_buff;
	// 		CGUID FamilyGuid = pPlayer->GetFamilyGuid();
	// 		if ( FamilyGuid != CGUID::GUID_INVALID )
	// 		{	
	// 			//д�����guid
	// 			char *pGuid = new char[125];
	// 			FamilyGuid.tostring(pGuid);
	// 			buff = GetWriteCString(2, "��������л�GUIDΪ :", pGuid);
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 			delete []pGuid;
	// 			pGuid = NULL;
	// 
	// 			//д���������
	// 			temp_buff = pPlayer->GetFamilyName();
	// 			buff = GetWriteCString(2, "        �л�����Ϊ :", temp_buff.c_str());
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 
	// 			//д�����ƺ�
	// 			temp_buff = pPlayer->GetFamilyTitle();
	// 			buff = GetWriteCString(2, "        �л�ƺ�Ϊ :", temp_buff.c_str());
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 
	// 			//д��᳤guid
	// 			//pGuid = new char[125];
	// 			//const CGUID &leader_guid = pPlayer->GetFamilyMasterGuid();
	// 			//FamilyGuid.tostring(pGuid);
	// 			//buff = GetWriteCString(2, "        �л� �᳤ GUIDΪ :", pGuid);
	// 			//file.WriteString(buff);
	// 			//file.WriteString("\n");
	// 			//delete []pGuid;
	// 			//pGuid = NULL;
	// 
	// 			//д���л�job�ȼ�
	// 			long joblevel = pPlayer->GetFamilyJobLev();
	// 			buff.Format("%d", joblevel);
	// 			buff = GetWriteCString(2, "        �л�job�ȼ�Ϊ :", buff);
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 		}
	// 		else
	// 		{
	// 			FamilyGuid = CGUID::GUID_INVALID;
	// 			char *pGuid = new char[125];
	// 			FamilyGuid.tostring(pGuid);
	// 			buff = GetWriteCString(2, "���û�м����л�,��ʱGuidΪ�� :", pGuid);
	// 			file.WriteString(buff);
	// 			file.WriteString("\n");
	// 			delete []pGuid;
	// 			pGuid = NULL;
	// 		}
	// 		file.Close();
	// 	}
	// 
	// 	
}

// load save faction data by .txt
void CCDKey::AddSaveFactionByTxt(CPlayer *pPlayer)
{
	// 	if (pPlayer == NULL)						//ָ���ж�
	// 	{
	// 		return ;
	// 	}
	// 
	// 	CStdioFile file;
	// 
	// 	//���ļ�
	// 
	// 	//��������
	// 	std::string strcdkey	 = m_strCDKey.c_str();
	// 	std::string strchar		 = "//";
	// 	std::string strfrist	 = "PlayerData//";
	// 	std::string strend       = "faction.txt";
	// 	std::string strtempname  = strfrist+strcdkey+strchar+strcdkey+strend;
	// 	string cstrBuff;		//����buff
	// 	string  tempBuff;		
	// 
	// 	if (file.Open(strtempname.c_str(), CFile::modeRead))
	// 	{	
	// 		while (file.ReadString(cstrBuff))				//����
	// 		{	
	// 			//��ȡ�л�guid
	// 			tempBuff				   = GetCStringToString(cstrBuff, ":");
	// 			CGUID guid(tempBuff.c_str());
	// 			m_SaveFaction.FamilyGuid   = guid;
	// 			if ( guid != CGUID::GUID_INVALID )
	// 			{
	// 				//��ȡ��������
	// 				file.ReadString(cstrBuff);
	// 				tempBuff				      = GetCStringToString(cstrBuff, ":");
	// 				m_SaveFaction.strFamilyName   = tempBuff;
	// 
	// 				//��ȡ����ƺ�
	// 				file.ReadString(cstrBuff);
	// 				tempBuff				      = GetCStringToString(cstrBuff, ":");
	// 				m_SaveFaction.strFamilyTitle  = tempBuff;
	// 
	// 				////��ȡ����᳤guid
	// 				//file.ReadString(cstrBuff);
	// 				//tempBuff				       = GetCStringToString(cstrBuff, ":");
	// 				//CGUID leader_guid(tempBuff.c_str());
	// 				//m_SaveFaction.FamilyMasterGuid = leader_guid;
	// 
	// 				//��ȡ�л�job�ȼ�
	// 				file.ReadString(cstrBuff);
	// 				tempBuff				       = GetCStringToString(cstrBuff, ":");
	// 				long lJobLevel				   = atoi(tempBuff.c_str());
	// 				m_SaveFaction.lFamilyJobLevel  = lJobLevel;
	// 			}
	// 			else
	// 			{
	// 				break;
	// 			}
	// 		}
	// 		file.Close();
	// 	}	
}

// check faction data
void CCDKey::CheckFaction(CPlayer *pPlayer)
{
	if (pPlayer == NULL)						//ָ���ж�
	{
		return ;
	}

	if ( m_SaveFaction.FamilyGuid == CGUID::GUID_INVALID )
	{
		AddSaveFactionByTxt(pPlayer);
	}
	//
	//��⿪ʼ
	string strPlayerName = pPlayer->GetName();
	string strWirte;
	string cstrInt;

	//����л�guid
//	const CGUID &check_guid = pPlayer->GetFamilyGuid();
// 	if ( (check_guid != m_SaveFaction.FamilyGuid) && check_guid != CGUID::GUID_INVALID && m_SaveFaction.FamilyGuid != CGUID::GUID_INVALID)
// 	{	
// 		strWirte = GetWirteString(2, strPlayerName.c_str(), "���л�GUID��һ��,�����л����ݶ�ʧ");
// 		PutCheckFactionErr(strWirte.c_str());
// 		return ;
// 	}
// 	else if ((check_guid == m_SaveFaction.FamilyGuid) && check_guid != CGUID::GUID_INVALID && m_SaveFaction.FamilyGuid != CGUID::GUID_INVALID )
// 	{	
// 
// 		//������᳤guid
// 		//const CGUID &leader_guid = pPlayer->GetFamilyMasterGuid();
// 		//if ( (leader_guid != m_SaveFaction.FamilyMasterGuid) && m_SaveFaction.lFamilyJobLevel != -1)
// 		//{
// 		//	strWirte = GetWirteString(2, strPlayerName.c_str(), "���л�᳤GUID��һ��,");
// 		//	PutCheckFactionErr(strWirte.c_str());
// 		//	return ;
// 		//}
// 
// 		//����л�����
// 		strWirte = pPlayer->GetFamilyName();
// 		if ( strWirte.compare(m_SaveFaction.strFamilyName) != 0)
// 		{
// 			strWirte = GetWirteString(2, strPlayerName.c_str(), "���л����ֲ�һ��,");
// 			PutCheckFactionErr(strWirte.c_str());
// 			return ;
// 		}
// 
// 		////����л�ƺ�
// 		//strWirte = pPlayer->GetFamilyTitle();
// 		//if ( strWirte.compare(m_SaveFaction.strFamilyTitle) != 0)
// 		//{
// 		//	strWirte = GetWirteString(2, strPlayerName.c_str(), "���л�ƺŲ�һ��,");
// 		//	PutCheckFactionErr(strWirte.c_str());
// 		//	return ;
// 		//}
// 
// 		////����л�job�ȼ�
// 		//long lCheckJobLevel = pPlayer->GetFamilyJobLev();
// 		//if ( lCheckJobLevel != m_SaveFaction.lFamilyJobLevel)
// 		//{	
// 		//	strWirte = GetWirteString(2, strPlayerName.c_str(), "���л�joblevel��һ��,");
// 		//	PutCheckFactionErr(strWirte.c_str());
// 		//	return ;
// 		//}
// 	}	
}

void CCDKey::PutCheckFactionErr(const char *msg)
{	
	static int num=0;
	char t[128];

	FILE *fp;
	if( (fp=fopen("check_faction_Erro.txt","a+")) == NULL )
		return;

	num++;
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(t, "\n%d(%02d-%02d %02d:%02d:%02d):", num, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	fseek(fp, 0, SEEK_END);
	fwrite(t, lstrlen(t), 1, fp);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);

	//������� 20M  ɾ��debug�ļ�
	if( GetFileLength("check_faction_Erro.txt")/(1024*1024) > 20 )
	{
		DeleteFileA("check_faction_Erro.txt");
	}
}

void CCDKey::ClearSaveFaction(CPlayer *pPlayer)
{
	m_SaveFaction.FamilyGuid	   = CGUID::GUID_INVALID;
	m_SaveFaction.FamilyMasterGuid = CGUID::GUID_INVALID;

//	pPlayer->SetFamilyGuid(CGUID::GUID_INVALID);
	//pPlayer->SetFamilyMasterGuid(CGUID::GUID_INVALID);
}