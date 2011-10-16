#include "StdAfx.h"
#include ".\MonsterList.h"
#include "..\setup\GlobeSetup.h"
#include "../../public/StrUnifyInput.h"
#include "../../public/StrType.h"
#ifdef _GAME_SERVER
#include "..\GameServer\AppServer\goods\cgoodsfactory.h"
#endif

#ifdef _WORLD_SERVER_
#include "..\WorldServer\AppWorld\goods\cgoodsfactory.h"
#endif

#ifdef _MONSTER_SERVER
#include "..\MonsterServer\AppMonster\goods\cgoodsfactory.h"
#endif


extern HWND g_hWnd;

map<string, CMonsterList::tagMonster>			CMonsterList::m_mapMonsterList;		// ԭʼMonster�б�(strOrginName, tagMonster)
//map<string, CMonsterList::tagDropGoods>		CMonsterList::m_mapDropGoods;		// ����Ʒ����(strMonsterOrginName, tagDropGoods)
vector<CMonsterList::tagDropGoods*>				CMonsterList::m_vecDropList;					//drop�Ĺ���
map<string , CMonsterList::tgMonsterDrop>		CMonsterList::m_mapMonsterDrop;			//monster����
map<int, int>									CMonsterList::m_mapRegionDrop;				//region����
vector<int>										CMonsterList::m_vecLevelDrop;					//�ȼ�����
int												CMonsterList::m_iWordDrop;					//������䣨������	

#ifdef _GAME_SERVER
//===========================================================================================================================
///
/// These codes below are only used for the Goods Drop function.
///
//===========================================================================================================================
struct Range
{
	int Min;
	int Max;
	size_t Index;

	Range( int _min, int _max, size_t _i ) : Min( _min ), Max( _max ), Index( _i ) 
	{
	}
};

/// range list
typedef std::vector<Range> RangeListType;

/// goods list
typedef std::vector<CMonsterList::tagDropGoods::tagOneGoods> GoodListType;

/// build the range list for goods list
static size_t BuildRangeTable( const GoodListType &goods, RangeListType &ranges )
{
	std::size_t goods_size = goods.size();
	std::size_t size = ranges.size();
	int prev = 0;
	for( std::size_t i = 0; i < goods_size; ++ i )
	{
		ranges.push_back( Range( prev + 1, goods[i].lProbability + prev, i ) );
		prev += goods[i].lProbability;
	}
	return ranges.size() - size;
}

/// query the goods index by a random number
static size_t QueryGoodsIndex( const RangeListType &ranges, int value )
{
	for( RangeListType::const_iterator it = ranges.begin(); it != ranges.end(); ++ it )
	{
		if( value >= (*it).Min && value <= (*it).Max )
		{
			return (*it).Index;
		}
	}
	// fatal error
	return (size_t)(-1);
}

///
/// ��Ʒ����ȼ�˥�����㺯��
///
#define COMP_RANGE 10e-5

static long CalLevelOdds( long level_sub, long odds_max, long odds, float level_atte, float level_atte_min )
{
	if( level_sub <= 0 || ( level_atte > -COMP_RANGE && level_atte < COMP_RANGE ) )
	{
		return odds;
	}
	// ����Ϊ�����odds_max������ֵ�������е�level_atte�������odds��
	long level_atte_l = level_atte * level_sub * odds;
	long level_min_l = level_atte_min * odds_max;

	if( odds <= level_min_l )
	{
		// ���ô��󣬲���ԭʼ����
		return odds;
	}

	long ret_odds = odds - level_atte_l;
	// ˥����СΪlevel_min_l
	ret_odds = ret_odds < level_min_l ? level_min_l : ret_odds;

	return ret_odds;
}

#endif
//===========================================================================================================================

CMonsterList::CMonsterList(void)
{
}

CMonsterList::~CMonsterList(void)
{
	m_mapMonsterList.clear();
	//m_vecDropList;//.clear();
	for (vector<tagDropGoods*>::iterator it=m_vecDropList.begin(); it!=m_vecDropList.end(); it++)
	{
		delete *it;
	}
	m_vecDropList.clear();
}

