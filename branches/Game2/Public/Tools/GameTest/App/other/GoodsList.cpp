#include "StdAfx.h"
#include ".\goodslist.h"
#include "..\goods.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

map<DWORD, CGoodsList::tagGoods2>		CGoodsList::m_mapGoodsList;			// ԭʼ��Ʒ�б�(wIndew, tagGoods)
map<string, CGoodsList::tagGoods2*>		CGoodsList::m_mapGoodsListByName;	// ������Ϊ������ԭʼ��Ʒ�б�(strName, *tagGoods)
map<DWORD/*��װID*/, CGoodsList::tagSuitAttribute>	CGoodsList::m_SuitList;				// ��װ�����Ա������������װ�ڸ��ּ��������¼������ܵ�����
map<string,DWORD>									CGoodsList::m_mapEffects;			// ӳ����Ʒ���ƺ͸�������
map<string,DWORD>									CGoodsList::m_mapAllEffects;		// ӳ�丽���������ƺ͸�������ö��ֵ
map<DWORD,string>									CGoodsList::m_mapEffectReverse;		// ӳ����Ʒ�������Ժ�����
map<DWORD/*�׿���ID��*/,CGoodsList::tagSeriesCardProperty>	CGoodsList::m_mapSeriesCardList;	// �׿������б�
vector<CGoodsList::tagMoneyGraphics> CGoodsList::m_stMoneyGraphics;

CGoodsList::CGoodsList(void)
{
}

CGoodsList::~CGoodsList(void)
{
	m_mapGoodsList.clear();
	m_mapGoodsListByName.clear();
	m_mapEffects.clear();
	m_mapEffects.clear();
	m_SuitList.clear();
	m_mapSeriesCardList.clear();
}

