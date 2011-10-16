
#include "stdafx.h"
#include "SpriteSystem.h"
#include "goods/CGoodsFactory.h"
#include "skills/SkillFactory.h"


SpriteSystem::SpriteSystem(void)
{

}

SpriteSystem::~SpriteSystem(void)
{

}

map<LONG, DWORD> SpriteSystem::c_mapCanUseGoodsIndex;
vector<LONG> SpriteSystem::c_vectorCanUseFightSkill;
vector<LONG> SpriteSystem::c_vectorCanUseAssistSkill;
vector<LONG> SpriteSystem::c_vectorCanUseAssistGoods;

//! ��ȡ����
BOOL SpriteSystem::LoadSetup(void)
{
	BOOL bReturnValue = TRUE;
	//! ----------------------------���ڵ�----------------------------
	TiXmlNode *pNode = NULL; 		
	
	CRFile* prFile = rfOpen("data/Sprite.xml");
	if (prFile == NULL)
	{
		return FALSE;
	}
	TiXmlDocument* m_Tdoc = new TiXmlDocument;

	if(!m_Tdoc->LoadData(prFile->GetData(),prFile->GetDatalen()))
		return FALSE;

	rfClose(prFile);
	pNode=m_Tdoc->FirstChild("Sprite");
	if (pNode==NULL) 
		return FALSE;

	TiXmlElement* pSprite = pNode->ToElement();//��ȡnode ��ָ��
	if (pSprite==NULL) 
		return FALSE;

	//! ----------------------------��ȡ����----------------------------

	try
	{
		//! �Զ�������
		TiXmlElement* pAutoSupplyGoods = pSprite->FirstChildElement("AutoSupplyGoods");

		TiXmlElement* pGoods = pAutoSupplyGoods->FirstChildElement("Goods");
		for (; pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
		{
			const char *pGoodsName = pGoods->Attribute("ItemName");
			LONG lGoodsBaseIndex = CGoodsFactory::QueryGoodsIDByOriginalName(pGoodsName);
			if (0 == lGoodsBaseIndex)
			{
				AddErrorLogText("��ȡ�Զ���������ʧ�ܣ�û���ҵ�ԭʼ��Ϊ[%s]����Ʒ������", pGoodsName);
				bReturnValue = FALSE;
				continue;
			}
			if (c_mapCanUseGoodsIndex.end() != c_mapCanUseGoodsIndex.find(lGoodsBaseIndex))
			{
				AddErrorLogText("��ȡ�Զ���������ʧ�ܣ��ظ�����ԭʼ��Ϊ[%s]����Ʒ������", pGoodsName);
				bReturnValue = FALSE;
				continue;
			}

			//! ��ƷӦ�÷�Χ
			DWORD dwBound = 0;
			const char *pBound = pGoods->Attribute("Bound");
			if (NULL == pBound)
			{
				AddErrorLogText("��ȡ�Զ���������ʧ�ܣ�ԭʼ��Ϊ[%s]����ƷӦ�÷�Χ���ô��󣡣���", pGoodsName);
				bReturnValue = FALSE;
				continue;
			}
			LONG lBound = atoi(pBound);
			lBound = lBound % 10000;
			if(0 != lBound / 1000)	dwBound |= eGAB_POWER;
			lBound = lBound % 1000;
			if(0 != lBound / 100)	dwBound |= eGAB_SMP;
			lBound = lBound % 100;
			if(0 != lBound / 10)	dwBound |= eGAB_MP;
			lBound = lBound % 10;
			if(0 != lBound)			dwBound |= eGAB_HP;
			if(0 == dwBound)
			{
				AddErrorLogText("��ȡ�Զ���������ʧ�ܣ�ԭʼ��Ϊ[%s]����ƷӦ�÷�Χ���ô��󣡣���", pGoodsName);
				bReturnValue = FALSE;
				continue;
			}
			
			c_mapCanUseGoodsIndex[lGoodsBaseIndex] = dwBound;
		}
        //����ս������
        TiXmlElement* pAutoFightSkills = pSprite->FirstChildElement("AutoFightSkills");
        TiXmlElement* pFightSkills = pAutoFightSkills->FirstChildElement("Skills");
        for (; pFightSkills != NULL; pFightSkills = pFightSkills->NextSiblingElement("Skills"))
        {
            const char* pSkillID = pFightSkills->Attribute("ID");
            if (pSkillID == NULL)
            {
                AddErrorLogText("��ȡ�Զ�ս����������ʧ�ܣ�����IDΪ[%s]���ô��󣡣���", pSkillID);
                bReturnValue = FALSE;
                continue;
            }
            LONG lID = atoi(pSkillID);
            if (FindExist(c_vectorCanUseFightSkill, lID))
            {
                AddErrorLogText("��ȡ�Զ�ʹ��ս����������ʧ�ܣ��ظ����ü���IDΪ[%s]������", pSkillID);
                bReturnValue = FALSE;
                continue;
            }
            c_vectorCanUseFightSkill.push_back(lID);
        }

        //���ظ�������
        TiXmlElement* pAutoAssistSkills = pSprite->FirstChildElement("AutoAssistSkills");
        TiXmlElement* pAssistSkills = pAutoAssistSkills->FirstChildElement("Skills");
        for (; pAssistSkills != NULL; pAssistSkills = pAssistSkills->NextSiblingElement("Skills"))
        {
            const char* pSkillID = pAssistSkills->Attribute("ID");
            if (pSkillID == NULL)
            {
                AddErrorLogText("��ȡ�Զ�ʹ�ø�����������ʧ�ܣ�����IDΪ[%s]���ô��󣡣���", pSkillID);
                bReturnValue = FALSE;
                continue;
            }
            LONG lAssistID = atoi(pSkillID);
            if (FindExist(c_vectorCanUseAssistSkill, lAssistID))
            {
                AddErrorLogText("��ȡ�Զ�ʹ�ø�����������ʧ�ܣ��ظ����ü���IDΪ[%s]������", pSkillID);
                bReturnValue = FALSE;
                continue;
            }
            c_vectorCanUseAssistSkill.push_back(lAssistID);
        }
        //���ظ�������
        TiXmlElement* pAutoAssistGoods = pSprite->FirstChildElement("AutoAssistGoods");
        TiXmlElement* pAssistGoods = pAutoAssistGoods->FirstChildElement("Goods");
        for (; pAssistGoods != NULL; pAssistGoods = pAssistGoods->NextSiblingElement("Goods"))
        {
            const char* pGoodsName = pAssistGoods->Attribute("ItemName");
            LONG lGoodsBaseIndex = CGoodsFactory::QueryGoodsIDByOriginalName(pGoodsName);
            if (lGoodsBaseIndex == 0)
            {
                AddErrorLogText("��ȡ�Զ�ʹ�ø�����������ʧ�ܣ�û���ҵ�ԭʼ��Ϊ[%s]����Ʒ���ߣ�����", pGoodsName);
                bReturnValue = FALSE;
                continue;
            }
            if (FindExist(c_vectorCanUseAssistGoods, lGoodsBaseIndex))
            {
                AddErrorLogText("��ȡ�Զ�ʹ�ø�����������ʧ�ܣ��ظ�����ԭʼ��Ϊ[%s]����Ʒ������", pGoodsName);
                bReturnValue = FALSE;
                continue;
            }
            c_vectorCanUseAssistGoods.push_back(lGoodsBaseIndex);
        }
	}
	catch (...)
	{
		AddErrorLogText("�ڶ�ȡС�������õ�ʱ�����˴��󣬶�ȡʧ�ܣ����ܵ�ԭ�������ò�����");
		if (NULL != m_Tdoc)
		{
			delete m_Tdoc;
			m_Tdoc = NULL;
		}
		return FALSE;
	}


	//! ----------------------------�������----------------------------
	delete m_Tdoc;
	m_Tdoc = NULL;
	return bReturnValue;
}

//! ���ñ���
void SpriteSystem::SetupAddToByteArray(vector<BYTE> &ByteArray)
{
	_AddToByteArray(&ByteArray, (LONG)c_mapCanUseGoodsIndex.size());
	for (map<LONG, DWORD>::iterator ite = c_mapCanUseGoodsIndex.begin(); ite != c_mapCanUseGoodsIndex.end(); ++ite)
	{
		_AddToByteArray(&ByteArray, ite->first);
		_AddToByteArray(&ByteArray, ite->second);
	}
    //ս������
    _AddToByteArray(&ByteArray, (LONG)c_vectorCanUseFightSkill.size());
    for (vector<LONG>::iterator ite = c_vectorCanUseFightSkill.begin(); ite != c_vectorCanUseFightSkill.end(); ++ite)
    {
        _AddToByteArray(&ByteArray, *ite);
    }
    //��������
    _AddToByteArray(&ByteArray, (LONG)c_vectorCanUseAssistSkill.size());
    for (vector<LONG>::iterator ite = c_vectorCanUseAssistSkill.begin(); ite != c_vectorCanUseAssistSkill.end(); ++ite)
    {
        _AddToByteArray(&ByteArray, *ite);
    }
    //��������
    _AddToByteArray(&ByteArray, (LONG)c_vectorCanUseAssistGoods.size());
    for (vector<LONG>::iterator ite = c_vectorCanUseAssistGoods.begin(); ite != c_vectorCanUseAssistGoods.end(); ++ite)
    {
        _AddToByteArray(&ByteArray, *ite);
    }
}

//! ����
void SpriteSystem::AddToByteArray(vector<BYTE> &ByteArray)
{
	m_AutoSupplyData.AddToByteArray(ByteArray);
}

//! ����
void SpriteSystem::GetFormByteArray(BYTE *pByteArray, long &lPos)
{
	m_AutoSupplyData.GetFormByteArray(pByteArray, lPos);
}

//!	����
void SpriteSystem::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	m_AutoSupplyData.CodeToDataBlock(setWriteDB);
    //�����ȶ��ȶ�ʱ��
    //setWriteDB.AddToByteArray(m_AutoFightData.GetStableTime());
    m_AutoFightData.CodeToDataBlock(setWriteDB);
}
//!	����
void SpriteSystem::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_AutoSupplyData.DecodeFromDataBlock(setReadDB);
    //�����ȶ�ʱ��
    //m_AutoFightData.SetStableTime(setReadDB.GetLongFromByteArray());
    m_AutoFightData.DecodeFromDataBlock(setReadDB);
}

//! �����ȶ��һ�ʱ��
void SpriteSystem::SetStableTime(DWORD dwTime)
{
    m_AutoFightData.SetStableTime(dwTime);
}

DWORD SpriteSystem::GetStableTime(void)
{
    return m_AutoFightData.GetStableTime();
}

bool SpriteSystem::FindExist(vector<LONG> &refVector, LONG lValue)
{
    if (lValue)
    {
        vector<LONG>::iterator ite = refVector.begin();
        for (; ite != refVector.end(); ++ite)
        {
            if ((*ite) == lValue)
            {
                return true;
            }
        }
    }
    return false;
}