#include "StdAfx.h"
#include "SkillListXml.h"
#include <sstream>
#include "../../../Public/Common/CStrConverter.h"
#include "../../../Public/Common/SkillRelated.h"
#include "SkillListIni.h"
#include "SkillIni.h"
#include "../Other/GlobeSetup.h"
#include "../../Game.h"
#include "../MsgBoxSys/MsgEventManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

map<DWORD, CSkillListXml::tagSkill> CSkillListXml::m_mapSkillListXml;

CSkillListXml::CSkillListXml(void)
{
}

CSkillListXml::~CSkillListXml(void)
{
	Release();
}

void CSkillListXml::Release()
{
	m_mapSkillListXml.clear();
}
// ��ȡ���м��������ļ�
BOOL CSkillListXml::LoadSkillListXml()
{
	Release();
	//=================================================================//
	/*CRFile *prfile = rfOpen("data/skills/SkillList.xml");
	if(NULL == prfile)
	{
	//���������Ϣ
	char strError[128];
	wsprintf(strError,"LoadFileError:%s","SkillList.xml");
	MessageBox(NULL,strError,"",MB_OK); 
	return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument doc("SkillList.xml");
	//���뼼�������ļ�
	if( !doc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
	//���������Ϣ
	char strError[128];
	wsprintf(strError,"LoadFileError:%s","SkillList.xml");
	MessageBox(NULL,strError,"",MB_OK); 
	rfClose(prfile);
	return false;
	}
	rfClose(prfile);
	//=================================================================//
	TiXmlElement	*m_pElem=NULL;
	pNode = doc.FirstChild("SkillList");
	if(NULL==pNode)
	{
	return false;
	}
	m_pElem = pNode->ToElement();

	for(TiXmlElement *pChildElem = m_pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
	{
	// ���ܽڵ�
	if(!strcmp(pChildElem->Value(),"SkillFile") && NULL!=pChildElem->Attribute("name"))
	{
	string str = "data/skills/";
	str += pChildElem->Attribute("name");
	LoadOneSkillXml(str.c_str());
	}
	}
	*/
	//list<string> listFileName;
	//GetGame()->FindFileList(dirname, ".xml", listFileName);

	//for(list<string>::iterator it=listFileName.begin(); it!=listFileName.end(); it++)
	//{
	//	const char* filename = it->c_str();
	//	LoadOneSkillXml(filename);
	//}
	/***********************************************************************/
	/* zhaohang add 2010-9-27 */
	/* ��������*/
	/***********************************************************************/
	SkillAttrIniSetup::LoadFile(CGlobeSetup::GetSetup()->strSkillAttrIniSetup);
	SynchroData();
	return true;
}