void CGoodsList::InitMapEffects()
{
	if (m_mapEffects.empty())
	{
		m_mapEffects["E_MaxHp"]=CGoods::GAP_MAXHP_CORRECT;//��������             

		m_mapEffects["E_MaxMp"]=CGoods::GAP_MAXMP_CORRECT;//��������                     

		m_mapEffects["E_HpRecoverSpeed"]=CGoods::GAP_HP_RESUME_CORRECT;//�����ָ��ٶ�     

		m_mapEffects["E_MpRecoverSpeed"]=CGoods::GAP_MP_RESUME_CORRECT;//�����ָ��ٶ�                     

		m_mapEffects["E_Strenth"]=CGoods::GAP_ADD_STR;//����         

		m_mapEffects["E_Agility"]=CGoods::GAP_ADD_DEX;//����          

		m_mapEffects["E_Consti"]=CGoods::GAP_ADD_CON;//����       

		m_mapEffects["E_Intell"]=CGoods::GAP_ADD_INT;//����

		m_mapEffects["E_Wisdom"]=CGoods::GAP_ADD_WIS;//�ǻ�

		m_mapEffects["E_Spirit"]=CGoods::GAP_ADD_SPI;//����         

		m_mapEffects["E_Block"]=CGoods::GAP_PARRY_CORRECT;//��            

		m_mapEffects["E_Parry"]=CGoods::GAP_BLOCK_CORRECT;//�м�            

		m_mapEffects["E_Dodge"]=CGoods::GAP_FLEE_CORRECT;//����           

		m_mapEffects["E_Hit"]=CGoods::GAP_HIT_RATE_CORRECT;//����

		m_mapEffects["E_Absorb"]=CGoods::GAP_DEDICANT_CORRECT;//רע            

		m_mapEffects["E_Def"]=CGoods::GAP_NOR_DEF;//�������  

		m_mapEffects["E_Mdef"]=CGoods::GAP_MAGIC_DEF;//ħ������            

		m_mapEffects["E_Cri"]=CGoods::GAP_NORATTACK_KILLRATE;//������         

		m_mapEffects["E_MAtkSpeed"]=CGoods::GAP_MAGIC_SPEED;//ʩ���ٶ�          

		m_mapEffects["E_MinAtk"]=CGoods::GAP_MIN_ATTACK;//��С������

		m_mapEffects["E_MaxAtk"]=CGoods::GAP_MAX_ATTACK;//��󹥻���

		m_mapEffects["E_MAtk"]=CGoods::GAP_FUMO_PROPERTY;//ħ��������            

		m_mapEffects["E_MCri"]=CGoods::GAP_MAGIC_KILLRATE;//ħ��������          

		m_mapEffects["E_BloodSuk"]=CGoods::GAP_LEECH_HP;//����������            

		m_mapEffects["E_ManaSuck"]=CGoods::GAP_LEECH_MP;//����������      

		m_mapEffects["E_WillDef"]=CGoods::GAP_WILL_DEF;//��־����     

		m_mapEffects["E_ConstiDef"]=CGoods::GAP_CON_DEF;//���ʿ���

		m_mapEffects["E_DeBuffDef_BLED"]=CGoods::GAP_BLED_DEF;//��Ѫ����

		m_mapEffects["E_DeBuffDef_DIZZY"]=CGoods::GAP_DIZZY_DEF;//���迹��

		m_mapEffects["E_DeBuffDef_LULL"]=CGoods::GAP_LULL_DEF;//��Կ���

		m_mapEffects["E_DeBuffDef_HYPNOSIS"]=CGoods::GAP_HYPNOSIS_DEF;//���߿���

		m_mapEffects["E_DeBuffDef_POSION"]=CGoods::GAP_POISON_DEF;//�ж�����

		m_mapEffects["E_DeBuffDef_FIX"]=CGoods::GAP_FIX_DEF;//������

		m_mapEffects["E_ElemDef_FIRE"]=CGoods::GAP_FIREDEF_CORRECT;//��        

		m_mapEffects["E_ElemDef_WATER"]=CGoods::GAP_WATERDEF_CORRECT;//ˮ��

		m_mapEffects["E_ElemDef_EARTH"]=CGoods::GAP_EARTHDEF_CORRECT;//�ؿ� 

		m_mapEffects["E_ElemDef_WIND"]=CGoods::GAP_WINDDEF_CORRECT;//�翹

		m_mapEffects["E_ElemDef_DARK"]=CGoods::GAP_DARKDEF_CORRECT;//�ڰ���

		m_mapEffects["E_ElemDef_HOLY"]=CGoods::GAP_HOLYDEF_CORRECT;//��ʥ��

		//m_mapEffects["E_Charm"]=CGoods::GAP_HOLYDEF_CORRECT;//����
	}
	//if (m_mapEffectReverse.empty())
	//{
	//	//��ת������ӳ��Map
	//	if (CStringReading::LoadString(206,23))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MAXHP_CORRECT] = CStringReading::LoadString(206,23);//��������
	//	}

	//	if (CStringReading::LoadString(206,24))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MAXMP_CORRECT]= CStringReading::LoadString(206,24);//�������� 
	//	}

	//	if (CStringReading::LoadString(206,25))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_HP_RESUME_CORRECT] = CStringReading::LoadString(206,25);//�����ָ��ٶ�
	//	}

	//	if (CStringReading::LoadString(206,26))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MP_RESUME_CORRECT] = CStringReading::LoadString(206,26);//�����ָ��ٶ�
	//	}

	//	if (CStringReading::LoadString(206,27))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_ADD_STR] = CStringReading::LoadString(206,27);//����
	//	}

	//	if (CStringReading::LoadString(206,28))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_ADD_DEX] = CStringReading::LoadString(206,28);//����
	//	}

	//	if (CStringReading::LoadString(206,29))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_ADD_CON] = CStringReading::LoadString(206,29);//����    
	//	}

	//	if (CStringReading::LoadString(206,30))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_ADD_INT] = CStringReading::LoadString(206,30);//����
	//	}

	//	if (CStringReading::LoadString(206,31))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_ADD_WIS] = CStringReading::LoadString(206,31);//�ǻ� 
	//	}

	//	if (CStringReading::LoadString(206,32))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_ADD_SPI] = CStringReading::LoadString(206,32);//����
	//	}

	//	if (CStringReading::LoadString(206,33))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_PARRY_CORRECT] = CStringReading::LoadString(206,33);//�� 
	//	}

	//	if (CStringReading::LoadString(206,34))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_BLOCK_CORRECT] = CStringReading::LoadString(206,34);//�м� 
	//	}

	//	if (CStringReading::LoadString(206,35))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_FLEE_CORRECT] = CStringReading::LoadString(206,35);//����  
	//	}

	//	if (CStringReading::LoadString(206,36))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_HIT_RATE_CORRECT] = CStringReading::LoadString(206,36);//����
	//	}

	//	if (CStringReading::LoadString(206,37))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_DEDICANT_CORRECT] = CStringReading::LoadString(206,37);//רע
	//	}

	//	if (CStringReading::LoadString(206,38))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_NOR_DEF] = CStringReading::LoadString(206,38);//�������
	//	}

	//	if (CStringReading::LoadString(206,39))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MAGIC_DEF] = CStringReading::LoadString(206,39);//ħ������
	//	}

	//	if (CStringReading::LoadString(206,40))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_NORATTACK_KILLRATE] = CStringReading::LoadString(206,40);//������
	//	}

	//	if (CStringReading::LoadString(206,41))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MAGIC_SPEED] = CStringReading::LoadString(206,41);//ʩ���ٶ�
	//	}

	//	if (CStringReading::LoadString(206,42))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MIN_ATTACK] = CStringReading::LoadString(206,42);//��С������
	//	}

	//	if (CStringReading::LoadString(206,43))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MAX_ATTACK] = CStringReading::LoadString(206,43);//��󹥻���
	//	}

	//	if (CStringReading::LoadString(206,44))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_FUMO_PROPERTY] = CStringReading::LoadString(206,44);//ħ��������
	//	}

	//	if (CStringReading::LoadString(206,45))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_MAGIC_KILLRATE] = CStringReading::LoadString(206,45);//ħ��������
	//	}

	//	if (CStringReading::LoadString(206,46))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_LEECH_HP] = CStringReading::LoadString(206,46);//����������
	//	}

	//	if (CStringReading::LoadString(206,47))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_LEECH_MP] = CStringReading::LoadString(206,47);//����������
	//	}

	//	if (CStringReading::LoadString(206,48))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_WILL_DEF] = CStringReading::LoadString(206,48);//��־����
	//	}

	//	if (CStringReading::LoadString(206,49))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_CON_DEF] = CStringReading::LoadString(206,49);//���ʿ���
	//	}

	//	if (CStringReading::LoadString(206,50))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_BLED_DEF] = CStringReading::LoadString(206,50);//��Ѫ����
	//	}

	//	if (CStringReading::LoadString(206,51))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_DIZZY_DEF] = CStringReading::LoadString(206,51);//���迹��
	//	}

	//	if (CStringReading::LoadString(206,52))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_LULL_DEF] = CStringReading::LoadString(206,52);//��Կ���
	//	}

	//	if (CStringReading::LoadString(206,53))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_HYPNOSIS_DEF] = CStringReading::LoadString(206,53);//���߿���
	//	}

	//	if (CStringReading::LoadString(206,54))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_POISON_DEF] = CStringReading::LoadString(206,54);//�ж�����
	//	}

	//	if (CStringReading::LoadString(206,55))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_FIX_DEF] = CStringReading::LoadString(206,55);//������
	//	}

	//	if (CStringReading::LoadString(206,56))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_FIREDEF_CORRECT] = CStringReading::LoadString(206,56);//��
	//	}

	//	if (CStringReading::LoadString(206,57))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_WATERDEF_CORRECT] = CStringReading::LoadString(206,57);//ˮ��
	//	}

	//	if (CStringReading::LoadString(206,58))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_EARTHDEF_CORRECT] = CStringReading::LoadString(206,58);//�ؿ�
	//	}

	//	if (CStringReading::LoadString(206,59))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_WINDDEF_CORRECT] = CStringReading::LoadString(206,59);//�翹
	//	}

	//	if (CStringReading::LoadString(206,60))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_DARKDEF_CORRECT] = CStringReading::LoadString(206,60);//�ڰ���
	//	}	      

	//	if (CStringReading::LoadString(206,61))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_HOLYDEF_CORRECT] = CStringReading::LoadString(206,61);//��ʥ��
	//	}                                                          

	//	if (CStringReading::LoadString(206,62))
	//	{
	//		m_mapEffectReverse[CGoods::GAP_HURT_SUCK_PER] = CStringReading::LoadString(206,62);//�˺����ձ���
	//	} 

	//}
	if (m_mapAllEffects.empty())
	{
		m_mapAllEffects["GAP_MAXHP_CORRECT"]=CGoods::GAP_MAXHP_CORRECT;//��������             

		m_mapAllEffects["GAP_MAXMP_CORRECT"]=CGoods::GAP_MAXMP_CORRECT;//��������                     

		m_mapAllEffects["GAP_HP_RESUME_CORRECT"]=CGoods::GAP_HP_RESUME_CORRECT;//�����ָ��ٶ�     

		m_mapAllEffects["GAP_MP_RESUME_CORRECT"]=CGoods::GAP_MP_RESUME_CORRECT;//�����ָ��ٶ�                     

		m_mapAllEffects["GAP_ADD_STR"]=CGoods::GAP_ADD_STR;//����         

		m_mapAllEffects["GAP_ADD_DEX"]=CGoods::GAP_ADD_DEX;//����          

		m_mapAllEffects["GAP_ADD_CON"]=CGoods::GAP_ADD_CON;//����       

		m_mapAllEffects["GAP_ADD_INT"]=CGoods::GAP_ADD_INT;//����

		m_mapAllEffects["GAP_ADD_WIS"]=CGoods::GAP_ADD_WIS;//�ǻ�

		m_mapAllEffects["GAP_ADD_SPI"]=CGoods::GAP_ADD_SPI;//����         

		m_mapAllEffects["GAP_PARRY_CORRECT"]=CGoods::GAP_PARRY_CORRECT;//��            

		m_mapAllEffects["GAP_BLOCK_CORRECT"]=CGoods::GAP_BLOCK_CORRECT;//�м�            

		m_mapAllEffects["GAP_FLEE_CORRECT"]=CGoods::GAP_FLEE_CORRECT;//����           

		m_mapAllEffects["GAP_HIT_RATE_CORRECT"]=CGoods::GAP_HIT_RATE_CORRECT;//����

		m_mapAllEffects["GAP_DEDICANT_CORRECT"]=CGoods::GAP_DEDICANT_CORRECT;//רע            

		m_mapAllEffects["GAP_NOR_DEF"]=CGoods::GAP_NOR_DEF;//�������  

		m_mapAllEffects["GAP_MAGIC_DEF"]=CGoods::GAP_MAGIC_DEF;//ħ������            

		m_mapAllEffects["GAP_NORATTACK_KILLRATE"]=CGoods::GAP_NORATTACK_KILLRATE;//������         

		m_mapAllEffects["GAP_MAGIC_SPEED"]=CGoods::GAP_MAGIC_SPEED;//ʩ���ٶ�          

		m_mapAllEffects["GAP_MIN_ATTACK"]=CGoods::GAP_MIN_ATTACK;//��С������

		m_mapAllEffects["GAP_MAX_ATTACK"]=CGoods::GAP_MAX_ATTACK;//��󹥻���

		m_mapAllEffects["GAP_FUMO_PROPERTY"]=CGoods::GAP_FUMO_PROPERTY;//ħ��������            

		m_mapAllEffects["GAP_MAGIC_KILLRATE"]=CGoods::GAP_MAGIC_KILLRATE;//ħ��������          

		m_mapAllEffects["GAP_LEECH_HP"]=CGoods::GAP_LEECH_HP;//����������            

		m_mapAllEffects["GAP_LEECH_MP"]=CGoods::GAP_LEECH_MP;//����������      

		m_mapAllEffects["GAP_WILL_DEF"]=CGoods::GAP_WILL_DEF;//��־����     

		m_mapAllEffects["GAP_CON_DEF"]=CGoods::GAP_CON_DEF;//���ʿ���

		m_mapAllEffects["GAP_BLED_DEF"]=CGoods::GAP_BLED_DEF;//��Ѫ����

		m_mapAllEffects["GAP_DIZZY_DEF"]=CGoods::GAP_DIZZY_DEF;//���迹��

		m_mapAllEffects["GAP_LULL_DEF"]=CGoods::GAP_LULL_DEF;//��Կ���

		m_mapAllEffects["GAP_HYPNOSIS_DEF"]=CGoods::GAP_HYPNOSIS_DEF;//���߿���

		m_mapAllEffects["GAP_POISON_DEF"]=CGoods::GAP_POISON_DEF;//�ж�����

		m_mapAllEffects["GAP_FIX_DEF"]=CGoods::GAP_FIX_DEF;//������

		m_mapAllEffects["GAP_FIREDEF_CORRECT"]=CGoods::GAP_FIREDEF_CORRECT;//��        

		m_mapAllEffects["GAP_WATERDEF_CORRECT"]=CGoods::GAP_WATERDEF_CORRECT;//ˮ��

		m_mapAllEffects["GAP_EARTHDEF_CORRECT"]=CGoods::GAP_EARTHDEF_CORRECT;//�ؿ� 

		m_mapAllEffects["GAP_WINDDEF_CORRECT"]=CGoods::GAP_WINDDEF_CORRECT;//�翹

		m_mapAllEffects["GAP_DARKDEF_CORRECT"]=CGoods::GAP_DARKDEF_CORRECT;//�ڰ���

		m_mapAllEffects["GAP_HOLYDEF_CORRECT"]=CGoods::GAP_HOLYDEF_CORRECT;//��ʥ��
	}
}

