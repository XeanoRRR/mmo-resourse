#include "stdafx.h"
#include "BusinessSystem.h"
#include "../../../Public/Common/CreditSystem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef map<WORD/*�����ҵ�ID*/,DWORD/*����ֵ*/>::iterator MapCountryCreditIter;
typedef map<WORD/*�����ҵ�ID*/,DWORD/*Ͷ���ܶ�*/>::iterator MapCountryInvestIter;
typedef map<DWORD,CBusinessSystem::tagTradeGoodsInfor>::iterator MapTradeGoodsInforIter;
typedef map<WORD,CBusinessSystem::tagBusinessZone>::iterator MapZoneIter;
typedef map<WORD,CBusinessSystem::tagBusinessPoint>::iterator MapPointIter;
typedef map<WORD,CBusinessSystem::tagBusinessZone>* MapZonePtr;
typedef map<WORD,CBusinessSystem::tagBusinessPoint>* MapPointPtr;
typedef vector<CBusinessSystem::tagCreditLevel>::iterator VecCreditLevelIter;
typedef map<int,DWORD>::iterator MapAreaIter;

map<WORD/*��ó������ID*/,CBusinessSystem::tagBusinessZone>  CBusinessSystem::m_mapBusinessZones;			//��ó��������
map<WORD/*��ó�׵��ID*/,CBusinessSystem::tagBusinessPoint> CBusinessSystem::m_mapBusinessPoint;			//��ó�׵�����
map<DWORD,CBusinessSystem::tagTradeGoodsInfor>				CBusinessSystem::m_mapTradeGoods;				//��ǰ������Ʒ��ĳ��NPC�ļ۸��
CBusinessSystem*											CBusinessSystem::m_pBusinessSystem = NULL;
CBusinessSystem::tagInvestInfor								CBusinessSystem::m_tagInvest;					//�����ĳ��ó�׵��һ��Ͷ����Ϣ
vector<CBusinessSystem::tagCreditLevel>						CBusinessSystem::m_vecCreditLevel;				//�����ȼ��б�
map<int,CBusinessSystem::tagAreaCredit>                     CBusinessSystem::m_mapCreditArea;
long                                                        CBusinessSystem::m_lAreaNumSize = 0;             //�����е���������
CCreditSystem*                                              CBusinessSystem::m_pCreditSystem = NULL;

CBusinessSystem::CBusinessSystem()
{
	//m_mapBusinessPoint.clear();
	//m_mapBusinessPoint.clear();
	//m_vecTradeGoods.clear();
}

CBusinessSystem::tagBusinessZone* CBusinessSystem::GetZoneInfor(WORD wZoneID)
{
	MapZoneIter iter = m_mapBusinessZones.find(wZoneID);
	if (iter!=m_mapBusinessZones.end())
	{
		return &(iter->second);
	}
	return NULL;
}

CBusinessSystem::tagBusinessPoint* CBusinessSystem::GetPointInfor(WORD wPointID)
{
	MapPointIter iter = m_mapBusinessPoint.find(wPointID);
	if (iter!=m_mapBusinessPoint.end())
	{
		return &(iter->second);
	}
	return NULL;
}

bool CBusinessSystem::CreateBusinessSystem()
{
	m_pBusinessSystem = new CBusinessSystem();	
	if (m_pBusinessSystem!=NULL)
	{
		return true;
	}
	return false;
}

//bool CBusinessSystem::InitBusinessSystem()
//{
//	
//}

void CBusinessSystem::Release()
{
	SAFE_DELETE(m_pBusinessSystem);
}

bool CBusinessSystem::ResetSystem()
{
	return false;
}

void CBusinessSystem::UpdateZoneData(WORD wZoneID)
{

}

void CBusinessSystem::UpdatePointData(WORD wPointID)
{
	
}

bool CBusinessSystem::InitCreditLevel(const char* filename)
{
	m_vecCreditLevel.clear();

	tagCreditLevel stCredit;

	stringstream stream;	
	CRFile* prfile = rfOpen(filename);
	if( !prfile )
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", filename);
		MessageBox(g_hWnd, str, "error", MB_OK);
		return FALSE;
	}
	prfile->ReadToStream(stream);
	rfClose(prfile);


	while(ReadTo(stream, "#"))
	{
		stream >> stCredit.lMinNum >> stCredit.lMaxNum >> stCredit.LevelName;
		m_vecCreditLevel.push_back(stCredit);
	}

	return TRUE;
}

//���ݴ��������ֵ��ȡ�����ȼ�����
char* CBusinessSystem::GetCreditLevelName(long lCurCreditValue)
{
	VecCreditLevelIter iter = m_vecCreditLevel.begin();
	for (;iter!=m_vecCreditLevel.end();iter++)
	{
		if (lCurCreditValue>=iter->lMinNum&&lCurCreditValue<=iter->lMaxNum)
		{
			return iter->LevelName;
		}
	}
	return NULL;
}

