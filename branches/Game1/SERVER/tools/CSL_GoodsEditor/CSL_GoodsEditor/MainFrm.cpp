#include <iostream> 
#include <sstream> 
#include <strstream>

#include "stdafx.h"
#include "CSL_GoodsEditor.h"

#include "MainFrm.h"
#include "GoodAttrPage.h"
#include "AllGoodsPage.h"
#include "AllGoodsTree.h"
#include "ContainerPage.h"
#include "Search.h"
#include ".\mainfrm.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



int GoodID=0;
int MaxID=0;
vector<tagGoodAttr> vecGoodAttr;


// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//ON_COMMAND(ID_FILE_SAVE, OnFileSave)	
	ON_UPDATE_COMMAND_UI(ID_NEW, OnUpdateNew)
	ON_UPDATE_COMMAND_UI(ID_SAVE, OnUpdateSave)
	ON_COMMAND(ID_NEW, OnNew)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_CREATEOK, OnCreateok)
	ON_UPDATE_COMMAND_UI(ID_CREATEOK, OnUpdateCreateok)
	ON_UPDATE_COMMAND_UI(ID_EDITOK, OnUpdateEditok)
	ON_COMMAND(ID_32783, On32783)
	ON_WM_CLOSE()
	ON_COMMAND(ID_COUNT_EQUIP, OnCountEquip)
	ON_COMMAND(ID_COUNT_SHOW, OnCountShow)
	ON_COMMAND(ID_COUNT_RANDOM, OnCountRandom)
	ON_COMMAND(ID_COUNT_RANDOMNUM, OnCountRandomnum)
	ON_COMMAND(ID_COUNT_ADD20, OnCountAdd20)
	ON_COMMAND(ID_ADD_TYPEEX, OnAddTypeex)
	ON_COMMAND(ID_ConvertE, &CMainFrame::OnConvertE)
	ON_COMMAND(ID_CheckDataToEduce, &CMainFrame::OnCheckDataToEduce)
	ON_COMMAND(ID_CheckDataToEduceE, &CMainFrame::OnCheckDataToEduceE)
	ON_COMMAND(ID_AddAttrBatAdd, &CMainFrame::OnAddAttrByBatAdd)
	ON_COMMAND(ID_AddArrtBatDel, &CMainFrame::OnAddArrtBatDel)
	ON_COMMAND(ID_EduceOneAttribute, &CMainFrame::OnEduceOneAttribute)
	ON_COMMAND(ID_CalculateAddAttrValue2, &CMainFrame::OnCalculateAddAttrValue2)
	ON_COMMAND(ID_SEARCH, &CMainFrame::OnSearch)
END_MESSAGE_MAP()

// CMainFrame ����/����
CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������	
	//LoadGoods();	

	LoadGoodsB();
}

CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
								CCreateContext* pContext)
{	
	/*if(!m_wndSplitter.CreateStatic(this,1,2)|| 
	!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CAllGoodsTree),CSize(200,0),pContext) ||
	!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CGoodAttrPage),CSize(0,0),pContext))
	return FALSE;*/	

	m_wndSplitter.CreateStatic(this,1,2);	
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CAllGoodsTree),CSize(200,0),pContext);
	//m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CGoodAttrPage),CSize(0,0),pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CContainerPage),CSize(0,0),pContext);
	SetTitle("��â��Ʒ�༭");
	//m_wndGoodAttr.Create("FORMVIEW","",WS_CHILD | WS_VISIBLE,CRect(20, 20, 20, 20), this, 1234,NULL);	
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ	
	return TRUE;
}


// CMainFrame ���
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
// CMainFrame ��Ϣ�������

void CMainFrame::LoadGoodsB()
{
	FILE* fp;
	fp = fopen("goodslist.dat","rb");
	if( fp == NULL )
	{
		char str[256];
		sprintf_s(str, _T("file '%s' can't found!"), _T("goodslist.dat"));
		MessageBox(str,"error");
		return;
	}
	// �ļ�ͷ�Ͱ汾
	char szHeader[6];
	memset(szHeader, 0, 6);
	fread(szHeader, 5, 1, fp);
	if( strcmp(szHeader, "GOODS") != 0 )
	{
		char str[256];
		sprintf_s(str,_T("file '%s' is not goodslist file!"), "goodslist.dat");
		MessageBox(str, "error");		
	}
	long lVersion = 0;
	fread(&lVersion, sizeof(long), 1, fp);
	// ��������	

	//��Ʒ����
	long lNum = 0;
	fread(&lNum, sizeof(long), 1, fp);
	for(int i=0; i<lNum; i++)
	{
		long lStrSize=0;
		char *pStr;
		tagGoodAttr stGoods;
		stGoods.dwSilverValue=0;
		stGoods.dwWeaponActType=0;

		fread(&stGoods.dwIndex, sizeof(stGoods.dwIndex), 1, fp);		//	���

		//	ԭʼ��   < 20
		fread(&lStrSize,sizeof(long),1,fp);
		pStr=new char[lStrSize+1];
		fread(pStr,lStrSize,1,fp);
		pStr[lStrSize]='\0';
		stGoods.strOrigName=pStr;
		delete [] pStr;

		//	�������� < 20
		fread(&lStrSize,sizeof(long),1,fp);
		pStr=new char[lStrSize+1];
		fread(pStr,lStrSize,1,fp);
		pStr[lStrSize]='\0';
		stGoods.strName=pStr;
		delete [] pStr;

		fread(&stGoods.bSave, sizeof(stGoods.bSave), 1, fp);			//	�Ƿ���Ҫ�����ݿ�
		fread(&stGoods.dwValue, sizeof(stGoods.dwValue), 1, fp);		//	�۸�
		fread(&stGoods.dwSilverValue, sizeof(stGoods.dwSilverValue), 1, fp);		//	�۸�
		//fread(&stGoods.lX_Size, sizeof(stGoods.lX_Size), 1, fp);		//	��ƷX�����С
		//fread(&stGoods.lY_Size, sizeof(stGoods.lY_Size), 1, fp);		//	��Ʒy�����С
		fread(&stGoods.dwType, sizeof(stGoods.dwType), 1, fp);			//	��Ʒ����
		//fread(&stGoods.dwWeight, sizeof(stGoods.dwWeight), 1, fp);		//	����

		// �ͻ���ר������
		fread(&stGoods.dwIconId, sizeof(stGoods.dwIconId), 1, fp);		//	����ͼ��ID
		fread(&stGoods.dwGroundId, sizeof(stGoods.dwGroundId), 1, fp);		//	��غ�ͼ��ID
		fread(&stGoods.dwEquipID, sizeof(stGoods.dwEquipID), 1, fp);		//	��װͼ��ID
		fread(&stGoods.dwSound, sizeof(stGoods.dwSound), 1, fp);		//	ʰȡ����
		fread(&stGoods.dwSoundID1, sizeof(stGoods.dwSoundID1), 1, fp);		//	�Ӷ�/���˺�����
		fread(&stGoods.dwSoundID2, sizeof(stGoods.dwSoundID2), 1, fp);		//	���⹥��/��������
		fread(&stGoods.bSoundSwitch, sizeof(stGoods.bSoundSwitch), 1, fp);	//	����ʱ�Ƿ����
		fread(&stGoods.dwWeaponActType, sizeof(stGoods.dwWeaponActType), 1, fp);//	������Ӧ�Ķ�������

		//	�������� < 64
		fread(&lStrSize,sizeof(long),1,fp);
		pStr=new char[lStrSize+1];
		fread(pStr,lStrSize,1,fp);
		pStr[lStrSize]='\0';
		stGoods.strContent=pStr;
		delete [] pStr;

		// ��չ����
		long lEffectNum = 0;
		fread(&lEffectNum, sizeof(long), 1, fp);
		for(int j=0; j<lEffectNum; j++)
		{
			//fread(&stEffect,sizeof(tagAddAttr)-sizeof(vector<tagModify>), 1, fp);
			tagAddAttr stEffect;
			fread(&(stEffect.wType),sizeof(WORD),1,fp);
			fread(&(stEffect.bEnable),sizeof(bool),1,fp);
			fread(&(stEffect.bHide),sizeof(bool),1,fp);
			fread(&(stEffect.lValue1),sizeof(DWORD),1,fp);
			fread(&(stEffect.lValue2),sizeof(DWORD),1,fp);
			//fread(&(stEffect.wEnableOdds),sizeof(WORD),1,fp);
			stGoods.vecAddAttr.push_back(stEffect);
		}

		// ��װ����
		lEffectNum = 0;
		fread(&lEffectNum, sizeof(long), 1, fp);
		for(int j=0; j<lEffectNum; j++)
		{
			//fread(&stEffect,sizeof(tagAddAttr)-sizeof(vector<tagModify>), 1, fp);
			tagSuitAttr stEffect;
			fread(&(stEffect.wType),sizeof(WORD),1,fp);			
			fread(&(stEffect.lValue1),sizeof(DWORD),1,fp);
			fread(&(stEffect.lValue2),sizeof(DWORD),1,fp);
			fread(&(stEffect.wSuitNum),sizeof(WORD),1,fp);
			stGoods.vecSuitAttr.push_back(stEffect);
		}

		//add by wang-xx 2009-6-30 begin
		//��������
		
		lEffectNum = 0;
		fread(&lEffectNum, sizeof(long), 1, fp);
		for(int j=0; j<lEffectNum; j++)
		{
			tagArouseAttr stEffect;
			fread(&(stEffect.wType),sizeof(WORD),1,fp);			
			fread(&(stEffect.lValue1),sizeof(DWORD),1,fp);
			fread(&(stEffect.lValue2),sizeof(DWORD),1,fp);
			fread(&(stEffect.wArouseCnt),sizeof(WORD),1,fp);
			stGoods.vecArouseAttr.push_back(stEffect);
		}
		//add by wang-xx 2009-6-30 end

		vecGoodAttr.push_back(stGoods);
	}
	fclose(fp);	
}

