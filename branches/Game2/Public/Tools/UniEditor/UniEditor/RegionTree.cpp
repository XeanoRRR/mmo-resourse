 #include "StdAfx.h"
#include "Monster.h"
//#include "NPC.h"
#include "RegionTree.h"
#include "resource.h"
#include "UniEditor.h"
#include "UniEditorDlg.h"
#include "DlgAddNode.h"

#include <algorithm>

#pragma warning (disable : 4996)
#pragma warning (disable : 4018)
#pragma warning (disable : 4244)

//BOOL CRegionTree::m_bItemFlag = FALSE;

CRegionTree::CRegionTree(void)
: m_uLayer(0), m_uOffset(0), m_uNo(0), m_hRCItem(NULL), m_uRCLayer(0), m_uRCOffset(0),
m_pRCRegion(NULL), m_uErrorType(ERROR_NONE)
{
}

CRegionTree::~CRegionTree(void)
{
	ITRTREE itr;
	for(itr=m_mapTree.begin(); itr!=m_mapTree.end(); ++itr)
	{
		if(itr->second != NULL)
		{
			CRegion* pRegion = itr->second;
			if (pRegion)
			{
				delete pRegion;
			}
		}
	}
	for(ITRREGION it = m_listRegionList.begin();it!=m_listRegionList.end();it++)
	{
		CRegionList* pRegionList = *it;
		if (pRegionList)
		{
			delete pRegionList;
		}
	}
	VECMONSTERNAMEITR it= m_vecMonsterName.begin();
	for (;it!=m_vecMonsterName.end();it++)
	{
		delete *it;
	}

	VECCLOOECTIONNAMEITR itC= m_vecCollectionName.begin();
	for (;itC!=m_vecCollectionName.end();itC++)
	{
		delete *itC;
	}
}

BEGIN_MESSAGE_MAP(CRegionTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CRegionTree::OnNMRclick)
	ON_COMMAND(ID__ADD, &CRegionTree::OnPopMenuAdd)
	ON_COMMAND(ID__DEL, &CRegionTree::OnPopMenuDel)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CRegionTree::OnTvnSelchanged)
	ON_WM_PAINT()
END_MESSAGE_MAP()


string NameTree(string str)
{
	int i=0;
	int j;
	char strVal[100];
	for(;;i++)
	{
		if(34==str[i])
		{
			for(j=0;34!=str[i+1];j++,i++)	
				strVal[j]=str[i+1];
			strVal[j]='\0';
			return strVal;	
		}
	}
	return strVal;
}
// ��ʼ�����οؼ�
BOOL CRegionTree::InitialRegionTree()
{
	m_ncurrRoads=0;
	HTREEITEM hItemRoot, hItemRegion;
	hItemRoot = InsertItem(_T("��Ϸ����"));
	SetItemData(hItemRoot, ITEM_ROOT);
	string str,strorig;
	ifstream ifStream,ifStreamMonster, ifStreamColl;
	
	
	// ��ȡregionlist.ini�ļ�
	ifStream.open(strFileRegion.c_str(), ios_base::in);
	if(!ifStream.is_open())
	{
#ifdef _RELEASELOG


		CTextLog::WriteLog("�����ļ��޷���!");

#endif
		MessageBox(_T("�����ļ��޷���!"));
		return FALSE;
	}

	while(TRUE == CUniEditorApp::ReadTo(_T("#"), ifStream))
	{
		CRegionList *pRegion = new CRegionList;
		ifStream >> *pRegion;
		
		CString szRegion;
		szRegion.Format(_T("%u %s"), pRegion->GetID(), pRegion->GetName().c_str());
		hItemRegion = InsertItem(szRegion, hItemRoot, TVI_SORT);
		SetItemData(hItemRegion, pRegion->GetID());

		m_listRegionList.push_back(pRegion);
	}
	ifStream.close();
#ifdef _RELEASELOG
	char strTemp[256];
	sprintf(strTemp,"�ɹ���ȡ�����б�regionlist.ini");
	CTextLog::WriteLog(strTemp);

#endif
	//��ȡ�����б��ļ�
	ifStreamMonster.open("data\\monsterlist.ini");
	if(!ifStreamMonster.is_open())
	{
#ifdef _RELEASELOG


		CTextLog::WriteLog("�޷���ȡmonsterlist.ini");

#endif
		MessageBox("�޷���ȡmonsterlist.ini");
		return FALSE;
	}
	while(TRUE == CUniEditorApp::ReadTo(_T("*"), ifStreamMonster))
	{
		tagMonsterName *pMonstername=new tagMonsterName;
		ifStreamMonster>>str>>(*pMonstername).strOrigName>>(*pMonstername).strName;
		m_vecMonsterName.push_back(pMonstername);
	}
	ifStreamMonster.close();
#ifdef _RELEASELOG

	sprintf(strTemp,"�ɹ���ȡ�����б�monsterlist.ini");
	CTextLog::WriteLog(strTemp);

#endif
	//��ȡ�ɼ��������ļ�
	ifStreamColl.open("data\\CollectionList.xml");
	if(!ifStreamColl.is_open())
	{
#ifdef _RELEASELOG

		
		CTextLog::WriteLog("�޷���ȡCollectionList.xml");

#endif
		MessageBox("�޷���ȡCollectionList.xml");
		return FALSE;
	}
	while(TRUE == CUniEditorApp::ReadTo(_T("<Collection"), ifStreamColl))
	{
		tagCollectionName *pCollectionName=new tagCollectionName;
		ifStreamColl>>str>>strorig;
		pCollectionName->strName=NameTree(str);
		pCollectionName->strOrigName=NameTree(strorig);
		m_vecCollectionName.push_back(pCollectionName);
	}
	Expand(hItemRoot, TVE_EXPAND);
#ifdef _RELEASELOG

	sprintf(strTemp,"�ɹ��ɼ����б�CollectionList.xml");
	CTextLog::WriteLog(strTemp);

#endif
	return TRUE;
}

const UINT CRegionTree::GetLayer(HTREEITEM hItem)
{
	unsigned uLayer = 0;
	while(NULL != (hItem = GetParentItem(hItem)))
	{
		++uLayer;
	}

	return uLayer;
}

const UINT CRegionTree::GetOffset(HTREEITEM hItem)
{
	unsigned uOffset = 0;
	while (hItem != NULL)
	{
		++uOffset;
		hItem = GetPrevSiblingItem(hItem);
	}

	return uOffset-1;
}