// ���ļ���ȡԭʼ��Ʒ�б�
BOOL CMonsterList::LoadMonsterList(const char* filename)
{
	m_mapMonsterList.clear();

	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		char str[256];
		_snprintf(str, 256, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return FALSE;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	//int i = 0;
	while(ReadTo(stream, "*"))
	{
		//if(++i == 132)
		//{
		//	int y = 0;
		//}
		tagMonster stMonster;
		int a=0;
		string strOriginalName;
		string strName;
		string strCollectionName;
		string strBornScript;
		string strFightScript;
		string strGBHScript;
		string strDeadScript;
		string strTitleName;
		string strCycleScript;
		string strSearchEnemyScript;
		string strFindTargetScriptName;
		string strKillTargetScriptName;

		stream	>>  stMonster.dwIndex
			>>  strOriginalName                 //ԭʼ��//..�ȶ�����һ����ʱstr�����У��ж��ڶ�
			>> 	strName                         //����
			>> 	*(int*)(&stMonster.byBlockType)				//�Ƿ����赲
			>>	*(int*)(&stMonster.byIsunbeatable)
			>>  *(int*)&stMonster.DropFlag
			>>  strCollectionName				//�ɼ�������
			>>	strTitleName					//��ν����
			>>  stMonster.dwTitleColor			//��ν��ʾ��ɫ
			>>  stMonster.dwTitlePicID			//��νͼƬ
			>> 	*(int*)(&stMonster.byCountry)	//����ID
			>> 	*(int*)(&stMonster.eNType)   //Npc��� (NNPC,Monster,Guard)
			>>  stMonster.wBossLevel			//boss�ȼ�
			>> 	stMonster.dwPicID				//ͼ��ID
			>> 	stMonster.dwPicLevel			//ͼ�εȼ�
			>> 	stMonster.dwNameColor			//������ɫ���,��Ӧ���ͻ�������]
			>> 	stMonster.dwColorOfLevel        //�ȼ���ɫ
			>> 	stMonster.dwHpBarColor			//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
			>> 	stMonster.dwSoundID				//����ID
			>> 	*(int*)(&stMonster.byTamable)	//�Ƿ����ѱ��
			>> 	stMonster.dwMaxTameCount		//�����ѱ������
			>> 	stMonster.wFigure				//����(0:1x1 1:3*3 2:5*5)*
			>> 	stMonster.wLevel			    //�ȼ�
			>> 	stMonster.dwExp					//����ֵ*
			>>	stMonster.dwOccuPoint			//ְҵ����
			>> 	stMonster.dwMaxHP               //���hp
			>> 	stMonster.wMinAtk				//��С������		    
			>> 	stMonster.wMaxAtk				//��󹥻���
			>> 	stMonster.wMinMAtk				//��С����������
			>> 	stMonster.wMaxMAtk				//�����������
			>> 	stMonster.wHit					//��������
			>> 	stMonster.lDef					//������
			>>  stMonster.lMDef					//ħ��������
			>> 	stMonster.wBlock				//��
			>> 	stMonster.wParry				//�м�
			>>  stMonster.lImmunity				//����
			>>  stMonster.wPierce				//��͸
			>> 	stMonster.wDodge				//��������
			//>> 	stMonster.wAtcSpeed				//�����ٶ�......
			>> 	stMonster.fMoveSpeed			//�ƶ��ٶ�
			>>	stMonster.fFightMoveSpeed		//ս���ƶ��ٶ�
			>>  stMonster.fTurningSpeed			//ת���ٶ�
			>> 	stMonster.lWillDef;				//��־����
		//stMonster.wElemDef[ET_Max];			//����Ԫ�ؿ���
		for(a=0;a<ET_Max;a++)
		{
			stream  >> stMonster.lElemDef[a];
		}

		stream	>> 	stMonster.wHpRecoverSpeed		//�����ָ��ٶ�
			>> 	stMonster.wMPRecoverSpeed		//ħ���ָ��ٶ�
			//			>> 	stMonster.wBaseMoveSpeed			//�ƶ��ٶ�
			>> 	stMonster.lBaseConstiDef;			//���ʿ���
	
		for(a=0; a<DET_Max;a++)
		{
			stream >>stMonster.lDeBuffDef[a];
		}

		stream	>> 	stMonster.wMinStopTimes		//����ͣ������
			>> 	stMonster.wMaxStopTimes			//���ͣ������
			>> 	stMonster.wMinRunTimes			//�����ƶ�����
			>>  stMonster.wMaxRunTimes			//����ƶ�����
			>> 	stMonster.wGuardRange			//���䷶Χ*
			>> 	stMonster.wChaseRange			//׷����Χ*
			>>  stMonster.wPeaceMoveRange		//��ƽ�ƶ���Χ
			>> 	stMonster.wAIModel              //AIģʽ
			>>	stMonster.wCanTalk				//�Ƿ�ɶԻ�
			>> 	*(int*)(&stMonster.eClass_)//��������
			>> 	*(int*)(&stMonster.eEType)	//Ԫ�����
			//>> 	stMonster.wClass				//���ࡣ��
			>> 	stMonster.wStopTime				//ͣ����ʱ��(��һ�������м�ͣ��֡��)
			>> 	stMonster.wAIHangupType;			//AI��϶
		//>> 	stMonster.wClassDam[C_Max];	//�Ը��������˺�
		for(a=0; a<C_Max; a++)
		{
			stream   >>stMonster.wClassDam[a];
		}
		//>> 	stMonster.wElemDam[ET_Max];	//�Ը���Ԫ���˺�
		for(a=0; a<ET_Max; a++)
		{
			stream   >>stMonster.wElemDam[a];
		}

		stream>>stMonster.wMoveRandomValue   //���� 
			>>stMonster.wCri				 //��������
			>>stMonster.wCriDamg			 //�������˺�
			>>stMonster.lCriDef				 //����������
			>>stMonster.wMCri				 //ħ��������
			>>stMonster.wMCriDamg			 //ħ�������˺�
			>>stMonster.lMCriDef;			 //ħ����������


		stream>>stMonster.fHurtProportion    //���˱���
			>>stMonster.dwGuardTime          //����ʱ��
			>>stMonster.fEscapePoint         //���ܵ���
			>>stMonster.wSkillProportion     //���ܸ���
			>>stMonster.dwSkillSpace	     //���ܼ��
			>>*(int*)(&stMonster.byIsFlyToPeace)//ת��Ϊ��ƽ״̬�Ƿ�ɻ�ȥ
			>>*(int*)(&stMonster.byIsResumeAll)//ת��Ϊ��ƽ״̬�Ƿ���ȫ�ָ� 
			>>stMonster.wReturnRange		 //��λ����
			>>stMonster.dwChaseTime;		 //׷��ʱ�䣬��׷����Χ���û�ܵ�����ת��Ϊ��ƽ��ʱ��

		for( a=0; a<MONSTER_AI_REFERRENCE_NUM; a++)
		{
			stream>>stMonster.dwAIReferrence [a];//ai�ο�  1����5
		}
		stream
			>>strBornScript					//���������ű�
			>>strFightScript				//ս�������ű�
			>>strGBHScript 					//���˴����ű�
			>>strDeadScript					//���������ű�
			>>strCycleScript				//���ڴ����ű�
			>>strSearchEnemyScript			//ѡ��Ŀ��ű�
			>> strFindTargetScriptName		//Ѱ�ҵ�Ŀ��ű�
			>> strKillTargetScriptName;		//ɱ��Ŀ��ű�

		stream
			>>stMonster.m_arrElementProperty[ME_Earth].uArrPro[AD_Attack]
			>>stMonster.m_arrElementProperty[ME_Water].uArrPro[AD_Attack]
			>>stMonster.m_arrElementProperty[ME_Fire].uArrPro[AD_Attack]
			>>stMonster.m_arrElementProperty[ME_Wind].uArrPro[AD_Attack]
			>>stMonster.m_arrElementProperty[ME_Bright].uArrPro[AD_Attack]
			>>stMonster.m_arrElementProperty[ME_Dark].uArrPro[AD_Attack]

			>>stMonster.m_arrElementProperty[ME_Earth].uArrPro[AD_Defend]
			>>stMonster.m_arrElementProperty[ME_Water].uArrPro[AD_Defend]
			>>stMonster.m_arrElementProperty[ME_Fire].uArrPro[AD_Defend]
			>>stMonster.m_arrElementProperty[ME_Wind].uArrPro[AD_Defend]
			>>stMonster.m_arrElementProperty[ME_Bright].uArrPro[AD_Defend]
			>>stMonster.m_arrElementProperty[ME_Dark].uArrPro[AD_Defend];
		//��ȡ����

		int nLen = strOriginalName.size ();  //ԭʼ����
		if( nLen<50)
		{
			memcpy(stMonster.cOriginalName,strOriginalName.c_str(),nLen);
			stMonster.cOriginalName[nLen]='\0';
		}
		else
		{
			strncpy(stMonster.cOriginalName,strOriginalName.c_str(),50-1);
			stMonster.cOriginalName[50-1]='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"OriginalName");
		}

		nLen = strName.size ();   //����
		if(nLen<50)
		{
			memcpy(stMonster.cName ,strName.c_str(),nLen);
			stMonster.cName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cName ,strName.c_str(),50-1);
			stMonster.cName[50-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"Name");

		}

		nLen = strCollectionName.size ();   //����
		if(nLen<50)
		{
			memcpy(stMonster.cCollectionName ,strCollectionName.c_str(),nLen);
			stMonster.cCollectionName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cCollectionName ,strCollectionName.c_str(),50-1);
			stMonster.cCollectionName[50-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"CollectionName");

		}


		nLen = strTitleName.size ();   //����
		if(nLen<50)
		{
			memcpy(stMonster.cTitleName ,strTitleName.c_str(),nLen);
			stMonster.cTitleName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cTitleName ,strTitleName.c_str(),50-1);
			stMonster.cTitleName[50-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"TitleName");
		}
		nLen = strBornScript.size ();  //���������ű�
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cBornScriptName ,strBornScript.c_str(),nLen);
			stMonster.cBornScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cBornScriptName ,strBornScript.c_str(),MAX_PATH-1);
			stMonster.cBornScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"BornScriptName");

		}

		nLen = strFightScript.size ();  //ս�������ű�
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cFightScriptName ,strFightScript.c_str(),nLen);
			stMonster.cFightScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cFightScriptName ,strFightScript.c_str(),MAX_PATH-1);
			stMonster.cFightScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"FightScriptName");

		}

		nLen = strGBHScript.size ();  //���˴���
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cGBHScriptName ,strGBHScript.c_str(),nLen);
			stMonster.cGBHScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cGBHScriptName ,strGBHScript.c_str(),MAX_PATH-1);
			stMonster.cGBHScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"GBHScriptName");

		}

		nLen = strDeadScript.size ();  //���������ű�
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cDeadScriptName ,strDeadScript.c_str(),nLen);
			stMonster.cDeadScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cDeadScriptName ,strDeadScript.c_str(),MAX_PATH-1);
			stMonster.cDeadScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"DeadScriptName");
		}

		nLen = strCycleScript.size ();  //���ڴ����ű�
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cCycleScriptName ,strCycleScript.c_str(),nLen);
			stMonster.cCycleScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cCycleScriptName ,strCycleScript.c_str(),MAX_PATH-1);
			stMonster.cCycleScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"CycleScriptName");
		}

		nLen = strSearchEnemyScript.size ();  //ѡ���ܽű�
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cSearchEnemyScriptName ,strSearchEnemyScript.c_str(),nLen);
			stMonster.cSearchEnemyScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cSearchEnemyScriptName ,strSearchEnemyScript.c_str(),MAX_PATH-1);
			stMonster.cSearchEnemyScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"SearchEnemyScriptName");
		}
		//
		nLen = strFindTargetScriptName.size ();  //Ѱ�ҵ�Ŀ��
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cFindTargetScriptName ,strFindTargetScriptName.c_str(),nLen);
			stMonster.cFindTargetScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cFindTargetScriptName ,strFindTargetScriptName.c_str(),MAX_PATH-1);
			stMonster.cFindTargetScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"FindTargetScriptName");
		}
		//
		nLen = strKillTargetScriptName.size ();  //ɱ��Ŀ��
		if(nLen<MAX_PATH)
		{
			memcpy(stMonster.cKillTargetScriptName ,strKillTargetScriptName.c_str(),nLen);
			stMonster.cKillTargetScriptName[nLen] ='\0';
		}
		else
		{
			memcpy(stMonster.cKillTargetScriptName ,strKillTargetScriptName.c_str(),MAX_PATH-1);
			stMonster.cKillTargetScriptName[MAX_PATH-1] ='\0';
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_DataSlopOver),
				"KillTargetScriptName");
		}
		//str�����жϽ���
		//ֵ�жϲ���
		if (stMonster.eNType<0 || stMonster.eNType>=NT_Max ||
			stMonster.eClass_<0 || stMonster.eClass_>=C_Max||
			stMonster.eEType<0 || stMonster.eEType>= ET_Max ||
			stMonster.wFigure>10)
		{
			//�������
			extern void AddLogText(char*, ...);
			AddLogText(CStringReading::LoadString(IDS_WS_MONSTERFILE_ERROR, WS_MONSTERLIST_IndexDataSlopOver));
		}

		if (stMonster.byBlockType==1)
		{
			stMonster.byBlockType=4;
		}
		else if (stMonster.byBlockType==2)
		{
			stMonster.byBlockType=3;
		}
		else
			stMonster.byBlockType=0;

		m_mapMonsterList[stMonster.cOriginalName] = stMonster;
	}

	return TRUE;
}