// ���浽�ļ�
void CMainFrame::SaveGoodsB()
{
	FILE* fp;
	fp = fopen(_T("goodslist.dat"), "wb");
	if( fp == NULL )
	{
		char str[256];
		sprintf_s(str, _T("file '%s' can't write!"), "goodslist.dat");
		MessageBox(str, "error");	
		return;
	}
	// �ļ�ͷ�Ͱ汾
	fwrite("GOODS", 5, 1, fp);
	long lVersion = 1;
	fwrite(&lVersion, sizeof(long), 1, fp);
	// ��������

	//��Ʒ����
	long lNum = (long)vecGoodAttr.size();
	fwrite(&lNum, sizeof(long), 1, fp);
	for(vector<tagGoodAttr>::iterator it=vecGoodAttr.begin(); it!=vecGoodAttr.end(); it++)
	{
		tagGoodAttr *pstGoodAttr = &(*it);

		fwrite(&pstGoodAttr->dwIndex, sizeof(pstGoodAttr->dwIndex), 1, fp);		//	���

		lNum=long(pstGoodAttr->strOrigName.size());
		fwrite(&lNum,sizeof(long),1,fp);
		fwrite(pstGoodAttr->strOrigName.c_str(),lNum,1,fp);//ԭʼ��

		lNum=long(pstGoodAttr->strName.size());
		fwrite(&lNum,sizeof(long),1,fp);
		fwrite(pstGoodAttr->strName.c_str(),lNum,1,fp);//������

		fwrite(&pstGoodAttr->bSave, sizeof(pstGoodAttr->bSave), 1, fp);			//	�Ƿ���Ҫ�����ݿ�
		fwrite(&pstGoodAttr->dwValue, sizeof(pstGoodAttr->dwValue), 1, fp);		//	�۸�
		fwrite(&pstGoodAttr->dwSilverValue, sizeof(pstGoodAttr->dwSilverValue), 1, fp);		//	�۸�

		//fwrite(&pstGoodAttr->lX_Size, sizeof(pstGoodAttr->lX_Size), 1, fp);		//	��ƷX�����С
		//fwrite(&pstGoodAttr->lY_Size, sizeof(pstGoodAttr->lY_Size), 1, fp);		//	��Ʒy�����С
		fwrite(&pstGoodAttr->dwType, sizeof(pstGoodAttr->dwType), 1, fp);			//	��Ʒ����
		//fwrite(&pstGoodAttr->dwWeight, sizeof(pstGoodAttr->dwWeight), 1, fp);		//	����

		// �ͻ���ר������
		fwrite(&pstGoodAttr->dwIconId, sizeof(pstGoodAttr->dwIconId), 1, fp);		//	����ͼ��ID
		fwrite(&pstGoodAttr->dwGroundId, sizeof(pstGoodAttr->dwGroundId), 1, fp);		//	��غ�ͼ��ID
		fwrite(&pstGoodAttr->dwEquipID, sizeof(pstGoodAttr->dwEquipID), 1, fp);		//	��װͼ��ID
		fwrite(&pstGoodAttr->dwSound, sizeof(pstGoodAttr->dwSound), 1, fp);		//	ʰȡ����
		fwrite(&pstGoodAttr->dwSoundID1, sizeof(pstGoodAttr->dwSoundID1), 1, fp);		//	�Ӷ�/���˺�����
		fwrite(&pstGoodAttr->dwSoundID2, sizeof(pstGoodAttr->dwSoundID2), 1, fp);		//	���⹥��/��������
		fwrite(&pstGoodAttr->bSoundSwitch, sizeof(pstGoodAttr->bSoundSwitch), 1, fp);	//	����ʱ�Ƿ����
		fwrite(&pstGoodAttr->dwWeaponActType, sizeof(pstGoodAttr->dwWeaponActType), 1, fp);//	������Ӧ�Ķ�������

		lNum=long(pstGoodAttr->strContent.size());
		fwrite(&lNum,sizeof(long),1,fp);
		fwrite(pstGoodAttr->strContent.c_str(),lNum,1,fp);//����

		// ��չ����
		lNum = (long)pstGoodAttr->vecAddAttr.size();
		fwrite(&lNum, sizeof(long), 1, fp);
		for(int i=0; i<lNum; i++)
		{
			fwrite(&(pstGoodAttr->vecAddAttr[i].wType),sizeof(WORD),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].bEnable),sizeof(bool),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].bHide),sizeof(bool),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].lValue1),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].lValue2),sizeof(DWORD),1,fp);
			//fwrite(&(pstGoodAttr->vecAddAttr[i].wEnableOdds),sizeof(WORD),1,fp);
		}

		//��װ����
		lNum = (long)pstGoodAttr->vecSuitAttr.size();
		fwrite(&lNum, sizeof(long), 1, fp);
		for(int i=0;i<lNum;i++)
		{
			fwrite(&(pstGoodAttr->vecSuitAttr[i].wType),sizeof(WORD),1,fp);			
			fwrite(&(pstGoodAttr->vecSuitAttr[i].lValue1),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecSuitAttr[i].lValue2),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecSuitAttr[i].wSuitNum),sizeof(WORD),1,fp);
		}

		//add by wang-xx 2009-6-30 begin
		//��������
		lNum = (long)pstGoodAttr->vecArouseAttr.size();
		fwrite(&lNum, sizeof(long), 1, fp);
		for(int i=0;i<lNum;i++)
		{
			fwrite(&(pstGoodAttr->vecArouseAttr[i].wType),sizeof(WORD),1,fp);			
			fwrite(&(pstGoodAttr->vecArouseAttr[i].lValue1),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecArouseAttr[i].lValue2),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecArouseAttr[i].wArouseCnt),sizeof(WORD),1,fp);
		}
		//add by wang-xx 2009-6-30 end
	}
	fclose(fp);	
}

// ���浽�ļ�client
void CMainFrame::SaveGoodsClient()
{
	FILE* fp;
	fp = fopen(_T("goods.dat"), "wb");
	if( fp == NULL )
	{
		char str[256];
		sprintf_s(str, _T("file '%s' can't write!"), _T("goods.dat"));
		MessageBox(str, "error");	
		return;
	}
	// �ļ�ͷ�Ͱ汾
	fwrite("GOODS", 5, 1, fp);
	long lVersion = 1;
	fwrite(&lVersion, sizeof(long), 1, fp);
	// ��������

	//��Ʒ����
	long lNum = (long)vecGoodAttr.size();
	fwrite(&lNum, sizeof(long), 1, fp);
	for(vector<tagGoodAttr>::iterator it=vecGoodAttr.begin(); it!=vecGoodAttr.end(); it++)
	{
		tagGoodAttr *pstGoodAttr = &(*it);

		fwrite(&pstGoodAttr->dwIndex, sizeof(pstGoodAttr->dwIndex), 1, fp);		//	���

		lNum=long(pstGoodAttr->strOrigName.size());
		fwrite(&lNum,sizeof(long),1,fp);
		fwrite(pstGoodAttr->strOrigName.c_str(),lNum,1,fp);//ԭʼ��

		lNum=long(pstGoodAttr->strName.size());
		fwrite(&lNum,sizeof(long),1,fp);
		fwrite(pstGoodAttr->strName.c_str(),lNum,1,fp);//������

		// �ͻ���ר������
		fwrite(&pstGoodAttr->dwIconId, sizeof(pstGoodAttr->dwIconId), 1, fp);		//	����ͼ��ID
		fwrite(&pstGoodAttr->dwGroundId, sizeof(pstGoodAttr->dwGroundId), 1, fp);		//	��غ�ͼ��ID
		fwrite(&pstGoodAttr->dwEquipID, sizeof(pstGoodAttr->dwEquipID), 1, fp);		//	��װͼ��ID
		fwrite(&pstGoodAttr->dwSound, sizeof(pstGoodAttr->dwSound), 1, fp);		//	ʰȡ����
		fwrite(&pstGoodAttr->dwSoundID1, sizeof(pstGoodAttr->dwSoundID1), 1, fp);		//	�Ӷ�/���˺�����
		fwrite(&pstGoodAttr->dwSoundID2, sizeof(pstGoodAttr->dwSoundID2), 1, fp);		//	���⹥��/��������
		fwrite(&pstGoodAttr->bSoundSwitch, sizeof(pstGoodAttr->bSoundSwitch), 1, fp);	//	����ʱ�Ƿ����
		fwrite(&pstGoodAttr->dwWeaponActType, sizeof(pstGoodAttr->dwWeaponActType), 1, fp);//	������Ӧ�Ķ�������
		fwrite(&pstGoodAttr->dwType, sizeof(pstGoodAttr->dwType), 1, fp);			//	��Ʒ����
		fwrite(&pstGoodAttr->dwValue,sizeof(pstGoodAttr->dwValue),1,fp);
		fwrite(&pstGoodAttr->dwSilverValue,sizeof(pstGoodAttr->dwSilverValue),1,fp);
		//fwrite(&pstGoodAttr->dwWeight, sizeof(pstGoodAttr->dwWeight), 1, fp);		//	����

		lNum=long(pstGoodAttr->strContent.size());
		fwrite(&lNum,sizeof(long),1,fp);
		fwrite(pstGoodAttr->strContent.c_str(),lNum,1,fp);//����

		//������������
		lNum = (long)pstGoodAttr->vecAddAttr.size();
		fwrite(&lNum, sizeof(long), 1, fp);
		for(int i=0; i<lNum; i++)
		{
			fwrite(&(pstGoodAttr->vecAddAttr[i].wType),sizeof(WORD),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].bEnable),sizeof(bool),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].bHide),sizeof(bool),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].lValue1),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecAddAttr[i].lValue2),sizeof(DWORD),1,fp);
			//fwrite(&(pstGoodAttr->vecAddAttr[i].wEnableOdds),sizeof(WORD),1,fp);
		}

		//��װ����
		lNum = (long)pstGoodAttr->vecSuitAttr.size();
		fwrite(&lNum, sizeof(long), 1, fp);
		for(int i=0;i<lNum;i++)
		{
			fwrite(&(pstGoodAttr->vecSuitAttr[i].wType),sizeof(WORD),1,fp);			
			fwrite(&(pstGoodAttr->vecSuitAttr[i].lValue1),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecSuitAttr[i].lValue2),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecSuitAttr[i].wSuitNum),sizeof(WORD),1,fp);
		}

		//add by wang-xx 2009-6-30 begin
		//��������
		lNum = (long)pstGoodAttr->vecArouseAttr.size();
		fwrite(&lNum, sizeof(long), 1, fp);
		for(int i=0;i<lNum;i++)
		{
			fwrite(&(pstGoodAttr->vecArouseAttr[i].wType),sizeof(WORD),1,fp);			
			fwrite(&(pstGoodAttr->vecArouseAttr[i].lValue1),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecArouseAttr[i].lValue2),sizeof(DWORD),1,fp);
			fwrite(&(pstGoodAttr->vecArouseAttr[i].wArouseCnt),sizeof(WORD),1,fp);
		}
		//add by wang-xx 2009-6-30 end
	}
	fclose(fp);	
}

void CMainFrame::OnUpdateNew(CCmdUI *pCmdUI)
{
	CContainerPage *pContainer=(CContainerPage*)m_wndSplitter.GetPane(0,1);
	if(pContainer->m_wndGoodAttrPage.GetAddFlag())
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CMainFrame::OnUpdateSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(true);
}

void CMainFrame::OnNew()
{
	// TODO: �ڴ���������������
	CContainerPage *pContainer=(CContainerPage*)m_wndSplitter.GetPane(0,1);
	CAllGoodsTree *pGoodsTree=(CAllGoodsTree*)m_wndSplitter.GetPane(0,0);
	pContainer->m_wndGoodAttrPage.SetEditFlag(false);	
	pContainer->m_wndGoodAttrPage.ClearCtrls();
	pContainer->m_wndGoodAttrPage.ClearAttr();
	pContainer->m_wndAllGoodsPage.ShowWindow(SW_HIDE);
	pContainer->m_wndGoodAttrPage.ShowWindow(SW_NORMAL);	
	tagGoodAttr stGoodAttr;
	int nMax=0;
	vector<tagGoodAttr>::iterator iter;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	{
		if(nMax<(int)iter->dwIndex)
			nMax=iter->dwIndex;
	}
	nMax++;
	char strTem[10];
	itoa(nMax,strTem,10);
	stGoodAttr.dwIndex=nMax;
	char strOrig[25];
	sprintf_s(strOrig,_T("ԭʼ��%d"),stGoodAttr.dwIndex);
	stGoodAttr.strOrigName=strOrig;
	stGoodAttr.strName="δ����";
	stGoodAttr.bSave=false;
	stGoodAttr.dwEquipID=0;
	stGoodAttr.dwGroundId=0;
	stGoodAttr.dwValue=0;
	stGoodAttr.dwSilverValue=0;
	stGoodAttr.dwSound=0;
	stGoodAttr.dwType=0;
	stGoodAttr.dwIconId=0;
	stGoodAttr.dwSoundID1=0;
	stGoodAttr.dwSoundID2=0;
	stGoodAttr.bSoundSwitch=false;
	stGoodAttr.dwWeaponActType=0;
	//stGoodAttr.dwWeight=0;
	//stGoodAttr.lX_Size=0;
	//stGoodAttr.lY_Size=0;
	stGoodAttr.strContent="";
	vecGoodAttr.push_back(stGoodAttr);

	char strVal[500];   
	int nCurItem=pContainer->m_wndAllGoodsPage.m_cAllGoodsList.InsertItem(0xffff,"");
	int nCnt=pContainer->m_wndAllGoodsPage.m_cAllGoodsList.GetItemCount();
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItemData(nCurItem,nCnt);
	sprintf_s(strVal,_T("%d"),stGoodAttr.dwIndex);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,0,LVIF_TEXT,strVal,NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,1,LVIF_TEXT,strOrig,NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,2,LVIF_TEXT,"δ����",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,3,LVIF_TEXT,"��",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,4,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,5,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,6,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,7,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,8,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,9,LVIF_TEXT,"0",NULL,0,0,0);	
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,11,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,12,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,13,LVIF_TEXT,"0",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItem(nCurItem,14,LVIF_TEXT," ",NULL,0,0,0);
	pContainer->m_wndAllGoodsPage.m_cAllGoodsList.SetItemState(nCurItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);


	pContainer->m_wndGoodAttrPage.SetVecIndex((int)(vecGoodAttr.size()-1));
	pGoodsTree->m_cGoodsTree.Expand(pGoodsTree->m_hTreeRoot,TVE_EXPAND);
	HTREEITEM newItem;
	TV_INSERTSTRUCT stItem;
	stItem.hParent=pGoodsTree->m_cGoodsTree.GetRootItem();
	stItem.hInsertAfter=TVI_LAST;
	stItem.item.pszText=strOrig;
	stItem.item.mask=TVIF_TEXT| TVIF_PARAM;
	stItem.item.lParam=stGoodAttr.dwIndex;
	newItem=pGoodsTree->m_cGoodsTree.InsertItem(&stItem);
	pGoodsTree->SetNewItemFlag(true);
	pGoodsTree->m_cGoodsTree.SelectItem(newItem);
	pGoodsTree->SetNewItemFlag(false);
	pContainer->m_wndGoodAttrPage.m_cIDLab.SetWindowText(strTem);
}

