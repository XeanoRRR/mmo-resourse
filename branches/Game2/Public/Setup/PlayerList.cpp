#include "StdAfx.h"
#include "PlayerList.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//string PlayerList::s_strOccupation[PlayerList::OC_NUMS] = {"��ʿ", "����", "����"};
string PlayerList::s_strOccupation[OCC_Max] = 
{"������", "սʿ", "��ʦ","����","��ʿ","��ʿ","���","����","��ʿ","��ʦ","ʥ��ʿ","����ʿ",
"�Ƕ�ʿ","��սʿ","Ѳ��","�̿�","������","����ʫ��","��ʦ","������ʦ","����","����"};
string PlayerList::s_strSex[PlayerList::SEX_NUMS] = {"��", "Ů"};


vector<PlayerList::tagLevelUp>					PlayerList::m_vectorPlayerExp;		// �����������ֵ
vector<PlayerList::tagOccuLevelUp>				PlayerList::m_vcOccuLvlExp[OCC_Max];//ÿ��ְҵ�ȼ������б�
DWORD											PlayerList::m_dwLevelPoint = 0;	// ÿ��һ������ҷ���ĵ���
list<PlayerList::tagOrginEquip>				PlayerList::m_listOrginEquip;		// ��ʼװ���б�

long PlayerList::m_lBasePropertyValue;//��������ֵ
float PlayerList::m_fPropertyLvlParam;//���Եȼ�ϵ��
map<DWORD, PlayerList::tagOccupModifyProperty>	PlayerList::m_mapOccuModifyPropertyList;   //ְҵ������
map<DWORD,PlayerList::tagOccuDefParam>	PlayerList::m_mapOccuDefParam;//��ְҵ�ķ�������ֵ
map<DWORD,PlayerList::tagOccuAtkParam>	PlayerList::m_mapOccuAtkParam;//��ְҵ�Ĺ�������ֵ

PlayerList::PlayerList(void)
{
}

PlayerList::~PlayerList(void)
{
	m_dwLevelPoint = 0;
	//	m_mapPlayerList.clear();
	m_vectorPlayerExp.clear();
	m_listOrginEquip.clear();
	m_mapOccuModifyPropertyList.clear();
}


// ���ļ���ȡexp�б�
bool PlayerList::LoadPlayerExpList(const char* filename)
{
	m_vectorPlayerExp.clear();

	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,filename);
		return false;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	string strTemp;
	ReadTo(stream, "*");
	stream >> strTemp >> m_dwLevelPoint;

	tagLevelUp st;
	while( ReadTo(stream, "#") )
	{
		stream >> st.dwExp >> st.dwExp >> st.dwExpMultiple >> st.lPresentExpPerMinute >> st.lMaxPresentExp >> st.fFacBoonExpScale >> st.lBaseCP;
		m_vectorPlayerExp.push_back(st);
	}

	return true;
}


//װ��ְҵ�ȼ������б�
bool PlayerList::LoadOccuLvlExpList()
{
	char pszFileName[256]="";
	for(int i=0;i<OCC_Max;i++)
	{
		m_vcOccuLvlExp[i].clear();
		_snprintf(pszFileName,256,"data/LevelExp/Occu%02dExp.ini",i);
		CRFile* prfile = rfOpen(pszFileName);
		if(prfile == NULL)
		{
			Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,pszFileName);
			return false;
		}
		stringstream stream;
		prfile->ReadToStream(stream);
		rfClose(prfile);
		tagOccuLevelUp stOccuLvl;
		long lLvl=0;
		while( ReadTo(stream, "#") )
		{
			stream >> lLvl >> stOccuLvl.dwExp;
			m_vcOccuLvlExp[i].push_back(stOccuLvl);
			if(m_vcOccuLvlExp[i].size() != lLvl)
			{
				Log4c::Error(ROOT_MODULE, "%-15s file '%s' config error!", __FUNCTION__,pszFileName);
				return false;
			}
		}
	}
	return true;
}