// ���浽�ļ�
BOOL CMonsterList::SaveMonsterList(const char* filename)
{
	return TRUE;
}

BOOL CMonsterList::LoadMonsterDrop(const char* filename)
{
	CRFile* prFile = rfOpen(filename);
	if (prFile == NULL)
	{
		return FALSE;
	}
	TiXmlNode *pNode = NULL; 		

	TiXmlDocument T_dox(filename);
	TiXmlDocument* m_Tdoc = & T_dox; 

	if(!m_Tdoc->LoadData(prFile->GetData(),prFile->GetDatalen()))
	{
		//���������Ϣ��������ļ���
		return FALSE;
	}
	rfClose(prFile);
	pNode=m_Tdoc->FirstChild("DropList");

	if (pNode==NULL)
	{
		//�޽ڵ㣬�������
		return FALSE;
	}

	TiXmlElement* te=pNode->ToElement();//��ȡnode ��ָ��
	if (te==NULL)
	{
		//���������Ϣ
		return FALSE;
	}


	TiXmlElement* p=te->FirstChildElement();

	for (; p!=NULL; p=p->NextSiblingElement())
	{
		if (!strcmp(p->Value(),"Drop"))
		{
			TiXmlElement* children=p->FirstChildElement();
			int sharedrop=DROPINDEX_NULL;
			list<string>  sharemonstername;

			for (;children!=NULL; children=children->NextSiblingElement())
			{
				if (!strcmp(children->Value(),"Drop_Block"))
				{
					tagDropGoods* dropgoods=LoadOneDrop(children);
					if (dropgoods!=NULL)
					{
						m_vecDropList.push_back(dropgoods);
						sharedrop=m_vecDropList.size()-1;
					}
				}
				else if (!strcmp(children->Value(),"MonsterName"))
				{
					string name=children->Attribute("name");
					tagDropGoods* dropgoods=LoadOneDrop(children);
					if (dropgoods!=NULL)
					{
						m_vecDropList.push_back(dropgoods);
						int index=m_vecDropList.size()-1;
						m_mapMonsterDrop[name].OwnDropIndex=index;
					}
					sharemonstername.push_back(name);
				}
			}

			for (list<string> ::iterator it= sharemonstername.begin(); it!= sharemonstername.end(); it++)
			{
				m_mapMonsterDrop[*it].ShareDropIndex=sharedrop;
			}
		}
	}
	return TRUE; 
}
BOOL CMonsterList::LoadActivityDrop(const char* filename)
{
	CRFile* prFile = rfOpen(filename);
	if (prFile == NULL)
	{
		return FALSE;
	}
	TiXmlNode *pNode = NULL; 		
	m_iWordDrop = DROPINDEX_NULL;
	TiXmlDocument T_dox(filename);
	TiXmlDocument* m_Tdoc = & T_dox; 

	if(!m_Tdoc->LoadData(prFile->GetData(),prFile->GetDatalen()))
	{
		//���������Ϣ��������ļ���
		return FALSE;
	}
	rfClose(prFile);
	pNode=m_Tdoc->FirstChild("DropList");

	if (pNode==NULL)
	{
		//�޽ڵ㣬�������
		return FALSE;
	}

	TiXmlElement* te=pNode->ToElement();//��ȡnode ��ָ��
	if (te==NULL)
	{
		//���������Ϣ
		return FALSE;
	}


	TiXmlElement* p=te->FirstChildElement();



	for (; p!=NULL; p=p->NextSiblingElement())
	{
		if (!strcmp(p->Value(),"World_Drop"))
		{
			tagDropGoods* dropgoods=LoadOneDrop(p);
			if (dropgoods!=NULL)
			{
				m_vecDropList.push_back(dropgoods);
				m_iWordDrop=m_vecDropList.size()-1;
			}
		}
		else if (!strcmp(p->Value(),"Grade_Drop"))
		{
			tagDropGoods* dropgoods=LoadOneDrop(p);
			if (dropgoods==NULL)
			{
				break;
			}
			m_vecDropList.push_back(dropgoods);
			int dropindex=m_vecDropList.size()-1;

			long MinLevel=0;
			long MaxLevel=0;
			if (!GetTwoNum(p->Attribute("levels"),MinLevel, MaxLevel))
			{
				break;
			}
			if (m_vecLevelDrop.size()<=MaxLevel)
			{
				m_vecLevelDrop.resize(MaxLevel+1);
			}
			for (int a=MinLevel; a<=MaxLevel; a++)
			{
				m_vecLevelDrop[a]=dropindex;
			}
		}
		else if (!strcmp(p->Value(),"Monster_Drop"))
		{
			string name=p->Attribute("name");
			tagDropGoods* dropgoods=LoadOneDrop(p);
			if (dropgoods==NULL)
			{
				break;
			}
			m_vecDropList.push_back(dropgoods);
			int dropindex=m_vecDropList.size()-1;
			m_mapMonsterDrop[name].ActivityIndex=dropindex;
		}
		else
		{

		}
	}
	return TRUE; 
}
BOOL CMonsterList::LoadRegionDrop(const char* filename,long regionID)
{
	TiXmlNode *pNode = NULL; 		

	TiXmlDocument T_dox(filename);
	TiXmlDocument* m_Tdoc = & T_dox; 

	if(!m_Tdoc->LoadFile())
	{
		//���������Ϣ��������ļ���
		return FALSE;
	}

	pNode=m_Tdoc->FirstChild("RegionDrop");

	if (pNode==NULL)
	{
		//�޽ڵ㣬�������
		return FALSE;
	}

	TiXmlElement* te=pNode->ToElement();//��ȡnode ��ָ��
	if (te==NULL)
	{
		//���������Ϣ
		return FALSE;
	}


	TiXmlElement* p=te->FirstChildElement();



	for (; p!=NULL; p=p->NextSiblingElement())
	{
		tagDropGoods* dropgoods=LoadOneDrop(p);
		if (dropgoods!=NULL)
		{
			m_vecDropList.push_back(dropgoods);
			m_mapRegionDrop[regionID]=m_vecDropList.size()-1;
		}
	}
	return TRUE; 
}
CMonsterList::tagDropGoods* CMonsterList::LoadOneDrop(TiXmlElement* te)
{
	if (te==NULL)
	{
		return NULL;
	}


	TiXmlElement* p=te->FirstChildElement();
	if (p==NULL)
	{
		return NULL;
	}

	CMonsterList::tagDropGoods* md= new CMonsterList::tagDropGoods;

	for (; p!=NULL; p=p->NextSiblingElement() )
	{
		if (!strcmp(p->Value(),"DropGoods"))
		{
			tagDropGoods::tagDrop drop;

			drop.strName=p->Attribute("goodsname");
			if (!GetTwoNum(p->Attribute("odds"),drop.lOdds,drop.lMaxOdds))
			{
				//����
				break;
			}
			
			if( !GetTwoNum( p->Attribute( "level" ), drop.lMinLevel, drop.lMaxLevel ) )
			{
				// failed, set default
				drop.lMinLevel = drop.lMaxLevel = 0;
			}
			drop.fLevelAttenuation=atof(p->Attribute("levelattenuation"));
			drop.fLevelAttenuationLimit=atof(p->Attribute("levelattenuationlimit"));
			drop.lGoodsIndex=CGoodsFactory::QueryGoodsIDByOriginalName( drop.strName.c_str() );

			md->vectorDrop.push_back(drop);
		}
		else if (!strcmp(p->Value(),"DropMoney"))
		{
			tagDropGoods::tagDrop drop;

			//const char* pszGoldName = //CGoodsFactory::GetGoldOrginName();
			//if(pszGoldName)
			//	drop.strName = pszGoldName;
			//else
			//	drop.strName = "";
			drop.strName=p->Attribute("goodsname");

			if (!GetTwoNum(p->Attribute("odds"),drop.lOdds,drop.lMaxOdds))
			{
				//����
				break;
			}
			if (!GetTwoNum(p->Attribute("dropnum"),drop.lMinMoney,drop.lMaxMoney))
			{
				//����
				break;
			}
			drop.fLevelAttenuation=atof(p->Attribute("levelattenuation"));
			drop.fLevelAttenuationLimit=atof(p->Attribute("levelattenuationlimit"));
			drop.lGoodsIndex=CGoodsFactory::QueryGoodsIDByOriginalName( drop.strName.c_str() );

			md->vectorDrop.push_back(drop);
		}
		else if (!strcmp(p->Value(), "DropGroup"))
		{
			tagDropGoods::tagFixedDrop fa;
			int goodsnum=0;
			TiXmlElement* children=p->FirstChildElement();
			for (; children!=NULL; children=children->NextSiblingElement())
			{
				goodsnum++;
				if (!strcmp(children->Value(),"OneDrop"))
				{
					tagDropGoods::tagOneGoods onegoods;
					onegoods.strName=children->Attribute("dropname");
					onegoods.lGoodsIndex=CGoodsFactory::QueryGoodsIDByOriginalName( onegoods.strName.c_str() );
					onegoods.lProbability=atoi(children->Attribute("dropodds"));
					
					if( !GetTwoNum( children->Attribute( "level" ), onegoods.lMinLevel, onegoods.lMaxLevel ) )
					{
						// failed, set default
						onegoods.lMinLevel = onegoods.lMaxLevel = 0;
					}

					fa.vecOneDropGroup.push_back(onegoods);
				}
				else
				{
				}
			}
			fa.iMaxDropNum=atoi(p->Attribute("maxdrop"));
			fa.iMinDropNum=atoi(p->Attribute("mindrop"));

			const char *attr = p->Attribute( "levelattenuation" );
			fa.fLevelAttenuation = attr != NULL ? atof( attr ) : 0.0f;
			attr = p->Attribute( "levelattenuationlimit" ); 
			fa.fLevelAttenuationLimit = attr != NULL ? atof( attr ) : 0.0f;

			// ��ȡ����ü���
			if( !GetTwoNum( p->Attribute( "odds" ), fa.lOdds, fa.lMaxOdds ) )
			{
				fa.lOdds = 1;
				fa.lMaxOdds = 1;
			}

			if (fa.iMinDropNum<0)
			{
				fa.iMaxDropNum=0;
			}

			if (fa.iMinDropNum>fa.iMaxDropNum)
			{
				fa.iMinDropNum=fa.iMaxDropNum;
			}

			md->vecFixedDrop.push_back(fa);
		}
		else
		{
			//������Ϣ
		}
	}

	if (md->vecFixedDrop.size()==0 && md->vectorDrop.size()==0)
	{
		delete md;
		return NULL;
	}
	return md;
}
BOOL CMonsterList::GetTwoNum(const char* str, long &first, long &second)
{
	if( str == 0 )
	{
		return FALSE;
	}

	int len=strlen(str);
	if (len==0 || len>64)
	{
		return FALSE;
	}

	string firstnum;
	string secongnum;

	int place=0;

	for (; place<len ; place++)
	{
		char c=*(str+place);
		if (c=='/')
		{
			++place;
			break;
		}
		else if (c>='0' && c<='9')
		{
			firstnum+=c;
		}
		else
		{
			return FALSE;
		}
	}

	if (place >=len)
	{
		return FALSE;
	}

	for (; place<len ; place++)
	{
		char c=*(str+place);
		if (c=='/')
		{
			++place;
			break;
		}
		else if (c>='0' && c<='9')
		{
			secongnum+=c;
		}
		else
		{
			return FALSE;
		}
	}

	if (firstnum.size()==0 || secongnum.size()==0)
	{
		return FALSE;
	}

	first=atoi(firstnum.c_str());
	second=atoi(secongnum.c_str());
	return TRUE;
}
// ���浽�ļ�
//bool CMonsterList::SaveDropGoodsList(const char* filename)
//{
//	return TRUE;
//}