void CMainFrame::OnSave()
{
	// TODO: �ڴ���������������
	SaveGoodsB();
	SaveGoodsClient();
	//	SaveGoods();
}

void CMainFrame::OnCreateok()
{
	// TODO: �ڴ���������������
	char strID[100],strName[500],strUIID[500],strGroundID[500],strCosplay[500],
		strPrice[500],/*strXSize[500],strYSize[500],strWeight[500],*/strSound[500],strDesc[5000];
	CContainerPage *pContainer=(CContainerPage*)m_wndSplitter.GetPane(0,1);	

	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwIndex=++GoodID;//
	sprintf_s(strID,_T("%d"),GoodID);
	pContainer->m_wndGoodAttrPage.m_cNameEdit.GetWindowText(strName,500);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.strName=strName;//1
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.bSave=pContainer->m_wndGoodAttrPage.m_cSaveDBCombo.GetCurSel()==1?true:false;//2
	pContainer->m_wndGoodAttrPage.m_cUIEdit.GetWindowText(strUIID,500);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwIconId=atoi(strUIID);//3
	pContainer->m_wndGoodAttrPage.m_cGroundEdit.GetWindowText(strGroundID,500);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwGroundId=atoi(strGroundID);//4
	pContainer->m_wndGoodAttrPage.m_cCosplayEdit.GetWindowText(strCosplay,500);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwEquipID=atoi(strCosplay);//5
	pContainer->m_wndGoodAttrPage.m_cPriceEdit.GetWindowText(strPrice,500);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwValue=atoi(strPrice);
	//pContainer->m_wndGoodAttrPage.m_cXSizeEdit.GetWindowText(strXSize,500);
	//pContainer->m_wndGoodAttrPage.m_tagGoodAttr.lX_Size=atoi(strXSize);
	//pContainer->m_wndGoodAttrPage.m_cYSizeEdit.GetWindowText(strYSize,500);
	//pContainer->m_wndGoodAttrPage.m_tagGoodAttr.lY_Size=atoi(strYSize);	
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwType=pContainer->m_wndGoodAttrPage.m_cGoodType.GetCurSel();
	pContainer->m_wndGoodAttrPage.m_cSoundEdit.GetWindowText(strSound,500);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwSound=atoi(strSound);
	//pContainer->m_wndGoodAttrPage.m_cWeightEdit.GetWindowText(strWeight,500);
	//pContainer->m_wndGoodAttrPage.m_tagGoodAttr.dwWeight=atoi(strWeight);
	pContainer->m_wndGoodAttrPage.m_cDescEdit.GetWindowText(strDesc,5000);
	pContainer->m_wndGoodAttrPage.m_tagGoodAttr.strContent=strDesc;
	vecGoodAttr.push_back(pContainer->m_wndGoodAttrPage.m_tagGoodAttr);	
	pContainer->m_wndGoodAttrPage.ShowWindow(SW_HIDE);
	pContainer->m_wndAllGoodsPage.ShowWindow(SW_NORMAL); 

}

void CMainFrame::OnUpdateCreateok(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CContainerPage *pContainer=(CContainerPage*)m_wndSplitter.GetPane(0,1);
	if(pContainer->m_wndGoodAttrPage.GetAddFlag())
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateEditok(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}

void CMainFrame::On32783()
{
	// TODO: �ڴ���������������
}

void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nRet=MessageBox("�뿪ǰҪ������?","",MB_YESNO);
	if(nRet==IDYES)
		SaveGoodsB();
	CFrameWnd::OnClose();
}

//��ͨװ����
//
//ԭʼ����ID���ɡ�����𡱡����ȼ���������ɫ���������͡�4������ɡ�
//
//ԭʼ���С��������1λ���ϵ���ĸ��ɡ�
//ԭʼ���С��ȼ�����2λ������ɣ�����10��ʮλ��0���
//ԭʼ���С���ɫ����1λ��ĸ��ɡ�
//ԭʼ���С����͡���2λ������ɣ�����10��ʮλ��0���
//
//����װ���Ĵ����Ϊ��S�����ȼ�10װ���ĵȼ�����Ϊ��01������ɫ2װ���ġ���ɫ��Ϊ��E����������͵�װ���ġ����͡�Ϊ��03������һ��10����2���Ľ���ԭʼ��Ϊ��S01E03����
//
//ID�С�����𡱡����ȼ���������ɫ���������͡�4���־�Ϊ2λ���֣�ÿ��װ����ID���ǳ��ȸպ�8λ����ֵ��
//
//
//ԭʼ����ID�Ķ�Ӧ����=ǰ��ԭʼ����=����ID��
//�����
//S = 10 �� K = 11 �� B = 12 �� C = 13 �� W = 14
//WA = 15 �� WH = 16 �� WG = 17 �� WB = 18
//HA = 19 �� HH = 20 �� HG = 21 �� HB = 22
//TA = 23 �� TH = 24 �� TG = 25 �� TB = 26
//AMU = 27 �� RING = 28
//�ȼ���
//ֱ��ת������ԭʼ���еȼ�05תΪID��Ϊ05
//��ɫ��
//B = 01 �� C = 02 �� D = 03 �� E = 04 �� F = 05 ��G = 06 ��H = 07 �� I = 08
//���ͣ�
//ֱ��ת����
//
//
//����60����2����������ԭʼ��Ϊ��AMU06I04����ת��ΪID��Ϊ��27060804����
//
//
//
//
//δ����װ����
//
//δ����װ��ԭʼ����ID���ɡ���λ��ʶ����������𡱡����ȼ���������ɫ��4������ɡ�
//
//ԭʼ����λ��ʶ��Ϊ��U����
//ԭʼ���С��������1λ��ĸ��ɡ�
//ԭʼ���С��ȼ�����2λ������ɣ�����10��ʮλ��0���
//ԭʼ���С���ɫ����1λ��ĸ��ɡ�
//ԭʼ�����Ⱦ�Ϊ5��
//
//ID��λ��ʶ��Ϊ1��
//ID�С�����𡱡����ȼ���������ɫ��3���־�Ϊ2λ���֣�ÿ��װ����ID���ǳ���7λ����ֵ��
//
//ԭʼ����ID�Ķ�Ӧ����=ǰ��ԭʼ����=����ID��
//��λ��ʶ��
//U = 1
//�����
//S = 10 �� K = 11 �� B = 12 �� C = 13 �� W = 14
//A = 15 �� H = 16 �� G = 17 �� X = 18
//J = 19 �� R = 20
//�ȼ���
//ֱ��ת������ԭʼ���еȼ�05תΪID��Ϊ05
//��ɫ��
//A = 01 �� B = 02 �� C = 03 ��D = 04	
//
//����δ������60����2������ԭʼ��Ϊ��UJ06D����ת��ΪID��Ϊ��1190604����

bool CountEquipLevelLimit(const char* strOrginName, tagAddAttr *pAttr)
{
	if (pAttr && pAttr->wType == 6)	// �ȼ�����
	{
		char strRet[256];
		memset(strRet,0,256);

		char strName[256];
		strcpy(strName, strOrginName);

		// δ����װ��
		if (strName[0] == 'U')
		{
			strRet[0] = '1';

			char TYPE[][2] = {
				'S', 10, 'K', 11, 'B', 12, 'C', 13, 'W', 14,
				'A', 15, 'H', 16, 'G', 17, 'X', 18,
				'J', 19, 'R', 20
			};

			// ����
			bool bVaild = false;
			for (int i=0; i<sizeof(TYPE)/2; i++)
			{
				if (strName[1] == TYPE[i][0])
				{
					sprintf(&strRet[1], _T("%02d"), TYPE[i][1]);
					bVaild = true;
					break;
				}
			}

			if (bVaild==false)	return false;	// �����Ϲ���

			// �ȼ�
			strRet[3] = strName[2];
			strRet[4] = strName[3];

			// ��ɫ
			bVaild = false;
			char COLOR[][2] = {
				'A', 01, 'B', 02, 'C', 03, 'D', 04
			};

			for (int i=0; i<sizeof(COLOR)/2; i++)
			{
				if (strName[4] == COLOR[i][0])
				{
					sprintf(&strRet[5], _T("%02d"), COLOR[i][1]);
					bVaild = true;
					break;
				}
			}

			if (bVaild==false)	return false;	// �����Ϲ���

			pAttr->lValue2 = atoi(strRet);
		}
		else	// ��ͨװ��
		{
			char *TYPE[] = {
				"S", "K", "B", "C", "W",
				"WA", "WH", "WG", "WB",
				"HA", "HH", "HG", "HB",
				"TA", "TH", "TG", "TB",
				"AMU", "RING"
			};
			long VALUE[] = {
				10, 11, 12, 13, 14,
				15, 16, 17, 18,
				19, 20, 21, 22,
				23, 24, 25, 26,
				27, 28
			};

			// Ѱ�ҵ�һ���ַ���
			char strType[256];
			memset(strType, 0, 256);
			int pos = 0;
			for (int i=0; i<lstrlen(strName); i++)
			{
				if (strName[i] >= '0' && strName[i] <= '9')
				{
					pos = i;
					break;
				}
				else
					strType[i] = strName[i];
			}

			// ����
			bool bVaild = false;
			for (int i=0; i<19; i++)
			{
				if ( strcmp(strType, TYPE[i]) == 0)
				{
					sprintf(&strRet[0], _T("%02d"), VALUE[i]);
					bVaild = true;
					break;
				}
			}

			if (bVaild==false)	return false;	// �����Ϲ���

			// �ȼ�
			strRet[2] = strName[pos++];
			strRet[3] = strName[pos++];

			// ��ɫ
			bVaild = false;
			char COLOR[][2] = {
				'S', 0, 'A', 01, 'B', 02, 'C', 03, 'D', 04, 'E', 05,
				'F', 06, 'G', 07, 'H', 8, 'I', 9
			};

			for (int i=0; i<sizeof(COLOR)/2; i++)
			{
				if (strName[pos] == COLOR[i][0])
				{
					sprintf(&strRet[4], _T("%02d"), COLOR[i][1]);
					bVaild = true;
					break;
				}
			}
			pos++;

			if (bVaild==false)	return false;	// �����Ϲ���

			// ����
			strRet[6] = strName[pos++];
			strRet[7] = strName[pos++];

			// �����Ϲ���
			if (strName[pos])
				return false;

			pAttr->lValue2 = atoi(strRet);
		}
		return true;
	}

	return false;
}

// ���¼�������װ���ĵȼ�����ֵ
void CMainFrame::OnCountEquip()
{/*
 vector<tagGoodAttr>::iterator iter;
 vector<tagAddAttr>::iterator iterAttr;
 vector<tagModify>::iterator iterModify;

 int nTem=0;
 for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
 {
 tagGoodAttr *pGoods = &(*iter);
 if (pGoods && pGoods->dwType >= 2 )	// װ����
 {
 for(iterAttr=iter->vecAddAttr.begin();iterAttr!=iter->vecAddAttr.end();iterAttr++)
 {
 tagAddAttr *pAttr = &(*iterAttr);
 CountEquipLevelLimit(pGoods->strOrigName.c_str(), pAttr);
 }
 }
 }*/
}

