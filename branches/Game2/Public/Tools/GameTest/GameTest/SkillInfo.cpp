/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CSkillInfo.cpp
*  ���ܣ�������Ҽ�����ʾ�б�
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ� 
*
============================================================
*/

#include "stdafx.h"
#include "GameTest.h"
#include "SkillInfo.h"
#include ".\skillinfo.h"
#include "..\App\SkillMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSkillInfo, CDialog)
CSkillInfo::CSkillInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillInfo::IDD, pParent)
	, m_strSkillInfo(_T(""))
{
	m_pDisplaySkill = NULL;
}
CSkillInfo::~CSkillInfo()
{
	list<tagOccupation*>::iterator it = m_OccupList.begin();
	for(; it != m_OccupList.end(); ++it)
	{
		SAFE_DELETE(*it);
		//*it = NULL;
	}
	m_OccupList.clear();

	vector<tagOccupSkill*>::iterator it2 = m_vecOccSkill.begin();
	for (;it2 != m_vecOccSkill.end();++it2)
	{
		SAFE_DELETE(*it2);
	}
	for( int i=0;i< (int)m_mapSkillListXml.size();++i)
	{
		m_mapSkillListXml[i];
	}
	m_vecOccSkill.clear();

	m_mapSkillListXml.clear();
}
void CSkillInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILL_LIST, m_CtrSkillList);
	DDX_Control(pDX, IDC_SKILL_EDIT, m_edSkillInfo);
	DDX_Text(pDX, IDC_SKILL_EDIT, m_strSkillInfo);
	DDX_Control(pDX, IDC_OccupSkillList, m_OccSkillList);
	DDX_Control(pDX, IDC_Occup, m_ctrlOccup);
}


BEGIN_MESSAGE_MAP(CSkillInfo, CDialog)
	//ON_WM_PAINT()
//	ON_NOTIFY(NM_CLICK, IDC_SKILL_LIST, OnNMClickSkillList)
//	ON_NOTIFY(NM_RCLICK, IDC_SKILL_LIST, OnNMRclickSkillList)
//	ON_COMMAND(ID_SkillInfo_USESKILL, OnSkillinfoUseskill)
END_MESSAGE_MAP()

BOOL CSkillInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	//LoadOccSkillListXml();
	//LoadOneSkillXml();
//TODO:  �ڴ���Ӷ���ĳ�ʼ��
	 m_CtrSkillList.InsertColumn(1,"����",0,70,0);
	 m_CtrSkillList.InsertColumn(2,"�ȼ�",10,50,10);

	 m_OccSkillList.InsertColumn(1,"����",0,70,0);
	 m_OccSkillList.InsertColumn(1,"MaxLve",0,50,0);
	 m_OccSkillList.InsertColumn(2,"��",0,25,0);

	   return TRUE;
}