bool CMonsterList::LoadSkillList(const char* filename)
{
	CRFile *prfile = rfOpen(filename);
	if(NULL == prfile)
	{
		//���������Ϣ
		char strError[128];
		_snprintf(strError, 128, "CDeputyOccuSystem::Init(%s) failed",filename);
		MessageBox(NULL,strError,"",MB_OK); 
		return FALSE;
	}

	TiXmlNode *pNode = NULL; 		
	TiXmlDocument T_dox;//TiXmlDocument T_dox(filename);
	TiXmlDocument* m_Tdoc = & T_dox;
	//���뼼�������ļ�
	if( !T_dox.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		char strError[128];
		_snprintf(strError, 128, "CDeputyOccuSystem::Init(%s) failed",filename);
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);

	pNode=m_Tdoc->FirstChild("MonsterSkillList");

	if (pNode==NULL)
	{
		return FALSE;
	}

	TiXmlElement* te=pNode->ToElement();//��ȡnode ��ָ��
	if (te==NULL)
	{
		return FALSE;
	}
	TiXmlElement* p=te->FirstChildElement();

	for (; p!=NULL; p=p->NextSiblingElement())
	{
		if (!strcmp(p->Value(),"Monster"))
		{
			int BaseSkill=0;
			int CommonSkill=0;
			int CycleSkill=0;
			//tagMonster* point=0;
			const char * name=p->Attribute("OriginalName");
			tagMonster* point=GetPropertyByName(name);
			if (point== NULL) continue;

			for (TiXmlElement* skill= p->FirstChildElement(); 
				skill!= NULL; skill= skill->NextSiblingElement())
			{
				const char * ppp= skill->Value();

				if (!strcmp(skill->Value(),"BaseSkill"))
				{
					if (BaseSkill<MONSTER_SKILL_SUM)
					{
						tagSkillInfo info;
						skill->Attribute("id",(int*)&info.dwSkillID);
						skill->Attribute("level",(int*)&info.dwSkillLevel);
						skill->Attribute("cooltime",(int*)&info.dwCoolDownTime);
						skill->Attribute("actmodify",&info.iActModifyTime);
						point->BaseSkillList[BaseSkill]=info;
						++BaseSkill;
					}
					else
						continue;
				}
				else if (!strcmp(skill->Value(),"CommonSkill"))
				{
					if (CommonSkill<MONSTER_SKILL_SUM)
					{
						tagSkillInfo info;
						skill->Attribute("id",(int*)&info.dwSkillID);
						skill->Attribute("level",(int*)&info.dwSkillLevel);
						skill->Attribute("cooltime",(int*)&info.dwCoolDownTime);
						skill->Attribute("actmodify",&info.iActModifyTime);
						point->CommonSkillList[CommonSkill]=info;
						++CommonSkill;
					}
					else
						continue;
				}
				else if (!strcmp(skill->Value(),"CycleSkill"))
				{
					if (CycleSkill<MONSTER_SKILL_SUM)
					{
						tagSkillInfo info;
						skill->Attribute("id",(int*)&info.dwSkillID);
						skill->Attribute("level",(int*)&info.dwSkillLevel);
						skill->Attribute("cooltime",(int*)&info.dwCoolDownTime);
						skill->Attribute("actmodify",&info.iActModifyTime);
						point->CycleSkillList[CycleSkill]= info;
						++CycleSkill;
					}
					else
						continue;
				}
				else
				{

				}

			}
		}

	}

	return true;

}