// ��ȡ����
// �˴����뽫���ݴ洢�ڶ�ջ��
BOOL CRegionTree::ReadData(HTREEITEM hItem)
{
	// ��ȡRegion��Ϣ
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));

	CRegion *pRegion = new CRegion;
	ifstream ifs;
	char strRgn[32];
	unsigned uRegionNo = GetItemData(hItem);

	sprintf(strRgn, _T("regions\\%u.rgn"), uRegionNo);
	pRegion->SetRegionNo(uRegionNo);
	pRegion->SetRegionName(GetItemText(hItem));
	ifs.open(strRgn, ios_base::in);
	if(!ifs.is_open())
	{
		SetItemData(hItem, ITEM_REGION_UNEXISTED);
		char strVal[59];
		sprintf_s(strVal,"���ܴ򿪱��Ϊ:%d��RGN�ļ�",uRegionNo);
		MessageBox(strVal);
		return FALSE;
	}
	ifs >> pRegion;
	ifs.close();
	ifs.clear();
	SetItemData(hItem, uRegionNo);

	HTREEITEM hItemMonster, hItemNPC,hItemSignPos,hItemRoads;

	// ���Monster�ӽ��
	hItemMonster = InsertItem(_T("����"), 0, 0, hItem);
	SetItemData(hItemMonster, uRegionNo);

	// ���NPC�ӽ��
	hItemNPC = InsertItem(_T("�ɼ���"), 0, 0, hItem);
	SetItemData(hItemNPC, uRegionNo);
	
	//���·����ӽ��
	hItemSignPos = InsertItem(_T("·���"), 0, 0, hItem);
	SetItemData(hItemSignPos, uRegionNo);

	//���·����ʶ�ӽ��
	hItemRoads =InsertItem(_T("·��"),0,0,hItem);
	SetItemData(hItemRoads,uRegionNo);

	CString strLog;
	string str;

	// ��ȡMonster��Ϣ
	sprintf(strRgn, _T("regions\\%u.monster"), uRegionNo);
	ifs.open(strRgn, ios_base::in);
	if(ifs.is_open())
	{
		int count = 0;
		//// ���˵����ص�����
		//while(CUniEditorApp::ReadTo(_T("#"), ifs))
		// ���˵�ǰ������
		int i = 9;
		while(--i)
		{
			ifs >> str;
		}

		while(_T("") == str && !ifs.eof())
		{
			ifs >> str;
		}

		while(!ifs.eof())
		{
			
			unsigned uNo;

			// ǰ��MONSTER��Ϣ��ȡ���
			if (strEnd == str)
			{
				break;
			}
			CMonster *pmonster = new CMonster(pRegion->Width(), pRegion->Height());
			if (_T("#") == str)
			{

				pmonster->SetSwitch();
				ifs >> uNo;
				pmonster->SetNo(uNo);
			}
			else
			{
				pmonster->SetSwitch(false);
				uNo = atoi(str.c_str());
				pmonster->SetNo(uNo);
			}

			if (0 == uNo)
			{
				strLog.Format(_T("%u.monster�д��ڱ��Ϊ0�Ĺ�����ܵ��¹����޷�����"), uRegionNo);
				pListLog->AddLog(LOG_ERROR, strLog);
			}

			ifs >> *pmonster;

			pRegion->GetMonster().push_back(pmonster);
			++count;
			ifs >> str;
		}

		
		// ��ȡMonsterInfo��Ϣ
		VECMONSTERNAMEITR  iter;
		VECMONSTER &vecMonster = pRegion->GetMonster();
		for(int i=0; i<count && !ifs.eof(); i++)
		{
			string strTemp;
			unsigned uNo(0), uInd(0);			// monster��ż�����vector�е��±�
			ifs >> strTemp >> uNo;

			if (_T("id") != strTemp)
			{
				strLog.Format(_T("%u.monster�ļ���ʽ����ȷ���޷�������ȡ�������������Ϣ"), uRegionNo);
				if(!pListLog->AddLog(LOG_ERROR, strLog))
				{
					break;
				}
				break;
			}
			
			if(!pRegion->CheckMonsterNo(uNo, uInd))
			{
				strLog.Format(_T("%u.monster�в����ڱ��Ϊ%u�Ĺ���"), uRegionNo, uNo);
				pListLog->AddLog(LOG_WARNING, strLog);
				break;
			}

			while(CUniEditorApp::ReadTo(_T("#"), ifs))
			{
				CMonsterInfo *pmoninfo = new CMonsterInfo;
				ifs >> *pmoninfo;
				vecMonster[uInd]->GetMonsterInfo().push_back(pmoninfo);
				if (pmoninfo->GetName() == _T("0"))
				{
					strLog.Format(_T("%u.monster�б��Ϊ%u�Ĺ����д��ڷǷ����ƹ���"), uRegionNo, vecMonster[uInd]->GetNo());
					pListLog->AddLog(LOG_ERROR, strLog);
				}
			}
		}

		for(int i=0; i<vecMonster.size(); i++)
		{
			unsigned uNo = vecMonster[i]->GetNo();
			CString str;
			string strMonsterName,strTemp;
			strTemp="";
			str.Format(_T("%u"), uNo);
			
			HTREEITEM hSubItem = InsertItem(str, hItemMonster);
			SetItemData(hSubItem, uRegionNo);
			VECMONINFO &vecMonInfo = vecMonster[i]->GetMonsterInfo();

			if (vecMonInfo.empty())
			{
				strLog.Format(_T("%u.monster�в����ڱ��Ϊ%u�Ĺ���ľ�����Ϣ�����ܵ��¹����޷�����"), uRegionNo, uNo);
				if(!pListLog->AddLog(LOG_ERROR, strLog))
				{
					break;
				}
			}

			else
			{
				for(int j=0; j<vecMonInfo.size(); j++)
				{
					str = vecMonInfo[j]->GetName().c_str();
					
					for(iter=m_vecMonsterName.begin();iter!=m_vecMonsterName.end();iter++)
					{
						strMonsterName=(*iter)->strOrigName.c_str();
						if(strMonsterName.c_str()==str)
						{
							
							strTemp=(*iter)->strName+"<"+str.GetString()+">";
							
							break;
						}
					}
					if(iter==m_vecMonsterName.end())
					{
						strTemp="С����<002squirrel>";
						vecMonInfo[j]->SetName(strTemp);
					}
					else
					{
						vecMonInfo[j]->SetName(strTemp);
					}
						HTREEITEM hInfoItem = InsertItem(strTemp.c_str(), hSubItem);
						SetItemData(hInfoItem, uRegionNo);
					
				}
			}
		}
	}
	ifs.close();
	ifs.clear();

#ifdef _RELEASELOG

	char strTemp[256];
	sprintf(strTemp,"�ɹ���ȡ����%u.monster",uRegionNo);
	CTextLog::WriteLog(strTemp);

#endif


	
	//��ȡ�ɼ�����Ϣ
	sprintf(strRgn, _T("regions\\%u.collection"), uRegionNo);
	ifs.open(strRgn, ios_base::in);
	if(ifs.is_open())
	{
		int count = 0;
		//// ���˵����ص�����
		//while(CUniEditorApp::ReadTo(_T("#"), ifs))
		// ���˵�ǰ������
		int i = 9;
		while(--i)
		{
			ifs >> str;
		}

		while(_T("") == str && !ifs.eof())
		{
			ifs >> str;
		}

		while(!ifs.eof())
		{
			
			unsigned uNo;

			
			if (strEnd == str)
			{
				break;
			}
			CCollection *pCollection = new CCollection(pRegion->Width(), pRegion->Height());
			if (_T("#") == str)
			{
				pCollection->SetSwitch();
				ifs >> uNo;
				pCollection->SetNo(uNo);
			}
			else
			{
				pCollection->SetSwitch(false);
				uNo = atoi(str.c_str());
				pCollection->SetNo(uNo);
			}

			if (0 == uNo)
			{
				strLog.Format(_T("%u.monster�д��ڱ��Ϊ0�Ĳɼ���,���ܵ����޷�����"), uRegionNo);
				pListLog->AddLog(LOG_ERROR, strLog);
			}

			ifs>>*pCollection;

			pRegion->GetCollection().push_back(pCollection);
			++count;
			ifs >> str;
		}
		
		VECCOLLECTION &vecCollection = pRegion->GetCollection();
		for(int i=0; i<count && !ifs.eof(); i++)
		{
			string strTemp;
			unsigned uNo(0), uInd(0);			// monster��ż�����vector�е��±�
			ifs >> strTemp >> uNo;

			if (_T("id") != strTemp)
			{
				strLog.Format(_T("%u.collection�ļ���ʽ����ȷ���޷�������ȡ�ɼ�����Ϣ"), uRegionNo);
				if(!pListLog->AddLog(LOG_ERROR, strLog))
				{
					break;
				}
				break;
			}
			
			if(!pRegion->CheckCollectionNo(uNo, uInd))
			{
				strLog.Format(_T("%u.collection�в����ڱ��Ϊ%u�Ĳɼ���"), uRegionNo, uNo);
				pListLog->AddLog(LOG_WARNING, strLog);
				break;
			}

			while(CUniEditorApp::ReadTo(_T("#"), ifs))
			{
				CCollectionInfo *pCollectionInfo = new CCollectionInfo;
				ifs >> *pCollectionInfo;
				vecCollection[uInd]->GetCollectionInfo().push_back(pCollectionInfo);
				if (pCollectionInfo->GetName() == _T("0"))
				{
					strLog.Format(_T("%u.collection�б��Ϊ%u�Ĺ����д��ڷǷ�����"), uRegionNo, vecCollection[uInd]->GetNo());
					pListLog->AddLog(LOG_ERROR, strLog);
				}
			}
		}
		//��Ӳɼ���ڵ�
		VECCLOOECTIONNAMEITR itercoll;
		for(int i=0; i<vecCollection.size(); i++)
		{
			unsigned uNo = vecCollection[i]->GetNo();
			CString str;
			string strCollName,strTemp;
			str.Format(_T("%u"), uNo);
			HTREEITEM hSubItem = InsertItem(str, hItemNPC);
			SetItemData(hSubItem, uRegionNo);
			VECCOLLECTIONINFO &vecCollectionInfo = vecCollection[i]->GetCollectionInfo();

			if (vecCollectionInfo.empty())
			{
				strLog.Format(_T("%u.collection�в����ڱ��Ϊ%u�Ĳɼ���ľ�����Ϣ�����ܵ��²ɼ����޷�����"), uRegionNo, uNo);
				if(!pListLog->AddLog(LOG_ERROR, strLog))
				{
					break;
				}
			}

			else
			{
				for(int j=0; j<vecCollectionInfo.size(); j++)
				{
					str = vecCollectionInfo[j]->GetName().c_str();
					for(itercoll=m_vecCollectionName.begin();itercoll!=m_vecCollectionName.end();itercoll++)
					{
						strCollName=(*itercoll)->strOrigName.c_str();
						if(strCollName.c_str()==str)
						{
							//str=str+strMonsterName.c_str();
							strTemp=(*itercoll)->strName+"<"+str.GetString()+">";
							
							break;
						}
					}
					if(itercoll==m_vecCollectionName.end())
					{
						strTemp="ͭ����<D01001>";
						vecCollectionInfo[j]->SetName(strTemp);
					}
					vecCollectionInfo[j]->SetName(strTemp);
					HTREEITEM hInfoItem = InsertItem(strTemp.c_str(), hSubItem);
					SetItemData(hInfoItem, uRegionNo);
				}
			}
		}
	}
	ifs.close();
	ifs.clear();