// ���ļ���ȡԭʼ��Ʒ�б�
BOOL CGoodsList::LoadGoodsList(const char* filename)
{
	m_mapGoodsList.clear();
	m_mapGoodsListByName.clear();

	//======================
	FILE* rF = fopen(filename,"rb"); 
	if (rF == NULL)
	{
		char cFileName[256];
		sprintf(cFileName,"��ʼ��%s����(�����ļ�δ�ܶ���)!",filename);
		PutDebugString(cFileName);
		return FALSE;
	}
	char rBuf[6] = "";  
	memset(rBuf, 0, 6);
	fread(&rBuf,5,1,rF); 
	if ( strcmp(rBuf,"GOODS") != 0)
		return FALSE;
	long lVersion = 0;
	fread(&lVersion,sizeof(long),1,rF);

	long num=0;
	fread(&num,sizeof(long),1,rF);

	SEEK_SET;
	SEEK_CUR;
	SEEK_END;

	for (int i=0;i<num;i++)
	{
		long lSize=0;
		tagGoods2 tGoods;

		fread(&tGoods.BaseProperty.dwIndex,sizeof(tGoods.BaseProperty.dwIndex),1,rF);
		//
		fread(&lSize,sizeof(long),1,rF);
		char *pStrOrginName=new char[lSize+1];
		fread(pStrOrginName,lSize,1,rF);
		pStrOrginName[lSize]='\0';
		tGoods.BaseProperty.strOrginName=pStrOrginName;
		SAFE_DELETE(pStrOrginName);

		fread(&lSize,sizeof(long),1,rF);
		char *pStrName=new char[lSize+1];
		fread(pStrName,lSize,1,rF);
		pStrName[lSize]='\0';
		tGoods.BaseProperty.strName=pStrName;
		SAFE_DELETE(pStrName);

		fread(&tGoods.BaseProperty.dwIconId, sizeof(tGoods.BaseProperty.dwIconId),1,rF);					//	����ͼ��ID
		fread(&tGoods.BaseProperty.dwGroundId, sizeof(tGoods.BaseProperty.dwGroundId),1,rF);				//	��غ�ͼ��ID
		fread(&tGoods.BaseProperty.dwEquipID, sizeof(tGoods.BaseProperty.dwEquipID),1,rF);					//	��װͼ��ID
		fread(&tGoods.BaseProperty.dwSound, sizeof(tGoods.BaseProperty.dwSound),1,rF);						//	ʰȡ����
		fread(&tGoods.BaseProperty.dwSoundID1, sizeof(tGoods.BaseProperty.dwSoundID1),1,rF);				//	�Ӷ�/���˺�����
		fread(&tGoods.BaseProperty.dwSoundID2, sizeof(tGoods.BaseProperty.dwSoundID2),1,rF);				//	���⹥��/��������
		fread(&tGoods.BaseProperty.bSoundSwitch, sizeof(tGoods.BaseProperty.bSoundSwitch),1,rF);			//	����ʱ�Ƿ����
		fread(&tGoods.BaseProperty.dwWeaponActType, sizeof(tGoods.BaseProperty.dwWeaponActType),1,rF);		//	������Ӧ�Ķ�������
		fread(&tGoods.BaseProperty.dwType, sizeof(tGoods.BaseProperty.dwType),1,rF);
		fread(&tGoods.BaseProperty.dwPrice,sizeof(tGoods.BaseProperty.dwPrice),1,rF);						//	��Ҽ۸�
		fread(&tGoods.BaseProperty.dwSilverPrice,sizeof(tGoods.BaseProperty.dwSilverPrice),1,rF);			//	���Ҽ۸�
		//		������������
		fread(&lSize, sizeof(long),1,rF);
		char *pStrContent=new char[lSize+1];
		fread(pStrContent, lSize,1,rF);
		pStrContent[lSize]='\0';
		tGoods.BaseProperty.strContent=pStrContent;
		SAFE_DELETE_ARRAY(pStrContent);
		//��������
		long lEffectNum = 0;
		fread(&lEffectNum, sizeof(long),1,rF);
		WORD wType=0;

		for(int j=1;j<=lEffectNum;j++)
		{
			tagEffect stEffect;
			fread(&wType,sizeof(WORD),1,rF);
			stEffect.wType = wType + 1;
			fread(&stEffect.bEnable,sizeof(bool),1,rF);
			fread(&stEffect.bHide,sizeof(bool),1,rF);
			fread(&stEffect.dwValue1,sizeof(DWORD),1,rF);
			fread(&stEffect.dwValue2,sizeof(DWORD),1,rF);

			tGoods.vectorEffect.push_back(stEffect);
		}	


		////////////////����װ����/////////////////////////
		fread(&lEffectNum, sizeof(long),1,rF);
		for(int i=0;i<lEffectNum;i++)
		{
			tagGoodsBeControlledAttri stSuitSingleGoodsAttri;
			fread(&(stSuitSingleGoodsAttri.wPropertyName),sizeof(WORD),1,rF);
			fread(&(stSuitSingleGoodsAttri.dwValue1),sizeof(long),1,rF);
			fread(&(stSuitSingleGoodsAttri.dwValue2),sizeof(long),1,rF);
			fread(&(stSuitSingleGoodsAttri.wActiveNum),sizeof(WORD),1,rF);
			tGoods.m_SuitProperty.push_back(stSuitSingleGoodsAttri);
		}

		fread(&lEffectNum, sizeof(long),1,rF);
		for(int i=0;i<lEffectNum;i++)
		{
			tagGoodsBeControlledAttri stGoodsAktiviertAttri;

			fread(&wType,sizeof(WORD),1,rF);
			stGoodsAktiviertAttri.wPropertyName = wType+1;
			fread(&(stGoodsAktiviertAttri.dwValue1),sizeof(DWORD),1,rF);
			fread(&(stGoodsAktiviertAttri.dwValue2),sizeof(DWORD),1,rF);
			fread(&(stGoodsAktiviertAttri.wActiveNum),sizeof(WORD),1,rF);

			tGoods.m_AkiviertProperty.push_back(stGoodsAktiviertAttri);
		}
		////////////////////////////////////////////////////
		m_mapGoodsList[tGoods.BaseProperty.dwIndex] = tGoods;
		m_mapGoodsListByName[tGoods.BaseProperty.strOrginName] = &m_mapGoodsList[tGoods.BaseProperty.dwIndex];
	}

	fclose(rF);
	return TRUE;
}
//���ļ���ȡ��װ�����б�
// void CGoodsList::LoadSuitList(const char* filename)
// {
// 	InitMapEffects();
// 	m_SuitList.clear();
// 	CRFile* prfile = rfOpen(filename);
// 
// 	if (prfile == NULL)
// 	{
// 		char str[256];
// 		sprintf(str, "file '%s' can't found!", filename);
// 		MessageBox(NULL, str, "error", MB_OK);
// 		return ;
// 	}
// 	stringstream  stream;
// 	prfile->ReadToStream(stream);
// 	rfClose(prfile);
// 
// 	DWORD suitID = 0;						//�����װ��ID��
// 	while (ReadTo(stream,"SUIT_ID"))
// 	{
// 		stream>>suitID;						
// 		if(ReadTo(stream,"<SUIT_GOODS>"))
// 		{		
// 			tagSuitAttribute tempSuit;
// 			//��ȡtagSuitAttribute��vector<string> Suit_OriginName��BYTE	wSuit_CountҪ��ŵ�����
// 			string tempString;
// 			while (true)				
// 			{					
// 				stream>>tempString;
// 				if (tempString == "</SUIT_GOODS>")
// 				{
// 					break;
// 				}
// 				tempSuit.wSuit_Count = (BYTE)atoi(tempString.c_str());
// 				string tempSuitOriginName;
// 				stream>>tempSuitOriginName;
// 				tempSuit.Suit_OriginName.push_back(tempSuitOriginName);
// 			}
// 			//��ȡmap<DWORD,map<DWORD,int>>��Ҫ��ŵ�����
// 			DWORD SuitNum;				//��װ���ܵļ���
// 			string SuitAttriName;		//��װ�ڶ�Ӧ�����¼������������
// 			DWORD SuitAttriValue;		//��Ӧ�����Ե�ֵ
// 			while (ReadTo(stream,"NUM"))
// 			{
// 				stream>>SuitNum;			
// 				if (ReadTo(stream,"<ATTR>"))
// 				{	
// 					map<DWORD,int> tempEffects;
// 					while (true)		
// 					{
// 						stream>>tempString;
// 						if (tempString == "</ATTR>"||tempString == "</ATTR_END>")
// 						{
// 							break;
// 						}
// 						SuitAttriName = tempString;
// 						stream>>SuitAttriValue;										//��ȡ������������ֵ
// 						DWORD SuitAttriEnum;
// 						SuitAttriEnum = m_mapEffects[SuitAttriName];					//ͨ���������ҵ���Ӧ������ö��ֵ
// 						tempEffects.insert(pair<DWORD,int>(SuitAttriEnum,SuitAttriValue));
// 						tempSuit.Suit_ActiveProperty[SuitNum] = tempEffects;
// 					}
// 					if (tempString == "</ATTR_END>")
// 					{
// 						break;
// 					}
// 				}
// 			}
// 			m_SuitList.insert(pair<DWORD,tagSuitAttribute>(suitID,tempSuit));
// 		}
// 	}
// }