bool PlayerList::AddToByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray,(long)m_dwLevelPoint);     //����ÿһ�ȼ��ļ��ܵ�
	
	long lLevelNum =(long)m_vectorPlayerExp.size();       /////������Ҿ����
	_AddToByteArray(pByteArray, lLevelNum);
	for(int i=0; i<lLevelNum; i++)
	{
		_AddToByteArray(pByteArray, &m_vectorPlayerExp[i], (long)sizeof(tagLevelUp));
	}

	for(int i=0;i<OCC_Max;i++)
	{
		long lLevelNum =(long)m_vcOccuLvlExp[i].size();       /////����ְҵ�����
		_AddToByteArray(pByteArray, lLevelNum);
		for(int j=0; j<lLevelNum; j++)
		{
			_AddToByteArray(pByteArray, &m_vcOccuLvlExp[i][j], (long)sizeof(tagOccuLevelUp));
		}
	}

	_AddToByteArray(pByteArray,m_lBasePropertyValue);
	_AddToByteArray(pByteArray,m_fPropertyLvlParam);
	_AddToByteArray(pByteArray, (long)m_mapOccuModifyPropertyList.size() );//���͵ȼ�����
	map<DWORD, PlayerList::tagOccupModifyProperty>::iterator itUpgrade = m_mapOccuModifyPropertyList.begin();
	for( ; itUpgrade != m_mapOccuModifyPropertyList.end(); itUpgrade ++ )
	{
		tagOccupModifyProperty &tPoint = (*itUpgrade).second;
		_AddToByteArray(pByteArray, &tPoint,(long)sizeof(tagOccupModifyProperty));
	}

	map<DWORD,tagOccuDefParam>::iterator it = m_mapOccuDefParam.begin();
	_AddToByteArray(pByteArray,(long)m_mapOccuDefParam.size());
	for(;it != m_mapOccuDefParam.end();it++)
	{
		tagOccuDefParam &OccuDefParam = (*it).second;
		_AddToByteArray(pByteArray,(*it).first);
		_AddToByteArray(pByteArray, &OccuDefParam, (long)sizeof(OccuDefParam));
	}

	map<DWORD,tagOccuAtkParam>::iterator itAtk = m_mapOccuAtkParam.begin();
	_AddToByteArray(pByteArray,(long)m_mapOccuAtkParam.size());
	for(;itAtk != m_mapOccuAtkParam.end();itAtk++)
	{
		tagOccuAtkParam &OccuAtkParam = (*itAtk).second;
		_AddToByteArray(pByteArray,(*itAtk).first);
		_AddToByteArray(pByteArray, &OccuAtkParam, (long)sizeof(OccuAtkParam));
	}
	
	return true;
}

bool PlayerList::DecordFromByteArray(uchar* pByte, long& pos)
{
	m_vectorPlayerExp.clear();
	m_mapOccuModifyPropertyList.clear();
	m_mapOccuDefParam.clear();
	m_mapOccuAtkParam.clear();

	m_dwLevelPoint = _GetLongFromByteArray(pByte, pos);     //ÿһ�ȼ����ӵĵ���
	
	long lLevelNum = _GetLongFromByteArray(pByte, pos);      //��Ҿ����
	for(int i=0; i<lLevelNum; i++)
	{
		tagLevelUp st;
		_GetBufferFromByteArray(pByte, pos, &st, sizeof(tagLevelUp));
		m_vectorPlayerExp.push_back(st);
	}

	for(int i=0;i<OCC_Max;i++)
	{
		m_vcOccuLvlExp[i].clear();
		long lLevelNum = _GetLongFromByteArray(pByte, pos);      //��Ҿ����
		tagOccuLevelUp st;
		for(int j=0; j<lLevelNum; j++)
		{
			_GetBufferFromByteArray(pByte, pos, &st, sizeof(tagOccuLevelUp));
			m_vcOccuLvlExp[i].push_back(st);
		}
	}

	m_lBasePropertyValue = _GetLongFromByteArray(pByte, pos);
	m_fPropertyLvlParam = _GetFloatFromByteArray(pByte, pos);
	tagOccupModifyProperty tPoint;                            //��ȡ�ȼ�������
	lLevelNum = _GetLongFromByteArray(pByte, pos);     
	for(int i=0; i<lLevelNum; i++)
	{
		_GetBufferFromByteArray(pByte, pos, &tPoint, sizeof(tagOccupModifyProperty));
		m_mapOccuModifyPropertyList[i] = tPoint;
	}

	DWORD dwOccuLevel = 0;
	tagOccuDefParam OccuDefParam;
	long lSize = _GetLongFromByteArray(pByte, pos);
	for(int i=0;i<lSize;i++)
	{
		 dwOccuLevel = _GetDwordFromByteArray(pByte, pos);
		 _GetBufferFromByteArray(pByte, pos, &OccuDefParam, sizeof(OccuDefParam));
		 m_mapOccuDefParam[dwOccuLevel] = OccuDefParam;
	}

	tagOccuAtkParam OccuAtkParam;
	lSize = _GetLongFromByteArray(pByte, pos);
	for(int i=0;i<lSize;i++)
	{
		 dwOccuLevel = _GetDwordFromByteArray(pByte, pos);
		 _GetBufferFromByteArray(pByte, pos, &OccuAtkParam, sizeof(OccuAtkParam));
		 m_mapOccuAtkParam[dwOccuLevel] = OccuAtkParam;
	}
	return true;
}