// ��ȡ�������������ļ�
//BOOL CSkillListXml::LoadOneSkillXml(const char* filename)
//{
//	//=================================================================//
//	CRFile *prfile = rfOpen(filename);
//	if(NULL == prfile)
//	{
//		//���������Ϣ
//		char strError[128];
//		wsprintf(strError,"LoadFileError:%s",filename);
//		MessageBox(NULL,strError,"",MB_OK); 
//		return false;
//	}
//	TiXmlNode *pNode = NULL; 		
//	TiXmlDocument doc(filename);
//	//���뼼�������ļ�
//	if( !doc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
//	{
//		//���������Ϣ
//		char strError[128];
//		wsprintf(strError,"LoadFileError:%s",filename);
//		MessageBox(NULL,strError,"",MB_OK); 
//		rfClose(prfile);
//		return false;
//	}
//	rfClose(prfile);
//	//=================================================================//
//	TiXmlElement	*m_pElem=NULL;
//	pNode = doc.FirstChild("Root");
//	if(NULL==pNode)
//	{
//		return false;
//	}
//	m_pElem = pNode->ToElement();
//
//	for(TiXmlElement *pChildElem = m_pElem->FirstChildElement(); pChildElem!=NULL; pChildElem = pChildElem->NextSiblingElement())
//	{
//		// ���ܽڵ�
//		if(!strcmp(pChildElem->Value(),"Skill"))
//		{
//			tagSkill stSkill;
//			// ���ܱ��
//			if(NULL!=pChildElem->Attribute("id"))
//				stSkill.dwSkillID = CStrConverter::parseInt(pChildElem->Attribute("id"));
//			// ��������
//			if(NULL!=pChildElem->Attribute("name"))
//				stSkill.strSkillName = pChildElem->Attribute("name");
//			// ����״̬��һ�㼼�ܡ��⻷�༼�ܣ�
//			if(NULL!=pChildElem->Attribute("skillstate"))
//				stSkill.dwSkillState = CStrConverter::parseInt(pChildElem->Attribute("skillstate"));
//			// ��������
//			if(NULL!=pChildElem->Attribute("skillelement"))
//				stSkill.dwSkillElement = CStrConverter::parseInt(pChildElem->Attribute("skillelement"));
//			// ��������
//			if(NULL!=pChildElem->Attribute("skilltype"))
//				stSkill.dwSkillType = CStrConverter::parseInt(pChildElem->Attribute("skilltype"));
//			// ����ʹ������(����ħ��)
//			if(NULL!=pChildElem->Attribute("skillusetype"))
//				stSkill.dwSkillUseType = CStrConverter::parseInt(pChildElem->Attribute("skillusetype"));
//			// �����ü���ʱ�������λ����ʾ����ЧID���������˸�ֵ����ʾΪ����ķ�Χѡ���ܣ�
//			if(NULL!=pChildElem->Attribute("effectid"))
//				stSkill.dwEffectID = CStrConverter::parseInt(pChildElem->Attribute("effectid"));
//			// ��������
//			if(NULL!=pChildElem->Attribute("skilldesc"))
//				stSkill.strSkillDesc = pChildElem->Attribute("skilldesc");
//			// �Ƿ�����Ŀ��
//			if(NULL!=pChildElem->Attribute("locktarget"))
//				stSkill.bLockTarget = CStrConverter::parseBool(pChildElem->Attribute("locktarget"));
//			// �Ƿ�����������
//			if(NULL!=pChildElem->Attribute("lockskill"))
//				stSkill.bLockSkill = CStrConverter::parseBool(pChildElem->Attribute("lockskill"));
//			// �Ƿ���Զ��������Ķ���ʹ��
//			if(NULL!=pChildElem->Attribute("isusetodiedobj"))
//				stSkill.bIsUseToDiedObj = CStrConverter::parseBool(pChildElem->Attribute("isusetodiedobj"));
//			// �Ƿ��������༼��(�ͷŽ׶ε���������)
//			if(NULL!=pChildElem->Attribute("islead"))
//				stSkill.bIsLead = CStrConverter::parseBool(pChildElem->Attribute("islead"));
//			// ���ܵȼ�����
//			for (TiXmlElement *pSkillChildElem = pChildElem->FirstChildElement(); pSkillChildElem!=NULL; pSkillChildElem = pSkillChildElem->NextSiblingElement())
//			{
//				// ����ְҵ����
//				if(!strcmp(pSkillChildElem->Value(), "UseByOcc"))
//				{
//					for (TiXmlElement *pSkillOccElem = pSkillChildElem->FirstChildElement(); pSkillOccElem != NULL; pSkillOccElem = pSkillOccElem->NextSiblingElement())
//					{
//						tagSkillOcc stOcc;
//						// ְҵ����
//						if(NULL != pSkillOccElem->Attribute("occname"))
//							stOcc.strOccName = pSkillOccElem->Attribute("occname");
//						// ְҵ���
//						if(NULL != pSkillOccElem->Attribute("occid"))
//							stOcc.dwOccId = CStrConverter::parseInt(pSkillOccElem->Attribute("occid"));
//						stSkill.vecUseByOcc.push_back(stOcc);
//					}
//				}
//				else if(!strcmp(pSkillChildElem->Value(),"Level"))
//				{
//					tagLevel stlevel;
//					// ���ܵȼ�
//					if(NULL!=pSkillChildElem->Attribute("skilllevel"))
//						stlevel.dwSkill_Level = CStrConverter::parseInt(pSkillChildElem->Attribute("skilllevel"));
//					// ���ܵȼ�����
//					if(NULL!=pSkillChildElem->Attribute("skillleveldesc"))
//						stlevel.strSkillLevelDesc = pSkillChildElem->Attribute("skillleveldesc");
//					// ���ܵȼ���������
//					if(NULL!=pSkillChildElem->Attribute("skillleveldescEx"))
//						stlevel.strSkillLevelDescEx = pSkillChildElem->Attribute("skillleveldescEx");
//					if(NULL!=pSkillChildElem->Attribute("skilllevelai"))
//						stlevel.dwSkillLevelAI = CStrConverter::parseInt(pSkillChildElem->Attribute("skilllevelai"));
//					for(TiXmlElement *pStepChildElem = pSkillChildElem->FirstChildElement(); pStepChildElem!=NULL; pStepChildElem = pStepChildElem->NextSiblingElement())
//					{
//						tagLevelValue stLevelValue;
//						// ���ͼ�����֤�׶�
//						if(!strcmp(pStepChildElem->Value(),"StepSend"))
//						{
//							for(TiXmlElement *pStepSendElem = pStepChildElem->FirstChildElement(); pStepSendElem!=NULL; pStepSendElem = pStepSendElem->NextSiblingElement())
//							{
//								if(NULL!=pStepSendElem->Attribute("value"))
//								{
//									stLevelValue.strKeyWord = pStepSendElem->Value();
//									stLevelValue.dwID = CStrConverter::parseInt(pStepSendElem->Attribute("id"));
//									stLevelValue.dwValue = CStrConverter::parseInt(pStepSendElem->Attribute("value"));
//									if(pStepSendElem->Attribute("strValue"))
//										stLevelValue.strValue = pStepSendElem->Attribute("strValue");
//									stlevel.vecStepSend.push_back(stLevelValue);
//								}
//							}
//						}
//						// ���ܿ�ʼ�׶�
//						else if(!strcmp(pStepChildElem->Value(),"StepBegin"))
//						{
//							for (TiXmlElement *pStepBeginElem = pStepChildElem->FirstChildElement(); pStepBeginElem!=NULL; pStepBeginElem = pStepBeginElem->NextSiblingElement())
//							{
//								if (NULL!=pStepBeginElem->Attribute("value"))
//								{
//									int ID = CStrConverter::parseInt(pStepBeginElem->Attribute("id"));
//									if(INTONATE_ACT == ID || RELEASE_ACT == ID || 
//										INTONATE_MAINACT == ID || RELEASE_ACTEX == ID)
//										stSkill.bHasAct = true;
//									stLevelValue.strKeyWord = pStepBeginElem->Value();
//									stLevelValue.dwID = ID;
//									stLevelValue.dwValue = CStrConverter::parseInt(pStepBeginElem->Attribute("value"));
//									if(pStepBeginElem->Attribute("strValue"))
//										stLevelValue.strValue = pStepBeginElem->Attribute("strValue");
//									stlevel.vecStepBegin.push_back(stLevelValue);
//								}
//							}
//						}
//						// ����ʹ�óɹ��׶�
//						else if (!strcmp(pStepChildElem->Value(),"StepRun"))
//						{
//							for (TiXmlElement *pStepRunElem = pStepChildElem->FirstChildElement(); pStepRunElem!=NULL; pStepRunElem = pStepRunElem->NextSiblingElement())
//							{
//								if(NULL!= pStepRunElem->Attribute("value"))
//								{
//									int ID = CStrConverter::parseInt(pStepRunElem->Attribute("id"));
//									if(INTONATE_ACT == ID || RELEASE_ACT == ID || 
//										INTONATE_MAINACT == ID || RELEASE_ACTEX == ID)
//										stSkill.bHasAct = true;
//									stLevelValue.strKeyWord = pStepRunElem->Value();
//									stLevelValue.dwID = ID;
//									stLevelValue.dwValue = CStrConverter::parseInt(pStepRunElem->Attribute("value"));
//									if(pStepRunElem->Attribute("strValue"))
//										stLevelValue.strValue = pStepRunElem->Attribute("strValue");
//									stlevel.vecStepRun.push_back(stLevelValue);
//								}
//							}
//						}
//						// ���ܽ����׶�
//						else if(!strcmp(pStepChildElem->Value(),"StepEnd"))
//						{
//							for (TiXmlElement *pStepEndElem = pStepChildElem->FirstChildElement();pStepEndElem!=NULL; pStepEndElem = pStepEndElem->NextSiblingElement())
//							{
//								if(NULL!= pStepEndElem->Attribute("value"))
//								{
//									int ID = CStrConverter::parseInt(pStepEndElem->Attribute("id"));
//									if(INTONATE_ACT == ID || RELEASE_ACT == ID || 
//										INTONATE_MAINACT == ID || RELEASE_ACTEX == ID)
//										stSkill.bHasAct = true;
//									stLevelValue.strKeyWord = pStepEndElem->Value();
//									stLevelValue.dwID = ID;
//									stLevelValue.dwValue = CStrConverter::parseInt(pStepEndElem->Attribute("value"));
//									if(pStepEndElem->Attribute("strValue"))
//										stLevelValue.strValue = pStepEndElem->Attribute("strValue");
//									stlevel.vecStepEnd.push_back(stLevelValue);
//								}
//							}
//						}
//						// �ٻ����ܽ׶�
//						else
//						{
//							for (TiXmlElement *pStepSummonElem = pStepChildElem->FirstChildElement();pStepSummonElem!=NULL; pStepSummonElem = pStepSummonElem->NextSiblingElement())
//							{
//								if(NULL!= pStepSummonElem->Attribute("value"))
//								{
//									int ID = CStrConverter::parseInt(pStepSummonElem->Attribute("id"));
//									if(INTONATE_ACT == ID || RELEASE_ACT == ID || 
//										INTONATE_MAINACT == ID || RELEASE_ACTEX == ID)
//										stSkill.bHasAct = true;
//									stLevelValue.strKeyWord = pStepSummonElem->Value();
//									stLevelValue.dwID = ID;
//									stLevelValue.dwValue = CStrConverter::parseInt(pStepSummonElem->Attribute("value"));
//									if(pStepSummonElem->Attribute("strValue"))
//										stLevelValue.strValue = pStepSummonElem->Attribute("strValue");
//									stlevel.vecStepSummon.push_back(stLevelValue);
//								}
//							}
//						}
//						/*else // ���ܳ���״̬��Ч
//						{
//						for (TiXmlElement *pStepStateElem = pStepChildElem->FirstChildElement();pStepStateElem!=NULL; pStepStateElem = pStepStateElem->NextSiblingElement())
//						{
//						if(NULL!= pStepStateElem->Attribute("value"))
//						{
//						stLevelValue.strKeyWord = pStepStateElem->Value();
//						stLevelValue.dwValue = CStrConverter::parseInt(pStepStateElem->Attribute("value"));
//						stlevel.vecStepState.push_back(stLevelValue);
//						}
//						}
//						}*/
//					}
//					stSkill.vectorLevels.push_back(stlevel);
//				}
//			}
//			m_mapSkillListXml[stSkill.dwSkillID] = stSkill;
//		}
//	}
//	doc.Clear();
//	return true;
//}
CSkillListXml::tagSkill* CSkillListXml::GetProperty(DWORD dwIndex)
{
	return m_mapSkillListXml.find(dwIndex)!=m_mapSkillListXml.end() ? &m_mapSkillListXml[dwIndex] : NULL;
}
CSkillListXml::tagSkill* CSkillListXml::GetPropertyByName(const char* strName)
{
	for (map<DWORD, tagSkill>::iterator it = m_mapSkillListXml.begin(); it != m_mapSkillListXml.end(); it++)
	{

		if ((*it).second.strSkillName == strName)
		{
			return &(*it).second;
		}
	}
	return NULL;
}