// ���ļ���ȡ�׿��б���Ϣ
// void CGoodsList::LoadCardList(const char* filename)
// {
// 	InitMapEffects();
// 	m_mapSeriesCardList.clear();
// 	CRFile* prfile = rfOpen(filename);
// 
// 	if (prfile == NULL)
// 	{
// 		char str[256];
// 		sprintf(str, "file '%s' can't found!", filename);
// 		MessageBox(NULL, str, "error", MB_OK);
// 		return ;
// 	}
// 	stringstream  stream;
// 	prfile->ReadToStream(stream);
// 	rfClose(prfile);
// 
// 	DWORD cardID = 0;							//����׿���ID��
// 	string CardAttriName = "";					//�׿��������������
// 	DWORD CardAttriValue = 0;					//��Ӧ�����Ե�ֵ
// 	DWORD CardAttriEnum = 0;					//���Զ�Ӧ��ö��ֵ
// 
// 	while (ReadTo(stream,"<KIT_CARD>")&&!stream.eof())
// 	{
// 		char buf[256];
// 		int num = 0;
// 		tagSeriesCardProperty	tempCardProperty;
// 		if (ReadTo(stream,"KIT_CARD_ID"))
// 		{
// 			stream>>cardID;
// 			PutDebugString(ltoa(cardID,buf,10));
// 		}
// 		if (ReadTo(stream,"HOLE_NUM_REQUIRE"))
// 		{
// 			stream>>num;
// 			tempCardProperty.HOLE_NUM_REQUIRE = (BYTE)num;
// 			PutDebugString(ltoa((long)tempCardProperty.HOLE_NUM_REQUIRE,buf,10));
// 		}
// 		if (ReadTo(stream,"CARD_NUM_REQUIRE"))
// 		{
// 			stream>>num;
// 			tempCardProperty.CARD_NUM_REQUIRE = (BYTE)num;	
// 			PutDebugString(ltoa((long)tempCardProperty.CARD_NUM_REQUIRE,buf,10));
// 		}
// 		if(ReadTo(stream,"CARD_ID"))
// 		{		
// 			DWORD tempCardID = 0;
// 			for (BYTE i= 0;i<tempCardProperty.CARD_NUM_REQUIRE;i++)
// 			{
// 				stream>>tempCardID;
// 				PutDebugString(ltoa(tempCardID,buf,10));
// 				tempCardProperty.CardsID.push_back(tempCardID);
// 			}
// 		}
// 		if (ReadTo(stream,"<ATTR>"))
// 		{
// 			string tempString = "";
// 			while(true)
// 			{
// 				stream>>tempString;
// 				PutDebugString(tempString.c_str());
// 				if (tempString=="</ATTR>") break;
// 				if (tempString[0]=='/') continue;
// 				CardAttriName = tempString;
// 				stream>>CardAttriValue;
// 				PutDebugString(ltoa((long)CardAttriValue,buf,10));
// 				CardAttriEnum = m_mapAllEffects[CardAttriName];					//ͨ���������ҵ���Ӧ������ö��ֵ
// 				tempCardProperty.CardsEffects.insert(pair<DWORD,int>(CardAttriEnum,CardAttriValue));
// 			}
// 		}
// 		if (ReadTo(stream,"</KIT_CARD>")) 
// 		{
// 			m_mapSeriesCardList[cardID] = tempCardProperty;
// 		}
// 	}
// }