#ifdef _RELEASELOG

	sprintf(strTemp,"�ɹ���ȡ����%u.collection",uRegionNo);
	CTextLog::WriteLog(strTemp);

#endif
	//��ȡ·�����Ϣ
	sprintf(strRgn, _T("regions\\%u.xml"), uRegionNo);
	ifs.open(strRgn, ios_base::in);
	VECSIGNPOS &vecsignpos=pRegion->GetSignPos();
	if(ifs.is_open())
	{
		
		
		int no=1;
		while(CUniEditorApp::ReadTo(_T("<SignPos"), ifs))
		{
			CSignPos *psignpos=new CSignPos;
			psignpos->SetNo(no);
			ifs >> *psignpos;
			vecsignpos.push_back(psignpos);
			no+=1;
		}
	}
	ifs.close();
	ifs.clear();
	//���·����ӽ��
	VECSIGNPOSITR itersign=vecsignpos.begin();
	for(;itersign!=vecsignpos.end();itersign++)
	{
		char strVal[10];
		string str=itoa((*itersign)->m_nNo,strVal,10);
	
		
		HTREEITEM hSubItem = InsertItem(str.c_str(), hItemSignPos);
		SetItemData(hSubItem, uRegionNo);
	}
#ifdef _RELEASELOG


	sprintf(strTemp,"�ɹ���ȡ����%u.xml",uRegionNo);
	CTextLog::WriteLog(strTemp);

#endif
	//��ȡ·����ʶ��Ϣ
	sprintf(strRgn, _T("regions\\%u.roads"), uRegionNo);
	ifs.open(strRgn, ios_base::in);
	MAPROADS &maproads=pRegion->GetRoads();
	if (ifs.is_open())
	{
		while(CUniEditorApp::ReadTo(_T("*"), ifs))
		{
			CRoads *pRoads=new CRoads;
			ifs>>*pRoads;
			maproads[pRoads->GetNo()]=pRoads;
		}
	}
	ifs.close();
	ifs.clear();
	//��ӽڵ�
	MAPROADSITR itroads=maproads.begin();
	for (;itroads!=maproads.end();itroads++)
	{
	
		char strVal[32];
		string str=itoa((*itroads).first,strVal,10);
		HTREEITEM hSubItem = InsertItem(str.c_str(), hItemRoads);
		SetItemData(hSubItem, uRegionNo);
		VECROADPOINT vecpoint=(*itroads->second).GetRoadsPoints();
		VECROADPOINTITR itroadspoint=vecpoint.begin();
		for (int i=1;itroadspoint!=vecpoint.end();itroadspoint++,i++)
		{
			sprintf_s(strVal,"%d",(*itroadspoint).npointNo);
			HTREEITEM hSubItempoint = InsertItem(strVal, hSubItem);
			SetItemData(hSubItempoint, uRegionNo);
		}
		
	}
#ifdef _RELEASELOG

	sprintf(strTemp,"�ɹ���ȡ����%u.roads",uRegionNo);
	CTextLog::WriteLog(strTemp);

#endif
	m_mapTree.insert(MAPTREE::value_type(uRegionNo, pRegion));
	return TRUE;
}

// �������MONSTER���
BOOL CRegionTree::AddItemMonster(HTREEITEM hItem, CRegion *pRegion,CRect *rect,CMonster *pMonster)
{
	VECMONSTER	&vecMonster = pRegion->GetMonster();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;

	if (!vecMonster.empty())
	{
		uNoAdd = static_cast<unsigned> (vecMonster.size());
		if(NULL == pMonster)
		{
			pMonster = new CMonster(*vecMonster[uNoAdd-1]);
			pMonster->SetCoorLeft(rect->left);
			pMonster->SetCoorRight(rect->right);
			pMonster->SetCoorTop(rect->top);
			pMonster->SetCoorBottom(rect->bottom);

		}
		uNoAdd = vecMonster[uNoAdd-1]->GetNo() + 1;
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pMonster)
		{
			pMonster = new CMonster(pRegion->Width(), pRegion->Height());
		}
	}

	pMonster->SetNo(uNoAdd);
	pMonster->SetTimeStart(pMonster->GetTimeStart());
	vecMonster.push_back(pMonster);
//	CRect rect(pMonster->GetCoordinate());

	CString		szAddLog = _T("���ӽ�� <Monster>\t\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u"), uNoAdd);
	pMonster->Monster2String(szNodeInfo);
	szAddLog += szNoAdd + szNodeInfo;

	HTREEITEM hItemAdd = InsertItem(szNoAdd, hItem);
	SetItemData(hItemAdd, uRegionNo);

	VECMONINFO &vecMonInfo = pMonster->GetMonsterInfo();
	CString szNoInfo;
	CString szMonInfo;
	for(int j=0; j<vecMonInfo.size(); j++)
	{
		szNoInfo.Format(_T("\t%-u"), j);
		HTREEITEM hInfoItem = InsertItem(vecMonInfo[j]->GetName().c_str(), hItemAdd);
		SetItemData(hInfoItem, uRegionNo);
		vecMonInfo[j]->MonsterInfo2String(szMonInfo);
		szAddLog += szNoInfo + szMonInfo;
	}

	char strFile[32];
	sprintf(strFile, _T("%u.monster"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);

	// ������һ�����������������ӽ��ʱ��������Ƶ�ͼ,����Ч��
	SelectItem(hItemAdd);
	return TRUE;
}
// ����MONSTER���
BOOL CRegionTree::AddItemMonster(HTREEITEM hItem, CRegion *pRegion, CMonster *pMonster)
{
	VECMONSTER	&vecMonster = pRegion->GetMonster();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;

	if (!vecMonster.empty())
	{
		uNoAdd = static_cast<unsigned> (vecMonster.size());
		if(NULL == pMonster)
		{
			pMonster = new CMonster(*vecMonster[uNoAdd-1]);
		}
		uNoAdd = vecMonster[uNoAdd-1]->GetNo() + 1;
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pMonster)
		{
			pMonster = new CMonster(pRegion->Width(), pRegion->Height());
		}
	}

	pMonster->SetNo(uNoAdd);
	pMonster->SetTimeStart(pMonster->GetTimeStart());
	vecMonster.push_back(pMonster);
	CRect rect(pMonster->GetCoordinate());

	CString		szAddLog = _T("���ӽ�� <Monster>\t\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u"), uNoAdd);
	pMonster->Monster2String(szNodeInfo);
	szAddLog += szNoAdd + szNodeInfo;

	HTREEITEM hItemAdd = InsertItem(szNoAdd, hItem);
	SetItemData(hItemAdd, uRegionNo);

	VECMONINFO &vecMonInfo = pMonster->GetMonsterInfo();
	CString szNoInfo;
	CString szMonInfo;
	for(int j=0; j<vecMonInfo.size(); j++)
	{
		szNoInfo.Format(_T("\t%-u"), j);
		HTREEITEM hInfoItem = InsertItem(vecMonInfo[j]->GetName().c_str(), hItemAdd);
		SetItemData(hInfoItem, uRegionNo);
		vecMonInfo[j]->MonsterInfo2String(szMonInfo);
		szAddLog += szNoInfo + szMonInfo;
	}

	char strFile[32];
	sprintf(strFile, _T("%u.monster"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);

	// ������һ�����������������ӽ��ʱ��������Ƶ�ͼ,����Ч��
	SelectItem(hItemAdd);
	return TRUE;
}
//������Ӳɼ�����
BOOL CRegionTree::AddItemCollection(HTREEITEM hItem, CRegion *pRegion, CRect *rect,CCollection *pCollection)
{
	VECCOLLECTION	&vecCollection = pRegion->GetCollection();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;

	if (!vecCollection.empty())
	{
		uNoAdd = static_cast<unsigned> (vecCollection.size());
		if(NULL == pCollection)
		{
			pCollection = new CCollection(*vecCollection[uNoAdd-1]);
			pCollection->SetCoorLeft((rect->left));
			pCollection->SetCoorRight(rect->right);
			pCollection->SetCoorTop(rect->top);
			pCollection->SetCoorBottom(rect->bottom);
		}
		uNoAdd = vecCollection[uNoAdd-1]->GetNo() + 1;
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pCollection)
		{
			pCollection = new CCollection(pRegion->Width(), pRegion->Height());
		}
	}

	pCollection->SetNo(uNoAdd);
	pCollection->SetTimeStart(pCollection->GetTimeStart());
	vecCollection.push_back(pCollection);