PlayerList::tagOccupModifyProperty*	PlayerList::GetOccuModifyProperty(WORD level)         //
{
	return &m_mapOccuModifyPropertyList[level];
}


bool PlayerList::LoadOccuModifyProperty(const char* filename)            //���ְҵ�������Ա�
{
	m_mapOccuModifyPropertyList.clear ();
	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,filename);
		return false;
	}
	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	short nOccup;
	tagOccupModifyProperty tProperty;
	string strTempt;
	
	stream>>strTempt>>m_lBasePropertyValue;
	stream>>strTempt>>m_fPropertyLvlParam;
	while( ReadTo(stream,"*"))
	{
		stream>> nOccup 
		>>strTempt
		>> tProperty.fStr
		>> tProperty.fDex
		>> tProperty.fCon
		>> tProperty.fIntellect
		>> tProperty.fSpiritualism

		>> tProperty.fStrAttRatio
		>> tProperty.fAgiAttRatio
		>> tProperty.fConDefRatio

		>> tProperty.fBlockStrRatio
		>> tProperty.fBlockValueRatio

		>> tProperty.fParryStrRatio
		>> tProperty.fParryAgiRatio
		>> tProperty.fParryValueRatio

		>> tProperty.fDodgeAgiRatio
		>> tProperty.fDodgeValueRatio

		>> tProperty.fCriAgiRatio
		>> tProperty.fCriValueRatio

		>> tProperty.fHitAgiRatio
		>> tProperty.fHitValueRatio

		>> tProperty.fBaseHit
		>> tProperty.fBaseHp
		>> tProperty.fLvlUpHp
		>> tProperty.fBaseMp
		>> tProperty.fLvlUpMp

		>> tProperty.fCriDamageStrRatio
		>> tProperty.fCriDamageValueRatio
		>> tProperty.fCriDefConstiRatio
		>> tProperty.fCriDefValueRatio
		>> tProperty.fConstiDefRatio
		>> tProperty.fConstiDefValueRatio
		>> tProperty.fConstiHpRatio
		>> tProperty.fMAtkIntellRatio
		>> tProperty.fMCriDamageIntellRatio
		>> tProperty.fMCriDamageValueRatio
		>> tProperty.fWillDefSpiritRatio
		>> tProperty.fWillMpRatio
		>> tProperty.fWillDefValueRatio
		>> tProperty.fMCriDefSpiritRatio
		>> tProperty.fMCriDefValueRatio
		>> tProperty.fMCriWisdomRatio
		>> tProperty.fMCriValueRatio
		>> tProperty.fMDefSpiritRatio
		>> tProperty.fMDefValueRatio
		>> tProperty.fAbsorbWisdomRatio
		>> tProperty.fAbsorbValueRatio
		>> tProperty.fBuffPowerIntellRatio
		>> tProperty.fBuffPowerValueRatio
		>> tProperty.fMAtkSpeedWisdomRatio
		>> tProperty.fMAtkSpeedValueRatio
		>> tProperty.fMaxBaseEnergy
		>> tProperty.fBaseEnergyRecoverValue
		>> tProperty.fBaseEnergyRecoverTime
		>> tProperty.fFightEnergyRecoverValue
		>> tProperty.fFightEnergyRecoverTime
		>> tProperty.fAttackEnergyValue
		>> tProperty.fBlastEnertyValue
		>> tProperty.fBeenAttackEnergyValue
		>> tProperty.fBeenBlastEnergyValue
		>> tProperty.lBasePKCP
		>> tProperty.lLvlupAddPKCP ;

		m_mapOccuModifyPropertyList[nOccup]=tProperty;
	}
	return true;
}