bool CGoodsList::FillGoodsInfo(DWORD dwIndex,CGoods *pGood)
{	
	if(!dwIndex)
		return false;
	tagBaseProperty p = GetProperty(dwIndex)->BaseProperty;
	pGood->SetName(GetProperty(dwIndex)->BaseProperty.strName.c_str());
	pGood->m_dwIndex=dwIndex;
	//pGood->m_lNum=1;
	//	pGood->m_stBaseProperty.strName=p.strName;
	pGood->m_vectorEffect=GetProperty(dwIndex)->vectorEffect;
	//	pGood->m_stBaseProperty.dwValue = p.dwPrice;
	//	pGood->m_stBaseProperty.dwSilverPrice = p.dwSilverPrice;
	//	pGood->m_stBaseProperty.dwType = p.dwType;
	//	pGood->SetGoodsBaseType((eGoodsBaseType)p.dwType);
	return true;
}

long CGoodsList::GetEffectVal(DWORD dwIndex,WORD wType,int nWhich)
{
	CGoodsList::tagGoods2* pstGoods2=CGoodsList::GetProperty(dwIndex);
	if (pstGoods2)
	{
		vector<CGoodsList::tagEffect>::iterator it=pstGoods2->vectorEffect.begin();
		for(;it!=pstGoods2->vectorEffect.end();it++)
		{
			if(it->wType==wType)
			{
				if(nWhich==1)
					return it->dwValue1;
				else if(nWhich==2)
					return it->dwValue2;
			}
		}
	}
	return 0;
}