bool CMonsterList::SaveSkillList(const char*filename)
{
	return false;
}
// ��ӵ�CByteArray
bool CMonsterList::AddToByteArray(vector<BYTE>* pByteArray)
{
	// ��������
	_AddToByteArray(pByteArray, (long)m_mapMonsterList.size());

	for(map<string, tagMonster>::iterator it=m_mapMonsterList.begin(); it!=m_mapMonsterList.end(); it++)
	{
		tagMonster *pMonster = &(*it).second;
		if( pMonster )
		{
			_AddToByteArray(pByteArray, (BYTE*)pMonster, (long)sizeof(tagMonster));
		}
	}

	// ����Ʒ�Ͳ���
	// droplist:
	_AddToByteArray(pByteArray, (long)m_vecDropList.size());

	for(vector<tagDropGoods*>::iterator it=m_vecDropList.begin(); it!=m_vecDropList.end(); it++)
	{
		tagDropGoods *pDrop =*it;
		if( pDrop )
		{
			//_AddToByteArray(pByteArray, index);
			_AddToByteArray(pByteArray, (long)pDrop->vectorDrop.size());
			for(int i=0; i<(long)pDrop->vectorDrop.size(); i++)
			{
				_AddToByteArray(pByteArray, (char*)pDrop->vectorDrop[i].strName.c_str());
				_AddToByteArray(pByteArray, (BYTE*)&pDrop->vectorDrop[i], sizeof(tagDropGoods::tagDrop)-sizeof(string));
			}
			//.....add by chenaly..07/6/22
			_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop.size());

			for (int a=0; a<(long)pDrop->vecFixedDrop.size(); a++)
			{
				_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].iMinDropNum);
				_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].iMaxDropNum);
				_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].lOdds );
				_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].lMaxOdds );
				_AddToByteArray(pByteArray, (float)pDrop->vecFixedDrop[a].fLevelAttenuation );
				_AddToByteArray(pByteArray, (float)pDrop->vecFixedDrop[a].fLevelAttenuationLimit );

				_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].vecOneDropGroup.size());

				for (int b=0; b< pDrop->vecFixedDrop[a].vecOneDropGroup.size(); b++)
				{
					_AddToByteArray(pByteArray, (char*)pDrop->vecFixedDrop[a].vecOneDropGroup[b].strName.c_str());
					_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].vecOneDropGroup[b].lProbability);
					_AddToByteArray(pByteArray, (long)pDrop->vecFixedDrop[a].vecOneDropGroup[b].lGoodsIndex);
					_AddToByteArray(pByteArray,(long)pDrop->vecFixedDrop[a].vecOneDropGroup[b].lMinLevel);
					_AddToByteArray(pByteArray,(long)pDrop->vecFixedDrop[a].vecOneDropGroup[b].lMaxLevel);
				}
			}
			//....end ...................
		}
	}
	// m_mapMonsterDrop
	_AddToByteArray(pByteArray, (long)m_mapMonsterDrop.size());
	for (map<string,tgMonsterDrop>::iterator it=m_mapMonsterDrop.begin(); it!=m_mapMonsterDrop.end();it++)
	{
		string name=(*it).first;
		_AddToByteArray(pByteArray,(*it).first.c_str());
		_AddToByteArray(pByteArray,&(*it).second,sizeof((*it).second));
	}
	//m_mapRegionDrop
	_AddToByteArray(pByteArray,(long)m_mapRegionDrop.size());

	for( map<int,int>::iterator it=m_mapRegionDrop.begin(); it!=m_mapRegionDrop.end(); it++)
	{
		_AddToByteArray(pByteArray,(long)(*it).first);
		_AddToByteArray(pByteArray,(long)(*it).second);
	}
	//m_vecLevelDrop
	_AddToByteArray(pByteArray,(long)m_vecLevelDrop.size());
	for (vector<int>::iterator it= m_vecLevelDrop.begin(); it!=m_vecLevelDrop.end(); it++)
	{
		_AddToByteArray(pByteArray,(long)*it);
	}
	//
	_AddToByteArray(pByteArray,(long)m_iWordDrop);
	return TRUE;
}