// ���ļ���ȡְҵ��������
bool PlayerList::LoadOccuDefParam(const char* filename)
{
	m_mapOccuDefParam.clear ();
	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,filename);
		return false;
	}
	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	short nOccuLevel=0;
	tagOccuDefParam OccuDefParam;
	while( ReadTo(stream,"*"))
	{
		stream >> nOccuLevel;
		for(int i=0;i<OCC_Max;i++)
		{
			stream >> OccuDefParam.Def[i];
			stream >> OccuDefParam.MDef[i];
		}
		m_mapOccuDefParam[nOccuLevel] = OccuDefParam;
	}
	return true;
}

// ���ļ���ȡְҵ��������
bool PlayerList::LoadOccuAtkParam(const char* filename)
{
	m_mapOccuAtkParam.clear ();
	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,filename);
		return false;
	}
	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	short nOccuLevel=0;
	tagOccuAtkParam OccuAtkParam;
	while( ReadTo(stream,"*"))
	{
		stream >> nOccuLevel;
		for(int i=0;i<OCC_Max;i++)
		{
			stream >> OccuAtkParam.Atk[i];
			stream >> OccuAtkParam.MAtk[i];
		}
		m_mapOccuAtkParam[nOccuLevel] = OccuAtkParam;
	}
	return true;
}

float PlayerList::GetOccuLvlCoe(eOccupation byOccu,WORD wOccuLevel)
{
	float fRet = 0;
	switch(byOccu)
	{
		//������
		case OCC_Novice:
			fRet = wOccuLevel;
			break;
		//սʿ����ʦ������
		case OCC_Fighter:
		case OCC_Mystic:
		case OCC_Rogue:
			fRet = (float)(wOccuLevel+9);
			break;
		//��ʿ����ʿ�������ˡ���ʿ����ʦ
		case OCC_Knight:
		case OCC_Warrior:
		case OCC_Scout:
		case OCC_Craftsman:
		case OCC_Warlock:
		case OCC_Cleric:
			fRet = (float)(wOccuLevel+39);
			break;
		//ʥ��ʿ������ʿ����ʿ����սʿ��Ѳ�ء��̿͡������ߡ�����ʫ�ˡ���ʦ�����鷨ʦ�����̡�����
		case OCC_Paladin:
		case OCC_DarkKnight:
		case OCC_Gladiator:
		case OCC_Berserker:
		case OCC_Ranger:
		case OCC_Assassin:
		case OCC_BladeDancer:
		case OCC_Bard:
		case OCC_Sorcerer:
		case OCC_Necromancer:
		case OCC_Bishop:
		case OCC_Prophet:
			fRet = (float)(wOccuLevel+79);
			break;
	};
	return fRet;
}

void PlayerList::GetDefParam(eOccupation byOccu,WORD wLeve,float& fDef,float& fMDef)
{
	if(byOccu < 0 || byOccu >= OCC_Max)	return;

	map<DWORD,tagOccuDefParam>::iterator it = m_mapOccuDefParam.find(wLeve);
	if(it != m_mapOccuDefParam.end())
	{
		fDef = (*it).second.Def[byOccu];
		fMDef = (*it).second.MDef[byOccu];
	}
}

void PlayerList::GetAtkParam(eOccupation byOccu,WORD wLeve,float& fAtk,float& fMAtk)
{
	if(byOccu < 0 || byOccu >= OCC_Max)	return;

	map<DWORD,tagOccuAtkParam>::iterator it = m_mapOccuAtkParam.find(wLeve);
	if(it != m_mapOccuAtkParam.end())
	{
		fAtk = (*it).second.Atk[byOccu];
		fMAtk = (*it).second.MAtk[byOccu];
	}
}