bool ReadTo(istream& stream, const char* name)
{
	string str;

	stream >> str;
	while (str != name)
	{
		if (stream.eof())
		{
			return false;
		}		
		stream >> str;
	}
	return true;
}

struct tagConvert
{
	//	string strSign;					//��ʶ�� #
	string strOrgName;				// ԭʼ��
	string strShowName;				// ��ʾ��
	string lGraphicID;				// ͼ��ID
	string lGroundID;				// ���ID
	string lEquipID;				// ��װID
	string lPrice;					// ���
	string lSilverPrice;			//���Ҽ۸�
	string strContent;				//	�������� < 64
	string lLevelRest;				//�ȼ�����
	string lRunScript;				//ִ�нű�
	string lColor;					//��ʾ��ɫ
	string lSound;					//ʰȡ����

	string lClass;					//����
	string lStudySkillID;			//	ѧϰ����id
};

struct tagConvertE 
{
	string	OrgName;				//ԭʼ��
	string	WeaponStrong;			//����ǿ��
	string	WeaponStringAdd;		//����ǿ�ȳɳ�
	string	MinAtk;					//��С������
	string	MaxAtk;					//��󹥻���
	string	MoGong;					//ħ������
	string	WuGongSockPro;			//��������һ����
	string	WuBaoJiDamage;			//�������˺�
	string	WuBaoJiFastness;		//����������
	string	MoGongSockPro;			//ħ������һ����
	string	MoBaoJiDamage;			//ħ�������˺�
	string	MoBaoJiFastness;		//ħ����������
	string	WuRecovery;				//�������
	string	MoRecovery;				//ħ������
	string	AsunderValue1;			//�ֽ��ֵ1
	string	GoodsChar;				//��ƷƷ��
	string	ForceAdd;				//����׷��
	string	MinJieAdd;				//����׷��
	string	TiZhiAdd;				//����׷��
	string	MindAdd;				//����׷��
	string	ZhiHuiAdd;				//�ǻ�׷��
	string	EnergyAdd;				//����׷��
	string	MingZhongRevise;		//������������
	string	DuckRevise;				//������������
	string	GeDangRevise;			//����������
	string	ZhaoJiaRevise;			//�м���������
	string	MaxChongNeng;			//��������	
	string	WuImmunity;				//��������ֵ
	string	MoImmunity;				//ħ������ֵ;
	string	GoodsUpPrice;			//��Ʒ�����۸�
	string	BasicAperValue1;		//��������ֵ1
	string	BasicAperValue2;		//��������ֵ2
	string	AsunderValue2;			//�ֽ��ֵ2
	string	LianDuValue2;			//����ֵ2
	string	DecomBlueCrystalAmount;					//�ֽ���ɫ������
	string	MainShang;				//����
	string	Pierce;					//��͸
	string	Const;					//����
	string	Lucky;					//����
	string	AtkValueED;				//����EDֵ
	string	CurAkkValueED;			//��ǰ����ֵ
	string	RecoveryValueED;		//����edֵ
	string	CurRecoveryValueED;		//��ǰ����ed

	string	RecoveryValueED2;		//����edֵ2
	string	AtkValueED2;			//����edֵ2
	string	EquipDengJie;				//�Ƚ�

	string	PhysMinAtkUp;			//	����С���ɳ�
	string	PyhsMaxAtkUp;			//	����󹥳ɳ�
	string	MagicAtkUp;				//	ħ�������ɳ�
	string	PhysRecoUp;				//	��������ɳ�
	string	MagicRecoUp;			//	ħ�������ɳ�

	string	SuperLifeMaxUp;			// ׷����������
	string	ModifyLifeMaxUp;		// ������������

	string	MinGongAdd;				//	С��׷��
	string	MaxGongAdd;				//	��׷��
	string	MagicGongAdd;			//	ī��׷��
	string	PhysFangAdd;			//	���׷��
	string	MaigcFangAdd;			//	ħ��׷��
};

vector<tagConvert> g_vConvert;
vector<tagConvertE> vecConvertE;
//
//bool CMainFrame::CountConvetEGoodsProperty(tagGoodAttr* pGoods)
//{
//	for ( int i=0; i< (int)vecConvertE.size();i++)
//	{
//		if (pGoods->strOrigName == vecConvertE[i].OrgName)
//		{
//			vector<tagAddAttr>::iterator it=pGoods->vecAddAttr.begin();
//			for(;it!=pGoods->vecAddAttr.end();it++)
//			{
//				tagAddAttr *pAddAttr = &(*it);
//				string str = "";
//				const char * temp = "\\";
//				WORD wType = pAddAttr->wType;
//				//���õ��ߵ�ÿ�����������Ƿ����
//				if ( wType == 12 )												//����ǿ��
//				{
//					str = vecConvertE[i].WeaponStrong;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( wType == 137 )										//����ǿ�ȳɳ�			
//				{
//					str = vecConvertE[i].WeaponStringAdd;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( wType == 13) 											//��С������
//				{
//					str = vecConvertE[i].MinAtk;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 14 == wType )											//��󹥻���
//				{
//					str = vecConvertE[i].MaxAtk;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 22 == wType )											//ħ������
//				{
//					str = vecConvertE[i].MoGong;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 18 == wType )											//��������һ����
//				{
//					str = vecConvertE[i].WuGongSockPro;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}						
//				else if ( 120 == wType )										//�������˺�
//				{
//					str = vecConvertE[i].WuBaoJiDamage;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}						
//				else if ( 121 == wType )										//����������
//				{
//					str = vecConvertE[i].WuBaoJiFastness;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}	
//				else if ( 23 == wType )											//ħ������һ����
//				{
//					str = vecConvertE[i].MoGongSockPro;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}						
//				else if ( 122 == wType )										//ħ�������˺�
//				{
//					str = vecConvertE[i].MoBaoJiDamage;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}	
//				else if ( 123 == wType )										//ħ����������
//				{
//					str = vecConvertE[i].MoBaoJiFastness;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}	
//				else if ( 15 == wType )											//�������
//				{
//					str = vecConvertE[i].WuRecovery;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}		
//				else if ( 26 == wType )											//ħ������
//				{
//					str = vecConvertE[i].MoRecovery;	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}		
//				else if ( 125 == wType )										//�ֽ��ֵ
//				{
//					str = vecConvertE[i].AsunderValue1;
//					if( strcmp(str.c_str(),"\\") )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					str = vecConvertE[i].AsunderValue2;
//					if( strcmp(str.c_str(),"\\") )
//						pAddAttr->lValue2 = atoi(str.c_str());
//					continue;
//				}			
//				else if ( 126 == wType )										//��ƷƷ��
//				{
//					str = vecConvertE[i].GoodsChar;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 41 == wType )												//����׷��
//				{
//					str = vecConvertE[i].ForceAdd;
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 42 == wType )
//				{
//					str = vecConvertE[i].MinJieAdd;									//����׷��
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 43 == wType )	
//				{
//					str = vecConvertE[i].TiZhiAdd;									//����׷��
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 44 == wType )						
//				{
//					str = vecConvertE[i].MindAdd;									//����׷��
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 45 == wType )
//				{
//					str = vecConvertE[i].ZhiHuiAdd;									//�ǻ�׷��
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 46 == wType )
//				{
//					str = vecConvertE[i].EnergyAdd;									//����׷��
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 17 == wType )
//				{
//					str = vecConvertE[i].MingZhongRevise;							//������������
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 19 == wType )
//				{
//					str = vecConvertE[i].DuckRevise;								//������������
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 20 == wType )
//				{
//					str = vecConvertE[i].GeDangRevise;								//����������
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 21 == wType )
//				{
//					str = vecConvertE[i].ZhaoJiaRevise;								//�м���������
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 128 == wType )
//				{
//					str = vecConvertE[i].MaxChongNeng;								//��������	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 159 == wType )
//				{
//					str = vecConvertE[i].WuImmunity;								//��������ֵ	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 160 == wType )
//				{
//					str = vecConvertE[i].MoImmunity;								//ħ������ֵ	
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 161 == wType	)
//				{
//					str = vecConvertE[i].GoodsUpPrice;								//��Ʒ�����۸�
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if ( 90 == wType )
//				{
//					str = vecConvertE[i].BasicAperValue1;							//��������ֵ1
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue1 = atoi(str.c_str());
//					str = vecConvertE[i].BasicAperValue2;							//..........2
//					if ( strcmp( str.c_str(),temp ) )
//						pAddAttr->lValue2 = atoi(str.c_str());
//					continue;
//				}
//				else if( 172 == wType )
//				{
//					str = vecConvertE[i].LianDuValue2;								//����ֵ2
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue2 = atoi(str.c_str());
//					continue;
//				}
//				else if( 183 == wType )
//				{
//					str = vecConvertE[i].DecomBlueCrystalAmount;					//�ֽ���ɫ������
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 184 == wType)										//����EDֵ
//				{
//					str = vecConvertE[i].AtkValueED;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//
//					str = vecConvertE[i].AtkValueED2;						//����edֵ2
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue2 = atoi(str.c_str());
//
//					continue;
//				}
//				else if(185 == wType)
//				{
//					str = vecConvertE[i].CurAkkValueED;							//��ǰ����edֵ
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(186 == wType)
//				{
//					str = vecConvertE[i].RecoveryValueED;						//����EDֵ
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					
//					str = vecConvertE[i].RecoveryValueED2;						//ֵ2
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue2 = atoi(str.c_str());
//					
//					continue;
//				}
//				else if(187 == wType)
//				{
//					str = vecConvertE[i].CurRecoveryValueED;						//��ǰ����EDֵ
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(188 == wType)
//				{
//					str = vecConvertE[i].MainShang;					//����
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(189 == wType)
//				{
//					str = vecConvertE[i].Pierce;				//c��͸
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(190 == wType)
//				{
//					str = vecConvertE[i].Const;				//����
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(191 == wType)
//				{
//					str = vecConvertE[i].Lucky;
//					if(strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(168 == wType )						//װ���Ƚ�
//				{
//					str = vecConvertE[i].EquipDengJie;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(94 == wType )		//	����С���ɳ�
//				{
//					str = vecConvertE[i].PhysMinAtkUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(95 == wType )		//	����󹥳ɳ�
//				{
//					str = vecConvertE[i].PyhsMaxAtkUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(96 == wType )		//	ħ�������ɳ�
//				{
//					str = vecConvertE[i].MagicAtkUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(97 == wType )		//	��������ɳ�
//				{
//					str = vecConvertE[i].PhysRecoUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if(98 == wType )		// ħ�������ɳ�
//				{
//					str = vecConvertE[i].MagicRecoUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 203 == wType )		//	׷����������
//				{
//					str = vecConvertE[i].SuperLifeMaxUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 47 == wType )		//	������������
//				{
//					str = vecConvertE[i].ModifyLifeMaxUp;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 194 == wType )		//	С��׷��
//				{
//					str = vecConvertE[i].MinGongAdd;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 195 == wType )		//	��׷��
//				{
//					str = vecConvertE[i].MaxGongAdd;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 196 == wType )		//	ī��׷��
//				{
//					str = vecConvertE[i].MagicGongAdd;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 197 == wType )		//	���׷��
//				{
//					str = vecConvertE[i].PhysFangAdd;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//				else if( 198 == wType )		//	ħ��׷��
//				{
//					str = vecConvertE[i].MaigcFangAdd;
//					if( strcmp(str.c_str(),temp))
//						pAddAttr->lValue1 = atoi(str.c_str());
//					continue;
//				}
//			}
//		}
//	}
//	return true;
//}

