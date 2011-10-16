#include "StdAfx.h"
#include ".\playerlist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;

//string CPlayerList::s_strOccupation[CPlayerList::OC_NUMS] = {"��ʿ", "����", "����"};
string CPlayerList::s_strOccupation[OCC_Max] = 
{"������", "սʿ", "��ʦ","����","��ʿ","��ʿ","���","����","��ʿ","��ʦ","ʥ��ʿ","����ʿ",
"�Ƕ�ʿ","��սʿ","Ѳ��","�̿�","������","����ʫ��","��ʦ","������ʦ","����","����"};
string CPlayerList::s_strSex[CPlayerList::SEX_NUMS] = {"��", "Ů"};


vector<CPlayerList::tagLevelUp>					CPlayerList::m_vectorPlayerExp;		// �����������ֵ
vector<CPlayerList::tagOccuLevelUp>				CPlayerList::m_vcOccuLvlExp[OCC_Max];//ÿ��ְҵ�ȼ������б�
DWORD											CPlayerList::m_dwLevelPoint = 0;	// ÿ��һ������ҷ���ĵ���
list<CPlayerList::tagOrginEquip>				CPlayerList::m_listOrginEquip;		// ��ʼװ���б�

long CPlayerList::m_lBasePropertyValue;//��������ֵ
float CPlayerList::m_fPropertyLvlParam;//���Եȼ�ϵ��
map<DWORD, CPlayerList::tagOccupModifyProperty>	CPlayerList::m_mapOccuModifyPropertyList;   //ְҵ������
map<DWORD,CPlayerList::tagOccuDefParam>	CPlayerList::m_mapOccuDefParam;//��ְҵ�ķ�������ֵ
map<DWORD,CPlayerList::tagOccuAtkParam>	CPlayerList::m_mapOccuAtkParam;//��ְҵ�Ĺ�������ֵ

CPlayerList::CPlayerList(void)
{
}

CPlayerList::~CPlayerList(void)
{
	m_dwLevelPoint = 0;
	//	m_mapPlayerList.clear();
	m_vectorPlayerExp.clear();
	m_listOrginEquip.clear();
	m_mapOccuModifyPropertyList.clear();
}


// ���ļ���ȡexp�б�
bool CPlayerList::LoadPlayerExpList(const char* filename)
{
	m_vectorPlayerExp.clear();

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
bool CPlayerList::LoadOccuLvlExpList()
{
	char pszFileName[256]="";
	for(int i=0;i<OCC_Max;i++)
	{
		m_vcOccuLvlExp[i].clear();
		_snprintf(pszFileName,256,"data/LevelExp/Occu%02dExp.ini",i);
		CRFile* prfile = rfOpen(pszFileName);
		if(prfile == NULL)
		{
			char str[256];
			_snprintf(str, 256, "file '%s' can't found!", pszFileName);
			MessageBox(g_hWnd, str, "error", MB_OK);
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
				char str[256];
				_snprintf(str, 256, "file '%s' config error!", pszFileName);
				MessageBox(g_hWnd, str, "error", MB_OK);
				return false;
			}
		}
	}
	return true;
}

// ���浽�ļ�exp�б�
bool CPlayerList::SavePlayerExpList(const char* filename)
{
	return true;
}

bool CPlayerList::AddToByteArray(vector<BYTE>* pByteArray)
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
	map<DWORD, CPlayerList::tagOccupModifyProperty>::iterator itUpgrade = m_mapOccuModifyPropertyList.begin();
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

bool CPlayerList::DecordFromByteArray(BYTE* pByte, long& pos)
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


CPlayerList::tagOccupModifyProperty*	CPlayerList::GetOccuModifyProperty(WORD level)         //
{
	return &m_mapOccuModifyPropertyList[level];
}