void PlayerList::SetBaseFightProperty(tagPPro *pPPro,tagPFPro *pPFPro)
{
	if(NULL != pPPro && NULL!=pPFPro)
	{
		//����������Ҫ�ľ���ֵ
		if(pPPro->wLevel <= 0 || pPPro->wLevel > m_vectorPlayerExp.size())
		{
			if(pPPro->wLevel <= 0)
				pPPro->wLevel = 1;
			else if(pPPro->wLevel > m_vectorPlayerExp.size() )
				pPPro->wLevel = (ushort)(m_vectorPlayerExp.size());
			Log4c::Warn(ROOT_MODULE,"[PLAYERLIST] Set Player Property,Level Error.");
		}

		WORD lLvl = pPPro->wLevel;
		if(lLvl>m_vectorPlayerExp.size())
		{
			pPPro->dwUpgradeExp = 0xFFFFFFFF;
			pPPro->dwUpgradeExpMultiple = 0xFFFFFFFF;
		}
		else
		{
			pPPro->dwUpgradeExp = m_vectorPlayerExp[lLvl-1].dwExp;
			pPPro->dwUpgradeExpMultiple = m_vectorPlayerExp[lLvl-1].dwExpMultiple;
		}

		//����ְҵ��������Ҫ�ľ���
		//��ǰְҵ
		eOccupation	byCurOccu = pPPro->byOccu;
		if(pPPro->byOccuLvl[byCurOccu] <= 0 || pPPro->byOccuLvl[byCurOccu] > m_vcOccuLvlExp[byCurOccu].size())
		{
			if(pPPro->byOccuLvl[byCurOccu] <= 0)
				pPPro->byOccuLvl[byCurOccu] = 1;
			else if(pPPro->byOccuLvl[byCurOccu] > m_vcOccuLvlExp[byCurOccu].size() )
				pPPro->byOccuLvl[byCurOccu] = (uchar)(m_vcOccuLvlExp[byCurOccu].size());
			Log4c::Warn(ROOT_MODULE,"[PLAYERLIST] Set Player Property,Occu Level Error.");
		}
		uchar byLevel = pPPro->byOccuLvl[byCurOccu];
		pPPro->dwCurOccuUpgradeExp = m_vcOccuLvlExp[byCurOccu][byLevel-1].dwExp;

		//����ս������
		map<DWORD, tagOccupModifyProperty>::iterator it =
			m_mapOccuModifyPropertyList.find(pPPro->byOccu);
		if(it != m_mapOccuModifyPropertyList.end())
		{
			tagOccupModifyProperty &OccuProperty = (*it).second;
			//�õ�ְҵ�ȼ�ϵ��
			float fOccuLvlFactor = GetOccuLvlCoe(pPPro->byOccu,pPPro->byOccuLvl[pPPro->byOccu]);

			memset(pPFPro,0,sizeof(tagPFPro));
			//HP�ظ�������ʼֵ��Ϊ100
			pPFPro->wHpRecoverRatio = 100;
			/*����������ԣ����������ݡ����ʡ��������ǻۡ�����
				==ϵ��A+ְҵ�ȼ�ϵ��*POWER(ϵ��2��2)*�������ñ��и�ְҵ������Ե�ֵ
			*/
			//float fPowValue = pow((float)m_fPropertyLvlParam,2);
			//����
            pPFPro->wStr = (short)(fOccuLvlFactor*OccuProperty.fStr + pPPro->wPoint2Str);
			//��
			pPFPro->wDex = (short)(fOccuLvlFactor*OccuProperty.fDex + pPPro->wPoint2Dex);
			//����
			pPFPro->wCon = (short)(fOccuLvlFactor*OccuProperty.fCon + pPPro->wPoint2Con);
			//��־
			pPFPro->wIntellect = (short)(fOccuLvlFactor*OccuProperty.fIntellect + pPPro->wPoint2Intellect);
			//����
			pPFPro->wSpiritualism = (short)(fOccuLvlFactor*OccuProperty.fSpiritualism + pPPro->wPoint2Spiritualism);
            // ʣ������
            pPFPro->wPoint = max(0, GetOccuLvlPoint(pPPro->byOccu,pPPro->byOccuLvl[pPPro->byOccu]) - 
                (pPPro->wPoint2Str + pPPro->wPoint2Dex + pPPro->wPoint2Con + pPPro->wPoint2Intellect + pPPro->wPoint2Spiritualism) );
		}
		else
		{
			Log4c::Warn(ROOT_MODULE,"[PLAYERLIST] Not found the configure attribute of %d occupation",pPPro->byOccu);//δ�ҵ�%dְҵ����������!
		}
	}
}