//	CRect rect(pCollection->GetCoordinate());

	CString		szAddLog = _T("���ӽ�� <Collection>\t\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u"), uNoAdd);
	pCollection->Collection2String(szNodeInfo);
	szAddLog += szNoAdd + szNodeInfo;

	HTREEITEM hItemAdd = InsertItem(szNoAdd, hItem);
	SetItemData(hItemAdd, uRegionNo);

	VECCOLLINFO &vecCollInfo = pCollection->GetCollectionInfo();
	CString szNoInfo;
	CString szCollInfo;
	for(int j=0; j<vecCollInfo.size(); j++)
	{
		szNoInfo.Format(_T("\t%-u"), j);
		HTREEITEM hInfoItem = InsertItem(vecCollInfo[j]->GetName().c_str(), hItemAdd);
		SetItemData(hInfoItem, uRegionNo);
//		vecCollInfo[j]->CollInfo2String(szCollInfo);
		szAddLog += szNoInfo + szCollInfo;
	}

	char strFile[32];
	sprintf(strFile, _T("%u.collection"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);

	// ������һ�����������������ӽ��ʱ��������Ƶ�ͼ,����Ч��
	SelectItem(hItemAdd);
	return TRUE;
}
//���Ӳɼ�����
BOOL CRegionTree::AddItemCollection(HTREEITEM hItem, CRegion *pRegion, CCollection *pCollection)
{
	VECCOLLECTION	&vecCollection = pRegion->GetCollection();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;

	if (!vecCollection.empty())
	{
		uNoAdd = static_cast<unsigned> (vecCollection.size());
		if(NULL == pCollection)
		{
			pCollection = new CCollection(*vecCollection[uNoAdd-1]);
		}
		uNoAdd = vecCollection[uNoAdd-1]->GetNo() + 1;
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pCollection)
		{
			pCollection = new CCollection(pRegion->Width(), pRegion->Height());
		}
	}

	pCollection->SetNo(uNoAdd);
	pCollection->SetTimeStart(pCollection->GetTimeStart());
	vecCollection.push_back(pCollection);
	CRect rect(pCollection->GetCoordinate());

	CString		szAddLog = _T("���ӽ�� <Collection>\t\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u"), uNoAdd);
	pCollection->Collection2String(szNodeInfo);
	szAddLog += szNoAdd + szNodeInfo;

	HTREEITEM hItemAdd = InsertItem(szNoAdd, hItem);
	SetItemData(hItemAdd, uRegionNo);

	VECCOLLINFO &vecCollInfo = pCollection->GetCollectionInfo();
	CString szNoInfo;
	CString szCollInfo;
	for(int j=0; j<vecCollInfo.size(); j++)
	{
		szNoInfo.Format(_T("\t%-u"), j);
		HTREEITEM hInfoItem = InsertItem(vecCollInfo[j]->GetName().c_str(), hItemAdd);
		SetItemData(hInfoItem, uRegionNo);
	//	vecCollInfo[j]->CollInfo2String(szCollInfo);
		szAddLog += szNoInfo + szCollInfo;
	}

	char strFile[32];
	sprintf(strFile, _T("%u.collection"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	CTextLog::WriteLog(szAddLog);
	//// ������һ�����������������ӽ��ʱ��������Ƶ�ͼ,����Ч��
	SelectItem(hItemAdd);
	return TRUE;
}
//����·�����
BOOL CRegionTree::AddItemRoads(HTREEITEM hItem, CRegion *pRegion, CRoads *pRoads)
{
	MAPROADS	&maproads = pRegion->GetRoads();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;
	MAPROADSITR itr=maproads.begin();
	int nmaxfirst=0;
	if (!maproads.empty())
	{
		
		for (;itr!=maproads.end();itr++)
		{
			if (itr->first>nmaxfirst)
			{
				nmaxfirst=itr->first;
			}
		}
		if(NULL == pRoads)
		{
			pRoads = new CRoads;
		}
		
	}
	else
	{
		uNoAdd = 1;
		if(NULL == pRoads)
		{
			pRoads = new CRoads;
		}
	}
	char strLog[256];
	sprintf(strLog," %d ",nmaxfirst+1);
	maproads[nmaxfirst+1]=pRoads;
	char strVal[10];
	itoa(nmaxfirst+1,strVal,10);
	string str(strVal);
	HTREEITEM hItemAdd = InsertItem(str.c_str(), hItem);

	

	SetItemData(hItemAdd, uRegionNo);
	SelectItem(hItemAdd);
	char strFile[32];
	sprintf(strFile, _T("%u.roads"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CString szAddLog="������·��:";
	CUniEditorDlg::GetLog()->AddLog(szAddLog+strLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog+strLog);
	CTextLog::WriteLog(szAddLog+strLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	return TRUE;
}
//�������·���
BOOL CRegionTree::AddItemSignPos(HTREEITEM hItem, CRegion *pRegion, CRect *rect,CSignPos *pSignPos)
{
	VECSIGNPOS	&vecSignPos= pRegion->GetSignPos();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;
	
	if (!vecSignPos.empty())
	{
		uNoAdd = static_cast<unsigned> (vecSignPos.size());
		if(NULL == pSignPos)
		{
			
			
			pSignPos = new CSignPos(uNoAdd,(*rect));
		}
		uNoAdd = vecSignPos[uNoAdd-1]->GetNo() + 1;
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pSignPos)
		{	CRect rect1(0,0,3,3);
			
			pSignPos = new CSignPos(uNoAdd,rect);
		}
	}

	pSignPos->SetNo(uNoAdd);
	vecSignPos.push_back(pSignPos);
	char strValPos[20];
	sprintf(strValPos,"x:%d,y:%d",(*rect).left,(*rect).top);
	string strPos(strValPos);
	CString szAddLog;
	szAddLog+=strPos.c_str();
	szAddLog +=_T("����·���<SignPos>\t\t������ ")+ pRegion->GetRegionName() + _T("\n\t");
	
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u"), uNoAdd);

	szAddLog += szNoAdd + szNodeInfo;

	//���������
	HTREEITEM hItemAdd = InsertItem(szNoAdd, hItem);
	SetItemData(hItemAdd, uRegionNo);

	
	

	char strFile[32];
	sprintf(strFile, _T("%u.xml"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	CTextLog::WriteLog(szAddLog);
	// ������һ�����������������ӽ��ʱ��������Ƶ�ͼ,����Ч��
	SelectItem(hItemAdd);
	return TRUE;
}
//����·���ڵ�
BOOL CRegionTree::AddItemSignPos(HTREEITEM hItem, CRegion *pRegion,CSignPos *pSignPos)
{
	VECSIGNPOS	&vecSignPos= pRegion->GetSignPos();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;
	CRect rect;
	if (!vecSignPos.empty())
	{
		uNoAdd = static_cast<unsigned> (vecSignPos.size());
		if(NULL == pSignPos)
		{
			
			rect.left=vecSignPos[uNoAdd-1]->GetxPos();
			rect.top=vecSignPos[uNoAdd-1]->GetyPos();
			rect.right=rect.left+3;
			rect.bottom=rect.top+3;
			pSignPos = new CSignPos(uNoAdd,rect);
		}
		uNoAdd = vecSignPos[uNoAdd-1]->GetNo() + 1;
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pSignPos)
		{	CRect rect1(0,0,3,3);
			rect=rect1;
			pSignPos = new CSignPos(uNoAdd,rect);
		}
	}

	pSignPos->SetNo(uNoAdd);
	vecSignPos.push_back(pSignPos);
	char strValPos[20];
	sprintf(strValPos,"x:%d,y:%d",rect.left,rect.top);
	string strPos(strValPos);
	CString szAddLog;
	szAddLog+=strPos.c_str();
	szAddLog +=_T("����·���<SignPos>\t\t������ ")+ pRegion->GetRegionName() + _T("\n\t");
	
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u"), uNoAdd);

	szAddLog += szNoAdd + szNodeInfo;
	
	//���������
	HTREEITEM hItemAdd = InsertItem(szNoAdd, hItem);
	SetItemData(hItemAdd, uRegionNo);

	
	

	char strFile[32];
	sprintf(strFile, _T("%u.xml"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	CTextLog::WriteLog(szAddLog);
	// ������һ�����������������ӽ��ʱ��������Ƶ�ͼ,����Ч��
	SelectItem(hItemAdd);
	return TRUE;
}
//�������·���ڵ�
BOOL CRegionTree::AddItemRoadsPoint(HTREEITEM hItem, CRegion *pRegion,int n,CRect *rect,CRoads *pRoads)
{
	MAPROADS	&maproads= pRegion->GetRoads();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	MAPROADSITR it=maproads.find(n);
	int uNoAdd;
	CRoads::tagRoadsPoint roadspoint;
	roadspoint.nx=rect->left;
	roadspoint.ny=rect->top;
	
	VECROADPOINT vecroadspoint=(*it->second).GetRoadsPoints();
	if (!vecroadspoint.empty())
	{
			
		uNoAdd=(vecroadspoint[vecroadspoint.size()-1]).npointNo+1;
		roadspoint.npointNo=uNoAdd;
		(*it->second).GetRoadsPoints().push_back(roadspoint);
	}

	else
	{
		uNoAdd = 1;
		if(NULL == pRoads)
		{		
			roadspoint.npointNo=uNoAdd;
			(*it->second).GetRoadsPoints().push_back(roadspoint);
		}
	}
	char strLog[256];
	sprintf(strLog,"X:%d,Y:%d",roadspoint.nx,roadspoint.ny);
	char strVal[32];
	itoa(uNoAdd,strVal,10);
	string str(strVal);
	//��������
	HTREEITEM hItemAdd = InsertItem(str.c_str(), hItem);
	SetItemData(hItemAdd, uRegionNo);

	ZeroMemory(strVal,32);
	sprintf_s(strVal,"%d",roadspoint.npointNo);
	SetItemText(hItemAdd,strVal);

	char strFile[32];
	sprintf(strFile, _T("%u.roads"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	SETUPDATE &setUpdate = pWnd->GetUpdateList();
	setUpdate.insert(strFile);
	CString szAddLog="������·����:";
	CUniEditorDlg::GetLog()->AddLog(szAddLog+strLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog+strLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	CTextLog::WriteLog( szAddLog+strLog);
	return TRUE;
}
// ����MONSTERINFO���
BOOL CRegionTree::AddItemMonInfo(HTREEITEM hItem, CRegion *pRegion, CMonsterInfo *pMonInfo)
{
	unsigned	uNoAddMonster(GetOffset(hItem)), uNoAddMoninfo;
	VECMONSTER	&vecMonster = pRegion->GetMonster();
	CMonster	*pMonster =  vecMonster[uNoAddMonster];
	VECMONINFO	&vecMonInfo = pMonster->GetMonsterInfo();
	unsigned	uRegionNo = pRegion->GetRegionNo();

	if (!vecMonInfo.empty())
	{
		uNoAddMoninfo = static_cast<unsigned> (vecMonInfo.size());
	}

	else
	{
		uNoAddMoninfo = 0;
	}

	if(NULL == pMonInfo)
	{
		pMonInfo = new CMonsterInfo();
		pMonInfo->SetProbability(0);
	}

	vecMonInfo.push_back(pMonInfo);
	HTREEITEM hItemAdd = InsertItem(pMonInfo->GetName().c_str(), hItem);
	SetItemData(hItemAdd, uRegionNo);

	CString		szAddLog = _T("���ӽ�� <MonsterInfo>\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAddMonster, szNoAddMoninfo, szNodeMonster, szNodeInfo;
	szNoAddMonster.Format(_T("%-u"), uNoAddMonster);
	szNoAddMoninfo.Format(_T("\t%-u"), uNoAddMoninfo);
	pMonster->Monster2String(szNodeMonster);
	pMonInfo->MonsterInfo2String(szNodeInfo);
	szAddLog += szNoAddMonster + szNodeMonster + szNoAddMoninfo + szNodeInfo;

	char strFile[32];
	sprintf(strFile, _T("%u.monster"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	pWnd->GetUpdateList().insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	SelectItem(hItemAdd);
	CTextLog::WriteLog( szAddLog);
	return TRUE;
}
// ����CollInfo���
BOOL CRegionTree::AddItemCollInfo(HTREEITEM hItem, CRegion *pRegion, CCollectionInfo *pCollInfo)
{
	unsigned	uNoAddColl(GetOffset(hItem)), uNoAddCollinfo;
	VECCOLLECTION	&vecCollection = pRegion->GetCollection();
	CCollection	*pCollection =  vecCollection[uNoAddColl];
	VECCOLLINFO	&vecCollInfo = pCollection->GetCollectionInfo();
	unsigned	uRegionNo = pRegion->GetRegionNo();

	if (!vecCollection.empty())
	{
		uNoAddCollinfo = static_cast<unsigned> (vecCollection.size());
	}

	else
	{
		uNoAddCollinfo = 0;
	}

	if(NULL == pCollInfo)
	{
		pCollInfo = new CCollectionInfo();
		pCollInfo->SetProbability(0);
	}

	vecCollInfo.push_back(pCollInfo);
	HTREEITEM hItemAdd = InsertItem(pCollInfo->GetName().c_str(), hItem);
	SetItemData(hItemAdd, uRegionNo);

	CString		szAddLog = _T("���ӽ�� <MonsterInfo>\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAddMonster, szNoAddMoninfo, szNodeMonster, szNodeInfo;
	szNoAddMonster.Format(_T("%-u"), uNoAddColl);
	szNoAddMoninfo.Format(_T("\t%-u"), uNoAddCollinfo);
//	pCollection->Collection2String(szNodeMonster);
//	pCollInfo->CollInfo2String(szNodeInfo);
	szAddLog += szNoAddMonster + szNodeMonster + szNoAddMoninfo + szNodeInfo;

	char strFile[32];
	sprintf(strFile, _T("%u.collection"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	pWnd->GetUpdateList().insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
	SelectItem(hItemAdd);
	CTextLog::WriteLog( szAddLog);
	return TRUE;
}

// ����NPC���
BOOL CRegionTree::AddItemNPC(HTREEITEM hItem, CRegion *pRegion, CNPC *pNPC)
{
	/*VECNPC		&vecNPC = pRegion->GetNPC();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	unsigned	uNoAdd;

	if (!vecNPC.empty())
	{
		uNoAdd = static_cast<unsigned> (vecNPC.size());
		if(NULL == pNPC)
		{
			pNPC = new CNPC(*vecNPC[uNoAdd-1]);
		}
	}

	else
	{
		uNoAdd = 0;
		if (NULL == pNPC)
		{
			pNPC = new CNPC;
		}
	}

	vecNPC.push_back(pNPC);
	HTREEITEM hItemAdd = InsertItem(pNPC->GetName().c_str(), hItem);
	SetItemData(hItemAdd, uRegionNo);

	CString		szAddLog = _T("���ӽ�� <NPC>\t\t\t������ ") + pRegion->GetRegionName() + _T("\n\t");
	CString		szNoAdd, szNodeInfo;
	szNoAdd.Format(_T("%-u\t"), uNoAdd);
	pNPC->NPC2String(szNodeInfo);
	szAddLog += szNoAdd + szNodeInfo;

	char strFile[32];
	sprintf(strFile, _T("%u.npc"), uRegionNo);
	CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
	pWnd->GetUpdateList().insert(strFile);
	CUniEditorDlg::GetLog()->AddLog(szAddLog);
	CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
	pListLog->AddLog(LOG_ADD, szAddLog);
	((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);

	SelectItem(hItemAdd);*/
	return TRUE;
}

// ɾ��MONSTER���
BOOL CRegionTree::DelItemMonster(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion)
{
	VECMONSTER	&vecMonster = pRegion->GetMonster();

	unsigned	uRegionNo = pRegion->GetRegionNo();
	VECMONSTERITR itr = find(vecMonster.begin(), vecMonster.end(), vecMonster[uOffset]);

	if(itr != vecMonster.end())
	{
		CString		szDelLog = _T("ɾ����� <Monster>\t\t�ӳ��� ") + pRegion->GetRegionName() + _T("\n\t");
		CString		szNoDel, szNodeInfo;
		CMonster	*pMonster = vecMonster[uOffset];

		szNoDel.Format(_T("%-u"), uOffset);
		pMonster->Monster2String(szNodeInfo);
		szDelLog += szNoDel + szNodeInfo;

		VECMONINFO &vecMonInfo = pMonster->GetMonsterInfo();

		CString szNoInfo;
		CString szMonInfo;
		for(int j=0; j<vecMonInfo.size(); j++)
		{
			szNoInfo.Format(_T("\t%-u"), j);
			vecMonInfo[j]->MonsterInfo2String(szMonInfo);
			szDelLog += szNoInfo + szMonInfo;
		}

		HTREEITEM hItemTemp = hItem;
		vecMonster.erase(itr);
		//for(int uNo=uOffset; uNo<vecMonster.size(); ++uNo)
		//{
		//	vecMonster[uNo]->SetNo(uNo);
		//	CString str(_T(""));
		//	str.Format(_T("%u"), uNo);
		//	hItem = GetNextSiblingItem(hItem);
		//	SetItemText(hItem, str);
		//}

		char strFile[32];
		sprintf(strFile, _T("%u.monster"), uRegionNo);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		pWnd->GetUpdateList().insert(strFile);
		CUniEditorDlg::GetLog()->AddLog(szDelLog);
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szDelLog);
		((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
		DeleteItem(hItemTemp);
		CTextLog::WriteLog( szDelLog);
	}

	return TRUE;
}

// ɾ��MONSTERINFO���
BOOL CRegionTree::DelItemMonInfo(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion)
{
	unsigned	uNoDelMonster(GetOffset(GetParentItem(hItem)));
	VECMONSTER	&vecMonster = pRegion->GetMonster();
	CMonster	*pMonster =  vecMonster[uNoDelMonster];
	VECMONINFO	&vecMonInfo = pMonster->GetMonsterInfo();
	unsigned	uRegionNo = pRegion->GetRegionNo();

	VECMONINFOITR itr = find(vecMonInfo.begin(), vecMonInfo.end(), vecMonInfo[uOffset]);

	if(itr != vecMonInfo.end())
	{
		CMonsterInfo *pMonInfo = vecMonInfo[uOffset];
		CString		szDelLog = _T("ɾ����� <MonsterInfo>\t�ӳ��� ") + pRegion->GetRegionName() + _T("\n\t");
		CString		szNoDelMonster, szNoDelMoninfo, szNodeMonster, szNodeInfo;
		szNoDelMonster.Format(_T("%-u"), uNoDelMonster);
		szNoDelMoninfo.Format(_T("\t%-u"), uOffset);
		pMonster->Monster2String(szNodeMonster);
		pMonInfo->MonsterInfo2String(szNodeInfo);
		szDelLog += szNoDelMonster + szNodeMonster + szNoDelMoninfo + szNodeInfo;

		vecMonInfo.erase(itr);
		char strFile[32];
		sprintf(strFile, _T("%u.monster"), uRegionNo);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		pWnd->GetUpdateList().insert(strFile);
		CUniEditorDlg::GetLog()->AddLog(szDelLog);
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szDelLog);
		((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
		DeleteItem(hItem);
		CTextLog::WriteLog( szDelLog);
	}

	return TRUE;
}
BOOL CRegionTree::DelItemRoadsPoint(HTREEITEM hItem,int m,int n, CRegion *pRegion)
{
	MAPROADS	&maproads= pRegion->GetRoads();
	
	unsigned	uRegionNo = pRegion->GetRegionNo();
	MAPROADSITR itr = maproads.find(n);
	VECROADPOINT &vecroadspoint=(*itr->second).GetRoadsPoints();
	HTREEITEM hItem1=GetParentItem(hItem);
	hItem1=GetParentItem(hItem1);
	CString str=GetItemText(hItem1);
	if (str=="·��")
	{
		char strLog[256];
		sprintf(strLog,"X:%d,Y:%d",(vecroadspoint.begin()+m)->nx,(vecroadspoint.begin()+m)->ny);

		vecroadspoint.erase(vecroadspoint.begin()+m);

		CString szAddLog="ɾ����·����:";
		CUniEditorDlg::GetLog()->AddLog(szAddLog+strLog);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szAddLog+strLog);
		DeleteItem(hItem);
		CTextLog::WriteLog( szAddLog+strLog);
	}
	
	
	return TRUE;
}
BOOL CRegionTree::DelItemRoads(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion)
{
	MAPROADS &maproads=pRegion->GetRoads();
	CString strroadsid = GetItemText(hItem);
	MAPROADSITR itr=maproads.find(atoi(strroadsid));
	char strLog[256];
	sprintf(strLog,"  %d ",uOffset);
	HTREEITEM hItem1=GetParentItem(hItem);

	CString str=GetItemText(hItem1);
	if(itr!=maproads.end()&&str=="·��")
	{
		delete itr->second;
		maproads.erase(itr);
		DeleteItem(hItem);
		CString szAddLog="ɾ����·��:";
		CUniEditorDlg::GetLog()->AddLog(szAddLog+strLog);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szAddLog+strLog);
		CTextLog::WriteLog( szAddLog+strLog);
	}

	return TRUE;
}
// ɾ���ɼ�����
BOOL CRegionTree::DelItemCollection(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion)
{
	VECCOLLECTION		&vecCollection = pRegion->GetCollection();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	VECCOLLECTIONITR itr = find(vecCollection.begin(), vecCollection.end(), vecCollection[uOffset]);

	if(itr != vecCollection.end())
	{
		CCollection		*pCollection = vecCollection[uOffset];
		CString		szDelLog = _T("ɾ����� <�ɼ���>\t\t\t�ӳ��� ") + pRegion->GetRegionName() + _T("\n\t");
		CString		szNoDel, szNodeInfo;
		szNoDel.Format(_T("%-u\t"), uOffset);
		pCollection->Collection2String(szNodeInfo);
		szDelLog += szNoDel + szNodeInfo;

		vecCollection.erase(itr);
		char strFile[32];
		sprintf(strFile, _T("%u.collection"), uRegionNo);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		pWnd->GetUpdateList().insert(strFile);
		CUniEditorDlg::GetLog()->AddLog(szDelLog);
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szDelLog);
		((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
		DeleteItem(hItem);
		CTextLog::WriteLog(szDelLog);
	}
	return TRUE;
}
BOOL CRegionTree::DelItemSignPos(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion)
{
	VECSIGNPOS	&vecSignPos= pRegion->GetSignPos();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	VECSIGNPOSITR itr = find(vecSignPos.begin(), vecSignPos.end(), vecSignPos[uOffset]);
	HTREEITEM hItem1=GetParentItem(hItem);

	CString str=GetItemText(hItem1);
	if(itr != vecSignPos.end()&&str=="·���")
	{
		CSignPos *pSignPos = vecSignPos[uOffset];
		CString		szDelLog = _T("ɾ��·��� <SignPos>\t\t\t�ӳ��� ") + pRegion->GetRegionName() + _T("\n\t");
		CString		szNoDel, szNodeInfo;
		szNoDel.Format(_T("%-u\t"), uOffset);
	
		szDelLog += szNoDel + szNodeInfo;

		vecSignPos.erase(itr);
		char strFile[32];
		sprintf(strFile, _T("%u.xml"), uRegionNo);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		pWnd->GetUpdateList().insert(strFile);
		CUniEditorDlg::GetLog()->AddLog(szDelLog);
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szDelLog);
		((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
		DeleteItem(hItem);
		CTextLog::WriteLog(szDelLog);
	}
	return TRUE;
}
//ɾ���ɼ�����ϸ��Ϣ�ڵ�
BOOL CRegionTree::DelItemCollInfo(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion)
{
	unsigned	uNoDelCollection(GetOffset(GetParentItem(hItem)));
	VECCOLLECTION		&vecCollection = pRegion->GetCollection();
	CCollection	*pCollection =  vecCollection[uNoDelCollection];
	VECCOLLINFO	&vecCollInfo = pCollection->GetCollectionInfo();
	unsigned	uRegionNo = pRegion->GetRegionNo();
	
	VECCOLLINFOITR itr = find(vecCollInfo.begin(), vecCollInfo.end(), vecCollInfo[uOffset]);

	if(itr != vecCollInfo.end())
	{
		CCollectionInfo *pCollInfo = vecCollInfo[uOffset];
		CString		szDelLog = _T("ɾ����� <MonsterInfo>\t�ӳ��� ") + pRegion->GetRegionName() + _T("\n\t");
		CString		szNoDelMonster, szNoDelMoninfo, szNodeMonster, szNodeInfo;
		szNoDelMonster.Format(_T("%-u"), uNoDelCollection);
		szNoDelMoninfo.Format(_T("\t%-u"), uOffset);
		pCollection->Collection2String(szNodeMonster);
		pCollInfo->CollInfo2String(szNodeInfo);
		szDelLog += szNoDelMonster + szNodeMonster + szNoDelMoninfo + szNodeInfo;

		vecCollInfo.erase(itr);
		char strFile[32];
		sprintf(strFile, _T("%u.collection"), uRegionNo);
		CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
		pWnd->GetUpdateList().insert(strFile);
		CUniEditorDlg::GetLog()->AddLog(szDelLog);
		CListLog *pListLog = (CListLog *)(pWnd->GetDlgItem(IDC_LIST_LOG));
		pListLog->AddLog(LOG_DEL, szDelLog);
		((CButton *)(pWnd->GetDlgItem(IDC_BTN_SAVE)))->ShowWindow(SW_SHOW);
		DeleteItem(hItem);
		CTextLog::WriteLog(szDelLog);
	}
	return TRUE;
}
// ���ӽ��
BOOL CRegionTree::AddNode(HTREEITEM hItem, unsigned uLayer, unsigned uOffset, CRegion *pRegion)
{
	switch(uLayer)
	{
	case ITEM_LAYER_ROOT:
		{
			CDlgAddNode dlgAdd2All;
			INT_PTR nResponse = dlgAdd2All.DoModal();

			if (nResponse == IDOK)
			{
				LISTITEM listItem = dlgAdd2All.GetListItem();
				ITRITEM itrItem = listItem.begin();
				if(itrItem != listItem.end())
				{
					SelectItem(*itrItem);
				}
				// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
				//  �Ի���Ĵ���
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
				//  �Ի���Ĵ���

			}
			
			//dlgAdd2All.SendMessage(WM_CLOSE, NULL, NULL); 
			return TRUE;
			break;
		}


	case ITEM_LAYER_REGION:
		break;

	case ITEM_LAYER_TYPE:
		switch(uOffset)
		{
		case ITEM_OFFSET_MONSTER:
			{
				// �������<Monster>
				if(AddItemMonster(hItem, pRegion))
				{
				}

				break;
			}

		case ITEM_OFFSET_NPC:
			{
				
				if(AddItemCollection(hItem, pRegion))
				{
				}

				break;
			}
		case ITEM_OFFSET_SIGNPOS:
			{
		
				if(AddItemSignPos(hItem, pRegion))
				{
				}

				break;
			}
		case ITEM_OFFSET_ROADS:
			{
				if(AddItemRoads(hItem, pRegion))
				{
				}

				break;
			}
		}
		break;
	case ITEM_LAYER_EDIT:
		{
			unsigned uOffsetParent = GetOffset(GetParentItem(hItem));
			switch(uOffsetParent)
			{
			case ITEM_OFFSET_MONSTER:
				{
					// �������<MonsterInfo>
					if(AddItemMonInfo(hItem, pRegion))
					{
					}

					break;
				}

			case ITEM_OFFSET_NPC:
					if(AddItemCollInfo(hItem, pRegion))
					{
					}

					break;
				break;

			default:
				break;
			}
		}
		break;

	case ITEM_LAYER_MONSTER:
		break;

	default:
		break;
	}
	return TRUE;
}
// ɾ�����
BOOL CRegionTree::DelNode(HTREEITEM hItem, unsigned uLayer, unsigned uOffset, CRegion *pRegion)
{

	if(IDOK == (MessageBox(_T("ɾ����ǰѡ����?"), _T("��������"), MB_OKCANCEL)))
	{
		switch(uLayer)
		{
		case ITEM_LAYER_REGION:
		case ITEM_LAYER_TYPE:
			break;

		case ITEM_LAYER_EDIT:
			{
				unsigned uOffsetParent = GetOffset(GetParentItem(hItem));
				switch(uOffsetParent)
				{
				case ITEM_OFFSET_MONSTER:
					if (DelItemMonster(hItem, uOffset, pRegion))
					{
					}
					break;

				case ITEM_OFFSET_NPC:
					if (DelItemCollection(hItem, uOffset, pRegion))
					{
					}
					break;
					
				case ITEM_OFFSET_SIGNPOS:
					if (DelItemSignPos(hItem, uOffset, pRegion))
					{
					}
				case ITEM_OFFSET_ROADS:
					{
						HTREEITEM hItem1;
						hItem1=GetSelectedItem();
						CString str2=GetItemText(hItem1);
						
					
						if (DelItemRoads(hItem, atoi(str2), pRegion))
						{
						}
					}
					break;
				}
				break;
			}

		case ITEM_LAYER_MONSTER:
			{
				HTREEITEM hItem1;
				hItem1=GetSelectedItem();
				CString str2=GetItemText(hItem1);
				hItem1=GetParentItem(hItem1);
				CString str1=GetItemText(hItem1);
				hItem1=GetParentItem(hItem1);
				CString str=GetItemText(hItem1);
				if("�ɼ���"==str)
					DelItemCollInfo(hItem,uOffset,pRegion);//DelItemCollInfo
				else if("����"==str)
					DelItemMonInfo(hItem, uOffset, pRegion);
				else if ("·��"==str)
					DelItemRoadsPoint(hItem,m_uOffset,atoi(str1),pRegion);
				break;
			}
		default:
			break;
		}
	}

	return TRUE;
}

// ��Ϣ��Ӧ���� : �Ҽ��������οؼ�
void CRegionTree::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menuPopup;
	BOOL b = menuPopup.LoadMenu(IDR_MENU1);

	UINT uFlag;
	CPoint point;

	GetCursorPos(&point);
	ScreenToClient(&point);
	m_hRCItem = HitTest(point, &uFlag);
	ClientToScreen(&point); 

	if(m_hRCItem!=NULL && ((TVHT_ONITEM|TVHT_ONITEMRIGHT)&uFlag))
	{
		m_uRCLayer = GetLayer(m_hRCItem);
		m_uRCOffset = GetOffset(m_hRCItem);
		m_uRCNo = GetItemData(m_hRCItem);

		if(m_mapTree.empty())
		{
			m_pRCRegion = NULL;
		}
		
		else
		{
			ITRTREE itr = m_mapTree.find(m_uRCNo);
			if(itr != m_mapTree.end())
			{
				m_pRCRegion = itr->second;
			}
			else
			{
				m_pRCRegion = NULL;
			}
		}

		if(m_uRCLayer != ITEM_LAYER_REGION)
		{
			menuPopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
	}

	*pResult = 0;
}

// ��Ϣ��Ӧ���� : ɾ��
void CRegionTree::OnPopMenuDel()
{
	// TODO: �ڴ���������������
	DelNode(m_hRCItem, m_uRCLayer, m_uRCOffset, m_pRCRegion);
}

// ��Ϣ��Ӧ���� : ���
void CRegionTree::OnPopMenuAdd()
{
	// TODO: �ڴ���������������
	AddNode(m_hRCItem, m_uRCLayer, m_uRCOffset, m_pRCRegion);
}

// ��Ϣ��Ӧ���� : ���ı�
void CRegionTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem1, hItem2;
	ITRTREE itrTree;

	m_hItem = GetSelectedItem();
	Expand(m_hItem, TVE_EXPAND);

	hItem1 = hItem2 = m_hItem;
	m_uNo = GetItemData(m_hItem);
	itrTree = m_mapTree.find(m_uNo);
	m_uLayer = GetLayer(m_hItem);
	CString str=GetItemText(m_hItem);
	m_uOffset = GetOffset(m_hItem);

	switch(m_uLayer)
	{
	case ITEM_LAYER_REGION:
		// ��һ�ζ�ȡ�����ļ�
		if(m_uNo > ITEM_REGION_UNEXISTED && itrTree == m_mapTree.end())
		{
			ReadData(m_hItem);
			m_uNo = GetItemData(m_hItem);
		}
		break;

	case ITEM_LAYER_TYPE:
		break;

	case ITEM_LAYER_EDIT:
		break;

	case ITEM_LAYER_MONSTER:
		break;
	case ITEM_LAYER_COLLECTION:
		break;
	case ITEM_LAYER_SIGNPOS:
		break;
	default:
		break;
	}

	((CUniEditorDlg *)GetParent())->ItemChanged(m_uNo);
	::PostMessageA(GetParent()->GetSafeHwnd(), WM_PAINT, 0, 0);
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////
void CRegionTree::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

//////////////////////////////////////////////////////////////////////
void CRegionTree::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState(hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD);
}

//////////////////////////////////////////////////////////////////////
void CRegionTree::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if(!m_mapColorFont.Lookup(hItem, cf))
		cf.logfont.lfFaceName[0] = '\0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
}

//////////////////////////////////////////////////////////////////////
BOOL CRegionTree::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if(!m_mapColorFont.Lookup(hItem, cf))
		return FALSE;
	if(cf.logfont.lfFaceName[0] == '\0') 
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;

}

//////////////////////////////////////////////////////////////////////
BOOL CRegionTree::GetItemBold(HTREEITEM hItem)
{
	return GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD;
}

//////////////////////////////////////////////////////////////////////
COLORREF CRegionTree::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if(!m_mapColorFont.Lookup(hItem, cf))
		return (COLORREF) - 1;
	return cf.color;

}

//////////////////////////////////////////////////////////////////////
void CRegionTree::OnPaint() 
{
	CPaintDC dc(this);

	// Create a memory DC compatible with the paint DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	CBitmap *pOldBitmap = memDC.SelectObject( &bitmap );

	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );
	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// First let the control do its default drawing.
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	HTREEITEM hItem = GetFirstVisibleItem();

	int iItemCount = GetVisibleCount() + 1;
	while(hItem && iItemCount--)
	{		
		CRect rect;

		// Do not meddle with selected items or drop highlighted items
		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
		Color_Font cf;

		//if ( !(GetTreeCtrl().GetItemState( hItem, selflag ) & selflag ) 
		//	&& m_mapColorFont.Lookup( hItem, cf ))

		if ((GetItemState(hItem, selflag) & selflag) 
			&& ::GetFocus() == m_hWnd)
			;
		else if (m_mapColorFont.Lookup(hItem, cf))
		{
			CFont *pFontDC;
			CFont fontDC;
			LOGFONT logfont;

			if(cf.logfont.lfFaceName[0] != '\0') 
				logfont = cf.logfont;
			else {
				// No font specified, so use window font
				CFont *pFont = GetFont();
				pFont->GetLogFont( &logfont );
			}

			if(GetItemBold(hItem))
				logfont.lfWeight = 700;

			fontDC.CreateFontIndirect(&logfont);
			pFontDC = memDC.SelectObject(&fontDC );

			if(cf.color != (COLORREF) - 1)
				memDC.SetTextColor(cf.color);
			else
				memDC.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));


			CString sItem = GetItemText(hItem);

			GetItemRect(hItem, &rect, TRUE);
			memDC.SetBkColor( GetSysColor(COLOR_WINDOW));
			memDC.TextOut(rect.left + 2, rect.top + 1, sItem);

			memDC.SelectObject(pFontDC);
		}
		hItem = GetNextVisibleItem(hItem);
	}


	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
		rcClip.left, rcClip.top, SRCCOPY);
	memDC.SelectObject(pOldBitmap);

	memDC.DeleteDC();
}
void CRegionTree::DlgADD(CRect *rect)
{
	//SHIFT+�����ӽ��
	HTREEITEM dlgItem;
	CString str;
	ITRTREE itr;
	dlgItem=GetSelectedItem();
	switch(m_uLayer)
	{
		case ITEM_LAYER_REGION:
			HTREEITEM childItem;
			this->Expand(dlgItem,TVE_TOGGLE);
			childItem=GetNextItem(dlgItem,TVGN_CHILD);
			str=GetItemText(childItem);
			this->SelectItem(childItem);
			this->Expand(childItem,TVE_EXPAND);
			m_uRCLayer=ITEM_LAYER_REGION+1;
			m_hRCItem=childItem;
			m_uRCNo=GetItemData(dlgItem);
			itr = m_mapTree.find(m_uRCNo);
			if(itr != m_mapTree.end())
			{
				m_pRCRegion = itr->second;
			}
			else
			{
				return;
			}
			
			AddItemMonster(m_hRCItem,m_pRCRegion,rect);
			break;
		case ITEM_LAYER_TYPE:
			childItem=GetSelectedItem();
			str=GetItemText(childItem);
			this->SelectItem(childItem);
			this->Expand(childItem,TVE_EXPAND);
			m_uRCLayer=ITEM_LAYER_REGION+1;
			m_hRCItem=childItem;
			m_uRCNo=GetItemData(dlgItem);
			itr = m_mapTree.find(m_uRCNo);
			if(itr != m_mapTree.end())
			{
				m_pRCRegion = itr->second;
			}
			else
			{
				return;
			}
			if("����"==str)
				AddItemMonster(m_hRCItem,m_pRCRegion,rect);
			if("�ɼ���"==str)
				AddItemCollection(m_hRCItem,m_pRCRegion,rect);
			if("·���"==str)
			{
				AddItemSignPos(m_hRCItem,m_pRCRegion,rect);
			}
			/*if("·��"==str)
			{
				AddItemRoadsPoint(m_hRCItem,m_pRCRegion,rect);
			}
			break;*/
		case ITEM_LAYER_EDIT:
			{
				childItem=GetSelectedItem();
				HTREEITEM hItem1=GetParentItem(childItem);
				CString str1=GetItemText(hItem1);
				str=GetItemText(childItem);
				this->SelectItem(childItem);
				this->Expand(childItem,TVE_EXPAND);
				m_uRCLayer=ITEM_LAYER_REGION+1;
				m_hRCItem=childItem;
				m_uRCNo=GetItemData(dlgItem);
				itr = m_mapTree.find(m_uRCNo);
				if(itr != m_mapTree.end())
				{
					m_pRCRegion = itr->second;
				}
				else
				{
					return;
				}
				if("·��"==str1)
				AddItemRoadsPoint(m_hRCItem,m_pRCRegion,atoi(str),rect);
			}
			break;
		case ITEM_LAYER_MONSTER:
			break;
	}
}
BOOL CRegionTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CRegion *pRegion = NULL;
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_ADD:
		case VK_INSERT:
			if(ITEM_LAYER_ROOT == m_uLayer)
			{
				AddNode(m_hItem, m_uLayer, m_uOffset, NULL);
			}

			else if(!m_mapTree.empty())
			{
				ITRTREE itr = m_mapTree.find(m_uNo);
				if(itr != m_mapTree.end())
				{
					pRegion = itr->second;
					AddNode(m_hItem, m_uLayer, m_uOffset, pRegion);
				}
			}
			return TRUE;
			break;

		case VK_SUBTRACT:
		case VK_DELETE:
			if(!m_mapTree.empty())
			{
				ITRTREE itr = m_mapTree.find(m_uNo);
				if(itr != m_mapTree.end())
				{
					pRegion = itr->second;
					DelNode(m_hItem, m_uLayer, m_uOffset, pRegion);
				}
			}
			return TRUE;
			break;

		case VK_TAB:
			{
				CUniEditorDlg *pWnd = (CUniEditorDlg *)GetParent();
				CDialog *pdlgVisiable = NULL;

				if(pWnd->m_dlgMonster.IsWindowVisible())
				{
					pdlgVisiable = &(pWnd->m_dlgMonster);
				}

				else if(pWnd->m_dlgMoninfo.IsWindowVisible())
				{
					pdlgVisiable = &(pWnd->m_dlgMoninfo);
				}

				else if(pWnd->m_dlgCollection.IsWindowVisible())
				{
					pdlgVisiable = &(pWnd->m_dlgCollection);
				}
				else if(pWnd->m_dlgSignPos.IsWindowVisible())
				{
					pdlgVisiable = &(pWnd->m_dlgSignPos);
				}
				if(pdlgVisiable != NULL)
				{
					pdlgVisiable->SetFocus();
				}

				return TRUE;
			}
			break;

		default:
		    break;
		}
		break;

	default:
		break;
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}