//���ݴ��������ֵ��ȡ��ǰ�ȼ����������ֵ
long CBusinessSystem::GetMaxCreditValue(long lCurCreditValue)
{
	VecCreditLevelIter iter = m_vecCreditLevel.begin();
	for (;iter!=m_vecCreditLevel.end();iter++)
	{
		if (lCurCreditValue>=iter->lMinNum&&lCurCreditValue<=iter->lMaxNum)
		{
			return iter->lMaxNum;
		}
	}
	return -1;
}

//���ݴ��������ֵ��ȡ��ǰ�ȼ����������ֵ
long CBusinessSystem::GetCurCreditLevel(long lCurCreditValue)
{
	VecCreditLevelIter iter = m_vecCreditLevel.begin();
	long lLevel=0;
	for (;iter!=m_vecCreditLevel.end();iter++)
	{
		if (lCurCreditValue>=iter->lMinNum&&lCurCreditValue<=iter->lMaxNum)
		{
			return lLevel;
		}
		lLevel++;
	}
	return -1;
}


//���ݴ��������id��������������ʾ��
const char* CBusinessSystem::GetAreaName(int id)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->CreditAreaNameByIndex(id);
	}
    return NULL;
}


bool  CBusinessSystem::InitCreditSystem()
{
	m_pCreditSystem = new CCreditSystem();
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->LoadAreaCredit();
	}
	return FALSE;
}

//���ݴ������֯ID �Ͷ�Ӧ�������ȼ����ص�ǰ�ȼ���������ֵ����
long  CBusinessSystem::GetOrgeCreditMaxValue(DWORD dwOrganID,DWORD dwValue)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->MaxOrganValue(dwOrganID,dwValue);
	}
	return 0;
}

//��ȡָ����֯��ָ������ֵ ����Ӧ������
const char* CBusinessSystem::CreditOrganName(DWORD dwOrganID,DWORD dwValue)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->CreditOrganName(dwOrganID,dwValue);
	}
	return NULL;
}

//���ݴ������֯id������ֵ�õ���Ӧ�������ȼ�
long CBusinessSystem::CreditOrganLevel(DWORD dwOrganID,DWORD dwValue)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->CreditOrganLevel(dwOrganID,dwValue);
	}
	return  0;
}

//��ȡָ���ȼ���������������
long CBusinessSystem::MaxAreaValue(DWORD dwValue)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->MaxAreaValue(dwValue);
	}
	return 0;
}

//��ȡָ��ֵ��ȡ�����ĵȼ�
long CBusinessSystem::AreaLevel(DWORD dwValue)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->CreditLevel(dwValue);
	}
	return 0;
}

//��������ֵ��ȡ��ǰ��������ʾ��
const char* CBusinessSystem::AreaCreditName(DWORD dwValue)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->CreditName(dwValue);
	}
	return NULL;
}

//�ͷ�����ϵͳ��Դ
void CBusinessSystem::ReleaseCreditSystem()
{
	if(m_pCreditSystem)
	{
	   SAFE_DELETE(m_pCreditSystem);
	}
}

//����������������
size_t CBusinessSystem::GetAreaNumSize()
{
	if(m_pCreditSystem)
	{
		return (int)m_pCreditSystem->GetCreditAreaSize();
	}
	return 0;
}

//���������֯������
size_t CBusinessSystem::GetOrganNumSize()
{
	if(m_pCreditSystem)
	{
		return (int)m_pCreditSystem->GetCreditOrganSize();
	}
	return 0;
}

//�����֯���������ֵ
long CBusinessSystem::GetMaxCreditByOrgan(long ID)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->GetMaxCreditByOrgan(ID);
	}
	return 0;
}

//����������������ֵ
long CBusinessSystem::MaxAreaCredit()
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->MaxAreaCredit();
	}
	return 0;
}

long CBusinessSystem::GetMaxCreditLevelByOrgan(long id)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->GetMaxCreditLevelByOrgan(id);
	}
	return 0;
}

long CBusinessSystem::MaxAreaCreditLevel()
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->MaxAreaCreditLevel();
	}
	return 0;
}

//������֯������ID��ȡ������������
const char* CBusinessSystem::GetOrganName(DWORD dwOrganID)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->GetOrganName(dwOrganID);
	}
	return NULL;
}

//������֯������ID��ȡ������������
const char* CBusinessSystem::GetDirtyName(DWORD dwOrganID)
{
	if(m_pCreditSystem)
	{
		return m_pCreditSystem->GetDirtyName(dwOrganID);
	}
	return NULL;
}