void PlayerList::SetFightProperty(tagPPro *pPPro,tagPFPro *pBasePFPro,tagPFPro *pExPFPro)
{
	if(NULL != pPPro && NULL != pBasePFPro && NULL!=pExPFPro)
	{
		//�õ�ְҵ�ȼ�ϵ��
		float fOccuLvlFactor = GetOccuLvlCoe(pPPro->byOccu,pPPro->byOccuLvl[pPPro->byOccu]);
		//�õ����������ħ����������ֵ
		float fDefParam = 0.0f;
		float fMDefParam = 0.0f;
		GetDefParam(pPPro->byOccu,(WORD)fOccuLvlFactor,fDefParam,fMDefParam);
		float fAtkParam = 0.0f;
		float fMAtkParam = 0.0f;
		GetAtkParam(pPPro->byOccu,(WORD)fOccuLvlFactor,fAtkParam,fMAtkParam);
		//����ս������
		map<DWORD, tagOccupModifyProperty>::iterator it =
			m_mapOccuModifyPropertyList.find(pPPro->byOccu);
		if(it != m_mapOccuModifyPropertyList.end())
		{
			tagOccupModifyProperty &OccuProperty = (*it).second;			
			//������
		//	long lTotalStrenth = pBasePFPro->wStrenth+pExPFPro->wStrenth;
            short lTotalStrenth = pBasePFPro->wStr + pExPFPro->wStr;
			if(lTotalStrenth < 0)	lTotalStrenth = 0;
			//������
		//	long lTotalAgility = pBasePFPro->wAgility+pExPFPro->wAgility;
            long lTotalAgility = pBasePFPro->wDex + pExPFPro->wDex;
			if(lTotalAgility < 0)	lTotalAgility = 0;
			//������
		//	long lTotalConsti = pBasePFPro->wConsti+pExPFPro->wConsti;
            long lTotalConsti = pBasePFPro->wCon + pExPFPro->wCon;
			if(lTotalConsti<0) lTotalConsti=0;
			//������
			long lTotalIntell = pBasePFPro->wIntellect + pExPFPro->wIntellect;
			if(lTotalIntell < 0)	lTotalIntell = 0;
			////���ǻ�
			//long lTotalWisdom = pBasePFPro->wWisdom+pExPFPro->wWisdom;
			//if(lTotalWisdom < 0)	lTotalWisdom = 0;
            long lTotalWisdom = 0;
			//�ܾ���
			long lTotalSpiri = pBasePFPro->wSpiritualism + pExPFPro->wSpiritualism;
			if(lTotalSpiri < 0) lTotalSpiri = 0;
			//��
			pBasePFPro->wFightBlock = (short)((lTotalStrenth)/OccuProperty.fBlockStrRatio);
			//�м�
			pBasePFPro->wParry = (short)((lTotalStrenth)/OccuProperty.fParryStrRatio +
							(lTotalAgility)/OccuProperty.fParryAgiRatio);
			//����
			pBasePFPro->wDodge = (short)((lTotalAgility)/OccuProperty.fDodgeAgiRatio);
			//����������
			pBasePFPro->wMinAtk = pBasePFPro->wMaxAtk = (short)((lTotalStrenth)*OccuProperty.fStrAttRatio+
												(lTotalAgility)*OccuProperty.fAgiAttRatio + fAtkParam);
			//�������
			pBasePFPro->wDef=(short)((lTotalConsti)*OccuProperty.fConDefRatio+fDefParam);
			//������ֵ
			pBasePFPro->wCri = (short)((lTotalAgility)/OccuProperty.fCriAgiRatio);
			//����ֵ
			pBasePFPro->wHit = (short)((lTotalAgility)/OccuProperty.fHitAgiRatio);
			//רעֵ
			pBasePFPro->wAbsorb = (short)((lTotalWisdom)/OccuProperty.fAbsorbWisdomRatio);
			//�ﱬ�˺�ֵ
			pBasePFPro->wCriDamage = (short)((lTotalStrenth)/OccuProperty.fCriDamageStrRatio);
			//�ﱬ����ֵ
			pBasePFPro->wCriDef = (short)((lTotalConsti)/OccuProperty.fCriDefConstiRatio);
			//���ʿ���ֵ
			pBasePFPro->wConstiDef = (short)((lTotalConsti)/OccuProperty.fConstiDefRatio);
			//ħ������
			pBasePFPro->wMAtk = (long)((lTotalIntell)*OccuProperty.fMAtkIntellRatio+fMAtkParam);
			//ħ�������˺�
			pBasePFPro->wMCriDamage = (short)((lTotalIntell)/OccuProperty.fMCriDamageIntellRatio);
			//��־����ֵ
			pBasePFPro->wWillDef = (short)((lTotalSpiri)/OccuProperty.fWillDefSpiritRatio);
			//ħ����������ֵ
			pBasePFPro->wMCriDef = (short)((lTotalSpiri)/OccuProperty.fMCriDefSpiritRatio);
			//ħ������ֵ
			pBasePFPro->wMdef = (short)((lTotalSpiri)*OccuProperty.fMDefSpiritRatio+fMDefParam);
			//ħ������ֵ
			pBasePFPro->wMCri = (long)((lTotalWisdom)/OccuProperty.fMCriWisdomRatio);
			//״̬����ǿ��
			pBasePFPro->wBuffPower = (short)((lTotalIntell)/OccuProperty.fBuffPowerIntellRatio);
			//ʩ���ٶ�ֵ
			pBasePFPro->wMAtkSpeed = (short)((lTotalWisdom)/OccuProperty.fMAtkSpeedWisdomRatio);
			//���������ͷ���
			//����
			//float fPowValue = pow(fOccuLvlFactor/5,(float)1.5);
			short shLvlDir = (short)(pPPro->wLevel-fOccuLvlFactor);
			if(shLvlDir < 0) shLvlDir = 0;
			//����ֵ
			pBasePFPro->dwMaxHp = (long)((OccuProperty.fBaseHp+(pPPro->wLevel)*OccuProperty.fLvlUpHp+lTotalConsti*OccuProperty.fConstiHpRatio )*0.34f);
			
			long lTotalMaxHp = pBasePFPro->dwMaxHp+pExPFPro->dwMaxHp;
			if(lTotalMaxHp <= 1)	lTotalMaxHp = 1;

			if(pPPro->dwHp == 0 || pPPro->dwHp > (ulong)lTotalMaxHp )
				pPPro->dwHp = lTotalMaxHp;
			//����ֵ
			pBasePFPro->wMaxMp = (short)(OccuProperty.fBaseMp+(pPPro->wLevel)*OccuProperty.fLvlUpMp+lTotalSpiri*OccuProperty.fWillMpRatio);
			
			long lTotalMaxMp = pBasePFPro->wMaxMp+pExPFPro->wMaxMp;
			if(lTotalMaxMp <= 1)	lTotalMaxMp = 1;

			if(pPPro->wMp == 0 || pPPro->wMp > lTotalMaxMp )
				pPPro->wMp = (ushort)lTotalMaxMp;
			//Hp�ָ��ٶ�ֵ	ÿ5�������ָ�=��ǰ����*��1-ְҵ�ȼ�ϵ��/220��
			float fTemp = pow((float)(1.0/pPPro->wLevel),(float)0.1);
			pBasePFPro->wHpRecoverSpeed = (short)(((((lTotalConsti*(1-pPPro->wLevel/220) + lTotalConsti)*(3-pPPro->wLevel/200))/5)*2)*fTemp);
			//Mp�ָ��ٶ�ֵ
			pBasePFPro->wMpRecoverSpeed = (short)(((((lTotalSpiri*(1-pPPro->wLevel/220) + lTotalSpiri)*(3-pPPro->wLevel/200))/5)*2)*fTemp);
			//����ֵ
			pBasePFPro->wMaxEnergy = (short)OccuProperty.fMaxBaseEnergy;				

			pBasePFPro->wEnerRecoverSpeed = (short)OccuProperty.fBaseEnergyRecoverValue;
			pBasePFPro->wFightEnerRecoverSpeed = (short)OccuProperty.fFightEnergyRecoverValue;

			// PK CPֵ
			pBasePFPro->lMaxPKCP = OccuProperty.lBasePKCP + pPPro->wLevel * OccuProperty.lLvlupAddPKCP;
			long lTotalPKCP = pBasePFPro->lMaxPKCP + pExPFPro->lMaxPKCP;
			if(  pPPro->lCurPKCP > lTotalPKCP )
			{
				pPPro->lCurPKCP = lTotalPKCP;
			}
		}
	}
}