// ItemConvert.ini
bool CMainFrame::CountEquipDisplayProperty(tagGoodAttr *pGoods)
{
	// ����
	bool bVaild = false;
	for (int i=0;i<(int)g_vConvert.size(); i++)
	{
		if (pGoods->strOrigName == g_vConvert[i].strOrgName)
		{	
			const char * temp = "\\";
			string str = g_vConvert[i].strShowName;
			if( strcmp(str.c_str(),temp))
				pGoods->strName = str;
			if( strcmp( g_vConvert[i].lGraphicID.c_str(),temp ))
				pGoods->dwIconId = atoi((g_vConvert[i].lGraphicID).c_str());
			if( strcmp( g_vConvert[i].lGroundID.c_str(),temp ))
				pGoods->dwGroundId = atoi((g_vConvert[i].lGroundID).c_str());
			if( strcmp( g_vConvert[i].lEquipID.c_str(),temp) )
				pGoods->dwEquipID = atoi((g_vConvert[i].lEquipID).c_str());
			if( strcmp( g_vConvert[i].lPrice.c_str(),temp))
				pGoods->dwValue = atoi((g_vConvert[i].lPrice).c_str());
			if( strcmp( g_vConvert[i].strContent.c_str(),temp ))
			{
				const char* s = "=";
				if( strcmp( g_vConvert[i].strContent.c_str(),s) )
					pGoods->strContent = g_vConvert[i].strContent;
				else
					pGoods->strContent = "";
			}
			if( strcmp( g_vConvert[i].lSound.c_str(), temp ) )
				pGoods->dwSound = atoi((g_vConvert[i].lSound).c_str());
			if( strcmp( g_vConvert[i].lSilverPrice.c_str(),temp ))
				pGoods->dwSilverValue = atoi((g_vConvert[i].lSilverPrice).c_str());

			vector<tagAddAttr>::iterator iterAttr;
			for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
			{
				tagAddAttr *pAttr = &(*iterAttr);
				if (pAttr)
				{
					string str = "";
					if (pAttr->wType == 7)					// �ȼ�����
					{
						str = g_vConvert[i].lLevelRest;
						if ( !strcmp( str.c_str(),temp )) continue ;
						pAttr->lValue1 = atoi(str.c_str());
						continue;
					}
					else if (pAttr->wType == 65)			//ִ�нű�
					{
						str = g_vConvert[i].lRunScript;
						if ( !strcmp( str.c_str(),temp ) ) continue ;
						pAttr->lValue1 = atoi(str.c_str());
						continue;
					}
					else if (pAttr->wType == 1) 
					{
						str = g_vConvert[i].lColor;			//��ʾ��ɫ
						if ( !strcmp( str.c_str(),temp ) ) continue ;
						pAttr->lValue1 = atoi(str.c_str());
						continue;
					}
					else if(pAttr->wType == 171)			//��Ʒ����
					{
						str = g_vConvert[i].lClass;
						if(!strcmp(str.c_str(),temp))	continue;
						pAttr->lValue1 = atoi(str.c_str());
						continue;
					}
					else if( pAttr->wType == 57 )			//	ѧϰ����id
					{
						str = g_vConvert[i].lStudySkillID;
						if( !strcmp( str.c_str(),temp ) )	continue;
						pAttr->lValue1 = atoi( str.c_str() );
						continue;
					}
				}
			}
			return true;
		}
	}
	return false;
}

// ���¼�������װ������ʾ���ԣ�����ItemConvert.ini
void CMainFrame::OnCountShow()
{
	std::ifstream file;
	file.open("ItemConvert.ini");
	if(file.fail())
	{
		MessageBox("��ȡ�ļ�ItemConvert.iniʧ��");
		return;
	}
	string line;
	g_vConvert.clear();

	//	��"ItemConvert.ini�����ļ��ڼ�¼ѹ��g_vConvert"
	while (ReadTo(file,"#"))
	{
		//	ReadTo(file,"#");
		tagConvert stConvert;

		file>>stConvert.strOrgName				//ԭʼ��
			>>stConvert.strShowName
			>>stConvert.lGraphicID
			>>stConvert.lGroundID
			>>stConvert.lEquipID
			>>stConvert.lPrice
			>>stConvert.strContent
			>>stConvert.lLevelRest
			>>stConvert.lRunScript
			>>stConvert.lColor
			>>stConvert.lSound
			>>stConvert.lSilverPrice
			>>stConvert.lClass
			>>stConvert.lStudySkillID;
		//�ж��Ƿ����ȷ
		if( !strcmp(stConvert.lStudySkillID.c_str(),"#") ||
			!strcmp(stConvert.lClass.c_str(),"#")	||	!strcmp(stConvert.lSilverPrice.c_str(),"#")	||	!strcmp(stConvert.lSound.c_str(),"#")		||
			!strcmp(stConvert.lColor.c_str(),"#")		||	!strcmp(stConvert.lRunScript.c_str(),"#")	||
			!strcmp(stConvert.lLevelRest.c_str(),"#")	||	!strcmp(stConvert.strContent.c_str(),"#")	||
			!strcmp(stConvert.lPrice.c_str(),"#")		||	!strcmp(stConvert.lEquipID.c_str(),"#")		||
			!strcmp(stConvert.lGroundID.c_str(),"#")	||	!strcmp(stConvert.strShowName.c_str(),"#")	||
			!strcmp(stConvert.strOrgName.c_str(),"#")	)
		{
			char str[128];
			sprintf_s(str,"���棺�����ļ��� %s �����д���",stConvert.strOrgName.c_str());
			MessageBox(str,NULL,0);
			file.close();
			return;
		}
		g_vConvert.push_back(stConvert);
	}

	file.close();

	vector<tagGoodAttr>::iterator iter;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	{
		tagGoodAttr *pGoods = &(*iter);
		if (pGoods)	// װ����
			CountEquipDisplayProperty(pGoods);
	}
	CContainerPage *pContainer=(CContainerPage*)m_wndSplitter.GetPane(0,1);
	pContainer->m_wndAllGoodsPage.UpDataAllGoodsList();

	CAllGoodsTree *pGoodsTree=(CAllGoodsTree*)m_wndSplitter.GetPane(0,0);

	HTREEITEM hSelItem = pGoodsTree->GetSelGoodsTree()->GetSelectedItem();
	HTREEITEM hRootItem = pGoodsTree->GetRootItem();
	if ( hSelItem == hRootItem )
		return;
	int nSelFlag = 0;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++,nSelFlag++)
	{			
		if(pGoodsTree->GetSelGoodsTree()->GetItemText(hSelItem).Compare(CString(iter->strOrigName.c_str()))==0)
			break;
	}
	pContainer->m_wndGoodAttrPage.FillCtrls(nSelFlag);		
}

struct tagBatAdd_E 
{
	//	ֵ����
	int nValueType;
	//	������������
	DWORD dwAddAttrCounts;
	//	������������
	vector<DWORD> vecAddAttrIndex;
	//	������
	map<string,vector<DWORD>>	mapAddAttrValue;
};
tagBatAdd_E myE;
//	ԭʼ�� 
//map<string,vector<DWORD>> mapConverE;
//�����ļ�ItemConvert_E.ini �����ռ�
void CMainFrame::OnConvertE()
{
	// TODO: �ڴ���������������

	std::ifstream file;
	file.open("ItemConvert_E.ini");
	if(file.fail())
	{
		MessageBox("��ȡ�ļ�ItemConvert_E.iniʧ��");
		return;
	}

	//	���������Ե���Ŀ������
	list<long>	listAddrIndex;
	long lAmount;
	long lType;
	//	����
	while (ReadTo(file,"#"))
	{
		//	����������Ŀ
		file>>lAmount;
		//	���������Ե�����
		for(int i=0;i<lAmount;i++)
		{
			file>>lType;
			listAddrIndex.push_back(lType);
		}
		break;
	}
	
	//	��ԭʼ���͸�������ֵ
	map<long,long>		mapIndexMapValue;
	map<string,map<long,long>>	mapNameMapIndex;

	//	����Ʒ
	string szOrgName;
	while ( ReadTo(file,"#"))
	{
		//	ԭʼ��
		file>>szOrgName;
		mapIndexMapValue.clear();

		//	��������������Ӧ�ĸ�������index
		list<long>::iterator it = listAddrIndex.begin();
		for(; it!=listAddrIndex.end();it++)
		{
			long lIndex = (long)(*it);

			//	����һ���ַ����ͣ��ɰ��������Ϊ�����ô���
			string szValue;
			file>>szValue;
			if( strcmp(szValue.c_str(),"#") == 0 )
			{
				AfxMessageBox("�����д���");
				return;
			}

			mapIndexMapValue[lIndex] = atoi(szValue.c_str());
		}
		//
		mapNameMapIndex[szOrgName] = mapIndexMapValue;
	}

	//	����������Ʒ
	vector<tagGoodAttr>::iterator itGoods = vecGoodAttr.begin();
	for( ; itGoods != vecGoodAttr.end();itGoods++)
	{
		tagGoodAttr* pGoods = &(*itGoods);
		map<string,map<long,long>>::iterator itOrgName = mapNameMapIndex.find( pGoods->strOrigName );
		//	�������Ʒ��Ҫ��������
		if( itOrgName != mapNameMapIndex.end() )
		{
			//	��������Ʒ�����и�������
			vector<tagAddAttr>::iterator itAddAttr = itGoods->vecAddAttr.begin();
			for(; itAddAttr != itGoods->vecAddAttr.end(); itAddAttr++ )
			{
				//	������Ҫ�޸ĵĸ��������б�
				map<long,long>::iterator itValue = itOrgName->second.find(itAddAttr->wType);
				if( itValue != itOrgName->second.end() )
				{
					(*itAddAttr).lValue1 = itValue->second;
				}
			}
		}

	}
}

