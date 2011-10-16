
#include "StdAfx.h"
#include "SpriteSystem.h"

#include "../../../Public/Setup/GoodsSetup.h"

SpriteSystem::SpriteSystem(void)
{

}

SpriteSystem::~SpriteSystem(void)
{

}

map<LONG, DWORD> SpriteSystem::c_mapCanUseGoodsIndex;

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
        if (pAutoSupplyGoods)
        {
            TiXmlElement* pGoods = pAutoSupplyGoods->FirstChildElement("Goods");
            for (; pGoods!=NULL; pGoods = pGoods->NextSiblingElement())
            {
                const char *pGoodsName = pGoods->Attribute("ItemName");
                LONG lGoodsBaseIndex = GoodsSetup::QueryGoodsIDByOriginalName(pGoodsName);
                if (0 == lGoodsBaseIndex)
                {
                    Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SPRITE_40", pGoodsName));
                    bReturnValue = FALSE;
                    continue;
                }
                if (c_mapCanUseGoodsIndex.end() != c_mapCanUseGoodsIndex.find(lGoodsBaseIndex))
                {
                    Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SPRITE_40", pGoodsName));
                    bReturnValue = FALSE;
                    continue;
                }

                //! ��ƷӦ�÷�Χ
                DWORD dwBound = 0;
                const char *pBound = pGoods->Attribute("Bound");
                if (NULL == pBound)
                {
                    Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SPRITE_41", pGoodsName));
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
                    Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SPRITE_41", pGoodsName));
                    bReturnValue = FALSE;
                    continue;
                }

                c_mapCanUseGoodsIndex[lGoodsBaseIndex] = dwBound;
            }
        }
	}
	catch (...)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SPRITE_42"));
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
}
//!	����
void SpriteSystem::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_AutoSupplyData.DecodeFromDataBlock(setReadDB);
}