// �жϴ���ļ��ܣ��Ƿ���ָ��ְҵ�¿���ʹ��
BOOL CSkillListXml::OccuJudgeStudy(DWORD dwID,long lOCcupID)
{
	CSkillListXml::tagSkill* pSkill = GetProperty(dwID);
	if(pSkill)
	{
		vector<CSkillListXml::tagSkillOcc>::iterator it=pSkill->vecUseByOcc.begin();
		for (;it!=pSkill->vecUseByOcc.end();++it)
		{
			if ((*it).dwOccId==lOCcupID)
			{
				return true;
			}
		}	
	}
	return false;
}

/***********************************************************************/
/* zhaohang add 2010-9-27 */
/***********************************************************************/
void CSkillListXml::SynchroData()
{
	Release();
	//�����İ�����
	SkillDocIni* pSkillDocIni = GetGame()->GetSkillDocConfig();
	//�������������ļ���
	SkillAttrIniSetup::SkillAttrMap::iterator iter = SkillAttrIniSetup::m_SkillAttrMap.begin();
	for ( ; iter != SkillAttrIniSetup::m_SkillAttrMap.end() ; ++iter)
	{
		//������������
		tagSkillAttr* ptSkillIni = &((*iter).second);
		uint id = (*iter).first;
		//�����İ�����
		tagSkillDoc* ptSkillDoc = pSkillDocIni->GetSkillDoc(id);
		if(!ptSkillDoc)
		{
            char strTemp[256];
            sprintf(strTemp,AppFrame::GetText("SDCC_64"),id);
            GetInst(MsgEventManager).PushEvent(Msg_Ok,strTemp,NULL,NULL,false);
            Log4c::Trace(ROOT_MODULE,strTemp);
		}
		//��������CSkillListXml::tagSkill����
		tagSkill*    pSkill = GetProperty(id);
		bool isNewSkill = false;
		tagSkill tempskill;
		if(pSkill == NULL)
		{
			pSkill = &tempskill;
			isNewSkill = true;
		}
		//���ܱ��
		pSkill->dwSkillID = ptSkillIni->GetBaseAttr()->GetID();
		//��������
		pSkill->strSkillName = ptSkillIni->GetBaseAttr()->GetName();
		//��������
		if(ptSkillDoc)
			pSkill->strSkillName = ptSkillDoc->GetSkillName();
		//����״̬
		pSkill->dwSkillState = 0 ; 
		//��������
		//pSkill->dwSkillElement = 0;
		//��������
		pSkill->dwSkillType = ptSkillIni->GetBaseAttr()->GetSkillType();
		//����ʹ������
		//pSkill->dwSkillUseType = 0;
		//�������λ����ʾ����Ч
		//pSkill->dwEffectID = 0;
		//�Ƿ�����Ŀ��
		pSkill->bLockTarget = ptSkillIni->GetBaseAttr()->IsLockTarget();
		//�Ƿ���������
		pSkill->bLockSkill = ptSkillIni->GetBaseAttr()->IsLockSkill();
		//�Ƿ�ӵ�ж���
		pSkill->bHasAct = ptSkillIni->IsHasAct();
		//�Ƿ���Զ�����������ʹ��
		//pSkill->bIsUseToDiedObj = 0;
		//�Ƿ��������༼��
		//pSkill->bIsLead = 0;
		//����ְҵ�б�
		//pSkill->vecUseByOcc = 0;

		//���ܵȼ�����
		for(uint i = 0 ;  i < ptSkillIni->mLevelAttrVec.size() ; ++i)
		{
			tagSkillLevelAttr* ptLevelStep = ptSkillIni->GetLevelAttrByIndex(i);
			//��ȡ��Ӧ�ĵȼ�
			tagLevel* ptagLevel = pSkill->GettagLevel(ptLevelStep->GetLevel());
			bool isnewtaglevel = false;
			tagLevel tempLevel;
			if( ptagLevel == NULL)
			{
				//û�о�ָ���¼����Ǹ�
				ptagLevel = &tempLevel;
				isnewtaglevel = true;
			}
			//��������
			//�ȼ�
			uint level = ptLevelStep->GetLevel();
			ptagLevel->dwSkill_Level = level;
			//���ܵȼ�����
			if(ptSkillDoc)
			{
				tagSkillLD* ptSkillLD = ptSkillDoc->GetLevel(level);
				if(!ptSkillLD)
				{
					char strTemp[256];
					sprintf(strTemp,AppFrame::GetText("SDCC_65"),id,level);
					GetInst(MsgEventManager).PushEvent(Msg_Ok,strTemp);
				}
				else
					ptagLevel->strSkillLevelDesc = ptSkillLD->GetDesc();
			}
			//���ܵȼ���������
			//ptagLevel->strSkillLevelDescEx = "";
			//�ȼ�Ai
			ptagLevel->dwSkillLevelAI = ptLevelStep->GetAiType();

			////////////////
			//���ͼ�����֤�׶���������ֻ���ͷŶ����ǿ���ͷŶ���

			//�����������֤�׶�
			ptagLevel->vecStepSend.clear();
			//�ͷŶ���
			for(uint i = 0 ; i < ptLevelStep->mObjVec.size() ; ++i)
			{
				tagLevelValue value;
				//�ͷŶ���ID��ԭ�������ж���Ϊ��
				value.dwID = SKILL_OBJECT;
				value.dwValue = ptLevelStep->mObjVec[i];
				value.strKeyWord = "SkillObject";
				//value.strValue = ;
				ptagLevel->vecStepSend.push_back(value);
			}
			//ǿ���ͷŶ���
			for (uint i = 0; i < ptLevelStep->mForceObjVec.size() ; ++i)
			{
				tagLevelValue value;
				value.dwID = SKILL_FORCE_OBJECT;
				value.dwValue = ptLevelStep->mForceObjVec[i];
				//����������ʱû���õ�
				value.strKeyWord = "SkillForceObject";
				//value.strValue = ;
				ptagLevel->vecStepSend.push_back(value);
			}
			//�Ƿ��ƽ״̬�ſ�ʹ��
			tagLevelValue startstateValue;
			startstateValue.dwID = SKILL_STAR_STATE;
			startstateValue.strKeyWord = "SkillStarState";
			startstateValue.dwValue = ptLevelStep->GetStartState();
			if(ptLevelStep->GetStartState() != -1)
				ptagLevel->vecStepSend.push_back(startstateValue);
			///////////////////////
			//���ܿ�ʼ�׶Σ����µ�����Step
			//�������ʼ�׶�
			ptagLevel->vecStepBegin.clear();
			tagIntonateStp* ptInStp = &(ptLevelStep->mIntoStp);
			if(ptLevelStep->IsHasIntoStp())//�������׶�����
			{
				tagLevelValue IntoValue;
				//��������
				IntoValue.dwID = INTONATE_ACT;
				IntoValue.strKeyWord = "IntonateAct";
				IntoValue.dwValue = ptInStp->mBaseAttr.GetActID();
				if(ptInStp->mBaseAttr.GetActID() != -1)//��Ϊ��ʼֵ,��ֵ��ʶû����������
					ptagLevel->vecStepBegin.push_back(IntoValue);
				//������Ҫѭ������
				IntoValue.dwID = INTONATE_MAINACT;
				IntoValue.strKeyWord = "IntonateMainAct";
				IntoValue.dwValue = ptInStp->GetMainActID();
				if(ptInStp->GetMainActID() != -1)//��Ϊ��ʼֵ����ֵ��ʶû����Ҫѭ������
				{
					ptagLevel->vecStepBegin.push_back(IntoValue);
				}
				//������Ч
				for (uint i = 0 ; i < ptInStp->mVecEffect.size() ; ++i)
				{
					IntoValue.dwID = INTONATE_EFFECT;
					IntoValue.strKeyWord = "IntonateEffect";
					IntoValue.dwValue = ptInStp->mVecEffect[i].GetID();
					ptagLevel->vecStepBegin.push_back(IntoValue);

					IntoValue.dwID = INTONATE_EFFECT_INDEX;
					IntoValue.strKeyWord = "IntonateEffectIndex";
					IntoValue.dwValue = ptInStp->mVecEffect[i].GetIndex();
					ptagLevel->vecStepBegin.push_back(IntoValue);

					IntoValue.dwID = INTONATE_EFFECT_POSITION;
					IntoValue.strKeyWord = "IntonateEffectPosition";	
					IntoValue.dwValue = ptInStp->mVecEffect[i].GetBindPos();
					ptagLevel->vecStepBegin.push_back(IntoValue);

					IntoValue.dwID = INTONATE_EFFECT_CYCLE;
					IntoValue.strKeyWord = "IntonateEffectCycle";
					IntoValue.dwValue = ptInStp->mVecEffect[i].IsCycle();
					ptagLevel->vecStepBegin.push_back(IntoValue);

					IntoValue.dwID = INTONATE_PLAY;
					IntoValue.strKeyWord = "IntonatePlay";
					IntoValue.dwValue = ptInStp->mVecEffect[i].IsPlay();
					ptagLevel->vecStepBegin.push_back(IntoValue);
				}
				//������Ч
				IntoValue.dwID = INTONATE_SOUND;
				IntoValue.strKeyWord = "IntonateSound";
				IntoValue.dwID = ptInStp->mBaseAttr.mSoundAttr.GetSoundID();
				ptagLevel->vecStepBegin.push_back(IntoValue);
				//������Ч�Ƿ�ѭ��
				IntoValue.dwID = INTONATE_SOUND_CYCLE;
				IntoValue.strKeyWord = "IntonateSoundCycle";
				IntoValue.dwValue =  ptInStp->mBaseAttr.mSoundAttr.IsSoundCycle();
				ptagLevel->vecStepBegin.push_back(IntoValue);
				//���ܿ�ʼʱ�Ƿ񲥷���Ч
				IntoValue.dwID = INTONATE_SOUND_PLAY;
				IntoValue.strKeyWord = "IntonateSoundPlay";
				IntoValue.dwValue =  ptInStp->mBaseAttr.mSoundAttr.IsSoundPlay();
				ptagLevel->vecStepBegin.push_back(IntoValue);
				//���������Ƿ���Ա����
				IntoValue.dwID = INTONATE_BREAK;
				IntoValue.strKeyWord = "IntonateBreak";
				IntoValue.dwID = ptInStp->IsBreak();
				ptagLevel->vecStepBegin.push_back(IntoValue);
			}
			///�ͷŽ׶�
			///�����
			ptagLevel->vecStepRun.clear();
			tagReleaseStp* ptRelStp = &(ptLevelStep->mRelStp);
			tagLevelValue relValue;
			if(ptLevelStep->IsHasRelStp())
			{

				//�ͷŶ���ֵ
				relValue.dwID = RELEASE_ACT;
				relValue.strKeyWord = "ReleaseAct";
				relValue.dwValue = ptRelStp->mBaseAttr.GetActID();
				if(relValue.dwValue != -1)//��ֵ��ʶû�ж���ֵ
					ptagLevel->vecStepRun.push_back(relValue);
				//�ͷ���Ч
				for (uint i = 0 ; i < ptRelStp->mVecEffect.size() ; ++i)
				{
					relValue.dwID = RELEASE_EFFECT;
					relValue.strKeyWord = "ReleaseEffect";
					relValue.dwValue = ptRelStp->mVecEffect[i].mEffect.GetID();
					ptagLevel->vecStepRun.push_back(relValue);

					relValue.dwID = RELEASE_EFFECT_INDEX;
					relValue.strKeyWord = "ReleaseEffectIndex";
					relValue.dwValue = ptRelStp->mVecEffect[i].mEffect.GetIndex();
					ptagLevel->vecStepRun.push_back(relValue);

					relValue.dwID = RELEASE_EFFECT_DEST;
					relValue.strKeyWord = "ReleaseEffectDest";
					relValue.strValue = ptRelStp->mVecEffect[i].GetDest();
					ptagLevel->vecStepRun.push_back(relValue);

					relValue.dwID = RELEASE_EFFECT_POSITION;
					relValue.strKeyWord = "ReleaseEffectPosition";
					relValue.dwValue = ptRelStp->mVecEffect[i].mEffect.GetBindPos() ;
					ptagLevel->vecStepRun.push_back(relValue);

					relValue.dwID = RELEASE_EFFECT_CYCLE;
					relValue.strKeyWord = "ReleaseEffectCycle";
					relValue.dwValue = ptRelStp->mVecEffect[i].mEffect.IsCycle();
					ptagLevel->vecStepRun.push_back(relValue);

					relValue.dwID = RELEASE_PLAY;
					relValue.strKeyWord = "ReleasePlay";
					relValue.dwValue = ptRelStp->mVecEffect[i].mEffect.IsPlay();
					ptagLevel->vecStepRun.push_back(relValue);
				}
				//��Ч����
				relValue.dwID = RELEASE_SOUND;
				relValue.strKeyWord = "ReleaseSound";
				relValue.dwValue = ptRelStp->mBaseAttr.mSoundAttr.GetSoundID();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = RELEASE_SOUND_CYCLE;
				relValue.strKeyWord = "ReleaseSoundCycle";
				relValue.dwValue = ptRelStp->mBaseAttr.mSoundAttr.IsSoundCycle();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = RELEASE_SOUND_DEST;
				relValue.strKeyWord = "ReleaseSoundDest";
				relValue.dwValue = ptRelStp->GetSoundDest();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = RELEASE_SOUND_PLAY;
				relValue.strKeyWord = "ReleaseSoundPlay";
				relValue.dwValue = ptRelStp->mBaseAttr.mSoundAttr.IsSoundPlay();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = ACTTIME;
				relValue.strKeyWord = "ActTime";
				relValue.dwValue = ptRelStp->GetActTime();
				ptagLevel->vecStepRun.push_back(relValue);

			}
			//���н׶�
			if(ptRelStp->IsHasFly()) 
			{
				relValue.dwID = FLY_EFFECT;
				relValue.strKeyWord = "FlyEffect";
				relValue.dwValue = ptRelStp->mFlyEffect.mBaseAttr.GetID();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_EFFECT_INDEX;
				relValue.strKeyWord = "FlyEffectIndex";
				relValue.dwValue = ptRelStp->mFlyEffect.mBaseAttr.GetIndex();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_EFFECT_POSITION;
				relValue.strKeyWord = "FlyEffectPosition";
				relValue.dwValue = ptRelStp->mFlyEffect.mBaseAttr.GetBindPos();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_EFFECT_CYCLE;
				relValue.strKeyWord = "FlyEffectCycle";
				relValue.dwValue = ptRelStp->mFlyEffect.mBaseAttr.IsCycle();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_EFFECT_TURNX;
				relValue.strKeyWord = "FlyEffectTurnx";
				relValue.dwValue = ptRelStp->mFlyEffect.GetTurnX();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_EFFECT_TURNY;
				relValue.strKeyWord = "FlyEffectTurny";
				relValue.dwValue = ptRelStp->mFlyEffect.GetTurnY();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_TIME;
				relValue.strKeyWord = "FlySpeed";
				relValue.dwValue = ptRelStp->mFlyEffect.GetSpeed();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_PLAY;
				relValue.strKeyWord = "FlyPlay";
				relValue.dwValue = ptRelStp->mFlyEffect.mBaseAttr.IsPlay();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_SOUND;
				relValue.strKeyWord = "FlySound";
				relValue.dwValue = ptRelStp->mFlyEffect.mSoundAttr.GetSoundID();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_SOUND_POSITION;
				relValue.strKeyWord = "FlySoundPosition";
				relValue.dwValue = ptRelStp->mFlyEffect.GetSoundPos();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_SOUND_CYCLE;
				relValue.strKeyWord = "FlySoundCycle";
				relValue.dwValue = ptRelStp->mFlyEffect.mSoundAttr.IsSoundCycle();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_SOUND_PLAY;
				relValue.strKeyWord = "FlySoundPlay";
				relValue.dwValue = ptRelStp->mFlyEffect.mSoundAttr.IsSoundPlay();
				ptagLevel->vecStepRun.push_back(relValue);

				relValue.dwID = FLY_DELAY;
				relValue.strKeyWord = "FlyDelay";
				relValue.dwValue = ptRelStp->mFlyEffect.GetDelayTime();
				ptagLevel->vecStepRun.push_back(relValue);

			}	
			//�����׶�
			tagEndStp* pEndStp = &(ptLevelStep->mEndStp);
			if(ptLevelStep->IsHasEndStp())
			{
				relValue.dwID = HITEFFECT;
				relValue.strKeyWord = "HitEffect";
				relValue.dwValue = pEndStp->GetHitEffID();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = HITSOUND;
				relValue.strKeyWord = "HitSound";
				relValue.dwValue = pEndStp->GetHitSoundID();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = FLASHEFFECT;
				relValue.strKeyWord = "FlashEffect";
				relValue.dwValue = pEndStp->IsFlashEffect();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = FLASHEEFFECTLUM;
				relValue.strKeyWord = "FlashEffectLum";
				relValue.dwValue = pEndStp->GetFlashEffLum();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = FLASHEEFFECTTIME;
				relValue.strKeyWord = "FlashEffectTime";
				relValue.dwValue = pEndStp->GetFlashEffTime();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = SHAKEEFFECT;
				relValue.strKeyWord = "ShakeEffect";
				relValue.dwValue = pEndStp->IsShakeEffect();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = SHAKEEFFECTEXTENT;
				relValue.strKeyWord = "ShakeEffectExtent";
				relValue.dwValue = pEndStp->GetShakeEffExtent();
				ptagLevel->vecStepEnd.push_back(relValue);

				relValue.dwID = SHAKEEFFECTTIME;
				relValue.strKeyWord = "ShakeEffectTime";
				relValue.dwValue = pEndStp->GetShakeEffTime();
				ptagLevel->vecStepEnd.push_back(relValue);
			}
			//���ս׶�
			tagSummonStp* pSumStp = &(ptLevelStep->mSummonStp);
			if(ptLevelStep->IsHasSummonStp())
			{
				relValue.dwID = SUMMON_EFFECT;
				relValue.strKeyWord = "SummonEffect";
				relValue.dwValue = pSumStp->GetID();
				ptagLevel->vecStepSummon.push_back(relValue);

				relValue.dwID = SUMMON_SOUND;
				relValue.strKeyWord = "SummonSound";
				relValue.dwValue = pSumStp->GetSoundID();
				ptagLevel->vecStepSummon.push_back(relValue);

				relValue.dwID = SUMMON_LIFETIME;
				relValue.strKeyWord = "SummonLifeTime";
				relValue.dwValue = pSumStp->GetLifeTime();
				ptagLevel->vecStepSummon.push_back(relValue);

				relValue.dwID = RANDOM_SUMMONX;
				relValue.strKeyWord = "RandomSummonX";
				relValue.dwValue = pSumStp->GetRangeX();
				ptagLevel->vecStepSummon.push_back(relValue);

				relValue.dwID = RANDOM_SUMMONY;
				relValue.strKeyWord = "RandomSummonY";
				relValue.dwValue = pSumStp->GetRangeY();
				ptagLevel->vecStepSummon.push_back(relValue);

				relValue.dwID = RANDOM_SUMMON_NUM;
				relValue.strKeyWord = "RandomSummonNum";
				relValue.dwValue = pSumStp->GetRandomNum();
				ptagLevel->vecStepSummon.push_back(relValue);

				relValue.dwID = SUMMON_DELAYTIMA;
				relValue.strKeyWord = "RandomSummonDelayTime";
				relValue.dwValue = pSumStp->GetRandomDelayTime();
				ptagLevel->vecStepSummon.push_back(relValue);
			}

			if( isnewtaglevel)
			{
				pSkill->vectorLevels.push_back(*ptagLevel);
			}
		}
		if(isNewSkill)
		{
			m_mapSkillListXml[pSkill->dwSkillID] = *pSkill;
		}
	}
}