void CMainFrame::UpdateChangeGoodsAddAttr(tagGoodAttr* pGoods,map<string,map<long,string>> & mapTemp)
{
	map<string,map<long,string>>::iterator itOrgName = mapTemp.find(pGoods->strOrigName);
	//	�������Ʒ��Ҫ��������
	if( itOrgName != mapTemp.end() )
	{
		//	��������Ʒ�����и�������
		vector<tagAddAttr>::iterator itAddAttr = pGoods->vecAddAttr.begin();
		for(; itAddAttr != pGoods->vecAddAttr.end(); itAddAttr++ )
		{
			tagAddAttr pAddAttr = (*itAddAttr);
			//	������Ҫ�޸ĵĸ��������б�
			map<long,string> mapValueTemp = itOrgName->second;
			map<long,string>::iterator itFind = mapValueTemp.find(pAddAttr.wType);
			if( itFind != mapValueTemp.end())
			{
				pAddAttr.lValue1 = atoi(itFind->second.c_str());
			}
		}
	}

	return;
	//////////////////////////////////////////////////////////////////////////
	//vecConvertE.clear();
	////��"ItemConvert.ini�����ļ��ڼ�¼ѹ��g_vConvert"
	//while (ReadTo(file, "#"))
	//{
	//	tagConvertE stConvertE;
	//	file>>stConvertE.OrgName					//ԭʼ��
	//		>>stConvertE.WeaponStrong				//����ǿ��
	//		>>stConvertE.WeaponStringAdd			//����ǿ�ȳɳ�
	//		>>stConvertE.MinAtk						//��С������
	//		>>stConvertE.MaxAtk						//��󹥻���
	//		>>stConvertE.MoGong						//ħ������
	//		>>stConvertE.WuGongSockPro				//��������һ����
	//		>>stConvertE.WuBaoJiDamage				//�������˺�
	//		>>stConvertE.WuBaoJiFastness			//����������
	//		>>stConvertE.MoGongSockPro				//ħ������һ����
	//		>>stConvertE.MoBaoJiDamage				//ħ�������˺�
	//		>>stConvertE.MoBaoJiFastness			//ħ����������
	//		>>stConvertE.WuRecovery					//�������
	//		>>stConvertE.MoRecovery					//ħ������
	//		>>stConvertE.AsunderValue1				//�ֽ��ֵ
	//		>>stConvertE.GoodsChar					//��ƷƷ��
	//		>>stConvertE.ForceAdd					//����׷��
	//		>>stConvertE.MinJieAdd					//����׷��
	//		>>stConvertE.TiZhiAdd					//����׷��
	//		>>stConvertE.MindAdd					//����׷��
	//		>>stConvertE.ZhiHuiAdd					//�ǻ�׷��
	//		>>stConvertE.EnergyAdd					//����׷��
	//		>>stConvertE.MingZhongRevise			//������������
	//		>>stConvertE.DuckRevise					//������������
	//		>>stConvertE.GeDangRevise				//����������
	//		>>stConvertE.ZhaoJiaRevise				//�м���������
	//		>>stConvertE.MaxChongNeng				//��������
	//		>>stConvertE.WuImmunity					//��������ֵ
	//		>>stConvertE.MoImmunity					//ħ������ֵ
	//		>>stConvertE.GoodsUpPrice				//��Ʒ�����۸�
	//		>>stConvertE.BasicAperValue1			//��������ֵ1
	//		>>stConvertE.BasicAperValue2			//��������ֵ2
	//		>>stConvertE.AsunderValue2				//�ֽ��ֵ2
	//		>>stConvertE.LianDuValue2				//����ֵ2
	//		>>stConvertE.DecomBlueCrystalAmount		//�ֽ���ɫ������
	//		>>stConvertE.MainShang					//����
	//		>>stConvertE.Pierce						//��͸
	//		>>stConvertE.Const						//����
	//		>>stConvertE.Lucky						//����
	//		>>stConvertE.AtkValueED					//����EDֵ
	//		>>stConvertE.CurAkkValueED				//��ǰ����EDֵ
	//		>>stConvertE.RecoveryValueED			//����EDֵ
	//		>>stConvertE.CurRecoveryValueED			//��ǰ����edֵ

	//		>>stConvertE.RecoveryValueED2			//����edֵ2
	//		>>stConvertE.AtkValueED2				//����edֵ2
	//		>>stConvertE.EquipDengJie				//װ���Ƚ�

	//		>>stConvertE.PhysMinAtkUp			//	����С���ɳ�
	//		>>stConvertE.PyhsMaxAtkUp			//	����󹥳ɳ�
	//		>>stConvertE.MagicAtkUp				//	ħ�������ɳ�
	//		>>stConvertE.PhysRecoUp				//	��������ɳ�
	//		>>stConvertE.MagicRecoUp			//	ħ�������ɳ�
	//		>>stConvertE.SuperLifeMaxUp			//	׷����������
	//		>>stConvertE.ModifyLifeMaxUp		//	������������
	//		>>stConvertE.MinGongAdd					//	С��׷��
	//		>>stConvertE.MaxGongAdd					//	��׷��
	//		>>stConvertE.MagicGongAdd				//	ī��׷��
	//		>>stConvertE.PhysFangAdd				//	���׷��
	//		>>stConvertE.MaigcFangAdd;				//	ħ��׷��

	//		//�ж��Ƿ����
	//	if( !strcmp(stConvertE.MaigcFangAdd.c_str(),"#") || !strcmp(stConvertE.PhysFangAdd.c_str(),"#")	||
	//		!strcmp(stConvertE.MagicGongAdd.c_str(),"#") || !strcmp(stConvertE.MaxGongAdd.c_str(),"#")	|| !strcmp(stConvertE.MinGongAdd.c_str(),"#") ||
	//		!strcmp(stConvertE.ModifyLifeMaxUp.c_str(),"#") || !strcmp(stConvertE.SuperLifeMaxUp.c_str(),"#") || 
	//		!strcmp(stConvertE.MagicRecoUp.c_str(),"#")	|| !strcmp(stConvertE.PhysRecoUp.c_str(),"#")	|| !strcmp(stConvertE.MagicAtkUp.c_str(),"#")	||
	//		!strcmp(stConvertE.PhysMinAtkUp.c_str(),"#") || !strcmp(stConvertE.PhysMinAtkUp.c_str(),"#") ||
	//		!strcmp(stConvertE.EquipDengJie.c_str(),"#")					||	!strcmp(stConvertE.AtkValueED2.c_str(),"#")	||	!strcmp(stConvertE.RecoveryValueED2.c_str(),"#") ||
	//		!strcmp(stConvertE.CurRecoveryValueED.c_str(),"#")		||	!strcmp(stConvertE.RecoveryValueED.c_str(),"#")			||
	//		!strcmp(stConvertE.CurAkkValueED.c_str(),"#")			||	!strcmp(stConvertE.AtkValueED.c_str(),"#")				||
	//		!strcmp(stConvertE.Lucky.c_str(),"#")					||	!strcmp(stConvertE.Const.c_str(),"#")					||
	//		!strcmp(stConvertE.Pierce.c_str(),"#")					||	!strcmp(stConvertE.MainShang.c_str(),"#")				||
	//		!strcmp(stConvertE.DecomBlueCrystalAmount.c_str(),"#") ||
	//		!strcmp(stConvertE.LianDuValue2.c_str(),"#")	||	!strcmp(stConvertE.AsunderValue2.c_str(),"#")	||	
	//		!strcmp(stConvertE.BasicAperValue2.c_str(),"#")	||	!strcmp(stConvertE.BasicAperValue1.c_str(),"#")	||
	//		!strcmp(stConvertE.GoodsUpPrice.c_str(),"#")	||	!strcmp(stConvertE.MoImmunity.c_str(),"#")		||
	//		!strcmp(stConvertE.MaxChongNeng.c_str(),"#")	||	!strcmp(stConvertE.ZhaoJiaRevise.c_str(),"#")	||
	//		!strcmp(stConvertE.GeDangRevise.c_str(),"#")	||	!strcmp(stConvertE.DuckRevise.c_str(),"#")		||
	//		!strcmp(stConvertE.MingZhongRevise.c_str(),"#")	||	!strcmp(stConvertE.EnergyAdd.c_str(),"#")		||
	//		!strcmp(stConvertE.ZhiHuiAdd.c_str(),"#")		||	!strcmp(stConvertE.MindAdd.c_str(),"#")			||
	//		!strcmp(stConvertE.TiZhiAdd.c_str(),"#")		||	!strcmp(stConvertE.MinJieAdd.c_str(),"#")		||
	//		!strcmp(stConvertE.ForceAdd.c_str(),"#")		||	!strcmp(stConvertE.GoodsChar.c_str(),"#")		||
	//		!strcmp(stConvertE.AsunderValue1.c_str(),"#")	||	!strcmp(stConvertE.MoRecovery.c_str(),"#")		||
	//		!strcmp(stConvertE.WuRecovery.c_str(),"#")		||	!strcmp(stConvertE.MoBaoJiFastness.c_str(),"#")	||
	//		!strcmp(stConvertE.MoBaoJiDamage.c_str(),"#")	||	!strcmp(stConvertE.MoGongSockPro.c_str(),"#")	||
	//		!strcmp(stConvertE.WuBaoJiFastness.c_str(),"#")	||	!strcmp(stConvertE.WuBaoJiDamage.c_str(),"#")	||
	//		!strcmp(stConvertE.WuGongSockPro.c_str(),"#")	||	!strcmp(stConvertE.MoGong.c_str(),"#")			||
	//		!strcmp(stConvertE.MaxAtk.c_str(),"#")			||	!strcmp(stConvertE.MinAtk.c_str(),"#")			||
	//		!strcmp(stConvertE.WeaponStringAdd.c_str(),"#")	||	!strcmp(stConvertE.WeaponStrong.c_str(),"#")	||
	//		!strcmp(stConvertE.OrgName.c_str(),"#")	)
	//	{
	//		char str[128];
	//		sprintf_s(str,"���棺�����ļ��� %s �����д���",stConvertE.OrgName.c_str());
	//		MessageBox(str,NULL,0);
	//		return;
	//	}
	//	vecConvertE.push_back(stConvertE);
	//}

	//file.close();

	//vector<tagGoodAttr>::iterator iter;
	//for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	//{
	//	tagGoodAttr *pGoods = &(*iter);
	//	if (pGoods)	// װ����
	//		CountConvetEGoodsProperty(pGoods);
	//}
}

// ��ȡ��Ʒ��������ֵһ
long GetValue1(tagGoodAttr * pGoods, WORD wType)
{
	vector<tagAddAttr>::iterator iterAttr;
	for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
	{
		tagAddAttr *pAttr = &(*iterAttr);
		if (pAttr && pAttr->wType == wType)
		{
			return pAttr->lValue1;
		}
	}
	return 0;
}

// ������Ʒ��������ֵһ
long SetValue1(tagGoodAttr * pGoods, WORD wType, long value)
{
	vector<tagAddAttr>::iterator iterAttr;
	for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
	{
		tagAddAttr *pAttr = &(*iterAttr);
		if (pAttr && pAttr->wType == wType)
		{
			pAttr->lValue1 = value;
			return value;
		}
	}
	return -1;
}

// 
long AddValue(tagGoodAttr * pGoods, WORD wType, long value1, long value2)
{
	vector<tagAddAttr>::iterator iterAttr;
	for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
	{
		tagAddAttr *pAttr = &(*iterAttr);
		if (pAttr && pAttr->wType == wType)
		{
			pAttr->lValue1 = value1;
			pAttr->lValue2 = value2;
			return 1;
		}
	}

	tagAddAttr sta;
	sta.bEnable = true;
	sta.bHide = false;
	sta.lValue1 = value1;
	sta.lValue2 = value2;
	//sta.wEnableOdds = 10000;
	sta.wType = wType;

	pGoods->vecAddAttr.push_back(sta);

	return 1;
}

// ��ȡ��Ʒ��������ֵһ�����1
long GetValue1Random(tagGoodAttr * pGoods, WORD wType)
{/*
 vector<tagAddAttr>::iterator iterAttr;
 for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
 {
 tagAddAttr *pAttr = &(*iterAttr);
 if (pAttr && pAttr->wType == wType)
 {
 if (pAttr->vecModify.size())
 {
 return 1;
 }
 else
 return 0;
 }
 }*/
	return 0;
}

// ������Ʒ��������ֵһ�����1
long SetValue1Random(tagGoodAttr * pGoods, WORD wType, long min, long max)
{/*
 vector<tagAddAttr>::iterator iterAttr;
 for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
 {
 tagAddAttr *pAttr = &(*iterAttr);
 if (pAttr && pAttr->wType == wType)
 {
 if (pAttr->vecModify.size())
 {
 pAttr->vecModify[0].dwMin = min;
 pAttr->vecModify[0].dwMax = max;
 }
 else
 return 0;
 }
 }*/
	return 0;
}


//
//1����������ԡ���󹥻�����ͬʱ���������С��������1�������޸��������1�ġ�ֵ1����1����ֵ2������󹥻���0.2
//
//2����������ԡ���С��������ͬʱ���������С��������1�������޸��������1�ġ�ֵ1����1����ֵ2������С������0.2
//
//3��ͬʱ����������С���С�����ɳ��������޸ĸ����Ե�ֵ1����󹥻���0.05
//
//4��ͬʱ����������С���󹥻��ɳ��������޸ĸ����Ե�ֵ1����󹥻���0.05
//
//5������е����С���������������ԣ�ͬʱ���������С��������1�������޸��������1�ġ�ֵ1����1����ֵ2���������������0.2
//
//6��ͬʱ����������С���������ɳ��������޸ĸ����Ե�ֵ1�������������0.05
//
//7������е����С������������������ԣ�ͬʱ���������С��������1�������޸��������1�ġ�ֵ1����1����ֵ2������������������0.2
//
//8��ͬʱ����������С��������������ɳ��������޸ĸ����Ե�ֵ1����������������0.05
//
//