BOOL CSkillInfo::LoadOccSkillListXml()
{
	char strError[128];
	char strFileName[] = "Data\\OccupSkillList.xml";
	TiXmlNode* pNode = NULL;
	TiXmlDocument doc(strFileName);
	if (!doc.LoadFile())
	{
		wsprintf(strError,"���������ļ�%sʧ��,����",strFileName);
		AfxMessageBox(strError,MB_OK,NULL);
		return FALSE;
	}
	pNode = doc.FirstChild("OccupSkillList");
	if (pNode==NULL)
	{
		wsprintf(strError,"û����OccupSkillList�ڵ�,����");
		MessageBox(strError,MB_OK,NULL); 
		return FALSE;
	}

	//����ת��
	TiXmlElement* pElem = pNode->ToElement();
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if(!strcmp(pChildElem->Value(),"Occup")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("ID"))
		{
			tagOccupation * pOccup = new tagOccupation;
			pOccup->strOccupName = pChildElem->Attribute(_T("name"));
			pOccup->lOccupID = (long)atoi(pChildElem->Attribute(_T("ID")));
			// ְҵ����
			if(NULL!=pChildElem->Attribute(_T("Desc")))
			{
				pOccup->strOccupDesc = pChildElem->Attribute(_T("Desc"));
			}
			// ��ÿ��ְҵ�ļ����б�
			if(Init(pChildElem))
				m_OccupList.push_back(pOccup);
			// ��ȡְҵ�����б�ʧ��
			else
			{
				delete pOccup;
				pOccup = NULL;
			}
		}
	}
	doc.Clear();
	return TRUE;
}
BOOL CSkillInfo::Init(TiXmlElement* pElem)
{
	if ( pElem!=NULL)
	{
		for( TiXmlElement* pChildElem = pElem->FirstChildElement();
			pChildElem != NULL;
			pChildElem = pChildElem->NextSiblingElement() )
		{
			if ( !strcmp(pChildElem->Value(),_T("Skill")) &&
				NULL != pChildElem->Attribute(_T("name")) &&
				NULL != pChildElem->Attribute(_T("ID"))&&
				NULL != pChildElem->Attribute(_T("lev")) )
			{
				tagOccupSkill* ptagOccupSkill = new tagOccupSkill;
				ptagOccupSkill->strSkillName = pChildElem->Attribute("name");
				ptagOccupSkill->lSkillID = (long)atoi(pChildElem->Attribute("ID"));
				ptagOccupSkill->lSkillMaxLeve = (long)atoi(pChildElem->Attribute("lev"));
				m_vecOccSkill.push_back(ptagOccupSkill);

			}
		}
		return TRUE;
	}
	return FALSE;
}
BOOL CSkillInfo::LoadOneSkillXml()
{
	HANDLE hFind;
	CString strFileName;
	CString strDir = "Data\\skills\\";
	WIN32_FIND_DATA dataFind;
	BOOL bMoreFile = TRUE;
	hFind = FindFirstFile(strDir + "\\*.*",&dataFind);
	while(hFind!=INVALID_HANDLE_VALUE && bMoreFile ==TRUE)
	{
		strFileName = strDir + "\\" + dataFind.cFileName;
		if(strFileName.Right(4) == ".xml")
		{
		//	m_OneSkillXmlFileName.push_back(filename);
			LoadSkillConfigXml(strFileName);
		}
		bMoreFile = FindNextFile(hFind,&dataFind);
	}
	return true;
}
BOOL CSkillInfo::LoadSkillConfigXml(const char* filename)
{
	TiXmlElement	*m_pElem=NULL;
	TiXmlNode *pNode = NULL;
	TiXmlDocument doc(filename);
	doc.LoadFile();

	pNode = doc.FirstChild("Root");
	if(NULL==pNode)
	{
		return false;
	}
	m_pElem = pNode->ToElement();

	for(TiXmlElement *pChildElem = m_pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{
		// ���ܽڵ�
		if(!strcmp(pChildElem->Value(),"Skill"))
		{
			tagOneSkill stSkill;
			// ���ܱ��
			if(NULL!=pChildElem->Attribute("id"))
				stSkill.dwSkillID = atoi( pChildElem->Attribute("id"));
			// ��������
			if(NULL!=pChildElem->Attribute("name"))
				stSkill.strSkillName = pChildElem->Attribute("name");
			// ����״̬��һ�㼼�ܡ��⻷�༼�ܣ�
			if(NULL!=pChildElem->Attribute("skillstate"))
				stSkill.dwSkillState = atoi(pChildElem->Attribute("skillstate"));
			// ��������
			if(NULL!=pChildElem->Attribute("skillelement"))
				stSkill.dwSkillElement = atoi(pChildElem->Attribute("skillelement"));
			// ��������
			if(NULL!=pChildElem->Attribute("skilltype"))
				stSkill.dwSkillType = atoi(pChildElem->Attribute("skilltype"));
			// ����ʹ������(����ħ��)
			if(NULL!=pChildElem->Attribute("skillusetype"))
				stSkill.dwSkillUseType = atoi(pChildElem->Attribute("skillusetype"));
			// ��������
			if(NULL!=pChildElem->Attribute("skilldesc"))
				stSkill.strSkillDesc = pChildElem->Attribute("skilldesc");
			// �Ƿ�����Ŀ��
			if(NULL!=pChildElem->Attribute("locktarget"))
				stSkill.bLockTarget = atoi(pChildElem->Attribute("locktarget")) == 0?true : false;
			// �Ƿ�����������
			if(NULL!=pChildElem->Attribute("lockskill"))
				stSkill.bLockSkill =atoi(pChildElem->Attribute("lockskill")) == 0?true:false;
			// ���ܵȼ�����
			for (TiXmlElement *pSkillChildElem = pChildElem->FirstChildElement(); pSkillChildElem!=NULL; pSkillChildElem = pSkillChildElem->NextSiblingElement())
			{
				// ����ְҵ����
				if(!strcmp(pSkillChildElem->Value(), "UseByOcc"))
				{
					for (TiXmlElement *pSkillOccElem = pSkillChildElem->FirstChildElement(); pSkillOccElem != NULL; pSkillOccElem = pSkillOccElem->NextSiblingElement())
					{
						tagSkillOcc stOcc;
						// ְҵ����
						if(NULL != pSkillOccElem->Attribute("occname"))
							stOcc.strOccName = pSkillOccElem->Attribute("occname");
						// ְҵ���
						if(NULL != pSkillOccElem->Attribute("occid"))
							stOcc.dwOccId = atoi(pSkillOccElem->Attribute("occid"));
						stSkill.vecUseByOcc.push_back(stOcc);
					}
				}
				else if(!strcmp(pSkillChildElem->Value(),"Level"))
				{
					tagLevel stlevel;
					// ���ܵȼ�
					if(NULL!=pSkillChildElem->Attribute("skilllevel"))
						stlevel.dwSkill_Level = atoi(pSkillChildElem->Attribute("skilllevel"));
					// ���ܵȼ�����
					if(NULL!=pSkillChildElem->Attribute("skillleveldesc"))
						stlevel.strSkillLevelDesc = pSkillChildElem->Attribute("skillleveldesc");
					// ���ܵȼ���������
					if(NULL!=pSkillChildElem->Attribute("skillleveldescEx"))
						stlevel.strSkillLevelDescEx = pSkillChildElem->Attribute("skillleveldescEx");
					if(NULL!=pSkillChildElem->Attribute("skilllevelai"))
						stlevel.dwSkillLevelAI = atoi(pSkillChildElem->Attribute("skilllevelai"));
					for(TiXmlElement *pStepChildElem = pSkillChildElem->FirstChildElement(); pStepChildElem!=NULL; pStepChildElem = pStepChildElem->NextSiblingElement())
					{
						tagLevelValue stLevelValue;
						// ���ͼ�����֤�׶�
						if(!strcmp(pStepChildElem->Value(),"StepSend"))
						{
							for(TiXmlElement *pStepSendElem = pStepChildElem->FirstChildElement(); pStepSendElem!=NULL; pStepSendElem = pStepSendElem->NextSiblingElement())
							{
								if(NULL!=pStepSendElem->Attribute("value"))
								{
									stLevelValue.strKeyWord = pStepSendElem->Value();
									stLevelValue.dwID = atoi(pStepSendElem->Attribute("id"));
									stLevelValue.dwValue = atoi(pStepSendElem->Attribute("value"));
									stlevel.vecStepSend.push_back(stLevelValue);
								}
							}
						}
						// ���ܿ�ʼ�׶�
						else if(!strcmp(pStepChildElem->Value(),"StepBegin"))
						{
							for (TiXmlElement *pStepBeginElem = pStepChildElem->FirstChildElement(); pStepBeginElem!=NULL; pStepBeginElem = pStepBeginElem->NextSiblingElement())
							{
								if (NULL!=pStepBeginElem->Attribute("value"))
								{
									int ID = atoi(pStepBeginElem->Attribute("id"));
									if(INTONATE_ACT == ID || RELEASE_ACT == ID || INTONATE_MAINACT == ID)
										stSkill.bHasAct = true;
									stLevelValue.strKeyWord = pStepBeginElem->Value();
									stLevelValue.dwID = ID;
									stLevelValue.dwValue = atoi(pStepBeginElem->Attribute("value"));
									stlevel.vecStepBegin.push_back(stLevelValue);
								}
							}
						}
						// ����ʹ�óɹ��׶�
						else if (!strcmp(pStepChildElem->Value(),"StepRun"))
						{
							for (TiXmlElement *pStepRunElem = pStepChildElem->FirstChildElement(); pStepRunElem!=NULL; pStepRunElem = pStepRunElem->NextSiblingElement())
							{
								if(NULL!= pStepRunElem->Attribute("value"))
								{
									int ID = atoi(pStepRunElem->Attribute("id"));
									if(INTONATE_ACT == ID || RELEASE_ACT == ID || INTONATE_MAINACT == ID)
										stSkill.bHasAct = true;
									stLevelValue.strKeyWord = pStepRunElem->Value();
									stLevelValue.dwID = ID;
									stLevelValue.dwValue = atoi(pStepRunElem->Attribute("value"));
									stlevel.vecStepRun.push_back(stLevelValue);
								}
							}
						}
						// ���ܽ����׶�
						else if(!strcmp(pStepChildElem->Value(),"StepEnd"))
						{
							for (TiXmlElement *pStepEndElem = pStepChildElem->FirstChildElement();pStepEndElem!=NULL; pStepEndElem = pStepEndElem->NextSiblingElement())
							{
								if(NULL!= pStepEndElem->Attribute("value"))
								{
									int ID =atoi(pStepEndElem->Attribute("id"));
									if(INTONATE_ACT == ID || RELEASE_ACT == ID || INTONATE_MAINACT == ID)
										stSkill.bHasAct = true;
									stLevelValue.strKeyWord = pStepEndElem->Value();
									stLevelValue.dwID = ID;
									stLevelValue.dwValue = atoi(pStepEndElem->Attribute("value"));
									stlevel.vecStepEnd.push_back(stLevelValue);
								}
							}
						}
						// �ٻ����ܽ׶�
						else
						{
							for (TiXmlElement *pStepSummonElem = pStepChildElem->FirstChildElement();pStepSummonElem!=NULL; pStepSummonElem = pStepSummonElem->NextSiblingElement())
							{
								if(NULL!= pStepSummonElem->Attribute("value"))
								{
									int ID = atoi(pStepSummonElem->Attribute("id"));
									if(INTONATE_ACT == ID || RELEASE_ACT == ID || INTONATE_MAINACT == ID)
										stSkill.bHasAct = true;
									stLevelValue.strKeyWord = pStepSummonElem->Value();
									stLevelValue.dwID = ID;
									stLevelValue.dwValue = atoi(pStepSummonElem->Attribute("value"));
									stlevel.vecStepSummon.push_back(stLevelValue);
								}
							}
						}
						/*else // ���ܳ���״̬��Ч
						{
						for (TiXmlElement *pStepStateElem = pStepChildElem->FirstChildElement();pStepStateElem!=NULL; pStepStateElem = pStepStateElem->NextSiblingElement())
						{
						if(NULL!= pStepStateElem->Attribute("value"))
						{
						stLevelValue.strKeyWord = pStepStateElem->Value();
						stLevelValue.dwValue = CStrConverter::parseInt(pStepStateElem->Attribute("value"));
						stlevel.vecStepState.push_back(stLevelValue);
						}
						}
						}*/
					}
					stSkill.vectorLevels.push_back(stlevel);
				}
			}
			m_mapSkillListXml[stSkill.dwSkillID] = stSkill;
		}
	}
	doc.Clear();
	return true;
}



//void CSkillInfo::ReleaseSkill()
//{
//	m_mapSkillListXml.clear();
//	//if( !m_OccupList.empty())
//	//{
//	//	if (!m_vecOccupSkillList.empty())
//	//	{
//	//		vector<tagOccupSkill*>::iterator pSkill = m_vecOccupSkillList.begin();
//	//		vector<tagOccupSkill*>::iterator pTemp;
//	//		for (;pSkill!=m_vecOccupSkillList.end();)
//	//		{
//	//			pTemp = pSkill;
//	//			++pSkill;
//	//			delete *pTemp;
//	//		}
//	//		m_vecOccupSkillList.clear();
//	//	}
//
//	//	list<tagOccup*>::iterator pOccup = m_OccupList.begin();
//	//	list<tagOccup*>::iterator pTemp2;
//	//	for (;pOccup!=m_OccupList.end();)
//	//	{
//	//		pTemp2 = pOccup;
//	//		++pOccup;
//	//		delete *pTemp2;
//	//	}
//	//	m_OccupList.clear();
//	//}
//
//}
//// ��ʼ������XML�����ļ�
//bool CSkillInfo::Init(const char *filename)
//{
//	if(!filename)	return false;
//	Release();
//	char strError[128];
//	TiXmlNode *pNode = NULL;
//	TiXmlDocument m_Tdoc;
//	//���������Ϣ��������ļ���
//	if(!m_Tdoc.LoadFile(filename))
//	{
//		wsprintf(strError,"���������ļ�%sʧ��,����",filename);
//		MessageBox(NULL,strError,"",MB_OK);
//		return false;
//	}
//	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
//	pNode = m_Tdoc.FirstChild("OccupSkillList");
//	if (pNode==NULL)
//	{
//		wsprintf(strError,"û����OccupSkillList�ڵ�,����");
//		MessageBox(NULL,strError,"",MB_OK); 
//		return false;
//	}
//	//����ת��
//	TiXmlElement* pElem = pNode->ToElement();
//	// �����ӽڵ㣬��������
//	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
//		pChildElem=pChildElem->NextSiblingElement())
//	{
//		if(!strcmp(pChildElem->Value(),"Occup")
//			&& NULL!=pChildElem->Attribute("name")
//			&& NULL!=pChildElem->Attribute("ID"))
//		{
//			COccupSkill * pOccupSkill = new COccupSkill;
//			pOccupSkill->SetName(pChildElem->Attribute("name"));
//			pOccupSkill->SetOccupId((long)atoi(pChildElem->Attribute("ID")));
//			// ְҵ����
//			if(NULL!=pChildElem->Attribute("Desc"))
//			{
//				pOccupSkill->SetDesc(pChildElem->Attribute("Desc"));
//			}
//			// ��ÿ��ְҵ�ļ����б�
//			if(pOccupSkill->Init(pChildElem))
//				m_OccupSkillList.push_back(pOccupSkill);
//			// ��ȡְҵ�����б�ʧ��
//			else
//			{
//				delete pOccupSkill;
//				pOccupSkill = NULL;
//			}
//		}
//	}
//	m_Tdoc.Clear();
//	return true;
//}
//

//
//BOOL CSkillInfo::LoadSkillListXml()
//{
//	ReleaseSkill();
//	string strFile = "Data\\OccupSkillList.xml";
//	TiXmlNode *pNode = NULL;
//	TiXmlDocument doc(strFile.c_str());
//	if( !doc.LoadFile())
//	{
//		PutDebugString("װ�������ļ�<<OccupSkillList.xml>>����");
//		return FALSE;
//	}
//	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
//	pNode = doc.FirstChild("OccupSkillList");
//	if (pNode==NULL)
//	{
//		PutDebugString("û����OccupSkillList�ڵ�,���飡");
//		return FALSE;
//	}
//	//����ת��
//	TiXmlElement* pElem = pNode->ToElement();
//	// �����ӽڵ㣬��������
//	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
//		pChildElem=pChildElem->NextSiblingElement())
//	{
//		if(!strcmp(pChildElem->Value(),_T("Occup"))
//			&& NULL!=pChildElem->Attribute(_T("name"))
//			&& NULL!=pChildElem->Attribute(_T("ID")))
//		{
//			tagOccup * pOccupSkill = new tagOccup;
//			pOccupSkill->strOccName = pChildElem->Attribute("name");
//			pOccupSkill->dwOccID = (long)atoi(pChildElem->Attribute("ID"));
//			// ְҵ����
//			if(NULL!=pChildElem->Attribute("Desc"))
//			{
//				pOccupSkill->strOccDesc = (pChildElem->Attribute("Desc"));
//			}
//			// ��ÿ��ְҵ�ļ����б�
//			if(LoadSkillListXml())
//				m_OccupList.push_back(pOccupSkill);
//			// ��ȡְҵ�����б�ʧ��
//			else
//			{
//				delete pOccupSkill;
//				pOccupSkill = NULL;
//			}
//		}
//	}
//	doc.Clear();
//}
//
//// ��ȡ���м��������ļ�
//BOOL CSkillInfo::LoadSkillListXml()
//{
//	HANDLE hFind;
//	CString filename;
//	CString filepath = "Data\\skills\\";
//	WIN32_FIND_DATA dataFind;
//	BOOL bMoreFile = TRUE;
//	hFind = FindFirstFile(filepath+"\\*.*",&dataFind);
//	while(hFind!=INVALID_HANDLE_VALUE && bMoreFile ==TRUE)
//	{
//		filename = filepath +"\\" + dataFind.cFileName;
//		if(filename.Right(4) == ".xml")
//		{
//			m_OneSkillXmlFileName.push_back(filename);
//			LoadOneSkillXml(filename);
//		}
//		bMoreFile = FindNextFile(hFind,&dataFind);
//	}
//	return true;
//}
//
//// ��ȡ�������������ļ�
//BOOL CSkillInfo::LoadOneSkillXml(const char* filename)
//{
//	
//	TiXmlElement	*m_pElem=NULL;
//	TiXmlNode *pNode = NULL;
//	TiXmlDocument doc(filename);
//	doc.LoadFile();
//
//	pNode = doc.FirstChild("Root");
//	if(NULL==pNode)
//	{
//		return false;
//	}
//	m_pElem = pNode->ToElement();
//	TiXmlElement *pChildElem = m_pElem->FirstChildElement();
//	for(; pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
//	{
//		// ���ܽڵ�
//		if(!strcmp(pChildElem->Value(),_T("Skill")))
//		{
//			tagSkill stSkill;
//			// ���ܱ��
//			if(NULL!=pChildElem->Attribute(_T("id")))
//				stSkill.dwSkillID = atoi(pChildElem->Attribute(_T("id")));
//			// ��������
//			if(NULL!=pChildElem->Attribute(_T("name")))
//				stSkill.strSkillName = pChildElem->Attribute(_T("name"));
//			//// ����״̬��һ�㼼�ܡ��⻷�༼�ܣ�
//			if(NULL!=pChildElem->Attribute(_T("skillstate")))
//				stSkill.dwSkillState = atoi(pChildElem->Attribute(_T("skillstate")));
//			// ��������
//			if(NULL!=pChildElem->Attribute(_T("skillelement")))
//				stSkill.dwSkillElement = atoi(pChildElem->Attribute(_T("skillelement")));
//			// ��������
//			if(NULL!=pChildElem->Attribute(_T("skilltype")))
//				stSkill.dwSkillType = atoi(pChildElem->Attribute(_T("skilltype")));
//			// ����ʹ������(����ħ��)
//			if(NULL!=pChildElem->Attribute(_T("skillusetype")))
//				stSkill.dwSkillUseType = atoi(pChildElem->Attribute(_T("skillusetype")));
//			// ��������
//			if(NULL!=pChildElem->Attribute(_T("skilldesc")))
//				stSkill.strSkillDesc = pChildElem->Attribute(_T("skilldesc"));
//			// �Ƿ�����Ŀ��
//			if(NULL!=pChildElem->Attribute(_T("locktarget")))
//				stSkill.bLockTarget = pChildElem->Attribute(_T("locktarget"))==0?false:true;
//			// �Ƿ�����������
//			if(NULL!=pChildElem->Attribute(_T("lockskill")))
//				stSkill.bLockSkill = pChildElem->Attribute(_T("lockskill"))==0?true:false;
//			// ���ܵȼ�����
//			for (TiXmlElement *pSkillChildElem = pChildElem->FirstChildElement(); pSkillChildElem!=NULL; pSkillChildElem = pSkillChildElem->NextSiblingElement())
//			{
//				// ����ְҵ����
//				if(!strcmp(pSkillChildElem->Value(), _T("UseByOcc")))
//				{
//					for (TiXmlElement *pSkillOccElem = pSkillChildElem->FirstChildElement(); pSkillOccElem != NULL; pSkillOccElem = pSkillOccElem->NextSiblingElement())
//					{
//						tagSkillOcc stOcc;
//						// ְҵ����
//						if(NULL != pSkillOccElem->Attribute(_T("occname")))
//							stOcc.strOccName = pSkillOccElem->Attribute(_T("occname"));
//						// ְҵ���
//						if(NULL != pSkillOccElem->Attribute(_T("occid")))
//							stOcc.dwOccId = atoi(pSkillOccElem->Attribute(_T("occid")));
//						stSkill.vecUseByOcc.push_back(stOcc);
//					}
//				}
//				else if(!strcmp(pSkillChildElem->Value(),_T("Level")))
//				{
//					tagLevel stlevel;
//					// ���ܵȼ�
//					if(NULL!=pSkillChildElem->Attribute(_T("skilllevel")))
//						stlevel.dwSkill_Level = atoi(pSkillChildElem->Attribute(_T("skilllevel")));
//					// ���ܵȼ�����
//					if(NULL!=pSkillChildElem->Attribute(_T("skillleveldesc")))
//						stlevel.strSkillLevelDesc = pSkillChildElem->Attribute(_T("skillleveldesc"));
//					// ���ܵȼ���������
//					if(NULL!=pSkillChildElem->Attribute(_T("skillleveldescEx")))
//						stlevel.strSkillLevelDescEx = pSkillChildElem->Attribute(_T("skillleveldescEx"));
//					if(NULL!=pSkillChildElem->Attribute(_T("skilllevelai")))
//						stlevel.dwSkillLevelAI =atoi(pSkillChildElem->Attribute(_T("skilllevelai")));
//					for(TiXmlElement *pStepChildElem = pSkillChildElem->FirstChildElement(); pStepChildElem!=NULL; pStepChildElem = pStepChildElem->NextSiblingElement())
//					{
//						tagLevelValue stLevelValue;
//						// ���ͼ�����֤�׶�
//						if(!strcmp(pStepChildElem->Value(),_T("StepSend")))
//						{
//							for(TiXmlElement *pStepSendElem = pStepChildElem->FirstChildElement(); pStepSendElem!=NULL; pStepSendElem = pStepSendElem->NextSiblingElement())
//							{
//								if(NULL!=pStepSendElem->Attribute(_T("value")))
//								{
//									stLevelValue.strKeyWord = pStepSendElem->Value();
//									stLevelValue.dwID = atoi(pStepSendElem->Attribute(_T("id")));
//									stLevelValue.dwValue = atoi(pStepSendElem->Attribute(_T("value")));
//									stlevel.vecStepSend.push_back(stLevelValue);
//								}
//							}
//						}
//						// ���ܿ�ʼ�׶�
//						else if(!strcmp(pStepChildElem->Value(),_T("StepBegin")))
//						{
//							for (TiXmlElement *pStepBeginElem = pStepChildElem->FirstChildElement(); pStepBeginElem!=NULL; pStepBeginElem = pStepBeginElem->NextSiblingElement())
//							{
//								if (NULL!=pStepBeginElem->Attribute(_T("value")))
//								{
//									stLevelValue.strKeyWord = pStepBeginElem->Value();
//									stLevelValue.dwID = atoi(pStepBeginElem->Attribute(_T("id")));
//									stLevelValue.dwValue = atoi(pStepBeginElem->Attribute(_T("value")));
//									stlevel.vecStepBegin.push_back(stLevelValue);
//								}
//							}
//						}
//						// ����ʹ�óɹ��׶�_T(
//						else if (!strcmp(pStepChildElem->Value(),_T("StepRun")))
//						{
//							for (TiXmlElement *pStepRunElem = pStepChildElem->FirstChildElement(); pStepRunElem!=NULL; pStepRunElem = pStepRunElem->NextSiblingElement())
//							{
//								if(NULL!= pStepRunElem->Attribute(_T("value")))
//								{
//									stLevelValue.strKeyWord = pStepRunElem->Value();
//									stLevelValue.dwID = atoi(pStepRunElem->Attribute(_T("id")));
//									stLevelValue.dwValue = atoi(pStepRunElem->Attribute(_T("value")));
//									stlevel.vecStepRun.push_back(stLevelValue);
//								}
//							}
//						}
//						// ���ܽ����׶�
//						else if(!strcmp(pStepChildElem->Value(),_T("StepEnd")))
//						{
//							for (TiXmlElement *pStepEndElem = pStepChildElem->FirstChildElement();pStepEndElem!=NULL; pStepEndElem = pStepEndElem->NextSiblingElement())
//							{
//								if(NULL!= pStepEndElem->Attribute(_T("value")))
//								{
//									stLevelValue.strKeyWord = pStepEndElem->Value();
//									stLevelValue.dwID = atoi(pStepEndElem->Attribute(_T("id")));
//									stLevelValue.dwValue = atoi(pStepEndElem->Attribute(_T("value")));
//									stlevel.vecStepEnd.push_back(stLevelValue);
//								}
//							}
//						}
//						// �ٻ����ܽ׶�
//						else
//						{
//							for (TiXmlElement *pStepSummonElem = pStepChildElem->FirstChildElement();pStepSummonElem!=NULL; pStepSummonElem = pStepSummonElem->NextSiblingElement())
//							{
//								if(NULL!= pStepSummonElem->Attribute(_T("value")))
//								{
//									stLevelValue.strKeyWord = pStepSummonElem->Value();
//									stLevelValue.dwID = atoi(pStepSummonElem->Attribute(_T("id")));
//									stLevelValue.dwValue = atoi(pStepSummonElem->Attribute(_T("value")));
//									stlevel.vecStepSummon.push_back(stLevelValue);
//								}
//							}
//						}
//					}
//					stSkill.vectorLevels.push_back(stlevel);
//				}
//			}
//			m_SkillListXml.push_back(stSkill);
//		}
//	}
//	doc.Clear();
//	return true;
//}
//
void CSkillInfo::SetDisplaySkill(CPlayer* pPlayer)
{
	if ( pPlayer == NULL ) return;
	m_CtrSkillList.DeleteAllItems();
	CString strSkill;
	CString strOccupType1("");
	CString strOccupType2("");
	//ְҵ
	long lOccupType = pPlayer->GetOccupation();
	strOccupType1.Format("%d",lOccupType);

	//list<tagSkill>::iterator OccSkill;																			//�����ļ��е����м���

	list<tagPlayerSkill>::iterator pStudyedSkill = pPlayer->GetSkillMgr()->GetSkillList().begin();					//���ѧ��ļ���

	int i =0 ;

	for ( ; pStudyedSkill!=pPlayer->GetSkillMgr()->GetSkillList().end(); ++pStudyedSkill)
	{
// 		for( OccSkill = GetOneSkillXmlProperty().begin() ;OccSkill!=GetOneSkillXmlProperty().end();++OccSkill)
// 		{
// 			if ( pStudyedSkill->dwID == OccSkill->dwSkillID )
// 			{
// 				if (pStudyedSkill->dwID>=10002 && pStudyedSkill->dwID <=10010)
// 					continue;
// 				m_CtrSkillList.InsertItem(i,OccSkill->strSkillName.c_str());
// 				char str[8] = "";
// 				sprintf(str,"%d",pStudyedSkill->wLevel);
// 				m_CtrSkillList.SetItemText(i,1,str);
// 				break;
// 			}
//		}
		char str[8] = "";
		sprintf(str,"%d",pStudyedSkill->dwID);
		m_CtrSkillList.SetItemText(i,1,str);
	}
}

BOOL CSkillInfo::QuerySkill(tagPlayerSkill* pSkill)
{
	for (vector<tagOccupSkill*>::iterator it = m_vecOccSkill.begin(); it!= m_vecOccSkill.end(); ++it )
	{
		if ( (*it)->lSkillID == pSkill->dwID )
		{
			return TRUE;
		}
	}
	return FALSE;
}