CSkillListXml::tagLevel* CSkillListXml::tagSkill::GettagLevel(uint level)
{
	for(uint i = 0; i < vectorLevels.size(); ++i)
	{
		if( vectorLevels[i].dwSkill_Level == level)
		{
			return &vectorLevels[i];
		}
	}
	return NULL;
}

void CSkillListXml::tagLevel::UpdateValue(const char* strKey,long value)
{
	for(size_t i = 0 ;i < vecStepSend.size() ; ++i)
	{
		if(strcmp(strKey,vecStepSend[i].strKeyWord.c_str()) == 0)
		{
			vecStepSend[i].dwValue = value;
		}
	}
	for(size_t i = 0 ;i < vecStepBegin.size() ; ++i)
	{
		if(strcmp(strKey,vecStepBegin[i].strKeyWord.c_str()) == 0)
		{
			vecStepBegin[i].dwValue = value;
		}
	}
	for(size_t i = 0 ;i < vecStepRun.size() ; ++i)
	{
		if(strcmp(strKey,vecStepRun[i].strKeyWord.c_str()) == 0)
		{
			vecStepRun[i].dwValue = value;
		}
	}
	for(size_t i = 0 ;i < vecStepEnd.size() ; ++i)
	{
		if(strcmp(strKey,vecStepEnd[i].strKeyWord.c_str()) == 0)
		{
			vecStepEnd[i].dwValue = value;
		}
	}
	for(size_t i = 0 ;i < vecStepSummon.size() ; ++i)
	{
		if(strcmp(strKey,vecStepSummon[i].strKeyWord.c_str()) == 0)
		{
			vecStepSummon[i].dwValue = value;
		}
	}

}

void UpdateSkillXmlList(uint skillID,uint skilllevel,std::vector<string>& vecStr,std::vector<long>& vecValue)
{
	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(skillID);
	CSkillListXml::tagLevel *pLevel = NULL;
	if(pSkill)
		pLevel = pSkill->GettagLevel(skilllevel);
	if(pLevel)
	{
		for(size_t sz = 0 ; sz < vecStr.size() ; ++sz)
		{
			string strKey = vecStr[sz];
			long value = vecValue[sz];
			pLevel->UpdateValue(strKey.c_str(),value);
		}
	}
}