// ���㹥������ͳɳ�
void CMainFrame::OnCountRandom()
{
	vector<tagGoodAttr>::iterator iter;
	vector<tagAddAttr>::iterator iterAttr;
	//vector<tagModify>::iterator iterModify;

	int nTem=0;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	{
		tagGoodAttr *pGoods = &(*iter);
		if (pGoods && pGoods->dwType >= 2 )	// װ����
		{
			long lAttackMin = GetValue1(pGoods, 13);				//��С����
			long lAttackMax = GetValue1(pGoods, 14);				//��󹥻�
			long lWuRecovery	= GetValue1(pGoods, 15);			//�������
			long lMagicAtk	= GetValue1(pGoods, 22);				//ħ������
			long lMagicRecovery = GetValue1(pGoods,26);				//ħ������
			//long lAttackMinUp = GetValue1(pGoods, 52);
			//long lAttackMaxUp = GetValue1(pGoods, 53);
			//long lMagicAtk = GetValue1(pGoods, 15);
			//long lMagicAtkUp = GetValue1(pGoods, 54);
			//long lDef = GetValue1(pGoods, 16);
			//long lDefUp = GetValue1(pGoods, 55);
			//long lMagicDefUp = GetValue1(pGoods, 72);

			// ���������ɳ�min
			if (lAttackMin>0)
			{
				float f = lAttackMin / 20.f;
				long n = (long)f;

				// 5��
				if (f-(float)n >= 0.5f)
				{
					n++;
				}

				if( n<1)
				{
						n = 1;
				}
				SetValue1(pGoods, 94, n);
			}

			// ���������ɳ�max
			if (lAttackMax>0)
			{
				float f = lAttackMax / 20.f;
				long n = (long)f;

				// 5��
				if (f-(float)n >= 0.5f)
				{
					n++;
				}
				if (n<1)
				{
						n=1;
				}
				SetValue1(pGoods, 95, n);
			}

			// ���������ɳ�max
			if (lMagicAtk>0)			
			{
				float f = lMagicAtk / 20.f;
				long n = (long)f;

				// 5��
				if (f-(float)n >= 0.5f)
				{
					n++;
				}
				if (n<1) 
				{
						n=1;
				}
				SetValue1(pGoods, 96, n);					//ħ���������ɳ�ֵ
			}

			// ��������ɳ�
			if (lWuRecovery>0)
			{
				float f = lWuRecovery / 20.f;
				long n = (long)f;

				// 5��
				if (f-(float)n >= 0.5f)
				{
					n++;
				}
				if (n<1) 
				{
						n=1;
				}
				SetValue1(pGoods, 97, n);					//����������ɳ�ֵ
			}

			// ����ħ���ɳ�
			if (lMagicRecovery>0)
			{
				float f = lMagicRecovery / 20.f;
				long n = (long)f;

				// 5��
				if (f-(float)n >= 0.5f)
				{
					n++;
				}
				if (n<1)
				{
						n=1;
				}
				SetValue1(pGoods, 98, n);					//ħ���������ɳ�ֵ
			}

			//// �����������min
			//if (lAttackMin && GetValue1Random(pGoods, 14))
			//{
			//	float f = lAttackMin / 5.f;
			//	long n = (long)f;

			//	// 5��
			//	if (f-(float)n > 0.5f)
			//	{
			//		n++;
			//	}
			//	if (n<1) n=1;
			//	SetValue1Random(pGoods, 14, 1, n);
			//}

			//// �����������max
			//if (lAttackMax && GetValue1Random(pGoods, 15))
			//{
			//	float f = lAttackMax / 5.f;
			//	long n = (long)f;

			//	// 5��
			//	if (f-(float)n > 0.5f)
			//	{
			//		n++;
			//	}
			//	if (n<1) n=1;
			//	SetValue1Random(pGoods, 15, 1, n);
			//}

			//// �����������
			//if (lMagicAtk && GetValue1Random(pGoods, 16))
			//{
			//	float f = lMagicAtk / 5.f;
			//	long n = (long)f;

			//	// 5��
			//	if (f-(float)n > 0.5f)
			//	{
			//		n++;
			//	}
			//	if (n<1) n=1;
			//	SetValue1Random(pGoods, 16, 1, n);
			//}

			//// ����������min
			//if (lDef && GetValue1Random(pGoods, 17))
			//{
			//	float f = lDef / 5.f;
			//	long n = (long)f;

			//	// 5��
			//	if (f-(float)n > 0.5f)
			//	{
			//		n++;
			//	}
			//	if (n<1) n=1;
			//	SetValue1Random(pGoods, 17, 1, n);
			//}

			//// ����ħ�����min
			//if (lMagicDef && GetValue1Random(pGoods, 23))
			//{
			//	float f = lMagicDef / 5.f;
			//	long n = (long)f;

			//	// 5��
			//	if (f-(float)n > 0.5f)
			//	{
			//		n++;
			//	}
			//	if (n<1) n=1;
			//	SetValue1Random(pGoods, 23, 1, n);
			//}
		}
	}
}

long ClearRandom(tagGoodAttr * pGoods)
{/*
 vector<tagAddAttr>::iterator iterAttr;
 for(iterAttr=pGoods->vecAddAttr.begin();iterAttr!=pGoods->vecAddAttr.end();iterAttr++)
 {
 tagAddAttr *pAttr = &(*iterAttr);
 if (pAttr)
 {
 while (pAttr->vecModify.size() > 1)
 {
 pAttr->vecModify.pop_back();
 }
 }
 }*/
	return 0;
}

// �������װ���������Ե����ֵ��ֻ������һ��
void CMainFrame::OnCountRandomnum()
{
	vector<tagGoodAttr>::iterator iter;
	vector<tagAddAttr>::iterator iterAttr;
	//vector<tagModify>::iterator iterModify;

	int nTem=0;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	{
		tagGoodAttr *pGoods = &(*iter);
		if (pGoods && pGoods->dwType >= 2 )	// װ����
		{
			ClearRandom(pGoods);
		}
	}
}

// ����ǿ��+20
void CMainFrame::OnCountAdd20()
{
	vector<tagGoodAttr>::iterator iter;
	vector<tagAddAttr>::iterator iterAttr;
	//vector<tagModify>::iterator iterModify;

	int nTem=0;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	{
		tagGoodAttr *pGoods = &(*iter);
		if (pGoods && pGoods->dwType >= 2 )	// װ����
		{
			long val = GetValue1(pGoods, 90);
			if (val != -1)
			{
				SetValue1(pGoods, 90, val + 20);
			}
		}
	}
}

void CMainFrame::OnAddTypeex()
{
	vector<tagGoodAttr>::iterator iter;
	vector<tagAddAttr>::iterator iterAttr;
	//vector<tagModify>::iterator iterModify;

	int nTem=0;
	for(iter=vecGoodAttr.begin();iter!=vecGoodAttr.end();iter++)
	{
		tagGoodAttr *pGoods = &(*iter);
		if (pGoods && pGoods->dwType >= 2 )	// װ����
		{
			long val = GetValue1(pGoods, 13);
			if (val == 0)
			{
				AddValue(pGoods, 13, 0, 0);
			}
		}
	}
}
//���ݵ����� ItemEduce.ini
void CMainFrame::OnCheckDataToEduce()
{
	// TODO: �ڴ���������������
	std::ofstream ofs("ItemEduce.ini",ios::out);

	ofs.write("ԭʼ��	", sizeof("ԭʼ��"));
	ofs.write("��ʾ��	", sizeof("��ʾ��"));
	ofs.write("����ͼ��	", sizeof("����ͼ��"));
	ofs.write("���ͼ��	", sizeof("���ͼ��"));
	ofs.write("��װͼ��	", sizeof("��װͼ��"));
	ofs.write("�۸�	", sizeof("�۸�"));
	ofs.write("����	", sizeof("����"));
	ofs.write("�ȼ�����	", sizeof("�ȼ�����"));
	ofs.write("ִ�нű�\n	", sizeof("ִ�нű�"));

	vector<tagGoodAttr>::iterator pAttr = vecGoodAttr.begin();
	for (; pAttr!=vecGoodAttr.end();pAttr++)
	{
		tagConvert pOutAttr;

		string str = pAttr->strOrigName.c_str();
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');

		str = pAttr->strName.c_str();
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');	

		char str1[16];
		str = _itoa(pAttr->dwIconId,str1,10);			
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');

		str = _itoa(pAttr->dwGroundId,str1,10);
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');

		str = _itoa(pAttr->dwEquipID,str1,10);
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');

		str = _itoa(pAttr->dwValue,str1,10);
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');

		str = pAttr->strContent.c_str();
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put('	');

		vector<tagAddAttr>::iterator pAdd = pAttr->vecAddAttr.begin();
		if (pAttr->vecAddAttr.empty())
		{
			for (int i=0;i<2;i++)
			{
				ofs.write("\\",4);
				ofs.put('	');
			}
			ofs.write("\n",1);
			ofs.close();
			return;
		}

		bool bFound;

		for (;pAdd!=pAttr->vecAddAttr.end();pAdd++)
		{
			if(pAdd->wType == 7)						//�ȼ�����
			{
				str = itoa(pAdd->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put('	');
				bFound = true;
				break;
			}
			bFound = false;
		}
		if(!bFound)
		{
			ofs.write("\\",4);
			ofs.put('	');
		}

		bFound = true;
		pAdd = pAttr->vecAddAttr.begin();
		for (;pAdd!=pAttr->vecAddAttr.end();pAdd++)
		{
			if (pAdd->wType == 65)						//ִ�нű�
			{
				str = _itoa(pAdd->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put('	');
				bFound = true;
				break;
			}
			bFound = false;
		}
		if(!bFound)
		{
			ofs.write("\\",4);
			ofs.put('	');
		}
		ofs.write("\n",1);
	}
	ofs.close();
}
//���ݵ����� ItemEduce_E.ini
void CMainFrame::OnCheckDataToEduceE()
{
	//	 TODO: �ڴ���������������
	std::ofstream ofs("ItemEduce_E.ini",ios::out);

	ofs.write("ԭʼ��	", sizeof("ԭʼ��"));
	ofs.write("����ǿ��	", sizeof("����ǿ��"));
	ofs.write("����ǿ�ȳɳ�	", sizeof("����ǿ�ȳɳ�"));
	ofs.write("��С������	", sizeof("��С������"));
	ofs.write("��󹥻���	", sizeof("��󹥻���"));
	ofs.write("ħ������	", sizeof("ħ������"));
	ofs.write("��������һ����	", sizeof("��������һ����"));
	ofs.write("�������˺�	", sizeof("�������˺�"));
	ofs.write("����������	", sizeof("����������"));
	ofs.write("ħ������һ����	", sizeof("ħ������һ����"));
	ofs.write("ħ�������˺�	", sizeof("ħ�������˺�"));
	ofs.write("ħ����������	", sizeof("ħ����������"));
	ofs.write("�������	", sizeof("�������"));
	ofs.write("ħ������	", sizeof("ħ������"));
	ofs.write("��������	", sizeof("��������"));
	ofs.write("�ֽ��ֵ	", sizeof("�ֽ��ֵ"));
	ofs.write("��ƷƷ��	", sizeof("��ƷƷ��"));
	ofs.write("����׷��	", sizeof("����׷��"));
	ofs.write("����׷��	", sizeof("����׷��"));
	ofs.write("����׷��	", sizeof("����׷��"));
	ofs.write("����׷��	", sizeof("����׷��"));
	ofs.write("�ǻ�׷��	", sizeof("�ǻ�׷��"));
	ofs.write("����׷��	", sizeof("����׷��"));
	ofs.write("������������	", sizeof("������������"));
	ofs.write("������������	", sizeof("������������"));
	ofs.write("����������	", sizeof("����������"));
	ofs.write("�м���������	", sizeof("�м���������"));
	ofs.write("��������", sizeof("��������"));
	ofs.write("��������ֵ	", sizeof("��������ֵ"));
	ofs.write("ħ������ֵ	", sizeof("ħ������ֵ"));
	ofs.write("��Ʒ�����۸�\n", sizeof("��Ʒ�����۸�"));

	vector<tagGoodAttr>::iterator pAttr = vecGoodAttr.begin();
	for (; pAttr!=vecGoodAttr.end();pAttr++)
	{
		tagConvertE pOutAttr;
		char s = '	';
		char str1[16];
		//ԭʼ��
		string str = pAttr->strOrigName.c_str();
		ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
		ofs.put(s);

		if (pAttr->vecAddAttr.empty())
		{
			for (int i=0;i<30;i++)
			{
				ofs.write("\\",4);
				ofs.put(s);
			}
			ofs.write("\n",1);
			continue;
		}

		vector<tagAddAttr>::iterator pAddAttr = pAttr->vecAddAttr.begin();
		bool bFind = false;
		for (;pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType==12)								//����ǿ��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 137)							//����ǿ�ȳɳ�
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	13)							//��С������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 14)							//��󹥻���
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 22)							//ħ������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	18)							//��������һ����
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 120)							//�������˺�
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 121)							//����������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	23)							//ħ������һ����
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 122)							//ħ�������˺�
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 123)							//ħ����������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	15)							//�������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 26)							//ħ������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		//for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		//{
		//	if(pAddAttr->wType == 90)							//��������
		//	{
		//		str = itoa(pAddAttr->lValue1,str1,10);
		//		ofs.write(str.c_str(),strlen(str.c_str()));
		//		ofs.put(s);
		//		bFind = true;
		//		break;
		//	}
		//	bFind = false;
		//}
		//if (!bFind)
		//{
		//	ofs.write("\\",4);
		//	ofs.put(s);
		//}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	125)							//�ֽ��ֵ
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 126)							//��ƷƷ��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 41)							//����׷��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	42)							//����׷��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 43)							//����׷��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 44)							//����׷��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	45)							//�ǻ�׷��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 46)							//����׷��
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 17)							//������������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	19)							//������������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 20)							//����������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 21)							//�м���������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(	pAddAttr->wType ==	128)							//��������
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 159)							//��������ֵ
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 160)							//ħ������ֵ
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}
		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		for(pAddAttr = pAttr->vecAddAttr.begin();pAddAttr!=pAttr->vecAddAttr.end();pAddAttr++)
		{
			if(pAddAttr->wType == 161)							//��Ʒ�����۸�
			{
				str = itoa(pAddAttr->lValue1,str1,10);
				ofs.write(str.c_str(),static_cast<int>(strlen(str.c_str())));
				ofs.put(s);
				bFind = true;
				break;
			}
			bFind = false;
		}

		if (!bFind)
		{
			ofs.write("\\",4);
			ofs.put(s);
		}

		ofs.write("\n",1);
	}
	ofs.close();
}
void CMainFrame::OnAddAttrByBatAdd()
{
//	 TODO: �ڴ���������������
	list<string> m_listBatOrgName;
	list<tagAddAttr> m_listBatAddAttrType;
	m_listBatOrgName.clear();
	m_listBatAddAttrType.clear();

	std::ifstream in_f,in_f2;
	in_f.open("OrgName.ini");	in_f2.open("AddAttr.ini");
	if(in_f.fail() || in_f2.fail())
	{
		MessageBox("��ȡ�ļ�ʧ��");
		return;
	}
	//��ԭʼ��
	while(true)
	{
		string strOrgName;
		in_f>>strOrgName;				//ԭʼ��
		if( strOrgName.empty())
		{
			break;
		}
		m_listBatOrgName.push_back(strOrgName);
	}
	//����������
	while(true)
	{
		tagAddAttr addData;
		WORD wShowType;
		long lValue1,lValue2;
		in_f2>>wShowType				//��������ֵ
			>>lValue1					//ֵ1
			>>lValue2;				//ֵ2

		addData.wType = wShowType;
		addData.lValue1 = lValue1;
		addData.lValue2 = lValue2;
		addData.bEnable = true;
		addData.bHide = false;

		m_listBatAddAttrType.push_back(addData);
		if( in_f2.eof())
			break;
	}

	in_f.close();		in_f2.close();

	list<string>::iterator itOrgNameIter = m_listBatOrgName.begin();
	//����ԭʼ��
	for(;itOrgNameIter != m_listBatOrgName.end();itOrgNameIter++)
	{
		DWORD dwIndex = -1;
		GetGoodsAddrByOrgName((*itOrgNameIter).c_str(),dwIndex);
		if( dwIndex != -1 )
		{
			//��ȡÿ�������
			list<tagAddAttr>::iterator it = m_listBatAddAttrType.begin();
			for(; it != m_listBatAddAttrType.end();it++)
			{
				WORD wType = (WORD)(*it).wType -1;

				tagAddAttr addAttr;
				addAttr.wType = wType;
				addAttr.lValue1 = it->lValue1;
				addAttr.lValue2 = it->lValue2;
				addAttr.bEnable = true;
				addAttr.bHide = false;

				//���Ϊ��,��ֱ�����
				if( vecGoodAttr[dwIndex].vecAddAttr.empty())
				{
					vecGoodAttr[dwIndex].vecAddAttr.push_back(addAttr);
					if( m_wndGoodAttr.GetEditFlag() )
					{
						m_wndGoodAttr.UpDataOwnAttrList(dwIndex);
					}
					break;

				}
				//�����Ϊ��
				else
				{					
					//�Ƿ���Ҫ���
					bool bAdd = true;			
				
					vector<tagAddAttr>::iterator itGoodsAttrIter = vecGoodAttr[dwIndex].vecAddAttr.begin();
					//�����Ѿ��еĸ�������
					for(;itGoodsAttrIter != vecGoodAttr[dwIndex].vecAddAttr.end();itGoodsAttrIter++)
					{
						//������,�����
						if(itGoodsAttrIter->wType == wType)
						{
							bAdd = false;
							break;
						}
						//��һ�����Լ����Typeǰ�����
						if( itGoodsAttrIter->wType > wType)
						{
							vecGoodAttr[dwIndex].vecAddAttr.insert(itGoodsAttrIter,addAttr);
							//�����
							bAdd = false;	
							if( m_wndGoodAttr.GetEditFlag() )
							{
								m_wndGoodAttr.UpDataOwnAttrList(dwIndex);
							}
							break;
						}
					}

					if( bAdd )
					{
						vecGoodAttr[dwIndex].vecAddAttr.push_back(addAttr);
						if( m_wndGoodAttr.GetEditFlag() )
						{
							m_wndGoodAttr.UpDataOwnAttrList(dwIndex);
						}
					}
				}
			}
		}
	}
	return;
}