BOOL CPlayerList::LoadOccuModifyProperty(const char* filename)            //���ְҵ�������Ա�
{
	m_mapOccuModifyPropertyList.clear ();
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

	short nOccup;
	tagOccupModifyProperty tProperty;
	string strTempt;
	
	stream>>strTempt>>m_lBasePropertyValue;
	stream>>strTempt>>m_fPropertyLvlParam;
	while( ReadTo(stream,"*"))
	{
		stream>> nOccup 
		>>strTempt
		>> tProperty.fStrenth
		>> tProperty.fAgility
		>> tProperty.fConsti
		>> tProperty.fIntell
		>> tProperty.fWisdom
		>> tProperty.fSpirit

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
bool CPlayerList::LoadOccuDefParam(const char* filename)
{
	m_mapOccuDefParam.clear ();
	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		char str[256];
		_snprintf(str, 256, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
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
bool CPlayerList::LoadOccuAtkParam(const char* filename)
{
	m_mapOccuAtkParam.clear ();
	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		char str[256];
		_snprintf(str, 256, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
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

BOOL CPlayerList::SaveOccuModifyProperty(const char* filename)
{
	return false;
}

float CPlayerList::GetOccuLvlCoe(eOccupation byOccu,WORD wOccuLevel)
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
			fRet = wOccuLevel+9;
			break;
		//��ʿ����ʿ�������ˡ���ʿ����ʦ
		case OCC_Knight:
		case OCC_Warrior:
		case OCC_Scout:
		case OCC_Craftsman:
		case OCC_Warlock:
		case OCC_Cleric:
			fRet = wOccuLevel+39;
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
			fRet = wOccuLevel+79;
			break;
	};
	return fRet;
}

void CPlayerList::GetDefParam(eOccupation byOccu,WORD wLeve,float& fDef,float& fMDef)
{
	if(byOccu < 0 || byOccu >= OCC_Max)	return;

	map<DWORD,tagOccuDefParam>::iterator it = m_mapOccuDefParam.find(wLeve);
	if(it != m_mapOccuDefParam.end())
	{
		fDef = (*it).second.Def[byOccu];
		fMDef = (*it).second.MDef[byOccu];
	}
	else
	{
		/*char str[500]="";
		_snprintf(str,500,"Don't find the defence modified property of level(%d) Occu(%d)!",wLeve,byOccu);
		PutDebugString(str);*/
	}
}

void CPlayerList::GetAtkParam(eOccupation byOccu,WORD wLeve,float& fAtk,float& fMAtk)
{
	if(byOccu < 0 || byOccu >= OCC_Max)	return;

	map<DWORD,tagOccuAtkParam>::iterator it = m_mapOccuAtkParam.find(wLeve);
	if(it != m_mapOccuAtkParam.end())
	{
		fAtk = (*it).second.Atk[byOccu];
		fMAtk = (*it).second.MAtk[byOccu];
	}
	else
	{
		/*char str[500]="";
		_snprintf(str,500,"Don't find the attck modified property of level(%d) Occu(%d)!",wLeve,byOccu);
		PutDebugString(str);*/
	}
}
void CPlayerList::SetBaseFightProperty(tagPPro *pPPro,tagPFPro *pPFPro)
{
	if(NULL != pPPro && NULL!=pPFPro)
	{
		//����������Ҫ�ľ���ֵ
		if(pPPro->wLevel <= 0 || pPPro->wLevel > m_vectorPlayerExp.size())
		{
			if(pPPro->wLevel <= 0)
				pPPro->wLevel = 1;
			else if(pPPro->wLevel > m_vectorPlayerExp.size() )
				pPPro->wLevel = m_vectorPlayerExp.size();
			PutDebugString("Set Player Property,Level Error.");
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
				pPPro->byOccuLvl[byCurOccu] = m_vcOccuLvlExp[byCurOccu].size();
			PutDebugString("Set Player Property,Occu Level Error.");
		}
		BYTE byLevel = pPPro->byOccuLvl[byCurOccu];
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
			pPFPro->wStrenth = fOccuLvlFactor*OccuProperty.fStrenth+0.5f;
			//����
			pPFPro->wAgility = fOccuLvlFactor*OccuProperty.fAgility+0.5f;
			//����
			pPFPro->wConsti = fOccuLvlFactor*OccuProperty.fConsti+0.5f;
			//����
			pPFPro->wIntell = fOccuLvlFactor*OccuProperty.fIntell+0.5f;
			//�ǻ�
			pPFPro->wWisdom = fOccuLvlFactor*OccuProperty.fWisdom+0.5f;
			//����
			pPFPro->wSpirit = fOccuLvlFactor*OccuProperty.fSpirit+0.5f;
		}
		else
		{
			char str[500]="";
			_snprintf(str, 500, "δ�ҵ�%dְҵ����������!",pPPro->byOccu);
			PutDebugString(str);
		}
	}
}


void CPlayerList::SetFightProperty(tagPPro *pPPro,tagPFPro *pBasePFPro,tagPFPro *pExPFPro)
{
	if(NULL != pPPro && NULL != pBasePFPro && NULL!=pExPFPro)
	{
		//�õ�ְҵ�ȼ�ϵ��
		float fOccuLvlFactor = GetOccuLvlCoe(pPPro->byOccu,pPPro->byOccuLvl[pPPro->byOccu]);
		//�õ����������ħ����������ֵ
		float fDefParam = 0.0f;
		float fMDefParam = 0.0f;
		GetDefParam(pPPro->byOccu,fOccuLvlFactor,fDefParam,fMDefParam);
		float fAtkParam = 0.0f;
		float fMAtkParam = 0.0f;
		GetAtkParam(pPPro->byOccu,fOccuLvlFactor,fAtkParam,fMAtkParam);
		//����ս������
		map<DWORD, tagOccupModifyProperty>::iterator it =
			m_mapOccuModifyPropertyList.find(pPPro->byOccu);
		if(it != m_mapOccuModifyPropertyList.end())
		{
			tagOccupModifyProperty &OccuProperty = (*it).second;			
			//������
			long lTotalStrenth = pBasePFPro->wStrenth+pExPFPro->wStrenth;
			if(lTotalStrenth < 0)	lTotalStrenth = 0;
			//������
			long lTotalAgility = pBasePFPro->wAgility+pExPFPro->wAgility;
			if(lTotalAgility < 0)	lTotalAgility = 0;
			//������
			long lTotalConsti = pBasePFPro->wConsti+pExPFPro->wConsti;
			if(lTotalConsti<0) lTotalConsti=0;
			//������
			long lTotalIntell = pBasePFPro->wIntell+pExPFPro->wIntell;
			if(lTotalIntell < 0)	lTotalIntell = 0;
			//���ǻ�
			long lTotalWisdom = pBasePFPro->wWisdom+pExPFPro->wWisdom;
			if(lTotalWisdom < 0)	lTotalWisdom = 0;
			//�ܾ���
			long lTotalSpiri = pBasePFPro->wSpirit+pExPFPro->wSpirit;
			if(lTotalSpiri < 0) lTotalSpiri = 0;
			//��
			pBasePFPro->wFightBlock = (double)(lTotalStrenth)/OccuProperty.fBlockStrRatio;
			//�м�
			pBasePFPro->wParry = (double)(lTotalStrenth)/OccuProperty.fParryStrRatio +
							(double)(lTotalAgility)/OccuProperty.fParryAgiRatio;
			//����
			pBasePFPro->wDodge = (double)(lTotalAgility)/OccuProperty.fDodgeAgiRatio;
			//����������
			pBasePFPro->wMinAtk = pBasePFPro->wMaxAtk = (double)(lTotalStrenth)*OccuProperty.fStrAttRatio+
												(double)(lTotalAgility)*OccuProperty.fAgiAttRatio + fAtkParam;
			//�������
			pBasePFPro->wDef=(double)(lTotalConsti)*OccuProperty.fConDefRatio+fDefParam;
			//������ֵ
			pBasePFPro->wCri = (double)(lTotalAgility)/OccuProperty.fCriAgiRatio;
			//����ֵ
			pBasePFPro->wHit = (double)(lTotalAgility)/OccuProperty.fHitAgiRatio;
			//רעֵ
			pBasePFPro->wAbsorb = (double)(lTotalWisdom)/OccuProperty.fAbsorbWisdomRatio;
			//�ﱬ�˺�ֵ
			pBasePFPro->wCriDamage = (double)(lTotalStrenth)/OccuProperty.fCriDamageStrRatio;
			//�ﱬ����ֵ
			pBasePFPro->wCriDef = (double)(lTotalConsti)/OccuProperty.fCriDefConstiRatio;
			//���ʿ���ֵ
			pBasePFPro->wConstiDef = (double)(lTotalConsti)/OccuProperty.fConstiDefRatio;
			//ħ������
			pBasePFPro->wMAtk = (double)(lTotalIntell)*OccuProperty.fMAtkIntellRatio+fMAtkParam;
			//ħ�������˺�
			pBasePFPro->wMCriDamage = (double)(lTotalIntell)/OccuProperty.fMCriDamageIntellRatio;
			//��־����ֵ
			pBasePFPro->wWillDef = (double)(lTotalSpiri)/OccuProperty.fWillDefSpiritRatio;
			//ħ����������ֵ
			pBasePFPro->wMCriDef = (double)(lTotalSpiri)/OccuProperty.fMCriDefSpiritRatio;
			//ħ������ֵ
			pBasePFPro->wMdef = (double)(lTotalSpiri)*OccuProperty.fMDefSpiritRatio+fMDefParam;
			//ħ������ֵ
			pBasePFPro->wMCri = (double)(lTotalWisdom)/OccuProperty.fMCriWisdomRatio;
			//״̬����ǿ��
			pBasePFPro->wBuffPower = (double)(lTotalIntell)/OccuProperty.fBuffPowerIntellRatio;
			//ʩ���ٶ�ֵ
			pBasePFPro->wMAtkSpeed = (double)(lTotalWisdom)/OccuProperty.fMAtkSpeedWisdomRatio;
			//���������ͷ���
			//����
			//float fPowValue = pow(fOccuLvlFactor/5,(float)1.5);
			short shLvlDir = pPPro->wLevel-fOccuLvlFactor;
			if(shLvlDir < 0) shLvlDir = 0;
			//����ֵ
			pBasePFPro->dwMaxHp = ( OccuProperty.fBaseHp+(pPPro->wLevel)*OccuProperty.fLvlUpHp+lTotalConsti*OccuProperty.fConstiHpRatio )*0.34f;
			
			long lTotalMaxHp = pBasePFPro->dwMaxHp+pExPFPro->dwMaxHp;
			if(lTotalMaxHp <= 1)	lTotalMaxHp = 1;

			if(pPPro->dwHp == 0 || pPPro->dwHp > lTotalMaxHp )
				pPPro->dwHp = lTotalMaxHp;
			//����ֵ
			pBasePFPro->wMaxMp = OccuProperty.fBaseMp+(pPPro->wLevel)*OccuProperty.fLvlUpMp+lTotalSpiri*OccuProperty.fWillMpRatio;
			
			long lTotalMaxMp = pBasePFPro->wMaxMp+pExPFPro->wMaxMp;
			if(lTotalMaxMp <= 1)	lTotalMaxMp = 1;

			if(pPPro->wMp == 0 || pPPro->wMp > lTotalMaxMp )
				pPPro->wMp = lTotalMaxMp;
			//Hp�ָ��ٶ�ֵ	ÿ5�������ָ�=��ǰ����*��1-ְҵ�ȼ�ϵ��/220��
			float fTemp = pow((float)(1.0/pPPro->wLevel),(float)0.1);
			pBasePFPro->wHpRecoverSpeed = (double)((((lTotalConsti*(1-pPPro->wLevel/220) + lTotalConsti)*(3-pPPro->wLevel/200))/5)*2)*fTemp;
			//Mp�ָ��ٶ�ֵ
			pBasePFPro->wMpRecoverSpeed = (double)((((lTotalSpiri*(1-pPPro->wLevel/220) + lTotalSpiri)*(3-pPPro->wLevel/200))/5)*2)*fTemp;
			//����ֵ
			pBasePFPro->wMaxEnergy = OccuProperty.fMaxBaseEnergy;				

			pBasePFPro->wEnerRecoverSpeed = OccuProperty.fBaseEnergyRecoverValue;
			pBasePFPro->wFightEnerRecoverSpeed = OccuProperty.fFightEnergyRecoverValue;

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

double CPlayerList::GetOccuValueRatio(const char* pszName,int nOccu)
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