// ����
bool CMonsterList::DecordFromByteArray(BYTE* pByte, long& pos)
{
	Release();//������ݣ������ض�BUG
	m_mapMonsterList.clear();
	//m_mapDropGoods.clear();
	//m_vecRoadsList.clear();

	char strTemp[256];

	// ��������

	long lMonsterNum = _GetLongFromByteArray(pByte, pos);
	for(int i=0; i<lMonsterNum; i++)
	{
		tagMonster stMonster;
		_GetBufferFromByteArray(pByte, pos, &stMonster, sizeof(tagMonster));

		m_mapMonsterList[string(stMonster.cOriginalName)] = stMonster;
	}

	// ����Ʒ����
	//droplist
	long lDropNum = _GetLongFromByteArray(pByte, pos);
	for(int i=0; i<lDropNum; i++)
	{
		tagDropGoods* stDrop=new tagDropGoods;

		//stDrop.strMonsterOrginName = _GetStringFromByteArray(pByte, pos, strTemp);
		long lGoodsNum = _GetLongFromByteArray(pByte, pos);
		for(int j=0; j<lGoodsNum; j++)
		{
			tagDropGoods::tagDrop stGoods;
			stGoods.strName = _GetStringFromByteArray(pByte, pos, strTemp);
			_GetBufferFromByteArray(pByte, pos, &stGoods, sizeof(tagDropGoods::tagDrop)-sizeof(string));
			stDrop->vectorDrop.push_back(stGoods);
		}
		//add by chenaly .....07/6/22.......
		lGoodsNum=_GetLongFromByteArray(pByte, pos);
		for (int a=0; a< lGoodsNum; a++)
		{
			tagDropGoods::tagFixedDrop stFix;
			stFix.iMinDropNum=_GetLongFromByteArray(pByte, pos);
			stFix.iMaxDropNum=_GetLongFromByteArray(pByte, pos);
			stFix.lOdds = _GetLongFromByteArray(pByte, pos);
			stFix.lMaxOdds = _GetLongFromByteArray(pByte, pos);
			stFix.fLevelAttenuation = _GetFloatFromByteArray(pByte, pos);
			stFix.fLevelAttenuationLimit = _GetFloatFromByteArray(pByte, pos);
			long dropgroup=_GetLongFromByteArray(pByte, pos);
			for (int b=0; b<dropgroup; b++)
			{
				tagDropGoods::tagOneGoods onegoods;
				onegoods.strName=_GetStringFromByteArray(pByte, pos, strTemp);
				onegoods.lProbability=_GetLongFromByteArray(pByte, pos);
				onegoods.lGoodsIndex=_GetLongFromByteArray(pByte, pos);
				onegoods.lMinLevel=_GetLongFromByteArray(pByte,pos);
				onegoods.lMaxLevel=_GetLongFromByteArray(pByte,pos);
				stFix.vecOneDropGroup.push_back(onegoods);
			}
			stDrop->vecFixedDrop.push_back(stFix);
		}
		m_vecDropList.push_back(stDrop);
	}
	//tgMonsterDrop
	long MonsterNum= _GetLongFromByteArray(pByte, pos);
	for (int a=0; a<MonsterNum; a++)
	{
		string name=_GetStringFromByteArray(pByte,pos,strTemp);
		tgMonsterDrop monsterdrop;
		_GetBufferFromByteArray(pByte,pos,&monsterdrop,sizeof(tgMonsterDrop));
		m_mapMonsterDrop[name]=monsterdrop;
	}
	//m_mapRegionDrop;
	long RegionDropNum=_GetLongFromByteArray(pByte,pos);
	for (int a=0; a<RegionDropNum; a++)
	{
		long reiondIndex=_GetLongFromByteArray(pByte,pos);
		long dropindex=_GetLongFromByteArray(pByte,pos);
		m_mapRegionDrop[reiondIndex]=dropindex;
	}
	//m_vecLevelDrop;					//�ȼ�����
	long WorldDropNum=_GetLongFromByteArray(pByte,pos);
	for (int a=0; a<WorldDropNum; a++)
	{
		long dropindex=_GetLongFromByteArray(pByte,pos);
		m_vecLevelDrop.push_back(dropindex);
	}
	//m_iWordDrop;					//������䣨������	
	m_iWordDrop=_GetLongFromByteArray(pByte,pos);

	return TRUE;
}


