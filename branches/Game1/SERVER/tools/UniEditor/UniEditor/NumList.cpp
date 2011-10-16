// NumList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UniEditor.h"
#include "NumList.h"
#include "RegionTree.h"
#include "Region.h"
#include "DlgMoninfo.h"
#include "TextLog.h"
#ifdef _DEBUG


#endif // _DEBUG

// CNumList

IMPLEMENT_DYNAMIC(CNumList, CListCtrl)

CNumList::CNumList()
{

}

CNumList::~CNumList()
{
}


BEGIN_MESSAGE_MAP(CNumList, CListCtrl)
	ON_NOTIFY(NM_CLICK, IDC_NUMLIST, &CNumList::OnNMClickNumlist)
END_MESSAGE_MAP()



// CNumList ��Ϣ�������


BOOL CNumList::InitialList()
{
	DWORD dwStyle = this->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;	//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;		//�����ߣ�ֻ������report����listctrl��
	SetExtendedStyle(dwStyle);			//������չ���

	InsertColumn(0,"����",LVCFMT_LEFT,150,0);
	InsertColumn(1,"����",LVCFMT_LEFT,150,0);
	CTextLog::WriteLog("�����б��ʼ���ɹ�");
	return TRUE;
}
struct tagInfo
{
	string name;
	int num;
};
void CNumList::SetData(CRegion* pRegion,int type)
{
	DeleteAllItems();
	char strVal[100];
	if (!pRegion)
		return;
	switch(type)
	{
	case ITEM_OFFSET_MONSTER:
			{
				m_mapStrMonster.clear();
				VECMONSTER vecmonster=pRegion->GetMonster();
				VECMONSTERITR itmonster=vecmonster.begin();
				long allnum=0;
				vector<tagInfo>vecInfo;
				vector<tagInfo>::iterator itInfo;
				map<string,tagList>::iterator itstr;
				for (;itmonster!=vecmonster.end();itmonster++)
				{
					tagList list;
					long lnum=(*itmonster)->GetQuantity();
					VECMONSTERINFO monsterinfo=(*itmonster)->GetMonsterInfo();
					if (monsterinfo.size()>0)
					{
						VECMONSTERINFOITR it=monsterinfo.begin();
						long lodds=(*it)->GetProbability()/100;
						itstr=m_mapStrMonster.find( (*it)->GetName() );
						if (itstr==m_mapStrMonster.end())
						{
							list.nNum=lodds*lnum;
							m_mapStrMonster[(*it)->GetName()]=list;
							allnum+=lodds*lnum;
						}
						else
						{
							itstr->second.nNum +=lodds*lnum;
							allnum+=lodds*lnum;
						}	
					}					
				}
				itstr=m_mapStrMonster.begin();
				int i=0;
				for (;itstr!=m_mapStrMonster.end();itstr++,i++)
				{
					InsertItem(i,"");
					SetItemText(i,0,itstr->first.c_str());
					_itoa(itstr->second.nNum,strVal,10);
					SetItemText(i,1,strVal);
					SetItemData(i,itstr->second.nNum);
				}
				InsertItem(i,"");
				sprintf_s(strVal,"�ϼ�monster����:%d",i);
				SetItemText(i,0,strVal);
				sprintf_s(strVal,"%d",allnum);
				SetItemText(i,1,strVal);
				SetItemData(i,allnum);
				//��¼��������������
				for (itmonster=vecmonster.begin();itmonster!=vecmonster.end();itmonster++)
				{
					VECMONSTERINFO monsterinfo=(*itmonster)->GetMonsterInfo();
					if (monsterinfo.size()>0)
					{
						VECMONSTERINFOITR it=monsterinfo.begin();
						string strname=(*it)->GetName();
						itstr=m_mapStrMonster.find(strname);
						if (itstr!=m_mapStrMonster.end())
						{
							itstr->second.vecNo.push_back( (*itmonster)->GetNo() );
						}
						else
						{

						}
						
					}
				}
			}
			break;
		case ITEM_OFFSET_NPC:
			{
				m_mapStrColl.clear();
				VECCOLLECTION vecmonster=pRegion->GetCollection();
				VECCOLLECTIONITR itmonster=vecmonster.begin();
				long allnum=0;
				map<string,tagList>::iterator itstr;
				for (;itmonster!=vecmonster.end();itmonster++)
				{
					tagList list;
					long lnum=(*itmonster)->GetQuantity();
					VECCOLLECTIONINFO monsterinfo=(*itmonster)->GetCollectionInfo();
					if (monsterinfo.size()>0)
					{
						VECCOLLINFOITR it=monsterinfo.begin();
						long lodds=(*it)->GetProbability()/100;
						itstr=m_mapStrColl.find( (*it)->GetName() );
						if (itstr==m_mapStrColl.end())
						{
							list.nNum=lodds*lnum;
							m_mapStrColl[(*it)->GetName()]=list;
							allnum+=lodds*lnum;
						}
						else
						{
							itstr->second.nNum +=lodds*lnum;
							allnum+=lodds*lnum;
						}	
					}					
				}
				itstr=m_mapStrColl.begin();
				int i=0;
				for (;itstr!=m_mapStrColl.end();itstr++,i++)
				{
					InsertItem(i,"");
					SetItemText(i,0,itstr->first.c_str());
					itoa(itstr->second.nNum,strVal,10);
					SetItemText(i,1,strVal);
					SetItemData(i,itstr->second.nNum);
				}
				InsertItem(i,"");
				sprintf_s(strVal,"�ϼ�collection����:%d",i);
				SetItemText(i,0,strVal);
				sprintf_s(strVal,"%d",allnum);
				SetItemText(i,1,strVal);
				SetItemData(i,allnum);
				//��¼��������������
				for (itmonster=vecmonster.begin();itmonster!=vecmonster.end();itmonster++)
				{
					VECCOLLECTIONINFO monsterinfo=(*itmonster)->GetCollectionInfo();
					if (monsterinfo.size()>0)
					{
						VECCOLLINFOITR it=monsterinfo.begin();
						string strname=(*it)->GetName();
						itstr=m_mapStrColl.find(strname);
						if (itstr!=m_mapStrColl.end())
						{
							itstr->second.vecNo.push_back( (*itmonster)->GetNo() );
						}
						else
						{

						}

					}
				}
			}
			break;
		case ITEM_OFFSET_SIGNPOS:
			{
				
				VECSIGNPOS vecsignpos=pRegion->GetSignPos();
				InsertItem(0,"");
				sprintf_s(strVal,"�ϼ�·���:");
				SetItemText(0,0,strVal);
				sprintf_s(strVal,"%d",vecsignpos.size());
				SetItemText(0,1,strVal);
				SetItemData(0,vecsignpos.size());
			}
			break;
		case ITEM_OFFSET_ROADS:
			{
				MAPROADS roadpoint=pRegion->GetRoads();
				long lsize=(long)roadpoint.size();
				MAPROADSREVERSEITR it=roadpoint.rbegin();
				int i=0;
				for (;it!=roadpoint.rend();it++)
				{
					InsertItem(i,"");
					sprintf_s(strVal,"·��:%d",it->first);
					SetItemText(i,0,strVal);
					VECROADPOINT vecpoint=( *it->second ).GetRoadsPoints();
					sprintf_s(strVal,"%d",vecpoint.size());
					SetItemText(i,1,strVal);
					SetItemData(i,vecpoint.size());
				}
				InsertItem(i,"");
				sprintf_s(strVal,"�ϼ�·��:%d",i);
				SetItemText(i,0,strVal);
				sprintf_s(strVal,"%d",roadpoint.size());
				SetItemText(i,1,strVal);
				SetItemData(i,roadpoint.size());
			}
			break;
	}	
}
void CNumList::OnNMClickNumlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}

BOOL CNumList::GetVecData(int nType,string str,vector<long>&vecData)
{
	switch(nType)
	{
		case ITEM_OFFSET_MONSTER:
			{
				map<string,tagList>::iterator it;
				it=m_mapStrMonster.find(str);
				if (it==m_mapStrMonster.end())
				{
					return false;
				}
				vector<long>& vecSrc = it->second.vecNo;
				vector<long>::iterator itr = vecSrc.begin();
				for (;itr!=vecSrc.end();itr++)
				{
					vecData.push_back(*itr);
				}
				//copy(vecSrc.begin(),vecSrc.end(),vecData.begin());

				return true;
			}
			break;
		case ITEM_OFFSET_NPC:
			{
				map<string,tagList>::iterator it;
				it=m_mapStrColl.find(str);
				if (it==m_mapStrColl.end())
				{
					return false;
				}
				vector<long>& vecSrc = it->second.vecNo;
				vector<long>::iterator itr = vecSrc.begin();
				for (;itr!=vecSrc.end();itr++)
				{
					vecData.push_back(*itr);
				}
				return true;
			}
			break;
	}
	return false;
}