double PlayerList::GetOccuValueRatio(const char* pszName,int nOccu)
{
	double dbRet = 0;
	if(NULL == pszName )	return dbRet;
	map<DWORD, tagOccupModifyProperty>::iterator it =
		m_mapOccuModifyPropertyList.find(nOccu);
	if(it != m_mapOccuModifyPropertyList.end())
	{
		tagOccupModifyProperty &OccuProperty = (*it).second;
		if(0 == strcmp(pszName,"block"))
			dbRet = OccuProperty.fBlockValueRatio;
		else if(0 == strcmp(pszName,"parry"))
			dbRet = OccuProperty.fParryValueRatio;
		else if(0 == strcmp(pszName,"dodge"))
			dbRet = OccuProperty.fDodgeValueRatio;
		else if(0 == strcmp(pszName,"cri"))
			dbRet = OccuProperty.fCriValueRatio;
		else if(0 == strcmp(pszName,"hit"))
			dbRet = OccuProperty.fHitValueRatio;
		else if(0 == strcmp(pszName,"basehit"))
			dbRet = OccuProperty.fBaseHit;
		else if(0 == strcmp(pszName,"cridamage"))
			dbRet = OccuProperty.fCriDamageValueRatio;
		else if(0 == strcmp(pszName,"cridef"))
			dbRet = OccuProperty.fCriDefValueRatio;
		else if(0 == strcmp(pszName,"condef"))
			dbRet = OccuProperty.fConstiDefValueRatio;
		else if(0 == strcmp(pszName,"mcridamage"))
			dbRet = OccuProperty.fMCriDamageValueRatio;
		else if(0 == strcmp(pszName,"willdef"))
			dbRet = OccuProperty.fWillDefValueRatio;
		else if(0 == strcmp(pszName,"mcridef"))
			dbRet = OccuProperty.fMCriDefValueRatio;
		else if(0 == strcmp(pszName,"mcri"))
			dbRet = OccuProperty.fMCriValueRatio;
		else if(0 == strcmp(pszName,"mdef"))
			dbRet = OccuProperty.fMDefValueRatio;
		else if(0 == strcmp(pszName,"absorb"))
			dbRet = OccuProperty.fAbsorbValueRatio;
		else if(0 == strcmp(pszName,"buffatkpower"))
			dbRet = OccuProperty.fBuffPowerValueRatio;
		else if(0 == strcmp(pszName,"matkspeed"))
			dbRet = OccuProperty.fMAtkSpeedValueRatio;
		else if(0 == strcmp(pszName,"baseenergyrecovertime"))
			dbRet = OccuProperty.fBaseEnergyRecoverTime;
		else if (0 == strcmp(pszName,"fightenergyrecovertime"))
			dbRet = OccuProperty.fFightEnergyRecoverTime;
		else if(0 == strcmp(pszName,"attackenergyvalue"))
			dbRet = OccuProperty.fAttackEnergyValue;
		else if(0 == strcmp(pszName,"blastenertyvalue"))
			dbRet = OccuProperty.fBlastEnertyValue;
		else if(0 == strcmp(pszName,"beenattackenergyvalue"))
			dbRet = OccuProperty.fBeenAttackEnergyValue;
		else if (0 == strcmp(pszName,"beenblastenergyvalue"))
			dbRet = OccuProperty.fBeenBlastEnergyValue;
		else if( 0 == strcmp( pszName, "basepkcp" ) )
			dbRet = OccuProperty.lBasePKCP;
		else if( 0 == strcmp( pszName, "lvlupaddpkcp" ) )
			dbRet = OccuProperty.lLvlupAddPKCP;

		
		
	}
	return dbRet;
}

float PlayerList::GetBaseCP(long lLevel)
{
	return ((long)(m_vectorPlayerExp.size())>=lLevel && lLevel>0)?(long)(m_vectorPlayerExp[lLevel - 1].lBaseCP):0.0f;
}

WORD PlayerList::GetOccuLvlPoint(eOccupation byOccu, WORD wOccuLevel)
{
    // Ԥ����ʽ
    return 0;
}