// ����ָ����NPC����
CMonsterList::tagMonster* CMonsterList::GetProperty(DWORD dwPicID)
{
	for(map<string, tagMonster>::iterator it=m_mapMonsterList.begin(); it!=m_mapMonsterList.end(); it++)
	{
		if( (*it).second.dwPicID == dwPicID )
		{
			return &(*it).second;
		}
	}
	return NULL;
}

CMonsterList::tagMonster* CMonsterList::GetPropertyByName(const char* strName)
{
	map<string, tagMonster>::iterator it=m_mapMonsterList.find(strName);
	if (it!=m_mapMonsterList.end())
	{
		return &(*it).second;
	}
	return NULL;
}

CMonsterList::tagMonster* CMonsterList::GetPropertyByOrginName(const char* strName)
{
	return (m_mapMonsterList.find(strName)!=m_mapMonsterList.end()) ? (&m_mapMonsterList[strName]) : NULL;
}

CMonsterList::tagMonster* CMonsterList::GetPropertyByOrginIndex(DWORD dwIndex)
{
	for(map<string, tagMonster>::iterator it=m_mapMonsterList.begin(); it!=m_mapMonsterList.end(); it++)
	{
		if( (*it).second.dwIndex == dwIndex )
		{
			return &(*it).second;
		}
	}
	return NULL;
}