void CMainFrame::GetGoodsAddrByOrgName(const char*strOrgName,DWORD &dwIndex)
{
	dwIndex = -1;
	for(int i = 0; i < (int)vecGoodAttr.size();i++)
	{
		tagGoodAttr goodsAttr = vecGoodAttr[i];
		if( strcmp(goodsAttr.strOrigName.c_str(),strOrgName) == 0 )
		{
			dwIndex = i;
			return;
		}
	}
}

void CMainFrame::OnAddArrtBatDel()
{
	// TODO: �ڴ���������������
	list<string> m_listBatOrgName;
	list<WORD> m_listBatAddAttrType;
	m_listBatOrgName.clear();
	m_listBatAddAttrType.clear();

	std::ifstream in_f,in_f2;
	in_f.open("OrgName.ini");	in_f2.open("AddAttr.ini");

	if(in_f.fail() || in_f2.fail())
	{
		MessageBox("��ȡ�ļ�ʧ��");
		return;
	}
	//��ԭʼ��
	while(true)
	{
		string strOrgName;
		in_f>>strOrgName;				//ԭʼ��
		if( strOrgName.empty())
		{
			break;
		}
		m_listBatOrgName.push_back(strOrgName);
	}
	//����������
	while(true)
	{
		WORD dwAddAttrType;
		in_f2>>dwAddAttrType;				//��������ֵ
		m_listBatAddAttrType.push_back(dwAddAttrType);
		if( in_f2.eof())
			break;
	}
	in_f.close();		in_f2.close();
	for(list<string>::iterator it = m_listBatOrgName.begin(); it != m_listBatOrgName.end();it++)
	{
		DWORD dwIndex = -1;
		GetGoodsAddrByOrgName((*it).c_str(),dwIndex);
		if( dwIndex != -1 )
		{
			//������Ҫɾ���ĸ�������
			for(list<WORD>::iterator itType = m_listBatAddAttrType.begin(); itType != m_listBatAddAttrType.end();itType++)
			{
				WORD wDelType = (WORD)(*itType) - 1;
				//�������е����и�������
				for( vector<tagAddAttr>::iterator it = vecGoodAttr[dwIndex].vecAddAttr.begin(); it != vecGoodAttr[dwIndex].vecAddAttr.end();it++)
				{
					//����У���ɾ��
					if( it->wType == wDelType)
					{
						vecGoodAttr[dwIndex].vecAddAttr.erase(it);
						if( m_wndGoodAttr.GetEditFlag() )
						{
							m_wndGoodAttr.UpDataOwnAttrList(dwIndex);
						}
						//������������һ��
						break;
					}
				}
			}
		}
	}
}

void CMainFrame::OnEduceOneAttribute()
{
	// TODO: �ڴ���������������
	std::ifstream in_f;
	in_f.open("EduceSetup.ini");

	long lFlag = 0;			//	1:�������ԣ�2����������
	long lAttrIndex = 0;	//	��������
	long lValueFlag = 0;	//	ֵ��
	while (ReadTo(in_f,"#"))
	{
		in_f>>lFlag
			>>lAttrIndex
			>>lValueFlag;
	}
	in_f.close();

	std::ofstream ofs("EduceData.ini",ios::out);

	//	����������Ʒ
	vector<tagGoodAttr>::iterator pAttr = vecGoodAttr.begin();
	for(; pAttr != vecGoodAttr.end(); pAttr++)
	{
		tagGoodAttr *pstGoodAttr = &(*pAttr);
		if( lFlag == 2 )
		{
			char szValue[128] = "\\";
			vector<tagAddAttr>::iterator it = pstGoodAttr->vecAddAttr.begin(); 
			for(; it != pstGoodAttr->vecAddAttr.end();it++)
			{
				tagAddAttr *pAttr = &(*it);
				//	��������
				if( pAttr->wType == lAttrIndex)
				{
					//	ֵ��
					if( lValueFlag == 1 )
					{
						sprintf(szValue,"%d",pAttr->lValue1);
					}
					else
					{
						sprintf(szValue,"%d",pAttr->lValue2);
					}

					ofs.write(pstGoodAttr->strName.c_str(),strlen(pstGoodAttr->strName.c_str()));
					ofs.write("	",1);
					ofs.write(pstGoodAttr->strOrigName.c_str(),strlen(pstGoodAttr->strOrigName.c_str()));
					ofs.write("	",1);
					ofs.write(szValue,strlen(szValue));
					ofs.write("\n",1);
				}
			}
		}
	}
	ofs.close();
}

void CMainFrame::OnCalculateAddAttrValue2()
{
	// TODO: �ڴ���������������

	std::ifstream file;
	file.open("Value2.ini");
	if(file.fail())
	{
		MessageBox("��ȡ�ļ�ItemConvert_E.iniʧ��");
		return;
	}

	list<long>	listFuJiaType;
	long lAmount;
	long lType;
	//	����
	while (ReadTo(file,"#"))
	{
		//	����������Ŀ
		file>>lAmount;
		//	���������Ե����
		for(int i=0;i<lAmount;i++)
		{
			file>>lType;
			listFuJiaType.push_back(lType);
		}
		break;
	}

	map<long,string>	mapFujiaValue;
	map<string,map<long,string>> mapOrgNameMapToValue;
	//	����Ʒ
	string szOrgName;
	while ( ReadTo(file,"#"))
	{
		//	ԭʼ��
		file>>szOrgName;
		mapFujiaValue.clear();
		//	ĳ���������Զ�Ӧ��ֵ
		list<long>::iterator it = listFuJiaType.begin();
		for(; it!=listFuJiaType.end();it++)
		{
			long key = (long)(*it);
			string szValue;
			file>>szValue;
			if( strcmp(szValue.c_str(),"#") == 0 )
			{
				AfxMessageBox("�����д���");
				return;
			}
			mapFujiaValue[key] = szValue;
		}
		mapOrgNameMapToValue[szOrgName] = mapFujiaValue;
	}
	file.close();

	//	����������Ʒ
	vector<tagGoodAttr>::iterator itAllGoods = vecGoodAttr.begin();
	for( ; itAllGoods != vecGoodAttr.end();itAllGoods++)
	{
		map<string,map<long,string>>::iterator itOrgName = mapOrgNameMapToValue.find(itAllGoods->strOrigName);
		//	�������Ʒ��Ҫ��������
		if( itOrgName != mapOrgNameMapToValue.end() )
		{
			//	��������Ʒ�����и�������
			vector<tagAddAttr>::iterator itAddAttr = itAllGoods->vecAddAttr.begin();
			for(; itAddAttr != itAllGoods->vecAddAttr.end(); itAddAttr++ )
			{
				//	������Ҫ�޸ĵĸ��������б�
				map<long,string> mapValueTemp = itOrgName->second;
				map<long,string>::iterator itFind = mapValueTemp.find(itAddAttr->wType);
				if( itFind != mapValueTemp.end())
				{
					itAddAttr->lValue2 = atoi(itFind->second.c_str());
				}
			}
		}

	}
}

void CMainFrame::OnSearch()
{
	// TODO: �ڴ���������������
	Search search(this);
	search.DoModal();
	return;
}