CGoodsList::tagEffect* CGoodsList::GetEffectTag(DWORD dwIndex,WORD wType)
{
	CGoodsList::tagGoods2* pstGoods=CGoodsList::GetProperty(dwIndex);
	if(pstGoods)
	{
		vector<CGoodsList::tagEffect>::iterator it=pstGoods->vectorEffect.begin();
		for(;it!=pstGoods->vectorEffect.end();it++)
		{
			if(it->wType == wType)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

// bool CGoodsList::InitMoneyGraphics(const char* filename)
// {
// 	//m_stMoneyGraphics[0].dwNum=50;
// 	//m_stMoneyGraphics[0].lGraphicsID=80000;
// 
// 	//m_stMoneyGraphics[1].dwNum=100;
// 	//m_stMoneyGraphics[1].lGraphicsID=80001;
// 
// 	//m_stMoneyGraphics[2].dwNum=500;
// 	//m_stMoneyGraphics[2].lGraphicsID=80002;
// 
// 	//m_stMoneyGraphics[3].dwNum=1000;
// 	//m_stMoneyGraphics[3].lGraphicsID=80003;
// 
// 	//m_stMoneyGraphics[4].lGraphicsID=80004;
// 	m_stMoneyGraphics.clear();
// 
// 	tagMoneyGraphics stMoney;
// 
// 	stringstream stream;	
// 	CRFile* prfile = rfOpen(filename);
// 	if( !prfile )
// 	{
// 		char str[256];
// 		sprintf(str, "file '%s' can't found!", filename);
// 		MessageBox(NULL, str, "error", MB_OK);
// 		return FALSE;
// 	}
// 	prfile->ReadToStream(stream);
// 	rfClose(prfile);
// 
// 
// 	while(ReadTo(stream, "#"))
// 	{
// 		stream >> stMoney.lMinNum >> stMoney.lMaxNum >> stMoney.lGraphicsID;
// 		m_stMoneyGraphics.push_back(stMoney);
// 	}
// 
// 	return TRUE;
// }

long CGoodsList::GetMoneyGraphics(long lMoneyNum)
{
	vector<tagMoneyGraphics>::iterator iter = m_stMoneyGraphics.begin();
	for (;iter!=m_stMoneyGraphics.end();iter++)
	{
		if (lMoneyNum>=iter->lMinNum&&lMoneyNum<=iter->lMaxNum)
		{
			return iter->lGraphicsID;
		}
	}
	return 0;
}

// ����װ����װ��Ż�ȡ��Ʒ����
CGoodsList::tagGoods2*	CGoodsList::GetPropertyByEquipID(DWORD dwEquipID)
{
	for (map<DWORD, tagGoods2>::iterator it=m_mapGoodsList.begin(); it!=m_mapGoodsList.end(); it++)
	{
		if (it->second.BaseProperty.dwEquipID == dwEquipID)
		{
			return &it->second;
		}
	}
	return NULL;
}

//��ȡ��ID����װ��Ʒ����������(nWhich = 1 ȡ��װ������Ʒ��ĳ������ֵ1�� = 2ȡ����ֵ2��= 3ȡ����ֵ3���������Ǽ�����������ﵽ����װ��Ʒ������)
long CGoodsList::GetSuitAttrOfGoods(DWORD dwIndex,DWORD wType,int nWhich)
{
	CGoodsList::tagGoods2* pstGoods2=CGoodsList::GetProperty(dwIndex);
	if (pstGoods2)
	{
		vector<CGoodsList::tagGoodsBeControlledAttri>::iterator it=pstGoods2->m_SuitProperty.begin();
		for(;it!=pstGoods2->m_SuitProperty.end();it++)
		{
			if(it->wPropertyName==wType)
			{
				if(nWhich==1)
					return it->dwValue1;
				else if(nWhich==2)
					return it->dwValue2;
				else if(nWhich == 3)
					return it->wActiveNum;
			}
		}
	}
	return 0;
}

//��ȡ��ID����װ���ܼ���
BYTE CGoodsList::GetSuitCount(DWORD SuitID)	
{
	if (SuitID!=0&&m_SuitList.find(SuitID)!=m_SuitList.end())
	{
		return m_SuitList[SuitID].wSuit_Count;
	}
	return 0;
}

//����ĳ����Ƭ��ID���Ҵ����׿���ID
DWORD CGoodsList::GetSeriesCardIndexByCardID(DWORD cardIndex)
{
	map<DWORD/*�׿���ID��*/,tagSeriesCardProperty>::iterator it = m_mapSeriesCardList.begin();
	for (;it!=m_mapSeriesCardList.end();it++)
	{
		vector<DWORD> temp = m_mapSeriesCardList[it->first].CardsID;
		for (int i = 0;i<temp.size();i++)
		{
			if (cardIndex==temp[i])
			{
				return it->first;
			}
		}
	}
	return 0;
}