// ����ָ���ĵ���Ʒ��
//CMonsterList::tagDropGoods* CMonsterList::GetDropGoods(const char* strName)
//{
//	return (m_mapDropGoods.find(strName)!=m_mapDropGoods.end()) ? &m_mapDropGoods[strName] : NULL;
//}
#ifdef _GAME_SERVER
BOOL CMonsterList::CheckDropGoods(string MonsterName, INT RegionIndex, INT Level,INT LevelBalances, vector<CGoods*> &vOut, BYTE drop_flag )
{
	//tgMonsterDrop* drop= m_mapMonsterDrop[MonsterName];
	map<string,tgMonsterDrop>::iterator it=m_mapMonsterDrop.find(MonsterName);
	if (it!=m_mapMonsterDrop.end())
	{
		tgMonsterDrop *md = &(*it).second;

		if (md->ShareDropIndex!=DROPINDEX_NULL && md->ShareDropIndex<m_vecDropList.size())
		{
			CMonsterList::tagDropGoods *drop=m_vecDropList[md->ShareDropIndex];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

		if (md->OwnDropIndex !=DROPINDEX_NULL && md->OwnDropIndex <m_vecDropList.size())
		{
			CMonsterList::tagDropGoods *drop=m_vecDropList[md->OwnDropIndex];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

		if (md->ActivityIndex !=DROPINDEX_NULL && md->ActivityIndex <m_vecDropList.size())
		{
			CMonsterList::tagDropGoods *drop=m_vecDropList[md->ActivityIndex];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

	}

	if( drop_flag != 1 )
	{
		map<int,int>::iterator ite=m_mapRegionDrop.find(Level);
		if (ite!=m_mapRegionDrop.end())
		{
			int index=(*ite).second;
			if (index>= 0 && index< m_vecDropList.size())
			{
				CMonsterList::tagDropGoods *drop=m_vecDropList[index];
				CheckOneDrop(drop,LevelBalances,vOut);
			}
		}

		if (Level>=0 && Level< m_vecLevelDrop.size())
		{
			int index=m_vecLevelDrop[Level];
			CMonsterList::tagDropGoods *drop=m_vecDropList[index];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

		if (m_vecDropList.size()>0 && m_iWordDrop >=0 && m_iWordDrop <m_vecDropList.size())
		{
			CMonsterList::tagDropGoods *drop=m_vecDropList[m_iWordDrop];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

	}
	return TRUE;
}
BOOL CMonsterList::CheckOneDrop(CMonsterList::tagDropGoods* pDrop,int LevelBalance, vector<CGoods*> &vOut)
{
	//1 ���Ƿ�������ɱ  2 �Ƿ��������� 3 �ȼ���
	//	CMonsterList::tagDropGoods* pDrop;//=CMonsterList::GetDropGoods(this->GetName());//GetProperty() -> strOrginName.c_str() );
	//##������Ʒ����ʼλ��
	LONG lDropGoodsStartPos = 0;
	if(pDrop )// bKilledByGuard == FALSE && 
	{
		for( size_t i = 0; i < pDrop -> vectorDrop.size(); i++ )
		{
			// ������Ʒ
			long lMaxOdds = pDrop->vectorDrop[i].lMaxOdds;
			long lOdds = pDrop->vectorDrop[i].lOdds;

			// ����ȼ�˥�� Kevin Lynx 4.7.2009 ����
			lOdds = CalLevelOdds( LevelBalance, lMaxOdds, lOdds, pDrop->vectorDrop[i].fLevelAttenuation, pDrop->vectorDrop[i].fLevelAttenuationLimit );

			if( lMaxOdds && random(lMaxOdds) < static_cast<LONG>( lOdds * CGlobeSetup::GetSetup() -> fMonsterDropGoodsScale ) )
			{
				DWORD dwAmount = 1;
				// Ǯ
				if( pDrop->vectorDrop[i].lGoodsIndex == CGoodsFactory::GetGoldCoinIndex() ||pDrop->vectorDrop[i].lGoodsIndex == CGoodsFactory::GetSilverIndex())
				{
					dwAmount = pDrop->vectorDrop[i].lMinMoney + random(abs(pDrop->vectorDrop[i].lMaxMoney - pDrop->vectorDrop[i].lMinMoney));
				}
				//				vector<CGoods*> vOut;
#ifdef __MEMORY_LEAK_CHECK__
				CBaseObject::SetConstructFlag(59);
#endif
				CGoods *pEquipment = NULL;
				if( dwAmount == 1 )
				{
					pEquipment = CGoodsFactory::CreateGoods( pDrop->vectorDrop[i].lGoodsIndex ,2);
					if( pEquipment != NULL )
					{
						vOut.push_back( pEquipment );
					}
				}
				else
				{
					CGoodsFactory::CreateGoods( pDrop -> vectorDrop[i].lGoodsIndex, dwAmount, vOut,3 );
				}
				// װ�����伴���(+1, +2, etc)
				long lMinLevel = pDrop->vectorDrop[i].lMinLevel;
				long lMaxLevel = pDrop->vectorDrop[i].lMaxLevel;
				if( pEquipment != NULL && lMaxLevel - lMinLevel > 0 )
				{
					// [lMinLevel, lMaxLevel]
					long level = random( lMaxLevel - lMinLevel + 1 ) + lMinLevel;
					CGoodsFactory::SetEquipmentLevel( pEquipment, level );
				}

			}
		}

		// �̶���Ʒ����
		for( size_t i = 0; i < pDrop->vecFixedDrop.size(); ++ i )
		{
			tagDropGoods::tagFixedDrop *pGoodsGroup = &pDrop->vecFixedDrop[i];
			// ����ȼ�˥������
			long lOdds = CalLevelOdds( LevelBalance, pGoodsGroup->lMaxOdds, pGoodsGroup->lOdds, pGoodsGroup->fLevelAttenuation, pGoodsGroup->fLevelAttenuationLimit );

			if( random( pGoodsGroup->lMaxOdds ) >= lOdds ) 
			{
				continue;
			}

			int BaseDropNum = pGoodsGroup->iMaxDropNum - pGoodsGroup->iMinDropNum ;
			int DropNum = random( BaseDropNum ) + pGoodsGroup->iMinDropNum;
			int MaxProbability = 0;
			for( size_t j = 0; j < pGoodsGroup->vecOneDropGroup.size(); ++ j )
			{
				MaxProbability += pGoodsGroup->vecOneDropGroup[j].lProbability ;
			}

			// build the range list
			RangeListType ranges;
			BuildRangeTable( pGoodsGroup->vecOneDropGroup, ranges );
			assert( ranges.size() == pGoodsGroup->vecOneDropGroup.size() );

			// create goods.
			for( int num = 0; num < DropNum; ++ num )
			{
				int probability = random( MaxProbability ) + 1;
				// query the goods index by this probability
				size_t index = QueryGoodsIndex( ranges, probability );
				assert( index < ranges.size() );	
				if( index < pGoodsGroup->vecOneDropGroup.size() )
				{
					CGoods *pGoods = CGoodsFactory::CreateGoods( pGoodsGroup->vecOneDropGroup[index].lGoodsIndex,4 );
					vOut.push_back( pGoods );
					
					// װ�����伴���(+1, +2, etc)
					long lMinLevel = pGoodsGroup->vecOneDropGroup[index].lMinLevel;
					long lMaxLevel = pGoodsGroup->vecOneDropGroup[index].lMaxLevel;
					if ( pGoods != NULL && lMaxLevel - lMinLevel > 0)
					{
						// [lMinLevel, lMaxLevel]
						long level = random( lMaxLevel - lMinLevel + 1 ) + lMinLevel;
						CGoodsFactory::SetEquipmentLevel( pGoods, level );
					}
				}
			}
		}
	}
	return FALSE;
}
#endif

// ˢ��WORLD�е�����NPC����ָ��
bool CMonsterList::RefeashAllMonster()
{
	return true;
}

void CMonsterList::Release()
{
	m_mapMonsterList.clear();
	for (vector<tagDropGoods*>::iterator it=m_vecDropList.begin(); it!=m_vecDropList.end(); it++)
	{
		delete *it;
	}
	m_vecDropList.clear();
}
